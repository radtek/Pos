#ifndef IReportSectionFactoryH
#define IReportSectionFactoryH

#include "IReportSection.h"
#include "ReportEnums.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"


class IReportSectionFactory
{
public:
	virtual IReportSection* CreateReportSection(ReportSectionType reportSectionType) = 0;
};

#endif

