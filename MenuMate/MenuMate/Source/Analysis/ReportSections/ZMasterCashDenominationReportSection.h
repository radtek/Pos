//---------------------------------------------------------------------------

#ifndef ZMasterCashDenominationReportSectionH
#define ZMasterCashDenominationReportSectionH
#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
//---------------------------------------------------------------------------
class ZMasterCashDenominationReportSection : public BaseReportSection
{

public:
    ZMasterCashDenominationReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    ZMasterCashDenominationReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~ZMasterCashDenominationReportSection();

    virtual void GetOutput(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);
};
#endif
