
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

#ifdef __cplusplus
}
#endif

#endif /* RECORD_H */

