/*
  EZipBuilder.h - Header file of unit for exception handling in TZipBuilder
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2007  Roger Aelbrecht

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

  last modified 2007-02-22
---------------------------------------------------------------------------*/
#ifndef EZipBuilderH
#define EZipBuilderH

//---------------------------------------------------------------------------
String __fastcall WinErr(int err);
namespace Ezipbuilder
{
#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif

 // new version 1.76
class PACKAGE EZipBuilder : public Sysutils::Exception
{
public:
  bool FDisplayMsg;	// We do not always want to see a message after an exception.
	int  FResIdent;	// We also save the Resource ID in case ZipMsg is not linked in the application.

  __fastcall EZipBuilder(const String& Msg);
  __fastcall EZipBuilder(const String& Msg, const bool Display);

	__fastcall EZipBuilder(int Ident, bool Display = true);
	__fastcall EZipBuilder(int Ident, const int DiskNo );
	__fastcall EZipBuilder(int Ident, const String& Drive );
	__fastcall EZipBuilder(int Ident, const String& File1, const String& File2 );
  __fastcall EZipBuilder(int Ident, const TVarRec* Args, const int Args_Size);
  __fastcall EZipBuilder(int Ident, const String& Msg, int eno);
	__fastcall virtual ~EZipBuilder(void) {}
	__property int Ident = {read=FResIdent, nodefault};

};
// new 1.73 handling of abort exceptions
class PACKAGE EZipAbort : public EZipBuilder
{
  public:
     __fastcall EZipAbort(void);
};
#pragma option -a.

}	/* namespace Ezipbuilder */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Ezipbuilder;
#endif

#endif

