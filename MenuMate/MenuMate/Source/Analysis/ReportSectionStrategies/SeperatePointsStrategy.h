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
	virtual void BuildSection(TPrintout* printOut);

protected:
    DataFormatUtilities* _dataFormatUtilities;
    TMembership* _memberShip;
    IReportSectionDisplayTraits* _pointsReportHeaderTraits;

};

#endif
