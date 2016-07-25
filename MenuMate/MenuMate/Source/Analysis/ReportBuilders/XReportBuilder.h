#ifndef XReportBuilderH
#define XReportBuilderH

#include "ReportEnums.h"
#include "EndOfDayReportBuilder.h"

class XReportBuilder : public EndOfDayReportBuilder
{
public:
	XReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction);
	~XReportBuilder();

protected:
	IReport* PrepareAndCompileSections();
};

//------------------------------------------------------------------------------
#endif
