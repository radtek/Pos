#ifndef ZBMsgStrH
#define ZBMsgStrH
/*
  ZBMsgStr.cpp - Header file of unit for message string handler in TZipBuilder
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

  last modified 2005-02-04
---------------------------------------------------------------------------*/

#include <vcl.h>
#include <system.hpp>
#include "ZipMsg.h"
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TZipStrEvent)(int Ident, String& DefStr);

typedef DynamicArray<Word> TResBlock;

bool IsAlphaChar(char c);

String ZipFmtLoadStr(int Ident, const System::TVarRec* Args, const int Args_Size);
String ZipLoadStr(int id);
// '*' = Auto, '' = default US,  language  (number = language id)
String SetZipMsgLanguage(const String& zl);
// get language at index (<0 - default, 0 - current)
String GetZipMsgLanguage(int idx);
// info (-1) = language id, 0 = name, other values as per windows LOCALE_
String GetZipMsgLanguageInfo(int Index, unsigned info);

String LanguageIdent(const String& seg);
String LocaleInfo(int loc, unsigned info);

unsigned LoadFromStream(TResBlock& blk, TStream* src);
bool FindInStream(TStream* src, String& segname, Word& lid);
bool IdInStream(TStream* src, unsigned& idx, String& lang);
String FindRes(const TResBlock& block, int id);
const char* Find(int id);
String FindConst(int id);
int LoadCompressedDef(const void* src);


#endif
