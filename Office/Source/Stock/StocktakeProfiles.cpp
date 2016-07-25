//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <memory>
#include "StocktakeProfiles.h"
#include "Utilities.h"
#include "StockData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmStocktakeProfiles::TfrmStocktakeProfiles(TComponent* Owner)
	: TForm(Owner)
{
	StocktakeProfileKey = 0;
}
//---------------------------------------------------------------------------
bool TfrmStocktakeProfiles::Execute(Stock::TStocktakeProfiles &StocktakeProfiles, AnsiString &Location)
{
	if (ShowModal() == mrOk)
	{
		for (TTreeNode *CurItem = tvProfiles->Items->GetFirstNode(); CurItem!=NULL; CurItem=CurItem->getNextSibling())
		{
			if (CurItem->StateIndex == STATE_IMAGE_CHECKED)
			{
				Stock::TStocktakeProfile StocktakeProfile;

				StocktakeProfile.ProfileKey	= reinterpret_cast<int>(CurItem->Data);
				StocktakeProfile.Name			= CurItem->Text;

				for (TTreeNode *CurCategory = CurItem->getFirstChild(); CurCategory!=NULL; CurCategory=CurCategory->getNextSibling())
				{
					for (TTreeNode *CurGroup = CurCategory->getFirstChild(); CurGroup!=NULL; CurGroup=CurGroup->getNextSibling())
					{
						StocktakeProfile.StockGroups.push_back(reinterpret_cast<int>(CurGroup->Data));
					}
				}
				StocktakeProfiles.push_back(StocktakeProfile);
			}
		}
		if (lbLocations->ItemIndex > -1)
		{
			Location = lbLocations->Items->Strings[lbLocations->ItemIndex];
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktakeProfiles::FormShow(TObject *Sender)
{
	LoadTree();
	if (tvProfiles->Items->Count == 0)
	{
		Application->MessageBox("Press 'Create Profile' to create a stocktake profile first.", "Create Profile", MB_ICONINFORMATION + MB_OK);
	}
	lbLocations->SetFocus();
}
//---------------------------------------------------------------------------
void TfrmStocktakeProfiles::LoadTree()
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();

	sqlLocation->Close();
	sqlLocation->ExecQuery();
	lbLocations->Items->Clear();
	while(!sqlLocation->Eof)
	{
		lbLocations->Items->Add(sqlLocation->FieldByName("Location")->AsString);
		sqlLocation->Next();
	}
	if (lbLocations->Count == 1)
	{
		lbLocations->ItemIndex = 0;
	}

	tvProfiles->Items->BeginUpdate();
	tvProfiles->Items->Clear();

	qrStockProfiles->Close();

	TTreeNode *ProfileNode = NULL;
	TTreeNode *CategoryNode = NULL;
	TTreeNode *GroupNode = NULL;
	TTreeNode *Selected = NULL;

	AnsiString LastProfile, LastCategory;

	for (qrStockProfiles->ExecQuery(); !qrStockProfiles->Eof; qrStockProfiles->Next())
	{
		if (!ProfileNode || (ProfileNode && LastProfile != qrStockProfiles->FieldByName("Profile_Name")->AsString))
		{
			ProfileNode							= tvProfiles->Items->Add(ProfileNode, qrStockProfiles->FieldByName("Profile_Name")->AsString);
			ProfileNode->StateIndex       = STATE_IMAGE_UNCHECKED;
			ProfileNode->Data					= (void *)qrStockProfiles->FieldByName("Stocktake_Profile_Key")->AsInteger;
			ProfileNode->ImageIndex			= ICON_FAVORITES_FOLDER_INDEX;
			ProfileNode->SelectedIndex		= ICON_FAVORITES_FOLDER_INDEX;
			if (StocktakeProfileKey == int(ProfileNode->Data))
			{
				Selected							= ProfileNode;
			}
			ProfileNode->Expand(false);
			LastProfile							= qrStockProfiles->FieldByName("Profile_Name")->AsString;
			CategoryNode						= NULL;
		}
		if (!CategoryNode || (CategoryNode && LastCategory != qrStockProfiles->FieldByName("Stock_Category")->AsString))
		{
			CategoryNode						= tvProfiles->Items->AddChild(ProfileNode, qrStockProfiles->FieldByName("Stock_Category")->AsString);
			CategoryNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
			CategoryNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
			CategoryNode->StateIndex		= -1;

			LastCategory						= qrStockProfiles->FieldByName("Stock_Category")->AsString;
		}
		GroupNode								= tvProfiles->Items->AddChild(CategoryNode, qrStockProfiles->FieldByName("Stock_Group")->AsString);
		GroupNode->ImageIndex				= ICON_CLOSED_FOLDER_INDEX;
		GroupNode->SelectedIndex			= ICON_OPEN_FOLDER_INDEX;
		GroupNode->StateIndex				= -1;
		GroupNode->Data						= (void *)qrStockProfiles->FieldByName("Stock_Group_Key")->AsInteger;

		CategoryNode->Expand(false);
	}
	qrStockProfiles->Close();
	Transaction->Commit();
	if (Selected)
	{
		tvProfiles->Selected = Selected;
	}
	tvProfiles->Items->EndUpdate();
	lbLocationsClick(tvProfiles);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktakeProfiles::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Rollback();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktakeProfiles::btnCreateProfileClick(TObject *Sender)
{
	std::auto_ptr<TfrmCreateProfile> frmCreateProfile(new TfrmCreateProfile(NULL));
	frmCreateProfile->ProfileMode = pmAddProfile;
	if (frmCreateProfile->ShowModal()== mrOk)
	{
		StocktakeProfileKey = frmCreateProfile->StocktakeProfileKey;
		LoadTree();
		tvProfiles->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktakeProfiles::btnEditProfileClick(TObject *Sender)
{
	if (tvProfiles->Selected && tvProfiles->Selected->Level == 0)
	{
		std::auto_ptr<TfrmCreateProfile> frmCreateProfile(new TfrmCreateProfile(NULL));

		frmCreateProfile->ProfileMode = pmEditProfile;
		frmCreateProfile->StocktakeProfileKey = int(tvProfiles->Selected->Data);
		if (frmCreateProfile->ShowModal() == mrOk)
		{
			StocktakeProfileKey = frmCreateProfile->StocktakeProfileKey;
			LoadTree();
			tvProfiles->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktakeProfiles::tvProfilesMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	THitTests HitTests = tvProfiles->GetHitTestInfoAt(X, Y);
	bool CheckBoxClicked = HitTests.Contains(htOnStateIcon);

	if (CheckBoxClicked)
	{
		TTreeNode *Node = tvProfiles->GetNodeAt(X, Y);
		if (Node)
		{
			ToggleCheckbox(Node);
		}
	}
}
//---------------------------------------------------------------------------
void TfrmStocktakeProfiles::ToggleCheckbox(TTreeNode *Node)
{
	if (Node->ImageIndex != ICON_EXCLAMATION_RED_INDEX)
	{
		if (Node->StateIndex == STATE_IMAGE_UNCHECKED) Node->StateIndex = STATE_IMAGE_CHECKED;
		else if (Node->StateIndex == STATE_IMAGE_CHECKED) Node->StateIndex = STATE_IMAGE_UNCHECKED;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktakeProfiles::btnOkClick(TObject *Sender)
{
	bool Checked = false;
	for (TTreeNode *CurItem = tvProfiles->Items->GetFirstNode(); CurItem!=NULL; CurItem=CurItem->getNextSibling())
	{
		if (CurItem->StateIndex == STATE_IMAGE_CHECKED)
		{
			Checked = true;
		}
	}
	if (lbLocations->ItemIndex > -1 && Checked)
	{
		ModalResult = mrOk;
	}
	else
	{
		Application->MessageBox("You must select a Location and/or Stock Group.", "Select Location/Stock Group", MB_ICONERROR + MB_OK);
	}
/*	if (!Transaction->InTransaction) Transaction->StartTransaction();
	qrClearVisibleGroups->Close();
	qrClearVisibleGroups->ExecQuery();
	int VisibleCount = 0;
	TTreeNode *CurItem = tvProfiles->Items->GetFirstNode();
	while (CurItem)
	{
		if (CurItem->StateIndex == STATE_IMAGE_CHECKED)
		{
			qrSetVisibleGroups->Close();
			qrSetVisibleGroups->ParamByName("Stocktake_Profile_Key")->AsInteger = int(CurItem->Data);
			qrSetVisibleGroups->ExecQuery();
			VisibleCount++;
		}
		CurItem = CurItem->getNextSibling();
	}
	if (lbLocations->ItemIndex > -1)
	{
		Location = lbLocations->Items->Strings[lbLocations->ItemIndex];
	}
	CreateStocktakeExportFile(Location);
	Transaction->Commit();
	if (VisibleCount > 0)
	{
		ModalResult = mrOk;
	}
	else
	{
		Application->MessageBox("You must select at least 1 profile for a stocktake.", "Select Profile", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------
bool TfrmStockGroupVisible::CreateStocktakeExportFile(AnsiString Location)
{
/*	try
	{
		if (CurrentConnection.StocktakePath != "" && CurrentConnection.StocktakeBarcodePos != -1 && CurrentConnection.StocktakeQtyPos != -1)
		{
			if (!DirectoryExists(CurrentConnection.StocktakePath))
			{
				if (!ForceDirectories(CurrentConnection.StocktakePath))
				{
					Application->MessageBox(AnsiString("Cannot create directory " + CurrentConnection.StocktakePath).c_str(), "Error", MB_ICONERROR + MB_OK);
					return false;
				}
			}
			TCsv ExportCSV;
			ExportCSV.Delimiter = ',';
			// Export of Barcodes now has 2 passes - Stock to Barcode and SupplierStock to Barcode
			// introduced since the introduction of Multiple Barcodes by Stock and Supplier
			qrStockExport->Close();
//			qrStocktakeExport->ParamByName("Location")->AsString = Location;
			for (qrStockExport->ExecQuery(); !qrStockExport->Eof; qrStockExport->Next())
			{
				int Row = ExportCSV.Add(",");
				ExportCSV.Cells[0][Row] = qrStockExport->FieldByName("Barcode")->AsString;
				ExportCSV.Cells[1][Row] = qrStockExport->FieldByName("Description")->AsString;
			}
			qrStockExport->Close();
			qrSupplierStockExport->Close();
			for (qrSupplierStockExport->ExecQuery(); !qrSupplierStockExport->Eof; qrSupplierStockExport->Next())
			{
				int Row = ExportCSV.Add(",");
				ExportCSV.Cells[0][Row] = qrSupplierStockExport->FieldByName("Barcode")->AsString;
				ExportCSV.Cells[1][Row] = qrSupplierStockExport->FieldByName("Description")->AsString + " (" + qrSupplierStockExport->FieldByName("Supplier_Unit")->AsString + ")";
			}
			qrSupplierStockExport->Close();

			ExportCSV.SaveToFile(CurrentConnection.StocktakePath + "\\" + CurrentConnection.StocktakeExportFile);
		}
		return true;
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		return false;
	}*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktakeProfiles::tvProfilesKeyPress(TObject *Sender,
		char &Key)
{
	TTreeNode *Node = tvProfiles->Selected;
	if (Node)
	{
		ToggleCheckbox(Node);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktakeProfiles::lbLocationsClick(TObject *Sender)
{
	bool Initialised = false;
	if (lbLocations->ItemIndex > -1)
	{
		TTreeNode *ProfileNode = tvProfiles->Items->GetFirstNode();
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		try
		{
			while (ProfileNode)
			{
				sqlInitialised->Close();
				sqlInitialised->ParamByName("Location")->AsString = lbLocations->Items->Strings[lbLocations->ItemIndex];
				sqlInitialised->ParamByName("StocktakeProfileKey")->AsInteger = int(ProfileNode->Data);
				sqlInitialised->ExecQuery();
				if (!sqlInitialised->Eof )
				{
					ProfileNode->ImageIndex			= ICON_EXCLAMATION_RED_INDEX;
					ProfileNode->SelectedIndex		= ICON_EXCLAMATION_RED_INDEX;
					ProfileNode->StateIndex			= STATE_IMAGE_UNCHECKED;
					Initialised							= true;
				}
				else
				{
					ProfileNode->ImageIndex			= ICON_FAVORITES_FOLDER_INDEX;
					ProfileNode->SelectedIndex		= ICON_FAVORITES_FOLDER_INDEX;
				}
				ProfileNode = ProfileNode->getNextSibling();
			}
		}
		__finally
		{
			Transaction->Commit();
		}
	}
	Image1->Visible = Initialised;
	Label6->Visible = Initialised;
}
//---------------------------------------------------------------------------

