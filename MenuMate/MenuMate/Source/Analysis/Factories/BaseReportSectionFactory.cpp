#include "BaseReportSectionFactory.h"


BaseReportSectionFactory::BaseReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
            : _dbTransaction(dbTransaction), _globalSettings(globalSettings)
{
}


BaseReportSectionFactory::~BaseReportSectionFactory()
{
}
