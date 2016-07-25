//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("main.cpp", frmMain);
USEFORM("..\..\MenuMate\Source\Security\Login.cpp", frmLogin);
USEFORM("..\..\Touch Utils\MMMessageBox.cpp", frmMessageBox);
USEFORM("..\..\ChefMate\Source\SystemConfigLite.cpp", frmSystemConfig);
USEFORM("..\..\Touch Utils\MMTouchNumpad.cpp", frmTouchNumpad);
USEFORM("..\..\Touch Utils\MMTouchKeyboard.cpp", frmTouchKeyboard);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "ImptMate";
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TfrmLogin), &frmLogin);
		Application->CreateForm(__classid(TfrmMessageBox), &frmMessageBox);
		Application->CreateForm(__classid(TfrmSystemConfig), &frmSystemConfig);
		Application->CreateForm(__classid(TfrmTouchNumpad), &frmTouchNumpad);
		Application->CreateForm(__classid(TfrmTouchKeyboard), &frmTouchKeyboard);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

