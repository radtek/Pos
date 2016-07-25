/*
  ZBMsgStr.cpp - Source file of unit for message string handler in TZipBuilder
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

  last modified 2008-10-07
---------------------------------------------------------------------------*/

#pragma hdrstop
#include "CondDefines.h"
#include "ZBMsgStr.h"
#include "ZBDefMsgs.h"
#include "ZipExtrLZ77.h"
#include "ZBCompat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#if (__BORLANDC__ >= 0x0560)
#define STRMSIZ __int64
#else
#define STRMSIZ int
#endif

const int DZRES_Str = 11610;  // compressed language strings
TZipStrEvent OnZipStr = NULL;
TResBlock DefRes;
TResBlock SelRes;
unsigned SelId;
String SelName;
//---------------------------------------------------------------------------
//bool IsAlphaChar(char c)
//{
//  return ( ((c >= 'A') && (c <= 'Z')) ||
//           ((c >= 'a') && (c <= 'z')) || (c == '_') ||
//           ((c >= '0') && (c <= '9')) );
//}
TSysCharSet lchars;
TSysCharSet numbs;
//TCharSet xnumbs;
void initsets(void)
{
    lchars << '_';
    for (char c = 'A'; c <= 'Z'; ++c)
        lchars << c;
    for (char c = '0'; c <= '9'; ++c)
        lchars << c;
    for (char c = 'a'; c <= 'z'; ++c)
        lchars << c;
    for (char c = '0'; c <= '9'; ++c)
        numbs << c;
// for(char c = '0'; c <= '9'; ++c) xnumbs << c;
// xnumbs << 'x' << 'X';
}
#pragma startup initsets
//---------------------------------------------------------------------------
String LanguageIdent(const String& seg)
{
    String Result;
    int Len = seg.Length();
    for (int i = 1; i <= Len; ++i)
    {
        char c = seg[i];
//    if(!IsAlphaChar(c))
        if (!CharInSet(c, lchars))
        {
            if ((!Result.IsEmpty()) || (c != ' '))
                break;
        }
        else
        {
            if ((Result.IsEmpty()) && CharInSet(c, numbs))
                break;
            Result += c;
        }
    }
    return Result.UpperCase();
}
//---------------------------------------------------------------------------
/// format is
// id: word, data_size: word, label_size: word, label: char[], data: byte[];... ;0
// stream at id
unsigned LoadFromStream(TResBlock& blk, TStream* src)
{
    blk.Length = 0;  // empty it
//  SelRes.Length = 0;
//  SelId = 0;
    Word sz[3];
    if (src->Read(&sz[0], (3 * sizeof(Word))) != static_cast<int>(3 * sizeof(Word)))
        return 0;
    if (src->Size < static_cast<int>(sz[1] + sz[2] + (3 * sizeof(Word))))
        return 0;
    src->Position = src->Position + sz[2]; // skip name;
    TMemoryStream* so = NULL;
    int Res = 0;
    try
    {
        so = new TMemoryStream();
        Res = LZ77Extract(so, src, sz[1]);
        if (!Res && (so->Size < 50000))
        {
            int szw = (static_cast<int>(so->Size) + (sizeof(Word) - 1))  / sizeof(Word);
            blk.Length = szw + 1;
            so->Position = 0;
            if (so->Read(&blk[0], static_cast<int>(so->Size)) == static_cast<int>(so->Size))
            {
                blk[szw] = -1;
                Res = sz[0];
            }
            else
                blk.Length = 0;
        }
    }
    __finally
    {
        delete so;
    }
    return Res;
}
//---------------------------------------------------------------------------
// format is
// id: word, data_size: word, label_size: word, label: char[], data: byte[];... ;0
// positions stream to point to id
// segname has identifier terminated by ':'
bool FindInStream(TStream* src, String& segname, Word& lid)
{
    bool Result = false;
    if (!src)
        return false;
    String seg = LanguageIdent(segname);
    if ((seg.Length() < 2) && !lid)
        return false;
    STRMSIZ p = src->Position;
    STRMSIZ ss = src->Size - ((3 * sizeof(Word)) + 2) ; // id + llen + dlen + min 2 chars
    String s;
    Word w3[3];  // id, dsize, nsize
    while (!Result && (p < ss))
    {
        src->Position = static_cast<int>(p);
        src->ReadBuffer(&w3[0], 3 * sizeof(Word));  // id, dsize, nsize
        Word w = w3[2];  // name size
        if (w)
        {
            s.SetLength(w);
            src->ReadBuffer(&s[1], w);  // read name
        }
        if (!lid)
        {
            String l;
            for (int i = 1; i <= w; ++i)
            {
                char c = s[i];
//        if(IsAlphaChar(c)) l += c;
                if (CharInSet(c, lchars))
                    l += c;
                else
                    break;
            }
            Result = AnsiCompareText(l, segname) == 0;
        }
        else
        {
            Result = (lid == w3[0]) || ((lid < 0x400) && ((w3[0] & 0x03FF) == lid));
        }
        if (!Result)
            p = src->Position + w3[1];  // skip data
    }
    if (Result)
    {
        segname = s;
        lid = w3[0];
        src->Position = p;
    }
    return Result;
}

//---------------------------------------------------------------------------
bool IdInStream(TStream* src, unsigned& idx, String& lang)
{
    if ((idx < 1) || !src)
        return false;
    STRMSIZ p = src->Position;
    STRMSIZ ss = src->Size - ((3 * sizeof(Word)) + 20); //id + dlen + nlen + 20 bytes
    Word w3[3];   // id, dsize, nsize
    if (p > ss)
        return false;
    do
    {
        src->ReadBuffer(&w3[0], 3 * sizeof(Word));  // id, dsize, nsize
        if (idx <= 1)
            break;
        --idx;
        p = src->Position + w3[1] + w3[2];   // after name + date
        if (p < ss)
            src->Position = p;
        else
            return false;
    }
    while (true);
    AnsiString s;
    s.SetLength(w3[2]);
    src->ReadBuffer(s.c_str(), w3[2]);  // read name
    lang = String(s);
    idx = w3[0];
    src->Position = p;
    return true;
}
//---------------------------------------------------------------------------
String SetZipMsgLanguage(const String& zl)
{
    TResourceStream* Res = NULL;
    SelRes.Length = 0;   // reset to default
    SelId = 0;
    SelName = "";
    String Result;
    Word id = 0;
    String ln = LanguageIdent(zl);
    if (ln.Length() < 2)
    {
        if (zl == "*")
            id = static_cast<Word>(GetUserDefaultLCID());
        else
        {
            int i = StrToIntDef(zl, -1);
            if ((i <= 0) || (i > 0xFFFF))
                return Result;
            id = static_cast<Word>(i);
        }
    }
    if ((ln == "US") || (id == 0x0409))
        return Result; // use default US
    void* inst = HInstance;
    HANDLE hFound = FindResource(inst, reinterpret_cast<const TCHAR*>(DZRES_Str), RT_RCDATA);
    if ((hFound == 0) && ModuleIsLib && MainInstance)
    {
        inst = MainInstance;
        hFound = FindResource(inst, reinterpret_cast<const TCHAR*>(DZRES_Str), RT_RCDATA);
    }
    if (hFound)
    {
        try
        {
            Res = new TResourceStream(reinterpret_cast<int>(inst), DZRES_Str, RT_RCDATA);
            if (Res && FindInStream(Res, ln, id))
                if ((SelId = LoadFromStream(SelRes, Res)) > 0)
                {
                    Result = ln;
                    SelName = ln;
                }
        }
        __finally
        {
            delete Res;
        }
    }
    return Result;
}
//---------------------------------------------------------------------------
// get language at index (<0 - default, 0 - current, >0 - index)
String GetZipMsgLanguage(int idx)
{
    return GetZipMsgLanguageInfo(idx, 0);
}
//---------------------------------------------------------------------------
String LocaleInfo(int loc, unsigned info)
{
    if ((loc <= 0) || (loc == 0x400))
        loc = LOCALE_USER_DEFAULT;
    String s;
    s.SetLength(1024);
    GetLocaleInfo((loc & 0xFFFF), info, s.T_STR(), 1023);
    String Result = s.T_STR(); // remove any trailing #0
    return Result;
}
//---------------------------------------------------------------------------
String GetZipMsgLanguageInfo(int Index, unsigned info)
{
    unsigned id = 0x0409;
    String Result = "US: default";  // default US English
    if (!Index)
    {
        if (SelRes.Length)
        {
            Result = SelName;
            id = SelId;
        }
    }
    if (Index > 0)
    {
        TResourceStream* Res = NULL;
        Result = "><";
        id = Index & 0xFF;
        String s;
        try
        {
            if (FindResource(HInstance, reinterpret_cast<const TCHAR*>(DZRES_Str), RT_RCDATA) != 0)
            {
                Res = new TResourceStream(reinterpret_cast<int>(HInstance), DZRES_Str, RT_RCDATA);
                if (Res && IdInStream(Res, id, s))
                    Result = s;
            }
            else
                if (ModuleIsLib && MainInstance &&
                        (FindResource(MainInstance, reinterpret_cast<const TCHAR*>(DZRES_Str), RT_RCDATA) != 0))
                {
                    Res = new TResourceStream(reinterpret_cast<int>(MainInstance), DZRES_Str, RT_RCDATA);
                    if (Res && IdInStream(Res, id, s))
                        Result = s;
                }
        }
        __finally
        {
            delete Res;
        }
    }
    if ((Result != "><") && info)
    {
        if (static_cast<int>(info) == -1)
            Result = "0x" + IntToHex(static_cast<int>(id), 4);
        else
            Result = LocaleInfo(id, info);
    }
    return Result;
}
//---------------------------------------------------------------------------
String FindRes(const TResBlock& block, int id)
{
    TResBlock blk = block; // need a copy to get the adress dereferencing
    String Result;
    int fid = id / 16;
    try
    {
        int bp = 0;
        while ((bp + 9) < blk.High)
        {
            ++bp &= 0x7FFFE;  // dword align
            unsigned DatSiz = blk[bp];
            unsigned HedSiz = blk[bp+2];
            if ((HedSiz + DatSiz) < 8)
                break;
            unsigned sz = (HedSiz + DatSiz) - 8;
            int hp = bp + 4;
            bp += 4 + (sz / 2);
            if (blk[hp] !=  0xFFFF)
                continue;   // bad res type
            if (blk[hp+1] != 6)
                continue;       // not string table
            if (blk[hp+2] != 0xFFFF)
                continue;  // is a named resource
            int rid = blk[hp + 3] - 1;         // id of block
            if (fid != rid)
                continue;           // not the block we want
            rid *= 16;
            hp += (HedSiz - 8) / 2;
            WideString ws;
            unsigned l;
            while (rid < id)
            {
                l = blk[hp];
                hp += l + 1;
                ++rid;
            }
            l = blk[hp];
            if (l)
            {
                ws.SetLength(l);
                memcpy(&ws[1], &blk[hp + 1], l * sizeof(WideChar));
                Result = ws;
                break;
            }
            break;
        }
    }
    catch (...)
    {
        Result = "";
    }
    return Result;
}
#ifndef USE_COMPRESSED_STRINGS
//---------------------------------------------------------------------------
const char* Find(int id)
{
    const char* Result = NULL;
    Word wi = static_cast<Word>(id);
    int ResTableSize = sizeof(ResTable) / sizeof(TZipResRec);
    for (int i = 0; i < ResTableSize; ++i)
    {
        if (ResTable[i].i == wi)
        {
            Result = ResTable[i].s;
            break;
        }
    }
    return Result;
}
//---------------------------------------------------------------------------
String FindConst(int id)
{
    if (id < 10000)
        return "";
    const char* p = Find(id);
    return (p) ? String(p) : String("");
}
#endif
//----------------------------------------------------------------------------
// format is
// id: word, data_size: word, label_size: word, label: char[], data: byte[];... ;0
int LoadCompressedDef(const void* src)
{
    int Result = -1;
    TMemoryStream* ms = NULL;
    Word* pw = const_cast<Word*>(static_cast<const Word*>(src));
    Word w;
    if (*pw == 0x0409)
    {
        w = *++pw;
        w += *++pw;
        w += static_cast<Word>(3 * sizeof(Word));
    }
    try
    {
        ms = new TMemoryStream();
        ms->Write(src, w);
        ms->Position = 0;
//    DefRes.Length = 0; // empty it
//    SelId = 0;
        Result = LoadFromStream(DefRes, ms);
    }
    __finally
    {
        delete ms;
    }
    return Result;
}
//---------------------------------------------------------------------------
String __export ZipLoadStr(int id)
{
    String Result;
    if (SelRes.Length)
        Result = FindRes(SelRes, id);
#ifdef USE_COMPRESSED_STRINGS
    if (Result.IsEmpty())
    {
        if (!DefRes.Length)
            LoadCompressedDef(CompBlock);
        Result = FindRes(DefRes, id);
    }
#else
    if (Result.IsEmpty())
        Result = FindConst(id);
#endif
    TZipStrEvent tmpOnZipStr = OnZipStr;
    if (tmpOnZipStr)
    {
        String d = Result;
        tmpOnZipStr(id, d);
        if (!d.IsEmpty())
            Result = d;
    }
    return (Result.IsEmpty()) ? String("Resource missing for " + IntToStr(id)) : Result;
}
//---------------------------------------------------------------------------
String __export ZipFmtLoadStr(int Ident, const System::TVarRec* Args, const int Args_Size)
{
    String Result = ZipLoadStr(Ident);
    return (!Result.IsEmpty()) ? Format(Result, Args, Args_Size) : Result;
}
//---------------------------------------------------------------------------

