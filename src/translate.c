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
    struct RECORD* record = (struct RECORD*) malloc(sizeof (struct RECORD));
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

    // No root set so lets set it
    if (handle->root == NULL)
    {
        handle->root = record;
    }
    else
    {
        // Ok we already have a root so we know we have a last, lets append the last record's next pointer to point to us.
        handle->last->next = record;
    }
    handle->last = record;
}

void TranslatorReadTHEADR(struct MagicOMFHandle* handle)
{
    struct RECORD* record = StartRecord(handle);
    if (record->type != THEADR_ID)
    {
        error(INVALID_THEADR_PROVIDED, handle);
        return;
    }

    struct THEADR* contents = malloc(sizeof (struct THEADR));
    record->contents = contents;
    contents->string_length = ReadUnsignedByte(&handle->next);
    contents->name_string = ReadStringAddTerminator(&handle->next, contents->string_length);
    EndRecord(record, handle);

}

void TranslatorReadLHEADR(struct MagicOMFHandle* handle)
{
    struct RECORD* record = StartRecord(handle);
    if (record->type != LHEADR_ID)
    {
        error(INVALID_LHEADR_PROVIDED, handle);
        return;
    }

    struct LHEADR* contents = malloc(sizeof (struct LHEADR));
    record->contents = contents;
    contents->string_length = ReadUnsignedByte(&handle->next);
    contents->name_string = ReadStringAddTerminator(&handle->next, contents->string_length);
    EndRecord(record, handle);
}

void TranslatorReadCOMENT(struct MagicOMFHandle* handle)
{
    struct RECORD* record = StartRecord(handle);
    if (record->type != COMENT_ID)
    {
        error(INVALID_COMENT_PROVIDED, handle);
        return;
    }

    struct COMENT* contents = malloc(sizeof (struct COMENT));
    record->contents = contents;
    contents->c_type = ReadUnsignedByte(&handle->next);
    contents->c_class = ReadUnsignedByte(&handle->next);

    uint16 string_size = record->length - 3;

    contents->c_string = ReadStringAddTerminator(&handle->next, string_size);
    contents->no_purge = (contents->c_type & 0x80) == 0x80;
    contents->no_list = (contents->c_type & 0x40) == 0x40;
    EndRecord(record, handle);

}

void TranslatorReadLNAMES(struct MagicOMFHandle* handle)
{
    struct RECORD* record = StartRecord(handle);
    if (record->type != LNAMES_ID)
    {
        error(INVALID_LNAMES_PROVIDED, handle);
        return;
    }

    // -1 for the checksum.
    char* end = handle->next + record->length-1;

    // Read pointless byte, someone change this if I am wrong, taking a bit of an educated guess here
    ReadUnsignedByte(&handle->next);

    struct LNAMES* prev = NULL;
    while (handle->next < end)
    {
        struct LNAMES* contents = malloc(sizeof (struct LNAMES));
        contents->next = NULL;
        if (prev == NULL)
        {
            // This is the first iteration so set the record contents to these contents
            record->contents = contents;
        }
        else
        {
            prev->next = contents;
        }
        prev = contents;

        contents->s_len = ReadUnsignedByte(&handle->next);
        contents->n_string = ReadStringAddTerminator(&handle->next, contents->s_len);
    }

    EndRecord(record, handle);

}

/* 32 bit is not currently supported, translator will read from lowest possible
 * possibility. */
void TranslatorReadSEGDEF(struct MagicOMFHandle* handle)
{
    struct RECORD* record = StartRecord(handle);
    if (record->type != SEGDEF_ID)
    {
        error(INVALID_SEGDEF_PROVIDED, handle);
        return;
    }

    struct SEGDEF* contents = malloc(sizeof (struct SEGDEF));
    record->contents = contents;
    uint8 ACBP = ReadUnsignedByte(&handle->next);
    uint8 A = ACBP >> 5;
    uint8 C = (ACBP >> 2) & 0x07;
    uint8 B = (ACBP >> 1) & 0x01;
    uint8 P = (ACBP & 0x01);
    contents->attributes.A = A;
    contents->attributes.C = C;
    contents->attributes.B = B;
    contents->attributes.P = P;
    contents->attributes.ACBP = ACBP;
    contents->seg_len = ReadUnsignedWord(&handle->next);

    // When B = 0 then the segment length must also be zero.
    if (B == 0)
    {
        if (!contents->seg_len != 0)
        {
            error(IMPROPERLY_FORMATTED_SEGDEF_PROVIDED, handle);
            return;
        }
    }
    contents->seg_name_index = ReadUnsignedByte(&handle->next);
    contents->class_name_index = ReadUnsignedByte(&handle->next);
    contents->overlay_name_index = ReadUnsignedByte(&handle->next);
    EndRecord(record, handle);

}
