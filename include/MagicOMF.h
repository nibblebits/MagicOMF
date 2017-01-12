
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
    
struct MagicOMFHandle* MagicOMFTranslate(char* buf, uint32 size, bool skip_unimplemented_records);
struct MagicOMFHandle* MagicOMFCreateHandle();
void MagicOMFAddRecord(struct MagicOMFHandle* handle, struct RECORD* record);
void MagicOMFAddTHEADR(struct MagicOMFHandle* handle, char* name);
void MagicOMFCloseHandle(struct MagicOMFHandle* handle);
char* MagicOMFGetLNAMESNameByIndex(struct MagicOMFHandle* handle, uint8 index);
struct SEGDEF* MagicOMFGetSEGDEFByIndex(struct MagicOMFHandle* handle, uint8 index);
struct LEDATA_16* MagicOMFGetLEDATABySegmentIndex(struct MagicOMFHandle* handle, uint8 index);
struct EXTDEF* MagicOMFGetEXTDEFByIndex(struct MagicOMFHandle* handle, uint8 index);
const char* MagicOMFErrorMessage(MAGIC_OMF_ERROR_CODE error_id);


#ifdef __cplusplus
}
#endif

#endif /* MAGICOMF_H */

