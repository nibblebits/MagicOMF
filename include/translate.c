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

#include "translate.h"
#include "record.h"
#include "IO.h"
#include <stdlib.h>


/* 
 * File:   translate.c
 * Author: Daniel McCarthy
 *
 * Created on 06 December 2016, 11:52
 */


struct RECORD* StartRecord(struct MagicOMFHandle* handle)
{
    struct RECORD* record = (struct RECORD*) malloc(sizeof(struct RECORD));
    // Read the standard record details
    record->type = ReadUnsignedByte(&handle->next);
    record->length = ReadUnsignedWord(&handle->next);
    
    // Set the child pointer to NULL
    record->next = NULL;
    
    return record;
}

void EndRecord(struct RECORD* record, struct MagicOMFHandle* handle)
{
    record->checksum = ReadUnsignedByte(&handle->next);
    
    // Set the child of our parent to us if any, this is required for iterations.
    if(handle->last != NULL)
    {
        handle->last->next = record;
    }
    
    handle->last = record;
}


void TranslatorReadLHEADR(struct MagicOMFHandle* handle)
{
    struct RECORD* record = StartRecord(handle);
    if (record->type != 0x80)
    {
        error(INVALID_THEADR_PROVIDED, handle);
        return;
    }
    
    struct THEADR* contents = malloc(sizeof(struct THEADR));
    record->contents = contents;
    contents->string_length = ReadUnsignedByte(&handle->next);
    contents->name_string = ReadStringAddTerminator(&handle->next, contents->string_length);
    EndRecord(record, handle);
    
    // LHEADR records will always be the root
    handle->root = record;
    handle->last = record;
}