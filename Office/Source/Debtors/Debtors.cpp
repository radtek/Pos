//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Debtors.h"
#include "Utilities.h"
#include "MMData.h"
#include "AddDebtorCompany.h"
#include "AddDebtor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TfrmDebtors *frmDebtors;
//---------------------------------------------------------------------------
static const Debtors::CompanyKey NO_COMPANY(0, "");
//---------------------------------------------------------------------------
__fastcall TfrmDebtors::TfrmDebtors(TComponent* Owner)
	: TForm(Owner)
{
	DoubleBuffered = true;
	Panel9->DoubleBuffered = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::FormShow(TObject *Sender)
{
	if (Transaction->InTransaction) Transaction->Commit();
	if (DebtorsMode == mmSelect)
	{
		btnOk->Visible				= true;
		btnCancel->Visible		= true;
		btnClose->Visible			= false;
	}
	else if (DebtorsMode == mmEdit)
	{
		btnOk->Visible				= false;
		btnCancel->Visible		= false;
		btnClose->Visible			= true;
	}
	tvDebtors->Items->BeginUpdate();
	LoadTree();
	if (DebtorKey != 0)
	{
		TTreeNode *CurItem = tvDebtors->Items->GetFirstNode();
		while (CurItem)
		{
			if (CurItem->Level == 1 && int(CurItem->Data) == DebtorKey)
			{
				tvDebtors->Selected = CurItem;
				break;
			}
			CurItem = CurItem->GetNext();
		}
	}
	else if (CompanyKey != 0)
	{
		TTreeNode *CurItem = tvDebtors->Items->GetFirstNode();
		while (CurItem)
		{
			if (CurItem->Level == 0 && int(CurItem->Data) == CompanyKey)
			{
				tvDebtors->Selected = CurItem;
				break;
			}
			CurItem = CurItem->GetNext();
		}
	}
	else
	{
		tvDebtors->Selected = tvDebtors->Items->GetFirstNode();
		if (!tvDebtors->Selected)
		{
			lbeTitle->Caption = "";
			pcDebtors->ActivePage = tsDebtors;
		}
	}
	tvDebtorsChange(tvDebtors, tvDebtors->Selected);
	tvDebtors->Items->EndUpdate();
	tvDebtors->SetFocus();
}
//---------------------------------------------------------------------------
void TfrmDebtors::LoadTree()
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();

	tvDebtors->Items->BeginUpdate();
	tvDebtors->OnChange = NULL;
	tvDebtors->Items->Clear();

	companies.clear();
	companies[NO_COMPANY].Key = 0;
	if (chbShowCompanies->Checked)
	{
		companies[NO_COMPANY].Name = "Unknown";

		qrDebtors->Close();
		qrDebtors->SQL->Text =
			"Select "
				"Company_Key,"
				"Company_Name,"
				"Phone Company_Phone,"
				"Fax Company_Fax,"
				"Mobile Company_Mobile,"
				"Email Company_Email,"
				"Location_Address Company_Location_Address,"
				"Mailing_Address Company_Mailing_Address,"
				"Note Company_Note "
			"From "
				"Companies "
			"Order By Company_Name";

		for (qrDebtors->Open(); !qrDebtors->Eof; qrDebtors->Next())
		{
//			Debtors::Company& company = companies[Debtors::CompanyKey(qrDebtors->FieldByName("Company_Key")->AsInteger)];
			Debtors::Company& company = companies[qrDebtors->FieldByName("Company_Key")->AsInteger];

			company.Key = qrDebtors->FieldByName("Company_Key")->AsInteger;
			company.Name = qrDebtors->FieldByName("Company_Name")->AsString;
			company.Phone = qrDebtors->FieldByName("Company_Phone")->AsString;
			company.Fax = qrDebtors->FieldByName("Company_Fax")->AsString;
			company.Mobile = qrDebtors->FieldByName("Company_Mobile")->AsString;
			company.Email = qrDebtors->FieldByName("Company_Email")->AsString;
			company.LocationAddress = qrDebtors->FieldByName("Company_Location_Address")->AsString;
			company.PostalAddress = qrDebtors->FieldByName("Company_Mailing_Address")->AsString;
			company.Note = qrDebtors->FieldByName("Company_Note")->AsString;
		}
	}
	else
	{
		companies[NO_COMPANY].Name = "Debtors";
	}

	qrDebtors->Close();
	qrDebtors->SQL->Text =
		"Select "
			"Contacts_Key,"
			"Company_Key,"
			"Name,"
			"First_Name,"
			"Middle_Name,"
			"Last_Name,"
			"Member_Number,"
			"Phone,"
			"Phone_Extension,"
			"Mobile,"
			"EMail,"
			"Fax,"
			"Location_Address,"
			"Mailing_Address "
		"From "
			"Contacts "
		"Where "
			"Contact_Type = 2 and member_type = 1 "
		"Order By Name";

	for (qrDebtors->Open(); !qrDebtors->Eof; qrDebtors->Next())
	{
		Debtors::Debtor debtor;

		debtor.Key = qrDebtors->FieldByName("Contacts_Key")->AsInteger;
		debtor.Name = qrDebtors->FieldByName("Name")->AsString;
		debtor.FirstName = qrDebtors->FieldByName("First_Name")->AsString;
		debtor.MiddleName = qrDebtors->FieldByName("Middle_Name")->AsString;
		debtor.LastName = qrDebtors->FieldByName("Last_Name")->AsString;
		debtor.CustomerNumber = qrDebtors->FieldByName("Member_Number")->AsString;
		debtor.Phone = qrDebtors->FieldByName("Phone")->AsString;
		debtor.PhoneExt = qrDebtors->FieldByName("Phone_Extension")->AsString;
		debtor.Fax = qrDebtors->FieldByName("Fax")->AsString;
		debtor.Mobile = qrDebtors->FieldByName("Mobile")->AsString;
		debtor.Email = qrDebtors->FieldByName("EMail")->AsString;
		debtor.LocationAddress = qrDebtors->FieldByName("Location_Address")->AsString;
		debtor.PostalAddress = qrDebtors->FieldByName("Mailing_Address")->AsString;

		if (debtor.CustomerNumber == "0") debtor.CustomerNumber = "";

		if (chbShowCompanies->Checked)
		{
//			companies[Debtors::CompanyKey(qrDebtors->FieldByName("Company_Key")->AsInteger)].Debtors[debtor.Key] = debtor;
//			companies[Debtors::CompanyKey(qrDebtors->FieldByName("Company_Key")->AsInteger)].Debtors.push_back(debtor);
			companies[qrDebtors->FieldByName("Company_Key")->AsInteger].Debtors.push_back(debtor);
		}
		else
		{
//			companies[NO_COMPANY].Debtors[debtor.Key] = debtor;
			companies[NO_COMPANY].Debtors.push_back(debtor);
		}
	}

	Debtors::CompanyDebtors::const_iterator i;
	for (i=companies.begin(); i!=companies.end(); ++i)
	{
		TTreeNode *companyNode = tvDebtors->Items->Add(NULL, i->second.Name);
//		TTreeNode *companyNode = tvDebtors->Items->Add(NULL, i->Name);

		companyNode->ImageIndex			= ICON_CLOSED_FOLDER_INDEX;
		companyNode->SelectedIndex		= ICON_OPEN_FOLDER_INDEX;
		companyNode->Data					= (void *)i->second.Key;
//		companyNode->Data					= (void *)i->Key;

		Debtors::DebtorList::const_iterator j;
		for (j=i->second.Debtors.begin(); j!=i->second.Debtors.end(); ++j)
//		for (j=i->Debtors.begin(); j!=i->Debtors.end(); ++j)
		{
//			TTreeNode *debtorNode = tvDebtors->Items->AddChild(companyNode, j->second.Name);
			TTreeNode *debtorNode = tvDebtors->Items->AddChild(companyNode, j->Name);
			debtorNode->ImageIndex		= ICON_BOX_INDEX;
			debtorNode->SelectedIndex	= ICON_BOX_INDEX;
//			debtorNode->Data				= (void *)j->second.Key;
			debtorNode->Data				= (void *)j->Key;
		}
		companyNode->Expand(true);
	}
	tvDebtors->OnChange = tvDebtorsChange;
	tvDebtors->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::chbShowCompaniesClick(TObject *Sender)
{
	FormShow(NULL);
//	LoadTree();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::tvDebtorsChange(TObject *Sender,
      TTreeNode *Node)
{
	if (Node)
	{
		lbeTitle->Caption = Node->Text;

		const Debtors::Company* company = 0;
		const Debtors::Debtor* debtor = 0;

		TTreeNode *companyNode = 0;
		TTreeNode *debtorNode = 0;

		if (Node->Level == 0)
		{
			companyNode = Node;

			pcDebtors->ActivePage = tsCompanies;
		}
		else if (Node->Level == 1)
		{
			debtorNode = Node;
			companyNode = Node->Parent;

			pcDebtors->ActivePage = tsDebtors;
		}


		if (companyNode && companyNode->Data)
		{
			if (chbShowCompanies->Checked)
			{
				//Debtors::FindCompanyPredicate f(int(companyNode->Data));

				//Debtors::CompanyDebtors::const_iterator c =
					//std::find_if(companies.begin(), companies.end(), f);

				Debtors::CompanyDebtors::const_iterator c = companies.find(Debtors::CompanyKey(int(companyNode->Data)));
				if (c != companies.end())
				{
					company = &c->second;
					//company = &(*c);
				}
			}
		}

		pnlCompanyDetails->Visible	= company;
		btnDeleteCompany->Enabled = company;
		btnEditCompany->Enabled = company;

		if (debtorNode && debtorNode->Data)
		{
			Debtors::DebtorList::const_iterator d;

			if (!company)
			{
				company = &companies[NO_COMPANY];
			}

			Debtors::FindDebtorPredicate f(int(debtorNode->Data));
			d = std::find_if(company->Debtors.begin(), company->Debtors.end(), f);

//				d = company->Debtors.find(int(debtorNode->Data));
			if (d != company->Debtors.end())
			{
//					debtor = &d->second;
				debtor = &(*d);
			}
			/*}
			else
			{
				d = companies[0].Debtors.find(int(debtorNode->Data));
				if (d != companies[0].Debtors.end())
				{
					debtor = &d->second;
				}
			}*/
		}

		pnlDebtorDetails->Visible = debtor;
		btnEditDebtor->Enabled = debtor;
		btnDeleteDebtor->Enabled = debtor;

		if (company)
		{
			CompanyKey = company->Key;

			lbeCompanyName->Caption = company->Name;
			lbeCompanyPhone->Caption = company->Phone;
			lbeCompanyFax->Caption = company->Fax;
			lbeCompanyMobile->Caption = company->Mobile;
			lbeCompanyEMail->Caption = company->Email;
			memCompanyLocationAddress->Text = company->LocationAddress;
			memCompanyPostalAddress->Text = company->PostalAddress;
			memCompanyNote->Text = company->Note;
		}
		else
		{
			CompanyKey = 0;
		}

		if (debtor)
		{
			DebtorKey = debtor->Key;

			lbeFirstName->Caption = debtor->FirstName;
			lbeMiddleName->Caption = debtor->MiddleName;
			lbeLastName->Caption = debtor->LastName;
			lbeCustomerNumber->Caption = debtor->CustomerNumber;
			lbeDebtorPhone->Caption = debtor->Phone;
			lbeDebtorPhoneExt->Caption = debtor->PhoneExt;
			lbeDebtorFax->Caption = debtor->Fax;
			lbeDebtorMobile->Caption = debtor->Mobile;
			lbeDebtorEMail->Caption = debtor->Email;
			memDebtorLocationAddress->Text = debtor->LocationAddress;
			memDebtorPostalAddress->Text = debtor->PostalAddress;

			lbeCustomerNumber->Visible = (lbeCustomerNumber->Caption != "");
		}
		else
		{
			DebtorKey = 0;
		}
	}
	else
	{
		pnlDebtorDetails->Visible = false;
		pnlCompanyDetails->Visible = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	try
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		FindDialog->CloseDialog();
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::miEditClick(TObject *Sender)
{
	btnEditClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::miNewDebtorClick(TObject *Sender)
{
	btnAddDebtorClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::tvDebtorsDblClick(TObject *Sender)
{
	if (tvDebtors->Selected && tvDebtors->Selected->Level == 1)
	{
		if (DebtorsMode == mmSelect)
		{
			btnOkClick(NULL);
		}
		else if (DebtorsMode == mmEdit)
		{
			miEditClick(NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::btnOkClick(TObject *Sender)
{
	if (tvDebtors->IsEditing())
	{
		if (tvDebtors->Selected)
		{
			tvDebtors->Selected->EndEdit(false);
		}
	}
	else
	{
		if (DebtorsMode == mmSelect)
		{
			if (tvDebtors->Selected && tvDebtors->Selected->Level == 1)
			{
				ModalResult = mrOk;
			}
			else
			{
				Application->MessageBox("You need to select a contact or press 'Cancel'.", "Error", MB_ICONERROR + MB_OK);
			}
		}
		else if (DebtorsMode == mmEdit)
		{
			ModalResult = mrOk;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::btnAddDebtorClick(TObject *Sender)
{
	if (tvDebtors->Selected)
	{
		if (tvDebtors->Selected->Level == 1)
		{
			if (chbShowCompanies->Checked)
			{
				CompanyKey = int(tvDebtors->Selected->Parent->Data);
			}
			else if (qrDebtors->Locate("Contacts_Key", int(tvDebtors->Selected->Data), TLocateOptions()))
			{
				CompanyKey = qrDebtors->FieldByName("Company_Key")->AsInteger;
			}
		}
		else
		{
			if (chbShowCompanies->Checked)
			{
				CompanyKey = int(tvDebtors->Selected->Data);
			}
			else
			{
				CompanyKey = 0;
			}
		}
	}
	else
	{
		CompanyKey = 0;
	}
	frmAddDebtor = new TfrmAddDebtor(this);
	try
	{
		frmAddDebtor->DebtorKey			= 0;
		frmAddDebtor->CompanyKey		= CompanyKey;
		frmAddDebtor->Mode				= amAddDebtor;
		if (frmAddDebtor->ShowModal()	== mrOk)
		{
			DebtorKey = frmAddDebtor->DebtorKey;
			FormShow(NULL);
		}
	}
	__finally
	{
		delete frmAddDebtor;
		frmAddDebtor = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::btnEditClick(TObject *Sender)
{
	if (tvDebtors->Selected && tvDebtors->Selected->Level == 0 && chbShowCompanies->Checked)
	{
		CompanyKey = int(tvDebtors->Selected->Data);
		if (CompanyKey != 0)
		{
			frmAddDebtorCompany = new TfrmAddDebtorCompany(this);
			try
			{
				if (frmAddDebtorCompany->EditCompany(CompanyKey) == mrOk)
				{
					FormShow(NULL);
//					LoadTree();
				}
			}
			__finally
			{
				delete frmAddDebtorCompany;
				frmAddDebtorCompany = NULL;
			}
		}
	}
	else if (tvDebtors->Selected && tvDebtors->Selected->Level == 1 && Sender == (TObject *)btnEditCompany)
	{
		CompanyKey = int(tvDebtors->Selected->Parent->Data);
		if (CompanyKey != 0)
		{
			frmAddDebtorCompany = new TfrmAddDebtorCompany(this);
			try
			{
				if (frmAddDebtorCompany->EditCompany(CompanyKey) == mrOk)
				{
					FormShow(NULL);
//					LoadTree();
				}
			}
			__finally
			{
				delete frmAddDebtorCompany;
				frmAddDebtorCompany = NULL;
			}
		}
	}
	else if (tvDebtors->Selected && tvDebtors->Selected->Level == 1)
	{
		if (chbShowCompanies->Checked)
		{
			CompanyKey						= int(tvDebtors->Selected->Parent->Data);
		}
		else
		{
			CompanyKey						= 0;
		}
		DebtorKey							= int(tvDebtors->Selected->Data);

		frmAddDebtor = new TfrmAddDebtor(this);
		try
		{
			frmAddDebtor->DebtorKey			= DebtorKey;
			frmAddDebtor->CompanyKey		= CompanyKey;

			frmAddDebtor->Mode				= amEditDebtor;
			if (frmAddDebtor->ShowModal()	== mrOk)
			{
				CompanyKey						= frmAddDebtor->CompanyKey;
				FormShow(NULL);
			}
		}
		__finally
		{
			delete frmAddDebtor;
			frmAddDebtor = NULL;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::tvDebtorsMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button == mbRight)
	{
		tvDebtors->Selected = tvDebtors->GetNodeAt(X, Y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::miNewCompanyClick(TObject *Sender)
{
	btnAddCompanyClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::btnAddCompanyClick(TObject *Sender)
{
	if (tvDebtors->Selected)
	{
		if (chbShowCompanies->Checked)
		{
			if (tvDebtors->Selected->Level == 1)
			{
				DebtorKey		= int(tvDebtors->Selected->Data);
				CompanyKey		= int(tvDebtors->Selected->Parent->Data);
			}
			else
			{
				DebtorKey		= 0;
				CompanyKey		= int(tvDebtors->Selected->Data);
			}
		}
		else if (tvDebtors->Selected->Level == 1)
		{
			DebtorKey			= int(tvDebtors->Selected->Data);
			CompanyKey 			= 0;
		}
		else
		{
			DebtorKey			= 0;
			CompanyKey 			= 0;
		}
	}
	else
	{
		CompanyKey = 0;
	}
	frmAddDebtorCompany = new TfrmAddDebtorCompany(this);
	try
	{
		CompanyKey = int(tvDebtors->Selected->Data);
		if (frmAddDebtorCompany->AddCompany() == mrOk)
		{
			FormShow(NULL);
//			LoadTree();
		}
	}
	__finally
	{
		delete frmAddDebtorCompany;
		frmAddDebtorCompany = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::btnDeleteDebtorClick(TObject *Sender)
{
	if (tvDebtors->Selected && tvDebtors->Selected->Level == 1)
	{
		DebtorKey = int(tvDebtors->Selected->Data);
		if (DebtorKey)
		{
			qrDebtorInvoices->Close();
			qrDebtorInvoices->ParamByName("Contacts_Key")->AsInteger = DebtorKey;
			qrDebtorInvoices->Open();
			if (qrDebtorInvoices->Eof)
			{
				if (Application->MessageBox("Are you sure you want to delete this debtor?", "Delete?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
				{
					qrDeleteDebtor->Close();
					qrDeleteDebtor->ParamByName("Name")->AsString = tvDebtors->Selected->Text + " " + Now().FormatString("dd-mm-yyyy");
					qrDeleteDebtor->ParamByName("Contacts_Key")->AsInteger = DebtorKey;
					qrDeleteDebtor->ExecSQL();
					FormShow(NULL);
				}
			}
			else
			{
				Application->MessageBox("This debtor still has 1 or more invoices due.", "Error", MB_ICONERROR + MB_OK);
			}
			qrDebtorInvoices->Close();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::btnFindClick(TObject *Sender)
{
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtors::FindDialogFind(TObject *Sender)
{
	TTreeNode *CurItem;
	if (tvDebtors->Selected)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = tvDebtors->Selected->GetNext();
		}
		else
		{
			CurItem = tvDebtors->Selected->GetPrev();
		}
	}
	else
	{
		CurItem = tvDebtors->Items->GetFirstNode();
	}
	while (CurItem)
	{
		AnsiString FindText, NodeText;
		if (FindDialog->Options.Contains(frMatchCase))
		{
			FindText = FindDialog->FindText;
			NodeText = CurItem->Text;
		}
		else
		{
			FindText = FindDialog->FindText.UpperCase();
			NodeText = CurItem->Text.UpperCase();
		}
		if (FindDialog->Options.Contains(frWholeWord))
		{
			if (FindText == NodeText)
			{
				tvDebtors->Selected = CurItem;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				tvDebtors->Selected = CurItem;
				break;
			}
		}
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = CurItem->GetNext();
		}
		else
		{
			CurItem = CurItem->GetPrev();
		}
	}
	if (!CurItem)
	{
		Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
	}
}
//---------------------------------------------------------------------------
bool TfrmDebtors::HasMembershipInstalled(bool ShowTheMessage)
{
	bool Result = false;
	if (!frmDebtors->Transaction->InTransaction)
		frmDebtors->Transaction->StartTransaction();
	// Check if a Contact Type of 4 is in the DB then only allow to add in Membership
	frmDebtors->sqlCheckDebtors->Close();
	frmDebtors->sqlCheckDebtors->ExecQuery();
	if (frmDebtors->sqlCheckDebtors->FieldByName("CntContactType ")->AsInteger > 0)
	{
		Result = true;
		frmDebtors->sqlCheckDebtors->Close();
		frmDebtors->Transaction->Commit();
		if (ShowTheMessage)
		{
			Application->MessageBox("Debtors/Members must be added in Membership!", "Error", MB_OK + MB_ICONERROR);
		}	
	}
	else
	{
		frmDebtors->sqlCheckDebtors->Close();
		frmDebtors->Transaction->Commit();
	}
	return Result;
}
//---------------------------------------------------------------------------

