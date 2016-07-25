//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "NetMessageChefMate.h"
#include "Stream.h"

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

__fastcall TNetMessageChefMate::TNetMessageChefMate()
{
	MessageType = udp_ktichen;
	TillColour = clGreen;
	Order = new TStringList;
	RemoveOrder = false;
   OrderTimeScanned = false;
	DisplayEnabled = false;
	FinalOrder = false;
	TimeKey = 0;
   Duration = 0;
}

TNetMessageChefMate::~TNetMessageChefMate()
{
	delete Order;
}

//---------------------------------------------------------------------------
void TNetMessageChefMate::Assign(TNetMessageChefMate *RHS)
{
	std::auto_ptr<TMemoryStream> Stream(new TMemoryStream);
	RHS->Encode(Stream.get());
	Stream->Position = 0;
	Decode(Stream.get());
}

void TNetMessageChefMate::Encode(TMemoryStream *Stream)
{
	EncodeNetMessage(Stream);
	StreamWrite(Stream,Device);
	StreamWrite(Stream,User);
	StreamWrite(Stream,TillColour);
	StreamWrite(Stream,RemoveOrder);
	StreamWrite(Stream,OrderTimeScanned);
	StreamWrite(Stream,FinalOrder);
	StreamWrite(Stream,TimeKey);
	StreamWrite(Stream,OrderNumber);
	StreamWrite(Stream,ChitNumber);
	StreamWrite(Stream,Received);
	StreamWrite(Stream,RemoveOrder);
	StreamWrite(Stream,tmPrinted);
	StreamWrite(Stream,Order);
}
//---------------------------------------------------------------------------
void TNetMessageChefMate::Decode(TMemoryStream *Stream)
{
	DecodeNetMessage(Stream);
	StreamRead(Stream,Device);
	StreamRead(Stream,User);
	StreamRead(Stream,TillColour);
	StreamRead(Stream,RemoveOrder);
	StreamRead(Stream,OrderTimeScanned);
	StreamRead(Stream,FinalOrder);
	StreamRead(Stream,TimeKey);
	StreamRead(Stream,OrderNumber);
	StreamRead(Stream,ChitNumber);
	StreamRead(Stream,Received);
	StreamRead(Stream,RemoveOrder);
	StreamRead(Stream,tmPrinted);
	StreamRead(Stream,Order);
}

void TNetMessageChefMate::Action()
{
}
//---------------------------------------------------------------------------

