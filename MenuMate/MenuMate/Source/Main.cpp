//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "EftposSmartLink.h"
#include "Analysis.h"
#include <process.h>
#include <inifiles.hpp>
#include "Comms.h"
#include "Main.h"
#include "enum.h"
#include "MMLogging.h"
#include "Setup.h"
#include "Maintain.h"
#include "DbMod.h"
#include "Pause.h"
#include "NewUser.h"
#include "Secure.h"
#include "PosMain.h"
#include "TimeClock.h"
#include "GetMoney.h"
#include "MMRegistry.h"
#include "GetMoney.h"
#include "Processing.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "Login.h"
#include "SelectDish.h"
#include "Locations.h"
#include "SystemConfig.h"
#include "SplitPayment.h"
#include "SelectActiveMenus.h"
#include "DBSaleTimes.h"
#include "Registration.h"
#include "ContactStaff.h"
#include "Invoice.h"
#include "MMMessageBox.h"
#include "Version.h"
#include "ManagerDiscount.h"
#include "RunManager.h"
#include "MessageManager.h"
#include "ManagerStock.h"
#include "ManagerVariable.h"
#include "FreebieManager.h"
#include "TableManager.h"
#include "ManagerNet.h"
#include "ReceiptManager.h"
#include "ManagerThirdParty.h"
#include "Rooms.h"
#include "SelectRoom.h"
#include "SelectMember.h"
#include "PhoenixHotelSystem.h"
#include "SelectPhoenixRoom.h"
#include "VirtualPrintersInterface.h"
#include "VirtualPrinterManager.h"
#include "ManagerPhysicalPrinter.h"
#include "ManagerPatron.h"
#include "DocketManager.h"
#include "Membership.h"
#include "MembershipERS.h"
#include "BillGroup.h"
#include "Paytype.h"
#include "ZForm.h"
#include "SmartCardAPI.h"
#include "EftPos.h"
#include "EftPosANZ.h"
#include "EftPosSyncro.h"
#include "EftposIngenico.h"
#include "EftposCadmus.h"
#include "EftposDPS.h"
#include "EftposIceLink.h"
#include "EftposCadmusCronos.h"
//#include "StringTableRes.h"
#include "StringTableVariables.h"
#include <StrUtils.hpp>
#include "VerticalSelect.h"
#include "WeighScale.h"
#include "MMData.h"
#include "DBTab.h"
#include "WebMate.h"
#include "DBGroups.h"
#include "DBContacts.h"
#include "MMTransactionRecovery.h"
#include "MMessageFromFileManager.h"
#include "XeroIntegration.h"
#include "MallExportManager.h"
#include "GlobalSettings.h"
#include "MallExportDefines.h"
#include "MMCustomerDisplayManager.h"
#include "ManagerCloudSync.h"
#include "CSVExportReceiver.h"
//#include "ManagerPanasonic.h"
#include "ManagerPMS.h"
#include "EftposSmartConnect.h"
#include "EftposAdyen.h"
#include "EFTPOSPaymentSense.h"
#include "SignalRUtility.h"
#include "ManagerAustriaFiscal.h"
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "pngimage"
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdIPWatch"

#pragma link "C:\\Program Files (x86)\\Embarcadero\RAD Studio\\7.0\\lib\\psdk\\oleacc.lib"

#pragma resource "*.dfm"

#define BTNNORMALCOLOUR clSilver
//0x0063E4DE
#define BTNOVERCOLOUR TColor(RGB(0xFC,0xDF,0xCF))


TfrmMain *frmMain;

const NETWORK = 0;

//---------------------------------------------------------------------------

__fastcall TfrmMain::TfrmMain(TComponent* Owner)
: TZForm(Owner)
{
	RequestClose  = false;
	TDeviceRealTerminal::Instance();
	TManagerLogs::Instance();
	// To make sure it's the very last thing freed, as IBLogs logs using it.
	TComms::Instance();
	TRooms::Instance();
	Receipt = new TReceipt;
	Invoice = new TInvoice;
	ManagerDiscount = new TManagerDiscount();
	ManagerRun = new TManagerRun();
	ManagerMessage = new TManagerMessage();
	ManagerFreebie = new TManagerFreebie();
	ManagerReceipt = new TManagerReceipt(TDeviceRealTerminal::Instance().DBControl);
	ManagerDockets = new TManagerDockets(TDeviceRealTerminal::Instance().DBControl);
	TManagerPatron::Instance();
	TManagerChitNumber::Instance();
	EftPos = NULL;
	frmSplitPayment = NULL;
	frmSecurity = NULL;
	frmSelectDish = NULL;
	Application->OnMessage = AppMessage;
	Application->OnIdle = IdleHandler;
	OleInitialize(NULL);
	FMenusToImportFolder = ExtractFilePath( Application->ExeName ) + "Menu Import";
	FMenusStockExchange= ExtractFilePath( Application->ExeName ) + "Stock Exchange";
	proxyMateManager = &TProxyMateManager::Instance();
	fpConnector   = NULL;
	fpConnectorUp = false;
	fpOpen        = false;
}

__fastcall TfrmMain::~TfrmMain()
{
	OleUninitialize();
	if (Win32Platform != VER_PLATFORM_WIN32_NT) ::Sleep(5000);  // Stops Win 98 closeing to fast and causeing an Access Violation.

	tiNewMenuCheck->Enabled = false;
	tiMenuUpdateReq->Enabled = false;
	tiCardSwipe->Enabled = false;
	tiBarStock->Enabled = false;
	tiBarStockturnover->Enabled = false;
	// For Mall Export Timer
	tiMallExportTiming->Enabled = false;
	tiCheckZedRead->Enabled = false;
	delete frmSplitPayment;
	delete frmSecurity;
	delete frmSelectDish;
	delete Receipt;
	Receipt = NULL;
	delete Invoice;
	Invoice = NULL;
	delete ManagerDiscount;
	ManagerDiscount = NULL;
	delete EftPos;
	EftPos = NULL;
	delete ManagerRun;
	ManagerRun = NULL;
	delete ManagerMessage;
	ManagerMessage = NULL;
	delete ManagerFreebie;
	ManagerFreebie = NULL;
	delete ManagerReceipt;
	ManagerReceipt = NULL;
	delete ManagerDockets;
	ManagerDockets = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	if (Win32Platform != VER_PLATFORM_WIN32_NT) ::Sleep(1000);  // Stops Win 98 closing too fast and causing an Access Violation.
	// Dont ask me how or why this seems to fix it.
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	frmMain->Tag = DESIGN_SCREEN_SIZE; //#Defined in Data.
	// Create the background Comms Object.
	JstBooted = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::IdleHandler(TObject *Sender, bool &Done)
{
	try
	{
		TManagerLogs::Instance().Process();
		TManagerLogs::Instance().Purge(TGlobalSettings::Instance().ClearLogsEveryXDays);
	}
	catch(Exception &E)
	{
		ShowMessage("IdleHandler Error " + E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	try
	{
		// Create DB.
		frmDBMod = TfrmDBMod::Create<TfrmDBMod>(this);
		frmDBMod->Show();
		frmDBMod->SetCaption("Loading...");
		Application->CreateForm(__classid(TfrmData), &frmData);
		frmDBMod->SetCaption("Opening Database...");
		if(!TDeviceRealTerminal::Instance().OpenDatabases())
		{
			Close();
			return;
		}
        std::auto_ptr<TManagerPMS> managerPMS (new TManagerPMS());
        managerPMS->Initialize(TDeviceRealTerminal::Instance().DBControl, TGlobalSettings::Instance().PMSType);
		lbeVersion->Caption = "";
		frmDBMod->SetCaption("Initialising Terminal...");
		Database::TDBTransaction DBBootTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBBootTransaction.StartTransaction();

		bool Registered = false;
		UnicodeString pRegisteredName = "";
		TDeviceRealTerminal::Instance().Registered(&Registered,&pRegisteredName);
//		if(!Registered)
//		{
//			frmDBMod->SetRegCaption("Unregistered");
//			lbeRegistration->Caption = pRegisteredName;
//		}
//		else
//		{
			TManagerLogs::Instance().Add("NA",REGISTRATIONLOG,"Application Registered");
			frmDBMod->SetRegCaption(pRegisteredName);
			lbeRegistration->Caption = pRegisteredName;
//		}

		frmDBMod->SetCaption("Updating Clock...");
		TDeviceRealTerminal::Instance().UpdateClockSyncInfo();
		frmDBMod->SetCaption("Initialising...");
		TComms::Instance().Initialise(DBBootTransaction);
		ManagerFreebie->Initialise(DBBootTransaction);
		Invoice->Initialise();
		ManagerDiscount->Initialise();
		ManagerReceipt->Initialise(TDeviceRealTerminal::Instance().ID.Name,TDeviceRealTerminal::Instance().ID.DeviceKey);
		ManagerDockets->Initialise(TDeviceRealTerminal::Instance().ID.Name,TDeviceRealTerminal::Instance().ID.DeviceKey);
		TManagerThirdParty::Instance().Initialise(TDeviceRealTerminal::Instance().ID.Product);
		TManagerPhysicalPrinter ManagerPhysicalPrinter;
		ManagerPhysicalPrinter.Initialise(DBBootTransaction,TDeviceRealTerminal::Instance().ID.DeviceKey,TDeviceRealTerminal::Instance().ID.ComputerName);
		TManagerVirtualPrinter::InitialisePrinters(DBBootTransaction,TDeviceRealTerminal::Instance().ID.DeviceKey,TDeviceRealTerminal::Instance().ID.ComputerName);
        //TManagerVirtualPrinter::InitialisePrinters(DBBootTransaction,TDeviceRealTerminal::Instance().ID.DeviceKey);
		TManagerChitNumber::Instance().Initialise(DBBootTransaction,TDeviceRealTerminal::Instance().ID.DeviceKey);
		Receipt->Initialise(DBBootTransaction);
		if(!TDBGroups::GroupExists(DBBootTransaction, "No Contacts Group"))
		{
			std::vector<ContactGroup> members;
			TDBContacts DBContacts;
			TMMContactInfo Info;
			ContactGroup GroupKey;
			GroupKey.Key = TDBGroups::SetDefaultGroup(DBBootTransaction);
			DBContacts.GetContactNameList(DBBootTransaction, members, 0);
			DBContacts.GetContactNameList(DBBootTransaction, members, 1);
			for(int j = 0; j < members.size(); j++)
			{
				if(!TDBContacts::GetCurrentGroups(DBBootTransaction, members.at(j).Key, Info))
				DBContacts.SetCurrentGroups(DBBootTransaction, members.at(j).Key, GroupKey);
			}
		}
		if(TGlobalSettings::Instance().TabsEnabled)
		{
			TManagerLogs::Instance().Add("NA",REGISTRATIONLOG,"Tabs Enabled");
		}
//		if(TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"])
//		{
			TManagerLogs::Instance().Add("NA",REGISTRATIONLOG,"Members Mod Registered");
//		}
		frmDBMod->SetCaption("Loading Rooms Interface...");
//		if(TDeviceRealTerminal::Instance().Modules.Status[eRegRooms]["Registered"])
//		{
			TRooms::Instance().Initialise(DBBootTransaction);
			TManagerLogs::Instance().Add("NA",REGISTRATIONLOG,"Rooms Mod Registered");
//		}
//		if(TDeviceRealTerminal::Instance().Modules.Status[ePhoenixHotelSystem]["Registered"])
//		{
   			TDeviceRealTerminal::Instance().BasePMS->Registered = true;
            ReFormatIpToUrl();
            TDeviceRealTerminal::Instance().BasePMS->LogPMSEnabling(eBoot);
			TDeviceRealTerminal::Instance().BasePMS->Initialise();
			TRooms::Instance().Enabled = false;
//		}
//		if(TDeviceRealTerminal::Instance().Modules.Status[eRegKitchenScreen]["Registered"])
//		{
			TDeviceRealTerminal::Instance().KitchenMod->Enabled = true;
			TManagerLogs::Instance().Add("NA",REGISTRATIONLOG,"Kitchen Mod Registered");
//		}
//		if(TDeviceRealTerminal::Instance().Modules.Status[eRegSaleTurnAround]["Registered"])
//		{
			TManagerLogs::Instance().Add("NA",REGISTRATIONLOG,"Sale Time Mod Registered");
//		}
		bool EftPosRegiestered = true;
//        TDeviceRealTerminal::Instance().Modules.Status[eEFTPOS]["Registered"];
        bool IsSyncroEftPosEnabled = false;

		if(EftPosRegiestered)
		{
			if (TGlobalSettings::Instance().EnableEftPosDPS)
			{
				EftPos = new TEftPosMMDPS();
				EftPos->Initialise();
			}
			else if(TGlobalSettings::Instance().EnableEftPosANZ)
			{
				EftPos = new TEftPosANZ();
				EftPos->Initialise();
			}
			else if (TGlobalSettings::Instance().EnableEftPosCadmus)
			{
				EftPos = new TEftPosCadmus();
				EftPos->Initialise();
			}
			else if (TGlobalSettings::Instance().EnableEftPosCadmusCronos)
			{
				EftPos = new TEftPosCadmusCronos();
				EftPos->Initialise();
			}
			else if (TGlobalSettings::Instance().EnableEftPosIngenico)
			{
				EftPos = new TEftPosIngenico();
				EftPos->Initialise();
			}
			else if (TGlobalSettings::Instance().EnableEftPosIceLink)
			{
				EftPos = new TEftPosIceLink();
				EftPos->Initialise();
			}
			else if (TGlobalSettings::Instance().EnableEftPosSmartPay)
			{
				EftPos = new TEftPosSmartLink();
				EftPos->Initialise();
			}
			else if (TGlobalSettings::Instance().EnableEftPosSmartConnect)
			{
				EftPos = new TEftPosSmartConnect();
				EftPos->Initialise();
			}
			else if (TGlobalSettings::Instance().EnableEftPosAdyen)
			{
				EftPos = new TEftposAdyen();
				EftPos->Initialise();
			}
			else if (TGlobalSettings::Instance().EnableEftPosPaymentSense)
			{
				EftPos = new TEftPosPaymentSense();
				EftPos->Initialise();
			}
			else
			{
				IsSyncroEftPosEnabled = true;
			}
		}
		else
		{
			IsSyncroEftPosEnabled = true;
		}
        if(IsSyncroEftPosEnabled)
        {
            EftPos = new TEftPosSyncro();
			EftPos->Initialise();
        }
//        TDeviceRealTerminal::Instance().Modules.Status[eReservations]["Registered"] = true;
//		TDeviceRealTerminal::Instance().Modules.Status[eWebMate]["Registered"] = true;
//		if(TGlobalSettings::Instance().WebMateEnabled && TDeviceRealTerminal::Instance().Modules.Status[eWebMate]["Registered"])
//		{
			TWebMate::Instance().Initialise(TGlobalSettings::Instance().WebMateEnabled, ExtractFilePath(Application->ExeName),TGlobalSettings::Instance().InterbaseIP,TGlobalSettings::Instance().DatabasePath, TGlobalSettings::Instance().WebMatePort);
//			TDeviceRealTerminal::Instance().Modules.Status[eWebMate]["Enabled"] = TWebMate::Instance().Enabled;
//		}

        //Posting CSV at specified IP
        if(TGlobalSettings::Instance().IsEnabledPeachTree)
        {
            TCSVExportReceiver::Instance().Initialise(TGlobalSettings::Instance().CSVExportIP, TGlobalSettings::Instance().CSVPath, 47001);
        }
        if(TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled)
        {
            TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = false;
            std::auto_ptr<TManagerAustriaFiscal> managerAustriaFiscal(new TManagerAustriaFiscal());

            if(TGlobalSettings::Instance().AustriaFiscalUrl != "")
            {
                if(TGlobalSettings::Instance().IsAustriaFiscalCommissioned)
                {
                    if(managerAustriaFiscal->GetEchoResponseFromMain())
                    {
                        TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = managerAustriaFiscal->SendZeroReceipt();
                        if(!TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled)
                           MessageBox("Could not communicate with Austria Fiscal.\rAustria Fiscal reporting is disabled.","Error",MB_OK+MB_ICONERROR);
                    }
                }
                else
                {
                    TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled = false;
                    MessageBox("Austria Fiscal is not commissioned at the moment.\rPlease run Commission command using option available under Configuration.","Info",MB_OK+MB_ICONINFORMATION);
                }
            }
        }
		// Set up Header and Footer Info.
		TDeviceRealTerminal::Instance().LoadHdrFtr();
		FormResize(Sender);
		if(JstBooted)
		{
			JstBooted = false;
			frmSplitPayment = TfrmSplitPayment::Create<TfrmSplitPayment>(this);
			frmSecurity = TfrmSecurity::Create<TfrmSecurity>(this);
			frmSelectDish = TfrmSelectDish::Create<TfrmSelectDish>(this);
            /* POS */

			frmDBMod->SetCaption("Cleaning Logs");
			RemoveOldLogs();
			frmDBMod->SetCaption("Loading Menus...");
			TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
			Request->Broadcast = false;
			TDeviceRealTerminal::Instance().Menus->GetCurrentMenus(DBBootTransaction,Request);
			TDeviceRealTerminal::Instance().Menus->MenuChanged(DBBootTransaction, Request, false);
			delete Request;
			// Swap in the Menus.
			TDeviceRealTerminal::Instance().Menus->ToggleMenus();
			// Check and Create a defualt user in the Users table if necessary.
			frmDBMod->SetCaption("Loading Users...");
			std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBBootTransaction));
			while(Staff->RecordCount(DBBootTransaction) == 0)
			{
				MessageBox("There are currently no users.\rPlease enter a user now.",
				"Information",
				MB_OK);
				std::auto_ptr<TfrmNewUser>(frmNewUser)(TfrmNewUser::Create<TfrmNewUser>(this,TDeviceRealTerminal::Instance().DBControl));
				frmNewUser->ShowModal();
			}

			frmDBMod->SetCaption("Downloading Printer Graphic...");
			TDeviceRealTerminal::Instance().LoadPrinterGraphic();
			TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplayDefault();
			// Set the Hint Pause.
			Application->HintPause = 1000;
			Application->HintHidePause = 9000;
			tiNewMenuCheck->Enabled = true;
			tiMenuUpdateReq->Enabled = true;
			tiBarStock->Enabled = true;
			tiBarStockturnover->Enabled = true;
			frmDBMod->Close();
			//--------------------------------- Customer Support File.
			if (FileExists(ExtractFilePath(Application->ExeName) + "\\Dealer.Info"))
			{
				MemInfo->Lines->LoadFromFile(ExtractFilePath(Application->ExeName) + "\\Dealer.Info");
			}
		}
		updateHTMLDisplay(DBBootTransaction);
		TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplayDefault();
		// recover any lost eftpos transactions
        bool processRecovery = true;
		TMMTransactionRecovery transactionRecovery;

        if(TGlobalSettings::Instance().EnableEftPosSmartConnect && TGlobalSettings::Instance().IsSmartConnectQRTransaction)
            processRecovery = false;

        if(processRecovery)
		    transactionRecovery.ProcessTransactionRecovery();

		SetGridColors(tgridMenu);
        tgridMenu->GridColor = RGB(255,255,255);
        tgridMenu->Color	= RGB(255,255,255);
		tiShowMsg->Interval = 3000;  // The timer will check for the msg.csv file every 3 sec
		tiShowMsg->Enabled  = true;
		TMMessageFromFileManager::Instance().Folder = ExtractFilePath(Application->ExeName) + "Import";
		InitXeroIntegration();
		tiFailedXeroInvoices->Interval = FailedXeroInvoivesToSend() ? TXeroIntegration::Instance().FailedActiveInterval : TXeroIntegration::Instance().FailedPauseInterval;
		tiFailedXeroInvoices->Enabled  = true;
		initProxyMateManager();
		// Mall export code
		EnablePOSForSpecifiedMallSchedule();
		InitializeExportTimings(TGlobalSettings::Instance().FirstMallSet);
		TGlobalSettings::Instance().FirstMallSet = false;
		SaveBoolVariable(vmFirstMallSet, TGlobalSettings::Instance().FirstMallSet);
		openCustomerDisplayServer();
//        if(TGlobalSettings::Instance().IsPanasonicIntegrationEnabled)
//        {
//            TManagerPanasonic::Instance();
//            TManagerPanasonic::Instance()->PrepareTenderTypes();
//            TManagerPanasonic::Instance()->PrepareTransactionTypesAndTerminalId();
//        }
        SyncCompanyDetails();
       //initialize this variable when application starts..
       TManagerVariable::Instance().SetDeviceBool(DBBootTransaction, vmNotifyLastWebOrder, TGlobalSettings::Instance().NotifyPOSForLastWebOrder);
       TManagerVariable::Instance().SetDeviceBool(DBBootTransaction, vmUpdateMenu, TGlobalSettings::Instance().UpdateMenu);
       TGlobalSettings::Instance().ForceHappyHour = false;
       TManagerVariable::Instance().SetDeviceBool(DBBootTransaction, vmForceHappyHour, TGlobalSettings::Instance().ForceHappyHour);
       if(TGlobalSettings::Instance().ItemPriceIncludeTax)
       {
            SaveItemPriceIncludeTaxToDatabase(vmItemPriceIncludeTax, TGlobalSettings::Instance().ItemPriceIncludeTax);
       }
       WriteDBPathAndIPToFile();
       if(TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled)
          SetUpAustriaFiscal();
       DBBootTransaction.Commit();
	}
	catch(Exception &E)
	{
		frmDBMod->Close();
		MessageBox(E.Message.w_str(), _T("Error"), MB_ICONERROR + MB_OK);
		Close();
	}
}
//---------------------------------------------------------------------------
void TfrmMain::SyncCompanyDetails()
{
    if (TGlobalSettings::Instance().LoyaltyMateEnabled)
    {
        TManagerCloudSync ManagerCloudSync;
        ManagerCloudSync.CheckSyndCodes();
        bool isSyncSuccessful = ManagerCloudSync.SyncCompanyDetails();

         AnsiString DirectoryName = ExtractFilePath(Application->ExeName) + "MemberEmails";
         if (!DirectoryExists(DirectoryName))
            CreateDir(DirectoryName);

         AnsiString filename = DirectoryName + "\\" + "MemberDetail.txt";
        std::auto_ptr <TStringList> logList(new TStringList);
        logList->SaveToFile(filename);

        if(isSyncSuccessful && TGlobalSettings::Instance().EnableOnlineOrdering)
        {
            UnloadSignalR();
            EnableOnlineOrdering();
        }
        else if(!isSyncSuccessful && TGlobalSettings::Instance().EnableOnlineOrdering)
        {
            DisableOnlineOrdering();
            UnicodeString strValue = "Online ordering could not be enabled since sync with online module failed.\r";
            strValue += "Please ensure below mentioned things:-.\r";
            strValue += "1. Syndicate code & Site Id are correct.\r";
            strValue += "2. POS terminal is connected to network.";
            MessageBox(strValue,"Info",MB_OK+MB_ICONINFORMATION);
        }
    }
    else
    {
       DisableOnlineOrdering();
    }

}
//---------------------------------------------------------------------------
void TfrmMain::UnsetOrderingDetails()
{
    if (TGlobalSettings::Instance().LoyaltyMateEnabled)
    {
        TManagerCloudSync ManagerCloudSync;
        ManagerCloudSync.CheckSyndCodes();
        bool isSyncSuccessful = ManagerCloudSync.UnsetSinalRConnectionStatus();
    }
    else
    {
       DisableOnlineOrdering();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormActivate(TObject *Sender)
{
	if( proxyMateManager->ClientUp && !fpConnectorUp )
	{
		OpenFPConnector(); // Open XeroMate's FloorPlan connector;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDeactivate(TObject *Sender)
{
	CloseFPConnector(); // Close XeroMate's FloorPlan connector;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnAnalysisClick(TObject *Sender)
{
	std::auto_ptr<TfrmAnalysis>(frmAnalysis)(TfrmAnalysis::Create<TfrmAnalysis>(this));
	frmAnalysis->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSetupClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckSetup);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		TDeviceRealTerminal::Instance().User = TempUserInfo;

		std::auto_ptr<TfrmSetup>(frmSetup)(TfrmSetup::Create<TfrmSetup>(this,TDeviceRealTerminal::Instance().DBControl));
		frmSetup->ShowModal();
		frmSecurity->LogOut();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to Setup.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnRemindersClick(TObject *Sender)
{
	std::auto_ptr<TfrmTimeClock>(frmTimeClock)(TfrmTimeClock::Create<TfrmTimeClock>(this,TDeviceRealTerminal::Instance().DBControl));
	frmTimeClock->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnExitClick(TObject *Sender)
{
	bool Continue = true;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	if(frmSelectDish->ParkedSales->GetCount(DBTransaction) > 0)
	{
		Continue = false;
		if (MessageBox("There are Currently Parked Sales!\rAre you sure you wish to Quit?",
					"Warning",
					MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
		{
			Continue = true;
		}
	}
	if(Continue)
	{
        TerminateProcess(TGlobalSettings::Instance().piOracleApp.hProcess , 0);
        // call api to set connection status of signalR
        if(TGlobalSettings::Instance().LoyaltyMateEnabled && TGlobalSettings::Instance().EnableOnlineOrdering)
        {
            UnsetOrderingDetails();
            UnloadSignalR();
        }
		frmSecurity->LogOut();
		frmMain->Close();
	}
	DBTransaction.Commit();

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
	if (frmMain->Tag != Screen->Width)
	{
		int Temp = frmMain->Tag;
		frmMain->Tag = Screen->Width;
		if((double)Screen->Width/Screen->Height < 1.4)
		{
			frmMain->ScaleBy(Screen->Width, Temp);
		}
		frmMain->Width = Screen->Width;
		frmMain->Height = Screen->Height;
	}
    tgridMenu->Visible = true;
	lbTitle->Top = Height * 0.82;
	lbeVersion->Top = lbTitle->Top + lbTitle->Height;
	lbeRegistration->Top = lbeVersion->Top + lbeVersion->Height;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnMaintenanceClick(TObject *Sender)
{
	std::auto_ptr<TfrmMaintain>(frmMaintain)(TfrmMaintain::Create<TfrmMaintain>(this));
	frmMaintain->ShowModal();
}
//---------------------------------------------------------------------------
void TfrmMain::RemoveOldLogs()
{
//
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnPOSClick(TObject *Sender)
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPOS);
	bool PaymentAccessResult = Staff->TestAccessLevel(TempUserInfo, CheckPaymentAccess);
	DBTransaction.Commit();

	if (Result == lsAccepted)
	{
		TDeviceRealTerminal::Instance().User = TempUserInfo;

		frmSelectDish->CurrentLocation = eLocation(LocRestaurant);
		if(TGlobalSettings::Instance().DefaultToBevMenu)
		{
			TDeviceRealTerminal::Instance().Menus->SelectMenuOfType(eDrinksMenu);
		}
		else
		{
			TDeviceRealTerminal::Instance().Menus->SelectMenuOfType(eFoodMenu);
		}

		if(TDeviceRealTerminal::Instance().Menus->VisibleMenu == NULL)
		{
			TListMenu *CurrentMenu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(0);
			if(CurrentMenu != NULL)
			{
				TDeviceRealTerminal::Instance().Menus->VisibleMenu = CurrentMenu;
			}
		}
		frmSelectDish->IsWaiterLogged =  !PaymentAccessResult ;
		frmSelectDish->PosRefresh = true;
		frmSelectDish->Show();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to item sales.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::AppMessage(tagMSG &Msg, bool &Handled)
{
	if( proxyMateWillHandle( Msg ) )
	{
		return;
	}

	if (Msg.message == WM_LBUTTONUP || Msg.message == WM_RBUTTONUP ||
			Msg.message == WM_KEYDOWN || Msg.message == WM_KEYUP)
	{
		TDeviceRealTerminal::Instance().ResetEventLockOutTimer();
	}
	if (Msg.message == WM_CHAR)
	{
		if (!(Msg.lParam & 0x1000000)) // Is the CTRL or ALT key being held down i.e is it an Extended Key.
		{

			if (	!Screen->ActiveForm->ClassNameIs("TfrmTouchKeyboard") &&
					!Screen->ActiveForm->ClassNameIs("TfrmPhoenixRoom") &&
					!Screen->ActiveForm->ClassNameIs("TfrmSelectRoom") &&
					!Screen->ActiveForm->ClassNameIs("TfrmSelectMember") &&
					!Screen->ActiveForm->ClassNameIs("TfrmSelectItem") &&
					!Screen->ActiveForm->ClassNameIs("TfrmRMSRoom"  ) &&
                    !Screen->ActiveForm->ClassNameIs("TfrmCaptNamePhone") &&
                    !Screen->ActiveForm->ClassNameIs("TfrmEditCustomer")  &&
                    !Screen->ActiveForm->ClassNameIs("TfrmPocketVoucher") &&
                    !frmSelectDish->IsTextBoxFocused()
					)
			{
				Handled = true;
				if (tiCardSwipe->Enabled)
				{
					tiCardSwipe->Enabled = false;
					if(!iscntrl((char)Msg.wParam))
					{
						CardSwipeString += (char)Msg.wParam;
					}
					tiCardSwipe->Enabled = true;
				}
				else
				{
					if(!iscntrl((char)Msg.wParam))
					{
						CardSwipeString = (char)Msg.wParam;
						tiCardSwipe->Enabled = true;
					}
				}
			}
		}
	}

	if(Msg.message == SMARTCARD_CARDSTATUSCHANGED)
	{
		if(Msg.lParam == SMARTCARD_CARDREMOVED)
		{
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ResetPoints();
			TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->SmartCardRemoved();

		}
		else if(Msg.lParam == SMARTCARD_CARDINSERTED)
		{
            TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ResetPoints();
			TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->SmartCardInserted();
		}
	}

	if(Msg.message == WEIGHSCALES_STATUSCHANGED)
	{
		if(Msg.lParam == WEIGHSCALES_WEIGHTCHANGED)
		{
			TDeviceRealTerminal::Instance().Scales->OnWeightChanged.Occured();
		}
		else if(Msg.lParam == WEIGHSCALES_STABILITYCHANGED)
		{
			TDeviceRealTerminal::Instance().Scales->OnStabilityChanged.Occured();
		}
	}

	if(Msg.message == WEBORDER_STATUSCHANGED)
	{
		if(Msg.lParam == WEBORDER_RECEIVED)
		{
			PostMessage(Screen->ActiveForm->Handle, WEBORDER_STATUSCHANGED, NULL, NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tiCardSwipeTimer(TObject *Sender)
{
	tiCardSwipe->Enabled = false;
	if (CardSwipeString != "")
	{
		PostMessage(Screen->ActiveForm->Handle, UWM_CARDSWIPE, (UINT)&CardSwipeString, NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	TManagerLogs::Instance().Process();
	//	CloseDB();

	closeCustomerDisplayServer();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormPaint(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tiNewMenuCheckTimer(TObject *Sender)
{
	if( TDeviceRealTerminal::Instance().DBControl.Connected() ) // If we not connected don't bother.
	{
		tiNewMenuCheck->Enabled = false;

		if( MenusToImportFolderExists() )
		{
			if( MenusToImport() )
			{
				ImportMenus();
			}
		}
		else
		{
			CreateMenusToImportFolder();
		}

		ImportMembers();

		tiNewMenuCheck->Enabled = true;
	}
}
//---------------------------------------------------------------------------
bool TfrmMain::MenusToImportFolderExists()
{
	return DirectoryExists( FMenusToImportFolder );
}
//---------------------------------------------------------------------------
void TfrmMain::CreateMenusToImportFolder()
{
	CreateDir( FMenusToImportFolder );
}
//---------------------------------------------------------------------------
bool TfrmMain::MenusToImport()
{
	bool result = false;
	TSearchRec sr;
	int iAttributes = faAnyFile;

	if( FindFirst( FMenusToImportFolder + "\\*.xml", iAttributes, sr ) == 0 )
	{
		result = ( sr.Attr & iAttributes );
	}
	FindClose(sr);
	return result;
}
//---------------------------------------------------------------------------
void TfrmMain::ImportMenus()
{
	AnsiString dirName = FMenusToImportFolder;

	if( TDeviceRealTerminal::Instance().Menus->LoadMenus( TDeviceRealTerminal::Instance().DBControl, dirName ) )
	{
		std::auto_ptr<TNetMessageMenuChanged> Request( new TNetMessageMenuChanged );
		Request->Broadcast         = true;
		Request->CompareToDataBase = true;
		Database::TDBTransaction DBBootTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBBootTransaction.StartTransaction();
		TDeviceRealTerminal::Instance().Menus->MenuAddedRemoved( DBBootTransaction,Request.get() );
		DBBootTransaction.Commit();
	}
}
//---------------------------------------------------------------------------
void TfrmMain::ImportMembers()
{
	try
	{
		if( !DirectoryExists(ExtractFilePath( Application->ExeName ) + "Import" ) )
		{
			AnsiString Path = ExtractFilePath( Application->ExeName ) + "Import";
			CreateDir(Path);
		}
		else
		{
			AnsiString FileName = ExtractFilePath( Application->ExeName ) + "Import\\" + "members.csv";
			if( FileExists( FileName ) )
			{
				Database::TDBTransaction DBTransaction( TDeviceRealTerminal::Instance().DBControl );
				DBTransaction.StartTransaction();
				TDeviceRealTerminal::Instance().UpdatePeople(DBTransaction,FileName);
				DBTransaction.Commit();
			}
		}
	}
	catch( Exception &E )
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::DEMOMODE1DrawItem(TObject *Sender,
TCanvas *ACanvas, TRect &ARect, bool Selected)
{
	ACanvas->Font->Color = clRed;
	ACanvas->Font->Size = 14;
	ACanvas->Brush->Color = clBlack;
	ACanvas->Brush->Style = bsSolid;
	ACanvas->FillRect(ARect);
	AnsiString TextOut = " DEMO MODE ";
	TSize TextSize;
	TextSize = ACanvas->TextExtent(TextOut);
	int Loop = ARect.Width() / TextSize.cx;
	for (int i = 0; i < Loop ; i++)
	{
		TextOut += " DEMO MODE ";
	}
	ACanvas->TextOut(ARect.Left,ARect.Top,TextOut);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::DEMOMODE1MeasureItem(TObject *Sender,
TCanvas *ACanvas, int &Width, int &Height)
{
	Width = Screen->Width - 60;
	AnsiString TextOut = " DEMO MODE ";
	TSize TextSize;
	TextSize = ACanvas->TextExtent(TextOut);
	Height = TextSize.cy;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tiMenuUpdateReqTimer(TObject *Sender)
{
	//--------------------------------- Check All Menus are Current.
	if(TDeviceRealTerminal::Instance().DBControl.Connected()) // If we not connected dont bother.
	{
		try
		{
			tiMenuUpdateReq->Enabled = false;

			Database::TDBTransaction DBBootTransaction(TDeviceRealTerminal::Instance().DBControl);
			DBBootTransaction.StartTransaction();

			TNetMessageMenuChanged *Request = new TNetMessageMenuChanged;
			Request->Broadcast = false;
			TDeviceRealTerminal::Instance().Menus->GetCurrentMenus(DBBootTransaction,Request);
			TDeviceRealTerminal::Instance().Menus->MenuChanged(DBBootTransaction,Request);
			delete Request;
			// Swap in the Menus.
			DBBootTransaction.Commit();
			tiMenuUpdateReq->Enabled = true;
		}
		catch(Exception &E)
		{
			TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
			MessageBox("Unable to check Menus are up to date.\r"
			+ E.Message , "Error", MB_OK + MB_ICONERROR);

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::updateHTMLDisplay(Database::TDBTransaction &DBTransaction)
{
//
}

void __fastcall TfrmMain::tgButtonsMouseClick(TObject *Sender,
TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	switch (GridButton->Tag)
	{
	case 1:
		btnPOSClick(Sender);
		break;
	case 2:
		btnAnalysisClick(Sender);
		break;
	case 3:
		btnRemindersClick(Sender);
		break;
	case 4:
		{
			LaunchProcess();
		}
		break;
	case 5:
		btnMaintenanceClick(Sender);
		break;
	case 6:
		btnSetupClick(Sender);
		break;
	case 7:
		Quit();
		break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::LaunchProcess()
{
	// Display Launch List
	std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

	TVerticalSelection Item;
	Item.Title = "Cancel";
	Item.Properties["Color"] = IntToStr(clMaroon);
	Item.CloseSelection = true;
	SelectionForm->Items.push_back(Item);

	TVerticalSelection Item1;
	Item1.Title = "PalmMate";
	Item1.Properties["Action"] = IntToStr(1);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "Office";
	Item1.Properties["Action"] = IntToStr(2);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "DeskMate";
	Item1.Properties["Action"] = IntToStr(3);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	Item1.Title = "FloorPlan";
	Item1.Properties["Action"] = IntToStr(4);
	Item1.Properties["Color"] = IntToStr(clNavy);
	Item1.CloseSelection = true;
	SelectionForm->Items.push_back(Item1);

	SelectionForm->ShowModal();
	TVerticalSelection SelectedItem;
	if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
	{
		int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
		switch(Action)
		{
		case 1:
			{
				TMMContactInfo TempUserInfo;
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();
				std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
				TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPOS);
				DBTransaction.Commit();
				if (Result == lsAccepted)
				{
					TDeviceRealTerminal::Instance().User = TempUserInfo;

					HWND PrevWnd;

					PrevWnd = FindWindow(_T("TfrmMain"), _T("PalmMate"));
					if (PrevWnd != NULL)
					{
						PrevWnd = GetWindow(PrevWnd, GW_OWNER);
						if (PrevWnd != NULL)
						{
							if (IsIconic(PrevWnd))
							{
								ShowWindow(PrevWnd, SW_RESTORE);
							}
							else
							{
								SetForegroundWindow(PrevWnd);
							}
						}
					}
					else
					{
						_wspawnl(
						P_NOWAIT,
						(ExtractFilePath(Application->ExeName) + "PalmMate.exe").w_str(),
						(ExtractFilePath(Application->ExeName) + "PalmMate.exe").w_str(),
						NULL);

						::Sleep(1000);
						PrevWnd = FindWindow(_T("TfrmMain"), _T("PalmMate"));
						if (PrevWnd != NULL)
						{
							PrevWnd = GetWindow(PrevWnd, GW_OWNER);
							if (PrevWnd != NULL)
							{
								if (IsIconic(PrevWnd))
								{
									ShowWindow(PrevWnd, SW_RESTORE);
								}
								else
								{
									SetForegroundWindow(PrevWnd);
								}
							}
						}
					}
					frmSecurity->LogOut();
				}
				else if (Result == lsDenied)
				{
					MessageBox("You do not have access to Setup.", "Error", MB_OK + MB_ICONERROR);
				}
				else if (Result == lsPINIncorrect)
				{
					MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
				}
			}
			break;
		case 2:
			{
				TMMContactInfo TempUserInfo;
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();
				std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
				TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckDisable);
				DBTransaction.Commit();
				if (Result == lsAccepted)
				{

					HWND PrevWnd;

					PrevWnd = FindWindow(_T("TfrmMain"), _T(""));
					if (PrevWnd != NULL)
					{
						PrevWnd = GetWindow(PrevWnd, GW_OWNER);
						if (PrevWnd != NULL)
						{
							if (IsIconic(PrevWnd))
							{
								ShowWindow(PrevWnd, SW_RESTORE);
							}
							else
							{
								SetForegroundWindow(PrevWnd);
							}
						}
					}
					else
					{
						_wspawnl(
						P_NOWAIT,
						(ExtractFilePath(Application->ExeName) + "Office.exe").c_str(),
						(ExtractFilePath(Application->ExeName) + "Office.exe").c_str(),
						NULL);

						::Sleep(1000);
						PrevWnd = FindWindow(_T("TfrmMain"), _T("MenuMate Office"));
						if (PrevWnd != NULL)
						{
							PrevWnd = GetWindow(PrevWnd, GW_OWNER);
							if (PrevWnd != NULL)
							{
								if (IsIconic(PrevWnd))
								{
									ShowWindow(PrevWnd, SW_RESTORE);
								}
								else
								{
									SetForegroundWindow(PrevWnd);
								}
							}
						}
					}
				}
				else if (Result == lsDenied)
				{
					MessageBox("You do not have access to Setup.", "Error", MB_OK + MB_ICONERROR);
				}
				else if (Result == lsPINIncorrect)
				{
					MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
				}
			}
			break;
		case 3:
			{
				TMMContactInfo TempUserInfo;
				Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
				DBTransaction.StartTransaction();
				std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
				TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckQuit);
				DBTransaction.Commit();
				if (Result == lsAccepted)
				{
					HWND PrevWnd;

					PrevWnd = FindWindow(_T("TfrmMain"), _T("DeskMate"));
					if (PrevWnd != NULL)
					{
						PrevWnd = GetWindow(PrevWnd, GW_OWNER);
						if (PrevWnd != NULL)
						{
							if (IsIconic(PrevWnd))
							{
								ShowWindow(PrevWnd, SW_RESTORE);
							}
							else
							{
								SetForegroundWindow(PrevWnd);
							}
						}
					}
					else
					{
						_wspawnl(
						P_NOWAIT,
						(ExtractFilePath(Application->ExeName) + "DeskMate.exe").c_str(),
						(ExtractFilePath(Application->ExeName) + "DeskMate.exe").c_str(),
						NULL);

						::Sleep(1000);
						PrevWnd = FindWindow(_T("TfrmMain"), _T("DeskMate"));
						if (PrevWnd != NULL)
						{
							PrevWnd = GetWindow(PrevWnd, GW_OWNER);
							if (PrevWnd != NULL)
							{
								if (IsIconic(PrevWnd))
								{
									ShowWindow(PrevWnd, SW_RESTORE);
								}
								else
								{
									SetForegroundWindow(PrevWnd);
								}
							}
						}

					}
				}
				else if (Result == lsDenied)
				{
					MessageBox("You do not have access to Setup.", "Error", MB_OK + MB_ICONERROR);
				}
				else if (Result == lsPINIncorrect)
				{
					MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
				}
			}
			break;
		case 4:
			{
				showFloorPlan();
			}
			break;

		}
	}
}

void __fastcall TfrmMain::Quit()
{
	if (!TDeviceRealTerminal::Instance().DBControl.Connected())
	{
		TDeviceRealTerminal::Instance().DBControl.Disconnect();
		Close();
	}
	else
	{
		// Display Launch List
		TMMContactInfo TempUserInfo;
		bool Allowed = false;
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
		TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckQuit);
		DBTransaction.Commit();
		if (Result == lsAccepted)
		{
			Allowed = true;
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access rights to the Start Bar.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}

		if(Allowed)
		{
			std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

			TVerticalSelection Item;
			Item.Title = "Cancel";
			Item.Properties["Color"] = IntToStr(CL_BUTTONCOLOUR_UNSELECTED);
			Item.CloseSelection = true;
			SelectionForm->Items.push_back(Item);

			TVerticalSelection Item1;
			Item1.Title = "Show Desktop";
			Item1.Properties["Action"] = IntToStr(1);
			Item1.Properties["Color"] = IntToStr(CL_BUTTONCOLOUR_UNSELECTED);
			Item1.CloseSelection = true;
			SelectionForm->Items.push_back(Item1);

			Item1.Title = "Quit MenuMate";
			Item1.Properties["Action"] = IntToStr(2);
			Item1.Properties["Color"] = IntToStr(CL_BUTTONCOLOUR_CANCEL);
			Item1.CloseSelection = true;
			SelectionForm->Items.push_back(Item1);

			SelectionForm->ShowModal();
			TVerticalSelection SelectedItem;
			if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
			{
				int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
				switch(Action)
				{
				case 1:
					{
						Application->Minimize();
						SendMessage(frmMain->Handle,
						WM_SYSCOMMAND,
						SC_TASKLIST,0);
					}
					break;
				case 2:
					{
						btnExitClick(NULL);
					}
					break;
				}
			}
		}
	}
}

void __fastcall TfrmMain::ImageTitleClick(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tiShowMsgTimer(TObject *Sender)
{
	tiShowMsg->Enabled = false;
	if( MessagesToBroadcast() )
	{
		BroadcastMessage();
	}
	tiShowMsg->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tiFailedXeroInvoicesTimer(TObject *Sender)
{
	tiFailedXeroInvoices->Enabled = false;
	if( FailedXeroInvoivesToSend() )
	{
		SendFailedXeroInvoices();
	}
	tiFailedXeroInvoices->Enabled = true;
}
//---------------------------------------------------------------------------
bool TfrmMain::MessagesToBroadcast()
{
	return TMMessageFromFileManager::Instance().MessagesInQueue;
}
//---------------------------------------------------------------------------
void TfrmMain::BroadcastMessage()
{
	return TMMessageFromFileManager::Instance().ShowMessage();
}
//---------------------------------------------------------------------------
void TfrmMain::InitXeroIntegration()
{
	TXeroIntegration::Instance().XeroMachineName = TGlobalSettings::Instance().XeroMachineName;
	TXeroIntegration::Instance().XeroFolderPath  = TGlobalSettings::Instance().XeroFolderPath;
	TXeroIntegration::Instance().XeroUserName    = TGlobalSettings::Instance().XeroUserName;
	TXeroIntegration::Instance().XeroPassword    = TGlobalSettings::Instance().XeroPassword;
	TXeroIntegration::Instance().Validate();
	TMYOBIntegration::Instance().MYOBMachineName = TGlobalSettings::Instance().MYOBMachineName;
	TMYOBIntegration::Instance().MYOBFolderPath  = TGlobalSettings::Instance().MYOBFolderPath;
	TMYOBIntegration::Instance().MYOBUserName    = TGlobalSettings::Instance().MYOBUserName;
	TMYOBIntegration::Instance().MYOBPassword    = TGlobalSettings::Instance().MYOBPassword;
	TMYOBIntegration::Instance().Validate();
}
//---------------------------------------------------------------------------
bool TfrmMain::FailedXeroInvoivesToSend()
{
	return TXeroIntegration::Instance().FailedInvoicesInQueue;
}
//---------------------------------------------------------------------------
void TfrmMain::SendFailedXeroInvoices()
{
	ResetFailedXeroInvoiceTimerInterval( TXeroIntegration::Instance().SendNextFailedXeroInvoice() );
}
//---------------------------------------------------------------------------
void TfrmMain::ResetFailedXeroInvoiceTimerInterval( unsigned inInvoiceCount )
{
	if( TXeroIntegration::Instance().XeroErrorMessage == XI_ERROR_NO_ERROR )
	{
		tiFailedXeroInvoices->Interval = inInvoiceCount > 0 ? TXeroIntegration::Instance().FailedActiveInterval : TXeroIntegration::Instance().FailedPauseInterval;
	}
	else
	{
		tiFailedXeroInvoices->Interval = TXeroIntegration::Instance().FailedFolderNotFoundInterval;
	}
}
//---------------------------------------------------------------------------
void TfrmMain::initProxyMateManager()
{
	proxyMateManager->Init();
}
//---------------------------------------------------------------------------
bool TfrmMain::proxyMateWillHandle( tagMSG &inMsg )
{
	if( proxyMateManager->AppConnectorServer
			&& proxyMateManager->AppConnectorServer->WillHandle( inMsg ) )
	{
		SendMessage( proxyMateManager->AppConnectorServer->WMsgHandle, inMsg.message, inMsg.wParam, inMsg.lParam );
		return true;
	}

	if( proxyMateManager->FPConnector && ( proxyMateManager->FPConnector->WillHandle( inMsg ) ) )
	{
		SendMessage( proxyMateManager->FPConnector->WMsgHandle, inMsg.message, inMsg.wParam, inMsg.lParam );
		return true;
	}

	if( proxyMateManager->TPConnector && ( proxyMateManager->TPConnector->WillHandle( inMsg ) ) )
	{
		SendMessage( proxyMateManager->TPConnector->WMsgHandle, inMsg.message, inMsg.wParam, inMsg.lParam );
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
void TfrmMain::OpenFPConnector()
{
	fpConnectorUp = false;
	fpOpen        = false;

	unsigned __int32 params[2];
	params[0] = 128; //maxLocations;
	params[1] = 128; //maxTablesInLocation;

	TProxyMateManager::Instance().AppConnectorServer->OpenConnectorAck = mmAppConnectorOpenConnectorAck;
	fpConnector = ( TMMFloorPlanConnectorServer* )TProxyMateManager::Instance().AppConnectorServer->OpenConnector( mmcFloorPlan, params, 2 );

	if( fpConnector != NULL )
	{
		fpConnector->OnClientUp           = fpClientUp;
		fpConnector->OnClientDown         = fpClientDown;
		fpConnector->OnUpdatedTablesEvent = fpUpdatedTables;
	}
}
//---------------------------------------------------------------------------
void TfrmMain::CloseFPConnector()
{
	TProxyMateManager::Instance().AppConnectorServer->CloseConnector( mmcFloorPlan );
	TProxyMateManager::Instance().AppConnectorServer->OpenConnectorAck = NULL;

	fpConnector   = NULL;
	fpConnectorUp = false;
	fpOpen        = false;
}
//---------------------------------------------------------------------------
void TfrmMain::showFloorPlan()
{
	if( fpConnectorUp )
	{
		showNewFloorPlan( fpConnector );
	}
	else
	{
		showOldFloorPlan();
	}
}
//---------------------------------------------------------------------------
void TfrmMain::showNewFloorPlan( TMMFloorPlanConnectorServer* inConnector )
{
	if( !fpOpen )
	{
		std::vector<TLocationStatus> currentTables;
		//setCurrentTables( currentTables );
		inConnector->ShowFloorPlan( currentTables );
		fpOpen = true;

		currentTables.clear();
	}
}
//---------------------------------------------------------------------------
void TfrmMain::showOldFloorPlan()
{
	TMMContactInfo TempUserInfo;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	TLoginSuccess Result = Staff->Login(this,DBTransaction,TempUserInfo, CheckPOS);
	DBTransaction.Commit();
	if (Result == lsAccepted)
	{
		TDeviceRealTerminal::Instance().User = TempUserInfo;

		HWND PrevWnd;

		PrevWnd = FindWindow(_T("TfrmMain"), _T("Menumate Floor Plan"));
		if (PrevWnd != NULL)
		{
			PrevWnd = GetWindow(PrevWnd, GW_OWNER);
			if (PrevWnd != NULL)
			{
				if (IsIconic(PrevWnd))
				{
					ShowWindow(PrevWnd, SW_RESTORE);
				}
				else
				{
					SetForegroundWindow(PrevWnd);
				}
			}
		}
		else
		{
			_wspawnl(
			P_NOWAIT,
			(ExtractFilePath(Application->ExeName) + "\\Floorplan\\GuiClient\\FloorPlanEditor.exe").w_str(),
			(ExtractFilePath(Application->ExeName) + "\\Floorplan\\GuiClient\\FloorPlanEditor.exe").w_str(),
			NULL);
			::Sleep(1000);
			PrevWnd = FindWindow(_T("TfrmMain"), _T("Menumate Floor Plan"));
			if (PrevWnd != NULL)
			{
				PrevWnd = GetWindow(PrevWnd, GW_OWNER);
				if (PrevWnd != NULL)
				{
					if (IsIconic(PrevWnd))
					{
						ShowWindow(PrevWnd, SW_RESTORE);
					}
					else
					{
						SetForegroundWindow(PrevWnd);
					}
				}
			}
		}
		frmSecurity->LogOut();
	}
	else if (Result == lsDenied)
	{
		MessageBox("You do not have access to Setup.", "Error", MB_OK + MB_ICONERROR);
	}
	else if (Result == lsPINIncorrect)
	{
		MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::
// Customer Display
//::::::::::::::::::::::::::

void TfrmMain::openCustomerDisplayServer()
{
	if( TGlobalSettings::Instance().ShowCustomerDisplay )
	{
		customer_display_manager = TMMCustomerDisplayManager::Instance();
		customer_display_manager->OpenCustomerDisplay();
	}
}
//.............................................................................
void TfrmMain::closeCustomerDisplayServer()
{
	if( TGlobalSettings::Instance().ShowCustomerDisplay )
	{
		customer_display_manager = TMMCustomerDisplayManager::Instance();
		customer_display_manager->CloseCustomerDisplay();
		TMMCustomerDisplayManager::Done();
	}
}
//.............................................................................
void __fastcall TfrmMain::mmAppConnectorOpenConnectorAck( System::TObject* Sender, int inACK, TMMConnectorTypes inConnectorType, TMMConnectorServer* inConnector  )
{
	switch( inConnectorType )
	{
	case mmcFloorPlan: fpConnectorUp = true; break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::fpClientUp( TObject* Sender )
{
	fpConnectorUp = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::fpClientDown( TObject* Sender  )
{
	// Might not be needed
	fpConnectorUp = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::fpUpdatedTables( TObject* Sender, TLocationStatus* inUpdatedTables, int inCount )
{
	fpOpen = false;
}
//---------------------------------------------------------------------------
// For Mall Export
// This function is for prompting an EOD message that the Z-Reading of the
// previous day is not yet performed.
void TfrmMain::ZForced()
{
	MessageBox( "Z-Reading must be done before using the POS.", "Last Z-Reading was not done", MB_OK );
	EnablePOSBtn( false );
}
//---------------------------------------------------------------------------
void TfrmMain::EnablePOSBtn( bool inEnabled )
{
	Touchcontrols::TGridButton *gridBtn = tgridMenu->Buttons[0][0];
	gridBtn->Enabled = inEnabled;
}
//---------------------------------------------------------------------------
void TfrmMain::InitializeExportTimings(bool FirstMallSet)
{
	std::auto_ptr<TMallExportManager> MEM(new TMallExportManager());

	if(TGlobalSettings::Instance().MallIndex != NOMALL && TGlobalSettings::Instance().MallIndex != DLFMALL)
	{
		MEM->IMallManager->SetExportTiming(tiMallExportTiming);
	}
	if((TGlobalSettings::Instance().MallIndex == ROBINSONMALL) || (TGlobalSettings::Instance().MallIndex == AYALAMALL) ||
       (TGlobalSettings::Instance().MallIndex == POWERPLANTMALL) || (TGlobalSettings::Instance().MallIndex == MEGAWORLDMALL) ||
       (TGlobalSettings::Instance().MallIndex == SHANGRILAMALL) || (TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL))
	{
		MEM->IMallManager->SetTimingZedCheck(tiCheckZedRead,FirstMallSet);
	}
}
//---------------------------------------------------------------------------
// This function is for saving the integer global variable
void TfrmMain::SaveBoolVariable(vmVariables vmVar, bool CompName)
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
	if(GlobalProfileKey == 0)
	{
		GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
	}
	TManagerVariable::Instance().SetProfileBool(DBTransaction, GlobalProfileKey, vmVar, CompName);
	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
// This method disable the POS button depending on the mall schedule
void TfrmMain::EnablePOSForSpecifiedMallSchedule()
{
	int Hour = StrToInt(Now().FormatString("hh"));
	bool TodaysZed = false;

	std::auto_ptr<TMallExportManager> MEM(new TMallExportManager());

	if (TGlobalSettings::Instance().MallIndex == ROBINSONMALL)
	{
		TodaysZed = MEM->IMallManager->CheckTodaysZed();

		if(((Hour > 3) && (Hour < 9)) || (TodaysZed))
		{
			EnablePOSBtn( false );
		}
	}
}
//---------------------------------------------------------------------------
bool TfrmMain::MenusToStockExchangeFolderExists()
{
	return DirectoryExists( TGlobalSettings::Instance().BarExchangePath );
}

bool TfrmMain::MenusToStockExchange()
{
	bool result = false;
	TSearchRec sr;
	int iAttributes = faAnyFile;

	if( FindFirst( TGlobalSettings::Instance().BarExchangePath + "\\*.PLU", iAttributes, sr ) == 0 )
	{
		result = ( sr.Attr & iAttributes );
	}
	FindClose(sr);
	return result;
}

void TfrmMain::ImportStockExchange()
{
	AnsiString dirName = FMenusStockExchange;
	AnsiString FileName;
	if( TGlobalSettings::Instance().BarExchangeSelect=="None" )
	{
		return;
	}
	if( TGlobalSettings::Instance().BarExchangeSelect=="BarStock" )
	{
		FileName = TGlobalSettings::Instance().BarExchangePath + "\\" + "BS.PLU";

	}
	else if( TGlobalSettings::Instance().BarExchangeSelect=="The Exchange")
	{
		FileName = TGlobalSettings::Instance().BarExchangePath + "\\" + "EX.PLU";

	}
	if( FileExists( FileName ) )
	{
		Database::TDBTransaction DBTransaction( TDeviceRealTerminal::Instance().DBControl );
		DBTransaction.StartTransaction();

		TDeviceRealTerminal::Instance().UpdateBarStockPrice(DBTransaction, FileName);
        TDeviceRealTerminal::Instance().Menus->SwapInNewMenus();
        DBTransaction.Commit();
        DBTransaction.StartTransaction();
		std::auto_ptr<TNetMessageMenuChanged> Request( new TNetMessageMenuChanged );
		Request->Broadcast         = true;
		Request->CompareToDataBase = false;
        for (int i = 0; i < TDeviceRealTerminal::Instance().Menus->Current->Count; i++)
           {
              TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
              Request->Menu_Names[Menu->MenuName] = eMenuAddReplace;
           }
        TDeviceRealTerminal::Instance().Menus->MenuAddedRemoved( DBTransaction,Request.get() );
        DBTransaction.Commit();

	}
}

void TfrmMain::CreateMenusToExchangeStockFolder()
{
	CreateDir( TGlobalSettings::Instance().BarExchangePath );
}
void __fastcall TfrmMain::tiBarStockTimer(TObject *Sender)
{
	if(!TGlobalSettings::Instance().BarExchangeEnabled)
	return;

	if( TDeviceRealTerminal::Instance().DBControl.Connected() ) // If we not connected don't bother.
	{
		tiBarStock->Enabled = false;
		//FMenusStockExchange
		if( MenusToStockExchangeFolderExists() )
		{
			if( MenusToStockExchange() )
			{
				ImportStockExchange();
			}
		}
		else
		{
			CreateMenusToExchangeStockFolder();
		}
		tiBarStock->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tiBarStockturnoverTimer(TObject *Sender)
{
	if(!TGlobalSettings::Instance().BarExchangeEnabled)
	return;

    if(!TGlobalSettings::Instance().BarExchangeServerEnabled)
    return;
    	tiBarStock->Enabled = false;
	AnsiString FileName = TGlobalSettings::Instance().BarExchangePath + "\\" + "turnover.dat";
	Database::TDBTransaction DBTransaction( TDeviceRealTerminal::Instance().DBControl );
	DBTransaction.StartTransaction();
	TDeviceRealTerminal::Instance().SelectBarStockTurnOver(DBTransaction,FileName);
	DBTransaction.Commit();
	tiBarStock->Enabled = true;
}
//---------------------------------------------------------------------------
void TfrmMain::ReFormatIpToUrl()
{
    if(TGlobalSettings::Instance().PMSType == SiHot)
    {
        Database::TDBTransaction DBTransaction( TDeviceRealTerminal::Instance().DBControl );
        DBTransaction.StartTransaction();
        try
        {
            AnsiString ipAddress = TManagerVariable::Instance().GetStr(DBTransaction,vmPMSIPAddress);
            int portNumber = TManagerVariable::Instance().GetInt(DBTransaction,vmPMSTCPPort);
            if(ipAddress.Pos(":") == 0 && ipAddress.Length() != 0 )
            {
                AnsiString url = "http://"+ipAddress+":"+portNumber+"/RMS";
                TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress = url.Trim();
                TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPMSIPAddress,TDeviceRealTerminal::Instance().BasePMS->TCPIPAddress);
            }
            DBTransaction.Commit();
        }
		catch(Exception &E)
		{
            DBTransaction.Rollback();
			TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
		}
    }
}
//---------------------------------------------------------------------------
void TfrmMain::SaveItemPriceIncludeTaxToDatabase(vmVariables vmVariable, bool value)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
    if(GlobalProfileKey == 0)
    {
	    GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
    }

    TManagerVariable::Instance().SetProfileBool(DBTransaction, GlobalProfileKey, vmVariable, value);

	DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TfrmMain::EnableOnlineOrdering()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        if(SyncOnlineOrderingDetails())
        {
            std::auto_ptr<TSignalRUtility> signalRUtility(new TSignalRUtility());
            if(signalRUtility->LoadSignalRUtility())
            {
                TGlobalSettings::Instance().EnableOnlineOrdering = true;
            }
        }
        else
        {
            TGlobalSettings::Instance().EnableOnlineOrdering = false;
        }
        TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmEnableOnlineOrdering, TGlobalSettings::Instance().EnableOnlineOrdering);
        DBTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        TGlobalSettings::Instance().EnableOnlineOrdering = false;
        DBTransaction.Rollback();
	}
}
//-----------------------------------------------------------------------------
void TfrmMain::DisableOnlineOrdering()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        UnloadSignalR();
        TGlobalSettings::Instance().EnableOnlineOrdering = false;
        TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmEnableOnlineOrdering, TGlobalSettings::Instance().EnableOnlineOrdering);
        DBTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        TGlobalSettings::Instance().EnableOnlineOrdering = false;
        DBTransaction.Rollback();
	}
}
//-----------------------------------------------------------------------------
void TfrmMain::UnloadSignalR()
{
    std::auto_ptr<TSignalRUtility> signalRUtility(new TSignalRUtility());
    signalRUtility->UnloadSignalRUtility();
}
//-----------------------------------------------------------------------------
bool TfrmMain::SyncOnlineOrderingDetails()
{
    bool result = false;
    TManagerCloudSync ManagerCloudSync;
    result = ManagerCloudSync.SyncOnlineOrderingDetails();
    return result;
}
//-----------------------------------------------------------------------------
void TfrmMain::WriteDBPathAndIPToFile()
{
    AnsiString DirectoryName = ExtractFilePath(Application->ExeName) + "/Menumate Services/MenumateDBPath";
    if (!DirectoryExists(DirectoryName))
        CreateDir(DirectoryName);
    AnsiString fileName =  DirectoryName + "/" + "DBPathAndIP.txt";

    std::auto_ptr <TStringList> logList(new TStringList);
    logList->Add(TGlobalSettings::Instance().InterbaseIP);
    logList->Add(TGlobalSettings::Instance().DatabasePath);
    if(TGlobalSettings::Instance().ReservationsEnabled)
        logList->Add(ExtractFilePath(Application->ExeName));
    logList->SaveToFile(fileName );
}
////------------------------------------------------------------------------------------------
void TfrmMain::SetUpAustriaFiscal()
{
    // Call for Setting up Austria Fiscal
}
//------------------------------------------------------------------------------
