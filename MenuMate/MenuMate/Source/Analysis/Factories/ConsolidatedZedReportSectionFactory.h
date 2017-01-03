#ifndef ConsolidatedZedReportSectionFactoryH
#define ConsolidatedZedReportSectionFactoryH

#include <memory>
#include "ReportEnums.h"
#include "IReportSection.h"
#include "BaseReportSectionFactory.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class ConsolidatedZedReportSectionFactory : public BaseReportSectionFactory
{
public:
	ConsolidatedZedReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~ConsolidatedZedReportSectionFactory();

	IReportSection* CreateReportSection(ReportSectionType reportSectionType);
};

#endif

