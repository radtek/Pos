//---------------------------------------------------------------------------


#pragma hdrstop

#include "ErrorPrinter.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
__fastcall EErrorPrinter::EErrorPrinter(int inErrNum)
{
   ErrNum = inErrNum;
   TManagerLogs::Instance().Add(__FUNC__,ERRORLOG, "EErrorPrinter.");
}

__fastcall EErrorPrinter::~EErrorPrinter()
{
}
//---------------------------------------------------------------------------
