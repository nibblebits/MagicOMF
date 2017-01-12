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
 * File:   generator.c
 * Author: dansb
 *
 * Created on 12 January 2017, 03:24
 */

#include "generator.h"
#include "record.h"

void GeneratorWriteTHEADR(char** ptr, struct RECORD* record)
{
    if (record->type != THEADR_ID)
    {
        error(INVALID_THEADR_PROVIDED, record->handle);
        return;
    }
    
    struct THEADR* theadr = (struct THEADR*) record->contents;
    // Setup where we will write to
    WritingToPointer(ptr);
    
    // Write the opcode
    WriteUnsignedByte(THEADR_ID);
    // Write the record length
    WriteUnsignedWord(record->length);
    // Write the string size
    WriteUnsignedByte(theadr->string_length);
    // Now finally write the string
    WriteStringNoTerminator(theadr->name_string, theadr->string_length);
}
