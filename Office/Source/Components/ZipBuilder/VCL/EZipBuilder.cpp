//---------------------------------------------------------------------------
// Unit for exception handling in ZipBuilder
// created version 1.76 22 May 2004
#include <vcl.h>
#pragma hdrstop

#include "EZipBuilder.h"
#include "ZipMsg.h"
#include "ZipBuildDefs.h"
#include "ZipStrs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)     

/* EZipBuilder::EZipBuilder----------------------------------------------------------------------------------------------------------------------
1.76 new     
*/
__fastcall EZipBuilder::EZipBuilder(const AnsiString Msg)
                      : Sysutils::Exception(Msg)
{
  FDisplayMsg = true;
  FResIdent = DS_UnknownError;
}
/* EZipBuilder::EZipBuilder------------------------------------------------------------------------
1.77 new
*/
__fastcall EZipBuilder::EZipBuilder(const AnsiString Msg, const bool Display)
                      : Sysutils::Exception(Msg)
{
  FDisplayMsg = Display;
//  FResIdent = DS_UnknownError;
}        
        
/* EZipBuilder::EZipBuilder----------------------------------------------------------------------------------------------------------------------
1.76 convert strings in constructor. The default exception constructor used.
*/
__fastcall EZipBuilder::EZipBuilder(int Ident, const TVarRec* Args, const int Args_Size)
                      : Sysutils::Exception(ZipLoadStr(Ident))
{
	if(Message.IsEmpty())
    Message = Format("id = %d %s", ARRAYOFCONST((Ident, RESOURCE_ERROR1)));
  else Message = Format(Message, Args, Args_Size);
  FResIdent = Ident;
  FDisplayMsg = true;
}

/* EZipBuilder::EZipBuilder----------------------------------------------------------------------------------------------------------------------
1.76 convert strings in constructor. The default exception constructor used.
*/
__fastcall EZipBuilder::EZipBuilder(int Ident, bool Display)
                      : Sysutils::Exception(ZipLoadStr(Ident))
{
	if(Message.IsEmpty())
    Message = Format("id = %d %s", ARRAYOFCONST((Ident, RESOURCE_ERROR1)));
  FResIdent = Ident;
  FDisplayMsg = Display;
}

/* EZipBuilder::EZipBuilder---------------------------------------------------------------------------
1.76 convert strings in constructor. The default exception constructor used.
*/
__fastcall EZipBuilder::EZipBuilder(int Ident, const int DiskNo)
                      : Sysutils::Exception(ZipLoadStr(Ident))
{
	if(Message.IsEmpty())
    Message = Format("id = %d [disk = %d]%s", ARRAYOFCONST((Ident, DiskNo, RESOURCE_ERROR1)));
  else
    Message = Format(Message, ARRAYOFCONST((DiskNo)));
  FResIdent = Ident;
  FDisplayMsg = true;
}
/* EZipBuilder::EZipBuilder---------------------------------------------------------------------------
1.76 convert strings in constructor. The default exception constructor used.
*/
__fastcall EZipBuilder::EZipBuilder(int Ident, const String Drive)
                      : Sysutils::Exception(ZipLoadStr(Ident))
{
	if(Message.IsEmpty())
    Message = Format("id = %d [drive = %s]%s", ARRAYOFCONST((Ident, Drive, RESOURCE_ERROR1)));
  else
    Message = Format(Message, ARRAYOFCONST((Drive)));
  FDisplayMsg = true;
  FResIdent = Ident;
}
/* EZipBuilder::EZipBuilder---------------------------------------------------------------------------
1.76 convert strings in constructor. The default exception constructor used.
*/
__fastcall EZipBuilder::EZipBuilder(int Ident, const String File1, const String File2)
                      : Sysutils::Exception(ZipLoadStr(Ident))
{
	if(Message.IsEmpty())
    Message = Format("id = %d [files = %s, %s]%s",
    									ARRAYOFCONST((Ident, File1, File2, RESOURCE_ERROR1)));
  else
    Message = Format(Message, ARRAYOFCONST((File1, File2)));
  FDisplayMsg = true;
  FResIdent = Ident;
}

// EZipAbort::EZipAbort---------------------------------------------------------
// new 1.73 (17 May 2003)
// exception handler for Abort exceptions
__fastcall EZipAbort::EZipAbort(void) : EZipBuilder(DS_Canceled, true)
{
}
// EZipAbort::EZipAbort
