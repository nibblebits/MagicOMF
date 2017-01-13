#include <stdlib.h>
#include <string.h>
#include "MagicOMF.h"
#include "translate.h"
#include "generator.h"
#include "IO.h"
#include "error.h"
#include "builder.h"

struct MagicOMFHandle* MagicOMFTranslate(char* buf, uint32 size, bool skip_unimplemented_records)
{
    struct MagicOMFHandle* handle = MagicOMFCreateHandle();
    MagicOMFSetupBuffer(handle, size, buf);
    handle->skip_unimplemented_records = skip_unimplemented_records;

    char* end = buf + size;

    /* OMF Files always expect either a THEADR or an LHEADR to begin with. 
     so we need to peak ahead and check its one of these*/
    uint8 type = ReadUnsignedByteNoNext(buf);
    if (type == THEADR_ID)
    {
        TranslatorReadTHEADR(handle);
    }
    else if (type == LHEADR_ID)
    {
        TranslatorReadLHEADR(handle);
    }
    else
    {
        error(EXPECTING_THEADR_OR_LHEADR, handle);
    }


    while (handle->next < end
            && !handle->has_error)
    {
        type = ReadUnsignedByteNoNext(handle->next);
        handle->record_type = type;
        switch (type)
        {
        case THEADR_ID:
            TranslatorReadTHEADR(handle);
            break;
        case LHEADR_ID:
            TranslatorReadLHEADR(handle);
            break;
        case COMENT_ID:
            TranslatorReadCOMENT(handle);
            break;
        case LNAMES_ID:
            TranslatorReadLNAMES(handle);
            break;
        case SEGDEF_ID:
            TranslatorReadSEGDEF(handle);
            break;
        case PUBDEF_16_ID:
            TranslatorReadPUBDEF16(handle);
            break;
        case LEDATA_16_ID:
            TranslatorReadLEDATA16(handle);
            break;
        case FIXUPP_16_ID:
            TranslatorReadFIXUPP16(handle);
            break;
        case MODEND_16_ID:
            TranslatorReadMODEND16(handle);
            break;
        case EXTDEF_ID:
            TranslatorReadEXTDEF(handle);
            break;
        default:
            if (handle->skip_unimplemented_records)
            {
                TranslatorSkipRecord(handle);
            }
            else
            {
                error(INVALID_RECORD_TYPE, handle);
            }
            break;
        }
    }


    // Finalize some things.
    TranslatorFinalize(handle);

    return handle;
}

struct MagicOMFHandle* MagicOMFCreateHandle()
{
    // Setup a basic handle
    struct MagicOMFHandle* handle = (struct MagicOMFHandle*) malloc(sizeof (struct MagicOMFHandle));
    handle->buf = NULL;
    handle->next = NULL;
    handle->root = NULL;
    handle->last = NULL;
    handle->skip_unimplemented_records = false;
    handle->has_error = false;
    handle->last_error_code = -1;

    return handle;
}

void MagicOMFSetupBuffer(struct MagicOMFHandle* handle, uint32 size, char* buf)
{
    handle->buf_size = size;
    handle->buf = buf;
    handle->next = buf;
}

void MagicOMFAddRecord(struct MagicOMFHandle* handle, struct RECORD* record)
{
    // No root set so lets set it
    if (handle->root == NULL)
    {
        handle->root = record;
    }
    else
    {
        // Ok we already have a root so we know we have a last, lets append the last record's next pointer to point to us.
        handle->last->next = record;
    }
    handle->last = record;
}

void MagicOMFAddTHEADR(struct MagicOMFHandle* handle, const char* name)
{
    struct RECORD* record;
    struct THEADR* theadr = BuildTHEADR((char*) name);
    uint16 record_len = theadr->string_length + 2; // +2 for string length and for checksum
    record = BuildRecord(handle, THEADR_ID, record_len, 0);
    record->contents = theadr;
    MagicOMFAddRecord(handle, record);
}

void MagicOMFAddCOMENT(struct MagicOMFHandle* handle, COMMENT_TYPE type, uint8 _class, const char* str)
{
    struct RECORD* record;
    struct COMENT* coment = BuildCOMENT(type, _class, (char*) str);
    uint16 record_len = strlen(str) + 3; // +3 for comment type, comment class and checksum
    record = BuildRecord(handle, COMENT_ID, record_len, 0);
    record->contents = coment;
    MagicOMFAddRecord(handle, record);
}

int MagicOMFCalculateBufferSize(struct MagicOMFHandle* handle)
{
    /* We need to calculate the buffer size for all given records. 
     * the algorithm for this is (record size field + 3) */

    int size = 0;
    struct RECORD* current = handle->root;
    while (current != NULL)
    {
        size += current->length + 3;
        current = current->next;
    }

    return size;
}

void MagicOMFGenerateBuffer(struct MagicOMFHandle* handle)
{
    if (handle->buf != NULL)
    {
        // Buffer is currently present so lets free the memory as we will soon do another malloc
        free(handle->buf);
    }
    
    uint32 buf_size = MagicOMFCalculateBufferSize(handle);
    // Allocate the new memory for the buffer    
    char* buf = (char*) malloc(buf_size);

    // Setup the new buffer
    MagicOMFSetupBuffer(handle, buf_size, buf);

    // Generate the records into the buffer.
    struct RECORD* current = handle->root;
    while (current != NULL)
    {
        switch (current->type)
        {
        case THEADR_ID:
            GeneratorWriteTHEADR(&handle->next, current);
            break;
        case COMENT_ID:
            GeneratorWriteCOMENT(&handle->next, current);
            break;
        default:
            error(INVALID_RECORD_TYPE, handle);
        }
        current = current->next;
    }

}

void MagicOMFCloseHandle(struct MagicOMFHandle* handle)
{
    // TO BE IMPLEMENTED
    struct RECORD* current = handle->root;
    while (current != NULL)
    {

        current = current->next;
    }
}

char* MagicOMFGetLNAMESNameByIndex(struct MagicOMFHandle* handle, uint8 index)
{
    struct RECORD* record = handle->root;
    int c_index = 1;
    while (record != NULL)
    {
        if (record->type == LNAMES_ID)
        {
            struct LNAMES* lnames_record = (struct LNAMES*) (record->contents);
            while (lnames_record != NULL)
            {
                if (c_index == index)
                {
                    return lnames_record->n_string;
                }
                c_index++;
                lnames_record = lnames_record->next;
            }
        }
        record = record->next;
    }

    return NULL;
}

struct SEGDEF* MagicOMFGetSEGDEFByIndex(struct MagicOMFHandle* handle, uint8 index)
{
    struct RECORD* record = handle->root;
    int c_index = 1;
    while (record != NULL)
    {
        if (record->type == SEGDEF_ID)
        {
            if (c_index == index)
            {
                return record->contents;
            }
            c_index++;
        }
        record = record->next;
    }

    return NULL;
}

struct LEDATA_16* MagicOMFGetLEDATABySegmentIndex(struct MagicOMFHandle* handle, uint8 index)
{
    struct RECORD* record = handle->root;
    while (record != NULL)
    {
        if (record->type == LEDATA_16_ID)
        {
            struct LEDATA_16* ledata_contents = (struct LEDATA_16*) (record->contents);
            if (ledata_contents->seg_index == index)
            {
                return ledata_contents;
            }
        }
        record = record->next;
    }

    return NULL;
}

struct EXTDEF* MagicOMFGetEXTDEFByIndex(struct MagicOMFHandle* handle, uint8 index)
{
    struct RECORD* record = handle->root;
    int c_index = 1;
    while (record != NULL)
    {
        if (record->type == EXTDEF_ID)
        {
            struct EXTDEF* extdef_record = (struct EXTDEF*) (record->contents);
            while (extdef_record != NULL)
            {
                if (c_index == index)
                {
                    return extdef_record;
                }
                c_index++;
                extdef_record = extdef_record->next;
            }
        }
        record = record->next;
    }

    return NULL;
}

const char* MagicOMFErrorMessage(MAGIC_OMF_ERROR_CODE error_id)
{
    return GetErrorMessage(error_id);
}