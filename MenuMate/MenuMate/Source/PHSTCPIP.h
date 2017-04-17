//---------------------------------------------------------------------------
#ifndef PHSTCPIPH
#define PHSTCPIPH
//---------------------------------------------------------------------------
#include <map>
#include <system.hpp>
#include "Money.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <memory>
#include "enumPayment.h"

typedef std::map<AnsiString,Currency> TPhoenixCategoryCurrency;

class TPhoenixBasePacket
{
public:
	AnsiString MessageIDOut; //1
	AnsiString MessageIDIn; //1
	int Length;
	int POSID;
	AnsiString AccountNumber;
	int FolderNumber;
	AnsiString virtual SerializeOut() = 0;
	void virtual SerializeIn(AnsiString) = 0;

	eTransactionResult Result;
	AnsiString ResultText;
	TPhoenixBasePacket();
};

class TPhoenixRoomCharge : public TPhoenixBasePacket
{
public:
	int Covers;
	int StaffID;
	AnsiString ReferenceNumber;
	Currency Total;
	TPhoenixCategoryCurrency Categories;
	AnsiString SerializeOut();
	void SerializeIn(AnsiString);
	TPhoenixRoomCharge();
};

class TPhoenixRoomStatusExt : public TPhoenixBasePacket
{
	char fStartOfPacket;
	char fSeperator;
public:
	AnsiString MessageID; //1
	Currency Balance;
	Currency CreditLimit;
	TStringList *Folders;

	eTransactionResult Result;

	void Clear();
	TPhoenixRoomStatusExt();
	virtual ~TPhoenixRoomStatusExt();
	AnsiString SerializeOut();
	void SerializeIn(AnsiString);
    AnsiString SiHotRoom;
};

class TPhoenixNetTCPManager
{
public:		// User declarations
	void SendAndFetch(TPhoenixBasePacket &Packet, AnsiString HostAddress, int Port);
	TPhoenixNetTCPManager();
};

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
