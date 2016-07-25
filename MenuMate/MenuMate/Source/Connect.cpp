//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "comms.h"
#include "enum.h"
#include "Data.h"
#include "IBLogs.h"
#include "stdio.h"
#define MESSAGETIME

#pragma package(smart_init)


#ifdef  PalmMate
#include "Palm.h"
#ifdef  IR_Palm
	#include "DataLink.h"
#endif
#endif
//---------------------------------------------------------------------------

//------------------------------------------------------------------------------

//---------------------------------------------------------------------------
//-------------------------TConnect Member Functions-----------------
//---------------------------------------------------------------------------

__fastcall TConnect::TConnect()
{
   #ifdef  IR_Palm
  	DataLinkControl = new TDataLinkControl(this);
   #endif
   ErrorCount = 0;
   DownTime = 0;
   Offline = false;
   LastOfflineStatus = false;
	Name = "";
	ConnectionKey = 0;
   ID = 0;
}

__fastcall TConnect::~TConnect()
{
   #ifdef  IR_Palm
	delete DataLinkControl;
	DataLinkControl = NULL;
   #endif
}

bool TConnect::ClassNameIs(AnsiString Name)
{
   if(ClassName == Name)
   {
      return true;
   }
   else
   {
      return false;
   }
}



//------------------------------------------------------------------------------
int TConnect::Locate(int DeviceKey)
{
   bool StartTrans = false;
   int RetVal = 0;
	try
   {
      if (!TDeviceRealTerminal::Instance().IBTransaction->InTransaction) {TDeviceRealTerminal::Instance().IBTransaction->StartTransaction(); StartTrans = true; }

      TDeviceRealTerminal::Instance().IBQuery->Close();
      TDeviceRealTerminal::Instance().IBQuery->SQL->Text =
      "SELECT CONNECT_KEY "
      "FROM "
         "CONNECTIONS "
      "WHERE "
         "DEVICE_KEY = :DEVICE_KEY AND "
         "CONNECT_NAME = :CONNECT_NAME AND "
         "CONNECT_TYPE = :CONNECT_TYPE";
      TDeviceRealTerminal::Instance().IBQuery->ParamByName("DEVICE_KEY")->AsString = DeviceKey;
      TDeviceRealTerminal::Instance().IBQuery->ParamByName("CONNECT_NAME")->AsString = this->Name;
      TDeviceRealTerminal::Instance().IBQuery->ParamByName("CONNECT_TYPE")->AsInteger = this->TRXType;
      TDeviceRealTerminal::Instance().IBQuery->Open();

      if(TDeviceRealTerminal::Instance().IBQuery->IsEmpty())
      {
         TDeviceRealTerminal::Instance().IBQuery->Close();
         TDeviceRealTerminal::Instance().IBQuery->SQL->Text =
         "SELECT CONNECT_KEY "
			"FROM "
            "CONNECTIONS "
         "WHERE "
            "DEVICE_KEY = :DEVICE_KEY AND "
            "CONNECT_KEY = :CONNECT_KEY";
         TDeviceRealTerminal::Instance().IBQuery->ParamByName("DEVICE_KEY")->AsString = DeviceKey;
         TDeviceRealTerminal::Instance().IBQuery->ParamByName("CONNECT_KEY")->AsString = this->ConnectionKey;
         TDeviceRealTerminal::Instance().IBQuery->Open();
         if(!TDeviceRealTerminal::Instance().IBQuery->IsEmpty())
         {
	         RetVal = TDeviceRealTerminal::Instance().IBQuery->FieldByName("CONNECT_KEY")->AsInteger;
         }
      }
      else
      {
         RetVal = TDeviceRealTerminal::Instance().IBQuery->FieldByName("CONNECT_KEY")->AsInteger;
      }
      if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Commit(); }
   }
   catch(Exception &E)
   {
      if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Rollback(); }
      DeviceKey = 0;
	}
   return RetVal;
}

//------------------------------------------------------------------------------

void TConnect::Verify(int ParentDeviceKey, bool Create)
{
  TBookmark SavePlace;
  ConnectionKey = Locate(ParentDeviceKey);
  if(ConnectionKey == 0) // Opens DB Table.
  {
      if(Create)
      {
         bool StartTrans = false;
         try
         {
            if (!TDeviceRealTerminal::Instance().IBTransaction->InTransaction) {TDeviceRealTerminal::Instance().IBTransaction->StartTransaction(); StartTrans = true; }
            TDeviceRealTerminal::Instance().IBQuery->Close();
            TDeviceRealTerminal::Instance().IBQuery->SQL->Text = "SELECT GEN_ID(GEN_CONNECTIONS, 1) FROM RDB$DATABASE";
            TDeviceRealTerminal::Instance().IBQuery->Open();
            ConnectionKey = TDeviceRealTerminal::Instance().IBQuery->Fields->Fields[0]->AsInteger;

				TDeviceRealTerminal::Instance().IBQuery->Close();
            TDeviceRealTerminal::Instance().IBQuery->SQL->Text =
            "INSERT INTO CONNECTIONS ("
               "CONNECT_KEY,"
               "DEVICE_KEY,"
               "CONNECT_NAME,"
               "CONNECT_ID,"
					"CONNECT_TYPE,"
					"DISABLED) "
				"VALUES ("
					":CONNECT_KEY,"
					":DEVICE_KEY,"
					":CONNECT_NAME,"
					":CONNECT_ID,"
					":CONNECT_TYPE,"
					":DISABLED);";
				TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_KEY")->AsInteger = ConnectionKey;
				TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("DEVICE_KEY")->AsInteger = ParentDeviceKey;
				TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_NAME")->AsString = this->Name;
				TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_ID")->AsInteger = this->ID;
				TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_TYPE")->AsInteger = this->TRXType;
				TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("DISABLED")->AsString = "F";
            TDeviceRealTerminal::Instance().IBQuery->ExecSQL();

				if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Commit(); }
         }
			catch(Exception &E)
			{
				TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
				if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Rollback(); }
         }
      }
   }
   else // update ID
   {
      bool StartTrans = false;
      try
      {
         if (!TDeviceRealTerminal::Instance().IBTransaction->InTransaction) {TDeviceRealTerminal::Instance().IBTransaction->StartTransaction(); StartTrans = true; }

         TDeviceRealTerminal::Instance().IBQuery->Close();
         TDeviceRealTerminal::Instance().IBQuery->SQL->Text =
            "UPDATE CONNECTIONS "
            "SET "
               "DEVICE_KEY = :DEVICE_KEY, "
               "CONNECT_NAME = :CONNECT_NAME, "
					"CONNECT_ID = :CONNECT_ID, "
					"CONNECT_TYPE = :CONNECT_TYPE, "
					"DISABLED = :DISABLED "
            "WHERE "
               "CONNECT_KEY	= :CONNECT_KEY";
         TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_KEY")->AsInteger = ConnectionKey;
         TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("DEVICE_KEY")->AsInteger = ParentDeviceKey;
         TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_NAME")->AsString = this->Name;
         TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_ID")->AsInteger = this->ID;
			TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("CONNECT_TYPE")->AsInteger = this->TRXType;
			TDeviceRealTerminal::Instance().IBQuery->Params->ParamByName("DISABLED")->AsString = "F";
         TDeviceRealTerminal::Instance().IBQuery->ExecSQL();
         if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Commit(); }
      }
		catch(Exception &E)
      {
         TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
         if (TDeviceRealTerminal::Instance().IBTransaction->InTransaction && StartTrans) { TDeviceRealTerminal::Instance().IBTransaction->Rollback(); }
      }
   }
}

//------------------------------------------------------------------------------
//-----------------------------TConnectPrinter----------------------------------
//------------------------------------------------------------------------------
__fastcall TConnectPrinter::TConnectPrinter()
{
   ClassName = "TConnectPrinter";
   TRXType = Unknown_TRX_Station;
   LastOfflineStatus = true; // Assume was offline.
	Offline = false;  // But is now Online.
	VirtualPrinterKey = 0;
	PhysicalPrinterKey = 0;
	WindowsDriverKey = 0;
	PhysicalProfileKey = 0;
}


void TConnectPrinter::LoadFormattedStream(TMemoryStream *JobStream)
{
	StreamPackets(JobStream);
   PrintJob->Clear();
}
//------------------------------------------------------------------------------

__fastcall TConnectPrinter::~TConnectPrinter()
{
}

//Print Job formatting routines.
void TConnectPrinter::AddLine(AnsiString Data)
{
   Data = Data.Trim(); // Remove Leading Spaces and any control characters.
   AnsiString CurrentLine = Data.SubString(1,CharPerLine);
   PrintJob->Write(CurrentLine.c_str(),CurrentLine.Length());
   NewLine();
}

//------------------------------------------------------------------------------

void TConnectPrinter::AddLines(AnsiString Data)
{
   PrintJob->Write(Data.c_str(),Data.Length());
   NewLine();
}

//------------------------------------------------------------------------------

void TConnectPrinter::Add(AnsiString Data)
{
   if(LargeFont)
   {
      Data = Data.Trim(); // Remove Leading Spaces and any control characters.
		AnsiString CurrentLine = Data.SubString(1,CharPerLine-1);
      PrintJob->Write(CurrentLine.c_str(),CurrentLine.Length());
   }
   else
   {
      Data = Data.Trim(); // Remove Leading Spaces and any control characters.
      AnsiString CurrentLine = Data.SubString(1,CharPerLine-1);
      PrintJob->Write(CurrentLine.c_str(),CurrentLine.Length());
   }
   NewLine();
}

void TConnectPrinter::AddRawPrintJob(AnsiString Data)
{
     PrintJob->Write(Data.c_str(),Data.Length());
}
//------------------------------------------------------------------------------

void TConnectPrinter::AddRightJustify(AnsiString Col1, AnsiString Col2)
{
   if(LargeFont)
   {
      int LineLength = CharPerLine;
		if(isBold)
      {
         LineLength -= 2;
      }

      int TotalTextLength = Col1.Length() + Col2.Length() + GapWidthLge;
      if(TotalTextLength > LineLength)
      {
         int LengthToSubtract = TotalTextLength - LineLength;
         Col1 = Col1.SubString(1,(Col1.Length() - LengthToSubtract));

         AnsiString Gap;
         Gap = Gap.StringOfChar(' ',GapWidthLge);
         AnsiString Line = Col1 + Gap + Col2;
         PrintJob->Write(Line.c_str(),Line.Length());
      }
      else if(TotalTextLength < LineLength)
      { // Pack out the Gap to make up the Difference.
         int LengthToPackGap = LineLength - TotalTextLength;
         AnsiString Gap;
         Gap = Gap.StringOfChar(' ',LengthToPackGap + GapWidthLge);
         AnsiString Line = Col1 + Gap + Col2;
         PrintJob->Write(Line.c_str(),Line.Length());
		}
      else
      {
         AnsiString Gap;
         Gap = Gap.StringOfChar(' ',GapWidthLge);
         AnsiString Line = Col1 + Gap + Col2;
         PrintJob->Write(Line.c_str(),Line.Length());
      }
   }
   else
   {
      int LineLength = CharPerLine;
      if(isBold)
      {
         LineLength -= 2;
      }
      int TotalTextLength = Col1.Length() + Col2.Length() + GapWidthSml;
      if(TotalTextLength > LineLength)
      {
         int LengthToSubtract = TotalTextLength - LineLength;
         if(isBold)
         {
            LengthToSubtract += 4;
			}
         Col1 = Col1.SubString(1,(Col1.Length() - LengthToSubtract));

         AnsiString Gap;
         Gap = Gap.StringOfChar(' ',GapWidthSml);
         AnsiString Line = Col1 + Gap + Col2;
         PrintJob->Write(Line.c_str(),Line.Length());
      }
      else if(TotalTextLength < CharPerLine)
      { // Pack out the Gap to make up the Difference.
         int LengthToPackGap = LineLength - TotalTextLength;
         if(isBold)
         {
            LengthToPackGap -= 4;
         }
         AnsiString Gap;
         Gap = Gap.StringOfChar(' ',LengthToPackGap + GapWidthSml);
         AnsiString Line = Col1 + Gap + Col2;
         PrintJob->Write(Line.c_str(),Line.Length());
      }
      else
      {
         AnsiString Gap;
			if(isBold)
         {
            GapWidthSml += 4;
         }
         Gap = Gap.StringOfChar(' ',GapWidthSml);
         AnsiString Line = Col1 + Gap + Col2;
         PrintJob->Write(Line.c_str(),Line.Length());
      }
   }
   NewLine();
}

void TConnectPrinter::AddCentered(AnsiString Col1)
{
   int LineLength = CharPerLine;
   if(LargeFont)
   {
      if(isBold)
      {
         LineLength--;
      }

      int TotalTextLength = Col1.Length();
		if(TotalTextLength > LineLength)
      {
         int LengthToSubtract = TotalTextLength - LineLength;
         Col1 = Col1.SubString(1,(Col1.Length() - LengthToSubtract));
         AnsiString Line = Col1;
         PrintJob->Write(Line.c_str(),Line.Length());
      }
      else
      { // Pack out the Gap to center
         int LengthToPackGap = (LineLength - TotalTextLength)/2;
         AnsiString Gap;
         Gap = Gap.StringOfChar(' ',LengthToPackGap);
         AnsiString Line = Gap + Col1;
         PrintJob->Write(Line.c_str(),Line.Length());
      }
   }
   else
   {
      int TotalTextLength = Col1.Length();
      if(isBold)
      {
         LineLength--;
      }

      if(TotalTextLength > LineLength)
      {
         int LengthToSubtract = TotalTextLength - LineLength;
         Col1 = Col1.SubString(1,(Col1.Length() - LengthToSubtract));
         AnsiString Line = Col1;;
         PrintJob->Write(Line.c_str(),Line.Length());
      }
      else
      { // Pack out the Gap to center...
         int LengthToPackGap = (LineLength - TotalTextLength)/2;
         AnsiString Gap;
         Gap = Gap.StringOfChar(' ',LengthToPackGap);
         AnsiString Line = Gap + Col1;
         PrintJob->Write(Line.c_str(),Line.Length());
      }
   }
   NewLine();
}


//------------------------------------------------------------------------------

void TConnectPrinter::Tab()
{
	char CmdTab[1] = {0x09};
	PrintJob->Write(CmdTab,sizeof(CmdTab));
}

//------------------------------------------------------------------------------

void TConnectPrinter::Font(bool inLarge,bool inBold,bool inUnderLined, bool Red)
{
   char CmdFont[3] = {0x1B,0x21,0x00};

   LargeFont = inLarge;
   isBold = inBold;
   UnderLined = inUnderLined;

   CharPerLine = NormalCharPerLine;
   char font = 0x00;

   if(PrinterType == A714)
   {
      if(Red == true)
      {
			char CmdRedOn[3] = {0x1B,0x72,0x01};
         PrintJob->Write(CmdRedOn,sizeof(CmdRedOn));
      }
      else
      {
         char CmdRedOff[3] = {0x1B,0x72,0x00};
         PrintJob->Write(CmdRedOff,sizeof(CmdRedOff));
      }
   }

   if(isBold)
   {
      font |= 0x08;
   }

   if(UnderLined)
   {
      font |= 0x80;
   }

   if(inLarge)
   {
      font |= 0x20;
		CharPerLine = NormalCharPerLine/2;
   }
   CmdFont[2] = font;
   PrintJob->Write(CmdFont,sizeof(CmdFont));
}

//------------------------------------------------------------------------------

void TConnectPrinter::Reverse(unsigned char Lines)
{
   char CmdBack[3] = {0x1B,0x65,0x00};
   CmdBack[2] = Lines;
   PrintJob->Write(CmdBack,sizeof(CmdBack));
}

//------------------------------------------------------------------------------

void TConnectPrinter::Cut(bool Full)
{
   //char CmdFillCut[2] = {0x1B,0x69};
   char CmdCut[2] = {0x1B,0x6D};

	for(int i = 0 ; i < CutterGap; i++)
	{
	   NewLine();
   }
   if(Full)
   {
      PrintJob->Write(CmdCut,sizeof(CmdCut));
   }
   else
   {
  		PrintJob->Write(CmdCut,sizeof(CmdCut));
   }
}

void TConnectPrinter::CheckKickDraw(TPrintJob *inPrintJob)
{
   if(PrinterType != prnScreen)
   {
      switch(inPrintJob->JobType)
      {
         case pjInit :
         break;
         case pjReceiptInvoice :
         case pjReceiptReceipt :
					switch(inPrintJob->JobReceivedFrom)
               {
                  case devPC :
                     if(DrawClosed)// Only Kick the Draw if it's Closed.
                     {
                        this->KickDraw();
                     }
                  break;
                  case devPalm :
                  break;
               }
         break;
         case pjKitchen :
         break;
      }
   }
}


void TConnectPrinter::KickDraw()
{
   unsigned char KickDraw[5] = {0x1B,0x70,0x00,100,160}; // Check Relay speed.
   unsigned char KickDraw1[5] = {0x1B,0x70,0x01,100,160}; // Check Relay speed.
	PrintJob->Write(KickDraw,sizeof(KickDraw));
	PrintJob->Write(KickDraw1,sizeof(KickDraw1));
}


//------------------------------------------------------------------------------

void TConnectPrinter::DrawLine()
{
   char CmdLine[1] = "_";
   int DashCount;
   if(LargeFont) // Less Dashes...
   {
      DashCount = NormalCharPerLine/2;
      if(isBold)
      {
         DashCount = DashCount - 3;
      }

      for(int i = 0 ; i < DashCount ; i++)
      {
         PrintJob->Write(CmdLine,sizeof(CmdLine));
      }
	}
   else
   {
      DashCount = NormalCharPerLine;
      if(isBold)
      {
         DashCount = DashCount - 3;
      }

      for(int i = 0 ; i < DashCount; i++)
      {
         PrintJob->Write(CmdLine,sizeof(CmdLine));
      }
   }
   NewLine();
}

//------------------------------------------------------------------------------

void TConnectPrinter::NewLine()
{
   char CmdNewLine[2] = {'\r', '\n'};
//   char CmdNewLine[1] = {'\r'};
//   char CmdNewLine[1] = {'\n'};
 	PrintJob->Write(CmdNewLine,sizeof(CmdNewLine));
}

//------------------------------------------------------------------------------

void TConnectPrinter::Open()
{
   if(!JobInProgress)
   {
      JobInProgress = true;
      PrintJob->Clear();
   }
}

//------------------------------------------------------------------------------

void TConnectPrinter::Close()
{
   // Reset the Font.
   Font(false,false,false);
   JobInProgress = false;
   StreamPackets(PrintJob);
	PrintJob->Clear();
}

//------------------------------------------------------------------------------

void TConnectPrinter::Reset()
{
   char CmdReset[2] = {0x1B,0x40};
 	PrintJob->Write(CmdReset,sizeof(CmdReset));
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void TConnectPrinter::ProcessConnection()
{
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// InData is the Data Queryed that was returned if any.
// OutData is the Data written.


//------------------------------------------------------------------------------
// In the Case of a Printer Conenction. OutData holds the Data to be printed.
void TConnectPrinter::StreamPackets(TMemoryStream *OutData)
{
   try
   {
      if(this->TRXType == Windows_Printer)
      {
         // Do Windows Printers.
         int PrinterCount = 0;
         bool PrintedOK = false;
         while(PrinterCount < 7 && PrintedOK == false)
         {
            OutData->Position = 0;
            unsigned char *ptr = (unsigned char *)OutData->Memory;
            PRINTER_INFO_5 pinfo5[3];
            DWORD       dwNeeded,dwReturned;
            DOC_INFO_1  DocInfo;
            DWORD       dwJob;
				DWORD       BytesWritten;
            HANDLE      PrinterHandle;

            DocInfo.pDocName     = "Receipt";
            DocInfo.pOutputFile  = NULL;
            DocInfo.pDatatype    = "RAW";

            PRINTER_DEFAULTS Defaults;
            memset(&Defaults,0,sizeof(Defaults));
            Defaults.DesiredAccess = PRINTER_ACCESS_USE;

            if(OpenPrinter(Name.c_str(),&PrinterHandle,&Defaults))
            {
               if(StartDocPrinter(PrinterHandle,1,(LPBYTE)&DocInfo))
               {
                  if(StartPagePrinter(PrinterHandle))
                  {
                     if(WritePrinter( PrinterHandle,	// handle to printer object
                                      ptr,	// pointer to array that contains printer data
                                      OutData->Size,	// size, in bytes, of array
                                      &BytesWritten)) 	// addr. of variable with count of bytes written
                     {
                        if(EndPagePrinter(PrinterHandle))
								{
                           if(EndDocPrinter(PrinterHandle))
                           {
                              if(ClosePrinter(PrinterHandle))
                              {
											PrintedOK = true;
                              }
                              else
                              {
                                 int ErrorNumber = GetLastError();
											TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Printing ClosePrinter Failed Error : " + IntToStr(ErrorNumber) + " Printer " + Name);
											::Sleep(1000);
                              }
                           }
                           else
                           {
                              int ErrorNumber = GetLastError();
										TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Printing EndDocPrinter Failed Error : " + IntToStr(ErrorNumber) + " Printer " + Name);
										::Sleep(1000);
                           }
                        }
                        else
                        {
									int ErrorNumber = GetLastError();
									TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Printing EndPagePrinter Failed Error : " + IntToStr(ErrorNumber) + " Printer " + Name);
									::Sleep(1000);
                        }
                     }
                     else
                     {
                        int ErrorNumber = GetLastError();
								TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Printing WritePrinter Failed Error : " + IntToStr(ErrorNumber) + " Printer " + Name);
								::Sleep(1000);
                     }
                  }
                  else
                  {
                     int ErrorNumber = GetLastError();
							TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Printing StartPagePrinter Failed Error : " + IntToStr(ErrorNumber) + " Printer " + Name);
							::Sleep(1000);
                  }
               }
               else
               {
                  int ErrorNumber = GetLastError();
						TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Printing StartDocPrinter Failed Error : " + IntToStr(ErrorNumber) + " Printer " + Name);
						::Sleep(1000);
               }
            }
            else
            {
               int ErrorNumber = GetLastError();
					TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Unable to open printer : " + IntToStr(ErrorNumber) + " Printer " + Name);
					::Sleep(1000);
            }
				PrinterCount++;            
   		}
      }
   }
   catch(Exception &Err)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"TConnectPrinter::StreamPackets " + Err.Message);
   }
}



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifdef  IR_Palm
//------------------------------------------------------------------------------
// Return false when it could not connect to a connection, in most cases this is only
// used when first forming a connection list, though the second thread will use it and take the
// correct action if someone removes a module on the fly...
bool TConnect::Connect(bool Reply)
{
	try
   {
		if(DataLinkControl->DataLayerConnect(Reply))
      {
			return true;
      }
      return false;
   }
   catch(EProcessConnectionError &Err)
   {
   	switch(Err.Type)
      {
			case EPC_TimeOut :
         	// Not connected / Could not connect.....
         	return false;
			case EPC_CRCDisconnect :
         break;
         case EPC_Terminate :
         break;
      }
      return false;
   }
}

//------------------------------------------------------------------------------

SlaveType __fastcall TConnect::GetSlaveType()
{
	// PC_TRX_REQ PACKET.
	TComms::Instance().Packet_To_Send->Header.hCommand = 0;           // Get type command... (Get info?)
	TComms::Instance().Packet_To_Send->Header.lCommand = SLAVE_TYPE; 	// Packet Request.
	TComms::Instance().Packet_To_Send->FormatForWrite(NULL); // No Data to write...

	DataLinkControl->WriteData(TComms::Instance().Packet_To_Send->Packet,true);

	// Recive in the reply packet. (Either a SLAVE_NACK_R or a SLAVE_ACK_R)
   TComms::Instance().Data_Inbound->Clear();
	DataLinkControl->ReadData(TComms::Instance().Data_Inbound);

   ProcessInboundData(TComms::Instance().Data_Inbound);
   // Ok thats pulled out the header and the actual data.
// DEBUG
   // Check Slave Packet Header Command for anything other than SLAVE_ACK_R
   if(TComms::Instance().Packet_To_Receive->Size > 0)
   {
      char *Data = (char *)TComms::Instance().Packet_To_Receive->Memory;
      if( *Data == IR_TRX_Station)
         return IR_TRX_Station;
      else
         return Unknown_TRX_Station;
   }
   else
   {
         return Unknown_TRX_Station;
   }
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
unsigned int __fastcall TConnect::GetAddress()
{
	// PC_TRX_REQ PACKET.
	TComms::Instance().Packet_To_Send->Header.hCommand = 0;           // Get type command... (Get info?)
	TComms::Instance().Packet_To_Send->Header.lCommand = SLAVE_ADDRESS; 	// Packet Request.
	TComms::Instance().Packet_To_Send->FormatForWrite(NULL); // No Data to write...

	DataLinkControl->WriteData(TComms::Instance().Packet_To_Send->Packet,true);

	// Recive in the reply packet. (Either a SLAVE_NACK_R or a SLAVE_ACK_R)
   TComms::Instance().Data_Inbound->Clear();
	DataLinkControl->ReadData(TComms::Instance().Data_Inbound);

   ProcessInboundData(TComms::Instance().Data_Inbound);
   // Ok thats pulled out the header and the actual data.
// DEBUG
   // Check Slave Packet Header Command for anything other than SLAVE_ACK_R
   if(TComms::Instance().Packet_To_Receive->Size > 0)
   {
      char *ptr = (char *)TComms::Instance().Packet_To_Receive->Memory;
      char hAddress = ptr[0];
      char lAddress = ptr[1];
      unsigned int CrntAddress = (hAddress * 256) + lAddress;
      return CrntAddress;
	}
   else
   {
      return -1;
   }
}


//------------------------------------------------------------------------------
bool __fastcall TConnect::SetAddress(unsigned short inAddress)
{
	TComms::Instance().Data_Outbound->Clear();
   char hAddress = (char)(inAddress >> 8);
	char lAddress = (char)(inAddress % 256);
	try
   {
      if(DataLinkControl->Address > 0xFFF0) // This Connect is for Global Address Trash any response as it will be Garbage.
      {

         TComms::Instance().Data_Outbound->Write(&hAddress,1);
         TComms::Instance().Data_Outbound->Write(&lAddress,1);

         // PC_TRX_REQ PACKET.
			TComms::Instance().Packet_To_Send->Header.hCommand = 0x80;           // Set type command... (Get info?)
         TComms::Instance().Packet_To_Send->Header.lCommand = SLAVE_ADDRESS; 	// Packet Request.
         // Global Assume Connected.
//			this->DataLinkControl->Connected = true;
         TComms::Instance().Packet_To_Send->FormatForWrite(TComms::Instance().Data_Outbound); // format up address.
   		DataLinkControl->WriteData(TComms::Instance().Packet_To_Send->Packet,false);
         return true;
      }
      else
      {
         TComms::Instance().Data_Outbound->Write(&hAddress,1);
         TComms::Instance().Data_Outbound->Write(&lAddress,1);

         // PC_TRX_REQ PACKET.
         TComms::Instance().Packet_To_Send->Header.hCommand = 0x80;           // Set type command... (Get info?)
         TComms::Instance().Packet_To_Send->Header.lCommand = SLAVE_ADDRESS; 	// Packet Request.
         // Global Assume Connected.
         TComms::Instance().Packet_To_Send->FormatForWrite(TComms::Instance().Data_Outbound); // format up address.

      	DataLinkControl->WriteData(TComms::Instance().Packet_To_Send->Packet,true);

      	// Recive in the reply packet. (Either a SLAVE_NACK_R or a SLAVE_ACK_R)
      	TComms::Instance().Data_Inbound->Clear();

         DataLinkControl->ReadData(TComms::Instance().Data_Inbound);

         ProcessInboundData(TComms::Instance().Data_Inbound);

         //Check for SLAVE_NACK_R (most likly to occur.)
         switch(LastMsgStatus)
         {
            case SLAVE_NACK_R 				: {return false;}
            // I seem to be recviing a NACK after the first packet exchange...?
            case SLAVE_ACK_NO_DATA_R 		: {return false;}
            case SLAVE_PRN_ERROR_R 			: {return false;}
            case SLAVE_INVALID_DATA_R 		: {return false;}
            case SLAVE_INVALID_COMMAND_R 	: {return false;}
            case SLAVE_BUSY_R 				: {return false;}
            case SLAVE_ACK_R					: {return true;}
            default 								: {return false;}
         }
      }
   }
   catch(EProcessConnectionError &Err)
   {
		return true;
	}
}

//------------------------------------------------------------------------------

void TConnect::Load()
{
}

//------------------------------------------------------------------------------
void TConnect::ProcessInboundData(TMemoryStream *Data)
{
	if(Data)
   {
      // Check actual Data arrived here....
      if(Data->Size != 0)
      {
         unsigned char *ptr = (unsigned char *)Data->Memory;
/*         if(ptr[0] & cIsLast)
         { //Retruns the MSB...
            Statbit = 1;
         }
         else
			{
            Statbit = 0;
         }*/
         LastMsgStatus = ptr[0] & 0x7F;
         Data->Position = 1;
         TComms::Instance().Packet_To_Receive->Clear();
         TComms::Instance().Packet_To_Receive->Position = 0;
         TComms::Instance().Packet_To_Receive->CopyFrom(Data,(Data->Size-1));
         TComms::Instance().Packet_To_Receive->Position = 0;// TMemoryStream
      }
      else
      {
         // No Response from Modual.
         if(TComms::Instance().StartLogging)
         {
				TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Read : Failed");
         }
         LastMsgStatus = SLAVE_NO_RESPONSE;
         TComms::Instance().Packet_To_Receive->Clear();
         TComms::Instance().Packet_To_Receive->Position = 0;
      }
   }
}

//---------------------------------------------------------------------------

SLAVE_COMMANDS TConnect::TransferCommsPackets()
{
   SLAVE_COMMANDS ReturnValue;
   static SLAVE_COMMANDS LastLastMesg;
	LastLastMesg = LastMsgStatus;
   LastMsgStatus = SLAVE_ACK_R;//SLAVE_NACK_R;
   TComms::Instance().Data_Inbound->Clear();
   int BusyCount = 0;
   do
   {
      // We have sent our Data Count for the number of packet we are transmitting.
      DataLinkControl->WriteData(TComms::Instance().Packet_To_Send->Packet,true);
      DataLinkControl->ReadData(TComms::Instance().Data_Inbound);
      ProcessInboundData(TComms::Instance().Data_Inbound);
      // Ok thats pulled out the header and the actual data.
      //Check for SLAVE_NACK_R (most likly to occur.)
      ReturnValue = LastMsgStatus;
      if(LastMsgStatus == SLAVE_BUSY_R)
      {
         BusyCount++;
         if(BusyCount > TComms::Instance().MaxBusyCount)
         {
				TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"void TConnect::TransferCommsPackets() Busy Count Expired.");
            throw ESlaveMessageError(Err_SLAVE_NACK_R,1);
         }
      }
   }while(LastMsgStatus == SLAVE_BUSY_R);

   AnsiString Msg;
   switch(LastMsgStatus)
   {
   	case SLAVE_ACK_R:
      Msg = "SLAVE_ACK_R";
      break;
   	case SLAVE_NACK_R:
      if(LastLastMesg != SLAVE_NACK_R)
      {
      	Msg = "SLAVE_NACK_R";
      }
      break;
   	case SLAVE_ACK_NO_DATA_R:
      Msg = "SLAVE_ACK_NO_DATA_R";
      break;
   	case SLAVE_PRN_ERROR_R:
      Msg = "SLAVE_PRN_ERROR_R";
      break;
   	case SLAVE_INVALID_DATA_R:
      Msg = "SLAVE_INVALID_DATA_R";
      break;
   	case SLAVE_INVALID_COMMAND_R:
      Msg = "SLAVE_INVALID_COMMAND_R";
      break;
   	case SLAVE_NO_RESPONSE:
      Msg = "SLAVE_NO_RESPONSE";
      ErrorCount++;
      break;
   	case SLAVE_CORRUPT_DATA:
      Msg = "SLAVE_CORRUPT_DATA";
      break;
      default :
	      Msg = "Unknown Return Value in TransferCommsPackets";
   }
   if(Msg != "")
   {
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,Msg);
   }
   return ReturnValue;
}

//------------------------------------------------------------------------------
//-----------------------------TConnectIR----------------------------------
//------------------------------------------------------------------------------

__fastcall TConnectIR::TConnectIR()
{
   ClassName = "TConnectIR";
}

__fastcall TConnectIR::~TConnectIR()
{
}


void TConnectIR::setGreenLight(bool On)
{
   try
   {
      TComms::Instance().Data_Outbound->Clear();
      fGreenLight = On;
      if(On)
      {
         unsigned char Light = 0x01;
         TComms::Instance().Data_Outbound->Write(&Light,1);
         // PC_TRX_REQ PACKET.
         TComms::Instance().Packet_To_Send->Header.hCommand = 0x80;           // Set type command... (Get info?)
         TComms::Instance().Packet_To_Send->Header.lCommand = GREEN_LIGHT; 	// Packet Request.
         // Global Assume Connected.
         TComms::Instance().Packet_To_Send->FormatForWrite(TComms::Instance().Data_Outbound); // format up address.

         DataLinkControl->WriteData(TComms::Instance().Packet_To_Send->Packet,true);

         // Recive in the reply packet. (Either a SLAVE_NACK_R or a SLAVE_ACK_R)
         TComms::Instance().Data_Inbound->Clear();

         DataLinkControl->ReadData(TComms::Instance().Data_Inbound);
         TComms::Instance().Data_Inbound->Clear();
      }
      else
      {
         unsigned char Light = 0x00;
         TComms::Instance().Data_Outbound->Write(&Light,1);
         // PC_TRX_REQ PACKET.
         TComms::Instance().Packet_To_Send->Header.hCommand = 0x80;           // Set type command... (Get info?)
         TComms::Instance().Packet_To_Send->Header.lCommand = GREEN_LIGHT; 	// Packet Request.
         // Global Assume Connected.
         TComms::Instance().Packet_To_Send->FormatForWrite(TComms::Instance().Data_Outbound); // format up address.

         DataLinkControl->WriteData(TComms::Instance().Packet_To_Send->Packet,true);

         // Recive in the reply packet. (Either a SLAVE_NACK_R or a SLAVE_ACK_R)
         TComms::Instance().Data_Inbound->Clear();

         DataLinkControl->ReadData(TComms::Instance().Data_Inbound);
         TComms::Instance().Data_Inbound->Clear();
      }
   }
	catch(EDataLinkError &Err) // Error Between Server and Slave.
   {
      TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"EDataLinkError in TConnectIR::setGreenLight");
	}
	catch(ESlaveMessageError &Err) // Error Between Server and Slave.
   {
      TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"ESlaveMessageError in TConnectIR::setGreenLight");
	}
   catch(EProcessConnectionError &Err) // Time Out Type Serial Error
   {
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"EProcessConnectionError in TConnectIR::setGreenLight");
   }
}

//------------------------------------------------------------------------------

bool TConnectIR::Disconnect()
{
	try
   {
		if(DataLinkControl->DataLayerDisconnect())
      {
			return true;
      }
   }
   catch(EProcessConnectionError &Err)
   {
   	switch(Err.Type)
      {
			case EPC_TimeOut :
         	// Not connected / Could not connect.....
         	return false;
         case EPC_CRCDisconnect :
         break;
         case EPC_Terminate :
         break;
      }
   }
   return false;
}

//------------------------------------------------------------------------------

void TConnectIR::ProcessConnection()
{
   unsigned int TimeStop,TimeStart;
   TimeStart = GetTickCount();
   if(Transmission()) // Do the initial transmission before starting timer.
   {
      TimeStop = TimeStart + 3000;
      bool StillTalking = true;
      do
      {
         TimeStart = GetTickCount();
         StillTalking = Transmission();
         if(StillTalking)
         {
            TimeStop = GetTickCount() + 1000;
         }
         else
         {
				TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"----------------- Nothing More to Transmit --------------------");
         }
      }while((TimeStart <= TimeStop) && (StillTalking));
      ErrorCount = 0;
   }
   else // remove this else to reset to old light version.
   {
      GreenLight = false;
      if(ErrorCount > 5)
      {
         DownTime = GetTickCount() + ERRORDOWNTIME;
         ErrorCount = 0;
         TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Module Timed Out - Error Count Max Reached.");
      }
   }
//   GreenLight = false;
}

bool TConnectIR::Transmission()
{
   bool Confirmed = true;
	TPalm *PalmB = NULL;
	TPalm *PalmE = NULL;
   TComms::Instance().Process_Inbound->Clear();
   Offline = false; // Set to true if communcations fails.
   // The number of times to ping a module that is supposed to have a Palm on the end of it.
	try
   {
      if(BeginTransmission(PalmB)) // Retruns true if transmission is OK.
      {
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"----------------- Transmission Accecpted ","Palm #" + IntToStr(PalmB->ID.DeviceKey));
         //GreenLight = true; // Un Rem this to reset to old light version
         if(PalmB->ID.DeviceKey == PalmNotFoundID)
         {
            PalmB->Status.Error = Err_DeviceID_Not_Found;
            PalmB->BuildPacket(NULL,KEEPALIVE);
				TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Palm ID Not Found Ending Transmission","Palm #"+ IntToStr(PalmB->ID.DeviceKey));
            if(EndTransmission(PalmE)) // Returns true if palm recived message OK.
            {
               Confirmed = true;
            }
            else
            {
               Confirmed = false;
            }
         }
         else
         {
            unsigned char *ptrStream;
            ptrStream = (unsigned char *)TComms::Instance().Process_Inbound->Memory;
            TComms::Instance().Sync->ProcessTransmission(ptrStream,PalmB);
            TComms::Instance().Process_Inbound->Clear();
            if(EndTransmission(PalmE))
            {
               if(PalmB == PalmE)
               {
                  if(PalmB->LastErrorNumber == proA_Ok)
                  {
                     switch(PalmB->LastTransmissionType) // Preform Palm Data Clean up depending on
                     {                        // Transmission type and weather it was susscful.
                        case KEEPALIVE :
                        break;
                        case ORDERS :
                        break;
                        case MENU_SYNC :
                              PalmB->resetMenuEnableState(); // Palm retrieved Data ok update this end.
                        break;
                        case DRINK_SYNC :
                              PalmB->resetMenuEnableState(); // Palm retrieved Data ok update this end.
                        break;
                        case LOGIN :
                              PalmB->resetMenuEnableState(); // Palm retrieved Data ok update this end.
                        break;
                        case INFO_SYNC :
                        break;
                        case REQUEST_RECEIPT :
                        break;
                        case RETRIEVE_ORDERS :
                              PalmB->setMenuEnableState(); // Palm retrieved Data ok update this end.
                              PalmB->ClearSeatNames();
                        break;
                        case REGISTRATION :
                           TDeviceRealTerminal::Instance().Registering = false;
                           TDeviceRealTerminal::Instance().Reregistering = false;
                        break;
                     }
                  }
                  Confirmed = true;
               }
               else
               {
						TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"----------------- Palm ID's Differ over duration of transaction","Palm #"+ IntToStr(PalmE->ID.DeviceKey));
                  Confirmed = false;
               }
            }
            else
            {
					TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"----------------- End Transmission Failed Beem Break","Palm #"+ IntToStr(PalmB->ID.DeviceKey));
               Confirmed = false;
            }
         }
      }
		else
      {
         Confirmed = false;
      }

      if(PalmB != NULL)
      {
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"----------------- Transmission Completed--------------------","Palm #"+ IntToStr(PalmB->ID.DeviceKey));
         PalmB->Outbound->Clear();
      }

      if(PalmE != NULL)
      {
         PalmE->Outbound->Clear();
      }
   }
	catch(ESlaveMessageError &Err) // Error Between Server and Slave.
   {
      if(PalmB != NULL)
      {
         PalmB->Outbound->Clear();
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Palm Outbound Que Cleared due to ESlaveMessageError","Palm #"+ IntToStr(PalmB->ID.DeviceKey));
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Slave Message Error type :" + IntToStr(Err.ErrNum),"Palm #"+ IntToStr(PalmB->ID.DeviceKey));
      }
      if(PalmE != NULL)
      {
         PalmE->Outbound->Clear();
      }
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Slave Message Error type :" + IntToStr(Err.ErrNum));
      Confirmed = false;
      Offline = true;
      //DEBUG TEMPERORY CODE
      TComms::Instance().StartLogging = false;
// See Peter (26/4/01)     ErrorCount++;
	}
   catch(EProcessConnectionError &Err) // Time Out Type Serial Error
   {
      if(PalmB != NULL)
      {
         PalmB->Outbound->Clear();
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Palm Outbound Que Cleared due to EProcessConnectionError","Palm #"+ IntToStr(PalmB->ID.DeviceKey));
      }
      if(PalmE != NULL)
      {
         PalmE->Outbound->Clear();
      }
      Confirmed = false;
      Offline = true;
      //DEBUG TEMPERORY CODE
      TComms::Instance().StartLogging = false;
     	DataLinkControl->Connected = false;
		ErrorCount++;
   }
	catch(Exception &Err)
   {
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"An Error occured in TConnectIR::Transmission" + Err.Message);
		Confirmed = false;
		Offline = true;
		if(PalmB != NULL)
		{
			PalmB->Outbound->Clear();
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Palm Outbound Que Cleared due to Exception","Palm #"+ IntToStr(PalmB->ID.DeviceKey));
      }
      if(PalmE != NULL)
      {
         PalmE->Outbound->Clear();
      }
      throw;
   }
   catch(...)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"An Error occured in TConnectIR::Transmission");
      Confirmed = false;
      Offline = true;
      if(PalmB != NULL)
      {
         PalmB->Outbound->Clear();
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Palm Outbound Que Cleared due to Other Error","Palm #"+ IntToStr(PalmB->ID.DeviceKey));
      }
      if(PalmE != NULL)
      {
         PalmE->Outbound->Clear();
      }
      throw;
   }
   return Confirmed;
}


bool TConnectIR::BeginTransmission(TPalm *&Palm)
{
   // Confrim weather we sent data to the Palm ( Always false in Begin Transmission.)
   bool Confirmed = false;
   // Query this connection... Any thing to say?
   // Palm -----> Request (Always Respond with a NULL. so Confimed will be false)
   // stream untill application message received.
   Palm = StreamPackets(TComms::Instance().Process_Inbound,Confirmed);
   // Bail out if no palm first time in....
   // Base retrun vaule on wether we linked to a palm or not.
   if(Palm == NULL)
   {
      Confirmed = false;
   }
   else
   {
      Palm->Status.Error = proA_Ok;
      Confirmed = true;
   }

   if((Palm != NULL) && (TComms::Instance().Process_Inbound->Size == 0))
   {
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Skipping Processing Due to TComms::Instance().Process_Inbound->Size == 0.");
      Confirmed = false;
   }
   return Confirmed;
}


bool TConnectIR::EndTransmission(TPalm *&Palm)
{
   // Confirm whether we sent data to the Palm.
   bool Confirmed = false;
   // retrieve a Null, Send Data. or Time out....
   unsigned int TimeStop,TimeStart;
   TimeStart = GetTickCount();
   TimeStop = TimeStart + 1000;
   Palm = NULL;
   do
   {
      TimeStart = GetTickCount();
      Palm = StreamPackets(TComms::Instance().Process_Inbound,Confirmed);
      // Keep going palm is waiting and we are still processing.
      if((Palm != NULL) && (Palm->ID.DeviceKey != PalmNotFoundID))
      {
         TimeStop = TimeStart + 1000;
      }
   }while((TimeStart <= TimeStop) && (Confirmed == false));
   return Confirmed;
}
//---------------------------------------------------------------------------

// Any out bound data is stored in the TComms::Instance().TransmissionList for the palm.
TPalm * TConnectIR::StreamPackets(TMemoryStream *inData,bool &Confirmed)
{
   TPalm *Palm;
	// Was a packet sent (if so confirm palm when sent).
	bool MessageSent = false;
	// PC_TRX_REQ PACKET.
   TComms::Instance().Packet_To_Send->Clear();
	TComms::Instance().Packet_To_Send->Header.hCommand = 0; // Get type command... (Get info?)
	TComms::Instance().Packet_To_Send->Header.lCommand = PC_TRX_REQ; // Packet Request.
	TComms::Instance().Packet_To_Send->FormatForWrite(NULL); // No Data to write...
   SLAVE_COMMANDS Response = TransferCommsPackets();
   if(Response != SLAVE_NACK_R &&  Response != SLAVE_NO_RESPONSE)
   {
      //DEBUG TEMPERORY CODE
      TComms::Instance().StartLogging = true;

      // retrieve Palm ID.
      unsigned char DeviceKey = PalmNotFoundID;
      TComms::Instance().Packet_To_Receive->Read(&DeviceKey, sizeof(DeviceKey));
      // retrieve Palm Data Count.
      unsigned char lPktSize = 0, hPktSize = 0;
      TComms::Instance().Packet_To_Receive->Read(&hPktSize, sizeof(hPktSize));
      TComms::Instance().Packet_To_Receive->Read(&lPktSize, sizeof(lPktSize));
      unsigned int InboundNumberOfPackets = ( (hPktSize << 8) + lPktSize);

      //DEBUG TEMPERORY CODE
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Packet Received from Palm #" + IntToStr(DeviceKey),"Palm #"+ IntToStr(DeviceKey));
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Number of Inbound Packets : " + IntToStr(InboundNumberOfPackets),"Palm #"+ IntToStr(DeviceKey));
      // retrieve the Palm if it exists.
      Palm = TDeviceRealTerminal::Instance().FindTPalm(DeviceKey);
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"----------------- Transmission Requested --------------------","Palm #"+ IntToStr(Palm->ID.DeviceKey));

      unsigned char outbound_lPktSize = 0, outbound_hPktSize = 0;
      unsigned short int OutBoundNumberOfPackets = 0;
      // Counts outgoing bytes for copying out of palm stream.
      unsigned int OutByteCount = 0;
      TMemoryStream *TempStream = NULL;
		 TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Palm Outbound Queue Size " + IntToStr(Palm->Outbound->Size),"Palm #"+ IntToStr(Palm->ID.DeviceKey));
      if(Palm->Outbound->Size != 0)
      {
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"MESSAGE Ready for palm pickup","Palm #"+ IntToStr(Palm->ID.DeviceKey));
         // Split and Pad outbound data into buffer Size quantities. TStream
         TempStream = Palm->Outbound;
         OutBoundNumberOfPackets = (unsigned short int)(TempStream->Size / DataLinkControl->Max_Buffer_Size);
         if(TempStream->Size % DataLinkControl->Max_Buffer_Size)
         {
            OutBoundNumberOfPackets++;
         }
         OutByteCount = TempStream->Size;
         outbound_hPktSize = (unsigned char)(OutBoundNumberOfPackets >> 8);
         outbound_lPktSize = (unsigned char)(OutBoundNumberOfPackets % 256);
         TempStream->Position = 0;
         MessageSent = true;
      }
      else
      {
         // Build a NULL packet so palm can check Sync number...
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"NOTHING to Send to Palm Padding a NULL : ","Palm #"+ IntToStr(Palm->ID.DeviceKey));

         Palm->BuildPacket(NULL,KEEPALIVE);
         // Split and Pad outbound data into buffer Size quantities. TStream
         TempStream = Palm->Outbound;
         OutBoundNumberOfPackets = (unsigned short int)(TempStream->Size / DataLinkControl->Max_Buffer_Size);
         if(TempStream->Size % DataLinkControl->Max_Buffer_Size)
         {
            OutBoundNumberOfPackets++;
         }
         OutByteCount = TempStream->Size;
         outbound_hPktSize = (unsigned char)(OutBoundNumberOfPackets >> 8);
         outbound_lPktSize = (unsigned char)(OutBoundNumberOfPackets % 256);
         TempStream->Position = 0;
         MessageSent = false; // This is a fuller packet the real on hasnt been sent yet.
      }

      //DEBUG TEMPERORY CODE
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Number of Out bound Packets : " + IntToStr(OutBoundNumberOfPackets),"Palm #"+ IntToStr(Palm->ID.DeviceKey));
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"--- Transmitting Packet Sizes ---","Palm #"+ IntToStr(Palm->ID.DeviceKey));
      // If we Do then insert the number of packets in the PC_TRX_ACK packet...
      // PC_TRX_ACK_PACKET.
      TComms::Instance().Packet_To_Send->Clear();
      TComms::Instance().Packet_To_Send->Header.hCommand = 0; // will always be the case...
      TComms::Instance().Packet_To_Send->Header.lCommand = PC_TRX_ACK;

      // Not sending Data yet just the number of packets.
      TComms::Instance().Data_Outbound->Clear();
      TComms::Instance().Data_Outbound->Write(&outbound_hPktSize, sizeof(outbound_hPktSize));
      TComms::Instance().Data_Outbound->Write(&outbound_lPktSize, sizeof(outbound_lPktSize));

      TComms::Instance().Packet_To_Send->FormatForWrite(TComms::Instance().Data_Outbound);

      Response = TransferCommsPackets();
      if(Response == SLAVE_NACK_R)
      {
         	Disconnect();
				throw ESlaveMessageError(Err_SLAVE_NACK_R,dPC_TRX_DATA_Err_SLAVE_NACK_R);
      }
      // Ok we have X number of Packets to send and X number to recive
      //  So enter a double for loop.

      int count = 0;
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"--- Transmitting Packets ---","Palm #"+ IntToStr(Palm->ID.DeviceKey));
      while((OutBoundNumberOfPackets != 0) || (InboundNumberOfPackets != 0))
      {

         //DEBUG TEMPERORY CODE
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Number of Out bound Packets Left to go : " + IntToStr(OutBoundNumberOfPackets),"Palm #"+ IntToStr(Palm->ID.DeviceKey));
			TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Number of inbound Packets left to go : " + IntToStr(InboundNumberOfPackets),"Palm #"+ IntToStr(Palm->ID.DeviceKey));

         count++;
         if(OutBoundNumberOfPackets != 0)
         {
            TComms::Instance().Packet_To_Send->Clear();
            TComms::Instance().Packet_To_Send->Header.hCommand = 0; // will always be the case...
            TComms::Instance().Packet_To_Send->Header.lCommand = PC_TRX_DATA;

            // Pack the first Packet.
            TComms::Instance().Data_Outbound->Clear();
            if(TempStream->Size < DataLinkControl->Max_Buffer_Size)
            {
               TempStream->Position = 0;
               TComms::Instance().Data_Outbound->CopyFrom(TempStream,TempStream->Size);
            }
            else
            {

               if(OutByteCount > DataLinkControl->Max_Buffer_Size)
               {// There is more bytes than a buffer full so copy max bytes... and dec byte count.
                  TComms::Instance().Data_Outbound->CopyFrom(TempStream,DataLinkControl->Max_Buffer_Size);
                  OutByteCount -= DataLinkControl->Max_Buffer_Size;
               }
               else
               {	// We are nearing the end of the stream so only copy the number of bytes in the stream that are left.
                  TComms::Instance().Data_Outbound->CopyFrom(TempStream,OutByteCount);
                  // The rest of the stream will be packed.
               }
            }
            // Mouve the source Stream along By X numebr of Bytes...
   //          TComms::Instance().CrntPalm->Stream->Position += DataLinkControl->Max_Buffer_Size;

            // Pad with spaces...
            if(TComms::Instance().Data_Outbound->Size < DataLinkControl->Max_Buffer_Size)
            {
               int Gap = DataLinkControl->Max_Buffer_Size - TComms::Instance().Data_Outbound->Size;
               int space = 0x20;
               while(Gap)
               {
                  TComms::Instance().Data_Outbound->Write(&space,1);
                  Gap--;
               }
            }
         }
         else
         {
            TComms::Instance().Packet_To_Send->Header.hCommand = 0; // will always be the case...
            TComms::Instance().Packet_To_Send->Header.lCommand = PC_TRX_NO_DATA;
         // Pack the first Packet.
            TComms::Instance().Data_Outbound->Clear();
         }


   // DEBUG
         if(TComms::Instance().Data_Outbound->Size > 32)
         {
            throw ESlaveMessageError(Err_SLAVE_PRN_ERROR_R,1);
         }

         TComms::Instance().Packet_To_Send->FormatForWrite(TComms::Instance().Data_Outbound);

         if(TComms::Instance().Packet_To_Send->Packet->Size > 34)
         {
            throw ESlaveMessageError(Err_SLAVE_PRN_ERROR_R,2);
         }

         Response = TransferCommsPackets();
         if(Response == SLAVE_NACK_R)
         {
         	Disconnect();
				throw ESlaveMessageError(Err_SLAVE_NACK_R,dPC_TRX_DATA_Err_SLAVE_NACK_R);
         }

         // Packet has gone/arrived :)
         if(OutBoundNumberOfPackets > 0)
         {
            OutBoundNumberOfPackets--;
         }
         // Add to Application Data Stream.

         inData->CopyFrom(TComms::Instance().Packet_To_Receive,TComms::Instance().Packet_To_Receive->Size);
         if(InboundNumberOfPackets > 0)
         {
            InboundNumberOfPackets--;
         }
      } // End while
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"--- Transmission Complete Disconnecting ---","Palm #"+ IntToStr(Palm->ID.DeviceKey));
      // All done...Send Dieconnect.
      TComms::Instance().Packet_To_Send->Clear();
      TComms::Instance().Packet_To_Send->Header.hCommand = 0; // will always be the case...
      TComms::Instance().Packet_To_Send->Header.lCommand = PC_DISC_REQ;
      TComms::Instance().Packet_To_Send->FormatForWrite(NULL); // No Data to write...

      TransferCommsPackets();

      if(MessageSent == true)
      {
         Confirmed = true;
      }
		TManagerLogs::Instance().Add(__FUNC__,PALMPACKETLAYERLOG,"Palm Outbound Que Cleared.","Palm #"+ IntToStr(Palm->ID.DeviceKey));
      Palm->Outbound->Clear();

      //DEBUG TEMPERORY CODE
      TComms::Instance().StartLogging = false;
      return Palm;
   }
   else
   {
      return NULL;
   }
}


//---------------------------------------------------------------------------

//------------------------------------------------------------------------------

#endif
