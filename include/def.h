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


enum
{
    COMMENT_CLASS_TRANSLATOR = 0,
    COMMENT_CLASS_INTEL_COPYRIGHT = 1,
    COMMENT_CLASS_INTEL_RESERVED_RANGE_START = 2,
    COMMENT_CLASS_INTEL_RESERVED_RANGE_END = 0x9b,
    COMMENT_CLASS_LIBRARY_SPECIFIER_OBSOLETE = 0x81,
    COMMENT_CLASS_MSDOS_VERSION_OBSOLETE = 0x9c,
    COMMENT_CLASS_MEMORY_MODEL = 0x9d,
    COMMENT_CLASS_DOSSEG = 0x9e,
    COMMENT_CLASS_DEFAULT_LIB_SEARCH_NAME = 0x9f,
    COMMENT_CLASS_OMF_EXTS = 0xa0,
    COMMENT_CLASS_NEW_OMF_EXT = 0xa1,
    COMMENT_CLASS_LINK_PASS_SEPARATOR = 0xa2,
    COMMENT_CLASS_LISMOD = 0xa3,
    COMMENT_CLASS_EXESTR = 0xa4,
    COMMENT_CLASS_INCERR = 0xa6,
    COMMENT_CLASS_NOPAD = 0xa7,
    COMMENT_CLASS_WKEXT = 0xa8,
    COMMENT_CLASS_LZEXT = 0xa9,
    COMMENT_CLASS_COMMENT = 0xda,
    COMMENT_CLASS_COMPILER = 0xdb,
    COMMENT_CLASS_DATE = 0xdc,
    COMMENT_CLASS_TIMESTAMP = 0xdd,
    COMMENT_CLASS_USER = 0xdf,
    COMMENT_CLASS_DEPENDENCY_FILE = 0xe9,
    COMMENT_CLASS_COMMAND_LINE = 0xff,
    
    
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

