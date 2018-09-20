#pragma hdrstop

#include "EnableFloorPlan.h"

#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "DBTables.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)

// Singleton Object Instance set to NULL.
//TEnableFloorPlan* TEnableFloorPlan::enableFloorPlan = NULL;

//---------------------------------------------------------------------------
TEnableFloorPlan::TEnableFloorPlan()
{
    changingName = false;
}
//---------------------------------------------------------------------------
TEnableFloorPlan::~TEnableFloorPlan()
{
}
//---------------------------------------------------------------------------
//TEnableFloorPlan* TEnableFloorPlan::Instance()
//{
//    if( enableFloorPlan == NULL )
//    {
//        enableFloorPlan = new TEnableFloorPlan();
//    }
//
//    return enableFloorPlan;
//}
//................................................................

bool TEnableFloorPlan::Run(
                                 TForm* inOwner,
                                  bool  inShowAll,
                TFloorPlanReturnParams& inFloorPlanReturnParams, bool isNormalArea )
{
	if( initialiseFloorPlan() )
    {
		return runNewFloorPlan( inOwner, inFloorPlanReturnParams, isNormalArea );
    }
	else // Runs the old grid floor plan if it fails to connect to the web floorplan
    {
		return runBackupFloorPlan( inOwner, inShowAll, inFloorPlanReturnParams, isNormalArea );
    }
}
//................................................................

#define MAX_UNSIGNED_INT_32 199;
//#define MAX_UNSIGNED_INT_32 4294967295;

unsigned __int32 TEnableFloorPlan::GetMaxTableCount()
{
	if( initialiseFloorPlan() )
    {
		return MAX_UNSIGNED_INT_32;
    }
	else // Runs the old grid floor plan if it fails to connect to the web floorplan
    {
		return 99;
    }
}
//................................................................

unsigned __int32 TEnableFloorPlan::GetMaxSeatCount()
{
	return 99;
}
//................................................................

void TEnableFloorPlan::SetChangingName(bool inChangingName)
{
	changingName = inChangingName;
}
//................................................................

//TEnableFloorPlan::TEnableFloorPlan()
//{
//    changingName = false;
//}
//................................................................

bool TEnableFloorPlan::initialiseFloorPlan()
{
    // if it initialises ok then table plan service is online; must do this!
//    controller.release();
    controller.reset( new TablePlan::PlanController() );

    if ( !TGlobalSettings::Instance().ReservationsEnabled ||
         !controller->Init() )
    {
        controller.reset(NULL);
    }

    return controller.get() != NULL;
}
//................................................................
void TEnableFloorPlan::ReleaseFormMemory(std::auto_ptr<TFrmSelectTable2> frmSelectTable2)
{
    if(frmSelectTable2->imgTables)
    {
     delete frmSelectTable2->imgTables;
     frmSelectTable2->imgTables = NULL;
    }
    if(frmSelectTable2->panelTables)
    {
    delete frmSelectTable2->panelTables;
    frmSelectTable2->panelTables = NULL;
    }
    if(frmSelectTable2->Panel1)
    {
    delete frmSelectTable2->Panel1;
    frmSelectTable2->Panel1 = NULL;
    }
    if(frmSelectTable2->TouchBtn2)
    {
    delete frmSelectTable2->TouchBtn2;
    frmSelectTable2->TouchBtn2 = NULL;
    }
    if(frmSelectTable2->tgridLocations)
    {
    delete frmSelectTable2->tgridLocations;
    frmSelectTable2->tgridLocations = NULL;
    }
    if(frmSelectTable2->PnlLocation)
    {
     delete frmSelectTable2->PnlLocation;
     frmSelectTable2->PnlLocation = NULL;
    }
}
//................................................................
bool TEnableFloorPlan::runNewFloorPlan(TForm* inOwner, TFloorPlanReturnParams& inFloorPlanReturnParams, bool isNormalArea )
{
    bool retValue = false;
    std::auto_ptr<TFrmSelectTable2>frmSelectTable2(TFrmSelectTable2::Create<TFrmSelectTable2>(inOwner, TDeviceRealTerminal::Instance().DBControl));

    //for mezzanine Area.
    if(!isNormalArea)
    {
        frmSelectTable2->TouchBtn2->Caption = "Ok";
        frmSelectTable2->TableMode = eMizzanineMode;
    }

    // must do this!
    frmSelectTable2->AssociateWithController(controller);
    inFloorPlanReturnParams.Ver = eNewFloorPlan;

    bool needToReopen = false;
    if (frmSelectTable2->ShowModal() == mrOk)
    {
        needToReopen = frmSelectTable2->NeedToReopen;
        inFloorPlanReturnParams.TabContainerNumber = frmSelectTable2->SelectedTabContainerNumber;
        inFloorPlanReturnParams.TabContainerName   = frmSelectTable2->SelectedTabContainerName;
        inFloorPlanReturnParams.PartyName = frmSelectTable2->SelectedPartyName;
        ReleaseFormMemory(frmSelectTable2);
        frmSelectTable2.reset();
        controller.reset();
        retValue = true;
    }
    else
    {
        ReleaseFormMemory(frmSelectTable2);
        frmSelectTable2.reset();
        controller.reset();
        retValue = false;
    }
    if(needToReopen)
    {
       initialiseFloorPlan();
       runNewFloorPlan(inOwner,inFloorPlanReturnParams, isNormalArea );
    }
    return retValue;
}
//................................................................

bool TEnableFloorPlan::runBackupFloorPlan(TForm* inOwner, bool  inShowAll, TFloorPlanReturnParams& inFloorPlanReturnParams, bool isNormalArea)
{
    std::auto_ptr<TfrmSelectTable>frmSelectTable(TfrmSelectTable::Create<TfrmSelectTable>(inOwner, TDeviceRealTerminal::Instance().DBControl));

    frmSelectTable->ShowAll      = inShowAll;
    frmSelectTable->ChangingName = changingName;
    inFloorPlanReturnParams.Ver  = eOldFloorPlan;

     //for mezzanine Area.
     if(!isNormalArea)
     {
        frmSelectTable->TableMode = eMizzanineMode;
     }

    if (frmSelectTable->ShowModal() == mrOk)
    {
        inFloorPlanReturnParams.TabContainerNumber = frmSelectTable->SelectedTabContainerNumber;
        inFloorPlanReturnParams.TabContainerName   = frmSelectTable->SelectedTabContainerName;
        inFloorPlanReturnParams.PartyName          = frmSelectTable->SelectedPartyName;
        inFloorPlanReturnParams.HasOnlineOrders    = TDBTables::HasOnlineOrders(inFloorPlanReturnParams.TabContainerNumber);

        return true;
    }

	return false;
}
//................................................................

/*
TEnableFloorPlan::TEnableFloorPlan(TForm * inOwner)
  : Owner(inOwner), ChangingName(false)
{
}

bool TEnableFloorPlan::InitialiseFloorPlan(void)
{
		// if it initialises ok then table plan service is online; must do this!
		controller.reset(new TablePlan::PlanController());

		if (!TGlobalSettings::Instance().ReservationsEnabled
			 || !controller->Init())
			controller.reset(NULL);

		return controller.get() != NULL;
}

bool TEnableFloorPlan::Run(TFloorPlanReturnParams &inFloorPlanReturnParams, bool ShowAll)
{
	if(this->InitialiseFloorPlan())
		return this->RunNewFloorPlan(inFloorPlanReturnParams);
	else // Runs the old grid floor plan if it fails to connect to the web floorplan
		return this->RunBackupFloorPlan(inFloorPlanReturnParams, ShowAll);
}


bool TEnableFloorPlan::RunNewFloorPlan(TFloorPlanReturnParams &inFloorPlanReturnParams)
{
			std::auto_ptr<TFrmSelectTable2>frmSelectTable2(TFrmSelectTable2::Create<TFrmSelectTable2>(Owner, TDeviceRealTerminal::Instance().DBControl));
			// must do this!
			frmSelectTable2->AssociateWithController(controller.get());
			inFloorPlanReturnParams.Ver = eNewFloorPlan;
			if (frmSelectTable2->ShowModal() == mrOk)
			{
				inFloorPlanReturnParams.TabContainerNumber = frmSelectTable2->SelectedTabContainerNumber;
				inFloorPlanReturnParams.TabContainerName = frmSelectTable2->SelectedTabContainerName;

				return true;
			}
	return false;
}


bool TEnableFloorPlan::RunBackupFloorPlan(TFloorPlanReturnParams &inFloorPlanReturnParams, bool ShowAll)
{
			std::auto_ptr<TfrmSelectTable>frmSelectTable(TfrmSelectTable::Create<TfrmSelectTable>(Owner, TDeviceRealTerminal::Instance().DBControl));
			frmSelectTable->ShowAll = ShowAll;
			frmSelectTable->ChangingName = ChangingName;
			inFloorPlanReturnParams.Ver = eOldFloorPlan;
			if (frmSelectTable->ShowModal() == mrOk)
			{
				inFloorPlanReturnParams.TabContainerNumber = frmSelectTable->SelectedTabContainerNumber;
				inFloorPlanReturnParams.TabContainerName = frmSelectTable->SelectedTabContainerName;
				inFloorPlanReturnParams.PartyName = frmSelectTable->SelectedPartyName;

				return true;
			}
	return false;
}

void TEnableFloorPlan::SetChangingName(bool inChangingName)
{
	ChangingName = inChangingName;
}
*/

