#ifndef IReportSectionDisplayTraitsH
#define IReportSectionDisplayTraitsH

#include "Printout.h"
#include "ReportEnums.h"

class IReportSectionDisplayTraits
{
public:
	virtual void ApplyTraits(TPrintout* printOut) = 0;
    virtual ReportSectionTraitType GetTraitType() = 0;
};

#endif
