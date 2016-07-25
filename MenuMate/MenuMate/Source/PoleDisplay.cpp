// ---------------------------------------------------------------------------

#pragma hdrstop

#include "PoleDisplay.h"
#include "DeviceRealTerminal.h"
#include "Enum.h"
#include "MMLogging.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

TPoleDisplay::TPoleDisplay()
{
   PoleDisplayPort = new TComPort(NULL);
   PoleDisplayPort->BaudRate = br9600;
   PoleDisplayPort->DataBits = dbEight;
   PoleDisplayPort->Parity->Bits = prNone;
   PoleDisplayPort->StopBits = sbOneStopBit;
   PoleDisplayPort->Buffer->InputSize = 16384;
   PoleDisplayPort->Buffer->OutputSize = 16384;
   PoleDisplayPort->DiscardNull = false;
   PoleDisplayPort->Timeouts->ReadInterval = 200;
   PoleDisplayPort->Timeouts->ReadTotalMultiplier = 200;
	PoleDisplayPort->Timeouts->WriteTotalMultiplier = 200;
	PoleDisplayPort->Timeouts->ReadTotalConstant = 300;
   PoleDisplayPort->FlowControl->ControlDTR = dtrEnable;
   PoleDisplayPort->FlowControl->ControlRTS = rtsHandshake;
   PoleDisplayPort->FlowControl->OutCTSFlow = true;
   PoleDisplayPort->FlowControl->XonXoffOut = false;
	PoleDisplayPort->FlowControl->XonXoffIn = false;
   PoleDisplayPort->OnTxEmpty = PortTxEmpty;
   EmulationMode = ePoleDispCD5220;
   CloseingPort = false;
   OpeningPort = false;
   OpeningFailed = false;
   AsyncMode = true;
   Busy = false;
   PortNumber = 0;
}

TPoleDisplay::~TPoleDisplay()
{
   Close();
   delete PoleDisplayPort;
}

bool TPoleDisplay::Open(int PortNum)
{
   bool retval = false;
   if (OpeningPort)
	  return retval;
   OpeningPort = true;
   OpeningFailed = false;
   try
   {
	  try
	  {
		 if (PortNum > 0)
		 {
			PoleDisplayPort->Port = "COM" + IntToStr(PortNum);
			PoleDisplayPort->Open();

			if (EmulationMode == ePoleDispCD5220)
			{
			   char SetMode[] =
			   {
				  0x1B, 0x40
			   };
			   if (AsyncMode && writeAsyncPtrList.size() < 2000)
			   {
                  AnsiString Data(SetMode,sizeof(SetMode));
				  AsyncWrite(Data);
			   }
               else if(!AsyncMode)
               {

                  TMemoryStream *Stream = new TMemoryStream;
                  try
                  {
                     Stream->Position = 0;
                     Stream->Write(SetMode, sizeof(SetMode));
                     Stream->Position = 0;
							PoleDisplayPort->Write((unsigned char*)(Stream->Memory), Stream->Size);
                  }
                  __finally
                  {
                     delete Stream;
                  }
               }
			}
			PortNumber = PortNum;
			retval = true;
		 }
		 else
		 {
			if (PoleDisplayPort->Connected)
			{
			   PoleDisplayPort->Close();
			}
		 }
	  }
	  catch(Exception & E)
	  {
			TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message + " Port Number : " + IntToStr(PortNum));
			OpeningFailed = true;
	  }
   }
   __finally
   {
	  OpeningPort = false;
   }
   return retval;
}

void TPoleDisplay::Close()
{
   if (CloseingPort)
	  return;
   CloseingPort = true;
   try
   {
	  if (writeAsyncPtrList.size() > 0)
	  {
		 PoleDisplayPort->AbortAllAsync();
		 writeAsyncPtrList.clear();
	  }

	  PoleDisplayPort->Close();
   }
   __finally
   {
	  CloseingPort = false;
   }
}

void TPoleDisplay::UpdatePoleDisplay(AnsiString TopLineLeft, AnsiString TopLineRight, AnsiString BottomLineLeft, AnsiString BottomLineRight)
{
   // This Code is Compatable with a Birch 20 char two line pole display.
   if (Busy)
	  return;
   Busy = true;
   try
   {
	  if (!OpeningFailed && Open(PortNumber))
	  {
		 TMemoryStream *Stream = new TMemoryStream;
		 try
		 {
			bool ShowTopLine = TopLineLeft != "";
			bool ShowBottomLine = BottomLineLeft != "";

			AnsiString CmdTopLn = "";
			AnsiString CmdBotLn = "";

			switch(EmulationMode)
			{
			case ePoleDispCD5220:
			   {
				  CmdTopLn = AnsiString(char(0x1B)) + "QA";
				  CmdBotLn = AnsiString(char(0x1B)) + "QB";
			   }break;
			case ePoleDispEscPOS:
			   {
				  char temp[] =
				  {
					 0x1F, 0x24, 0x01, 0x01
				  };
				  CmdTopLn = AnsiString(temp, sizeof(temp));
				  char temp2[] =
				  {
					 0x1F, 0x24, 0x01, 0x02
				  };
				  CmdBotLn = AnsiString(temp2, sizeof(temp2));
			   }break;
			case ePoleDispDSPUTC:
			   {
				  char temp[] =
				  {
					 0x1B, 0x64, 0x1B, 0x75, 0x41
				  };
				  CmdTopLn = AnsiString(temp, sizeof(temp));
				  char temp2[] =
				  {
					 0x1B, 0x64, 0x1B, 0x75, 0x42
				  };
				  CmdBotLn = AnsiString(temp2, sizeof(temp2));
			   }break;
			case ePoleDispDSPEMaxAdex:
			   {
				  char temp[] =
				  {
					 0x21, 0x23, 0x31
				  };
				  CmdTopLn = AnsiString(temp, sizeof(temp));
				  char temp2[] =
				  {
					 0x21, 0x23, 0x32
				  };
				  CmdBotLn = AnsiString(temp2, sizeof(temp2));
			   }
			case ePoleDispToshiba:
			   {
				  char temp[] =
				  {
					 0x1B, 0x5B, 0x31, 0x3B, 0x31, 0x48, 0x1B, 0x5B, 0x30, 0x4B
				  };
				  CmdTopLn = AnsiString(temp, sizeof(temp));
				  char temp2[] =
				  {
					 0x1B, 0x5B, 0x32, 0x3B, 0x31, 0x48, 0x1B, 0x5B, 0x30, 0x4B
				  };
				  CmdBotLn = AnsiString(temp2, sizeof(temp2));
			   }break;
			default:
			   {
			   }
			}

			TopLineRight = " " + TopLineRight;
			BottomLineRight = " " + BottomLineRight;

			TopLineRight = TopLineRight.SubString(1, 20);
			BottomLineRight = BottomLineRight.SubString(1, 20);

			TopLineLeft = TopLineLeft + AnsiString::StringOfChar(' ', 20 - TopLineRight.Length());
			BottomLineLeft = BottomLineLeft + AnsiString::StringOfChar(' ', 20 - BottomLineRight.Length());

			TopLineLeft = TopLineLeft.SubString(1, 20 - TopLineRight.Length()); // Gap for space
			BottomLineLeft = BottomLineLeft.SubString(1, 20 - BottomLineRight.Length());

			switch(EmulationMode)
			{
			case ePoleDispCD5220:
			case ePoleDispDSPEMaxAdex:
			   TopLineRight += AnsiString(char(0x0D));
			   BottomLineRight += AnsiString(char(0x0D));
			   break;
			default:
			   {
			   }
			}

			if (ShowTopLine)
			{
			   if (AsyncMode && writeAsyncPtrList.size() < 2000)
			   {
				  AsyncWrite(CmdTopLn + TopLineLeft + TopLineRight);
			   }
               else if(!AsyncMode)
               {
                  Stream->Position = 0;
                  Stream->Write(CmdTopLn.c_str(), CmdTopLn.Length());
                  Stream->Write(TopLineLeft.c_str(), TopLineLeft.Length());
                  Stream->Write(TopLineRight.c_str(), TopLineRight.Length());
                  Stream->Position = 0;
                  PoleDisplayPort->Write((unsigned char*)(Stream->Memory), Stream->Size);
               }
			}

			if (ShowBottomLine)
			{
			   if (AsyncMode && writeAsyncPtrList.size() < 2000)
			   {
				  AsyncWrite(CmdBotLn + BottomLineLeft + BottomLineRight);
			   }
               else if(!AsyncMode)
               {
                  Stream->Position = 0;
                  Stream->Write(CmdBotLn.c_str(), CmdBotLn.Length());
                  Stream->Write(BottomLineLeft.c_str(), BottomLineLeft.Length());
                  Stream->Write(BottomLineRight.c_str(), BottomLineRight.Length());
                  Stream->Position = 0;
                  PoleDisplayPort->Write((unsigned char*)(Stream->Memory), Stream->Size);
               }
			}

		 }
		 __finally
		 {
			delete Stream;
		 }
	  }
   }
   __finally
   {
	  Busy = false;
   }
}

void TPoleDisplay::AsyncWrite(AnsiString Data)
{
   PAsync ptr;
   InitAsync(ptr);
   PoleDisplayPort->WriteAsync(Data.c_str(),Data.Length(), ptr);
   writeAsyncPtrList.push_back(ptr);
}

void TPoleDisplay::UpdatePoleDisplay(AnsiString TopLineLeft, AnsiString BottomLineLeft)
{
   // This Code is Compatable with a Birch 20 char two line pole display.
   if (Busy)
	  return;
   Busy = true;
   try
   {
	  if (!OpeningFailed && Open(PortNumber))
	  {
		 TMemoryStream *Stream = new TMemoryStream;
		 try
		 {
			AnsiString CmdTopLn = "";
			AnsiString CmdBotLn = "";

			switch(EmulationMode)
			{
			case ePoleDispCD5220:
			   {
				  CmdTopLn = AnsiString(char(0x1B)) + "QA";
				  CmdBotLn = AnsiString(char(0x1B)) + "QB";
			   }break;
			case ePoleDispEscPOS:
			   {
				  char temp[] =
				  {
					 0x1F, 0x24, 0x01, 0x01
				  };
				  CmdTopLn = AnsiString(temp, sizeof(temp));
				  char temp2[] =
				  {
					 0x1F, 0x24, 0x01, 0x02
				  };
				  CmdBotLn = AnsiString(temp2, sizeof(temp2));
			   }break;
			case ePoleDispDSPUTC:
			   {
				  char temp[] =
				  {
					 0x1B, 0x64, 0x1B, 0x75, 0x41
				  };
				  CmdTopLn = AnsiString(temp, sizeof(temp));
				  char temp2[] =
				  {
					 0x1B, 0x64, 0x1B, 0x75, 0x42
				  };
				  CmdBotLn = AnsiString(temp2, sizeof(temp2));
			   }break;
			case ePoleDispDSPEMaxAdex:
			   {
				  char temp[] =
				  {
					 0x21, 0x23, 0x31
				  };
				  CmdTopLn = AnsiString(temp, sizeof(temp));
				  char temp2[] =
				  {
					 0x21, 0x23, 0x32
				  };
				  CmdBotLn = AnsiString(temp2, sizeof(temp2));
			   }
			case ePoleDispToshiba:
			   {
				  char temp[] =
				  {
					 0x1B, 0x5B, 0x31, 0x3B, 0x31, 0x48, 0x1B, 0x5B, 0x30, 0x4B
				  };
				  CmdTopLn = AnsiString(temp, sizeof(temp));
				  char temp2[] =
				  {
					 0x1B, 0x5B, 0x32, 0x3B, 0x31, 0x48, 0x1B, 0x5B, 0x30, 0x4B
				  };
				  CmdBotLn = AnsiString(temp2, sizeof(temp2));
			   }break;
			default:
			   {
			   }
			}

			if (TopLineLeft.Length() > 20)
			{
			   TopLineLeft = TopLineLeft.SubString(1, 20);
			}
			else if (TopLineLeft.Length() < 20)
			{
			   TopLineLeft = TopLineLeft + AnsiString::StringOfChar(' ', 20 - TopLineLeft.Length());
			}

			if (BottomLineLeft.Length() > 20)
			{
			   BottomLineLeft = BottomLineLeft.SubString(1, 20);
			}
			else if (BottomLineLeft.Length() < 20)
			{
			   BottomLineLeft = BottomLineLeft + AnsiString::StringOfChar(' ', 20 - BottomLineLeft.Length());
			}

			switch(EmulationMode)
			{
			case ePoleDispCD5220:
			   TopLineLeft += AnsiString(char(0x0D));
			   BottomLineLeft += AnsiString(char(0x0D));
			   break;
			default:
			   {
			   }
			}

            if (AsyncMode && writeAsyncPtrList.size() < 2000)
            {
               AsyncWrite(CmdTopLn + TopLineLeft + CmdBotLn + BottomLineLeft);
            }
            else if(!AsyncMode)
            {
               Stream->Position = 0;
               Stream->Write(CmdTopLn.c_str(), CmdTopLn.Length());
               Stream->Write(TopLineLeft.c_str(), TopLineLeft.Length());
               Stream->Position = 0;
               PoleDisplayPort->Write((unsigned char*)(Stream->Memory), Stream->Size);

               Stream->Position = 0;
               Stream->Write(CmdBotLn.c_str(), CmdBotLn.Length());
               Stream->Write(BottomLineLeft.c_str(), BottomLineLeft.Length());
               Stream->Position = 0;
               PoleDisplayPort->Write((unsigned char*)(Stream->Memory), Stream->Size);
            }
		 }
		 __finally
		 {
			delete Stream;
		 }
	  }
   }
   __finally
   {
	  Busy = false;
   }
}

void TPoleDisplay::UpdatePoleDisplayDefault()
{
   if (TGlobalSettings::Instance().PoleDisplayTopLine == "" && TGlobalSettings::Instance().PoleDisplayBottomLine == "")
   {
	  ClearPoleDisplay();
   }
   else
   {
	  UpdatePoleDisplay(TGlobalSettings::Instance().PoleDisplayTopLine, "", TGlobalSettings::Instance().PoleDisplayBottomLine, "");
   }
}

void TPoleDisplay::ClearPoleDisplay()
{
   // This Code is Compatable with a Birch 20 char two line pole display.
   if (Busy)
	  return;
   Busy = true;
   try
   {
	  if (!OpeningFailed && Open(PortNumber))
	  {
		 TMemoryStream *Stream = new TMemoryStream;
		 try
		 {
			AnsiString CmdTopLn = "";
			AnsiString TopLineLeft = "";
			AnsiString CmdBotLn = "";
			AnsiString BottomLineLeft = "";

			switch(EmulationMode)
			{
			case ePoleDispCD5220:
			case ePoleDispEscPOS:
			case ePoleDispDSPEMaxAdex:
			   {
				  CmdTopLn = AnsiString(char(0x0C));
			   }break;
			case ePoleDispDSPUTC:
			   {
				  char temp[] =
				  {
					 0x1B, 0x0F, 0x0D, 0x1F
				  };
				  CmdTopLn = AnsiString(temp, sizeof(temp));
			   }break;
			case ePoleDispToshiba:
			   {
				  char temp[] =
				  {
					 0x1B, 0x5B, 0x32, 0x4A
				  };
				  CmdTopLn = AnsiString(temp, sizeof(temp));
			   }break;
			default:
			   {
				  AnsiString TopLineLeft = AnsiString::StringOfChar(' ', 20);
				  AnsiString BottomLineLeft = AnsiString::StringOfChar(' ', 20);
			   }
			}

			switch(EmulationMode)
			{
			case ePoleDispCD5220:
			case ePoleDispEscPOS:
			   break;
			default:
			   {
				  TopLineLeft += '\r';
				  BottomLineLeft += '\r';
			   }
			}

            if (AsyncMode && writeAsyncPtrList.size() < 2000)
            {
               AsyncWrite(CmdTopLn + TopLineLeft + CmdBotLn + BottomLineLeft);
            }
            else if(!AsyncMode)
            {

               Stream->Position = 0;
               Stream->Write(CmdTopLn.c_str(), CmdTopLn.Length());
               Stream->Write(TopLineLeft.c_str(), TopLineLeft.Length());
               Stream->Position = 0;
               PoleDisplayPort->Write((unsigned char*)(Stream->Memory), Stream->Size);

               Stream->Position = 0;
               Stream->Write(CmdBotLn.c_str(), CmdBotLn.Length());
               Stream->Write(BottomLineLeft.c_str(), BottomLineLeft.Length());
               Stream->Position = 0;
               PoleDisplayPort->Write((unsigned char*)(Stream->Memory), Stream->Size);
            }
		 }
		 __finally
		 {
			delete Stream;
		 }
	  }
   }
   __finally
   {
	  Busy = false;
   }
}

TComPort *TPoleDisplay::GetTComPort()
{
   return PoleDisplayPort;
}

void __fastcall TPoleDisplay::PortTxEmpty(TObject *Sender)
{
   std::list <PAsync> ::iterator i;
   for (i = writeAsyncPtrList.begin(); i != writeAsyncPtrList.end(); )
   {
      PAsync async = *i;
	  if (PoleDisplayPort->IsAsyncCompleted(async))
	  {
		 DoneAsync(async);
		 writeAsyncPtrList.erase(i);
         i = writeAsyncPtrList.begin();
	  }
	  else
	  {
		 ++i;
	  }
   }
}

void TPoleDisplay::Initialise(Database::TDBTransaction &DBTransaction)
{
   UnicodeString PortProfile = "PoleDisplaySerialPort" + IntToStr(TManagerVariable::Instance().DeviceProfileKey);
   int ProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, PortProfile);
   bool EmptyProfile = false;
   if (ProfileKey != 0)
   {
	  PortNumber = -1;
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialPort, PortNumber);

	  AsyncMode = true;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialAsync, AsyncMode);

	  int BaudRate = int(br19200);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialBaudRate, BaudRate);
	  PoleDisplayPort->BaudRate = TBaudRate(BaudRate);

	  int DataBits = int(dbEight);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialDataBits, DataBits);
	  PoleDisplayPort->DataBits = TDataBits(DataBits);

	  int StopBits = int(sbOneStopBit);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialStopBits, StopBits);
	  PoleDisplayPort->StopBits = TStopBits(StopBits);

	  int ParityBits = int(prNone);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialParityBits, ParityBits);
	  PoleDisplayPort->Parity->Bits = TParityBits(ParityBits);

	  int ControlRTS = int(rtsHandshake);
	  TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialRTSFlowControl, ControlRTS);
	  PoleDisplayPort->FlowControl->ControlRTS = TRTSFlowControl(ControlRTS);

	  bool OutCTSFlow = true;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialOutCTSFlow, OutCTSFlow);
	  PoleDisplayPort->FlowControl->OutCTSFlow = OutCTSFlow;

	  bool XonXoffOut = false;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffOut, XonXoffOut);
	  PoleDisplayPort->FlowControl->XonXoffOut = XonXoffOut;

	  bool XonXoffIn = false;
	  TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffIn, XonXoffIn);
	  PoleDisplayPort->FlowControl->XonXoffIn = XonXoffIn;
   }
}

void TPoleDisplay::SaveSettings(Database::TDBTransaction &DBTransaction)
{
   UnicodeString PoleDisplayPortProfile = "PoleDisplaySerialPort" + IntToStr(TManagerVariable::Instance().DeviceProfileKey);
   int ProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, PoleDisplayPortProfile);
   if (ProfileKey == 0)
   {
	  ProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eSystemProfiles, PoleDisplayPortProfile);
   }

   if (ProfileKey != 0)
   {
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialPort, PortNumber);
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialBaudRate, static_cast <int> (PoleDisplayPort->BaudRate));
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialDataBits, static_cast <int> (PoleDisplayPort->DataBits));
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialStopBits, static_cast <int> (PoleDisplayPort->StopBits));
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialParityBits, static_cast <int> (PoleDisplayPort->Parity->Bits));
	  TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialRTSFlowControl,
		 static_cast <int> (PoleDisplayPort->FlowControl->ControlRTS));
	  TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialOutCTSFlow, PoleDisplayPort->FlowControl->OutCTSFlow);
	  TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffOut, PoleDisplayPort->FlowControl->XonXoffOut);
	  TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffIn, PoleDisplayPort->FlowControl->XonXoffIn);
	  TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialAsync, AsyncMode);
   }
}
