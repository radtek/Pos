/*
  ZMHash.h - Header file for Central Directory hash list
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

  last modified 2009-05-16
---------------------------------------------------------------------------*/

#ifndef ZBHashH
#define ZBHashH
#include "ZBStructs.h"
//---------------------------------------------------------------------------
namespace Zbhash
{

const HDEBlockEntries = 511; // number of entries per block

struct THashedDirEntry
{
  THashedDirEntry* Next;
  ZipDirEntry* ZRec;
  unsigned Hash;
};

struct THDEBlock
{
  THashedDirEntry Entries[HDEBlockEntries];
  THDEBlock*  Next;
};

class __declspec(delphiclass) TZipDirHashList : public TObject
{
typedef TObject inherited;
private:
	THDEBlock*  FLastBlock;
  unsigned FNextEntry;
  unsigned __fastcall GetSize(void);
  void __fastcall SetSize(const unsigned Value);
protected:
  int FBlocks;
  DynamicArray<THashedDirEntry*> Chains;
  THashedDirEntry* __fastcall GetEntry(void);
  bool __fastcall Same(THashedDirEntry* Entry, unsigned Hash, const String& Str);
public:
	__fastcall TZipDirHashList(void) {};
	__fastcall virtual ~TZipDirHashList( void ) {};
  ZipDirEntry* __fastcall Add(const ZipDirEntry* Rec);
  void __fastcall AfterConstruction(void);
  void __fastcall AutoSize(unsigned Req);
  void __fastcall BeforeDestruction(void);
  void __fastcall Clear(void);
  ZipDirEntry* __fastcall Find(const String& FileName);
  __property unsigned Size={read=GetSize,write=SetSize};
};


}	/* namespace Zbhash */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Zbhash;
#endif

#endif
