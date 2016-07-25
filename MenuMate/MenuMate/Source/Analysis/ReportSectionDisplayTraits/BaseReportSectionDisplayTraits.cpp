#include "BaseReportSectionDisplayTraits.h"

BaseReportSectionDisplayTraits::BaseReportSectionDisplayTraits(ReportSectionTraitType reportSectionTraitType)
        : _reportSectionTraitType(reportSectionTraitType)
{
}

ReportSectionTraitType BaseReportSectionDisplayTraits::GetTraitType()
{
    return _reportSectionTraitType;
}
