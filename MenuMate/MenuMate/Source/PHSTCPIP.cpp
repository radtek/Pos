// ---------------------------------------------------------------------------

#pragma hdrstop

#include "Main.h"
#include "PHSTCPIP.h"
#include "MMLogging.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

int AnsiStringRead(AnsiString Data, AnsiString &Value, char StopChar, int inIndex)
{
	unsigned char input = NULL;
	int Index = inIndex;
	Value = "";
	do
	{
		input = Data[Index];
		if (input != StopChar)
		{
			Value += (char)(input);
		}
		Index++;
	}
	while (input != StopChar && Index <= Data.Length());
	return Index;
}

TPhoenixRoomStatusExt::TPhoenixRoomStatusExt()
{
	Folders = new TStringList;
	Clear();
}

TPhoenixRoomStatusExt::~TPhoenixRoomStatusExt()
{
	delete Folders;
}

void TPhoenixRoomStatusExt::Clear()
{
	MessageIDOut = 1; // 1
	MessageIDIn = 2; // 2
	Length = 0;
	POSID = 0;
	AccountNumber = 0;
	FolderNumber = 0;
	Balance = 0;
	CreditLimit = 0;
	Folders->Clear();
	Result = eFailed;
	ResultText = "Result not set.";
	fStartOfPacket = '\\';
	fSeperator = ',';
}

AnsiString TPhoenixRoomStatusExt::SerializeOut()
{
	AnsiString HeaderData, Data;
	AnsiString Length = "0000";
	int Size = 0;

	HeaderData = "\\7,";
	Data = "," + IntToStr(POSID) + "," + AccountNumber;

	Size = (HeaderData + Length + Data).Length() - 1;
	Length.sprintf("%04d", Size);

	return HeaderData + Length + Data + '\r';
}

// ---------------------------------------------------------------------------
void TPhoenixRoomStatusExt::SerializeIn(AnsiString Data)
{
	if (Data != "")
	{
		if (Data[1] == '\\')
		{
			Data = Data.SubString(2, Data.Length());
			if (Data[1] == '8')
			{
				Data = Data.SubString(3, Data.Length());
				AnsiString Length, RegNumber, RoomNo, TempBalance, TempCreditLimit, DataCount, NameTemp;
				int Index = 1;
				Index = AnsiStringRead(Data, Length, ',', Index);
				Index = AnsiStringRead(Data, RegNumber, ',', Index);
				Index = AnsiStringRead(Data, RoomNo, ',', Index);
				Index = AnsiStringRead(Data, TempBalance, ',', Index);
				Index = AnsiStringRead(Data, TempCreditLimit, ',', Index);
				Index = AnsiStringRead(Data, DataCount, ',', Index);
				int LoopCount = StrToIntDef(DataCount, 0);
				if (LoopCount == 0)
				{
					Result = eFailed;
					Index = AnsiStringRead(Data, NameTemp, ',', Index);
					ResultText = NameTemp;
				}
				else
				{
					Result = eAccepted;
					Balance = (StrToCurrDef(TempBalance, 0) / 100.00);
					CreditLimit = (StrToCurrDef(TempCreditLimit, 999900) / 100.00);
					Folders->Clear();
					for (int i = 0; i < LoopCount; i++)
					{
						Index = AnsiStringRead(Data, NameTemp, ',', Index);
						Folders->Add(NameTemp);
					}
				}
			}
			else
			{
				Result = eFailed;
				ResultText = "Incorrect PMS Packet Received Expected Type 8 got " + Data;
				TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, ResultText);
				// TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet->ResultText);
			}
		}
		else
		{
			Result = eFailed;
			ResultText = "PMS Packet Missing \\";
			TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, ResultText);
			// TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet->ResultText);
		}
	}
}

// ---------------------------------------------------------------------------
AnsiString TPhoenixRoomCharge::SerializeOut()
{
	AnsiString HeaderData, Data;
	AnsiString Length = "0000";
	int Size = 0;
	HeaderData = "\\3,";

	Data = "," + IntToStr(POSID) + "-" + ReferenceNumber + "," + AccountNumber + "," + IntToStr(FolderNumber) + "," + IntToStr(Covers) + "," + IntToStr(StaffID) + "," + IntToStr(int(Total * 100.00));

	TPhoenixCategoryCurrency::const_iterator Cats = Categories.begin();
	for (; Cats != Categories.end(); Cats++)
	{
		// this needs to round to the nearest cent.
		Currency Value = RoundToNearest(Cats->second, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown) * 100;

		/* TManagerLogs::Instance().Add(__FUNC__,PHOENIXINTERFACELOG,"Cat Out :" + Cats->first +
		"Float : " + FormatFloat("0000.0000",Cats->second) +
		"Rnd : " + FormatFloat("0000.0000",Value) +
		" Int : " + IntToStr(int(Cats->second*100.00))); */

		Data += "," + Cats->first + "," + IntToStr(int(Value));
	}

	Size = (HeaderData + Length + Data).Length() - 1;
	Length.sprintf("%04d", Size);

	return HeaderData + Length + Data + '\r';
}

// ---------------------------------------------------------------------------
void TPhoenixRoomCharge::SerializeIn(AnsiString Data)
{
	if (Data != "")
	{
		if (Data[1] == '\\')
		{
			Data = Data.SubString(2, Data.Length());
			if (Data[1] == '4')
			{
				Data = Data.SubString(3, Data.Length());
				AnsiString Length, RegNumber, RoomNo, FolderNumber, DataCount, DataTemp;
				int Index = 1;
				Index = AnsiStringRead(Data, Length, ',', Index);
				Index = AnsiStringRead(Data, RegNumber, ',', Index);
				Index = AnsiStringRead(Data, RoomNo, ',', Index);
				Index = AnsiStringRead(Data, FolderNumber, ',', Index);
				Index = AnsiStringRead(Data, DataTemp, ',', Index);
				ResultText = DataTemp;

				if (ResultText != "ACK")
				{
					Result = eFailed;
				}
			}
			else
			{
				Result = eFailed;
				ResultText = "Incorrect PMS Packet Received Expected Type 4 got " + Data;
				TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, ResultText);
				// TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet->ResultText);
			}
		}
		else
		{
			Result = eFailed;
			ResultText = "PMS Packet Missing \\";
			TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, ResultText);
			// TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet->ResultText);
		}
	}
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

TPhoenixNetTCPManager::TPhoenixNetTCPManager()
{
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void TPhoenixNetTCPManager::SendAndFetch(TPhoenixBasePacket &Packet, AnsiString HostAddress, int Port)
{
	bool Retry = false;
	int RetryCount = 0;
    std::auto_ptr<TIdTCPClient>fTCPClient(new TIdTCPClient(NULL));
	do
	{
		Retry = false;
		try
		{

			fTCPClient->IPVersion = Id_IPv4;

			if (HostAddress != "")
			{
				Packet.Result = eAccepted;
				AnsiString Data = Packet.SerializeOut();
				if (!fTCPClient->Connected())
				{
					fTCPClient->Host = HostAddress;
					fTCPClient->Port = Port;
					fTCPClient->ConnectTimeout = 8000;
					fTCPClient->BoundPort = 0;
					fTCPClient->ReadTimeout = 20000;
					fTCPClient->Connect();
				}

				if (fTCPClient->Connected())
				{
					TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, "Write :" + Data);
					fTCPClient->IOHandler->WriteLn(Data);
					Data = "";
					Data = fTCPClient->IOHandler->ReadLn(Idglobal::CR, 10000, 255);

					if (Data == "")
					{
						Packet.Result = eFailed;
						Packet.ResultText = "PMS Server Failed To Respond.";
						TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, Packet.ResultText);
					}
					else
					{
						if (fTCPClient->IOHandler->InputBuffer->Size == 1)
						{ // Gobble left over Line feed.
							fTCPClient->IOHandler->ReadByte();
						}
						else if (fTCPClient->IOHandler->InputBuffer->Size > 1)
						{
							Packet.ResultText = "Server Responeded but not all Data read";
							TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, "Server Responeded but not all Data read");
							TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, "Data : " + Data);
							TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, "Remaining Buffer Size : " + IntToStr(fTCPClient->IOHandler->InputBuffer->Size));
						}

						TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, "Read :" + Data);
						Packet.SerializeIn(Data);
					}
				}
				else
				{
					Packet.Result = eFailed;
					Packet.ResultText = "Failed To Connect to PMS Server.";
					TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, Packet.ResultText);
				}

				// Disconnect Reguardless.
				fTCPClient->Disconnect();
				fTCPClient->IOHandler->InputBuffer->Clear();
			}
			else
			{
				Packet.Result = eFailed;
				Packet.ResultText = "Invalid Host Address :" + HostAddress;
				TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, "Invalid Host Address :" + HostAddress);
			}
		}
		catch(Exception & E)
		{
			// TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
			// TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
			if (fTCPClient->IOHandler)
			{
				AnsiString Dat = fTCPClient->IOHandler->ReadLn(Idglobal::LF, 10000, 255);
			}
			fTCPClient->Disconnect();
			Retry = true;
			RetryCount++;
			Packet.Result = eFailed;
			Packet.ResultText = E.Message;
			TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, Packet.ResultText);
			::Sleep(1000);
		}
	}
	while (Retry == true && RetryCount < 6);
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

TPhoenixBasePacket::TPhoenixBasePacket()
{
	MessageIDOut = ""; // 1
	MessageIDIn = ""; // 1
	Length = 0;
	POSID = 0;
	AccountNumber = 0;
	FolderNumber = 1;
	Result = eAccepted;
	ResultText = "";
}

TPhoenixRoomCharge::TPhoenixRoomCharge()
{
	StaffID = 0;
	Total = 0;
	ReferenceNumber = "";
	Covers = 0;
}
