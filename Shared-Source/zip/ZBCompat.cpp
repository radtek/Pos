/*
  ZBCompat.cpp - Types and utility functions required for some compilers
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2009  Roger Aelbrecht

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

  last modified 2008-10-19
---------------------------------------------------------------------------*/

#pragma hdrstop

#include "ZBCompat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

bool CharInSet(Byte C, const TSysCharSet CharSet)
{
	return CharSet.Contains(C);
}

AnsiChar* MakeStrP(const String& str)
{
#ifdef UNICODE
  AnsiString StrA = AnsiString(str);
  AnsiChar* Result = AnsiStrAlloc(StrA.Length() + 1);
  StrPLCopy(Result, StrA, StrA.Length() + 1);
#else
  AnsiChar* Result = StrAlloc(str.Length() + 1);
  StrPLCopy(Result, str, str.Length() + 1);
#endif
	return Result;
}

