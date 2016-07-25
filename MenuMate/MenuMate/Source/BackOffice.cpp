//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("BackOffice.res");
USEFORM("BackOffice\Tree.cpp", frmTree);
USEUNIT("BackOffice\message.cpp");
USEUNIT("BackOffice\RTable.cpp");
USEUNIT("BackOffice\Seat.cpp");
USEFORM("BackOffice\SeatInfo.cpp", frmGetSeat);
USEFORM("BackOffice\Servery.cpp", frmServery);
USEFORM("BackOffice\Setup.cpp", frmSetup);
USEFORM("BackOffice\TableOpt.cpp", frmTableOpt);
USEFORM("BackOffice\Analysis.cpp", frmAnalysis);
USEFORM("BackOffice\Main.cpp", frmMain);
USEFORM("BackOffice\data.cpp", frmData);
USEFORM("Register2.cpp", frmRegister2);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
      Application->Initialize();
      Application->CreateForm(__classid(TfrmMain), &frmMain);
      Application->CreateForm(__classid(TfrmTree), &frmTree);
      Application->CreateForm(__classid(TfrmGetSeat), &frmGetSeat);
      Application->CreateForm(__classid(TfrmServery), &frmServery);
      Application->CreateForm(__classid(TfrmSetup), &frmSetup);
      Application->CreateForm(__classid(TfrmTableOpt), &frmTableOpt);
      Application->CreateForm(__classid(TfrmAnalysis), &frmAnalysis);
      Application->CreateForm(__classid(TfrmData), &frmData);
      Application->CreateForm(__classid(TfrmRegister2), &frmRegister2);
      Application->Run();
   }
   catch (Exception &exception)
   {
      Application->ShowException(&exception);
   }
   return 0;
}
//---------------------------------------------------------------------------
