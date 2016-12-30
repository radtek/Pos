#ifndef BaseReportSectionFactoryH
#define BaseReportSectionFactoryH

#include "IReportSectionFactory.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class BaseReportSectionFactory : public IReportSectionFactory
{
public:
	BaseReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    BaseReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~BaseReportSectionFactory();

protected:
	Database::TDBTransaction* _dbTransaction;
	TGlobalSettings* _globalSettings;
    TDateTime* _startTime;
    TDateTime* _endTime;
};

#endif

