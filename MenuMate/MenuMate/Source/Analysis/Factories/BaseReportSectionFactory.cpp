#include "BaseReportSectionFactory.h"


BaseReportSectionFactory::BaseReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
            : _dbTransaction(dbTransaction), _globalSettings(globalSettings)
{
}

BaseReportSectionFactory::BaseReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
            : _dbTransaction(dbTransaction), _globalSettings(globalSettings), _startTime(startTime), _endTime(endTime)
{
}

BaseReportSectionFactory::~BaseReportSectionFactory()
{
}
