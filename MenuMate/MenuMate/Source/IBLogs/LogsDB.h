//----------------------------------------------------------------------------
#ifndef LogsDBH
#define LogsDBH
//----------------------------------------------------------------------------
#include "Manager.h"
#include "MM_DBCore.h"
#include "ManagerNet.h"
#include "logsdef.h"

#define Err_Panic       "Panic"
#define Err_Error       "Error"
#define Err_Warning     "Warning"
#define Err_Standard    "Standard"
#define Err_Info        "Info"

class TManagerLogs : public TManager
{
public:
	__fastcall TManagerLogs();
	~TManagerLogs();
	bool OnLineLogging;
   TList *EventList;
   std::auto_ptr<TStringList> ErrorsToLog;
   class TLogRecord
   {
	public:
		TLogRecord(UnicodeString inhost_name, UnicodeString inFunction, UnicodeString inType, UnicodeString inMsg, TDateTime InTimeStamp);

		UnicodeString	host_name;
		UnicodeString	Function;
		UnicodeString	Type;
		UnicodeString	Msg;
		TDateTime	TimeStamp;
		int			Key;
   };
   void Add(UnicodeString Function,UnicodeString Type, UnicodeString Msg,UnicodeString Terminal = "");
   void AddLastError(UnicodeString Type,UnicodeString Func = "NA");
   void ClearAll();
   void ClearAllDB();
   void Process();
   void RecordToDB(TIBSQL *Query, int Key, TLogRecord *CurrentRecord);
   void RecordToStrings(TLogRecord * CurrentRecord,TStrings * List);
	void Purge(int DayCount);
   bool ProcessingEnabled;
   UnicodeString TerminalName;
	void Initialise(UnicodeString inTerminalName);

	static TManagerLogs& Instance() {
		static TManagerLogs singleton;
		return singleton;
	}

private:

	void addExceptionTrace(
			UnicodeString functionCaught,
			AnsiString terminalName );

	AnsiString extractFileNameFromPath( AnsiString path );

	bool HaltProcessSome;
	Database::TDBControl &DBControl;
};

//---------------------------------------------------------------------------
// Call to New failed in table construction.
#define ENum_TableCreateFailed 0001

//---------------------------------------------------------------------------
#define dDL_WriteData_ConnectFailed 			3		//EDL_Fatal
#define dDL_WriteData_SendData			 		4		//EDL_Fatal
#define dDL_WriteData_WriteFailed		 		3		//EPC_Terminate

#define dDL_ReadData_HeaderFailed				1		//EDL_CRC
#define dDL_ReadData_CRCFailed					2		//EDL_CRC
#define dDL_ReadData_RetryCountExpired			1		//EPC_CRCDisconnect
#define dDL_ReadData_TimeOutCountExpired		2		//EPC_TimeOut

#define dDL_ReciveData_TimeOut					5		//EDL_TimeOut
#define dDL_ReciveData_TimeOutCountExpired	6		//EDL_TimeOut

#define dDL_Connect_RetrieveHeaderFailed		7		//EDL_TimeOut
#define dDL_Connect_HeaderFailed				8		//EDL_CRC
#define dDL_Connect_RetrieveData				9		//EDL_TimeOut
#define dDL_Connect_DataCountFailed			10		//EDL_CRC
#define dDL_Connect_RetrieveMaxDataSize		11		//EDL_TimeOut
#define dDL_Connect_RetrieveHighCRC			12		//EDL_TimeOut
#define dDL_Connect_RetrieveLowCRC			13		//EDL_TimeOut
#define dDL_Connect_CRCFailed					14		//EDL_CRC
#define dDL_Connect_WriteFailedOnConnect	15		//EDL_Fatal
#define dDL_Connect_CRCErrorDisconnect		4		//EPC_CRCDisconnect
#define dDL_Connect_TimeOutDisconnect		5		//EPC_TimeOut
#define dDL_Connect_FatalDisconnect			6		//EPC_Terminate


// Throw Error Counters for IR Errors.
#define dPC_TRX_REQ_Err_SLAVE_ACK_NO_DATA_R         2
#define dPC_TRX_REQ_Err_SLAVE_PRN_ERROR_R           3
#define dPC_TRX_REQ_Err_SLAVE_INVALID_DATA_R        4
#define dPC_TRX_REQ_Err_SLAVE_INVALID_COMMAND_R     5
#define dPC_TRX_REQ_Err_SLAVE_UNKNOWN               6
#define dPC_TRX_REQ_Err_SLAVE_BUSY_R					 7

#define dPC_TRX_ACK_Err_SLAVE_NACK_R                8

#define dPC_TRX_ACK_Err_SLAVE_ACK_R                 9
#define dPC_TRX_ACK_Err_SLAVE_ACK_NO_DATA_R         10
#define dPC_TRX_ACK_Err_SLAVE_PRN_ERROR_R           11
#define dPC_TRX_ACK_Err_SLAVE_INVALID_DATA_R        12
#define dPC_TRX_ACK_Err_SLAVE_INVALID_COMMAND_R     13
#define dPC_TRX_ACK_Err_SLAVE_UNKNOWN               14
#define dPC_TRX_ACK_Err_SLAVE_BUSY_R                15

#define dPC_TRX_DATA_Err_SLAVE_NACK_R               16
#define dPC_TRX_DATA_Err_SLAVE_ACK_R                17
#define dPC_TRX_DATA_Err_SLAVE_ACK_NO_DATA_R        18
#define dPC_TRX_DATA_Err_SLAVE_PRN_ERROR_R          19
#define dPC_TRX_DATA_Err_SLAVE_INVALID_DATA_R       20
#define dPC_TRX_DATA_Err_SLAVE_INVALID_COMMAND_R    21
#define dPC_TRX_DATA_Err_SLAVE_UNKNOWN              22
#define dPC_TRX_DATA_Err_SLAVE_BUSY_R               23

#define dPC_TRX_DISC_Err_SLAVE_NACK_R              24
#define dPC_TRX_DISC_Err_SLAVE_ACK_R               25
#define dPC_TRX_DISC_Err_SLAVE_ACK_NO_DATA_R       26
#define dPC_TRX_DISC_Err_SLAVE_PRN_ERROR_R         27
#define dPC_TRX_DISC_Err_SLAVE_INVALID_DATA_R      28
#define dPC_TRX_DISC_Err_SLAVE_INVALID_COMMAND_R   29
#define dPC_TRX_DISC_Err_SLAVE_UNKNOWN             30
#define dPC_TRX_DISC_Err_SLAVE_BUSY_R              31

//-----------------------Table & Seat Handleing Errors-----------------------
#define Error_Intro "An Error has Occured "
#define EMsg_OutOfMemory "Memory Allocation Error. You are mostly Likely Out of Memory."
#define EFix_OutOfMemory "Check Free Swap Disk space and reboot your computer"

// Serial Routine Downtimes.
#define EXECPTIONDOWNTIME 10000
#define ERRORDOWNTIME 10000


// Process connection error messages.
enum EPCType {EPC_TimeOut, EPC_CRCDisconnect, EPC_Terminate,EPC_Destructor};
//enum EPCType {EPC_TimeOut, EPC_CRCDisconnect, EPC_Terminate,EPC_InternalPkt_CRC,EPC_InternalPkt_Overrun,EPC_Destructor};



enum ETCPIPErrorType {  Err_Failed = 1,
                        Ok_TCPIP = 255};


// Datalink Layer error messages.
enum EDLType {EDL_Ok,EDL_TimeOut,EDL_CRC,EDL_Fatal,EDL_Destructor};

// Datalink Layer error messages.
enum ESlaveType {Err_SLAVE_ACK_R, Err_SLAVE_NACK_R, Err_SLAVE_ACK_NO_DATA_R, Err_SLAVE_PRN_ERROR_R,Err_SLAVE_INVALID_DATA_R,
                 Err_SLAVE_INVALID_COMMAND_R,Err_SLAVE_BUSY_R,Err_SLAVE_UNKNOWN,Err_SlaveDestructor, Err_OUT_OF_PHASE};
//	EDL_CRC checks RetryCount and either tries again or thorws a EPC_CRCDisconnect.
// EDL_TimeOut throws a EPC_Timeout.
// EDL_Fatal set Terminate to True for this connection, and throws an EPC_Terminate.

//------------------------------------------------------------------------------
class EProcessConnectionError
{
public:
	EPCType Type;
   int ErrNum;
   __fastcall EProcessConnectionError(EPCType inType, int inErrNum);
   __fastcall ~EProcessConnectionError();

};

// base class for all Data link errors.
class EDataLinkError
{
public:
	EDLType Type;
   int ErrNum;
   __fastcall EDataLinkError(EDLType inType, int inErrNum);
   __fastcall ~EDataLinkError();
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//
// base class for all Slave Command errors.
class ESlaveMessageError
{
public:
	ESlaveType Type;
   int ErrNum;
   __fastcall ESlaveMessageError(ESlaveType inType, int inErrNum);
   __fastcall ~ESlaveMessageError();
};


class ETableCreateError
{
public:
   	__fastcall ETableCreateError(UnicodeString inMsg,UnicodeString inFix, int inErrNum)
      {
         Msg = inMsg;
         Fix = inFix;
      	ErrNum = inErrNum;
      }
     UnicodeString Msg;
     UnicodeString Fix;
     int ErrNum;
};

//---------------------------------------------------------------------------

// Application Layer Repsonse messages.

// Error's caused by main thread processing incomming data/ screen refreash etc....
class EPrimaryProcessingError
{
public:
	EProcessingErrorType Type;
   unsigned char Command;
   __fastcall EPrimaryProcessingError(EProcessingErrorType inType,unsigned char inCommand);
   __fastcall ~EPrimaryProcessingError();
};

class EHelixError
{
public:
   UnicodeString ErrMsg;
   __fastcall EHelixError(UnicodeString inErrMsg);
   __fastcall ~EHelixError();
};
//----------------------------------------------------------------------------
#endif
