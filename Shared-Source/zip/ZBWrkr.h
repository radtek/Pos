#ifndef ZBWrkrH
#define ZBWrkrH
/*
  ZBWrkr.h - Header file of woker unit of TZipBuilder
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

  last modified 2009-04-01
---------------------------------------------------------------------------*/

#include "CondDefines.h"
#include "ZBStructs.h"
#include "ZipUtils.h"
#ifdef STATIC_LOAD_DELZIP_DLL
  #include "ZBSBind.h"
#else
  #include "ZBDBind.h"
#endif
#ifndef NO_SFX
#include "ZipIntSFX.h"
#endif

#include "ZBHash.h"
typedef TMsgDlgBtn TPasswordButton;
typedef TMsgDlgButtons TPasswordButtons; //set of TPasswordButton;

//---------------------------------------------------------------------------
//----------------------BCB1, BCB3, BCB4 and BCB5  differences---------------
#if __BORLANDC__ > 1328		// BCB3=0x0530(1328)
#define BINT __int64
#else
#define BINT long
#endif

#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif

union I64Rec
{
  __int64 I;
  struct
  {
  	unsigned Lo;
    unsigned Hi;
  } Rec;
};

struct ZipRenameRec
{
	String			Source;
	String			Dest;
  String      Comment; // ** new
	unsigned long	DateTime;
};
#pragma option -a.

// Definition for if the datadescriptor record is present.
#define DescUsed( DirRec ) (DirRec.Flag & 0x0008)

//---------------------------------------------------------------------------
namespace Zbworker
{

typedef long __stdcall (*GetSHFormatAddress)( HWND hwnd, long drive, long Fid, long Opt );
typedef BOOL __stdcall (*GDiskFreeSpaceEx)( char *RootName, __int64 *FreeForCaller, __int64 *TotNoOfBytes, __int64 *TotNoOfFreeBytes );

#pragma option -b-
enum AddOptsEnum  { AddDirNames, AddRecurseDirs, AddMove, AddFreshen, AddUpdate,AddZipTime,
              AddForceDOS, AddHiddenFiles, AddArchiveOnly, AddResetArchive,
						  AddEncrypt,  AddSeparateDirs, AddVolume, AddFromDate, AddSafe, AddForceDest,
              AddDiskSpan, AddDiskSpanErase };
#pragma option -b.
typedef Set<AddOptsEnum, AddDirNames, AddDiskSpanErase>  AddOpts;
#pragma option -b-
enum AddStoreSuffix{ assGIF, assPNG, assZ, assZIP, assZOO, assARC, assLZH, assARJ, assTAZ,
                     assTGZ, assLHA, assRAR, assACE, assCAB, assGZ, assGZIP, assJAR, assEXE, assEXT,
                    // extra extensions 1.79
                     assJPG, assJPEG, ass7ZP, assMP3, assWMA, assWMV, assDVRMS, assAVI};
#pragma option -b.
typedef Set< AddStoreSuffix, assGIF, assAVI> AddStoreExts;
#pragma option -b-
enum ExtrOptsEnum {ExtrDirNames, ExtrOverWrite, ExtrFreshen, ExtrUpdate, ExtrForceDirs, ExtrTest };
#pragma option -b.
typedef Set<ExtrOptsEnum, ExtrDirNames, ExtrTest>  ExtrOpts;
#pragma option -b-
enum SpanOptsEnum { spNoVolumeName, spCompatName, spWipeFiles, spTryFormat };
#pragma option -b.
typedef Set<SpanOptsEnum, spNoVolumeName, spTryFormat> SpanOpts;
#pragma option -b-
enum OvrOpts {OvrConfirm, OvrAlways, OvrNever};
enum ReplaceOpts {rplConfirm, rplAlways, rplNewer, rplNever};
 // added 1.79.10.07
enum TZBRenameOpts {htrDefault, htrOnce, htrFull};
enum CPOpts	{cpAuto, cpNone, cpOEM};
enum CodePageDirection	{ cpdOEM2ISO, cpdISO2OEM };
enum TZipDelayedItems { zdiList, zdiComment };
enum TZipNumberScheme { znsNone, znsVolume, znsName, znsExt };
#pragma option -b.
typedef Set<TZipDelayedItems, zdiList, zdiComment>  TZipDelays;

#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif

#ifndef NO_STREAM
class __declspec(delphiclass) TZipStream : public TMemoryStream
{
	friend class TZBWorker;

	protected:
		inline __fastcall virtual ~TZipStream( void ) { }
		inline __fastcall virtual TZipStream( void ) {Clear(); }
	public:
		void __fastcall virtual SetPointer( void *Ptr, int Size );
};
#endif

// 1.79 10 Mar 2009 RP changed for setting new comment
// 1.75 18 Feb 2004 RP changed for >2G
class TMDZipData       // MyDirZipData
{
private:
  unsigned long	FComprSize;
  unsigned long	FCRC32;
  int	FDateTime;
	unsigned short FDiskStart;
  AnsiString FFileNameA;
  AnsiString FNComment;
	__int64 FRelOffLocal;
  unsigned long	FUnComprSize;
  Word FVerMade;
  String __fastcall GetFileName(void);
  void __fastcall SetFileName(const String& Value);
public:
	void __fastcall SetName(const char* src, int len);
  __property unsigned long ComprSize={read=FComprSize,write=FComprSize};
  __property unsigned long CRC32={read=FCRC32,write=FCRC32};
  __property int DateTime={read=FDateTime,write=FDateTime};
  __property unsigned short DiskStart={read=FDiskStart,write=FDiskStart};
  __property String FileName={read=GetFileName,write=SetFileName};
  __property AnsiString FileNameA={read=FFileNameA,write=FFileNameA};
  __property AnsiString NComment={read=FNComment,write=FNComment};
  __property __int64 RelOffLocal={read=FRelOffLocal,write=FRelOffLocal};
  __property unsigned long UnComprSize={read=FUnComprSize,write=FUnComprSize};
  __property Word VerMade={read=FVerMade,write=FVerMade};
};

class TMZipDataList : public TList
{
	private:
		TMDZipData* GetItems(int Index);
	public:
		__fastcall TMZipDataList(int TotalEntries);
		__fastcall ~TMZipDataList(void);
		__property TMDZipData* Items[int Index]={read=GetItems};
		int IndexOf(const String& fname);
    int IndexOf(const char* hname, int len);
};

#ifdef STATIC_LOAD_DELZIP_DLL
  class __declspec(delphiclass) TZBWorker : public TZBStaticBind
#else
  class __declspec(delphiclass) TZBWorker : public TZBDynamicBind
#endif
{
	friend class TSHFormatThread;

typedef Zbcore::TZBCore inherited;
	private:
		int	FAddCompLevel;
    AddOpts FAddOptions;
    AddStoreExts FAddStoreSuffixes;
    int FBufSize;
    CPOpts FCodePage;
    bool FConfirmErase;
    int	FDateStamp;
    TZipDelays FDelaying;
    int	FDirOnlyCount;
    __int64 FDiskFree;
    int	FDiskNr;
#ifndef NO_SPAN
		unsigned FDiskWritten;
#endif
		String FDrive;
    bool FDriveFixed;
#ifndef NO_SPAN
    int	 FDriveNr;
#endif
    bool FEncrypt;
    String FEOCComment;
    String FExtAddStoreSuffixes;
    String FExtrBaseDir;
    ExtrOpts FExtrOptions;
    unsigned FFreeOnAllDisks;
    __int64 FFreeOnDisk;
    unsigned FFreeOnDisk1;
		unsigned FFileSize;
    TDateTime FFromDate;
    TStrings* FFSpecArgs;
    TStrings* FFSpecArgsExcl;
    HWND FHandle;
    unsigned char	FHostNum;
		unsigned char	FHostVer;
    TDeleteOpts FHowToDelete;
    String FInFileName;
    int	FInFileHandle;
    int FMaxVolumeSize;
    int	FMinFreeVolSize;
    TZipDirHashList* FDirHashs;
#ifndef NO_SPAN
    bool FNewDisk;
#endif
    TZipNumberScheme FNumbering;
    bool FIsSpanned;
    int	FOutFileHandle;
#ifndef NO_SPAN
    String FOutFileName;
#endif
    unsigned FRealFileSize;
    String FRootDir;
    String FPassword;
    unsigned FPasswordReqCount;
    unsigned FSFXOffset;
#ifndef NO_SFX
    // internal sfx class
    TZipIntSFX* FSFXSlave;
#endif
    TZipShowProgress FShowProgress;
    __int64 FSizeOfDisk;
    SpanOpts FSpanOptions;
    RawByteString FStoredExtraData;
    unsigned FZipSOC;
    int FSuccessCnt;
    unsigned short FTotalDisks;
    bool FUseDirOnlyEntries;
    bool FUnzBusy;
    String FVolumeName;
    bool FVolumeReadOnly;      // new 1.79
    bool FWrongZipStruct;
    bool FZipBusy;
    String FZipComment;
    Classes::TList* FZipContents;
#ifndef NO_SPAN
    TZipDiskStatus	FZipDiskStatus;
    TZipDiskAction	FZipDiskAction;
#endif
    unsigned FZipEOC;
    String FZipFileName;
#ifndef NO_STREAM
    class TZipStream	*FZipStream;
#endif
	String __fastcall AddStoreExtStr(const AddStoreExts SufOption);
#ifndef NO_SPAN
    void __fastcall CheckForDisk(bool Writing);
    void __fastcall ClearFloppy(String dir);
    void __fastcall CreateMVFileName(String& FileName,bool StripPartNbr, bool Compat);
#endif
    String __fastcall ConvertOEM(const String Source, CodePageDirection Direction);
		int    __fastcall CopyBuffer(int InFile, int OutFile, __int64 ReadLen);
    void   __fastcall DiskFreeAndSize(int Action );
    void __fastcall DllCallBack(PZCallBackStruct cbr);
    void   __fastcall ExtAdd(int UseStream, DWORD StrFileDate, DWORD StrFileAttr,
                                TMemoryStream *MemStream);
    void   __fastcall ExtExtract(int UseStream, TMemoryStream *MemStream );
    String __fastcall ExtMsg(String CMsg, PZCallBackStruct cbr);
    void __fastcall FreeZipDirEntryRecords( void );
		int    __fastcall GetCount( void );
		ZipDirEntry* __fastcall GetDirEntry(int idx);
		bool __fastcall GetDriveProps(void);
#ifndef NO_SPAN
		 int __fastcall GetLastVolume(String FileName,ZipEndOfCentral &EOC, bool AllowNotExists);
#endif
		void   __fastcall GetNewDisk( int DiskSeq );
		String __fastcall GetZipComment( void );
	  bool __fastcall HasExtendedChars(const String& FileName);
    bool __fastcall CheckDestWritable(const String& fname, bool NoExc);
	  bool __fastcall IsFixedDrive(const String& drv);
	  bool __fastcall IsPathOnly(const String& f);
#ifndef NO_SPAN
		bool   __fastcall IsRightDisk(void);
		String __fastcall MakeString(char* Buffer, int Size);
		String __fastcall NameOfPart(const String& fn, bool compat);
#endif
		bool __fastcall OpenEOC(ZipEndOfCentral &EOC, bool DoExcept );
 		void __fastcall ReadJoin(void *Buffer, int BufSize, int DSErrIdent);
    void __fastcall RecreateMVArchive(const String& TmpZipName, bool Recreate);
    int __fastcall RejoinMVArchive(String& TmpZipName, bool Rejoin);
    void __fastcall RemoveDriveSpec(String& FullPathName);
#ifndef NO_SPAN
    void   __fastcall RWJoinData(char* Buffer, unsigned ReadLen, int DSErrIdent );
    void   __fastcall RWSplitData(char* Buffer, unsigned ReadLen, int DSErrIdent );
#endif
   void __fastcall SetActive(bool Value);
   void __fastcall SetCancel(bool Value);
   void __fastcall SetDeleteSwitches(void);
   void __fastcall SetDirHashs(const TZipDirHashList* Value);
   void __fastcall SetFileName(const String Value );
   void __fastcall SetExtAddStoreSuffixes(const String Value);
   void __fastcall SetPasswordReqCount(unsigned Value);
   void __fastcall SetUnZipSwitches(String& NameOfZipFile, pUnZipParms uzParms);
   void __fastcall Setup_extend(void);
   void __fastcall SetZipComment(const String Value );
   void __fastcall SetZipSwitches(String& NameOfZipFile, PZipParms zpParms);
   void __fastcall WriteJoin(const void* Buffer, unsigned BufSize, int DSErrIdent );
#ifndef NO_SPAN
   void   __fastcall WriteSplit(const void* Buffer, unsigned Len, unsigned MinSize );
   int  __fastcall ZipFormat(void );
#endif

protected:
    static int __stdcall ZCallback(void* Caller,PZCallBackStruct cbr );

public:
		__fastcall TZBWorker(TObject* ABuilder);
		__fastcall virtual ~TZBWorker( void );
		int __fastcall Add(void);
#ifndef NO_STREAM
    int   __fastcall AddStreamToFile(const String& FileName, DWORD FileDate, DWORD FileAttr);
    TZipStream * __fastcall AddStreamToStream(TMemoryStream *InStream );
#endif
    virtual void __fastcall BeforeDestruction(void);
    void __fastcall Clear(void);
#ifndef NO_SFX
		int    __fastcall ConvertSFX( void );
		int    __fastcall ConvertZIP( void );
#endif
    int    __fastcall CopyZippedFiles(TZBWorker *DestZipBuilder, bool DeleteFromSource, OvrOpts OverwriteDest );
    int    __fastcall Copy_File(const String src, const String dest );
    int __fastcall Delete(void);
    virtual void __fastcall Done(void);
  	int __fastcall Extract(void);
#ifndef NO_STREAM
    TZipStream * __fastcall ExtractFileToStream(const String& FileName);
		TZipStream * __fastcall ExtractStreamToStream(TMemoryStream *InStream, unsigned long OutSize = 32768 );
#endif
		String __fastcall GetAddPassword(TPasswordButton& Response);
		String __fastcall GetExtrPassword( TPasswordButton& Response );
		TPasswordButton __fastcall GetPassword(const String& DialogCaption,
                                           const String& MsgTxt,
                                           TPasswordButtons pwButtons,
		                                       String& ResultStr );
    TPasswordButton __fastcall GetPassword(const String& DialogCaption,
                                           const String& MsgTxt, int ctx,
                                           TPasswordButtons pwb, String& ResultStr);
#ifndef NO_SFX
		int    __fastcall IsZipSFX(const String& SFXExeName );
#endif
		int  __fastcall List(void);
#ifndef NO_SPAN
		int    __fastcall ReadSpan(const String InFileName, String& OutFilePath, bool UseXprogress );
#endif
// changed 1.79.10.07
		int __fastcall Rename( TList &RenameList, int DateTime, TZBRenameOpts How);
    virtual void __fastcall Starting(void);
#ifndef NO_SPAN
		int    __fastcall WriteSpan(const String InFileName, const String OutFileName, bool UseXprogress);
#endif
    String __fastcall ZipDllPath(void);
    __property bool Active = {read=FActive, write=SetActive, nodefault};
    __property int AddCompLevel={read=FAddCompLevel, write= FAddCompLevel};
    __property TDate AddFrom={read=FFromDate,write=FFromDate};
    __property AddOpts AddOptions={ read= FAddOptions,write=FAddOptions};
    __property AddStoreExts AddStoreSuffixes={read=FAddStoreSuffixes,write=FAddStoreSuffixes};
    __property int BufSize={read=FBufSize, write=FBufSize, default=65535};
    __property CPOpts CodePage={read=FCodePage,write=FCodePage}; // default cpAuto;
#ifndef NO_SPAN
    __property bool ConfirmErase={read=FConfirmErase,write=FConfirmErase};
#endif
    __property int Count = {read=GetCount, nodefault};
		__property ZipDirEntry* DirEntry[int idx] = {read=GetDirEntry};
    __property TZipDirHashList* DirHashs={read=FDirHashs,write=SetDirHashs};
		__property int DirOnlyCount	= {read=FDirOnlyCount,   default=0};
		__property String ExtAddStoreSuffixes = {read=FExtAddStoreSuffixes,
                                        write=SetExtAddStoreSuffixes, nodefault};
		__property String ExtrBaseDir={read=FExtrBaseDir,write=FExtrBaseDir};
    __property ExtrOpts ExtrOptions={read=FExtrOptions,write=FExtrOptions};
		__property TStrings *FSpecArgs ={read=FFSpecArgs,write=FFSpecArgs};
    __property TStrings *FSpecArgsExcl ={read=FFSpecArgsExcl, write=FFSpecArgsExcl};
		__property HWND Handle = {read=FHandle, write=FHandle, nodefault};
		__property TDeleteOpts HowToDelete = {read=FHowToDelete, write=FHowToDelete};
		__property bool	IsSpanned	= {read=FIsSpanned, nodefault};
#ifndef NO_SPAN
		 __property unsigned KeepFreeOnAllDisks={read=FFreeOnAllDisks,write=FFreeOnAllDisks};
		 __property unsigned KeepFreeOnDisk1={read=FFreeOnDisk1,write=FFreeOnDisk1};
		 __property int MaxVolumeSize={ read= FMaxVolumeSize, write= FMaxVolumeSize};
     __property int MinFreeVolumeSize={read=FMinFreeVolSize,write=FMinFreeVolSize,default=65536};
#endif
		__property String	Password = {read=FPassword, write=FPassword, nodefault};
		__property unsigned PasswordReqCount={read=FPasswordReqCount,write=SetPasswordReqCount};
		__property String RootDir={read=FRootDir,write=FRootDir};
#ifndef NO_SPAN
		__property SpanOpts SpanOptions={ read=FSpanOptions,write=FSpanOptions};
#endif
		__property unsigned SFXOffset	= {read=FSFXOffset, nodefault};
#ifndef NO_SFX
    __property TZipIntSFX* SFXSlave={read=FSFXSlave,write=FSFXSlave};
#endif
		__property int SuccessCnt = {read=FSuccessCnt, nodefault};
		__property BINT  TotalSizeToProcess = {read=GetTotalSizeToProcess,    nodefault};
		__property bool UseDirOnlyEntries={read=FUseDirOnlyEntries,write=FUseDirOnlyEntries, default=false};
		__property bool UnzBusy = {read=FUnzBusy, nodefault};
		__property bool ZipBusy = {read=FZipBusy, nodefault};
		__property String ZipComment = {read=GetZipComment,   write=SetZipComment, nodefault};
		__property Classes::TList *ZipContents = {read=FZipContents};
		__property unsigned	ZipEOC = {read=FZipEOC,	nodefault};
		__property String ZipFileName = {read=FZipFileName, write=SetFileName, nodefault};
		__property unsigned	ZipFileSize	= {read=FRealFileSize,	nodefault};
#ifndef NO_STREAM
		__property TZipStream	*ZipStream = {read=FZipStream, nodefault};
#endif
		__property unsigned	ZipSOC = {read=FZipSOC,	nodefault};
};

#pragma option -a.

#if __BORLANDC__ < 1344		// BCB4=0x540(1344)
 //-- template instantiations -------------------------------------------------
 template class ExtrOpts;
 template class AddOpts;
 template class AddStoreExts;
 template class SpanOpts;
 template class TZipDelays;
#endif
}	/* EOF namespace Zbworker */

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
	using namespace Zbworker;
#endif


#endif



