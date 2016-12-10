#include "ReportSectionDisplayStrategyProvider.h"
#include "ClientDetailsFromFileStrategy.h"
#include "FullDateWithTimeStrategy.h"
#include "SessionDateWithoutTimeStrategy.h"
#include "BlindBalanceCalculationStrategy.h"
#include "IReportSectionDisplayTraits.h"
#include "PointsReportHeaderTrait.h"
#include "SeperatePointsStrategy.h"
#include "UnifiedPointsStrategy.h"
#include "CashDenominationCalculationStrategy.h"

ReportSectionDisplayStrategyProvider::ReportSectionDisplayStrategyProvider(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
{
	_globalSettings = globalSettings;
    _dbTransaction = dbTransaction;
}


ReportSectionDisplayStrategyProvider::~ReportSectionDisplayStrategyProvider()
{
}

IReportSectionDisplayStrategy* ReportSectionDisplayStrategyProvider::CreateSectionStrategy(ReportType reportType,
                                                                                                ReportSectionType reportSectionType)
{
	IReportSectionDisplayStrategy* reportSectionDisplayStrategy;

    switch(reportType)
    {
        case mmXReport:
        {
            switch(reportSectionType)
            {
                case mmClientDetailsSection:
                    reportSectionDisplayStrategy = new ClientDetailsFromFileStrategy(_dbTransaction, _globalSettings);
                    break;
                case mmCurrentDateDetailsSection:
                    reportSectionDisplayStrategy = new FullDateWithTimeStrategy(_dbTransaction, _globalSettings);
                    break;
                case mmSessionDateDetailsSection:
                    reportSectionDisplayStrategy = new SessionDateWithoutTimeStrategy(_dbTransaction, _globalSettings);
                    break;
                case mmBlindBalancesDetailsSection:
                    reportSectionDisplayStrategy = new BlindBalanceCalculationStrategy(_dbTransaction, _globalSettings, false);
                    break;
                case mmMasterBlindBalancesDetailsSection:
                    reportSectionDisplayStrategy = new BlindBalanceCalculationStrategy(_dbTransaction, _globalSettings, true);
                    break;
                case mmPointsReportDetailsSection:
                    {
                        IReportSectionDisplayTraits* pointsReportHeaderTraits = new PointsReportHeaderTrait(mmPointsReportHeaderTrait);
                        if (_globalSettings->EnableSeperateEarntPts)
                        {
                            reportSectionDisplayStrategy = new SeperatePointsStrategy(_dbTransaction, _globalSettings, pointsReportHeaderTraits);
                        }
                        else
                        {
                            reportSectionDisplayStrategy = new UnifiedPointsStrategy(_dbTransaction, _globalSettings, pointsReportHeaderTraits);
                        }
                    }
                    break;
               case mmCashDenominationDetailsSection:
                    reportSectionDisplayStrategy = new CashDenominationCalculationStrategy(_dbTransaction, _globalSettings, false);
                    break;
               case mmMasterCashDenominationDetailsSection:
                    reportSectionDisplayStrategy = new CashDenominationCalculationStrategy(_dbTransaction, _globalSettings, true);
                    break;
                default:
                    reportSectionDisplayStrategy = NULL;
                    break;
            }
        }
        break;
        case mmZReport:
        {
            switch(reportSectionType)
            {
               case mmBlindBalancesDetailsSection:
                    reportSectionDisplayStrategy = new BlindBalanceCalculationStrategy(_dbTransaction, _globalSettings, false);
                    break;
               case mmCashDenominationDetailsSection:
                    reportSectionDisplayStrategy = new CashDenominationCalculationStrategy(_dbTransaction, _globalSettings, false);
                    break;
                default:
                    reportSectionDisplayStrategy = NULL;
                    break;
            }
         }
         break;
        default:
            reportSectionDisplayStrategy = NULL;
            break;
    }

	return reportSectionDisplayStrategy;
}
