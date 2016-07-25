/*
  ZipIntSFX.h - Header file of internal SFX unit for TZipBuilder
                Code is derived from TZipBuilderSFX component
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

  last modified 2007-02-19
---------------------------------------------------------------------------*/
#ifndef ZipIntSFXH
#define ZipIntSFXH

#ifndef NO_SFX

#include "ZBStructs.h"
#include "CondDefines.h"
#include "Ziputils.h"



bool CALLBACK ResEnumFunc(HANDLE hMod, TCHAR* szType, TCHAR* szName, TStrings* lParam);

namespace Zipintsfx
{
//-- type declarations -------------------------------------------------------

enum SpanSfxType {stSpanSfx, stMVSfx};
enum TSFXOption { soAskCmdLine, soAskFiles, soHideOverWriteBox, soAutoRun, soNoSuccessMsg,
                  soExpandVariables, soInitiallyHideFiles, soForceHideFiles,
                  soCheckAutoRunFileName, soCanBeCancelled, soDetached };
enum TSFXOverwriteMode { somOverwrite, somSkip, somAsk };
typedef Set<TSFXOption, soAskCmdLine, soDetached>  TSFXOptions;
typedef Set<TSFXOption, soExpandVariables, soDetached> TSFXOptionsExt;


#pragma pack(push, 1)
struct TSFXFileHeader
{
	unsigned Signature;
	Word Size;
	TSFXOptions Options;
	TSFXOverwriteMode DefOVW;
	Byte CaptionSize;
	Byte PathSize;
	Byte CmdLineSize;
	Byte RegFailPathSize;
	Byte StartMsgSize;
	unsigned StartMsgType;
} ;

struct TSFXFileEndOfHeader
{
	unsigned Signature;
	unsigned HeaderSize;
} ;

struct TSFXDetachedHeader
{
  unsigned Signature;
  unsigned NameLen; // length of filename without extension of detached archive
  unsigned ExtLen;  // length of file ext without leading .
};
#pragma pack(pop)

#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
    BYTE        bWidth;          // Width, in pixels, of the image
    BYTE        bHeight;         // Height, in pixels, of the image
    BYTE        bColorCount;     // Number of colors in image (0 if >=8bpp)
    BYTE        bReserved;       // Reserved ( must be 0)
    WORD        wPlanes;         // Color Planes
    WORD        wBitCount;       // Bits per pixel
    DWORD       dwBytesInRes;    // How many bytes in this resource?
    DWORD       dwImageOffset;   // Where in the file is this image?
} ICONDIRENTRY;
typedef struct
{
    WORD           idReserved;   // Reserved (must be 0)
    WORD           idType;       // Resource Type (1 for icons)
    WORD           idCount;      // How many images?
    ICONDIRENTRY   idEntries[1]; // An entry for each image (idCount of 'em)
} ICONDIR;
#pragma pack( pop );

#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif
//-----------------------------------------------------------------
class PACKAGE TZipIntSFX : public Classes::TComponent
{

private:
  bool Detached;
  TComponent* FBuilder;
  String FMySFXMessage;
	unsigned FMySFXMessageFlags;
	String FRegFailPath;
	String FSFXLanguage;
	TSFXOptions FMySFXOptions;
  TSFXOverwriteMode FMySFXOverWriteMode;
  TSFXOptionsExt FSFXOptionsExt;
  String FSFXPath;
  bool FUseDelphiBin;
  TStringList* Included_Languages;
  String BinVersion;
  String ResVersion;
	TMemoryStream* SFXBinStream;
  pIRDatE  __fastcall BrowseResDir( pIRD ResStart, pIRD dir, int Depth );
	int __fastcall CheckZipBuilder(void);
	int __fastcall CopyAndAdjustCentralDir(const int InFile, const int OutFile,
                                         const int NumEntries, const int DiskToAdjust,
                                         const int Offset);
	int __fastcall CopyBuffer(const int InFile, const int OutFile, int ReadLen);
	bool __fastcall CreateSFXBinStream(void);
	int __fastcall GetFirstLocalHeader(const int InFile, const int NumEntries,
                                     const int WhichDisk, const int Position);
#ifndef NO_SPAN
  String __fastcall GetLastVolume(const String& FileName,ZipEndOfCentral& EOC);
#endif
	String __fastcall GetSFXLanguage(void);
	String __fastcall GetVersion(void);
  bool __fastcall LoadFromResource(const String& SfxType);
  bool __fastcall LookForBinFile(const String& Sfxtype);
  pIRDatE  __fastcall LookForDirs( const void *SectionData, const DWord SectionVirtualStart,
															const int SectionLen, pIDD directories );
  __int64 __fastcall ReadEOC(const int InFile, ZipEndOfCentral &EOC);
  int __fastcall ReplaceIcon(const int &ExeFile, const int MaxPos, Graphics::TIcon* NewIcon);
  pIRDatE  __fastcall SearchResDirEntry(pIRD ResStart, pIRDirE entry, int Depth);
  void __fastcall SetRegFailPath(const String Value);
  void __fastcall SetSFXLanguage(const String Value);
  void __fastcall SetSFXPath(const String Value);
	void __fastcall SetVersion(const String Value);
  int __fastcall WriteIconToStream(Classes::TStream* Stream, HICON Icon,
                                 int Width, int Height,int depth);
protected:
	int __fastcall DoConvertOrCreateSFX(const String& File, const bool CreateNew);
	void __fastcall GetPropertiesFromZipBuilder(void);
	virtual void __fastcall Notification(Classes::TComponent* aComponent, Classes::TOperation Operation);
	int __fastcall ReadNewSFXProperties(const int InFile, const int MaxPos);
	int __fastcall ReadSFXIcon(const String& File);
	int __fastcall ReadSFXProperties(const int InFile, const int MaxPos);
public:
	__fastcall virtual TZipIntSFX(Classes::TComponent* aOwner);
	__fastcall virtual ~TZipIntSFX(void);
	int __fastcall CreateNewSFX(const String& File);
	int __fastcall ConvertToSFX(void);
#ifndef NO_SPAN
  int __fastcall ConvertToSpanSFX(String OutFilePath);
#endif
	int __fastcall ConvertToZip(void);
__published:
  __property String RegFailPath={read=FRegFailPath, write=SetRegFailPath};
  __property String SFXLanguage={read=GetSFXLanguage, write=SetSFXLanguage};
  __property TSFXOptionsExt SFXOptionsExt={read=FSFXOptionsExt, write=FSFXOptionsExt,nodefault};
  __property String SFXPath = {read=FSFXPath, write=SetSFXPath};
  __property bool UseDelphiBin = {read=FUseDelphiBin, write=FUseDelphiBin, default=false};
	__property String Version = {read=GetVersion, write=SetVersion};
	__property TComponent* ZipBuilder = {read=FBuilder, write=FBuilder};
};

#pragma option -a.
#if __BORLANDC__ < 1344		// BCB4=0x540(1344)
 //-- template instantiations -------------------------------------------------
 template class TSFXOptions;
 template class TSFXOptionsExt;
#endif
}	/* namespace Zipintsfx */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Zipintsfx;
#endif

#endif   // no_sfx

#endif	// ZipIntSFX
