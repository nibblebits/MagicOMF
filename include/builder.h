/*
    Craft compiler v0.1.0 - The standard compiler for the Craft programming language.
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
 * File:   builder.h
 * Author: dansb
 *
 * Created on 11 January 2017, 22:44
 */

#ifndef BUILDER_H
#define BUILDER_H

#include "def.h"
#include "record.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
struct RECORD* BuildRecord(struct MagicOMFHandle* handle, uint8 record_type, uint16 record_length, uint8 checksum);
struct THEADR* BuildTHEADR(char* name);
struct THEADR* BuildTHEADR_DefinedSize(char* name, int size);

#ifdef __cplusplus
}
#endif

#endif /* BUILDER_H */

