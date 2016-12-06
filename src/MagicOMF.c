#include <stdlib.h>
#include "MagicOMF.h"
#include "translate.h"
#include "error.h"

struct MagicOMFHandle* MagicOMFTranslate(char* buf)
{
    struct MagicOMFHandle* handle = (struct MagicOMFHandle*) malloc(sizeof(struct MagicOMFHandle));
    handle->buf = buf;
    handle->next = buf;
    handle->root = NULL;
    handle->last = NULL;
    handle->has_error = false;
    handle->last_error_code = -1;
    // OMF Files always expect a LHEADR to begin with.
    TranslatorReadLHEADR(handle);
    if (handle->has_error)
    {
        // The LHEADR has an error and we are expecting an LHEADR so change the error code
       error(EXPECTING_THEADR, handle);
    }
    
    return handle;
}


const char* MagicOMFErrorMessage(MAGIC_OMF_ERROR_CODE error_id)
{
    return GetErrorMessage(error_id);
}