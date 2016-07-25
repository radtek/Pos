#ifndef ReportBuilderFactoryH
#define ReportBuilderFactoryH

#include <memory>
#include "IReportBuilder.h"
#include "ReportEnums.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class ReportBuilderFactory
{
public:
	ReportBuilderFactory(TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction);
	~ReportBuilderFactory();

	IReportBuilder* CreateReportBuilder(ReportType reportType);

private:
	TGlobalSettings* _globalSettings;
	Database::TDBTransaction* _dbTransaction;
};

#endif