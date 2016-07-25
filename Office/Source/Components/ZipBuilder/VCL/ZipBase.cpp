//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CondDefines.h"
#include "ZipBase.h"                                   
#include "ZipMsg.h"
#include "ZipStrs.h"
#include "ZipUtils.h"
#include "ZipCtx.h"
#include "ZipDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// Added to compile/build under BCB6
#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

//----------------------------------------------------------------------------
__int64 __fastcall TZipBase::GetTotalSizeToProcess(void)
{
  return static_cast<TProgDetails*>(FProgDetails)->TotalSize;
}
//----------------------------------------------------------------------------
__fastcall TZipBase::TZipBase(void)
{
  Builder = this;
  FActive = false;
  FProgDetails = new TProgDetails();
  FMessage      = "";
  FErrCode      = -1;
  FUnattended   = true;                      // during construction
  FCurWaitCount = 0;
  FVerbose = false;
  FTrace = false;
  FDLLDirectory = "";
  FTempDir = "";
  FNotMainTask = false;
  FAnswerAll.Clear();
  FOnMessage = NULL;
  FOnProgress = NULL;
  FOnCheckTerminate = NULL;
  FOnTick = NULL;
  FOnTotalProgress = NULL;
  FOnItemProgress = NULL;
  FOnProgressDetails = NULL;
  FOnZipDialog = NULL;
}
//---------------------------------------------------------------------------
__fastcall /*virtual*/ TZipBase::~TZipBase(void)
{
	delete FProgDetails;
}
//---------------------------------------------------------------------------
void __fastcall TZipBase::Attacked(TObject* /*AnObject*/)
{
  FReenter = true;
  if(FVerbose) Report(zacMessage, 0, "Re-entry", 0);
}
/*TZipBase::Starting----------------------------------------------------------
*/
void __fastcall TZipBase::Starting(void)
{
  FReenter = false;
  FBusy = true;
  FAnswerAll.Clear();
}
//? TZipBase::Starting
//---------------------------------------------------------------------------
void __fastcall TZipBase::Done(void)
{
  FBusy = false;
}
//---------------------------------------------------------------------------
void __fastcall TZipBase::Clear(void)
{
  FReenter = false;
  FMessage = "";
  FCancel = false;
  FErrCode = 0;
  FFullErrCode = 0;
  FUnattended = false;
  FVerbose = false;
  FTrace = false;
  static_cast<TProgDetails*>(FProgDetails)->Clear();
  FEventErr = "";
  fIsDestructing = false;
}
//---------------------------------------------------------------------------
void __fastcall TZipBase::SetCancel(bool Value)
{
  FCancel = Value;
}
//---------------------------------------------------------------------------
 // NOTE: we will allow a dir to be specified that doesn't exist,
 // since this is not the only way to locate the DLLs.
void __fastcall TZipBase::SetDLLDirectory(AnsiString Value)
{
  if(Value != FDLLDirectory)
  {
    int ValLen = Value.Length();
    // if there is a trailing \ in dirname, cut it off:
    if(ValLen && Value[ValLen] == '\\')
        Value.SetLength(ValLen - 1);       // shorten the dirname by one
    FDLLDirectory = Value;
  }
}
//---------------------------------------------------------------------------
/*? TZipBase::GetMessage
1.73 13 July 2003 RP only return message if error
*/
AnsiString __fastcall TZipBase::GetMessage(void)
{
  AnsiString Result = "";
  if(FErrCode)
  {
  	Result = FMessage;
    if(Result == "")
      Result = ZipLoadStr(FErrCode);
    if(Result == "")
      Result = ZipFmtLoadStr(GE_Unknown,ARRAYOFCONST((FErrCode)));
  }
  return Result;
}
//? TZipBase::GetMessage

/*? TZipBase.ShowZipMessage-----------------------------------------------------
*/
void __fastcall TZipBase::ShowZipMessage(int Ident, AnsiString UserStr)
{
  AnsiString Msg = ZipLoadStr(Ident);
  if(Msg.IsEmpty()) Msg = Format(RESOURCE_ERROR, ARRAYOFCONST((Ident)));
  Msg += UserStr;
  FMessage = Msg;
  ErrCode = Ident;
  if(!Unattended) ZipMessageDlg(Msg, zmtInformation + DHC_ZipMessage);
  if(OnMessage) OnMessage(Builder, FErrCode, Msg);
}
//? TZipBase.ShowZipMessage

/*? TZipBase.ShowExceptionError-------------------------------------------------
1.80 strings already formatted
1.73 10 July 2003 RP translate exception messages (again)
 Somewhat different from ShowZipMessage() because the loading of the resource
 string is already done in the constructor of the exception class.
*/
void __fastcall TZipBase::ShowExceptionError(const EZipBuilder &ZBExcept)
{
  FErrCode = ZBExcept.Ident;
  FMessage = ZBExcept.Message;
  if(ZBExcept.FDisplayMsg && !FUnattended) ZipMessageDlg(FMessage, zmtError + DHC_ExMessage);
  if(OnMessage) OnMessage(Builder, FErrCode, FMessage);
}
//? TZipBase.ShowExceptionError

/*? TZipBase::StartWaitCursor
1.75.0.5 10 March 2004 only set wait if forground task
*/
void __fastcall TZipBase::StartWaitCursor(void)
{
  if(!FNotMainTask)
  {
  	if(!FCurWaitCount)
  	{
    	FSaveCursor = Screen->Cursor;
    	Screen->Cursor = crHourGlass;
  	}
  	++FCurWaitCount;
  }
}
//? TZipBase::StartWaitCursor

/*? TZipBase::StopWaitCursor
1.75.0.5 10 March 2004 only set wait if forground task
*/
void __fastcall TZipBase::StopWaitCursor(void)
{
  if(!FNotMainTask && FCurWaitCount)
  {
    if(--FCurWaitCount == 0)
      Screen->Cursor = FSaveCursor;
  }
}
//? TZipBase::StopWaitCursor

/*? TZipBase.Call_Back---------------------------------------------------------
  return true if handled
*/
bool __fastcall TZipBase::Call_back(ActionCodes /*ActionCode*/, int /*ErrorCode*/,
                                    AnsiString /*Msg*/, unsigned /*FileSize*/)
{
  return false;
}
//? TZipBase.Call_Back


/*? TZipBase.Report----------------------------------------------------------
1.77.2.0 14 September 2004 - RP fix setting ErrCode caused re-entry
1.77.2.0 14 September 2004 - RP alter thread support & OnCheckTerminate
1.77 16 July 2004 - RP preserve last errors message
1.76 24 April 2004 - only handle 'progress' and information
*/
void __fastcall TZipBase::Report(ActionCodes ActionCode, int ErrorCode,
                                   AnsiString Msg, __int64 FileSize)
{
  bool DoStop;
  TProgressDetails* Details = static_cast<TProgressDetails*>(FProgDetails);
  TProgDetails* PDetails = static_cast<TProgDetails*>(FProgDetails);
  if(fIsDestructing) return;
  if(ActionCode != zacNone)
  {
  	switch(ActionCode)
  		{
    	case zacTick:              // "Tick" Just checking / processing messages}
    	    if(OnTick) OnTick(Builder);
    	break;
    	case zacItem:  // progress type 1 = starting any ZIP operation on a new file
    	  if(OnProgress)
#ifdef ALLOW_2G
    	     OnProgress(Builder, NewFile, Msg, FileSize);
#else
           OnProgress(Builder, NewFile, Msg, static_cast<int>(FileSize));
#endif
        PDetails->SetItem(Msg, static_cast<unsigned>(FileSize));
#ifdef DEBUG_PROGRESS
           if(Verbose)
           	Report(zacMessage, 0, Format("#Item - \"%s\" %d",
           					ARERAYOFCONST((Details->ItemName, Details->Itemsize)), 0);
#endif
        if(OnProgressDetails) OnProgressDetails(Builder, Details);
    	  if(OnItemProgress) OnItemProgress(Builder, Details->ItemName, static_cast<unsigned>(FileSize),0);
    	break;
    	  // Progress type 2 = increment bar.
    	case zacProgress:
    	  if(OnProgress)
#ifdef ALLOW_2G
    	    OnProgress(Builder, ProgressUpdate, "",FileSize );
#else
          OnProgress(Builder, ProgressUpdate, "", static_cast<int>(FileSize));
#endif
        PDetails->Advance(static_cast<unsigned>(FileSize));
#ifdef DEBUG_PROGRESS
           if(Verbose)
           	Report(zacMessage, 0, Format("#Progress  - [inc:%d] ipos:%d isiz:%d, tpos:%d tsiz:%d",
           					ARERAYOFCONST((FileSize, Details->ItemPosition,
           					               Details->Itemsize, Details->TotalPosition,
           					               Details->TotalSize)), 0);
#endif
        if(OnProgressDetails) OnProgressDetails(Builder, Details);
    	  if(OnItemProgress) OnItemProgress(Builder, Details->ItemName, Details->ItemPosition,
                                Details->ItemPerCent);
        if(OnTotalProgress) OnTotalProgress(Builder,
                static_cast<TProgressSize>(Details->TotalSize), Details->TotalPerCent);
    	break;
    	// End of a batch of 1 or more files.
    	case zacEndOfBatch:
    	  if(OnProgress) OnProgress(Builder, EndOfBatch, "", 0);
				PDetails->SetEnd();
#ifdef DEBUG_PROGRESS
           if(Verbose)
           	Report(zacMessage, 0, "#End Of Batch", 0);
#endif
				if(OnProgressDetails) OnProgressDetails(Builder, Details);
    	  if(OnItemProgress) OnItemProgress(Builder,Details->ItemName,0,101);
    	  if(OnTotalProgress) OnTotalProgress(Builder,0,101);
    	break;
    	case zacMessage:	// Show a routine status message.
      {
        AnsiString Erm = Msg;
    	  if(ErrorCode)
    	  {  // W'll always keep the last ErrorCode
          FMessage = Msg;
    	    FErrCode = (int)(char)(ErrorCode & 0xFF);
    	    if((FErrCode == 9) && (FEventErr != "")) //user cancel
          {
            FMessage = ZipFmtLoadStr(GE_EventEx, ARRAYOFCONST((FEventErr)));
            Erm = FMessage;
          }
    	    FFullErrCode = ErrorCode;
    	  }
    	  if(OnMessage) OnMessage(Builder, ErrorCode, Erm);
      }
    	break;
    	// The total number of files which will be processed.
    	case zacCount:
    		PDetails->SetCount(static_cast<unsigned>(FileSize));
#ifdef DEBUG_PROGRESS
           if(Verbose)
           	Report(zacMessage, 0, Format("#Count - %d",
           					ARERAYOFCONST((Details->TotalCount)), 0);
#endif
    		if(OnProgressDetails) OnProgressDetails(Builder, Details);
        if(OnProgress)
#ifdef ALLOW_2G
    	  	OnProgress(Builder, TotalFiles2Process, "", FileSize );
#else
					OnProgress(Builder, TotalFiles2Process, "", static_cast<int>(FileSize));
#endif
    	break;
    	case zacSize:
    		PDetails->SetSize(FileSize);
#ifdef DEBUG_PROGRESS
           if(Verbose)
           	Report(zacMessage, 0, Format("#Size - %d",
           					ARERAYOFCONST((Details->TotalSize)), 0);
#endif
    		if(OnProgressDetails) OnProgressDetails(Builder, Details);
    	  if(OnProgress)
#ifdef ALLOW_2G
    	  	OnProgress(Builder, TotalSize2Process, "", FileSize);
#else
     			OnProgress(Builder, TotalSize2Process, "", static_cast<int>(FileSize));
#endif
    	break;
    	case zacXItem:          // progress type 15 = starting new extra operation
    	{
    		if(OnProgress)
#ifdef ALLOW_2G
    	  	OnProgress(Builder, NewExtra, Msg, FileSize);
#else
					OnProgress(Builder, NewExtra, Msg, static_cast<int>(FileSize));
#endif
         PDetails->SetItemXtra(ErrorCode, Msg, static_cast<unsigned>(FileSize));
#ifdef DEBUG_PROGRESS
           if(Verbose)
           	Report(zacMessage, 0, Format("#XItem - %s size = %d",
           					ARERAYOFCONST((Details->ItemName, FileSize)), 0);
#endif
         if(OnProgressDetails) OnProgressDetails(Builder, Details);
    	   if(OnItemProgress)
    	         OnItemProgress(Builder,Details->ItemName,static_cast<unsigned>(FileSize),0);
    	}
    	break;
    	case zacXProgress:  // progress type 16 = increment bar for extra operation
    	{
    		if(OnProgress)
#ifdef ALLOW_2G
    	  	OnProgress(Builder, ExtraUpdate, Details->ItemName, FileSize);
#else
					OnProgress(Builder, ExtraUpdate, Details->ItemName, static_cast<int>(FileSize));
#endif
        PDetails->AdvanceXtra(static_cast<unsigned>(FileSize));
#ifdef DEBUG_PROGRESS
           if(Verbose)
           	Report(zacMessage, 0, Format("#XProgress - [inc:%d] pos:%d size: %d",
           					ARERAYOFCONST((FileSize, Details->Position, Details->Itemsize)), 0);
#endif
        if(OnProgressDetails) OnProgressDetails(Builder, Details);
    	  if(OnItemProgress)
					    OnItemProgress(Builder, Details->ItemName, Details->ItemSize, Details->ItemPerCent);
    	}
    	break;
    	default:  // unhandled event
    	  Call_back(ActionCode,ErrorCode,Msg,static_cast<unsigned>(FileSize));
  	}  //end switch
  }
  if(OnCheckTerminate)
  {
    DoStop = Cancel;
    OnCheckTerminate(Builder, DoStop);
    if(DoStop) Cancel = true;
  }
  else if(!FNotMainTask) Application->ProcessMessages();
}
//? TZipBase::Report

/*? TZipBase::Report-------------------------------------------------------------
1.76 4 July 2004 - handle written counts
*/
void __fastcall TZipBase::Report(ActionCodes ActionCode, int ErrorCode,
                                   AnsiString Msg, __int64 FileSize, unsigned written)
{
  static_cast<TProgDetails*>(FProgDetails)->Wrote(written);
  Report(ActionCode, ErrorCode, Msg, FileSize);
}
//? TZipBase::Report

// TZipBase::MakeTempFileName------------------------------------------------
// 1.73(22 May 2003) use of DelimitPath
// Make a temporary filename like: C:\...\zipxxxx.zip
// Prefix and extension are default: "zip" and ".zip"
AnsiString __fastcall TZipBase::MakeTempFileName(AnsiString Prefix, AnsiString Extension)
{
	AnsiString  Result = "";
	char *Buffer = NULL;

	try
	{
		if(Prefix == "") Prefix = "zip";
		if(Extension == "") Extension = ".zip";
		if(!FTempDir.Length())
		{	// Get the system temp dir
			// 1.	The path specified by the TMP environment variable.
			//	2.	The path specified by the TEMP environment variable, if TMP is not defined.
			//	3.	The current directory, if both TMP and TEMP are not defined.
			DWORD len = ::GetTempPath(0, Buffer);
			Buffer = new char[max((int)len, MAX_PATH)];
			::GetTempPath(len, Buffer);
		}
		else
		{								// Use Temp dir provided by ZipBuilder
      FTempDir = DelimitPath(FTempDir, true);
			Buffer = new char[max(FTempDir.Length() + 13, MAX_PATH)];
			StrPLCopy(Buffer, FTempDir, FTempDir.Length() + 1);
		}
		if(::GetTempFileName(Buffer, Prefix.c_str(), 0, Buffer))
		{
			::DeleteFile(Buffer);		// Needed because GetTempFileName create                                                                s the file also.
			Result = ChangeFileExt(Buffer, Extension);	// And finally change the extension.
		}
	}
	catch( ... ) { }
	delete[] Buffer;
	return Result;
}
// TZipBase::MakeTempFileName

/* TZipBase::ZipMessageDialog -------------------------------------------------
*/
TModalResult __fastcall TZipBase::ZipMessageDialog(const AnsiString Title, AnsiString& msg,
                                                  int Context, TMsgDlgButtons Btns)
{
  AnsiString t = Title;
  if(t == "") t = Application->Title;
  if(Verbose)
  {
    t = Format("%s   (%d)",ARRAYOFCONST((Title, Context & 0xFFFF)));
    Report(zacMessage,0,t,0);
  }
  if(OnZipDialog)
  {
    AnsiString s = msg;
    int ctx = Context;
    OnZipDialog(Builder, t, s, ctx, Btns);
    if((ctx > 0) && (ctx <= mrYesToAll))
    {
      msg = s; // return password
      return static_cast<TModalResult>(ctx);
    }
  }
  TZipDialogBox* dlg = new TZipDialogBox(Application, Context);
  TModalResult Result;
  try
  {
    dlg->Build(t, msg, Btns);
    dlg->ShowModal();
    Result = dlg->ModalResult;
    if(dlg->DlgType == zmtPassword)
    {
      if(Result == mrOk) msg = dlg->PWrd;
      else msg = "";
    }
  }
  __finally
  {
    delete dlg;
  }
  return Result;
}
//? TZipBase::ZipMessageDialog

/* TZipBase::ZipMessageDlg -------------------------------------------------
*/
TModalResult __fastcall TZipBase::ZipMessageDlg(const AnsiString Title,AnsiString msg,
                                                int context, TMsgDlgButtons Btns)
{
  AnsiString m = msg;
  return ZipMessageDialog(Title, m, context, Btns);
}
//? TZipBase::ZipMessageDlg

/* TZipBase::ZipMessageDlg -------------------------------------------------
*/
void __fastcall TZipBase::ZipMessageDlg(const AnsiString msg, int context)
{
  ZipMessageDlg("", msg, context, TMsgDlgButtons() << mbOK);
}
//? TZipBase::ZipMessageDlg


