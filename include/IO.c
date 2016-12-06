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
 * File:   IO.c
 * Author: Daniel McCarthy
 *
 * Created on 06 December 2016, 12:16
 */

#include <stdlib.h>

#include "IO.h"

uint8 ReadUnsignedByteNoNext(char* ptr)
{
    uint8 v = *ptr;
    return v;
}

uint16 ReadUnsignedWordNoNext(char* ptr)
{
    uint8 l = *ptr;
    uint8 h = *(ptr+1);
    uint16 word = (h << 8) | (l);
    return word;
}

char* ReadStringAddTerminator(char** ptr, uint8 size)
{
    char* buf = (char*) malloc(size+1);
    for(uint8 i = 0; i < size; i++)
    {
        buf[i] = **ptr;
        *ptr+=1;
    }
    buf[size+1] = 0;
    return buf;
}

uint8 ReadUnsignedByte(char** ptr)
{
    uint8 b = ReadUnsignedByteNoNext(*ptr);
    *ptr += 1;
    return b;
}

uint16 ReadUnsignedWord(char** ptr)
{
    uint16 word = ReadUnsignedWordNoNext(*ptr);
    *ptr += 2;
    return word;
}

uint32 ReadUnsignedInt(char** ptr)
{

}

int8 ReadSignedByte(char** ptr)
{

}

int16 ReadSignedWord(char** ptr)
{

}

int32 ReadSignedInt(char** ptr)
{

}
