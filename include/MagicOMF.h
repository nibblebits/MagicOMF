
/* 
 * File:   MagicOMF.h
 * Author: dansb
 *
 * Created on 06 December 2016, 11:34
 */

#ifndef MAGICOMF_H
#define MAGICOMF_H

#ifdef __cplusplus
extern "C" {
#endif
#include "def.h"
#include "types.h"
#include "record.h"
    
struct MagicOMFHandle* MagicOMFTranslate(char* buf, uint32 size);
char* MagicOMFGetLNAMESNameByIndex(struct MagicOMFHandle* handle, uint8 index);
const char* MagicOMFErrorMessage(MAGIC_OMF_ERROR_CODE error_id);
#ifdef __cplusplus
}
#endif

#endif /* MAGICOMF_H */

