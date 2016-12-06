#include <stdlib.h>
#include "MagicOMF.h"
#include "translate.h"
#include "IO.h"
#include "error.h"

struct MagicOMFHandle* MagicOMFTranslate(char* buf)
{
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

    return handle;
}

const char* MagicOMFErrorMessage(MAGIC_OMF_ERROR_CODE error_id)
{
    return GetErrorMessage(error_id);
}