/*
  ZBCore.cpp - Source file for event triggering
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

  last modified 2008-11-11
---------------------------------------------------------------------------*/
#include <vcl.h>
#pragma hdrstop

#include "CondDefines.h"
#include "ZBCore.h"
#include "ZipMsg.h"
#include "ZipUtils.h"
#include "ZBCtx.h"
#include "ZBDlg.h"
#include "ZBDelZip.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
const String RESOURCE_ERROR = "ZipMsgXX.res is probably not linked to the executable\nMissing String ID is: %d";

//----------------------------------------------------------------------------
void __fastcall TProgDetails::Advance(unsigned adv)
{
    FTotalPosition += adv;
    FItemPosition += adv;
    FProgType = ProgressUpdate;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::AdvanceXtra(unsigned adv)
{
    FItemPosition += adv;
    FProgType = ExtraUpdate;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::Clear(void)
{
    FProgType = EndOfBatch;
    FItemCount = 0;
    FWritten = 0;
    FTotalSize = 0;
    FTotalPosition = 0;
    FItemSize = 0;
    FItemPosition = 0;
    FItemName = "";
    FItemNumber = 0;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::SetCount(unsigned count)
{
    Clear();
    FItemCount = count;
    FItemNumber = 0;
    FProgType = TotalFiles2Process;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::SetEnd(void)
{
    FItemName = "";
    FItemSize = 0;
    FProgType = EndOfBatch;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::SetItem(const String& fname, unsigned fsize)
{
    ++FItemNumber;
    FItemName = fname;
    FItemSize = fsize;
    FItemPosition = 0;
    FProgType = NewFile;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::SetItemXtra(const String& xmsg, unsigned fsize)
{
    FItemName = xmsg;
    FItemSize = fsize;
    FItemPosition = 0;
    FProgType = NewExtra;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::SetSize(__int64 FullSize)
{
    FTotalSize = FullSize;
    FTotalPosition = 0;
    FItemName = "";
    FItemSize = 0;
    FItemPosition = 0;
    FProgType = TotalSize2Process;
    FWritten = 0;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::Written(__int64 bytes)
{
    FWritten = bytes;
}

// ============================ZBCore============================================
//---------------------------------------------------------------------------
void __fastcall TZBCore::Attacked(TObject* /*AnObject*/)
{
    FReenter = true;
    if (FVerbose)
        Report(zacMessage, 0, "Re-entry", 0);
}

/*? TZBCore::Call_Back---------------------------------------------------------
  return true if handled
*/
bool __fastcall TZBCore::Call_back(ActionCodes /*ActionCode*/, int /*ErrorCode*/,
                                   String /*Msg*/, unsigned /*FileSize*/)
{
    return false;
}
//? TZBCore::Call_Back

//---------------------------------------------------------------------------
void __fastcall TZBCore::Clear(void)
{
    SetZipMsgLanguage("");  // clear languages
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
void __fastcall TZBCore::Done(void)
{
    FBusy = false;
    if (!NotMainTask && (FCurWaitCount > 0))
    {
        FCurWaitCount = 0;
        Screen->Cursor = FSaveCursor;
    }
}
//---------------------------------------------------------------------------
TZipEventsRec* __fastcall TZBCore::GetEvents(void)
{
    return &FEvents;
}
//---------------------------------------------------------------------------
/*? TZBCore::GetMessage
1.73 13 July 2003 RP only return message if error
*/
String __fastcall TZBCore::GetMessage(void)
{
    String Result = "";
    if (FErrCode)
    {
        Result = FMessage;
        if (Result.IsEmpty())
            Result = ZipLoadStr(FErrCode);
        if (Result.IsEmpty())
            Result = ZipFmtLoadStr(GE_Unknown, ARRAYOFCONST((FErrCode)));
    }
    return Result;
}
//? TZBCore::GetMessage

//----------------------------------------------------------------------------
__int64 __fastcall TZBCore::GetTotalSizeToProcess(void)
{
    return FProgDetails->TotalSize;
}

// TZBCore::MakeTempFileName------------------------------------------------
// 1.73(22 May 2003) use of DelimitPath
// Make a temporary filename like: C:\...\zipxxxx.zip
// Prefix and extension are default: "zip" and ".zip"
String __fastcall TZBCore::MakeTempFileName(String Prefix, String Extension)
{
    String Result;
    TCHAR *Buffer = NULL;

    try
    {
        if (Prefix.IsEmpty())
            Prefix = "zip";
        if (Extension.IsEmpty())
            Extension = ".zip";
        if (!FTempDir.Length())
        { // Get the system temp dir
            // 1. The path specified by the TMP environment variable.
            // 2. The path specified by the TEMP environment variable, if TMP is not defined.
            // 3. The current directory, if both TMP and TEMP are not defined.
            DWORD len = ::GetTempPath(0, Buffer);
            Buffer = new TCHAR[(len + 12) * sizeof(TCHAR)];
            ::GetTempPath(len, Buffer);
        }
        else
        {        // Use Temp dir provided by ZipBuilder
            FTempDir = DelimitPath(FTempDir, true);
            Buffer = new TCHAR[(FTempDir.Length() + 13) * sizeof(TCHAR)];
            StrPLCopy(Buffer, FTempDir, FTempDir.Length() + 1);
        }
        if (::GetTempFileName(Buffer, Prefix.T_STR(), 0, Buffer))
        {
            ::DeleteFile(Buffer);  // Needed because GetTempFileName create                                                                s the file also.
            Result = ChangeFileExt(Buffer, Extension); // And finally change the extension.
        }
    }
    __finally
    {
        delete[] Buffer;
    }
    return Result;
}
// TZBCore::MakeTempFileName

/*? TZBCore.Report----------------------------------------------------------
1.77.2.0 14 September 2004 - RP fix setting ErrCode caused re-entry
1.77.2.0 14 September 2004 - RP alter thread support & OnCheckTerminate
1.77 16 July 2004 - RP preserve last errors message
1.76 24 April 2004 - only handle 'progress' and information
*/
void __fastcall TZBCore::Report(ActionCodes ActionCode, int ErrorCode,
                                String Msg, __int64 FileSize)
{
    bool DoStop;
    TProgressEvent tmpProgress;
    TProgressDetailsEvent tmpProgressDetails;
    TItemProgressEvent tmpItemProgress;
    TTotalProgressEvent tmpTotalProgress;
    TProgressDetails* Details = FProgDetails;
    TProgDetails* PDetails = static_cast<TProgDetails*>(FProgDetails);
    if (fIsDestructing)
        return;
    if (ActionCode != zacNone)
    {
        switch (ActionCode)
        {
            /*   ActionCode = 0, zacTick, just roll the loop */
            case zacTick:  // "Tick" Just checking / processing messages}
            {
                TTickEvent tmpTick =  FEvents.FOnTick;
                if (tmpTick)
                    tmpTick(Builder);
            }
            break;
            /*   ActionCode = 1, zacItem,
             *  we're starting a zip operation on a new file
             *   (O) FileSize(__int64) = filesize of file we're going to operate on
             *   (O) MsgP = pathname of file
             *   (O) Written (__int64) = [Zip] Bytes written
             *   IMPORTANT: The user's function must do the math for the progress
             *   bar upon getting this call.  See the CDemo sample application. */
            case zacItem:  // progress type 1 = starting any ZIP operation on a new file
                tmpProgress = FEvents.FOnProgress;
                if (tmpProgress)
#ifdef ALLOW_2G
                    tmpProgress(Builder, NewFile, Msg, FileSize);
#else
                    tmpProgress(Builder, NewFile, Msg, static_cast<int>(FileSize));
#endif
                PDetails->SetItem(Msg, static_cast<unsigned>(FileSize));
#ifdef DEBUG_PROGRESS
                if (Verbose)
                    Report(zacMessage, 0, Format("#Item - \"%s\" %d",
                            ARERAYOFCONST((Details->ItemName, Details->Itemsize)), 0);
#endif
                tmpProgressDetails = FEvents.FOnProgressDetails;
                if (tmpProgressDetails)
                    tmpProgressDetails(Builder, Details);
                tmpItemProgress = FEvents.FOnItemProgress;
                if (tmpItemProgress)
                    tmpItemProgress(Builder, Details->ItemName,
                            static_cast<unsigned>(FileSize), 0);
           break;
           /*   ActionCode = 2, zacProgress,
            *  increment the progress bar
            *      These calls will occur after every 32K of input file has been
            *      processed. One additional call is made at the end of each file,
            *      just to make sure the progress bar is max'ed out - this is also
            *      critical for files less than 32K in size (this last one will be
            *      their only one).
            *   (O) FileSize(__int64) = filesize of file we're going to operate on
            *   (O) Written (__int64) = [Zip] Bytes written     */
            case zacProgress:
                tmpProgress = FEvents.FOnProgress;
                if (FEvents.FOnProgress)
#ifdef ALLOW_2G
                   tmpProgress(Builder, ProgressUpdate, "", FileSize);
#else
                   tmpProgress(Builder, ProgressUpdate, "", static_cast<int>(FileSize));
#endif
                PDetails->Advance(static_cast<unsigned>(FileSize));
#ifdef DEBUG_PROGRESS
                if (Verbose)
                    Report(zacMessage, 0, Format("#Progress  - [inc:%d] ipos:%d isiz:%d, tpos:%d tsiz:%d",
                                                    ARERAYOFCONST((FileSize, Details->ItemPosition,
                                                                   Details->Itemsize, Details->TotalPosition,
                                                                   Details->TotalSize)), 0);
#endif
                tmpProgressDetails = FEvents.FOnProgressDetails;
                if (tmpProgressDetails)
                    tmpProgressDetails(Builder, Details);
                tmpItemProgress = FEvents.FOnItemProgress;
                if (tmpItemProgress)
                    tmpItemProgress(Builder, Details->ItemName,
                                                               Details->ItemPosition,
                                                               Details->ItemPerCent);
                tmpTotalProgress = FEvents.FOnTotalProgress;
                if (tmpTotalProgress)
                    tmpTotalProgress(Builder,
                                     static_cast<TProgressSize>(Details->TotalSize),
                                     Details->TotalPerCent);
            break;
            /*   ActionCode = 3, zacEndOfBatch,
             *  we're done with a batch of files
             *       - program flow will quickly return to the user's program.
             *   NOTE: the end of a every file will always be followed by an
             *         action of 1 or 3, so a separate call for end of one file
             *         isn't needed.
             *   (O) Written (__int64) = [Zip] Bytes written      */
            case zacEndOfBatch:
                tmpProgress = FEvents.FOnProgress;
                if (tmpProgress)
                    tmpProgress(Builder, EndOfBatch, "", 0);
                PDetails->SetEnd();
#ifdef DEBUG_PROGRESS
                if (Verbose)
                    Report(zacMessage, 0, "#End Of Batch", 0);
#endif
                tmpProgressDetails = FEvents.FOnProgressDetails;

                if (tmpProgressDetails)
                    tmpProgressDetails(Builder, Details);

                tmpItemProgress = FEvents.FOnItemProgress;

                if (tmpItemProgress)
                    tmpItemProgress(Builder, Details->ItemName, 0, 101);

                tmpTotalProgress = FEvents.FOnTotalProgress;

                if (tmpTotalProgress)
                    tmpTotalProgress(Builder, 0, 101);

            break;
            /*   ActionCode = 4, zacMessage, a routine message is being passed
            *   (O) Arg1 = error_code - code corresponding to message (not widely used yet)
            *   (O) MsgP = text of message    */
            case zacMessage:
            {
                String Erm = Msg;
                if (ErrorCode)
                {  // W'll always keep the last ErrorCode
                    FMessage = Msg;
                    FErrCode = static_cast<char>(ErrorCode & 0xFF);
                    if ((FErrCode == 9) && (!FEventErr.IsEmpty())) //user cancel
                    {
                        FMessage = ZipFmtLoadStr(GE_EventEx, ARRAYOFCONST((FEventErr)));
                        Erm = FMessage;
                    }
                    FFullErrCode = ErrorCode;
                }
                TMsgEvent tmpMessage = FEvents.FOnMessage;
                if (tmpMessage)
                    tmpMessage(Builder, ErrorCode, Erm);
            }
            break;
            /*  ActionCode = 5, zacCount,
             *         the total number of files is being passed.
             *   (O) Arg1 = The total number of files.  */
            case zacCount:
                PDetails->SetCount(static_cast<unsigned>(FileSize));
#ifdef DEBUG_PROGRESS
                if (Verbose)
                Report(zacMessage, 0, Format("#Count - %d",
                                             ARERAYOFCONST((Details->TotalCount)), 0);
#endif
                tmpProgressDetails = FEvents.FOnProgressDetails;
                if (tmpProgressDetails)
                    tmpProgressDetails(Builder, Details);
                tmpProgress = FEvents.FOnProgress;
                if (tmpProgress)
#ifdef ALLOW_2G
                    tmpProgress(Builder, TotalFiles2Process, "", FileSize);
#else
                    tmpProgress(Builder, TotalFiles2Process, "", static_cast<int>(FileSize));
#endif
            break;
            /*   ActionCode = 6, zacSize,
             *  the total file size is being passed.
             *   (O) FileSize (__int64 total file size   */
            case zacSize:
                PDetails->SetSize(FileSize);
#ifdef DEBUG_PROGRESS
                if (Verbose)
                   Report(zacMessage, 0, Format("#Size - %d",
                                                ARERAYOFCONST((Details->TotalSize)), 0);
#endif
                tmpProgressDetails = FEvents.FOnProgressDetails;
                if (tmpProgressDetails)
                    tmpProgressDetails(Builder, Details);
                tmpProgress = FEvents.FOnProgress;
                if (tmpProgress)
#ifdef ALLOW_2G
                    tmpProgress(Builder, TotalSize2Process, "", FileSize);
#else
                    tmpProgress(Builder, TotalSize2Process, "", static_cast<int>(FileSize));
#endif
            break;
            /*   ActionCode = 15, zacXItem, we're starting a zip operation on a new file
             *   (O) Arg1 = type - 1 = archive bit 2 = move file
             *   (O) Filesize = filesize of file we're going to operate on
             *   (O) MsgP = pathname of file             */
            case zacXItem:
                tmpProgress = FEvents.FOnProgress;
                if (tmpProgress)
#ifdef ALLOW_2G
                    tmpProgress(Builder, NewExtra, Msg, FileSize);
#else
                    tmpProgress(Builder, NewExtra, Msg, static_cast<int>(FileSize));
#endif
                PDetails->SetItemXtra(ZipLoadStr(PR_Progress + ErrorCode), static_cast<unsigned>(FileSize));
#ifdef DEBUG_PROGRESS
                if (Verbose)
                    Report(zacMessage, 0, Format("#XItem - %s size = %d",
                                               ARERAYOFCONST((Details->ItemName, FileSize)), 0);
#endif
                tmpProgressDetails = FEvents.FOnProgressDetails;
                if (tmpProgressDetails)
                    tmpProgressDetails(Builder, Details);
                tmpItemProgress = FEvents.FOnItemProgress;
                if (tmpItemProgress)
                    tmpItemProgress(Builder, Details->ItemName, static_cast<unsigned>(FileSize), 0);
            break;
             /*   ActionCode = 16, zacXProgress, increment the progress bar
              *      These calls will occur after every 32K of input file has been
              *      processed. One additional call is made at the end of each file,
              *      just to make sure the progress bar is max'ed out - this is also
              *      critical for files less than 32K in size (this last one will be
              *      their only one).
              *   (O) Arg1 = type
              *   (O) FileSize (__int64) = size           */
            case zacXProgress:
                tmpProgress = FEvents.FOnProgress;
                if (tmpProgress)
#ifdef ALLOW_2G
                    tmpProgress(Builder, ExtraUpdate, Details->ItemName, FileSize);
#else
                    tmpProgress(Builder, ExtraUpdate, Details->ItemName, static_cast<int>(FileSize));
#endif
                PDetails->AdvanceXtra(static_cast<unsigned>(FileSize));
#ifdef DEBUG_PROGRESS
                if (Verbose)
                    Report(zacMessage, 0, Format("#XProgress - [inc:%d] pos:%d size: %d",
                                                  ARERAYOFCONST((FileSize, Details->Position, Details->Itemsize)), 0);
#endif
                tmpProgressDetails = FEvents.FOnProgressDetails;
                if (tmpProgressDetails)
                    tmpProgressDetails(Builder, Details);
                tmpItemProgress = FEvents.FOnItemProgress;
                if (tmpItemProgress)
                    tmpItemProgress(Builder, Details->ItemName, Details->ItemSize, Details->ItemPerCent);
            break;
            default:  // unhandled event
                Call_back(ActionCode, ErrorCode, Msg, static_cast<unsigned>(FileSize));
        }  //end switch
    }
    TCheckTerminateEvent tmpCheckTerminate = FEvents.FOnCheckTerminate;
    if (tmpCheckTerminate)
    {
        DoStop = Cancel;
        tmpCheckTerminate(Builder, DoStop);
        if (DoStop)
            Cancel = true;
    }
    else
        if (!FNotMainTask)
            Application->ProcessMessages();
}
//? TZBCore::Report

//---------------------------------------------------------------------------
void __fastcall TZBCore::SetCancel(bool Value)
{
    FCancel = Value;
}

/*? TZBCore::SetTempDir--------------------------------------------
1.79 23 Dec 2004 RA new setter
*/
void __fastcall TZBCore::SetTempDir(const String value)
{
    if (value != FTempDir)
    {
        if (!value.IsEmpty())
        {
            String Drive = ExtractFileDrive(value) + "\\";
            int drt = ::GetDriveType(Drive.T_STR());
            // only fixed drive or ramdisk is fast enough to be used as tempdir
            if ((drt != DRIVE_FIXED) && (drt != DRIVE_RAMDISK))
                throw EZipBuilder(DS_NotChangeable, Drive);
        }
        FTempDir = value;
    }
}
//? TZBCore::SetTempDir

/*? TZBCore.ShowExceptionError-------------------------------------------------
1.80 strings already formatted
1.73 10 July 2003 RP translate exception messages (again)
 Somewhat different from ShowZipMessage() because the loading of the resource
 string is already done in the constructor of the exception class.
*/
void __fastcall TZBCore::ShowExceptionError(const EZipBuilder &ZBExcept)
{
    FErrCode = ZBExcept.Ident;
    FMessage = ZBExcept.Message;
    if (ZBExcept.FDisplayMsg && !FUnattended)
        ZipMessageDlg(FMessage, zmtError + DHC_ExMessage);
    TMsgEvent tmpMessage = FEvents.FOnMessage;
    if (tmpMessage)
        tmpMessage(Builder, FErrCode, FMessage);
}
//? TZBCore.ShowExceptionError

/*? TZBCore.ShowZipMessage-----------------------------------------------------
*/
void __fastcall TZBCore::ShowZipMessage(int Ident, const String UserStr)
{
    TMsgEvent tmpMessage = FEvents.FOnMessage;
    if (!FUnattended || tmpMessage)
    {
        String Msg = ZipLoadStr(Ident);
        if (Msg.IsEmpty())
            Msg = Format(RESOURCE_ERROR, ARRAYOFCONST((Ident)));
        Msg += UserStr;
        FMessage = Msg;
        ErrCode = Ident;
        if (!Unattended)
            ZipMessageDlg(Msg, zmtInformation + DHC_ZipMessage);
        if (tmpMessage)
            tmpMessage(Builder, FErrCode, Msg);
    }
}
//? TZBCore.ShowZipMessage

/*? TZBCore.ShowZipFmtMessage ------------------------------------------------
1.79 RP added
*/
void __fastcall TZBCore::ShowZipFmtMessage(int Ident, const System::TVarRec* Args,
        const int Args_Size)
{
    TMsgEvent tmpMessage = FEvents.FOnMessage;
    if (!FUnattended || tmpMessage)
    {
        FMessage = ZipFmtLoadStr(Ident, Args, Args_Size);
        ErrCode  = Ident;
        if (!FUnattended)
            ZipMessageDlg(FMessage, zmtInformation + DHC_ZipMessage);
        if (tmpMessage)
            tmpMessage(Builder, FErrCode , FMessage);
    }
}
//? TZBCore.ShowZipFmtMessage

/*TZBCore::Starting----------------------------------------------------------
*/
void __fastcall TZBCore::Starting(void)
{
    FReenter = false;
    FBusy = true;
    FAnswerAll.Clear();
    if (GetCurrentThreadId() != MainThreadID)
        NotMainTask = true;
}
//? TZBCore::Starting

/*? TZBCore::StartWaitCursor
1.75.0.5 10 March 2004 only set wait if forground task
*/
void __fastcall TZBCore::StartWaitCursor(void)
{
    if (!FNotMainTask)
    {
        if (!FCurWaitCount)
        {
            FSaveCursor = Screen->Cursor;
            Screen->Cursor = crHourGlass;
        }
        ++FCurWaitCount;
    }
}
//? TZBCore::StartWaitCursor

/*? TZBCore::StopWaitCursor
1.75.0.5 10 March 2004 only set wait if forground task
*/
void __fastcall TZBCore::StopWaitCursor(void)
{
    if (!FNotMainTask && FCurWaitCount)
    {
        if (--FCurWaitCount == 0)
            Screen->Cursor = FSaveCursor;
    }
}
//? TZBCore::StopWaitCursor

//----------------------------------------------------------------------------
__fastcall TZBCore::TZBCore(TObject* ABuilder)
{
    Builder = ABuilder;
    FActive = false;
    FProgDetails = new TProgDetails();
    FMessage      = "";
    FErrCode      = -1;
    FUnattended   = true;                      // during construction
    FCurWaitCount = 0;
    FVerbose = false;
    FTrace = false;
    FTempDir = "";
    FNotMainTask = false;
}
/* TZBCore::ZipMessageDialog -------------------------------------------------
*/
TModalResult __fastcall TZBCore::ZipMessageDialog(const String& Title, String& msg,
        int Context, TMsgDlgButtons Btns)
{
    String t = Title;
    if (t.IsEmpty())
        t = Application->Title;
    if (Verbose)
    {
        t = Format("%s   (%d)", ARRAYOFCONST((t, Context & 0xFFFF)));
        Report(zacMessage, 0, t, 0);
    }
    TZipDialogEvent tmpZipDialog = FEvents.FOnZipDialog;
    if (tmpZipDialog)
    {
        String s = msg;
        int ctx = Context;
        tmpZipDialog(Builder, t, s, ctx, Btns);
        if ((ctx > 0) && (ctx <= mrYesToAll))
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
        if (dlg->DlgType == zmtPassword)
        {
            msg = (Result == mrOk) ? dlg->PWrd : String("");
        }
    }
    __finally
    {
        delete dlg;
    }
    return Result;
}
//? TZBCore::ZipMessageDialog

/* TZBCore::ZipMessageDlg -------------------------------------------------
*/
void __fastcall TZBCore::ZipMessageDlg(const String& msg, int context)
{
    ZipMessageDlg("", msg, context, TMsgDlgButtons() << mbOK);
}
//? TZBCore::ZipMessageDlg

/* TZBCore::ZipMessageDlg -------------------------------------------------
*/
TModalResult __fastcall TZBCore::ZipMessageDlg(const String& Title, const String& msg,
        int context, TMsgDlgButtons Btns)
{
    String m = msg;
    return ZipMessageDialog(Title, m, context, Btns);
}
//? TZBCore::ZipMessageDlg

//---------------------------------------------------------------------------
__fastcall TZBCore::~TZBCore(void)
{
    delete FProgDetails;
}