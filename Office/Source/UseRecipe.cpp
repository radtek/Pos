//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UseRecipe.h"
#include "CreateRecipe.h"
#include "StockData.h"
#include "MM_StockTransactions.h"
#include "Login.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmUseRecipe::TfrmUseRecipe(TComponent* Owner)
    : TForm(Owner), HideCosts(false) 
{
    vtvStock->NodeDataSize = sizeof(TRecipeNodeData);
	if (!Transaction->InTransaction)
    {
        Transaction->StartTransaction();
        IBTransaction1->StartTransaction();
    }

    if(IsSecretRecipe())
    {
        NumTotal->Visible = false;
        Label6->Visible = false;
        HideCosts = true;

    }
    IsSearchBoxClicked = false;
	dtRecipes->Close();
	dtRecipes->Open();
}
//---------------------------------------------------------------------------
void __fastcall TfrmUseRecipe::DBGridOnClick(TColumn *Column)
{
    RecipeName = dtRecipes->FieldByName("Recipe")->AsString;
    Label1->Caption = "Use Recipe: " + RecipeName;
    DisplayStock();
}
//---------------------------------------------------------------------------
void TfrmUseRecipe::DisplayStock(void)
{

    ItemPrices.clear();
    NumTotal->Value = 0;

    vtvStock->Clear();
	qrRecipe->Close();
	qrRecipe->ParamByName("Recipe")->AsString = RecipeName;
	qrRecipe->ParamByName("Deleted")->AsString = 'F';
	for (qrRecipe->Open(); !qrRecipe->Eof; qrRecipe->Next())
	{
        PVirtualNode RecipeNode						= NULL;
        TRecipeNodeData *NodeData				= NULL;
        double temp;

        RecipeNode = vtvStock->AddChild(NULL);
        NodeData = (TRecipeNodeData *)vtvStock->GetNodeData(RecipeNode);

        Edit1->Text = qrRecipe->FieldByName("Final_Stock")->AsString;
        Memo1->Text = qrRecipe->FieldByName("Description")->AsString;

	    Query->Close();
        Query->SQL->Text =
            "Select Stock_Key, Stocktake_Unit from Stock "
            "Where Description = :Description;";
        Query->ParamByName("Description")->AsString = Edit1->Text;
        Query->ExecQuery();
        if(Query->RecordCount)
            Label7->Caption = Query->FieldByName("Stocktake_Unit")->AsString;

        if(qrRecipe->FieldByName("Description")->AsString == "Description")
            Memo1->Text = "";

        RecipeID = qrRecipe->FieldByName("Recipes_ID")->AsInteger;

        ComboBox2->Clear();
        GetLocations(Edit1->Text, ComboBox2);
        ComboBox2->Text = qrRecipe->FieldByName("Final_Stock_Location")->AsString;

        NodeData->Text = qrRecipe->FieldByName("Required_Stock")->AsString;
        NodeData->Location = qrRecipe->FieldByName("Stock_Location")->AsString;
        NodeData->Qty = StrToFloat(FloatToStrF(qrRecipe->FieldByName("Stock_Qty")->AsFloat,ffFixed,19, CurrentConnection.SettingDecimalPlaces));
        NodeData->Unit = qrRecipe->FieldByName("Stock_Unit")->AsString;
        NodeData->Code = qrRecipe->FieldByName("Stock_Code")->AsString;

        PopulatePriceArray(qrRecipe->FieldByName("Stock_Location")->AsString, qrRecipe->FieldByName("Required_Stock")->AsString);
        temp = ItemPrices[NodeData->Text + "," + NodeData->Location] * NodeData->Qty;

        NumTotal->Value = NumTotal->Value + temp;
        NumTotal->DecimalPlaces = CurrentConnection.SettingDecimalPlaces;
        Filter = true;
	}
}
//---------------------------------------------------------------------------
void TfrmUseRecipe::PopulatePriceArray(AnsiString Location, AnsiString Item)
{
    AnsiString Key = Item + "," + Location;
    int Stock_Key;


    Query->Close();
    Query->SQL->Text =
        "Select Stock_Key from Stock "
        "Where Description = :Description;";
    Query->ParamByName("Description")->AsString = Item;
    Query->ExecQuery();
    if(Query->RecordCount)
        Stock_Key = Query->FieldByName("Stock_Key")->AsInteger;

    Query->Close();
    Query->SQL->Text =
        "Select Average_Cost, Latest_Cost from StockLocation "
        "Where Stock_Key = :Stock_Key And Location = :Location";
    Query->ParamByName("Stock_Key")->AsString = Stock_Key;
    Query->ParamByName("Location")->AsString = Location;
    Query->ExecQuery();
    if(Query->RecordCount)
        ItemPrices[Key] = (Query->FieldByName("Average_Cost")->AsFloat);
}

//---------------------------------------------------------------------------
void __fastcall TfrmUseRecipe::vtvStockGetText(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
      WideString &CellText)
{
	TRecipeNodeData *NodeData = (TRecipeNodeData *)Sender->GetNodeData(Node);
  
	if (NodeData)
	{
        if(Filter)
        {
            NodeData->RecipeQty = NodeData->Qty * NumQty->Value;
        //    NodeData->AverageCost = FloatToStrF((NodeData->RecipeQty * ItemPrices[NodeData->Text + "," + NodeData->Location]), ffCurrency, 19, 2);
            NodeData->AverageCost = (NodeData->RecipeQty * ItemPrices[NodeData->Text + "," + NodeData->Location]);
        }
		switch (Column)
		{
			case 0:	CellText = NodeData->Text;
						break;
			case 1:	CellText = NodeData->Location;
						break;
			case 2:	CellText = NodeData->Unit;
						break;
			case 3:	CellText = NodeData->Qty;

						break;
            case 4: CellText = NodeData->RecipeQty;
                        break;
            case 5: CellText = FloatToStrF(NodeData->AverageCost, ffCurrency, 19, CurrentConnection.SettingDecimalPlaces);
                    if(HideCosts)
                        CellText = "Unavailable";
                     break;
		}
    }
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::QuantityOnKeyPress(TObject *Sender,
      char &Key)
{
    if(Key == '-')
    {
       Key = NULL;
    }
    
    std::vector<AnsiString> LocationArray;
    int i = 0;

    if(vtvStock->IsEditing())
        vtvStock->EndEditNode();

    PVirtualNode Node = vtvStock->GetFirst();

    while (Node)
    {
        TRecipeNodeData *NodeData	= (TRecipeNodeData *)vtvStock->GetNodeData(Node);
        LocationArray.push_back(NodeData->Location);
        Node = vtvStock->GetNext(Node);
    }

    vtvStock->Clear();
	qrRecipe->Close();
	qrRecipe->ParamByName("Recipe")->AsString = RecipeName;
	qrRecipe->ParamByName("Deleted")->AsString = 'F';
	for (qrRecipe->Open(); !qrRecipe->Eof; qrRecipe->Next())
	{
        PVirtualNode RecipeNode						= NULL;
        TRecipeNodeData *NodeData				= NULL;
        double temp;

        RecipeNode = vtvStock->AddChild(NULL);
        NodeData = (TRecipeNodeData *)vtvStock->GetNodeData(RecipeNode);
        NodeData->Text = qrRecipe->FieldByName("Required_Stock")->AsString;
        NodeData->Location = LocationArray.at(i);
        NodeData->Qty = qrRecipe->FieldByName("Stock_Qty")->AsFloat;
        NodeData->Unit = qrRecipe->FieldByName("Stock_Unit")->AsString;
        NodeData->Code = qrRecipe->FieldByName("Stock_Code")->AsString;

        Filter = true;
        i++;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::vtvStockCreateEditor(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      IVTEditLink *EditLink)
{

	if (Node && Column == 1)
	{
 		TRecipeNodeData *NodeData = (TRecipeNodeData *)Sender->GetNodeData(Node);
        ComboBox1->Clear();
        GetLocations(NodeData->Text, ComboBox1);
		ComboBox1->Text = NodeData->Location;

		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, ComboBox1);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(ComboBox1->Handle, EM_SETSEL, 0, -1);
	}
	else if (Node && Column == 4)
	{
 		TRecipeNodeData *NodeData = (TRecipeNodeData *)Sender->GetNodeData(Node);
		NumericEdit1->Value = NodeData->RecipeQty;

		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, NumericEdit1);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(NumericEdit1->Handle, EM_SETSEL, 0, -1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::vtvStockEdited(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column)
{
	if (vtvStock->FocusedNode)
	{
        if (Column == 1)
		{
			TRecipeNodeData *NodeData	= (TRecipeNodeData *)vtvStock->GetNodeData(vtvStock->FocusedNode);
            for(int i = 0; i < Locations.size(); i++)
            {
                if(ComboBox1->Text == Locations.at(i))
                {
                    Filter = false;
                    PopulatePriceArray(ComboBox1->Text, NodeData->Text);
                    NodeData->Location = ComboBox1->Text;
                    NodeData->AverageCost = (ItemPrices[NodeData->Text + "," + NodeData->Location] * NodeData->RecipeQty);
                    vtvStock->InvalidateNode(vtvStock->FocusedNode);
                    UpdateTotal();
                    return;
                }
            }
            ComboBox1->Text = Locations.at(0);
            PopulatePriceArray(ComboBox1->Text, NodeData->Text);
			NodeData->Location = ComboBox1->Text;
        }
		else if (Column == 4)
		{
            Filter = false;
			TRecipeNodeData *NodeData	= (TRecipeNodeData *)vtvStock->GetNodeData(vtvStock->FocusedNode);
          
			NodeData->RecipeQty					= NumericEdit1->Value;
            NodeData->AverageCost = (ItemPrices[NodeData->Text + "," + NodeData->Location] * NumericEdit1->Value);
            UpdateTotal();
		}

		vtvStock->InvalidateNode(vtvStock->FocusedNode);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::vtvStockOnEditing(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column, bool &Allowed)
{
    Allowed = (Column == 4 || Column == 1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::vtvStockAfterPaint(TBaseVirtualTree *Sender,
      TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::vtvStockBeforePaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::vtvStockFocusChanged(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	if (Node && !Sender->IsEditing())
	{
		if (Sender->FocusedColumn != 4 && Sender->FocusedColumn != 1)
		{
			Sender->FocusedColumn = 1;
		}
		Sender->EditNode(Node, Column);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::vtvStockFocusChanging(
      TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
      TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}
}
//---------------------------------------------------------------------------

void TfrmUseRecipe::GetLocations(AnsiString Item, TComboBox *ComboBox)
{
        
    int Stock_Key;

	if (!Transaction->InTransaction) Transaction->StartTransaction();
    Locations.clear();

	Query->Close();
    Query->SQL->Text =
        "Select Stock_Key from Stock "
        "Where Description = :Description;";
    Query->ParamByName("Description")->AsString = Item;
    Query->ExecQuery();
    if(Query->RecordCount)
        Stock_Key = Query->FieldByName("Stock_Key")->AsInteger;
    Query->Close();


    Query->SQL->Text =
        "Select Location from StockLocation "
        "Where Stock_Key = :Stock_Key;";
    Query->ParamByName("Stock_Key")->AsInteger = Stock_Key;
    for(Query->ExecQuery(); !Query->Eof; Query->Next())
    {
        ComboBox->Items->AddObject(Query->FieldByName("Location")->AsString,
                                                            (TObject *)Stock_Key);
        Locations.push_back(Query->FieldByName("Location")->AsString);
    }

}
//-----------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::BtnOkOnClick(TObject *Sender)
{
    if(vtvStock->IsEditing())
        vtvStock->EndEditNode();

    PVirtualNode Node = vtvStock->GetFirst();


    if(!Node)
    {
        Application->MessageBox("Please choose a recipe to use", "error", MB_ICONINFORMATION + MB_OK);
        return;
    }
    if(!NumQty->Value)
    {
        Application->MessageBox("You cannot have a quantity of 0", "error", MB_ICONINFORMATION + MB_OK);
        return;
    }
    if( Application->MessageBox("Is this data correct? Please look at all locations and quantities to confirm that they are correct", "", MB_YESNO + MB_ICONWARNING) == IDYES)
    {
        UpdateDB();
        Close();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::BtnCancelOnClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void TfrmUseRecipe::UpdateDB(void)
{
    int RecipeUsedID;

    UpdatePrices(Edit1->Text, ComboBox2->Text);
    UpdateLocationsDB(Edit1->Text, ComboBox2->Text, NumQty->Value);
    UpdateDescription(RecipeID);

    PVirtualNode Node = vtvStock->GetFirst();

    try
    {
        int RecipeKey;
        AnsiString temp[4], tempFinalItem[4];

        Query->Close();
        Query->SQL->Text = "Select Gen_id(Gen_Stocktrans, 1) From rdb$database";
        Query->ExecQuery();
        RecipeKey = Query->Fields[0]->AsInteger;

        Stock::TManufactureStock ManufactureStock(dmStockData->dbStock, NumTotal->Value, RecipeKey);

        tempFinalItem[0] = ComboBox2->Text;
        tempFinalItem[1] = Edit1->Text;
        tempFinalItem[2] = Label7->Caption;
        tempFinalItem[3] = RecipeName;
        double UnitPrice = (double)NumTotal->Value/(double)NumQty->Value;

        ManufactureStock.UpdateStock(tempFinalItem, NumQty->Value, UnitPrice);

        while (Node)
        {
            TRecipeNodeData *NodeData	= (TRecipeNodeData *)vtvStock->GetNodeData(Node);
            UpdateLocationsDB(NodeData->Text, NodeData->Location, -NodeData->RecipeQty);

            temp[0] = NodeData->Location;
            temp[1] = NodeData->Text;
            temp[2] = NodeData->Unit;
            temp[3] = RecipeName;

            double AveCost = ItemPrices[NodeData->Text + "," + NodeData->Location];

            ManufactureStock.UpdateStock(temp, -NodeData->RecipeQty, AveCost, true);     // change for manufacture receipes qty for initial items......

            Node = vtvStock->GetNext(Node);
        }   
    }
	catch (Exception &E)
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}
//---------------------------------------------------------------------------

void TfrmUseRecipe::UpdateLocationsDB(AnsiString Item, AnsiString Location, double Qty)
{
    try
    {
        int Stock_Key;

        Query->Close();
        Query->SQL->Text =
            "Select Stock_Key from Stock "
            "Where Description = :Description;";
        Query->ParamByName("Description")->AsString = Item;
        Query->ExecQuery();
        if(Query->RecordCount)
            Stock_Key = Query->FieldByName("Stock_Key")->AsInteger;


        Query->Close();

        Query->SQL->Text =
            "Update StockLocation "
            "Set On_Hand = On_Hand + :Quantity "
            "Where Location = :Location and Stock_Key = :Stock_Key;";
        Query->ParamByName("Location")->AsString = Location;
        Query->ParamByName("Stock_Key")->AsInteger = Stock_Key;
        Query->ParamByName("Quantity")->AsFloat = Qty;
        Query->ExecQuery();
    }
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}
//-----------------------------------------------------------------------------

void TfrmUseRecipe::UpdateDescription(int Recipe_ID)
{
    Query->Close();
    Query->SQL->Text =
        "Update Recipes "
        "Set Description = :Description "
        "Where Recipes_ID = :Recipes_ID;";
    Query->ParamByName("Description")->AsString = Memo1->Text;
    Query->ParamByName("Recipes_ID")->AsInteger = Recipe_ID;
    Query->ExecQuery();
}
//---------------------------------------------------------------------------

void TfrmUseRecipe::UpdatePrices(AnsiString Item, AnsiString Location)
{
    try
    {
        int Stock_Key;
        double Price = 0;
        double OnHand = 0;
        Currency AverageCostNew = 0;

        Query->Close();
        Query->SQL->Text =
            "Select Stock_Key from Stock "
            "Where Description = :Description;";
        Query->ParamByName("Description")->AsString = Item;
        Query->ExecQuery();
        if(Query->RecordCount)
            Stock_Key = Query->FieldByName("Stock_Key")->AsInteger;

        Query->Close();
        Query->SQL->Text =
            "Select On_Hand, Average_Cost from StockLocation "
            "Where Location = :Location and Stock_Key = :Stock_Key";
        Query->ParamByName("Location")->AsString = Location;
        Query->ParamByName("Stock_Key")->AsInteger = Stock_Key;
        Query->ExecQuery();
        if(Query->RecordCount)
        {
            OnHand = Query->FieldByName("On_Hand")->AsFloat;
            Price = Query->FieldByName("Average_Cost")->AsFloat;
        }

        Query->Close();
        Query->SQL->Text =
            "Update StockLocation "
            "Set Average_Cost = :Average_Cost, Latest_Cost = :Latest_Cost "
            "Where Stock_Key = :Stock_Key And Location = :Location;";
        Query->ParamByName("Location")->AsString = Location;
        Query->ParamByName("Stock_Key")->AsInteger = Stock_Key;
        AverageCostNew =  ((OnHand * Price) + ((double)NumTotal->Value/(double)NumQty->Value * NumQty->Value)) / (OnHand + NumQty->Value);
        Query->ParamByName("Average_Cost")->AsCurrency = AverageCostNew > -1000000 ? AverageCostNew : 0;
        Query->ParamByName("Latest_Cost")->AsCurrency = (double)NumTotal->Value/(double)NumQty->Value;
        Query->ExecQuery();
    }
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::NumQtyOnChange(TObject *Sender)
{
	PVirtualNode Node = vtvStock->GetFirst();
    double temp = 0;
    NumTotal->Value = 0;

    while(Node)
    {
        TRecipeNodeData *NodeData	= (TRecipeNodeData *)vtvStock->GetNodeData(Node);

        temp = ItemPrices[NodeData->Text + "," + NodeData->Location] * NodeData->Qty * NumQty->Value;
        NumTotal->Value = NumTotal->Value + temp;

        Node = vtvStock->GetNext(Node);
    }

}
//---------------------------------------------------------------------------
void TfrmUseRecipe::UpdateTotal(void)
{
	PVirtualNode Node = vtvStock->GetFirst();
    double temp = 0;
    NumTotal->Value = 0;

    while(Node)
    {
        TRecipeNodeData *NodeData	= (TRecipeNodeData *)vtvStock->GetNodeData(Node);

        temp = ItemPrices[NodeData->Text + "," + NodeData->Location] * NodeData->RecipeQty;
        NumTotal->Value = NumTotal->Value + temp;

        Node = vtvStock->GetNext(Node);
    }


}
//---------------------------------------------------------------------------
bool TfrmUseRecipe::IsSecretRecipe(void)
{
    bool RetVal = false;
    MMQuery->Close();
    MMQuery->SQL->Text =
        "Select * from Registration Where Company Like 'Secret%';";
    MMQuery->ExecQuery();
    if(MMQuery->RecordCount)
        RetVal = true;
    MMQuery->Close();
    return RetVal;
}

//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::SearchBtnClick(TObject *Sender)
{

   AnsiString ReceipeName;
   ReceipeName =  SearchReceipe->Text;
   vtvStock->Clear();        
   ComboBox2->Clear();
   NumTotal->Value = 0;
   Edit1->Clear();
   Searchreceipe( ReceipeName);


   if( !IsSearchBoxClicked  )
   {
       loadreceipe();
   }

   
}
//---------------------------------------------------------------------------


 void TfrmUseRecipe::Searchreceipe(AnsiString ReceipeName)
  {
         dtRecipes->Close();
          dtRecipes->SelectSQL->Text =
          "Select Distinct Recipe  From Recipes Where Deleted = 'F'  AND LOWER(Recipes.Recipe )CONTAINING LOWER( '" + ReceipeName+"') order by Recipe ;";
         dtRecipes->Open();

         if(!dtRecipes->RecordCount && SearchReceipe->Text !="Search")
         {
            Label7->Caption = "";
            Memo1->Text = "";
           Application->MessageBox("No recipes found. Search again", "error", MB_ICONINFORMATION + MB_OK);
           SearchReceipe->Text = "";
          loadreceipe();
         }
         
  }

void __fastcall TfrmUseRecipe::SearchReceipeOnClick(TObject *Sender)
{
        
    if(!IsSearchBoxClicked)
    {
        SearchReceipe->Font->Style = TFontStyles() << fsBold;
        SearchReceipe->Font->Color = clBlack;
        SearchReceipe->Text = "";
        IsSearchBoxClicked = true;
    }

  if(SearchReceipe->Text =="")
 {
      loadreceipe();
     
  }
}

 void TfrmUseRecipe::loadreceipe()
 {
       dtRecipes->Close();
       dtRecipes->SelectSQL->Text =
     "  Select Distinct Recipe  From Recipes Where Deleted = 'F' order by Recipe ;";
       dtRecipes->Open();
 }


void __fastcall TfrmUseRecipe::NumTotalKeyPress(TObject *Sender, char &Key)
{
    if(Key == '-')
    {
       Key = NULL;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmUseRecipe::NumericEdit1KeyPress(TObject *Sender,
      char &Key)
{
    if(Key == '-')
    {
       Key = NULL;
    }
}
//---------------------------------------------------------------------------

