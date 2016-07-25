//---------------------------------------------------------------------------


#pragma hdrstop

#include "SecurityPort.h"
#include "ManagerVariable.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TSecurityPort::TSecurityPort()
{
   Port = new TComPort(NULL);
   Port->BaudRate = br19200;
   Port->DataBits = dbEight;
   Port->Parity->Bits = prNone;
   Port->StopBits = sbOneStopBit;
   Port->Parity->Check = false;
   Port->Parity->Replace = false;
   Port->Buffer->InputSize = 16384;
   Port->Buffer->OutputSize = 16384;
   Port->DiscardNull = false;
   Port->OnRxChar = GetData;
   Port->Timeouts->ReadInterval = 200;
   Port->Timeouts->ReadTotalMultiplier = 200;
   Port->Timeouts->WriteTotalMultiplier = 200;
   Port->Timeouts->ReadTotalConstant = 300;
   Port->FlowControl->ControlDTR = dtrEnable;
   Port->FlowControl->ControlRTS = rtsHandshake;
   Port->FlowControl->OutCTSFlow = true;
   Port->FlowControl->XonXoffOut = false;
   Port->FlowControl->XonXoffIn = false;
   CloseingPort = false;
   OpeningPort = false;
   Busy = false;
   PortNumber = -1;
   AsyncMode = true;
   Port->OnTxEmpty = PortTxEmpty;
}

TSecurityPort::~TSecurityPort()
{
   Close();
   delete Port;
}

void TSecurityPort::SaveSettings(Database::TDBTransaction &DBTransaction)
{
   UnicodeString SecurityPortProfile = "SecuritySerialPort" + IntToStr(TManagerVariable::Instance().DeviceProfileKey);
   int SecurityProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, SecurityPortProfile);
   if (SecurityProfileKey == 0)
   {
	  SecurityProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eSystemProfiles, SecurityPortProfile);
   }

   if (SecurityProfileKey != 0)
   {
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, SecurityProfileKey, vmSerialPort, PortNumber);
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, SecurityProfileKey, vmSerialBaudRate, static_cast <int> (Port->BaudRate));
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, SecurityProfileKey, vmSerialDataBits, static_cast <int> (Port->DataBits));
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, SecurityProfileKey, vmSerialStopBits, static_cast <int> (Port->StopBits));
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, SecurityProfileKey, vmSerialParityBits, static_cast <int> (Port->Parity->Bits));
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, SecurityProfileKey, vmSerialRTSFlowControl,
		 static_cast <int> (Port->FlowControl->ControlRTS));
	  TManagerVariable::Instance().SetProfileBool(DBTransaction, SecurityProfileKey, vmSerialOutCTSFlow, Port->FlowControl->OutCTSFlow);
	  TManagerVariable::Instance().SetProfileBool(DBTransaction, SecurityProfileKey, vmSerialXonXoffOut, Port->FlowControl->XonXoffOut);
	  TManagerVariable::Instance().SetProfileBool(DBTransaction, SecurityProfileKey, vmSerialXonXoffIn, Port->FlowControl->XonXoffIn);
	  TManagerVariable::Instance().SetProfileBool(DBTransaction, SecurityProfileKey, vmSerialAsync, AsyncMode);
   }
}

void TSecurityPort::Initialise(Database::TDBTransaction &DBTransaction)
{
   UnicodeString SecurityPortProfile = "SecuritySerialPort" + IntToStr(TManagerVariable::Instance().DeviceProfileKey);
   int SecurityProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, SecurityPortProfile);
   bool EmptyProfile = false;
   if (SecurityProfileKey == 0)
   {
	  // Try the old Global Profile...
	  SecurityPortProfile = "SecuritySerialPort";
	  int SecurityProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, SecurityPortProfile);
	  if (SecurityProfileKey != 0)
	  {
		 PortNumber = -1;
		 TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialPort, PortNumber);

		 AsyncMode = true;
		 TManagerVariable::Instance().GetProfileBool(DBTransaction, SecurityProfileKey, vmSerialAsync, AsyncMode);

		 int BaudRate = int(br19200);
		 TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialBaudRate, BaudRate);
		 Port->BaudRate = TBaudRate(BaudRate);

		 int DataBits = int(dbEight);
		 TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialDataBits, DataBits);
		 Port->DataBits = TDataBits(DataBits);

		 int StopBits = int(sbOneStopBit);
		 TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialStopBits, StopBits);
		 Port->StopBits = TStopBits(StopBits);

		 int ParityBits = int(prNone);
		 TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialParityBits, ParityBits);
		 Port->Parity->Bits = TParityBits(ParityBits);

		 int ControlRTS = int(rtsHandshake);
		 TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialRTSFlowControl, ControlRTS);
		 Port->FlowControl->ControlRTS = TRTSFlowControl(ControlRTS);

		 bool OutCTSFlow = true;
		 TManagerVariable::Instance().GetProfileBool(DBTransaction, SecurityProfileKey, vmSerialOutCTSFlow, OutCTSFlow);
		 Port->FlowControl->OutCTSFlow = OutCTSFlow;

		 bool XonXoffOut = false;
		 TManagerVariable::Instance().GetProfileBool(DBTransaction, SecurityProfileKey, vmSerialXonXoffOut, XonXoffOut);
		 Port->FlowControl->XonXoffOut = XonXoffOut;

		 bool XonXoffIn = false;
		 TManagerVariable::Instance().GetProfileBool(DBTransaction, SecurityProfileKey, vmSerialXonXoffIn, XonXoffIn);
		 Port->FlowControl->XonXoffIn = XonXoffIn;

		 SaveSettings(DBTransaction);
	  }
   }
   else
   {
	  PortNumber = -1;
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialPort, PortNumber);

	  AsyncMode = true;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, SecurityProfileKey, vmSerialAsync, AsyncMode);

	  int BaudRate = int(br19200);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialBaudRate, BaudRate);
	  Port->BaudRate = TBaudRate(BaudRate);

	  int DataBits = int(dbEight);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialDataBits, DataBits);
	  Port->DataBits = TDataBits(DataBits);

	  int StopBits = int(sbOneStopBit);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialStopBits, StopBits);
	  Port->StopBits = TStopBits(StopBits);

	  int ParityBits = int(prNone);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialParityBits, ParityBits);
	  Port->Parity->Bits = TParityBits(ParityBits);

	  int ControlRTS = int(rtsHandshake);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, SecurityProfileKey, vmSerialRTSFlowControl, ControlRTS);
	  Port->FlowControl->ControlRTS = TRTSFlowControl(ControlRTS);

	  bool OutCTSFlow = true;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, SecurityProfileKey, vmSerialOutCTSFlow, OutCTSFlow);
	  Port->FlowControl->OutCTSFlow = OutCTSFlow;

	  bool XonXoffOut = false;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, SecurityProfileKey, vmSerialXonXoffOut, XonXoffOut);
	  Port->FlowControl->XonXoffOut = XonXoffOut;

	  bool XonXoffIn = false;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, SecurityProfileKey, vmSerialXonXoffIn, XonXoffIn);
	  Port->FlowControl->XonXoffIn = XonXoffIn;
   }
}

TComPort *TSecurityPort::GetTComPort()
{
   return Port;
}

bool TSecurityPort::Open(int PortNum)
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

void TSecurityPort::Close()
{
   if (CloseingPort)
	  return;
   CloseingPort = true;
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
	  CloseingPort = false;
   }
}

void __fastcall TSecurityPort::GetData(TObject *Sender, int count)
{
   try
   {
	  try
	  {
		 Port->ClearBuffer(false, true);
#define Max_Bytes 20
		 int CurrentTime, TimeOut;
		 unsigned char TempBuffer[Max_Bytes];
		 ZeroMemory(TempBuffer, Max_Bytes);
		 int BytesRead = Port->Read(TempBuffer, Max_Bytes);
		 UnicodeString Temp = "SECDATA" + UnicodeString((const char*)(TempBuffer), BytesRead);
		 LastData = Temp.Trim();
		 // PostMessage(Screen->ActiveForm->Handle, UWM_CARDSWIPE, (UINT)&LastData, NULL);
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

void __fastcall TSecurityPort::SetData(AnsiString Data)
{
   // This Code is Compatable with a Birch 20 char two line pole display.
   if (Busy)
	  return;
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
			}
			__finally
			{
			   delete Stream;
			}
		 }
	  }
   }
   __finally
   {
	  Busy = false;
   }
}

void __fastcall TSecurityPort::PortTxEmpty(TObject *Sender)
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
