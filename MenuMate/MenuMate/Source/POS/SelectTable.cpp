//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectTable.h"
#include "Processing.h"
#include "TableManager.h"
#include "DeviceRealTerminal.h"
#include "DBTables.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmSelectTable::TfrmSelectTable(TComponent* Owner, Database::TDBControl &inIBDatabase)
	: TZForm(Owner) , IBDatabase(inIBDatabase)
{
    TableMode = eNormalNode;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectTable::FormShow(TObject *Sender)
{
	FormResize(this);

    if(TableMode)
    {
        TMezzanineTable tableDetails;
        tableDetails.FloorplanVer = 1;
        tableDetails.LocationId = 0;
        AssignedMezzanineTable = TDBTables::GetMezzanineAreaTables(tableDetails);
    }
    else
    {
        tiUpdateTableReq->Enabled = true;

    }

    // Reset all the tables.
    UpdateFloorPlanSheet();
}

void TfrmSelectTable::SetTablesColor(Database::TDBTransaction &DBTransaction)
{
  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
  IBInternalQuery->Close();
  IBInternalQuery->SQL->Text =
		"Select "
		"Max(Tables.TABLE_NUMBER) TABLE_NUMBER, "
		"Max(Orders.TIME_STAMP) TIME_STAMP "
		"From "
		"((Tables inner join Seat on "
		"Tables.Table_Key = Seat.Table_Key) inner join Tab on "
		"Seat.Tab_Key = Tab.Tab_Key) inner join Orders on "
		"Tab.Tab_Key = Orders.Tab_Key "
		"Group By "
		"Tables.TABLE_NUMBER "
		"Order By "
		"Tables.TABLE_NUMBER ";
	IBInternalQuery->ExecQuery();
	for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		int TableNo = IBInternalQuery->FieldByName("TABLE_NUMBER")->AsInteger;
		int Row = TableNo / tgridTables->RowCount;
		int Col = (TableNo % tgridTables->ColCount) - 1;

        if(Row < tgridTables->RowCount && Col < tgridTables->ColCount)
        {
            TGridButton *GridButton = tgridTables->Buttons[Row][Col];

            GridButton->Caption                = GridButton->Caption + "\r" + IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime.FormatString("hh:nn:ss");
            GridButton->Enabled = true;
            TDateTime OrderTime = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            if(!TGlobalSettings::Instance().UpdateTableGUIOnOrderStatus)
             {
                SetColorAsPerNoServiceTime(GridButton,OrderTime);
             }
            else
            {
               int status = TDBTables::GetTableBillingStatus(DBTransaction,TableNo);
               switch(status)
                {
                   // prelim
                   case ePrelim:
                   GridButton->Color = clGreen;
                   break;
                   //call away
                   case eCallAwayStatus:
                   GridButton->Color =  (TColor)RGB(230,230,0);
                   break;
                   case ePartialSplit:
                   //split and partial
                   GridButton->Color = (TColor)RGB(255,153,0);
                   break;
                   default :
                    SetColorAsPerNoServiceTime(GridButton,OrderTime);
                }
            }
            GridButton->FontColor	= clWhite;
        }
    }
}
//---------------------------------------------------------------------------
void TfrmSelectTable::SetColorAsPerNoServiceTime(TGridButton *GridButton,TDateTime OrderTime)
{
    TDateTime PeriodTillBadAsItGets = double(1.0 / 24.0 / 60.0) * double(TGlobalSettings::Instance().NonServicePeriodMinutes); // An Hour.
    if(double(PeriodTillBadAsItGets) == 0)
    {
        PeriodTillBadAsItGets = 1;
    }
    TDateTime Difference 	= Now() - OrderTime;
    float PercentageColour 	= double(Difference) / double(PeriodTillBadAsItGets);
    if(PercentageColour > 1) PercentageColour = 1;
    GridButton->Color = (TColor)RGB((192 * PercentageColour),0,256 - (256 * PercentageColour));
}

//---------------------------------------------------------------------------
void __fastcall TfrmSelectTable::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	ShowAll = false;
	ChangingName = false;
    tiUpdateTableReq->Enabled = false;

}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectTable::FormResize(TObject *Sender)
{
	 int Temp = Tag;
	 Tag = Screen->Width;
     ScaleBy(Screen->Width, Temp);
     Realign();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectTable::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectTable::stCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
    tiUpdateTableReq->Enabled = false;

}

//---------------------------------------------------------------------------
void __fastcall TfrmSelectTable::FormCreate(TObject *Sender)
{
	ShowAll = false;
	ChangingName = false;
    SelectedTabContainerNumber = 1;
	SelectedTabContainerName = "";
	SelectedPartyName = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectTable::tgridTablesMouseClick(TObject *Sender,
		TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{

    if (GridButton->Tag == 100)
	{
        if(TableMode)
        {
            TDBTables::SaveMezzanineAreaTables(MezzanineTables);
            MezzanineTables.clear();
            AssignedMezzanineTable.clear();
        }

		ModalResult = mrCancel;
	}
	else
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();

        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        SelectedTabContainerNumber = GridButton->Tag;

         SelectedPartyName = TDBTables::GetPartyName(DBTransaction,SelectedTabContainerNumber);
         SelectedTabContainerName = TDBTables::GetTableName(DBTransaction,SelectedTabContainerNumber);
         DBTransaction.Commit();
         if(!TableMode)
        {

        ModalResult = mrOk;

        }
        else
        {
            std::map<int, std::vector<TMezzanineTable> >::iterator outerit = MezzanineTables.find(SelectedTabContainerNumber);
            std::set<int>::iterator it = AssignedMezzanineTable.find(SelectedTabContainerNumber);
            bool isTableSelected;

            if(outerit != MezzanineTables.end())
            {
                isTableSelected = it != AssignedMezzanineTable.end();
                MezzanineTables.erase(SelectedTabContainerNumber);
            }
            else
            {
                isTableSelected = !(it != AssignedMezzanineTable.end());
                TMezzanineTable mezzanineTableDetails;
                mezzanineTableDetails.FloorplanVer = 1;
                mezzanineTableDetails.SelectionType = isTableSelected == true ? eSelected : eDeSelected;
                mezzanineTableDetails.LocationId = 0;
                MezzanineTables[SelectedTabContainerNumber].push_back(mezzanineTableDetails);
            }
             UpdateColor(SelectedTabContainerNumber, isTableSelected);
        }
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectTable::tiUpdateTableReqTimer(TObject *Sender)
{
                 UpdateFloorPlanSheet();
}
void TfrmSelectTable::UpdateFloorPlanSheet()
{
    SelectedTabContainerName = "";
	SelectedPartyName = "";
	for (int Row=0; Row < tgridTables->RowCount; Row++)
	{
		for (int Col=0; Col< tgridTables->ColCount; Col++)
		{
			int TableNo								= Row * tgridTables->RowCount + Col + 1;
			tgridTables->Buttons[Row][Col]->Caption = "Table #" + IntToStr(TableNo);
			tgridTables->Buttons[Row][Col]->FontColor = clBlack;
			tgridTables->Buttons[Row][Col]->Color = clWhite;
			tgridTables->Buttons[Row][Col]->Enabled = (ShowAll || ChangingName || TableNo == 100);
			tgridTables->Buttons[Row][Col]->Tag = TableNo;

			if(TableNo == 100)
			{
				tgridTables->Buttons[Row][Col]->Caption				= TableMode == false ? "Cancel" : "Ok";
				tgridTables->Buttons[Row][Col]->Color				= clMaroon;
				tgridTables->Buttons[Row][Col]->FontColor			= clWhite;
			}
		}
	}

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	// Set the tables caption.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	" SELECT "
	" t.\"TABLE_KEY\", t.\"TABLE_NUMBER\", t.\"TABLE_NAME\", t.\"PARTY_NAME\" "
	" FROM "
	"  \"TABLES\" t "
	" WHERE "
	" t.\"TABLE_NAME\" IS NOT NULL OR t.\"PARTY_NAME\" IS NOT NULL ";
	IBInternalQuery->ExecQuery();
	for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		int TableNo = IBInternalQuery->FieldByName("TABLE_NUMBER")->AsInteger;
		int Row = TableNo / tgridTables->RowCount;
		int Col = (TableNo % tgridTables->ColCount) - 1;

        // need to check whether the row and column is inside acceptable boundaries because once the floor plan is disabled the system will try to access
        // tables higher than 99 (using the table_number from db) where the grid will throw index-out-of bounds exceptions
        if(Row < tgridTables->RowCount && Col < tgridTables->ColCount)
        {
            TGridButton *GridButton = tgridTables->Buttons[Row][Col];

            AnsiString TableCaption;
            if (IBInternalQuery->FieldByName("TABLE_NAME")->AsString != "")
            {
                TableCaption				= IBInternalQuery->FieldByName("TABLE_NAME")->AsString;
            }
            else
            {
                TableCaption				= "Table #" + IntToStr(TableNo);
            }

            if (IBInternalQuery->FieldByName("PARTY_NAME")->AsString != "")
            {
                TableCaption				+= "\r" + IBInternalQuery->FieldByName("PARTY_NAME")->AsString;
            }

            if(TDBTables::IsTableMarked(DBTransaction, TableNo))
                TableCaption += " (S)";

            GridButton->Caption	 		= TableCaption;
        }
	}

    if(!TableMode)
    {
        SetTablesColor(DBTransaction);
    }
    else
    {
        SetMezzanineTablesColor();
    }
	if (SelectedTabContainerNumber == 0)
	{
		SelectedTabContainerNumber		= 1;
		SelectedTabContainerName		= "";
		SelectedPartyName					= "";
	}
	DBTransaction.Commit();
}
//-------------------------------------------------------------------------------------------------
void TfrmSelectTable::SetMezzanineTablesColor()
{
    for (std::set<int> ::iterator it = AssignedMezzanineTable.begin(); it != AssignedMezzanineTable.end(); ++it)
    {
		UpdateColor(*it, true);
    }
}
//----------------------------------------------------------------------------
void TfrmSelectTable::UpdateColor(int tableNo, bool isSelected)
{
    int Row = tableNo / tgridTables->RowCount;
    int Col = (tableNo % tgridTables->ColCount) - 1;

    if(Row < tgridTables->RowCount && Col < tgridTables->ColCount)
    {
        TGridButton *GridButton = tgridTables->Buttons[Row][Col];
        GridButton->Enabled = true;

        if(isSelected)
        {
            GridButton->Color = clGray;
            GridButton->FontColor	= clWhite;
        }
        else
        {
            GridButton->Color = clWhite;
            GridButton->FontColor	= clBlack;
        }
    }
}




