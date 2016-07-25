//---------------------------------------------------------------------------

#ifndef MembershipEbetDataH
#define MembershipEbetDataH

//---------------------------------------------------------------------------
#include <system.hpp>

//AnsiStrings are 1 indexed
#define ebetSTXPos 				1
#define ebetAddrPos 				2
#define ebetAddrLength  			3
#define ebetStatusPos 				5
#define ebetStatusLength 			1
#define ebetMsgPos 				8
#define ebetTypePos 				6
#define ebetTypeLength				3
#define ebetPktPos 				10
#define ebetPktLength				2
#define ebetAccountPos				13
#define ebetAccountLength	  		1
#define ebetDOBPos				15
#define ebetDOBLength		  		4
#define ebetMemNumberPos			19
#define ebetMemNumberLength			6
#define ebetPointsPos				26
#define ebetPointsLength			10
#define ebetMemNamePos				37
#define ebetMemNameLength			20
#define ebetDiscountRatePos 			58
#define ebetDiscountLength			6


#define ebetChrAck Char(0x06)
#define ebetChrPNak Char(0x01)
#define ebetChrAbort 'K'
#define ebetChrOk 'w'
#define ebetChrRequest 'w'
#define ebetChrPing 'T'
#define ebetChrSTX Char(0x02)
#define ebetChrETX Char(0x03)
#define ebetChrSeperator ','
#define ebetChrPointsAccount '1'
#define ebetChrCashAccount 'C'

#define MaxRetryCount 3
#define SleepOnException 300

enum eEBetStatus { eEbetOk=0, eEbetResend, eEbetAbort };

enum EBetRequestTranactionType { ebetRedeemPoints, ebetAddPoints,
								 ebetRefundPoints, ebetAddPointsPurchases, ebetRedeemPointsPurchases,
								 ebetRequestPoints,ebetRequestPointsMemNo, ebetRedeemPointsMemNo, ebetAddPointsMemNo,ebetSendAck,ebetPacketPing};

const char* const eBetStrRequests[] =
						{
						 "010",
						 "011",
						 "021",
						 "030",
						 "032",
						 "130",
                         "135",
                         "015",
                         "013",
						 "",
						 ""
						};


class TBCCPacket
{
public:
	char CalcBCC(AnsiString Packet);
	bool checkBCC(AnsiString Data);
};

class TMembershipEBetOutBoundPacket : TBCCPacket
{
private:
	int PacketNumber;
	int Value; // Points Value in cents.
	AnsiString CardData;
    AnsiString MemberNumber;
	EBetRequestTranactionType TransType;
public:
	int TillID;
	TMembershipEBetOutBoundPacket();
	void Init(int inTillID,EBetRequestTranactionType inTransType,int inValue,AnsiString inCardData);
	void Init(int inTillID,EBetRequestTranactionType inTransType,int inValue,AnsiString inCardData,AnsiString inMemberNumber);
	AnsiString BuildPacket();
};

//enum eEBetStatus { ebetOk, ebetResend, ebetAbort };


class TMembershipEBetInBoundPacket : TBCCPacket
{
private:
	AnsiString RawData;
	eEBetStatus _EBetStatus;
	AnsiString _EBetMsg;

	AnsiString _Address;
	AnsiString _MembersName;
	AnsiString _MembersNumber;
	AnsiString _AccountNumber;
	int 		  _PacketNumber;
	TDateTime  _DOB;
	Currency   _Points;
	Currency   _DiscountRate;

	bool validate(AnsiString Data);

	AnsiString getAddress(AnsiString Data);
	AnsiString getMembersName(AnsiString Data);
	AnsiString getMembersNumber(AnsiString Data);
	int getPacketNumber(AnsiString Data);
	int getAccoutNumber(AnsiString Data);
	eEBetStatus getStatus(AnsiString Data);
	TDateTime getDOB(AnsiString Data);
	AnsiString getMessage(AnsiString Data);
	Currency getPoints(AnsiString Data);
	Currency getDiscountRate(AnsiString Data);
public:
	void Process(AnsiString Data);
	TMembershipEBetInBoundPacket();
	void Erase();
	void setStatus(eEBetStatus EBetStatus);
	void setMsg(AnsiString Message);

	eEBetStatus Status();
	UnicodeString Msg();


	int TillID();
	int PacktNumber();
	int AccountNumber();
	UnicodeString MemberName();
	UnicodeString MemberNumber();
	TDateTime MemberDOB();
	Currency MemberPoints();
	double MemberDiscount();
};

#endif
