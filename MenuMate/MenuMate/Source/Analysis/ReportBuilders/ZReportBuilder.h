#ifndef ZReportBuilderH
#define ZReportBuilderH

#include "ReportEnums.h"
#include "EndOfDayReportBuilder.h"

class ZReportBuilder : public EndOfDayReportBuilder
{
public:
	ZReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction);
	~ZReportBuilder();

protected:
	IReport* PrepareAndCompileSections();
};

#endif
