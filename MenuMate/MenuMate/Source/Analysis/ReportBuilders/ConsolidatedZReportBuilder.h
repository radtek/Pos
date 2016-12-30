#ifndef ConsolidatedZReportBuilderH
#define ConsolidatedZReportBuilderH

#include "ReportEnums.h"
#include "ConsolidatedEndOfDayReportBuilder.h"

class ConsolidatedZReportBuilder : public ConsolidatedEndOfDayReportBuilder
{
public:
	ConsolidatedZReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction, TDateTime* startTime, TDateTime* endTime);
	~ConsolidatedZReportBuilder();

protected:
	IReport* PrepareAndCompileSections();
};

#endif
