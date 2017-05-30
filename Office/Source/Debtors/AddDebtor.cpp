//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddDebtor.h"
#include "AddDebtorCompany.h"
#include "MMData.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define CONTACT_DEBTOR_TYPE 2

TfrmAddDebtor *frmAddDebtor;
//---------------------------------------------------------------------------
__fastcall TfrmAddDebtor::TfrmAddDebtor(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtor::FormShow(TObject *Sender)
{                                                                             
	FormResize(NULL);
	try
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();

		cbCompany->Items->Clear();
		cbCompany->Items->AddObject("- None -", NULL);
		cbCompany->ItemIndex = 0;

		dtDebtorEdit->Close();
		if (Mode == amAddDebtor)
		{
			Caption = "Add Debtor";

			dtDebtorEdit->Open();
			dtDebtorEdit->Append();
			dtpDateOfBirth->Date = Date();
			dtpDateOfBirth->Checked = false;
			chbAllowInvoice->Checked = false;
		}
		else if (Mode == amEditDebtor)// Edit mode
		{
			Caption = "Edit Debtor";

			dtDebtorEdit->ParamByName("Contacts_Key")->AsInteger = DebtorKey;
			dtDebtorEdit->Open();
			dtDebtorEdit->Edit();
			CompanyKey = dtDebtorEdit->FieldByName("Company_Key")->AsInteger;
			if (int(dtDebtorEdit->FieldByName("DateOfBirth")->AsDateTime) == 0)
			{
				dtpDateOfBirth->Date = Date();
				dtpDateOfBirth->Checked = false;
			}
			else
			{
				dtpDateOfBirth->Date = dtDebtorEdit->FieldByName("DateOfBirth")->AsDateTime;
				dtpDateOfBirth->Checked = true;
			}
			cbSex->ItemIndex = cbSex->Items->IndexOf(dtDebtorEdit->FieldByName("Sex")->AsString);
			cbTitle->ItemIndex = cbTitle->Items->IndexOf(dtDebtorEdit->FieldByName("Title")->AsString);
			chbAllowInvoice->Checked = (dtDebtorEdit->FieldByName("Tab_Enalbed")->AsString == "T");
		}
		for (qrCompanies->Open(); !qrCompanies->Eof; qrCompanies->Next())
		{
			int Index = cbCompany->Items->AddObject(qrCompanies->FieldByName("Company_Name")->AsString, (TObject *)qrCompanies->FieldByName("Company_Key")->AsInteger);
			if (qrCompanies->FieldByName("Company_Key")->AsInteger == CompanyKey)
			{
				cbCompany->ItemIndex = Index;
			}
		}
		cbCompany->SetFocus();
	}
	catch (Exception &E)
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	try
	{
		if (Transaction->InTransaction) Transaction->Rollback();
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtor::FormResize(TObject *Sender)
{
	ClientHeight						= btnCancel->Top + btnCancel->Height + GroupBox2->Left;
	ClientWidth							= GroupBox2->Width + (GroupBox2->Left * 2);
	Left									= (Screen->Width - Width) / 2;
	Top									= (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtor::btnOkClick(TObject *Sender)
{
	btnOk->SetFocus(); // Post current editing field.

	if (dtpDateOfBirth->Checked)
	{
		dtDebtorEdit->FieldByName("DateOfBirth")->AsDateTime = (int)dtpDateOfBirth->Date;
	}
	else
	{
		dtDebtorEdit->FieldByName("DateOfBirth")->Clear();
	}
	if (dtDebtorEdit->FieldByName("Name")->AsString == "")
	{
		Application->MessageBox("You must enter a unique name.", "Error", MB_ICONERROR + MB_OK);
		dbeName->SetFocus();
		return;
	}
	dtDebtorEdit->FieldByName("Sex")->AsString							= cbSex->Text;
	dtDebtorEdit->FieldByName("Title")->AsString							= cbTitle->Text;
	dtDebtorEdit->FieldByName("Tab_Enalbed")->AsString					= chbAllowInvoice->Checked?"T":"F";

	dtDebtorEdit->FieldByName("Company_Key")->AsInteger				= int(cbCompany->Items->Objects[cbCompany->ItemIndex]);
	dtDebtorEdit->FieldByName("Contact_Type")->AsInteger				= CONTACT_DEBTOR_TYPE;
	dtDebtorEdit->FieldByName("Contacts_3rdParty_Key")->AsInteger	= 0;
	dtDebtorEdit->FieldByName("PIN")->AsString = "1111"; //Currently a 'Not Null' field!

	if (Mode == amAddDebtor)
	{
		dtDebtorEdit->FieldByName("Total_Spent")->AsCurrency = 0.00;
		dtDebtorEdit->FieldByName("Member_Number")->AsString = GetMemberNumber();
		dtDebtorEdit->FieldByName("Site_ID")->AsInteger = GetSiteID(); //Currently a 'Not Null' field!
	}
	//and now needs a site id!!!
	dtDebtorEdit->Post();

	DebtorKey		= dtDebtorEdit->FieldByName("Contacts_Key")->AsInteger;
	CompanyKey		= dtDebtorEdit->FieldByName("Company_Key")->AsInteger;        
	Transaction->Commit();    

	if (Mode == amAddDebtor)
	{
        UpdateMembershipSubsDetails(DebtorKey);
    }
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
#include "Manager.h"
#include "VariableManager.h"
AnsiString TfrmAddDebtor::GetMemberNumber()
{
	Database::TDBTransaction transaction(dmMMData->dbMenuMate);
	transaction.Start();

	int MemberNumber = 0;

	TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
	IBInternalQuery->SQL->Text =
		"Select "
			"MEMBER_NUMBER "
		"from "
			"CONTACTS "
		"where "
			"(CONTACT_TYPE = 2 or CONTACT_TYPE = 4) "
			"and MEMBER_NUMBER = :MEMBER_NUMBER";

	for(int i=1; MemberNumber==0; i++)
	{
		IBInternalQuery->Close();
//		IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = CONTACT_DEBTOR_TYPE;
		IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = IntToStr(i);
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->Eof)
		{
			MemberNumber = i;
		}
	}
	return IntToStr(MemberNumber);
}
//---------------------------------------------------------------------------
int TfrmAddDebtor::GetSiteID()
{
	Database::TDBTransaction transaction(dmMMData->dbMenuMate);
	transaction.Start();
	TManagerVariable varManager;

	return varManager.GetInt(transaction, vmSiteID, -1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtor::btnAddCompanyClick(TObject *Sender)
{
	frmAddDebtorCompany = new TfrmAddDebtorCompany(this);
	try
	{
		if (frmAddDebtorCompany->AddCompany() == mrOk)
		{
			cbCompany->Items->Clear();
			cbCompany->Items->AddObject("- None -", NULL);
			cbCompany->ItemIndex = 0;
			qrCompanies->Close();
			for (qrCompanies->Open(); !qrCompanies->Eof; qrCompanies->Next())
			{
				int Index = cbCompany->Items->AddObject(qrCompanies->FieldByName("Company_Name")->AsString, (TObject *)qrCompanies->FieldByName("Company_Key")->AsInteger);
				if (qrCompanies->FieldByName("Company_Key")->AsInteger == frmAddDebtorCompany->CompanyKey)
				{
					cbCompany->ItemIndex = Index;
					btnCopyCompanyClick(NULL);
				}
			}
		}
	}
	__finally
	{
		delete frmAddDebtorCompany;
		frmAddDebtorCompany = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtor::dbeFirstNameExit(TObject *Sender)
{
	if (dtDebtorEdit->FieldByName("Name")->AsString == "" && dtDebtorEdit->FieldByName("First_Name")->AsString != "")
	{
		dtDebtorEdit->FieldByName("Name")->AsString = dtDebtorEdit->FieldByName("First_Name")->AsString;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDebtor::btnCopyCompanyClick(TObject *Sender)
{
	if (qrCompanies->Locate("Company_Key", int(cbCompany->Items->Objects[cbCompany->ItemIndex]), TLocateOptions()))
	{
		if (dtDebtorEdit->FieldByName("Phone")->AsString == "")
		{
			dtDebtorEdit->FieldByName("Phone")->AsString					= qrCompanies->FieldByName("Phone")->AsString;
		}
//		if (dtDebtorEdit->FieldByName("Fax")->AsString == "")
//		{
//			dtDebtorEdit->FieldByName("Fax")->AsString					= qrCompanies->FieldByName("Fax")->AsString;
//		}
		if (dtDebtorEdit->FieldByName("Mobile")->AsString == "")
		{
			dtDebtorEdit->FieldByName("Mobile")->AsString				= qrCompanies->FieldByName("Mobile")->AsString;
		}
		if (dtDebtorEdit->FieldByName("Email")->AsString == "")
		{
			dtDebtorEdit->FieldByName("Email")->AsString					= qrCompanies->FieldByName("Email")->AsString;
		}
		if (dtDebtorEdit->FieldByName("Location_Address")->AsString == "")
		{
			dtDebtorEdit->FieldByName("Location_Address")->AsString	= qrCompanies->FieldByName("Location_Address")->AsString;
		}
		if (dtDebtorEdit->FieldByName("Mailing_Address")->AsString == "")
		{
			dtDebtorEdit->FieldByName("Mailing_Address")->AsString	= qrCompanies->FieldByName("Mailing_Address")->AsString;
		}
	}
}
//---------------------------------------------------------------------------
void TfrmAddDebtor::UpdateMembershipSubsDetails(int debtorKey)
{
    Database::TDBTransaction transaction(dmMMData->dbMenuMate);
    transaction.Start();

    int useMemberSubs = 9066;
    int pointsRulesSubs = 192;
    int pointsRules = 0;

    TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
    IBInternalQuery->SQL->Text =
        "Select "
            "INTEGER_VAL "
        "from "
            "VARSPROFILE "
        "where "
            "INTEGER_VAL = :INTEGER_VAL and VARIABLES_KEY = :VARIABLES_KEY ";
    IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = 1;
    IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = useMemberSubs;
    IBInternalQuery->ExecQuery();


    int subsKey = 0;
    TIBSQL *IBInternalQuery1 = transaction.Query(transaction.AddQuery());
    IBInternalQuery1->SQL->Text = "SELECT GEN_ID(GEN_MEMBERSHIP_SUBS_KEY, 1) FROM RDB$DATABASE";
    IBInternalQuery1->ExecQuery();
    subsKey = IBInternalQuery1->Fields[0]->AsInteger;
    TIBSQL *IBInternalQuery2 = transaction.Query(transaction.AddQuery());
    IBInternalQuery2->SQL->Text = "INSERT INTO MEMBERSHIP_SUBS_DETAILS   "
    "(" " MEMBERSHIP_SUBS_KEY, SUBS_PAID_DATE, SUBS_EXPIRY_DATE,SUBS_PAID_AMOUNT, SUBS_PAID_RECEIPT_NO ,SUBS_TYPE, "
                             " SUBS_PAID,POINTS_RULES_SUBS,CONTACTS_KEY,ISLOCAL_MEMBER ) "
                 " VALUES "
                 "(" " :MEMBERSHIP_SUBS_KEY, :SUBS_PAID_DATE, :SUBS_EXPIRY_DATE,:SUBS_PAID_AMOUNT, :SUBS_PAID_RECEIPT_NO ,:SUBS_TYPE, "
                             ":SUBS_PAID,:POINTS_RULES_SUBS,:CONTACTS_KEY,:ISLOCAL_MEMBER ) ";
    if(IBInternalQuery->RecordCount == 0)
    {
        IBInternalQuery2->ParamByName("MEMBERSHIP_SUBS_KEY")->AsInteger = subsKey;
        IBInternalQuery2->ParamByName("POINTS_RULES_SUBS")->AsInteger = pointsRulesSubs;
        IBInternalQuery2->ParamByName("SUBS_PAID")->AsString = "T";
        IBInternalQuery2->ParamByName("ISLOCAL_MEMBER")->AsString = "T";
        IBInternalQuery2->ParamByName("SUBS_TYPE")->AsString = "AUTO";
        IBInternalQuery2->ParamByName("CONTACTS_KEY")->AsInteger = debtorKey;
        pointsRules = 0;
    }
    else
    {
        if(CheckLocalMembership())
        {
            IBInternalQuery2->ParamByName("MEMBERSHIP_SUBS_KEY")->AsInteger = subsKey;
            IBInternalQuery2->ParamByName("POINTS_RULES_SUBS")->AsInteger = 50;
            IBInternalQuery2->ParamByName("SUBS_PAID")->AsString = "F";
            IBInternalQuery2->ParamByName("ISLOCAL_MEMBER")->AsString = "T";
            IBInternalQuery2->ParamByName("SUBS_TYPE")->AsString = "";
            IBInternalQuery2->ParamByName("CONTACTS_KEY")->AsInteger = debtorKey;
            pointsRules = 50;
        }
        else
        {
            IBInternalQuery2->ParamByName("MEMBERSHIP_SUBS_KEY")->AsInteger = subsKey;
            IBInternalQuery2->ParamByName("POINTS_RULES_SUBS")->AsInteger = 192;
            IBInternalQuery2->ParamByName("SUBS_PAID")->AsString = "F";
            IBInternalQuery2->ParamByName("ISLOCAL_MEMBER")->AsString = "F";
            IBInternalQuery2->ParamByName("SUBS_TYPE")->AsString = "";
            IBInternalQuery2->ParamByName("CONTACTS_KEY")->AsInteger = debtorKey;
            pointsRules = 0;
        }
    }
    IBInternalQuery2->ExecQuery();
    transaction.Commit();
    UpdateContactsPointsRules(pointsRules,debtorKey);
}
//---------------------------------------------------------------------------
void TfrmAddDebtor::UpdateContactsPointsRules(int pointsRules,int debtorKey)
{
    Database::TDBTransaction transaction(dmMMData->dbMenuMate);
    transaction.Start();

    TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
    IBInternalQuery->SQL->Text =
        "UPDATE "
            "CONTACTS SET POINTS_RULES = :POINTS_RULES WHERE CONTACTS_KEY = :CONTACTS_KEY ";
    IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = debtorKey;
    IBInternalQuery->ParamByName("POINTS_RULES")->AsInteger = pointsRules;
    IBInternalQuery->ExecQuery();
    transaction.Commit();
}
//---------------------------------------------------------------------------
bool TfrmAddDebtor::CheckLocalMembership()
{
    bool retValue = true;
    Database::TDBTransaction transaction(dmMMData->dbMenuMate);
    transaction.Start();

    TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
    IBInternalQuery->SQL->Text =
        "Select "
            "INTEGER_VAL "
        "from "
            "VARSPROFILE "
        "where "
            "INTEGER_VAL = :INTEGER_VAL and VARIABLES_KEY = :VARIABLES_KEY ";

    IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger != 0;
    IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = 2001;
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount > 0)
    {
        retValue = false;
    }
    else
    {
        if(CheckLoyaltymateEnabled())
            retValue = false;
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TfrmAddDebtor::CheckLoyaltymateEnabled()
{
    bool retValue = false;
    Database::TDBTransaction transaction(dmMMData->dbMenuMate);
    transaction.Start();

    TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
    IBInternalQuery->SQL->Text =
        "Select "
            "INTEGER_VAL "
        "from "
            "VARSPROFILE "
        "where "
            " VARIABLES_KEY = :VARIABLES_KEY ";

    //IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger = 1;
    IBInternalQuery->ParamByName("VARIABLES_KEY")->AsInteger = 7038;
    //IBInternalQuery->ExecQuery();
    /*if(IBInternalQuery->RecordCount > 0)
    {
       retValue = true;
    }  */
    for(IBInternalQuery->ExecQuery(); !IBInternalQuery->Eof; IBInternalQuery->Next())
    {
        if(IBInternalQuery->ParamByName("INTEGER_VAL")->AsInteger == 1)
        {
            retValue = true;
            break;
        }
    }
    return retValue;
}
//---------------------------------------------------------------------------


