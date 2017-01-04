/*
    Magic OMF Library v1.0 - A linker for the OMF(Object Model Format).
    Copyright (C) 2016  Daniel McCarthy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/* 
 * File:   common.h
 * Author: dansb
 *
 * Created on 06 December 2016, 11:24
 */

#ifndef COMMON_H
#define COMMON_H

#define EXPORT __declspec(dllexport)

#include "types.h"
#include "def.h"
#ifdef __cplusplus
extern "C"
{
#endif

enum
{
    INVALID_THEADR_PROVIDED,
    EXPECTING_THEADR_OR_LHEADR,
    INVALID_LHEADR_PROVIDED,
    INVALID_RECORD_TYPE,
    INVALID_COMENT_PROVIDED,
    UNKNOWN_COMENT_CLASS,
    INVALID_LNAMES_PROVIDED,
    INVALID_SEGDEF_PROVIDED,
    IMPROPERLY_FORMATTED_SEGDEF_PROVIDED,
    INVALID_PUBDEF_16_PROVIDED,
    PUBDEF_16_ABSOLUTE_ADDRESSING_NOT_SUPPORTED,
};

void error(MAGIC_OMF_ERROR_CODE error_code, struct MagicOMFHandle* handle);
const char* GetErrorMessage(MAGIC_OMF_ERROR_CODE error_code);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

