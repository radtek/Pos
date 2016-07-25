//---------------------------------------------------------------------------
#ifndef CronosWrapperH
#define CronosWrapperH
//---------------------------------------------------------------------------
#include "CadmusEftposLib_OCX.h"
//---------------------------------------------------------------------------
typedef void __fastcall (__closure * TCronosEFTPOSMessage)(class ChronosEFTPOS* Sender, AnsiString Text);
//---------------------------------------------------------------------------
class ChronosEFTPOS
{
public:
	ChronosEFTPOS();
	~ChronosEFTPOS();

	void Write(const AnsiString& text);

	__property TCronosEFTPOSMessage OnCronosEFTPOSMessage={ read=FOnCronosEFTPOSMessage, write=FOnCronosEFTPOSMessage };

private:
	TCronosInterface* const cronosInterface;

	void __fastcall OnMessage(System::TObject * Sender, OleVariant* psaMsg);

	TCronosEFTPOSMessage FOnCronosEFTPOSMessage;
};
//---------------------------------------------------------------------------
#endif
