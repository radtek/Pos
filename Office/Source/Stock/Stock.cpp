//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Stock.h"
#include "StockData.h"
#include "MMData.h"
#include "Utilities.h"
#include <list>
#include <exception>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define MAX_STOCK_ITEMS_IN_EVALUATION_MODE 20


#include <vector>
#include <set>
#include <stdint.h>

TfrmStock *frmStock;

void
TfrmStock::RemoveCategory(
TIBQuery *query,
TIBTransaction *transaction,
unsigned long long category_key,
unsigned long long group_key,
unsigned long long item_key)
{
	query->Close();
	query->SQL->Text =
	"select count(*) remaining_groups "
	"       from stockgroup "
	"            where stock_category_key = :category_key;";
	query->ParamByName("category_key")->AsInteger = category_key;
	query->Open();

	if (query->FieldByName("remaining_groups")->AsInteger)
	return;

	query->SQL->Text =
	"delete from stockcategory "
	"             where stock_category_key = :category_key;";
	query->ParamByName("category_key")->AsInteger = category_key;
	query->ExecSQL();
}

void
TfrmStock::RemoveGroup(
TIBQuery *query,
TIBTransaction *transaction,
unsigned long long category_key,
unsigned long long group_key,
unsigned long long item_key)
{
	query->Close();
	query->SQL->Text =
	"select count(*) remaining_items "
	"       from stock where stock_group_key = :group_key;";
	query->ParamByName("group_key")->AsInteger = group_key;
	query->Open();

	if (query->FieldByName("remaining_items")->AsInteger)
	return;

	query->Close();
	query->SQL->Text = "delete from stockgroup "
	"       where stock_group_key = :group_key;";
	query->ParamByName("group_key")->AsInteger = group_key;
	query->ExecSQL();

	RemoveCategory(query, transaction, category_key, 0, 0);
}

void
TfrmStock::RemoveItem(
TIBQuery *query,
TIBTransaction *transaction,
unsigned long long category_key,
unsigned long long group_key,
unsigned long long item_key)
{
	query->Close();
	query->SQL->Text =
	"delete from stock where stock_key = :stock_key;";
	query->ParamByName("stock_key")->AsInteger = item_key;
	query->ExecSQL();

	RemoveGroup(query, transaction, category_key, group_key, 0);
}

enum RemovalClass {
	RcCategory,
	RcGroup,
	RcItem
};

void
TfrmStock::RemoveKickstart(
unsigned char removal_class,
unsigned long long category_key,
unsigned long long group_key,
unsigned long long item_key)
{
	static void (TfrmStock::* funtab[3])(TIBQuery *,
	TIBTransaction *,
	unsigned long long,
	unsigned long long,
	unsigned long long) = {
		&(TfrmStock::RemoveCategory),
		&(TfrmStock::RemoveGroup),
		&(TfrmStock::RemoveItem)
	};

	std::auto_ptr<TIBQuery> query(new TIBQuery(NULL));
	TIBTransaction *stock_transaction = dmStockData->trStock;
	bool initial_transaction_state = stock_transaction->Active;

	query->Database = dmStockData->dbStock;
	query->Transaction = stock_transaction;

	try {
		if (!initial_transaction_state)
		stock_transaction->StartTransaction();
		(*this.*funtab[removal_class])(query.get(),
		stock_transaction,
		category_key,
		group_key,
		item_key);
		stock_transaction->Commit();
	} catch (Exception &e) {
		stock_transaction->Rollback();
	}

	if (initial_transaction_state)
	stock_transaction->StartTransaction();
}

void
TfrmStock::RemoveRelatedRecipes()
{
	std::auto_ptr<TIBQuery> query(new TIBQuery(NULL));
	TIBTransaction *stock_transaction = dmStockData->trStock;
	bool stock_initial_state = stock_transaction->Active;
	AnsiString temp;

	query->Database = dmStockData->dbStock;
	query->Transaction = stock_transaction;

	try {
		if (!stock_initial_state)
		stock_transaction->StartTransaction();

		query->SQL->Text = "select code from stock where deleted = 'T';";
		for (query->Open(); !query->Eof; query->Next()) {
			temp = query->FieldByName("code")->AsString;
			RemoveRelatedRecipes(temp);
		}

		stock_transaction->Commit();
	} catch (Exception &e) {
		stock_transaction->Rollback();
	}

	if (stock_initial_state)
	stock_transaction->Commit();
}

void
TfrmStock::RemoveRelatedRecipes(
AnsiString &stock_code)
{
	TIBTransaction *menumate_transaction = dmMMData->trMenuMate;
	bool menumate_initial_state = menumate_transaction->Active;
	std::auto_ptr<TIBQuery> query(new TIBQuery(NULL));

	query->Database = dmMMData->dbMenuMate;
	query->Transaction = menumate_transaction;
	query->SQL->Text = "delete from recipe where stock_code = :code";
	query->ParamByName("code")->AsString = stock_code;

	try {
		if (!menumate_initial_state)
		menumate_transaction->StartTransaction();

		query->ExecSQL();

		menumate_transaction->Commit();
	} catch (Exception &e) {
		menumate_transaction->Rollback();
	}

	if (menumate_initial_state)
	menumate_transaction->StartTransaction();
}

//---------------------------------------------------------------------------
__fastcall TfrmStock::TfrmStock(TComponent* Owner)
: TForm(Owner),
frmAddStock(new TfrmAddStock(NULL)),
frmAddStockCategoryGroup(new TfrmAddStockCategoryGroup(NULL)),
ignorePendingDeletions(true)
{
	DoubleBuffered = true;
	lbStockItems->DoubleBuffered = true;
	Panel9->DoubleBuffered = true;
	sgLocations->DoubleBuffered = true;
	sgSuppliers->DoubleBuffered = true;
	dmMMData->Registered(&Registered);
	DeletedNode = NULL;
	StockItemIconIndex = ICON_BOX_INDEX;

}

//---------------------------------------------------------------------------
void __fastcall TfrmStock::WMRefreshStockGroup(TMessage& Message)
{
	RefreshStockGroup();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::FormShow(TObject *Sender)
{
	tvStock->Items->Clear();
	lbStockGroups->Items->Clear();
	lbStockItems->Items->Clear();
	//	InitCategories();
	if (StockMode == smSelect)
	{
		btnOk->Visible				= true;
		btnCancel->Visible		= true;
		btnClose->Visible			= false;
	}
	else if (StockMode == smEdit)
	{
		btnOk->Visible				= false;
		btnCancel->Visible		= false;
		btnClose->Visible			= true;
	}

	if(Location == NULL || Location == "")
	{
		LoadTree();
	}
	else
	{
		LoadTreeforStockRequest();
	}
	SelectInTree(LocateInTree(StockCategoryKey, StockGroupKey, StockKey, StockSelectedDeleted));
	tvStock->SetFocus();
}
//---------------------------------------------------------------------------
void TfrmStock::LoadTree()
{
	tvStock->Items->BeginUpdate();
	tvStock->OnChange = NULL;
	tvStock->Items->Clear();

	DeletedNode								= NULL;

	int StockItemCount					= 1;
	bool ShowRegWarning					= false;

	TTreeNode *CategoryNode				= NULL;
	TTreeNode *GroupNode					= NULL;
	TTreeNode *StockNode					= NULL;

	int LastCategoryKey					= 0;
	int LastGroupKey						= 0;

	DeletedNode								= tvStock->Items->Add(NULL, "Deleted Items");
	DeletedNode->ImageIndex				= ICON_RECYCLE_FOLDER_INDEX;
	DeletedNode->SelectedIndex			= ICON_RECYCLE_FOLDER_INDEX;
	TStockNodeData *DeleteBinData		= new TStockNodeData(snDeletedBin);
	DeletedNode->Data						= DeleteBinData;

	RefreshDeletedBin();

	if (!Transaction->InTransaction) Transaction->StartTransaction();

	qrStockList->Close();
	for (qrStockList->ExecQuery(); !qrStockList->Eof; qrStockList->Next())
	{
		if (!CategoryNode || LastCategoryKey != qrStockList->FieldByName("Stock_Category_Key")->AsInteger)
		{
			if (qrStockList->FieldByName("Category_Deleted")->AsString == "F" || qrStockList->FieldByName("Category_Deleted")->IsNull)
			{
				CategoryNode						= tvStock->Items->Add(NULL, qrStockList->FieldByName("Stock_Category")->AsString);

				CategoryNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
				CategoryNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;

				LastCategoryKey					= qrStockList->FieldByName("Stock_Category_Key")->AsInteger;

				CreateStockCategory(CategoryNode,// qrStockList, false);
				qrStockList->FieldByName("Stock_Category")->AsString,
				LastCategoryKey,
				qrStockList->FieldByName("Category_GL_Code")->AsString,
				false);
			}
		}
		if ((!GroupNode || LastGroupKey != qrStockList->FieldByName("Stock_Group_Key")->AsInteger) &&
				!qrStockList->FieldByName("Stock_Group_Key")->IsNull)
		{
			if ((qrStockList->FieldByName("Group_Deleted")->AsString == "F" || qrStockList->FieldByName("Group_Deleted")->IsNull) &&
					(qrStockList->FieldByName("Category_Deleted")->AsString == "F" || qrStockList->FieldByName("Category_Deleted")->IsNull))
			{
				GroupNode							= tvStock->Items->AddChild(CategoryNode, qrStockList->FieldByName("Stock_Group")->AsString);
				GroupNode->ImageIndex			= ICON_CLOSED_FOLDER_INDEX;
				GroupNode->SelectedIndex		= ICON_OPEN_FOLDER_INDEX;

				LastGroupKey						= qrStockList->FieldByName("Stock_Group_Key")->AsInteger;

				CreateStockGroup(GroupNode,// qrStockList, false);
				qrStockList->FieldByName("Stock_Category")->AsString,
				qrStockList->FieldByName("Stock_Category_Key")->AsInteger,
				qrStockList->FieldByName("Group_GL_Code")->AsString,
				qrStockList->FieldByName("Stock_Group")->AsString,
				qrStockList->FieldByName("Stock_Group_Key")->AsInteger,
				false);
			}
		}
		if (!Registered)
		{
			if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
			{
				ShowRegWarning = true;
				break;
			}
		}
		if (!qrStockList->FieldByName("Stock_Key")->IsNull)
		{
			if ((qrStockList->FieldByName("Group_Deleted")->AsString == "F" || qrStockList->FieldByName("Group_Deleted")->IsNull) &&
					(qrStockList->FieldByName("Category_Deleted")->AsString == "F" || qrStockList->FieldByName("Category_Deleted")->IsNull) &&
					(qrStockList->FieldByName("Item_Deleted")->AsString == "F" || qrStockList->FieldByName("Item_Deleted")->IsNull))
			{
				StockNode							= tvStock->Items->AddChild(GroupNode, qrStockList->FieldByName("Description")->AsString);
				// Check to see if any of the Stock Codes have been initialised for Stocktake
				if (qrStockList->FieldByName("Initialised")->AsString == "T")//StockInStocktake(qrStockList->FieldByName("Stock_Key")->AsInteger))
				{
					StockNode->ImageIndex			= ICON_EXCLAMATION_RED_INDEX;
					StockNode->SelectedIndex		= ICON_EXCLAMATION_RED_INDEX;
				}
				else
				{
					StockNode->ImageIndex			= ICON_BOX_INDEX;
					StockNode->SelectedIndex		= ICON_BOX_INDEX;
				}
				CreateStockItem(StockNode,
				qrStockList->FieldByName("Description")->AsString,
				qrStockList->FieldByName("Stock_Key")->AsInteger,
				qrStockList->FieldByName("Code")->AsString,
				qrStockList->FieldByName("Stocktake_Unit")->AsString,
				qrStockList->FieldByName("Stock_Category")->AsString,
				qrStockList->FieldByName("Stock_Category_Key")->AsInteger,
				qrStockList->FieldByName("Stock_Group")->AsString,
				qrStockList->FieldByName("Stock_Group_Key")->AsInteger,
				false);

				StockItemCount++;
				if (!Registered)
				{
					if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
					{
						ShowRegWarning = true;
						break;
					}
				}
			}
		}
	}
	if (ShowRegWarning)
	{
		Application->MessageBox(AnsiString("You are limited to " + IntToStr(MAX_STOCK_ITEMS_IN_EVALUATION_MODE) + " items in the evaluation version.").c_str(), "Error", MB_ICONERROR + MB_OK);
	}
	Transaction->Commit();
	tvStock->Items->EndUpdate();
	tvStock->OnChange = tvStockChange;
}

void TfrmStock::LoadTreeforStockRequest()
{
	tvStock->Items->BeginUpdate();
	tvStock->OnChange = NULL;
	tvStock->Items->Clear();

	DeletedNode								= NULL;

	int StockItemCount					= 1;
	bool ShowRegWarning					= false;

	TTreeNode *CategoryNode				= NULL;
	TTreeNode *GroupNode					= NULL;
	TTreeNode *StockNode					= NULL;

	int LastCategoryKey					= 0;
	int LastGroupKey						= 0;

	DeletedNode								= tvStock->Items->Add(NULL, "Deleted Items");
	DeletedNode->ImageIndex				= ICON_RECYCLE_FOLDER_INDEX;
	DeletedNode->SelectedIndex			= ICON_RECYCLE_FOLDER_INDEX;
	TStockNodeData *DeleteBinData		= new TStockNodeData(snDeletedBin);
	DeletedNode->Data						= DeleteBinData;

	RefreshDeletedBin();

	if (!Transaction->InTransaction)
	Transaction->StartTransaction();
	
	qrStockRequestLocation->Close();
	for (qrStockRequestLocation->ExecQuery(); !qrStockRequestLocation->Eof; qrStockRequestLocation->Next())
	{
		if(Location == qrStockRequestLocation->FieldByName("LOCATION")->AsString)
		{
			if (!CategoryNode || LastCategoryKey != qrStockRequestLocation->FieldByName("Stock_Category_Key")->AsInteger  )
			{
				if (qrStockRequestLocation->FieldByName("Category_Deleted")->AsString == "F" || qrStockRequestLocation->FieldByName("Category_Deleted")->IsNull)
				{
					CategoryNode						= tvStock->Items->Add(NULL, qrStockRequestLocation->FieldByName("Stock_Category")->AsString);

					CategoryNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
					CategoryNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
					LastCategoryKey					= qrStockRequestLocation->FieldByName("Stock_Category_Key")->AsInteger;
					CreateStockCategory(CategoryNode,// qrStockRequestLocation, false);
					qrStockRequestLocation->FieldByName("Stock_Category")->AsString,
					LastCategoryKey,
					qrStockRequestLocation->FieldByName("Category_GL_Code")->AsString,false);
				}
			}
			if ((!GroupNode || LastGroupKey != qrStockRequestLocation->FieldByName("Stock_Group_Key")->AsInteger) &&
					!qrStockRequestLocation->FieldByName("Stock_Group_Key")->IsNull)
			{
				if ((qrStockRequestLocation->FieldByName("Group_Deleted")->AsString == "F" || qrStockRequestLocation->FieldByName("Group_Deleted")->IsNull) &&
						(qrStockRequestLocation->FieldByName("Category_Deleted")->AsString == "F" || qrStockRequestLocation->FieldByName("Category_Deleted")->IsNull))
				{
					GroupNode							= tvStock->Items->AddChild(CategoryNode, qrStockRequestLocation->FieldByName("Stock_Group")->AsString);
					GroupNode->ImageIndex			= ICON_CLOSED_FOLDER_INDEX;
					GroupNode->SelectedIndex		= ICON_OPEN_FOLDER_INDEX;
					LastGroupKey						= qrStockRequestLocation->FieldByName("Stock_Group_Key")->AsInteger;
					CreateStockGroup(GroupNode,// qrStockRequestLocation, false);
					qrStockRequestLocation->FieldByName("Stock_Category")->AsString,
					qrStockRequestLocation->FieldByName("Stock_Category_Key")->AsInteger,
					qrStockRequestLocation->FieldByName("Group_GL_Code")->AsString,
					qrStockRequestLocation->FieldByName("Stock_Group")->AsString,
					qrStockRequestLocation->FieldByName("Stock_Group_Key")->AsInteger,
					false);
				}
			}
			if (!Registered)
			{
				if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
				{
					ShowRegWarning = true;
					break;
				}
			}
			if (!qrStockRequestLocation->FieldByName("Stock_Key")->IsNull)
			{
				if ((qrStockRequestLocation->FieldByName("Group_Deleted")->AsString == "F" || qrStockRequestLocation->FieldByName("Group_Deleted")->IsNull) &&
						(qrStockRequestLocation->FieldByName("Category_Deleted")->AsString == "F" || qrStockRequestLocation->FieldByName("Category_Deleted")->IsNull) &&
						(qrStockRequestLocation->FieldByName("Item_Deleted")->AsString == "F" || qrStockRequestLocation->FieldByName("Item_Deleted")->IsNull))
				{
					StockNode							= tvStock->Items->AddChild(GroupNode, qrStockRequestLocation->FieldByName("Description")->AsString);
					// Check to see if any of the Stock Codes have been initialised for Stocktake
					if (qrStockRequestLocation->FieldByName("Initialised")->AsString == "T")//StockInStocktake(qrStockRequestLocation->FieldByName("Stock_Key")->AsInteger))
					{
						StockNode->ImageIndex			= ICON_EXCLAMATION_RED_INDEX;
						StockNode->SelectedIndex		= ICON_EXCLAMATION_RED_INDEX;
					}
					else
					{
						StockNode->ImageIndex			= ICON_BOX_INDEX;
						StockNode->SelectedIndex		= ICON_BOX_INDEX;
					}
					CreateStockItem(StockNode,
					qrStockRequestLocation->FieldByName("Description")->AsString,
					qrStockRequestLocation->FieldByName("Stock_Key")->AsInteger,
					qrStockRequestLocation->FieldByName("Code")->AsString,
					qrStockRequestLocation->FieldByName("Stocktake_Unit")->AsString,
					qrStockRequestLocation->FieldByName("Stock_Category")->AsString,
					qrStockRequestLocation->FieldByName("Stock_Category_Key")->AsInteger,
					qrStockRequestLocation->FieldByName("Stock_Group")->AsString,
					qrStockRequestLocation->FieldByName("Stock_Group_Key")->AsInteger,
					false);

					StockItemCount++;
					if (!Registered)
					{
						if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
						{
							ShowRegWarning = true;
							break;
						}
					}
				}
			}
		}
	}
	if (ShowRegWarning)
	{
		Application->MessageBox(AnsiString("You are limited to " + IntToStr(MAX_STOCK_ITEMS_IN_EVALUATION_MODE) + " items in the evaluation version.").c_str(), "Error", MB_ICONERROR + MB_OK);
	}
	Transaction->Commit();
	tvStock->Items->EndUpdate();
	tvStock->OnChange = tvStockChange;
}


void TfrmStock::TreeviewData(  )
{
}
//---------------------------------------------------------------------------
bool TfrmStock::StockInStocktake( int StockKey )
{
	bool Initialised = false;
	qrStockLocation->Close();
	qrStockLocation->ParamByName("Stock_Key")->AsInteger = StockKey;
	for (qrStockLocation->Open(); !qrStockLocation->Eof; qrStockLocation->Next())
	{
		if (qrStockLocation->FieldByName("Initialised")->AsString == "T")
		Initialised = true;
	}
	return Initialised;
}
//---------------------------------------------------------------------------
void TfrmStock::RefreshDeletedBin()
{
	int StockItemCount					= GetStockItemCount() + 1;
	bool ShowRegWarning					= false;

	if (DeletedNode)
	{
		TStockNodeData *NodeData = (TStockNodeData *)DeletedNode->Data;
		if (NodeData->Type == snDeletedBin)
		{
			tvStock->Items->BeginUpdate();
			tvStock->OnChange = NULL;
			DeletedNode->DeleteChildren();
			ListView1->Items->Clear();
			if (!Transaction->InTransaction) Transaction->StartTransaction();

			qrDeletedList->Close();
			for (qrDeletedList->ExecQuery(); !qrDeletedList->Eof; qrDeletedList->Next())
			{
				if (!Registered)
				{
					if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
					{
						ShowRegWarning = true;
						break;
					}
				}
				if (!qrDeletedList->FieldByName("Stock_Key")->IsNull)
				{
					TTreeNode *StockNode				= tvStock->Items->AddChild(DeletedNode, qrDeletedList->FieldByName("Description")->AsString);
					StockNode->ImageIndex			= ICON_BOX_INDEX;
					StockNode->SelectedIndex		= ICON_BOX_INDEX;

					CreateStockItem(StockNode,
					qrDeletedList->FieldByName("Description")->AsString,
					qrDeletedList->FieldByName("Stock_Key")->AsInteger,
					qrDeletedList->FieldByName("Code")->AsString,
					qrDeletedList->FieldByName("Stocktake_Unit")->AsString,
					qrDeletedList->FieldByName("Stock_Category")->AsString,
					qrDeletedList->FieldByName("Stock_Category_Key")->AsInteger,
					qrDeletedList->FieldByName("Stock_Group")->AsString,
					qrDeletedList->FieldByName("Stock_Group_Key")->AsInteger,
					true);

					StockItemCount++;
					if (!Registered)
					{
						if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
						{
							ShowRegWarning = true;
							break;
						}
					}
				}
			}
			if (ShowRegWarning)
			{
				Application->MessageBox(AnsiString("You are limited to " + IntToStr(MAX_STOCK_ITEMS_IN_EVALUATION_MODE) + " items in the evaluation version.").c_str(), "Error", MB_ICONERROR + MB_OK);
			}
			if (Transaction->InTransaction) Transaction->Commit();
			tvStock->Items->EndUpdate();
			tvStock->OnChange = tvStockChange;
		}
	}
}
//---------------------------------------------------------------------------
void TfrmStock::RefreshStockGroup()
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	tvStock->Items->BeginUpdate();
	tvStock->OnChange = NULL;
	// Find the StockGroup in the tree. Could be 2. One in the deleted bin.
	TTreeNode *CurCat = tvStock->Items->GetFirstNode();
	while (CurCat)
	{
		TStockNodeData *CategoryData = (TStockNodeData *)CurCat->Data;
		if (CategoryData->Type == snStockCategory)
		{
			TTreeNode *CurGroup = CurCat->getFirstChild();
			while (CurGroup)
			{
				TStockNodeData *GroupData = (TStockNodeData *)CurGroup->Data;
				if (GroupData->Type == snStockGroup && GroupData->Key == StockGroupKey)
				{
					RefreshStockGroup(CurGroup, StockGroupKey);
					break;
				}
				CurGroup = CurGroup->getNextSibling();
			}
		}
		CurCat = CurCat->getNextSibling();
	}
	tvStock->OnChange = tvStockChange;
	if (StockKey)
	{
		SelectInTree(LocateInTree(NULL, NULL, StockKey));
	}
	if (Transaction->InTransaction) Transaction->Commit();
	tvStock->Selected->MakeVisible();
	tvStock->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void TfrmStock::RefreshStockGroup(TTreeNode *StockGroupNode, int StockGroupKey)
{
	StockGroupNode->DeleteChildren();

	TStockNodeData *StockGroupData = (TStockNodeData *)StockGroupNode->Data;

	int StockItemCount = GetStockItemCount()+1;

	qrGroupList->Close();
	qrGroupList->ParamByName("Stock_Group_Key")->AsInteger = StockGroupKey;

	for (qrGroupList->ExecQuery(); !qrGroupList->Eof; qrGroupList->Next())
	{
		if (!Registered)
		{
			if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
			{
				break;
			}
		}
		if (StockGroupData->Deleted	== (qrGroupList->FieldByName("Deleted")->AsString == "T"))
		{
			TTreeNode *StockNode				= tvStock->Items->AddChild(StockGroupNode, qrGroupList->FieldByName("Description")->AsString);

			//StockNode->ImageIndex			= ICON_BOX_INDEX; // cww today
			//StockNode->SelectedIndex		= ICON_BOX_INDEX;

			if (StockInStocktake(qrGroupList->FieldByName("Stock_Key")->AsInteger))
			{
				StockNode->ImageIndex			= ICON_EXCLAMATION_RED_INDEX;
				StockNode->SelectedIndex		= ICON_EXCLAMATION_RED_INDEX;
			}
			else
			{
				StockNode->ImageIndex			= ICON_BOX_INDEX;
				StockNode->SelectedIndex		= ICON_BOX_INDEX;
			}

			//			TStockNodeData *StockNodeData	= CreateStockItem(StockNode, qrGroupList, (qrGroupList->FieldByName("Deleted")->AsString == "T"));

			CreateStockItem(StockNode,
			qrGroupList->FieldByName("Description")->AsString,
			qrGroupList->FieldByName("Stock_Key")->AsInteger,
			qrGroupList->FieldByName("Code")->AsString,
			qrGroupList->FieldByName("Stocktake_Unit")->AsString,
			qrGroupList->FieldByName("Stock_Category")->AsString,
			qrGroupList->FieldByName("Stock_Category_Key")->AsInteger,
			qrGroupList->FieldByName("Stock_Group")->AsString,
			qrGroupList->FieldByName("Stock_Group_Key")->AsInteger,
			false);
			/*			TStockNodeData *StockNodeData	= new TStockNodeData(snStockItem);
			StockNode->Data					= StockNodeData;

			StockNodeData->Description		= qrGroupList->FieldByName("Description")->AsString;
			StockNodeData->ImageIndex		= ICON_BOX_INDEX;
			StockNodeData->SelectedIndex	= ICON_BOX_INDEX;
			StockNodeData->Key				= qrGroupList->FieldByName("Stock_Key")->AsInteger;
			StockNodeData->Deleted			= (qrGroupList->FieldByName("Deleted")->AsString == "T");
			StockNodeData->StockCode		= qrGroupList->FieldByName("Code")->AsString;
			StockNodeData->StocktakeUnit	= qrGroupList->FieldByName("Stocktake_Unit")->AsString;
			StockNodeData->Category			= qrGroupList->FieldByName("Stock_Category")->AsString;
			StockNodeData->CategoryKey		= qrGroupList->FieldByName("Stock_Category_Key")->AsInteger;
			StockNodeData->Group				= qrGroupList->FieldByName("Stock_Group")->AsString;
			StockNodeData->GroupKey			= qrGroupList->FieldByName("Stock_Group_Key")->AsInteger;*/

			StockItemCount++;
		}
	}
	qrGroupList->Close();
	/*	bool Found = false;
	while (StockGroupNode && !Found)
	{
		if (StockGroupNode->Level == TreeLevelGroup && int(StockGroupNode->Data) == StockGroupKey)
		{
			Found = true;
		}
		else
		{
			StockGroupNode = StockGroupNode->GetNext();
		}
	}
	if (!Found) return;

	tvStock->Items->BeginUpdate();
	tvStock->OnChange = NULL;
	StockGroupNode->DeleteChildren();

	int StockItemCount = GetStockItemCount()+1;

	if (!Transaction->InTransaction) Transaction->StartTransaction();

	qrStockCategory->Open();
	qrStockCategory->Locate("Stock_Category_Key", StockCategoryKey, TLocateOptions());

	qrStockGroup->Open();
	if (qrStockGroup->Locate("Stock_Group_Key", StockGroupKey, TLocateOptions()))
	{
		for (qrStock->Open(); !qrStock->Eof; qrStock->Next())
		{
			if (!Registered)
			{
				if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
				{
					break;
				}
			}
			TTreeNode *StockNode			= tvStock->Items->AddChild(StockGroupNode, qrStock->FieldByName("Description")->AsString);
			StockNode->ImageIndex		= ICON_BOX_INDEX;
			StockNode->SelectedIndex	= ICON_BOX_INDEX;
			StockNode->Data				= (void *)qrStock->FieldByName("Stock_Key")->AsInteger;
			StockItemCount++;
		}
		qrStock->Close();
	}
	if (Transaction->InTransaction) Transaction->Commit();
}
//---------------------------------------------------------------------------
void TfrmStock::RefreshStockCategory()
{
	// Find the StockGroup in the tree
	TTreeNode *StockCategoryNode = tvStock->Items->GetFirstNode();
/*	while (StockCategoryNode)
	{
		if (StockCategoryNode->Level == TreeLevelCategory && int(StockCategoryNode->Data) == StockCategoryKey)
		{
			break;
		}
		StockCategoryNode = StockCategoryNode->GetNext();
	}
	if (!StockCategoryNode) return; // StockGroup not found.

	tvStock->Items->BeginUpdate();
	tvStock->OnChange = NULL;
	StockCategoryNode->DeleteChildren();

	int StockItemCount = GetStockItemCount()+1;

	TTreeNode *GroupNode = NULL;
	TTreeNode *StockNode = NULL;
	qrStockCategory->Open();
	if (qrStockCategory->Locate("Stock_Category_Key", StockCategoryKey, TLocateOptions()))
	{
		for (qrStockGroup->Open(); !qrStockGroup->Eof; qrStockGroup->Next())
		{
			GroupNode						= tvStock->Items->AddChild(StockCategoryNode, qrStockGroup->FieldByName("Stock_Group")->AsString);
			GroupNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
			GroupNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
			GroupNode->Data				= (void *)qrStockGroup->FieldByName("Stock_Group_Key")->AsInteger;
			for (qrStock->Open(); !qrStock->Eof; qrStock->Next())
			{
				if (!Registered)
				{
					if (StockItemCount > MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
					{
//						Application->MessageBox(AnsiString("You are limited to " + IntToStr(MAX_STOCK_ITEMS_IN_EVALUATION_MODE) + " items in the evaluation version.").c_str(), "Error", MB_ICONERROR + MB_OK);
						break;
					}
				}
				StockNode						= tvStock->Items->AddChild(GroupNode, qrStock->FieldByName("Description")->AsString);
				StockNode->ImageIndex		= ICON_BOX_INDEX;
				StockNode->SelectedIndex	= ICON_BOX_INDEX;
				StockNode->Data				= (void *)qrStock->FieldByName("Stock_Key")->AsInteger;
				StockItemCount++;
			}
			qrStock->Close();
		}
		qrStockGroup->Close();
	}
	tvStock->OnChange = tvStockChange;
	if (StockGroupKey || StockKey)
	{
		TTreeNode *CurItem = StockCategoryNode->getFirstChild();
		while (CurItem)
		{
			if (CurItem->Level == TreeLevelGroup && int(CurItem->Data) == StockGroupKey)
			{
				tvStock->Selected = CurItem;
			}
			else if (CurItem->Level == TreeLevelStock && int(CurItem->Data) == StockKey)
			{
				tvStock->Selected = CurItem;
				break;
			}
			CurItem = CurItem->GetNext();
		}
	}
	if (tvStock->Selected)
	{
		tvStock->Selected->MakeVisible();
	}
	tvStock->Items->EndUpdate(); */
}
//---------------------------------------------------------------------------
void TfrmStock::CreateStockItem(TTreeNode *ItemNode,
AnsiString Description, int StockKey, AnsiString Code, AnsiString StocktakeUnit,
AnsiString Category, int CategoryKey,
AnsiString Group, int GroupKey,
bool Deleted)     
{
	TStockNodeData *StockNodeData	= new TStockNodeData(snStockItem);
	ItemNode->Data						= StockNodeData;

	StockNodeData->Description		= Description;
	StockNodeData->Key				= StockKey;
	StockNodeData->Deleted			= Deleted;
	StockNodeData->StockCode		= Code;
	StockNodeData->StocktakeUnit	= StocktakeUnit;
	StockNodeData->Category			= Category;
	StockNodeData->CategoryKey		= CategoryKey;
	StockNodeData->Group				= Group;
	StockNodeData->GroupKey			= GroupKey;
}
//---------------------------------------------------------------------------
void TfrmStock::CreateStockGroup(TTreeNode *ItemNode,
AnsiString Category, int CategoryKey, AnsiString GLCode,
AnsiString Group, int GroupKey,
bool Deleted)
{
	TStockNodeData *StockNodeData	= new TStockNodeData(snStockGroup);
	ItemNode->Data						= StockNodeData;

	StockNodeData->Description		= Group;
	StockNodeData->Key				= GroupKey;
	StockNodeData->Deleted			= Deleted;
	StockNodeData->GLCode			= GLCode;
	StockNodeData->Category			= Category;
	StockNodeData->CategoryKey		= CategoryKey;
}
//---------------------------------------------------------------------------
void TfrmStock::CreateStockCategory(TTreeNode *ItemNode,
AnsiString Category, int CategoryKey, AnsiString GLCode,
bool Deleted)
{
	TStockNodeData *StockNodeData	= new TStockNodeData(snStockCategory);
	ItemNode->Data						= StockNodeData;

	StockNodeData->Description		= Category;
	StockNodeData->Key				= CategoryKey;
	StockNodeData->CategoryKey		= CategoryKey;
	StockNodeData->Deleted			= Deleted;
	StockNodeData->GLCode			= GLCode;
}
//---------------------------------------------------------------------------
int TfrmStock::GetStockItemCount()
{
	int Count = 0;
	TTreeNode *CurItem = tvStock->Items->GetFirstNode();
	while (CurItem)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)CurItem->Data;
		if (StockNodeData->Type == snStockItem)
		{
			Count++;
		}
		CurItem = CurItem->GetNext();
	}
	return Count;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::FormClose(TObject *Sender,
TCloseAction &Action)
{
	if (tvStock->Selected)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)tvStock->Selected->Data;

		if (StockNodeData->Type == snStockItem)
		{
			if (StockNodeData->Deleted)
			{
				StockCategoryKey		= NULL;
				StockGroupKey			= NULL;
				StockKey					= StockNodeData->Key;
			}
			else
			{
				StockCategoryKey		= ((TStockNodeData *)tvStock->Selected->Parent->Parent->Data)->Key;
				StockGroupKey			= ((TStockNodeData *)tvStock->Selected->Parent->Data)->Key;
				StockKey					= StockNodeData->Key;
			}
		}
		else if (StockNodeData->Type == snStockGroup)
		{
			StockCategoryKey		= ((TStockNodeData *)tvStock->Selected->Parent->Data)->Key;
			StockGroupKey			= StockNodeData->Key;
			StockKey					= NULL;
		}
		else if (StockNodeData->Type == snStockCategory)
		{
			StockCategoryKey		= StockNodeData->Key;
			StockGroupKey			= NULL;
			StockKey					= NULL;
		}
		StockSelectedDeleted		= StockNodeData->Deleted;
	}
	else
	{
		StockCategoryKey			= NULL;
		StockGroupKey				= NULL;
		StockKey						= NULL;
		StockSelectedDeleted		= false;
	}
	NewItemName = "";
	FindDialog->CloseDialog();

}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::WMDisplayChange(TWMDisplayChange& Message)
{
}
//---------------------------------------------------------------------------
void TfrmStock::ResizeGrids()
{
	const ColCount = 4;
	TStringGrid *Grid = sgSuppliers;
	const PercArray[ColCount] = {4, 50, 23, 23};
	int WidthSoFar = 0;
	int PercLeft = 100;
	for (int i=0; i<Grid->ColCount; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray[i] / PercLeft;
		if (PercArray[i] != 0)
		{
			Grid->ColWidths[i] = ColWidth - 1;
			WidthSoFar += ColWidth;
			PercLeft -= PercArray[i];
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::CMDialogChar(TWMKey &Msg)
{
	if (!(HIWORD(Msg.KeyData) & KF_ALTDOWN))
	{
		SendMessage(ActiveControl->Handle, WM_CHAR, Msg.CharCode, 0);
		Msg.Result = 1;
	}
	TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tvStockMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button == mbRight)
	{
		if (tvStock->GetNodeAt(X, Y))
		{
			tvStock->Selected = tvStock->GetNodeAt(X, Y);
			TPoint PopupPoint = ClientToScreen(TPoint(X, Y));
			pmTreeView->Popup(PopupPoint.x , PopupPoint.y);
		}
		/*		else
		{
			tvStock->Selected = NULL;
		}
		TPoint PopupPoint = ClientToScreen(TPoint(X, Y));
		pmTreeView->Popup(PopupPoint.x , PopupPoint.y); */
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::pmTreeViewPopup(TObject *Sender)
{
	if (tvStock->Selected)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)tvStock->Selected->Data;

		if (StockNodeData->Deleted)
		{
			miNewStockCategory->Enabled	= false;
			miNewStockGroup->Enabled		= false;
			miNewStockItem->Enabled			= false;

			miEdit->Enabled					= false;
			miRename->Enabled					= false;
			miDelete->Enabled					= (StockNodeData->Type == snStockItem);
			miRestore->Enabled				= (StockNodeData->Type == snStockItem);
		}
		else
		{
			miRestore->Enabled					= false;
			if (StockNodeData->Type == snStockItem)
			{
				miNewStockCategory->Enabled	= false;
				miNewStockGroup->Enabled		= true;
				miNewStockItem->Enabled			= true;

				miEdit->Enabled					= true;
				miRename->Enabled					= true;
				miDelete->Enabled					= true;
			}
			else if (StockNodeData->Type == snStockGroup)
			{
				miNewStockCategory->Enabled	= true;
				miNewStockGroup->Enabled		= true;
				miNewStockItem->Enabled			= true;

				miEdit->Enabled					= true;
				miRename->Enabled					= true;
				miDelete->Enabled					= true;
			}
			else if (StockNodeData->Type == snStockCategory)
			{
				miNewStockCategory->Enabled	= true;
				miNewStockGroup->Enabled		= true;
				miNewStockItem->Enabled			= false;

				miEdit->Enabled					= true;
				miRename->Enabled					= true;
				miDelete->Enabled					= true;
			}
			else if (StockNodeData->Type == snDeletedBin)
			{
				miNewStockCategory->Enabled	= false;
				miNewStockGroup->Enabled		= false;
				miNewStockItem->Enabled			= false;

				miEdit->Enabled					= false;
				miRename->Enabled					= false;
				miDelete->Enabled					= false;
			}
		}
	}
	else
	{
		miEdit->Enabled		= false;
		miRename->Enabled		= false;
		miDelete->Enabled		= false;
		miRestore->Enabled	= false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tvStockDragOver(TObject *Sender,
TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	Accept = false;
	if (Sender == Source)
	{
		if (tvStock->Selected)
		{
			TTreeNode* DropNode = tvStock->GetNodeAt(X, Y);
			TStockNodeData *SourceNodeData = (TStockNodeData *)tvStock->Selected->Data;
			TStockNodeData *DestNodeData = NULL;
			if (DropNode)
			{
				DestNodeData = (TStockNodeData *)DropNode->Data;
			}

			// Trying to move the rubbish bin.
			if (SourceNodeData->Type == snDeletedBin)
			{
				Accept = false;
			}
			// Trying to drop on deleted cat/group/item
			else if (SourceNodeData->Deleted)
			{
				Accept = false;
			}
			// Trying to move a deleted cat/group/item
			// Might allow this later.
			else if (DestNodeData && DestNodeData->Deleted)
			{
				Accept = false;
			}
			// Allow dropping in the rubbish if not already deleted.
			else if (DestNodeData && DestNodeData->Type == snDeletedBin)
			{
				Accept = !SourceNodeData->Deleted;
			}
			// Dragging a StockCategory
			else if (SourceNodeData->Type == snStockCategory)
			{
				// Can drop in free space. Goes to end.
				if (!DropNode)
				{
					Accept = true;
				}
				// Insert in other categories
				else if (DestNodeData->Type == snStockCategory)
				{
					Accept = true;
				}
			}
			// Dragging a StockGroup
			else if (SourceNodeData->Type == snStockGroup)
			{
				/*				// Can drop in free space only if not viewing categories. Goes to end.
				if (!DropNode)
				{
					if (!chbShowCategories->Checked)
					{
						Accept = true;
					}
				}
				// Can drop onto other groups
				else if (!chbShowCategories->Checked && DropNode->Level == TreeLevelGroup)
				{
					// Can't drop on itself!
					if (DropNode != tvStock->Selected)
					{
						Accept = true;
					}
				}
				else if (chbShowCategories->Checked)*/
				if (DestNodeData)
				{
					// Can drop onto other groups
					if (DestNodeData->Type == snStockGroup)
					{
						if (DropNode != tvStock->Selected)
						{
							Accept = true;
						}
					}
					// Can drop onto other categories
					else if (DestNodeData->Type == snStockCategory)
					{
						Accept = true;
					}
				}
			}
			// Dragging a stock item
			else if (SourceNodeData->Type == snStockItem)
			{
				// Cant drop in free space. Must be a StockGroup
				if (DestNodeData && DestNodeData->Type == snStockGroup)
				{
					// No point moving it to where it already is!
					if (DropNode != tvStock->Selected->Parent)
					{
						Accept = true;
					}
				}
				else if (DestNodeData && DestNodeData->Type == snStockItem)
				{
					// No point moving it to where it already is!
					if (DropNode->Parent != tvStock->Selected->Parent)
					{
						Accept = true;
					}
				}

			}
		}
		//		int Offset = abs(tvStock->Font->Height/2);
		if (Y >= tvStock->ClientHeight - 15 || Y <= 5)
		{
			tiTreeScroll->Enabled = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tiTreeScrollTimer(TObject *Sender)
{
	TPoint MousePoint = Mouse->CursorPos;
	TPoint TreePoint = ScreenToClient(MousePoint);

	int /*NbPixels, RMin=0, RMax=0,*/ ScrollPos;

	//	GetScrollRange(tvStock->Handle, SB_VERT, &RMin, &RMax);
	ScrollPos = GetScrollPos(tvStock->Handle, SB_VERT);

	//	NbPixels = abs(tvStock->Font->Height*2);

	TDragImageList	*DragImageList;
	DragImageList = dynamic_cast<TDragImageList *>(tvStock->Images);

	if (DragImageList)
	{
		if (TreePoint.y > tvStock->ClientHeight - 15)
		{
			DragImageList->HideDragImage();
			tvStock->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
			if (ScrollPos == GetScrollPos(tvStock->Handle, SB_VERT))
			{
				DragImageList->ShowDragImage();
				tiTreeScroll->Enabled = false;
			}
			else
			{
				if (tvStock->DropTarget)
				{
					tvStock->DropTarget = tvStock->DropTarget->GetNextVisible();
				}
				DragImageList->ShowDragImage();
			}
		}
		else if (TreePoint.y <= 15)
		{
			DragImageList->HideDragImage();
			tvStock->Perform(WM_VSCROLL, SB_LINEUP, 0);
			if (ScrollPos == GetScrollPos(tvStock->Handle, SB_VERT))
			{
				tiTreeScroll->Enabled = false;
				DragImageList->ShowDragImage();
			}
			else
			{
				tvStock->DropTarget = tvStock->TopItem;
				DragImageList->ShowDragImage();
			}
		}
		else
		{
			tiTreeScroll->Enabled = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tvStockEdited(TObject *Sender, TTreeNode *Node,
AnsiString &S)
{
	TStockNodeData *NodeData = (TStockNodeData *)Node->Data;
	// Edited a StockCategory
	if (NodeData->Type == snStockCategory)
	{
		AnsiString OldName = Node->Text;
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		try
		{
			bool RollBack = false;
			if (S == "")
			{
				Application->MessageBox("This stock category must have a name!", "Error", MB_OK | MB_ICONERROR);
				RollBack = true;
			}
			else
			{
				S = S.SubString(0, MAX_STOCK_STOCK_GROUP_LENGTH);
				qrFindStockCategory->Close();
				qrFindStockCategory->ParamByName("Stock_Category")->AsString		= S.UpperCase();
				qrFindStockCategory->ParamByName("Stock_Category_Key")->AsInteger	= NodeData->Key;
				qrFindStockCategory->ExecQuery();
				if (!qrFindStockCategory->Eof)
				{
					Application->MessageBox("This stock category already exists!", "Error", MB_OK | MB_ICONERROR);
					RollBack = true;
				}
				else
				{
					qrUpdateStockCategory->Close();
					qrUpdateStockCategory->SQL->Text = "Update StockCategory Set Stock_Category = :Stock_Category Where Stock_Category_Key = :Stock_Category_Key";
					qrUpdateStockCategory->ParamByName("Stock_Category_Key")->AsInteger	= NodeData->Key;
					qrUpdateStockCategory->ParamByName("Stock_Category")->AsString			= S;
					qrUpdateStockCategory->ExecQuery();
					lbeTitle->Caption			= S;
					NodeData->Description	= S;
				}
			}
			if (RollBack)
			{
				S = OldName;
				//				Node->EditText();	// Doesn't work correctly. Use PostMessage(TVM_EDITLABEL)
				PostMessage(Node->TreeView->Handle, TVM_EDITLABEL, 0, reinterpret_cast<LPARAM>(Node->ItemId));
			}
			Transaction->Commit();
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction)
			{
				S = OldName;
				Transaction->Rollback();
			}
			throw;
		}
	}
	// Edited a StockGroup
	if (NodeData->Type == snStockGroup)
	{
		AnsiString OldName = Node->Text;
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		try
		{
			bool RollBack = false;
			if (S == "")
			{
				Application->MessageBox("This stock group must have a name!", "Error", MB_OK | MB_ICONERROR);
				RollBack = true;
			}
			else
			{
				S = S.SubString(0, MAX_STOCK_STOCK_GROUP_LENGTH);
				qrFindStockGroup->Close();
				qrFindStockGroup->ParamByName("Stock_Group_Key")->AsInteger	= NodeData->Key;
				qrFindStockGroup->ParamByName("Stock_Group")->AsString		= "%" + S.UpperCase() + "%";
				qrFindStockGroup->ExecQuery();
				if (!qrFindStockGroup->Eof)
				{
					Application->MessageBox("This stock group already exists!", "Error", MB_OK | MB_ICONERROR);
					RollBack = true;
				}
				else
				{
					qrUpdateStockGroup->Close();
					qrUpdateStockGroup->SQL->Text = "Update StockGroup Set Stock_Group = :Stock_Group Where Stock_Group_Key = :Stock_Group_Key";
					qrUpdateStockGroup->ParamByName("Stock_Group_Key")->AsInteger	= NodeData->Key;
					qrUpdateStockGroup->ParamByName("Stock_Group")->AsString			= S;
					qrUpdateStockGroup->ExecQuery();
					lbeTitle->Caption			= S;
					NodeData->Description	= S;
				}
			}
			if (RollBack)
			{
				S = OldName;
				//				Node->EditText();	// Doesn't work correctly. Use PostMessage(TVM_EDITLABEL)
				PostMessage(Node->TreeView->Handle, TVM_EDITLABEL, 0, reinterpret_cast<LPARAM>(Node->ItemId));
			}
			Transaction->Commit();
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction)
			{
				S = OldName;
				Transaction->Rollback();
			}
			throw;
		}
	}
	// Edited a stock item
	if (NodeData->Type == snStockItem)
	{
		AnsiString OldName = Node->Text;
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		try
		{
			bool RollBack = false;

			if (S == "")
			{
				Application->MessageBox("This stock item must have a name!", "Error", MB_OK | MB_ICONERROR);
				RollBack = true;
			}
			else
			{
				S = S.SubString(0, MAX_STOCK_ITEM_LENGTH);
				qrFindStock->Close();
				qrFindStock->ParamByName("Stock_Key")->AsInteger = NodeData->Key;
				qrFindStock->ParamByName("Description")->AsString = "%" + S.UpperCase() + "%";
				qrFindStock->ExecQuery();
				if (!qrFindStock->Eof)
				{
					Application->MessageBox("This stock item already exists!", "Error", MB_OK | MB_ICONERROR);
					RollBack = true;
				}
				else
				{
					qrUpdateStock->Close();
					qrUpdateStock->SQL->Text = "Update Stock Set Description = :Description Where Stock_Key = :Stock_Key";
					qrUpdateStock->ParamByName("Stock_Key")->AsInteger = NodeData->Key;
					qrUpdateStock->ParamByName("Description")->AsString = S;
					qrUpdateStock->ExecQuery();

					StockGroupKey						= ((TStockNodeData *)Node->Parent->Data)->Key;
					StockKey								= ((TStockNodeData *)Node->Data)->Key;
					StockSelectedDeleted				= false;
					// Cant delete nodes inside this event!!!
					PostMessage(Handle, UWM_REFRESH_STOCK_GROUP, StockGroupKey, StockKey);
				}
			}
			if (RollBack)
			{
				S = OldName;
				//				Node->EditText();	// Doesn't work correctly. Use PostMessage(TVM_EDITLABEL)
				PostMessage(Node->TreeView->Handle, TVM_EDITLABEL, 0, reinterpret_cast<LPARAM>(Node->ItemId));
			}
			if (Transaction->InTransaction) Transaction->Commit();
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction)
			{
				S = OldName;
				Transaction->Rollback();
			}
			throw;
		}
	}
}
//---------------------------------------------------------------------------
void TfrmStock::SetSortOrder()
{
	qrUpdateStockCategory->Close();
	qrUpdateStockCategory->SQL->Text	=
	"Update "
	"StockCategory "
	"Set "
	"Sort_Order = :Sort_Order "
	"Where "
	"Stock_Category_Key = :Stock_Category_Key";

	qrUpdateStockGroup->Close();
	qrUpdateStockGroup->SQL->Text		=
	"Update "
	"StockGroup "
	"Set "
	"Sort_Order = :Sort_Order "
	"Where "
	"Stock_Group_Key = :Stock_Group_Key";

	int GroupOrder = 0, CategoryOrder = 0;
	TTreeNode *CurItem = tvStock->Items->GetFirstNode();
	while (CurItem)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)CurItem->Data;
		if (StockNodeData->Type == snStockGroup)
		{
			qrUpdateStockGroup->ParamByName("Stock_Group_Key")->AsInteger			= StockNodeData->Key;
			qrUpdateStockGroup->ParamByName("Sort_Order")->AsInteger					= GroupOrder++;
			qrUpdateStockGroup->ExecQuery();
		}
		else if (StockNodeData->Type == snStockCategory)
		{
			qrUpdateStockCategory->ParamByName("Stock_Category_Key")->AsInteger	= StockNodeData->Key;
			qrUpdateStockCategory->ParamByName("Sort_Order")->AsInteger				= CategoryOrder++;
			qrUpdateStockCategory->ExecQuery();
		}
		CurItem = CurItem->GetNext();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tvStockDblClick(TObject *Sender)
{
	if (tvStock->Selected && ((TStockNodeData *)tvStock->Selected->Data)->Type == snStockItem)
	{
		if (StockMode == smSelect)
		{
			btnOkClick(NULL);
		}
		else if (StockMode == smEdit)
		{
			miEditClick(NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnOkClick(TObject *Sender)
{
	if (tvStock->IsEditing())
	{
		if (tvStock->Selected)
		{
			tvStock->Selected->EndEdit(false);
		}
	}
	else
	{
		if (StockMode == smSelect)
		{
			if (tvStock->Selected)
			{
				TStockNodeData *NodeData = (TStockNodeData *)tvStock->Selected->Data;

				if (NodeData->Deleted)
				{
					Application->MessageBox("This item has been deleted.", "Error", MB_ICONERROR + MB_OK);
					return;
				}
				else if (NodeData->Type == snStockItem)
				{
					ModalResult = mrOk;
					return;
				}
				else if (NodeData->Type == snStockGroup)
				{
					if (lbStockItems->ItemIndex > -1)
					{
						tvStock->Selected = tvStock->Selected->Item[lbStockItems->ItemIndex];
						ModalResult = mrOk;
						return;
					}
				}
			}
			Application->MessageBox("You need to select a stock item or press 'Cancel'.", "Error", MB_ICONERROR + MB_OK);
		}
		else if (StockMode == smEdit)
		{
			// kludge ...
			try {
				if (Transaction->Active)
				Transaction->StartTransaction();
				qrUpdateStock->Close();
				qrUpdateStock->SQL->Text =
				"delete from stockcategory where deleted = 'T';";
				qrUpdateStock->ExecQuery();
				qrUpdateStock->Close();
				qrUpdateStock->SQL->Text =
				"delete from stockgroup where deleted = 'T';";
				qrUpdateStock->ExecQuery();
				Transaction->Commit();
			} catch (Exception &e) {
				Transaction->Rollback();
			}

			ModalResult = mrOk;
			return;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnCancelClick(TObject *Sender)
{
	if (tvStock->IsEditing())
	{
		if (tvStock->Selected)
		{
			tvStock->Selected->EndEdit(true);
		}
	}
	else
	{
		ModalResult = mrCancel;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tvStockChange(TObject *Sender, TTreeNode *Node)
{
	if (Node)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)Node->Data;

		lbeTitle->Caption = Node->Text;
		if (StockNodeData->Deleted)
		{
			pcStockDetails->ActivePage = tsDeletedItem;
		}
		else if (StockNodeData->Type == snDeletedBin)
		{
			//			try
			//			{
			//				tvStock->OnChange		= NULL;

			ListView1->Items->BeginUpdate();
			ListView1->Items->Clear();
			pcStockDetails->ActivePage = tsDeletedItems;
			if (DeletedNode)
			{
				TStockNodeData *NodeData = (TStockNodeData *)DeletedNode->Data;
				if (NodeData->Type == snDeletedBin)
				{
					TTreeNode *CurItem = DeletedNode->getFirstChild();
					while (CurItem)
					{
						TStockNodeData *ItemData = (TStockNodeData *)CurItem->Data;
						if (ItemData->Type == snStockItem && ItemData->Deleted)
						{
							TListItem *ListItem	= ListView1->Items->Add();
							ListItem->Caption		= ItemData->Description;
							ListItem->ImageIndex	= -1;

							ListItem->SubItems->Add(ItemData->Category);
							ListItem->SubItems->Add(ItemData->Group);

							if (StockNodeData->Type == snStockItem && ListItem->Index == Node->Index)
							{
								ListView1->ItemIndex = ListItem->Index;
							}
						}
						CurItem = CurItem->getNextSibling();
					}
				}
			}
			ListView1->Items->EndUpdate();
			//			}
			//			__finally
			//			{
			//				tvStock->OnChange		= tvStockChange;
			//			}
		}
		else if (StockNodeData->Type == snStockCategory)
		{
			lbStockGroups->Items->Clear();
			for (int i=0; i<Node->Count; i++)
			{
				lbStockGroups->Items->Add(Node->Item[i]->Text);
			}
			pcStockDetails->ActivePage = tsStockCategory;
		}
		else if (StockNodeData->Type == snStockGroup)
		{
			lbStockItems->Items->Clear();
			for (int i=0; i<Node->Count; i++)
			{
				// for lbStockItemsDrawItem
				if (Node->Item[i]->SelectedIndex	== ICON_EXCLAMATION_RED_INDEX)
				StockItemIconIndex = ICON_EXCLAMATION_RED_INDEX;
				else
				StockItemIconIndex = ICON_BOX_INDEX;
				lbStockItems->Items->Add(Node->Item[i]->Text);
			}
			pcStockDetails->ActivePage = tsStockGroup;
		}
		else if (StockNodeData->Type == snStockItem)
		{
			if (!Transaction->InTransaction) Transaction->StartTransaction();

			if (Node->SelectedIndex	  == ICON_EXCLAMATION_RED_INDEX)
			pnlStocktake->Visible = true;
			else
			pnlStocktake->Visible = false;

			lbeStockCode->Caption		= StockNodeData->StockCode;//qrStock->FieldByName("Code")->AsString;
			lbeUnit->Caption				= StockNodeData->StocktakeUnit;//qrStock->FieldByName("Stocktake_Unit")->AsString;

			sgLocations->Cells[0][0]	= "Location";
			sgLocations->Cells[1][0]	= "On Hand";
			sgLocations->Cells[2][0]	= "On Order";
			sgLocations->Cells[3][0]	= "Ave Cost";
			sgLocations->Cells[4][0]	= "Latest Cost";
			sgLocations->Cells[5][0]	= "Min";
			sgLocations->Cells[6][0]	= "Max";
			sgLocations->Cells[7][0]	= "Initialised";
			sgLocations->Cells[8][0]	= "Sales Pending";
			sgLocations->Cells[9][0]	= "WriteOffs Pending";
			sgLocations->Cells[0][1]	= "";
			sgLocations->Cells[1][1]	= "";
			sgLocations->Cells[2][1]	= "";
			sgLocations->Cells[3][1]	= "";
			sgLocations->Cells[4][1]	= "";
			sgLocations->Cells[5][1]	= "";
			sgLocations->Cells[6][1]	= "";
			sgLocations->Cells[7][1]	= "";
			sgLocations->Cells[8][1]	= "";
			sgLocations->Cells[9][1]	= "";
			sgLocations->RowCount = 2;
			int Row = 1;
			qrStockLocation->Close();
			qrStockLocation->ParamByName("Stock_Key")->AsInteger = StockNodeData->Key;
			for (qrStockLocation->Open(); !qrStockLocation->Eof; qrStockLocation->Next())
			{
				if (Row > 1)
				{
					sgLocations->RowCount++;
				}
                
				 sgLocations->Cells[0][Row] = qrStockLocation->FieldByName("Location")->AsString;


			   sgLocations->Cells[1][Row] = FloatToStrF(qrStockLocation->FieldByName("On_Hand")->AsFloat,ffFixed,19, CurrentConnection.SettingDecimalPlaces);
			   sgLocations->Cells[2][Row] = FloatToStrF(qrStockLocation->FieldByName("On_Order")->AsFloat,ffFixed,19, CurrentConnection.SettingDecimalPlaces); 
                sgLocations->Cells[3][Row] = FloatToStrF(qrStockLocation->FieldByName("Average_Cost")->AsFloat, ffNumber, 19, CurrentConnection.SettingDecimalPlaces);
                sgLocations->Cells[4][Row] = FloatToStrF(qrStockLocation->FieldByName("Latest_Cost")->AsFloat, ffNumber, 19, CurrentConnection.SettingDecimalPlaces);
               
				sgLocations->Cells[5][Row] = qrStockLocation->FieldByName("Min_Level")->AsString;
				sgLocations->Cells[6][Row] = qrStockLocation->FieldByName("Max_Level")->AsString;
				sgLocations->Cells[7][Row] = qrStockLocation->FieldByName("Initialised")->AsString;
				sgLocations->Cells[8][Row] = FloatToStrF(qrStockLocation->FieldByName("Sales_Pending")->AsFloat,ffNumber,19,2);
				sgLocations->Cells[9][Row] = FloatToStrF(qrStockLocation->FieldByName("WriteOffs_Pending")->AsFloat,ffNumber,19,2);
				Row++;
			}
			sgSuppliers->Cells[0][0] = "";
			sgSuppliers->Cells[1][0] = "Supplier";
			sgSuppliers->Cells[2][0] = "Unit";
			sgSuppliers->Cells[3][0] = "Latest Cost";
			sgSuppliers->Cells[0][1] = "";
			sgSuppliers->Cells[1][1] = "";
			sgSuppliers->Cells[2][1] = "";
			sgSuppliers->Cells[3][1] = "";
			sgSuppliers->RowCount = 2;
			Row = 1;
			qrStockSuppliers->Close();
			qrStockSuppliers->ParamByName("Stock_Key")->AsInteger = StockNodeData->Key;
			for (qrStockSuppliers->Open(); !qrStockSuppliers->Eof; qrStockSuppliers->Next())
			{
				if (Row > 1)
				{
					sgSuppliers->RowCount++;
				}
				sgSuppliers->Cells[0][Row] = qrStockSuppliers->FieldByName("Preferred_Supplier")->AsBoolean?"*":"";
				sgSuppliers->Cells[1][Row] = qrStockSuppliers->FieldByName("Supplier_Name")->AsString;
				sgSuppliers->Cells[2][Row] = qrStockSuppliers->FieldByName("Supplier_Unit")->AsString;
				sgSuppliers->Cells[3][Row] = FloatToStrF(qrStockSuppliers->FieldByName("Latest_Cost")->AsFloat,ffNumber,19, CurrentConnection.SettingDecimalPlaces);
				Row++;
			}
			Transaction->Commit();
			ResizeGrids();
			pcStockDetails->ActivePage = tsStockItem;
		}
	}
	else
	{
		lbeTitle->Caption = "Press 'Add Category' to start.";
		lbStockItems->Items->Clear();
		lbStockGroups->Items->Clear();
		pcStockDetails->ActivePage = tsStockCategory;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::miNewStockCategoryClick(TObject *Sender)
{
	AnsiString NewStockCategoryName = "New Category";
	if (FindStockCategory(NewStockCategoryName))
	{
		int i = 2;
		NewStockCategoryName = "New Category (" + IntToStr(i) + ")";
		while (FindStockCategory(NewStockCategoryName))
		{
			i++;
			NewStockCategoryName = "New Category (" + IntToStr(i) + ")";
		}
	}
	frmAddStockCategoryGroup->StockGroupType	= gtCategory;
	frmAddStockCategoryGroup->Name				= NewStockCategoryName;
	frmAddStockCategoryGroup->GLCode				= "";
	if (frmAddStockCategoryGroup->ShowModal() == mrOk)
	{
		if (frmAddStockCategoryGroup->Name == "")
		{
			Application->MessageBox("This stock category must have a name!", "Error", MB_OK | MB_ICONERROR);
		}
		else
		{
			frmAddStockCategoryGroup->Name = frmAddStockCategoryGroup->Name.SubString(0, MAX_STOCK_STOCK_GROUP_LENGTH);

			if (!Transaction->InTransaction) Transaction->StartTransaction();

			qrFindStockCategory->Close();
			qrFindStockCategory->ParamByName("Stock_Category")->AsString = frmAddStockCategoryGroup->Name.UpperCase();
			qrFindStockCategory->ParamByName("Stock_Category_Key")->AsInteger = 0;
			qrFindStockCategory->ExecQuery();
			if (!qrFindStockCategory->Eof)
			{
				Application->MessageBox("This stock category already exists!", "Error", MB_OK | MB_ICONERROR);
			}
			else
			{
				TTreeNode *NewNode = NULL;

				if (tvStock->Selected && ((TStockNodeData *)tvStock->Selected->Data)->Type == snStockCategory)
				{
					NewNode = tvStock->Items->Insert(tvStock->Selected, frmAddStockCategoryGroup->Name);
				}
				else if (tvStock->Selected && ((TStockNodeData *)tvStock->Selected->Data)->Type == snStockGroup)
				{
					NewNode = tvStock->Items->Insert(tvStock->Selected->Parent, frmAddStockCategoryGroup->Name);
				}
				else
				{
					NewNode = tvStock->Items->Add(NULL, frmAddStockCategoryGroup->Name);
				}

				try
				{
					sqlAddLK->Close();
					sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_STOCK_CATEGORY_KEY, 1) FROM RDB$DATABASE";
					sqlAddLK->ExecQuery();

					qrUpdateStockCategory->Close();
					qrUpdateStockCategory->SQL->Text =
					"Insert Into StockCategory ("
					"Stock_Category_Key,"
					"Stock_Category,"
					"GL_Code,"
					"Sort_Order) "
					"Values ("
					":Stock_Category_Key,"
					":Stock_Category,"
					":GL_Code,"
					":Sort_Order)";

					qrUpdateStockCategory->ParamByName("Stock_Category_Key")->AsInteger	= sqlAddLK->Fields[0]->AsInteger;
					qrUpdateStockCategory->ParamByName("Stock_Category")->AsString			= frmAddStockCategoryGroup->Name;
					qrUpdateStockCategory->ParamByName("GL_Code")->AsString					= frmAddStockCategoryGroup->GLCode;
					qrUpdateStockCategory->ParamByName("Sort_Order")->AsInteger				= 0;
					qrUpdateStockCategory->ExecQuery();

					/*					TStockNodeData *StockNodeData	= new TStockNodeData(snStockCategory);
					NewNode->Data						= StockNodeData;

					StockNodeData->Key				= qrUpdateStockCategory->ParamByName("Stock_Category_Key")->AsInteger;
					StockNodeData->Deleted			= false;
					StockNodeData->Description		= frmAddStockCategoryGroup->Name;
					StockNodeData->GLCode			= frmAddStockCategoryGroup->GLCode;
*/
					CreateStockCategory(NewNode,
					frmAddStockCategoryGroup->Name,
					qrUpdateStockCategory->ParamByName("Stock_Category_Key")->AsInteger,
					frmAddStockCategoryGroup->GLCode,
					false);

					SetSortOrder();
					if (Transaction->InTransaction) Transaction->Commit();

					NewNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
					NewNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
					tvStock->Selected			= NewNode;

					//						NewNode->EditText();
				}
				catch (EIBError &E)
				{
					if (Transaction->InTransaction)
					{
						Transaction->Rollback();
						NewNode->Delete();
					}
					throw;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::miNewStockGroupClick(TObject *Sender)
{
	AnsiString NewStockGroupName = "New Group";
	if (FindStockGroup(NewStockGroupName))
	{
		int i = 2;
		NewStockGroupName = "New Group (" + IntToStr(i) + ")";
		while (FindStockGroup(NewStockGroupName))
		{
			i++;
			NewStockGroupName = "New Group (" + IntToStr(i) + ")";
		}
	}
	TTreeNode *NewNode = NULL;

	if (tvStock->Selected)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)tvStock->Selected->Data;
		if (StockNodeData->Type == snStockCategory)
		{
			NewNode = tvStock->Items->AddChild(tvStock->Selected, NewStockGroupName);
		}
		else if (StockNodeData->Type == snStockGroup)
		{
			NewNode = tvStock->Items->Insert(tvStock->Selected, NewStockGroupName);
		}
		else if (StockNodeData->Type == snStockItem)
		{
			NewNode = tvStock->Items->Insert(tvStock->Selected->Parent, NewStockGroupName);
		}
	}
	else
	{
		return;
	}

	try
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();

		TStockNodeData *CategoryData = (TStockNodeData *)NewNode->Parent->Data;

		sqlAddLK->Close();
		sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_STOCK_GROUP_KEY, 1) FROM RDB$DATABASE";
		sqlAddLK->ExecQuery();

		qrUpdateStockGroup->Close();
		qrUpdateStockGroup->SQL->Text =
		"Insert Into StockGroup ("
		"Stock_Group_Key,"
		"Stock_Category_Key,"
		"Sort_Order,"
		"Stock_Group) "
		"Values ("
		":Stock_Group_Key,"
		":Stock_Category_Key,"
		":Sort_Order,"
		":Stock_Group)";

		qrUpdateStockGroup->ParamByName("Stock_Group_Key")->AsInteger		= sqlAddLK->Fields[0]->AsInteger;
		qrUpdateStockGroup->ParamByName("Stock_Category_Key")->AsInteger	= CategoryData->Key;
		qrUpdateStockGroup->ParamByName("Sort_Order")->AsInteger				= 0;
		qrUpdateStockGroup->ParamByName("Stock_Group")->AsString				= NewStockGroupName;
		qrUpdateStockGroup->ExecQuery();

		/*		TStockNodeData *StockNodeData	= new TStockNodeData(snStockGroup);
		NewNode->Data						= StockNodeData;

		StockNodeData->Key				= qrUpdateStockGroup->ParamByName("Stock_Group_Key")->AsInteger;
		StockNodeData->Deleted			= false;
		StockNodeData->Description		= NewStockGroupName;
*/
		CreateStockGroup(NewNode,
		CategoryData->Description,
		CategoryData->Key,
		"",
		NewStockGroupName,
		qrUpdateStockGroup->ParamByName("Stock_Group_Key")->AsInteger,
		false);

		SetSortOrder();
		if (Transaction->InTransaction) Transaction->Commit();

		tvStock->Selected			= NewNode;
		NewNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
		NewNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
		NewNode->EditText();
	}
	catch (EIBError &E)
	{
		if (Transaction->InTransaction)
		{
			Transaction->Rollback();
			NewNode->Delete();
		}
		throw;
	}
}
//---------------------------------------------------------------------------
bool	TfrmStock::FindStockCategory(AnsiString Text)
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	qrUpdateStock->Close();
	qrUpdateStock->SQL->Text =
	"Select "
	"Stock_Category_Key "
	"From "
	"StockCategory "
	"Where "
	"Stock_Category = :Stock_Category";
	qrUpdateStock->ParamByName("Stock_Category")->AsString = Text;
	qrUpdateStock->ExecQuery();
	bool Found = (!qrUpdateStock->Eof);
	qrUpdateStock->Close();
	Transaction->Commit();
	return Found;
	/*	TTreeNode *CurItem = tvStock->Items->GetFirstNode();
	while (CurItem)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)CurItem->Data;
		if (StockNodeData->Type == snStockCategory && CurItem->Text == Text && !StockNodeData->Deleted)
		{
			return CurItem;
		}
		CurItem = CurItem->GetNext();
	}
	return NULL; */
}
//---------------------------------------------------------------------------
bool TfrmStock::FindStockGroup(AnsiString Text)
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	qrUpdateStock->Close();
	qrUpdateStock->SQL->Text =
	"Select "
	"Stock_Group_Key "
	"From "
	"StockGroup "
	"Where "
	"Stock_Group = :Stock_Group";
	qrUpdateStock->ParamByName("Stock_Group")->AsString = Text;
	qrUpdateStock->ExecQuery();
	bool Found = (!qrUpdateStock->Eof);
	qrUpdateStock->Close();
	Transaction->Commit();
	return Found;
	/*
	TTreeNode *CurItem = tvStock->Items->GetFirstNode();
	while (CurItem)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)CurItem->Data;
		if (StockNodeData->Type == snStockGroup && CurItem->Text == Text && !StockNodeData->Deleted)
		{
			return CurItem;
		}
		CurItem = CurItem->GetNext();
	}
	return NULL;*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::miNewStockItemClick(TObject *Sender)
{
	if (!Registered)
	{
		if (GetStockItemCount() >= MAX_STOCK_ITEMS_IN_EVALUATION_MODE)
		{
			Application->MessageBox(AnsiString("You are limited to " + IntToStr(MAX_STOCK_ITEMS_IN_EVALUATION_MODE) + " items in the evaluation version.").c_str(), "Error", MB_ICONERROR + MB_OK);
			return;
		}
	}
	AnsiString CurrentItemUnit, CurrentGLCode;
	if (tvStock->Selected && ((TStockNodeData *)tvStock->Selected->Data)->Type == snStockItem)
	{
		StockCategoryKey		= ((TStockNodeData *)tvStock->Selected->Parent->Parent->Data)->Key;
		StockGroupKey			= ((TStockNodeData *)tvStock->Selected->Parent->Data)->Key;
		StockKey					= ((TStockNodeData *)tvStock->Selected->Data)->Key;
		StockSelectedDeleted	= ((TStockNodeData *)tvStock->Selected->Data)->Deleted;
		CurrentItemUnit		= lbeUnit->Caption;
	}
	else if (tvStock->Selected && ((TStockNodeData *)tvStock->Selected->Data)->Type == snStockGroup)
	{
		StockCategoryKey		= ((TStockNodeData *)tvStock->Selected->Parent->Data)->Key;
		StockGroupKey			= ((TStockNodeData *)tvStock->Selected->Data)->Key;
		StockKey					= NULL;
		StockSelectedDeleted	= ((TStockNodeData *)tvStock->Selected->Data)->Deleted;
		CurrentItemUnit		= "";
	}
	else
	{
		Application->MessageBox("You must select a stock group first.", "Error", MB_ICONERROR + MB_OK);
		return;
	}
	AnsiString NewStockName = NewItemName;
	if (FindStockItem(NewStockName))
	{
		int i = 2;
		NewStockName = NewItemName + " (" + IntToStr(i) + ")";
		while (FindStockItem(NewStockName))
		{
			i++;
			NewStockName = NewItemName + " (" + IntToStr(i) + ")";
		}
	}
	frmAddStock->StockKey			= StockKey;
	frmAddStock->StockGroupKey		= StockGroupKey;
	frmAddStock->DefaultUnit		= CurrentItemUnit;
	frmAddStock->NewItemName		= NewStockName;
	frmAddStock->Mode					= amAddStock;

	if (frmAddStock->ShowModal()	== mrOk)
	{
		// Find which category it actually went into.
		bool Found = false;
		TTreeNode *CurCat = tvStock->Items->GetFirstNode();
		while (CurCat)
		{
			TStockNodeData *CategoryData = (TStockNodeData *)CurCat->Data;
			if (CategoryData->Type == snStockCategory)
			{
				TTreeNode *CurGroup = CurCat->getFirstChild();
				while (CurGroup)
				{
					TStockNodeData *GroupData = (TStockNodeData *)CurGroup->Data;
					if (GroupData->Type == snStockGroup && GroupData->Key == frmAddStock->StockGroupKey)
					{
						StockCategoryKey	= CategoryData->Key;
						Found					= true;
						break;
					}
					CurGroup = CurGroup->getNextSibling();
				}
				if (Found) break;
			}
			CurCat = CurCat->getNextSibling();
		}
		StockGroupKey					= frmAddStock->StockGroupKey;
		StockKey							= frmAddStock->StockKey;
		StockSelectedDeleted			= false;
		RefreshStockGroup();
	}
}
//---------------------------------------------------------------------------
bool TfrmStock::FindStockItem(AnsiString Text)
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	qrUpdateStock->Close();
	qrUpdateStock->SQL->Text =
	"Select "
	"Stock_Key "
	"From "
	"Stock "
	"Where "
	"Description = :Description";
	qrUpdateStock->ParamByName("Description")->AsString = Text;
	qrUpdateStock->ExecQuery();
	bool Found = (!qrUpdateStock->Eof);
	qrUpdateStock->Close();
	Transaction->Commit();
	return Found;
	/*	TTreeNode *CurItem = tvStock->Items->GetFirstNode();
	while (CurItem)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)CurItem->Data;
		if (StockNodeData->Type == snStockItem && CurItem->Text == Text)
		{
			return CurItem;
		}
		CurItem = CurItem->GetNext();
	}
	return NULL;*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tvStockKeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{
	if (Key == VK_DELETE && !tvStock->IsEditing())
	{
		btnDeleteClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::miDeleteClick(TObject *Sender)
{
	btnDeleteClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::miRenameClick(TObject *Sender)
{
	if (tvStock->Selected)
	{
		tvStock->Selected->EditText();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tvStockDragDrop(TObject *Sender,
TObject *Source, int X, int Y)
{
	if (Source != (TObject *)tvStock || tvStock->Selected == NULL)
	{
		return;
	}

	THitTests HT = tvStock->GetHitTestInfoAt(X, Y);
	TTreeNode *TargetNode = tvStock->DropTarget;

	TStockNodeData *TargetData = NULL;
	if (TargetNode)
	{
		TargetData = (TStockNodeData *)TargetNode->Data;
	}
	TStockNodeData *SourceData = (TStockNodeData *)tvStock->Selected->Data;

	//Moving a StockCategory
	if (SourceData->Type == snStockCategory)
	{
		TNodeAttachMode AttachMode;
		if (HT.Contains(htNowhere))
		{
			AttachMode = naAdd;
			TargetNode = NULL;
		}
		else if (TargetData)
		{
			if (TargetData->Type == snStockCategory)
			{
				AttachMode = naInsert;
			}
			else
			{
				return;
			}
		}
		else
		{
			AttachMode = naAdd;
		}
		bool Expanded = tvStock->Selected->Expanded;
		tvStock->Items->BeginUpdate();
		tvStock->Selected->MoveTo(TargetNode, AttachMode);
		if (Expanded)
		{
			tvStock->Selected->Expand(false);
		}
		tvStock->Items->EndUpdate();
		try
		{
			if (!Transaction->InTransaction) Transaction->StartTransaction();
			SetSortOrder();
			Transaction->Commit();
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction) Transaction->Rollback();
			throw;
		}
	}
	//Moving a StockGroup
	else if (SourceData->Type == snStockGroup)
	{
		TNodeAttachMode AttachMode;
		if (!HT.Contains(htNowhere) && TargetData)
		{
			if (TargetData->Type == snStockGroup)
			{
				AttachMode = naInsert;
			}
			else if (TargetData->Type == snStockCategory)
			{
				AttachMode = naAddChild;
			}
			else
			{
				return;
			}
		}
		else
		{
			return;
		}
		bool Expanded = tvStock->Selected->Expanded;
		tvStock->Items->BeginUpdate();
		tvStock->Selected->MoveTo(TargetNode, AttachMode);
		if (Expanded)
		{
			tvStock->Selected->Expand(false);
		}
		tvStock->Items->EndUpdate();
		try
		{
			if (!Transaction->InTransaction) Transaction->StartTransaction();

			qrUpdateStockGroup->Close();
			qrUpdateStockGroup->SQL->Text =
			"Update "
			"StockGroup "
			"Set "
			"Stock_Category_Key = :Stock_Category_Key "
			"Where "
			"Stock_Group_Key  = :Stock_Group_Key";

			if (TargetData->Type == snStockGroup)
			{
				qrUpdateStockGroup->ParamByName("Stock_Category_Key")->AsInteger	= ((TStockNodeData *)TargetNode->Parent->Data)->Key;
			}
			else if (TargetData->Type == snStockCategory)
			{
				qrUpdateStockGroup->ParamByName("Stock_Category_Key")->AsInteger	= ((TStockNodeData *)TargetNode->Data)->Key;
			}
			qrUpdateStockGroup->ParamByName("Stock_Group_Key")->AsString			= ((TStockNodeData *)tvStock->Selected->Data)->Key;
			qrUpdateStockGroup->ExecQuery();

			SetSortOrder();
			Transaction->Commit();
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction) Transaction->Rollback();
			throw;
		}
	}
	// Moving a stock item
	else if (SourceData->Type == snStockItem)
	{
		if (!HT.Contains(htNowhere) && TargetData)
		{
			TTreeNode *NewStockGroupNode = NULL;
			if (TargetData && TargetData->Type == snStockGroup)
			{
				NewStockGroupNode	= TargetNode;
			}
			else if (TargetData && TargetData->Type == snStockItem)
			{
				NewStockGroupNode	= TargetNode->Parent;
			}
			else
			{
				return;
			}
			StockCategoryKey		= ((TStockNodeData *)NewStockGroupNode->Parent->Data)->Key;
			StockGroupKey			= ((TStockNodeData *)NewStockGroupNode->Data)->Key;
			StockKey					= ((TStockNodeData *)tvStock->Selected->Data)->Key;
			StockSelectedDeleted	= ((TStockNodeData *)tvStock->Selected->Data)->Deleted;
			try
			{
				if (!Transaction->InTransaction) Transaction->StartTransaction();

				qrUpdateStockGroup->Close();
				qrUpdateStockGroup->SQL->Text = "Update Stock Set Stock_Group_Key = :Stock_Group_Key Where Stock_Key = :Stock_Key";

				qrUpdateStockGroup->ParamByName("Stock_Group_Key")->AsInteger	= StockGroupKey;
				qrUpdateStockGroup->ParamByName("Stock_Key")->AsInteger			= ((TStockNodeData *)tvStock->Selected->Data)->Key;
				qrUpdateStockGroup->ExecQuery();

				//				qrStockGroup->Close();
				//				qrStock->Close();

				TTreeNode *NodeToDelete = tvStock->Selected;
				Transaction->Commit();

				StockSelectedDeleted = false;
				RefreshStockGroup();
				if (NodeToDelete)
				{
					NodeToDelete->Delete();
				}
			}
			catch (Exception &E)
			{
				if (Transaction->InTransaction) Transaction->Rollback();
				throw;
			}
		}
		else
		{
			return;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::miEditClick(TObject *Sender)
{
	if (tvStock->Selected)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)tvStock->Selected->Data;

		if (StockNodeData->Type == snStockItem)
		{
			if (((TStockNodeData *)tvStock->Selected->Parent->Data)->Type == snDeletedBin)
			{
				if (Application->MessageBox("Do you wish to restore this item?", "Question", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON1) == ID_OK)
				{
					btnRestoreClick(NULL);
				}
			}
			else
			{
				StockCategoryKey					= ((TStockNodeData *)tvStock->Selected->Parent->Parent->Data)->Key;
				StockGroupKey						= ((TStockNodeData *)tvStock->Selected->Parent->Data)->Key;
				StockKey								= StockNodeData->Key;
				StockSelectedDeleted				= StockNodeData->Deleted;

				frmAddStock->StockKey			= StockKey;
				frmAddStock->StockGroupKey		= StockGroupKey;

				frmAddStock->Mode					= amEditStock;
				if (frmAddStock->ShowModal()	== mrOk)
				{
					// Find which category it actually went into.
					TTreeNode *CurItem = tvStock->Items->GetFirstNode();
					while (CurItem)
					{
						TStockNodeData *StockNodeData = (TStockNodeData *)CurItem->Data;
						if (StockNodeData->Type == snStockGroup && StockNodeData->Key == frmAddStock->StockGroupKey)
						{
							StockCategoryKey	= ((TStockNodeData *)CurItem->Parent->Data)->Key;
						}
						CurItem = CurItem->GetNext();
					}
					StockGroupKey = frmAddStock->StockGroupKey;

					tvStock->Selected->Delete();
					StockSelectedDeleted = false;
					RefreshStockGroup();
				}
			}
		}
		else if (StockNodeData->Type == snStockCategory)
		{
			if (!Transaction->InTransaction) Transaction->StartTransaction();
			try
			{
				//				qrStockCategory->Open();
				//				if (qrStockCategory->Locate("Stock_Category_Key", StockNodeData->Key, TLocateOptions()))
				//				{
				frmAddStockCategoryGroup->StockGroupType	= gtCategory;
				frmAddStockCategoryGroup->Name				= StockNodeData->Description;//qrStockCategory->FieldByName("Stock_Category")->AsString;
				frmAddStockCategoryGroup->GLCode				= StockNodeData->GLCode;//qrStockCategory->FieldByName("GL_Code")->AsString;
				if (frmAddStockCategoryGroup->ShowModal() == mrOk)
				{
					if (frmAddStockCategoryGroup->Name == "")
					{
						Application->MessageBox("This stock category must have a name!", "Error", MB_OK | MB_ICONERROR);
					}
					else
					{
						frmAddStockCategoryGroup->Name = frmAddStockCategoryGroup->Name.SubString(0, MAX_STOCK_STOCK_GROUP_LENGTH);
						qrFindStockCategory->Close();
						qrFindStockCategory->ParamByName("Stock_Category")->AsString = frmAddStockCategoryGroup->Name.UpperCase();
						qrFindStockCategory->ParamByName("Stock_Category_Key")->AsInteger = StockNodeData->Key;
						qrFindStockCategory->ExecQuery();
						if (!qrFindStockCategory->Eof)
						{
							Application->MessageBox("This stock category already exists!", "Error", MB_OK | MB_ICONERROR);
						}
						else
						{
							qrUpdateStockCategory->Close();
							qrUpdateStockCategory->SQL->Text = "Update StockCategory Set Stock_Category = :Stock_Category, GL_Code = :GL_Code Where Stock_Category_Key = :Stock_Category_Key";
							qrUpdateStockCategory->ParamByName("Stock_Category_Key")->AsInteger	= StockNodeData->Key;
							qrUpdateStockCategory->ParamByName("Stock_Category")->AsString			= frmAddStockCategoryGroup->Name;
							qrUpdateStockCategory->ParamByName("GL_Code")->AsString					= frmAddStockCategoryGroup->GLCode;
							qrUpdateStockCategory->ExecQuery();
							tvStock->Selected->Text		= frmAddStockCategoryGroup->Name;
							StockNodeData->Description	= frmAddStockCategoryGroup->Name;
							StockNodeData->GLCode		= frmAddStockCategoryGroup->GLCode;
							lbeTitle->Caption				= frmAddStockCategoryGroup->Name;
						}
					}
				}
				//				}
				Transaction->Commit();
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
		else if (StockNodeData->Type == snStockGroup)
		{
			tvStock->Selected->EditText();
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmStock::btnDeleteClick(TObject *Sender)
{
	if (tvStock->Selected)
	{

		TStockNodeData *StockNodeData = (TStockNodeData *)tvStock->Selected->Data;
		if (StockNodeData->Type == snDeletedBin)
		{
			if (ListView1->ItemIndex > -1)
			{
				tvStock->Selected = tvStock->Selected->Item[ListView1->ItemIndex];
				StockNodeData = (TStockNodeData *)tvStock->Selected->Data;
			}
		}
		if (StockNodeData->Type == snStockItem)
		{
			if (StockNodeData->Deleted)
			{
				if (Application->MessageBox("Are you sure you wish to permenantly remove this stock item?", "Question", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON2) == ID_OK)
				{
					try
					{
						StockCategoryKey	= ((TStockNodeData *)tvStock->Selected->Data)->CategoryKey;
						StockGroupKey		= ((TStockNodeData *)tvStock->Selected->Data)->GroupKey;
						StockKey				= ((TStockNodeData *)tvStock->Selected->Data)->Key;
						AnsiString stock_code = ((TStockNodeData *)tvStock->Selected->Data)->StockCode;

						if (!Transaction->InTransaction) Transaction->StartTransaction();

						qrUpdateStock->Close();
						qrUpdateStock->SQL->Text =
						"Select "
						"PurchaseStock.Purchase_Stock_LK "
						"From "
						"PurchaseStock Left Join PurchaseOrder On "
						"PurchaseStock.Order_FK = PurchaseOrder.Order_LK "
						"Where "
						"PurchaseStock.Code = :Code And "
						"PurchaseOrder.Received = 'F'";
						qrUpdateStock->ParamByName("Code")->AsString = stock_code;
						qrUpdateStock->ExecQuery();
						if (!qrUpdateStock->Eof)
						{
							Application->MessageBox("This item is currently used in a purchase order and cannot be deleted.",
							"Error", MB_ICONERROR + MB_OK);
							return;
						}

						if (Application->MessageBox(
									"Would you like to remove the recipes that use this item?",
									"Prune recipes",
									MB_YESNO) == IDYES) {
							RemoveRelatedRecipes(stock_code);
						}

						RemoveKickstart(RcItem, StockCategoryKey,
						StockGroupKey, StockKey);

						if (Transaction->InTransaction)
						Transaction->Commit();

						tvStock->Selected->Delete();
						if(Location == NULL || Location == "")
						{
							LoadTree();
						}
						else
						{
							LoadTreeforStockRequest();
						}
						RefreshDeletedBin();
					}
					catch (Exception &E)
					{
						if (Transaction->InTransaction)
						Transaction->Rollback();
						if(Location == NULL || Location == "")
						{
							LoadTree();
						}
						else
						{
							LoadTreeforStockRequest();
						}
						throw;
					}
				}
			}
			else
			{
				StockCategoryKey	= ((TStockNodeData *)tvStock->Selected->Parent->Parent->Data)->Key;
				StockGroupKey		= ((TStockNodeData *)tvStock->Selected->Parent->Data)->Key;
				StockKey				= ((TStockNodeData *)tvStock->Selected->Data)->Key;

				qrStockLocation->Close();
				qrStockLocation->ParamByName("Stock_Key")->AsInteger = StockKey;
				
				bool stillOnHand = false;
				bool stillOnOrder = false;
				bool salesPending = false;
				bool initialised = false;

				for (qrStockLocation->Open(); !qrStockLocation->Eof; qrStockLocation->Next())
				{
					if (qrStockLocation->FieldByName("On_Hand")->AsCurrency != 0)
					{
						stillOnHand = true;
					}
					if (qrStockLocation->FieldByName("On_Order")->AsCurrency != 0)
					{
						stillOnOrder = true;
					}
					if (qrStockLocation->FieldByName("Initialised")->AsString == "T")
					{
						initialised = true;
					}
					if (qrStockLocation->FieldByName("Sales_Pending")->AsCurrency != 0 ||
							qrStockLocation->FieldByName("WriteOffs_Pending")->AsCurrency != 0)
					{
						salesPending = true;
					}
				}

				bool canDeleteStock = true;

				if (stillOnHand || stillOnOrder || salesPending || initialised)
				{
					std::auto_ptr<TStringList> messages(new TStringList());

					if (stillOnHand)
					{
						messages->Add("There are still items on hand.");
					}

					if (stillOnOrder)
					{
						messages->Add("There are still items on order.");
					}

					if (salesPending)
					{
						messages->Add("There are still item sales pending.");
					}

					if (initialised)
					{
						messages->Add("The item is initialised for stocktake.");
					}

					if (Application->MessageBox(
								("This item should not be deleted as it is still in use.\r\r" + messages->Text).c_str(),
								"Warning",
								MB_ICONWARNING + MB_OKCANCEL + MB_DEFBUTTON2) != ID_OK)
					{
						canDeleteStock = false;
					}
				}

				if (canDeleteStock && Application->MessageBox(
							"Do you wish to delete this stock item?",
							"Question",
							MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON2) == ID_OK)
				{
					if (!Transaction->InTransaction) Transaction->StartTransaction();

					qrUpdateStock->Close();
					qrUpdateStock->SQL->Text =
					"Select "
					"Initialised "
					"From "
					"Stock Inner Join StockLocation On "
					"Stock.Stock_Key = StockLocation.Stock_Key "
					"Where "
					"StockLocation.Initialised = 'T' And "
					"Stock.Stock_Key = :StockKey";
					qrUpdateStock->ParamByName("StockKey")->AsInteger = StockKey;
					qrUpdateStock->ExecQuery();
					bool Continue = qrUpdateStock->Eof;
					if (Transaction->InTransaction) Transaction->Commit();

					if (!Continue)
					{
						Application->MessageBox("This item is currently initialised for a stock take and cannot be deleted.",
						"Error", MB_ICONERROR + MB_OK);
						return;
					}
					else
					{
						try
						{
							if (!Transaction->InTransaction) Transaction->StartTransaction();

							qrUpdateStock->Close();
							qrUpdateStock->SQL->Text =
							"Update "
							"Stock "
							"Set "
							"Deleted = 'T' "
							"Where "
							"Stock.Stock_Key = :Stock_Key";
							qrUpdateStock->ParamByName("Stock_Key")->AsInteger = StockKey;
							qrUpdateStock->ExecQuery();
							if (Transaction->InTransaction) Transaction->Commit();

							tvStock->Selected->Delete();
							RefreshDeletedBin();
						}
						catch (Exception &E)
						{
							if (Transaction->InTransaction)
							Transaction->Rollback();
							if(Location == NULL || Location == "")
							{
								LoadTree();
							}
							else
							{
								LoadTreeforStockRequest();
							}
							throw;
						}
					}
				}
			}
		}
		else if (StockNodeData->Type == snStockGroup)
		{

			if (Application->MessageBox("Do you wish to delete this stock group and all its items?", "Question", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON2) == ID_OK)
			{

				try
				{
					StockCategoryKey	= ((TStockNodeData *)tvStock->Selected->Parent->Data)->Key;
					StockGroupKey		= ((TStockNodeData *)tvStock->Selected->Data)->Key;

					if (!Transaction->InTransaction) Transaction->StartTransaction();

					qrUpdateStock->Close();
					qrUpdateStock->SQL->Text =
					"Update "
					"StockGroup "
					"Set "
					"Deleted = 'T' "
					"Where "
					"StockGroup.Stock_Group_Key = :Stock_Group_Key";
					qrUpdateStock->ParamByName("Stock_Group_Key")->AsInteger = StockGroupKey;
					qrUpdateStock->ExecQuery();

					qrUpdateStock->Close();
					qrUpdateStock->SQL->Text =
					"Update "
					"Stock "
					"Set "
					"Deleted = 'T' "
					"Where "
					"Stock.Stock_Group_Key = :Stock_Group_Key";
					qrUpdateStock->ParamByName("Stock_Group_Key")->AsInteger = StockGroupKey;
					qrUpdateStock->ExecQuery();

					if (Transaction->InTransaction) Transaction->Commit();

					tvStock->Selected->Delete();
					RefreshDeletedBin();
				}
				catch (Exception &E)
				{
					if (Transaction->InTransaction)
					Transaction->Rollback();
					if(Location == NULL || Location == "")
					{
						LoadTree();
					}
					else
					{
						LoadTreeforStockRequest();
					}
					throw;
				}
			}
		}
		else if (StockNodeData->Type == snStockCategory)
		{
			if (Application->MessageBox("Do you wish to delete this stock category and all its items?", "Question", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON2) == ID_OK)
			{
				try
				{
					StockCategoryKey	= ((TStockNodeData *)tvStock->Selected->Data)->Key;

					if (!Transaction->InTransaction) Transaction->StartTransaction();

					qrUpdateStock->Close();
					qrUpdateStock->SQL->Text =
					"Update "
					"StockCategory "
					"Set "
					"Deleted = 'T' "
					"Where "
					"StockCategory.Stock_Category_Key = :Stock_Category_Key";
					qrUpdateStock->ParamByName("Stock_Category_Key")->AsInteger = StockCategoryKey;
					qrUpdateStock->ExecQuery();

					qrUpdateStock->Close();
					qrUpdateStock->SQL->Text =
					"Update "
					"StockGroup "
					"Set "
					"Deleted = 'T' "
					"Where "
					"StockGroup.Stock_Category_Key = :Stock_Category_Key";
					qrUpdateStock->ParamByName("Stock_Category_Key")->AsInteger = StockCategoryKey;
					qrUpdateStock->ExecQuery();

					qrUpdateStock->Close();
					qrUpdateStock->SQL->Text =
					"Update "
					"Stock "
					"Set "
					"Deleted = 'T' "
					"Where "
					"Stock.Stock_Group_Key In ("
					"Select "
					"Stock_Group_Key "
					"From "
					"StockGroup "
					"Where "
					"StockGroup.Stock_Category_Key = :Stock_Category_Key) ";
					qrUpdateStock->ParamByName("Stock_Category_Key")->AsInteger = StockCategoryKey;
					qrUpdateStock->ExecQuery();

					if (Transaction->InTransaction) Transaction->Commit();

					tvStock->Selected->Delete();
					RefreshDeletedBin();
				}
				catch (Exception &E)
				{
					if (Transaction->InTransaction)
					Transaction->Rollback();
					if(Location == NULL || Location == "")
					{
						LoadTree();
					}
					else
					{
						LoadTreeforStockRequest();
					}
					throw;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnEmptyDeletedBinClick(TObject *Sender)
{
	ignorePendingDeletions = false;

	if (Application->MessageBox("Are you sure you wish to permenantly remove all deleted stock items?", "Question", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON2) == ID_OK)
	{
		try
		{
			if (!Transaction->InTransaction)
			Transaction->StartTransaction();

			qrUpdateStock->Close();
			qrUpdateStock->SQL->Text =
			"Select "
			"PurchaseStock.Purchase_Stock_LK "
			"From "
			"PurchaseStock Inner Join Stock On "
			"PurchaseStock.Code = Stock.Code "
			"Left Join PurchaseOrder On "
			"PurchaseStock.Order_FK = PurchaseOrder.Order_LK "
			"Where "
			"Stock.Deleted = 'T' And "
			"PurchaseOrder.Received = 'F'";
			qrUpdateStock->ExecQuery();
			if (!qrUpdateStock->Eof)
			{
				ignorePendingDeletions = true;
				Application->MessageBox("Some items are currently used in a purchase order and cannot be deleted.",
				"Error", MB_ICONERROR + MB_OK);
				return;
			}

			if (Application->MessageBox("Would you like to remove recipes related to these items?",
						"Recipe Pruning", MB_YESNO) == IDYES)
			RemoveRelatedRecipes();

			qrUpdateStock->Close();
			qrUpdateStock->SQL->Text = "delete from stock where deleted = 'T';";
			qrUpdateStock->ExecQuery();
			qrUpdateStock->Close();
			qrUpdateStock->SQL->Text = "delete from stockgroup where deleted = 'T';";
			qrUpdateStock->ExecQuery();
			qrUpdateStock->Close();
			qrUpdateStock->SQL->Text = "delete from stockcategory where deleted = 'T';";
			qrUpdateStock->ExecQuery();

			if (Transaction->InTransaction)
			Transaction->Commit();

			RefreshDeletedBin();
			if(Location == NULL || Location == "")
			{
				LoadTree();
			}
			else
			{
				LoadTreeforStockRequest();
			}
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction)
			Transaction->Rollback();
			if(Location == NULL || Location == "")
			{
				LoadTree();
			}
			else
			{
				LoadTreeforStockRequest();
			}
			throw;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnEditClick(TObject *Sender)
{
	miEditClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnAddStockClick(TObject *Sender)
{
	tvStock->SetFocus();
	miNewStockItemClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnAddStockGroupClick(TObject *Sender)
{
	tvStock->SetFocus();
	miNewStockGroupClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnRenameStockGroupClick(TObject *Sender)
{
	if (tvStock->Selected)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)tvStock->Selected->Data;
		if (StockNodeData->Type == snStockCategory)
		{
			if (lbStockGroups->ItemIndex > -1)
			{
				tvStock->Selected = tvStock->Selected->Item[lbStockGroups->ItemIndex];
				tvStock->SetFocus();
				miRenameClick(NULL);
			}
		}
		if (StockNodeData->Type == snStockGroup)
		{
			tvStock->SetFocus();
			miRenameClick(NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnDeleteStockGroupClick(TObject *Sender)
{
	tvStock->SetFocus();
	btnDeleteClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnEditStockClick(TObject *Sender)
{
	if (tvStock->Selected && lbStockItems->ItemIndex > -1)
	{
		tvStock->Selected = tvStock->Selected->Item[lbStockItems->ItemIndex];
		tvStock->SetFocus();
		miEditClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::lbStockItemsDblClick(TObject *Sender)
{
	if (tvStock->Selected && lbStockItems->ItemIndex > -1)
	{
		tvStock->Selected = tvStock->Selected->Item[lbStockItems->ItemIndex];
		if (StockMode == smSelect)
		{
			btnOkClick(NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tvStockEndDrag(TObject *Sender, TObject *Target,
int X, int Y)
{
	tiTreeScroll->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnFindClick(TObject *Sender)
{
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::FindDialogFind(TObject *Sender)
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
void __fastcall TfrmStock::tvStockKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		tvStockDblClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::lbStockItemsDrawItem(TWinControl *Control,
int Index, TRect &Rect, TOwnerDrawState State)
{
	TListBox *ListBox = (TListBox *)Control;

	ListBox->Canvas->FillRect(Rect);
	dmUtilities->imlTreeImages->Draw(ListBox->Canvas, Rect.Left + 2, Rect.Top, StockItemIconIndex);
	//dmUtilities->imlTreeImages->Draw(ListBox->Canvas, Rect.Left + 2, Rect.Top, ICON_BOX_INDEX);
	ListBox->Canvas->TextOut(Rect.Left + dmUtilities->imlTreeImages->Width + 6, Rect.Top, ListBox->Items->Strings[Index]);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::GridDrawCell(TObject *Sender, int ACol,
int ARow, TRect &Rect, TGridDrawState State)
{
	TStringGrid *Grid = (TStringGrid *)Sender;
	if (!State.Contains(gdFixed))
	{
		Grid->Canvas->Brush->Color = Grid->Color;
		Grid->Canvas->Font->Color = Grid->Font->Color;
		Grid->Canvas->FillRect(Rect);
	}
	Grid->Canvas->TextRect(Rect, Rect.Left+2, Rect.Top+2, Grid->Cells[ACol][ARow]);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::Splitter1CanResize(TObject *Sender,
int &NewSize, bool &Accept)
{
	if (NewSize < Panel8->Constraints->MinWidth)
	{
		NewSize = Panel8->Constraints->MinWidth;
		Accept = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::Button1Click(TObject *Sender)
{
	miNewStockGroupClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::Splitter1Moved(TObject *Sender)
{
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::FormResize(TObject *Sender)
{
	ResizeGrids();
}
//---------------------------------------------------------------------------
/*void TfrmStock::InitCategories()
{
	if (tvStock->Selected)
	{
		if (tvStock->Selected->Level == TreeLevelCategory)
		{
			if (tvStock->Selected->Index == 0)
			{
				StockCategoryKey			= NULL;
			}
			else
			{
				StockCategoryKey			= ((TStockNodeData *)tvStock->Selected->Data)->Key;
			}
			StockGroupKey					= NULL;
			StockKey							= NULL;
		}
		else if (tvStock->Selected->Level == TreeLevelGroup)
		{
			StockCategoryKey				= ((TStockNodeData *)tvStock->Selected->Parent->Data)->Key;
			StockGroupKey					= ((TStockNodeData *)tvStock->Selected->Data)->Key;
			StockKey							= NULL;
		}
		else if (tvStock->Selected->Level == TreeLevelStock)
		{
			StockCategoryKey				= ((TStockNodeData *)tvStock->Selected->Parent->Parent->Data)->Key;
			StockGroupKey					= ((TStockNodeData *)tvStock->Selected->Parent->Data)->Key;
			StockKey							= ((TStockNodeData *)tvStock->Selected->Data)->Key;
		}
	}
	else
	{
		StockGroupKey						= NULL;
		StockCategoryKey					= NULL;
		StockKey								= NULL;
	}

	TreeLevelStock							= 2;
	TreeLevelGroup							= 1;
	TreeLevelCategory						= 0;

	btnAddCategory->Enabled				= true;
	btnMoveGroup->Enabled				= true;

	if (dmStockData->dbStock->Connected)
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		qrStock->Close();
		qrStockGroup->Close();
		qrStockCategory->Close();

		if (StockGroupKey)
		{
			// Find which Category node will be selected.
			qrStockGroup->DataSource	= NULL;
			qrStockGroup->SQL->Text		=
				"Select "
					"Stock_Group_Key,"
					"Stock_Group,"
					"Sort_Order,"
					"Stock_Category_Key,"
					"Stocktake_Visible "
				"From "
					"StockGroup "
				"Order by "
					"Sort_Order";
			qrStockGroup->Open();
			if (qrStockGroup->Locate("Stock_Group_Key", StockGroupKey, TLocateOptions()))
			{
				StockCategoryKey			= qrStockGroup->FieldByName("Stock_Category_Key")->AsInteger;
			}
		}
		// Now link groups to categories
		qrStockGroup->DataSource		= dsStockCategory;
		qrStockGroup->SQL->Text			=
			"Select "
				"Stock_Group_Key,"
				"Stock_Group,"
				"Sort_Order,"
				"Stock_Category_Key,"
				"Stocktake_Visible "
			"From "
				"StockGroup "
			"Where "
				"Stock_Category_Key = :Stock_Category_Key "
			"Order by "
				"Sort_Order";
		Transaction->Commit();
	}
}*/
//---------------------------------------------------------------------------
void __fastcall TfrmStock::lbStockGroupsDrawItem(TWinControl *Control,
int Index, TRect &Rect, TOwnerDrawState State)
{
	TListBox *ListBox = (TListBox *)Control;

	ListBox->Canvas->FillRect(Rect);
	dmUtilities->imlTreeImages->Draw(ListBox->Canvas, Rect.Left + 2, Rect.Top, ICON_CLOSED_FOLDER_INDEX);
	ListBox->Canvas->TextOut(Rect.Left + dmUtilities->imlTreeImages->Width + 6, Rect.Top, ListBox->Items->Strings[Index]);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnDeleteCategoryClick(TObject *Sender)
{
	tvStock->SetFocus();
	btnDeleteClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnRenameCategoryClick(TObject *Sender)
{
	tvStock->SetFocus();
	miEditClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnSortCategoriesClick(TObject *Sender)
{
	if (Application->MessageBox("Do you wish sort the categories to alphabetically?", "Question", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON1) == ID_OK)
	{
		tvStock->AlphaSort(false);
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		SetSortOrder();
		Transaction->Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnSortStockGroupsClick(TObject *Sender)
{
	if (Application->MessageBox("Do you wish sort the groups to alphabetically?", "Question", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON2) == ID_OK)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)tvStock->Selected->Data;
		if (StockNodeData->Type == snStockGroup)
		{
			tvStock->Selected->Parent->AlphaSort(false);
		}
		else if (StockNodeData->Type == snStockCategory)
		{
			tvStock->Selected->AlphaSort(false);

			lbStockGroups->Items->Clear();
			for (int i=0; i<tvStock->Selected->Count; i++)
			{
				lbStockGroups->Items->Add(tvStock->Selected->Item[i]->Text);
			}
		}
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		SetSortOrder();
		Transaction->Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnAddCategoryClick(TObject *Sender)
{
	tvStock->SetFocus();
	miNewStockCategoryClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::lbStockGroupsDblClick(TObject *Sender)
{
	if (tvStock->Selected && lbStockGroups->ItemIndex > -1)
	{
		tvStock->Selected = tvStock->Selected->Item[lbStockGroups->ItemIndex];
		tvStock->Selected->Expand(false);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::Splitter2CanResize(TObject *Sender,
int &NewSize, bool &Accept)
{
	if (NewSize < Panel5->Constraints->MinHeight)
	{
		Panel5->Height = Panel5->Constraints->MinHeight;
		Accept = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::ExpandTree1Click(TObject *Sender)
{
	if (tvStock->Selected)
	{
		tvStock->Selected->Expand(true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::CollapseTree1Click(TObject *Sender)
{
	if (tvStock->Selected)
	{
		tvStock->Selected->Collapse(true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tvStockEditing(TObject *Sender, TTreeNode *Node,
bool &AllowEdit)
{
	if (Node->Level == 0)
	{
		AllowEdit = (Node->Index != 0);
	}
	else if (Node->Level == 1)
	{
		AllowEdit = (Node->Parent->Index != 0);
	}
	else if (Node->Level == 2)
	{
		AllowEdit = (Node->Parent->Parent->Index != 0);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::tvStockDeletion(TObject *Sender,
TTreeNode *Node)
{	TStockNodeData *StockNodeData = (TStockNodeData *)Node->Data;
	delete StockNodeData;
	Node->Data = NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TfrmStock::LocateInTree(int StockCategoryKey, int StockGroupKey, int StockKey, bool Deleted)
{
	TTreeNode *NodeFound = NULL;
	if (StockKey != 0)
	{
		TTreeNode *CurItem = tvStock->Items->GetFirstNode();
		while (CurItem)
		{
			TStockNodeData *StockNodeData = (TStockNodeData *)CurItem->Data;
			if (StockNodeData->Type == snStockItem && StockNodeData->Key == StockKey)
			{
				NodeFound = CurItem;
				break;
			}
			CurItem = CurItem->GetNext();
		}
	}
	else if (StockGroupKey != 0)
	{
		TTreeNode *CurCat = tvStock->Items->GetFirstNode();
		while (CurCat)
		{
			TStockNodeData *NodeData = (TStockNodeData *)CurCat->Data;
			// Find the Deleted Bin and get first child
			if (NodeData->Type == snDeletedBin)
			{
				if (Deleted)
				{
					CurCat = CurCat->getFirstChild();
				}
				else
				{
					CurCat = CurCat->getNextSibling();
				}
			}
			// Or look in this category
			else
			{
				if (NodeData->Type == snStockCategory &&
						(Deleted && NodeData->Deleted) ||
						(!Deleted && !NodeData->Deleted))
				{
					TTreeNode *CurGroup = CurCat->getFirstChild();
					while (CurGroup)
					{
						TStockNodeData *GroupData = (TStockNodeData *)CurGroup->Data;
						if (GroupData->Type == snStockGroup && GroupData->Key == StockGroupKey && GroupData->Deleted == Deleted)
						{
							NodeFound = CurGroup;
							break;
						}
						CurGroup = CurGroup->getNextSibling();
					}
				}
				CurCat = CurCat->getNextSibling();
			}
			if (NodeFound) break;
		}
	}
	else if (StockCategoryKey != 0)
	{
		/*		TTreeNode *CurCat = tvStock->Items->GetFirstNode();
		while (CurCat)
		{
			TStockNodeData *CategoryData = (TStockNodeData *)CurCat->Data;
			if (CategoryData->Type == snStockCategory && CategoryData->Key == StockCategoryKey && CategoryData->Deleted == Deleted)
			{
				NodeFound = CurCat;
				break;
			}
			CurCat = CurCat->getNextSibling();
		} */
		TTreeNode *CurCat = tvStock->Items->GetFirstNode();
		while (CurCat)
		{
			TStockNodeData *NodeData = (TStockNodeData *)CurCat->Data;
			// Find the Deleted Bin and get first child
			if (NodeData->Type == snDeletedBin)
			{
				if (Deleted)
				{
					CurCat = CurCat->getFirstChild();
				}
				else
				{
					CurCat = CurCat->getNextSibling();
				}
			}
			// Or look in this category
			else
			{
				if (NodeData->Type == snStockCategory &&
						(Deleted && NodeData->Deleted) ||
						(!Deleted && !NodeData->Deleted))
				{
					if (NodeData->Type == snStockCategory && NodeData->Key == StockCategoryKey && NodeData->Deleted == Deleted)
					{
						NodeFound = CurCat;
						break;
					}
				}
				CurCat = CurCat->getNextSibling();
			}
		}
	}
	return NodeFound;
}
//---------------------------------------------------------------------------
void TfrmStock::SelectInTree(TTreeNode *Node)
{
	if (Node)
	{
		tvStock->Selected = Node;
		if (!tvStock->Selected)
		{
			tvStock->Selected = tvStock->Items->GetFirstNode();
		}
	}
	else
	{
		tvStock->Selected = tvStock->Items->GetFirstNode();
	}
	if (!tvStock->Selected)
	{
		lbeTitle->Caption = "";
		pcStockDetails->ActivePage = tsStockCategory;
	}
	else if (Node)
	{
		tvStock->Selected->Expand(false);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::miRestoreClick(TObject *Sender)
{
	if (tvStock->Selected)
	{
		TStockNodeData *StockNodeData = (TStockNodeData *)tvStock->Selected->Data;
		if (StockNodeData->Type == snStockItem)
		{
			try
			{
				if (!Transaction->InTransaction) Transaction->StartTransaction();

				StockCategoryKey	= StockNodeData->CategoryKey;
				StockGroupKey		= StockNodeData->GroupKey;
				StockKey				= StockNodeData->Key;

				qrUpdateStock->Close();
				qrUpdateStock->SQL->Text = "Update Stock Set Deleted = 'F' Where Stock_Key = :Stock_Key";
				qrUpdateStock->ParamByName("Stock_Key")->AsInteger = StockKey;
				qrUpdateStock->ExecQuery();

				qrUpdateStock->Close();
				qrUpdateStock->SQL->Text = "Update StockGroup Set Deleted = 'F' Where Stock_Group_Key = :Stock_Group_Key";
				qrUpdateStock->ParamByName("Stock_Group_Key")->AsInteger = StockGroupKey;
				qrUpdateStock->ExecQuery();

				qrUpdateStock->Close();
				qrUpdateStock->SQL->Text = "Update StockCategory Set Deleted = 'F' Where Stock_Category_Key = :Stock_Category_Key";
				qrUpdateStock->ParamByName("Stock_Category_Key")->AsInteger = StockCategoryKey;
				qrUpdateStock->ExecQuery();

				if (Transaction->InTransaction) Transaction->Commit();

				RefreshDeletedBin();
				//RefreshStockGroup();
				if(Location == NULL || Location == "")
				{
					LoadTree();
				}
				else
				{
					LoadTreeforStockRequest();
				}
				SelectInTree(LocateInTree(StockCategoryKey, StockGroupKey, StockKey, false));
				tvStock->SetFocus();
			}
			catch (Exception &E)
			{
				if (Transaction->InTransaction) Transaction->Rollback();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnRestoreClick(TObject *Sender)
{
	TStockNodeData *StockNodeData = (TStockNodeData *)tvStock->Selected->Data;
	if (StockNodeData->Type == snDeletedBin)
	{
		if (ListView1->ItemIndex > -1)
		{
			tvStock->Selected = tvStock->Selected->Item[ListView1->ItemIndex];
			StockNodeData = (TStockNodeData *)tvStock->Selected->Data;
		}
	}
	if (StockNodeData->Type == snStockItem && StockNodeData->Deleted)
	{
		try
		{
			StockCategoryKey	= ((TStockNodeData *)tvStock->Selected->Data)->CategoryKey;
			StockGroupKey		= ((TStockNodeData *)tvStock->Selected->Data)->GroupKey;
			StockKey				= ((TStockNodeData *)tvStock->Selected->Data)->Key;

			if (!Transaction->InTransaction) Transaction->StartTransaction();

			qrUpdateStock->Close();
			qrUpdateStock->SQL->Text =
			"Update "
			"Stock "
			"Set "
			"Deleted = 'F' "
			"Where "
			"Stock_Key = :Stock_Key";
			qrUpdateStock->ParamByName("Stock_Key")->AsInteger = StockKey;
			qrUpdateStock->ExecQuery();

			qrUpdateStock->Close();
			qrUpdateStock->SQL->Text =
			"Update "
			"StockGroup "
			"Set "
			"Deleted = 'F' "
			"Where "
			"Stock_Group_Key = :Stock_Group_Key";
			qrUpdateStock->ParamByName("Stock_Group_Key")->AsInteger = StockGroupKey;
			qrUpdateStock->ExecQuery();

			qrUpdateStock->Close();
			qrUpdateStock->SQL->Text =
			"Update "
			"StockCategory "
			"Set "
			"Deleted = 'F' "
			"Where "
			"Stock_Category_Key = :Stock_Category_Key";
			qrUpdateStock->ParamByName("Stock_Category_Key")->AsInteger = StockCategoryKey;
			qrUpdateStock->ExecQuery();

			if (Transaction->InTransaction)
			Transaction->Commit();

			if(Location == NULL || Location == "")
			{
				LoadTree();
			}
			else
			{
				LoadTreeforStockRequest();
			}
			SelectInTree(LocateInTree(StockCategoryKey, StockGroupKey, StockKey, false));
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction)
			Transaction->Rollback();
			if(Location == NULL || Location == "")
			{
				LoadTree();
			}
			else
			{
				LoadTreeforStockRequest();
			}
			throw;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::btnRestoreAllClick(TObject *Sender)
{
	if (Application->MessageBox("Do you wish to restore all deleted items?", "Question", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON1) == ID_OK)
	{
		try
		{
			if (!Transaction->InTransaction) Transaction->StartTransaction();

			qrUpdateStock->Close();
			qrUpdateStock->SQL->Text =
			"Update "
			"Stock "
			"Set "
			"Deleted = 'F' ";
			qrUpdateStock->ExecQuery();

			qrUpdateStock->Close();
			qrUpdateStock->SQL->Text =
			"Update "
			"StockGroup "
			"Set "
			"Deleted = 'F' ";
			qrUpdateStock->ExecQuery();

			qrUpdateStock->Close();
			qrUpdateStock->SQL->Text =
			"Update "
			"StockCategory "
			"Set "
			"Deleted = 'F' ";
			qrUpdateStock->ExecQuery();

			Transaction->Commit();

			if(Location == NULL || Location == "")
			{
				LoadTree();
			}
			else
			{
				LoadTreeforStockRequest();
			}
		}
		catch (Exception &E)
		{
			if (Transaction->InTransaction)
			Transaction->Rollback();
			if(Location == NULL || Location == "")
			{
				LoadTree();
			}
			else
			{
				LoadTreeforStockRequest();
			}
			throw;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmStock::ListView1DblClick(TObject *Sender)
{
	TStockNodeData *StockNodeData = (TStockNodeData *)tvStock->Selected->Data;
	if (StockNodeData->Type == snDeletedBin)
	{
		if (ListView1->ItemIndex > -1)
		{
			tvStock->Selected = tvStock->Selected->Item[ListView1->ItemIndex];
			StockNodeData = (TStockNodeData *)tvStock->Selected->Data;
			if (StockNodeData->Type == snStockItem && StockNodeData->Deleted)
			{
				if (Application->MessageBox("Do you wish to restore this item?", "Question", MB_ICONQUESTION + MB_OKCANCEL + MB_DEFBUTTON1) == ID_OK)
				{
					btnRestoreClick(NULL);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
