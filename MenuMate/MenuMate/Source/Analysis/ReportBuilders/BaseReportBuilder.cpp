#include "BaseReportBuilder.h"
#include "ReportSectionFactoryProvider.h"
#include "PrintFormat.h"
#include "Comms.h"

BaseReportBuilder::BaseReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction)
    : _reportType(reportType), _globalSettings(globalSettings), _dbTransaction(dbTransaction)
{
	ReportSectionFactoryProvider* reportSectionFactoryProvider = new ReportSectionFactoryProvider;

	//Get the report section factory and assign it to the member variable for use in building report sections..
	_reportSectionFactory = reportSectionFactoryProvider->CreateReportSectionFactory(reportType, dbTransaction, globalSettings);
}

IReport* BaseReportBuilder::BuildReport()
{
	//Ask the concrete classes to prepare the report..
	IReport* report = PrepareAndCompileSections();

	return report;
}

ReportType BaseReportBuilder::GetCurrentReportType()
{
	return _reportType;
}

TGlobalSettings* BaseReportBuilder::GetGlobalSettings()
{
	return _globalSettings;
}

Database::TDBTransaction* BaseReportBuilder::GetDatabaseTransaction()
{
	return _dbTransaction;
}

int BaseReportBuilder::AddReportSectionToReport(IReport* report, ReportSectionType reportSectionType, bool isEnabled)
{
    //Use the Factory to get the actual report section..
    if(ValidateReportSection(reportSectionType))
    {
       IReportSection* section = _reportSectionFactory->CreateReportSection(reportSectionType);

        if (section)
        {
            section->SetIsEnabled(isEnabled);
            int currentPosition = report->AddSection(section);
            return currentPosition;
        }
    }
    return -1;
}
bool BaseReportBuilder::ValidateReportSection(ReportSectionType reportSectionType)
{
    bool retValue = true;
    if(_reportType == mmXReport)
    {
        switch(reportSectionType)
        {
           case mmBlindBalancesDetailsSection:
              retValue = false;
              break;
           case mmMasterBlindBalancesDetailsSection:
              retValue = false;
              break;
           case mmTransactionSummaryGroupDetailsSection:
              if(TGlobalSettings::Instance().EnableBlindBalances)
                retValue = false;
              break;
           default:
              retValue = true;
              break;
        }
    }
    return retValue;
}



