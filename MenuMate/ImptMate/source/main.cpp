//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#define MMBaseKey "\\Software\\IQWorks\\MenuMate\\"

#include "main.h"
#include "FileCtrl.hpp"
#include "MMTouchNumpad.h"
#include "Invoice.h"
#include "CSV.h"
#include "Contact.h"
#include "SystemConfigLite.h"
#include "MMRegistry.h"
#include "Tabs.h"
#include "IBLogs.h"
#include "MMMessageBox.h"
#include "ManagerVariable.h"
#include "ManagerNet.h"

//---------------------------------------------------------------------------
#define ProgramLabel "ImptMate"

#pragma package(smart_init)
#pragma link "TouchButton"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	FormResize(Sender);
   IBDatabase 								= new TIBDatabase(NULL);
	IBTransaction							= new TIBTransaction(NULL);

   IBDatabase->Params->Add("user_name=SYSDBA");
   IBDatabase->Params->Add("password=masterkey");
   IBDatabase->LoginPrompt = false;
   IBDatabase->SQLDialect = 3;
   IBDatabase->DefaultTransaction = IBTransaction;
	IBTransaction->DefaultDatabase	= IBDatabase;
   IBTransaction->Params->Add("read_committed");
   IBTransaction->Params->Add("rec_version");
   IBTransaction->Params->Add("nowait");

	IBQuery									= new TIBQuery(NULL);
	IBQuery1									= new TIBQuery(NULL);
	IBQuery->Database						= IBDatabase;
	IBQuery1->Database					= IBDatabase;
	IBQuery->Transaction					= IBTransaction;
	IBQuery1->Transaction				= IBTransaction;

	ManagerLogs = new TManagerLogs;
	ManagerNet = new TManagerNet(65000);
	Membership = new TContactMember;
	Staff = new TContactStaff;
	Invoice = new TInvoice;
	Tabs = new TTabs;
	VM = new TManagerVariable;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	delete Membership;
   delete Staff;
   delete Invoice;
   delete Tabs;
	delete VM;
	delete ManagerNet;
	delete ManagerLogs;
}
//---------------------------------------------------------------------------
void TfrmMain::OpenDB()
{
   AnsiString DatabasePath = "";
   RegistryRead(MMBaseKey "Database","DatabasePath",DatabasePath);

   AnsiString InterbaseIP = "";
   RegistryRead(MMBaseKey "Database","InterbaseIP",InterbaseIP);

	// Set and Open the DB.
	IBDatabase->Connected		= false;
	IBDatabase->DatabaseName	= InterbaseIP + ":" + DatabasePath;
	IBDatabase->Connected		= true;

}

void TfrmMain::CloseDB()
{
	IBDatabase->ForceClose();
}

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	// Connect to DB.
   bool DBOk = false;
   do
   {
      try
      {
         bool Reconfigure = false;
         RegistryRead(MMBaseKey "Database", "Reconfigure", Reconfigure);
         if(Reconfigure)
         {
            frmSystemConfig->ShowModal();
         }
         OpenDB();
         DBOk = true;
      }
      catch(Exception &Err)
      {
         CloseDB();
         DBOk = false;
      }
      if(DBOk == false)
      {
         frmSystemConfig->ShowModal();
         if(frmSystemConfig->ModalResult == mrAbort)
         {
            Close();
            return;
         }
      }
   }while(DBOk == false);

	TDBTab::Initialise(IBDatabase,IBTransaction);
	Membership->Initialise(IBDatabase,IBTransaction);
	Staff->Initialise(IBDatabase,IBTransaction);
   Invoice->Initialise(IBDatabase,IBTransaction);

	TMMContactInfo TempUserInfo;
   bool LoginOk = false;
   do
   {
      TLoginSuccess Result = Staff->Login(TempUserInfo, CheckNone);
      if (Result == lsAccepted)
      {
         UserKey = TempUserInfo.ContactKey;
         LoginOk = true;
      }
      else if(Result == lsCancel)
      {
         Close();
         LoginOk = true;
      }
      TempUserInfo.Clear();
   }
   while(LoginOk == false);

   ImportFileName = "C:\\drsex.csv";
   RegistryRead(MMBaseKey "Configuration","ImptMateFile",ImportFileName);
   Label1->Caption = ImportFileName;

	Timer->Enabled = true;   
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::TouchButton2Click(TObject *Sender)
{
   AnsiString Directory = ExtractFilePath(ImportFileName);
	bool DirChanged = SelectDirectory(Directory,TSelectDirOpts() << sdAllowCreate << sdPerformCreate << sdPrompt,0);
	if(DirChanged)
   {
      ImportFileName = Directory + "drsex.csv";
	   RegistryWrite(MMBaseKey "Configuration","ImptMateFile",ImportFileName);      
      Label1->Caption = ImportFileName;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::TouchButton1Click(TObject *Sender)
{
   frmTouchNumpad->Caption = "Enter the number of seconds between checks.";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->INTInitial = Timer->Interval/1000;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
      Timer->Enabled = false;
      Timer->Interval = frmTouchNumpad->INTResult * 1000;
      Timer->Enabled = true;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::TimerTimer(TObject *Sender)
{
  	AnsiString FileName = "";
	try
   {
      if(!DirectoryExists(ExtractFilePath(Application->ExeName) + "CSV Import\\Failed"))
      {
         ForceDirectories(ExtractFilePath(Application->ExeName) + "CSV Import\\Failed");
      }
      if(!DirectoryExists(ExtractFilePath(Application->ExeName) + "CSV Import\\Success"))
      {
         ForceDirectories(ExtractFilePath(Application->ExeName) + "CSV Import\\Success");
      }
      if(FileExists(ImportFileName))
      {
         FileName = ExtractFilePath(ImportFileName) + Now().FormatString("dd mmm yyyy h.nn.ss am/pm ") + ExtractFileName(ImportFileName);
			AnsiString BadFileName = ExtractFilePath(Application->ExeName) + "CSV Import\\Failed\\BadData.csv";
			AnsiString DupFileName = ExtractFilePath(Application->ExeName) + "CSV Import\\Failed\\Duplicates.csv";
			if(RenameFile(ImportFileName, FileName))
         { // If this fails file is in use leave it and try again later.
            bool StartedTrans = false;
            int SecRef = GetNextSecurityRef();

            TStringList *Data = new TStringList;
            TStringList *BadData = new TStringList;
            TStringList *DuplicateData = new TStringList;
            try
            {
               Data->LoadFromFile(FileName);
               if(FileExists(BadFileName))
               {
						BadData->LoadFromFile(BadFileName);
               }
               if(FileExists(DupFileName))
               {
						DuplicateData->LoadFromFile(DupFileName);
               }

               AnsiString NextWord;
               if (!IBTransaction->InTransaction) { IBTransaction->StartTransaction(); StartedTrans = true;}
               for(int i=0; i<Data->Count; i++)
               {
	               int Offset = 0;
                  try
                  {
                     const int TRANS_CODE			= 0;
                     const int DEBTOR_CODE   	= 1;
                     const int NAME					= 2;
                     const int DATE					= 3;
                     const int AMOUNT				= 4;
                     const int INVOICE_NUMBER	= 5;

                     const double GST_RATE_PERCENT = 12.5;

                     NextWord = ReadCSVWord(Data->Strings[i], Offset, ',', '"','\\');
                     int TransCode = StrToIntDef(NextWord,0);
                     NextWord = ReadCSVWord(Data->Strings[i], Offset, ',', '"','\\');
                     int ContactID = StrToIntDef(NextWord,0);
                     NextWord = ReadCSVWord(Data->Strings[i], Offset, ',', '"','\\');
                     AnsiString ContactName = NextWord;
                     NextWord = ReadCSVWord(Data->Strings[i], Offset, ',', '"','\\');
                     TDateTime TimeStamp = NextWord;
                     NextWord = ReadCSVWord(Data->Strings[i], Offset, ',', '"','\\');
                     Currency InvoiceAmountInc = NextWord;
                     Currency TotalCostExcl = (double(InvoiceAmountInc) * double(100.0)) / (GST_RATE_PERCENT + double(100.0));
                     NextWord = ReadCSVWord(Data->Strings[i], Offset, ',', '"','\\');
                     AnsiString InvoiceNumber = "LQ " + NextWord;

                     int ContactKey = ContactLookUp(ContactID,ContactName);

                     // Create a Tab to Store this Invoice.
                     int TabyKey = TDBTab::GetOrCreateTab(0);
                     TDBTab::SetTabOwner(TabyKey,ContactKey,TabMember);

                     ProcessSecurity(SecRef,UserKey,SecurityTypes[secOrderedBy]
                                    ,ProgramLabel,ProgramLabel,Now(),ProgramLabel);


                     SetOrder(TabyKey,ContactKey ,InvoiceAmountInc,TimeStamp,SecRef);

                     if(Invoice->GetInvoiceExists(InvoiceNumber))
                     { // Add this Row to the Duplicates.csv
	                     DuplicateData->Add(Data->Strings[i] + ", Duplicate Invoice Number ");
                     }
                     else
                     {
                        int InvoiceKey = Invoice->CreateInvoice(ContactKey,TotalCostExcl,InvoiceAmountInc,InvoiceNumber);
                        TDBTab::SetTabToInvoice(TabyKey,InvoiceKey);
                     }
                  }
                  catch(Exception &E)
                  {
                     BadData->Add(Data->Strings[i] + ", Error Msg : " + E.Message );
                     MessageBox("Invoice Failed To Import : Line Number " + IntToStr(i) + " Error Msg : " + E.Message , "Error",	MB_OK + MB_ICONWARNING);
                  }
               }
            }
            __finally
            {
               BadData->SaveToFile(BadFileName);
               DuplicateData->SaveToFile(DupFileName);
               Data->SaveToFile(ExtractFilePath(Application->ExeName) + "CSV Import\\Success\\" +  ExtractFileName(FileName));
            	delete Data;
            }
            if (IBTransaction->InTransaction && StartedTrans) { IBTransaction->Commit(); }
         }
      }
   }
   catch(Exception &Err)
   {
   	ShowMessage(Err.Message);
   }
}
//---------------------------------------------------------------------------


int TfrmMain::ContactLookUp(int ContactID, AnsiString ContactName)
{
   int ContactKey = Membership->GetContactByCode(ContactID);
   if(ContactKey == 0)
   {
      ContactKey = Membership->GetContactByName(ContactName);
      if(ContactKey == 0)
      {
         // New Contact.
         ContactKey = Membership->GetOrCreateContact(ContactKey,ContactName);
         Membership->SetContactCode(ContactKey,ContactID);
      }
      else
      {
         Membership->SetContactCode(ContactKey,ContactID);
      }
   }
   return ContactKey;
}


void TfrmMain::SetOrder(int TabKey,int ContactKey ,Currency InvoiceAmountInc,
								TDateTime TimeStamp,int SecurityRef)
{
   bool StartedTrans = false; // Leave the Trans State the way we found it.
	try
   {
      if (!IBTransaction->InTransaction) { IBTransaction->StartTransaction(); StartedTrans = true;}

      IBQuery->SQL->Clear();
      IBQuery->Params->Clear();
      IBQuery->ParamCheck = true;
      IBQuery->SQL->Text =
      " SELECT ORDER_KEY FROM ORDERS "
      " WHERE "
      " ITEM_ID = :ITEM_ID AND "
		" SIZE_NAME = :SIZE_NAME AND "
		" TERMINAL_NAME = :TERMINAL_NAME AND "
		" TRANSNO = :TRANSNO";
      IBQuery->ParamByName("ITEM_ID")->AsInteger = 9999;
      IBQuery->ParamByName("SIZE_NAME")->AsString = "Default";
      IBQuery->ParamByName("TERMINAL_NAME")->AsString = ProgramLabel;
      IBQuery->ParamByName("TRANSNO")->AsInteger = TabKey;
		IBQuery->Open();
		if(IBQuery->IsEmpty())
      {
         IBQuery->Close();
         IBQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDERS, 1) FROM RDB$DATABASE";
         IBQuery->Open();
         int Order_Key = IBQuery->Fields->Fields[0]->AsInteger;

         IBQuery->Close();
         IBQuery->Params->Clear();
         IBQuery->ParamCheck = true;
         IBQuery->SQL->Clear();
         IBQuery->SQL->Text =
         "INSERT INTO ORDERS ("
           "ORDER_KEY,"
           //"ORDERMOVE_KEY,"
           "TAB_KEY,"
           "ITEM_NAME,"
           "ITEM_ID,"
           "SIZE_NAME,"
           "NOTE,"
           //"SERVER_NAME,"
           //"SERVER_INITIALS,"
           "PARTY_NAME,"
           "TABLE_NUMBER,"
           "TABLE_NAME,"
           "SEATNO,"
           "PRICE,"
           "PRICE_ADJUST,"
           "PRINTED,"
           "TRANSNO,"
           "ORDER_TYPE,"
           "TERMINAL_NAME,"
           "MENU_NAME,"
           "TAB_NAME,"
           "COURSE_NAME,"
           "HAPPYHOUR,"
           "ORDER_LOCATION,"
           "TAB_TYPE,"
           "TIME_STAMP,"
           "COST,"
           "LOYALTY_KEY,"
           "MASTER_CONTAINER,"
           "SETMENU_MASK,"
           "SETMENU_GROUP,"
           "ITEM_CATEGORY,"
           "SECURITY_REF,"
           "TIME_KEY,"
           "GST_PERCENT,"
           "COST_GST_PERCENT,"
           //"SIDE_ORDER_KEY,"
           "QTY,"
           "DISCOUNT,"
           "REDEEMED,"
           "ITEM_KITCHEN_NAME,"
           "SIZE_KITCHEN_NAME,"
           "COURSE_KITCHEN_NAME)"
         " VALUES ("
           ":ORDER_KEY,"
           //":ORDERMOVE_KEY,"
           ":TAB_KEY,"
           ":ITEM_NAME,"
           ":ITEM_ID,"
           ":SIZE_NAME,"
           ":NOTE,"
           //":SERVER_NAME,"
           //":SERVER_INITIALS,"
           ":PARTY_NAME,"
           ":TABLE_NUMBER,"
           ":TABLE_NAME,"
           ":SEATNO,"
           ":PRICE,"
           ":PRICE_ADJUST,"
           ":PRINTED,"
           ":TRANSNO,"
           ":ORDER_TYPE,"
           ":TERMINAL_NAME,"
           ":MENU_NAME,"
           ":TAB_NAME,"
           ":COURSE_NAME,"
           ":HAPPYHOUR,"
           ":ORDER_LOCATION,"
           ":TAB_TYPE,"
           ":TIME_STAMP,"
           ":COST,"
           ":LOYALTY_KEY,"
           ":MASTER_CONTAINER,"
           ":SETMENU_MASK,"
           ":SETMENU_GROUP,"
           ":ITEM_CATEGORY,"
           ":SECURITY_REF,"
           ":TIME_KEY,"
           ":GST_PERCENT,"
           ":COST_GST_PERCENT,"
           //":SIDE_ORDER_KEY,"
           ":QTY,"
           ":DISCOUNT,"
           ":REDEEMED,"
           ":ITEM_KITCHEN_NAME,"
           ":SIZE_KITCHEN_NAME,"
           ":COURSE_KITCHEN_NAME);";

         IBQuery->Params->ParamByName("ORDER_KEY")->AsInteger = Order_Key;
         //IBQuery->Params->ParamByName("ORDERMOVE_KEY")->AsInteger = 0;
         IBQuery->Params->ParamByName("TAB_KEY")->AsInteger = TabKey;
         IBQuery->Params->ParamByName("ITEM_NAME")->AsString = "Imported Invoice";
         IBQuery->Params->ParamByName("ITEM_ID")->AsInteger = 9999;
         IBQuery->Params->ParamByName("SIZE_NAME")->AsString = "Default";
         IBQuery->Params->ParamByName("NOTE")->AsString = "";
         //IBQuery->Params->ParamByName("SERVER_NAME")->AsString = "";
         //IBQuery->Params->ParamByName("SERVER_INITIALS")->AsString = "";
         IBQuery->Params->ParamByName("PARTY_NAME")->AsString = "";
         IBQuery->Params->ParamByName("TABLE_NUMBER")->AsInteger = 0;
         IBQuery->Params->ParamByName("TABLE_NAME")->AsString = "";
         IBQuery->Params->ParamByName("SEATNO")->AsInteger = 0;
         IBQuery->Params->ParamByName("PRICE")->AsCurrency = InvoiceAmountInc;
         IBQuery->Params->ParamByName("PRICE_ADJUST")->AsCurrency = 0;
         IBQuery->Params->ParamByName("PRINTED")->AsString = "F";
         IBQuery->Params->ParamByName("TRANSNO")->AsInteger = TabKey;
         IBQuery->Params->ParamByName("ORDER_TYPE")->AsInteger = 0;
         IBQuery->Params->ParamByName("TERMINAL_NAME")->AsString = "ImptMate";
         IBQuery->Params->ParamByName("MENU_NAME")->AsString = "Imported";
         IBQuery->Params->ParamByName("TAB_NAME")->AsString =  "";
         IBQuery->Params->ParamByName("COURSE_NAME")->AsString = "Imported";
			IBQuery->Params->ParamByName("HAPPYHOUR")->AsString 	= "F";
         IBQuery->Params->ParamByName("ORDER_LOCATION")->AsString = ProgramLabel;
         IBQuery->Params->ParamByName("TAB_TYPE")->AsInteger = TabInvoice;
         IBQuery->Params->ParamByName("TIME_STAMP")->AsDateTime = TimeStamp;
         IBQuery->Params->ParamByName("COST")->AsCurrency = 0;
         IBQuery->Params->ParamByName("LOYALTY_KEY")->AsInteger = ContactKey;
         IBQuery->Params->ParamByName("MASTER_CONTAINER")->AsString = "";
         IBQuery->Params->ParamByName("SETMENU_MASK")->AsInteger = 0;
         IBQuery->Params->ParamByName("SETMENU_GROUP")->AsInteger = 0;
         IBQuery->Params->ParamByName("ITEM_CATEGORY")->AsString = "";
         IBQuery->Params->ParamByName("SECURITY_REF")->AsInteger = SecurityRef;
         IBQuery->Params->ParamByName("TIME_KEY")->AsInteger = 0;
         IBQuery->Params->ParamByName("GST_PERCENT")->AsFloat = 12.5;
         IBQuery->Params->ParamByName("COST_GST_PERCENT")->AsFloat = 12.5;
         //IBQuery->Params->ParamByName("SIDE_ORDER_KEY")->AsInteger = 0;
         IBQuery->Params->ParamByName("QTY")->AsFloat = 1;
         IBQuery->Params->ParamByName("DISCOUNT")->AsFloat = 0;
         IBQuery->Params->ParamByName("REDEEMED")->AsFloat = 0;
         IBQuery->Params->ParamByName("ITEM_KITCHEN_NAME")->AsString = "";
         IBQuery->Params->ParamByName("SIZE_KITCHEN_NAME")->AsString = "";
         IBQuery->Params->ParamByName("COURSE_KITCHEN_NAME")->AsString = "";
         IBQuery->ExecSQL();

         // Ensure Catagory is in the archive table.
         IBQuery->Close();
         IBQuery->SQL->Text = "SELECT CATEGORY_KEY,CATEGORY FROM ARCCATEGORIES "
                               "WHERE CATEGORY = :CATEGORY";
         IBQuery->ParamByName("CATEGORY")->AsString = "LQ Imported";
         IBQuery->Open();
         int key = IBQuery->FieldByName("CATEGORY_KEY")->AsInteger;

         if (IBQuery->IsEmpty())
         {
            IBQuery->Close();
            IBQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCCATEGORIES, 1) FROM RDB$DATABASE";
            IBQuery->Open();
            key = IBQuery->Fields->Fields[0]->AsInteger;

            IBQuery->Close();
            IBQuery->Params->Clear();
            IBQuery->SQL->Text =
               "INSERT INTO ARCCATEGORIES ("
                  "CATEGORY_KEY,"
                  "CATEGORY) "
               "VALUES ("
                  ":CATEGORY_KEY,"
                  ":CATEGORY);";
            IBQuery->Params->ParamByName("CATEGORY_KEY")->AsInteger = key;
            IBQuery->Params->ParamByName("CATEGORY")->AsString = "LQ Imported";
            IBQuery->ExecSQL();
         }

         IBQuery->Close();
         IBQuery->Params->Clear();
         IBQuery->SQL->Text =
            "INSERT INTO ORDERCATEGORY ("
               "ORDER_KEY,"
               "CATEGORY_KEY) "
            "VALUES ("
               ":ORDER_KEY,"
               ":CATEGORY_KEY);";
         IBQuery->Params->ParamByName("ORDER_KEY")->AsInteger = Order_Key;
         IBQuery->Params->ParamByName("CATEGORY_KEY")->AsInteger = key;
         IBQuery->ExecSQL();
         // Category key should now point to the archived version of the Category.
      }
      else
      {
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG, "WARNING");
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG, "Order Already Exists");
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG, "Order already added with the Unique Key.");
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG, "Device :" ProgramLabel);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG, "Item :" ProgramLabel);
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG, "Size :" "Default");
         TManagerLogs::Instance().Add(__FUNC__,ERRORLOG, "Trans No :" + IntToStr(TabKey));
      }

   	if (IBTransaction->InTransaction && StartedTrans) { IBTransaction->Commit(); }
	}
	catch(Exception &E)
	{
		if (IBTransaction->InTransaction  && StartedTrans ) { IBTransaction->Rollback(); }
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}


int TfrmMain::GetNextSecurityRef()
{
   bool CloseTransaction = false;
   if (!IBTransaction->InTransaction) { IBTransaction->StartTransaction(); CloseTransaction = true;}
   int key;
   IBQuery->Close();
   IBQuery->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_REF, 1) FROM RDB$DATABASE";
   IBQuery->Open();
   key = IBQuery->Fields->Fields[0]->AsInteger;
   if (IBTransaction->InTransaction && CloseTransaction) { IBTransaction->Commit(); }
   return key;
}

void TfrmMain::ProcessSecurity(int SecRef,int UserKey,
                              AnsiString Event,AnsiString FromVal,
                              AnsiString ToVal,TDateTime TimeStamp,
                              AnsiString TerminalName)
{
   bool CommitTransaction = false;
	if (!IBTransaction->InTransaction) { IBTransaction->StartTransaction(); CommitTransaction = true; }

   int seckey;
   IBQuery->Close();
   IBQuery->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE";
   IBQuery->Open();
   seckey = IBQuery->Fields->Fields[0]->AsInteger;

   // Add new Reocrd.
   IBQuery1->Close();
   IBQuery1->Params->Clear();
   IBQuery1->ParamCheck = true;
   IBQuery1->SQL->Clear();
   IBQuery1->SQL->Text =
      "INSERT INTO SECURITY ("
         "SECURITY_KEY, "
         "SECURITY_REF, "
         "SECURITY_EVENT, "
         "FROM_VAL, "
         "TO_VAL, "
         "TERMINAL_NAME, "
         "USER_KEY, "
         "TIME_STAMP) "
      "VALUES ("
         ":SECURITY_KEY, "
         ":SECURITY_REF, "
         ":SECURITY_EVENT, "
         ":FROM_VAL, "
         ":TO_VAL, "
         ":TERMINAL_NAME, "
         ":USER_KEY, "
         ":TIME_STAMP);";
   IBQuery1->Params->ParamByName("SECURITY_KEY")->AsInteger	= seckey;
   IBQuery1->Params->ParamByName("SECURITY_REF")->AsInteger	= SecRef;
   IBQuery1->Params->ParamByName("USER_KEY")->AsInteger	   = UserKey;
   IBQuery1->Params->ParamByName("SECURITY_EVENT")->AsString  = Event;
   IBQuery1->Params->ParamByName("FROM_VAL")->AsString	    = FromVal;
   IBQuery1->Params->ParamByName("TO_VAL")->AsString			 = ToVal;
   IBQuery1->Params->ParamByName("TIME_STAMP")->AsDateTime	 = TimeStamp;
   IBQuery1->Params->ParamByName("TERMINAL_NAME")->AsString	 = TerminalName;
   IBQuery1->ExecSQL();

	if (IBTransaction->InTransaction && CommitTransaction) { IBTransaction->Commit(); }
}
void __fastcall TfrmMain::TouchButton3Click(TObject *Sender)
{
	TimerTimer(Sender);	
}
//---------------------------------------------------------------------------
AnsiString TfrmMain::ReadCSVWord(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape)
{
	AnsiString Temp = "";

	int StartPos   = Offset + 1; // Not Zero based indexing.
	bool Escaped   = false;
	bool InsideQualifier = false;
	bool Completed   = false;

	for (int i=StartPos; i<=Line.Length() && !Completed; i++)
	{
		if (Escape != NULL && Line[i] == Escape)
		{
			if (Escaped)
			{
				Temp += Escape;
				Escaped = false;
			}
			else
			{
				Escaped = true;
			}
		}
		else if (Qualifier != NULL && Line[i] == Qualifier) // Check for "
		{
			if (Escaped)
			{
				Temp += Qualifier;
				Escaped = false;
			}
			else
			{
				InsideQualifier = !InsideQualifier;
			}
		}
		else if (Line[i] == Delimiter) // Check for ,
		{
			if (Escaped)
			{
				Temp += Delimiter;
				Escaped = false;
			}
			else if (InsideQualifier)
			{
				Temp += Delimiter;
			}
			else
			{
				Completed = true;
			}
		}
		else
		{
			if (Escaped)
			{
				Temp += Escape; // Shouldn't be escape. Assume the Escape char was part of the word.
				Escaped = false;
			}
			Temp += Line[i];
		}
		Offset++;
	}
	return Temp.Trim();
}

void __fastcall TfrmMain::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
   }
}
//---------------------------------------------------------------------------

