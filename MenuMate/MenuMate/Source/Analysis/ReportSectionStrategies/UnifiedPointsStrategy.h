#ifndef UnifiedPointsStrategyH
#define UnifiedPointsStrategyH

#include "BaseReportSectionDisplayStrategy.h"
#include "Printout.h"
#include "Membership.h"
#include "ReportUtilities.h"
#include "IReportSectionDisplayTraits.h"

class UnifiedPointsStrategy : public BaseReportSectionDisplayStrategy
{
public:
    UnifiedPointsStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, IReportSectionDisplayTraits* pointsReportHeaderTraits);
    UnifiedPointsStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, IReportSectionDisplayTraits* pointsReportHeaderTraits, TDateTime* startTime, TDateTime* endTime);
	virtual void BuildSection(TPrintout* printOut);

protected:
    DataFormatUtilities* _dataFormatUtilities;
    TMembership* _memberShip;
    IReportSectionDisplayTraits* _pointsReportHeaderTraits;
private:
   void GetPointsDetailsForNormalZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition);
   void GetPointsDetailsForConsolidatedZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition);

};

#endif
