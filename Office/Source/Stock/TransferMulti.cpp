//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <vector>
#include "TransferMulti.h"
#include "Login.h"
#include "StockData.h"
#include "GetBarcode.h"
#include "Utilities.h"
#include "MM_Math.h"
#include "MM_StockTransactions.h"
#include "stockreportdata.h"
#include "Reports.h"
#include "Setup.h"
//---------------------------------------------------------------------------    
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
//TfrmTransferMulti *frmTransferMulti;
//---------------------------------------------------------------------------
__fastcall TfrmTransferMulti::TfrmTransferMulti(TComponent* Owner)
: TForm(Owner)
{
	isStockRequestMode = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::FormShow(TObject *Sender)
{
	vtvStockQty->Clear();
	vtvStockQty->NodeDataSize	= sizeof(TStockNodeData);
	lbeTitle->Caption				= "Loading...";
	lbeFrom->Caption				= Source;
	lbeTo->Caption					= Destination;
	edReference->Text				= "";
  	dtpTransferDate->Date		= Now();
	LoadTreeView();
	isStockRequestMode = SelectedStockRequestKeys->Count > 0;
     if (!Transaction->InTransaction)
	Transaction->StartTransaction();
    sqlTransferNumber->Close();
    sqlTransferNumber->SQL->Text = "SELECT GEN_ID(GEN_TRANSFER_NUMBER, 0)FROM RDB$DATABASE" ;
    sqlTransferNumber->ExecQuery();
    lbeTransferNumber->Caption = sqlTransferNumber->Fields[0]->AsInteger +1;
     Transfer_no  = sqlTransferNumber->Fields[0]->AsInteger +1;

    Transaction->Commit();
	if(isStockRequestMode)
	    btnScan->Enabled = false;

	PostMessage(Handle, WM_LOADTRANSFER, 0, 0);



}
//---------------------------------------------------------------------------

void TfrmTransferMulti::LoadTreeView()
{
	BatchKeyList = new TStringList;
	tvTransfers->Items->BeginUpdate();
	tvTransfers->OnChange = NULL;
	tvTransfers->Items->Clear();
	TTreeNode *PreviousTransferNode		= NULL;
	TTreeNode *CurrentTransferNode		= NULL;
	PreviousTransferNode = tvTransfers->Items->Add(NULL,PreviousTransfer);
	CurrentTransferNode = tvTransfers->Items->Add(NULL,CurrentTransfer);
	PreviousTransferNode->ImageIndex = ICON_CLOSED_FOLDER_INDEX;
	CurrentTransferNode->ImageIndex = ICON_CLOSED_FOLDER_INDEX;
	PreviousTransferNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
	CurrentTransferNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
	CurrentTransferNode->Data = (void *)NULL;
	PreviousTransferNode->Data = (void *)NULL;
	if (!Transaction->InTransaction)
	Transaction->StartTransaction();
	qrGetPreviousTransfers->Close();
	qrGetPreviousTransfers->ParamByName("LOCATION")->AsString =Source;
	qrGetPreviousTransfers->ParamByName("PURCHASER_NAME")->AsString =Destination;
	for (qrGetPreviousTransfers->Open(); !qrGetPreviousTransfers->Eof; qrGetPreviousTransfers->Next())
	{
		tvTransfers->Items->AddChild(PreviousTransferNode,qrGetPreviousTransfers->FieldByName("CREATED")->AsDateTime);
		BatchKeyList->Add(AnsiString(qrGetPreviousTransfers->FieldByName("BATCH_KEY")->AsInteger));
	}
    Transaction->Commit();
    BatchKeyList->Clear();
    tvTransfers->Items->EndUpdate();
	tvTransfers->OnChange = tvTransfersChange;
	tvTransfers->Selected = CurrentTransferNode;
	tvTransfersChange(tvTransfers, tvTransfers->Selected);
}

void __fastcall TfrmTransferMulti::WMLoadTransfer(TMessage& Message)
{
	if (!Transaction->InTransaction)
	    Transaction->StartTransaction();

	if(isStockRequestMode)
    	LoadStocksForStockRequestMode();
	else
	    LoadStocksForManualMode();

    if (Transaction->InTransaction)
	    Transaction->Commit();

	vtvStockQty->FullExpand();
	vtvStockQty->EndUpdate();

	lbeTitle->Caption = "Enter the quantities of stock you wish to transfer";
}
//---------------------------------------------------------------------------
void TfrmTransferMulti::LoadStocksForManualMode()
{
	qrStockTransferManual->Close();

	std::set<int> availableProducts;

	for (qrStockTransferManual->Open(); !qrStockTransferManual->Eof; qrStockTransferManual->Next())
	{
		if (qrStockTransferManual->FieldByName("Location")->AsString == Source)
		{
			availableProducts.insert(qrStockTransferManual->FieldByName("Stock_Key")->AsInteger);
		}
	}

	PVirtualNode CategoryNode		= NULL;
	PVirtualNode GroupNode			= NULL;
	PVirtualNode StockNode			= NULL;
	TStockNodeData *NodeData		= NULL;

	AnsiString LastCategory, LastGroup, LastStock;

	bool Initialised = false;
    AnsiString DecPlacesFormat = CurrentConnection.SettingDecimalPlaces == 4 ? "0.0000" : "0.00";

	vtvStockQty->BeginUpdate();

	for (qrStockTransferManual->First(); !qrStockTransferManual->Eof; qrStockTransferManual->Next())
	{
		if (availableProducts.find(qrStockTransferManual->FieldByName("Stock_Key")->AsInteger) != availableProducts.end())
		{
			if (!CategoryNode || LastCategory != qrStockTransferManual->FieldByName("Stock_Category")->AsString)
			{
				CategoryNode	= vtvStockQty->AddChild(NULL);
				NodeData			= (TStockNodeData *)vtvStockQty->GetNodeData(CategoryNode);
				LastCategory	= qrStockTransferManual->FieldByName("Stock_Category")->AsString;
				NodeData->Text	= LastCategory;
			}

			if (!GroupNode || LastGroup != qrStockTransferManual->FieldByName("Stock_Group")->AsString)
			{
				GroupNode		= vtvStockQty->AddChild(CategoryNode);
				NodeData			= (TStockNodeData *)vtvStockQty->GetNodeData(GroupNode);
				LastGroup		= qrStockTransferManual->FieldByName("Stock_Group")->AsString;
				NodeData->Text	= LastGroup;
			}

			if (!StockNode || LastStock != qrStockTransferManual->FieldByName("Description")->AsString)
			{
				StockNode							= vtvStockQty->AddChild(GroupNode);
				NodeData								= (TStockNodeData *)vtvStockQty->GetNodeData(StockNode);
				LastStock							= qrStockTransferManual->FieldByName("Description")->AsString;
				NodeData->Text						= LastStock;
				NodeData->Unit						= qrStockTransferManual->FieldByName("Stocktake_Unit")->AsString;
				NodeData->Key						= qrStockTransferManual->FieldByName("Stock_Key")->AsInteger;
				NodeData->Initialised			= (qrStockTransferManual->FieldByName("Initialised")->AsString == "T");
				NodeData->InitialisedTime		= qrStockTransferManual->FieldByName("Initialised_Time")->AsDateTime;
				NodeData->DestOnHand				= 0;
				NodeData->StockRequestKey = 0;
			}


			if (qrStockTransferManual->FieldByName("Location")->AsString == Source)
			{
                NodeData->SourceOnHand = StrToFloat(FormatFloat(DecPlacesFormat,qrStockTransferManual->FieldByName("On_Hand")->AsFloat));
			}
			else if (qrStockTransferManual->FieldByName("Location")->AsString == Destination)
			{
                NodeData->DestOnHand = StrToFloat(FormatFloat(DecPlacesFormat,qrStockTransferManual->FieldByName("On_Hand")->AsFloat));
			}

			if (NodeData->Initialised)
			{
				Initialised = true;
			}
		}
	}

	if (Initialised)
	{
		Application->MessageBox("1 or more items have been initialsed for stocktake.", "Warning", MB_OK + MB_ICONWARNING);
	}
}

//---------------------------------------------------------------------------

void TfrmTransferMulti::LoadStocksForStockRequestMode()
{
	qrStock->Close();

	std::set<int> availableProducts;

	for (qrStock->Open(); !qrStock->Eof; qrStock->Next())
	{
		if (qrStock->FieldByName("SOURCE_LOCATION")->AsString == Source &&qrStock->FieldByName("DESTINATION_LOCATION")->AsString== Destination && SelectedStockRequestKeys->IndexOf(qrStock->FieldByName("REQUEST_NUMBER")->AsString) != -1)
		{
			availableProducts.insert(qrStock->FieldByName("Stock_Key")->AsInteger);
		}
	}



	PVirtualNode CategoryNode		= NULL;
	PVirtualNode GroupNode			= NULL;
	PVirtualNode StockNode			= NULL;
	TStockNodeData *NodeData		= NULL;

	AnsiString LastCategory, LastGroup, LastStock;

	bool Initialised = false;

	vtvStockQty->BeginUpdate();

	for (qrStock->First(); !qrStock->Eof; qrStock->Next())
	{
		if (availableProducts.find(qrStock->FieldByName("Stock_Key")->AsInteger) != availableProducts.end())
		{
			if (!CategoryNode || LastCategory != qrStock->FieldByName("Stock_Category")->AsString)
			{
				CategoryNode	= vtvStockQty->AddChild(NULL);
				NodeData			= (TStockNodeData *)vtvStockQty->GetNodeData(CategoryNode);
				LastCategory	= qrStock->FieldByName("Stock_Category")->AsString;
				NodeData->Text	= LastCategory;
			}

			if (!GroupNode || LastGroup != qrStock->FieldByName("Stock_Group")->AsString)
			{
				GroupNode		= vtvStockQty->AddChild(CategoryNode);
				NodeData			= (TStockNodeData *)vtvStockQty->GetNodeData(GroupNode);
				LastGroup		= qrStock->FieldByName("Stock_Group")->AsString;
				NodeData->Text	= LastGroup;
			}

			if ((!StockNode || LastStock != qrStock->FieldByName("Description")->AsString) && SelectedStockRequestKeys->IndexOf(qrStock->FieldByName("REQUEST_NUMBER")->AsString) != -1)
			{   


                qrStockReqUnit->Close();
                qrStockReqUnit->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger = qrStock->FieldByName("STOCK_REQUEST_KEY")->AsInteger;
                qrStockReqUnit->Open();
                qrStockReqUnit->First();
                AnsiString	StockReqUnit =   qrStockReqUnit->FieldByName("STOCK_REQUEST_UNIT")->AsString;

             qrQuantityUpdate->Close();
			qrQuantityUpdate->ParamByName("STOCK_KEY")->AsInteger = qrStock->FieldByName("STOCK_KEY")->AsInteger;;
			qrQuantityUpdate->ParamByName("SUPPLIER_UNIT")->AsString = StockReqUnit;

			qrQuantityUpdate->Open();
			qrQuantityUpdate->First();
	     	StockTakeQty =   qrQuantityUpdate->FieldByName("QTY")->AsFloat;


				StockNode							= vtvStockQty->AddChild(GroupNode);
				NodeData								= (TStockNodeData *)vtvStockQty->GetNodeData(StockNode);
				LastStock							= qrStock->FieldByName("Description")->AsString;
				NodeData->Text						= LastStock;
				NodeData->Unit						=    StockReqUnit;  //qrStock->FieldByName("STOCK_REQUEST_UNIT")->AsString;
				NodeData->Key						= qrStock->FieldByName("Stock_Key")->AsInteger;
				NodeData->Initialised			= (qrStock->FieldByName("Initialised")->AsString == "T");
				NodeData->InitialisedTime		= qrStock->FieldByName("Initialised_Time")->AsDateTime;
				NodeData->DestOnHand				= 0;
				NodeData->Quantity =  qrStock->FieldByName("QTY_REQUIRED")->AsFloat;   //Qty*
               	NodeData->Transfer_Real_Quantity =qrStock->FieldByName("QTY_REQUIRED")->AsFloat;//Qty*


				NodeData->StockRequestKey = qrStock->FieldByName("STOCK_REQUEST_KEY")->AsInteger;
                   


         

			}

			if (qrStock->FieldByName("SOURCE_LOCATION")->AsString == Source)
			{
				NodeData->SourceOnHand = qrStock->FieldByName("On_Hand")->AsFloat;
			}
			else if (qrStock->FieldByName("SOURCE_LOCATION")->AsString == Destination)
			{
				NodeData->DestOnHand = qrStock->FieldByName("On_Hand")->AsFloat;
			}

			if (NodeData->Initialised)
			{
				Initialised = true;
			}
		}
	}

	if (Initialised)
	{
		Application->MessageBox("1 or more items have been initialsed for stocktake.", "Warning", MB_OK + MB_ICONWARNING);
	}
}


void __fastcall TfrmTransferMulti::FormCloseQuery(TObject *Sender,
bool &CanClose)
{
	if (Transaction->InTransaction && vtvStockQty->Visible)
	{   Transaction->Commit();
	   //	CanClose = (Application->MessageBox("Are you sure you wish to cancel?", "Cancel Transfer?", MB_OKCANCEL + MB_ICONWARNING) == IDOK);
	}
	else
	{
		CanClose = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::FormClose(TObject *Sender,
TCloseAction &Action)
{
	if (Transaction->InTransaction)
	{
		Transaction->Rollback();
	}
	FindDialog->CloseDialog();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::vtvStockQtyGetText(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
TVSTTextType TextType, WideString &CellText)
{
	TStockNodeData *NodeData = (TStockNodeData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		if (Column == 0)
		{
			CellText = NodeData->Text;
		}
		else if (Sender->GetNodeLevel(Node) == 2)
		{
			if (Column == 1)
			{
				CellText = NodeData->Unit;
			}
			else if (Column == 2)
			{
             if(CurrentConnection.SettingDecimalPlaces==4)
            {
				CellText =  FormatFloat("0.0000",NodeData->SourceOnHand);  //MMMath::FloatString(NodeData->SourceOnHand);
			}
            else
            {
             	CellText =  FormatFloat("0.00",NodeData->SourceOnHand); 

            }
            }
			else if (Column == 3)
			{
            if(CurrentConnection.SettingDecimalPlaces==4)
            {
				CellText =  FormatFloat("0.0000",NodeData->DestOnHand); //MMMath::FloatString(NodeData->DestOnHand);
			}
            else
            {
            	CellText =  FormatFloat("0.00",NodeData->DestOnHand);
            }
            }
			else if (Column == 4 )//&& NodeData->Quantity != 0)
			{
             if(CurrentConnection.SettingDecimalPlaces==4)
                {
				CellText =  FormatFloat("0.0000",NodeData->Quantity);//MMMath::FloatString(NodeData->Quantity);
                }
                else
                {
                 	CellText =  FormatFloat("0.00",NodeData->Quantity);
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
void __fastcall TfrmTransferMulti::vtvStockQtyGetImageIndex(
TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind,
TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
	TStockNodeData *NodeData = (TStockNodeData *)Sender->GetNodeData(Node);
	if (Column == ((TVirtualStringTree *)Sender)->Header->MainColumn)
	{
		if (NodeData)
		{
			switch (Sender->GetNodeLevel(Node))
			{
			case 0:	// Category
			case 1:	// Group
				{
					switch (Kind)
					{
					case ikState:
						{
							ImageIndex = ICON_CLOSED_FOLDER_INDEX;
							break;
						}
					case ikNormal:
						{
							ImageIndex = ICON_CLOSED_FOLDER_INDEX;
							break;
						}
					case ikSelected:
						{
							ImageIndex = ICON_OPEN_FOLDER_INDEX;
							break;
						}
					}
					break;
				}
			case 2:	// Stock Item
				{
					ImageIndex = ICON_BOX_INDEX;
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
void __fastcall TfrmTransferMulti::vtvStockQtyBeforePaint(
TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::vtvStockQtyAfterPaint(
TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::vtvStockQtyKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{
	if (!vtvStockQty->IsEditing())
	{
		if (Key == VK_RETURN)
		{
			vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
		}
		else if (Key == VK_RIGHT)
		{
			vtvStockQty->Expanded[vtvStockQty->FocusedNode] = true;
		}
		else if (Key == VK_LEFT)
		{
			vtvStockQty->Expanded[vtvStockQty->FocusedNode] = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::vtvStockQtyFocusChanged(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	if (Node && !Sender->IsEditing())
	{
		if (Sender->GetNodeLevel(Node) != 2)
		{
			Sender->FocusedColumn = 0;
		}
		else
		{
			Sender->FocusedColumn = 4;
			Sender->EditNode(Node, Column);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::vtvStockQtyFocusChanging(
TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::vtvStockQtyCreateEditor(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
IVTEditLink *EditLink)
{


	if (Node && Column == 4)
	{
		if (Sender->GetNodeLevel(Node) == 2)
		{

			TStockNodeData *NodeData = (TStockNodeData *)Sender->GetNodeData(Node);
			neStockQty->Value = NodeData->Quantity;
            neStockQty->DecimalPlaces = CurrentConnection.SettingDecimalPlaces;
			TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, neStockQty);
			PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
			PostMessage(neStockQty->Handle, EM_SETSEL, 0, -1);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::vtvStockQtyEditing(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
bool &Allowed)
{
	Allowed = (Column == 4 && Sender->GetNodeLevel(Node) == 2);
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::vtvStockQtyEdited(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	TStockNodeData *NodeData = (TStockNodeData *)vtvStockQty->GetNodeData(vtvStockQty->FocusedNode);
	NodeData->Quantity = neStockQty->Value;
	//	vtvStockQty->Repaint();
	vtvStockQty->InvalidateNode(vtvStockQty->FocusedNode);
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::neStockQtyKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN || Key == VK_DOWN || Key == VK_UP)
	{
		vtvStockQty->BeginUpdate();
		try
		{
			vtvStockQty->EndEditNode();
		}
		__finally
		{
			vtvStockQty->EndUpdate();
		}
		if (Key == VK_RETURN || Key == VK_DOWN)
		{
			if (vtvStockQty->GetNext(vtvStockQty->FocusedNode))
			{
				vtvStockQty->FocusedNode = vtvStockQty->GetNext(vtvStockQty->FocusedNode);
				vtvStockQty->Selected[vtvStockQty->FocusedNode] = true;
			}
			else
			{
				vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
			}
			if (Key == VK_RETURN)
			{
				Key = NULL;
			}
		}
		else if (Key == VK_UP)
		{
			if (vtvStockQty->GetPrevious(vtvStockQty->FocusedNode))
			{
				vtvStockQty->FocusedNode = vtvStockQty->GetPrevious(vtvStockQty->FocusedNode);
				vtvStockQty->Selected[vtvStockQty->FocusedNode] = true;
			}
			else
			{
				vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
			}
		}
	}
	else if (Key == VK_ESCAPE)
	{
		vtvStockQty->CancelEditNode();
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}





//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::btnOkClick(TObject *Sender)
{

    bool item_in_source_is_less_than_transfer=false;
    std::vector<int > stock_request_stock_key;
    std::vector<int > stock_request_to_be_deleted;
    double getQuantity;


	btnOk->SetFocus();
	vtvStockQty->EndEditNode();

        bool ContinueCheck = true;
        bool CheckQuantityExist=false;
        PVirtualNode NodeCheck = vtvStockQty->GetFirst();
        while (NodeCheck && ContinueCheck)
        {
           TStockNodeData *NodeDataCheck = (TStockNodeData *)vtvStockQty->GetNodeData(NodeCheck);
            if(NodeDataCheck->Quantity>0)
            {
             CheckQuantityExist=true;

                break;
            }


            NodeCheck = vtvStockQty->GetNext(NodeCheck);
        }
       if(!CheckQuantityExist)
        {
      //  Close();
        // return;
        }
        else{
            if (Application->MessageBox(("This will transfer stock from \"" + Source + "\" to \"" +
                            Destination + "\".\rDo you wish to continue?").c_str(),
                        "Transfer Stock?",
                        MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) != IDOK)
            {

                return;
            }

        }
 /**********************************************/ //done

  bool Continue = true;
	bool IgnoreLowOnHand = false;
	PVirtualNode Node = vtvStockQty->GetFirst();
   while (Node && Continue)
	{
            TStockNodeData *NodeData = (TStockNodeData *)vtvStockQty->GetNodeData(Node);
        	if (NodeData->Quantity < 0)
				{
					if (Application->MessageBox(("You have requested a negative quantity transfer for the item \"" + NodeData->Text +
									"\".\rThis will actually transfer a positive quantity in the opposite direction.\rDo you wish to continue?").c_str(),
								"Transfer Stock?",
								MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) != IDOK)
					{
						Continue = false;

					}
				}
                   else if(NodeData->Quantity>=0)
		              {
                    getQuantity = NodeData->Quantity;
                     item_in_source_is_less_than_transfer=true;

                    //     if (!Transaction->InTransaction)
					//       Transaction->StartTransaction();

                           qrfetch_request_number->Close();
                           qrfetch_request_number->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger=NodeData->StockRequestKey ;
                           qrfetch_request_number->Open() ;
                           bool Stock_request_number_Present=false;

                          for(std::vector<int>::iterator i = stock_request_stock_key.begin(); i!= stock_request_stock_key.end();++i)
                           {
                              if ( (*i)== ( qrfetch_request_number->FieldByName("REQUEST_NUMBER")->AsInteger))
                              {
                                 Stock_request_number_Present =True;
                              }

                        	}


                            if(  !Stock_request_number_Present)
                            {
                               stock_request_stock_key.push_back( qrfetch_request_number->FieldByName("REQUEST_NUMBER")->AsInteger);
                            }

                        // 	Transaction->Commit();
                             stock_request_stock_key.clear();

			if (vtvStockQty->GetNodeLevel(Node) == 2)
			{

				//	if (NodeData->Quantity != 0)
				//	{

				 if (NodeData->Quantity > NodeData->SourceOnHand && !IgnoreLowOnHand&& NodeData->Quantity!=0)
				{
                    int Result = MessageDlg("You have requested to transfer more than you have on hand for the item \"" +
					StringReplace(NodeData->Text, "&", "&&", TReplaceFlags() << rfReplaceAll) +
					"\".\rDo you wish to continue?",
					mtConfirmation,
					TMsgDlgButtons() << mbYes << mbNo << mbYesToAll,
					0);
					if (Result == mrYes)
					{
						Continue = true;
					}
					else if (Result == mrNo)
					{
						Continue = false;
					}
					if (Result == mrYesToAll)
					{
						IgnoreLowOnHand = true;
					}
				}
				//	}
			}
		}
		Node = vtvStockQty->GetNext(Node);

	}
	bool Success = true;

     if (!Transaction->InTransaction)
	Transaction->StartTransaction();
      sqlTransferNumber->Close();
    sqlTransferNumber->SQL->Text = "SELECT GEN_ID(GEN_TRANSFER_NUMBER, 1)FROM RDB$DATABASE" ;
    sqlTransferNumber->ExecQuery();
    lbeTransferNumber->Caption = sqlTransferNumber->Fields[0]->AsInteger;
     Transfer_no  = sqlTransferNumber->Fields[0]->AsInteger;
     	Transaction->Commit();

	if (Continue)
	{
		bool Initialised					= false;
		TransfferedStockRequestKeys = new TStringList;
		TDateTime MinInitialiseTime	= 0.0;

		std::vector<Stock::TTransferInfo> Transfers;



         bool stock_request_delete_option_selected=false;
         bool stock_request_key_to_be_added=true;



        for(std::vector<int>::iterator i = stock_request_stock_key.begin(); i!= stock_request_stock_key.end();++i)
        {
                stock_request_delete_option_selected=false;

            	PVirtualNode Node = vtvStockQty->GetFirst();

          	while (Node && Success)
	           	{


	            	if (vtvStockQty->GetNodeLevel(Node) == 2)
		               	{
                           stock_request_key_to_be_added=true;

                          TStockNodeData *NodeData = (TStockNodeData *)vtvStockQty->GetNodeData(Node);
                                      qrfetch_request_number->Close();
                                      qrfetch_request_number->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger=NodeData->StockRequestKey ;
                                      qrfetch_request_number->Open() ;

		                      if (NodeData->Quantity >= 0 && ( (*i)== ( qrfetch_request_number->FieldByName("REQUEST_NUMBER")->AsInteger))   )

		                	 	{

                                       if (NodeData->Transfer_Real_Quantity > NodeData->Quantity)
                                       {
                                             if ( stock_request_delete_option_selected)
                                              {

                                                              if (NodeData->Transfer_Real_Quantity >=0)
                                                             {



                                                               	qrUpdateStockRequestQuantity->Close();
				                                                qrUpdateStockRequestQuantity->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger=NodeData->StockRequestKey ;
                                                                qrUpdateStockRequestQuantity->ParamByName("QTY_REQUIRED")->AsFloat=(NodeData->Transfer_Real_Quantity-NodeData->Quantity);
                                                               	qrUpdateStockRequestQuantity->ExecSQL();
                                                                stock_request_key_to_be_added=false;

                                                              //  NodeData->Quantity= NodeData->SourceOnHand;
                                                               }
                                              }
                                              else
                                              {

                                                  AnsiString Msg =  "All of the items in Stock Request no " + IntToStr( *i )+ " haven't been transferred completely. Do you wish to close the stock request?";
                                                     if (Application->MessageBox(Msg.c_str(), "MESSAGE", MB_ICONWARNING + MB_YESNO) == ID_NO)
			                                          {
                                                          //  user Press NO Button which means user dont want to delete the stock Request
                                                               if (NodeData->Transfer_Real_Quantity >=0)
                                                             {


                                                               	qrUpdateStockRequestQuantity->Close();
				                                                qrUpdateStockRequestQuantity->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger=NodeData->StockRequestKey ;
                                                                qrUpdateStockRequestQuantity->ParamByName("QTY_REQUIRED")->AsFloat=NodeData->Transfer_Real_Quantity - NodeData->Quantity ;
                                                               	qrUpdateStockRequestQuantity->ExecSQL();



                                                             }
                                                               stock_request_delete_option_selected=false;
                                                               stock_request_key_to_be_added=false;
                                                       } else
                                                       {


                                                            	qrUpdateStockRequestQuantity->Close();
				                                                qrUpdateStockRequestQuantity->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger=NodeData->StockRequestKey ;
                                                                qrUpdateStockRequestQuantity->ParamByName("QTY_REQUIRED")->AsFloat= NodeData->Quantity ;
                                                               	qrUpdateStockRequestQuantity->ExecSQL();


                                                         stock_request_to_be_deleted.push_back( qrfetch_request_number->FieldByName("REQUEST_NUMBER")->AsInteger);
                                                         stock_request_delete_option_selected=true;
                                                       }
                                               }
                                          }

                                          if (NodeData->Quantity > 0 )
                                          {
                                               Stock::TTransferInfo			TransferInfo;
                                                TransferInfo.StockKey		= NodeData->Key;
                                                if(StockTakeQty > 0)
                                                {
                                                    TransferInfo.Quantity		= NodeData->Quantity*StockTakeQty;
                                                }
                                                else
                                                {
                                                     TransferInfo.Quantity		= NodeData->Quantity;
                                                }
                                                TransferInfo.Source			= Source;
                                                TransferInfo.Destination	= Destination;
                                                TransferInfo.STOCK_REQUEST_NO= (*i);
                                                

                                                if (stock_request_delete_option_selected)
                                                {
                                                  TransferInfo.REQUESTED_QTY_ORIGINAL_VAL= NodeData->Quantity;

                                                }
                                                else
                                                {
                                                    TransferInfo.REQUESTED_QTY_ORIGINAL_VAL= NodeData->Transfer_Real_Quantity;

                                                }
                                                TransferInfo.STOCK_REQUEST_STOCK_KEY = NodeData->StockRequestKey;
                                                TransferInfo.Transfer_id = StrToInt( lbeTransferNumber->Caption);
                                                Transfers.push_back(TransferInfo);
                                               // Transfers.clear();
                                           }
                                            if( stock_request_key_to_be_added)
                                            {
                                                TransfferedStockRequestKeys->Add((AnsiString)NodeData->StockRequestKey);
                                            }

				                         	if (NodeData->Initialised)
				                            {
				    	                    	Initialised = true;
					                        	if (NodeData->InitialisedTime != TDateTime(0.0))
					                        	{
					                            		if (NodeData->InitialisedTime < MinInitialiseTime || MinInitialiseTime == TDateTime(0.0))
					                             		{
						                                		MinInitialiseTime = NodeData->InitialisedTime;
						                            	}
						                        }
				                        	}
			                       }

			                }
		                    	Node = vtvStockQty->GetNext(Node);

                               // delete
                               stock_request_stock_key.clear();


	               	}

       }

		if (Initialised)
		{
			if (MinInitialiseTime < dtpTransferDate->Date)
			{
				// Record the transaction as 1 min before initialisation of the earliest initialisation.
				MinInitialiseTime -= EncodeTime(0, 1, 0, 0);

				if (Application->MessageBox(AnsiString("Some items you wish to transfer have been initialised for a stocktake.\r\r"
								"If you continue, the date of the transfer will be recorded as " + MinInitialiseTime.DateTimeString() + "\r"
								"and the transfer amounts will be included in the stocktake." + MinInitialiseTime.DateTimeString()).c_str(),
							"Continue?", MB_OKCANCEL + MB_ICONQUESTION) != ID_OK)
				{
					Continue = false;
				}
			}
		}



		 if (Continue)
		 {


			Stock::TTransferStock			CreateTransfers(dmStockData->dbStock);
			Stock::TTransactionBatchInfo	BatchInfo(Stock::ttTransfer);

			BatchInfo.User_ID		= frmLogin->CurrentUserID;

            BatchInfo.User_Name	= frmLogin->CurrentUserName;

			BatchInfo.Reference	= edReference->Text;

			if (Initialised)
			{
				BatchInfo.Created = MinInitialiseTime;
			}
			else
			{
			    dtpTransferDate->Time = Now().CurrentTime();
				BatchInfo.Created = dtpTransferDate->DateTime;
			}



		     if (CreateTransfers.TransferStock(BatchInfo, Transfers))
			   {
		   		if( isStockRequestMode)

				    {
				     	//Update Stock Request as per the user selection
					    if (!Transaction->InTransaction)
					       Transaction->StartTransaction();

                           if(TransfferedStockRequestKeys->Count > 0)
                           {

                                  for (int i=0; i<TransfferedStockRequestKeys->Count; i++)
		                        	{
                                          qrUpdateStockRequest->Close();
                                         qrUpdateStockRequest->ParamByName("STOCK_REQUEST_STOCKKEY")->AsInteger= StrToInt(TransfferedStockRequestKeys->Strings[i]);
                                          qrUpdateStockRequest->ExecSQL();

                                    }



                           }
                              delete TransfferedStockRequestKeys;
                            //delete Stock Request as per the user selection
                            for(std::vector<int>::iterator i = stock_request_to_be_deleted.begin(); i!= stock_request_to_be_deleted.end();++i)
                              {
                                   qrdelete_stock_request->ParamByName("REQUEST_NUMBER")->AsInteger= *i;
                                  qrdelete_stock_request->ExecSQL();
                              }

                         	   Transaction->Commit();
                               stock_request_to_be_deleted.clear();

                      }


				}

                    stock_request_stock_key.clear();

                    if(CurrentConnection.AutoPrintStockTransferAudit && getQuantity !=0  )
                    {
                         TStringList *Locations=new TStringList;
                            Locations->Add(Source);
                            Locations->Add(Destination);



                                 dmStockReportData->StockTrans->StartTransaction();
                             try
                            {

                            dmStockReportData->SetupStockTransfer(BatchInfo.Created,BatchInfo.Created,NULL,NULL,StrToInt(lbeTransferNumber->Caption));
                            if (frmReports->rvStock->SelectReport("repStockTransferAudit", false))
                            {

                           //      AnsiString DateRange =	"From " + BatchInfo.Created.FormatString("ddddd 'at' hh:nn") +
						   //						"\rto " + BatchInfo.Created.FormatString("ddddd 'at' hh:nn");
                           //         frmReports->rvStock->SetParam("ReportRange", DateRange);

			                    	frmReports->rvStock->SetParam("Source", Source);
                                    frmReports->rvStock->SetParam("Destination", Destination);


                                    frmReports->rvStock->SetParam("CompanyName", CurrentConnection.CompanyName);
                                    frmReports->rvStock->SetParam("CompanyName", CurrentConnection.CompanyName);


                                   	frmReports->rvStock->SetParam("UserName",  BatchInfo.User_Name +" at " + Now());
                                       
					             	frmReports->rvStock->Execute();
                            }

                          }
                        __finally
	                      {

                               dmStockReportData->StockTrans->Commit();
                               delete Locations;

	                         }

                      }


                // delete NodeData ;
                Close();

			}
			else
			{
				Application->MessageBox("There was a problem transferring the stock. No stock has been transferred", "Error", MB_OK + MB_ICONERROR);
			}
		}



}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::btnFindClick(TObject *Sender)
{
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::FindDialogFind(TObject *Sender)
{
	PVirtualNode CurItem;
	if (vtvStockQty->FocusedNode)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = vtvStockQty->GetNext(vtvStockQty->FocusedNode);
		}
		else
		{
			CurItem = vtvStockQty->GetPrevious(vtvStockQty->FocusedNode);
		}
	}
	else
	{
		CurItem = vtvStockQty->GetFirst();
	}
	while (CurItem)
	{
		TStockNodeData *NodeData = (TStockNodeData *)vtvStockQty->GetNodeData(CurItem);
		AnsiString FindText, NodeText;
		if (FindDialog->Options.Contains(frMatchCase))
		{
			FindText = FindDialog->FindText;
			NodeText = NodeData->Text;
		}
		else
		{
			FindText = FindDialog->FindText.UpperCase();
			NodeText = AnsiString(NodeData->Text).UpperCase();
		}
		if (FindDialog->Options.Contains(frWholeWord))
		{
			if (FindText == NodeText)
			{
				vtvStockQty->FocusedNode = CurItem;
				vtvStockQty->Selected[CurItem] = true;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				vtvStockQty->FocusedNode = CurItem;
				vtvStockQty->Selected[CurItem] = true;
				break;
			}
		}
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = vtvStockQty->GetNext(CurItem);
		}
		else
		{
			CurItem = vtvStockQty->GetPrevious(CurItem);
		}
	}
	if (!CurItem)
	{
		Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::btnScanClick(TObject *Sender)
{
	if (frmGetBarcode->ShowModal() == mrOk)
	{
		bool Found = false;
		/*PVirtualNode CurItem = vtvStockQty->GetFirst();
		while (CurItem)
		{
			if (vtvStockQty->GetNodeLevel(CurItem) == 2)
			{
				TStockNodeData *NodeData = (TStockNodeData *)vtvStockQty->GetNodeData(CurItem);
				if (NodeData->Barcode == frmGetBarcode->Barcode)
				{
					vtvStockQty->FocusedNode = CurItem;
					vtvStockQty->Selected[CurItem] = true;
					Found = true;
					break;
				}
			}
			CurItem = vtvStockQty->GetNext(CurItem);
		}  */
		if (!Found)
		{
			// can only be 1 barcode
			qrFindBarcode->Close();
			qrFindBarcode->ParamByName("Barcode")->AsString = frmGetBarcode->Barcode;
			qrFindBarcode->Open();

			if (!qrFindBarcode->Eof)
			{
				int StockKey = qrFindBarcode->FieldByName("Stock_Key")->AsInteger;
				// maybe linked to SupplierStock and not Stock
				if (StockKey == -1)
				{
					StockKey = qrFindBarcode->FieldByName("Supplier_Stock_Stock_Key")->AsInteger;
				}
				PVirtualNode CurItem = vtvStockQty->GetFirst();
				while (CurItem)
				{
					if (vtvStockQty->GetNodeLevel(CurItem) == 2)
					{
						TStockNodeData *NodeData = (TStockNodeData *)vtvStockQty->GetNodeData(CurItem);
						if (NodeData->Key == StockKey)
						{
							vtvStockQty->FocusedNode = CurItem;
							vtvStockQty->Selected[CurItem] = true;
							Found = true;
							break;
						}
					}
					CurItem = vtvStockQty->GetNext(CurItem);
				}
			}
			qrFindBarcode->Close();
		}
		if (!Found)
		{
			Application->MessageBox("Product not found.", "Not found", MB_ICONWARNING + MB_OK);
		}
	}
	if (vtvStockQty->FocusedNode && !vtvStockQty->IsEditing())
	{
		vtvStockQty->FocusedColumn = 4;
		vtvStockQty->EditNode(vtvStockQty->FocusedNode, vtvStockQty->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferMulti::neStockQtyKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN) Key = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransferMulti::tvTransfersChange(TObject *Sender,
TTreeNode *Node)
{
	if (Node && Node->Level == 0)
	{
		if(Node->Text == PreviousTransfer)
		{
			btnOk->Enabled = false;
			btnFind->Enabled = false;
			btnScan->Enabled = false;
			dbgTransferedStock->Visible =true;
			vtvStockQty->Visible = false;


		}
		else
		{
			btnOk->Enabled = true;
			btnFind->Enabled = true;
			btnScan->Enabled = true;
			dbgTransferedStock->Visible =false;
			vtvStockQty->Visible = true;


            lbeTransferNumber->Caption =  Transfer_no;

            btnReprint->Visible =false;


		}

	}
	else
	{
        int batchKey = StrToInt(BatchKeyList->Strings[Node->Index]);
		btnOk->Enabled = false;
		btnFind->Enabled = false;
		btnScan->Enabled = false;
		dbgTransferedStock->Visible =true;
		vtvStockQty->Visible = false;
        qrGetTransferStock->Close();
	    qrGetTransferStock->ParamByName("BATCH_KEY")->AsInteger = batchKey;
		qrGetTransferStock->Open();

         btnReprint->Visible =true;


           qrfindCommitedRequest->Close();
           qrfindCommitedRequest->ParamByName("BATCH_KEY")->AsInteger =batchKey;
           qrfindCommitedRequest->Open();



          lbeTransferNumber->Caption = qrfindCommitedRequest->FieldByName("TRANSFER_ID")->AsInteger;


	}

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TfrmTransferMulti::btnReprintClick(TObject *Sender)
{
    dmStockReportData->StockTrans->StartTransaction();
    try
    {

        TDateTime startTime = qrfindCommitedRequest->FieldByName("CREATED")->AsDateTime;

        dmStockReportData->SetupStockTransfer(startTime,startTime,NULL,NULL,StrToInt(lbeTransferNumber->Caption));

        if (frmReports->rvStock->SelectReport("repStockTransferAudit", false))
        {

         //   AnsiString DateRange =	"From " + startTime.FormatString("ddddd 'at' hh:nn") +
         //           "\rto " + startTime.FormatString("ddddd 'at' hh:nn");
        //    frmReports->rvStock->SetParam("ReportRange", DateRange);

            frmReports->rvStock->SetParam("Source", lbeFrom->Caption);
            frmReports->rvStock->SetParam("Destination", lbeTo->Caption);


            frmReports->rvStock->SetParam("CompanyName", CurrentConnection.CompanyName);
            frmReports->rvStock->SetParam("UserName",  frmLogin->CurrentUserName	 +" at " + Now());
            frmReports->rvStock->Execute();
            frmReports->SetToPreview();
        }

    }
    __finally
    {

        dmStockReportData->StockTrans->Commit();
    }

}
//---------------------------------------------------------------------------


