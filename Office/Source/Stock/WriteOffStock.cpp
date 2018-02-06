//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Connections.h"
#include "Login.h"
#include "WriteOffStock.h"
#include "StockData.h"
#include "MM_StockTransactions.h"
#include "SelectStockItem.h"
#include <Vector>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma resource "*.dfm"
//TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TfrmWriteOffStock::TfrmWriteOffStock(TComponent* Owner)
    : TForm(Owner), Item(""), Quantity(NULL), CostDisplayChangeOk(false), RowNumber(1), totalcost(0)
{
    lUnit->Caption = "";
    DateTimePicker1->Date = Now();

    sgWriteOffs->Cells[0][0] = "Date";
    sgWriteOffs->Cells[1][0] = "Location";
    sgWriteOffs->Cells[2][0] = "Item";
    sgWriteOffs->Cells[3][0] = "Unit";
    sgWriteOffs->Cells[4][0] = "Quantity";
    sgWriteOffs->Cells[5][0] = "Comment";
    neCost1->DecimalPlaces = CurrentConnection.SettingDecimalPlaces;
    neCost3->DecimalPlaces =  CurrentConnection.SettingDecimalPlaces;
    neTotal->DecimalPlaces =  CurrentConnection.SettingDecimalPlaces;

}
//---------------------------------------------------------------------------


void __fastcall TfrmWriteOffStock::LocationOnSelect(TObject *Sender)
{

    try
    {
        Locations = cbLocations->Text;

        qrPrice->Close();
        qrPrice->ParamByName("Stock_Key")->AsInteger = StockKey;
        qrPrice->ParamByName("Location")->AsString = Locations;
        qrPrice->Open();
        if(qrPrice->RecordCount)
        {
            Price = qrPrice->FieldByName("Latest_Cost")->AsFloat;
        }

        CostDisplayChange();

        if(CostDisplayChangeOk)
        {
            neCost1->Value = Price;
            neCost3->Value = Price * Quantity;
        }

        EnableAdd();
    }
	catch (Exception &E)
    {
		if (IBTransaction1->InTransaction) IBTransaction1->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmWriteOffStock::QuantityOnChange(TObject *Sender)
{
    Quantity  = StrToFloat(FloatToStrF(neQty->Value,ffFixed,19, CurrentConnection.SettingDecimalPlaces));

    if(CostDisplayChangeOk)
    {
                neCost1->Value = Price;
                neCost3->Value = Price * Quantity;
    }

   EnableAdd();
}
//---------------------------------------------------------------------------

void TfrmWriteOffStock::CostDisplayChange(void)
{
    if(cbLocations->Text != "" && EItem->Text != "")
        CostDisplayChangeOk = true;
    else
        CostDisplayChangeOk = false;

}
//---------------------------------------------------------------------------

void TfrmWriteOffStock::EnableAdd(void)
{
    if(cbLocations->Text != "" && neQty->Value != 0 && EItem->Text != "")
        btnAddSupplierUnit->Enabled = true;
    else
        btnAddSupplierUnit->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmWriteOffStock::btnAddOnClick(TObject *Sender)
{
    bool Location_Error = true;
    Query->Close();
    Query->SQL->Text =
        "Select Distinct Location From StockLocation;";
    for(Query->ExecQuery(); !Query->Eof; Query->Next())
    {
        if(cbLocations->Text == Query->FieldByName("Location")->AsString)
        {
            Location_Error = false;
            break;
        }
    }


    if(Location_Error)
    {
        MessageBox(NULL,"Unknown Location", "Error", 0);
        return;
    }


    btnOk->Enabled = true;

    DateTimePicker1->Time = Now().CurrentTime();
    sgWriteOffs->Cells[0][RowNumber] = DateTimePicker1->Date.FormatString("DD/MM/YYYY hh:nn");
    sgWriteOffs->Cells[1][RowNumber] = cbLocations->Text;
    sgWriteOffs->Cells[2][RowNumber] = EItem->Text;
    sgWriteOffs->Cells[3][RowNumber] = SuppliedUnit->Text;
    sgWriteOffs->Cells[4][RowNumber] = FloatToStrF(neQty->Value, ffNumber, 19, CurrentConnection.SettingDecimalPlaces);
    
    sgWriteOffs->Cells[5][RowNumber] = Edit1->Text;

    ItemQty.push_back(neQty->Value);
    itemcosts.push_back(neCost3->Value);
    DateArray.push_back(DateTimePicker1->Date);
    totalcost += neCost3->Value;
    neTotal->Value = totalcost;
    neTotal->DecimalPlaces=CurrentConnection.SettingDecimalPlaces;


    EItem->Text = "";
    neQty->Value = 0;
    SuppliedUnit->Text = "";
    Unit = "";
    Item = "";
    Quantity = 0;
    Price = 0;

    EnableAdd();
    RowNumber++;
    sgWriteOffs->RowCount++;
    neCost1->Value = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmWriteOffStock::sgWriteOffsSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    btnRemove->Enabled = true;
    DeleteRow = ARow;

}
//---------------------------------------------------------------------------

void __fastcall TfrmWriteOffStock::btnRemoveOnClick(TObject *Sender)
{
    if(DeleteRow != RowNumber)
    {
        double erase = itemcosts.at(DeleteRow - 1);
        itemcosts.erase(itemcosts.begin() + DeleteRow - 1);
        ItemQty.erase(ItemQty.begin() + DeleteRow - 1);
        DateArray.erase(DateArray.begin() + DeleteRow - 1);
        totalcost = totalcost - erase;
        neTotal->Value = totalcost;

        for(int i = DeleteRow + 1; i <= RowNumber; i++)
        {
            sgWriteOffs->Cells[0][i - 1] = sgWriteOffs->Cells[0][i];
            sgWriteOffs->Cells[1][i - 1] = sgWriteOffs->Cells[1][i];
            sgWriteOffs->Cells[2][i - 1] = sgWriteOffs->Cells[2][i];
            sgWriteOffs->Cells[3][i - 1] = sgWriteOffs->Cells[3][i];
            sgWriteOffs->Cells[4][i - 1] = sgWriteOffs->Cells[4][i];
            sgWriteOffs->Cells[5][i - 1] = sgWriteOffs->Cells[5][i];
        }

        RowNumber--;
        sgWriteOffs->RowCount--;
    }
    if(RowNumber == 1)
    {
        btnOk->Enabled = false;
        btnRemove->Enabled = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmWriteOffStock::btnokOnClick(TObject *Sender)
{
    UpdateStockTrans();
    StockLocation();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmWriteOffStock::btncancelOnClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void TfrmWriteOffStock::UpdateStockLocation(int StockKey)
{
 	try
	{

		if (!IBTransaction1->InTransaction) IBTransaction1->StartTransaction();

   	 	cbLocations->Items->Clear();
        cbLocations->Text = "";

        qrLocations->Close();
        qrLocations->ParamByName("Stock_Key")->AsInteger = StockKey;
        qrLocations->Open();

        while(!qrLocations->Eof)
        {
            cbLocations->Items->AddObject(qrLocations->FieldByName("Location")->AsString,
													        (TObject *)StockKey);
            if(Locations == qrLocations->FieldByName("Location")->AsString)
               cbLocations->Text = Locations;
		 	qrLocations->Next();
		}

        qrLocations->Close();


    }
	catch (Exception &E)
	{
		if (IBTransaction1->InTransaction) IBTransaction1->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}


void TfrmWriteOffStock::UpdateStockTrans(void)
{

    Stock::TWriteOffStock WriteOffStock(dmStockData->dbStock);

 	try
	{

        AnsiString temp[5];

		if (!IBTransaction1->InTransaction) IBTransaction1->StartTransaction();
        for(int i = 1; i < RowNumber; i++)
        {

            temp[0] = sgWriteOffs->Cells[0][i],
            temp[1] = sgWriteOffs->Cells[1][i],
            temp[2] = sgWriteOffs->Cells[2][i],
            temp[3] = sgWriteOffs->Cells[3][i],
            temp[4] = sgWriteOffs->Cells[5][i],
            WriteOffStock.UpdateStock(temp, -ItemQty.at(i - 1), DateArray.at(i - 1));
        }

    }
    catch (Exception &E)
    {
		if (IBTransaction1->InTransaction) IBTransaction1->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}

}


void __fastcall TfrmWriteOffStock::btnAddItemOnClick(TObject *Sender)
{
	frmSelectStockItem->Reset = true;
    AnsiString StockCode;

 	try
	{

	if (frmSelectStockItem->ShowModal() == mrOk)
	{
		StockCode = frmSelectStockItem->StockCode;
        cbLocations->Enabled = true;



        qrItem->Close();
        qrItem->ParamByName("Code")->AsString = StockCode;
        qrItem->Open();
        if(qrItem->RecordCount)
        {
            Item = qrItem->FieldByName("Description")->AsString;
            Unit = qrItem->FieldByName("Stocktake_Unit")->AsString;
            StockKey = qrItem->FieldByName("Stock_Key")->AsInteger;

            EItem->Text = Item;
            SuppliedUnit->Text = Unit;


            UpdateStockLocation(StockKey);

    LocationOnSelect(Sender);

            neCost1->Value = Price;
            neCost1->DecimalPlaces = CurrentConnection.SettingDecimalPlaces;
            neCost3->Value = Price * Quantity;
            neCost3->DecimalPlaces = CurrentConnection.SettingDecimalPlaces;


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
//---------------------------------------------------------------------------

void TfrmWriteOffStock::StockLocation(void)
{
    try
    {

    if (!IBTransaction1->InTransaction) IBTransaction1->StartTransaction();

        for(int i = 1; i < RowNumber; i++)
        {
            int Stock_Key;

            Query->Close();
            Query->SQL->Text =
                "Select Stock_Key From Stock Where Description = :Description;";
            Query->ParamByName("Description")->AsString = sgWriteOffs->Cells[2][i];
            Query->ExecQuery();
            if(Query->RecordCount)
                Stock_Key = Query->FieldByName("Stock_Key")->AsInteger;

            else
                throw Exception("StockLocation WriteOff error");
            Query->Close();


            Query->SQL->Text =
                "update stocklocation "
                "set on_hand = on_hand - :Quantity, writeoff = writeoff - :Quantity "
                "where Location = :Location and Stock_Key = :Stock_Key; ";

            Query->ParamByName("Stock_Key")->AsInteger = Stock_Key;
            Query->ParamByName("Location")->AsString = sgWriteOffs->Cells[1][i];
            Query->ParamByName("Quantity")->AsFloat = ItemQty.at(i - 1);

            Query->ExecQuery();
            Query->Close();


        }
        MessageBox(NULL,"Stock Database Updated", "Updated", 0);
        Close();

    }
	catch (Exception &E)
	{
		if (IBTransaction1->InTransaction) IBTransaction1->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}


}
void __fastcall TfrmWriteOffStock::neQtyKeyPress(TObject *Sender,
      char &Key)
{
   if(Key == '-')
   {
      Key = NULL;
   }
}
//---------------------------------------------------------------------------

