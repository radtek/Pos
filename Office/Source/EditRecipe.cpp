//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Connections.h"
#include "EditRecipe.h"

#include "SelectStockItem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
double Converted_value;
 String Unit;
//---------------------------------------------------------------------------
__fastcall TfrmEditRecipe::TfrmEditRecipe(TComponent* Owner)
    : TForm(Owner),
    StockLocation(""),
    ChangeInRecipe(false),
    RecipeName("")
 //	frmReceiveStockItem(new TfrmReceiveStockItem(NULL))
{
    vtvStock->NodeDataSize = sizeof(TRecipeNodeData);
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	dtRecipes->Close();                                                                             
	dtRecipes->Open();
    NumericEdit1->Enabled=false;
    NumericEdit1->DecimalPlaces=CurrentConnection.SettingDecimalPlaces;
    NumericEdit2->DecimalPlaces=CurrentConnection.SettingDecimalPlaces;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::DBGridOnClick(TColumn *Column)
{
    if(ChangeInRecipe && (Application->MessageBox("Would you like to save the changes made to your recipe?", "Warning",MB_YESNO) == ID_YES))
    {
        UpdateDB();
    }
    else
    {
        ChangeInRecipe = false;
    }

    NumericEdit2->Value = 0;
    RecipeName = dtRecipes->FieldByName("Recipe")->AsString;
    Label1->Caption = "Use Recipe: " + RecipeName;
    DisplayStock(RecipeName);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TfrmEditRecipe::PopulatePriceArray(AnsiString Location, AnsiString Item)
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
            "Select Average_Cost from StockLocation "
            "Where Stock_Key = :Stock_Key And Location = :Location";
        Query->ParamByName("Stock_Key")->AsString = Stock_Key;
        Query->ParamByName("Location")->AsString = Location;
        Query->ExecQuery();
        if(Query->RecordCount)
        {
            ItemPrices[Key] = (Query->FieldByName("Average_Cost")->AsFloat);
        }
}
//---------------------------------------------------------------------------
void TfrmEditRecipe::DisplayStock(AnsiString inRecipe)
{
    vtvStock->Clear();
	qrRecipe->Close();

	qrRecipe->ParamByName("Recipe")->AsString = inRecipe;
	qrRecipe->ParamByName("Deleted")->AsString = 'F';
	for (qrRecipe->Open(); !qrRecipe->Eof; qrRecipe->Next())
	{
        PVirtualNode RecipeNode						= NULL;
        TRecipeNodeData *NodeData				= NULL;

        RecipeNode = vtvStock->AddChild(NULL);
        NodeData = (TRecipeNodeData *)vtvStock->GetNodeData(RecipeNode);
        RecipeID = qrRecipe->FieldByName("Recipes_ID")->AsInteger;
        NodeData->Text = qrRecipe->FieldByName("Required_Stock")->AsString;
        NodeData->Location = qrRecipe->FieldByName("Stock_Location")->AsString;
        NodeData->Qty = StrToFloat(FloatToStrF(qrRecipe->FieldByName("Stock_Qty")->AsFloat,ffFixed,19, CurrentConnection.SettingDecimalPlaces));
        NodeData->Unit = qrRecipe->FieldByName("Stock_Unit")->AsString;
        NodeData->Code = qrRecipe->FieldByName("Stock_Code")->AsString;
        PopulatePriceArray(qrRecipe->FieldByName("Stock_Location")->AsString, qrRecipe->FieldByName("Required_Stock")->AsString);
        NodeData->AverageCost =ItemPrices[NodeData->Text + "," + NodeData->Location] * NodeData->Qty;
        NumericEdit2->Value = NumericEdit2->Value + RoundTo(NodeData->AverageCost,-CurrentConnection.SettingDecimalPlaces);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditRecipe::vtvStockGetText(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
      WideString &CellText)
{
	TRecipeNodeData *NodeData = (TRecipeNodeData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		switch (Column)
		{
			case 0:	CellText = NodeData->Text;
						break;
			case 1:	CellText = NodeData->Location;
						break;
			case 2:	CellText = NodeData->Unit;
						break;
			case 3:
                if(CurrentConnection.SettingDecimalPlaces==4)
                    CellText = FormatFloat("0.0000",NodeData->Qty);          //NodeData->Qty;
                else
                    CellText = FormatFloat("0.00",NodeData->Qty);
            break;
            case 4:
                if(CurrentConnection.SettingDecimalPlaces==4)
                   CellText = FormatFloat("0.0000",NodeData->AverageCost);  //NodeData->AverageCost; //NodeData->AverageCost;
                else
                   CellText = FormatFloat("0.00",NodeData->AverageCost);
            break;
		}
    }
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditRecipe::FindItemOnClick(TObject *Sender)
{
    if (!Transaction->InTransaction) Transaction->StartTransaction();

	frmSelectStockItem->Reset = true;
	if (frmSelectStockItem->ShowModal()					== mrOk)
	{
        qrDescription->Close();
        qrDescription->ParamByName("Code")->AsString = frmSelectStockItem->StockCode;
        qrDescription->Open();

	    if (QueryRecipe->Transaction->InTransaction)
		    QueryRecipe->Transaction->Commit();

        if (!Transaction->InTransaction) Transaction->StartTransaction();

        StockCode = frmSelectStockItem->StockCode;
        Edit1->Text = qrDescription->FieldByName("Description")->AsString;
        Label3->Caption = qrDescription->FieldByName("Stocktake_unit")->AsString;
        Label4->Caption = qrDescription->FieldByName("Reduction_unit")->AsString;
         NumericEdit1->Enabled=true;
           ((TEdit *)NumericEdit1)->Text = "0";
        ChangeFont(Edit1, eStockBox);
        cbLocation->Enabled = true;
        BoxExit(cbLocation);
        GetStockLocations(frmSelectStockItem->StockCode);
        CheckDefaultLocation();
    }

}
//---------------------------------------------------------------------------
void __fastcall TfrmEditRecipe::BoxExit(TObject *Sender)
{
    if((TNumericEdit *)Sender == NumericEdit1)
    {
        if(NumericEdit1->Value == 0)
        {
            ((TEdit *)NumericEdit1)->Text = "Recipe Quantity";
            NumericEdit1->Font->Style = TFontStyles() << fsItalic;
            NumericEdit1->Font->Color = clGray;
        }
    }
    else if((TComboBox *)Sender == cbLocation)
    {
        ((TComboBox *)Sender)->Clear();
        ((TComboBox *)Sender)->Text = "Stock Location";
        ((TComboBox *)Sender)->Font->Style = TFontStyles() << fsItalic;
        ((TComboBox *)Sender)->Font->Color = clGray;
    }

    else if((TEdit *)Sender == Edit1)
    {
        Edit1->Text = "Stock Item";
        Edit1->Font->Style = TFontStyles() << fsItalic;
        Edit1->Font->Color = clGray;
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void TfrmEditRecipe::ChangeFont(TObject *Object, ebox box)
{
    switch(box)
    {
        case eStockBox:     ((TEdit *)Object)->Font->Style = TFontStyles() << fsBold;
                            ((TEdit *)Object)->Font->Color = clBlack;
                                break;

        case eQuantity:     ((TNumericEdit *)Object)->Font->Style = TFontStyles() << fsBold;
                            ((TNumericEdit *)Object)->Font->Color = clBlack;
                                break;
        case eLocation:     ((TComboBox *)Object)->Font->Style = TFontStyles() << fsBold;
                            ((TComboBox *)Object)->Font->Color = clBlack;
                            ((TComboBox *)Object)->Text = "";
                                break;

    }
}
//---------------------------------------------------------------------------

void TfrmEditRecipe::CheckDefaultLocation(void)
{
    for(int i = 0; i < TempLocations.size(); i++)
    {
        if(TempLocations.at(i) == StockLocation)
        {
            cbLocation->Text = StockLocation;

            cbLocation->Font->Style = TFontStyles() << fsBold;
            cbLocation->Font->Color = clBlack;
        }
    }
}

//---------------------------------------------------------------------------

void TfrmEditRecipe::GetStockLocations(AnsiString Code)
{
    try
    {
    if(Code == NULL)
        return;

        TempLocations.clear();
        std::vector<int> Stock_Keys;
        int Stock_Key;

	    Query->Close();
	    Query->SQL->Text =
            "Select Stock_Key from Stock "
            "Where Code = :Code;";
        Query->ParamByName("Code")->AsString = Code;
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
            TempLocations.push_back(Query->FieldByName("Location")->AsString);
        }

        Query->Close();

    }
	catch (Exception &E)
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}



}
//----------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::IncludeOnClick(TObject *Sender)
{

    if(RecipeName == "")
    {
        Application->MessageBox("Please select a recipe to edit", "error", MB_ICONINFORMATION + MB_OK);
        return;
    }

    bool Proceed = false;

    int MessageNumber;

    for(int i = 0; i < StockLocations.size(); i++)
    {
        if(Edit1->Text != "Stock Item"                   &&
        ((TEdit *)NumericEdit1)->Text != "Recipe Quantity" &&
        ((TEdit *)NumericEdit1)->Text != ""               &&
        cbLocation->Text == StockLocations.at(i))
        {
            Proceed = true;
            break;
        }
        else if(cbLocation->Text != StockLocations.at(i))
            MessageNumber = 2;

    }

    if(Edit1->Text == "Stock Item")
        MessageNumber = 0;
    else if(((TEdit *)NumericEdit1)->Text == "Recipe Quantity")
        MessageNumber = 1;
    else if(((TEdit *)NumericEdit1)->Text == "")
        MessageNumber = 1;
    else if(cbLocation->Text == "Stock Location")
        MessageNumber = 2;

    if(!Proceed)
    {
        Application->MessageBox(MessagesArray[MessageNumber], "error", MB_ICONINFORMATION + MB_OK);
        return;
    }

    PVirtualNode RecipeNode						= NULL;
    TRecipeNodeData *NodeData				= NULL;
    double temp;

    RecipeNode = vtvStock->AddChild(NULL);
    NodeData = (TRecipeNodeData *)vtvStock->GetNodeData(RecipeNode);


    NodeData->Text = Edit1->Text;
    NodeData->Location = cbLocation->Text;
    NodeData->Qty = Converted_value;
    NodeData->Unit = Unit;
    NodeData->Code = StockCode;

    temp = GetItemCost(Edit1->Text, cbLocation->Text);
    NodeData->AverageCost = temp * Converted_value;

    NumericEdit2->Value = NumericEdit2->Value + temp;
    StockLocation = cbLocation->Text;

    StockCode = NULL;
    Label3->Caption = 0;
    BoxExit(Label3);
    BoxExit(cbLocation);
    BoxExit(Edit1);
    cbLocation->Enabled = false;
    Label3->Caption = "";

    ChangeInRecipe = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditRecipe::LocationOnDropDown(TObject *Sender)
{
    GetStockLocations(cbLocation, StockCode);
    ChangeFont(cbLocation, eLocation);
}
//---------------------------------------------------------------------------
void TfrmEditRecipe::GetStockLocations(TComboBox *ComboBox, AnsiString Code)
{
    try
    {
    if(Code == NULL)
        return;

        ComboBox->Clear();

        StockLocations.clear();


        std::vector<int> Stock_Keys;
        int Stock_Key;

	    Query->Close();
	    Query->SQL->Text =
            "Select Stock_Key from Stock "
            "Where Code = :Code;";
        Query->ParamByName("Code")->AsString = Code;
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
            StockLocations.push_back(Query->FieldByName("Location")->AsString);

        }

        Query->Close();


    }
	catch (Exception &E)
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}



}
//---------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::QuantityOnClick(TObject *Sender)
{
    ChangeFont(NumericEdit1, eQuantity);
}
//---------------------------------------------------------------------------

double TfrmEditRecipe::GetItemCost(AnsiString Item, AnsiString Location)
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
               "Select Average_Cost, Latest_Cost from StockLocation "
               "Where Location = :Location and Stock_Key = :Stock_Key;";
            Query->ParamByName("Location")->AsString = Location;
            Query->ParamByName("Stock_Key")->AsInteger = Stock_Key;
            Query->ExecQuery();
            if(Query->RecordCount)
                return (Query->FieldByName("Latest_Cost")->AsFloat);
    }
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}
void __fastcall TfrmEditRecipe::CancelOnClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditRecipe::OkOnClick(TObject *Sender)
{
    if(RecipeName == "")
    {
        Application->MessageBox("Please select a recipe to edit", "error", MB_ICONINFORMATION + MB_OK);
        return;
    }

    UpdateDB();


    Close();
}

//----------------------------------------------------------------------------------

void TfrmEditRecipe::UpdateDB(void)
{
    try
    {
	    PVirtualNode Node = vtvStock->GetFirst();
        int Stock_Key;
        double Costs[2];
		int RecipeID;
        AnsiString Final_Stock, Final_Stock_Code, Description, Final_Stock_Location;
        float Average_Cost, Latest_Cost;

        Query->Close();
        Query->SQL->Text =
            "Select Recipes_ID, "
            "Final_Stock, "
            "Final_Stock_Code, "
            "Description, "
            "Final_Stock_Location, "
            "Stock_Average_Cost, "
            "Stock_Latest_Cost "
            "From Recipes "
            "Where Recipe = :Recipe And Deleted = :Deleted";
        Query->ParamByName("Recipe")->AsString = RecipeName;
        Query->ParamByName("Deleted")->AsString = "F";
        Query->ExecQuery();
        if(Query->RecordCount)
        {
            RecipeID = Query->FieldByName("Recipes_ID")->AsInteger;
            Final_Stock = Query->FieldByName("Final_Stock")->AsString;
            Final_Stock_Code = Query->FieldByName("Final_Stock_Code")->AsString;
            Description = Query->FieldByName("Description")->AsString;
            Final_Stock_Location = Query->FieldByName("Final_Stock_Location")->AsString;
            Average_Cost = Query->FieldByName("Stock_Average_Cost")->AsFloat;
            Latest_Cost = Query->FieldByName("Stock_Latest_Cost")->AsFloat;
        }

        Query->Close();
        Query->SQL->Text =
            "Delete From Recipes "
            "Where Recipes_ID = :Recipes_ID;";
        Query->ParamByName("Recipes_ID")->AsInteger = RecipeID;
        Query->ExecQuery();



        while (Node)
        {
            TRecipeNodeData *NodeData	= (TRecipeNodeData *)vtvStock->GetNodeData(Node);

            QueryRecipe->Close();
            QueryRecipe->SQL->Text =
                "INSERT INTO Recipes ("
                    "Recipes_ID,"
                    "Recipe,"
                    "Final_Stock,"
                    "Final_Stock_Code,"
                    "Description,"
                    "Final_Stock_Location,"
                    "Time_Stamp,"
                    "Required_Stock,"
                    "Stock_Code,"
                    "Stock_Qty,"
                    "Stock_Location,"
                    "Stock_Unit,"
                    "Stock_Average_Cost,"
                    "Stock_Latest_Cost,"
                    "Deleted,"
                    "Editted)"
                "Values ("
                    ":Recipes_ID,"
                    ":Recipe,"
                    ":Final_Stock,"
                    ":Final_Stock_Code,"
                    ":Description,"
                    ":Final_Stock_Location,"
                    ":Time_Stamp,"
                    ":Required_Stock,"
                    ":Stock_Code,"
                    ":Stock_Qty,"
                    ":Stock_Location,"
                    ":Stock_Unit,"
                    ":Stock_Average_Cost,"
                    ":Stock_Latest_Cost,"
                    ":Deleted,"
                    ":Editted"
                    ")";

            QueryRecipe->ParamByName("Recipes_ID")->AsInteger = RecipeID;
            QueryRecipe->ParamByName("Recipe")->AsString = RecipeName;
            QueryRecipe->ParamByName("Final_Stock")->AsString = Final_Stock;
            QueryRecipe->ParamByName("Final_Stock_Code")->AsString = Final_Stock_Code;
            QueryRecipe->ParamByName("Description")->AsString = Description;
            QueryRecipe->ParamByName("Final_Stock_Location")->AsString = Final_Stock_Location;
            QueryRecipe->ParamByName("Time_Stamp")->AsDateTime = Now();
            QueryRecipe->ParamByName("Required_Stock")->AsString = NodeData->Text;
            QueryRecipe->ParamByName("Stock_Code")->AsString = NodeData->Code;
            QueryRecipe->ParamByName("Stock_Qty")->AsFloat = NodeData->Qty;
            QueryRecipe->ParamByName("Stock_Location")->AsString = NodeData->Location;
            QueryRecipe->ParamByName("Stock_Unit")->AsString = NodeData->Unit;
            QueryRecipe->ParamByName("Stock_Average_Cost")->AsFloat = Average_Cost;
            QueryRecipe->ParamByName("Stock_Latest_Cost")->AsFloat = Latest_Cost;
            QueryRecipe->ParamByName("Deleted")->AsString = "F";
            QueryRecipe->ParamByName("Editted")->AsString = "T";

            QueryRecipe->ExecSQL();
            QueryRecipe->Close();

            Node = vtvStock->GetNext(Node);
        }

    ChangeInRecipe = false;

	if (Transaction->InTransaction)
		Transaction->Commit();


    }
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}
//-------------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::vtvStockCreateEditor(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      IVTEditLink *EditLink)
{
	if (Node && Column == 5)
	{
		TPropertyEdit* PropertyLink = new TPropertyEdit(Sender, Node, Column, BitBtn6);
		PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		PostMessage(BitBtn6->Handle, EM_SETSEL, 0, -1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::vtvStockOnEditing(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column, bool &Allowed)
{
    Allowed = (Column == 5);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::vtvStockOnEdited(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column)
{
	if (vtvStock->FocusedNode)
	{
		if (Column == 5)
		{
//            RemoveItem();
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::vtvStockAfterPaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;    
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::vtvStockBeforePaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;    
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::vtvStockFocusChanged(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	if (Node && !Sender->IsEditing())
	{
		if (Sender->FocusedColumn != 5)
		{
			Sender->FocusedColumn = 5;
		}
		Sender->EditNode(Node, Column);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::vtvStockFocusChanging(
      TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
      TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}    
}

//---------------------------------------------------------------------------

void __fastcall TfrmEditRecipe::RemoveItemOnClick(TObject *Sender)
{
	if (vtvStock->IsEditing())
	{
		vtvStock->EndEditNode();
	}
	if (vtvStock->FocusedNode)
	{
		TRecipeNodeData *NodeData	= (TRecipeNodeData *)vtvStock->GetNodeData(vtvStock->FocusedNode);
		if (Application->MessageBox(("Do you wish to remove the item \"" + NodeData->Text + "\" from the order?").c_str(),
											  "", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
		{
            NumericEdit2->Value = NumericEdit2->Value - ItemPrices[NodeData->Text + "," + NodeData->Location] * NodeData->Qty;
			vtvStock->DeleteNode(vtvStock->FocusedNode);
            ChangeInRecipe = true;
		}
	}
	if (!vtvStock->IsEditing() && vtvStock->FocusedNode)
	{
		vtvStock->EditNode(vtvStock->FocusedNode, vtvStock->FocusedColumn);
	}
}


void __fastcall TfrmEditRecipe::NumericEdit1Change(TObject *Sender)
{
    double Convert;
    qrDescription->Close();
    qrDescription->Open();
    Convert=qrDescription->FieldByName("Conversion_Factor")->AsFloat;
    if(Convert <= 0)
    {   Converted_value = NumericEdit1->Value;
        Unit=  qrDescription->FieldByName("Stocktake_unit")->AsString;
        Label3->Caption ="  "+FloatToStr(NumericEdit1->Value);// +"("+  qrDescription->FieldByName("Stocktake_unit")->AsString +")"  ;
    }
    else
    {
        Converted_value = NumericEdit1->Value/Convert;
        Unit=  qrDescription->FieldByName("Stocktake_unit")->AsString;
        Label3->Caption =  "  "+FloatToStr(NumericEdit1->Value/Convert)+"("+  Unit +")";//qrdescription->ConversionFactor;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditRecipe::NumericEdit1KeyPress(TObject *Sender,
      char &Key)
{
   if(Key == '-')
   {
      Key = NULL;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditRecipe::NumericEdit2KeyPress(TObject *Sender,
      char &Key)
{
   if(Key == '-')
   {
      Key = NULL;
   }
}
//---------------------------------------------------------------------------

