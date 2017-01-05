#include <stdlib.h>
#include "MagicOMF.h"
#include "translate.h"
#include "IO.h"
#include "error.h"

struct MagicOMFHandle* MagicOMFTranslate(char* buf, uint32 size)
{
    char* end = buf + size;
    struct MagicOMFHandle* handle = (struct MagicOMFHandle*) malloc(sizeof (struct MagicOMFHandle));
    handle->buf = buf;
    handle->next = buf;
    handle->root = NULL;
    handle->last = NULL;
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
            error(INVALID_RECORD_TYPE, handle);
            break;
        }
    }

    return handle;
}

const char* MagicOMFErrorMessage(MAGIC_OMF_ERROR_CODE error_id)
{
    return GetErrorMessage(error_id);
}