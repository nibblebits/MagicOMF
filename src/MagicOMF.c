#include <stdlib.h>
#include "MagicOMF.h"
#include "translate.h"
#include "IO.h"
#include "error.h"

struct MagicOMFHandle* MagicOMFTranslate(char* buf, uint32 size, bool skip_unimplemented_records)
{
    char* end = buf + size;
    struct MagicOMFHandle* handle = (struct MagicOMFHandle*) malloc(sizeof (struct MagicOMFHandle));
    handle->buf = buf;
    handle->next = buf;
    handle->root = NULL;
    handle->last = NULL;
    handle->skip_unimplemented_records = skip_unimplemented_records;
    handle->has_error = false;
    handle->last_error_code = -1;

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

const char* MagicOMFErrorMessage(MAGIC_OMF_ERROR_CODE error_id)
{
    return GetErrorMessage(error_id);
}