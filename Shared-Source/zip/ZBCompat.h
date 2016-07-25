/*
  ZBCompat.h - Types and utility functions required for some compilers
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

#ifndef ZBCompatH
#define ZBCompatH
//---------------------------------------------------------------------------
#include <vcl.h>

#if __BORLANDC__ < 1328		// BCB1=0x0520(1312)
#	// BCB_1 doesn't know 'PACKAGE'
#	define PACKAGE
namespace Sysutils
{
	extern bool __fastcall DeleteFile( const System::AnsiString aStr );
}
#	// The old style Out of Memory exception.
#	include <except.h>
#	define MEMEXCEPT	xalloc
#else								// BCB3=0x0530(1328)
#	// xalloc is obsolete and not used in BCB3.
#	include <new.h>
#	define MEMEXCEPT	std::bad_alloc
#endif

#if (__BORLANDC__ >= 0x0600)
    typedef unsigned __int64 ZU64;
    #define T_STR t_str
    #define POS Pos
    #define COMPARE Compare
#else
	#if __BORLANDC__ <= 0x0560
		typedef AnsiString UTF8String;
    typedef AnsiString RawByteString;
	#else
    typedef AnsiString RawByteString;
  #endif
	#if (__BORLANDC__ >= 0x0560)
		typedef unsigned __int64 ZU64;
	#else
		typedef __int64 ZU64;
	#endif

  bool CharInSet(Byte C, const TSysCharSet CharSet);
  #define T_STR c_str
  #define POS AnsiPos
  #define COMPARE AnsiCompare


#endif


AnsiChar* MakeStrP(const String& str);

#endif
