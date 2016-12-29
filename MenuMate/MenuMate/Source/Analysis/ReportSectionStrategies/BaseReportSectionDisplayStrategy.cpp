#include "BaseReportSectionDisplayStrategy.h"

BaseReportSectionDisplayStrategy::BaseReportSectionDisplayStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
        : _dbTransaction(dbTransaction), _globalSettings(globalSettings)
{
  IsConsolidatedStartegy = false;
}

BaseReportSectionDisplayStrategy::BaseReportSectionDisplayStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
        : _dbTransaction(dbTransaction), _globalSettings(globalSettings), _startTime(startTime), _endTime(endTime)
{
   IsConsolidatedStartegy = true;
}
