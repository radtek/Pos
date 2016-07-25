//---------------------------------------------------------------------------


#pragma hdrstop

#include "NetMessageTabChanged.h"
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
TNetMessageTabChanged::TNetMessageTabChanged()
{
	TabKey = 0;
	MessageType = udp_TabChanged;
}
//---------------------------------------------------------------------------
void TNetMessageTabChanged::Encode(TMemoryStream *Stream)
{
	EncodeNetMessage(Stream);
	StreamWrite(Stream,TabKey);
}

void TNetMessageTabChanged::Decode(TMemoryStream *Stream)
{
	DecodeNetMessage(Stream);
   StreamRead(Stream,TabKey);
}
//---------------------------------------------------------------------------
void TNetMessageTabChanged::Action()
{
	#ifdef MenuMate
		TDeviceRealTerminal::Instance().TriggerTabStateChanged();
	#endif
	#ifdef PalmMate
		Database::TDBTransaction DBTransaction(TDeviceRealPalms::Instance().DBControl);
		DBTransaction.StartTransaction();
		TDeviceRealPalms::Instance().CheckTabsSetMenuList(DBTransaction);
		DBTransaction.Commit();
	#endif
}

