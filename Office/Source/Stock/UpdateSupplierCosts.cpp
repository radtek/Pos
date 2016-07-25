//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UpdateSupplierCosts.h"
#include "MM_Math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma link "NumericEdit"
#pragma link "NumericEdit"
#pragma resource "*.dfm"
TfrmUpdateSupplierCosts *frmUpdateSupplierCosts;
//---------------------------------------------------------------------------
__fastcall TfrmUpdateSupplierCosts::TfrmUpdateSupplierCosts(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateSupplierCosts::FormShow(TObject *Sender)
{
    vtvStockItem->Clear();
    vtvStockItem->NodeDataSize	= sizeof(TSupplierItemNodeData);
    lbeTitle->Caption = "Update Supplier Costs";
    dbcbPreferredSupplier->Items->Clear();
    dbcbPreferredSupplier->Items->Add(OptionTrue);
    dbcbPreferredSupplier->Items->Add(OptionFalse);
    AddDataToList();
}
//---------------------------------------------------------------------------

void TfrmUpdateSupplierCosts::AddDataToList()
{
    qrGetSupplierStock->Close();
    try
       {
           if (!Transaction->InTransaction)
                Transaction->StartTransaction();
           qrGetSupplierStock->ParamByName("SUPPLIER_KEY")->AsInteger = ContactKey;
           for (qrGetSupplierStock->Open(); !qrGetSupplierStock->Eof; qrGetSupplierStock->Next())
             {
                PVirtualNode OrderItemNode = NULL;
                TSupplierItemNodeData *NodeData = NULL;
                OrderItemNode	= vtvStockItem->AddChild(NULL);
                NodeData = (TSupplierItemNodeData *)vtvStockItem->GetNodeData(OrderItemNode);
                NodeData->SupplierStockKey = qrGetSupplierStock->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger;
                NodeData->SupplierKey = qrGetSupplierStock->FieldByName("SUPPLIER_KEY")->AsInteger;
                NodeData->StockKey = qrGetSupplierStock->FieldByName("STOCK_KEY")->AsInteger;
                NodeData->StockCode = qrGetSupplierStock->FieldByName("CODE")->AsString;
                NodeData->SupplierCode = qrGetSupplierStock->FieldByName("SUPPLIER_CODE")->AsString;
                NodeData->Description = qrGetSupplierStock->FieldByName("DESCRIPTION")->AsString;
                NodeData->StockUnit = qrGetSupplierStock->FieldByName("STOCKTAKE_UNIT")->AsString;
                NodeData->OrderUnit = qrGetSupplierStock->FieldByName("SUPPLIER_UNIT")->AsString;
                NodeData->StockQuantity = qrGetSupplierStock->FieldByName("QTY")->AsCurrency;
                NodeData->SupplierUnitCost = qrGetSupplierStock->FieldByName("LATEST_COST")->AsFloat;
                NodeData->PreferredSupplier = qrGetSupplierStock->FieldByName("PREFERRED_SUPPLIER")->AsString == 'T' ? true : false;
             }

        Transaction->Commit();
    }
    catch (Exception &e)
    {
        Transaction->Rollback();
    }

    if (Transaction->InTransaction)
        Transaction->Commit();
}

void TfrmUpdateSupplierCosts::LoadUnits(int StockKey)
{
    StockUnits = new TStringList;
    dbcbUnit->Items->Clear();
    qrGetStockUnit->Close();
    try
       {
           if (!Transaction->InTransaction)
                Transaction->StartTransaction();
           qrGetStockUnit->ParamByName("STOCK_KEY")->AsInteger = StockKey;
           for (qrGetStockUnit->Open(); !qrGetStockUnit->Eof; qrGetStockUnit->Next())
              {
                  if (qrGetStockUnit->FieldByName("Stocktake_Unit")->AsString != "")
                     {
                        dbcbUnit->Items->Add(qrGetStockUnit->FieldByName("Stocktake_Unit")->AsString);
                        StockUnits->Add(qrGetStockUnit->FieldByName("Stocktake_Unit")->AsString);
                     }
              }

        Transaction->Commit();
    }
    catch (Exception &e)
    {
        Transaction->Rollback();
    }

    if (Transaction->InTransaction)
        Transaction->Commit();
}



void __fastcall TfrmUpdateSupplierCosts::vtvStockItemAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdateSupplierCosts::vtvStockItemBeforePaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdateSupplierCosts::vtvStockItemCreateEditor(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink)
{
	if (Node && Column != 1)
	{
		TSupplierItemNodeData *NodeData = (TSupplierItemNodeData *)Sender->GetNodeData(Node);
	   TPropertyEdit* PropertyLink ;
      switch(Column)
           {
             case 0: PropertyLink = new TPropertyEdit(Sender, Node, Column, neTextEdit);
                  	neTextEdit->Text = NodeData->SupplierCode;
	               	PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
	               	PostMessage(neTextEdit->Handle, EM_SETSEL, 0, -1);
						break;
             case 2: LoadUnits(NodeData->StockKey);
                     PropertyLink = new TPropertyEdit(Sender, Node, Column, dbcbUnit);
                     dbcbUnit->ItemIndex = StockUnits->IndexOf(NodeData->StockUnit);
	               	PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
	               	PostMessage(dbcbUnit->Handle, EM_SETSEL, 0, -1);
						break;
             case 3: LoadUnits(NodeData->StockKey);
                     PropertyLink = new TPropertyEdit(Sender, Node, Column, dbcbUnit);
                     dbcbUnit->ItemIndex = StockUnits->IndexOf(NodeData->OrderUnit);
	               	PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
	               	PostMessage(dbcbUnit->Handle, EM_SETSEL, 0, -1);
						break;
             case 4: PropertyLink = new TPropertyEdit(Sender, Node, Column, neStockNumericEdit);
                  	neStockNumericEdit->Value = NodeData->StockQuantity;
	               	PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
	               	PostMessage(neStockNumericEdit->Handle, EM_SETSEL, 0, -1);
						break;
             case 5: PropertyLink = new TPropertyEdit(Sender, Node, Column, neStockNumericEdit);
                     neStockNumericEdit->Value = NodeData->SupplierUnitCost;
	               	PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
	               	PostMessage(neStockNumericEdit->Handle, EM_SETSEL, 0, -1);
						break;
             case 6: PropertyLink = new TPropertyEdit(Sender, Node, Column, dbcbPreferredSupplier);
                     if(NodeData->PreferredSupplier)
                       dbcbPreferredSupplier->ItemIndex = 0;
                     else
                       dbcbPreferredSupplier->ItemIndex = 1;
	             PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
	             PostMessage(dbcbPreferredSupplier->Handle, EM_SETSEL, 0, -1);
						break;
           }

	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdateSupplierCosts::vtvStockItemEdited(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column)
{
	if (vtvStockItem->FocusedNode)
	{
		if (Column != 1)
		{
			TSupplierItemNodeData *NodeData	= (TSupplierItemNodeData *)vtvStockItem->GetNodeData(vtvStockItem->FocusedNode);
			switch (Column)
		         {
                 	         case 0:	NodeData->SupplierCode = neTextEdit->Text;
					         	break;
			         case 2:	NodeData->StockUnit =    dbcbUnit->Items->Strings[dbcbUnit->ItemIndex];
					         	break;
			         case 3:	NodeData->OrderUnit = dbcbUnit->Items->Strings[dbcbUnit->ItemIndex];
						         break;
			         case 4:	NodeData->StockQuantity = neStockNumericEdit->Value;
					         	break;
			         case 5:	NodeData->SupplierUnitCost = neStockNumericEdit->Value;
						         break;
			         case 6:	NodeData->PreferredSupplier = dbcbPreferredSupplier->ItemIndex == 0 ? true : false;
						         break;
       }
		}
		vtvStockItem->InvalidateNode(vtvStockItem->FocusedNode);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdateSupplierCosts::vtvStockItemEditing(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, bool &Allowed)
{
   Allowed = (Column != 1 && Column!=2);
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdateSupplierCosts::vtvStockItemFocusChanged(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column)
{
	if (Node && !Sender->IsEditing() && Sender->FocusedColumn != 1 && Sender->FocusedColumn != 2)
	{
		Sender->EditNode(Node, Column);
	} 
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdateSupplierCosts::vtvStockItemFocusChanging(TBaseVirtualTree *Sender,
          PVirtualNode OldNode, PVirtualNode NewNode,
          TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	} 
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdateSupplierCosts::vtvStockItemGetImageIndex(TBaseVirtualTree *Sender,
          PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
          bool &Ghosted, int &ImageIndex)
{
	ImageIndex = -1;
	TSupplierItemNodeData *NodeData = (TSupplierItemNodeData *)Sender->GetNodeData(Node);
	if ((Kind == ikNormal || Kind == ikSelected) && NodeData)
	{
      switch(Column)
            {
              case 4:if(NodeData->StockQuantity < 0)
                        ImageIndex = ICON_EXCLAMATION_INDEX;
                     break;
              case 5:if(NodeData->SupplierUnitCost < 0)
                        ImageIndex = ICON_EXCLAMATION_INDEX;
                     break;
            }

	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdateSupplierCosts::vtvStockItemGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText)
{
	TSupplierItemNodeData *NodeData = (TSupplierItemNodeData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Column)
		{
			case 0:	CellText = NodeData->SupplierCode;
						break;
			case 1:	CellText = NodeData->Description;
						break;
			case 2:	CellText = NodeData->StockUnit;
						break;
			case 3:	CellText = NodeData->OrderUnit;
						break;
			case 4:	CellText = MMMath::FloatString(NodeData->StockQuantity);
						break;
                        case 5:	CellText =  FormatFloat("0.00",NodeData->SupplierUnitCost);  //MMMath::FloatString(NodeData->SupplierUnitCost);
						break;
                        case 6:  CellText =  NodeData->PreferredSupplier ? OptionTrue : OptionFalse;
						break;
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdateSupplierCosts::vtvStockItemKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	if (!vtvStockItem->IsEditing() && vtvStockItem->FocusedNode)
	{
		if (Key == VK_RETURN)
		{
			vtvStockItem->EditNode(vtvStockItem->FocusedNode, vtvStockItem->FocusedColumn);
		}
	}
}
//Key down handler for all ediatable field
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateSupplierCosts::KeyDownHandler(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	if (Key == VK_RETURN || Key == VK_DOWN || Key == VK_UP)
	{
		vtvStockItem->BeginUpdate();
		try
		{
			vtvStockItem->EndEditNode();
		}
		__finally
		{
			vtvStockItem->EndUpdate();
		}
		if (Key == VK_DOWN || (Key == VK_RETURN && vtvStockItem->FocusedColumn == 6))
		{
			if (vtvStockItem->GetNext(vtvStockItem->FocusedNode))
			{
				try
				{
					vtvStockItem->OnEdited = NULL;
					vtvStockItem->FocusedNode = vtvStockItem->GetNext(vtvStockItem->FocusedNode);
				}
				__finally
				{
					vtvStockItem->OnEdited = vtvStockItemEdited;
				}
				vtvStockItem->Selected[vtvStockItem->FocusedNode] = true;
			}
         else
          {
            btnCommit->SetFocus();
          }
		}
      else if (Key == VK_RETURN)
		{
         if(vtvStockItem->FocusedColumn == 0)
          {
			   vtvStockItem->FocusedColumn = 3;
          }
         else if(vtvStockItem->FocusedColumn != 6)
          {
             vtvStockItem->FocusedColumn++;
          }
			vtvStockItem->EditNode(vtvStockItem->FocusedNode, vtvStockItem->FocusedColumn);
		}
		else if (Key == VK_UP)
		{
			if (vtvStockItem->GetPrevious(vtvStockItem->FocusedNode))
			{
				vtvStockItem->FocusedNode = vtvStockItem->GetPrevious(vtvStockItem->FocusedNode);
				vtvStockItem->Selected[vtvStockItem->FocusedNode] = true;
			}
			else
			{
				vtvStockItem->EditNode(vtvStockItem->FocusedNode, vtvStockItem->FocusedColumn);
			}
		}
	}
	else if (Key == VK_ESCAPE)
	{
		vtvStockItem->CancelEditNode();
		vtvStockItem->EditNode(vtvStockItem->FocusedNode, vtvStockItem->FocusedColumn);
	}
}

void __fastcall TfrmUpdateSupplierCosts::btnCancelClick(TObject *Sender)
{
	 Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUpdateSupplierCosts::btnCommitClick(TObject *Sender)
{
  if (vtvStockItem->IsEditing())
   	{
	   	vtvStockItem->EndEditNode();
	   }

    try
       {
        if(!Transaction->InTransaction)
            Transaction->StartTransaction();
        Query->Close();
        Query->SQL->Text = "update SUPPLIERSTOCK  set "
                           "SUPPLIER_CODE =  :SUPPLIER_CODE, "
                           "SUPPLIER_UNIT =  :SUPPLIER_UNIT, "
                           "QTY = :QTY , "
                           "LATEST_COST = :LATEST_COST, "
                           "PREFERRED_SUPPLIER = :PREFERRED_SUPPLIER "
                           "where SUPPLIER_STOCK_KEY = :SUPPLIER_STOCK_KEY ";
        PVirtualNode Node = vtvStockItem->GetFirst();
        TSupplierItemNodeData *NodeData = NULL;
        while (Node)
	        {       NodeData = (TSupplierItemNodeData *)vtvStockItem->GetNodeData(Node);
                        if(NodeData->IsNewlyAdded)
                          {
                            AddSupplierStock(NodeData);
                          }
                        else
                          {
                               Query->ParamByName("SUPPLIER_CODE")->AsString = NodeData->SupplierCode;
                               Query->ParamByName("SUPPLIER_UNIT")->AsString = NodeData->OrderUnit;
                               Query->ParamByName("QTY")->AsCurrency = NodeData->StockQuantity;
                               Query->ParamByName("LATEST_COST")->AsFloat = NodeData->SupplierUnitCost;
                               Query->ParamByName("PREFERRED_SUPPLIER")->AsString = NodeData->PreferredSupplier ? 'T' : 'F';
                               Query->ParamByName("SUPPLIER_STOCK_KEY")->AsInteger = NodeData->SupplierStockKey;
                               Query->ExecQuery();
                               if(NodeData->PreferredSupplier)
                                  UpdateStockData(NodeData);
                          }
                          Node = vtvStockItem->GetNext(Node);
	        }

          Transaction->Commit();
    }
    catch (Exception &e)
    {
        Transaction->Rollback();
    }
    Close();
}


//---------------------------------------------------------------------------
void TfrmUpdateSupplierCosts::UpdateStockData(TSupplierItemNodeData* StockInfo)
{
    PreferredSupplierQuery->Close();
    PreferredSupplierQuery->SQL->Text = "update SUPPLIERSTOCK  set "
                       "PREFERRED_SUPPLIER = 'F' "
                        "where SUPPLIER_KEY != :SUPPLIER_KEY and "
                      " STOCK_KEY = :STOCK_KEY  ";
    PreferredSupplierQuery->ParamByName("SUPPLIER_KEY")->AsInteger = StockInfo->StockKey;
    PreferredSupplierQuery->ParamByName("STOCK_KEY")->AsInteger = StockInfo->SupplierKey;
    PreferredSupplierQuery->ExecQuery();
}
void __fastcall TfrmUpdateSupplierCosts::btnImportClick(TObject *Sender)
{
  if (vtvStockItem->IsEditing())
   	{
	   	vtvStockItem->EndEditNode();
        }
    if (dlgOpen->Execute())
	{
	   AnsiString LastItem;
	   try
            {
		TCsv ImportCSV;
		ImportCSV.Qualifier = '\"';
		ImportCSV.LoadFromFile(dlgOpen->FileName);
                if (ImportCSV.RowCount > 0)
		   {
                        AddedSupplierStocks = new TList;
			for (int i=1; i<ImportCSV.RowCount; i++)
			{
                            PVirtualNode Node = vtvStockItem->GetFirst();
                            TSupplierItemNodeData *NodeData = NULL;
                            bool IsStockExist = false;
                            if (ImportCSV.Cells[0][i] != "")
				                {
                                  IsStockExist = false;
                                  while (Node)
	                               {
                                     NodeData = (TSupplierItemNodeData *)vtvStockItem->GetNodeData(Node);
                                     if(NodeData->StockCode == ImportCSV.Cells[0][i])
                                     {
                                        IsStockExist = true;
                                        NodeData->SupplierCode = ImportCSV.Cells[5][i];
                                        NodeData->OrderUnit = ImportCSV.Cells[6][i];
                                        NodeData->SupplierUnitCost = StrToCurr(ImportCSV.Cells[3][i]);
                                        NodeData->StockQuantity = StrToCurr(ImportCSV.Cells[7][i]);
                                        NodeData->PreferredSupplier = ImportCSV.Cells[4][i] == "TRUE" ? true : false;
                                        break;
                                     }
                                     Node = vtvStockItem->GetNext(Node);
	                              }
                                  if(!IsStockExist)
                                  {
                                     TSupplierItemNodeData *NewSupplierStock = new TSupplierItemNodeData;
                                     NewSupplierStock->StockCode = ImportCSV.Cells[0][i];
                                     NewSupplierStock->SupplierKey = ContactKey;
                                     NewSupplierStock->SupplierCode = ImportCSV.Cells[5][i];
                                     NewSupplierStock->OrderUnit = ImportCSV.Cells[6][i];
                                     NewSupplierStock->SupplierUnitCost = StrToCurr(ImportCSV.Cells[3][i]);
                                     NewSupplierStock->StockQuantity = StrToCurr(ImportCSV.Cells[7][i]);
                                     NewSupplierStock->PreferredSupplier = ImportCSV.Cells[4][i] == "TRUE" ? true : false;
                                     NewSupplierStock->IsNewlyAdded = true;
                                     AddedSupplierStocks->Add(NewSupplierStock);
                                  }
                                }
                        }
                        if(!Transaction->InTransaction)
                           Transaction->StartTransaction();
                        //Add New entries to grid
                        for(int counter = 0 ; counter < AddedSupplierStocks->Count;  counter++)
                        {
                           TSupplierItemNodeData *NewSupplierStock = (TSupplierItemNodeData *)AddedSupplierStocks->Items[counter];
                           if(GetStockDetail(NewSupplierStock))
                           {
                              PVirtualNode OrderItemNode = NULL;
                              TSupplierItemNodeData *NodeData = NULL;
                              OrderItemNode	= vtvStockItem->AddChild(NULL);
                              NodeData = (TSupplierItemNodeData *)vtvStockItem->GetNodeData(OrderItemNode);
                              NodeData->StockKey =NewSupplierStock->StockKey;
                              NodeData->SupplierKey = NewSupplierStock->SupplierKey;
                              NodeData->StockUnit =NewSupplierStock->StockUnit;
                              NodeData->Description = NewSupplierStock->Description;
                              NodeData->SupplierCode = NewSupplierStock->SupplierCode;
                              NodeData->OrderUnit = NewSupplierStock->OrderUnit;
                              NodeData->SupplierUnitCost = NewSupplierStock->SupplierUnitCost;
                              NodeData->StockQuantity = NewSupplierStock->StockQuantity;
                              NodeData->PreferredSupplier = NewSupplierStock->PreferredSupplier;
                              NodeData->IsNewlyAdded = NewSupplierStock->IsNewlyAdded;
                           }
                        }
                        Transaction->Commit();
                        vtvStockItem->Refresh();
                   }
            }
           catch (Exception &E)
            {
                Application->MessageBox("Failed on item " , "Error", MB_ICONERROR + MB_OK);
            }
            vtvStockItem->Refresh();
        }
}

bool TfrmUpdateSupplierCosts::GetStockDetail(TSupplierItemNodeData* NodeData)
{
     bool IsStockExist = false;
     Query->Close();
     Query->SQL->Text = "SELECT a.STOCK_KEY,a.DESCRIPTION, a.STOCKTAKE_UNIT FROM STOCK a "
                        "where a.CODE = :Stock_Code";
     Query->ParamByName("Stock_Code")->AsString = NodeData->StockCode;
     Query->ExecQuery();
     if(!Query->Eof)
     {
        NodeData->StockKey = Query->FieldByName("STOCK_KEY")->AsInteger;
        NodeData->StockUnit = Query->FieldByName("STOCKTAKE_UNIT")->AsString;
        NodeData->Description = Query->FieldByName("DESCRIPTION")->AsString;
        IsStockExist  = true;
     }
     return IsStockExist;
}

void TfrmUpdateSupplierCosts::AddSupplierStock(TSupplierItemNodeData* NodeData)
{
   try
      {
          AddSupplierStockQuery->Close();
          AddSupplierStockQuery->SQL->Text = "SELECT GEN_ID(GEN_SUPPLIERSTOCK, 1) FROM RDB$DATABASE";
          AddSupplierStockQuery->ExecQuery();
          int supplierStockKey = AddSupplierStockQuery->Fields[0]->AsInteger;
          AddSupplierStockQuery->Close();
          AddSupplierStockQuery->SQL->Text = "INSERT INTO SUPPLIERSTOCK ( "
                                             "SUPPLIER_STOCK_KEY, "
                                             "SUPPLIER_KEY, "
                                             "STOCK_KEY, "
                                             "SUPPLIER_UNIT, "
                                             "SUPPLIER_CODE, "
                                             "PREFERRED_SUPPLIER, "
                                             "DEFAULT_ORDER_UNIT, "
                                             "QTY, "
                                             "LATEST_COST, "
                                             "MIN_ORDER_QTY)"
                                             "VALUES ( "
                                             ":SUPPLIER_STOCK_KEY, "
                                             ":SUPPLIER_KEY, "
                                             ":STOCK_KEY, "
                                             ":SUPPLIER_UNIT, "
                                             ":SUPPLIER_CODE, "
                                             ":PREFERRED_SUPPLIER, "
                                             ":DEFAULT_ORDER_UNIT, "
                                             ":QTY, "
                                             ":LATEST_COST, "
                                             ":MIN_ORDER_QTY )";
          AddSupplierStockQuery->ParamByName("SUPPLIER_STOCK_KEY")->AsInteger = supplierStockKey;
          AddSupplierStockQuery->ParamByName("SUPPLIER_KEY")->AsInteger = ContactKey;
          AddSupplierStockQuery->ParamByName("STOCK_KEY")->AsInteger = NodeData->StockKey;
          AddSupplierStockQuery->ParamByName("SUPPLIER_UNIT")->AsString = NodeData->OrderUnit;
          AddSupplierStockQuery->ParamByName("SUPPLIER_CODE")->AsString = NodeData->SupplierCode;
          AddSupplierStockQuery->ParamByName("PREFERRED_SUPPLIER")->AsString = NodeData->PreferredSupplier ? 'T' : 'F';
          AddSupplierStockQuery->ParamByName("DEFAULT_ORDER_UNIT")->AsString = 'T';
          AddSupplierStockQuery->ParamByName("QTY")->AsCurrency = NodeData->StockQuantity;
          AddSupplierStockQuery->ParamByName("LATEST_COST")->AsFloat = NodeData->SupplierUnitCost;
          AddSupplierStockQuery->ParamByName("MIN_ORDER_QTY")->AsCurrency = 1;
          AddSupplierStockQuery->ExecQuery();
          AddSupplierStockQuery->Close();
    }
    catch(Exception &E)
    {

    }      
}


