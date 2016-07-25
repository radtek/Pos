/*
  ZipProg.cpp - Source file of progress reporting unit of TZipBuilder
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

#if (__BORLANDC__ < 0x0600)
#include <values.h>
#endif
#include "ZipProg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

const int MAX_PERCENT = MAXINT/10000;
//----------------------------------------------------------------------------
int __fastcall TProgressDetails::GetItemPerCent(void)
{
  if((FItemSize > 0) && (FItemPosition > 0))
  {
    if(FItemSize < MAX_PERCENT) return (100 * FItemPosition) / FItemSize;
    return FItemPosition / (FItemSize / 100);
	}
  return 0;
}
//----------------------------------------------------------------------------
int __fastcall TProgressDetails::GetTotalPerCent(void)
{
  if((FTotalSize > 0) && (FTotalPosition > 0))
    return static_cast<int>((100 * FTotalPosition) / FTotalSize);
  return 0;
}

//----------------------------------------------------------------------------
__fastcall TProgressDetails::~TProgressDetails( void )
{
  FItemName = "";
}
