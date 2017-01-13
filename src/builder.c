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
 * File:   builder.c
 * Author: dansb
 *
 * Created on 11 January 2017, 22:44
 */

#include <string.h>
#include <stdlib.h>
#include "builder.h"

struct RECORD* BuildRecord(struct MagicOMFHandle* handle, uint8 record_type, uint16 record_length, uint8 checksum)
{
    struct RECORD* record = (struct RECORD*) malloc(sizeof (struct RECORD));

    // Read the standard record details
    record->type = record_type;
    record->length = record_length;
    if (handle->next == NULL)
    {
        record->end_of_record = NULL;
    }
    else
    {
        // -1 so end of record will point to the checksum.
        record->end_of_record = handle->next + record->length - 1;
    }
    // Set the child pointer to NULL
    record->next = NULL;
    // Lets set the previous
    record->prev = handle->last;
    // Assume we have a checksum until otherwise specified
    record->has_checksum = true;
    record->contents = NULL;
    record->checksum = checksum;
    record->handle = handle;
    return record;
}

struct THEADR* BuildTHEADR(char* name)
{
    return BuildTHEADR_DefinedSize(name, strlen(name));
}

struct THEADR* BuildTHEADR_DefinedSize(char* name, int size)
{
    struct THEADR* contents = malloc(sizeof (struct THEADR));
    contents->string_length = size;
    contents->name_string = name;
    return contents;
}

struct COMENT* BuildCOMENT(COMMENT_TYPE type, uint8 _class, char* str)
{
    struct COMENT* contents = malloc(sizeof (struct COMENT));
    contents->c_type = type;
    contents->c_class = _class;
    contents->c_string = str;
    contents->no_list = type & 0x40;
    contents->no_purge = type & 0x80;
    if (contents->c_class == COMENT_CLASS_LINK_PASS_SEPARATOR)
    {
        contents->is_link_pass_seperator = true;
    }
    else
    {
        contents->is_link_pass_seperator = false;
    }
    
    return contents;
}