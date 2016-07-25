#ifndef ZBCoreH
#define ZBCoreH
/*
  ZBCore.h - Header file of core unit of TZipBuilder
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

  last modified 2009-02-13
---------------------------------------------------------------------------*/

#include "EZipBuilder.h"
#include "ZipProg.h"
#include "ZBDlg.h"
#include "ZBCompat.h"
#include "ZBStructs.h"


namespace Zbcore
{
//-- type declarations -------------------------------------------------------

#pragma option -b-
enum ActionCodes {zacTick, zacItem, zacProgress,zacEndOfBatch, zacMessage, zacCount,
                  zacSize, zacNewName, zacPassword, zacCRCError, zacOverwrite, zacSkipped,
                  zacComment, zacStream, zacData, zacXItem, zacXProgress, zacExtName,
                  zacKey, zacNone};
enum ProgressTypes { zprFile, zprArchive, zprCopyTemp, zprSFX, zprHeader, zprFinish,
                     zprCompressed, zprCentral, zprChecking, zprLoading, zprJoining,
                     zprSplitting   };
enum TZipShowProgress {zspNone, zspExtra, zspFull};
enum TZipAllwaysItems {zaaYesOvrwrt, zyaDummy};
#pragma option -b.
typedef Set<TZipAllwaysItems, zaaYesOvrwrt, zyaDummy>  TZipAnswerAlls;
#pragma option -b-
enum TZipDiskAction	{zdaOk, zdaErase, zdaReject, zdaCancel, zdaYesToAll};
enum ZipDiskStatusEnum {zdsEmpty, zdsHasFiles, zdsPreviousDisk, zdsSameFileName, zdsNotEnoughSpace };
enum UnZipSkipTypes	{stOnFreshen, stNoOverwrite, stFileExists, stBadPassword, stNoEncryptionDLL,
							 	  stCompressionUnknown, stUnknownZipHost, stZipFileFormatWrong, stGeneralExtractError };
#pragma option -b.
typedef Set<ZipDiskStatusEnum, zdsEmpty, zdsNotEnoughSpace> TZipDiskStatus;



typedef void __fastcall (__closure *TCheckTerminateEvent)(TObject* Sender, bool &abort);
typedef void __fastcall (__closure *TCopyZipOverwriteEvent)(System::TObject *Sender, String ForFile,
                                                             bool &DoOverwrite );
typedef void __fastcall (__closure *TCRC32ErrorEvent)(System::TObject *Sender, String ForFile,
                                                       unsigned FoundCRC, unsigned ExpectedCRC,
                                                       bool &DoExtract );
typedef void __fastcall (__closure *TExtractOverwriteEvent)(System::TObject *Sender, String ForFile,
                                                            bool IsOlder, bool &DoOverwrite, int DirIndex );
typedef void __fastcall (__closure *TExtractSkippedEvent)(System::TObject *Sender, String ForFile,
                                                          UnZipSkipTypes SkipType, int ExtError );
typedef void __fastcall (__closure *TFileCommentEvent)(System::TObject *Sender, String ForFile,
                                                      String &FileComment, bool &IsChanged );
typedef void __fastcall (__closure *TFileExtraEvent)(TObject *Sender,String ForFile, RawByteString& Data,
                                                     bool &IsChanged);
typedef void __fastcall (__closure *TGetNextDiskEvent)( System::TObject *Sender, int DiskSeqNo, int DiskTotal,
                                                       String Drive, bool &Cancel );
typedef void __fastcall (__closure *TItemProgressEvent)(TObject* Sender, String Item,
                                                        unsigned TotalSize, int PerCent);
typedef void __fastcall (__closure *TMsgEvent)(TObject* Sender, int ErrCode, String Message);
typedef void __fastcall (__closure *TNewNameEvent)( System::TObject *Sender, int SeqNo,const ZipDirEntry* ZipEntry );
typedef void __fastcall (__closure *TPasswordErrorEvent)( System::TObject *Sender, bool IsZipAction, String &NewPassword,
                                                         String ForFile, unsigned &RepeatCount, TMsgDlgBtn &Action );
typedef void __fastcall (__closure *TProgressDetailsEvent)(TObject* Sender, TProgressDetails* Details);
typedef void __fastcall (__closure *TProgressEvent)(TObject* Sender, ProgressType ProgrType,
	                                                  String Filename,
                                                    TProgressSize FileSize);
typedef void __fastcall (__closure *TSetAddNameEvent)(TObject* Sender, String& FileName,
                                                       const String ExtName, bool& IsChanged);
typedef void __fastcall (__closure *TSetExtNameEvent)(TObject* Sender, String& Filename, bool& IsChanged);
typedef void __fastcall (__closure *TSetNewNameEvent)( System::TObject *Sender, String &OldFileName, bool &IsChanged );
typedef void __fastcall (__closure *TStatusDiskEvent)( System::TObject *Sender, int PreviousDisk, String PreviousFile, TZipDiskStatus Status, TZipDiskAction &Action );
typedef void __fastcall (__closure *TTickEvent)(TObject* Sender);
typedef void __fastcall (__closure *TTotalProgressEvent)(TObject* Sender, TProgressSize TotalSize, int PerCent);
typedef void __fastcall (__closure *TZipDialogEvent)(TObject* Sender, const String Title,
                                                     String& msg, int& Context, TMsgDlgButtons Btns);
#pragma option -a1
struct TZipEventsRec
{
  TCheckTerminateEvent FOnCheckTerminate;
  TCopyZipOverwriteEvent FOnCopyZipOverwrite;
  TCRC32ErrorEvent FOnCRC32Error;
  Classes::TNotifyEvent		FOnDirUpdate;
  TExtractOverwriteEvent FOnExtractOverwrite;
  TExtractSkippedEvent FOnExtractSkipped;
  TFileCommentEvent FOnFileComment;
  TFileExtraEvent FOnFileExtra;
#ifndef NO_SPAN
  TGetNextDiskEvent FOnGetNextDisk;
#endif
  TItemProgressEvent FOnItemProgress;
  TMsgEvent FOnMessage;
  TNewNameEvent FOnNewName;
  TPasswordErrorEvent FOnPasswordError;
  TProgressEvent FOnProgress;
  TProgressDetailsEvent FOnProgressDetails;
#ifndef NO_SPAN
  TStatusDiskEvent FOnStatusDisk;
#endif
  TSetAddNameEvent FOnSetAddName;
  TSetExtNameEvent FOnSetExtName;
  TSetNewNameEvent FOnSetNewName;
  TTickEvent FOnTick;
  TTotalProgressEvent FOnTotalProgress;
  TZipDialogEvent FOnZipDialog;
};
#pragma option -a.

#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif

class __declspec(delphiclass) TProgDetails : public TProgressDetails
{
public:
  __fastcall TProgDetails(void) {};
	__fastcall virtual ~TProgDetails( void ) {};
  void __fastcall Advance(unsigned adv);
  void __fastcall AdvanceXtra(unsigned adv);
  virtual void __fastcall Clear(void);
  void __fastcall SetCount(unsigned count);
  void __fastcall SetEnd(void);
  void __fastcall SetItem(const String& fname, unsigned fsize);
  void __fastcall SetItemXtra(const String& xmsg, unsigned fsize);
  void __fastcall SetSize(__int64 FullSize);
  void __fastcall Written(__int64 bytes);
};

class __declspec(delphiclass) TZBCore : public TObject
{
private:
	int FCurWaitCount;
  TCursor FSaveCursor;
  TZipEventsRec* __fastcall GetEvents(void);
  String __fastcall GetMessage(void);
  void __fastcall SetTempDir(const String value);
protected:
  bool FActive;
  TZipAnswerAlls FAnswerAll;
  TObject* FBuilder;
  bool FBusy;
  bool FCancel;
	int FErrCode;
	String FEventErr;
  TZipEventsRec FEvents;
  int FFullErrCode;
  bool fIsDestructing;
	String FMessage;
	bool FNotMainTask;
	TProgressDetails* FProgDetails;
	bool FReenter;
	String FTempDir;
	bool FTrace;
	bool FUnattended;
	bool FVerbose;
	virtual bool __fastcall Call_back(ActionCodes ActionCode, int ErrorCode, String Msg,
	                                 unsigned FileSize);
	virtual void __fastcall SetCancel(bool Value);
	TModalResult __fastcall ZipMessageDialog(const String& Title, String& msg,
                                           int Context, TMsgDlgButtons Btns);
  TModalResult __fastcall ZipMessageDlg(const String& Title,const String& msg,
                                           int Context, TMsgDlgButtons Btns);
  void __fastcall ZipMessageDlg(const String& msg, int Context);

public:
  __fastcall TZBCore(TObject* ABuilder);
	__fastcall ~TZBCore(void);
  void __fastcall Attacked(TObject* AnObject);
  void __fastcall Clear(void);
  void __fastcall CheckExec(int retval);
  virtual void __fastcall Done(void);
  __int64 __fastcall GetTotalSizeToProcess(void);
  String __fastcall MakeTempFileName(String Prefix, String Extension);
  void __fastcall Report(ActionCodes ActionCode, int ErrorCode, String Msg, __int64 FileSize);
  virtual void __fastcall Starting(void);
  void __fastcall ShowExceptionError(const EZipBuilder &ZBExcept);
  void __fastcall ShowZipFmtMessage(int Ident, const System::TVarRec* Args, const int Args_Size);
  void __fastcall ShowZipMessage(int Ident, const String UserStr);
 	void __fastcall StartWaitCursor(void);
	void __fastcall StopWaitCursor(void);
	__property TObject* Builder = {read=FBuilder, write=FBuilder};
	__property bool Busy = {read=FBusy, write= FBusy};
	__property bool Cancel = {read=FCancel, write=SetCancel, nodefault};
  __property int ErrCode = {read=FErrCode, write=FErrCode, nodefault};
  __property TZipEventsRec* Events = {read=GetEvents};
  __property int FullErrCode = {read=FFullErrCode};
  __property String Message = {read=GetMessage, write=FMessage};
  __property bool NotMainTask={read=FNotMainTask, write=FNotMainTask};
  __property bool Reentry = {read=FReenter, write= FReenter};
	__property String TempDir = {read=FTempDir, write=SetTempDir};
	__property bool Trace = {read=FTrace, write=FTrace, nodefault};
	__property bool Unattended = {read=FUnattended, write=FUnattended, nodefault};
	__property bool Verbose = {read=FVerbose, write=FVerbose, nodefault};
};
#pragma option -a.

#if __BORLANDC__ < 1344		// BCB4=0x540(1344)
 //-- template instantiations -------------------------------------------------
 template class TZipAnswerAlls;
 template class TZipDiskStatus;
#endif

}	/* namespace Zbcore */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Zbcore;
#endif
#endif

