#include "BaseReportSectionDisplayStrategy.h"

BaseReportSectionDisplayStrategy::BaseReportSectionDisplayStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
        : _dbTransaction(dbTransaction), _globalSettings(globalSettings)
{
}
