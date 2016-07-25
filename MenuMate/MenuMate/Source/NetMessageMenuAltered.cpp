//---------------------------------------------------------------------------


#pragma hdrstop

#include "NetMessageMenuAltered.h"
#include "Stream.h"
#include "MM_DBCore.h"

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)

TNetMessageMenuAltered::TNetMessageMenuAltered()
{
	MessageType = udp_MenuAltered;
	ItemEnabled = false;
	AlterDB = false;
	Broadcast = false;
	MenuKey = 0;
	ItemKey = 0;
}

void TNetMessageMenuAltered::Encode(TMemoryStream *Stream)
{
	EncodeNetMessage(Stream);
	StreamWrite(Stream,MenuKey);
	StreamWrite(Stream,ItemEnabled);
	StreamWrite(Stream,ItemKey);
	StreamWrite(Stream,AlterDB);
}
//---------------------------------------------------------------------------
void TNetMessageMenuAltered::Decode(TMemoryStream *Stream)
{
	DecodeNetMessage(Stream);
	StreamRead(Stream,MenuKey);
	StreamRead(Stream,ItemEnabled);
	StreamRead(Stream,ItemKey);
	StreamRead(Stream,AlterDB);
}

void TNetMessageMenuAltered::Action()
{
	try
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		TDeviceRealTerminal::Instance().Menus->UpdateMenuAltered(DBTransaction,this);
		DBTransaction.Commit();
	}
	catch(Exception &Err)
	{
	}
}



