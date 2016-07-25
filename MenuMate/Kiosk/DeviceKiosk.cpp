// ---------------------------------------------------------------------------
#pragma hdrstop

#include "DeviceKiosk.h"
#include "SystemConfig.h"
#include "MMMessageBox.h"
#include "ManagerLocations.h"
#include "MMRegistry.h"
#include "Printout.h"
#include "GlobalSettings.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

TDeviceKiosk::TDeviceKiosk(TWebBrowser *inwebDisplay) : Timer(new TTimer(NULL)), ManagerMembership(new TManagerMembership(DBControl, Modules)), Logging(new TManagerLogs())
{
	htmlbusy = false;
	Enabled = false;
	UserDisplayed = false;
	Timer->Enabled = false;
	Timer->Interval = 5000;
	Timer->OnTimer = OnTimerTick;
	EventTimerLastFired = ::GetTickCount();
	EventTimer.RegisterForEvent(OnRefreshDisplay);
	EventTimer.RegisterForEvent(OnRefreshUsers);

	Display = inwebDisplay;

	UnicodeString DBPath = "";
	RegistryRead(MMBaseKey "Database", "DatabasePath", DBPath);
	UnicodeString ServerName = "";
	RegistryRead(MMBaseKey "Database", "InterbaseIP", ServerName);

	TGlobalSettings::Instance().MembershipDatabasePath = "";
	RegistryRead(MMBaseKey "Database", "MembershipDatabasePath", TGlobalSettings::Instance().MembershipDatabasePath);

	Printer = "";
	RegistryRead(MMBaseKey, "Printer", Printer);
	if (Printer == "")
	{
		Printer = "Receipt";
	}

	int MembershipType = MembershipTypeExternal;
	RegistryRead(MMBaseKey "Database", "KioskMembershipType", MembershipType);
	TGlobalSettings::Instance().MembershipType = MembershipType;

	TGlobalSettings::Instance().InterbaseIP = ServerName;
	TGlobalSettings::Instance().DatabasePath = DBPath;
	Mode = eDispModePoints;

	Modules.Status[eSmartCardSystem]["Registered"] = false;
	Modules.Status[eSmartCardSystem]["Enabled"] = false;
	Modules.Status[eRegMembers]["Registered"] = true;
}

__fastcall TDeviceKiosk::~TDeviceKiosk()
{
	EventTimer.DeregisterForEvent(OnRefreshDisplay);
	EventTimer.DeregisterForEvent(OnRefreshUsers);
}

void TDeviceKiosk::Initialise(Database::TDBTransaction &DBTransaction)
{
	Logging->Initialise(ID.Name);
	TGlobalSettings::Instance().MembershipType = TManagerVariable::Instance().GetInt(DBTransaction, vmKioskMembershipMode, (int)MembershipTypeERS);
	TManagerVariable::Instance().InitialisePOSVars(DBTransaction);
	Mode = TManagerVariable::Instance().GetInt(DBTransaction, vmKioskDisplayMode, (int)eDispModePoints);
	Timer->Enabled = true;
}

bool TDeviceKiosk::Online()
{
	bool retVal = false;
	try
	{
		if (OpenMenuMateDB())
		{
			Database::TDBTransaction DBBootTransaction(DBControl);
			DBBootTransaction.StartTransaction();
			// VM->Initialise(DBBootTransaction);
			CreateDevice(DBBootTransaction);
			CreateProfile(DBBootTransaction);
			Initialise(DBBootTransaction);
			DBBootTransaction.Commit();

			Database::TDBTransaction DBTransaction(DBControl);
			RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			ManagerMembership->Initialise(DBTransaction);
			DBTransaction.Commit();
			if(OpenMembershipDB(false))
			{
				retVal = true;
				Enabled = true;
			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
    return retVal;
}

bool TDeviceKiosk::OpenMenuMateDB()
{
	std::auto_ptr<TfrmSystemConfig>frmSystemConfig(TfrmSystemConfig::Create(Screen->ActiveForm));
	frmSystemConfig->EnableMenuMate();

	TGlobalSettings::Instance().DatabasePath = "";
	RegistryRead(MMBaseKey "Database", "DatabasePath", TGlobalSettings::Instance().DatabasePath);

	TGlobalSettings::Instance().InterbaseIP = "";
	RegistryRead(MMBaseKey "Database", "InterbaseIP", TGlobalSettings::Instance().InterbaseIP);

	bool RetVal = false;
	bool Abort = false;
	do
	{
		try
		{
			if (TGlobalSettings::Instance().InterbaseIP != "" && TGlobalSettings::Instance().DatabasePath != "")
			{
				DBControl.Disconnect();
				DBControl.Init(Database::TDBSettings(TGlobalSettings::Instance().InterbaseIP, TGlobalSettings::Instance().DatabasePath, false));
				DBControl.Connect();
				Database::TDBUtilities Utils(DBControl);
				// Utils.SetForcedWrites();
				RetVal = true;
			}
			Abort = false;
		}
		catch(Exception & Err)
		{
			MessageBox("Unable to Connect to MenuMate Database.\r" + Err.Message, "Error", MB_OK + MB_ICONERROR);
			CloseDatabases();
			Abort = false;
			RetVal = false;
		}

		if (RetVal == false)
		{
			// Grey out all the other settings.
			if (frmSystemConfig->ShowModal() == mrOk)
			{
				Abort = false;
			}
			else if (frmSystemConfig->ModalResult == mrAbort)
			{
				Abort = true;
			}
		}
	}
	while (RetVal == false && Abort == false);

	return RetVal;
}

void TDeviceKiosk::CloseDatabases()
{
	DBControl.Disconnect();
}

bool TDeviceKiosk::OpenMembershipDB(bool Reconfigure)
{
	bool RetVal = false;
	try
	{
		if (Modules.Status[eRegMembers]["Registered"])
		{
			TGlobalSettings::Instance().MembershipDatabasePath = "";
			RegistryRead(MMBaseKey "Database", "MembershipDatabasePath", TGlobalSettings::Instance().MembershipDatabasePath);
			TGlobalSettings::Instance().MembershipDatabaseIP = "";
			RegistryRead(MMBaseKey "Database", "MembershipInterbaseIP", TGlobalSettings::Instance().MembershipDatabaseIP);

			Database::TDBTransaction DBMembershipTransaction(DBControl);
			DBMembershipTransaction.StartTransaction();
			RetVal = ManagerMembership->OpenMembershipDB(DBMembershipTransaction, Reconfigure);
			DBMembershipTransaction.Commit();
			if (RetVal)
			{
				Database::TDBTransaction Trans(DBControl);
				RegisterTransaction(Trans);
				Trans.StartTransaction();
				RetVal = ManagerMembership->CheckVersionNumber(Trans);
				TManagerVariable::Instance().SetDeviceStr(Trans, vmMembershipDatabaseIP, TGlobalSettings::Instance().MembershipDatabaseIP);
				TManagerVariable::Instance().SetDeviceStr(Trans, vmMembershipDatabasePath, TGlobalSettings::Instance().MembershipDatabasePath);
				Trans.Commit();
			}
		}
		else
		{
			RetVal = true;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
	return RetVal;
}

void TDeviceKiosk::CardSwipe(Database::TDBTransaction &DBTransaction, UnicodeString Card)
{
	try
	{
		UnicodeString index = ExtractFilePath(Application->ExeName) + "\\Kiosk\\Index.html";
		UnicodeString FinancialIndex = ExtractFilePath(Application->ExeName) + "\\Kiosk\\indexFinancial.html";

		if (Mode == eDispModeFinancial && FileExists(FinancialIndex))
		{
			index = FinancialIndex;
		}

		if (Enabled && FileExists(index))
		{
			std::auto_ptr<TStringList>Report(new TStringList);
			Report->LoadFromFile(index);

			std::map<UnicodeString, UnicodeString>Data;

			TMMContactInfo Info;
			Info.CardStr = Card;
			TLoginSuccess LoginSuccess = ManagerMembership->FindMember(DBTransaction, Info);

			GetKioskDisplayMsg(DBTransaction, Info, Data);

			Data["%GREETING%"] = "Sorry <br> ";
			if (LoginSuccess == lsAccountBlocked)
			{
				if (Mode == eDispModeFinancial)
				{
					Data["%MSG%"] = "Sorry you are no longer a Member.";
					Data["%NAME%"] = Info.Name;
					Data["%NUMBER%"] = Info.MembershipNumber;
					Data["%CONTACT_KEY%"] = Info.ContactKey;
				}
				else
				{
					Data["%MSG%"] = "You are not a Financial Member. <br> Please pay at the Office";
				}
			}
			else if (LoginSuccess == lsUserNotFound)
			{
				Data["%NAME%"] = "Card not found!";
			}
			else if (LoginSuccess == lsAccepted)
			{
				if (Info.Valid())
				{
					bool PrintDockets = false;
					Data["%GREETING%"] = "Welcome";
					Data["%NAME%"] = Info.Name;
					Data["%NUMBER%"] = Info.MembershipNumber;
					Data["%CONTACT_KEY%"] = Info.ContactKey;
					if (Mode == eDispModeFinancial)
					{
						Data["%EXPIRES%"] = "Membership Expires on " + Info.Expires.FormatString("DD/MM/YYYY");
						Data["%POINTS%"] = "";
					}
					else
					{
						Data["%EXPIRES%"] = "";
						Data["%POINTS%"] = "Points : " + FormatFloat("0.00", Info.Points.getPointsBalance(pasDatabase));
					}

					if (ManagerMembership->MembershipSystem->ValidateVisit(Info))
					{
						Info.TotalVisits += 1;
						Info.RewardVisits += 1;
						if (Info.RewardVisits > 100)
						{
							Info.RewardVisits = 1;
						}
						ManagerMembership->MembershipSystem->SetVisitCount(DBTransaction, Info);
						PrintDockets = true;
						Data["%VISITS%"] = "Visit Number : " + IntToStr(Info.RewardVisits);
						GetKioskDisplayMsg(DBTransaction, Info, Data);
					}
					else
					{
						GetKioskDisplayMsg(DBTransaction, Info, Data);
						Data["%MSG%"] = "You Have Already Visited Today :" + IntToStr(Info.RewardVisits);
						Data["%MSG1%"] = "";
					}

					if (PrintDockets && Data["DOCKET_VISIT_PRINT"] == "T")
					{
						UnicodeString index = ExtractFilePath(Application->ExeName) + "\\Kiosk\\DocketVisit.txt";
						if (!FileExists(index))
						{
							index = ExtractFilePath(Application->ExeName) + "\\Kiosk\\Docket.txt";
						}

						if (FileExists(index))
						{
							std::auto_ptr<TStringList>MyDocket(new TStringList);
							MyDocket->LoadFromFile(index);
							MyDocket->Text = StringReplace(MyDocket->Text, "%TITLE%", Data["%TITLE%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%DOCKET_HEADER%", Data["%VISIT_HEADER%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%NAME%", Data["%NAME%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%NUMBER%", Data["%NUMBER%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%DATETIME%", Now().FormatString("dd/mm/yyyy hh:nn:ss am/pm").t_str(), TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%GREETING%", Data["%GREETING%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%DETIALS%", Data["%VISITS%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%REWARD_MESSAGE%", "", TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%MSG%", Data["%MSG%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%MSG1%", Data["%MSG1%"], TReplaceFlags() << rfReplaceAll);
							PrintKioskDocket(MyDocket.get());
						}
					}

					if (PrintDockets && Data["DOCKET_BIRTHDAY_PRINT"] == "T")
					{
						UnicodeString index = ExtractFilePath(Application->ExeName) + "\\Kiosk\\DocketBirthday.txt";
						if (!FileExists(index))
						{
							index = ExtractFilePath(Application->ExeName) + "\\Kiosk\\Docket.txt";
						}

						if (FileExists(index))
						{
							std::auto_ptr<TStringList>MyDocket(new TStringList);
							MyDocket->LoadFromFile(index);
							MyDocket->Text = StringReplace(MyDocket->Text, "%TITLE%", Data["%TITLE%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%DOCKET_HEADER%", Data["%BIRTHDAY_HEADER%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%NAME%", Data["%NAME%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%NUMBER%", Data["%NUMBER%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%DATETIME%", Now().FormatString("dd/mm/yyyy hh:nn:ss am/pm").t_str(), TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%GREETING%", Data["%GREETING%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%DETIALS%", Data["%VISITS%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%REWARD_MESSAGE%", Data["%BIRTHDAY_TEXT%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%MSG%", "", TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%MSG1%", "", TReplaceFlags() << rfReplaceAll);
							PrintKioskDocket(MyDocket.get());
						}
					}

					if (PrintDockets && Data["DOCKET_LUCKYDRAW_PRINT"] == "T")
					{
						UnicodeString index = ExtractFilePath(Application->ExeName) + "\\Kiosk\\DocketDraw.txt";
						if (!FileExists(index))
						{
							index = ExtractFilePath(Application->ExeName) + "\\Kiosk\\Docket.txt";
						}
						if (FileExists(index))
						{
							std::auto_ptr<TStringList>MyDocket(new TStringList);
							MyDocket->LoadFromFile(index);
							MyDocket->Text = StringReplace(MyDocket->Text, "%TITLE%", Data["%TITLE%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%DOCKET_HEADER%", Data["%DRAW_HEADER%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%NAME%", Data["%NAME%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%NUMBER%", Data["%NUMBER%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%DATETIME%", Now().FormatString("dd/mm/yyyy hh:nn:ss am/pm").t_str(), TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%GREETING%", Data["%GREETING%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%DETIALS%", Data["%VISITS%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%REWARD_MESSAGE%", Data["%DRAW_TEXT%"], TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%MSG%", "", TReplaceFlags() << rfReplaceAll);
							MyDocket->Text = StringReplace(MyDocket->Text, "%MSG1%", "", TReplaceFlags() << rfReplaceAll);
							PrintKioskDocket(MyDocket.get());
						}
					}

				}
			}
			else
			{
				Data["%MSG%"] = "Unable to Log you in. <br> Please see office";
			}

			Report->Text = StringReplace(Report->Text, "%TITLE%", Data["%TITLE%"], TReplaceFlags() << rfReplaceAll);
			Report->Text = StringReplace(Report->Text, "%GREETING%", Data["%GREETING%"], TReplaceFlags() << rfReplaceAll);
			Report->Text = StringReplace(Report->Text, "%NAME%", Data["%NAME%"] + " (" + Data["%NUMBER%"] + ")", TReplaceFlags() << rfReplaceAll);
			Report->Text = StringReplace(Report->Text, "%VISITS%", Data["%VISITS%"], TReplaceFlags() << rfReplaceAll);
			Report->Text = StringReplace(Report->Text, "%MSG%", Data["%MSG%"], TReplaceFlags() << rfReplaceAll);
			Report->Text = StringReplace(Report->Text, "%MSG1%", Data["%MSG1%"], TReplaceFlags() << rfReplaceAll);
			Report->Text = StringReplace(Report->Text, "%CONTACT_KEY%", Data["%CONTACT_KEY%"], TReplaceFlags() << rfReplaceAll);

			if (Mode == eDispModeFinancial)
			{
				Report->Text = StringReplace(Report->Text, "%DETAILS%", Data["%EXPIRES%"], TReplaceFlags() << rfReplaceAll);
			}
			else
			{
				Report->Text = StringReplace(Report->Text, "%DETAILS%", Data["%POINTS%"], TReplaceFlags() << rfReplaceAll);
			}
			Report->Text = StringReplace(Report->Text, "%DIR%", ExtractFilePath(Application->ExeName), TReplaceFlags() << rfReplaceAll);
			DisplayLastUpdated = ::GetTickCount();
			UserDisplayed = true;
			Navigate(Report.get());
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

void TDeviceKiosk::RegisterTransaction(Database::TDBTransaction & DBTransaction)
{
	ManagerMembership->RegisterTransaction(DBTransaction);
}

void __fastcall TDeviceKiosk::OnTimerTick(TObject * Sender)
{
	if (EventTimerLastFired /* + (60 * 1000) */ < ::GetTickCount())
	{
		EventTimer.Occured();
		EventTimerLastFired = ::GetTickCount();
	}
}

void TDeviceKiosk::OnRefreshDisplay(TSystemEvents * Sender)
{
	if (DisplayLastUpdated + (60 * 1000) < ::GetTickCount())
	{
		RefreshDisplay();
		DisplayLastUpdated = ::GetTickCount();
	}
}

void TDeviceKiosk::OnRefreshUsers(TSystemEvents * Sender)
{
	if (UserDisplayed && DisplayLastUpdated + (10 * 1000) < ::GetTickCount())
	{
		UserDisplayed = false;
		RefreshDisplay();
		DisplayLastUpdated = ::GetTickCount();
	}
}

void TDeviceKiosk::CheckLocation(Database::TDBTransaction & DBTransaction)
{
	TManagerLocations ManagerLocations;
	UnicodeString Location = "Kiosk";
	if (ID.Location == "")
	{
		int LocationKey = ManagerLocations.FindLocation(DBTransaction, Location);
		if (LocationKey == 0)
		{
			ManagerLocations.SetLocation(DBTransaction, LocationKey, Location);
		}
		ID.Location = "Kiosk";
		ID.LocationKey = LocationKey;
	}
}

void TDeviceKiosk::GetKioskDisplayMsg(Database::TDBTransaction & DBTransaction, TMMContactInfo & Member, std::map<UnicodeString, UnicodeString>&Data)
{
	if (ManagerMembership->MembershipSystem.get() != NULL)
	{
		ManagerMembership->MembershipSystem->GetKioskDisplayMsg(DBTransaction, Member, Data);
	}
}

void __fastcall TDeviceKiosk::RefreshDisplay()
{
	UnicodeString index = ExtractFilePath(Application->ExeName) + "\\Kiosk\\Index.html";

	if (Enabled && FileExists(index))
	{
		std::auto_ptr<TStringList>Report(new TStringList);
		Report->LoadFromFile(index);

		std::map<UnicodeString, UnicodeString>Data;
		TMMContactInfo Info;
		Data["%GREETING%"] = "Please Swipe Your Card";

		Database::TDBTransaction DBTransaction(DBControl);
		RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		GetKioskDisplayMsg(DBTransaction, Info, Data);
		DBTransaction.Commit();

		Report->Text = StringReplace(Report->Text, "%TITLE%", Data["%TITLE%"], TReplaceFlags() << rfReplaceAll);
		Report->Text = StringReplace(Report->Text, "%GREETING%", Data["%GREETING%"], TReplaceFlags() << rfReplaceAll);
		Report->Text = StringReplace(Report->Text, "%NAME%", Data["%NAME%"], TReplaceFlags() << rfReplaceAll);
		Report->Text = StringReplace(Report->Text, "%VISITS%", Data["%VISITS%"], TReplaceFlags() << rfReplaceAll);
		Report->Text = StringReplace(Report->Text, "%MSG%", Data["%MSG%"], TReplaceFlags() << rfReplaceAll);
		Report->Text = StringReplace(Report->Text, "%MSG1%", Data["%MSG1%"], TReplaceFlags() << rfReplaceAll);
		Report->Text = StringReplace(Report->Text, "%DETAILS%", Data["%DETAILS%"], TReplaceFlags() << rfReplaceAll);
		Report->Text = StringReplace(Report->Text, "%DIR%", ExtractFilePath(Application->ExeName), TReplaceFlags() << rfReplaceAll);

		Navigate(Report.get());
	}
}

// ---------------------------------------------------------------------------
void TDeviceKiosk::Navigate(TStringList * Html)
{
	try
	{
		if (Enabled && htmlbusy == false)
		{
			htmlbusy = true;
			try
			{
				std::auto_ptr<TMemoryStream>StreamRpt(new TMemoryStream);
				Html->SaveToStream(StreamRpt.get());
				IPersistStreamInit *psi;
				StreamRpt->Seek(0, 0);
				TStreamAdapter *sa = new TStreamAdapter(StreamRpt.get(), soReference);
				_di_IDispatch doc = Display->Document;
				if (doc == NULL)
				{
					OleVariant url = "about:blank";
					Display->Navigate2(url);
					while (doc == NULL)
					{
						Application->ProcessMessages();
						doc = Display->Document;
					}
				}

				TComInterface<IHTMLDocument2>HTMLDocument;
				TComInterface<IHTMLWindow2>parentWindow;

				if (SUCCEEDED(Display->Document->QueryInterface(IID_IHTMLDocument2, (LPVOID*) & HTMLDocument)))
				{
					IHTMLElement* pBodyElem = 0;
					HRESULT hr = HTMLDocument->get_body(&pBodyElem);
					if (SUCCEEDED(hr) && pBodyElem != 0)
					{
						IHTMLBodyElement* pBody = 0;
						hr = pBodyElem->QueryInterface(IID_IHTMLBodyElement, (void**) & pBody);
						if (SUCCEEDED(hr))
						{
							// hide 3D border
							IHTMLStyle* pStyle;
							hr = pBodyElem->get_style(&pStyle);
							if (SUCCEEDED(hr))
							{
								pStyle->put_borderStyle(BSTR("none"));
								pStyle->Release();
							}
							// hide scrollbars
							pBody->put_scroll(BSTR("no"));
						}
						pBodyElem->Release();
					}
					else if (pBodyElem != 0)
					{
						pBodyElem->Release();
					}
				}

				if (doc->QueryInterface(IID_IPersistStreamInit, (void**) & psi) == S_OK)
				{
					if (psi)
						psi->Load(*sa);
				}
			}
			__finally
			{
				htmlbusy = false;
			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}

}

void TDeviceKiosk::PrintMemberDocket(Database::TDBTransaction & DBTransaction, int ContactKey)
{
	UnicodeString index = ExtractFilePath(Application->ExeName) + "\\Kiosk\\DocketMember.txt";
	if (!FileExists(index))
	{
		index = ExtractFilePath(Application->ExeName) + "\\Kiosk\\Docket.txt";
	}
	if (FileExists(index))
	{
		std::map<UnicodeString, UnicodeString>Data;

		TMMContactInfo Info;
		Info.ContactKey = ContactKey;

		TLoginSuccess LoginSuccess = ManagerMembership->FindMember(DBTransaction, Info);
		GetKioskDisplayMsg(DBTransaction, Info, Data);

		Data["%GREETING%"] = "Membership Status <br> ";
		if (LoginSuccess == lsUserNotFound)
		{
			Data["%NAME%"] = "Membership not found!";
		}
		else
		{
			Data["%NAME%"] = Info.Name;
			Data["%NUMBER%"] = Info.MembershipNumber;
			Data["%EXPIRES%"] = "Membership Expires on " + Info.Expires.FormatString("DD/MM/YYYY");
			Data["%POINTS%"] = "Points : " + FormatFloat("0.00", Info.Points.getPointsBalance(pasDatabase));

			if (LoginSuccess == lsAccountBlocked)
			{
				if (Mode == eDispModeFinancial)
				{
					Data["%MSG%"] = "Sorry you are no longer a Member.";
                	Data["%EXPIRES%"] = "Membership Expired on " + Info.Expires.FormatString("DD/MM/YYYY");
				}
				else
				{
					Data["%MSG%"] = "You are not a Financial Member. <br> Please pay at the Office";
				}
			}
		}

		std::auto_ptr<TStringList>MyDocket(new TStringList);
		MyDocket->LoadFromFile(index);
		MyDocket->Text = StringReplace(MyDocket->Text, "%TITLE%", Data["%TITLE%"], TReplaceFlags() << rfReplaceAll);
		MyDocket->Text = StringReplace(MyDocket->Text, "%DOCKET_HEADER%", Data["%DRAW_HEADER%"], TReplaceFlags() << rfReplaceAll);
		MyDocket->Text = StringReplace(MyDocket->Text, "%NAME%", Data["%NAME%"], TReplaceFlags() << rfReplaceAll);
		MyDocket->Text = StringReplace(MyDocket->Text, "%NUMBER%", Data["%NUMBER%"], TReplaceFlags() << rfReplaceAll);
		MyDocket->Text = StringReplace(MyDocket->Text, "%DATETIME%", Now().FormatString("dd/mm/yyyy hh:nn:ss am/pm").t_str(), TReplaceFlags() << rfReplaceAll);
		MyDocket->Text = StringReplace(MyDocket->Text, "%GREETING%", Data["%GREETING%"], TReplaceFlags() << rfReplaceAll);
		MyDocket->Text = StringReplace(MyDocket->Text, "%DETIALS%", Data["%EXPIRES%"], TReplaceFlags() << rfReplaceAll);
		MyDocket->Text = StringReplace(MyDocket->Text, "%REWARD_MESSAGE%", Data["%DRAW_TEXT%"], TReplaceFlags() << rfReplaceAll);
		MyDocket->Text = StringReplace(MyDocket->Text, "%MSG%", "", TReplaceFlags() << rfReplaceAll);
		MyDocket->Text = StringReplace(MyDocket->Text, "%MSG1%", "", TReplaceFlags() << rfReplaceAll);
		PrintKioskDocket(MyDocket.get());
	}
}

void TDeviceKiosk::PrintKioskDocket(TStringList * Docket)
{
	try
	{
		std::auto_ptr<TPrintout>Printout(new TPrintout);

		TPrinterPhysical DefaultScreenPrinter;
		DefaultScreenPrinter.NormalCharPerLine = 40;
		DefaultScreenPrinter.BoldCharPerLine = 40;
		DefaultScreenPrinter.ShareName = Printer;
		DefaultScreenPrinter.Type = ptWindows_Printer;
		DefaultScreenPrinter.CutterGap = 5;
		DefaultScreenPrinter.TopMargin = 5;
		DefaultScreenPrinter.EmulationMode = emEpson;

		Printout->Printer = DefaultScreenPrinter;
		Printout->PrintFormat->DocumentName = "Kiosk Docket";

		std::auto_ptr<TStringList>MultiLine(new TStringList);
		for (int i = 0; i < Docket->Count; i++)
		{
			MultiLine->Clear();
			UnicodeString Line = Docket->Strings[i];

			Printout->PrintFormat->Line->ColCount = 1;
			Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
			Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;

			if (Line.Pos("?DH?"))
			{
				Printout->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
				Line = StringReplace(Line, "?DH?", "", TReplaceFlags() << rfReplaceAll);
			}
			else
			{
				Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			}

			if (Line.Pos("?DW?"))
			{
				Printout->PrintFormat->Line->FontInfo.Width = fsDoubleSize;
				Line = StringReplace(Line, "?DW?", "", TReplaceFlags() << rfReplaceAll);
			}
			else
			{
				Printout->PrintFormat->Line->FontInfo.Width = fsNormalSize;
			}

			if (Line.Pos("?DL?"))
			{
				Line = StringReplace(Line, "?DL?", "", TReplaceFlags() << rfReplaceAll);
				Printout->PrintFormat->Line->Columns[0]->DoubleLine();
				Printout->PrintFormat->AddLine();
			}
			else
			{
				Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				MultiLine->Text = StringReplace(Line, "<br>", "\r", TReplaceFlags() << rfReplaceAll);
				for (int j = 0; j < MultiLine->Count; j++)
				{
					Printout->PrintFormat->Line->Columns[0]->Text = MultiLine->Strings[j];
					Printout->PrintFormat->AddLine();
				}
			}
		}
		Printout->PrintFormat->Cut();
		Printout->PrintFormat->Print(DefaultScreenPrinter.ShareName, "");
		Printout->PrintFormat->GetText()->SaveToFile("PrintedDocket.txt");
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

void TDeviceKiosk::SetDisplayMode(Database::TDBTransaction & DBTransaction, eDispMode inMode)
{
	Mode = inMode;
	TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmKioskDisplayMode, (int)Mode);
}

eDispMode TDeviceKiosk::GetDisplayMode()
{
	return Mode;
}

void TDeviceKiosk::DisplayEntrances(Database::TDBTransaction & DBTransaction)
{
    std::auto_ptr <TStringList> Report(new TStringList);
    ManagerMembership->MembershipSystem->GetReportMemberOnPremisesStart(DBTransaction, Report.get());
    ManagerMembership->MembershipSystem->GetReportMemberOnPremisesInfo(DBTransaction, Report.get());
    ManagerMembership->MembershipSystem->GetReportStop(DBTransaction, Report.get());
    Navigate(Report.get());
}

void TDeviceKiosk::SetMembershipMode(Database::TDBTransaction & DBTransaction, eMembershipType inMode)
{
	TGlobalSettings::Instance().MembershipType = inMode;
	TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmKioskMembershipMode, (int)Mode);
	ManagerMembership->Initialise(DBTransaction);
	OpenMembershipDB(false);
}

eMembershipType TDeviceKiosk::GetMembershipType()
{
	return TGlobalSettings::Instance().MembershipType;
}
