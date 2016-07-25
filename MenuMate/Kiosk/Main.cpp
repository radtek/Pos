// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>
#include <mshtml.h>
#include <oleacc.h>
#include "SHDocVw_OCX.h"
#include "Main.h"

#include "LogsString.h"
#include "GlobalSettings.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdTCPClient"
#pragma link "IdTCPConnection"
#pragma resource "*.dfm"
TfrmMain *frmMain;

// ---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner), MyKiosk(new TDeviceKiosk(webDisplay))
{
}

__fastcall TfrmMain::~TfrmMain()
{

}

void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	frmMain->Tag = DESIGN_SCREEN_SIZE;
	Application->OnMessage = AppMessage;
	Application->OnIdle = IdleHandler;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::IdleHandler(TObject *Sender, bool &Done)
{
	try
	{
		TManagerLogs::Instance().Process();
	}
	catch(Exception & E)
	{
		ShowMessage("IdleHandler Error " + E.Message);
		throw;
	}
}

void __fastcall TfrmMain::FormResize(TObject *Sender)
{
	frmMain->Width = Screen->Width;
	frmMain->Height = Screen->Height;
	webDisplay->Align = alTop;
	webDisplay->Height = frmMain->Height - 10;
	Button1->Top = Screen->Height - Button1->Height - 10;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	FormResize(this);

	if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "Kiosk"))
	{
		ForceDirectories(ExtractFilePath(Application->ExeName) + "Kiosk");
	}

	if(!MyKiosk->Online())
	{
		Close();
	}
	else
	{
		MyKiosk->RefreshDisplay();
		MembershipMode1->Checked = MyKiosk->GetDisplayMode() == eDispModeFinancial;
		PointsMode1->Checked = MyKiosk->GetDisplayMode() == eDispModePoints;
		ExternalMembership1->Checked = MyKiosk->GetMembershipType() == MembershipTypeExternal;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::AppMessage(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_CHAR)
	{
		if (!(Msg.lParam & 0x1000000)) // Is the CTRL or ALT key being held down i.e is it an Extended Key.
		{
			if (!Screen->ActiveForm->ClassNameIs("TfrmTouchKeyboard"))
			{
				Handled = true;
				if (tiCardSwipe->Enabled)
				{
					tiCardSwipe->Enabled = false;
					if (!iscntrl((char)Msg.wParam))
					{
						CardSwipeString += (char)Msg.wParam;
					}
					tiCardSwipe->Enabled = true;
				}
				else
				{
					if (!iscntrl((char)Msg.wParam))
					{
						CardSwipeString = (char)Msg.wParam;
						tiCardSwipe->Enabled = true;
					}
				}
			}
		}
	}
}

void __fastcall TfrmMain::tiCardSwipeTimer(TObject *Sender)
{
	tiCardSwipe->Enabled = false;
	if (CardSwipeString != "")
	{
		PostMessage(Screen->ActiveForm->Handle, UWM_CARDSWIPE, (UINT) & CardSwipeString, NULL);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::CardSwipe(TMessage& Message)
{
	UnicodeString Data = *((UnicodeString*)Message.WParam);
	try
	{
		Database::TDBTransaction DBTransaction(MyKiosk->DBControl);
		MyKiosk->RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		MyKiosk->CardSwipe(DBTransaction, Data);
		DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Card Swipe Error" + E.Message);
	}
}

void __fastcall TfrmMain::Close1Click(TObject *Sender)
{
	Close();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::Refresh2Click(TObject *Sender)
{
	MyKiosk->RefreshDisplay();
	MyKiosk->htmlbusy = false;
	TManagerLogs::Instance().Process();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::Button1Click(TObject *Sender)
{
    PopupMenu1->Popup(0,0);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::PointsMode1Click(TObject *Sender)
{
	try
	{
		Database::TDBTransaction DBTransaction(MyKiosk->DBControl);
		MyKiosk->RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		MyKiosk->SetDisplayMode(DBTransaction, eDispModePoints);
		DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Card Swipe Error" + E.Message);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::MembershipMode1Click(TObject *Sender)
{
	try
	{
		Database::TDBTransaction DBTransaction(MyKiosk->DBControl);
		MyKiosk->RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		MyKiosk->SetDisplayMode(DBTransaction, eDispModeFinancial);
		DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Card Swipe Error" + E.Message);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::webDisplayBeforeNavigate2(TObject *ASender, const IDispatch *pDisp, OleVariant &URL, OleVariant &Flags, OleVariant &TargetFrameName, OleVariant &PostData,
	OleVariant &Headers, WordBool &Cancel)
{
	UnicodeString CommandUrl = URL;
	if (CommandUrl.Pos(":internal:") != 0)
	{
		AnsiString Command = "";
		if (CommandUrl.Pos("[") != 0 && CommandUrl.Pos("]") != 0)
		{
			Command = CommandUrl.SubString(CommandUrl.Pos("[") + 1, CommandUrl.Pos("]") - CommandUrl.Pos("[") - 1);
		}

		if (Command.LowerCase() == "print")
		{
			AnsiString strContactKey = 0;
			strContactKey = CommandUrl.SubString(CommandUrl.Pos("{") + 1, CommandUrl.Pos("}") - CommandUrl.Pos("{") - 1);
			int ContactKey = StrToIntDef(strContactKey, -1);
			if (ContactKey != -1)
			{
				Database::TDBTransaction DBTransaction(MyKiosk->DBControl);
				MyKiosk->RegisterTransaction(DBTransaction);
				DBTransaction.StartTransaction();
				MyKiosk->PrintMemberDocket(DBTransaction, ContactKey);
				DBTransaction.Commit();
			}
		}
        Cancel = -1;
	}

}
// ---------------------------------------------------------------------------
void __fastcall TfrmMain::ShowEntrances1Click(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(MyKiosk->DBControl);
    MyKiosk->RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();
    MyKiosk->DisplayEntrances(DBTransaction);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::ERSClubMembership1Click(TObject *Sender)
{
 	try
	{
		Database::TDBTransaction DBTransaction(MyKiosk->DBControl);
		MyKiosk->RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		MyKiosk->SetMembershipMode(DBTransaction, MembershipTypeERS);
		DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Set Membership Mode Error" + E.Message);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ExternalMembership1Click(TObject *Sender)
{
	try
	{
		Database::TDBTransaction DBTransaction(MyKiosk->DBControl);
		MyKiosk->RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		MyKiosk->SetMembershipMode(DBTransaction, MembershipTypeExternal);
		DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Set Membership Mode Error" + E.Message);
	}
}
//---------------------------------------------------------------------------

