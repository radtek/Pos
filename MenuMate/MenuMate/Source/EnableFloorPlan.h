//---------------------------------------------------------------------------

#ifndef EnableFloorPlanH
#define EnableFloorPlanH

#include "Forms.hpp"
#include "SelectTable.h"
#include "SelectTable2.h"
#include "PlanController.h"

//---------------------------------------------------------------------------
enum eFloorPlanVer { eNewFloorPlan, eOldFloorPlan };

struct TFloorPlanReturnParams {
	UnicodeString TabContainerName;
	UnicodeString PartyName;
	int TabContainerNumber;
	eFloorPlanVer Ver;
};

class TEnableFloorPlan
{
	public:
        static TEnableFloorPlan* Instance();

		bool Run(
                                 TForm* inOwner,
                                  bool  inShowAll,
                TFloorPlanReturnParams& inFloorPlanReturnParams );

        void SetChangingName(bool inChangingName);
		unsigned __int32 GetMaxTableCount();
        unsigned __int32 GetMaxSeatCount();

	private:
        TEnableFloorPlan();
        static TEnableFloorPlan* enableFloorPlan;

		bool runNewFloorPlan(
                                 TForm* inOwner,
                TFloorPlanReturnParams& inFloorPlanReturnParams );

		bool runBackupFloorPlan(
                                 TForm* inOwner,
                                  bool  inShowAll,
                TFloorPlanReturnParams& inFloorPlanReturnParams );

		bool initialiseFloorPlan();

		bool changingName;
		std::auto_ptr<TablePlan::PlanController> controller;
        void ReleaseFormMemory(std::auto_ptr<TFrmSelectTable2> frmSelectTable2);
};

/*
class TEnableFloorPlan
{
	public:
		TEnableFloorPlan(TForm * inOwner);
		bool Run(TFloorPlanReturnParams &inFloorPlanReturnParams, bool ShowAll);
		void SetChangingName(bool inChangingName);

	private:
		bool  ChangingName;
		TForm *Owner;
		std::auto_ptr<TablePlan::PlanController> controller;

		bool RunBackupFloorPlan(TFloorPlanReturnParams &inFloorPlanReturnParams, bool ShowAll);
		bool RunNewFloorPlan(TFloorPlanReturnParams &inFloorPlanReturnParams);
		bool InitialiseFloorPlan(void);

};
*/

#endif
