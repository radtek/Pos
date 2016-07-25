#ifndef PointsReportHeaderTraitH
#define PointsReportHeaderTraitH

#include "BaseReportSectionDisplayTraits.h"

class PointsReportHeaderTrait : public BaseReportSectionDisplayTraits
{
public:
    PointsReportHeaderTrait::PointsReportHeaderTrait(ReportSectionTraitType reportSectionTraitType);
	void ApplyTraits(TPrintout* printOut);
};


#endif
