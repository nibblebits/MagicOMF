
/* 
 * File:   record.h
 * Author: dansb
 *
 * Created on 06 December 2016, 11:06
 */

#ifndef RECORD_H/*
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

#define RECORD_H

#include "types.h"
#ifdef __cplusplus
extern "C"
{
#endif

struct RECORD
{
    uint8 type;
    uint16 length;
    void* contents;
    uint8 checksum;
    bool has_checksum;
    char* end_of_record;
    struct RECORD* next;
};

struct THEADR
{
    uint8 string_length;
    char* name_string;
};

struct LHEADR
{
    uint8 string_length;
    char* name_string;
};


struct COMENT
{
    uint8 c_type;
    uint8 c_class;
    bool is_link_pass_seperator;
    char* c_string;
    /* The no_purge signifies that this comment is to be preserved by utility programs that manipulate object
modules.  This can protect an important comment, such as a copyright message, from deletion*/
    bool no_purge;
/* The no_list signifies that a comment should not be shown to the utility programs that
 list the object file contents*/
    bool no_list;
};


struct LNAMES
{
    uint8 s_len;
    char* n_string;
    struct LNAMES* next;
};

struct Attributes
{
    uint8 ACBP;
    uint8 A;
    uint8 C;
    uint8 B;
    uint8 P;
};

struct SEGDEF
{
    struct Attributes attributes;
    uint16 seg_len;
    uint8 seg_name_index;
    uint8 class_name_index;
    uint8 overlay_name_index;
};

struct PUBDEF_16_IDEN
{
    uint8 str_len;
    char* name_str;
    uint16 p_offset;
    uint8 type_index;
    struct PUBDEF_16_IDEN* next;
};

struct PUBDEF_16
{
    uint8 bg_index;
    uint8 bs_index;
    struct PUBDEF_16_IDEN* iden;
    
};

#ifdef __cplusplus
}
#endif

#endif /* RECORD_H */

