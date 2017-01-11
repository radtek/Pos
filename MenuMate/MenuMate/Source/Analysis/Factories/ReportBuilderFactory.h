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
    ReportBuilderFactory(TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction, TDateTime* startTime, TDateTime* endTime);

private:
	TGlobalSettings* _globalSettings;
	Database::TDBTransaction* _dbTransaction;
    TDateTime* _startTime;
    TDateTime* _endTime;
};

#endif