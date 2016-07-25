//---------------------------------------------------------------------------


#pragma hdrstop

#include "MembershipEbetData.h"
#include <Sysutils.hpp>
#include <Dateutils.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)

char TBCCPacket::CalcBCC(AnsiString Data)
{
	BYTE BCC = 0;
	if((Data.Pos(ebetChrSTX) > 0) && (Data.Pos(ebetChrETX) != -1))
	{
		for (int i = Data.Pos(ebetChrSTX); (i <= Data.Pos(ebetChrETX) && i <= Data.Length()); i++) {
			BCC ^= Data[i];
		}
	}
	return char(BCC);
}

bool TBCCPacket::checkBCC(AnsiString Data)
{
	BYTE PacketBCC = Data[Data.Pos(ebetChrETX)+1];
	BYTE BCC = 0;
	for (int i = Data.Pos(ebetChrSTX); i <= Data.Pos(ebetChrETX); i++) {
		BCC ^= Data[i];
	}
	return BCC == PacketBCC;
}

TMembershipEBetInBoundPacket::TMembershipEBetInBoundPacket()
{
	Erase();
}

UnicodeString TMembershipEBetInBoundPacket::MemberName()
{
	return _MembersName;
}

UnicodeString TMembershipEBetInBoundPacket::MemberNumber()
{
	return _MembersNumber;
}

TDateTime TMembershipEBetInBoundPacket::MemberDOB()
{
	return _DOB;
}

Currency TMembershipEBetInBoundPacket::MemberPoints()
{
	return _Points;
}

void TMembershipEBetInBoundPacket::setStatus(eEBetStatus EBetStatus)
{
	_EBetStatus = EBetStatus;
}

eEBetStatus TMembershipEBetInBoundPacket::Status()
{
	return _EBetStatus;
}

void TMembershipEBetInBoundPacket::setMsg(AnsiString Message)
{
	_EBetMsg = Message;
}

void TMembershipEBetInBoundPacket::Process(AnsiString Data)
{
	if(validate(Data))
	{
		if(checkBCC(Data))
		{
			RawData = Data;
			setStatus(getStatus(RawData));
			switch(Status())
			{
				case eEbetOk :
					_Address 			= getAddress(RawData);
					_MembersName		= getMembersName(RawData);
					_MembersNumber		= getMembersNumber(RawData);
					_AccountNumber		= getAccoutNumber(RawData);
					_PacketNumber		= getPacketNumber(RawData);
					_DOB				= getDOB(RawData);
					_Points				= getPoints(RawData);
					_DiscountRate		= getDiscountRate(RawData);
				break;
				case eEbetResend :
				break;
				case eEbetAbort :
					setMsg(getMessage(RawData));
				break;
			}
		}
		else
		{
			setStatus(eEbetAbort);
			setMsg("EBet Invalid CRC");
		}
	}
	else
	{
		setStatus(eEbetAbort);
		setMsg("EBet Invalid Packet");
	}
}

UnicodeString TMembershipEBetInBoundPacket::Msg()
{
	return _EBetMsg;
}

void TMembershipEBetInBoundPacket::Erase()
{
	RawData = "";
	_Address 			= "";
	_MembersName		= "";
	_MembersNumber		= "";
	_AccountNumber		= "";
	_PacketNumber		= 0;
	_DOB					= 0;
	_Points				= 0;
	_DiscountRate		= 0;
}

bool TMembershipEBetInBoundPacket::validate(AnsiString Data)
{
	bool PacketStructureOk = true;

	if(Data.Length() == 0)
		PacketStructureOk = false;

	if(Data[ebetSTXPos] != ebetChrSTX)
		PacketStructureOk = false;

	if(Data.Pos(ebetChrETX) == -1)
		PacketStructureOk = false;

	if(Data.Pos(ebetChrSTX) > Data.Pos(ebetChrETX))
		PacketStructureOk = false;

	if(Data.Pos(ebetChrETX) != Data.Length()-1)
		PacketStructureOk = false;

	return PacketStructureOk;
}

eEBetStatus TMembershipEBetInBoundPacket::getStatus(AnsiString Data)
{
	eEBetStatus RetVal = ebetChrAbort;
	if(Data.Length() > ebetStatusPos)
	{
		switch(RawData[ebetStatusPos])
		{
			case ebetChrOk :
				RetVal = eEbetOk;
			break;
			case ebetChrPNak :
				RetVal = eEbetResend;
			break;
			case ebetChrAbort :
				RetVal = eEbetAbort;
			break;
		}
	}
	else
	{
		throw Exception("Currupt EBet Status:" + Data );
	}
	return RetVal;
}


AnsiString TMembershipEBetInBoundPacket::getAddress(AnsiString Data)
{
	AnsiString RetVal = "";

	if(Data.Length() > ebetAddrPos + ebetAddrLength)
	{
		RetVal = Data.SubString(ebetAddrPos,ebetAddrLength);
	}
	else
	{
		throw Exception("Currupt EBet Member Address :" + Data );
	}
	return RetVal;
}

AnsiString TMembershipEBetInBoundPacket::getMembersName(AnsiString Data)
{
	AnsiString RetVal = "";

	if(Data.Length() > ebetMemNamePos + ebetMemNameLength)
	{
		RetVal = Data.SubString(ebetMemNamePos,ebetMemNameLength).Trim();
	}
	else
	{
		throw Exception("Currupt EBet Member Name :" + Data );
	}
	return RetVal;
}

AnsiString TMembershipEBetInBoundPacket::getMembersNumber(AnsiString Data)
{
	AnsiString RetVal = "";

	if(Data.Length() > ebetMemNumberPos + ebetMemNumberLength)
	{
		RetVal = Data.SubString(ebetMemNumberPos,ebetMemNumberLength);
	}
	else
	{
		throw Exception("Currupt EBet Member Number :" + Data );
	}
	return RetVal;

}

int TMembershipEBetInBoundPacket::getPacketNumber(AnsiString Data)
{
	int RetVal = 0;

	if(Data.Length() > ebetPktPos + ebetPktLength)
	{
		RetVal = StrToIntDef(Data.SubString(ebetPktPos,ebetPktLength),0);
	}
	else
	{
		throw Exception("Currupt EBet Member Packet No. :" + Data );
	}
	return RetVal;
}

int TMembershipEBetInBoundPacket::getAccoutNumber(AnsiString Data)
{
	int RetVal = 0 ;

	if(Data.Length() > ebetAccountPos + ebetAccountLength)
	{
		RetVal = StrToInt(Data.SubString(ebetAccountPos,ebetAccountLength));
	}
	else
	{
		throw Exception("Currupt EBet Data Acc No. " + Data );
	}
	return RetVal;

}

TDateTime TMembershipEBetInBoundPacket::getDOB(AnsiString Data)
{
	TDateTime RetVal = 0 ;

	if(Data.Length() > ebetDOBPos + ebetDOBLength)
	{
		AnsiString DOB = Data.SubString(ebetDOBPos,ebetDOBLength);
		Word Day = StrToInt(DOB.SubString(1,2));
		Word Month = StrToInt(DOB.SubString(3,2));
		Word Year = YearOf(Now());
		RetVal = EncodeDate(Year,Month,Day);
	}
	else
	{
		throw Exception("Currupt EBet Data DOB :" + Data );
	}
	return RetVal;
}

Currency TMembershipEBetInBoundPacket::getPoints(AnsiString Data)
{
	Currency RetVal = 0 ;
	if(Data.Length() > ebetPointsPos + ebetPointsLength)
	{
		double int64Points = StrToInt(Data.SubString(ebetPointsPos,ebetPointsLength));
		RetVal = Currency(int64Points) / Currency(100);
	}
	else
	{
		throw Exception("Currupt EBet Data Points :" + Data );
	}
	return RetVal;
}

AnsiString TMembershipEBetInBoundPacket::getMessage(AnsiString Data)
{
	AnsiString RetVal = "" ;
	if(Data.Length() > ebetMsgPos && RawData.Pos(ebetChrETX) > ebetMsgPos)
	{
		RetVal = AnsiString(RawData.SubString(ebetMsgPos,RawData.Pos(ebetChrETX) - ebetMsgPos));
	}
	else
	{
		throw Exception("Currupt EBet Data Message :" + Data );
	}
	return RetVal;
}


Currency TMembershipEBetInBoundPacket::getDiscountRate(AnsiString Data)
{
	Currency RetVal = 0 ;
	if(Data.Length() > ebetDiscountRatePos + ebetDiscountLength)
	{
		double int64Points = StrToInt(Data.SubString(ebetDiscountRatePos,ebetDiscountLength));
		RetVal = Currency(int64Points) / Currency(100);
	}
	return RetVal;
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

TMembershipEBetOutBoundPacket::TMembershipEBetOutBoundPacket()
{
	TransType = ebetRequestPoints;
	TillID = 0;
	PacketNumber = 1;
}

AnsiString TMembershipEBetOutBoundPacket::BuildPacket()
{
	AnsiString Packet = "";
	Packet += ebetChrSTX;

	AnsiString Address = "000";
	Address.sprintf("%03d",TillID);
	Packet += Address;
	switch(TransType)
	{
		case ebetSendAck :
			Packet += ebetChrAck;
		break;
		case ebetPacketPing :
			Packet += ebetChrPing;
		break;
        case ebetRequestPointsMemNo :
        case ebetRedeemPointsMemNo :
        case ebetAddPointsMemNo :
        {
			Packet += ebetChrRequest;
			Packet += eBetStrRequests[TransType];
			Packet += ebetChrSeperator;

			AnsiString PacketNo = "00";
			PacketNo.sprintf("%02d",PacketNumber);
			Packet += PacketNo;
			Packet += ebetChrSeperator;

			Packet += ebetChrPointsAccount;
			Packet += ebetChrSeperator;

			AnsiString DataNumeric1 = "0000000000";
			switch(TransType)
			{
				case ebetAddPointsPurchases :
				case ebetRedeemPointsPurchases :
					DataNumeric1 = FormatFloat("0000000000",TillID);
					Packet += DataNumeric1;
				break;
				default:
					Packet += DataNumeric1;
			}
			Packet += ebetChrSeperator;

			AnsiString DataNumeric2 = FormatFloat("0000000000",Value);
			Packet += DataNumeric2;
			Packet += ebetChrSeperator;
			MemberNumber += MemberNumber.StringOfChar(' ',20 - MemberNumber.Length());
			Packet += MemberNumber.SubString(1,20);
        }
        break;
		default :
		{
			Packet += ebetChrRequest;
			Packet += eBetStrRequests[TransType];
			Packet += ebetChrSeperator;

			AnsiString PacketNo = "00";
			PacketNo.sprintf("%02d",PacketNumber);
			Packet += PacketNo;
			Packet += ebetChrSeperator;

			Packet += ebetChrPointsAccount;
			Packet += ebetChrSeperator;

			AnsiString DataNumeric1 = "0000000000";
			switch(TransType)
			{
				case ebetAddPointsPurchases :
				case ebetRedeemPointsPurchases :
					DataNumeric1 = FormatFloat("0000000000",TillID);
					Packet += DataNumeric1;
				break;
				default:
					Packet += DataNumeric1;
			}
			Packet += ebetChrSeperator;

			AnsiString DataNumeric2 = FormatFloat("0000000000",Value);
			Packet += DataNumeric2;
			Packet += ebetChrSeperator;
			CardData += CardData.StringOfChar(' ',20 - CardData.Length());
			Packet += CardData.SubString(1,20);
		}
		break;
	}

	Packet += ebetChrETX;
	Packet += CalcBCC(Packet);
	return Packet;
}

void TMembershipEBetOutBoundPacket::Init(int inTillID,EBetRequestTranactionType inTransType,int inValue,AnsiString inCardData)
{
	TillID = inTillID;
	TransType = inTransType;
	Value = inValue;
	CardData = inCardData;
    MemberNumber = "";
}

void TMembershipEBetOutBoundPacket::Init(int inTillID,EBetRequestTranactionType inTransType,int inValue,AnsiString inCardData,AnsiString inMemberNumber)
{
	TillID = inTillID;
	TransType = inTransType;
	Value = inValue;
	CardData = inCardData;
    MemberNumber = inMemberNumber;
}
