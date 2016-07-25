/*
  ZipProg.h - Header file of progress reporting unit of TZipBuilder
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
#ifndef ZipProgH
#define ZipProgH
//---------------------------------------------------------------------------
#ifdef KEEP_BACKWARD
  #ifdef ALLOW_2G
    #define TProgressSize unsigned     // - will allow uncompressed size to almost 4G
  #else
    #define TProgressSize int          // - will allow uncompressed size to almost 2G
  #endif
#else
  #define TProgressSize __int64
#endif

enum ProgressType {NewFile, ProgressUpdate, EndOfBatch, TotalFiles2Process,
    								 TotalSize2Process, NewExtra, ExtraUpdate };

namespace Zipprogress
{


#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif
class PACKAGE TProgressDetails : public TObject
{
protected:
  int FItemCount;
  String FItemName;
  int FItemNumber;
  unsigned FItemPosition;
  unsigned FItemSize;
  __int64 FTotalPosition;
  __int64 FTotalSize;
  __int64 FWritten;
  ProgressType FProgType;
  int __fastcall GetItemPerCent(void);
  int __fastcall GetTotalPerCent(void);
public:
    __fastcall TProgressDetails(void) {};
		__fastcall virtual ~TProgressDetails(void);
    __property __int64 BytesWritten = {read=FWritten};
    __property String ItemName = {read=FItemName};
    __property int ItemNumber = {read=FItemNumber};
    __property int ItemPerCent ={read=GetItemPerCent};
    __property unsigned ItemPosition = {read=FItemPosition};
    __property unsigned ItemSize = {read=FItemSize}; 
    __property ProgressType Order = {read=FProgType};
    __property int TotalCount = {read=FItemCount};
    __property int TotalPerCent ={read=GetTotalPerCent};
    __property __int64 TotalPosition = {read=FTotalPosition};
    __property __int64 TotalSize = {read=FTotalSize};
};

#pragma option -a.


}	/* namespace Zipprogress */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Zipprogress;
#endif

#endif

