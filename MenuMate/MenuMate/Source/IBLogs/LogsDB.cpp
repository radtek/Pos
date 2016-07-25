// ---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MMLogging.h"
#include "enum.h"
#include "FileCtrl.hpp"
#include "MMRegistry.h"
#include "ManagerVariable.h"
#include "except.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif

// ---------------------------------------------------------------------
const int MaxFileLogRecords = 1000;
const int EventsToLog = 50; // Log this many each loop

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
__fastcall TManagerLogs::TManagerLogs()
: DBControl(TDeviceRealTerminal::Instance().DBControl) ,ErrorsToLog (new TStringList())
{
   Enabled = false;
   EventList = new TList;
   HaltProcessSome = false;
   ProcessingEnabled = true;
   OnLineLogging = false;
   ErrorsToLog->Add(EXCEPTIONLOG);
   ErrorsToLog->Add(ERRORLOG);
   ErrorsToLog->Add(VERSIONLOG);
   ErrorsToLog->Add(UPDATELOG);
#ifdef _DEBUG
   OnLineLogging = true;
   ErrorsToLog->Add(DEBUGLOG);
   ErrorsToLog->Add(PHOENIXINTERFACELOG);
   ErrorsToLog->Add(PALMBTLAYERLOG);
   ErrorsToLog->Add(PALMPACKETLAYERLOG);
   ErrorsToLog->Add(PALMDECODEPACKETLOG);
   ErrorsToLog->Add(SMARTCARDLOG);
   ErrorsToLog->Add(XMLINTERFACELOG);
   ErrorsToLog->Add(DLLSTOCKLOG);
   ErrorsToLog->Add(EFTPOSLOG);
   ErrorsToLog->Add(DLLNZALOG);
   ErrorsToLog->Add(REGISTRATIONLOG);
   ErrorsToLog->Add(EBETINTERFACELOG);
   ErrorsToLog->Add(WEBMATELOG);
	ErrorsToLog->Add(INTAMATEINTERFACELOG);
#endif

}

// ---------------------------------------------------------------------
TManagerLogs::~TManagerLogs()
{
   try
   {
	  ClearAll();
	  Enabled = false;

	  delete EventList;
	  EventList = NULL;
   }
   catch(Exception & E)
   {
	  Enabled = false;
   }
}

// ---------------------------------------------------------------------
void TManagerLogs::ClearAll()
{
   __int64 startTick = ::GetTickCount();
   while (EventList->Count > 0)
   {
	  Process();
	  // Try logging the remains for 10 seconds then give up.
	  if (::GetTickCount() > startTick + 10000)
	  {
		 break;
	  }
   }
   ErrorsToLog->Clear();
}

// ---------------------------------------------------------------------
void TManagerLogs::Initialise(UnicodeString inTerminalName)
{
   TerminalName = inTerminalName;
   Enabled = true;
}

// ---------------------------------------------------------------------
void TManagerLogs::Process()
{
   if (!HaltProcessSome && ProcessingEnabled && Enabled && EventList->Count > 0)
   {
	  HaltProcessSome = true;
	  try
	  {
		 try
		 {
			if (DBControl.Connected())
			{
			   Database::TDBTransaction DBTransaction(DBControl);
			   DBTransaction.StartTransaction();

			   TIBSQL *logsQuery = DBTransaction.Query(DBTransaction.AddQuery());
			   logsQuery->SQL->Text =
				   "INSERT INTO LOGS (" "LOGS_KEY," "TERMINAL_NAME," "ERROR_TYPE," "ERROR_MSG," "TIME_STAMP," "FUNCTION_NAME)"
				   "VALUES (" ":LogsKey," ":Host," ":Type," ":Message," ":LogTime," ":Function);";

			   for (int i = 0; i < EventsToLog && i < EventList->Count; ++i)
			   {
				  int Key = DBTransaction.GetGeneratorVal("GEN_LOGSKEY");

				  TLogRecord *CurrentRecord = (TLogRecord*)EventList->Items[i];
				  RecordToDB(logsQuery, Key, CurrentRecord);
			   }
			   for (int i = 0; i < EventsToLog && EventList->Count != 0; ++i)
			   {
				  TLogRecord *CurrentRecord = (TLogRecord*)EventList->Items[0];

				  try
				  {
					 delete CurrentRecord;
				  }
				  __finally
				  {
					 EventList->Delete(0);
				  }
			   }
			   DBTransaction.Commit();
			}
		 }
		 catch(Exception & E)
		 {
			try
			{
			   std::auto_ptr <TStringList> List(new TStringList);
			   if (FileExists(ExtractFilePath(Application->ExeName) + "logdump.log"))
			   {
				  List->LoadFromFile(ExtractFilePath(Application->ExeName) + "logdump.log");

				  while (List->Count > MaxFileLogRecords)
				  {
					 List->Delete(0);
				  }
			   }
			   while (EventList->Count != 0)
			   {
				  TLogRecord *CurrentRecord = (TLogRecord*)EventList->Items[0];
				  try
				  {
					 List->Add(E.Message);
					 RecordToStrings(CurrentRecord, List.get());
				  }
				  __finally
				  {
					 delete CurrentRecord;
					 EventList->Delete(0);
				  }
			   }
			   List->SaveToFile(ExtractFilePath(Application->ExeName) + "logdump.log");
			}
			catch(Exception & E)
			{
			   // Cant do much now!
			   // Application->ShowException(&E);

			   // Logs lost. Never mind.
			}
		 }
	  }
	  __finally
	  {
		 HaltProcessSome = false;
	  }
   }
}

// ---------------------------------------------------------------------
void TManagerLogs::Purge(int DayCount)
{
   if (!Enabled)
	  return;
   try
   {
	  static __int64 LastRun = 0;
	  if (::GetTickCount() > LastRun + 3600000) // Run Once an Hour Max.
	  {
		 LastRun = ::GetTickCount();

		 Database::TDBTransaction DBTransaction(DBControl);
		 DBTransaction.StartTransaction();
		 TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		 if (DayCount != 0)
		 {
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "DELETE FROM LOGS WHERE TIME_STAMP < :OldestRecord";
			IBInternalQuery->ParamByName("OldestRecord")->AsDateTime = Now() - DayCount;
			IBInternalQuery->ExecQuery();
		 }

		 int MaxValue = TManagerVariable::Instance().GetInt(DBTransaction, vmMaxLogsRecords);
		 if (MaxValue != 0)
		 {
			// Ensure Logs stay below the Max number of logs records.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text = "Select MAX(LOGS_KEY) MAX_KEY from LOGS";
			IBInternalQuery->ExecQuery();

			MaxValue = IBInternalQuery->FieldByName("MAX_KEY")->AsInteger - MaxValue;
			if (MaxValue > 0)
			{
			   IBInternalQuery->Close();
			   IBInternalQuery->SQL->Clear();
			   IBInternalQuery->SQL->Text = "DELETE FROM LOGS WHERE LOGS_KEY < :MAX_KEY";
			   IBInternalQuery->ParamByName("MAX_KEY")->AsInteger = MaxValue;
			   IBInternalQuery->ExecQuery();
			}
		 }
		 DBTransaction.Commit();
	  }
   }
   catch(Exception & E)
   {
	  ShowMessage("Logs Error " + E.Message);
   }
}

// ---------------------------------------------------------------------
void TManagerLogs::RecordToDB(TIBSQL *Query, int Key, TLogRecord *CurrentRecord)
{
   Query->Close();
   Query->ParamByName("LogsKey")->AsInteger = Key;
   Query->ParamByName("Host")->AsString = CurrentRecord->host_name.SubString(1, 22);
   Query->ParamByName("Type")->AsString = CurrentRecord->Type.SubString(1, 20);
   Query->ParamByName("Message")->AsString = CurrentRecord->Msg.SubString(1, 180);
   Query->ParamByName("LogTime")->AsDateTime = CurrentRecord->TimeStamp;
   Query->ParamByName("Function")->AsString = CurrentRecord->Function.SubString(1, 60);
   Query->ExecQuery();
   /* try
   {
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   int Key = DBTransaction.GetGeneratorVal("GEN_LOGSKEY");

   int Key;
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_LOGSKEY, 1) FROM RDB$DATABASE";
   IBInternalQuery->ExecQuery();
   Key = IBInternalQuery->Fields[0]->AsInteger;

   if(Key != 0) // This can happen on shut down....
   {
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Clear();
   IBInternalQuery->SQL->Add("INSERT INTO LOGS (LOGS_KEY,TERMINAL_NAME,ERROR_TYPE,ERROR_MSG,TIME_STAMP,FUNCTION_NAME)");
   IBInternalQuery->SQL->Add("VALUES (:LogsKey, :Host , :Type, :Message, :LogTime, :Function);");

   IBInternalQuery->ParamByName("LogsKey")->AsInteger = Key;
   IBInternalQuery->ParamByName("Host")->AsString = CurrentRecord->host_name.SubString(1,22);
   IBInternalQuery->ParamByName("Type")->AsString = CurrentRecord->Type.SubString(1,20);
   IBInternalQuery->ParamByName("Message")->AsString = CurrentRecord->Msg.SubString(1,180);
   IBInternalQuery->ParamByName("LogTime")->AsDateTime = CurrentRecord->TimeStamp;
   IBInternalQuery->ParamByName("Function")->AsString = CurrentRecord->Function.SubString(1,60);
   IBInternalQuery->ExecQuery();
   }
   }
   catch(Exception &E)
   {
   TStringList *List = new TStringList;
   if(FileExists(ExtractFilePath(Application->ExeName)+ "logdump.log"))
   {
   List->LoadFromFile(ExtractFilePath(Application->ExeName)+ "logdump.log");
   }
   List->Add(E.Message);
   List->SaveToFile(ExtractFilePath(Application->ExeName)+ "logdump.log");
   delete List;
   } */
}

// ---------------------------------------------------------------------
void TManagerLogs::RecordToStrings(TLogRecord * CurrentRecord, TStrings * List)
{
   try
   {
	  List->Add(CurrentRecord->TimeStamp.DateTimeString() + "\t\t\t" + CurrentRecord->Type.SubString(1,
			20) + "\t\t\t" + CurrentRecord->Msg.SubString(1, 180) + "\t\t\t" + CurrentRecord->Function.SubString(1, 60));
   }
   catch(Exception & E)
   {
	  TStringList *List = new TStringList;
	  if (FileExists(ExtractFilePath(Application->ExeName) + "logdump.log"))
	  {
		 List->LoadFromFile(ExtractFilePath(Application->ExeName) + "logdump.log");
	  }
	  List->Add(E.Message);
	  List->SaveToFile(ExtractFilePath(Application->ExeName) + "logdump.log");
	  delete List;
   }
}
// ---------------------------------------------------------------------

void TManagerLogs::Add(UnicodeString Function, UnicodeString Type, UnicodeString Msg, UnicodeString Terminal)
{
   try
   {
	  if (ErrorsToLog->IndexOf(Type) != -1 && Enabled)
	  {
		 if (Terminal == "")
		 {
			Terminal = TerminalName;
		 }
		 if (EventList->Count < 4999)
		 {
            if( Type == EXCEPTIONLOG )
            {
                // add the exception trace only on an exception
			    addExceptionTrace( Function, Terminal );
            }

			EventList->Add(new TLogRecord(Terminal, Function, Type, Msg, Now()));
		 }
	  }
   }
   catch(...)
   {
   }
}
//------------------------------------------------------------------------------

void TManagerLogs::ClearAllDB()
{
   HaltProcessSome = true;
   try
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->SQL->Clear();
	  IBInternalQuery->SQL->Text = "DELETE FROM LOGS";
	  IBInternalQuery->ExecQuery();
	  DBTransaction.Commit();
   }
   __finally
   {
	  HaltProcessSome = false;
   }
}

TManagerLogs::TLogRecord::TLogRecord(UnicodeString inhost_name, UnicodeString inFunction, UnicodeString inType, UnicodeString inMsg,
   TDateTime InTimeStamp) : Key(0)
{
   host_name = inhost_name;
   Function = inFunction;
   Type = inType;
   Msg = inMsg;
   TimeStamp = InTimeStamp;
}

// ---------------------------------------------------------------------------
void TManagerLogs::AddLastError(UnicodeString Type, UnicodeString Func)
{
   UnicodeString WinErrorMsg = SysErrorMessage(GetLastError());
   Add(Func, Type, WinErrorMsg);
}
// ---------------------------------------------------------------------------

void TManagerLogs::addExceptionTrace(
					UnicodeString functionCaught,
					AnsiString terminalName)
{
	try
	{
		AnsiString lineNumber 	= AnsiString( __ThrowLineNumber() );
		AnsiString fileName		= __ThrowFileName();
		AnsiString locationDetails = "@" + lineNumber + " : " + extractFileNameFromPath( fileName );

		EventList->Add(
			new TLogRecord(
					terminalName,
					functionCaught,
					EXCEPTIONLOG,
					locationDetails,
					Now())
					);
	}
	catch(...)
	{
	}
}
//------------------------------------------------------------------------------

AnsiString TManagerLogs::extractFileNameFromPath( AnsiString path )
{
	AnsiString fileName = ExtractFileName( path );
	return fileName;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
__fastcall EProcessConnectionError::EProcessConnectionError(EPCType inType, int inErrNum)
{
   Type = inType;
   ErrNum = inErrNum;
   UnicodeString Err = "EProcessConnectionError : ";
   UnicodeString Err1 = "";
   switch(inErrNum)
   {
   case dDL_ReadData_RetryCountExpired:
	  Err1 = " dDL_ReadData_RetryCountExpired";
	  break;
   case dDL_ReadData_TimeOutCountExpired:
	  Err1 = " dDL_ReadData_TimeOutCountExpired";
	  break;
   case dDL_WriteData_WriteFailed:
	  Err1 = " dDL_WriteData_WriteFailed";
	  break;
   case dDL_Connect_CRCErrorDisconnect:
	  Err1 = " dDL_Connect_CRCErrorDisconnect";
	  break;
   case dDL_Connect_TimeOutDisconnect:
	  Err1 = " dDL_Connect_TimeOutDisconnect";
	  break;
   case dDL_Connect_FatalDisconnect:
	  Err1 = " dDL_Connect_FatalDisconnect";
	  break;
   }
   TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Err);
   TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Err1);
}

// ---------------------------------------------------------------------------
__fastcall EProcessConnectionError::~EProcessConnectionError()
{
   Type = EPC_Destructor;
   ErrNum = EPC_Destructor;
}

// ---------------------------------------------------------------------------
__fastcall EDataLinkError::EDataLinkError(EDLType inType, int inErrNum)
{
   Type = inType;
   ErrNum = inErrNum;

   UnicodeString Err = "EDataLinkError : ";
   UnicodeString Err1;
   switch(inErrNum)
   {
   case dDL_ReadData_HeaderFailed:
	  Err1 = "dDL_ReadData_HeaderFailed : ";
	  break;
   case dDL_ReadData_CRCFailed:
	  Err1 = "dDL_ReadData_CRCFailed : ";
	  break;
   case dDL_WriteData_ConnectFailed:
	  Err1 = "dDL_WriteData_ConnectFailed : ";
	  break;
   case dDL_WriteData_SendData:
	  Err1 = "dDL_WriteData_SendData : ";
	  break;
   case dDL_ReciveData_TimeOut:
	  Err1 = "dDL_ReciveData_TimeOut : ";
	  break;
   case dDL_ReciveData_TimeOutCountExpired:
	  Err1 = "dDL_ReciveData_TimeOutCountExpired : ";
	  break;
   case dDL_Connect_RetrieveHeaderFailed:
	  Err1 = "dDL_Connect_RetrieveHeaderFailed : ";
	  break;
   case dDL_Connect_HeaderFailed:
	  Err1 = "dDL_Connect_HeaderFailed : ";
	  break;
   case dDL_Connect_RetrieveData:
	  Err1 = "dDL_Connect_RetrieveData : ";
	  break;
   case dDL_Connect_DataCountFailed:
	  Err1 = "EProcessConnectionError : ";
	  break;
   case dDL_Connect_RetrieveMaxDataSize:
	  Err1 = "dDL_Connect_RetrieveMaxDataSize : ";
	  break;
   case dDL_Connect_RetrieveHighCRC:
	  Err1 = "dDL_Connect_RetrieveHighCRC : ";
	  break;
   case dDL_Connect_RetrieveLowCRC:
	  Err1 = "dDL_Connect_RetrieveLowCRC : ";
	  break;
   case dDL_Connect_CRCFailed:
	  Err1 = "dDL_Connect_CRCFailed : ";
	  break;
   case dDL_Connect_WriteFailedOnConnect:
	  Err1 = "dDL_Connect_WriteFailedOnConnect : ";
	  break;
   }
   TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Err);
   TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Err1);
}

// ---------------------------------------------------------------------------
__fastcall EDataLinkError::~EDataLinkError()
{
   Type = EDL_Destructor;
   ErrNum = EDL_Destructor;
}

// ---------------------------------------------------------------------------
__fastcall ESlaveMessageError::ESlaveMessageError(ESlaveType inType, int inErrNum)
{
  UnicodeString Err = "ESlaveMessageError : ";
  UnicodeString Err1;
  switch(inErrNum)
  {
  case dPC_TRX_REQ_Err_SLAVE_ACK_NO_DATA_R:
    Err1 = "pc_trx_req_err_slave_ack_no_data_r \n";
    break; // 2
  case dPC_TRX_REQ_Err_SLAVE_PRN_ERROR_R:
    Err1 = "pc_trx_req_err_slave_prn_error_r \n";
    break; // 3
  case dPC_TRX_REQ_Err_SLAVE_INVALID_DATA_R:
    Err1 = "pc_trx_req_err_slave_invalid_data_r \n";
    break; // 4
  case dPC_TRX_REQ_Err_SLAVE_INVALID_COMMAND_R:
    Err1 = "pc_trx_req_err_slave_invalid_command_r \n";
    break; // 5
  case dPC_TRX_REQ_Err_SLAVE_UNKNOWN:
    Err1 = "pc_trx_req_err_slave_unknown \n";
    break; // 6
  case dPC_TRX_REQ_Err_SLAVE_BUSY_R:
    Err1 = "pc_trx_req_err_slave_busy_r \n";
    break; // 7

  case dPC_TRX_ACK_Err_SLAVE_NACK_R:
    Err1 = "pc_trx_ack_err_slave_nack_r \n";
    break; // 8
  case dPC_TRX_ACK_Err_SLAVE_ACK_R:
    Err1 = "pc_trx_ack_err_slave_ack_r \n";
    break; // 9
  case dPC_TRX_ACK_Err_SLAVE_ACK_NO_DATA_R:
    Err1 = "pc_trx_ack_err_slave_ack_no_data_r \n";
    break; // 10
  case dPC_TRX_ACK_Err_SLAVE_PRN_ERROR_R:
    Err1 = "pc_trx_ack_err_slave_prn_error_r \n";
    break; // 11
  case dPC_TRX_ACK_Err_SLAVE_INVALID_DATA_R:
    Err1 = "pc_trx_ack_err_slave_invalid_data_r \n";
    break; // 12
  case dPC_TRX_ACK_Err_SLAVE_INVALID_COMMAND_R:
    Err1 = "pc_trx_ack_err_slave_invalid_command_r \n";
    break; // 13
  case dPC_TRX_ACK_Err_SLAVE_UNKNOWN:
    Err1 = "pc_trx_ack_err_slave_unknown \n";
    break; // 14
  case dPC_TRX_ACK_Err_SLAVE_BUSY_R:
    Err1 = "pc_trx_ack_err_slave_busy_r \n";
    break; // 15

  case dPC_TRX_DATA_Err_SLAVE_NACK_R:
    Err1 = "pc_trx_data_err_slave_nack_r \n";
    break; // 16
  case dPC_TRX_DATA_Err_SLAVE_ACK_R:
    Err1 = "pc_trx_data_err_slave_ack_r \n";
    break; // 17
  case dPC_TRX_DATA_Err_SLAVE_ACK_NO_DATA_R:
    Err1 = "pc_trx_data_err_slave_ack_no_data_r \n";
    break; // 18
  case dPC_TRX_DATA_Err_SLAVE_PRN_ERROR_R:
    Err1 = "pc_trx_data_err_slave_prn_error_r \n";
    break; // 19
  case dPC_TRX_DATA_Err_SLAVE_INVALID_DATA_R:
    Err1 = "pc_trx_data_err_slave_invalid_data_r \n";
    break; // 20
  case dPC_TRX_DATA_Err_SLAVE_INVALID_COMMAND_R:
    Err1 = "pc_trx_data_err_slave_invalid_command_r \n";
    break; // 21
  case dPC_TRX_DATA_Err_SLAVE_UNKNOWN:
    Err1 = "pc_trx_data_err_slave_unknown \n";
    break; // 22
  case dPC_TRX_DATA_Err_SLAVE_BUSY_R:
    Err1 = "pc_trx_data_err_slave_busy_r \n";
    break; // 23

  case dPC_TRX_DISC_Err_SLAVE_NACK_R:
    Err1 = "pc_trx_disc_err_slave_nack_r \n";
    break; // 24
  case dPC_TRX_DISC_Err_SLAVE_ACK_R:
    Err1 = "pc_trx_disc_err_slave_ack_r \n";
    break; // 25
  case dPC_TRX_DISC_Err_SLAVE_ACK_NO_DATA_R:
    Err1 = "pc_trx_disc_err_slave_ack_no_data_r \n";
    break; // 26
  case dPC_TRX_DISC_Err_SLAVE_PRN_ERROR_R:
    Err1 = "pc_trx_disc_err_slave_prn_error_r \n";
    break; // 27
  case dPC_TRX_DISC_Err_SLAVE_INVALID_DATA_R:
    Err1 = "pc_trx_disc_err_slave_invalid_data_r \n";
    break; // 28
  case dPC_TRX_DISC_Err_SLAVE_INVALID_COMMAND_R:
    Err1 = "pc_trx_disc_err_slave_invalid_command_r \n";
    break; // 29
  case dPC_TRX_DISC_Err_SLAVE_UNKNOWN:
    Err1 = "pc_trx_disc_err_slave_unknown \n";
    break; // 30
  case dPC_TRX_DISC_Err_SLAVE_BUSY_R:
    Err1 = "pc_trx_disc_err_slave_busy_r \n";
    break; // 31
  }
  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Err1 + "# " + IntToStr(inErrNum));
   Type = inType;
   ErrNum = inErrNum;
}

// ---------------------------------------------------------------------------
__fastcall ESlaveMessageError::~ESlaveMessageError()
{
   Type = Err_SlaveDestructor;
   ErrNum = Err_SlaveDestructor;
}

// ---------------------------------------------------------------------------
__fastcall EPrimaryProcessingError::EPrimaryProcessingError(EProcessingErrorType inType, unsigned char inCommand)
{
   Type = inType;
   Command = Command;
   UnicodeString Err = "EPrimaryProcessingError : ";
   UnicodeString Err1;

   if (inType == Err_Msg_CRC)
   {
	  Err1 = "Msg_Currupt_Message Err_Msg_CRC";
   }
   else
   {
	  Err1 = "Msg_Currupt_Message";
   }
   TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Err);
   TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Err1);
}

// ---------------------------------------------------------------------------
__fastcall EPrimaryProcessingError::~EPrimaryProcessingError()
{
   Type = Err_Destructor;
}
// ---------------------------------------------------------------------------

__fastcall EHelixError::EHelixError(UnicodeString inErrMsg)
{
   ErrMsg = inErrMsg;
   TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, ErrMsg);
}

// ---------------------------------------------------------------------------
__fastcall EHelixError::~EHelixError()
{
}
// ---------------------------------------------------------------------------
