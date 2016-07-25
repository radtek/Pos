//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "bluetooth.h"
#include "CrcTable.h"
#include "Enum.h"
#include "MMRegistry.h"
#include "Palm.h"
#include "ManagerVariable.h"
#include "Version.h"
#include <numeric>
#include <algorithm>

#if (DEBUGLEVEL & debugExpandPalmComms )
#include <stdio.h>
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
using std::max;
using std::min;
//---------------------------------------------------------------------------
void __fastcall TBluetoothLayer::GetStuff(TObject *Sender,int count)
{
   try
   {
      try
		{
//			long TimeStart = ::GetTickCount();
         inData->Clear();
         Packet->Clear();
         CommPort->ClearBuffer(false,true);
         // Fill in the Header and hte rest of the packet.
         if(ReceiveData((unsigned char *)&Packet->Header,sizeof(TBlueToothpktHeader)))
         {
			   #if (DEBUGLEVEL & debugExpandPalmComms )
				TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BT DATA RECEIVED","Palm #"+ IntToStr(Packet->Header.DeviceKey));
            char out[10];
            char *SentData = (char *)&Packet->Header;
            UnicodeString TextOut,Fo0,Fo1;
            char arg;
            for (int i = 0; i < sizeof(TBlueToothpktHeader); i++)
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
				TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BT HEADER " + TextOut,"Palm #"+ IntToStr(Packet->Header.DeviceKey) );
				#endif
				// Retrive the rest of the data and store it in InData.

				if(Packet->Header.StartFlag != 0x02 ||							// Wrong stream version)
					UnicodeString(Packet->Header.ProductVersion) != "4.1")//"4.1")	// Wrong file version
//					UnicodeString(Packet->Header.ProductVersion) != IntToStr(GetFileVersionMajor()) + "." + IntToStr(GetFileVersionMinor()))//"4.1")	// Wrong file version
				{
					switch (Packet->Header.StartFlag)
					{
						case 0x01:
						case 0x02:
						case 0x7E:
						{
							Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
							DBTransaction.StartTransaction();
							TPalm *Palm = TDeviceRealPalms::Instance().FindTPalm(DBTransaction,Packet->Header.DeviceKey);
							DBTransaction.Commit();
							if(Palm != NULL)
							{
								Palm->Status.Error = Err_Incorrect_Version;
								Palm->BuildPacket(NULL,KEEPALIVE);
								SendBlueToothPacket(Palm);
								#if (DEBUGLEVEL & debugExpandPalmComms )
								TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"INCORRECT BLUETOOTH PACKET VERSION","Palm #"+ IntToStr(Palm->ID.DeviceKey));
								#endif
							}
							break;
						}
						default:
						{
							#if (DEBUGLEVEL & debugExpandPalmComms )
							TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BLUETOOTH PACKET BAD START FLAG","Palm #"+ IntToStr(Packet->Header.DeviceKey));
							#endif
							break;
						}
					}
				}
				else
				{
					if(Packet->Header.DataCount > 0)
					{
						//if(TDeviceRealPalms::Instance().OS.MMSoftwareVersion != UnicodeString(Packet->Header.ProductVersion))
/*						if(UnicodeString("4.0") != UnicodeString(Packet->Header.ProductVersion))
						{
							Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
							DBTransaction.StartTransaction();
							TPalm *Palm = TDeviceRealPalms::Instance().FindTPalm(DBTransaction,Packet->Header.DeviceKey);
							DBTransaction.Commit();
							if(Palm != NULL)
							{
								Palm->Status.Error = Err_Incorrect_Version;
								Palm->BuildPacket(NULL,KEEPALIVE);
								SendBlueToothPacket(Palm);
								#if (DEBUGLEVEL & debugExpandPalmComms )
								TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"INCORRECT BLUETOOTH PACKET VERSION","Palm #"+ IntToStr(Palm->ID.DeviceKey));
								#endif
							}
						}
						else*/ if(RetriveBlueToothPacket())
						{
							Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
							DBTransaction.StartTransaction();
							TPalm *Palm = TDeviceRealPalms::Instance().FindTPalm(DBTransaction,Packet->Header.DeviceKey);
							DBTransaction.Commit();
							Palm->Status.Error = proA_Ok;
							Palm->ptrRawData = (unsigned char *)inData->Memory;
							Palm->ProcessTransmission();

							// Send Responce.
							#if (DEBUGLEVEL & debugExpandPalmComms )
							TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BLUETOOTH DATA SENT","Palm #"+ IntToStr(Palm->ID.DeviceKey));
							TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BLUETOOTH GET ORDERS TURNAROUND TIME " + IntToStr((int)Packet->GetNewOrderTimeout()),"");
							TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BLUETOOTH SET ORDERS TURNAROUND TIME (PER ORDER) " + IntToStr((int)Packet->GetRetrieveOrdersTimeout()),"");
							#endif
							if (SendBlueToothPacket(Palm)) // Check to see if the palm is listening
							{
								if(Palm->LastErrorNumber == proA_Ok)
								{
									switch(Palm->LastTransmissionType) // Preform Palm Data Clean up depending on
									{                        // Transmission type and weather it was susscful.
										case KEEPALIVE :
										break;
										case ORDERS :
										break;
										case MENU_SYNC :
												Palm->resetMenuEnableState(); // Palm retrieved Data ok update this end.
										break;
										case DRINK_SYNC :
												Palm->resetMenuEnableState(); // Palm retrieved Data ok update this end.
										break;
										case LOGIN :
												Palm->resetMenuEnableState(); // Palm retrieved Data ok update this end.
										break;
										case INFO_SYNC :
										break;
										case REQUEST_RECEIPT :
										break;
										case RETRIEVE_ORDERS :
												Palm->setMenuEnableState(); // Palm retrieved Data ok update this end.
	//											Palm->ClearSeatNames();
												Palm->ClearMessageQue();
										break;
										case REGISTRATION :
												if(Palm->Delete)
												{
													Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
													DBTransaction.StartTransaction();
													Palm->Destroy(DBTransaction);
													DBTransaction.Commit();
													TDeviceRealPalms::Instance().PalmList->Remove(Palm);
													delete Palm;
												}
										break;
									}
									// No one should be able to log in on this palm.
									if(TDeviceRealPalms::Instance().PalmNotFound)
									{
										TDeviceRealPalms::Instance().PalmNotFound->User.ContactKey = 0;
									}
								}
							}
						}
						else
						{
							#if (DEBUGLEVEL & debugExpandPalmComms )
							TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BLUETOOTH RetriveBlueToothPacket FAILED","Palm #"+ IntToStr(Packet->Header.DeviceKey));
							#endif
							Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
							DBTransaction.StartTransaction();
							TPalm *Palm = TDeviceRealPalms::Instance().FindTPalm(DBTransaction,Packet->Header.DeviceKey);
							DBTransaction.Commit();
							if(Palm != NULL)
							{
								Palm->Status.Error = Err_Msg_CRC;
								Palm->BuildPacket(NULL,KEEPALIVE);
								SendBlueToothPacket(Palm);
							}
						}
					}
					else
					{
						#if (DEBUGLEVEL & debugExpandPalmComms )
						TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BLUETOOTH PACKET EMPTY","Palm #"+ IntToStr(Packet->Header.DeviceKey));
						#endif
					}
				}
/*				else
				{
					#if (DEBUGLEVEL & debugExpandPalmComms )
					TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BLUETOOTH PACKET BAD START FLAG","Palm #"+ IntToStr(Packet->Header.DeviceKey));
					#endif
				} */
			}
		}
		__finally
		{
			CommPort->ClearBuffer(true,false);
		}
	}
	catch(Exception &E)
	{
		UnicodeString SysMessage = SysErrorMessage(GetLastError());

		TManagerLogs::Instance().Add(__FUNC__, PALMBTLAYERLOG,	E.Message + ": " + SysMessage);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG,	E.Message + ": " + SysMessage);

		CommPort->ClearBuffer(true,true);
	}
}

TBluetoothLayer::TBluetoothLayer()
{
	Packet = new TBlueToothPacket;

	inData = new TMemoryStream;

   CommPort = new TComPort(NULL);
   CommPort->BaudRate = br115200;
   CommPort->Buffer->InputSize = 16384;
   CommPort->Buffer->OutputSize = 16384;
   CommPort->OnRxChar = GetStuff;
   CommPort->DiscardNull = false;
   TComEvents Events;
   Events << evRxChar;
	CommPort->Events = Events;
}

TBluetoothLayer::~TBluetoothLayer()
{
   CommPort->Close();
   delete CommPort;
   delete inData;
   delete Packet;
}

bool TBluetoothLayer::SendBlueToothPacket(TPalm *Palm)
{
   TMemoryStream *Outbound = Palm->Outbound;

   unsigned char Data;
//	unsigned char EscapeChar = 0x7D;
	unsigned char StartFlag = 0x7E;

	Packet->Clear();
	Packet->Header.StartFlag = StartFlag;
	Packet->Header.DeviceKey = (unsigned short)Palm->ID.DeviceKey;
   Packet->Header.DataCount = Outbound->Size + 2;
	Packet->Header.ProcSetDelay = Packet->GetNewOrderTimeout();
	Packet->Header.ProcGetDelay = Packet->GetRetrieveOrdersTimeout();
	// Format Memory Stream.

   Packet->Buffer->Write(&Packet->Header, sizeof(TBlueToothpktHeader));
   #if (DEBUGLEVEL & debugExpandPalmComms )
   {
      char out[10];
      char *SentData = (char *)&Packet->Header;
      UnicodeString TextOut,Fo0,Fo1;
      char arg;
      for (int i = 0; i < sizeof(TBlueToothpktHeader); i++)
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
		TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BT HEADER SENT " + TextOut,"Palm #"+ IntToStr(Packet->Header.DeviceKey) );
	}
	#endif

   Outbound->Position = 0;
   Packet->Buffer->Write(Outbound->Memory, Outbound->Size);
   #if (DEBUGLEVEL & debugExpandPalmComms )
   {
      char out[10];
      char *SentData = (char *)(Outbound->Memory);
      UnicodeString TextOut,Fo0,Fo1;
      char arg;
      for (int i = 0; i < Outbound->Size; i++)
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
		TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BT DATA SENT " + TextOut,"Palm #"+ IntToStr(Packet->Header.DeviceKey) );
	}
	#endif

   // Calculate CRC on memory stream.
   unsigned short int CRC = 0, TempCRC = 0;
   unsigned char *ptrBuffer = (unsigned char *)Outbound->Memory;
   // Cal CRC
   TempCRC = StartCRC(*ptrBuffer);
   for(int i = 1; i < Outbound->Size;i++) //Dont include CRC Section.
   {
   	TempCRC = UpdateCRC(TempCRC,ptrBuffer[i]);
   }

   CRC =  (HIBYTE(TempCRC));
   Packet->Buffer->Write(&CRC,1);
   CRC =  (LOBYTE(TempCRC));
   Packet->Buffer->Write(&CRC,1);

/*   for(int i = 0; i < Outbound->Size; i++)
   {
   	if(i == 0) //stright copy the start flag.
		{
          Packet->Buffer->Write(&ptrBuffer[i],1);
      }
      else if(ptrBuffer[i] == StartFlag || ptrBuffer[i] == EscapeChar) // Escape Character.
      {
            //insert escape char.
            Packet->Buffer->Write(&EscapeChar,1);
            // Get the next byte and escape it.
            Data = ptrBuffer[i];
            Data ^= 0x20;
         	Packet->Buffer->Write(&Data,1);
      }
      else
      {
          Packet->Buffer->Write(&ptrBuffer[i],1);
      }
   }*/

//	Packet->Buffer->Position = 0;
//	int Count = CommPort->Write(Packet->Buffer->Memory,Packet->Buffer->Size);
//	TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"Expected to Write "  + IntToStr(Packet->Buffer->Size) + " Wrote " + IntToStr(Count) + " Bytes Written" ,"Palm #"+ IntToStr(Packet->Header.DeviceKey) );
//	return true;


	Packet->Buffer->Position = 0;
	int Count = 0;
	char *Buffer = static_cast<char *>(Packet->Buffer->Memory);
	int MessageSize = Packet->Buffer->Size;
	if (MessageSize > 0)
	{
		Count = CommPort->Write(Buffer++, 1);
		if (Count == 0)
		{
			CommPort->ClearBuffer(false, true);
			TManagerLogs::Instance().Add(__FUNC__, PALMBTLAYERLOG, "Write timed out", "Palm #" + IntToStr(Packet->Header.DeviceKey));
			return false;
		}
		else if (MessageSize > 1)
		{
			Count += CommPort->Write(Buffer, MessageSize - 1);
		}
	}
	TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"Expected to Write "  + IntToStr(Packet->Buffer->Size) + " Wrote " + IntToStr(Count) + " Bytes Written" ,"Palm #"+ IntToStr(Packet->Header.DeviceKey) );
	return true;
}


bool TBluetoothLayer::RetriveBlueToothPacket()
{
   bool Retval = false;
   unsigned char *ptrAppData = new unsigned char[Packet->Header.DataCount];
   ZeroMemory(ptrAppData,Packet->Header.DataCount);
   try
   {
      if(ReceiveData(ptrAppData,Packet->Header.DataCount))
      {
		   #if (DEBUGLEVEL & debugExpandPalmComms )
         char out[10];
         char *SentData = (char *)ptrAppData;
         UnicodeString TextOut,Fo0,Fo1;
         char arg;
         for (int i = 0; i < Packet->Header.DataCount; i++)
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
			TManagerLogs::Instance().Add(__FUNC__,PALMBTLAYERLOG,"BT DATA RECEIVED " + TextOut,"Palm #"+ IntToStr(Packet->Header.DeviceKey) );
			#endif
         // Copy the data into the Program layer structure.
         inData->Write(ptrAppData,Packet->Header.DataCount);
         Retval = true;
      }
   }
   __finally
   {
      delete []ptrAppData;
   }
   return Retval;
}



bool TBluetoothLayer::ReceiveData(unsigned char *ptrBuffer, int BytesToRead)
{
// DEBUG Needs REVISING Time out Handling.
   #define Max_Bytes 50000
//   unsigned char EscapeChar = 0x7D;
   int CurrentTime,TimeOut;

	unsigned char TempBuffer[Max_Bytes];
   ZeroMemory(TempBuffer,Max_Bytes);
   int BytesStillRequired = BytesToRead;

	CurrentTime = ::GetTickCount();
   TimeOut = CurrentTime + TimeOutms;

   while(BytesStillRequired != 0)
   {
      int unParsedBytesRead =  CommPort->Read(TempBuffer,BytesStillRequired);
      // Loop though all unparsed bytes
      for(int i = 0 ; i < unParsedBytesRead ; i++)
      {
         // read in each byte.
         unsigned char Data = TempBuffer[i];
         *ptrBuffer++ = Data;
      }

      BytesStillRequired -= unParsedBytesRead;
		CurrentTime = ::GetTickCount();
      if(unParsedBytesRead != 0)
      {
         TimeOut = CurrentTime + TimeOutms;
      }
      unParsedBytesRead = 0;
      if(CurrentTime > TimeOut)
      {
         return false;
      }
   }
   return true;
}

bool TBluetoothLayer::Open()
{
	bool RetVal = false;
	try
	{
		CommPort->Open();
		CommPort->ClearBuffer(true,true);
		RetVal = true;
	}
	catch(Exception &E)
	{
		ShowMessage(E.Message);
		RetVal = false;
	}
	return RetVal;
}

bool TBluetoothLayer::Close()
{
   try
   {
		CommPort->Close();
		return true;
   }
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"BLUETOOTH Exception",E.Message);
		ShowMessage(E.Message);
      return false;
   }
}

void TBluetoothLayer::SetGetOrderProcDelay(unsigned short ProcGetDelay)
{
	Packet->SetRetrieveOrdersTimeout(ProcGetDelay);
}

void TBluetoothLayer::SetNewOrderTimeout(DWORD TotalDurationMs, int OrderCount)
{
	Packet->SetNewOrderTimeout(TotalDurationMs, OrderCount);
}

void __fastcall TBlueToothPacket::Clear()
{
	Buffer->Clear();
	Header.StartFlag = 0;
	Header.DeviceKey = 0;	// Reserved. bitfields
	Header.DataCount = 0;
}
//---------------------------------------------------------------------------
const int TBlueToothPacket::InitNewOrderTimeout					= 300;
const int TBlueToothPacket::InitGetOrdersTimeout				= 10000;
const int TBlueToothPacket::MinNewOrderTimeout					= 100;
const int TBlueToothPacket::MinGetOrdersTimeout					= 5000;
const int TBlueToothPacket::RunningAverageElementCount		= 10;
const int TBlueToothPacket::NewOrdersTimeoutScalerPercent	= 125;

// 1 or 2 orders shouldn't take longer than 5s each. A 5s additional total timeout is added by the palm.
const int TBlueToothPacket::SmallOrderMaxTimeout				= 5000;
// 100 or so orders shouldn't take longer than 0.5s each.
const int TBlueToothPacket::BigOrderMaxTimeout					= 500;
// How many orders is a 'Big' order?
const int TBlueToothPacket::BigOrderSize							= 100;
//---------------------------------------------------------------------------
TBlueToothPacket::TQueue TBlueToothPacket::NewOrderTimeoutQueue(RunningAverageElementCount, InitNewOrderTimeout);
TBlueToothPacket::TQueue TBlueToothPacket::GetOrdersTimeoutQueue(RunningAverageElementCount, InitGetOrdersTimeout);

int TBlueToothPacket::NewOrderQueueHeadIndex(0);
int TBlueToothPacket::GetOrdersQueueHeadIndex(0);
//---------------------------------------------------------------------------
TBlueToothPacket::TBlueToothPacket() : Buffer(new TMemoryStream)
{
/*   msMinGetProcDelay = 10000;
   msMinSetProcDelay = 10000;
   msCurrentSetProcDelay = msMinSetProcDelay;
   tickCurrentSetProcDelay = msMinSetProcDelay/10;
   tickCurrentGetProcDelay = msMinGetProcDelay/10;   
	msCurrentGetProcDelay = msMinGetProcDelay; */
}
//---------------------------------------------------------------------------
TBlueToothPacket::~TBlueToothPacket()
{
	delete Buffer;
}
//---------------------------------------------------------------------------
void TBlueToothPacket::SetNewOrderTimeout(DWORD TotalDuration, int OrderCount)
{
	// Need to limit the raw timeout in case of exceptional circumstances.
	// Probably during debugging.

	// Calculate raw per item time
	int duration = TotalDuration / OrderCount;

	// Max timeout per order is proportional to the number of orders.
	// 1 order will take longer (per order) than 100 orders.
	// 1 order max's out at about 5 seconds.
	// More than about 100 orders max's out at 500ms.

	// y = mx + c
	// -ve m for decreasing max
	int maxDuration = -OrderCount * (SmallOrderMaxTimeout / BigOrderSize) + SmallOrderMaxTimeout;
	// Absolute lowest max. Represents a total wait of 50ish seconds for 100 orders, 100s for 200 orders etc.
	maxDuration = max(maxDuration, BigOrderMaxTimeout);

	AddTimeoutToQueue(NewOrderTimeoutQueue, NewOrderQueueHeadIndex, min(duration, maxDuration));
}
//---------------------------------------------------------------------------
DWORD TBlueToothPacket::GetNewOrderTimeout()
{
	return GetTimeoutQueueAverage(NewOrderTimeoutQueue, MinNewOrderTimeout);
}
//---------------------------------------------------------------------------
void TBlueToothPacket::SetRetrieveOrdersTimeout(DWORD Duration)
{
	AddTimeoutToQueue(GetOrdersTimeoutQueue, GetOrdersQueueHeadIndex, Duration);
}
//---------------------------------------------------------------------------
DWORD TBlueToothPacket::GetRetrieveOrdersTimeout()
{
	return GetTimeoutQueueAverage(GetOrdersTimeoutQueue, MinGetOrdersTimeout);
}
//---------------------------------------------------------------------------
void TBlueToothPacket::AddTimeoutToQueue(TQueue &Queue, int &HeadIndex, DWORD Duration)
{
	DWORD average = std::accumulate(Queue.begin(), Queue.end(), 0) / Queue.size();
	DWORD newEntity = (average + (Duration * 3)) / 4; // A bit of padding to stop a large anomily to have full effect for 10 beams
	if (newEntity < average)
	{
		Queue[HeadIndex++] = newEntity;
		if (HeadIndex >= static_cast<int>(Queue.size())) HeadIndex = 0;
	}
	else
	{
		// Instantly jump to new LONG timeout so next beam wont fail.
		Queue.clear();
		Queue = TQueue(RunningAverageElementCount, Duration);
	}
}
//---------------------------------------------------------------------------
DWORD TBlueToothPacket::GetTimeoutQueueAverage(TQueue &Queue, unsigned int Min)
{
	return (std::max)(Min, std::accumulate(Queue.begin(), Queue.end(), 0) / Queue.size()) * NewOrdersTimeoutScalerPercent / 100;
}
//---------------------------------------------------------------------------
/*void TBlueToothPacket::SetGetOrderProcDelay(unsigned short ProcGetDelay)
{
	ProcGetDelay += msMinGetProcDelay;
	if(ProcGetDelay > msCurrentGetProcDelay)
   {
      msCurrentGetProcDelay = ProcGetDelay;
   }
   else
	{
      if( (msCurrentGetProcDelay - 100) > ProcGetDelay)
      {
          msCurrentGetProcDelay -= 100;
      }
      else
      {
         msCurrentGetProcDelay = ProcGetDelay;
      }
   }
   tickCurrentGetProcDelay = (msCurrentGetProcDelay/10);
}
//---------------------------------------------------------------------------
void TBlueToothPacket::SetNewOrderTimeout(DWORD OrderDurationMs)
{
	ProcSetDelay += msMinSetProcDelay;
   if(ProcSetDelay > msCurrentSetProcDelay)
   {
      msCurrentSetProcDelay = ProcSetDelay;
   }
	else
   {
      if( (msCurrentSetProcDelay - 100) > ProcSetDelay)
      {
          msCurrentSetProcDelay -= 100;
      }
      else
      {
         msCurrentSetProcDelay = ProcSetDelay;
      }
   }
	tickCurrentSetProcDelay = (msCurrentSetProcDelay/10);
} */
//---------------------------------------------------------------------------
void TBluetoothLayer::Initialise(Database::TDBTransaction &DBTransaction)
{
	UnicodeString SerialPort = TManagerVariable::Instance().GetStr(DBTransaction,vmBluetoothPort);
	TimeOutms = TManagerVariable::Instance().GetInt(DBTransaction,vmBluetoothSerialTimeOut,250);

   if(SerialPort != "")
   {
      CommPort->Port = SerialPort;
    	Open();
   }
   else
   {
      CommPort->Close();
   }
}
