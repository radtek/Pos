//---------------------------------------------------------------------------


#pragma hdrstop

#include <Forms.hpp>

#include "ProxReader.h"
#include "ManagerVariable.h"
#include "MMLogging.h"

#include "Enum.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TProxReader::TProxReader()
{
   Port = new TComPort(NULL);
   Port->BaudRate = br9600;
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
   Port->Timeouts->ReadTotalConstant = 300;
   CloseingPort = false;
   OpeningPort = false;
   Busy = false;
   ProxFormat = eproxStd;
   PortNumber = -1;
}

TProxReader::~TProxReader()
{
   delete Port;
}

TComPort *TProxReader::GetTComPort()
{
   return Port;
}

void TProxReader::Initialise(Database::TDBTransaction &DBTransaction)
{
   ProxFormat = (eProxFormat)TManagerVariable::Instance().GetInt(DBTransaction, vmProxWiegandFormat, (int)(eproxStd));

   int ProfileKey = TManagerVariable::Instance().GetInt(DBTransaction, vmProxProfile, -1);
   if (ProfileKey == -1)
   {
	  ProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
		 "ProxReader" + IntToStr(TManagerVariable::Instance().DeviceProfileKey));
	  TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmProxProfile, ProfileKey);
   }

   if (ProfileKey != 0)
   {
	  PortNumber = -1;
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialPort, PortNumber);

	  AsyncMode = true;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialAsync, AsyncMode);

	  int BaudRate = int(br2400);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialBaudRate, BaudRate);
	  Port->BaudRate = TBaudRate(BaudRate);

	  int DataBits = int(dbSeven);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialDataBits, DataBits);
	  Port->DataBits = TDataBits(DataBits);

	  int StopBits = int(sbOneStopBit);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialStopBits, StopBits);
	  Port->StopBits = TStopBits(StopBits);

	  int ParityBits = int(prEven);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialParityBits, ParityBits);
	  Port->Parity->Bits = TParityBits(ParityBits);

	  int ControlRTS = int(rtsHandshake);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialRTSFlowControl, ControlRTS);
	  Port->FlowControl->ControlRTS = TRTSFlowControl(ControlRTS);

	  bool OutCTSFlow = true;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialOutCTSFlow, OutCTSFlow);
	  Port->FlowControl->OutCTSFlow = OutCTSFlow;

	  bool XonXoffOut = false;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffOut, XonXoffOut);
	  Port->FlowControl->XonXoffOut = XonXoffOut;

	  bool XonXoffIn = false;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffIn, XonXoffIn);
	  Port->FlowControl->XonXoffIn = XonXoffIn;
   }
}

void TProxReader::SaveSettings(Database::TDBTransaction &DBTransaction)
{
   int ProfileKey = TManagerVariable::Instance().GetInt(DBTransaction, vmProxProfile, -1);
   if (ProfileKey == -1)
   {
	  ProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles,
		 "ProxReader" + IntToStr(TManagerVariable::Instance().DeviceProfileKey));
	  TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmProxProfile, ProfileKey);
   }

   if (ProfileKey != 0)
   {
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialPort, PortNumber);

	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialBaudRate, static_cast <int> (Port->BaudRate));

	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialDataBits, static_cast <int> (Port->DataBits));

	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialStopBits, static_cast <int> (Port->StopBits));

	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialParityBits, static_cast <int> (Port->Parity->Bits));

	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialRTSFlowControl,
		 static_cast <int> (Port->FlowControl->ControlRTS));

	  TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialOutCTSFlow, Port->FlowControl->OutCTSFlow);

	  TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffOut, Port->FlowControl->XonXoffOut);

	  TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffIn, Port->FlowControl->XonXoffIn);

	  TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialAsync, AsyncMode);
   }

   if (PortNumber != -1)
   {
	  Close();
	  Initialise(DBTransaction);
	  Open(PortNumber);
   }
}

bool TProxReader::Open(int PortNum)
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
			   if (Port->Connected)
			   {
				  Port->Close();
			   }
			   Port->Port = "COM" + IntToStr(PortNum);
			   Port->Open();
			   Success = true;
			   PortNumber = PortNum;
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

void TProxReader::Close()
{
   if (CloseingPort)
	  return;
   CloseingPort = true;
   try
   {
	  Port->Close();
   }
   __finally
   {
	  CloseingPort = false;
   }
}

void __fastcall TProxReader::GetData(TObject *Sender, int count)
{
	try
	{
		try
		{
			Port->ClearBuffer(false,true);
			#define Max_Bytes 20
			unsigned char TempBuffer[Max_Bytes];
			ZeroMemory(TempBuffer,Max_Bytes);
			int BytesRead =  Port->Read(TempBuffer,Max_Bytes);

         AnsiString Temp = AnsiString((const char *)(TempBuffer),BytesRead);
         if(ProxFormat == eproxWiegand)
         { // Skip the first 4 bytes.

            // Card Number : 15199
            // Mem  Number :  5666     0x3B5F
            // Hex 3B5F
            // 0011 1011 0101 1111
            // CC HID Reserved
            // DD Card Data
            // AA Cecksum
            //CCDDDDDDDDDDXX           From Reader
            //00000766639161\r\n       From Reader
            //     7666391              After Stripping.
//              7    6    6    7    6    B    E    A    1
//            0111 0110 0110 0111 0110 1011 1110 1010 0001
//            0x3B5F       | 3 |  B |  5 |  F |
//                         0 0111 0110 101 1111
//              0   0    1    F     F    F   E     -    -  // Mask 131070 dec
            //MMPROX15199     After Formatting.
            AnsiString CheckSumData = Temp.Trim();
            AnsiString strChecksum = "0x" + CheckSumData.SubString(CheckSumData.Length()-1,2);
            CheckSumData = CheckSumData.SubString(1,CheckSumData.Length()-2);
            int intChecksum =    StrToIntDef(strChecksum,0);
            int intCheck    =    0;
            for (int i = 1; i < CheckSumData.Length();i+=2)
            {
               AnsiString strCheck = "0x" + CheckSumData.SubString(i,2);
               intCheck +=  StrToIntDef(strCheck,0);
            }

            intCheck = intCheck & 0x00FF;
            if(intChecksum != intCheck)
            {
               throw Exception("Proximity Reader CRC error");
            }

            Temp = Temp.Trim();
            Temp = Temp.SubString(6,7);
            Temp = "0x" + Temp;
            int Card = StrToIntDef(Temp,0);
            if(Card != 0)
            {
               int mask = 131070;
               Card = Card & mask;
               int MemberNumber = Card >> 1;
               Temp = IntToStr(MemberNumber);
            }
         }

         if(Temp != "")
         {
            Temp = "MMPROX" + Temp;
   			LastCard = Temp.Trim();
            if(LastCard != AnsiString("MMPROX")) // Fix for AXEZE prox readers that send though "Nothing" while in proximity.
            {
	   			PostMessage(Screen->ActiveForm->Handle, UWM_CARDSWIPE, (UINT)&LastCard, NULL);
            }
         }
		}
		__finally
		{
			Port->ClearBuffer(true,false);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Proximity Exception",E.Message);
		Port->ClearBuffer(true,true);
	}
}
