#ifndef WaiterAppZReportBuilderH
#define WaiterAppZReportBuilderH

#include "ReportEnums.h"
#include "WaiterAppEndOfDayReportBuilder.h"

class WaiterAppZReportBuilder : public WaiterAppEndOfDayReportBuilder
{
public:
	WaiterAppZReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction);
	~WaiterAppZReportBuilder();

protected:
	IReport* PrepareAndCompileSections();
};
#endif
