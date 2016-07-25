//---------------------------------------------------------------------------

#ifndef MessageReceiverH
#define MessageReceiverH

//---------------------------------------------------------------------------

#include <vcl.h>

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::
//   TMsgReceiver
//:::::::::::::::::::::::::::
class TMsgReceiver : public TObject
{
protected:
  HWND fMsgHandlerHWND;

public:
	__fastcall  TMsgReceiver();
	__fastcall ~TMsgReceiver();

	virtual bool processMessage( Messages::TMessage& Msg );

	__property HWND WMsgHandle = { read=fMsgHandlerHWND };

private:
	void __fastcall WndMethod( Messages::TMessage& Msg );
};

//---------------------------------------------------------------------------
#endif
