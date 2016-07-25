/*
  ZipCtx.h - Header file with dialogBox help context values
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
#ifndef ZipCtxH
#define ZipCtxH

#define DHCBase            10000
#define DHC_ZipMessage 		DHCBase
#define DHC_ExMessage   	DHCBase + 1
#define DHC_Password    	DHCBase + 2 // just GetPassword default password
#define DHC_ExtrPwrd  		DHCBase + 3
#define DHC_AddPwrd1  		DHCBase + 4
#define DHC_AddPwrd2  		DHCBase + 5
#define DHC_WrtSpnDel  		DHCBase + 6
#define DHC_ExSFX2EXE  		DHCBase + 7
#define DHC_ExSFX2Zip  		DHCBase + 8
#define DHC_SpanNxtW  		DHCBase + 9
#define DHC_SpanNxtR  		DHCBase + 10  
#define DHC_SpanOvr  			DHCBase + 11
#define DHC_SpanNoOut  		DHCBase + 12
#define DHC_SpanSpace  		DHCBase + 13
#define DHC_CpyZipOvr  		DHCBase + 14
#define DHC_FormErase  		DHCBase + 15
#define DHC_ExSFXNew      DHCBase + 16


#endif
