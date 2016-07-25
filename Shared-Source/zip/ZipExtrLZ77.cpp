/*
  ZipExtrlZ77.cpp - Source file of LZ77 stream expander
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2005  Roger Aelbrecht

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License (licence.txt) for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  contact: Zipbuilder AT drehoeksw DOT net
  DelphiZip maillist subscribe at http://www.geocities.com/SiliconValley/Orchard/8607/
  updates: http://www.drehoeksw.net

  last modified 2005-11-20
---------------------------------------------------------------------------*/
#include <vcl.h>
#pragma hdrstop
#include <limits.h>
#include "ZipExtrLZ77.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//----------------------------------------------------------------------------
int GetByte(int& bytes, TStream* src)
{

    unsigned char ch;
    if ((bytes > 4) && (src->Size > src->Position))
    {
        --bytes;
        if (src->Read(&ch, 1) == 1)
            return ch;
        else
            bytes = 0;
    }
    return -1;

}
// expects src at orig_size (integer), data (bytes)
//---------------------------------------------------------------------------
int LZ77Extract(TStream* dst, TStream* src, int size)
{
    int bytes = size;
    if (!bytes)
        bytes = INT_MAX;
    int filesize;
    src->ReadBuffer(&filesize, sizeof(int));
    int written = 0;
    unsigned char* buffer = new unsigned char[N];
    int i = N - F;
    while (true)
    {
        int bits;
        if ((bits = GetByte(bytes, src)) < 0)
            break;
        int mask = 1;
        while (mask < 256)
        {
            if (!(bits & mask))
            {
                int j;
                if ((j = GetByte(bytes, src)) < 0)
                    break;
                int len = GetByte(bytes, src);
                j += (len & 0xF0) << 4;
                len = (len & 15) + 3;
                do
                {
                    buffer[i] = buffer[j];
                    dst->WriteBuffer(&buffer[i], 1);
                    ++written;
                    j = ++j & NMask;
                    i = ++i & NMask;// do not use ++i &= NMask -> problem with optimization
                }
                while (--len);
            }
            else
            {
                int ch;
                if ((ch = GetByte(bytes, src)) < 0)
                    break;
                buffer[i] = static_cast<unsigned char>(ch);
                dst->WriteBuffer(&ch, 1);
                ++written;
                ++i &= NMask;
            }
            mask += mask;
        }
    }
    delete[] buffer;
    if ((filesize == written) && (bytes == 4))
        return 0;   // good
    return (bytes == 4) ? -2 : -1;   // -2 = wrong length, -1 invalid data
}
