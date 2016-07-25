//---------------------------------------------------------------------------

#ifndef bluetoothH
#define bluetoothH

#include "CPortCtl.hpp"
#include "CPort.hpp"

#include <queue>

#include "MM_DBCore.h"


class TPalm;
/*------------------------------------------------------------------------------

7E // Start Flag
00 // Device ID
01 // Datacount Hibyte
00 // Datacount lobyte
[Internal Packet Data]


------------------------------------------------------------------------------*/
#pragma pack(1)
struct TBlueToothpktHeader
{
	unsigned char StartFlag;
   char ProductVersion[6];
   unsigned short DeviceKey;
   unsigned short ProcSetDelay;
   unsigned short ProcGetDelay;
   unsigned short DataCount;
};
#pragma pack()

class TBlueToothPacket
{
public:
	TBlueToothPacket();
	~TBlueToothPacket();

	void __fastcall Clear();
	bool FormatForWrite(TMemoryStream *inData);
	TBlueToothpktHeader Header;
	TMemoryStream *Buffer;

	// Timeout are in ms.
	// New Orders timeout is per order.
	// RetrieveOrders is the total time.
	void SetNewOrderTimeout(DWORD TotalDurationMs, int OrderCount);
	DWORD GetNewOrderTimeout();

	void SetRetrieveOrdersTimeout(DWORD Duration);
	DWORD GetRetrieveOrdersTimeout();

private:
	static const int		InitNewOrderTimeout;
	static const int		InitGetOrdersTimeout;

	static const int		MinNewOrderTimeout;
	static const int		MinGetOrdersTimeout;

	static const int		RunningAverageElementCount;
	static const int		NewOrdersTimeoutScalerPercent;

	static const int		SmallOrderMaxTimeout;
	static const int		BigOrderMaxTimeout;
	static const int		BigOrderSize;

	typedef std::vector<DWORD> TQueue;

	static TQueue			NewOrderTimeoutQueue;
	static int				NewOrderQueueHeadIndex;

	static TQueue			GetOrdersTimeoutQueue;
	static int				GetOrdersQueueHeadIndex;

	void						AddTimeoutToQueue(TQueue &Queue, int &HeadIndex, DWORD Duration);
	DWORD						GetTimeoutQueueAverage(TQueue &Queue, unsigned int Min);


//	unsigned short tickCurrentSetProcDelay;
//	unsigned short tickCurrentGetProcDelay;
//	double  msCurrentGetProcDelay;
//	double msCurrentSetProcDelay;
//	unsigned short msMinGetProcDelay;
//	unsigned short msMinSetProcDelay;
};

class TBluetoothLayer
{
   void __fastcall BlueToothDataPacket(TObject *Sender, const UnicodeString Str);
	TBlueToothPacket *Packet;
	bool ReceiveData(unsigned char *ptrBuffer, int BytesToRead);
   bool RetriveBlueToothPacket();
   bool SendBlueToothPacket(TPalm *Palm);
   int TimeOutms;
   TMemoryStream *inData;
   public:
   void SetGetOrderProcDelay(unsigned short ProcGetDelay);
	void SetNewOrderTimeout(DWORD TotalDuration, int OrderCount);
	TComPort *CommPort;
   TBluetoothLayer();
   virtual ~TBluetoothLayer();
   bool Open();
   bool Close();
   void __fastcall GetStuff(TObject *Sender,int count);
	void Initialise(Database::TDBTransaction &DBTransaction);

};


//---------------------------------------------------------------------------
#endif


