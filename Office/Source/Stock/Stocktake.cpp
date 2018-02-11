//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Connections.h"
#include "Stocktake.h"
#include "StockData.h"
#include "Login.h"
#include "MMData.h"
#include "Connections.h"
#include "Utilities.h"

#include "Reports.h"
#include "StockReportData.h"
#include "Csv.h"
#include "MM_Math.h"
#include "SelectStocktake.h"
#include "StocktakeVarianceParameters.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
class TStockData
{
public:
	TStockData() : ImageIndex(-1), SelectedIndex(-1) {}

//	WideString		Text;
	int				ImageIndex;
	int				SelectedIndex;

	int				Key;
	AnsiString		Location;
	AnsiString		Description;
	//AnsiString		Barcode;
	bool	      	Initialised;
//	AnsiString		StrInitialised;
	AnsiString		Unit;
	Currency			OnHand;
	Currency			Stocktake;
	Currency			Variance;

};


// storage of key and qty that were loaded from the file - multiple barcodes in the file
typedef std::map<int,double> TStockMap;
//---------------------------------------------------------------------------
void TStocktakeGUI::Execute()
{
	Stock::TStocktakeControl StocktakeControl(dmStockData->dbStock);
	Stock::TStocktakeDetails Stocktake;

	std::auto_ptr<TfrmSelectStocktake> frmSelectStocktake(new TfrmSelectStocktake(StocktakeControl));
	if (frmSelectStocktake->Execute(Stocktake))
	{
		std::auto_ptr<TfrmStocktake> frmStocktake(new TfrmStocktake(StocktakeControl, Stocktake));
		frmStocktake->ShowModal();
	}
}
//---------------------------------------------------------------------------
__fastcall TfrmStocktake::TfrmStocktake(Stock::TStocktakeControl &StocktakeControl, Stock::TStocktakeDetails &Stocktake)
	: TForm(static_cast<TComponent*>(NULL)), fStocktakeControl(StocktakeControl), fStocktake(Stocktake)
{
	Panel2->DoubleBuffered = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::FormShow(TObject *Sender)
{
    neStockQty->DecimalPlaces=CurrentConnection.SettingDecimalPlaces;
	lbeLocation->Caption = "Loading...";
	PostMessage(Handle, WM_AFTERSHOW, 0, 0);
	btnImportCount->Enabled = (CurrentConnection.StocktakePath != "" && CurrentConnection.StocktakeBarcodePos != -1 && CurrentConnection.StocktakeQtyPos != -1);
	btnCancelStocktake->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::AfterShow(TMessage& Message)
{
	vtvStocktake->Clear();
	Update();

	CurrentStockCategory = "";
	CurrentStockGroup = "";
	CurrentStockKey = 0;

	if (!Transaction->InTransaction) Transaction->StartTransaction();
	// Recalculate the variance incase a stock levels have changed while in
	// the middle of a stocktake. The stock update should fail if stock items are
	// initialised. Should also warn if variance has changed below.
	qrCalcVariance->Close();
	qrCalcVariance->ParamByName("Stocktake_Key")->AsString = fStocktake.StocktakeKey;
	qrCalcVariance->ExecSQL();
	qrCalcVariance->Close();

	Transaction->Commit();
	Transaction->StartTransaction();

	LoadTree();
}
//---------------------------------------------------------------------------
void TfrmStocktake::LoadTree()
{
    
	if (fStocktake.Committed)
	{
		lbeLocation->Caption = "Stocktake Location: " + fStocktake.Location + " - Committed";
	}
	else if (fStocktake.Initialised)
	{
		lbeLocation->Caption = "Stocktake Location: " + fStocktake.Location + " - Initialised";
	}
	else
	{
		lbeLocation->Caption = "Stocktake Location: " + fStocktake.Location;
	}
	vtvStocktake->BeginUpdate();
	vtvStocktake->Clear();
	vtvStocktake->NodeDataSize				= sizeof(TStockData);

	PVirtualNode StockCategoryNode		= NULL;
	PVirtualNode StockGroupNode			= NULL;
	PVirtualNode StockNode					= NULL;
	TStockData *NodeData						= NULL;

	PVirtualNode SelectedCategoryNode	= NULL;
	PVirtualNode SelectedGroupNode		= NULL;
	PVirtualNode SelectedStockNode		= NULL;

	AnsiString LastCategory, LastGroup;

	qrStock->Close();
	qrStock->ParamByName("Stocktake_Key")->AsInteger = fStocktake.StocktakeKey;
	for (qrStock->ExecQuery(); !qrStock->Eof; qrStock->Next())
	{
		if (!StockCategoryNode || (StockCategoryNode && LastCategory != qrStock->FieldByName("Stock_Category")->AsString))
		{
			StockCategoryNode							   = vtvStocktake->AddChild(NULL);
			NodeData										   = (TStockData *)vtvStocktake->GetNodeData(StockCategoryNode);
			NodeData->ImageIndex						   = ICON_CLOSED_FOLDER_INDEX;
			NodeData->SelectedIndex					   = ICON_OPEN_FOLDER_INDEX;

			NodeData->Key								   = 0;
			NodeData->Description					   = qrStock->FieldByName("Stock_Category")->AsString;
			LastCategory									= NodeData->Description;

			if (NodeData->Description == CurrentStockCategory)
			{
				SelectedCategoryNode						= StockCategoryNode;
			}
		}
		if (!StockGroupNode || (StockGroupNode && LastGroup != qrStock->FieldByName("Stock_Group")->AsString))
		{
			StockGroupNode								   = vtvStocktake->AddChild(StockCategoryNode);
			NodeData										   = (TStockData *)vtvStocktake->GetNodeData(StockGroupNode);
			NodeData->ImageIndex						   = ICON_CLOSED_FOLDER_INDEX;
			NodeData->SelectedIndex					   = ICON_OPEN_FOLDER_INDEX;

			NodeData->Key								   = 0;
			NodeData->Description					   = qrStock->FieldByName("Stock_Group")->AsString;
			LastGroup										= NodeData->Description;

			if (NodeData->Description == CurrentStockGroup)
			{
				SelectedGroupNode							= StockGroupNode;
			}
		}
		StockNode								         = vtvStocktake->AddChild(StockGroupNode);
		NodeData									         = (TStockData *)vtvStocktake->GetNodeData(StockNode);
		NodeData->ImageIndex					         = ICON_BOX_INDEX;
		NodeData->SelectedIndex				         = ICON_BOX_INDEX;

		NodeData->Key							         = qrStock->FieldByName("Stock_Key")->AsInteger;
		NodeData->Location					         = qrStock->FieldByName("Location")->AsString;
		NodeData->Description				         = qrStock->FieldByName("Description")->AsString;
		//NodeData->Barcode								   = qrStock->FieldByName("Barcode")->AsString;
		NodeData->Initialised				         = qrStock->FieldByName("Initialised")->AsString == "T";
		NodeData->Unit							         = qrStock->FieldByName("Stocktake_Unit")->AsString;
        NodeData->OnHand					         =  qrStock->FieldByName("On_Hand")->AsDouble;
        NodeData->Stocktake				         =   qrStock->FieldByName("Stocktake")->AsDouble;
		NodeData->Variance					         = qrStock->FieldByName("Variance")->AsDouble;
        
		if (NodeData->Key == CurrentStockKey)
		{
			SelectedStockNode								= StockNode;
		}

		vtvStocktake->Expanded[StockCategoryNode]	= true;
	}
	vtvStocktake->EndUpdate();
//	vtvStocktake->OnFocusChanging = NULL;
//	try
//	{
		if (SelectedStockNode)
		{
			vtvStocktake->FocusedNode = SelectedStockNode;
			vtvStocktake->Selected[vtvStocktake->FocusedNode] = true;
		}
		else if (SelectedGroupNode)
		{
			vtvStocktake->FocusedNode = SelectedGroupNode;
			vtvStocktake->Selected[vtvStocktake->FocusedNode] = true;
		}
		else if (SelectedCategoryNode)
		{
			vtvStocktake->FocusedNode = SelectedCategoryNode;
			vtvStocktake->Selected[vtvStocktake->FocusedNode] = true;
		}
		else if (vtvStocktake->GetFirst())
		{
			vtvStocktake->FocusedNode = vtvStocktake->GetFirst();
			vtvStocktake->Selected[vtvStocktake->FocusedNode] = true;
		}
//	}
//	__finally
//	{
//		vtvStocktake->OnFocusChanging = vtvStocktakeFocusChanging;
//	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::btnInitialiseClick(TObject *Sender)
{
	GetCurrentNode();
	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->EndEditNode();
		if (vtvStocktake->FocusedNode)
		{
			vtvStocktake->Selected[vtvStocktake->FocusedNode] = false;
			vtvStocktake->FocusedNode = NULL;
		}
	}
	// can only Reset Count or Undo the Stocktake
	if (fStocktake.Initialised)
	{
		const std::auto_ptr<TfrmVarianceParams> frmVarianceParams(new TfrmVarianceParams(NULL));
		frmVarianceParams->VarianceParams = vResetUndo;
		if (frmVarianceParams->ShowModal() == mrOk)
		{
			if (frmVarianceParams->RadioButtonValue == 2)
			{
				// Undo
				btnCancelStocktakeClick(Sender);
			}
			else
			{
				// Reset to OnHand or Zero
				ResetInitialisation( frmVarianceParams->RadioButtonValue);
			}
		}
		return;
	}
	/* zero off Sales from the POS - Check first
		if they have counted the sales prior or havn't counted then they will still need to Close Stock	*/
	if (MenuMateTransaction->DefaultDatabase->Connected)
	{
		bool Return = false;
		try
		{
			if (!MenuMateTransaction->InTransaction) MenuMateTransaction->StartTransaction();
			sqlCheckStockTrack->Close();
			sqlCheckStockTrack->ExecQuery();
			if (sqlCheckStockTrack->FieldByName("cntStockTrack")->AsInteger > 0)
			{
				if (Application->MessageBox(AnsiString("There are " + sqlCheckStockTrack->FieldByName("cntStockTrack")->AsString +
												" MenuMate POS sales which will NOT be included in this stocktake.\r"
												"It would be advisable to do a Zed or Close Stock on one POS.\r"
												"Do you wish to continue?").c_str(), "Confirmation", MB_ICONQUESTION + MB_OKCANCEL) == ID_CANCEL)
				{
					Return = true;
				}
			}
		}
		__finally
		{
			sqlCheckStockTrack->Close();
			if (MenuMateTransaction->InTransaction) MenuMateTransaction->Commit();
		}
		if (Return) return;
	}
	const std::auto_ptr<TfrmVarianceParams> frmVarianceParams(new TfrmVarianceParams(NULL));
	frmVarianceParams->VarianceParams = vInitialisation;
	if (frmVarianceParams->ShowModal() == mrOk)
	{
	  // RadioButtonValue 0 = Onhand   ,  1 = zeros
		bool CopyOnHand = frmVarianceParams->RadioButtonValue == 0;
		// Initialising sets variance to 0 and sets Stocktake to On_Hand and Initialised to 'T'
		if (fStocktakeControl.Initialise(fStocktake.StocktakeKey, frmLogin->CurrentUserID, frmLogin->CurrentUserName, CopyOnHand))
		{
			fStocktake.Initialised = true;
		}
		LoadTree();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->EndEditNode();
	}
	if (Transaction->InTransaction) Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::btnCommitClick(TObject *Sender)
{
	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->EndEditNode();
	}
	if (!fStocktake.Initialised)
	{
		Application->MessageBox("You must Initialise the Stocktake before Committing.", "Error", MB_ICONERROR + MB_OK);
		return;
	}
	if (Application->MessageBox("This will update your On Hand values.\rDo you wish to continue?", "Question", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
	{
	  //	if (Application->MessageBox("This is your last chance to print a variance report.\rContinue?", "Question", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
	  //	{
			GetCurrentNode();
			Screen->Cursor = crHourGlass;
			try
			{
				std::vector<Stock::TStocktakeItemInfo> StocktakeItems;

				PVirtualNode Node = vtvStocktake->GetFirst();
				while (Node)
				{
					if (vtvStocktake->GetNodeLevel(Node) == 2)
					{
						TStockData *NodeData	= (TStockData *)vtvStocktake->GetNodeData(Node);

						Stock::TStocktakeItemInfo StocktakeItemInfo;

						StocktakeItemInfo.StockKey		= NodeData->Key;
						StocktakeItemInfo.Location		= NodeData->Location;
						StocktakeItemInfo.Counted		= NodeData->Stocktake;
						StocktakeItemInfo.Variance		= NodeData->Variance;

						StocktakeItems.push_back(StocktakeItemInfo);

						// All done for this item. Get the next one.

//						TotalCost	+= NodeData->Variance * NodeData->SupplierUnitCost;
//						TotalGST		+= InvoiceItemInfo.Total_GST;
					}
					Node = vtvStocktake->GetNext(Node);
				}
				Stock::TTransactionBatchInfo BatchInfo(Stock::ttStocktake);

				BatchInfo.User_ID			= frmLogin->CurrentUserID;
				BatchInfo.User_Name		= frmLogin->CurrentUserName;
				if (fStocktakeControl.CommitStocktake(BatchInfo, fStocktake.StocktakeKey, StocktakeItems))
				{
					Screen->Cursor = crDefault;
					fStocktake.Committed = true;
					LoadTree();
					if (Transaction->InTransaction) Transaction->Commit();
					if (!Transaction->InTransaction) Transaction->StartTransaction();
					dmStockReportData->StockTrans->StartTransaction();
					const std::auto_ptr<TfrmVarianceParams> frmVarianceParams(new TfrmVarianceParams(NULL));
					frmVarianceParams->VarianceParams = vReports;
					frmVarianceParams->StocktakeKey   = fStocktake.StocktakeKey;
					frmVarianceParams->ShowModal();
					this->Close();
				}
				else
				{
					Application->MessageBox("There was a problem updating the stock.", "Error", MB_ICONERROR + MB_OK);
					fStocktake.Committed = false;
				}
			}
			__finally
			{
				Screen->Cursor = crDefault;
				if (dmStockReportData->StockTrans->InTransaction)
					dmStockReportData->StockTrans->Commit();
				if (Transaction->InTransaction) Transaction->Commit();
				if (!Transaction->InTransaction) Transaction->StartTransaction();

				LoadTree();
			}
		//}
	}
}
//---------------------------------------------------------------------------
void TfrmStocktake::GetCurrentNode()
{
	CurrentStockCategory = "";
	CurrentStockGroup = "";
	CurrentStockKey = 0;
	if (vtvStocktake->FocusedNode)
	{
		TStockData *NodeData = (TStockData *)vtvStocktake->GetNodeData(vtvStocktake->FocusedNode);
		if (vtvStocktake->GetNodeLevel(vtvStocktake->FocusedNode) == 0)
		{
			CurrentStockCategory = NodeData->Description;
		}
		else if (vtvStocktake->GetNodeLevel(vtvStocktake->FocusedNode) == 1)
		{
			CurrentStockGroup = NodeData->Description;
			NodeData = (TStockData *)vtvStocktake->GetNodeData(vtvStocktake->FocusedNode->Parent);
			CurrentStockCategory = NodeData->Description;
		}
		else if (vtvStocktake->GetNodeLevel(vtvStocktake->FocusedNode) == 2)
		{
			CurrentStockKey = NodeData->Key;
			NodeData = (TStockData *)vtvStocktake->GetNodeData(vtvStocktake->FocusedNode->Parent);
			CurrentStockGroup = NodeData->Description;
			NodeData = (TStockData *)vtvStocktake->GetNodeData(vtvStocktake->FocusedNode->Parent->Parent);
			CurrentStockCategory = NodeData->Description;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::btnCloseClick(TObject *Sender)
{
	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->EndEditNode();
	}
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::btnPrintStocktakeClick(TObject *Sender)
{
	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->EndEditNode();
	}
	dmStockReportData->StockTrans->StartTransaction();
	try
	{
		bool SeeOnHand;
		if (CurrentConnection.HideStocktakeOnHand)
		{
			SeeOnHand = (Application->MessageBox("Do you wish to see the 'On Hand' values?",
								"See On Hand?", MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON2) == ID_YES);
		}
		else
		{
			SeeOnHand = (Application->MessageBox("Do you wish to see the 'On Hand' values?",
								"See On Hand?", MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON1) == ID_YES);
		}

		bool SeeBarcodes = (Application->MessageBox("Do you wish to see barcodes?",
								"See barcodes?", MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON2) == ID_YES);

		dmStockReportData->SetupStocktake(fStocktake.StocktakeKey, !SeeOnHand, !SeeBarcodes);

		if (frmReports->rvStock->SelectReport("repStocktake", false))
		{
			frmReports->rvStock->Execute();
		}
	}
	__finally
	{
		dmStockReportData->StockTrans->Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::btnPrintVarianceClick(TObject *Sender)
{
  
	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->EndEditNode();
	}
	const std::auto_ptr<TfrmVarianceParams> frmVarianceParams(new TfrmVarianceParams(NULL));
	frmVarianceParams->VarianceParams = vVarianceReport;

	if (frmVarianceParams->ShowModal() == mrOk)
	{
		if (Transaction->InTransaction) Transaction->Commit();
		if (!Transaction->InTransaction) Transaction->StartTransaction();

		dmStockReportData->StockTrans->StartTransaction();
//		const std::auto_ptr<TStringList> StringList(new TStringList);
		try
		{
//			StringList->Add(Location);

			dmStockReportData->SetupStockVariance(fStocktake.StocktakeKey,frmVarianceParams->RadioButtonValue,
				frmVarianceParams->neVarianceAmount->Value);
			if (frmReports->rvStock->SelectReport("repStocktakeVariance", false))
			{
				frmReports->rvStock->SetParam("TotalBankingInc", frmVarianceParams->Results().TotalBankingInc);
				frmReports->rvStock->SetParam("TotalBankingExcl", frmVarianceParams->Results().TotalBankingExcl);
				frmReports->rvStock->SetParam("UserVariance", frmVarianceParams->Results().UserVariance);
				frmReports->rvStock->SetParam("ReportRange", "");
				frmReports->rvStock->Execute();
			}
		}
		__finally
		{
			dmStockReportData->StockTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::btnPrintValuationClick(TObject *Sender)
{
	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->EndEditNode();
	}
	if (Transaction->InTransaction) Transaction->Commit();
	if (!Transaction->InTransaction) Transaction->StartTransaction();

	dmStockReportData->StockTrans->StartTransaction();

	std::auto_ptr<TStringList> StringList(new TStringList);
	StringList->Add(IntToStr(fStocktake.StocktakeKey));
	dmStockReportData->SetupStocktakeStockValuation(StringList.get());
	if (frmReports->rvStock->SelectReport("repStockValueByLocation", false))
	{
		frmReports->rvStock->SetParam("Heading", "Stock Value by Stocktake");
		frmReports->rvStock->Execute();
	}
	dmStockReportData->StockTrans->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::vtvStocktakeColumnDblClick(
      TBaseVirtualTree *Sender, TColumnIndex Column, TShiftState Shift)
{
	vtvStocktake->EditNode(vtvStocktake->FocusedNode, vtvStocktake->FocusedColumn);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::vtvStocktakeCreateEditor(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      IVTEditLink *EditLink)
{
	TStockData *NodeData							= (TStockData *)Sender->GetNodeData(Node);

	neStockQty->Value								= NodeData->Stocktake;
	TPropertyEdit* PropertyLink				= new TPropertyEdit(Sender, Node, Column, neStockQty);
	PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::vtvStocktakeEdited(
		TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	TStockData *NodeData = (TStockData *)Sender->GetNodeData(Node);
	if (Sender->GetNodeLevel(Node) == 2 && NodeData->Stocktake != neStockQty->Value)
	{
		if (!NodeData->Initialised)
		{
			neStockQty->Value = NodeData->Stocktake;
			Application->MessageBox("You must Initialise first.", "Error", MB_ICONERROR + MB_OK);
		}
		else
		{
			NodeData->Stocktake	= neStockQty->Value;
			NodeData->Variance	= NodeData->Stocktake - NodeData->OnHand;

			fStocktakeControl.SetCount(NodeData->Key, NodeData->Location, NodeData->Stocktake, NodeData->OnHand, NodeData->Variance);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::vtvStocktakeEditing(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      bool &Allowed)
{
	Allowed = (Sender->GetNodeLevel(Node) == 2 && Column == 3);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::vtvStocktakeFocusChanged(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	if (Node && !Sender->IsEditing())
	{
		if (Sender->GetNodeLevel(Node) == 0 || Sender->GetNodeLevel(Node) == 1)
		{
			if (Column != 0)
			{
				Sender->FocusedColumn = 0;
				Sender->SetFocus();
			}
		}
		else if (Sender->GetNodeLevel(Node) == 2)
		{
			if (Column != 3)
			{
				Sender->FocusedColumn = 3;
			}
			else
			{
				Sender->EditNode(Node, Column);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::vtvStocktakeFocusChanging(
      TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
      TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::vtvStocktakeGetImageIndex(
      TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind,
      TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
	TStockData *NodeData = (TStockData *)Sender->GetNodeData(Node);
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
void __fastcall TfrmStocktake::vtvStocktakeGetText(
		TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
		TVSTTextType TextType, WideString &CellText)
{
	TStockData *NodeData = (TStockData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Sender->GetNodeLevel(Node))
		{
			case 0:
			case 1:
			{
				if (Column == 0)
				{
					CellText = NodeData->Description;
				}
				else
				{
					CellText = "";
				}
				break;
			}
			case 2:
			{
				switch (Column)
				{
					case 0:	CellText = NodeData->Description; break;
					case 1:	CellText = NodeData->Unit;		  break;
					case 2:
                        {
                        double onHand = NodeData->OnHand;
                        onHand = RoundTo(onHand,-CurrentConnection.SettingDecimalPlaces);
                        NodeData->OnHand = onHand;  // Node value also changed
                        CellText = onHand;//MMMath::FloatString(NodeData->OnHand);
                        break;
                        }
					case 3:
                       {
                       double countValue = NodeData->Stocktake;
                       countValue = RoundTo(countValue,-CurrentConnection.SettingDecimalPlaces);
                       NodeData->Stocktake = countValue;
                       CellText = countValue;//MMMath::FloatString(NodeData->Stocktake);
                       break;
                       }
					case 4:
					{
						if (NodeData->Variance != 0 && NodeData->Initialised)
						{
                            double initValue = NodeData->Variance;
                            initValue = RoundTo(initValue,-CurrentConnection.SettingDecimalPlaces);
                            NodeData->Variance = initValue;
                            CellText = initValue;//MMMath::FloatString(NodeData->Variance, CurrentConnection.SettingDecimalPlaces);
						}
						else
						{
							CellText = "";
						}
						break;
					}
				}
				break;
			}
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::vtvStocktakeKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if (!vtvStocktake->IsEditing() && vtvStocktake->FocusedNode)
	{
		if (Key == VK_RETURN)
		{
			vtvStocktake->EditNode(vtvStocktake->FocusedNode, vtvStocktake->FocusedColumn);
		}
		else if (Key == VK_RIGHT)
		{
			vtvStocktake->Expanded[vtvStocktake->FocusedNode] = true;
		}
		else if (Key == VK_LEFT)
		{
			vtvStocktake->Expanded[vtvStocktake->FocusedNode] = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::neStockQtyKeyDown(TObject *Sender,
		WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN || Key == VK_DOWN || Key == VK_UP)
	{
		vtvStocktake->BeginUpdate();
		try
		{
			vtvStocktake->EndEditNode();
		}
		__finally
		{
			vtvStocktake->EndUpdate();
		}
		if (Key == VK_RETURN || Key == VK_DOWN)
		{
			if (vtvStocktake->GetNext(vtvStocktake->FocusedNode))
			{
				vtvStocktake->FocusedNode = vtvStocktake->GetNext(vtvStocktake->FocusedNode);
				vtvStocktake->Selected[vtvStocktake->FocusedNode] = true;
			}
			else
			{
				vtvStocktake->EditNode(vtvStocktake->FocusedNode, vtvStocktake->FocusedColumn);
			}
			if (Key == VK_RETURN)
			{
				Key = NULL;
			}
		}
		else if (Key == VK_UP)
		{
			if (vtvStocktake->GetPrevious(vtvStocktake->FocusedNode))
			{
				vtvStocktake->FocusedNode = vtvStocktake->GetPrevious(vtvStocktake->FocusedNode);
				vtvStocktake->Selected[vtvStocktake->FocusedNode] = true;
			}
			else
			{
				vtvStocktake->EditNode(vtvStocktake->FocusedNode, vtvStocktake->FocusedColumn);
			}
		}
	}
	else if (Key == VK_ESCAPE)
	{
		vtvStocktake->CancelEditNode();
		vtvStocktake->EditNode(vtvStocktake->FocusedNode, vtvStocktake->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::neStockQtyKeyUp(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	if (Key == VK_DOWN || Key == VK_UP)
	{
		neStockQty->SelectAll();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::neStockQtyKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::vtvStocktakeAfterPaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::vtvStocktakeBeforePaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::btnFindClick(TObject *Sender)
{
	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->EndEditNode();
	}
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::FindDialogFind(TObject *Sender)
{
	PVirtualNode CurItem;
	if (vtvStocktake->FocusedNode)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = vtvStocktake->GetNext(vtvStocktake->FocusedNode);
		}
		else
		{
			CurItem = vtvStocktake->GetPrevious(vtvStocktake->FocusedNode);
		}
	}
	else
	{
		CurItem = vtvStocktake->GetFirst();
	}
	while (CurItem)
	{
		TStockData *NodeData = (TStockData *)vtvStocktake->GetNodeData(CurItem);
		AnsiString FindText, NodeText;
		if (FindDialog->Options.Contains(frMatchCase))
		{
			FindText = FindDialog->FindText;
			NodeText = NodeData->Description;
		}
		else
		{
			FindText = FindDialog->FindText.UpperCase();
			NodeText = AnsiString(NodeData->Description).UpperCase();
		}
		if (FindDialog->Options.Contains(frWholeWord))
		{
			if (FindText == NodeText)
			{
				vtvStocktake->FocusedNode = CurItem;
				vtvStocktake->Selected[CurItem] = true;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				vtvStocktake->FocusedNode = CurItem;
				vtvStocktake->Selected[CurItem] = true;
				break;
			}
		}
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = vtvStocktake->GetNext(CurItem);
		}
		else
		{
			CurItem = vtvStocktake->GetPrevious(CurItem);
		}
	}
	if (!CurItem)
	{
		Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::btnImportCountClick(TObject *Sender)
{
/* Multiple barcodes in the Stocktake file are taken care of at the point of Entry
	If they initialize to SOH or Zero, the Stocktake(Count) column
	will always show what has come from the Stocktake file. */
	AnsiString FileName = CurrentConnection.StocktakePath + "\\" + CurrentConnection.StocktakeImportFile;
	if (!FileExists(FileName))
	{
		Application->MessageBox(AnsiString("Can't find file " + FileName + ".").c_str(), "Error", MB_ICONERROR + MB_OK);
		return;
	}
	else
	{
		if (vtvStocktake->IsEditing())
		{
			vtvStocktake->EndEditNode();
		}
		try
		{
			Screen->Cursor = crHourGlass;
			int UpdatedCount = 0, NotUpdatedCount = 0 , StockKey = 0;
			double LookupQty = 0.00;
			bool LookupSuccess = false;
			TCsv ExportCSV;
			TStockMap::iterator iStockMap;
			TStockMap StockMap;
			ExportCSV.Delimiter = ',';
			ExportCSV.LoadFromFile(FileName);
			// loop thru the ExportCSV - stocktake file
			for (int i=0; i<ExportCSV.RowCount;)
			{
				StockKey = FindStockKey(ExportCSV.Cells[CurrentConnection.StocktakeBarcodePos][i], LookupQty, LookupSuccess);
				// if a Stock barcode matches one in the file
				if (StockKey != -1 && LookupSuccess )
				{
					UpdatedCount++;
					iStockMap = StockMap.find(StockKey);
					if ( iStockMap == StockMap.end() )
					{
						// add a new entry and initialize to StocktakeQtyPos * LookupQty
						StockMap[StockKey] = StrToFloat(ExportCSV.Cells[CurrentConnection.StocktakeQtyPos][i]) * LookupQty;
					}
					else
					{
						// matching Stock Key (+= qty)
						StockMap[StockKey] += StrToFloat(ExportCSV.Cells[CurrentConnection.StocktakeQtyPos][i]) * LookupQty;
					}
					// delete the row from the StockTake file
					ExportCSV.DeleteRow(i);
				}
				else
				{
					NotUpdatedCount++;
					i++;
				}
			}
			// load the Qty onto the Node - only affects Stock that are in the Stocktake File
			PVirtualNode CurItem = vtvStocktake->GetFirst();
			while (CurItem)
			{
				TStockData *NodeData = (TStockData *)vtvStocktake->GetNodeData(CurItem);
				if (NodeData->Key == 0)
				{
					// is a StockGroup or StockCategory
					CurItem = vtvStocktake->GetNext(CurItem);
				}
				else
				{
					if (!NodeData->Initialised)
					{
						Application->MessageBox("You must Initialise first.", "Error", MB_ICONERROR + MB_OK);
						return;
					}
					iStockMap = StockMap.find(NodeData->Key);
					if ( iStockMap != StockMap.end() )
					{
						// found the Stock , get the second map which is Qty
						NodeData->Stocktake = (*iStockMap).second;
						fStocktakeControl.SetCount(NodeData->Key, NodeData->Location, NodeData->Stocktake, NodeData->OnHand, NodeData->Variance);
					}
					CurItem = vtvStocktake->GetNext(CurItem);
				}
			}
			if (Transaction->InTransaction) Transaction->Commit();
			if (!Transaction->InTransaction) Transaction->StartTransaction();
			ExportCSV.SaveToFile(FileName);
			vtvStocktake->Invalidate();
			Screen->Cursor = crDefault;
			Application->MessageBox(AnsiString("Import Complete.\r\r" + IntToStr(UpdatedCount) + " Updated.\r" + IntToStr(NotUpdatedCount) + " Not updated.").c_str(), "Complete", MB_ICONINFORMATION + MB_OK);
		}
		__finally
		{
			Screen->Cursor = crDefault;
		}
//		vtvStocktake->EditNode(vtvStocktake->FocusedNode, vtvStocktake->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
int TfrmStocktake::FindStockKey(AnsiString Barcode,double &LookupQty, bool &LookupSuccess)
{
	const AnsiString BarcodeNotInStocktakeMsg  = "\r Barcode has not been added to this Stocktake";
	LookupSuccess = false;
	LookupQty     = 1;
	if (Barcode == "") return -1;
	sqlBarcodeLookup->Close();
	sqlBarcodeLookup->SQL->Text = "Select count(Barcode) as CountBarcodes from Barcode where Barcode = :Barcode";
	sqlBarcodeLookup->ParamByName("Barcode")->AsString = Barcode;
  	sqlBarcodeLookup->Prepare();
	sqlBarcodeLookup->ExecQuery();
	int ctr = sqlBarcodeLookup->FieldByName("CountBarcodes")->AsInteger;
	// Check integrity - just in case
	// check the number of Barcodes in Barcode table - should only be one
	if ( ctr == 0 )
	{
		return -1;
	}
	else if ( ctr > 1 )
	{
		Application->MessageBox(AnsiString("There is more than one Barcode " + Barcode = " one must be deleted " + BarcodeNotInStocktakeMsg).c_str(), "Error", MB_ICONERROR + MB_OK);
		return -1;
	}
	sqlBarcodeLookup->Close();
	sqlBarcodeLookup->SQL->Text = "Select * from Barcode where Barcode = :Barcode";
	sqlBarcodeLookup->ParamByName("Barcode")->AsString = Barcode;
	sqlBarcodeLookup->ExecQuery();
	int StockKey = -1,SupplierStockKey = -1, ReturnStockKey = -1;
	if (!sqlBarcodeLookup->Eof )
	{
		StockKey         = sqlBarcodeLookup->FieldByName("Stock_Key")->AsInteger;
		SupplierStockKey = sqlBarcodeLookup->FieldByName("Supplier_Stock_Key")->AsInteger;
	}
	else
	{
		// no barcode - should be trapped above
		return -1;
	}
	sqlBarcodeLookup->Close();
	// Note Stock_Key in Barcode is either a valid Stock_Key or -1 ( same as Supplier_Stock_Key)
	// The rules are that there can only be one barcode in barcode table either to Stock or SupplierStock
	if ( StockKey != -1 )
	{
		// Stock Lookup - Check Count
		sqlBarcodeLookup->SQL->Text = "Select Count(Stock_Key) as CountStockKey from Stock where Stock_Key = :StockKey";
		sqlBarcodeLookup->ParamByName("StockKey")->AsInteger = StockKey;
		sqlBarcodeLookup->ExecQuery();
		ctr = sqlBarcodeLookup->FieldByName("CountStockKey")->AsInteger;
		sqlBarcodeLookup->Close();
		switch (ctr)
		{
		case 1   : LookupSuccess  = true;
					  ReturnStockKey = StockKey;
					  break;
		case 0   : Application->MessageBox(AnsiString("There is no Stock Item attached to Barcode " + Barcode).c_str(), "Error", MB_ICONERROR + MB_OK);
					  return -1;
		default  : Application->MessageBox(AnsiString("There is more than one Stock Item attached to Barcode " + Barcode + BarcodeNotInStocktakeMsg).c_str(), "Error", MB_ICONERROR + MB_OK);
					  return -1;
		}
	}
	else if ( SupplierStockKey != -1 )
	{
		// Supplier Stock Lookup
		sqlBarcodeLookup->Close();
		sqlBarcodeLookup->SQL->Text = "Select Count(Supplier_Stock_Key) as CountSupplierStock from SupplierStock where Supplier_Stock_Key = :SupplierStockKey";
		sqlBarcodeLookup->ParamByName("SupplierStockKey")->AsInteger = SupplierStockKey;
		sqlBarcodeLookup->ExecQuery();
		ctr = sqlBarcodeLookup->FieldByName("CountSupplierStock")->AsInteger;
		switch (ctr)
		{
		case 1 :
			sqlBarcodeLookup->Close();
			sqlBarcodeLookup->SQL->Text = "Select Stock_Key,Qty from SupplierStock where Supplier_Stock_Key = :SupplierStockKey";
			sqlBarcodeLookup->ParamByName("SupplierStockKey")->AsInteger = SupplierStockKey;
			sqlBarcodeLookup->ExecQuery();
			ReturnStockKey = sqlBarcodeLookup->FieldByName("Stock_Key")->AsInteger;
			// carton Qty's from SupplierStock
			LookupQty = sqlBarcodeLookup->FieldByName("Qty")->AsInteger;
			sqlBarcodeLookup->Close();
			LookupSuccess = true;

			break;
		case 0  : Application->MessageBox(AnsiString("There is no Supplier Stock Item attached to Barcode " + Barcode).c_str(), "Error", MB_ICONERROR + MB_OK);
					 return -1;
		default : Application->MessageBox(AnsiString("There is more than one Supplier Stock Item attached to Barcode " + Barcode + BarcodeNotInStocktakeMsg).c_str(), "Error", MB_ICONERROR + MB_OK);
					 return -1;
		}
	}
	return ReturnStockKey;
	/*PVirtualNode CurItem = vtvStocktake->GetFirst();
	while (CurItem)
	{
		TStockData *NodeData = (TStockData *)vtvStocktake->GetNodeData(CurItem);
		if (NodeData->Barcode == Barcode)
		{
			return NodeData->Key;
		}
		CurItem = vtvStocktake->GetNext(CurItem);
	}
	return -1; */

}
//---------------------------------------------------------------------------
void __fastcall TfrmStocktake::btnCancelStocktakeClick(TObject *Sender)
{
	if (vtvStocktake->IsEditing())
	{
		vtvStocktake->EndEditNode();
	}
	if (Application->MessageBox("This will invalidate your stocktake count and undo the initialise process.\r"
										 "Re-counting stock levels will be necessary as held stock will be transferred.\r"
										 "Do you wish to continue?", "Warning", MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
	{
		GetCurrentNode();
		Screen->Cursor = crHourGlass;
		try
		{
/*			std::vector<Stock::TStocktakeItemInfo> StocktakeItems;

			PVirtualNode Node = vtvStocktake->GetFirst();
			while (Node)
			{
				if (vtvStocktake->GetNodeLevel(Node) == 2)
				{
					TStockData *NodeData	= (TStockData *)vtvStocktake->GetNodeData(Node);

					Stock::TStocktakeItemInfo StocktakeItemInfo;

					StocktakeItemInfo.StockKey		= NodeData->Key;
					StocktakeItemInfo.Location		= NodeData->Location;
					StocktakeItemInfo.Counted		= NodeData->OnHand;
					StocktakeItemInfo.Variance		= 0;

					StocktakeItems.push_back(StocktakeItemInfo);
				}
				Node = vtvStocktake->GetNext(Node);
			} */
			if (fStocktakeControl.CancelInitialisation(fStocktake.StocktakeKey))
			{
				fStocktake.Initialised = false;
			}
		}
		__finally
		{
			Screen->Cursor = crDefault;
			if (Transaction->InTransaction) Transaction->Commit();
			if (!Transaction->InTransaction) Transaction->StartTransaction();

			LoadTree();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmStocktake::ResetInitialisation(int RadioButtonValue)
{
	Screen->Cursor = crHourGlass;
	try
	{
		bool CopyOnHand = RadioButtonValue == 0;
		fStocktakeControl.ResetInitialisation(fStocktake.StocktakeKey,CopyOnHand);
	}
	__finally
	{
		Screen->Cursor = crDefault;
		if (Transaction->InTransaction) Transaction->Commit();
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		LoadTree();
	}
}
//---------------------------------------------------------------------------

