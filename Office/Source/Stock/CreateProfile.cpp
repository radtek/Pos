//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>
#include "CreateProfile.h"
#include "Utilities.h"
#include "StockData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmCreateProfile *frmCreateProfile;
//---------------------------------------------------------------------------
__fastcall TfrmCreateProfile::TfrmCreateProfile(TComponent* Owner)
	: TForm(Owner)
{
	StocktakeProfileKey = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateProfile::FormShow(TObject *Sender)
{
	LoadTree();
	if (ProfileMode == pmAddProfile)
	{
		edProfileName->Text = "";
		btnDelete->Visible = false;
		edProfileName->Width = btnDelete->Left + btnDelete->Width - edProfileName->Left;
	}
	else if (ProfileMode == pmEditProfile)
	{
		edProfileName->Width = btnDelete->Left - edProfileName->Left - Label1->Left;
		btnDelete->Visible = true;
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		dtProfile->Close();
		dtProfile->ParamByName("Stocktake_Profile_Key")->AsInteger = StocktakeProfileKey;
		dtProfile->Open();
		edProfileName->Text = dtProfile->FieldByName("Profile_Name")->AsString;
		if (Transaction->InTransaction) Transaction->Commit();
	}
	edProfileName->SetFocus();
}
//---------------------------------------------------------------------------
void TfrmCreateProfile::LoadTree()
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();

	tvStockGroups->Items->BeginUpdate();
	tvStockGroups->Items->Clear();

	qrStock->Close();
	qrStockGroup->Close();
	qrStockCategory->Close();

	dtProfileGroups->Close();
	dtProfile->Close();
	if (ProfileMode == pmEditProfile)
	{
		dtProfile->ParamByName("Stocktake_Profile_Key")->AsInteger = StocktakeProfileKey;
		dtProfile->Open();
		dtProfileGroups->Open();
	}

	TTreeNode *CategoryNode = NULL;
	TTreeNode *GroupNode = NULL;
	TTreeNode *StockNode = NULL;

	for (qrStockCategory->Open(); !qrStockCategory->Eof; qrStockCategory->Next())
	{
		CategoryNode								= tvStockGroups->Items->Add(CategoryNode, qrStockCategory->FieldByName("Stock_Category")->AsString);
		CategoryNode->ImageIndex				= ICON_CLOSED_FOLDER_INDEX;
		CategoryNode->SelectedIndex			= ICON_OPEN_FOLDER_INDEX;
		CategoryNode->Data						= (void *)qrStockCategory->FieldByName("Stock_Category_Key")->AsInteger;
		CategoryNode->StateIndex				= STATE_IMAGE_UNCHECKED;
		bool AllChecked							= true;
		bool SomeChecked							= false;
		for (qrStockGroup->Open(); !qrStockGroup->Eof; qrStockGroup->Next())
		{
			GroupNode								= tvStockGroups->Items->AddChild(CategoryNode, qrStockGroup->FieldByName("Stock_Group")->AsString);
			GroupNode->ImageIndex				= ICON_CLOSED_FOLDER_INDEX;
			GroupNode->SelectedIndex			= ICON_OPEN_FOLDER_INDEX;
			if (ProfileMode == pmEditProfile)
			{
				if (dtProfileGroups->Locate("Stock_Group_Key", qrStockGroup->FieldByName("Stock_Group_Key")->AsInteger, TLocateOptions()))
				{
					GroupNode->StateIndex		= STATE_IMAGE_CHECKED;
					SomeChecked						= true;
				}
				else
				{
					GroupNode->StateIndex		= STATE_IMAGE_UNCHECKED;
					AllChecked						= false;
				}
			}
			else
			{
				GroupNode->StateIndex			= STATE_IMAGE_UNCHECKED;
			}
			GroupNode->Data						= (void *)qrStockGroup->FieldByName("Stock_Group_Key")->AsInteger;
			for (qrStock->Open(); !qrStock->Eof; qrStock->Next())
			{
				StockNode							= tvStockGroups->Items->AddChild(GroupNode, qrStock->FieldByName("Description")->AsString);
				StockNode->ImageIndex			= ICON_BOX_INDEX;
				StockNode->SelectedIndex		= ICON_BOX_INDEX;
				StockNode->StateIndex			= -1;
			}
			qrStock->Close();
		}
		qrStockGroup->Close();
		if (ProfileMode == pmEditProfile)
		{
			if (AllChecked)
			{
				CategoryNode->StateIndex			= STATE_IMAGE_CHECKED;
			}
			else if (SomeChecked)
			{
				CategoryNode->StateIndex			= STATE_IMAGE_GRAY_CHECKED;
			}
			else
			{
				CategoryNode->StateIndex			= STATE_IMAGE_UNCHECKED;
			}
		}
	}
	Transaction->Commit();
	tvStockGroups->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateProfile::tvStockGroupsMouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	 THitTests HitTests = tvStockGroups->GetHitTestInfoAt(X, Y);
	 bool CheckBoxClicked = HitTests.Contains(htOnStateIcon);

	 if (CheckBoxClicked)
	 {
		  TTreeNode *Node = tvStockGroups->GetNodeAt(X, Y);
		  if (Node)
		  {
			  if (Node->Level == 0)
			  {
					if (Node->StateIndex == STATE_IMAGE_UNCHECKED || Node->StateIndex == STATE_IMAGE_GRAY_CHECKED)
					{
						Node->StateIndex = STATE_IMAGE_CHECKED;
					}
					else if (Node->StateIndex == STATE_IMAGE_CHECKED)
					{
						Node->StateIndex = STATE_IMAGE_UNCHECKED;
					}
					TTreeNode *GroupNode = Node->getFirstChild();
					while (GroupNode)
					{
						GroupNode->StateIndex = Node->StateIndex;
						GroupNode = GroupNode->getNextSibling();
					}
					Node->Expand(false);
			  }
			  else if (Node->Level == 1)
			  {
					if (Node->StateIndex == STATE_IMAGE_UNCHECKED)
					{
						Node->StateIndex = STATE_IMAGE_CHECKED;
					}
					else if (Node->StateIndex == STATE_IMAGE_CHECKED)
					{
						Node->StateIndex = STATE_IMAGE_UNCHECKED;
					}
					bool AllSelected = true;
					bool SomeSelected = false;
					TTreeNode *GroupNode = Node->Parent->getFirstChild();
					while (GroupNode)
					{
						if (GroupNode->StateIndex == STATE_IMAGE_CHECKED)
						{
							SomeSelected = true;
						}
						else
						{
							AllSelected = false;
						}
						GroupNode = GroupNode->getNextSibling();
					}
					if (AllSelected)
					{
						Node->Parent->StateIndex = STATE_IMAGE_CHECKED;
					}
					else if (SomeSelected)
					{
						Node->Parent->StateIndex = STATE_IMAGE_GRAY_CHECKED;
					}
					else
					{
						Node->Parent->StateIndex = STATE_IMAGE_UNCHECKED;
					}
			  }
		  }
	 }
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateProfile::btnOkClick(TObject *Sender)
{
	if (edProfileName->Text == "")
	{
		Application->MessageBox("You must give this profile a name.", "Error", MB_ICONERROR + MB_OK);
		edProfileName->SetFocus();
		return;
	}
	if (ProfileMode == pmAddProfile)
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		try
		{
			sqlAddLK->Close();
			sqlAddLK->SQL->Text = "Select gen_id(gen_stocktake_profile, 1) from rdb$database";
			sqlAddLK->ExecQuery();

			StocktakeProfileKey = sqlAddLK->Fields[0]->AsInteger;

			dtProfile->Close();
			dtProfile->Open();
			dtProfile->Append();
			dtProfile->FieldByName("Stocktake_Profile_Key")->AsInteger = StocktakeProfileKey;
			dtProfile->FieldByName("Profile_Name")->AsString = edProfileName->Text;
			try
			{
				dtProfile->Post();
			}
			catch (EIBError &E)
			{
				if (Transaction->InTransaction)
				{
					Transaction->Rollback();
				}
				throw;
			}
			if (SaveProfile() == 0)
			{
				Application->MessageBox("There are no stock groups in this profile.", "Error", MB_ICONERROR + MB_OK);
				if (Transaction->InTransaction) Transaction->Rollback();
				tvStockGroups->SetFocus();
			}
			else
			{
				if (Transaction->InTransaction) Transaction->Commit();
				ModalResult = mrOk;
			}
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction)
			{
				Transaction->Rollback();
			}
			throw;
		}
	}
	else if (ProfileMode == pmEditProfile)
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		try
		{
			dtProfile->Close();
			dtProfile->ParamByName("Stocktake_Profile_Key")->AsInteger = StocktakeProfileKey;
			dtProfile->Open();
			dtProfile->Edit();
			dtProfile->FieldByName("Profile_Name")->AsString = edProfileName->Text;
			try
			{
				dtProfile->Post();
			}
			catch (EIBError &E)
			{
				if (Transaction->InTransaction)
				{
					Transaction->Rollback();
				}
				throw;
			}
			dtProfileGroups->Close();
			dtProfileGroups->Open();
			while (!dtProfileGroups->Eof)
			{
				dtProfileGroups->Delete();
			}
			if (SaveProfile() == 0)
			{
				if (Application->MessageBox("There are no stock groups in this profile.\rDo you wish to delete this profile?", "Delete Profile?", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON2) == ID_OK)
				{
					qrDeleteProfile->Close();
					qrDeleteProfile->ParamByName("Stocktake_Profile_Key")->AsInteger = StocktakeProfileKey;
					qrDeleteProfile->ExecQuery();
					if (Transaction->InTransaction) Transaction->Commit();
					ModalResult = mrOk;
				}
				else
				{
					if (Transaction->InTransaction) Transaction->Rollback();
//					LoadTree();
					tvStockGroups->SetFocus();
				}
			}
			else
			{
				if (Transaction->InTransaction) Transaction->Commit();
				ModalResult = mrOk;
			}
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction)
			{
				Transaction->Rollback();
			}
			throw;
		}
	}
}
//---------------------------------------------------------------------------
int TfrmCreateProfile::SaveProfile()
{
	int Count = 0;
	dtProfileGroups->Close();
	dtProfileGroups->Open();

	TTreeNode *CurItem = tvStockGroups->Items->GetFirstNode();
	while (CurItem)
	{
		if (CurItem->Level == 1 && CurItem->StateIndex == STATE_IMAGE_CHECKED)
		{
			dtProfileGroups->Append();
			dtProfileGroups->FieldByName("Stock_Group_Key")->AsInteger = int(CurItem->Data);
			dtProfileGroups->FieldByName("Stocktake_Profile_Key")->AsInteger = StocktakeProfileKey;
			dtProfileGroups->Post();
			Count++;
		}
		CurItem = CurItem->GetNext();
	}
	return Count;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateProfile::btnDeleteClick(TObject *Sender)
{
	if (Application->MessageBox("Do you wish to delete this profile?", "Delete Profile?", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON2) == ID_OK)
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		try
		{
			qrDeleteProfile->Close();
			qrDeleteProfile->ParamByName("Stocktake_Profile_Key")->AsInteger = StocktakeProfileKey;
			qrDeleteProfile->ExecQuery();
			if (Transaction->InTransaction) Transaction->Commit();
			ModalResult = mrOk;
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction)
			{
				Transaction->Rollback();
			}
			throw;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateProfile::btnSelectAllClick(TObject *Sender)
{
	TTreeNode *Node = tvStockGroups->Items->GetFirstNode();
	while (Node)
	{
		Node->StateIndex = STATE_IMAGE_CHECKED;
		TTreeNode *GroupNode = Node->getFirstChild();
		while (GroupNode)
		{
			GroupNode->StateIndex = STATE_IMAGE_CHECKED;
			GroupNode = GroupNode->getNextSibling();
		}
		Node = Node->getNextSibling();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateProfile::tvStockGroupsKeyPress(TObject *Sender,
      char &Key)
{
	TTreeNode *Node = tvStockGroups->Selected;
	if (Node)
	{
		if (Node->StateIndex == STATE_IMAGE_UNCHECKED) Node->StateIndex = STATE_IMAGE_CHECKED;
		else if (Node->StateIndex == STATE_IMAGE_CHECKED) Node->StateIndex = STATE_IMAGE_UNCHECKED;
	}
}
//---------------------------------------------------------------------------

