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
extern "C" {
#endif
    
#include "types.h"

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

