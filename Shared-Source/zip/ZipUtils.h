/*
  ZipUtils.h - Header file for common utilities for TZipBuilder
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

  last modified 2005-02-06
---------------------------------------------------------------------------*/
#ifndef ZipUtilsH
#define ZipUtilsH
#include <vcl.h>

// c++ replacement of macros max and min
template<class T>
  inline const T& min(const T& a, const T& b) { return a < b ? a : b; }

template<class T>
  inline const T& max(const T& a, const T& b) { return a > b ? a : b; }

//---------------------------------------------------------------------------
           // QueryZip return bit values and errors
const int zqbStartEXE =     1;  // is EXE file may be SFX
const int zqbStartLocal =   2;  // normal zip file start
const int zqbStartSpan =    4;  // first part of span
const int zqbStartCentral = 8;  // continuing Central Header
const int zqbGoodComment = 16;  // comment length good (no junk at end)
const int zqbHasLocal =    32;  // first Central entry points to local header
const int zqbHasCentral =  64;  // Central entry where it should be
const int zqbHasEOC =     128;  // End of Central entry
const int zqFileError =    -7;  // file handling error
const int zqGeneralError = -9;  // unspecified failure

enum TDeleteOpts	{htdFinal, htdAllowUndo};
enum TPathSlashDirection {psdExternal, psdInternal};

void CloseFileHandle(int& FileHandle);
String DelimitPath(const String path, bool sep=true);
bool DirExists( const String Name );
int EraseFile(const String Fname, TDeleteOpts How);
bool FileNameMatch(const String Pat, const String Subj);
String FileVersion(const String fname);
bool ForceDirectory(const String Dir);
String GetVolumeLabel(const String drive);
bool IsSameFile(const String Fname1, const String Fname2);
bool IsWild(const String fspec);
bool IsWinXP(void);
String PathConcat(const String path,const String extra);
int QueryZip(const String Name);
String SetSlash(const String path, TPathSlashDirection dir);

#endif

