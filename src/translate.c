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
#include "MagicOMF.h"
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
    // -1 so end of record will point to the checksum.
    record->end_of_record = handle->next + record->length - 1;

    // Set the child pointer to NULL
    record->next = NULL;
    // Assume we have a checksum until otherwise specified
    record->has_checksum = true;

    return record;
}

void EndRecord(struct RECORD* record, struct MagicOMFHandle* handle)
{

    // Do we have a checksum? if so lets read it
    if (record->has_checksum)
    {
        record->checksum = ReadUnsignedByte(&handle->next);
    }

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

void TranslatorReadCOMENT_Translator(struct RECORD* record, struct MagicOMFHandle* handle)
{
    struct COMENT* contents = (struct COMENT*) (record->contents);
    contents->c_string = ReadStringUntilEndAddTerminator(&handle->next, record->end_of_record);
}

void TranslatorReadCOMENT_LinkPassSeparator(struct RECORD* record, struct MagicOMFHandle* handle)
{
    struct COMENT* contents = (struct COMENT*) (record->contents);
    uint8 subtype = ReadUnsignedByte(&handle->next);
    if (subtype != 1)
    {
        // This is a problem it should be 1.
        // Show an error here.
    }
    else
    {
        contents->is_link_pass_seperator = true;
        // Specification states that its possible bytes can come after the subtype but they must be ignored
        ReadStringUntilEndAddTerminator(&handle->next, record->end_of_record);
    }
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
    contents->c_string = NULL;

    switch (contents->c_class)
    {
    case COMENT_CLASS_LINK_PASS_SEPARATOR:
        TranslatorReadCOMENT_LinkPassSeparator(record, handle);
        break;
    case COMENT_CLASS_TRANSLATOR:
        TranslatorReadCOMENT_Translator(record, handle);
        break;
    default:
        error(UNKNOWN_COMENT_CLASS, handle);
    }


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
    
    struct LNAMES* prev = NULL;
    struct LNAMES* root_lnames = NULL;
    while (handle->next < record->end_of_record)
    {
        struct LNAMES* contents = malloc(sizeof (struct LNAMES));
        contents->s_len = ReadUnsignedByte(&handle->next);
        contents->n_string = ReadStringAddTerminator(&handle->next, contents->s_len);
        contents->next = NULL;

        if (root_lnames == NULL)
        {
            // This must be the root
            root_lnames = contents;
        }
        else if (prev->next == NULL)
        {
            prev->next = contents;
        }
        prev = contents;
    }

    record->contents = root_lnames;
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

    // Lets set the LNAME string that this SEGDEF is using just for ease of access
    contents->class_name_str = MagicOMFGetLNAMESNameByIndex(handle, contents->seg_name_index);
    if (contents->class_name_str == NULL)
    {
        // The file doesn't define an LNAME with the index specified...
        error(LNAMES_NOT_FOUND, handle);
    }
    EndRecord(record, handle);

}

void TranslatorReadPUBDEF16(struct MagicOMFHandle* handle)
{
    struct RECORD* record = StartRecord(handle);
    if (record->type != PUBDEF_16_ID)
    {
        error(INVALID_PUBDEF_16_PROVIDED, handle);
    }

    struct PUBDEF_16* contents = malloc(sizeof (struct PUBDEF_16));
    record->contents = contents;
    contents->bg_index = ReadUnsignedByte(&handle->next);
    contents->bs_index = ReadUnsignedByte(&handle->next);
    if (contents->bs_index == 0)
    {
        if (contents->bg_index == 0)
        {
            // Absolute addressing is not supported
            error(PUBDEF_16_ABSOLUTE_ADDRESSING_NOT_SUPPORTED, handle);
        }
        else
        {
            // Ok this is legal but we do not care about the base frame so just read but ignore it
            ReadUnsignedWord(&handle->next);
        }
    }

    struct PUBDEF_16_IDEN* prev = NULL;
    while (handle->next < record->end_of_record)
    {
        struct PUBDEF_16_IDEN* iden = malloc(sizeof (struct PUBDEF_16_IDEN));
        iden->str_len = ReadUnsignedByte(&handle->next);
        iden->name_str = ReadStringAddTerminator(&handle->next, iden->str_len);
        iden->p_offset = ReadUnsignedWord(&handle->next);
        iden->type_index = ReadUnsignedByte(&handle->next);
        if (prev == NULL)
        {
            // This is the first iteration so set the contents iden pointer
            contents->iden = iden;
        }
        else
        {
            // Set the previous identifier to point to us
            prev->next = iden;
        }
        prev = iden;
    }

    EndRecord(record, handle);

}

void TranslatorReadLEDATA16(struct MagicOMFHandle* handle)
{
    struct RECORD* record = StartRecord(handle);
    if (record->type != LEDATA_16_ID)
    {
        error(INVALID_LEDATA_16_PROVIDED, handle);
    }

    struct LEDATA_16* contents = malloc(sizeof (struct LEDATA_16));
    contents->seg_index = ReadUnsignedByte(&handle->next);
    if (contents->seg_index == 0)
    {
        // Seg index must be non zero!
        error(INVALID_LEDATA_16_PROVIDED, handle);
    }

    contents->data_offset = ReadUnsignedWord(&handle->next);

    // -4 due to seg index, data offset and checksum.
    contents->data_bytes_size = record->length - 4;
    if (contents->data_bytes_size > 1024)
    {
        // Specification prohibits the data bytes size being above 1024
        error(INVALID_LEDATA_16_PROVIDED, handle);
    }

    contents->data_bytes = ReadDataUntilEnd(&handle->next, record->end_of_record);

    EndRecord(record, handle);
}

void TranslatorReadFIXUPP16(struct MagicOMFHandle* handle)
{
    struct RECORD* record = StartRecord(handle);
    if (record->type != FIXUPP_16_ID)
    {
        error(INVALID_FIXUPP_16_PROVIDED, handle);
    }

    while (handle->next < record->end_of_record)
    {
        uint8 thread_or_fixup = ReadUnsignedByte(&handle->next);
        if (thread_or_fixup & 0x80)
        {
            // Ok this is a FIXUP
            // Build the locat
            uint16 locat = (thread_or_fixup << 8 | ReadUnsignedByte(&handle->next));
            record->contents = malloc(sizeof (struct FIXUPP_16_FIXUP_SUBRECORD));
            TranslatorReadFIXUPP16_FIXUP_SUBRECORD(locat, record, handle);
        }
        else
        {
            // THREAD not supported
            error(FIXUPP_16_THREAD_NOT_SUPPORTED, handle);
        }
    }
    EndRecord(record, handle);
}

void TranslatorReadFIXUPP16_FIXUP_SUBRECORD(uint16 locat, struct RECORD* record, struct MagicOMFHandle* handle)
{
    struct FIXUPP_16_FIXUP_SUBRECORD* contents = (struct FIXUPP_16_FIXUP_SUBRECORD*) (record->contents);
    contents->mode = (locat >> 14) & 0x01;
    contents->location = (locat >> 10) & 0x0f;
    contents->data_record_offset = (locat) & 0x3ff;

    // Ok only segment relative fixups and 16 bit offsets are currently supported
    if (contents->mode != FIXUPP_MODE_SEGMENT_RELATIVE_FIXUP
            || contents->location != FIXUPP_LOCATION_16_BIT_OFFSET)
    {
        error(FIXUPP_16_MODE_OR_LOCATION_NOT_SUPPORTED, handle);
    }

    uint8 fix_data = ReadUnsignedByte(&handle->next);
    uint8 F = (fix_data >> 7) & 0x01;
    uint8 frame = (fix_data >> 4) & 0x07;
    uint8 T = (fix_data >> 3) & 0x01;
    uint8 P = (fix_data >> 2) & 0x01;
    uint8 targt = (fix_data) & 0x03;

    // We don't support a lot of these options at the moment.
    if (F == 1 || T == 1 || P == 0)
    {
        error(FIXUPP_16_FIX_DATA_UNSUPPORTED, handle);
    }

    // Read the frame datum field
    contents->frame_datum = ReadUnsignedByte(&handle->next);
}

void TranslatorReadMODEND16(struct MagicOMFHandle* handle)
{
    struct RECORD* record = StartRecord(handle);
    if (record->type != MODEND_16_ID)
    {
        error(INVALID_MODEND_16_PROVIDED, handle);
    }

    struct MODEND_16* contents = malloc(sizeof (struct MODEND_16));
    uint8 module_type = ReadUnsignedByte(&handle->next);
    if (module_type & 0x40)
    {
        // We don't support start addresses
        error(MODEND_16_MODULE_TYPE_UNSUPPORTED, handle);
    }

    contents->is_main = (module_type >> 6) & 0x01;
    contents->has_start_address = (module_type >> 7);

    EndRecord(record, handle);
}