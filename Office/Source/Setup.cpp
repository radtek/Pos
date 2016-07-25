//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <FileCtrl.hpp>
#include <Registry.hpp>
#include <memory>
#include "MM_BrowseDirectory.h"
#include "Setup.h"
#include "MMRegistry.h"
#include "Consts.h"
#include "MMData.h"
#include "StockData.h"
#include "ChefMateData.h"
#include "Splash.h"
#include "SelectCompany.h"
#include "SetupLocations.h"
#include "Users.h"
#include "ConfigureRolesForm.h"
//#include "CreateTaxProfile.h"
#include "TaxProfileController.h"
#include "TaxProfileGUI.h"
//#include "ConfigureRolesController.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CPortCtl"
#pragma link "NumericEdit"
#pragma resource "*.dfm"

TfrmSetup *frmSetup;
//---------------------------------------------------------------------------
__fastcall TfrmSetup::TfrmSetup(TComponent* Owner)
	: TForm(Owner)
{
	CompanyDetailsList	= new TList;
	ServersList				= new TStringList;
	RetrieveNamesThread	= new TRetrieveNamesThread(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::FormCreate(TObject *Sender)
{
	RetrieveNamesThread->ComputerNames		= ServersList;
	RetrieveNamesThread->FreeOnTerminate	= true;
	RetrieveNamesThread->OnTerminate			= RetrieveNamesThreadTerminate;
	RetrieveNamesThread->Resume();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::FormDestroy(TObject *Sender)
{
	for (int i=0; i<CompanyDetailsList->Count; i++)
	{
		delete (TConnectionDetails *)CompanyDetailsList->Items[i];
	}
	delete CompanyDetailsList;

	if (RetrieveNamesThread)
	{
		RetrieveNamesThread->Terminate();
		unsigned long TickCount = GetTickCount();
		while (RetrieveNamesThread && GetTickCount() < (TickCount + 1000))
		{
			Application->ProcessMessages();
			Sleep(0);
		}
//		if (RetrieveNamesThread)
//		{
//			TerminateThread((void *)RetrieveNamesThread->Handle, 0);
//		}
	}
	delete ServersList;
	ServersList = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::RetrieveNamesThreadTerminate(TObject *Sender)
{
	RetrieveNamesThread = NULL;
//	delete ServersList;
//	ServersList = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::FormShow(TObject *Sender)
{
	FormResize(NULL);
	pcSettings->ActivePage = tsMMConnection;
	LoadSettings();
	tcCompanyNames->TabIndex = tcCompanyNames->Tabs->IndexOf(CurrentConnection.CompanyName);
	if (tcCompanyNames->TabIndex == -1) tcCompanyNames->TabIndex = 0;
	tcCompanyNamesChange(NULL);
}
//---------------------------------------------------------------------------
void TfrmSetup::LoadSettings()
{
	tcCompanyNames->Tabs->Clear();
	for (int i=0; i<CompanyDetailsList->Count; i++)
	{
		delete (TConnectionDetails *)CompanyDetailsList->Items[i];
	}
	CompanyDetailsList->Clear();
	TStringList *CompanyNames = new TStringList;
	try
	{
		if (RegistryGetKeys(OfficeKey, CompanyNames))
		{
			for (int i=0; i<CompanyNames->Count; i++)
			{
				tcCompanyNames->Tabs->Add(CompanyNames->Strings[i]);

				TConnectionDetails *CompanyDetails = new TConnectionDetails;
				CompanyDetails->CompanyName = CompanyNames->Strings[i];
				LoadSettings(CompanyDetails);
				CompanyDetailsList->Add(CompanyDetails);
			}
		}
	}
	__finally
	{
		delete CompanyNames;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::tcCompanyNamesChange(TObject *Sender)
{
    char str[2];
	TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		memCompanyDetails->Lines->Text		= CompanyDetails->ReportHeader;

		int Pos = CompanyDetails->MenuMateDB.DBName.AnsiPos(":\\");
		if (Pos == 0 || Pos == 2)
		{
			cbMMDBServer->Text					= "";
			edMMDBName->Text					= CompanyDetails->MenuMateDB.DBName;
		}
		else
		{
			int ServerPos						= CompanyDetails->MenuMateDB.DBName.AnsiPos(":");

			cbMMDBServer->Text					= CompanyDetails->MenuMateDB.DBName.SubString(1, ServerPos-1);
			edMMDBName->Text					= CompanyDetails->MenuMateDB.DBName.SubString(ServerPos+1, CompanyDetails->MenuMateDB.DBName.Length() - (ServerPos-1));
		}
		Pos = CompanyDetails->StockDB.DBName.AnsiPos(":\\");
		if (Pos == 0 || Pos == 2)
		{
			cbStockDBServer->Text				= "";
			edStockDBName->Text					= CompanyDetails->StockDB.DBName;
		}
		else
		{
			int ServerPos						= CompanyDetails->StockDB.DBName.AnsiPos(":");

			cbStockDBServer->Text				= CompanyDetails->StockDB.DBName.SubString(1, ServerPos-1);
			edStockDBName->Text					= CompanyDetails->StockDB.DBName.SubString(ServerPos+1, CompanyDetails->StockDB.DBName.Length() - (ServerPos-1));
		}
		Pos = CompanyDetails->ChefMateDB.DBName.AnsiPos(":\\");
		if (Pos == 0 || Pos == 2)
		{
			cbChefMateDBServer->Text			= "";
			edChefMateDBName->Text				= CompanyDetails->ChefMateDB.DBName;
		}
		else
		{
			int ServerPos						= CompanyDetails->ChefMateDB.DBName.AnsiPos(":");

			cbChefMateDBServer->Text			= CompanyDetails->ChefMateDB.DBName.SubString(1, ServerPos-1);
			edChefMateDBName->Text				= CompanyDetails->ChefMateDB.DBName.SubString(ServerPos+1, CompanyDetails->ChefMateDB.DBName.Length() - (ServerPos-1));
		}

//		edMMDBName->Text							= CompanyDetails->MenuMateDB.DBName;
//		edStockDBName->Text						= CompanyDetails->StockDB.DBName;

		edCommitPath->Text						= CompanyDetails->ServerPath;
		edMenuSavePath->Text						= CompanyDetails->MenuSavePath;

		if (CompanyDetails->UseSerialBarcodeReader)
		{
			cbBarcodeSerialPort->ItemIndex = CompanyDetails->SerialBarcodeReaderPort;
		}
		else
		{
			cbBarcodeSerialPort->ItemIndex = 0;
		}

//		cbGST->Checked								= CompanyDetails->IncludeGST;
		chbHideStocktakeOnHand->Checked		= CompanyDetails->HideStocktakeOnHand;

		edGSTNumber->Text							= CompanyDetails->GSTNumber;
		edVIPCreditName->Text               = CompanyDetails->VIPCreditName;

		edDefaultMenuGST->Text					= FloatToStrF(CompanyDetails->DefaultMenuGST, ffFixed, 15, 2);
		edDefaultStockGST->Text					= FloatToStrF(CompanyDetails->DefaultStockGST, ffFixed, 15, 2);

		chbSingleLocation->Checked				= CompanyDetails->SingleLocation;
      cbDefaultLocation->Text					= CompanyDetails->DefaultLocation;

		cbPayrollSystem->ItemIndex				= cbPayrollSystem->Items->IndexOf(CompanyDetails->PayrollSystem);
		edExportPath->Text						= ExcludeTrailingBackslash(CompanyDetails->PayrollExportPath);

		cbAccountSystem->ItemIndex				= cbAccountSystem->Items->IndexOf(CompanyDetails->AccountSystem);
		edMYOBPath->Text							= ExcludeTrailingBackslash(CompanyDetails->MYOBPath);
		edMYOBJobCode->Text						= CompanyDetails->MYOBJobCode;

		edStocktakePath->Text					= ExcludeTrailingBackslash(CompanyDetails->StocktakePath);
		edStocktakeExportFile->Text			= CompanyDetails->StocktakeExportFile;
		edStocktakeImportFile->Text			= CompanyDetails->StocktakeImportFile;
		cbStocktakeBarcodePos->ItemIndex		= CompanyDetails->StocktakeBarcodePos;
		cbStocktakeQtyPos->ItemIndex			= CompanyDetails->StocktakeQtyPos;
         	RBRoundingPoint->ItemIndex	=  StrToInt(CompanyDetails->SettingDecimalPlaces)==2?0:1;
        chbAutoPrintStockTransferAudit->Checked				= CompanyDetails->AutoPrintStockTransferAudit;

         chbAutoPrintReceiveTransferAudit->Checked				= CompanyDetails->AutoPrintReceiveTransferAudit;
         chbSuppliersFromDefaultLocationsOnly->Checked				= CompanyDetails->SuppliersFromDefaultLocationsOnly;
      //   edNoOfPriceLevels->Text					= itoa(CompanyDetails->NoOfPriceLevels, str, 10);

        if (sqlGetNoOfPriceLevels->Database->Connected && !sqlGetNoOfPriceLevels->Transaction->InTransaction)
        {
            sqlGetNoOfPriceLevels->Transaction->StartTransaction();
            sqlGetNoOfPriceLevels->Close();
            sqlGetNoOfPriceLevels->ExecQuery();
            if(sqlGetNoOfPriceLevels->RecordCount > 0)
            {
                edNoOfPriceLevels->Value					=	 sqlGetNoOfPriceLevels->Fields[0]->AsInteger;//, str, 10);
            }
            else
            {
                 edNoOfPriceLevels->Value					=  2;
            }
            sqlGetNoOfPriceLevels->Transaction->Commit();
        }


		cbDefaultLocation->Items->Clear();
		if (CurrentConnection.CompanyName == CompanyDetails->CompanyName)
		{
			if (sqlLocation->Database->Connected && !sqlLocation->Transaction->InTransaction)
			{
				sqlLocation->Transaction->StartTransaction();
				sqlLocation->Close();
				for (sqlLocation->ExecQuery(); !sqlLocation->Eof; sqlLocation->Next())
				{
					cbDefaultLocation->Items->Add(sqlLocation->FieldByName("Name")->AsString);
				}
				sqlLocation->Transaction->Commit();
			}
		}

		try
		{
			LoadRASEntryLists(CompanyDetails);
			RASAvailable = true;
			chbMMRemote->Enabled = true;
			chbStockRemote->Enabled = true;
		}
		catch (Exception &E)
		{
			RASAvailable = false;
		}
		if (!RASAvailable)
		{
			chbMMRemote->Checked = false;
			chbMMRemote->Enabled = false;
			chbStockRemote->Checked = false;
			chbStockRemote->Enabled = false;
		}
		else
		{
			chbMMRemote->Checked = false;
			for (int i=0; i<cbMMRASEntry->Items->Count; i++)
			{
				if (cbMMRASEntry->Items->Strings[i] == CompanyDetails->MenuMateDB.RASEntry)
				{
					cbMMRASEntry->ItemIndex = i;
					AnsiString UserName, Password;
//					CompanyDetails->MenuMateDB.GetEntryDetails(UserName, Password);
//					if (Password == "") Password = CompanyDetails->MenuMateDB.RASPassword;
//					edMMRASUsername->Text = UserName;
//					edMMRASPassword->Text = Password;
					edMMRASUsername->Text = CompanyDetails->MenuMateDB.RASUserName;
					edMMRASPassword->Text = CompanyDetails->MenuMateDB.RASPassword;
//					try
//					{
//						chbMMRemote->OnClick = NULL;
						chbMMRemote->Checked = CompanyDetails->MenuMateDB.RemoteConnection;
//					}
//					__finally
//					{
//						chbMMRemote->OnClick = chbMMRemoteClick;
//					}
					break;
				}
			}
			chbStockRemote->Checked = false;
			for (int i=0; i<cbStockRASEntry->Items->Count; i++)
			{
				if (cbStockRASEntry->Items->Strings[i] == CompanyDetails->StockDB.RASEntry)
				{
					cbStockRASEntry->ItemIndex = i;
					AnsiString UserName, Password;
//					CompanyDetails->StockDB.GetEntryDetails(UserName, Password);
//					if (Password == "") Password = CompanyDetails->StockDB.RASPassword;
//					edStockRASUsername->Text = UserName;
//					edStockRASPassword->Text = Password;
					edStockRASUsername->Text = CompanyDetails->StockDB.RASUserName;
					edStockRASPassword->Text = CompanyDetails->StockDB.RASPassword;
//					try
//					{
//						chbStockRemote->OnClick = NULL;
						chbStockRemote->Checked = CompanyDetails->StockDB.RemoteConnection;
//					}
//					__finally
//					{
//						chbStockRemote->OnClick = chbStockRemoteClick;
//					}
					break;
				}
			}
			if (CompanyDetails->MenuMateDB.Connected)
			{
				btnDialMM->Enabled = false;
				btnHangUpMM->Enabled = chbMMRemote->Checked;
			}
			else
			{
				btnDialMM->Enabled = chbMMRemote->Checked;
				btnHangUpMM->Enabled = false;
			}
			if (CompanyDetails->StockDB.Connected)
			{
				btnDialStock->Enabled = false;
				btnHangUpStock->Enabled = chbStockRemote->Checked;
			}
			else
			{
				btnDialStock->Enabled = chbStockRemote->Checked;
				btnHangUpStock->Enabled = false;
			}



		}
	}
}
//---------------------------------------------------------------------------
void TfrmSetup::GetInterbasePathComponents(AnsiString ConnectionString, AnsiString *Server, AnsiString *Path)
{
	int Pos = ConnectionString.AnsiPos(":\\");
	if (Pos == 0)
	{
		*Server			= "";
		*Path				= ConnectionString;
	}
	else if (Pos == 2)
	{
		*Server			= "";
		*Path				= ConnectionString.SubString(4, ConnectionString.Length() - 3);
	}
	else
	{
		int ServerPos	= ConnectionString.AnsiPos(":");

		*Server			= ConnectionString.SubString(1, ServerPos-1);
		*Path				= ConnectionString.SubString(ServerPos+1, ConnectionString.Length() - (ServerPos-1));
	}
}
//---------------------------------------------------------------------------
void TfrmSetup::LoadRASEntryLists(TConnectionDetails *CompanyDetails)
{
	try
	{
		cbMMRASEntry->OnChange = NULL;
		cbStockRASEntry->OnChange = NULL;

//		cbMMRASEntry->Items->Clear();
//		cbStockRASEntry->Items->Clear();

		CompanyDetails->RASGetEntries(cbMMRASEntry->Items);
		CompanyDetails->RASGetEntries(cbStockRASEntry->Items);
	}
	__finally
	{
		cbMMRASEntry->OnChange = cbMMRASEntryChange;
		cbStockRASEntry->OnChange = cbStockRASEntryChange;
	}
}
//---------------------------------------------------------------------------
void TfrmSetup::LoadSettings(TConnectionDetails *CompanyDetails)
{
	AnsiString Key = OfficeKey + "\\" + CompanyDetails->CompanyName;
	AnsiString Text;
	bool Found;

	Text = "";
	RegistryRead(Key, "CompanyDetails", Text);
	CompanyDetails->ReportHeader = Text;

	Text = "";
	RegistryRead(Key, "MenuCommitPath", Text);
	if (Text == "" && FirstRun)
	{
		Text = "C:\\Program Files\\MenuMate";
	}
	CompanyDetails->ServerPath = Text;
	Text = "";
	RegistryRead(Key, "MenuSavePath", Text);
	if (Text == "" && FirstRun)
	{
		Text = "C:\\Program Files\\MenuMate";
	}
	CompanyDetails->MenuSavePath = Text;
	Text = "";
	RegistryRead(Key, "MMDataFile", Text);
	if (Text == "" && FirstRun)
	{
		Text = "localhost:C:\\Program Files\\MenuMate\\MenuMate.fdb";
	}
	CompanyDetails->MenuMateDB.DBName = Text;
	Text = "";
	RegistryRead(Key, "StockDataFile", Text);
	if (Text == "" && FirstRun)
	{
		Text = "localhost:C:\\Program Files\\MenuMate\\Stock.fdb";
	}
	CompanyDetails->StockDB.DBName = Text;
	Text = "";
	RegistryRead(Key, "ChefMateDataFile", Text);
	if (Text == "" && FirstRun)
	{
		Text = "localhost:C:\\Program Files\\MenuMate\\ChefMate.fdb";
	}
	CompanyDetails->ChefMateDB.DBName = Text;

	Text = "";
	RegistryRead(Key + "\\Payroll", "PayrollSystem", Text);
	CompanyDetails->PayrollSystem = Text;
	Text = "";
	RegistryRead(Key + "\\Payroll", "ExportPath", Text);
	CompanyDetails->PayrollExportPath = Text;

	Text = "";
	RegistryRead(Key + "\\MYOB", "AccountSystem", Text);
	CompanyDetails->AccountSystem = Text;
	Text = "";
	RegistryRead(Key + "\\MYOB", "MYOBPath", Text);
	CompanyDetails->MYOBPath = Text;
	Text = "";
	RegistryRead(Key + "\\MYOB", "MYOBJobCode", Text);
	CompanyDetails->MYOBJobCode = Text;
	Text = "";
	RegistryRead(Key + "\\MYOB", "SalesInvoiceExportAccount", Text);
	CompanyDetails->SalesInvoiceExportAccount = Text;
	Text = "";
	RegistryRead(Key + "\\MYOB", "SalesInvoiceExportType", Text);
	CompanyDetails->SalesInvoiceExportType = Text;

	Text = "";
	RegistryRead(Key + "\\Stocktake", "StocktakePath", Text);
	CompanyDetails->StocktakePath = Text;
	Text = "Product.txt";
	RegistryRead(Key + "\\Stocktake", "StocktakeExportFile", Text);
	CompanyDetails->StocktakeExportFile = Text;
	Text = "Stock.txt";
	RegistryRead(Key + "\\Stocktake", "StocktakeImportFile", Text);
	CompanyDetails->StocktakeImportFile = Text;
	Text = "-1";
	RegistryRead(Key + "\\Stocktake", "StocktakeBarcodePos", Text);
	CompanyDetails->StocktakeBarcodePos = StrToInt(Text);
	Text = "-1";
	RegistryRead(Key + "\\Stocktake", "StocktakeQtyPos", Text);
	CompanyDetails->StocktakeQtyPos = StrToInt(Text);

	Text = "0";
	RegistryRead(Key, "UseSerialBarcodeReader", Text);
	CompanyDetails->UseSerialBarcodeReader = (Text == "1");
	Text = "-1";
	RegistryRead(Key, "SerialBarcodeReaderPort", Text);
	CompanyDetails->SerialBarcodeReaderPort = StrToInt(Text);

	Text = "0";
	RegistryRead(Key, "HideStocktakeOnHand", Text);
	CompanyDetails->HideStocktakeOnHand = (Text == "1");

	Text = "";
	RegistryRead(Key, "VIPCreditName", Text);
	CompanyDetails->VIPCreditName = Text;

	Text = "";
	RegistryRead(Key, "GSTNumber", Text);
	CompanyDetails->GSTNumber = Text;

	Text = "15";
	RegistryRead(Key, "DefaultMenuGST", Text);
	CompanyDetails->DefaultMenuGST	= StrToFloat(Text);
	Text = "15";
	RegistryRead(Key, "DefaultStockGST", Text);
	CompanyDetails->DefaultStockGST	= StrToFloat(Text);

	Text = "0";
	RegistryRead(Key, "SingleLocation", Text);
	CompanyDetails->SingleLocation = (Text == "1");
	Text = "";
	RegistryRead(Key, "DefaultLocation", Text);
	CompanyDetails->DefaultLocation = Text;

	Text = "0";
	RegistryRead(Key, "RemoteMM", Text);
	CompanyDetails->MenuMateDB.RemoteConnection = (Text == "1");
	Text = "";
	RegistryRead(Key, "RemoteMMEntry", Text);
	CompanyDetails->MenuMateDB.RASEntry = Text;
	Text = "";
	RegistryRead(Key, "RemoteMMUsername", Text);
	CompanyDetails->MenuMateDB.RASUserName = Text;
	Text = "";
	RegistryRead(Key, "RemoteMMPassword", Text);
	CompanyDetails->MenuMateDB.RASPassword = Text;

	Text = "0";
	RegistryRead(Key, "RemoteStock", Text);
	CompanyDetails->StockDB.RemoteConnection = (Text == "1");
	Text = "";
	RegistryRead(Key, "RemoteStockEntry", Text);
	CompanyDetails->StockDB.RASEntry = Text;
	Text = "";
	RegistryRead(Key, "RemoteStockUsername", Text);
	CompanyDetails->StockDB.RASUserName = Text;
	Text = "";
	RegistryRead(Key, "RemoteStockPassword", Text);
	CompanyDetails->StockDB.RASPassword = Text;

	Text = "0";
	RegistryRead(Key, "RemoteChefMate", Text);
	CompanyDetails->ChefMateDB.RemoteConnection = (Text == "1");
	Text = "";
	RegistryRead(Key, "RemoteChefMateEntry", Text);
	CompanyDetails->ChefMateDB.RASEntry = Text;
	Text = "";
	RegistryRead(Key, "RemoteChefMateUsername", Text);
	CompanyDetails->ChefMateDB.RASUserName = Text;
	Text = "";
	RegistryRead(Key, "RemoteChefMatePassword", Text);
	CompanyDetails->ChefMateDB.RASPassword = Text;


    	Text = "0";
	RegistryRead(Key , "SettingDecimalPlaces", Text);
	CompanyDetails->SettingDecimalPlaces = StrToInt(Text);



    Text = "0";
	RegistryRead(Key, "AutoPrintStockTransferAudit", Text);
	CompanyDetails->AutoPrintStockTransferAudit = (Text == "1");
	Text = "";

    Text = "0";
	RegistryRead(Key, "AutoPrintReceiveTransferAudit", Text);
	CompanyDetails->AutoPrintReceiveTransferAudit = (Text == "1");
	Text = "";

    Text = "0";
	RegistryRead(Key, "SuppliersFromDefaultLocationsOnly", Text);
	CompanyDetails->SuppliersFromDefaultLocationsOnly = (Text == "1");
	Text = "";

   /* Text = "15";
	RegistryRead(Key , "NoOfPriceLevels", Text);
	CompanyDetails->NoOfPriceLevels = StrToInt(Text);*/

}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::FormResize(TObject *Sender)
{
/*	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;

		ScaleBy(Screen->Width, Temp);
	} */
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::chbMMRemoteClick(TObject *Sender)
{
	cbMMRASEntry->Enabled			= chbMMRemote->Checked;
	edMMRASUsername->Enabled		= chbMMRemote->Checked;
	edMMRASPassword->Enabled		= chbMMRemote->Checked;
//	btnAddRAS->Enabled				= chbMMRemote->Checked || chbStockRemote->Checked;
//	cbMMRASEntryChange(NULL);
	if (chbMMRemote->Checked)
	{
		cbMMRASEntryClick(NULL);
	}
	else
	{
		btnDialMM->Enabled = false;
		btnHangUpMM->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::StaticText1Click(TObject *Sender)
{
	chbMMRemote->Checked = !chbMMRemote->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::chbStockRemoteClick(TObject *Sender)
{
	cbStockRASEntry->Enabled		= chbStockRemote->Checked;
	edStockRASUsername->Enabled	= chbStockRemote->Checked;
	edStockRASPassword->Enabled	= chbStockRemote->Checked;
//	btnAddRAS->Enabled				= chbMMRemote->Checked || chbStockRemote->Checked;
//	cbStockRASEntryChange(NULL);
	if (chbStockRemote->Checked)
	{
		cbStockRASEntryClick(NULL);
	}
	else
	{
		btnDialStock->Enabled = false;
		btnHangUpStock->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::StaticText2Click(TObject *Sender)
{
	chbStockRemote->Checked = !chbStockRemote->Checked;
}
//---------------------------------------------------------------------------
TConnectionDetails *TfrmSetup::GetCurrentCompany()
{
	TConnectionDetails *CompanyDetails = NULL;
	for (int i=0; i<CompanyDetailsList->Count; i++)
	{
		if (((TConnectionDetails *)CompanyDetailsList->Items[i])->CompanyName == tcCompanyNames->Tabs->Strings[tcCompanyNames->TabIndex])
		{
			CompanyDetails = (TConnectionDetails *)CompanyDetailsList->Items[i];
			break;
		}
	}
	return CompanyDetails;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::cbMMRASEntryChange(TObject *Sender)
{
	TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		CompanyDetails->MenuMateDB.RASEntry = cbMMRASEntry->Text;

		AnsiString UserName, Password;
//		CompanyDetails->MenuMateDB.GetEntryDetails(UserName, Password);
//		edMMRASUsername->Text = UserName;
//		CompanyDetails->MenuMateDB.RASUserName = UserName;
//		edMMRASPassword->Text = Password;
//		CompanyDetails->MenuMateDB.RASPassword = Password;

		if (CompanyDetails->MenuMateDB.Connected)
		{
			btnDialMM->Enabled = false;
			btnHangUpMM->Enabled = chbMMRemote->Checked;
		}
		else
		{
			btnDialMM->Enabled = chbMMRemote->Checked;
			btnHangUpMM->Enabled = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::cbMMRASEntryClick(TObject *Sender)
{
	TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		if (CompanyDetails->MenuMateDB.Connected)
		{
			btnDialMM->Enabled = false;
			btnHangUpMM->Enabled = chbMMRemote->Checked;
		}
		else
		{
			btnDialMM->Enabled = chbMMRemote->Checked;
			btnHangUpMM->Enabled = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::cbStockRASEntryChange(TObject *Sender)
{
	TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		CompanyDetails->StockDB.RASEntry = cbStockRASEntry->Text;

		AnsiString UserName, Password;
//		CompanyDetails->StockDB.GetEntryDetails(UserName, Password);
//		edStockRASUsername->Text = UserName;
//		CompanyDetails->StockDB.RASUserName = UserName;
//		edStockRASPassword->Text = Password;
//		CompanyDetails->StockDB.RASPassword = Password;

		if (CompanyDetails->StockDB.Connected)
		{
			btnDialStock->Enabled = false;
			btnHangUpStock->Enabled = chbStockRemote->Checked;
		}
		else
		{
			btnDialStock->Enabled = chbStockRemote->Checked;
			btnHangUpStock->Enabled = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::cbStockRASEntryClick(TObject *Sender)
{
	TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		if (CompanyDetails->StockDB.Connected)
		{
			btnDialStock->Enabled = false;
			btnHangUpStock->Enabled = chbStockRemote->Checked;
		}
		else
		{
			btnDialStock->Enabled = chbStockRemote->Checked;
			btnHangUpStock->Enabled = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnAddRASClick(TObject *Sender)
{
	if (RASAvailable)
	{
		AnsiString OldMMEntry = cbMMRASEntry->Text;
		AnsiString OldStockEntry = cbStockRASEntry->Text;

		TConnectionDetails *CompanyDetails = GetCurrentCompany();
		if (CompanyDetails)
		{
			CompanyDetails->RASCreateEntry();
//			CompanyDetails->RASGetEntries(cbMMRASEntry->Items);
//			CompanyDetails->RASGetEntries(cbStockRASEntry->Items);
			LoadRASEntryLists(CompanyDetails);
		}
		bool Found = false;
		for (int i=0; i<cbMMRASEntry->Items->Count; i++)
		{
			if (cbMMRASEntry->Items->Strings[i] == OldMMEntry)
			{
				cbMMRASEntry->ItemIndex = i;
				Found = true;
				break;
			}
		}
		if (!Found)
		{
			cbMMRASEntryChange(NULL);
		}

		Found = false;
		for (int i=0; i<cbStockRASEntry->Items->Count; i++)
		{
			if (cbStockRASEntry->Items->Strings[i] == OldStockEntry)
			{
				cbStockRASEntry->ItemIndex = i;
				Found = true;
				break;
			}
		}
		if (!Found)
		{
			cbStockRASEntryChange(NULL);
		}
	}
	else
	{
		Application->MessageBox("Dial-up networking has not been installed on this computer.", "Error", MB_OK + MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnHelpRASClick(TObject *Sender)
{
	if (RASAvailable)
	{
		Application->MessageBox("To enable remote access to either the MenuMate or Stock Database, "
										"check the appropriate check box. You can then select a phonebook entry "
										"from the drop box. If a suitable entry doesn't exist, click 'New Phonebook "
										"Entry' and follow the wizard to add one. \r"
										"Once selected, enter the username and password for the Dial-in user on the remote "
										"machine. Make sure this user has Dial-in rights.",
										"Information", MB_ICONINFORMATION + MB_OK);
	}
	else
	{
		Application->MessageBox("Dial-up networking is not available on this computer. "
										"It must be installed before this facility is available.",
										"Information",
										MB_ICONINFORMATION + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnHelpMMDatabaseClick(TObject *Sender)
{
	Application->MessageBox("Enter the database name for the MenuMate database.\r"
									"It is in the format of [Name or IP Address]:[Local Path] where [Local Path] "
									"is the path found on the hard drive of the InterBase Server.\r"
									"e.g.\r"
									"SERVER:C:\\MenuMate.fdb   or\r"
									"192.168.1.1:C:\\DB\\MenuMate.fdb\r\r"
									"If you are connecting via a dial-up connection, the IP address can be obtained "
									"by clicking 'Dial' in the Remote MenuMate Database section. Once connected, the "
									"IP address of the server will be displayed.",
									"Information",
									MB_ICONINFORMATION + MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnHelpStockDatabaseClick(TObject *Sender)
{
	Application->MessageBox("Enter the database name for the Stock database.\r"
									"It is in the format of [Name or IP Address]:[Local Path] where [Local Path] "
									"is the path found on the hard drive of the InterBase Server.\r"
									"e.g.\r"
									"SERVER:C:\\Stock.fdb  or\r"
									"192.168.1.1:C:\\DB\\Stock.fdb\r\r"
									"If you are connecting via a dial-up connection, the IP address can be obtained "
									"by clicking 'Dial' in the Remote Stock Database section. Once connected, the "
									"IP address of the server will be displayed.",
									"Information",
									MB_ICONINFORMATION + MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnHelpMenuCommitClick(TObject *Sender)
{
	Application->MessageBox("This is the path to the MenuMate directory for the machine that will import "
									"committed menus. It is recommended that this be the same machine that the InterBase "
									"server is running on, or a machine that always has MenuMate active. This path can "
									"be a network path, so if you're connecting remotely, make sure you're connected to "
									"allow browsing for this path.\r"
									"e.g. \\\\SERVER\\MenuMate",
									"Information",
									MB_ICONINFORMATION + MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnHelpMenuSaveClick(TObject *Sender)
{
	Application->MessageBox("This is the path where menu saves will default to. Sub-directories will be created "
									"under this path for food and drink menus. If you wish to save to a remote machine, "
									"make sure you're connected to allow browsing for this path.\r"
									"e.g. \\\\SERVER\\MenuMate",
									"Information",
									MB_ICONINFORMATION + MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnBrowseMMServerPathClick(TObject *Sender)
{
/*	TFolderDialog FolderDialog
	(
		Handle,
		TDialogSettings
		(
			edCommitPath->Text,
			"Select the path to the MenuMate Server\re.g. \\\\SERVER\\MenuMate",
			TBrowseInfoFlagSet() << ShowPath << NewDialogStyle << EditBox
		)
	);
	if (FolderDialog.Execute())
	{
		if (DirectoryExists(FolderDialog.Selection()))
		{
			edCommitPath->Text = FolderDialog.Selection();
		}
	}*/

	AnsiString Dir = "";
	if (SelectDirectory("Select the path to the MenuMate Server\re.g. \\\\SERVER\\MenuMate", "", Dir))
	{
		edCommitPath->Text = Dir;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnBrowseMMSavePathClick(TObject *Sender)
{
	AnsiString Dir = "";
	if (SelectDirectory("Select the path for saving menus\re.g. C:\\Program Files\\MenuMate", "", Dir))
	{
		edMenuSavePath->Text = Dir;
	}
/*	dfsBrowseDirectoryDlg1->Caption = "Select Path";
	dfsBrowseDirectoryDlg1->Title = "Select the path for saving menus\re.g. C:\\Program Files\\MenuMate";
	if (dfsBrowseDirectoryDlg1->Execute())
	{
		edMenuSavePath->Text = dfsBrowseDirectoryDlg1->Selection;
	} */
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnConnectClick(TObject *Sender)
{
	SaveCurrentCompany();
	TConnectionDetails *CurrentCompany = GetCurrentCompany();
	if (CurrentCompany)
	{
		Screen->Cursor = crHourGlass;
		TIBDatabase *dbTest = new TIBDatabase(this);
		try
		{
			try
			{
				if (Sender == btnConnectMM)
				{
					dbTest->DatabaseName = CurrentCompany->MenuMateDB.DBName;
				}
				else if (Sender == btnConnectStock)
				{
					dbTest->DatabaseName = CurrentCompany->StockDB.DBName;
				}
				dbTest->LoginPrompt = false;
				dbTest->Params->Clear();
				dbTest->Params->Add("user_name=SYSDBA");
				dbTest->Params->Add("password=masterkey");
				dbTest->Connected	= true;

				if (dbTest->TestConnected())
				{
					TIBDatabaseInfo *dbInfo = new TIBDatabaseInfo(this);
					try
					{
						dbInfo->Database = dbTest;
						Application->MessageBox(("Successfully connected to " + dbInfo->DBSiteName + ".").c_str(),
											 "Information", MB_OK + MB_ICONINFORMATION);
					}
					__finally
					{
						delete dbInfo;
					}
				}
				else
				{
					Application->MessageBox("Could not attach to the database.", "Error", MB_ICONERROR + MB_OK);
				}
				dbTest->Connected = false;
			}
			catch (EIBError &E)
			{
				AnsiString Msg = "An error occured while attaching to the database.\r" + E.Message;
				Application->MessageBox(Msg.c_str(), "Error", MB_ICONERROR + MB_OK);
			}
		}
		__finally
		{
			Screen->Cursor = crDefault;
			delete dbTest;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnDialStockClick(TObject *Sender)
{
	TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		Screen->Cursor = crHourGlass;
		try
		{
			AnsiString ErrorMsg;
			if (!CompanyDetails->StockDB.Dial(ErrorMsg))
			{
				AnsiString Msg = "Error while connection to " + CompanyDetails->StockDB.RASEntry + ".\r" + ErrorMsg;
				Application->MessageBox(Msg.c_str(), "Error", MB_OK + MB_ICONERROR);
			}
			else
			{
				AnsiString Msg = "This IP: " + CompanyDetails->StockDB.ClientIP + "\rServer IP: " + CompanyDetails->StockDB.ServerIP;
				Application->MessageBox(Msg.c_str(), "Conected", MB_OK + MB_ICONINFORMATION);
			}
			if (CompanyDetails->StockDB.Connected)
			{
				btnDialStock->Enabled = false;
				btnHangUpStock->Enabled = chbStockRemote->Checked;
			}
			else
			{
				btnDialStock->Enabled = chbStockRemote->Checked;
				btnHangUpStock->Enabled = false;
			}
		}
		__finally
		{
			Screen->Cursor = crDefault;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnDialMMClick(TObject *Sender)
{
	TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		Screen->Cursor = crHourGlass;
		try
		{
			AnsiString ErrorMsg;
			if (!CompanyDetails->MenuMateDB.Dial(ErrorMsg))
			{
				AnsiString Msg = "Error while connection to " + CompanyDetails->MenuMateDB.RASEntry + ".\r" + ErrorMsg;
				Application->MessageBox(Msg.c_str(), "Error", MB_OK + MB_ICONERROR);
			}
			else
			{
				AnsiString Msg = "This IP: " + CompanyDetails->MenuMateDB.ClientIP + "\rServer IP: " + CompanyDetails->MenuMateDB.ServerIP;
				Application->MessageBox(Msg.c_str(), "Conected", MB_OK + MB_ICONINFORMATION);
			}
			cbMMRASEntryChange(NULL);
			cbStockRASEntryChange(NULL);
		}
		__finally
		{
			Screen->Cursor = crDefault;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnHangUpMMClick(TObject *Sender)
{
	TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		Screen->Cursor = crHourGlass;
		try
		{
			CompanyDetails->MenuMateDB.HangUp();
			cbMMRASEntryChange(NULL);
			cbStockRASEntryChange(NULL);
		}
		__finally
		{
			Screen->Cursor = crDefault;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnHangUpStockClick(TObject *Sender)
{
	TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		Screen->Cursor = crHourGlass;
		try
		{
			CompanyDetails->StockDB.HangUp();
			cbMMRASEntryChange(NULL);
			cbStockRASEntryChange(NULL);
		}
		__finally
		{
			Screen->Cursor = crDefault;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnAddCompanyClick(TObject *Sender)
{
	AnsiString NewName = InputBox("Company Name?", "Please enter the name of the company.", "");
	if (NewName != "")
	{
		if (!RegistryKeyExists(OfficeKey + "\\" + NewName))
		{
			if (!CreateKey(OfficeKey + "\\" + NewName))
			{
				Application->MessageBox("Could not add this company.", "Error", MB_OK + MB_ICONERROR);
				return;
			}
		}
		else
		{
			Application->MessageBox("This company already exists.", "Error", MB_OK + MB_ICONERROR);
		}
		SaveSettings();
		LoadSettings();
		for (int i=0; i<tcCompanyNames->Tabs->Count; i++)
		{
			if (tcCompanyNames->Tabs->Strings[i] == NewName)
			{
				tcCompanyNames->TabIndex = i;
				break;
			}
		}
		tcCompanyNamesChange(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnEditCompanyClick(TObject *Sender)
{
	TConnectionDetails *CurrentCompany = GetCurrentCompany();
	if (CurrentCompany)
	{
		AnsiString NewCompanyName = InputBox("Company Name?", "Please enter the new name of the company.", CurrentCompany->CompanyName);
		if (NewCompanyName != "" && NewCompanyName != CurrentCompany->CompanyName)
		{
			SaveCurrentCompany();
			if (!RegistryKeyExists(OfficeKey + "\\" + NewCompanyName))
			{
				SaveSettings();
				RegistryMoveKey(OfficeKey + "\\" + CurrentCompany->CompanyName, OfficeKey + "\\" + NewCompanyName);
				LoadSettings();
				for (int i=0; i<tcCompanyNames->Tabs->Count; i++)
				{
					if (tcCompanyNames->Tabs->Strings[i] == NewCompanyName)
					{
						tcCompanyNames->TabIndex = i;
						break;
					}
				}
				tcCompanyNamesChange(NULL);
			}
			else
			{
				Application->MessageBox("This company already exists.", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnDeleteCompanyClick(TObject *Sender)
{
	if (Application->MessageBox("This will perminantly remove this company's details.\r"
										 "Are you sure you wish to continue?",
										 "Delete?", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
	{
		TConnectionDetails *CompanyDetails = GetCurrentCompany();
		if (CompanyDetails)
		{
			if (RegistryDelete(OfficeKey + "\\" + CompanyDetails->CompanyName))
			{
				LoadSettings();
				tcCompanyNamesChange(NULL);
			}
		}
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnCloseClick(TObject *Sender)
{

        if(!ShowNoOfPriceLevelMessage())
        {
            TConnectionDetails *CurrentCompany = GetCurrentCompany();
            if (CurrentCompany)
            {
                if (CurrentConnection.CompanyName == CurrentCompany->CompanyName)
                {
                    btnConnectCompanyClick(NULL);
                    ModalResult = mrOk;
                    return;
                }
            }

            SaveCurrentCompany();
            SaveSettings();
            ModalResult = mrOk;
      }
}
//---------------------------------------------------------------------------
void TfrmSetup::SaveSettings()
{
	for (int i=0; i<CompanyDetailsList->Count; i++)
	{
		SaveSettings((TConnectionDetails *)CompanyDetailsList->Items[i]);
	}
}
//---------------------------------------------------------------------------
void TfrmSetup::SaveSettings(TConnectionDetails *CurrentCompany)
{
	AnsiString Key = OfficeKey + "\\" +									CurrentCompany->CompanyName;

	RegistryWrite(Key, "CompanyDetails",								CurrentCompany->ReportHeader);

	RegistryWrite(Key, "MenuCommitPath",								CurrentCompany->ServerPath);
	RegistryWrite(Key, "MenuSavePath",									CurrentCompany->MenuSavePath);

	RegistryWrite(Key + "\\Payroll", "PayrollSystem",				CurrentCompany->PayrollSystem);
	RegistryWrite(Key + "\\Payroll", "ExportPath",					CurrentCompany->PayrollExportPath);

	RegistryWrite(Key + "\\MYOB", "AccountSystem",					CurrentCompany->AccountSystem);
	RegistryWrite(Key + "\\MYOB", "MYOBPath", 						CurrentCompany->MYOBPath);
	RegistryWrite(Key + "\\MYOB", "MYOBJobCode",						CurrentCompany->MYOBJobCode);
	RegistryWrite(Key + "\\MYOB", "SalesInvoiceExportAccount",	CurrentCompany->SalesInvoiceExportAccount);
	RegistryWrite(Key + "\\MYOB", "SalesInvoiceExportType",		CurrentCompany->SalesInvoiceExportType);

	RegistryWrite(Key + "\\Stocktake", "StocktakePath",			CurrentCompany->StocktakePath);
	RegistryWrite(Key + "\\Stocktake", "StocktakeExportFile",	CurrentCompany->StocktakeExportFile);
	RegistryWrite(Key + "\\Stocktake", "StocktakeImportFile",	CurrentCompany->StocktakeImportFile);
	RegistryWrite(Key + "\\Stocktake", "StocktakeBarcodePos",	IntToStr(CurrentCompany->StocktakeBarcodePos));
	RegistryWrite(Key + "\\Stocktake", "StocktakeQtyPos",			IntToStr(CurrentCompany->StocktakeQtyPos));

	RegistryWrite(Key, "MMDataFile",										CurrentCompany->MenuMateDB.DBName);
	RegistryWrite(Key, "StockDataFile",									    CurrentCompany->StockDB.DBName);
	RegistryWrite(Key, "ChefMateDataFile",									CurrentCompany->ChefMateDB.DBName);

//	RegistryWrite(Key, "IncludeGST",										AnsiString(cbGST->Checked?"1":"0"));
	RegistryWrite(Key, "HideStocktakeOnHand",							AnsiString(chbHideStocktakeOnHand->Checked?"1":"0"));

	RegistryWrite(Key, "GSTNumber",										edGSTNumber->Text);
	RegistryWrite(Key, "VIPCreditName",									edVIPCreditName->Text);

	RegistryWrite(Key, "DefaultMenuGST",								edDefaultMenuGST->Text);
	RegistryWrite(Key, "DefaultStockGST",								edDefaultStockGST->Text);

	RegistryWrite(Key, "SingleLocation",								AnsiString(chbSingleLocation->Checked?"1":"0"));
	RegistryWrite(Key, "DefaultLocation",								cbDefaultLocation->Text);

	RegistryWrite(Key, "UseSerialBarcodeReader",		(cbBarcodeSerialPort->ItemIndex == 0)?"0":"1");
	RegistryWrite(Key, "SerialBarcodeReaderPort",	(cbBarcodeSerialPort->ItemIndex == 0)?AnsiString("-1"):IntToStr(cbBarcodeSerialPort->ItemIndex));

	RegistryWrite(Key, "RemoteMM",						AnsiString(CurrentCompany->MenuMateDB.RemoteConnection?"1":"0"));
	RegistryWrite(Key, "RemoteMMEntry",					CurrentCompany->MenuMateDB.RASEntry);
	RegistryWrite(Key, "RemoteMMUsername",				CurrentCompany->MenuMateDB.RASUserName);
	RegistryWrite(Key, "RemoteMMPassword",				CurrentCompany->MenuMateDB.RASPassword);

	RegistryWrite(Key, "RemoteStock",					AnsiString(CurrentCompany->StockDB.RemoteConnection?"1":"0"));
	RegistryWrite(Key, "RemoteStockEntry",				CurrentCompany->StockDB.RASEntry);
	RegistryWrite(Key, "RemoteStockUsername",			CurrentCompany->StockDB.RASUserName);
	RegistryWrite(Key, "RemoteStockPassword",			CurrentCompany->StockDB.RASPassword);

	RegistryWrite(Key, "RemoteChefMate",				AnsiString(CurrentCompany->ChefMateDB.RemoteConnection?"1":"0"));
	RegistryWrite(Key, "RemoteChefMateEntry",			CurrentCompany->ChefMateDB.RASEntry);
	RegistryWrite(Key, "RemoteChefMateUsername",		CurrentCompany->ChefMateDB.RASUserName);
	RegistryWrite(Key, "RemoteChefMatePassword",		CurrentCompany->ChefMateDB.RASPassword);
    RegistryWrite(Key, "SettingDecimalPlaces",		(RBRoundingPoint->ItemIndex == 0)?AnsiString("2"):AnsiString("4"));   //SettingDecimalPlaces

   	RegistryWrite(Key, "AutoPrintStockTransferAudit",								AnsiString(chbAutoPrintStockTransferAudit->Checked?"1":"0"));
   	RegistryWrite(Key, "AutoPrintReceiveTransferAudit",								AnsiString(chbAutoPrintReceiveTransferAudit->Checked?"1":"0"));
    RegistryWrite(Key, "SuppliersFromDefaultLocationsOnly",								AnsiString(chbSuppliersFromDefaultLocationsOnly->Checked?"1":"0"));

  //  RegistryWrite(Key, "NoOfPriceLevels",								edNoOfPriceLevels->Text);


}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::tcCompanyNamesChanging(TObject *Sender,
		bool &AllowChange)
{
	try
	{
		SaveCurrentCompany();
	}
	catch(Exception&E)
	{
		AllowChange = false;
	}
}
//---------------------------------------------------------------------------
void TfrmSetup::SaveCurrentCompany()
{
	TConnectionDetails *CurrentCompany = GetCurrentCompany();
	if (CurrentCompany)
	{
		if(cbDefaultLocation->Text != "")
		{
			int index = cbDefaultLocation->Items->IndexOf(cbDefaultLocation->Text);
			if(index == -1 || cbDefaultLocation->Items->Strings[index] != cbDefaultLocation->Text) // case sensitive
			{
				if(Application->MessageBox(AnsiString("Warning: The default location for " + CurrentCompany->CompanyName +
					" has not been correctly set up.\r\rContinue?").c_str(), "Warning", MB_YESNO + MB_ICONWARNING) == ID_NO)
				{
					throw Exception("Incorrect default location");
				}
			}
		}
		if (cbMMDBServer->Text == "")
		{
			CurrentCompany->MenuMateDB.DBName = edMMDBName->Text;
		}
		else
		{
			CurrentCompany->MenuMateDB.DBName = cbMMDBServer->Text + ":" + edMMDBName->Text;
		}
		if (cbStockDBServer->Text == "")
		{
			CurrentCompany->StockDB.DBName	  = edStockDBName->Text;
		}
		else
		{
			CurrentCompany->StockDB.DBName	  = cbStockDBServer->Text + ":" + edStockDBName->Text;
		}
		if (cbChefMateDBServer->Text == "")
		{
			CurrentCompany->ChefMateDB.DBName = edChefMateDBName->Text;
		}
		else
		{
			CurrentCompany->ChefMateDB.DBName = cbChefMateDBServer->Text + ":" + edChefMateDBName->Text;
		}

		CurrentCompany->MenuMateDB.RemoteConnection	= chbMMRemote->Checked;
		CurrentCompany->MenuMateDB.RASUserName		= edMMRASUsername->Text;
		CurrentCompany->MenuMateDB.RASPassword		= edMMRASPassword->Text;

		CurrentCompany->StockDB.RemoteConnection	= chbStockRemote->Checked;
		CurrentCompany->StockDB.RASUserName			= edStockRASUsername->Text;
		CurrentCompany->StockDB.RASPassword			= edStockRASPassword->Text;

		CurrentCompany->ChefMateDB.RemoteConnection	= chbChefMateRemote->Checked;
		CurrentCompany->ChefMateDB.RASUserName		= edChefMateRASUsername->Text;
		CurrentCompany->ChefMateDB.RASPassword		= edChefMateRASPassword->Text;

		CurrentCompany->ReportHeader				= memCompanyDetails->Lines->Text;

		CurrentCompany->ServerPath					= edCommitPath->Text;
		CurrentCompany->MenuSavePath				= edMenuSavePath->Text;

		CurrentCompany->PayrollSystem				= cbPayrollSystem->Text;
		CurrentCompany->PayrollExportPath			= ExcludeTrailingBackslash(edExportPath->Text);

		CurrentCompany->AccountSystem				= cbAccountSystem->Text;
		CurrentCompany->MYOBPath					= ExcludeTrailingBackslash(edMYOBPath->Text);
		CurrentCompany->MYOBJobCode					= edMYOBJobCode->Text;

		CurrentCompany->StocktakePath				= ExcludeTrailingBackslash(edStocktakePath->Text);
		CurrentCompany->StocktakeExportFile			= edStocktakeExportFile->Text;
		CurrentCompany->StocktakeImportFile			= edStocktakeImportFile->Text;
		CurrentCompany->StocktakeBarcodePos			= cbStocktakeBarcodePos->ItemIndex;
		CurrentCompany->StocktakeQtyPos				= cbStocktakeQtyPos->ItemIndex;

//		CurrentCompany->IncludeGST					= cbGST->Checked;
		CurrentCompany->HideStocktakeOnHand			= chbHideStocktakeOnHand->Checked;

		CurrentCompany->UseSerialBarcodeReader		= cbBarcodeSerialPort->ItemIndex == 0;
		CurrentCompany->SerialBarcodeReaderPort		  = (cbBarcodeSerialPort->ItemIndex == 0)?-1:cbBarcodeSerialPort->ItemIndex;

		CurrentCompany->SingleLocation				= chbSingleLocation->Checked;
		CurrentCompany->DefaultLocation				= cbDefaultLocation->Text;

		CurrentCompany->GSTNumber					= edGSTNumber->Text;

		CurrentCompany->DefaultMenuGST				= StrToFloat(edDefaultMenuGST->Text);
		CurrentCompany->DefaultStockGST				= StrToFloat(edDefaultStockGST->Text);
        CurrentCompany->SettingDecimalPlaces	=  RBRoundingPoint->ItemIndex==0?2:4;

        CurrentCompany->AutoPrintStockTransferAudit	   = chbAutoPrintStockTransferAudit->Checked;
        CurrentCompany->AutoPrintReceiveTransferAudit	   = chbAutoPrintReceiveTransferAudit->Checked;
        CurrentCompany->SuppliersFromDefaultLocationsOnly	   = chbSuppliersFromDefaultLocationsOnly->Checked;

		return;
	}
	throw Exception("Incorrect default location");
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnConnectCompanyClick(TObject *Sender)
{
    if(!ShowNoOfPriceLevelMessage())
    {
        SaveCurrentCompany();
        SaveNoOfPriceLevels();
        dmMMData->CloseDB();
        dmStockData->CloseDB();
        dmChefMateData->CloseDB();
        dmMMData->Disconnect();
        dmStockData->Disconnect();
        dmChefMateData->Disconnect();

        TConnectionDetails *CurrentCompany = GetCurrentCompany();
        if (CurrentCompany)
        {
            SaveSettings(CurrentCompany);
            CurrentConnection.CompanyName = CurrentCompany->CompanyName;
            CurrentConnection.LoadSettings();
            CurrentConnection.StockDB.CreateDB = FirstRun;
            dmMMData->Connect(NULL);
            dmStockData->Connect(NULL);
            dmChefMateData->Connect(NULL);

            if (RASAvailable)
            {
                cbMMRASEntryChange(NULL);
                cbStockRASEntryChange(NULL);
                cbChefMateRASEntryChange(NULL);
            }

            Application->MessageBox(("Connected to " + CurrentCompany->CompanyName + ".").c_str(), "Connected", MB_OK + MB_ICONINFORMATION);
            RegistryWrite(OfficeKey, "DefaultCompany", CurrentCompany->CompanyName);
        }
    }    
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnSelectSysDestPathClick(TObject *Sender)
{
	AnsiString Dir = "";
	if (SelectDirectory("Select the path to export payroll file to\re.g. C:\\AcePayroll", "", Dir))
	{
		edExportPath->Text = Dir;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::Button1Click(TObject *Sender)
{
	AnsiString StockCode = "";
	if (InputQuery("Stock Code", "Enter the initial stock code number.", StockCode))
	{
		if (StockCode == "")
		{
			return;
		}
		else
		{
			try
			{
				StrToInt(StockCode);
			}
			catch (...)
			{
				Application->MessageBox("You must enter a number.", "Error", MB_OK + MB_ICONERROR);
				return;
			}
		}
		if (!sqlSetStockCode->Transaction->InTransaction)
		{
			sqlSetStockCode->Transaction->StartTransaction();
		}
		try
		{
			sqlSetStockCode->Close();
			sqlSetStockCode->SQL->Text = "Set Generator Gen_StockCode to " + IntToStr(StrToInt(StockCode));
//			sqlSetStockCode->ParamByName("StockCode")->AsInteger = StrToInt(StockCode);
			sqlSetStockCode->ExecQuery();
			sqlSetStockCode->Transaction->Commit();
		}
		catch (Exception &E)
		{
			sqlSetStockCode->Transaction->Rollback();
			throw;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::pcSettingsChange(TObject *Sender)
{
	btnAddRAS->Visible =  (pcSettings->ActivePage == tsMMConnection ||
                           pcSettings->ActivePage == tsStockConnection ||
                           pcSettings->ActivePage == tsChefMateConnection);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::tcCompanyNamesDrawTab(
		TCustomTabControl *Control, int TabIndex, const TRect &Rect,
      bool Active)
{
	TTabControl *TabControl = (TTabControl *)Control;
	TPoint Pos;
	Pos.x = ((Rect.Right-Rect.Left) - TabControl->Canvas->TextWidth(TabControl->Tabs->Strings[TabIndex])) / 2 + Rect.Left;
	Pos.y = ((Rect.Bottom-Rect.Top) - TabControl->Canvas->TextHeight(TabControl->Tabs->Strings[TabIndex])) / 2 + Rect.Top;
	TabControl->Canvas->TextOut(Pos.x, Pos.y, TabControl->Tabs->Strings[TabIndex]);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnSweepMenuMateClick(TObject *Sender)
{
	if (Application->MessageBox("This will slow down the MenuMate system. "
										 "It is recommended that a sweep is performed outside of trading hours.\r"
										 "Do you wish to continue?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
	{
		dmMMData->Sweep(cbMMDBServer->Text + ":" + edMMDBName->Text);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnSweepStockClick(TObject *Sender)
{
	if (Application->MessageBox("This will slow down the MenuMate stock system. "
										 "It is recommended that a sweep is performed outside of trading hours.\r"
										 "Do you wish to continue?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
	{
		dmStockData->Sweep(cbStockDBServer->Text + ":" + edStockDBName->Text);
	}
}
//---------------------------------------------------------------------------
bool TfrmSetup::InitialiseCompanies()
{
	AnsiString CompanyName;

	CurrentConnection.CompanyName = "";

	bool CompanySelected = false;
	if (RegistryValueExists(OfficeKey, "StockDataFile"))
	{
		// Import old settings from before multi-company version.
		AnsiString Key = OfficeKey + "\\Unknown";
		if (CreateKey(Key))
		{
			AnsiString Text;

			Text = "";
			RegistryRead(OfficeKey, "CompanyDetails", Text);
			if (RegistryWrite(Key, "CompanyDetails", Text))
				RegistryDelete(OfficeKey, "CompanyDetails");
			Text = "";
			RegistryRead(OfficeKey, "MenuCommitPath", Text);
			if (RegistryWrite(Key, "MenuCommitPath", Text))
				RegistryDelete(OfficeKey, "MenuCommitPath");
			Text = "";
			RegistryRead(OfficeKey, "MenuSavePath", Text);
			if (RegistryWrite(Key, "MenuSavePath", Text))
				RegistryDelete(OfficeKey, "MenuSavePath");

			Text = "";
			RegistryRead(OfficeKey, "MMDataFile", Text);
			if (RegistryWrite(Key, "MMDataFile", Text))
				RegistryDelete(OfficeKey, "MMDataFile");
			Text = "";
			RegistryRead(OfficeKey, "StockDataFile", Text);
			if (RegistryWrite(Key, "StockDataFile", Text))
				RegistryDelete(OfficeKey, "StockDataFile");
			Text = "";
			RegistryRead(OfficeKey, "ChefMateDataFile", Text);
			if (RegistryWrite(Key, "ChefMateDataFile", Text))
				RegistryDelete(OfficeKey, "ChefMateDataFile");

//			Text = "0";
//			RegistryRead(OfficeKey, "IncludeGST", Text);
//			if (RegistryWrite(Key, "IncludeGST", Text))
//				RegistryDelete(OfficeKey, "IncludeGST");

			Text = "0";
			RegistryRead(OfficeKey, "RemoteMM", Text);
			if (RegistryWrite(Key, "RemoteMM", Text))
				RegistryDelete(OfficeKey, "RemoteMM");
			Text = "";
			RegistryRead(OfficeKey, "RemoteMMEntry", Text);
			if (RegistryWrite(Key, "RemoteMMEntry", Text))
				RegistryDelete(OfficeKey, "RemoteMMEntry");
			Text = "";
			RegistryRead(OfficeKey, "RemoteMMUsername", Text);
			if (RegistryWrite(Key, "RemoteMMUsername", Text))
				RegistryDelete(OfficeKey, "RemoteMMUsername");
			Text = "";
			RegistryRead(OfficeKey, "RemoteMMPassword", Text);
			if (RegistryWrite(Key, "RemoteMMPassword", Text))
				RegistryDelete(OfficeKey, "RemoteMMPassword");

			Text = "0";
			RegistryRead(OfficeKey, "RemoteStock", Text);
			if (RegistryWrite(Key, "RemoteStock", Text))
				RegistryDelete(OfficeKey, "RemoteStock");
			Text = "";
			RegistryRead(OfficeKey, "RemoteStockEntry", Text);
			if (RegistryWrite(Key, "RemoteStockEntry", Text))
				RegistryDelete(OfficeKey, "RemoteStockEntry");
			Text = "";
			RegistryRead(OfficeKey, "RemoteStockUsername", Text);
			if (RegistryWrite(Key, "RemoteStockUsername", Text))
				RegistryDelete(OfficeKey, "RemoteStockUsername");
			Text = "";
			RegistryRead(OfficeKey, "RemoteStockPassword", Text);
			if (RegistryWrite(Key, "RemoteStockPassword", Text))
				RegistryDelete(OfficeKey, "RemoteStockPassword");

			Text = "0";
			RegistryRead(OfficeKey, "RemoteChefMate", Text);
			if (RegistryWrite(Key, "RemoteChefMate", Text))
				RegistryDelete(OfficeKey, "RemoteChefMate");
			Text = "";
			RegistryRead(OfficeKey, "RemoteChefMateEntry", Text);
			if (RegistryWrite(Key, "RemoteChefMateEntry", Text))
				RegistryDelete(OfficeKey, "RemoteChefMateEntry");
			Text = "";
			RegistryRead(OfficeKey, "RemoteChefMateUsername", Text);
			if (RegistryWrite(Key, "RemoteChefMateUsername", Text))
				RegistryDelete(OfficeKey, "RemoteChefMateUsername");
			Text = "";
			RegistryRead(OfficeKey, "RemoteChefMatePassword", Text);
			if (RegistryWrite(Key, "RemoteChefMatePassword", Text))
				RegistryDelete(OfficeKey, "RemoteChefMatePassword");

//			RegistryDelete(OfficeKey, "IncludeGST");

			TStringList *Keys = new TStringList;
			try
			{
				if (RegistryGetKeys(OfficeKey, Keys))
				{
					for (int i=0; i<Keys->Count; i++)
					{
						if (Keys->Strings[i] != "Unknown")
						{
							RegistryMoveKey(OfficeKey + "\\" + Keys->Strings[i], Key + "\\" + Keys->Strings[i]);
						}
					}
				}
			}
			__finally
			{
				delete Keys;
			}
			CompanyName = "Unknown";
			CompanySelected = true;
		}
	}
	else
	{
		TStringList *CompanyNames = new TStringList;
		try
		{
			RegistryGetKeys(OfficeKey, CompanyNames);
			if (CompanyNames->Count == 0)
			{
				AnsiString NewName = InputBox("Company Name?", "Please enter the name or your company.", "");
				if (NewName != "")
				{
					if (CreateKey(OfficeKey + "\\" + NewName))
					{
						CompanySelected = true;
						CompanyName = NewName;
					}
				}
			}
			else if (CompanyNames->Count == 1)
			{
				CompanySelected = true;
				CompanyName = CompanyNames->Strings[0];
			}
			else
			{
				frmSelectCompany = new TfrmSelectCompany(Application);
				try
				{
					frmSelectCompany->cbCompanyNames->Items->Assign(CompanyNames);
					if (frmSelectCompany->ShowModal() == mrOk)
					{
						CompanySelected = true;
						CompanyName = CompanyNames->Strings[frmSelectCompany->cbCompanyNames->ItemIndex];
					}
				}
				__finally
				{
					delete frmSelectCompany;
				}
			}
		}
		__finally
		{
			delete CompanyNames;
		}
	}
	if (CompanySelected)
	{
		bool Found = RegistryValueExists(OfficeKey + "\\" + CompanyName, "RemoteMM");
		if (Found)
		{
			Found = RegistryValueExists(OfficeKey + "\\" + CompanyName, "RemoteStock");
		}
		if (!Found)
		{
			frmSetup->FirstRun = true;
			CurrentConnection.CompanyName = CompanyName;
			if (frmSetup->ShowModal() == mrCancel)
			{
				return false;
			}
			bool Found = RegistryValueExists(OfficeKey + "\\" + CompanyName, "RemoteMM");
			if (Found)
			{
				Found = RegistryValueExists(OfficeKey + "\\" + CompanyName, "RemoteStock");
			}
			if (!Found)
			{
				return false;
			}
			if (CurrentConnection.CompanyName != "")
			{
				RegistryWrite(OfficeKey, "DefaultCompany", CompanyName);
				return true;
			}
		}
		RegistryWrite(OfficeKey, "DefaultCompany", CompanyName);
		CurrentConnection.CompanyName = CompanyName;
		CurrentConnection.LoadSettings();

		dmMMData->Connect(frmSplash->lbeStatus);

		dmStockData->Connect(frmSplash->lbeStatus);

		dmChefMateData->Connect(frmSplash->lbeStatus);
		CreateIBConsoleSettings();
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
void TfrmSetup::CreateIBConsoleSettings()
{
	AnsiString MMServer, MMPath, StockServer, StockPath, MMServerReg, StockServerReg;

	GetInterbasePathComponents(CurrentConnection.MenuMateDB.DBName, &MMServer, &MMPath);
	GetInterbasePathComponents(CurrentConnection.StockDB.DBName, &StockServer, &StockPath);

	if (MMServer == StockServer)
	{
		MMServerReg		= "Software\\Borland\\Interbase\\IBConsole\\Servers\\MM - " + CurrentConnection.CompanyName;
		StockServerReg	= "Software\\Borland\\Interbase\\IBConsole\\Servers\\MM - " + CurrentConnection.CompanyName;
	}
	else
	{
		MMServerReg		= "Software\\Borland\\Interbase\\IBConsole\\Servers\\" + CurrentConnection.CompanyName + " MenuMate";
		StockServerReg	= "Software\\Borland\\Interbase\\IBConsole\\Servers\\" + CurrentConnection.CompanyName + " Stock";
	}
	try
	{
		TRegistry *Registry = new TRegistry;
		try
		{
			Registry->RootKey = HKEY_CURRENT_USER;
			if (dmMMData->dbMenuMate->Connected)
			{
				if (Registry->OpenKey(MMServerReg, true))
				{
					Registry->WriteString("ServerName", MMServer);
					Registry->WriteString("UserName", "SYSDBA");
					Registry->WriteString("Password", "D61637475627B65697");
					Registry->WriteInteger("Protocol", 0);

					Registry->CloseKey();
				}
				if (Registry->OpenKey(MMServerReg + "\\Databases\\MenuMate", true))
				{
					Registry->WriteString("DatabaseFiles", MMPath);
					Registry->WriteString("UserName", "SYSDBA");
					Registry->WriteString("Role", "");
					Registry->WriteString("CharacterSet", "");
					Registry->WriteInteger("CaseSensitiveRole", 0);

					Registry->CloseKey();
				}
			}
			if (dmStockData->dbStock->Connected)
			{
				if (Registry->OpenKey(StockServerReg, true))
				{
					Registry->WriteString("ServerName", StockServer);
					Registry->WriteString("UserName", "SYSDBA");
					Registry->WriteString("Password", "D61637475627B65697");
					Registry->WriteInteger("Protocol", 0);

					Registry->CloseKey();
				}
				if (Registry->OpenKey(StockServerReg + "\\Databases\\Stock", true))
				{
					Registry->WriteString("DatabaseFiles", StockPath);
					Registry->WriteString("UserName", "SYSDBA");
					Registry->WriteString("Role", "");
					Registry->WriteString("CharacterSet", "");
					Registry->WriteInteger("CaseSensitiveRole", 0);

					Registry->CloseKey();
				}
			}
		}
		__finally
		{
			delete Registry;
		}
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::cbDBServerDropDown(TObject *Sender)
{
	if (ServersList)
	{
		((TComboBox *)Sender)->Items->Assign(ServersList);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnGetMMIPClick(TObject *Sender)
{
	AnsiString ComputerIP = dmUtilities->GetComputerIP(cbMMDBServer->Text);
	if (ComputerIP != "")
	{
		cbMMDBServer->Text = ComputerIP;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnGetStockIPClickClick(TObject *Sender)
{
	AnsiString ComputerIP = dmUtilities->GetComputerIP(cbStockDBServer->Text);
	if (ComputerIP != "")
	{
		cbStockDBServer->Text = ComputerIP;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnSelectMYOBPathClick(TObject *Sender)
{
	AnsiString Dir = "";
	if (SelectDirectory("Select the path to export and import files for MYOB", "", Dir))
	{
		edMYOBPath->Text = Dir;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::Button2Click(TObject *Sender)
{
	frmSetupLocations = new TfrmSetupLocations(this);
	try
	{
		frmSetupLocations->ShowModal();
	}
	__finally
	{
		delete frmSetupLocations;
		frmSetupLocations = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::cbDefaultLocationDropDown(TObject *Sender)
{
	TConnectionDetails *CurrentCompany = GetCurrentCompany();
	if (CurrentCompany)
	{
		if (CurrentConnection.CompanyName == CurrentCompany->CompanyName)
		{
			if (sqlLocation->Database->Connected && !sqlLocation->Transaction->InTransaction)
			{
				sqlLocation->Transaction->StartTransaction();
				cbDefaultLocation->Items->Clear();
				for (sqlLocation->ExecQuery(); !sqlLocation->Eof; sqlLocation->Next())
				{
					cbDefaultLocation->Items->Add(sqlLocation->FieldByName("Name")->AsString);
				}
				sqlLocation->Transaction->Commit();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnSelectStocktakePathClick(TObject *Sender)
{
	AnsiString Dir = "";
	if (SelectDirectory("Select the path to export and import files for stocktake", "", Dir))
	{
		edStocktakePath->Text = Dir;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::Button3Click(TObject *Sender)
{
	frmUsers->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetup::btnSecurityRolesClick(TObject *Sender)
{
	std::auto_ptr<Security::TfrmConfigureRoles> form(new Security::TfrmConfigureRoles());
	Security::ConfigureRolesFormController controller(form.get());
	form->Show();
}
//---------------------------------------------------------------------------


void __fastcall TfrmSetup::btnGetChefMateIPClick(TObject *Sender)
{
    AnsiString ComputerIP = dmUtilities->GetComputerIP(cbChefMateDBServer->Text);
	if (ComputerIP != "")
	{
		cbChefMateDBServer->Text = ComputerIP;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnSweepChefMateClick(TObject *Sender)
{
    if (Application->MessageBox("This will slow down the ChefMate system. "
										 "It is recommended that a sweep is performed outside of trading hours.\r"
										 "Do you wish to continue?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
	{
		dmChefMateData->Sweep(cbChefMateDBServer->Text + ":" + edChefMateDBName->Text);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbChefMateRASEntryChange(TObject *Sender)
{
    TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		CompanyDetails->ChefMateDB.RASEntry = cbChefMateRASEntry->Text;

		AnsiString UserName, Password;
//		CompanyDetails->StockDB.GetEntryDetails(UserName, Password);
//		edStockRASUsername->Text = UserName;
//		CompanyDetails->StockDB.RASUserName = UserName;
//		edStockRASPassword->Text = Password;
//		CompanyDetails->StockDB.RASPassword = Password;

		if (CompanyDetails->ChefMateDB.Connected)
		{
			btnDialStock->Enabled = false;
			btnHangUpStock->Enabled = chbChefMateRemote->Checked;
		}
		else
		{
			btnDialStock->Enabled = chbChefMateRemote->Checked;
			btnHangUpStock->Enabled = false;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnHelpChefMateDatabaseClick(TObject *Sender)
{
    Application->MessageBox("Enter the database name for the ChefMate database.\r"
                                "It is in the format of [Name or IP Address]:[Local Path] where [Local Path] "
                                "is the path found on the hard drive of the InterBase Server.\r"
                                "e.g.\r"
                                "SERVER:C:\\Stock.fdb  or\r"
                                "192.168.1.1:C:\\DB\\Stock.fdb\r\r"
                                "If you are connecting via a dial-up connection, the IP address can be obtained "
                                "by clicking 'Dial' in the Remote Stock Database section. Once connected, the "
                                "IP address of the server will be displayed.",
                                "Information",
                                MB_ICONINFORMATION + MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::chbChefMateRemoteClick(TObject *Sender)
{
    cbChefMateRASEntry->Enabled			= chbChefMateRemote->Checked;
	edChefMateRASUsername->Enabled		= chbChefMateRemote->Checked;
	edChefMateRASPassword->Enabled		= chbChefMateRemote->Checked;
//	btnAddRAS->Enabled				= chbMMRemote->Checked || chbStockRemote->Checked || chbChefMateRemote->Checked;
//	cbChefMateRASEntryChange(NULL);
	if (chbChefMateRemote->Checked)
	{
		cbChefMateRASEntryClick(NULL);
	}
	else
	{
		btnDialChefMate->Enabled = false;
		btnHangUpChefMate->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnDialChefMateClick(TObject *Sender)
{
    TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		Screen->Cursor = crHourGlass;
		try
		{
			AnsiString ErrorMsg;
			if (!CompanyDetails->ChefMateDB.Dial(ErrorMsg))
			{
				AnsiString Msg = "Error while connection to " + CompanyDetails->ChefMateDB.RASEntry + ".\r" + ErrorMsg;
				Application->MessageBox(Msg.c_str(), "Error", MB_OK + MB_ICONERROR);
			}
			else
			{
				AnsiString Msg = "This IP: " + CompanyDetails->ChefMateDB.ClientIP + "\rServer IP: " + CompanyDetails->ChefMateDB.ServerIP;
				Application->MessageBox(Msg.c_str(), "Conected", MB_OK + MB_ICONINFORMATION);
			}
			cbMMRASEntryChange(NULL);
			cbStockRASEntryChange(NULL);
			cbChefMateRASEntryChange(NULL);
		}
		__finally
		{
			Screen->Cursor = crDefault;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnHangUpChefMateClick(TObject *Sender)
{
    TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		Screen->Cursor = crHourGlass;
		try
		{
			CompanyDetails->ChefMateDB.HangUp();
			cbMMRASEntryChange(NULL);
			cbStockRASEntryChange(NULL);
			cbChefMateRASEntryChange(NULL);
		}
		__finally
		{
			Screen->Cursor = crDefault;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::cbChefMateRASEntryClick(TObject *Sender)
{
	TConnectionDetails *CompanyDetails = GetCurrentCompany();
	if (CompanyDetails)
	{
		if (CompanyDetails->ChefMateDB.Connected)
		{
			btnDialChefMate->Enabled = false;
			btnHangUpChefMate->Enabled = chbChefMateRemote->Checked;
		}
		else
		{
			btnDialChefMate->Enabled = chbChefMateRemote->Checked;
			btnHangUpChefMate->Enabled = false;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::btnTaxProfilesClick(TObject *Sender)
{
   // create the view
   TfrmTaxProfile* frmTaxProfile = new TfrmTaxProfile(NULL);

   // assign the controller, display view
   TaxProfileController controller(frmTaxProfile);
   controller.ShowView();

   // clean up
   delete frmTaxProfile;
}
//---------------------------------------------------------------------------

   //---------------------------------------------------------------------------
void __fastcall TfrmSetup::SyncStockLocationClick(TObject *Sender)
{
   	if (!Transaction->InTransaction)
	    Transaction->StartTransaction();

	for (qrLocation->Open(); !qrLocation->Eof; qrLocation->Next())
	{
		AnsiString Location=qrLocation->FieldByName("Location")->AsString;
        	if (Location != "")
		{
        if (!MMTransaction->InTransaction)
	    MMTransaction->StartTransaction();

			qrFindLocation->Close();
			qrFindLocation->ParamByName("Name")->AsString = Location.UpperCase();
			qrFindLocation->ExecQuery();
			if (qrFindLocation->Eof)
			{
				//Add the location!
				sqlAddLK->Close();
				sqlAddLK->ExecQuery();

				qrAddLocation->Close();
				qrAddLocation->ParamByName("Location_Key")->AsInteger		= sqlAddLK->Fields[0]->AsInteger;
				qrAddLocation->ParamByName("Location_Type")->AsInteger	= 1;
				qrAddLocation->ParamByName("Name")->AsString	= Location;
                qrAddLocation->ParamByName("CATEGORY_TYPE")->AsString					= 99;

				qrAddLocation->ExecQuery();

				MMTransaction->Commit();
                }
           }
	}
	qrLocation->Close();
}


void __fastcall TfrmSetup::RBRoundingPointClick(TObject *Sender)
{
     if(RBRoundingPoint->ItemIndex ==1)
     {}
     else
     {}
}
//---------------------------------------------------------------------------
void TfrmSetup::SaveNoOfPriceLevels()
{
       int maxPriceLevelsEnabled = 2;
       int maxPriceLevelsDisabled =0;
       int maxItemSizePriceLevelsEnabled  = 2;
       int maxItemSizePriceLevelsDisabled = 0;
       int gen_Value;
       int noOfRecords = 0;  

        if (sqlGetNoOfPriceLevels->Database->Connected && !sqlGetNoOfPriceLevels->Transaction->InTransaction)
        {
            sqlGetNoOfPriceLevels->Transaction->StartTransaction();
            sqlGetNoOfPriceLevels->Close();
            sqlGetNoOfPriceLevels->ExecQuery();
            noOfRecords = sqlGetNoOfPriceLevels->RecordCount;
            sqlGetNoOfPriceLevels->Transaction->Commit();
           if(!sqlSetNoOfPriceLevels->Transaction->InTransaction)
            {
                sqlSetNoOfPriceLevels->Transaction->StartTransaction();
           }
            sqlSetNoOfPriceLevels->Close();
            if(noOfRecords > 0)
            {
            sqlSetNoOfPriceLevels->SQL->Text="	UPDATE  VARIABLES  "
                            "SET INTEGER_VAL  = :INTEGER_VAL  "  	
                        "WHERE  "
                            "VARIABLES_KEY = 5014;  " ;   
                    sqlSetNoOfPriceLevels->ParamByName("INTEGER_VAL")->AsInteger = edNoOfPriceLevels->Value;
                    sqlSetNoOfPriceLevels->ExecQuery();

                    sqlSetNoOfPriceLevels->Transaction->Commit();
            }
            else
            {
                    sqlSetNoOfPriceLevels->SQL->Text =
                                "INSERT INTO VARIABLES ("
                                               "VARIABLES_KEY,"
                                               "VARIABLE_NAME,"
                                               "VARIABLE_DESCRIPTION,"
                                               "VARIABLE_GROUP,"
                                               "INTEGER_VAL) "
                                               "VALUES ("
                                               ":VARIABLES_KEY,"
                                               ":VARIABLE_NAME,"
                                               ":VARIABLE_DESCRIPTION,"
                                               ":VARIABLE_GROUP,"
                                                               ":INTEGER_VAL);";
                   sqlSetNoOfPriceLevels->ParamByName("VARIABLES_KEY")->AsInteger = 5014;
                   sqlSetNoOfPriceLevels->ParamByName("VARIABLE_NAME")->AsString = "No Of Price Levels.";
                   sqlSetNoOfPriceLevels->ParamByName("VARIABLE_DESCRIPTION")->AsString = "No Of Price Levels for Happy Hour.Default is 2.";
                   sqlSetNoOfPriceLevels->ParamByName("VARIABLE_GROUP")->AsInteger = 6;
                   sqlSetNoOfPriceLevels->ParamByName("INTEGER_VAL")->AsInteger = edNoOfPriceLevels->Value;
                   sqlSetNoOfPriceLevels->ExecQuery();
                   sqlSetNoOfPriceLevels->Transaction->Commit();
          }
     } 
      maxPriceLevelsEnabled  = GetMaxPriceLevels("T");
      maxPriceLevelsDisabled = GetMaxPriceLevels("F");
       AnsiString priceLevels = "PriceLevel";
      maxItemSizePriceLevelsEnabled = GetMaxPriceLevelsInItemSize("T");
      maxItemSizePriceLevelsDisabled = GetMaxPriceLevelsInItemSize("F");

        if(edNoOfPriceLevels->Value > maxItemSizePriceLevelsEnabled)
        {
            if(maxItemSizePriceLevelsDisabled > maxItemSizePriceLevelsEnabled)
            {
                UpdateItemSizePriceLevels("T","F", maxItemSizePriceLevelsEnabled);
                maxItemSizePriceLevelsEnabled = maxItemSizePriceLevelsDisabled;
            }
            if (qrSelectPriceLevelItemSize->Database->Connected && !qrSelectPriceLevelItemSize->Transaction->InTransaction)
            {
                qrSelectPriceLevelItemSize->Transaction->StartTransaction();
                 qrSelectPriceLevelItemSize->Close();

                qrSelectPriceLevelItemSize->ExecQuery();
                
   //           qrSelectPriceLevelItemSize->Transaction->Commit();
                for(;!qrSelectPriceLevelItemSize->Eof; qrSelectPriceLevelItemSize->Next())
                {
                     for(int i= qrSelectPriceLevelItemSize->Fields[3]->AsInteger+1;i<=edNoOfPriceLevels->Value;i++)
                    {
                        if (qrGenPriceLevelItemSizeKey->Database->Connected && !qrGenPriceLevelItemSizeKey->Transaction->InTransaction)
                        {

                            qrGenPriceLevelItemSizeKey->Transaction->StartTransaction();
                          }

                            qrGenPriceLevelItemSizeKey->Close();
                            qrGenPriceLevelItemSizeKey->ExecQuery();
                            gen_Value = qrGenPriceLevelItemSizeKey->Fields[0]->AsInteger;
                        //    qrGenPriceLevelItemSizeKey->Transaction->Commit();


                        if (qrInsertInToPriceLevelItemSize->Database->Connected && !qrInsertInToPriceLevelItemSize->Transaction->InTransaction)
                        {
                            qrInsertInToPriceLevelItemSize->Transaction->StartTransaction();
                         }
                            qrInsertInToPriceLevelItemSize->Close();
                            qrInsertInToPriceLevelItemSize->ParamByName("PRICELEVELITEMSIZE_KEY")->AsInteger = gen_Value;
                            qrInsertInToPriceLevelItemSize->ParamByName("ITEMSIZE_KEY")->AsInteger = qrSelectPriceLevelItemSize->Fields[0]->AsInteger;
                            qrInsertInToPriceLevelItemSize->ParamByName("PRICELEVEL_KEY")->AsInteger = i;
                            qrInsertInToPriceLevelItemSize->ParamByName("PRICE")->AsCurrency = qrSelectPriceLevelItemSize->Fields[1]->AsCurrency;
                            qrInsertInToPriceLevelItemSize->ParamByName("ISENABLED")->AsString = qrSelectPriceLevelItemSize->Fields[2]->AsString;
                            qrInsertInToPriceLevelItemSize->ExecQuery();
                        }
                 }
                 qrInsertInToPriceLevelItemSize->Transaction->Commit();

            }
         }
         else
         {
             if(edNoOfPriceLevels->Value < maxItemSizePriceLevelsEnabled)
             {
                  UpdateItemSizePriceLevels("F", "T", maxItemSizePriceLevelsEnabled);
             }
         }    



    if(edNoOfPriceLevels->Value > maxPriceLevelsEnabled)
    {

        if(maxPriceLevelsDisabled > maxPriceLevelsEnabled)
        {
            UpdatePriceLevels("T","F", maxPriceLevelsEnabled);
            maxPriceLevelsEnabled = maxPriceLevelsDisabled;
        }
        bool isNameExist = false;


        for(int i= maxPriceLevelsEnabled+1;i<=edNoOfPriceLevels->Value;i++)
        {
             if (qrInsertInToPriceLevelItem->Database->Connected && !qrInsertInToPriceLevelItem->Transaction->InTransaction)
            {

                qrInsertInToPriceLevelItem->Transaction->StartTransaction();
                qrInsertInToPriceLevelItem->Close();

                qrInsertInToPriceLevelItem->ParamByName("PRICELEVEL_KEY")->AsInteger = i;
                qrInsertInToPriceLevelItem->ParamByName("PRICELEVEL_NAME")->AsString = priceLevels +" "+i;

            if (qrGetExistingPriceNames->Database->Connected && !qrGetExistingPriceNames->Transaction->InTransaction)
            {
                qrGetExistingPriceNames->Transaction->StartTransaction();
            }
            qrGetExistingPriceNames->Close();
            qrGetExistingPriceNames->ExecQuery();

           bool priceLevelPresent  =false;
            while(!qrGetExistingPriceNames->Eof)
            {
               if((qrInsertInToPriceLevelItem->ParamByName("PRICELEVEL_NAME")->AsString).LowerCase() == qrGetExistingPriceNames->FieldByName("PRICELEVEL_NAME")->AsString.LowerCase())
                {
               //    qrInsertInToPriceLevelItem->ParamByName("PRICELEVEL_NAME")->AsString = priceLevels +" "+i+"_1";
                   priceLevelPresent=true;
                   break;
                }
               qrGetExistingPriceNames->Next();
            }


            if (priceLevelPresent)
            {
              qrInsertInToPriceLevelItem->ParamByName("PRICELEVEL_NAME")->AsString =  getSuffixValue(qrInsertInToPriceLevelItem->ParamByName("PRICELEVEL_NAME")->AsString ) ;

            }


            qrInsertInToPriceLevelItem->ParamByName("ISENABLED")->AsString = "T";

            qrInsertInToPriceLevelItem->ExecQuery();
            qrInsertInToPriceLevelItem->Transaction->Commit();
         }
        if(qrGetExistingPriceNames->Transaction->InTransaction)
        {
            qrGetExistingPriceNames->Transaction->Commit();
        }
       }
    }
    else
    {
         if(edNoOfPriceLevels->Value < maxPriceLevelsEnabled)
         {
            UpdatePriceLevels("F", "T", maxPriceLevelsEnabled);
         }
    }
}
/////////--------------------------------------------------------------------
                                         
 AnsiString TfrmSetup::getSuffixValue(AnsiString priceLevelName)
 {
    bool namePresent= false;
    qrGetExistingPriceNames->Close();
    qrGetExistingPriceNames->ExecQuery();

    while(!qrGetExistingPriceNames->Eof)
    {
       if( priceLevelName.LowerCase() == qrGetExistingPriceNames->FieldByName("PRICELEVEL_NAME")->AsString.LowerCase())
        {
           namePresent=true;
           break;
         }
         qrGetExistingPriceNames->Next();
    }

     if (namePresent)
     {
         return  priceLevelName= getSuffixValue(priceLevelName + "_");
     }else
     {
        priceLevelName= priceLevelName + "_";
        return   priceLevelName;
     }
  }
//---------------------------------------------------------------------------
 void TfrmSetup::UpdatePriceLevels(AnsiString isEnabled, AnsiString alreadyEnabled, int currentPriceLevelsEnabled)
 {
     if (qrUpdatePriceLevels->Database->Connected && !qrUpdatePriceLevels->Transaction->InTransaction)
        {
            qrUpdatePriceLevels->Transaction->StartTransaction();
            qrUpdatePriceLevels->Close();
            qrUpdatePriceLevels->ParamByName("ISENABLED")->AsString = isEnabled;
            qrUpdatePriceLevels->ParamByName("ISTRUE")->AsString = alreadyEnabled;
            if(edNoOfPriceLevels->Value < currentPriceLevelsEnabled)
            {
                qrUpdatePriceLevels->ParamByName("PRICELEVEL_KEY")->AsInteger = edNoOfPriceLevels->Value;
                qrUpdatePriceLevels->ParamByName("MAXPRICELEVEL_KEY")->AsInteger = currentPriceLevelsEnabled;
            }
            else
            {
                 qrUpdatePriceLevels->ParamByName("PRICELEVEL_KEY")->AsInteger = currentPriceLevelsEnabled;
                qrUpdatePriceLevels->ParamByName("MAXPRICELEVEL_KEY")->AsInteger = edNoOfPriceLevels->Value;
            }
            qrUpdatePriceLevels->ExecQuery();
            qrUpdatePriceLevels->Transaction->Commit();
        }
 }
//---------------------------------------------------------------------------
 int TfrmSetup::GetMaxPriceLevels(AnsiString isEnabled)
 {
      if (qrMaxPriceLevels->Database->Connected && !qrMaxPriceLevels->Transaction->InTransaction)
    {
        qrMaxPriceLevels->Transaction->StartTransaction();

        qrMaxPriceLevels->Close();
        qrMaxPriceLevels->ParamByName("ISENABLED")->AsString = isEnabled;

        qrMaxPriceLevels->ExecQuery();
        qrMaxPriceLevels->Transaction->Commit();
         return(qrMaxPriceLevels->Fields[0]->AsInteger);

    }
 }

 //---------------------------------------------------------------------------
 int TfrmSetup::GetMaxPriceLevelsInItemSize(AnsiString isEnabled)
 {
      if (qrMaxItemSizePriceLevels->Database->Connected && !qrMaxItemSizePriceLevels->Transaction->InTransaction)
    {
        qrMaxPriceLevels->Transaction->StartTransaction();

        qrMaxItemSizePriceLevels->Close();
        qrMaxItemSizePriceLevels->SQL->Text =
        "SELECT max(a.PRICELEVEL_KEY) "
                "FROM PRICELEVELITEMSIZE a "
        "where a.ISENABLED = :ISENABLED ";

        qrMaxItemSizePriceLevels->ParamByName("ISENABLED")->AsString = isEnabled;

        qrMaxItemSizePriceLevels->ExecQuery();
        qrMaxItemSizePriceLevels->Transaction->Commit();
        return(qrMaxItemSizePriceLevels->Fields[0]->AsInteger);

    }
 }

 //---------------------------------------------------------------------------
 void TfrmSetup::UpdateItemSizePriceLevels(AnsiString isEnabled, AnsiString alreadyEnabled, int currentPriceLevelsEnabled)
 {
     if (qrUpdateItemSizePriceLevels->Database->Connected && !qrUpdateItemSizePriceLevels->Transaction->InTransaction)
        {
            qrUpdateItemSizePriceLevels->Transaction->StartTransaction();
            qrUpdateItemSizePriceLevels->Close();
           qrUpdateItemSizePriceLevels->ParamByName("ISENABLED")->AsString = isEnabled;
            qrUpdateItemSizePriceLevels->ParamByName("ISTRUE")->AsString = alreadyEnabled;
            if(edNoOfPriceLevels->Value < currentPriceLevelsEnabled)
            {
                qrUpdateItemSizePriceLevels->ParamByName("PRICELEVEL_KEY")->AsInteger = edNoOfPriceLevels->Value;
                qrUpdateItemSizePriceLevels->ParamByName("MAXPRICELEVEL_KEY")->AsInteger = currentPriceLevelsEnabled;
            }
            else
            {
                 qrUpdateItemSizePriceLevels->ParamByName("PRICELEVEL_KEY")->AsInteger = currentPriceLevelsEnabled;
                qrUpdateItemSizePriceLevels->ParamByName("MAXPRICELEVEL_KEY")->AsInteger = edNoOfPriceLevels->Value;
            }
            qrUpdateItemSizePriceLevels->ExecQuery();
            qrUpdateItemSizePriceLevels->Transaction->Commit();
        }
 }
//---------------------------------------------------------------------------

void __fastcall TfrmSetup::edNoOfPriceLevelsKeyPress(TObject *Sender,
      char &Key)
{
  if((Key == '-'))
   {
      Key = NULL;
   }
   else
   {
    // PostMessage(edNoOfPriceLevels->Handle, WM_CHAR, Key, 0);
    // edNoOfPriceLevels->SetFocus();
   }
}
//---------------------------------------------------------------------------
bool TfrmSetup::ShowNoOfPriceLevelMessage()
{
     if(edNoOfPriceLevels->Value <2 || edNoOfPriceLevels->Value >10 )
    {
         Application->MessageBox("Number of PriceLevels Required Text Box must have value between 1 and 11.", "Error", MB_OK + MB_ICONERROR);
         edNoOfPriceLevels->SetFocus();
         return true;
    }
    return false;
}

