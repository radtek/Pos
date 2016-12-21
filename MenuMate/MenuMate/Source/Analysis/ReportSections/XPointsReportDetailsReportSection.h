#ifndef XPointsReportDetailsReportSectionH
#define XPointsReportDetailsReportSectionH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "Membership.h"

class XPointsReportDetailsReportSection : public BaseReportSection
{

public:
    XPointsReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    XPointsReportDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
    ~XPointsReportDetailsReportSection();

    virtual void GetOutput(TPrintout* printOut);
    //virtual void GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime);

private:
    TMembership* _memberShip;
    DataFormatUtilities* _dataFormatUtilities;

    void GetGiftCardOutput(TPrintout* printOut);
    void GetFirstVisitOutput(TPrintout* printOut);
    void GetBirthdayOutput(TPrintout* printOut);
    void GetOutputForAdjustmentType(int adjustmentType, UnicodeString title, TPrintout* printOut);
    TIBSQL* GetPointsQuery(int adjustmentType);
    void GetGiftCardOutputForConsolidatedZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition);
    void GetGiftCardOutputForNormalZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition);
    void GetPointsQueryForConsolidatedZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition);
    void GetPointsQueryForNormalZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition);
};

#endif
