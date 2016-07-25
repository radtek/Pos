#ifndef ZipBuilderH
#define ZipBuilderH
/*
  ZipBuilder.h - Header file of main unit of TZipBuilder
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

  last modified 2009-05-18
---------------------------------------------------------------------------*/
#include "CondDefines.h"
#include "ZBWrkr.h"
#include "EZipBuilder.h"

//---------------------------------------------------------------------------
#if __BORLANDC__ < 1328		// BCB1=0x0520(1312)
#	// BCB_1 doesn't know 'PACKAGE'
#	define PACKAGE
#endif

const int BUSY_ERROR = -255;
const int Reentry_Error = 0x4000000;
const int DefSuffixes = assGIF + assPNG + assZ + assZIP + assZOO + assARC
														+ assLZH + assARJ + assTAZ + assTGZ + assLHA
														+ assRAR + assACE + assCAB + assGZ + assGZIP + assJAR + assEXE
                            + assJPG + assJPEG + ass7ZP + assMP3 + assWMA + assWMV
                            + assDVRMS + assAVI;
namespace ZipBuilder
{

#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif

#pragma option -b-
enum SFXOptsEnum  { SFXAskCmdLine, SFXAskFiles, SFXAutoRun, SFXHideOverWriteBox,
						  SFXCheckSize, SFXNoSuccessMsg };
#pragma option -b.
typedef Set<SFXOptsEnum, SFXAskCmdLine, SFXNoSuccessMsg> SfxOpts;

class PACKAGE TCustomZipBuilder : public Classes::TComponent
{
typedef Classes::TComponent inherited;
#ifndef NO_SFX
  friend class TZipIntSFX;
#endif
private:
	int FActive;
	int	FAddCompLevel;
 	AddOpts	FAddOptions;
   AddStoreExts FAddStoreSuffixes;
   int FBufSize;
   bool FBusy;        // true = busy
   CPOpts FCodePage;
   bool FConfirmErase;
   String FDLLDirectory;  
   bool FDllLoad;
   bool FEncrypt;
    //events
  String FExtAddStoreSuffixes;
  // private version of property variables
  String FExtrBaseDir;
  ExtrOpts FExtrOptions;
  unsigned FFreeOnAllDisks;
  unsigned FFreeOnDisk1;
  TDateTime FFromDate;
  TStrings* FFSpecArgs;
  TStrings* FFSpecArgsExcl;
  HWND FHandle;
  TDeleteOpts FHowToDelete;
  String FLanguage;
  int FMaxVolumeSize;
  int FMinFreeVolSize;
  bool FNotMainThread;
  String FPassword;
  unsigned FPasswordReqCount;
  bool FReentry;     // true = re-entry attempted
  String FRootDir;
#ifndef NO_SFX
  String	FSFXCaption;				// dflt = "Self-extracting Archive"
	String	FSFXCommandLine;	// dflt = ""
	String	FSFXDefaultDir;		// dflt = ""
	TIcon*	FSFXIcon;
	String	FSFXMessage;
	SfxOpts	FSFXOptions;
	OvrOpts	FSFXOverWriteMode;	// OvrConfirm  (others: OvrAlways, OvrNever)
  TZipIntSFX* FSFXSlave ;
#endif
  SpanOpts FSpanOptions;
  String FTempDir;
  bool FTrace;
  bool FUnattended;
  bool FUseDirOnlyEntries;
  bool FVerbose;
  TZBWorker* FZip;
    // Property get/set functions
  bool __fastcall GetActive(void);
  bool __fastcall GetBusy(void);
  int __fastcall GetBuild(void);
  ZipDirEntry* __fastcall GetEntry(const String Name);
  bool __fastcall GetCancel(void);
  int __fastcall GetCount(void);
  ZipDirEntry* __fastcall GetDirEntry(int idx);
  int __fastcall GetDirOnlyCount(void);
  int __fastcall GetDll_Build(void);
  bool __fastcall GetDll_Load(void);
  String __fastcall GetDll_Path(void);
  String __fastcall GetDll_Version(void);
  int __fastcall GetErrCode(void);
  String __fastcall GetErrMessage(void);
  int __fastcall GetFullErrCode(void);
  bool __fastcall GetIsSpanned(void);
  String __fastcall GetLanguage(void);
  String __fastcall GetLanguageInfo(int index, unsigned info);
  TCheckTerminateEvent __fastcall GetOnCheckTerminate(void);
  TCopyZipOverwriteEvent __fastcall GetOnCopyZipOverwrite(void);
  TCRC32ErrorEvent __fastcall GetOnCRC32Error(void);
  TNotifyEvent __fastcall GetOnDirUpdate(void);
  TExtractOverwriteEvent __fastcall GetOnExtractOverwrite(void);
  TExtractSkippedEvent __fastcall GetOnExtractSkipped(void);
  TFileCommentEvent __fastcall GetOnFileComment(void);
  TFileExtraEvent __fastcall GetOnFileExtra(void);
#ifndef NO_SPAN
  TGetNextDiskEvent __fastcall GetOnGetNextDisk(void);
#endif
  TItemProgressEvent __fastcall GetOnItemProgress(void);
  TMsgEvent __fastcall GetOnMessage(void);
  TNewNameEvent __fastcall GetOnNewName(void);
  TPasswordErrorEvent __fastcall GetOnPasswordError(void);
  Zbcore::TProgressEvent __fastcall GetOnProgress(void);
  TProgressDetailsEvent __fastcall GetOnProgressDetails(void);
#ifndef NO_SPAN
  TStatusDiskEvent __fastcall GetOnStatusDisk(void);
#endif
  TSetAddNameEvent __fastcall GetOnSetAddName(void);
  TSetExtNameEvent __fastcall GetOnSetExtName(void);
  TSetNewNameEvent __fastcall GetOnSetNewName(void);
  TTickEvent __fastcall GetOnTick(void);
  TTotalProgressEvent __fastcall GetOnTotalProgress(void);
  TZipDialogEvent __fastcall GetOnZipDialog(void);
  String __fastcall GetPPassword(void);
  unsigned __fastcall GetRealFileSize(void);
#ifndef NO_SFX
  String __fastcall GetSFXPath(void);
  String __fastcall GetSFXLanguage(void);
#endif
  int __fastcall GetSFXOffset(void);
  int __fastcall GetSuccessCnt(void);
  __int64 __fastcall GetTotalSizeToProcess(void);
  bool __fastcall GetUnzBusy(void);
  String __fastcall GetVersion(void);
  bool __fastcall GetZipBusy(void);
  String __fastcall GetZipComment(void);
  TList* __fastcall GetZipContents(void);
  unsigned __fastcall GetZipEOC(void);
  String __fastcall GetZipFileName(void);
  unsigned __fastcall GetZipSOC(void);
#ifndef NO_STREAM
  TZipStream* __fastcall GetZipStream(void);
#endif
#ifdef KEEP_BACKWARD
  int __fastcall GetUnzVers(void);
  int __fastcall GetZipVers(void);
#endif
  void __fastcall SetActive(bool Value);
  void __fastcall SetCancel(bool Value);
  void __fastcall SetDll_Load(const bool Value);
  void __fastcall SetErrCode(int Value);
  void __fastcall SetFileName(const String Value);
  void __fastcall SetFSpecArgs(const TStrings* Value);
  void __fastcall SetFSpecArgsExcl(const TStrings* Value);
  void __fastcall SetLanguage(const String Value);
  void __fastcall SetOnCheckTerminate(TCheckTerminateEvent Value);
  void __fastcall SetOnCopyZipOverwrite(TCopyZipOverwriteEvent Value);
  void __fastcall SetOnCRC32Error(TCRC32ErrorEvent Value);
  void __fastcall SetOnDirUpdate(TNotifyEvent	Value);
  void __fastcall SetOnExtractOverwrite(TExtractOverwriteEvent Value);
  void __fastcall SetOnExtractSkipped(TExtractSkippedEvent Value);
  void __fastcall SetOnFileComment(TFileCommentEvent Value);
  void __fastcall SetOnFileExtra(TFileExtraEvent Value);
#ifndef NO_SPAN
  void __fastcall SetOnGetNextDisk(TGetNextDiskEvent Value);
#endif
  void __fastcall SetOnItemProgress(TItemProgressEvent Value);
  void __fastcall SetOnMessage(TMsgEvent Value);
  void __fastcall SetOnNewName(TNewNameEvent Value);
  void __fastcall SetOnPasswordError(TPasswordErrorEvent Value);
  void __fastcall SetOnProgress(Zbcore::TProgressEvent Value);
  void __fastcall SetOnProgressDetails(TProgressDetailsEvent Value);
#ifndef NO_SPAN
  void __fastcall SetOnStatusDisk(TStatusDiskEvent Value);
#endif
  void __fastcall SetOnSetAddName(TSetAddNameEvent Value);
  void __fastcall SetOnSetExtName(TSetExtNameEvent Value);
  void __fastcall SetOnSetNewName(TSetNewNameEvent Value);
  void __fastcall SetOnTick(TTickEvent Value);
  void __fastcall SetOnTotalProgress(TTotalProgressEvent Value);
  void __fastcall SetOnZipDialog(TZipDialogEvent Value);
  void __fastcall SetPassword(const String& Value);
  void __fastcall SetPasswordReqCount(unsigned Value);
#ifndef NO_SFX
  void __fastcall SetSFXIcon(TIcon* aIcon );
  void __fastcall SetSFXLanguage(const String Value);
  void __fastcall SetSFXPath(TFileName Value);
#endif
  void __fastcall SetVersion(const String Value);
  void __fastcall SetZipComment(const String Value);

protected:
  void __fastcall Done(void);
  int __fastcall Starting(void);
  bool __fastcall Stopped(void);
  __property bool Active={read=GetActive,write=SetActive, default=true};
  __property int AddCompLevel={read=FAddCompLevel,write=FAddCompLevel, default=9};
  __property TDateTime AddFrom={read=FFromDate,write=FFromDate};
  __property AddOpts AddOptions={read=FAddOptions,write=FAddOptions};
  __property AddStoreExts AddStoreSuffixes={read=FAddStoreSuffixes,write=FAddStoreSuffixes};
  __property int BufSize={read=FBufSize, write=FBufSize, default=65535};
  __property int Build = {read=GetBuild};
  __property bool Busy ={read=GetBusy};
  __property bool Cancel={read=GetCancel,write=SetCancel};
  __property CPOpts CodePage={read=FCodePage,write=FCodePage,default=cpAuto};
#ifndef NO_SPAN
 __property bool ConfirmErase = {read=FConfirmErase, write=FConfirmErase, default=true};
#endif
  __property int Count={read=GetCount};
  __property ZipDirEntry* DirEntry[int idx]={read=GetDirEntry};
  __property int DirOnlyCount={read=GetDirOnlyCount};
  __property String DLLDirectory={read=FDLLDirectory,write=FDLLDirectory};
  __property int Dll_Build={read=GetDll_Build};
  __property bool Dll_Load={read=GetDll_Load,write=SetDll_Load};
  __property String Dll_Path={read=GetDll_Path};
  __property String Dll_Version={read=GetDll_Version};
  __property ZipDirEntry* Entry[const String Name]={read=GetEntry};
  __property int ErrCode={read=GetErrCode,write=SetErrCode};
  __property String ExtAddStoreSuffixes={read=FExtAddStoreSuffixes,write=FExtAddStoreSuffixes};
  __property String ExtrBaseDir={read=FExtrBaseDir,write=FExtrBaseDir};
  __property ExtrOpts ExtrOptions={read=FExtrOptions,write=FExtrOptions};
  __property TStrings* FSpecArgs={read=FFSpecArgs,write=SetFSpecArgs};
  __property TStrings* FSpecArgsExcl={read=FFSpecArgsExcl,write=SetFSpecArgsExcl};
  __property int FullErrCode={read=GetFullErrCode};
  __property HWND Handle={read=FHandle, write=FHandle};
  __property TDeleteOpts HowToDelete={read=FHowToDelete,write=FHowToDelete,default=htdAllowUndo};
  __property bool IsSpanned={read=GetIsSpanned};
#ifndef NO_SPAN
	__property unsigned KeepFreeOnDisk1={read=FFreeOnDisk1, write=FFreeOnDisk1, nodefault};
	__property unsigned KeepFreeOnAllDisks={read=FFreeOnAllDisks, write=FFreeOnAllDisks,nodefault};
#endif
  __property String Language={read=GetLanguage,write=SetLanguage};
  __property String LanguageInfo[int index][unsigned info]={read=GetLanguageInfo};
#ifndef NO_SPAN
	__property int MaxVolumeSize={read=FMaxVolumeSize, write=FMaxVolumeSize, default=0};
#endif
  __property String Message={read=GetErrMessage};
#ifndef NO_SPAN
	__property int MinFreeVolumeSize={read=FMinFreeVolSize, write=FMinFreeVolSize, default=65536};
#endif
  __property bool NotMainThread={read=FNotMainThread,write=FNotMainThread};
  __property String Password={read=FPassword,write=FPassword};
  __property unsigned PasswordReqCount={read=FPasswordReqCount,write=FPasswordReqCount,default=1};
  __property String RootDir={read=FRootDir,write=FRootDir};
#ifndef NO_SFX
	__property String	 SFXCaption = {read=FSFXCaption, write=FSFXCaption, nodefault};
	__property String	 SFXCommandLine = {read=FSFXCommandLine, write=FSFXCommandLine, nodefault};
	__property String	 SFXDefaultDir = {read=FSFXDefaultDir, write=FSFXDefaultDir,  nodefault};
	__property TIcon  *SFXIcon = {read=FSFXIcon, write=SetSFXIcon, nodefault};
	__property String SFXLanguage = {read=GetSFXLanguage, write=SetSFXLanguage, nodefault};
	__property String  SFXMessage = {read=FSFXMessage, write=FSFXMessage, nodefault};
#endif
  __property int SFXOffset={read=GetSFXOffset};
#ifndef NO_SFX
	__property SfxOpts SFXOptions = {read=FSFXOptions, write=FSFXOptions, default=SFXCheckSize};
	__property OvrOpts SFXOverWriteMode = {read=FSFXOverWriteMode, write=FSFXOverWriteMode, default=OvrConfirm};
  __property TFileName	SFXPath	= {read=GetSFXPath,	write=SetSFXPath,	nodefault};
#endif
  __property SpanOpts SpanOptions = {read=FSpanOptions, write=FSpanOptions, nodefault};
  __property int SuccessCnt={read=GetSuccessCnt};
  __property String TempDir={read=FTempDir,write=FTempDir};
  __property BINT TotalSizeToProcess={read=GetTotalSizeToProcess};
  __property bool Trace={read=FTrace,write=FTrace};
  __property bool Unattended={read=FUnattended,write=FUnattended};
  __property bool UnzBusy={read=GetUnzBusy};
  __property bool UseDirOnlyEntries={read=FUseDirOnlyEntries,write=FUseDirOnlyEntries};
  __property bool Verbose={read=FVerbose,write=FVerbose};
  __property String Version={read=GetVersion,write=SetVersion};
  __property String VersionInfo={read=GetVersion,write=SetVersion};
  __property bool ZipBusy={read=GetZipBusy};
  __property String ZipComment={read=GetZipComment,write=SetZipComment};
  __property TList* ZipContents={read=GetZipContents};
  __property unsigned ZipEOC={read=GetZipEOC};
  // At runtime: every time the filename is assigned a value, the ZipDir will automatically be read.
  __property String ZipFileName={read=GetZipFileName,write=SetFileName};
  __property unsigned ZipFileSize={read=GetRealFileSize};
  __property unsigned ZipSOC={read=GetZipSOC};
#ifndef NO_STREAM
  __property TZipStream* ZipStream={read=GetZipStream};
#endif
#ifdef KEEP_BACKWARD
  __property int UnzVers={read=GetUnzVers};
  __property int ZipVers={read=GetZipVers};
#endif
    // Events
  __property TCheckTerminateEvent OnCheckTerminate={read=GetOnCheckTerminate, write=SetOnCheckTerminate};
  __property TCopyZipOverwriteEvent OnCopyZipOverwrite={read=GetOnCopyZipOverwrite,write=SetOnCopyZipOverwrite};
  __property TCRC32ErrorEvent OnCRC32Error={read=GetOnCRC32Error,write=SetOnCRC32Error};
  __property TNotifyEvent OnDirUpdate={read=GetOnDirUpdate,write=SetOnDirUpdate};
  __property TExtractOverwriteEvent OnExtractOverwrite={read=GetOnExtractOverwrite,write=SetOnExtractOverwrite};
	__property TExtractSkippedEvent OnExtractSkipped={read=GetOnExtractSkipped,write=SetOnExtractSkipped};
	__property TFileCommentEvent OnFileComment={read=GetOnFileComment,write=SetOnFileComment};
  __property TFileExtraEvent OnFileExtra={read=GetOnFileExtra,write=SetOnFileExtra};
#ifndef NO_SPAN
 	__property TGetNextDiskEvent OnGetNextDisk={read=GetOnGetNextDisk,write=SetOnGetNextDisk};
#endif
  __property TItemProgressEvent OnItemProgress={read=GetOnItemProgress,write=SetOnItemProgress};
  __property TMsgEvent OnMessage={read=GetOnMessage, write=SetOnMessage};
  __property TNewNameEvent OnNewName={read=GetOnNewName,write=SetOnNewName};
  __property TPasswordErrorEvent OnPasswordError={read=GetOnPasswordError, write=SetOnPasswordError};
	__property Zbcore::TProgressEvent OnProgress={read=GetOnProgress,write=SetOnProgress};
	__property TProgressDetailsEvent OnProgressDetails={read=GetOnProgressDetails,write=SetOnProgressDetails};
	__property TSetAddNameEvent OnSetAddName={read=GetOnSetAddName,write=SetOnSetAddName};
  __property TSetExtNameEvent OnSetExtName={read=GetOnSetExtName,write=SetOnSetExtName};
	__property TSetNewNameEvent OnSetNewName={read=GetOnSetNewName,write=SetOnSetNewName};
#ifndef NO_SPAN
	__property TStatusDiskEvent OnStatusDisk={read=GetOnStatusDisk,write=SetOnStatusDisk};
#endif
	__property TTickEvent OnTick={read=GetOnTick, write=SetOnTick};
	__property TTotalProgressEvent OnTotalProgress={read=GetOnTotalProgress,write=SetOnTotalProgress};
	__property TZipDialogEvent OnZipDialog={read=GetOnZipDialog,write=SetOnZipDialog};
public:
  __fastcall TCustomZipBuilder( Classes::TComponent *AOwner );
	__fastcall virtual ~TCustomZipBuilder( void );
  void __fastcall AbortDll(void);
  int __fastcall Add(void);
#ifndef NO_STREAM
  int __fastcall AddStreamToFile(const String& Filename = "",const DWORD FileDate = 0,
                                                          const DWORD FileAttr = 0);
  TZipStream* __fastcall AddStreamToStream(TMemoryStream* InStream);
 #endif
  String __fastcall AppendSlash(const String sDir);
  virtual void __fastcall BeforeDestruction(void);
  void __fastcall Clear(void);
#ifndef NO_SFX
  int __fastcall ConvertSFX(void);
  int __fastcall ConvertZIP(void);
#endif
	int __fastcall CopyZippedFiles(TCustomZipBuilder* DestZipBuilder, bool DeleteFromSource, OvrOpts OverwriteDest);
	int __fastcall Copy_File(const String InFileName,const String OutFileName);
	int __fastcall Delete(void);
  int __fastcall Extract(void);
#ifndef NO_STREAM
  TZipStream* __fastcall ExtractFileToStream(const String& Filename);
  TZipStream* __fastcall ExtractStreamToStream(TMemoryStream* InStream, unsigned long OutSize);
#endif
  ZipDirEntry* __fastcall Find(const String& fspec, int& idx);
  ZipDirEntry* __fastcall Find(const String& FileName);
  String __fastcall FullVersionString(void);
  String __fastcall GetAddPassword(void);
  String __fastcall GetAddPassword(TPasswordButton& Response);
  String __fastcall GetExtrPassword(void);
  String __fastcall GetExtrPassword(TPasswordButton& Response);
  TPasswordButton __fastcall GetPassword(const String& DialogCaption,
                                         const String& MsgTxt,
                                         TPasswordButtons pwb,
                                         String& ResultStr);
  int __fastcall IndexOf(const String& fname);
#ifndef NO_SFX
  int __fastcall IsZipSFX(const String& SFXExeName);
#endif
  int __fastcall List(void);
  virtual void __fastcall Loaded(void);
  String __fastcall MakeTempFileName(const String& Prefix,const String& Extension);
#ifndef NO_SPAN
  int __fastcall ReadSpan(const String InFileName, String OutFilePath);
#endif
//  changed 1.79.10.07
  int __fastcall Rename(TList& RenameList, int DateTime, TZBRenameOpts How = htrDefault);
  void __fastcall ShowExceptionError(const EZipBuilder& ZBExcept);
  void __fastcall ShowZipMessage(int Ident, const String UserStr);
  int __fastcall TheErrorCode(int errCode);
#ifndef NO_SPAN
  int __fastcall WriteSpan(const String InFileName, const String OutFileName);
#endif
#ifdef KEEP_BACKWARD
  int __fastcall Load_Unz_Dll(void);
  int __fastcall Load_Unz_Dll(bool Silent);
  int __fastcall Load_Zip_Dll(void);
  int __fastcall Load_Zip_Dll(bool Silent);
  void __fastcall Unload_Unz_Dll(void);
  void __fastcall Unload_Zip_Dll(void);
  String __fastcall UnzDllPath(void);
  String __fastcall ZipDllPath(void);
#endif
};

class PACKAGE TZipBuilder : public TCustomZipBuilder
{
private:
public:
  __property BufSize;
  __property Build;
  __property Busy;
  __property Cancel;
  __property Count;
  __property DirEntry;
  __property DirOnlyCount;
  __property Dll_Build;
  __property Dll_Path;
  __property Dll_Version;
  __property ErrCode;
  __property FullErrCode;
  __property Handle;
  __property IsSpanned;
  __property LanguageInfo;
  __property Message;
  __property NotMainThread;
  __property SFXOffset;
  __property SuccessCnt;
  __property TotalSizeToProcess;
  __property UnzBusy;
  __property ZipBusy;
  __property ZipComment;
  __property ZipContents;
  __property ZipEOC;
  __property ZipFileSize;
  __property ZipSOC;
#ifndef NO_STREAM
  __property ZipStream;
#endif
#ifdef KEEP_BACKWARD
  __property UnzVers;
  __property ZipVers;
#endif
  __fastcall TZipBuilder( Classes::TComponent *AOwner ) : TCustomZipBuilder(AOwner) {};
	__fastcall virtual ~TZipBuilder( void )  {};
__published:
  __property Active;
  __property AddCompLevel;
  __property AddFrom;
  __property AddOptions;
  __property AddStoreSuffixes={default=DefSuffixes};
  __property CodePage;
#ifndef NO_SPAN
  __property ConfirmErase;
#endif
  __property Dll_Load;
  __property DLLDirectory;
  __property ExtAddStoreSuffixes;
  __property ExtrBaseDir;
  __property ExtrOptions;
  __property FSpecArgs;
  __property FSpecArgsExcl;
  __property HowToDelete;
#ifndef NO_SPAN
  __property KeepFreeOnAllDisks;
  __property KeepFreeOnDisk1;
#endif
  __property Language;
#ifndef NO_SPAN
  __property MaxVolumeSize;
  __property MinFreeVolumeSize;
#endif
  __property Password;
  __property PasswordReqCount;
//  __property PrefLang;
  __property RootDir;
#ifndef NO_SFX
  __property SFXCaption;
  __property SFXCommandLine;
  __property SFXDefaultDir;
  __property SFXIcon;
  __property SFXLanguage;
  __property SFXMessage;
  __property SFXOptions;
  __property SFXOverWriteMode;
  __property SFXPath;
#endif
#ifndef NO_SPAN
  __property SpanOptions;
#endif
  __property TempDir;
  __property Trace;
  __property Unattended;
  __property UseDirOnlyEntries;
  __property Verbose;
  __property Version={stored=false};
  __property VersionInfo={stored=false};
  __property ZipFileName;
  __property OnCheckTerminate;
  __property OnCopyZipOverwrite;
  __property OnCRC32Error;
  __property OnDirUpdate;
  __property OnExtractOverwrite;
  __property OnExtractSkipped;
  __property OnFileComment;
  __property OnFileExtra;
#ifndef NO_SPAN
  __property OnGetNextDisk;
#endif
  __property OnItemProgress;
  __property OnMessage;
  __property OnNewName;
  __property OnPasswordError;
  __property OnProgress;
  __property OnProgressDetails;
  __property OnSetAddName;
  __property OnSetExtName;
  __property OnSetNewName;
#ifndef NO_SPAN
  __property OnStatusDisk;
#endif
  __property OnTick;
  __property OnTotalProgress;
  __property OnZipDialog;
};
#pragma option -a.

#if __BORLANDC__ < 1344		// BCB4=0x540(1344)
 //-- template instantiations -------------------------------------------------
 template class SfxOpts;
#endif
}	// end namespace ZipBuilder

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
	using namespace ZipBuilder;
#endif

#endif




