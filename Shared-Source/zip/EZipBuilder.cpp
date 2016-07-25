/*
  EZipBuilder.cpp - Source file of unit for exception handling in TZipBuilder
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 20057  Roger Aelbrecht

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

  last modified 2007-02-26
---------------------------------------------------------------------------*/
#include <vcl.h>
#pragma hdrstop

#include "EZipBuilder.h"
#include "ZipMsg.h"
#include "ZBMsgStr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

const String ERRORMSG = "Failed to Locate string";
/*WinErr----------------------------------------------------------------------------------------------------------------------
1.79 new function to format systemerror messages
*/
String __fastcall WinErr(int err)
{
  if(err > 0) return Format("  <%X : %s>", ARRAYOFCONST((err, SysErrorMessage(err))));
  else return "";
}
/* EZipBuilder::EZipBuilder----------------------------------------------------------------------------------------------------------------------
1.76 new
*/
__fastcall EZipBuilder::EZipBuilder(const String& Msg)
                      : Sysutils::Exception(Msg)
{
  FDisplayMsg = true;
  FResIdent = DS_UnknownError;
}
/* EZipBuilder::EZipBuilder------------------------------------------------------------------------
1.77 new
*/
__fastcall EZipBuilder::EZipBuilder(const String& Msg, const bool Display)
                      : Sysutils::Exception(Msg)
{
  FDisplayMsg = Display;
}

/* EZipBuilder::EZipBuilder----------------------------------------------------------------------------------------------------------------------
1.76 convert strings in constructor. The default exception constructor used.
*/
__fastcall EZipBuilder::EZipBuilder(int Ident, bool Display)
                                     : Sysutils::Exception(ZipLoadStr(Ident))
{
	if(Message.IsEmpty()) Message = ERRORMSG;
  FResIdent = Ident;
  FDisplayMsg = Display;
}

/* EZipBuilder::EZipBuilder---------------------------------------------------------------------------
1.76 convert strings in constructor. The default exception constructor used.
*/
__fastcall EZipBuilder::EZipBuilder(int Ident, const int DiskNo)
                                          : Sysutils::Exception(ZipLoadStr(Ident))
{
	if(Message.IsEmpty()) Message = ERRORMSG;
  else Message = Format(Message, ARRAYOFCONST((DiskNo)));
  FResIdent = Ident;
  FDisplayMsg = true;
}
/* EZipBuilder::EZipBuilder---------------------------------------------------------------------------
1.76 convert strings in constructor. The default exception constructor used.
*/
__fastcall EZipBuilder::EZipBuilder(int Ident, const String& Drive)
                                            : Sysutils::Exception(ZipLoadStr(Ident))
{
	if(Message.IsEmpty()) Message = ERRORMSG;
  else Message = Format(Message, ARRAYOFCONST((Drive)));
  FDisplayMsg = true;
  FResIdent = Ident;
}
/* EZipBuilder::EZipBuilder---------------------------------------------------------------------------
1.76 convert strings in constructor. The default exception constructor used.
*/
__fastcall EZipBuilder::EZipBuilder(int Ident, const String& File1, const String& File2)
                                            : Sysutils::Exception(ZipLoadStr(Ident))
{
	if(Message.IsEmpty()) Message = ERRORMSG;
  else Message = Format(Message, ARRAYOFCONST((File1, File2)));
  FDisplayMsg = true;
  FResIdent = Ident;
}

/* EZipBuilder::EZipBuilder----------------------------------------------------------------------------------------------------------------------
1.76 convert strings in constructor. The default exception constructor used.
*/
__fastcall EZipBuilder::EZipBuilder(int Ident, const TVarRec* Args, const int Args_Size)
                                : Sysutils::Exception(ZipFmtLoadStr(Ident, Args, Args_Size))
{
  if(Message.IsEmpty()) Message = ERRORMSG;
  else Message = Format(Message, Args, Args_Size);
  FResIdent = Ident;
  FDisplayMsg = true;
}

/* EZipAbort::EZipAbort---------------------------------------------------------
1.79 4 Aug 2005 RA Don't show message in Abort , if show is needed use EZipBuilder exception
 new 1.73 (17 May 2003)
 exception handler for Abort exceptions
*/
__fastcall EZipAbort::EZipAbort(void) : EZipBuilder(DS_Canceled, false)
{
}
// EZipAbort::EZipAbort

__fastcall EZipBuilder::EZipBuilder(int Ident, const String& Msg, int eno)
                                : Sysutils::Exception(ZipLoadStr(Ident))
{
	if(Message.IsEmpty()) Message = ERRORMSG;
  else Message = Format(Message, ARRAYOFCONST((Msg)));
  Message += WinErr(eno);
  FDisplayMsg = true;
  FResIdent = Ident;
}
