//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

#include "DataLink.h"
#include "comms.h"
#include "MMRegistry.h"
//------------------------------------------------------------------------------

TDataLinkControl::TDataLinkControl(TConnect *inParent)
{
   SendPacket = new TPacket();
   RecivePacket = new TPacket();
   Parent = inParent;
   Address = 0x00;
	Connected = false;
   StartFlag = 0x7E;
	SequenceNumber = 0;
   RetryCount = 15;
   TimeOutms = 25;
   TimeOutBetweenms = 20;

   RegistryRead(MMBaseKey, "SerialRetryCount", RetryCount);
   RegistryRead(MMBaseKey, "SerialTimeOutms", TimeOutms);
   RegistryRead(MMBaseKey, "SerialTimeOutBetweenms", TimeOutBetweenms);

//	Port = Assigned Port;
}

__fastcall TDataLinkControl::TDataLinkControl()
{
   SendPacket = new TPacket();
   RecivePacket = new TPacket();
   Parent = NULL;
   Address = 0x00;
	Connected = false;
   StartFlag = 0x7E;
   RetryCount = 15;
   TimeOutms = 25;
   TimeOutBetweenms = 20;
   RegistryRead(MMBaseKey, "SerialRetryCount", RetryCount);
   RegistryRead(MMBaseKey, "SerialTimeOutms", TimeOutms);
   RegistryRead(MMBaseKey, "SerialTimeOutBetweenms", TimeOutBetweenms);

//	Port = Assigned Port;
}


void TDataLinkControl::setAddress(unsigned short int inAddress)
{
	FAddress = inAddress;
   SendPacket->Header.lAddress = LOBYTE(inAddress);
   SendPacket->Header.hAddress = HIBYTE(inAddress);
}

void TDataLinkControl::setStartFlag(unsigned char inStartFlag)
{
   SendPacket->Header.StartFlag = inStartFlag;
}

//---------------------------------------------------------------------------

__fastcall TDataLinkControl::~TDataLinkControl()
{
	delete (TPacket *) SendPacket;
   SendPacket = NULL;
	delete (TPacket *) RecivePacket;
   RecivePacket = NULL;
   RegistryWrite(MMBaseKey, "SerialRetryCount", RetryCount);
   RegistryWrite(MMBaseKey, "SerialTimeOutms", TimeOutms);
   RegistryWrite(MMBaseKey, "SerialTimeOutBetweenms", TimeOutBetweenms);
}

//---------------------------------------------------------------------------
bool TDataLinkControl::ReadData(TMemoryStream *inData)
{
	int TimeOutCount = 0;
   int CRCRetryCount = 0;

   bool TimedOut = false;
   bool CRCFailed = false;
   bool RetVal = false;
   if(Port)
   {
      if(!Connected)
      {
         DataLayerConnect();
         // Do the rest....Format up packet.
      }
      bool Completed = false;
      do
      {
         TimedOut = false;
         CRCFailed = false;
         RetVal = false;

         // Empty the Program layer buffer.
         inData->Clear();
         // Empty the Recivepacket layer buffer.
         this->RecivePacket->Clear();

         unsigned char *ptrBuffer;
         ptrBuffer = (unsigned char *) &RecivePacket->Header;
         // retrieve and strip header.
         if(ReceiveData(ptrBuffer,sizeof(RecivePacket->Header)))
         {
            if(ValidHeader(DL_SINGLE))             // Header valid?
            {
               // Header Valid copy into recive buffer.
               this->RecivePacket->PazedBuffer->Write(&RecivePacket->Header,sizeof(RecivePacket->Header));
               // retrieve Datablock Size.
               if(ReceiveData(&RecivePacket->DataCount,1))
               {
                  // copy  datasize into recive buffer.
                  this->RecivePacket->PazedBuffer->Write(&RecivePacket->DataCount,sizeof(RecivePacket->DataCount));

                  // Create a memory space for App DataBlock.
                  unsigned char *ptrAppData = new unsigned char[RecivePacket->DataCount];
                  // retrieve Datablock.
                  if(ReceiveData(ptrAppData,RecivePacket->DataCount))
                  {
                     // copy data into recive buffer.
                     this->RecivePacket->PazedBuffer->Write(ptrAppData,RecivePacket->DataCount);

                     // Also copy the data into the Program layer structure.
                     inData->Write(ptrAppData,RecivePacket->DataCount);
                     delete []ptrAppData;

                     unsigned char tempCRC[2];
                     // retrieve CRC.
                     if(ReceiveData(tempCRC,2))
                     {
                        int inCRC = ((tempCRC[0] *256) + tempCRC[1]);

                        // Calculate the new CRC
                        unsigned char *ptrPacket = (unsigned char *)this->RecivePacket->PazedBuffer->Memory;
                        unsigned short int CRC = 0;
                        CRC =  StartCRC(ptrPacket[0]);
                        for(int i = 1; i < this->RecivePacket->PazedBuffer->Size;i++) //Dont include CRC Section.
                        {
                           CRC = UpdateCRC(CRC,ptrPacket[i]);
                        }

                        // Check CRC.
                        if(CRC != inCRC)
                        {
                           CRCFailed = true;
                        }
                        else
                        {
                           // Ok we received a valid packet toggle our Sequence number.
                           if(this->SequenceNumber == 0)
                           {
                              this->SequenceNumber = 1;
                           }
                           else
                           {
                              this->SequenceNumber = 0;
                           }
                        }
                        this->RecivePacket->PazedBuffer->Write(tempCRC,sizeof(tempCRC));
                     }
                     else
                     {
                        TimedOut = true;
                     }
                  }
                  else
                  {
                     delete []ptrAppData;
                     TimedOut = true;
                  }
               }
               else
               {
                  TimedOut = true;
               }
            }
            else
            {
               CRCFailed = true;
            }
         }
         else
         {
            TimedOut = true;
         }

         if(CRCFailed == true)
         {
            if(CRCRetryCount > 1)
            {
               // empty any inbound junk.
               Connected = false;
               if(Parent != NULL)
               {
                  Parent->LastMsgStatus = SLAVE_CORRUPT_DATA;
               }
               Port->Clear();
               Completed = true;
               RetVal = false;
            }
            else
            { // Resend Old Databuffer.
               // empty any inbound junk.
               Port->Clear();
               SendData((unsigned char *)SendPacket->PazedBuffer->Memory, SendPacket->PazedBuffer->Size);
               CRCRetryCount++;
            }
         }
         else if(TimedOut == true)
         {
            if(TimeOutCount > RetryCount)
            {
               // empty any inbound junk.
               Connected = false;
               if(Parent != NULL)
               {
                  Parent->LastMsgStatus = SLAVE_NO_RESPONSE;
               }
               Port->Clear();
               Completed = true;
               RetVal = false;
            }
            else
            { // Resend Old Databuffer.
               // empty any inbound junk.
               Port->Clear();
               SendData((unsigned char *)SendPacket->PazedBuffer->Memory, SendPacket->PazedBuffer->Size);
               TimeOutCount++;
            }
         }
         else
         {
            Completed = true;
            RetVal = true;
         }
      }while(Completed == false);
   }
   #if (DEBUGLEVEL & debugExpandSerialComms )
      if(TComms::Instance().StartLogging)
      {
         if(TimedOut)
         {
            TManagerLogs::Instance().Add(__FUNC__,MSGLOG,"Read : Timedout");
         }
         else
         {
            char out[10];
            char *SentData = (char *)(this->RecivePacket->PazedBuffer->Memory);
            AnsiString TextOut,Fo0,Fo1;
            char arg;
            for (int i = 0; i < this->RecivePacket->PazedBuffer->Size; i++)
            {
               for(int j=0;j<10;j++)
               {
                  out[j] = NULL;
               }
               arg = (char)(*SentData++);
               sprintf(out,"%02X ",arg);
               Fo0 = out;
               Fo1 += Fo0[Fo0.Length()-2];
               Fo1 += Fo0[Fo0.Length()-1];
               Fo1 += Fo0[Fo0.Length()];
               TextOut += Fo1;//Format(Formatter, arg,1);
               Fo1 = "";
            }
            TManagerLogs::Instance().Add(__FUNC__,MSGLOG,"READ : " + TextOut);
         }
      }
   #endif
   return RetVal;
}

//---------------------------------------------------------------------------

bool TDataLinkControl::WriteData(TMemoryStream *inData, bool Reply)
{
   if(Port)
   {
      if(!Connected)
      {
         DataLayerConnect(Reply);
         if(!Connected)
         {	// DIsconnect as we could not reconnect.
            throw EDataLinkError(EDL_Fatal,dDL_WriteData_ConnectFailed);
         }
      }

      // empty any inbound junk.
      Port->Clear();

      try
      {
         this->SendPacket->Header.Slave = 0;
         if(Reply)
            this->SendPacket->Header.Reply = 1;
         else
            this->SendPacket->Header.Reply = 0;
         this->SendPacket->Header.Sequence = this->SequenceNumber;
         this->SendPacket->Header.DataLinkControl = DL_SINGLE;

         this->SendPacket->FormatForWrite(inData);

         if(SendData((unsigned char *)SendPacket->PazedBuffer->Memory, SendPacket->PazedBuffer->Size))
         {
			   #if (DEBUGLEVEL & debugExpandSerialComms)
               if(TComms::Instance().StartLogging)
               {
                  char *SentData = (char *)(SendPacket->PazedBuffer->Memory);
                  char out[10];
                  AnsiString LogText;
                  AnsiString Fo0,Fo1;
                  char arg;
                  for (int i = 0; i < SendPacket->PazedBuffer->Size; i++)
                  {
                     for(int j=0;j<10;j++)
                     {
                        out[j] = NULL;
                     }
                     arg = (char)(*SentData++);
                     sprintf(out,"%02X",arg);
                     Fo0 = out;
                     Fo1 += Fo0[Fo0.Length()-1];
                     Fo1 += Fo0[Fo0.Length()];
                     Fo1 += " ";
                     LogText += Fo1;//Format(Formatter, arg,1);
                     Fo1 = "";
                  }
                  TManagerLogs::Instance().Add(__FUNC__,MSGLOG,"WRITE : " + LogText);
               }
            #endif
            return true;
         }
         else
         {
            // empty any inbound junk.
            if(TComms::Instance().StartLogging)
            {
               TManagerLogs::Instance().Add(__FUNC__,MSGLOG,"Write : Failed");
            }

            Port->Clear();
            throw EDataLinkError(EDL_Fatal,dDL_WriteData_SendData);
         }
      }
      catch(EDataLinkError &Err)
      {
         // empty any inbound junk.
         Port->Clear();
         throw EProcessConnectionError(EPC_Terminate,dDL_WriteData_WriteFailed);
      }
   }
}

//---------------------------------------------------------------------------

bool TDataLinkControl::ReceiveData(unsigned char *ptrBuffer, int BytesToRead)
{
// DEBUG Needs REVISING Time out Handling.
#define Max_Bytes 50000
   unsigned char EscapeChar = 0x7D;
   unsigned long CurrentTime, TimeOut;
   if(Port == NULL || BytesToRead > Max_Bytes)
   {
      return false;
   }

//   unsigned char *TempBuffer = new unsigned char[BytesToRead];
	unsigned char TempBuffer[Max_Bytes];
   ZeroMemory(TempBuffer,Max_Bytes);
   try
   {
      int BytesStillRequired = BytesToRead;
      CurrentTime = GetTickCount();
      TimeOut = CurrentTime + TimeOutms;
      while(BytesStillRequired != 0)
      {
         int unParsedBytesRead = Port->Read(TempBuffer,BytesStillRequired);
         // Loop though all unparsed bytes
         for(int i = 0 ; i < unParsedBytesRead ; i++)
         {
            // read in each byte.
            unsigned char Data = TempBuffer[i];
            if(Data == EscapeChar) // If Escape Character.
            {
               // Are we right on the end of our temp holding array.
               // I.e is this the very last byte of the set.
               if(TempBuffer[i] == TempBuffer[unParsedBytesRead-1])
               {
                  // write over the escape character on the end of our array.
                  int GetNextByte = Port->Read(&TempBuffer[i],1);
                  if(GetNextByte == 1)
                  {
                     Data = TempBuffer[i];
                     Data ^= 0x20;
                  }
                  else
                  {
                     return false;
                  }
               }// we sucked the ecp char into the middle of our array and thus need to get in an extra byte.
               else
               {
                  Data = TempBuffer[++i];
                  Data ^= 0x20;
                  BytesStillRequired++;
               }
            }
            *ptrBuffer++ = Data;
         }
         if(unParsedBytesRead != -1)
         {
            BytesStillRequired -= unParsedBytesRead;
            CurrentTime = GetTickCount();
            if(unParsedBytesRead != 0)
            {
               TimeOut = CurrentTime + TimeOutBetweenms;
            }
            unParsedBytesRead = 0;
            if(CurrentTime > TimeOut)
            {
               return false;
            }
         }
         else // No valid port handle -1 returned by Port Read..
         {
            #if (DEBUGLEVEL & debugExpandLowLevelSerialComms )
               TManagerLogs::Instance().Add(__FUNC__,LOWLEVELSERIAL,"DataLink ReceiveData,Failed Invalid Commport handle. Timed Out");
            #endif
            return false;
         }
      }
   }
   __finally
   {
//      delete[] TempBuffer;
   }
   return true;
}

//---------------------------------------------------------------------------

bool TDataLinkControl::SendData(unsigned char *ptrBuffer,int BytesToWrite)
{
   if(!Port->Write(ptrBuffer,BytesToWrite))
   {
      return false;
   }
   else
   {
		return true;
   }
}

//---------------------------------------------------------------------------

bool TDataLinkControl::DataLayerConnect(bool Reply)
{
   // Number of attempts to transmit a message.
   // Maximum number of attempts is Two.
	int TimeOutCount = 0;
   Connected = false;
   // empty any inbound junk.
   if(Port)
   {
      Port->Clear();
      bool Completed = false;
      bool TimedOut = false;
      EDLType ErrorLevel = EDL_Ok;
      do
      {
         SequenceNumber = 0;
         this->SendPacket->Header.Slave = 0;
         this->SendPacket->Header.Reply = Reply;
         this->SendPacket->Header.Sequence = SequenceNumber;
         this->SendPacket->Header.DataLinkControl = DL_CONNECT;

         TMemoryStream *Data;
         Data = new TMemoryStream;
         try
         {
            char Temp = EscapeCharaterDisableFlag;
            Data->Write(&Temp,1);
            this->SendPacket->FormatForWrite(Data);
         }
         __finally
			{
         	delete Data;
         	Data = NULL;
         }

         if(Port->Write((unsigned char *)SendPacket->PazedBuffer->Memory, SendPacket->PazedBuffer->Size))
         {
            if(Reply)
            {
               // retrieve Acknowledge.
               RecivePacket->Clear();
               unsigned char *ptrBuffer;
               ptrBuffer = (unsigned char *) &RecivePacket->Header;
               // retrieve and strip header.
               if(!ReceiveData(ptrBuffer,sizeof(RecivePacket)))
               {
                  #if (DEBUGLEVEL & debugExpandLowLevelSerialComms )
                     TManagerLogs::Instance().Add(__FUNC__,LOWLEVELSERIAL,"DataLink Connect, Retrieve Header Failed. Timed Out");
                  #endif
                  ErrorLevel = EDL_TimeOut;
               }

               if(ErrorLevel == EDL_Ok && !ValidHeader(DL_CONNECT) ) // Header valid?
               {
                  #if (DEBUGLEVEL & debugExpandLowLevelSerialComms )
                     TManagerLogs::Instance().Add(__FUNC__,LOWLEVELSERIAL,"DataLink Connect, Header Not Valid");
                  #endif
                  ErrorLevel = EDL_CRC;
               }

               if(ErrorLevel == EDL_Ok)
               {
                  // Header Valid copy into recive buffer.
                  this->RecivePacket->PazedBuffer->Write(&RecivePacket->Header,sizeof(RecivePacket->Header));
               }

               // retrieve Datablock Size.
               if(ErrorLevel == EDL_Ok && !ReceiveData(&RecivePacket->DataCount,1))
               {
                  #if (DEBUGLEVEL & debugExpandLowLevelSerialComms )
                     TManagerLogs::Instance().Add(__FUNC__,LOWLEVELSERIAL,"DataLink Connect, Retriving Data Count. Timed out");
                  #endif
                  ErrorLevel = EDL_TimeOut;
               }

               if(ErrorLevel == EDL_Ok )
               {
                  // copy  datasize into recive buffer.
                  this->RecivePacket->PazedBuffer->Write(&RecivePacket->DataCount,sizeof(RecivePacket->DataCount));
               }

               // We are only expecting 1 byte of data for the DL_Connect.
               if(ErrorLevel == EDL_Ok && RecivePacket->DataCount != 1)
               {
                  #if (DEBUGLEVEL & debugExpandLowLevelSerialComms )
                     TManagerLogs::Instance().Add(__FUNC__,LOWLEVELSERIAL,"DataLink Connect, Data Count is not 1 byte as required by connect.");
                  #endif
                  ErrorLevel = EDL_CRC;
               }

               // retrieve Maximum databuffer size for this TConnect.
               if(ErrorLevel == EDL_Ok && !ReceiveData(&Max_Buffer_Size,1))
               {
                  #if (DEBUGLEVEL & debugExpandLowLevelSerialComms )
                     TManagerLogs::Instance().Add(__FUNC__,LOWLEVELSERIAL,"DataLink Connect, Failed to retrive the data block of the connect. Timed out.");
                  #endif
                  ErrorLevel = EDL_TimeOut;
               }

               if(ErrorLevel == EDL_Ok)
               {
                  // copy data into recive buffer.
                  this->RecivePacket->PazedBuffer->Write(&Max_Buffer_Size,sizeof(Max_Buffer_Size));

                  Max_Buffer_Size -= 2;
                  // retrieve CRC.
                  // DEBUG check this doesnt load the int backwards...
               }

               // Check the CRC.
               unsigned char highCRC=0;
               unsigned char lowCRC=0;
               if(ErrorLevel == EDL_Ok && !ReceiveData(&highCRC,1))
               {
                  #if (DEBUGLEVEL & debugExpandLowLevelSerialComms )
                     TManagerLogs::Instance().Add(__FUNC__,LOWLEVELSERIAL,"DataLink Connect, Failed to retrive the High CRC byte of the connect Timed out.");
                  #endif
                  ErrorLevel = EDL_TimeOut;
               }

               if(ErrorLevel == EDL_Ok && !ReceiveData(&lowCRC,1))
               {
                  #if (DEBUGLEVEL & debugExpandLowLevelSerialComms )
                     TManagerLogs::Instance().Add(__FUNC__,LOWLEVELSERIAL,"DataLink Connect, Failed to retrive the Low CRC byte of the connect Timed out.");
                  #endif
                  ErrorLevel = EDL_TimeOut;
               }

               if(ErrorLevel == EDL_Ok )
               {
                  int inCRC = ((highCRC *256) + lowCRC);

                  // Calculate the new CRC
                  unsigned char *ptrPacket = (unsigned char *)this->RecivePacket->PazedBuffer->Memory;
                  unsigned short int CRC = 0;

                  CRC =  StartCRC(ptrPacket[0]);
                  for(int i = 1; i < this->RecivePacket->PazedBuffer->Size;i++) //Dont include CRC Section.
                  {
                     CRC = UpdateCRC(CRC,ptrPacket[i]);
                  }

                  // Check CRC.
                  if(CRC != inCRC)
                  {
                     #if (DEBUGLEVEL & debugExpandLowLevelSerialComms )
                        TManagerLogs::Instance().Add(__FUNC__,LOWLEVELSERIAL,"DataLink Connect, CRC Failed. ");
                     #endif
                     ErrorLevel = EDL_CRC;
                  }
                  Connected = true;
                  Completed = true;
               }
            }
            else
            {
               Connected = true;
               Completed = true;
            }
         }
         else
         {
            #if (DEBUGLEVEL & debugExpandLowLevelSerialComms )
               TManagerLogs::Instance().Add(__FUNC__,LOWLEVELSERIAL,"DataLink Connect, Write data failed. ");
            #endif
            ErrorLevel = EDL_Fatal;
         }

         switch(ErrorLevel)
         {
            case EDL_CRC:
            {
               Port->Clear();
               throw EProcessConnectionError(EPC_CRCDisconnect,dDL_Connect_CRCErrorDisconnect);
            }
            case EDL_TimeOut:
            {
               Port->Clear();
               if(TimeOutCount > RetryCount)  // Will default to a NACK.
               {
                  throw EProcessConnectionError(EPC_TimeOut,dDL_Connect_TimeOutDisconnect);
               }
               else
               { // Loop again and Resend Databuffer.
                  TimeOutCount++;
               }
               break;
            }
            case EDL_Fatal:
            {
               Port->Clear();
               throw EProcessConnectionError(EPC_Terminate,dDL_Connect_FatalDisconnect);
            }
         }
         ErrorLevel = EDL_Ok;
      }while(Completed == false);
   }
return true;
}

/*bool TDataLinkControl::DataLayerConnect(bool Reply)
{
   // Number of attempts to transmit a message.
   // Maximum number of attempts is Two.
	int TimeOutCount = 0;
   Connected = false;
   // empty any inbound junk.
   if(Port)
   {
      Port->Clear();
      bool Completed = false;
      do
      {
         try
         {
            SequenceNumber = 0;
            this->SendPacket->Header.Slave = 0;
            this->SendPacket->Header.Reply = Reply;
            this->SendPacket->Header.Sequence = SequenceNumber;
            this->SendPacket->Header.DataLinkControl = DL_CONNECT;

            TMemoryStream *Data;
            Data = new TMemoryStream;
            char Temp = EscapeCharaterDisableFlag;
            Data->Write(&Temp,1);
            this->SendPacket->FormatForWrite(Data);
            delete Data;
            Data = NULL;

            if(Port->Write((unsigned char *)SendPacket->PazedBuffer->Memory, SendPacket->PazedBuffer->Size))
            {
               if(Reply)
               {
                  // retrieve Acknowledge.
                  RecivePacket->Clear();
                  unsigned char *ptrBuffer;
                  ptrBuffer = (unsigned char *) &RecivePacket->Header;
                  // retrieve and strip header.
                  if(!ReceiveData(ptrBuffer,sizeof(RecivePacket)))
                  {
                     throw EDataLinkError(EDL_TimeOut,dDL_Connect_RetrieveHeaderFailed);
                  }
                  // Header valid?
                  if(!ValidHeader(DL_CONNECT))
                  {
                     throw EDataLinkError(EDL_CRC,dDL_Connect_HeaderFailed);
                  }

                  // Header Valid copy into recive buffer.
                  this->RecivePacket->PazedBuffer->Write(&RecivePacket->Header,sizeof(RecivePacket->Header));


                  // retrieve Datablock Size.
                  if(!ReceiveData(&RecivePacket->DataCount,1))
                  {

                     throw EDataLinkError(EDL_TimeOut,dDL_Connect_RetrieveData);
                  }

                  // copy  datasize into recive buffer.
                  this->RecivePacket->PazedBuffer->Write(&RecivePacket->DataCount,sizeof(RecivePacket->DataCount));

                  // We are only expecting 1 byte of data for the DL_Connect.
                  if(RecivePacket->DataCount != 1)
                  {
                     throw EDataLinkError(EDL_CRC,dDL_Connect_DataCountFailed);
                  }

                  // retrieve Maximum databuffer size for this TConnect.
                  if(!ReceiveData(&Max_Buffer_Size,1))
                  {
                     throw EDataLinkError(EDL_TimeOut,dDL_Connect_RetrieveMaxDataSize);
                  }

                  // copy data into recive buffer.
                  this->RecivePacket->PazedBuffer->Write(&Max_Buffer_Size,sizeof(Max_Buffer_Size));

                  Max_Buffer_Size -= 2;

                  // Check the CRC.
                  unsigned char highCRC=0;
                  unsigned char lowCRC=0;
                  // retrieve CRC.
                  // DEBUG check this doesnt load the int backwards...
                  if(!ReceiveData(&highCRC,1))
                  {
                     throw EDataLinkError(EDL_TimeOut,dDL_Connect_RetrieveHighCRC);
                  }
                  if(!ReceiveData(&lowCRC,1))
                  {
                     throw EDataLinkError(EDL_TimeOut,dDL_Connect_RetrieveLowCRC);
                  }
                  int inCRC = ((highCRC *256) + lowCRC);

                  // Calculate the new CRC
                  unsigned char *ptrPacket = (unsigned char *)this->RecivePacket->PazedBuffer->Memory;
                  unsigned short int CRC = 0;

                  CRC =  StartCRC(ptrPacket[0]);
                  for(int i = 1; i < this->RecivePacket->PazedBuffer->Size;i++) //Dont include CRC Section.
                  {
                     CRC = UpdateCRC(CRC,ptrPacket[i]);
                  }

                  // Check CRC.
                  if(CRC != inCRC)
                  {
                     throw EDataLinkError(EDL_CRC,dDL_Connect_CRCFailed);
                  }
                  Connected = true;
                  Completed = true;
               }
               else
               {
                  Connected = true;
                  Completed = true;
               }
            }
            else
            {
               throw EDataLinkError(EDL_Fatal,dDL_Connect_WriteFailedOnConnect);
            }
         }
         catch(EDataLinkError &Err)
         {
            Port->Clear();
            switch(Err.Type)
            {
               case EDL_CRC:
               {
                  throw EProcessConnectionError(EPC_CRCDisconnect,dDL_Connect_CRCErrorDisconnect);
               }
               case EDL_TimeOut:
               {
                  if(TimeOutCount > RetryCount)  // Will default to a NACK.
                  {
                     throw EProcessConnectionError(EPC_TimeOut,dDL_Connect_TimeOutDisconnect);
                  }
                  else
                  { // Loop again and Resend Databuffer.
                     TimeOutCount++;
                  }
                  break;
               }
               case EDL_Fatal:
               {
                  throw EProcessConnectionError(EPC_Terminate,dDL_Connect_FatalDisconnect);
               }
            }
         }
      }while(Completed == false);
   }
return true;
}
*/

//---------------------------------------------------------------------------
bool TDataLinkControl::DataLayerDisconnect()
{
   // Number of attempts to transmit a message.
   // Maximum number of attempts is Two.
   int RetryCountSoFar = 0;
 	Connected = false;
   return true;
   
/*   int CurrentTime = GetTickCount();
   int StopTime = CurrentTime + 2000;
   bool TimedOut = false;
   while(Connected == true && (!TimedOut))
   {
		CurrentTime = GetTickCount();
      if(StopTime < CurrentTime)
      {
	      TimedOut = true;
      }
      try
      {
         this->SendPacket->Header.Slave = 0;
         this->SendPacket->Header.Reply = 1;
         this->SendPacket->Header.Sequence = 0;
         this->SendPacket->Header.DataLinkControl = DL_DISCONNECT;

         TMemoryStream *Data;
         Data = new TMemoryStream;
         this->SendPacket->FormatForWrite(Data);
         delete Data;
         Data = NULL;
			// empty any inbound junk.
         Port->Clear();

         if(Port->Write((unsigned char *)SendPacket->PazedBuffer->Memory, SendPacket->PazedBuffer->Size))
         {
            // retrieve Acknowledge.

            unsigned char *ptrBuffer;
            ptrBuffer = (unsigned char *) &RecivePacket->Header;
            // retrieve and strip header.
            if(!ReceiveData(ptrBuffer,sizeof(RecivePacket)))
            {
               throw EDataLinkError(EDL_TimeOut,1);
            }
            // Header valid?
            if(!ValidHeader(DL_CONNECT))
            {
               throw EDataLinkError(EDL_CRC,2);
            }

            // Header Valid copy into recive buffer.
            this->RecivePacket->PazedBuffer->Write(&RecivePacket->Header,sizeof(RecivePacket->Header));


            // retrieve Datablock Size.
            if(!ReceiveData(&RecivePacket->DataCount,1))
            {
               throw EDataLinkError(EDL_TimeOut,3);
            }

            // copy  datasize into recive buffer.
            this->RecivePacket->PazedBuffer->Write(&RecivePacket->DataCount,sizeof(RecivePacket->DataCount));

            // We are only expecting 1 byte of data for the DL_Connect.
            if(RecivePacket->DataCount != 0)
            {
               throw  EDataLinkError(EDL_TimeOut,3);
            }

            // Check the CRC.
            unsigned char highCRC=0;
            unsigned char lowCRC=0;
            // retrieve CRC.
            // DEBUG check this doesnt load the int backwards...
            if(!ReceiveData(&highCRC,1))
            {
               throw EDataLinkError(EDL_TimeOut,4);
            }
            if(!ReceiveData(&lowCRC,1))
            {
 					throw EDataLinkError(EDL_TimeOut,5);
            }

            int inCRC = ((highCRC *256) + lowCRC);

            // Calculate the new CRC
            unsigned char *ptrPacket = (unsigned char *)this->RecivePacket->PazedBuffer->Memory;
            unsigned short int CRC = 0;

            CRC =  StartCRC(ptrPacket[0]);
            for(int i = 1; i < this->RecivePacket->PazedBuffer->Size;i++) //Dont include CRC Section.
            {
               CRC = UpdateCRC(CRC,ptrPacket[i]);
            }

            // Check CRC.
            if(CRC != inCRC)
            {
            	throw EDataLinkError(EDL_CRC,6);
            }
            // Disconnect.
            Connected = false;
         }
         else
         {
            throw EDataLinkError(EDL_Fatal,3);
         }
      }
      catch(EDataLinkError &Err)
      {
         Port->Clear();
			switch(Err.Type)
         {
				case EDL_CRC:
            {
              	if(RetryCountSoFar > RetryCount)
               {
               	throw EProcessConnectionError(EPC_CRCDisconnect,1);
               }
               else
               {
               	RetryCountSoFar++;
               }
					break;
            }
            case EDL_TimeOut:
            {
				   	Connected = false;
               	throw EProcessConnectionError(EPC_TimeOut,1);
				}
            case EDL_Fatal:
            {
			   	Connected = false;
               throw EProcessConnectionError(EPC_Terminate,1);
            }
         }
      }
   }
   if(TimedOut)
   {
   	Connected = false;
   }
	return true;*/
}

//---------------------------------------------------------------------------


bool TDataLinkControl::ValidHeader(LinkControl inCommand)
{
	switch(inCommand)
   {
		case DL_CONNECT :
			if(Address == ((RecivePacket->Header.hAddress * 265) + RecivePacket->Header.lAddress) )
         {
            if((RecivePacket->Header.Slave == 1)
            && (RecivePacket->Header.Reply == 0) // DEBUG This could be echoed.
            && (RecivePacket->Header.Sequence == 0)
            && (RecivePacket->Header.DataLinkControl == DL_ACKNOWLEDGE))
            {
               return true;
            }
            else
            {
               return false;
            }
         }
         else
         {
				return false;
         }
      case DL_DISCONNECT :
         //Check Address.
			if(Address == ((RecivePacket->Header.hAddress * 265) + RecivePacket->Header.lAddress))
         {
            if((RecivePacket->Header.Slave == 1)
            && (RecivePacket->Header.Reply == 0) // DEBUG This could be echoed.
            && (RecivePacket->Header.Sequence == 0)
            && (RecivePacket->Header.DataLinkControl == DL_ACKNOWLEDGE))
            {
               return true;
            }
            else
            {
               return false;
            }
         }
         else
         {
				return false;
         }
      case DL_SINGLE					:
         //Check Address.
			if(Address == ((RecivePacket->Header.hAddress * 265) + RecivePacket->Header.lAddress))
         {
            if((RecivePacket->Header.Slave == 1)
            && (RecivePacket->Header.Reply == 0)
 // DEBUG Correct Sequence plase.
            && (RecivePacket->Header.Sequence == SequenceNumber)
            && (RecivePacket->Header.DataLinkControl == DL_SINGLE))
            {
               return true;
            }
            else
            {
               return false;
            }
         }
         else
         {
				return false;
         }
      case DL_ACKNOWLEDGE			:
      break;
      case DL_BUFFER_OVERFLOW		:
      break;
      case DL_ERROR					:
      break;
	}
   return false;
}



//---------------------------------------------------------------------------
//------------------------------ TPacket ---------------------------------------
//---------------------------------------------------------------------------


__fastcall TPacket::TPacket()
{
	this->PazedBuffer = new TMemoryStream();
	this->UnPazedBuffer = new TMemoryStream();
}



//---------------------------------------------------------------------------

__fastcall TPacket::~TPacket()
{
  	delete (TMemoryStream *)PazedBuffer;
   PazedBuffer = NULL;
	delete (TMemoryStream *)UnPazedBuffer;
   UnPazedBuffer = NULL;
}

void __fastcall TPacket::Clear()
{
	PazedBuffer->Clear();
	UnPazedBuffer->Clear();
   Header.DataLinkControl = 0;
	Header.SpaceHolder = 0;	// Reserved. bitfields
	Header.Sequence = 0;
	Header.Reply = 0;
	Header.Slave = 0;
}


//---------------------------------------------------------------------------

bool TPacket::FormatForWrite(TMemoryStream *inData)
{
   unsigned char Data;
   unsigned char EscapeChar = 0x7D;
	unsigned char StartFlag = 0x7E;

	PazedBuffer->Clear();
	UnPazedBuffer->Clear();

	// Format Memory Stream.
   UnPazedBuffer->Write(&Header, sizeof(Header));

   DataCount = inData->Size;
   UnPazedBuffer->Write(&DataCount, sizeof(DataCount));
   // Copy Data.
	this->UnPazedBuffer->Write(inData->Memory,inData->Size);
   // Calculate CRC on memory stream.
   unsigned short int CRC = 0, TempCRC = 0;
   unsigned char *ptrBuffer = (unsigned char *)UnPazedBuffer->Memory;

   // Cal CRC
   TempCRC = StartCRC(*ptrBuffer);
   for(int i = 1; i < UnPazedBuffer->Size;i++) //Dont include CRC Section.
   {
   	TempCRC = UpdateCRC(TempCRC,ptrBuffer[i]);
   }

   CRC =  (HIBYTE(TempCRC));
	this->UnPazedBuffer->Write(&CRC,1);
   CRC =  (LOBYTE(TempCRC));
	this->UnPazedBuffer->Write(&CRC,1);

	//Move the UnPazed data into the Pazzed Data buffer Pazzing it on the way.
   unsigned char *ptrUnPazedBuffer = (unsigned char *)UnPazedBuffer->Memory;
   for(int i = 0; i < UnPazedBuffer->Size; i++)
   {
   	if(i == 0) //stright copy the start flag.
      {
          PazedBuffer->Write(&ptrUnPazedBuffer[i],1);
      }
      else if(ptrUnPazedBuffer[i] == StartFlag || ptrUnPazedBuffer[i] == EscapeChar) // Escape Character.
      {
            //insert escape char.
            PazedBuffer->Write(&EscapeChar,1);
            // Get the next byte and escape it.
            Data = ptrUnPazedBuffer[i];
            Data ^= 0x20;
         	PazedBuffer->Write(&Data,1);
      }
      else
      {
          PazedBuffer->Write(&ptrUnPazedBuffer[i],1);
      }
   }
   return true;
}


