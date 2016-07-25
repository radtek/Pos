//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("ThemeManagerBCB5.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("..\UxTheme.pas");
USEUNIT("..\ThemeMgrDB.pas");
USEUNIT("..\ThemeSrv.pas");
USEUNIT("..\TmSchema.pas");
USEUNIT("..\ThemeMgr.pas");
USERES("..\ThemeMgr.dcr");
USEPACKAGE("Vclx50.bpi");
USEPACKAGE("Vcldb50.bpi");
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
