#ifndef BaseReportSectionDisplayTraitsH
#define BaseReportSectionDisplayTraitsH

#include "IReportSectionDisplayTraits.h"
#include "ReportEnums.h"

class BaseReportSectionDisplayTraits : public IReportSectionDisplayTraits
{
public:
    BaseReportSectionDisplayTraits(ReportSectionTraitType reportSectionTraitType);
    virtual ReportSectionTraitType GetTraitType();

protected:
    ReportSectionTraitType _reportSectionTraitType;
};

#endif
