#ifndef ZipBaseH
#define ZipBaseH
#include "EZipBuilder.h"
#include "ZipProg.h"


#if __BORLANDC__ > 1328		// BCB3=0x0530(1328)  BCB3 does not have __int64 property
#define BINT __int64
#else
#define BINT long
#endif

#ifdef ALLOW_2G
#define TProgressSize __int64
#else
#define TProgressSize int          // - will allow uncompressed size to almost 2G
//  TProgressSize  unsinged;   // - will allow uncompressed size to almost 4G
#endif


namespace Zipbase
{
//-- type declarations -------------------------------------------------------

#pragma option -b-
enum ActionCodes {zacTick, zacItem, zacProgress,zacEndOfBatch, zacMessage, zacCount,
                  zacSize, zacNewName, zacPassword, zacCRCError, zacOverwrite, zacSkipped,
                  zacComment, zacStream, zacData, zacXItem, zacXProgress, zacExtName, zacNone};

enum ProgressTypes { zprFile, zprArchive, zprCopyTemp, zprSFX, zprHeader, zprFinish,
                     zprCompressed, zprCentral, zprChecking, zprLoading, zprJoining,
                     zprSplitting   };
enum TZipShowProgress {zspNone, zspExtra, zspFull};
enum TZipAllwaysItems {zaaYesOvrwrt, zyaDummy};
#pragma option -b.    

typedef Set<TZipAllwaysItems, zaaYesOvrwrt, zyaDummy>  TZipAnswerAlls;

typedef void __fastcall (__closure *TCheckTerminateEvent)(TObject* Sender, bool &abort);
typedef void __fastcall (__closure *TTickEvent)(TObject* Sender);
typedef void __fastcall (__closure *TTotalProgressEvent)(TObject* Sender, TProgressSize TotalSize, int PerCent);
typedef void __fastcall (__closure *TProgressDetailsEvent)(TObject* Sender, TProgressDetails* Details);
typedef void __fastcall (__closure *TItemProgressEvent)(TObject* Sender, AnsiString Item,
                                                        unsigned TotalSize, int PerCent);
typedef void __fastcall (__closure *TProgressEvent)(TObject* Sender, ProgressType ProgrType,
	                                                  AnsiString Filename,
                                                    TProgressSize FileSize);
typedef void __fastcall (__closure *TMsgEvent)(TObject* Sender, int ErrCode, AnsiString Message);
typedef void __fastcall (__closure *TZipDialogEvent)(TObject* Sender, const AnsiString Title,
                                                     AnsiString& msg, int& Context, TMsgDlgButtons Btns);

                                                    
class __declspec(delphiclass) TZipBase : public TObject
{
private:
	int FCurWaitCount;
  TCursor FSaveCursor;  
     // events
  TMsgEvent FOnMessage;
  TProgressEvent FOnProgress;
  TCheckTerminateEvent FOnCheckTerminate;
  TTickEvent FOnTick;
  TTotalProgressEvent FOnTotalProgress;
  TItemProgressEvent FOnItemProgress;
  TProgressDetailsEvent FOnProgressDetails;
  TZipDialogEvent FOnZipDialog;

  AnsiString __fastcall GetMessage(void);
protected:
  TObject* FBuilder;
	bool FActive;
  bool FBusy;
  bool FReenter;
	AnsiString FMessage;
	bool FCancel;
	int FErrCode;
	int FFullErrCode;
	bool FUnattended;
	bool FVerbose;
	bool FTrace;
	TObject* FProgDetails;
	AnsiString FEventErr;
	bool fIsDestructing;
	AnsiString FDLLDirectory;
	AnsiString FTempDir;
	bool FNotMainTask;
	TZipAnswerAlls FAnswerAll;
	virtual void __fastcall SetCancel(bool Value);
	void __fastcall SetDLLDirectory(AnsiString Value);
	TModalResult __fastcall ZipMessageDialog(const AnsiString Title, AnsiString& msg,
                                           int Context, TMsgDlgButtons Btns);
  TModalResult __fastcall ZipMessageDlg(const AnsiString Title, AnsiString msg,
                                           int Context, TMsgDlgButtons Btns);
  void __fastcall ZipMessageDlg(const AnsiString msg, int Context);

	virtual bool __fastcall Call_back(ActionCodes ActionCode, int ErrorCode, AnsiString Msg,
	                                 unsigned FileSize);
public:
  virtual void __fastcall Starting(void);
  virtual void __fastcall Done(void);
  virtual void __fastcall Clear(void);
	void __fastcall ShowZipMessage(int Ident, const AnsiString UserStr="");
	void __fastcall ShowExceptionError(const EZipBuilder &ZBExcept);
	void __fastcall StartWaitCursor(void);
	void __fastcall StopWaitCursor(void);
	void __fastcall Report(ActionCodes ActionCode, int ErrorCode, AnsiString Msg, __int64 FileSize);
  void __fastcall Report(ActionCodes ActionCode, int ErrorCode,
                         AnsiString Msg, __int64 FileSize, unsigned written);
	AnsiString __fastcall MakeTempFileName(AnsiString Prefix = "", AnsiString Extension = "");
  void __fastcall Attacked(TObject* AnObject);
  __int64 __fastcall GetTotalSizeToProcess(void);
	__fastcall TZipBase(void);
	__fastcall virtual ~TZipBase(void);
	__property TObject* Builder = {read=FBuilder, write=FBuilder};
	__property bool Cancel = {read=FCancel, write=SetCancel, nodefault};
  __property bool Busy = {read=FBusy, write= FBusy};
  __property bool Reentry = {read=FReenter, write= FReenter};
	__property int ErrCode = {read=FErrCode, write=FErrCode, nodefault};
	__property AnsiString Message = {read=GetMessage, write=FMessage};
	__property bool Verbose = {read=FVerbose, write=FVerbose, nodefault};
	__property bool Trace = {read=FTrace, write=FTrace, nodefault};
	__property bool Unattended = {read=FUnattended, write=FUnattended, nodefault};
	__property AnsiString DLLDirectory = {read=FDLLDirectory, write=SetDLLDirectory};
	__property AnsiString TempDir = {read=FTempDir, write=FTempDir};
  __property bool NotMainTask={read=FNotMainTask, write=FNotMainTask};
  __property TMsgEvent OnMessage={read=FOnMessage,write=FOnMessage};

  __property TProgressEvent OnProgress={read=FOnProgress,write=FOnProgress};
  __property TTotalProgressEvent OnTotalProgress={read=FOnTotalProgress,write=FOnTotalProgress};
  __property TItemProgressEvent OnItemProgress={read=FOnItemProgress,write=FOnItemProgress};
  __property TProgressDetailsEvent OnProgressDetails={read=FOnProgressDetails,write=FOnProgressDetails};
  __property TTickEvent OnTick={read=FOnTick,write=FOnTick};
  __property TCheckTerminateEvent OnCheckTerminate={read=FOnCheckTerminate,write=FOnCheckTerminate};
  __property TZipDialogEvent OnZipDialog={read=FOnZipDialog, write=FOnZipDialog};
};



}	/* namespace Zipbase */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Zipbase;
#endif
#endif

