//---------------------------------------------------------------------------


#pragma hdrstop

#include "SecurityReference.h"

//---------------------------------------------------------------------------

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#pragma package(smart_init)

TSecurityReference::TSecurityReference()
{
	TerminalName = TDeviceRealTerminal::Instance().ID.Name;
}

void TSecurityReference::SecRefCopy(TSecurityReference *in)
{
   Event = in->Event;
   From = in->From;
	Note = in->Note;
   TerminalName = in->TerminalName;
	TimeStamp = in->TimeStamp;
   To = in->To;
   UserKey = in->UserKey;
}
