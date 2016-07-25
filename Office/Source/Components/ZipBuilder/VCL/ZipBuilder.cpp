/* TCustomZipBuilder component v1.60 by Chris Vleghert
 * a C++ Builder 1, 3, 4 , 5 and 6 wrapper for the freeware ZIP and UNZIP DLL's
 * from Chris Vleghert and E.W. Engler.
 *	 e-mail: englere@abraxis.com
 *  www:    http://www.geocities.com/SiliconValley/Network/2114
 *  v1.8 by Roger Aelbrecht October 18, 2004.
 *          http://home.tiscali.be/driehoeksw
 */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ZipBuilder.h"
#include "ZipBuildDefs.h"
#include "ZipMsg.h"
#include "ZipUtils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

// ================== changed or New functions =============================

//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetFSpecArgs(const TStrings* Value)
{
  FFSpecArgs->Assign(const_cast<TPersistent*>(static_cast<const TPersistent*>(Value)));
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetFSpecArgsExcl(const TStrings* Value)
{
  FFSpecArgsExcl->Assign(const_cast<TPersistent*>(static_cast<const TPersistent*>(Value)));
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::TheErrorCode(int errCode)
{
  return errCode & (Reentry_Error -1);
}

void __fastcall TCustomZipBuilder::Starting(void)
{
  FBusy = true;
  FReentry = false;
  SetZipStr(OnZipStr, true);
  FZip->Builder = this;
  FZip->OnCheckTerminate = OnCheckTerminate;
  FZip->OnItemProgress = OnItemProgress;
  FZip->OnMessage = OnMessage;
  FZip->OnProgress = OnProgress;
  FZip->OnProgressDetails = OnProgressDetails;
  FZip->OnTick = OnTick;
  FZip->OnTotalProgress = OnTotalProgress;
  FZip->OnZipDialog = OnZipDialog;
  FZip->OnCopyZipOverwrite = OnCopyZipOverwrite;
  FZip->OnCRC32Error = OnCRC32Error;
  FZip->OnDirUpdate = OnDirUpdate;
  FZip->OnExtractOverwrite = OnExtractOverwrite;
  FZip->OnExtractSkipped = OnExtractSkipped;
  FZip->OnFileComment = OnFileComment;
  FZip->OnFileExtra = OnFileExtra;
  FZip->OnNewName = OnNewName;
  FZip->OnPasswordError = OnPasswordError;
  FZip->OnSetAddName = OnSetAddName;
  FZip->OnSetExtName = OnSetExtName;
  FZip->OnSetNewName = OnSetNewName;
#ifndef NO_SPAN
  FZip->OnGetNextDisk = OnGetNextDisk;
  FZip->OnStatusDisk = OnStatusDisk;
#endif
  FZip->AddCompLevel = AddCompLevel;
  FZip->AddFrom = AddFrom;
  FZip->AddOptions = AddOptions;
  FZip->AddStoreSuffixes = AddStoreSuffixes;
  FZip->BufSize = BufSize;
  FZip->ExtAddStoreSuffixes = ExtAddStoreSuffixes;
  FZip->CodePage = CodePage;
  FZip->ExtrBaseDir = ExtrBaseDir;
  FZip->ExtrOptions = ExtrOptions;
  FZip->Unattended = Unattended;
  FZip->Verbose = Verbose;
  FZip->Trace = Trace;
  FZip->DLLDirectory = DLLDirectory;
  FZip->TempDir = TempDir;
  FZip->NotMainTask = NotMainThread;
#ifndef NO_SPAN
  FZip->ConfirmErase = ConfirmErase;
  FZip->KeepFreeOnAllDisks = KeepFreeOnAllDisks;
  FZip->KeepFreeOnDisk1 = KeepFreeOnDisk1;
  FZip->MaxVolumeSize = MaxVolumeSize;
  FZip->MinFreeVolumeSize = MinFreeVolumeSize;
  FZip->SpanOptions = SpanOptions;
#endif
  FZip->FSpecArgs->Assign(FSpecArgs);
  FZip->FSpecArgsExcl->Assign(FSpecArgsExcl);
  FZip->Handle = Handle;
  FZip->HowToDelete = HowToDelete;
  FZip->Password = Password;
  FZip->PasswordReqCount = PasswordReqCount;
  FZip->RootDir = RootDir;
  FZip->Unattended = Unattended;
  FZip->UseDirOnlyEntries = UseDirOnlyEntries;
#ifndef NO_SFX
  FZip->SFXCaption = SFXCaption;
	FZip->SFXCommandLine = SFXCommandLine;
	FZip->SFXDefaultDir = SFXDefaultDir;
	FZip->SFXIcon->Assign(SFXIcon);
	FZip->SFXMessage = SFXMessage;
	FZip->SFXOptions = SFXOptions;
	FZip->SFXOverWriteMode = SFXOverWriteMode;
	FZip->SFXPath = SFXPath;
#endif
  FZip->Starting();
}

void __fastcall TCustomZipBuilder::Done(void)
{
  FZip->Done();
  FBusy = false;
  if(FZip->Reentry) FReentry = true;
  FFSpecArgs->Assign(FZip->FSpecArgs);
  FFSpecArgsExcl->Assign(FZip->FSpecArgsExcl);
}

bool __fastcall TCustomZipBuilder::Stopped(void)
{
  if(!FBusy) return true;
  else
  {
    FReentry = true;
    FZip->Attacked(this);
    return false;
  }
}
bool __fastcall TCustomZipBuilder::CanStart(void)
{
  if(ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading))
    return false;
  else return Active & Stopped();
}

bool __fastcall TCustomZipBuilder::GetCancel(void)
{
  return FZip->Cancel;
}

int __fastcall TCustomZipBuilder::GetCount(void)
{
  if(Active ) return FZip->Count;
  else return 0;
}

ZipDirEntry* __fastcall TCustomZipBuilder::GetDirEntry(int idx)
{
  if(Active) return FZip->DirEntry[idx];
  else return NULL;
}

int __fastcall TCustomZipBuilder::GetDirOnlyCount(void)
{
  return FZip->DirOnlyCount;
}

int __fastcall TCustomZipBuilder::GetErrCode(void)
{
  if(FReentry) return FZip->ErrCode | Reentry_Error;
  else if(!Active) return GE_Inactive;
  return FZip->ErrCode;
}

AnsiString __fastcall TCustomZipBuilder::GetErrMessage(void)
{
  AnsiString Result;
  if(Active) Result = FZip->Message;
  else Result = ZipLoadStr(GE_Inactive);
  if(FReentry) Result = ZipFmtLoadStr(GE_WasBusy, ARRAYOFCONST((Result)));
  return Result;
}

int __fastcall TCustomZipBuilder::GetFullErrCode(void)
{
  return FZip->FullErrCode;
}

bool __fastcall TCustomZipBuilder::GetIsSpanned(void)
{
  return FZip->IsSpanned;
}

AnsiString __fastcall TCustomZipBuilder::GetPPassword(void)
{
  return FZip->Password;
}

unsigned __fastcall TCustomZipBuilder::GetRealFileSize(void)
{
  return FZip->ZipFileSize;
}

int __fastcall TCustomZipBuilder::GetSFXOffset(void)
{
  return FZip->SFXOffset;
}

int __fastcall TCustomZipBuilder::GetSuccessCnt(void)
{
  return FZip->SuccessCnt;
}

__int64 __fastcall TCustomZipBuilder::GetTotalSizeToProcess(void)
{
  return FZip->TotalSizeToProcess;
}

bool __fastcall TCustomZipBuilder::GetUnzBusy(void)
{
  return FZip->UnzBusy;
}

int __fastcall TCustomZipBuilder::GetUnzVers(void)
{
  return FZip->UnzVers;
}

int __fastcall TCustomZipBuilder::GetVer(void)
{
	return FZip->Ver;
}

bool __fastcall TCustomZipBuilder::GetZipBusy(void)
{
  return FZip->ZipBusy;
}

AnsiString __fastcall TCustomZipBuilder::GetZipComment(void)
{
  return FZip->ZipComment;
}

TList* __fastcall TCustomZipBuilder::GetZipContents(void)
{
  return FZip->ZipContents;
}

unsigned __fastcall TCustomZipBuilder::GetZipEOC(void)
{
  return FZip->ZipEOC;
}

AnsiString __fastcall TCustomZipBuilder::GetZipFileName(void)
{
  return FZip->ZipFileName;
}

unsigned __fastcall TCustomZipBuilder::GetZipSOC(void)
{
  return FZip->ZipSOC;
}

#ifndef NO_STREAM
TZipStream* __fastcall TCustomZipBuilder::GetZipStream(void)
{
  return FZip->ZipStream;
}
#endif

int __fastcall TCustomZipBuilder::GetZipVers(void)
{
  return FZip->ZipVers;
}

void __fastcall TCustomZipBuilder::SetCancel(bool Value)
{
  FZip->Cancel = Value;
}

void __fastcall TCustomZipBuilder::SetErrCode(int Value)
{
  if(Stopped()) FZip->ErrCode = Value;
}

void __fastcall TCustomZipBuilder::SetFileName(AnsiString Value)
{
  if(ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading))
    FZip->Active = false; // stop any actions
  if(Stopped())
  {
    try
    {
      Starting();
      FZip->ZipFileName = Value;
    }
    __finally
    {
      Done();
    }
  }
}

void __fastcall TCustomZipBuilder::SetPassword(AnsiString Value)
{
  FZip->Password = Value;
}

void __fastcall TCustomZipBuilder::SetPasswordReqCount(unsigned Value)
{
  if(Value != FPasswordReqCount)
  {
    if(Value > 15) Value = 15;
    FPasswordReqCount = Value;
  }
}

void __fastcall TCustomZipBuilder::SetVersionInfo(AnsiString Value)
{
  //FZip->VersionInfo = Value;
}

void __fastcall TCustomZipBuilder::SetZipComment(AnsiString Value)
{
  if(ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading))
    FZip->Active = false; // stop any actions
  if(Stopped())
  {
    try
    {
      Starting();
      FZip->ZipComment = Value;
    }
    __finally
    {
      Done();
    }
  }
}

TZipStrEvent __fastcall TCustomZipBuilder::GetOnZipStr(void)
{
  return SetZipStr(NULL, false);
}

void __fastcall TCustomZipBuilder::SetOnZipStr(TZipStrEvent Value)
{
  SetZipStr(Value, true);
}

__fastcall TCustomZipBuilder::TCustomZipBuilder(Classes::TComponent *AOwner) : TComponent(AOwner)
{
  FZip = new TZipWorker();
  FActive = false;
  FBusy = false;
  FNotMainThread = false;
  FBufSize = 65536; // 64 kB
  FFSpecArgs = new TStringList();
  FFSpecArgsExcl = new TStringList();
  FAddCompLevel	=  9;		// Default to tightest compression.
  FAddStoreSuffixes = AddStoreExts() << assGIF << assPNG << assZ << assZIP << assZOO << assARC
														<< assLZH << assARJ << assTAZ << assTGZ << assLHA
														<< assRAR << assACE << assCAB << assGZ << assGZIP << assJAR;
  FCodePage = cpAuto;
  FEncrypt = false;
  FFromDate = 0;
  FHandle = Application->Handle;
  FHowToDelete = htdAllowUndo;
  FPassword	= "";
	FPasswordReqCount = 1;
  FUnattended	= false;
  FUseDirOnlyEntries = false;
  FVersionInfo			= ZIPBUILDERVERSION;
#ifndef NO_SPAN
  FFreeOnDisk1 =  0;			// Don't leave anything free.
  FFreeOnAllDisks = 0;         // 1.72  use all space
  FMaxVolumeSize =  0;			// Use the maximum disk size.
	FMinFreeVolSize	= 65536;		// Reject disks with less bytes than...
  SpanOptions.Clear();
	FConfirmErase			= true;
#endif
#ifndef NO_SFX
	FSFXIcon					= new TIcon();
	FSFXOverWriteMode		= OvrConfirm;
	FSFXCaption				= "Self-extracting Archive";
	FSFXDefaultDir			= "";
	FSFXCommandLine			= "";
	FSFXOptions				= SfxOpts() << SFXCheckSize;
	FSFXPath					= "ZipSFX.bin";
#endif
  // events
  FOnCheckTerminate = NULL;
  FOnItemProgress = NULL;
  FOnMessage = NULL;
  FOnProgress = NULL;
  FOnProgressDetails = NULL;
  FOnTick   = NULL;
  FOnTotalProgress = NULL;
  FOnZipDialog = NULL;
  FOnZipStr = NULL;
  FOnCopyZipOverwrite = NULL;
  FOnCRC32Error = NULL;
  FOnDirUpdate = NULL;
  FOnExtractOverwrite = NULL;
  FOnExtractSkipped = NULL;
  FOnFileComment = NULL;
  FOnFileExtra = NULL;
  FOnNewName = NULL;
  FOnPasswordError = NULL;
  FOnSetAddName = NULL;
  FOnSetExtName = NULL;
  FOnSetNewName = NULL;
#ifndef NO_SPAN
  FOnGetNextDisk = NULL;
  FOnStatusDisk = NULL;
#endif
  FActive = true;
  FZip->Active = Active;
}

__fastcall TCustomZipBuilder::~TCustomZipBuilder( void )
{
  delete FFSpecArgsExcl;
	delete FFSpecArgs;
  delete FZip;
#ifndef NO_SFX
	delete FSFXIcon;
#endif
}

void __fastcall TCustomZipBuilder::BeforeDestruction(void)
{
  if(FZip) FZip->Active = false;
  inherited::BeforeDestruction();
}

void __fastcall TCustomZipBuilder::Loaded(void)
{
  inherited::Loaded();
  if(!ComponentState.Contains(csDesigning))
    FZip->Active = Active;
}

int __fastcall TCustomZipBuilder::Add(void)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->Add();
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

#ifndef NO_STREAM
int __fastcall TCustomZipBuilder::AddStreamToFile(AnsiString Filename,DWORD FileDate,DWord FileAttr)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->AddStreamToFile(Filename, FileDate, FileAttr);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}
#endif

#ifndef NO_STREAM
TZipStream* __fastcall TCustomZipBuilder::AddStreamToStream(TMemoryStream* InStream)
{
  TZipStream* Result = NULL;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->AddStreamToStream(InStream);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}
#endif

AnsiString __fastcall TCustomZipBuilder::AppendSlash(AnsiString sDir)
{
  return DelimitPath(sDir, true);
}

int __fastcall TCustomZipBuilder::Copy_File(const AnsiString InFileName, AnsiString OutFileName)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->Copy_File(InFileName, OutFileName);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

int __fastcall TCustomZipBuilder::CopyZippedFiles(TCustomZipBuilder* DesTCustomZipBuilder, bool DeleteFromSource,
                                            OvrOpts OverwriteDest)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->CopyZippedFiles(DesTCustomZipBuilder->FZip, DeleteFromSource, OverwriteDest);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

int __fastcall TCustomZipBuilder::Delete(void)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->Delete();
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

int __fastcall TCustomZipBuilder::Extract(void)
{
  int Result= BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->Extract();
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

#ifndef NO_STREAM
TZipStream* __fastcall TCustomZipBuilder::ExtractFileToStream(AnsiString Filename)
{
  TZipStream* Result = NULL;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->ExtractFileToStream(Filename);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}
#endif

#ifndef NO_STREAM
TZipStream* __fastcall TCustomZipBuilder::ExtractStreamToStream(TMemoryStream* InStream, unsigned long OutSize)
{
  TZipStream* Result = NULL;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->ExtractStreamToStream(InStream, OutSize);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}
#endif

ZipDirEntry* __fastcall TCustomZipBuilder::Find(const AnsiString fspec, int& idx)
{
  ZipDirEntry* Result = NULL;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->Find(fspec, idx);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

AnsiString __fastcall TCustomZipBuilder::FullVersionString(void)
{
  return FZip->FullVersionString();
}

AnsiString __fastcall TCustomZipBuilder::GetAddPassword(void)
{
  TPasswordButton Resp;
  return FZip->GetAddPassword(Resp);
}

AnsiString __fastcall TCustomZipBuilder::GetExtrPassword(void)
{
  TPasswordButton Resp;
  return FZip->GetExtrPassword(Resp);
}

AnsiString __fastcall TCustomZipBuilder::GetAddPassword(TPasswordButton& Response)
{
  return FZip->GetAddPassword(Response);
}

AnsiString __fastcall TCustomZipBuilder::GetExtrPassword(TPasswordButton& Response)
{
  return FZip->GetExtrPassword(Response);
}


TPasswordButton __fastcall TCustomZipBuilder::GetPassword(AnsiString DialogCaption, AnsiString MsgTxt,
                                                     TPasswordButtons pwb, AnsiString& ResultStr)
{
  return FZip->GetPassword(DialogCaption, MsgTxt, pwb, ResultStr);
}

int __fastcall TCustomZipBuilder::IndexOf(const AnsiString fname)
{
  int Result = -1;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->IndexOf(fname);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

int __fastcall TCustomZipBuilder::List(void)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->List();
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

int __fastcall TCustomZipBuilder::Load_Unz_Dll(void)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->Load_Unz_Dll();
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

int __fastcall TCustomZipBuilder::Load_Zip_Dll(void)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->Load_Zip_Dll();
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

AnsiString __fastcall TCustomZipBuilder::MakeTempFileName(AnsiString Prefix, AnsiString Extension)
{
  return FZip->MakeTempFileName(Prefix, Extension);
}

int __fastcall TCustomZipBuilder::Rename(TList& RenameList,int DateTime)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->Rename(RenameList, DateTime);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

AnsiString __fastcall TCustomZipBuilder::UnzDllPath(void)
{
  return FZip->UnzDllPath();
}

AnsiString __fastcall TCustomZipBuilder::ZipDllPath(void)
{
  return FZip->ZipDllPath();
}

void __fastcall TCustomZipBuilder::AbortDlls(void)
{
  FZip->AbortDlls();
}

void __fastcall TCustomZipBuilder::Clear(void)
{
  if(CanStart())
  {
    FZip->Clear();
    Done();
    FReentry = false;
  }
}

void __fastcall TCustomZipBuilder::ShowExceptionError(const EZipBuilder &ZBExcept)
{
  FZip->ShowExceptionError(ZBExcept);
}

void __fastcall TCustomZipBuilder::ShowZipMessage(int Ident, AnsiString UserStr)
{
  FZip->ShowZipMessage(Ident, UserStr);
}

void __fastcall TCustomZipBuilder::Unload_Unz_Dll(void)
{
  if(CanStart()) FZip->Unload_Unz_Dll();
}

void __fastcall TCustomZipBuilder::Unload_Zip_Dll(void)
{
  if(CanStart()) FZip->Unload_Zip_Dll();
}

#ifndef NO_SPAN
int __fastcall TCustomZipBuilder::ReadSpan(AnsiString InFileName,AnsiString& OutFilePath)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->ReadSpan(InFileName, OutFilePath, false);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}

int __fastcall TCustomZipBuilder::WriteSpan(AnsiString InFileName, AnsiString OutFileName)
{
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->WriteSpan(InFileName, OutFileName, false);
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}
#endif

#ifdef INTERNAL_SFX
int __fastcall TCustomZipBuilder::ConvertSFX(void)
{
//  if(CanStart()) return FZip->ConvertSFX();
//  return BUSY_ERROR;
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->ConvertSFX();
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}
#endif

#ifdef INTERNAL_SFX
int __fastcall TCustomZipBuilder::ConvertZIP(void)
{
//  if(CanStart()) return FZip->ConvertZIP();
//  return BUSY_ERROR;
  int Result = BUSY_ERROR;
  if(CanStart())
  {
    try
    {
      Starting();
      Result = FZip->ConvertZIP();
    }
    __finally
    {
      Done();
    }
  }
  return Result;
}
#endif

#ifndef NO_SFX
int __fastcall TCustomZipBuilder::IsZipSFX(const AnsiString SFXExeName)
{
  return FZip->IsZipSFX(SFXExeName);
}
#endif

void __fastcall TCustomZipBuilder::SetActive(bool Value)
{
  if(Active != Value) FActive = Value;
  if(ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading))
      FZip->Active = false;
  else FZip->Active = Active;
}

bool __fastcall TCustomZipBuilder::GetBusy(void)
{
  return FBusy;
}
#ifndef NO_SFX
//---------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetSFXIcon(TIcon *aIcon)
{
	FSFXIcon->Assign(aIcon);
}
#endif

//----------------------------------------------------------------------------
static inline TCustomZipBuilder *ValidCtrCheck()
{
	return new TCustomZipBuilder( NULL );
}
//----------------------------------------------------------------------------
namespace Zipbuilder
{
	void __fastcall PACKAGE Register()
  {
		TComponentClass classes[ 1 ] = {__classid( TZipBuilder ) };
		RegisterComponents( "ZipComponents", classes, 0 );
 	}
}




