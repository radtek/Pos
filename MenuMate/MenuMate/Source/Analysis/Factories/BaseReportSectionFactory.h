#ifndef BaseReportSectionFactoryH
#define BaseReportSectionFactoryH

#include "IReportSectionFactory.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class BaseReportSectionFactory : public IReportSectionFactory
{
public:
	BaseReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~BaseReportSectionFactory();

protected:
	Database::TDBTransaction* _dbTransaction;
	TGlobalSettings* _globalSettings;
};

#endif

