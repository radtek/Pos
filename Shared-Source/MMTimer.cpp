//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMTimer.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::
// PUBLIC
//::::::::::::::::::::::::::
__fastcall TMMTimer::TMMTimer(Classes::TComponent* aOwner) : TTimer( aOwner )
{
    Enabled  = true;
    Interval = 1000;
    FWindowHandle = AllocateHWnd( WndMethod );
}
//---------------------------------------------------------------------------

__fastcall TMMTimer::~TMMTimer(void)
{
    Enabled  = false;
    DeallocateHWnd( FWindowHandle );
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::
// PROTECTED
//::::::::::::::::::::::::::
void __fastcall TMMTimer::WndMethod( Messages::TMessage& Msg )
{
	if( !processMessage( Msg ) )
	{
		Msg.Result = DefWindowProc( FWindowHandle, Msg.Msg, Msg.WParam, Msg.LParam );
	}
	else
	{
		Msg.Result = 0;
	}
}
//...........................................................................
bool TMMTimer::processMessage( TMessage& Msg )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::::

    if( Msg.Msg == WM_TIMER )
    {
        Timer();

        result = true;
    }

	//:::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

