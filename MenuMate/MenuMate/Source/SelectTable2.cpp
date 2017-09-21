// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectTable2.h"
#include "DeviceRealTerminal.h"

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
        TMezzanineTable tableDetails;
        tableDetails.FloorplanVer = 0;
        tableDetails.LocationId = TGlobalSettings::Instance().LastSelectedFloorPlanLocationID;
        AssignedMezzanineTable = TDBTables::GetMezzanineAreaTables(tableDetails);
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
		_controller->DrawCurrentPlan(DBTransaction);
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
            std::set<int>::iterator it = AssignedMezzanineTable.find(SelectedTabContainerNumber);
            bool isTableSelected, isTableAlreadyInserted = false;

            if(it != AssignedMezzanineTable.end())
                isTableSelected = false;
            else
                isTableSelected = true;

            if(outerit != MezzanineTables.end())
            {
                for(std::vector<TMezzanineTable>::iterator innerit = outerit->second.begin(); innerit != outerit->second.end(); ++innerit)
                {
                    if(innerit->LocationId == TGlobalSettings::Instance().LastSelectedFloorPlanLocationID)
                    {
                        MezzanineTables.erase(SelectedTabContainerNumber);
                        isTableAlreadyInserted = true;
                    }
                }
            }


            if(outerit == MezzanineTables.end() || !isTableAlreadyInserted)
            {  
                TMezzanineTable mezzanineTableDetails;
                mezzanineTableDetails.FloorplanVer = 0;
                mezzanineTableDetails.SelectionType = isTableSelected == true ? eSelected : eDeSelected;
                mezzanineTableDetails.LocationId = TGlobalSettings::Instance().LastSelectedFloorPlanLocationID;
                MezzanineTables[SelectedTabContainerNumber].push_back(mezzanineTableDetails);
            }
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
        AssignedMezzanineTable.clear();
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
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmLastSelectedFloorPlanLocationID,
    TGlobalSettings::Instance().LastSelectedFloorPlanLocationID);
    DBTransaction.Commit();

	_controller->SetLocation(TGlobalSettings::Instance().LastSelectedFloorPlanLocationID);
	PnlLocation->Caption = _controller->GetCurrentPlanName();
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
		_controller->DrawCurrentPlan(DBTransaction);
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

