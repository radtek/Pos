//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Connections.h"
#include "CreateRecipe.h"
#include "SelectStockItem.h"
#include "MM_Math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma link "NumericEdit"
#pragma resource "*.dfm"
  double Converted_value;
  String Unit;
//---------------------------------------------------------------------------
__fastcall TfrmCreateRecipe::TfrmCreateRecipe(TComponent* Owner)
    : TForm(Owner),
    firstclick(GUIBoxes),
    StockCode(NULL),
    Stock_Unit(""),
    StockLocation("")

{

    std::fill(firstclick.begin(), firstclick.end(), false);
    ((TEdit *)NumQuantity)->Text = "Recipe Quantity";
    NumQuantity->Enabled=false;
    vtvStock->NodeDataSize = sizeof(TRecipeNodeData);
    NumQuantity->DecimalPlaces=CurrentConnection.SettingDecimalPlaces;
    NumericEdit1->DecimalPlaces=CurrentConnection.SettingDecimalPlaces;

} 
//---------------------------------------------------------------------------

void TfrmCreateRecipe::EnterText(AnsiString &Text)
{



}
void __fastcall TfrmCreateRecipe::DescriptionOnClick(TObject *Sender)
{
    if(firstclick[eDescription] == false)
    {
        ChangeFont(MemDescription, eDescription);
        firstclick[eDescription] = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmCreateRecipe::FindItemMouseClick(TObject *Sender)
{
    if (!IBTransaction1->InTransaction) IBTransaction1->StartTransaction();

	frmSelectStockItem->Reset = true;
	if (frmSelectStockItem->ShowModal()					== mrOk)
	{
        qrDescription->Close();
        qrDescription->ParamByName("Code")->AsString = frmSelectStockItem->StockCode;
        qrDescription->Open();

	    if (QueryRecipe->Transaction->InTransaction)
		    QueryRecipe->Transaction->Commit();

        if (!IBTransaction1->InTransaction) IBTransaction1->StartTransaction();

        if((TBitBtn *)Sender == BitBtn1)
        {
            FinalStockCode = frmSelectStockItem->StockCode;
            Edit3->Text = qrDescription->FieldByName("Description")->AsString;
            ChangeFont(Edit3, eStockBox);

            cbFinalLocation->Enabled = true;
            BoxExit(cbFinalLocation);
        }
        else if((TBitBtn *)Sender == BitBtn6)
        {
            StockCode = frmSelectStockItem->StockCode;
            Edit4->Text = qrDescription->FieldByName("Description")->AsString;
            Label2->Caption = qrDescription->FieldByName("Stocktake_unit")->AsString; //+ "("+ qrDescription->FieldByName("Stocktake_unit")->AsString + ")";
            Label4->Caption = qrDescription->FieldByName("REDUCTION_UNIT")->AsString;
                NumQuantity->Enabled=true;
               
                   ((TEdit *)NumQuantity)->Text =  "0";
            ChangeFont(Edit4, eStockBox);
            cbStockLocation->Enabled = true;
            BoxExit(cbStockLocation);
            GetStockLocations(frmSelectStockItem->StockCode);
            CheckDefaultLocation();
        }

        qrDescription->Close();

    }
}
//---------------------------------------------------------------------------

void TfrmCreateRecipe::ChangeFont(TObject *Object, ebox box)
{
    switch(box)
    {
        case eDescription:  ((TMemo *)Object)->Font->Style = TFontStyles() << fsBold;
                            ((TMemo *)Object)->Font->Color = clBlack;
                            ((TMemo *)Object)->Lines->Text = "";
                                break;
        case eStockBox:     ((TEdit *)Object)->Font->Style = TFontStyles() << fsBold;
                            ((TEdit *)Object)->Font->Color = clBlack;
                                break;

        case eRecipeName:   ((TEdit *)Object)->Font->Style = TFontStyles() << fsBold;
                            ((TEdit *)Object)->Font->Color = clBlack;
                            ((TEdit *)Object)->Text = "";
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
void __fastcall TfrmCreateRecipe::RecipeNameOnClick(TObject *Sender)
{
    if(firstclick[eRecipeName] == false)
    {
        ChangeFont(ERecipeName, eRecipeName);
        firstclick[eRecipeName] = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmCreateRecipe::QuantityOnClick(TObject *Sender)
{
    if(firstclick[eQuantity] == false)
    {
        ChangeFont(NumQuantity, eQuantity);
        firstclick[eQuantity] = true;
    }
}
//---------------------------------------------------------------------------


void __fastcall TfrmCreateRecipe::BoxExit(TObject *Sender)
{
    if((TEdit *)Sender == ERecipeName)
    {
        if(ERecipeName->Text == "")
        {
            ERecipeName->Text = "Recipe Name";
            ERecipeName->Font->Style = TFontStyles() << fsItalic;
            ERecipeName->Font->Color = clGray;
            firstclick[eRecipeName] = false;
        }
    }
    else if((TMemo *)Sender == MemDescription)
    {
        if(MemDescription->Text == "")
        {
            MemDescription->Text = "Description";
            MemDescription->Font->Style = TFontStyles() << fsItalic;
            MemDescription->Font->Color = clGray;
            firstclick[eDescription] = false;

        }
    }
    else if((TNumericEdit *)Sender == NumQuantity)
    {
        if(NumQuantity->Value == 0)
        {
            ((TEdit *)NumQuantity)->Text = "Recipe Quantity";
            NumQuantity->Font->Style = TFontStyles() << fsItalic;
            NumQuantity->Font->Color = clGray;
            firstclick[eQuantity] = false;
        }
    }
    else if((TComboBox *)Sender == cbStockLocation || (TComboBox *)Sender == cbFinalLocation)
    {
        ((TComboBox *)Sender)->Clear();

            if((TComboBox *)Sender == cbStockLocation)
                ((TComboBox *)Sender)->Text = "Stock Location";
            else
                ((TComboBox *)Sender)->Text = "Final Stock Location";

        ((TComboBox *)Sender)->Font->Style = TFontStyles() << fsItalic;
        ((TComboBox *)Sender)->Font->Color = clGray;
    }
    else if((TEdit *)Sender == Edit4)
    {
        Edit4->Text = "Stock Item";
        Edit4->Font->Style = TFontStyles() << fsItalic;
        Edit4->Font->Color = clGray;
    }
    else if((TEdit *)Sender == Edit3)
    {
        Edit3->Text = "Final Stock";
        Edit3->Font->Style = TFontStyles() << fsItalic;
        Edit3->Font->Color = clGray;
    }

}
//---------------------------------------------------------------------------

void TfrmCreateRecipe::GetStockLocations(TComboBox *ComboBox, AnsiString Code)
{
    try
    {
        if(Code == NULL)
            return;

        ComboBox->Clear();

        if(ComboBox == cbStockLocation)
            StockLocations.clear();
        else if(ComboBox == cbFinalLocation)
            FinalStockLocations.clear();

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

            if(ComboBox == cbStockLocation)
                StockLocations.push_back(Query->FieldByName("Location")->AsString);
            else if(ComboBox == cbFinalLocation)
                FinalStockLocations.push_back(Query->FieldByName("Location")->AsString);
        }

        Query->Close();


    }
	catch (Exception &E)
	{
		if (IBTransaction1->InTransaction) IBTransaction1->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}



}
//---------------------------------------------------------------------------
void __fastcall TfrmCreateRecipe::LocationDropDown(TObject *Sender)
{
    if((TComboBox *)Sender == cbStockLocation)
    {
        GetStockLocations(cbStockLocation, StockCode);
        ChangeFont(cbStockLocation, eLocation);
    }
    else if((TComboBox *)Sender == cbFinalLocation)
    {
        GetStockLocations(cbFinalLocation, FinalStockCode);
        ChangeFont(cbFinalLocation, eLocation);
    }

}
//---------------------------------------------------------------------------

void __fastcall TfrmCreateRecipe::IncludeOnClick(TObject *Sender)
{
    bool Proceed = false;

    int MessageNumber;

    for(int i = 0; i < StockLocations.size(); i++)
    {
        if(Edit4->Text != "Stock Item"                   &&
        ((TEdit *)NumQuantity)->Text != "Recipe Quantity" &&
        ((TEdit *)NumQuantity)->Text != ""               &&
        cbStockLocation->Text == StockLocations.at(i))
        {
            Proceed = true;
            break;
        }



        else if(cbStockLocation->Text != StockLocations.at(i))
            MessageNumber = 2;

    }

    if(Edit4->Text == "Stock Item")
        MessageNumber = 0;
    else if(((TEdit *)NumQuantity)->Text == "Recipe Quantity")
        MessageNumber = 1;
    else if(((TEdit *)NumQuantity)->Text == "")
        MessageNumber = 1;
    else if(cbStockLocation->Text == "Stock Location")
        MessageNumber = 2;



    if(!Proceed)
    {
        Application->MessageBox(MessagesArray[MessageNumber], "error", MB_ICONINFORMATION + MB_OK);
        return;
    }

    PVirtualNode RecipeNode						= NULL;
    TRecipeNodeData *NodeData				= NULL;
    double temp = 0;

    RecipeNode = vtvStock->AddChild(NULL);
    NodeData = (TRecipeNodeData *)vtvStock->GetNodeData(RecipeNode);


    NodeData->Text = Edit4->Text;
    NodeData->Location = cbStockLocation->Text;
    NodeData->Qty = RoundTo(Converted_value,-CurrentConnection.SettingDecimalPlaces);
    //MMMath::CurrencyString(Converted_value,CurrentConnection.SettingDecimalPlaces);//Converted_value;
    NodeData->Unit = Unit;
    NodeData->Code = StockCode;

    temp = GetItemCost(Edit4->Text, cbStockLocation->Text);
    temp = temp * Converted_value;;

    NodeData->AverageCost = temp;
   NumericEdit1->Value = NumericEdit1->Value + (GetItemCost(Edit4->Text, cbStockLocation->Text) *  Converted_value);

   // NumericEdit1->DecimalPlaces = Decimalpalaces;
    StockLocation = cbStockLocation->Text;

    StockCode = NULL;
    Label2->Caption  = 0;
    BoxExit(Label2 );
    BoxExit(cbStockLocation);
    BoxExit(Edit4);
    cbStockLocation->Enabled = false;
    Label2->Caption = "";
    Label4->Caption = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmCreateRecipe::vtvStockGetText(TBaseVirtualTree *Sender,
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
	            if(CurrentConnection.SettingDecimalPlaces==2)
	            {
	            	CellText = FloatToStrF(NodeData->Qty, ffFixed, 19, 2);
	            }
	            else
	            {
	             	CellText = FloatToStrF(NodeData->Qty, ffFixed, 19, 4);
	            }
                CellText = NodeData->Qty;
				break;
           case 4:
                if(CurrentConnection.SettingDecimalPlaces==2)
                {
                   CellText = FloatToStrF(NodeData->AverageCost, ffFixed, 19, 2);
                }
                else
                {
                    CellText = FloatToStrF(NodeData->AverageCost, ffFixed, 19, 4);
                }
                break;
		}
    }
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmCreateRecipe::RemoveOnClick(TObject *Sender)
{
	if (vtvStock->IsEditing())
	{
		vtvStock->EndEditNode();
	}
	if (vtvStock->FocusedNode)
	{
		TRecipeNodeData *NodeData	= (TRecipeNodeData *)vtvStock->GetNodeData(vtvStock->FocusedNode);
		if (Application->MessageBox(("Do you wish to remove the item \"" + NodeData->Text + "\" from the order?").c_str(),
											  "Update?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
		{
            NumericEdit1->Value = NumericEdit1->Value - (NodeData->AverageCost);
			vtvStock->DeleteNode(vtvStock->FocusedNode);
		}
	}
	if (!vtvStock->IsEditing() && vtvStock->FocusedNode)
	{
		vtvStock->EditNode(vtvStock->FocusedNode, vtvStock->FocusedColumn);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmCreateRecipe::CancelOnClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------


void __fastcall TfrmCreateRecipe::OkOnClick(TObject *Sender)
{

	if (vtvStock->IsEditing())
		vtvStock->EndEditNode();


    PVirtualNode Node = vtvStock->GetFirst();
    if(!Node)
    {
        Application->MessageBox("There is nothing to commit, please include your stock items, quantity and locations", "error", MB_ICONINFORMATION + MB_OK);
        return;
    }
    if(!CheckFinalStock_Recipe())
        return;



    if(CheckRecipeExists())
        return;

    UpdateLocationDB();
    vtvStock->Clear();

    ERecipeName->Text = "";
    MemDescription->Text = "";
    Edit3->Text = "";
    cbFinalLocation->Text = "";
   //  NumQuantity->Value = 0;

    BoxExit(ERecipeName);
    BoxExit(MemDescription);
    BoxExit(Edit3);
    BoxExit(cbFinalLocation);

	if (IBTransaction1->InTransaction)
	{
		IBTransaction1->Commit();
	}

}
//---------------------------------------------------------------------------

void TfrmCreateRecipe::CheckDefaultLocation(void)
{
    for(int i = 0; i < TempLocations.size(); i++)
    {
        if(TempLocations.at(i) == StockLocation)
        {
            cbStockLocation->Text = StockLocation;

            cbStockLocation->Font->Style = TFontStyles() << fsBold;
            cbStockLocation->Font->Color = clBlack;
        }
    }
}
//---------------------------------------------------------------------------

void TfrmCreateRecipe::GetStockLocations(AnsiString Code)
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
		if (IBTransaction1->InTransaction) IBTransaction1->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}



}
//-----------------------------------------------------------------------------

bool TfrmCreateRecipe::CheckQuantity(std::vector<double> &QtyVec, std::vector<std::pair<AnsiString, AnsiString> > VecOfKeys)
{
   try
    {
        AnsiString Item;
        AnsiString Location;
        double Qty;
        std::pair<AnsiString, AnsiString> tempKeys;
        int Stock_Key;


//        if (!IBTransaction1->InTransaction) IBTransaction1->StartTransaction();

        for(int i = 0; i < QtyVec.size(); i++)
        {
            Qty = QtyVec.at(i);
            tempKeys = VecOfKeys.at(i);

            Item = tempKeys.first;
            Location = tempKeys.second;



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
                "Select On_Hand from StockLocation "
                "Where Location = :Location and Stock_Key = :Stock_Key";
            Query->ParamByName("Location")->AsString = Location;
            Query->ParamByName("Stock_Key")->AsInteger = Stock_Key;
            Query->ExecQuery();

            if(Query->RecordCount)
            {
                if(Qty > Query->FieldByName("On_Hand")->AsFloat)
                {


                    if(Application->MessageBox(AnsiString("Warning, there are " + Query->FieldByName("On_Hand")->AsString + " of " + Item + " in location: " + Location + ", proceed?").c_str(), "Warning", MB_ICONWARNING + MB_YESNO) == ID_NO)
                        return false;
                }
            }
        }


    }
	catch (Exception &E)
	{
		if (IBTransaction1->InTransaction) IBTransaction1->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}
//-----------------------------------------------------------------------------------


//-------------------------------------------------------------------------------

bool TfrmCreateRecipe::CheckFinalStock_Recipe(void)
{
    bool Proceed = false;

    if(Edit3->Text == "" || Edit3->Text == "Final Stock")
    {
        Application->MessageBox("Please select your final stock item", "error", MB_ICONINFORMATION + MB_OK);
        return false;
    }
    else if(ERecipeName->Text == "" || ERecipeName->Text == "Recipe Name")
    {
        Application->MessageBox("Please enter a name for your recipe", "error", MB_ICONINFORMATION + MB_OK);
        return false;
    }


    for(int i = 0; i < FinalStockLocations.size(); i++)
    {
        if(cbFinalLocation->Text == FinalStockLocations.at(i))
        {
            Proceed = true;
            break;
        }
    }

    if(!Proceed)
    {
        Application->MessageBox("Please select a valid final stock location", "error", MB_ICONINFORMATION + MB_OK);
        return false;
    }


    return true;
}
//----------------------------------------------------------------------------------

void TfrmCreateRecipe::UpdateLocationDB(void)
{
    try
    {
	    PVirtualNode Node = vtvStock->GetFirst();
        int Stock_Key;
        double Costs[2] = {0};

		Query->Close();
		Query->SQL->Text = "Select Gen_id(Gen_Recipes_Key, 1) From rdb$database";
		Query->ExecQuery();
		int RecipeID = Query->Fields[0]->AsInteger;

        while (Node)
        {
            TRecipeNodeData *NodeData	= (TRecipeNodeData *)vtvStock->GetNodeData(Node);

            Query->Close();
            Query->SQL->Text =
                "Select Stock_Key from Stock "
                "Where Description = :Description;";
            Query->ParamByName("Description")->AsString = NodeData->Text;
            Query->ExecQuery();
            if(Query->RecordCount)
                Stock_Key = Query->FieldByName("Stock_Key")->AsInteger;

            Query->Close();

            Query->SQL->Text =
               "Select Average_Cost, Latest_Cost from StockLocation "
               "Where Location = :Location and Stock_Key = :Stock_Key;";
            Query->ParamByName("Location")->AsString = NodeData->Location;
            Query->ParamByName("Stock_Key")->AsInteger = Stock_Key;
            Query->ExecQuery();
            if(Query->RecordCount)
            {
                Costs[eAVG_COST] = Query->FieldByName("Average_Cost")->AsFloat;
                Costs[eLATEST_COST] = Query->FieldByName("Latest_Cost")->AsFloat;
            }
            Query->Close();

            UpdateRecipeDB(NodeData, Costs, RecipeID);
            Node = vtvStock->GetNext(Node);
        }
    }
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}
//------------------------------------------------------------------------------------

void TfrmCreateRecipe::UpdateRecipeDB(TRecipeNodeData *NodeData, double Costs[], int RecipeID)
{
    try
    {
        AnsiString FinalCode;
        AnsiString Description = MemDescription->Text;

        if(MemDescription->Text == "" || MemDescription->Text == "Description")
            Description = "";

	    Query->Close();
	    Query->SQL->Text =
            "Select Code from Stock "
            "Where Description = :Description;";
        Query->ParamByName("Description")->AsString = Edit3->Text;
        Query->ExecQuery();
        if(Query->RecordCount)
            FinalCode = Query->FieldByName("Code")->AsString;

        Query->Close();

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
                "Deleted)"
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
                ":Deleted"
                ")";

        QueryRecipe->ParamByName("Recipes_ID")->AsInteger = RecipeID;
        QueryRecipe->ParamByName("Recipe")->AsString = ERecipeName->Text;
        QueryRecipe->ParamByName("Final_Stock")->AsString = Edit3->Text;
        QueryRecipe->ParamByName("Final_Stock_Code")->AsString = FinalCode;
        QueryRecipe->ParamByName("Description")->AsString = Description;
        QueryRecipe->ParamByName("Final_Stock_Location")->AsString = cbFinalLocation->Text;
        QueryRecipe->ParamByName("Time_Stamp")->AsDateTime = Now();
        QueryRecipe->ParamByName("Required_Stock")->AsString = NodeData->Text;
        QueryRecipe->ParamByName("Stock_Code")->AsString = NodeData->Code;
        QueryRecipe->ParamByName("Stock_Qty")->AsFloat = NodeData->Qty;
        QueryRecipe->ParamByName("Stock_Location")->AsString = NodeData->Location;
        QueryRecipe->ParamByName("Stock_Unit")->AsString = NodeData->Unit;
        QueryRecipe->ParamByName("Stock_Average_Cost")->AsFloat = Costs[0] > -1000000 ? Costs[0] : 0;
        QueryRecipe->ParamByName("Stock_Latest_Cost")->AsFloat = Costs[1] > -1000000 ? Costs[1] : 0;
        QueryRecipe->ParamByName("Deleted")->AsString = "F";

        QueryRecipe->ExecSQL();
        QueryRecipe->Close();


    }
	catch (Exception &E)
	{
		if (IBTransaction1->InTransaction) IBTransaction1->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

bool TfrmCreateRecipe::CheckRecipeExists(void)
{
    bool Exists = false;

    Query->Close();
    Query->SQL->Text =
        "Select distinct Recipe from Recipes where Deleted = :Deleted;";
    Query->ParamByName("Deleted")->AsString = 'F';
    for(Query->ExecQuery(); !Query->Eof; Query->Next())
    {
        if(ERecipeName->Text == Query->FieldByName("Recipe")->AsString)
        {
            Exists = true;
            Application->MessageBox("This recipe already exists", "error", MB_ICONINFORMATION + MB_OK);
            break;
        }
    }
    Query->Close();

    return Exists;
}
void __fastcall TfrmCreateRecipe::vtvStockAfterPaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCreateRecipe::vtvStockBeforePaint(
      TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCreateRecipe::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	if (QueryRecipe->Transaction->InTransaction)
	{
		QueryRecipe->Transaction->Commit();
	}
}
//---------------------------------------------------------------------------

double TfrmCreateRecipe::GetItemCost(AnsiString Item, AnsiString Location)
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
                return (Query->FieldByName("Average_Cost")->AsFloat);
    }
	catch (Exception &E)
	{
		if (Query->Transaction->InTransaction) Query->Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}

void __fastcall TfrmCreateRecipe::NumQuantityChange(TObject *Sender)
{          double Convert;
           qrDescription->Close();
       // qrDescription->ParamByName("Code")->AsString = frmSelectStockItem->StockCode;
           qrDescription->Open();
           Convert=qrDescription->FieldByName("Conversion_Factor")->AsFloat;
            if(Convert <= 0)
          {    Converted_value=NumQuantity->Value;
            Label2->Caption =  "  "+FloatToStr(NumQuantity->Value); //+ "(" + qrDescription->FieldByName("Stocktake_unit")->AsString + ")";
          }
         else
          {     Converted_value=NumQuantity->Value/Convert;
                Unit= qrDescription->FieldByName("Stocktake_unit")->AsString ;
                double newValue = NumQuantity->Value/Convert;
            Label2->Caption =   "  "+ MMMath::CurrencyString(newValue,CurrentConnection.SettingDecimalPlaces)+ "(" + qrDescription->FieldByName("Stocktake_unit")->AsString + ")";//qrdescription->ConversionFactor;
            }


}
//---------------------------------------------------------------------------

void __fastcall TfrmCreateRecipe::NumQuantityClick(TObject *Sender)
{
   if(firstclick[eQuantity] == false)
    {
        ChangeFont(NumQuantity, eQuantity);
        firstclick[eQuantity] = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmCreateRecipe::NumQuantityKeyPress(TObject *Sender,
      char &Key)
{
   if(Key == '-')
   {
      Key = NULL;
   }
}
//---------------------------------------------------------------------------

