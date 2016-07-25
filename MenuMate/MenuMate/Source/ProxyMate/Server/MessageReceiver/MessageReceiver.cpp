//---------------------------------------------------------------------------

#pragma hdrstop

#include "MessageReceiver.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TMsgReceiver::TMsgReceiver() : TObject()
{
	fMsgHandlerHWND = AllocateHWnd( WndMethod );
}
//...........................................................................

__fastcall TMsgReceiver::~TMsgReceiver()
{
	DeallocateHWnd( fMsgHandlerHWND );
}
//...........................................................................

void __fastcall TMsgReceiver::WndMethod( Messages::TMessage& Msg )
{
	if( !processMessage( Msg ) )
	{
		Msg.Result = DefWindowProc( fMsgHandlerHWND, Msg.Msg, Msg.WParam, Msg.LParam );
	}
	else
	{
		Msg.Result = 0;
	}
}
//...........................................................................

bool TMsgReceiver::processMessage( Messages::TMessage& Msg )
{
	return false;
}
//...........................................................................

