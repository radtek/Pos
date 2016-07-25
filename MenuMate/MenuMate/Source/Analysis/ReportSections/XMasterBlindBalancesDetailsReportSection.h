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
    ~XMasterBlindBalancesDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
};

#endif
