//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("VirtualTreesBCB5.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("..\VTHeaderPopup.pas");
USEPACKAGE("ThemeManagerBCB5.bpi");
USEUNIT("..\VirtualTrees.pas");
USERES("..\VirtualTrees.dcr");
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
