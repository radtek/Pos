#ifndef XMasterBlindBalancesDetailsReportSectionH
#define XMasterBlindBalancesDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "BlindBalanceController.h"

class XMasterBlindBalancesDetailsReportSection : public BaseReportSection
{

public:
    XMasterBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XMasterBlindBalancesDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XMasterBlindBalancesDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);
};

#endif
