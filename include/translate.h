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
 * File:   translate.h
 * Author: Daniel McCarthy
 *
 * Created on 06 December 2016, 12:37
 */

#ifndef TRANSLATE_H
#define TRANSLATE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "error.h"
    
struct RECORD* StartRecord();
void EndRecord(struct RECORD* record, struct MagicOMFHandle* handle);
void TranslatorReadTHEADR(struct MagicOMFHandle* handle);
void TranslatorReadLHEADR(struct MagicOMFHandle* handle);

#ifdef __cplusplus
}
#endif

#endif /* TRANSLATE_H */

