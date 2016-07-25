//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectSupplierStockItem.h"
#include "SelectStockItem.h"
#include "MMData.h"
#include "Utilities.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define MAX_STOCK_ITEMS_IN_EVALUATION_MODE 20

int TfrmSelectSupplierStockItem::LastStockKey = 0;

//TfrmSelectSupplierStockItem *frmSelectSupplierStockItem;
//---------------------------------------------------------------------------
__fastcall TfrmSelectSupplierStockItem::TfrmSelectSupplierStockItem(TComponent* Owner)
	: TForm(Owner)
{
	dmMMData->Registered(&Registered);
  //	LastStockKey = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockItem::FormShow(TObject *Sender)
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	LoadTree();
	tvStock->SetFocus();
	if (tvStock->Items->Count == 0)
	{
		PostMessage(btnOther->Handle, BM_CLICK, 0, 0);
	}
}
//---------------------------------------------------------------------------
void TfrmSelectSupplierStockItem::LoadTree()
{
	tvStock->Items->BeginUpdate();
	tvStock->OnChange = NULL;
	tvStock->Items->Clear();

	TTreeNode *CategoryNode	= NULL;
	TTreeNode *GroupNode		= NULL;
	TTreeNode *StockNode		= NULL;
	TTreeNode *SelectNode	= NULL;

	int StockItemCount = 1;

	int LastCategoryKey, LastGroupKey;

    //qrLocationSupplierStock
if(Location=="")
{
	qrSupplierStock->ParamByName("Supplier_Key")->AsString = CompanyKey;
	for (qrSupplierStock->Open(); !qrSupplierStock->Eof; qrSupplierStock->Next())
	{
		if (qrSupplierStock->FieldByName("Stock_Category_Key")->AsInteger != LastCategoryKey)
		{
			CategoryNode						= tvStock->Items->Add(CategoryNode, qrSupplierStock->FieldByName("Stock_Category")->AsString);
			CategoryNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
			CategoryNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
			CategoryNode->Data				= (void *)qrSupplierStock->FieldByName("Stock_Category_Key")->AsInteger;

			LastCategoryKey 					= qrSupplierStock->FieldByName("Stock_Category_Key")->AsInteger;
		}
		if (qrSupplierStock->FieldByName("Stock_Group_Key")->AsInteger != LastGroupKey)
		{
			GroupNode							= tvStock->Items->AddChild(CategoryNode, qrSupplierStock->FieldByName("Stock_Group")->AsString);
			GroupNode->ImageIndex			= ICON_CLOSED_FOLDER_INDEX;
			GroupNode->SelectedIndex		= ICON_OPEN_FOLDER_INDEX;
			GroupNode->Data					= (void *)qrSupplierStock->FieldByName("Stock_Group_Key")->AsInteger;

			LastGroupKey 						= qrSupplierStock->FieldByName("Stock_Group_Key")->AsInteger;
		}
		StockNode								= tvStock->Items->AddChild(GroupNode, qrSupplierStock->FieldByName("Description")->AsString);
		StockNode->ImageIndex				= ICON_BOX_INDEX;
		StockNode->SelectedIndex			= ICON_BOX_INDEX;
		StockNode->Data						= (void *)qrSupplierStock->FieldByName("Stock_Key")->AsInteger;
		if (LastStockKey == qrSupplierStock->FieldByName("Stock_Key")->AsInteger)
		{
			// Last time an invoice had an item added to it!
			SelectNode = StockNode;
		}
		StockItemCount++;

		if (!Registered)
		{
			if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
			{
				Application->MessageBox(AnsiString("You are limited to " + IntToStr(MAX_STOCK_ITEMS_IN_EVALUATION_MODE) + " items in the evaluation version.").c_str(), "Error", MB_ICONERROR + MB_OK);
				break;
			}
		}
	}
	}
	else
	{
    qrLocationSupplierStock->ParamByName("Supplier_Key")->AsString = CompanyKey;
      qrLocationSupplierStock->ParamByName("Location")->AsString = Location;
	for (qrLocationSupplierStock->Open(); !qrLocationSupplierStock->Eof; qrLocationSupplierStock->Next())
	{
		if (qrLocationSupplierStock->FieldByName("Stock_Category_Key")->AsInteger != LastCategoryKey)
		{
			CategoryNode						= tvStock->Items->Add(CategoryNode, qrLocationSupplierStock->FieldByName("Stock_Category")->AsString);
			CategoryNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
			CategoryNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
			CategoryNode->Data				= (void *)qrLocationSupplierStock->FieldByName("Stock_Category_Key")->AsInteger;

			LastCategoryKey 					= qrLocationSupplierStock->FieldByName("Stock_Category_Key")->AsInteger;
		}
		if (qrLocationSupplierStock->FieldByName("Stock_Group_Key")->AsInteger != LastGroupKey)
		{
			GroupNode							= tvStock->Items->AddChild(CategoryNode, qrLocationSupplierStock->FieldByName("Stock_Group")->AsString);
			GroupNode->ImageIndex			= ICON_CLOSED_FOLDER_INDEX;
			GroupNode->SelectedIndex		= ICON_OPEN_FOLDER_INDEX;
			GroupNode->Data					= (void *)qrLocationSupplierStock->FieldByName("Stock_Group_Key")->AsInteger;

			LastGroupKey 						= qrLocationSupplierStock->FieldByName("Stock_Group_Key")->AsInteger;
		}
		StockNode								= tvStock->Items->AddChild(GroupNode, qrLocationSupplierStock->FieldByName("Description")->AsString);
		StockNode->ImageIndex				= ICON_BOX_INDEX;
		StockNode->SelectedIndex			= ICON_BOX_INDEX;
		StockNode->Data						= (void *)qrLocationSupplierStock->FieldByName("Stock_Key")->AsInteger;
		if (LastStockKey == qrLocationSupplierStock->FieldByName("Stock_Key")->AsInteger)
		{
			// Last time an invoice had an item added to it!
			SelectNode = StockNode;
		}
		StockItemCount++;

		if (!Registered)
		{
			if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
			{
				Application->MessageBox(AnsiString("You are limited to " + IntToStr(MAX_STOCK_ITEMS_IN_EVALUATION_MODE) + " items in the evaluation version.").c_str(), "Error", MB_ICONERROR + MB_OK);
				break;
			}
		}
	}
	
	}
	tvStock->Items->EndUpdate();
	tvStock->OnChange = tvStockChange;
	if (SelectNode)
	{
		tvStock->Selected = SelectNode;
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockItem::btnCloseClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockItem::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
	FindDialog->CloseDialog();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockItem::btnSelectClick(TObject *Sender)
{
 if(Location=="")
{

	if (tvStock->Selected && tvStock->Selected->Level == 2 && qrSupplierStock->FieldByName("Stock_Key")->AsInteger == int(tvStock->Selected->Data))
	{
		StockCode		= qrSupplierStock->FieldByName("Code")->AsString;
		LastStockKey	= qrSupplierStock->FieldByName("Stock_Key")->AsInteger;
		ModalResult		= mrOk;
	}
  }
  else
  {
     	if (tvStock->Selected && tvStock->Selected->Level == 2 && qrLocationSupplierStock->FieldByName("Stock_Key")->AsInteger == int(tvStock->Selected->Data))
	{
		StockCode		= qrLocationSupplierStock->FieldByName("Code")->AsString;
		LastStockKey	= qrLocationSupplierStock->FieldByName("Stock_Key")->AsInteger;
		ModalResult		= mrOk;
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockItem::btnOtherClick(TObject *Sender)
{
	frmSelectStockItem->Reset = true;
	if (frmSelectStockItem->ShowModal() == mrOk)
	{
      StockCode = frmSelectStockItem->StockCode;
      ModalResult = mrOk;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockItem::DBGrid1DblClick(
      TObject *Sender)
{
   btnSelectClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockItem::tvStockChange(TObject *Sender,
      TTreeNode *Node)
{
	if (Node && Node->Level == 2)
	{
		qrSupplierStock->Close();
		qrSupplierStock->Open();

		Variant Key[3];
		Key[0] = int(Node->Parent->Parent->Data);
		Key[1] = int(Node->Parent->Data);
		Key[2] = int(Node->Data);
	qrSupplierStock->Locate("Stock_Category_Key;Stock_Group_Key;Stock_Key", VarArrayOf(Key, 2), TLocateOptions());        
if(Location=="")
        {
             qrSupplierStock->Locate("Stock_Category_Key;Stock_Group_Key;Stock_Key", VarArrayOf(Key, 2), TLocateOptions());
        }
        else
          qrLocationSupplierStock->Locate("Stock_Category_Key;Stock_Group_Key;Stock_Key", VarArrayOf(Key, 2), TLocateOptions());
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockItem::tvStockDblClick(
      TObject *Sender)
{
	btnSelectClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockItem::btnFindClick(TObject *Sender)
{
	tvStock->Selected = NULL;
	FindDialog->Position = Point(Left + Width, Top);
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectSupplierStockItem::FindDialogFind(
      TObject *Sender)
{
	TTreeNode *CurItem;
	if (tvStock->Selected)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = tvStock->Selected->GetNext();
		}
		else
		{
			CurItem = tvStock->Selected->GetPrev();
		}
	}
	else
	{
		CurItem = tvStock->Items->GetFirstNode();
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
				tvStock->Selected = CurItem;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				tvStock->Selected = CurItem;
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

