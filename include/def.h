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
 * File:   structs.h
 * Author: Daniel McCarthy
 *
 * Created on 06 December 2016, 15:13
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "types.h"

enum
{
    THEADR_ID = 0x80,
    LHEADR_ID = 0x82,
    COMENT_ID = 0x88,
    LNAMES_ID = 0x96,
    SEGDEF_ID = 0x98
};

enum
{
    SEG_ATTR_ALIGNMENT_ABS_SEG,
    SEG_ATTR_ALIGNMENT_RELOC_BYTE_ALIGNED,
    SEG_ATTR_ALIGNMENT_RELOC_WORD_ALIGNED,
    SEG_ATTR_ALIGNMENT_PARAGRAPH_WORD_ALIGNED,
    SEG_ATTR_ALIGNMENT_RELOC_PAGE_BOUNDARY_ALIGNED,
    SEG_ATTR_ALIGNMENT_DOUBLE_WORD_ALIGNED,
    SEG_ATTR_ALIGNMENT_NOT_SUPPORTED,
    SEG_ATTR_ALIGNMENT_NOT_DEFINED
};

enum
{
    SEG_ATTR_COMBINATION_PRIVATE,
    SEG_ATTR_COMBINATION_RESERVED_1,
    SEG_ATTR_COMBINATION_PUBLIC_2,
    SEG_ATTR_COMBINATION_RESERVED_3,
    SEG_ATTR_COMBINATION_PUBLIC_4,
    SEG_ATTR_COMBINATION_STACK,
    SEG_ATTR_COMBINATION_COMMON,
    SEG_ATTR_COMBINATION_PUBLIC_7,
};

struct MagicOMFHandle
{
    char* buf;
    char* next;
    struct RECORD* root;
    struct RECORD* last;

    MAGIC_OMF_ERROR_CODE last_error_code;
    bool has_error;
};


#ifdef __cplusplus
}
#endif

#endif /* STRUCTS_H */

