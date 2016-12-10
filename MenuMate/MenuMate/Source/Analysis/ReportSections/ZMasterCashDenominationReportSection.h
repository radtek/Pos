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
    ~ZMasterCashDenominationReportSection();

    virtual void GetOutput(TPrintout* printOut);
};
#endif
