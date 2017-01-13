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

void GeneratorWriteRecordHeader(char** ptr, struct RECORD* record)
{
    // Setup where we will write to
    WritingToPointer(ptr);
    // Write the opcode
    WriteUnsignedByte(record->type);
    // Write the record length
    WriteUnsignedWord(record->length);
}

void GeneratorWriteTHEADR(char** ptr, struct RECORD* record)
{
    if (record->type != THEADR_ID)
    {
        error(INVALID_THEADR_PROVIDED, record->handle);
        return;
    }

    struct THEADR* theadr = (struct THEADR*) record->contents;
    // Write the record header
    GeneratorWriteRecordHeader(ptr, record);
    // Write the string size
    WriteUnsignedByte(theadr->string_length);
    // Now write the string
    WriteStringNoTerminator(theadr->name_string, theadr->string_length);

    // Now the checksum
    WriteUnsignedByte(0);
}

void GeneratorWriteCOMENT(char** ptr, struct RECORD* record)
{
    if (record->type != COMENT_ID)
    {
        error(INVALID_COMENT_PROVIDED, record->handle);
        return;
    }

    // Write the record header
    GeneratorWriteRecordHeader(ptr, record);

    struct COMENT* coment = (struct COMENT*) record->contents;
    WriteUnsignedByte(coment->c_type);
    WriteUnsignedByte(coment->c_class);
    WriteData(coment->c_string, record->length - 3);

    // Now the checksum
    WriteUnsignedByte(0);
}

void GeneratorWriteLNAMES(char** ptr, struct RECORD* record)
{
    if (record->type != LNAMES_ID)
    {
        error(INVALID_LNAMES_PROVIDED, record->handle);
        return;
    }

    // Write the record header
    GeneratorWriteRecordHeader(ptr, record);

    struct LNAMES* lnames = (struct LNAMES*) record->contents;
    struct LNAMES* current = lnames;
    while (current != NULL)
    {
        WriteUnsignedByte(current->s_len);
        WriteData(current->n_string, current->s_len);
        current = current->next;
    }

    // Write the checksum
    WriteUnsignedByte(0);
}

void GeneratorWriteSEGDEF16(char** ptr, struct RECORD* record)
{
    if (record->type != SEGDEF_16_ID)
    {
        error(INVALID_SEGDEF_16_PROVIDED, record->handle);
        return;
    }

    // Write the record header
    GeneratorWriteRecordHeader(ptr, record);

    struct SEGDEF_16* segdef_16 = (struct SEGDEF_16*) record->contents;
    struct Attributes attributes = segdef_16->attributes;
    // Lets create the ACBP
    uint8 ACBP = (attributes.A << 5) | (attributes.C << 2) | (attributes.B << 1) | (attributes.P);
    // Write the ACBP
    WriteUnsignedByte(ACBP);
    if (attributes.A == SEG_ATTR_ALIGNMENT_ABS_SEG)
    {
        // Alignment is an absolute segment so we are expecting a frame number and an offset
        WriteUnsignedWord(attributes.frame_number);
        WriteUnsignedByte(attributes.offset);
    }

    if (attributes.B == 1)
    {
        WriteUnsignedWord(0);
    }
    else
    {
        WriteUnsignedWord(segdef_16->seg_len);
    }
    
    WriteUnsignedByte(segdef_16->seg_name_index);
    WriteUnsignedByte(segdef_16->class_name_index);
    WriteUnsignedByte(segdef_16->overlay_name_index);
    
    // Finally write the checksum
    WriteUnsignedByte(0);


}