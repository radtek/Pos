//---------------------------------------------------------------------------

#ifndef ZipBuilderH
#define ZipBuilderH
//---------------------------------------------------------------------------
#include "CondDefines.h"
#include "ZipWrkr.h"
#include "EZipBuilder.h"
#include "ZipStrs.h"

//---------------------------------------------------------------------------

namespace ZipBuilder
{

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
struct ZipRenameRec
{
	String			Source;
	String			Dest;
	unsigned long	DateTime;
};

class PACKAGE TCustomZipBuilder : public Classes::TComponent

{
typedef Classes::TComponent inherited;

private:
  TZipWorker* FZip;
  bool FReentry;     // true = re-entry attempted
  bool FActive;    // true = not active
  bool FBusy;        // true = busy

  // fields of published properties
  int	FAddCompLevel;
  AddOpts	FAddOptions;
  AddStoreExts FAddStoreSuffixes;
  AnsiString FExtAddStoreSuffixes;
  AnsiString FExtrBaseDir;
  TStrings* FFSpecArgs;
  TStrings* FFSpecArgsExcl;
  AnsiString FTempDir;
  AnsiString FVersionInfo;
  AnsiString FDLLDirectory;
  AnsiString FRootDir;
  AnsiString FPassword;
  bool FVerbose;
  bool FTrace;
  HWND FHandle;
  ExtrOpts FExtrOptions;
  bool FEncrypt;
  SpanOpts FSpanOptions;
  TDateTime FFromDate;
  bool FUnattended;
  CPOpts FCodePage;
  DeleteOpts FHowToDelete;
  bool FUseDirOnlyEntries;
  unsigned FPasswordReqCount;
  bool FNotMainThread;
  int FBufSize;
#ifndef NO_SPAN
  bool FConfirmErase;
  unsigned FFreeOnDisk1;
  unsigned FFreeOnAllDisks;
  int FMaxVolumeSize;
  int FMinFreeVolSize;
#endif
#ifndef NO_SFX
		String					FSFXCaption;				// dflt = "Self-extracting Archive"
		String						FSFXCommandLine;	// dflt = ""
		String						FSFXDefaultDir;		// dflt = ""
		TIcon*						FSFXIcon;
		String						FSFXMessage;
		SfxOpts						FSFXOptions;
		OvrOpts						FSFXOverWriteMode;	// OvrConfirm  (others: OvrAlways, OvrNever)
		String						FSFXPath;
#endif
    //events
    TCheckTerminateEvent FOnCheckTerminate;
    TCopyZipOverwriteEvent FOnCopyZipOverwrite;
    TCRC32ErrorEvent FOnCRC32Error;
    Classes::TNotifyEvent		FOnDirUpdate;
    TExtractOverwriteEvent FOnExtractOverwrite;
    TExtractSkippedEvent FOnExtractSkipped;
    TFileCommentEvent FOnFileComment;
    TFileExtraEvent FOnFileExtra;
    TItemProgressEvent FOnItemProgress;
    TMsgEvent FOnMessage;
    TNewNameEvent FOnNewName;
    TPasswordErrorEvent FOnPasswordError;
    Zipbase::TProgressEvent FOnProgress;
    TProgressDetailsEvent FOnProgressDetails;
    TSetAddNameEvent FOnSetAddName;
    TSetExtNameEvent FOnSetExtName;
    TSetNewNameEvent FOnSetNewName;
    TTickEvent FOnTick;
    TTotalProgressEvent FOnTotalProgress;
    TZipDialogEvent FOnZipDialog;
    TZipStrEvent FOnZipStr;
#ifndef NO_SPAN
    TGetNextDiskEvent FOnGetNextDisk;
    TStatusDiskEvent FOnStatusDisk;
#endif
    // Property get/set functions
  bool __fastcall GetBusy(void);
  bool __fastcall GetCancel(void);
  int __fastcall GetCount(void);
  ZipDirEntry* __fastcall GetDirEntry(int idx);
  int __fastcall GetDirOnlyCount(void);
  int __fastcall GetErrCode(void);
  AnsiString __fastcall GetErrMessage(void);
  int __fastcall GetFullErrCode(void);
  bool __fastcall GetIsSpanned(void);
  AnsiString __fastcall GetPPassword(void);
  unsigned __fastcall GetRealFileSize(void);
  int __fastcall GetSFXOffset(void);
  int __fastcall GetSuccessCnt(void);
  __int64 __fastcall GetTotalSizeToProcess(void);
  bool __fastcall GetUnzBusy(void);
  int __fastcall GetUnzVers(void);
  int __fastcall GetVer(void);
  bool __fastcall GetZipBusy(void);
  AnsiString __fastcall GetZipComment(void);
  TList* __fastcall GetZipContents(void);
  unsigned __fastcall GetZipEOC(void);
  AnsiString __fastcall GetZipFileName(void);
  unsigned __fastcall GetZipSOC(void);
  int __fastcall GetZipVers(void);
  void __fastcall SetCancel(bool Value);
  void __fastcall SetErrCode(int Value);
  void __fastcall SetActive(bool Value);
  void __fastcall SetFileName(AnsiString Value);
  void __fastcall SetPassword(AnsiString Value);
  void __fastcall SetPasswordReqCount(unsigned Value);
  void __fastcall SetVersionInfo(AnsiString Value);
  void __fastcall SetZipComment(AnsiString Value);
  void __fastcall SetOnZipStr(TZipStrEvent Value);
  TZipStrEvent __fastcall GetOnZipStr(void);
  void __fastcall SetFSpecArgs(const TStrings* Value);
  void __fastcall SetFSpecArgsExcl(const TStrings* Value);
#ifndef NO_STREAM
   TZipStream* __fastcall GetZipStream(void);
#endif

#ifndef NO_SFX
    void   __fastcall SetSFXIcon( TIcon *aIcon );
#endif

protected:
  void __fastcall Starting(void);
  void __fastcall Done(void);
  bool __fastcall Stopped(void);
  bool __fastcall CanStart(void);

public:
  __fastcall TCustomZipBuilder( Classes::TComponent *AOwner );
	__fastcall virtual ~TCustomZipBuilder( void );
  virtual void __fastcall BeforeDestruction(void);
  virtual void __fastcall Loaded(void);
  // Public Properties (run-time only)
protected:
  __property bool Busy ={read=GetBusy};
  __property bool Cancel={read=GetCancel,write=SetCancel};
  __property int Count={read=GetCount};
  __property ZipDirEntry* DirEntry[int idx]={read=GetDirEntry};
  __property int DirOnlyCount={read=GetDirOnlyCount};
  __property int ErrCode={read=GetErrCode,write=SetErrCode};
  __property int FullErrCode={read=GetFullErrCode};
  __property HWND Handle={read=FHandle, write=FHandle};
  __property bool IsSpanned={read=GetIsSpanned};
  __property AnsiString Message={read=GetErrMessage};
  __property int SFXOffset={read=GetSFXOffset};
  __property int SuccessCnt={read=GetSuccessCnt};
  __property BINT TotalSizeToProcess={read=GetTotalSizeToProcess};
  __property bool UnzBusy={read=GetUnzBusy};
  __property int UnzVers={read=GetUnzVers};
  __property int Ver = {read=GetVer, nodefault};
  __property bool ZipBusy={read=GetZipBusy};
  __property AnsiString ZipComment={read=GetZipComment,write=SetZipComment};
  __property TList* ZipContents={read=GetZipContents};
  __property unsigned ZipEOC={read=GetZipEOC};
  __property unsigned ZipFileSize={read=GetRealFileSize};
  __property unsigned ZipSOC={read=GetZipSOC};
  __property int ZipVers={read=GetZipVers};
  __property bool NotMainThread={read=FNotMainThread,write=FNotMainThread};
#ifndef NO_STREAM
  __property TZipStream* ZipStream={read=GetZipStream};
#endif
public:
    // Public Methods
    // NOTE: Test is an sub-option of extract
  int __fastcall Add(void);
  AnsiString __fastcall AppendSlash(AnsiString sDir);
  int __fastcall Copy_File(const AnsiString InFileName,const AnsiString OutFileName);
  int __fastcall CopyZippedFiles(TCustomZipBuilder* DestZipBuilder, bool DeleteFromSource, OvrOpts OverwriteDest);
  int __fastcall Delete(void);
  int __fastcall Extract(void);
  ZipDirEntry* __fastcall Find(const AnsiString fspec, int& idx);
  AnsiString __fastcall FullVersionString(void);
  AnsiString __fastcall GetAddPassword(void);
  AnsiString __fastcall GetAddPassword(TPasswordButton& Response);
  AnsiString __fastcall GetExtrPassword(void);
  AnsiString __fastcall GetExtrPassword(TPasswordButton& Response);
  TPasswordButton __fastcall GetPassword(AnsiString DialogCaption,AnsiString MsgTxt,TPasswordButtons pwb,
                               AnsiString& ResultStr);
  int __fastcall IndexOf(const AnsiString fname);
  int __fastcall List(void);
  int __fastcall Load_Unz_Dll(void);
  int __fastcall Load_Zip_Dll(void);
  AnsiString __fastcall MakeTempFileName(AnsiString Prefix,AnsiString Extension);
  int __fastcall Rename(TList& RenameList, int DateTime);
  int __fastcall TheErrorCode(int errCode);
  AnsiString __fastcall UnzDllPath(void);
  AnsiString __fastcall ZipDllPath(void);
  void __fastcall AbortDlls(void);
  void __fastcall Clear(void);
  void __fastcall ShowExceptionError(const EZipBuilder &ZBExcept);
  void __fastcall ShowZipMessage(int Ident, AnsiString UserStr = "");
  void __fastcall Unload_Unz_Dll(void);
  void __fastcall Unload_Zip_Dll(void);

#ifndef NO_SPAN
  int __fastcall ReadSpan(AnsiString InFileName, AnsiString& OutFilePath);
  int __fastcall WriteSpan(AnsiString InFileName, AnsiString OutFileName);
#endif
#ifndef NO_SFX
  int __fastcall IsZipSFX(const AnsiString SFXExeName);
  int __fastcall ConvertSFX(void);
  int __fastcall ConvertZIP(void);
#endif
#ifndef NO_STREAM
  int __fastcall AddStreamToFile(AnsiString Filename,DWORD FileDate, DWORD FileAttr);
  TZipStream* __fastcall AddStreamToStream(TMemoryStream* InStream);
  TZipStream* __fastcall ExtractFileToStream(AnsiString Filename);
  TZipStream* __fastcall ExtractStreamToStream(TMemoryStream* InStream, unsigned long OutSize);
#endif
protected:
  __property bool Active={read=FActive,write=SetActive, default = true};
  __property int AddCompLevel={read=FAddCompLevel,write=FAddCompLevel};
  __property TDateTime AddFrom={read=FFromDate,write=FFromDate};
  __property AddOpts AddOptions={read=FAddOptions,write=FAddOptions};
  __property AddStoreExts AddStoreSuffixes={read=FAddStoreSuffixes,write=FAddStoreSuffixes};
  __property int BufSize={read=FBufSize, write=FBufSize, default=65535};
  __property CPOpts CodePage={read=FCodePage,write=FCodePage,default=cpAuto};
  __property AnsiString DLLDirectory={read=FDLLDirectory,write=FDLLDirectory};
  __property AnsiString ExtAddStoreSuffixes={read=FExtAddStoreSuffixes,write=FExtAddStoreSuffixes};
  __property AnsiString ExtrBaseDir={read=FExtrBaseDir,write=FExtrBaseDir};
  __property ExtrOpts ExtrOptions={read=FExtrOptions,write=FExtrOptions};
  __property TStrings* FSpecArgs={read=FFSpecArgs,write=SetFSpecArgs};
  __property TStrings* FSpecArgsExcl={read=FFSpecArgsExcl,write=SetFSpecArgsExcl};
  __property DeleteOpts HowToDelete={read=FHowToDelete,write=FHowToDelete,default=htdAllowUndo};
  __property unsigned PasswordReqCount={read=FPasswordReqCount,write=FPasswordReqCount,default=1};
  __property AnsiString RootDir={read=FRootDir,write=FRootDir};
  __property AnsiString TempDir={read=FTempDir,write=FTempDir};
  __property bool Trace={read=FTrace,write=FTrace};
  __property bool Unattended={read=FUnattended,write=FUnattended};
  __property bool UseDirOnlyEntries={read=FUseDirOnlyEntries,write=FUseDirOnlyEntries};
  __property bool Verbose={read=FVerbose,write=FVerbose};
  __property AnsiString VersionInfo={read=FVersionInfo,write=SetVersionInfo};
    // At runtime: every time the filename is assigned a value,
    //the ZipDir will automatically be read.
  __property AnsiString ZipFileName={read=GetZipFileName,write=SetFileName};
  __property AnsiString Password={read=FPassword,write=FPassword};
//      __property bool DeferList={read GetDeferList write SetDeferList; // new 1.80
#ifndef NO_SPAN
  __property bool ConfirmErase = {read=FConfirmErase, write=FConfirmErase, default=true};
	__property unsigned KeepFreeOnDisk1 = {read=FFreeOnDisk1, write=FFreeOnDisk1, nodefault};
	__property unsigned KeepFreeOnAllDisks = {read=FFreeOnAllDisks, write=FFreeOnAllDisks,nodefault};
	__property int MaxVolumeSize = {read=FMaxVolumeSize, write=FMaxVolumeSize, default=0};
	__property int MinFreeVolumeSize = {read=FMinFreeVolSize, write=FMinFreeVolSize, default=65536};
  __property SpanOpts SpanOptions = {read=FSpanOptions, write=FSpanOptions, nodefault};
	__property TGetNextDiskEvent OnGetNextDisk = {read=FOnGetNextDisk, write=FOnGetNextDisk};
	__property TStatusDiskEvent OnStatusDisk = {read=FOnStatusDisk, write=FOnStatusDisk};
#endif
    // Events
  __property TCheckTerminateEvent OnCheckTerminate = {read=FOnCheckTerminate, write=FOnCheckTerminate};
  __property TCopyZipOverwriteEvent OnCopyZipOverwrite = {read=FOnCopyZipOverwrite, write=FOnCopyZipOverwrite};
  __property TCRC32ErrorEvent OnCRC32Error = {read=FOnCRC32Error, write=FOnCRC32Error};
  __property TNotifyEvent OnDirUpdate={read=FOnDirUpdate,write=FOnDirUpdate};
  __property TExtractOverwriteEvent OnExtractOverwrite = {read=FOnExtractOverwrite, write=FOnExtractOverwrite};
	__property TExtractSkippedEvent OnExtractSkipped = {read=FOnExtractSkipped, write=FOnExtractSkipped};
	__property TFileCommentEvent OnFileComment = {read=FOnFileComment, write=FOnFileComment};
  __property TFileExtraEvent OnFileExtra = {read=FOnFileExtra, write=FOnFileExtra};
  __property TItemProgressEvent OnItemProgress = {read=FOnItemProgress, write=FOnItemProgress};
  __property TMsgEvent OnMessage = {read=FOnMessage, write=FOnMessage};
  __property TNewNameEvent OnNewName = {read=FOnNewName, write=FOnNewName};
  __property TPasswordErrorEvent OnPasswordError = {read=FOnPasswordError, write=FOnPasswordError};
	__property Zipbase::TProgressEvent OnProgress = {read=FOnProgress, write=FOnProgress};
	__property TProgressDetailsEvent OnProgressDetails = {read=FOnProgressDetails, write=FOnProgressDetails};
	__property TSetAddNameEvent OnSetAddName = {read=FOnSetAddName, write=FOnSetAddName};
	__property TSetExtNameEvent OnSetExtName = {read=FOnSetExtName, write=FOnSetExtName};
	__property TSetNewNameEvent OnSetNewName = {read=FOnSetNewName, write=FOnSetNewName};
	__property TTickEvent OnTick = {read=FOnTick, write=FOnTick};
	__property TTotalProgressEvent OnTotalProgress = {read=FOnTotalProgress, write=FOnTotalProgress};
	__property TZipStrEvent OnZipStr = {read=FOnZipStr, write=FOnZipStr};
	__property TZipDialogEvent OnZipDialog={read=FOnZipDialog,write=FOnZipDialog};
#ifndef NO_SFX
		__property SfxOpts SFXOptions = {read=FSFXOptions, write=FSFXOptions, default=SFXCheckSize};
		__property String	 SFXPath	= {read=FSFXPath,	write=FSFXPath,	nodefault};
		__property OvrOpts SFXOverWriteMode = {read=FSFXOverWriteMode, write=FSFXOverWriteMode, default=OvrConfirm};
		__property String	 SFXCaption = {read=FSFXCaption, write=FSFXCaption, nodefault};
		__property String	 SFXDefaultDir = {read=FSFXDefaultDir, write=FSFXDefaultDir,  nodefault};
		__property String	 SFXCommandLine = {read=FSFXCommandLine, write=FSFXCommandLine, nodefault};
		__property String  SFXMessage = {read=FSFXMessage, write=FSFXMessage, nodefault};
		__property TIcon  *SFXIcon = {read=FSFXIcon, write=SetSFXIcon, nodefault};
#endif
};

class PACKAGE TZipBuilder : public TCustomZipBuilder
{
private:
public:
  __property BufSize;
  __property Busy;
  __property Cancel;
  __property Count;
  __property DirEntry;
  __property DirOnlyCount;
  __property ErrCode;
  __property FullErrCode;
  __property Handle;
  __property IsSpanned;
  __property Message;
  __property SFXOffset;
  __property SuccessCnt;
  __property TotalSizeToProcess;
  __property UnzBusy;
  __property UnzVers;
  __property Ver;
  __property ZipBusy;
  __property ZipComment;
  __property ZipContents;
  __property ZipEOC;
  __property ZipFileSize;
  __property ZipSOC;
  __property ZipVers;
  __property NotMainThread;
  __fastcall TZipBuilder( Classes::TComponent *AOwner ) : TCustomZipBuilder(AOwner) {};
	__fastcall virtual ~TZipBuilder( void )  {};
#ifndef NO_STREAM
  __property ZipStream;
#endif
__published:
  __property Active;
  __property AddCompLevel;
  __property AddFrom;
  __property AddOptions;
  __property AddStoreSuffixes;
  __property CodePage;
  __property DLLDirectory;
  __property ExtAddStoreSuffixes;
  __property ExtrBaseDir;
  __property ExtrOptions;
  __property FSpecArgs;
  __property FSpecArgsExcl;
  __property HowToDelete;
  __property PasswordReqCount;
  __property RootDir;
  __property TempDir;
  __property Trace;
  __property Unattended;
  __property UseDirOnlyEntries;
  __property Verbose;
  __property VersionInfo;
  __property ZipFileName;
  __property Password;
#ifndef NO_SPAN
  __property ConfirmErase;
  __property KeepFreeOnAllDisks;
  __property KeepFreeOnDisk1;
  __property MaxVolumeSize;
  __property MinFreeVolumeSize;
  __property SpanOptions;
#endif
  __property OnCheckTerminate;
  __property OnDirUpdate;
  __property OnTotalProgress;
  __property OnItemProgress;
  __property OnProgressDetails;
  __property OnProgress;
  __property OnMessage;
  __property OnSetNewName;
  __property OnNewName;
  __property OnCRC32Error;
  __property OnPasswordError;
  __property OnExtractOverwrite;
  __property OnExtractSkipped;
  __property OnCopyZipOverwrite;
  __property OnFileComment;
  __property OnTick;
  __property OnFileExtra;
  __property OnSetAddName;
  __property OnSetExtName;
  __property OnZipDialog;
  __property OnZipStr;
#ifndef NO_SPAN
  __property OnGetNextDisk;
  __property OnStatusDisk;
#endif

#ifndef NO_SFX
  __property SFXCaption;
  __property SFXCommandLine;
  __property SFXDefaultDir;
  __property SFXIcon;
  __property SFXMessage;
  __property SFXOptions;
  __property SFXOverWriteMode;
  __property SFXPath;
#endif
};
#pragma option -a.

}	// end namespace ZipBuilder

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
	using namespace ZipBuilder;
#endif

#endif




