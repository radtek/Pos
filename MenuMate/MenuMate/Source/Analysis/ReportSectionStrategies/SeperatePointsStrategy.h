#ifndef SeperatePointsStrategyH
#define SeperatePointsStrategyH

#include "BaseReportSectionDisplayStrategy.h"
#include "Printout.h"
#include "Membership.h"
#include "ReportUtilities.h"
#include "IReportSectionDisplayTraits.h"

class SeperatePointsStrategy : public BaseReportSectionDisplayStrategy
{
public:
    SeperatePointsStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, IReportSectionDisplayTraits* pointsReportHeaderTraits);
    SeperatePointsStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, IReportSectionDisplayTraits* pointsReportHeaderTraits, TDateTime* startTime, TDateTime* endTime);
	virtual void BuildSection(TPrintout* printOut);

protected:
    DataFormatUtilities* _dataFormatUtilities;
    TMembership* _memberShip;
    IReportSectionDisplayTraits* _pointsReportHeaderTraits;

private:
  void GetPointDetailsNormalZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition);
  void GetPointDetailsConsolidatedZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition);
  void GetEarnedPointDetailsForNormalZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition);
  void GetEarnedPointDetailsForConsolidatedZed(TIBSQL *ibInternalQuery, AnsiString masterSlaveCondition);

};

#endif
