//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectStocktake.h"
#include "StocktakeProfiles.h"
#include "Connections.h"
#include "Utilities.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
struct TTreeNodeData
{
	TTreeNodeData() : Index(-1) {}

	int				Key;
	AnsiString		Description;
	AnsiString		Location;
	AnsiString		Initialised;
	AnsiString		By;

	int				ImageIndex;
	int				SelectedIndex;

	int				Index;
};
//---------------------------------------------------------------------------
__fastcall TfrmSelectStocktake::TfrmSelectStocktake(Stock::TStocktakeControl &StocktakeControl) :
	TForm(static_cast<TComponent*>(NULL)),
	fStocktakeControl(StocktakeControl),
	fStocktake(NULL),
	fCurrentStocktake(0)
{
}
//---------------------------------------------------------------------------
bool TfrmSelectStocktake::Execute(Stock::TStocktakeDetails &Stocktake)
{
	fStocktake = &Stocktake;
	fLoadList();

	return ShowModal() == mrOk;
}
//---------------------------------------------------------------------------
/*void TfrmSelectStocktake::fLoadList()
{
	fStocktakeControl.EnumStocktakes(fStocktakeList, true, true, false);

	lvStocktakes->Items->Clear();
	for (Stock::TStocktakeList::size_type i=0; i<fStocktakeList.size(); ++i)
	{
		TListItem *ListItem	= lvStocktakes->Items->Add();
		ListItem->Caption		= fStocktakeList[i].Name;

		ListItem->SubItems->Add(fStocktakeList[i].Location);

		if (fStocktakeList[i].Initialised)
		{
			ListItem->SubItems->Add(fStocktakeList[i].InitialisedTime.FormatString("dd/mm/yyyy HH:mm:ss"));
			ListItem->SubItems->Add(fStocktakeList[i].InitialisedBy);
		}
		else
		{
			ListItem->SubItems->Add("No");
			ListItem->SubItems->Add("");
		}
	}
}*/
//---------------------------------------------------------------------------
void TfrmSelectStocktake::fLoadList()
{
	fStocktakeControl.EnumStocktakes(fStocktakeList, true, true, false);

	vtvStocktake->BeginUpdate();
	vtvStocktake->Clear();
	vtvStocktake->NodeDataSize				= sizeof(TTreeNodeData);

	PVirtualNode StocktakeNode				= NULL;
	PVirtualNode StockCategoryNode		= NULL;
	PVirtualNode StockGroupNode			= NULL;
	PVirtualNode StockNode					= NULL;
	TTreeNodeData *NodeData					= NULL;

	PVirtualNode SelectedStocktake		= NULL;

//	int LastStocktake							= 0;

     //std::sort (fStocktakeList.begin(), fStocktakeList.begin() + fStocktakeList.size());

	if (!Transaction->InTransaction) Transaction->StartTransaction();
	for (Stock::TStocktakeList::size_type i=0; i<fStocktakeList.size(); ++i)
	{
//		if (!StocktakeNode || (StocktakeNode && LastStocktake != fStocktakeList[i]->StocktakeKey))
		{
			StocktakeNode								   = vtvStocktake->AddChild(NULL);
			NodeData										   = (TTreeNodeData *)vtvStocktake->GetNodeData(StocktakeNode);
			NodeData->ImageIndex						   = ICON_NOTEPAD_INDEX;
			NodeData->SelectedIndex					   = ICON_NOTEPAD_INDEX;
			NodeData->Key								   = fStocktakeList[i].StocktakeKey;
			NodeData->Description					   = fStocktakeList[i].Name;
			NodeData->Location				         = fStocktakeList[i].Location;
			NodeData->Index								= i;
			if (fStocktakeList[i].Initialised)
			{
				NodeData->Initialised					= fStocktakeList[i].InitialisedTime.FormatString("dd/mm/yyyy HH:mm:ss");
				NodeData->By								= fStocktakeList[i].InitialisedBy;
			}
			else
			{
				NodeData->Initialised					= "No";
			}
//			LastStocktake									= fStocktakeList[i].StocktakeKey;

			if (NodeData->Key == fCurrentStocktake) SelectedStocktake = StocktakeNode;

			AnsiString LastCategory, LastGroup;

			qrStoctakeItems->Close();
			qrStoctakeItems->ParamByName("Stocktake_Key")->AsInteger = fStocktakeList[i].StocktakeKey;
			for (qrStoctakeItems->ExecQuery(); !qrStoctakeItems->Eof; qrStoctakeItems->Next())
			{
				if (!StockCategoryNode || (StockCategoryNode && LastCategory != qrStoctakeItems->FieldByName("Stock_Category")->AsString))
				{
					StockCategoryNode					   = vtvStocktake->AddChild(StocktakeNode);
					NodeData								   = (TTreeNodeData *)vtvStocktake->GetNodeData(StockCategoryNode);
					NodeData->ImageIndex				   = ICON_CLOSED_FOLDER_INDEX;
					NodeData->SelectedIndex			   = ICON_OPEN_FOLDER_INDEX;
					NodeData->Key						   = 0;
					NodeData->Description			   = qrStoctakeItems->FieldByName("Stock_Category")->AsString;
					LastCategory							= NodeData->Description;
				}
				if (!StockGroupNode || (StockGroupNode && LastGroup != qrStoctakeItems->FieldByName("Stock_Group")->AsString))
				{
					StockGroupNode						   = vtvStocktake->AddChild(StockCategoryNode);
					NodeData								   = (TTreeNodeData *)vtvStocktake->GetNodeData(StockGroupNode);
					NodeData->ImageIndex				   = ICON_CLOSED_FOLDER_INDEX;
					NodeData->SelectedIndex			   = ICON_OPEN_FOLDER_INDEX;
					NodeData->Key						   = 0;
					NodeData->Description			   = qrStoctakeItems->FieldByName("Stock_Group")->AsString;
					LastGroup								= NodeData->Description;
				}
				StockNode						         = vtvStocktake->AddChild(StockGroupNode);
				NodeData							         = (TTreeNodeData *)vtvStocktake->GetNodeData(StockNode);
				NodeData->ImageIndex			         = ICON_BOX_INDEX;
				NodeData->SelectedIndex		         = ICON_BOX_INDEX;
				NodeData->Key					         = 0;
				NodeData->Description		         = qrStoctakeItems->FieldByName("Description")->AsString;
			}
		}
	}
	Transaction->Commit();
	vtvStocktake->EndUpdate();
	if (SelectedStocktake)
	{
		vtvStocktake->FocusedNode = SelectedStocktake;
		vtvStocktake->Selected[vtvStocktake->FocusedNode] = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStocktake::btnNewStocktakeClick(TObject *Sender)
{
	Stock::TStocktakeProfiles StocktakeProfiles;
	AnsiString Location;

	std::auto_ptr<TfrmStocktakeProfiles> frmStocktakeProfiles(new TfrmStocktakeProfiles(NULL));
	if (frmStocktakeProfiles->Execute(StocktakeProfiles, Location))
	{
		int Count = 0;
		AnsiString Name = "";
		for (unsigned int i=0; i<StocktakeProfiles.size(); i++)
		{
			Count += StocktakeProfiles[i].StockGroups.size();

			Name = Name + StocktakeProfiles[i].Name;
			if (i != StocktakeProfiles.size()-1)
			{
				Name = Name + "/";
			}
		}
		if (InputQuery("Stocktake Name?", "Please enter the identifier for this stocktake.", Name))
		{
			if (Name != "")
			{
				fCurrentStocktake = fStocktakeControl.CreateStocktake(StocktakeProfiles, Name, Location);
				fLoadList();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStocktake::btnDeleteStocktakeClick(
		TObject *Sender)
{
	if (vtvStocktake->FocusedNode && vtvStocktake->Selected[vtvStocktake->FocusedNode])
	{
		TTreeNodeData *NodeData = (TTreeNodeData *)vtvStocktake->GetNodeData(vtvStocktake->FocusedNode);
		if (fStocktake && NodeData && NodeData->Index > -1)
		{
			if (fStocktakeList[NodeData->Index].Initialised == false)
			{
				if (Application->MessageBox("Delete this un-initialised stocktake?", "Delete?", MB_OKCANCEL | MB_ICONQUESTION) == ID_OK)
				{
					fStocktakeControl.DeleteStocktake(fStocktakeList[NodeData->Index].StocktakeKey);
					fLoadList();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TfrmSelectStocktake::CreateStocktakeExportFile(int StocktakeKey)
{
	if (CurrentConnection.StocktakePath != "" && CurrentConnection.StocktakeBarcodePos != -1 && CurrentConnection.StocktakeQtyPos != -1)
	{
		if (!DirectoryExists(CurrentConnection.StocktakePath))
		{
			if (!ForceDirectories(CurrentConnection.StocktakePath))
			{
				Application->MessageBox(AnsiString("Cannot create directory " + CurrentConnection.StocktakePath).c_str(), "Error", MB_ICONERROR + MB_OK);
				return;
			}
		}
		fStocktakeControl.CreateStockSCV(StocktakeKey, CurrentConnection.StocktakePath + "\\" + CurrentConnection.StocktakeExportFile);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStocktake::vtvStocktakeAfterPaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStocktake::vtvStocktakeBeforePaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStocktake::vtvStocktakeGetText(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      TVSTTextType TextType, WideString &CellText)
{
	TTreeNodeData *NodeData = (TTreeNodeData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Sender->GetNodeLevel(Node))
		{
			case 0:
			{
				switch (Column)
				{
					case 0:	CellText = NodeData->Description;	break;
					case 1:	CellText = NodeData->Location;		break;
					case 2:	CellText = NodeData->Initialised;	break;
					case 3:	CellText = NodeData->By;				break;
				}
				break;
			}
			case 1:
			case 2:
			case 3:
			{
				switch (Column)
				{
					case 0:	CellText = NodeData->Description;	break;
					default:	CellText = "";								break;
				}
			}
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStocktake::vtvStocktakeGetImageIndex(
      TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind,
      TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
	TTreeNodeData *NodeData = (TTreeNodeData *)Sender->GetNodeData(Node);
	if (Column == ((TVirtualStringTree *)Sender)->Header->MainColumn)
	{
		if (NodeData)
		{
			switch (Kind)
			{
				case ikState:
				{
					ImageIndex = NodeData->ImageIndex;
					break;
				}
				case ikNormal:
				{
					ImageIndex = NodeData->ImageIndex;
					break;
				}
				case ikSelected:
				{
					ImageIndex = NodeData->SelectedIndex;
					break;
				}
			}
		}
	}
	else
	{
		ImageIndex = -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStocktake::vtvStocktakeChange(
      TBaseVirtualTree *Sender, PVirtualNode Node)
{
	if (Node)
	{
		Enabled = (vtvStocktake->GetNodeLevel(Node) == 0);
	}
	else
	{
		Enabled = true;
		btnOk->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStocktake::vtvStocktakeFocusChanging(
		TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
		TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if ((Allowed = (vtvStocktake->GetNodeLevel(NewNode) == 0)) == true)
	{
		TTreeNodeData *NodeData = (TTreeNodeData *)Sender->GetNodeData(NewNode);
		if (fStocktake && NodeData && NodeData->Index > -1)
		{
			btnDeleteStocktake->Enabled = (fStocktakeList[NodeData->Index].Initialised == false);
		}
	}
	btnOk->Enabled = Allowed;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectStocktake::btnOkClick(TObject *Sender)
{
	if (vtvStocktake->FocusedNode && vtvStocktake->Selected[vtvStocktake->FocusedNode])
	{
		TTreeNodeData *NodeData = (TTreeNodeData *)vtvStocktake->GetNodeData(vtvStocktake->FocusedNode);
		if (fStocktake && NodeData && NodeData->Index > -1)
		{
			*fStocktake = fStocktakeList[NodeData->Index];
			CreateStocktakeExportFile(fStocktake->StocktakeKey);
		   	ModalResult = mrOk;
		}
	}
}
//---------------------------------------------------------------------------

