//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>
#include "Suppliers.h"
#include "StockData.h"
#include "Utilities.h"
#include "CSV.h"
#include "MMData.h"
#include "Registration.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSuppliers *frmSuppliers;
//---------------------------------------------------------------------------
__fastcall TfrmSuppliers::TfrmSuppliers(TComponent* Owner)
	: TForm(Owner),
	frmAddContact(new TfrmAddContact(NULL))
{
	ContactKey = 0;
	DoubleBuffered = true;
	lbContacts->DoubleBuffered = true;
	Panel9->DoubleBuffered = true;

	dmMMData->Registered(&Registered, &Modules);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::FormShow(TObject *Sender)
{
/*	if (ContactsMode == cmSelect)
	{
		btnOk->Visible = true;
	}
	else if (ContactsMode == cmEdit)
	{
		btnOk->Visible = false;
	}
	FormResize(NULL);

	if (!Transaction->InTransaction) Transaction->StartTransaction();
	qrContact->Close();
	qrContact->Open();
	if (ContactKey != 0)
	{
		qrContact->Locate("CONTACT_LK", ContactKey, TLocateOptions());
	}
	ResizeGrids();
	edFind->SetFocus();*/
	int MYOBAcountSystem = AnsiPos("MYOB",CurrentConnection.AccountSystem); // now different versions
	btnImport->Visible = (Modules & eAccounting) && (MYOBAcountSystem > 0) ;
	if (btnImport->Visible)
	{
		btnImport->Caption = "Import From " + CurrentConnection.AccountSystem;
	}
	if (ContactsMode == cmSelect)
	{
		btnOk->Visible				= true;
		btnCancel->Visible		= true;
		btnClose->Visible			= false;
	}
	else if (ContactsMode == cmEdit)
	{
		btnOk->Visible				= false;
		btnCancel->Visible		= false;
		btnClose->Visible			= true;
	}
	LoadTree();
	if (ContactKey != 0)
	{
		TTreeNode *CurItem = tvContacts->Items->GetFirstNode();
		while (CurItem)
		{
			if (CurItem->Level == 1 && int(CurItem->Data) == ContactKey)
			{
				tvContacts->Selected = CurItem;
				break;
			}
			CurItem = CurItem->GetNext();
		}
	}
	else
	{
		tvContacts->Selected = tvContacts->Items->GetFirstNode();
		if (tvContacts->Selected)
		{
			tvContacts->Selected->Expand(false);
		}
	}
	tvContacts->SetFocus();
}
//---------------------------------------------------------------------------
void TfrmSuppliers::LoadTree()
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();

	ContactTreeLevel = 1;

	tvContacts->Items->BeginUpdate();
	tvContacts->OnChange = NULL;
	tvContacts->Items->Clear();
	qrContact->Close();

	TTreeNode *RootNode				= tvContacts->Items->Add(NULL, "Suppliers");
	RootNode->ImageIndex				= ICON_CLOSED_FOLDER_INDEX;
	RootNode->SelectedIndex			= ICON_OPEN_FOLDER_INDEX;
	RootNode->Data						= NULL;
	TTreeNode *ChildNode = NULL;

	for (qrContact->Open(); !qrContact->Eof; qrContact->Next())
	{
		ChildNode						= tvContacts->Items->AddChild(RootNode, qrContact->FieldByName("Company_Name")->AsString);
		ChildNode->ImageIndex		= ICON_BOX_INDEX;
		ChildNode->SelectedIndex	= ICON_BOX_INDEX;
		ChildNode->Data				= (void *)qrContact->FieldByName("Contact_LK")->AsInteger;
	}
	Transaction->Commit();
	tvContacts->Items->EndUpdate();
	tvContacts->OnChange = tvContactsChange;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::FormClose(TObject *Sender,
		TCloseAction &Action)
{
/*	try
	{
		ContactKey = qrContact->FieldByName("CONTACT_LK")->AsInteger;
		if (Transaction->InTransaction) Transaction->Commit();
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
	}*/
	if (tvContacts->Selected && tvContacts->Selected->Level == 1)
	{
		ContactKey	= int(tvContacts->Selected->Data);
		CompanyName	= tvContacts->Selected->Text;
	}
	else if (tvContacts->Selected && tvContacts->Selected->Level == 0)
	{
		ContactKey	= NULL;
		CompanyName	= "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::FormResize(TObject *Sender)
{
/*	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}
	pnlDetails->Left			= ClientWidth - pnlDetails->Width - pnlSelect->Left;
//	pnlSearch->Left			= pnlDetails->Left;
	pnlSelect->Width			= pnlDetails->Left - (pnlSelect->Left * 2);
	pnlSelect->Height			= ClientHeight - (pnlSelect->Top * 2);
	pnlContacts->Width		= pnlSelect->ClientWidth - (pnlContacts->Left * 2);
	pnlContacts->Height		= pnlSelect->ClientHeight - (pnlContacts->Top * 2);

	btnDeleteContact->Top	= pnlContacts->ClientHeight - btnDeleteContact->Height - pnlSelect->Top;
	btnEditContact->Top		= btnDeleteContact->Top;
	btnAddContact->Top		= btnDeleteContact->Top;

	btnDeleteContact->Left	= pnlContacts->ClientWidth - btnDeleteContact->Width - pnlSelect->Left;
	btnEditContact->Left		= btnDeleteContact->Left - btnEditContact->Width - pnlSelect->Left;
	btnAddContact->Left		= btnEditContact->Left - btnAddContact->Width - pnlSelect->Left;

//	dbgContacts->Top			= 0;
//	dbgContacts->Left			= 0;
//	dbgContacts->Width		= pnlContacts->ClientWidth;
//	dbgContacts->Height		= btnAddContact->Top - pnlSelect->Top;

	btnCancel->Left			= ClientWidth - btnCancel->Width - pnlSelect->Left;
	btnOk->Left					= btnCancel->Left - btnOk->Width - pnlSelect->Left;
	btnCancel->Top				= ClientHeight - btnCancel->Height - pnlSelect->Top;
	btnOk->Top					= btnCancel->Top;
//	pnlSearch->Height			= btnCancel->Top - pnlSearch->Top - pnlSelect->Top;

	dbgContacts->Left			= pnlSelect->Left;
	dbgContacts->Width		= pnlContacts->ClientWidth - (dbgContacts->Left * 2);
	dbgContacts->Height		= btnAddContact->Top - dbgContacts->Top - pnlSelect->Top;
	pnlSearch->Height			= pnlDetails->ClientHeight - (Panel2->Top + Panel2->Height) - (pnlDetails->BorderWidth*2);

	ResizeGrids();*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::WMDisplayChange(TWMDisplayChange& Message)
{
//	FormResize(NULL);
}
//---------------------------------------------------------------------------
void TfrmSuppliers::ResizeGrids()
{
//	dbgContacts->TitleFont->Size				= dbgContacts->Font->Size;
//	dbgContacts->Columns->Items[0]->Width	= dbgContacts->ClientWidth;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::edFindChange(TObject *Sender)
{
/*	if (rbCompanyName->Checked)
	{
		qrContact->Locate("COMPANY_NAME", edFind->Text, TLocateOptions() << loCaseInsensitive << loPartialKey);
	}
	else if (rbContact->Checked)
	{
		qrContact->Locate("CONTACT_NAME", edFind->Text, TLocateOptions() << loCaseInsensitive << loPartialKey);
	}
	else if (rbPhone->Checked)
	{
		qrContact->Locate("PHONE", edFind->Text, TLocateOptions() << loCaseInsensitive << loPartialKey);
	}
	ResizeGrids();*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::rbSearchClick(TObject *Sender)
{
//	edFind->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::edFindKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
/*	if (Key == VK_UP)
	{
		edFind->Text = "";
		Key = NULL;
		qrContact->Prior();
	}
	else if (Key == VK_DOWN)
	{
		edFind->Text = "";
		Key = NULL;
		qrContact->Next();
	}
	else if (Key == VK_RETURN)
	{
		Key = NULL;
		dbgContactsDblClick(Sender);
	} */
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::edFindKeyPress(TObject *Sender, char &Key)
{
/*	if (Key == VK_RETURN)
	{
		Key = NULL;
	}*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::tvContactsChange(TObject *Sender,
      TTreeNode *Node)
{
	if (Node)
	{
		lbeTitle->Caption = Node->Text;
		if (Node->Level == 0)
		{
			lbContacts->Items->Clear();
			for (int i=0; i<Node->Count; i++)
			{
				lbContacts->Items->Add(Node->Item[i]->Text);
			}
			pcContactsDetails->ActivePage = tsContacts;
		}
		else if (Node->Level == ContactTreeLevel)
		{
			if (!Transaction->InTransaction) Transaction->StartTransaction();
			qrContact->Open();
			qrContact->Locate("Contact_LK", int(Node->Data), TLocateOptions());

			lbePhone->Caption						= qrContact->FieldByName("Phone")->AsString;
			lbeMobile->Caption					= qrContact->FieldByName("Mobile")->AsString;
			lbeFax->Caption						= qrContact->FieldByName("Fax")->AsString;
			lbeEmail->Caption						= qrContact->FieldByName("Email")->AsString;
			lbeContactName->Caption				= qrContact->FieldByName("Contact_Name")->AsString;
			memLocationAddress->Lines->Text	= qrContact->FieldByName("Location_Address")->AsString;
			memBillingAddress->Lines->Text	= qrContact->FieldByName("Bill_Address")->AsString;
			memNote->Lines->Text					= qrContact->FieldByName("Note")->AsString;

			Transaction->Commit();
			ResizeGrids();
			pcContactsDetails->ActivePage = tsContact;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::tvContactsMouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button == mbRight)
	{
		if (tvContacts->GetNodeAt(X, Y))
		{
			tvContacts->Selected = tvContacts->GetNodeAt(X, Y);
		}
		else
		{
			tvContacts->Selected = NULL;
		}
		TPoint PopupPoint = ClientToScreen(TPoint(X, Y));
		pmTreeView->Popup(PopupPoint.x , PopupPoint.y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::pmTreeViewPopup(TObject *Sender)
{
	if (tvContacts->Selected && tvContacts->Selected->Level == ContactTreeLevel)
	{
		miEdit->Enabled	= true;
		miRename->Enabled	= true;
		miDelete->Enabled	= true;
	}
	else
	{
		miEdit->Enabled	= false;
		miRename->Enabled	= false;
		miDelete->Enabled	= false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::tvContactsEdited(TObject *Sender,
		TTreeNode *Node, AnsiString &S)
{
	// Edited a Contact
	if (Node->Level == ContactTreeLevel)
	{
		AnsiString OldName = Node->Text;
		try
		{
			if (!Transaction->InTransaction) Transaction->StartTransaction();

			bool RollBack = false;
			if (S == "")
			{
				Application->MessageBox("This contact must have a name!", "Error", MB_OK | MB_ICONERROR);
				RollBack = true;
			}
			else
			{
				qrFindContact->Close();
				qrFindContact->ParamByName("Contact_LK")->AsInteger	= int(Node->Data);
				qrFindContact->ParamByName("Company_Name")->AsString	= "%" + S.UpperCase() + "%";
				qrFindContact->ExecQuery();
				if (!qrFindContact->Eof)
				{
					Application->MessageBox("This contact already exists!", "Error", MB_OK | MB_ICONERROR);
					RollBack = true;
				}
				else
				{
					qrUpdateContact->Close();
					qrUpdateContact->SQL->Text = "Update Contact Set Company_Name = :Company_Name Where Contact_LK = :Contact_LK";
					qrUpdateContact->ParamByName("Contact_LK")->AsInteger	= int(Node->Data);
					qrUpdateContact->ParamByName("Company_Name")->AsString		= S;
					qrUpdateContact->ExecQuery();
				}
			}
			if (RollBack)
			{
				S = OldName;
//				Node->EditText();	// Doesn't work correctly. Use PostMessage(TVM_EDITLABEL)
				PostMessage(Node->TreeView->Handle, TVM_EDITLABEL, 0, reinterpret_cast<LPARAM>(Node->ItemId));
			}
			else
			{
				lbeTitle->Caption = S;
			}
			Transaction->Commit();
		}
		catch (Exception &E)
		{
			S = OldName;
			if (Transaction->InTransaction)
			{
				Transaction->Rollback();
			}
			throw;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::tvContactsDblClick(TObject *Sender)
{
	if (tvContacts->Selected && tvContacts->Selected->Level == ContactTreeLevel)
	{
		if (ContactsMode == cmSelect)
		{
			btnOkClick(NULL);
		}
		else if (ContactsMode == cmEdit)
		{
			miEditClick(NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::btnOkClick(TObject *Sender)
{
	if (ContactsMode == cmSelect)
	{
		if (tvContacts->Selected && tvContacts->Selected->Level == ContactTreeLevel)
		{
			ModalResult = mrOk;
		}
		else if (tvContacts->Selected && tvContacts->Selected->Level == 0)
		{
			if (lbContacts->ItemIndex > -1)
			{
				tvContacts->Selected = tvContacts->Selected->Item[lbContacts->ItemIndex];
				ModalResult = mrOk;
			}
			else
			{
				Application->MessageBox("You need to select a contact or press 'Cancel'.", "Error", MB_ICONERROR + MB_OK);
				ModalResult = mrCancel;
			}
		}
	}
	else if (ContactsMode == cmEdit)
	{
		ModalResult = mrOk;
	}
/*	if (!qrContact->IsEmpty())
	{
		ContactKey	= qrContact->FieldByName("CONTACT_LK")->AsInteger;
		CompanyName	= qrContact->FieldByName("COMPANY_NAME")->AsString;
		ModalResult	= mrOk;
	}*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::miNewClick(TObject *Sender)
{
/*	ContactKey = qrContact->FieldByName("CONTACT_LK")->AsInteger;

	frmAddContact->ContactKey = ContactKey;
	frmAddContact->Mode = amAddContact;
	if (frmAddContact->ShowModal() == mrOk)
	{
		ContactKey = frmAddContact->ContactKey;
		FormShow(NULL);
	} */
	if (tvContacts->Selected && tvContacts->Selected->Level == ContactTreeLevel)
	{
		ContactKey	= int(tvContacts->Selected->Data);
	}
	else
	{
		ContactKey = NULL;
	}
	AnsiString NewContactName = "New Contact";
	if (FindContact(NewContactName))
	{
		int i = 2;
		NewContactName = "New Contact (" + IntToStr(i) + ")";
		while (FindContact(NewContactName))
		{
			i++;
			NewContactName = "New Contact (" + IntToStr(i) + ")";
		}
	}
	TTreeNode *NewNode					= tvContacts->Items->AddChild(tvContacts->Items->GetFirstNode(), NewContactName);
	NewNode->Data							= NULL;
	NewNode->ImageIndex					= ICON_BOX_INDEX;
	NewNode->SelectedIndex				= ICON_BOX_INDEX;

	frmAddContact->ContactKey			= ContactKey;
	frmAddContact->Mode					= amAddContact;

	if (frmAddContact->ShowModal()	== mrOk)
	{
		ContactKey							= frmAddContact->ContactKey;
		NewNode->Data						= (void *)frmAddContact->ContactKey;
	}
	FormShow(NULL);
}
//---------------------------------------------------------------------------
TTreeNode * TfrmSuppliers::FindContact(AnsiString Text)
{
	TTreeNode *CurItem = tvContacts->Items->GetFirstNode();
	while (CurItem)
	{
		if (CurItem->Level == 1 && CurItem->Text == Text)
		{
			return CurItem;
		}
		CurItem = CurItem->GetNext();
	}
	return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::miEditClick(TObject *Sender)
{
/*	if (!qrContact->IsEmpty())
	{
		ContactKey = qrContact->FieldByName("CONTACT_LK")->AsInteger;

		frmAddContact->ContactKey = ContactKey;
		frmAddContact->Mode = amEditContact;
		if (frmAddContact->ShowModal() == mrOk)
		{
			FormShow(NULL);
		}
	}*/
	if (tvContacts->Selected && tvContacts->Selected->Level == ContactTreeLevel)
	{
		ContactKey 								= int(tvContacts->Selected->Data);

		frmAddContact->ContactKey			= ContactKey;
		frmAddContact->Mode					= amEditContact;
		if (frmAddContact->ShowModal()	== mrOk)
		{
			ContactKey							= frmAddContact->ContactKey;
			FormShow(NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::miRenameClick(TObject *Sender)
{
	if (tvContacts->Selected && tvContacts->Selected->Level == ContactTreeLevel)
	{
		tvContacts->Selected->EditText();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::miDeleteClick(TObject *Sender)
{
	if (tvContacts->Selected->Level == 1)
	{
		if (Application->MessageBox("Do you wish to delete this contact?", "Question", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON2) == ID_OK)
		{
			try
			{
				if (!Transaction->InTransaction) Transaction->StartTransaction();

				qrUpdateContact->Close();
				qrUpdateContact->SQL->Text = "Delete From Contact Where Contact_LK = :Contact_LK";
				qrUpdateContact->ParamByName("Contact_LK")->AsInteger = int(tvContacts->Selected->Data);
				qrUpdateContact->ExecQuery();

				tvContacts->Selected->Delete();

				// tvContacts fires onchange event and Commits in that event  
				if (Transaction->InTransaction) Transaction->Commit();
			}
			catch (Exception &E)
			{
				if (Transaction->InTransaction)
				{
					Transaction->Rollback();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::btnFindClick(TObject *Sender)
{
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::FindDialogFind(TObject *Sender)
{
	TTreeNode *CurItem;
	if (tvContacts->Selected)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = tvContacts->Selected->GetNext();
		}
		else
		{
			CurItem = tvContacts->Selected->GetPrev();
		}
	}
	else
	{
		CurItem = tvContacts->Items->GetFirstNode();
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
				tvContacts->Selected = CurItem;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				tvContacts->Selected = CurItem;
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
void __fastcall TfrmSuppliers::btnAddStockClick(TObject *Sender)
{
	miNewClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::btnEditStockClick(TObject *Sender)
{
	if (tvContacts->Selected)
	{
		if (tvContacts->Selected->Level == 0)
		{
			tvContacts->Selected = tvContacts->Selected->Item[lbContacts->ItemIndex];
			miEditClick(NULL);
		}
		else if (tvContacts->Selected->Level == ContactTreeLevel)
		{
			miEditClick(NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::btnRenameStockClick(TObject *Sender)
{
	if (tvContacts->Selected && lbContacts->ItemIndex > -1)
	{
		tvContacts->Selected = tvContacts->Selected->Item[lbContacts->ItemIndex];
		miRenameClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::btnDeleteStockClick(TObject *Sender)
{
	if (tvContacts->Selected)
	{
		if (tvContacts->Selected->Level == 0 && lbContacts->ItemIndex > -1)
		{
			tvContacts->Selected = tvContacts->Selected->Item[lbContacts->ItemIndex];
			miDeleteClick(NULL);
		}
		else if (tvContacts->Selected->Level == ContactTreeLevel)
		{
			miDeleteClick(NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::lbContactsDblClick(TObject *Sender)
{
	if (tvContacts->Selected && lbContacts->ItemIndex > -1)
	{
		tvContacts->Selected = tvContacts->Selected->Item[lbContacts->ItemIndex];
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::lbContactsDrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
{
	lbContacts->Canvas->FillRect(Rect);
	dmUtilities->imlTreeImages->Draw(lbContacts->Canvas, Rect.Left + 2, Rect.Top, 0);
	lbContacts->Canvas->TextOut(Rect.Left + dmUtilities->imlTreeImages->Width + 6, Rect.Top, lbContacts->Items->Strings[Index]);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::tvContactsKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if (Key == VK_DELETE && !tvContacts->IsEditing())
	{
		miDeleteClick(tvContacts);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::tvContactsKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		tvContactsDblClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::btnImportClick(TObject *Sender)
{
	// MYOB Field Indexs for different versions
	int SupplierIDIndex;
	int ContactNameIndex;
	int ContactPhoneIndex;
	int PhoneIndex;
	int FAXIndex;
	int EMailIndex;
	int MYOB1Index;
	int MYOB2Index;
	int MYOB3Index;
	int BillAddressStart;
	int LocAddressStart;
	int AddressFieldCount;
	if (CurrentConnection.AccountSystem == "MYOB 6" || CurrentConnection.AccountSystem == "MYOB 7.2.5H" )
	{
		SupplierIDIndex 	= 0;
		ContactNameIndex 	= 19;
		ContactPhoneIndex = 13;
		PhoneIndex 			= 14;
		FAXIndex   			= 16;
		EMailIndex        = 17;
		MYOB1Index        = 96; // Terms - Payment is Due
		MYOB2Index        = 97; // Discount Days
		MYOB3Index        = 98; // Balance Due Days
		BillAddressStart  = 21;
		LocAddressStart   = 5;
		AddressFieldCount = 8;
	}
	else if (CurrentConnection.AccountSystem == "MYOB 15" )
	{
	  SupplierIDIndex   = 0;
	  ContactNameIndex  = 18;
	  ContactPhoneIndex = 12;
	  PhoneIndex        = 13;
	  FAXIndex          = 15;
	  EMailIndex        = 16;
	  MYOB1Index        = 93; // Terms - Payment is Due
	  MYOB2Index        = 94; // Discount Days
	  MYOB3Index        = 95; // Balance Due Days
	  BillAddressStart  = 20;
	  LocAddressStart   = 4;
	  AddressFieldCount = 7;
	}
	else
	{
		Application->MessageBox("MYOB version is not supported for the Import!", "Error", MB_OK + MB_ICONERROR);
	}
	if (odImport->Execute())
	{
		qrContactGroup->Close();
		qrContactGroup->Open();

		if (qrContactGroup->Eof)
		{
			sqlAddLK->Close();
			sqlAddLK->SQL->Text = "select gen_id(gen_contact_group_key, 1) from rdb$database";
			sqlAddLK->ExecQuery();

			qrAddContactGroup->Close();
			qrAddContactGroup->ParamByName("Contact_Group_Key")->AsInteger	= sqlAddLK->Fields[0]->AsInteger;
			qrAddContactGroup->ParamByName("Contact_Group")->AsString		= "All";
			qrAddContactGroup->ParamByName("Sort_Order")->AsInteger			= 0;
			qrAddContactGroup->ExecSQL();
			sqlAddLK->Close();

			qrContactGroup->Close();
			qrContactGroup->Open();
			if (qrContactGroup->Eof)
			{
				Application->MessageBox("Supplier group not found!", "Error", MB_OK + MB_ICONERROR);
				return;
			}
		}

		TCsv ImportCSV;
		ImportCSV.Delimiter = '\t';
		ImportCSV.LoadFromFile(odImport->FileName);

		if (ImportCSV.RowCount > 0)
		{
			for (int i=1; i<ImportCSV.RowCount; i++)
			{
				if (ImportCSV.Cells[0][i] != "")
				{
					if (!Transaction->InTransaction) Transaction->StartTransaction();

					qrFindContact->Close();
					qrFindContact->ParamByName("Contact_LK")->AsInteger	= -1;
					qrFindContact->ParamByName("Company_Name")->AsString	= ImportCSV.Cells[0][i].UpperCase();
					qrFindContact->ExecQuery();
					if (qrFindContact->Eof)
					{
						qrContactGroup->Open();

						sqlAddLK->Close();
						sqlAddLK->SQL->Text = "select gen_id(gen_contact, 1) from rdb$database";
						sqlAddLK->ExecQuery();

						qrAddContact->Close();
						qrAddContact->ParamByName("Contact_LK")->AsInteger				= sqlAddLK->Fields[0]->AsInteger;
						qrAddContact->ParamByName("Contact_Group_Key")->AsInteger	= qrContactGroup->FieldByName("Contact_Group_Key")->AsInteger;
						qrAddContact->ParamByName("Company_Name")->AsString			= ImportCSV.Cells[SupplierIDIndex][i];
						qrAddContact->ParamByName("Supplier_ID")->AsString				= ImportCSV.Cells[SupplierIDIndex][i];
						qrAddContact->ParamByName("Contact_Name")->AsString			= ImportCSV.Cells[ContactNameIndex][i];
						qrAddContact->ParamByName("Contact_Phone")->AsString			= ImportCSV.Cells[ContactPhoneIndex][i];
						qrAddContact->ParamByName("Phone")->AsString						= ImportCSV.Cells[PhoneIndex][i];
						qrAddContact->ParamByName("FAX")->AsString						= ImportCSV.Cells[FAXIndex][i];
						qrAddContact->ParamByName("EMail")->AsString						= ImportCSV.Cells[EMailIndex][i];

						qrAddContact->ParamByName("MYOB1")->AsString						= ImportCSV.Cells[MYOB1Index][i];	// Terms - Payment is Due
						qrAddContact->ParamByName("MYOB2")->AsString						= ImportCSV.Cells[MYOB2Index][i];	// Discount Days
						qrAddContact->ParamByName("MYOB3")->AsString						= ImportCSV.Cells[MYOB3Index][i];	// Balance Due Days

						std::auto_ptr<TStringList> Address(new TStringList());
						// billing address  ( includes Address 1,2,3,4 + Post Code City Country etc ) fields 21 to 29
						for (int j = BillAddressStart; j < (BillAddressStart + AddressFieldCount); j++)
						{
							if (ImportCSV.Cells[j][i] != "")
							{
								Address->Add(ImportCSV.Cells[j][i]);
							}
						}
						qrAddContact->ParamByName("Bill_Address")->AsString			= Address->Text;
						Address->Clear();
						// location address - fields 5 to 13
						for (int j = LocAddressStart; j < (LocAddressStart + AddressFieldCount); j++)
						{
							if (ImportCSV.Cells[j][i] != "")
							{
								Address->Add(ImportCSV.Cells[j][i]);
							}
						}
						qrAddContact->ParamByName("Location_Address")->AsString		= Address->Text;
						qrAddContact->ParamByName("Note")->AsString						= "Imported from MYOB";
						qrAddContact->ExecQuery();
					}
					if (Transaction->InTransaction) Transaction->Commit();
				}
			}
		}
		FormShow(NULL);
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmSuppliers::Splitter1CanResize(TObject *Sender,
		int &NewSize, bool &Accept)
{
	if (NewSize < Panel8->Constraints->MinWidth)
	{
		Panel8->Width = Panel8->Constraints->MinWidth;
		Accept = false;
	}
}
//---------------------------------------------------------------------------


