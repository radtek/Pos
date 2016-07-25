#pragma hdrstop

#include "EnableFloorPlan.h"

#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "SelectTable2.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// Singleton Object Instance set to NULL.
TEnableFloorPlan* TEnableFloorPlan::enableFloorPlan = NULL;

TEnableFloorPlan* TEnableFloorPlan::Instance()
{
    if( enableFloorPlan == NULL )
    {
        enableFloorPlan = new TEnableFloorPlan();
    }

    return enableFloorPlan;
}
//................................................................

bool TEnableFloorPlan::Run(
                                 TForm* inOwner,
                                  bool  inShowAll,
                TFloorPlanReturnParams& inFloorPlanReturnParams )
{
	if( initialiseFloorPlan() )
    {
		return runNewFloorPlan( inOwner, inFloorPlanReturnParams );
    }
	else // Runs the old grid floor plan if it fails to connect to the web floorplan
    {
		return runBackupFloorPlan( inOwner, inShowAll, inFloorPlanReturnParams );
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

TEnableFloorPlan::TEnableFloorPlan()
{
    changingName = false;
}
//................................................................

bool TEnableFloorPlan::initialiseFloorPlan()
{
    // if it initialises ok then table plan service is online; must do this!
    controller.reset( new TablePlan::PlanController() );

    if ( !TGlobalSettings::Instance().ReservationsEnabled ||
         !controller->Init() )
    {
        controller.reset(NULL);
    }

    return controller.get() != NULL;
}
//................................................................

bool TEnableFloorPlan::runNewFloorPlan(
                                 TForm* inOwner,
                TFloorPlanReturnParams& inFloorPlanReturnParams )
{
    std::auto_ptr<TFrmSelectTable2>frmSelectTable2(TFrmSelectTable2::Create<TFrmSelectTable2>(inOwner, TDeviceRealTerminal::Instance().DBControl));

    // must do this!
    frmSelectTable2->AssociateWithController( controller.get() );
    inFloorPlanReturnParams.Ver = eNewFloorPlan;

    if (frmSelectTable2->ShowModal() == mrOk)
    {
        inFloorPlanReturnParams.TabContainerNumber = frmSelectTable2->SelectedTabContainerNumber;
        inFloorPlanReturnParams.TabContainerName   = frmSelectTable2->SelectedTabContainerName;
        inFloorPlanReturnParams.PartyName = frmSelectTable2->SelectedPartyName;
        return true;
    }

	return false;
}
//................................................................

bool TEnableFloorPlan::runBackupFloorPlan(
                                 TForm* inOwner,
                                  bool  inShowAll,
                TFloorPlanReturnParams& inFloorPlanReturnParams )
{
    std::auto_ptr<TfrmSelectTable>frmSelectTable(TfrmSelectTable::Create<TfrmSelectTable>(inOwner, TDeviceRealTerminal::Instance().DBControl));

    frmSelectTable->ShowAll      = inShowAll;
    frmSelectTable->ChangingName = changingName;
    inFloorPlanReturnParams.Ver  = eOldFloorPlan;

    if (frmSelectTable->ShowModal() == mrOk)
    {
        inFloorPlanReturnParams.TabContainerNumber = frmSelectTable->SelectedTabContainerNumber;
        inFloorPlanReturnParams.TabContainerName   = frmSelectTable->SelectedTabContainerName;
        inFloorPlanReturnParams.PartyName          = frmSelectTable->SelectedPartyName;

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

