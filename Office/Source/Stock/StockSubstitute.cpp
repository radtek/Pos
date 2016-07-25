//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StockSubstitute.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TfrmStockSubstitute *frmStockSubstitute;
//---------------------------------------------------------------------------
__fastcall TfrmStockSubstitute::TfrmStockSubstitute(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmStockSubstitute::FormShow(TObject *Sender)
{
         vtvStockSubstitute->Clear();
         vtvStockSubstitute->NodeDataSize	= sizeof(TStockSubstituteData);
	      lbeTitle->Caption = "Select Stock Substitute";
         Substitutes = new TList;
         LoadLocations();
         GetRecipeStocks();
}
//---------------------------------------------------------------------------
void TfrmStockSubstitute::GetStockSubstitute(AnsiString Location)
{
  int stockKey = 0;
  int Counter = 1;
  qrGetSubstituteStock->Close();
    try
       {
           if (!Transaction->InTransaction)
                Transaction->StartTransaction();
           qrGetSubstituteStock->ParamByName("Location")->AsString = Location;
           for (qrGetSubstituteStock->Open(); !qrGetSubstituteStock->Eof; qrGetSubstituteStock->Next())
             {
                TStockSubstituteData *NodeData = new TStockSubstituteData;
                NodeData->SubstituteKey = qrGetSubstituteStock->FieldByName("SUBSTITUTE_KEY")->AsInteger;
                NodeData->StockKey = qrGetSubstituteStock->FieldByName("STOCK_KEY")->AsInteger;
                NodeData->StockCode = qrGetSubstituteStock->FieldByName("CODE")->AsString;
                NodeData->StockName = qrGetSubstituteStock->FieldByName("STOCK_NAME")->AsString;
                NodeData->StockLatestCost = qrGetSubstituteStock->FieldByName("STOCK_COST")->AsFloat;
                NodeData->StockOnHandQty = qrGetSubstituteStock->FieldByName("STOCK_ONHAND")->AsFloat;
                NodeData->StockMinLevel = qrGetSubstituteStock->FieldByName("STOCK_MINLEVEL")->AsFloat;
                NodeData->SubstituteStockKey = qrGetSubstituteStock->FieldByName("SUBSTITUTE_STOCK_KEY")->AsInteger;
                NodeData->SubstituteStockCode = qrGetSubstituteStock->FieldByName("SUBSTITUTE_CODE")->AsString;
                NodeData->SubstituteStockName = qrGetSubstituteStock->FieldByName("SUBSTITUTE_NAME")->AsString;
                NodeData->SubstituteLatestCost = qrGetSubstituteStock->FieldByName("SUBSTITUTE_COST")->AsFloat;
                NodeData->SubstituteOnHandQty = qrGetSubstituteStock->FieldByName("SUBSTITUTE_ONHAND")->AsFloat;
                NodeData->SubstituteMinLevel = qrGetSubstituteStock->FieldByName("SUBSTITUTE_MINLEVEL")->AsFloat;
                NodeData->Condition1 = qrGetSubstituteStock->FieldByName("SUBSTITUTE_COND1")->AsInteger;
                NodeData->Condition2 = qrGetSubstituteStock->FieldByName("SUBSTITUTE_COND2")->AsInteger;
                NodeData->Variance = qrGetSubstituteStock->FieldByName("SUBSTITUTE_COND3")->AsInteger;
                NodeData->RevertToOriginal = qrGetSubstituteStock->FieldByName("REVERT_TO_ORIGINAL")->AsString == 'T' ? true : false;
                NodeData->IsSelected = false;
                NodeData->IsDisplay = CheckSubstitute(NodeData);
                Substitutes->Add(NodeData);
                if(stockKey == 0 || stockKey != NodeData->StockKey)
                  {
                    stockKey = NodeData->StockKey;
                    Counter = 1;
                  }
                NodeData->SubstituteNumber = Counter;
                Counter ++;
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
//---------------------------------------------------------------------------
void TfrmStockSubstitute::DisplayStockSubstitute()
{
   int stockKey = 0;
   for(int i = 0; i< Substitutes->Count ; i++)
    {
       TStockSubstituteData *NodeData = (TStockSubstituteData *)Substitutes->Items[i];
       if((stockKey == 0 || stockKey != NodeData->StockKey) && NodeData->IsDisplay)
        {
             stockKey = NodeData->StockKey;
             DisplayStockSubstituteItem(NodeData);
        }
    }
}
//---------------------------------------------------------------------------
void TfrmStockSubstitute::DisplayStockSubstituteItem(TStockSubstituteData *SubstituteData)
{
    PVirtualNode RequestItemNode = NULL;
    TStockSubstituteData *NodeData = NULL;
    RequestItemNode	= vtvStockSubstitute->AddChild(NULL);
    NodeData = (TStockSubstituteData *)vtvStockSubstitute->GetNodeData(RequestItemNode);
    CloneSubstituteData(SubstituteData,NodeData);
    RequestItemNode->CheckState = csUncheckedNormal;
    RequestItemNode->CheckType = ctCheckBox;
}
//---------------------------------------------------------------------------
void TfrmStockSubstitute::CloneSubstituteData(TStockSubstituteData *Source,TStockSubstituteData *Destination)
{
    Destination->SubstituteKey = Source->SubstituteKey;
    Destination->StockKey = Source->StockKey;
    Destination->StockCode = Source->StockCode;
    Destination->StockName = Source->StockName;
    Destination->StockLatestCost = Source->StockLatestCost;
    Destination->StockOnHandQty = Source->StockOnHandQty ;
    Destination->StockMinLevel = Source->StockMinLevel ;
    Destination->SubstituteStockKey = Source->SubstituteStockKey;
    Destination->SubstituteStockCode = Source->SubstituteStockCode;
    Destination->SubstituteStockName = Source->SubstituteStockName ;
    Destination->SubstituteLatestCost = Source->SubstituteLatestCost ;
    Destination->SubstituteOnHandQty = Source->SubstituteOnHandQty;
    Destination->SubstituteMinLevel = Source->SubstituteMinLevel ;
    Destination->Condition1 = Source->Condition1 ;
    Destination->Condition2 = Source->Condition2 ;
    Destination->Variance = Source->Variance ;
    Destination->RevertToOriginal = Source->RevertToOriginal;
    Destination->IsSelected = Source->IsSelected;
    Destination->IsDisplay = Source->IsDisplay;
    Destination->SubstituteNumber = Source->SubstituteNumber;
}
//---------------------------------------------------------------------------
bool TfrmStockSubstitute::CheckSubstitute(TStockSubstituteData *SubstituteData)
{
   bool retValue = false;
   if(RecipeStocks->IndexOf(SubstituteData->StockCode) == -1)
    {
      return false;
    }

   if((SubstituteData->Condition1 == 0 &&  SubstituteData->StockOnHandQty <  SubstituteData->StockMinLevel)  ||
      (SubstituteData->Condition1 == 1 &&  SubstituteData->StockOnHandQty <=  0))
     {
        retValue = true;
     }
   else
     {
        return false;
     }

   if((SubstituteData->Condition2 == 0 &&  SubstituteData->SubstituteOnHandQty >  SubstituteData->SubstituteMinLevel)  ||
      (SubstituteData->Condition2 == 1 &&  SubstituteData->SubstituteOnHandQty >  0))
     {
        retValue = retValue && true;
     }
   else
     {
       return false;
     }

   Currency costVariance = 0;
   if(SubstituteData->StockLatestCost > 0)
    {
      costVariance = (SubstituteData->StockLatestCost -  SubstituteData->SubstituteLatestCost)/SubstituteData->StockLatestCost;
      if(costVariance < 0)
        {
           costVariance = costVariance * -1;
        }
    }
   return retValue &&  costVariance <= SubstituteData->Variance;
}

//---------------------------------------------------------------------------
void __fastcall TfrmStockSubstitute::vtvStockSubstituteAfterItemPaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas, PVirtualNode Node,
      TRect &ItemRect)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockSubstitute::vtvStockSubstituteAfterPaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockSubstitute::vtvStockSubstituteGetText(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      TVSTTextType TextType, WideString &CellText)
{
     TStockSubstituteData *NodeData = (TStockSubstituteData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Column)
		{
			case 0:	CellText = NodeData->StockName;
						break;
			case 1:	CellText = CurrToStr(NodeData->StockOnHandQty);
						break;
			case 2:	CellText = NodeData->StockLatestCost;
						break;
			case 3:	CellText = NodeData->SubstituteStockName;
						break;
         case 4:	CellText = NodeData->SubstituteOnHandQty;
						break;
         case 5:	CellText = NodeData->SubstituteLatestCost;
                  break;
         case 6:	CellText = NodeData->SubstituteNumber;
                  break;
         case 7:	CellText = NodeData->Variance;
                  break;
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmStockSubstitute::vtvStockSubstituteChecked(
      TBaseVirtualTree *Sender, PVirtualNode Node)
{
    TStockSubstituteData *NodeData = (TStockSubstituteData *)Sender->GetNodeData(Node);

    if(Node->CheckState == csCheckedNormal)
        NodeData->IsSelected = true;
    else if(Node->CheckState == csUncheckedNormal)
        NodeData->IsSelected = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockSubstitute::vtvStockSubstituteCreateEditor(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      IVTEditLink *EditLink)
{
	if (Node && Column == 6)
	{
		TStockSubstituteData *NodeData = (TStockSubstituteData *)Sender->GetNodeData(Node);
      LoadNumbers(NodeData);
		cbSubstituteNumber->ItemIndex = cbSubstituteNumber->Items->IndexOf(IntToStr(NodeData->SubstituteNumber));
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, cbSubstituteNumber);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(cbSubstituteNumber->Handle, EM_SETSEL, 0, -1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockSubstitute::vtvStockSubstituteEdited(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	if (vtvStockSubstitute->FocusedNode)
	{
		if (Column == 6)
		{
			TStockSubstituteData *NodeData	= (TStockSubstituteData *)vtvStockSubstitute->GetNodeData(vtvStockSubstitute->FocusedNode);
			NodeData->SubstituteNumber	= StrToInt(cbSubstituteNumber->Items->Strings[cbSubstituteNumber->ItemIndex]);
		}
		vtvStockSubstitute->InvalidateNode(vtvStockSubstitute->FocusedNode);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockSubstitute::vtvStockSubstituteEditing(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      bool &Allowed)
{
  Allowed = (Column == 6);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockSubstitute::vtvStockSubstituteFocusChanged(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
  	if (Node && !Sender->IsEditing())
	{
		if (Sender->FocusedColumn != 6)
		{
			Sender->FocusedColumn = 6;
		}
		Sender->EditNode(Node, Column);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockSubstitute::vtvStockSubstituteFocusChanging(
      TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
      TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockSubstitute::btnCancelClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void TfrmStockSubstitute::UpdateSubstituteStocks()
{
  Database::TDBTransaction transaction(dmMMData->dbMenuMate);
 try
   {

        TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
        transaction.StartTransaction();
        IBInternalQuery->SQL->Text =   "UPDATE RECIPE a "
                                       "Set a.STOCK_CODE = :NEWSTOCKCODE "
                                       "where a.STOCK_CODE = :OLDSTOCKCODE and a.STOCK_LOCATION = :Location";
        IBInternalQuery->Close();
        PVirtualNode Node = vtvStockSubstitute->GetFirst();
        TStockSubstituteData *NodeData;
        while(Node)
          {
               NodeData = (TStockSubstituteData *)vtvStockSubstitute->GetNodeData(Node);
               IBInternalQuery->ParamByName("OLDSTOCKCODE")->AsString = NodeData->StockCode;
               IBInternalQuery->ParamByName("NEWSTOCKCODE")->AsString = NodeData->SubstituteStockCode;
               IBInternalQuery->ParamByName("Location")->AsString = Location;
               IBInternalQuery->ExecQuery();
               IBInternalQuery->Close();
               Node = vtvStockSubstitute->GetNext(Node);
          }
        transaction.Commit();
    }
    catch (Exception &e)
    {
        transaction.Rollback();
    }
    transaction.Commit();
}
//---------------------------------------------------------------------------

void TfrmStockSubstitute::UpdateToOriginalStock()
{
 Database::TDBTransaction transaction(dmMMData->dbMenuMate);
 int stockKey = 0;
 try
  {
     TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
     transaction.StartTransaction();
     IBInternalQuery->Close();
     IBInternalQuery->SQL->Text =   "UPDATE RECIPE a "
                                    "Set a.STOCK_CODE = a.ORGINAL_STOCK_CODE "
                                    "where a.ORGINAL_STOCK_CODE = :ORGINAL_STOCK_CODE and a.STOCK_LOCATION = :Location ";
      for(int i = 0; i< Substitutes->Count ; i++)
       {
         TStockSubstituteData *NodeData = (TStockSubstituteData *)Substitutes->Items[i];
         if((stockKey == 0 || stockKey != NodeData->StockKey) &&   NodeData->RevertToOriginal && !NodeData->IsSelected
             && NodeData->StockOnHandQty > NodeData->StockMinLevel)
          {
            stockKey = NodeData->StockKey;
            IBInternalQuery->ParamByName("ORGINAL_STOCK_CODE")->AsString = NodeData->StockCode;
            IBInternalQuery->ParamByName("Location")->AsString = Location;
            IBInternalQuery->ExecQuery();
            IBInternalQuery->Close();
          }
       }
       transaction.Commit();
  }
 catch (Exception &e)
 {
     transaction.Rollback();
 }
 transaction.Commit();
}

//---------------------------------------------------------------------------
void __fastcall TfrmStockSubstitute::btnNextClick(TObject *Sender)
{
  if (vtvStockSubstitute->IsEditing())
	{
		vtvStockSubstitute->EndEditNode();
	}
   UpdateSubstituteStocks();
   if(cbRevertToOriginal->Checked)
    {
      UpdateToOriginalStock();
    }
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStockSubstitute::cbStockLocationChange(TObject *Sender)
{
  Substitutes->Clear();
  vtvStockSubstitute->Clear();
   Location = cbStockLocation->Items->Strings[cbStockLocation->ItemIndex];
  GetStockSubstitute(Location);
  DisplayStockSubstitute();
}
//---------------------------------------------------------------------------
void TfrmStockSubstitute::LoadLocations()
{
  cbStockLocation->Items->Clear();
  try
   {
     if(!Transaction->InTransaction)
       Transaction->StartTransaction();
      Query->Close();
      Query->SQL->Text = "SELECT DISTINCT SL.LOCATION "
                         "FROM STOCKLOCATION SL "
                         "ORDER BY SL.LOCATION  ";
      Query->ExecQuery();
      while (!Query->Eof)
         {
            cbStockLocation->Items->Add(Query->FieldByName("LOCATION")->AsString);
            Query->Next();
         }
      Query->Close();
      Transaction->Commit();
    }
    catch (Exception &e)
    {
        Transaction->Rollback();
    }

    if (Transaction->InTransaction)
        Transaction->Commit();
}
//---------------------------------------------------------------------------
void TfrmStockSubstitute::LoadNumbers(TStockSubstituteData *SubstituteData)
{
   cbSubstituteNumber->Clear();
   for(int i = 0; i< Substitutes->Count ; i++)
    {
       TStockSubstituteData *NodeData = (TStockSubstituteData *)Substitutes->Items[i];
       if(NodeData->StockKey == SubstituteData->StockKey && NodeData->IsDisplay)
         {
           cbSubstituteNumber->Items->Add(IntToStr(NodeData->SubstituteNumber));
         }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmStockSubstitute::cbSubstituteNumberChange(
      TObject *Sender)
{
	vtvStockSubstitute->BeginUpdate();
	try
	{
		vtvStockSubstitute->EndEditNode();
		TStockSubstituteData *NodeData	= (TStockSubstituteData *)vtvStockSubstitute->GetNodeData(vtvStockSubstitute->FocusedNode);
      int substituteNumber	= StrToInt(cbSubstituteNumber->Items->Strings[cbSubstituteNumber->ItemIndex]);
      for(int i = 0; i< Substitutes->Count ; i++)
       {
          TStockSubstituteData *SubstituteData = (TStockSubstituteData *)Substitutes->Items[i];
          if(NodeData->StockKey == SubstituteData->StockKey && SubstituteData->SubstituteNumber == substituteNumber)
            {
              CloneSubstituteData(SubstituteData,NodeData);
              break;
            }
       }

	}
	__finally
	{
		vtvStockSubstitute->EndUpdate();
	}
}
//---------------------------------------------------------------------------
void TfrmStockSubstitute::GetRecipeStocks()
{
  RecipeStocks = new TStringList;
  Database::TDBTransaction transaction(dmMMData->dbMenuMate);
  try
   {

        TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
        transaction.StartTransaction();
        IBInternalQuery->SQL->Text =   "SELECT distinct STOCK_CODE FROM RECIPE";
        IBInternalQuery->Close();
        IBInternalQuery->ExecQuery();
        while(!IBInternalQuery->Eof)
          {
            RecipeStocks->Add(IBInternalQuery->FieldByName("STOCK_CODE")->AsString);
            IBInternalQuery->Next();
          }
        transaction.Commit();
    }
    catch (Exception &e)
    {
        transaction.Rollback();
    }
    transaction.Commit();
}

//---------------------------------------------------------------------------


