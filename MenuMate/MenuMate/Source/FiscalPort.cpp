//---------------------------------------------------------------------------
#pragma hdrstop

#include "FiscalPort.h"
#include "ManagerVariable.h"
#include "MMLogging.h"
#include "MMMessageBox.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
TFiscalPort::TFiscalPort()
{
   Port = new TComPort(NULL);
   Port->BaudRate = br57600;
   Port->DataBits = dbEight;
   Port->Parity->Bits = prNone;
   Port->StopBits = sbOneStopBit;
   Port->Parity->Check = false;
   Port->Parity->Replace = false;
   Port->Buffer->InputSize = 16384;
   Port->Buffer->OutputSize = 16384;
   Port->DiscardNull = false;
   Port->OnRxChar = NULL;//GetData;
   Port->Timeouts->ReadInterval = 30;
   Port->Timeouts->ReadTotalMultiplier = 30;
   Port->Timeouts->WriteTotalMultiplier = 30;
   Port->Timeouts->ReadTotalConstant = 30;//30;
   Port->Timeouts->WriteTotalConstant = 30;
   Port->FlowControl->ControlDTR = dtrEnable;
   Port->FlowControl->ControlRTS = rtsHandshake;
   Port->FlowControl->OutCTSFlow = false;
   Port->FlowControl->XonXoffOut = false;
   Port->FlowControl->XonXoffIn = false;
   ClosingPort = false;
   OpeningPort = false;
   Busy = false;
   PortNumber = -1;
   AsyncMode = false;
   Port->OnTxEmpty = PortTxEmpty;
   CurrentRequestType = eFiscalNone;
}
//-----------------------------------------------------------------------------
TFiscalPort::~TFiscalPort()
{
   Close();
   delete Port;
}
//-----------------------------------------------------------------------------
void TFiscalPort::SaveSettings(Database::TDBTransaction &DBTransaction)
{
  TManagerVariable::Instance().SetDeviceInt(DBTransaction,  vmFiscalServerPortNumber, PortNumber);
  TManagerVariable::Instance().SetDeviceInt(DBTransaction,  vmFiscalBaudRate, static_cast <int> (Port->BaudRate));
  TManagerVariable::Instance().SetDeviceInt(DBTransaction,  vmFiscalDataBits, static_cast <int> (Port->DataBits));
  TManagerVariable::Instance().SetDeviceInt(DBTransaction,  vmFiscalStopBits, static_cast <int> (Port->StopBits));
  TManagerVariable::Instance().SetDeviceInt(DBTransaction,  vmFiscalParityBits, static_cast <int> (Port->Parity->Bits));
  TManagerVariable::Instance().SetDeviceInt(DBTransaction,  vmFiscalRTSFlowControl,static_cast <int> (Port->FlowControl->ControlRTS));
  TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmFiscalOutCTSFlow, Port->FlowControl->OutCTSFlow);
  TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmFiscalXonXoffOut, Port->FlowControl->XonXoffOut);
  TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmFiscalXonXoffIn, Port->FlowControl->XonXoffIn);
  TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmFiscalAsync, AsyncMode);
}
//-----------------------------------------------------------------------------
void TFiscalPort::Initialise(Database::TDBTransaction &DBTransaction)
{
     PortNumber = TManagerVariable::Instance().GetInt(DBTransaction, vmFiscalServerPortNumber, 0);

     int BaudRate = TManagerVariable::Instance().GetInt(DBTransaction, vmFiscalBaudRate, 0);
     Port->BaudRate = TBaudRate(BaudRate);

     int DataBits = TManagerVariable::Instance().GetInt(DBTransaction, vmFiscalDataBits, 0);
     Port->DataBits = TDataBits(DataBits);

     int StopBits = TManagerVariable::Instance().GetInt(DBTransaction, vmFiscalStopBits, 0);
     Port->StopBits = TStopBits(StopBits);

     int ParityBits = TManagerVariable::Instance().GetInt(DBTransaction, vmFiscalParityBits, 0);
     Port->Parity->Bits = TParityBits(ParityBits);

     int ControlRTS = TManagerVariable::Instance().GetInt(DBTransaction, vmFiscalRTSFlowControl, 0);
     Port->FlowControl->ControlRTS = TRTSFlowControl(ControlRTS);

     bool OutCTSFlow = TManagerVariable::Instance().GetBool(DBTransaction, vmFiscalOutCTSFlow, false);
     Port->FlowControl->OutCTSFlow = OutCTSFlow;

     bool XonXoffOut = TManagerVariable::Instance().GetBool(DBTransaction, vmFiscalXonXoffOut, false);
     Port->FlowControl->XonXoffOut = XonXoffOut;

     bool XonXoffIn = TManagerVariable::Instance().GetBool(DBTransaction, vmFiscalXonXoffIn, false);
     Port->FlowControl->XonXoffIn = XonXoffIn;

     AsyncMode = TManagerVariable::Instance().GetBool(DBTransaction, vmFiscalAsync, false);

//     SaveSettings(DBTransaction);
}
//-----------------------------------------------------------------------------
TComPort *TFiscalPort::GetTComPort()
{
   return Port;
}
//-----------------------------------------------------------------------------
bool TFiscalPort::Open(int PortNum)
{
   bool Success = false;
   try
   {
	  if (!OpeningPort)
	  {
		 OpeningPort = true;
		 try
		 {
			if (PortNum > 0)
			{
			   if (Port->Connected && Port->Port != ("COM" + IntToStr(PortNum)))
			   { // Comports Changed.
				  Port->Close();
				  Port->Port = "COM" + IntToStr(PortNum);
				  Port->Open();
				  Success = true;
				  PortNumber = PortNum;
			   }
			   else if (!Port->Connected)
			   {
				  Port->Port = "COM" + IntToStr(PortNum);
				  Port->Open();
				  Success = true;
				  PortNumber = PortNum;
			   }
			   else // Port already open and on the right commport.
			   {
				  Success = true;
			   }
			}
			else
			{
			   if (Port->Connected)
			   {
				  Port->Close();
			   }

			}
		 }
		 __finally
		 {
			OpeningPort = false;
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message + " Port Number : " + IntToStr(PortNum));
	  Success = false;
   }
   return Success;
}
//-----------------------------------------------------------------------------
void TFiscalPort::Close()
{
   if (ClosingPort)
	  return;
   ClosingPort = true;
   try
   {
	  if (writeAsyncPtrList.size() > 0)
	  {
		 Port->AbortAllAsync();
		 writeAsyncPtrList.clear();
	  }
	  Port->Close();
   }
   __finally
   {
	  ClosingPort = false;
   }
}
//-----------------------------------------------------------------------------
AnsiString TFiscalPort::GetFiscalData()
{
   AnsiString response = "";
   try
   {
	  try
	  {
		 Port->ClearBuffer(false, true);
#define Max_Bytes 300
		 int CurrentTime, TimeOut;
		 unsigned char TempBuffer[Max_Bytes];
		 ZeroMemory(TempBuffer, Max_Bytes);
		 int BytesRead = Port->Read(TempBuffer, Max_Bytes);
		 UnicodeString Temp = UnicodeString((const char*)(TempBuffer), BytesRead);
		 LastData = Temp.Trim();
         response = LastData;
	  }
	  __finally
	  {
		 Port->ClearBuffer(true, false);
	  }
   }
   catch(Exception & E)
   {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Security Logging Exception", E.Message);
	  	Port->ClearBuffer(true, true);
   }
   return response;
}
//-----------------------------------------------------------------------------
void __fastcall TFiscalPort::GetData(TObject *Sender, int count)
{
   try
   {
	  try
	  {
		 Port->ClearBuffer(false, true);
#define Max_Bytes 300
		 int CurrentTime, TimeOut;
		 unsigned char TempBuffer[Max_Bytes];
		 ZeroMemory(TempBuffer, Max_Bytes);
		 int BytesRead = Port->Read(TempBuffer, Max_Bytes);
		 UnicodeString Temp = UnicodeString((const char*)(TempBuffer), BytesRead);
		 LastData = Temp.Trim();
	  }
	  __finally
	  {
		 Port->ClearBuffer(true, false);
	  }
   }
   catch(Exception & E)
   {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Security Logging Exception", E.Message);
	  	Port->ClearBuffer(true, true);
   }
}
//-----------------------------------------------------------------------------
bool TFiscalPort::AnalyzeData(AnsiString Response)
{
    bool retValue = false;
    switch(CurrentRequestType)
    {
        case eFiscalValidate:
        {
            retValue = CheckValidationResponse(Response);
            break;
        }
        case eFiscalReceipt:
        case eFiscalCopy:
        case eFiscalPractice:
        case eFiscalProof:
        {
            retValue = CheckPostResponse(Response);
            break;
        }
        default:
            break;
    }
    CurrentRequestType = eFiscalNone;
    return retValue;
}
//-----------------------------------------------------------------------------
AnsiString TFiscalPort::SetFiscalData(AnsiString Data,FiscalRequestType requestType)
{
   // This Code is Compatable with a Birch 20 char two line pole display.
   CurrentRequestType = requestType;
   AnsiString response = "";
   if (Busy)
	  return "";
   Busy = true;
   try
   {
	  if (Open(PortNumber))
	  {
		 if (AsyncMode)
		 {
			if (writeAsyncPtrList.size() < 2000)
			{
			   Data += "\r\n";
			   PAsync ptr;
			   InitAsync(ptr);
			   Port->WriteStrAsync(Data, ptr);
			   writeAsyncPtrList.push_back(ptr);
			}
			else
			{
			   TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Security Port Buffer Full.");
			}
		 }
		 else
		 {
			TMemoryStream *Stream = new TMemoryStream;
			try
			{
			   Data += "\r\n";

			   Stream->Position = 0;
			   Stream->Write(Data.c_str(), Data.Length());
			   Stream->Position = 0;
			   Port->Write((unsigned char*)(Stream->Memory), Stream->Size);
               response = GetFiscalData();
			}
			__finally
			{
               makeLogFile(Data, response);
			   delete Stream;
			}
		 }
	  }
   }
   __finally
   {
	  Busy = false;
   }
   return response;
}
//-----------------------------------------------------------------------------
void TFiscalPort::makeLogFile(AnsiString str1, AnsiString str2)
{
    AnsiString directoryName = ExtractFilePath(Application->ExeName) + "/PosPlus Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = Now().CurrentDate().FormatString("DDMMYYYY")+ ".txt";
    AnsiString fileName =  directoryName + "/" + name;
    std::auto_ptr<TStringList> List(new TStringList);
    if (FileExists(fileName) )
      List->LoadFromFile(fileName);

    List->Add("Request- "+ str1 +  "\n");
    List->Add("Response- "+ str2 +  "\n");
    List->Add("=============================================================================================================");
    List->Add("\n");

    List->SaveToFile(fileName );
}
//-----------------------------------------------------------------------------
void __fastcall TFiscalPort::PortTxEmpty(TObject *Sender)
{
   std::list <PAsync> ::iterator i;
   for (i = writeAsyncPtrList.begin(); i != writeAsyncPtrList.end(); )
   {
	  if (Port->IsAsyncCompleted(*i))
	  {
		 DoneAsync(*i);
		 writeAsyncPtrList.erase(i++);
	  }
	  else
	  {
		 ++i;
	  }
   }
}
//-----------------------------------------------------------------------------
bool TFiscalPort::CheckPostResponse(AnsiString inData)
{
    // Check if received zero at 0 location, if yes, Functionality should get enabled.
    int position = inData.Pos(0);
    MessageBox(position,"position of zero",MB_OK);
    return inData.Pos("0") == 1;
}
//-----------------------------------------------------------------------------
bool TFiscalPort::CheckValidationResponse(AnsiString inData)
{
    // Check if received zero at 0, On zero record in db would be marked as posted
    // if not zero, data will remain unposted.
    // if not zero, subsequest error message would be recorded. In all the cases
    // Manager class would be used.
    // but this method has to return true or false simply.
    int position = inData.Pos(0);
    MessageBox(position,"position of zero",MB_OK);
    return inData.Pos(0) == 1;
}
//-----------------------------------------------------------------------------
