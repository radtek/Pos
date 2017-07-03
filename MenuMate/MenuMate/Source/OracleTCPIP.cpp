//---------------------------------------------------------------------------


#pragma hdrstop

#include "OracleTCPIP.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
TOracleTCP::TOracleTCP()
{
}
//---------------------------------------------------------------------------
TOracleTCP::~TOracleTCP()
{
}
//---------------------------------------------------------------------------
void TOracleTCP::SendAndFetch(AnsiString data,AnsiString address,int portNumber)
{
	if (data != "")
	{
//		if (Data[1] == '\\')
//		{
//			Data = Data.SubString(2, Data.Length());
//			if (Data[1] == '8')
//			{
//				Data = Data.SubString(3, Data.Length());
//				AnsiString Length, RegNumber, RoomNo, TempBalance, TempCreditLimit, DataCount, NameTemp;
//				int Index = 1;
//				Index = AnsiStringRead(Data, Length, ',', Index);
//				Index = AnsiStringRead(Data, RegNumber, ',', Index);
//				Index = AnsiStringRead(Data, RoomNo, ',', Index);
//				Index = AnsiStringRead(Data, TempBalance, ',', Index);
//				Index = AnsiStringRead(Data, TempCreditLimit, ',', Index);
//				Index = AnsiStringRead(Data, DataCount, ',', Index);
//				int LoopCount = StrToIntDef(DataCount, 0);
//				if (LoopCount == 0)
//				{
//					Result = eFailed;
//					Index = AnsiStringRead(Data, NameTemp, ',', Index);
//					ResultText = NameTemp;
//				}
//				else
//				{
//					Result = eAccepted;
//					Balance = (StrToCurrDef(TempBalance, 0) / 100.00);
//					CreditLimit = (StrToCurrDef(TempCreditLimit, 999900) / 100.00);
//					Folders->Clear();
//					for (int i = 0; i < LoopCount; i++)
//					{
//						Index = AnsiStringRead(Data, NameTemp, ',', Index);
//						Folders->Add(NameTemp);
//					}
//				}
//			}
//			else
//			{
//				Result = eFailed;
//				ResultText = "Incorrect PMS Packet Received Expected Type 8 got " + Data;
//				TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, ResultText);
//				// TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet->ResultText);
//			}
//		}
//		else
//		{
//			Result = eFailed;
//			ResultText = "PMS Packet Missing \\";
//			TManagerLogs::Instance().Add(__FUNC__, PHOENIXINTERFACELOG, ResultText);
//			// TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet->ResultText);
//		}
	}
}
//---------------------------------------------------------------------------
AnsiString TOracleTCP::SerializeOut(TPostRoomInquiry roomInquiry)
{

}
//---------------------------------------------------------------------------
AnsiString TOracleTCP::SerializeOut(TPostRequest postRequest)
{
}
//---------------------------------------------------------------------------
TRoomInquiryItem TOracleTCP::SerializeIn(AnsiString data, TRoomInquiryAnswer roomAnswer)
{
}
//---------------------------------------------------------------------------
TPostRequestAnswer TOracleTCP::SerializeIn(AnsiString data)
{
}
//---------------------------------------------------------------------------
