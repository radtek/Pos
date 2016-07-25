/*
  ZMHash.cpp - Central Directory hash list
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

  last modified 2009-05-18
---------------------------------------------------------------------------*/

#pragma hdrstop

#include <vcl.h>
#include "ZBHash.h"
#include <assert.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

const ChainsMax = 40961,
  ChainsMin = 61;

// P. J. Weinberger Hash function
unsigned __fastcall HashFunc(const String& Str)
{
  unsigned Result = 0;
  for(int i = 1; i <= Str.Length(); ++i)
  {
    Result = (Result << 4) + Str[i];
    unsigned x = Result & 0xF0000000;
    if(x) Result = (Result ^ (x >> 24)) & 0x0FFFFFFF;
  }
  return Result;
};

ZipDirEntry* __fastcall TZipDirHashList::Add(const ZipDirEntry* Rec)
{
  assert(Rec);
  if(!Chains.Length) Size = 1283;
  ZipDirEntry* Result = NULL;
  String UpperName = UpperCase(Rec->FileName);
  unsigned Hash = HashFunc(UpperName);
  int Idx = Hash % static_cast<unsigned>(Chains.Length);
  THashedDirEntry* Entry = Chains[Idx];
  if(!Entry)
  {
    Entry = GetEntry();
    Entry->Hash = Hash;
    Entry->ZRec = const_cast<ZipDirEntry*>(Rec);
    Entry->Next = NULL;
    Chains[Idx] = Entry;
  }
  else
  {
    do
    {
      if(Same(Entry, Hash, UpperName))
      {
        return Entry->ZRec;   // duplicate name
      }
      THashedDirEntry* Parent = Entry;
      Entry = Entry->Next;
      if(!Entry)
      {
        Entry = GetEntry();
        Entry->ZRec = NULL;
        Parent->Next = Entry;
      }
    }while(Entry->ZRec);
    // we have an entry so fill in the details
    Entry->Hash = Hash;
    Entry->ZRec = const_cast<ZipDirEntry*>(Rec);
    Entry->Next = NULL;
  }
  return Result;
};

void __fastcall TZipDirHashList::AfterConstruction(void)
{
  inherited::AfterConstruction();
  Chains.Length = 0;
  FBlocks = 0;
  FLastBlock = NULL;
	FNextEntry = 0xffffffff;
}

// set size to a reasonable prime number
void __fastcall TZipDirHashList::AutoSize(unsigned Req)
{
const unsigned PrimeSizes[29] =
  {61, 131, 257, 389, 521, 641, 769, 1031, 1283, 1543, 2053, 2579, 3593,
   4099, 5147, 6151, 7177, 8209, 10243, 12289, 14341, 16411, 18433, 20483,
   22521, 24593, 28687, 32771, 40961};

  if(Req < 15000)
  {
    // use next higher size
    for(int i = 0; i < 29; ++i)
      if(PrimeSizes[i] >= Req)
      {
        Req = PrimeSizes[i];
        break;
      }
  }
  else
  {
    // use highest smaller size
    for(int i = 28; i >= 0; --i)
      if(PrimeSizes[i] < Req)
      {
        Req = PrimeSizes[i];
        break;
      }
  }
  SetSize(Req);
};

void __fastcall TZipDirHashList::BeforeDestruction(void)
{
  Clear();
  inherited::BeforeDestruction();
}

void __fastcall TZipDirHashList::Clear(void)
{
  while(FLastBlock)
  {
    THDEBlock* TmpBlock = FLastBlock;
    FLastBlock = TmpBlock->Next;
    delete TmpBlock;
  }
  Chains.Length = 0;
  FBlocks = 0;
  FLastBlock = NULL;
  FNextEntry = 0xffffffff;
}

ZipDirEntry* __fastcall TZipDirHashList::Find(const String& FileName)
{
  ZipDirEntry* Result = NULL;
  if(!Chains.Length) return Result;
  String UpperName = UpperCase(FileName);
  unsigned Hash = HashFunc(UpperName);
  unsigned idx  = Hash % static_cast<unsigned>(Chains.Length);
  THashedDirEntry* Entry = Chains[idx];
  // check entries in this chain
  while(Entry)
  {
    if(Same(Entry, Hash, UpperName))
    {
      Result = Entry->ZRec;
      break;
    }
    else Entry = Entry->Next;
  }
  return Result;
}

// return address in allocated block
THashedDirEntry* __fastcall TZipDirHashList::GetEntry(void)
{
  if((FBlocks < 1) || (FNextEntry >= HDEBlockEntries))
  {
    // we need a new block
    THDEBlock* TmpBlock = new THDEBlock;
    memset(TmpBlock, 0, sizeof(THDEBlock));
    TmpBlock->Next = FLastBlock;
    FLastBlock = TmpBlock;
    ++FBlocks;
    FNextEntry = 0;
  }
  THashedDirEntry* Result = &FLastBlock->Entries[FNextEntry];
  ++FNextEntry;
  return Result;
};

unsigned __fastcall TZipDirHashList::GetSize(void)
{
  return Chains.Length;
}

bool __fastcall TZipDirHashList::Same(THashedDirEntry* Entry, unsigned Hash,
																			const String& Str)
{
  return (Hash == Entry->Hash) && (CompareText(Str, Entry->ZRec->FileName) == 0);
}

void __fastcall TZipDirHashList::SetSize(const unsigned Value)
{
  Clear();
  if(Value > 0)
  {
    int TableSize = Value;
    // keep within reasonable limits
    if(TableSize < ChainsMin) TableSize = ChainsMin;
    else if(TableSize > ChainsMax) TableSize = ChainsMax;
    Chains.Length = TableSize;
    memset(&Chains[0], 0, Size * sizeof(THashedDirEntry*));
  }
};