//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("VirtualTreesBCB5D.res");
USEPACKAGE("vcl50.bpi");
USEPACKAGE("ThemeManagerBCB5.bpi");
USEPACKAGE("dsnide50.bpi");
USEPACKAGE("VirtualTreesBCB5.bpi");
USEUNIT("VirtualTreesReg.pas");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
	return 1;
}
//---------------------------------------------------------------------------
