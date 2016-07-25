//---------------------------------------------------------------------------


#pragma hdrstop

#include "NetMessageMenuChanged.h"
#include "Stream.h"

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#pragma package(smart_init)
//---------------------------------------------------------------------------
TNetMessageMenuChanged::TNetMessageMenuChanged()
{
	MessageType = udp_MenuChange;
	Broadcast = false;
	CompareToDataBase = false;
}

//---------------------------------------------------------------------------
void TNetMessageMenuChanged::Encode(TMemoryStream *Stream)
{
   unsigned char nully = NULL;
	EncodeNetMessage(Stream);
	StreamWrite(Stream,CompareToDataBase);
   int size = Menu_Names.size();
   StreamWrite(Stream,size);
   for ( std::map<UnicodeString,eMenuCommand>::const_iterator Names = Menu_Names.begin();
         Names != Menu_Names.end();
         Names++)
   {
      UnicodeString Menu = Names->first;
	   StreamWrite(Stream,Menu);
      int MenuCommand = Names->second;
	   StreamWrite(Stream,MenuCommand);
   }
}
//---------------------------------------------------------------------------
void TNetMessageMenuChanged::Decode(TMemoryStream *Stream)
{
   DecodeNetMessage(Stream);
   int size = 0;
	StreamRead(Stream,CompareToDataBase);
   StreamRead(Stream,size);
   for (int i = 0; i < size ; i++)
   {
      UnicodeString MenuName = "";
		StreamRead(Stream,MenuName);
      int MenuCommand = eMenuAddReplace;
	   StreamRead(Stream,MenuCommand);
      Menu_Names[MenuName] = (eMenuCommand)MenuCommand;
   }
}
//---------------------------------------------------------------------------

void TNetMessageMenuChanged::Action()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	if(this->CompareToDataBase)
	{
		TDeviceRealTerminal::Instance().Menus->MenuAddedRemoved(DBTransaction,this);
	}
	else
	{
		TDeviceRealTerminal::Instance().Menus->MenuChanged(DBTransaction,this);
	}
	// The new Menus will be swapped in when the POS is ready for them.
	// See Select Dish tmPosRefresh Timer.
	DBTransaction.Commit();
}




