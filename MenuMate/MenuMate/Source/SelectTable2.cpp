// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectTable2.h"
#include "DeviceRealTerminal.h"
#include "VerticalSelect.h"
#include "BillGroup.h"
#include "DBOrder.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchBtn"
#pragma resource "*.dfm"
TFrmSelectTable2 *frmSelectTable2;
// ---------------------------------------------------------------------------
__fastcall TFrmSelectTable2::TFrmSelectTable2(TComponent* Owner, Database::TDBControl &inIBDatabase) : TZForm(Owner), _iBDatabase(inIBDatabase)
{
    TableMode = eNormalNode;
}
// ----------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::FormShow(TObject *Sender)
{
    UpdateTableOnFormShow();
    NeedToReopen = false;
    if(TableMode)
    {
        AssignedMezzanineTables = LoadMizzanineTables();
    }
    else
    {
        tiUpdateFloorPlanReq->Enabled = true;
        tiUpdateFloorPlanRefresh->Enabled = true;
    }
}
// ---------------------------------------------------------------------------
void TFrmSelectTable2::AssociateWithController(std::auto_ptr<TablePlan::PlanController> c)
{
    _controller = c;
    UpdateTableFloorPlan();
}
// ---------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::FormPaint(TObject *Sender)
{
     UpdateTableOnFormPaint();
}
// ---------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::FormResize(TObject *Sender)
{
	if (_controller->IsInitOk())
	{
        imgTables->Align = alNone;
        imgTables->Align = alClient;
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
        if(TableMode)
        {    
		    _controller->DrawCurrentPlan(DBTransaction, false); 
            DrawMezzanineArea();
        }
        else
        {
            _controller->DrawCurrentPlan(DBTransaction);
        }
		DBTransaction.Commit();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::imgTablesMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	_controller->UpdateMousePos(X, Y);
}
// ---------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::imgTablesClick(TObject *Sender)
{
    tiUpdateFloorPlanReq->Enabled = false;
    tiUpdateFloorPlanRefresh->Enabled = false;
	AnsiString message(_controller->GetTableDesc());
	DTOReservable *Table = _controller->GetCurrentTable();

	if (Table != NULL )
	{
		SelectedTabContainerName = Table->Name;
		SelectedTabContainerNumber = Table->Number;
		// Keep our Tables DB in sync for reports.
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		SelectedPartyName = TDBTables::GetPartyName(DBTransaction, SelectedTabContainerNumber);
		TDBTables::SetTableName(DBTransaction, SelectedTabContainerNumber, SelectedTabContainerName);
        DBTransaction.Commit();

        if(!TableMode)
        {
            if(_controller->image)
            {
                delete _controller->image;
                _controller->image = NULL;
            }
            if(_controller->locations.size())
            {
                _controller->locations.clear();
            }
            _controller.reset();
            ModalResult = mrOk;
        }
        else
        {
            std::map<int, std::vector<TMezzanineTable> >::iterator outerit = MezzanineTables.find(SelectedTabContainerNumber);
            std::set<int>::iterator it = AssignedMezzanineTables.find(SelectedTabContainerNumber);
            bool isTableSelected, isTableAlreadyInserted = false;

            if(outerit != MezzanineTables.end())
            {
                for(std::vector<TMezzanineTable>::iterator innerit = outerit->second.begin(); innerit != outerit->second.end(); ++innerit)
                {
                    if(innerit->LocationId == TGlobalSettings::Instance().LastSelectedFloorPlanLocationID)
                    {
                        MezzanineTables.erase(SelectedTabContainerNumber);
                        isTableAlreadyInserted = true;
                        isTableSelected = it != AssignedMezzanineTables.end();
                        break;
                    }
                }
            }

            if(outerit == MezzanineTables.end() || !isTableAlreadyInserted)
            {
                isTableSelected = (it == AssignedMezzanineTables.end());
                TMezzanineTable mezzanineTableDetails;
                mezzanineTableDetails.FloorplanVer = 0;
                mezzanineTableDetails.SelectionType = isTableSelected == true ? eSelected : eDeSelected;
                mezzanineTableDetails.LocationId = TGlobalSettings::Instance().LastSelectedFloorPlanLocationID;
                MezzanineTables[SelectedTabContainerNumber].push_back(mezzanineTableDetails);
            }
            DrawMezzanineArea(false, isTableSelected);
	  }
    }
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::TouchBtn2MouseClick(TObject *Sender)
{

    tiUpdateFloorPlanReq->Enabled = false;
    tiUpdateFloorPlanRefresh->Enabled = false;

    if(TableMode)
    {
        TDBTables::SaveMezzanineAreaTables(MezzanineTables);
        MezzanineTables.clear();
        AssignedMezzanineTables.clear();
    }

    if(_controller->image)
    {
        delete _controller->image;
        _controller->image = NULL;
    }
    if(_controller->locations.size())
    {
        _controller->locations.clear();
    }
    _controller.reset();

	ModalResult = mrCancel;
}
// ---------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::tgridLocationsMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    TGlobalSettings::Instance().LastSelectedFloorPlanLocationID = GridButton->Tag;
	SaveLocationId(TGlobalSettings::Instance().LastSelectedFloorPlanLocationID);
	_controller->SetLocation(TGlobalSettings::Instance().LastSelectedFloorPlanLocationID);
	PnlLocation->Caption = _controller->GetCurrentPlanName();

    if(TableMode)
    {
        AssignedMezzanineTables.clear();
        AssignedMezzanineTables = LoadMizzanineTables();
    }

	this->Invalidate();
}
// ---------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::tiUpdateFloorPlanReqTimer(TObject *Sender)
{
   UpdateTableFloorPlan();
   UpdateTableOnFormShow();
   UpdateTableOnFormPaint();
}
//----------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::tiUpdateFloorPlanRefreshTimer(TObject *Sender)
{
    tiUpdateFloorPlanReq->Enabled = false;
    tiUpdateFloorPlanRefresh->Enabled = false;
    if(MessageBox("The screen was left idle for 5 minutes. Do you wish to continue?","Information",MB_YESNO + MB_ICONQUESTION) == IDYES)
    {
          NeedToReopen = true;
          if(_controller->image)
          {
            delete _controller->image;
            _controller->image = NULL;
          }
          if(_controller->locations.size())
          {
            _controller->locations.clear();
          }
        _controller.reset();
        ModalResult = mrOk;
    }
    else
    {
      if(_controller->image)
      {
        delete _controller->image;
        _controller->image = NULL;
      }
      if(_controller->locations.size())
      {
        _controller->locations.clear();
      }
       _controller.reset();
       ModalResult = mrCancel;
    }
}
// ---------------------------------------------------------------------------
void TFrmSelectTable2::UpdateTableFloorPlan()
{
	_controller->SetView(imgTables);

	SelectedTabContainerName = "";
	SelectedPartyName = "";
	SelectedTabContainerNumber = 0;

	tgridLocations->ColCount = 0;
	tgridLocations->RowCount = 0;

	std::vector<DTOLocation*>Locations = _controller->getLocations();
//    std::vector<TPlanLocation>Locations = _controller->getLocations();
    _controller->SetLocation(TGlobalSettings::Instance().LastSelectedFloorPlanLocationID);
	PnlLocation->Caption = _controller->GetCurrentPlanName();

	if (Locations.size() != 0)
	{
		tgridLocations->ColCount = 1;
		tgridLocations->RowCount = Locations.size();

        if(TGlobalSettings::Instance().LastSelectedFloorPlanLocationID == -1 && TableMode)
            SaveLocationId(Locations[0]->Id);

		for (int i = 0; i < Locations.size(); i++)
		{
			tgridLocations->Buttons[i][0]->Caption = Locations[i]->Name;
			tgridLocations->Buttons[i][0]->Color = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_BUTTONCOLOR];
			tgridLocations->Buttons[i][0]->FontColor = ButtonColors[BUTTONTYPE_UNSELECTED][ATTRIB_FONTCOLOR];
			tgridLocations->Buttons[i][0]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
			tgridLocations->Buttons[i][0]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
			tgridLocations->Buttons[i][0]->Tag = Locations[i]->Id;
			tgridLocations->Buttons[i][0]->Visible = true;
			if (Locations[i]->Id == _controller->CurrentID())
			{
				tgridLocations->Buttons[i][0]->Latched = true;
			}
		}
	}
    Locations.clear();
}
// ---------------------------------------------------------------------------
void TFrmSelectTable2::UpdateTableOnFormShow()
{
  	SetGridColors(tgridLocations);
	SelectedTabContainerName = "";
	SelectedPartyName = "";
	SelectedTabContainerNumber = 0;
}
// ---------------------------------------------------------------------------
void TFrmSelectTable2::UpdateTableOnFormPaint()
{
  if (_controller->IsInitOk())
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

        if(TableMode)
        {
		    _controller->DrawCurrentPlan(DBTransaction, false);
            DrawMezzanineArea();
        }
        else
        {
            _controller->DrawCurrentPlan(DBTransaction);
        }
		DBTransaction.Commit();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::FormClose(TObject *Sender)
{
   tiUpdateFloorPlanReq->Enabled = false;
   tiUpdateFloorPlanRefresh->Enabled = false;
}
// ---------------------------------------------------------------------------
void TFrmSelectTable2::DrawMezzanineArea(bool isLoadTime, bool isTableSelected)
{
	// save canvas brush
	TBrushRecall *savedBrush = new TBrushRecall(_controller->image->Canvas->Brush);
	savedBrush->Store();
    try
    {
        if (_controller->locations.size() > 0)
        {
            ArrayOfDTOReservable tables = _controller->_client->GetTablesForLocation(TGlobalSettings::Instance().LastSelectedFloorPlanLocationID);
            bool isTableExist;

            for (int i = 0; i < tables.Length; i++)
            {
                isTableExist = false;
                if(isLoadTime)
                {
                    for(std::map<int, std::vector<TMezzanineTable> >::iterator outerit = MezzanineTables.begin();
                                outerit!= MezzanineTables.end(); ++outerit)
                    {
                        if(tables[i]->Number == outerit->first)
                        {
                            for(std::vector<TMezzanineTable>::iterator innerit = outerit->second.begin(); innerit != outerit->second.end(); ++innerit)
                            {
                                if( innerit->LocationId == TGlobalSettings::Instance().LastSelectedFloorPlanLocationID)
                                {
                                    isTableExist = true;
                                    isTableSelected =  innerit->SelectionType == eSelected ? true : false;
                                    break;
                                }
                            }
                        }
                    }

                    if(!isTableExist)
                    {
                        std::set<int>::iterator it = AssignedMezzanineTables.find(tables[i]->Number);
                        isTableExist = it != AssignedMezzanineTables.end();
                    }
                }
                else
                {
                    isTableExist = tables[i]->Number == SelectedTabContainerNumber;
                }

                if(isTableExist)
                {
                    int OrigX = tables[i]->X;
                    int OrigY = tables[i]->Y;
                    int OrigHeight = tables[i]->Height;
                    int OrigWidth  = tables[i]->Width;
                    OrigX = ((double)OrigX) * _controller->ScaleFactor;
                    OrigWidth = ((double)OrigWidth) * _controller->ScaleFactor;
                    OrigY = ((double)OrigY) * _controller->ScaleFactor;
                    OrigHeight = ((double)OrigHeight) * _controller->ScaleFactor;
                    TRect rect(OrigX, OrigY, OrigX + OrigWidth , OrigY + OrigHeight );

                    if((isLoadTime && isTableSelected) || isTableSelected)
                    {
                        _controller->image->Canvas->Brush->Style = bsClear;
                        _controller->image->Canvas->Brush->Color = clGray;
                        _controller->image->Canvas->Font->Color =  clWhite;
                    }
                    else
                    {
                        _controller->image->Canvas->Brush->Color = clSilver;
                        _controller->image->Canvas->Font->Color =  clBlack;
                        _controller->image->Canvas->Font->Color = clBlack;
                    }

                    if (tables[i]->Shape == "r")
                    {
                        _controller->image->Canvas->FillRect(rect);
                    }
                    else
                    {
                        _controller->image->Canvas->Ellipse(rect);
                    }

                    std::auto_ptr <TStringList> TableText(new TStringList);
                    TableText->Add(tables[i]->Name);
                    int LineHeight = _controller->image->Canvas->TextHeight(tables[i]->Name.t_str());

                    int TotalTxtHeight = LineHeight * TableText->Count;

                    int Top = rect.Top + (rect.Height() / 2) - (TotalTxtHeight / 2);
                    for(int i = 0; i < TableText->Count; i++)
                    {
                        int txtWidth = _controller->image->Canvas->TextWidth(TableText->Strings[i]);
                        int Left = rect.Left + (rect.Width() / 2) - (txtWidth / 2);
                        _controller->image->Canvas->TextOutA(Left,Top , TableText->Strings[i]);
                        Top += LineHeight;
                    }
                }
                if(isTableExist && !isLoadTime)
                    break;
            }
        }
        delete savedBrush;
    }
    catch(Exception & Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Mezzanine Area Error " + Err.Message);
    }
}
//----------------------------------------------------------------------------------------------------
std::set<int> TFrmSelectTable2::LoadMizzanineTables()
{
    std::set<int> MezzanineTables;
    TMezzanineTable tableDetails;
    tableDetails.FloorplanVer = 0;
    tableDetails.LocationId = TGlobalSettings::Instance().LastSelectedFloorPlanLocationID;
    MezzanineTables = TDBTables::GetMezzanineAreaTables(tableDetails);

    return MezzanineTables;
}
//-------------------------------------------------------------------------------------------------
void TFrmSelectTable2::SaveLocationId(int locationId)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TGlobalSettings::Instance().LastSelectedFloorPlanLocationID = locationId;
    TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmLastSelectedFloorPlanLocationID, TGlobalSettings::Instance().LastSelectedFloorPlanLocationID);
    DBTransaction.Commit();
}
//--------------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::tiTimerEnableReqTimer(TObject *Sender)
{
    Database::TDBTransaction dBTransaction(TDeviceRealTerminal::Instance().DBControl);
    dBTransaction.StartTransaction();
    try
    {
        tiUpdateFloorPlanReq->Enabled = false;
        tiUpdateFloorPlanRefresh->Enabled = false;
        AnsiString message(_controller->GetTableDesc());
        DTOReservable *Table = _controller->GetCurrentTable();

        if (Table != NULL )
        {
            SelectedTabContainerName = Table->Name;
            SelectedTabContainerNumber = Table->Number;

            bool isTableAlreadySeated = TDBTables::IsTableMarked(dBTransaction, SelectedTabContainerNumber);
            bool showSelectionForm = false;
            std::auto_ptr<TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));

            TVerticalSelection Item;
            Item.Title = "Cancel";
            Item.Properties["Color"] = IntToStr(clMaroon);
            Item.CloseSelection = true;
            SelectionForm->Items.push_back(Item);

            if(TGlobalSettings::Instance().EnableOnlineOrdering)
            {
                showSelectionForm = true;
                TVerticalSelection Item1;
                Item1.Title = "Seat";
                Item1.Properties["Action"] = IntToStr(1);

                if(isTableAlreadySeated)
                {
                    Item1.Properties["Color"] = IntToStr(clGreen);
                }
                else
                {
                    Item1.Properties["Color"] = IntToStr(clNavy);
                }

                Item1.CloseSelection = true;
                SelectionForm->Items.push_back(Item1);
            }

            if(TDBOrder::IsOrderSavedToTable(dBTransaction, SelectedTabContainerNumber))
            {
                showSelectionForm = true;
                TVerticalSelection Item2;
                Item2.Title = "Bill";
                Item2.Properties["Action"] = IntToStr(2);
                Item2.Properties["Color"] = IntToStr(clNavy);
                Item2.CloseSelection = true;
                SelectionForm->Items.push_back(Item2);
            }

            SelectionForm->ShowModal();

            TVerticalSelection SelectedItem;
            if(SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel" )
            {
                int Action = StrToIntDef(SelectedItem.Properties["Action"],0);
                switch(Action)
                {
                    case 1 :
                    {
                        UnicodeString tableName =  "Table " + IntToStr(SelectedTabContainerNumber);
                        if(isTableAlreadySeated)
                        {
                            MessageBox(tableName + " is now Not Seated.", "Info", MB_OK + MB_ICONINFORMATION);
                            TDBTables::UpdateTableStateForOO(dBTransaction, SelectedTabContainerNumber, false);
                        }
                        else
                        {
                            MessageBox(tableName + " is now Seated.", "Info", MB_OK + MB_ICONINFORMATION);
                            SelectedPartyName = TDBTables::GetPartyName(dBTransaction, SelectedTabContainerNumber);
                            TDBTables::SetTableName(dBTransaction, SelectedTabContainerNumber, SelectedTabContainerName, true);
                        }
                        _controller->DrawCurrentPlan(dBTransaction);

                    }break;
                    case 2 :
                    {
                        TfrmBillGroup* frmBillGroup  = new  TfrmBillGroup(this, TDeviceRealTerminal::Instance().DBControl);
                        frmBillGroup->CurrentTable = SelectedTabContainerNumber;
                        frmBillGroup->CurrentDisplayMode = eTables;
                        frmBillGroup->HasOnlineOrders = TDBTables::HasOnlineOrders(SelectedTabContainerNumber);

                        frmBillGroup->ShowModal();
                        delete frmBillGroup;
                        frmBillGroup = NULL;
                    }break;
                }
            }
         }
         dBTransaction.Commit();
    }
    catch(Exception & Err)
    {
        dBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Mezzanine Area Error " + Err.Message);
    }

}
//--------------------------------------------------------------------------------
void __fastcall TFrmSelectTable2::imgTablesMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    if(TGlobalSettings::Instance().EnableOnlineOrdering)
        tiTimerEnableReq->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSelectTable2::imgTablesMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    tiTimerEnableReq->Enabled = false;
}
//---------------------------------------------------------------------------

