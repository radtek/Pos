//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("ZBldr179C4.res");
USEPACKAGE("vcl40.bpi");
USERES("..\ZipBuilder.dcr");
USEUNIT("..\EZipBuilder.cpp");
USEUNIT("..\ZBCompat.cpp");
USEUNIT("..\ZBCore.cpp");
USEUNIT("..\ZBDBind.cpp");
USEUNIT("..\ZBDlg.cpp");
USEUNIT("..\ZBHash.cpp");
USEUNIT("..\ZBMsgStr.cpp");
USEUNIT("..\ZBSBind.cpp");
USEUNIT("..\ZBWrkr.cpp");
USEUNIT("..\ZipBuilder.cpp");
USEUNIT("..\ZipExtrLZ77.cpp");
USEUNIT("..\ZipIntSFX.cpp");
USEUNIT("..\ZipProg.cpp");
USEUNIT("..\ZipUtils.cpp");
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
