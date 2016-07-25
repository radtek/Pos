//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Main.cpp", MainForm);
USEFORM("ExtractFiles.cpp", Extract);
USEFORM("Message.cpp", MsgForm);
USEFORM("SFX.cpp", MakeSFX);
USEFORM("Rename.cpp", RenForm);
USEFORM("AddFiles.cpp", AddFile);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TExtract), &Extract);
		Application->CreateForm(__classid(TMsgForm), &MsgForm);
		Application->CreateForm(__classid(TMakeSFX), &MakeSFX);
		Application->CreateForm(__classid(TRenForm), &RenForm);
		Application->CreateForm(__classid(TAddFile), &AddFile);
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
