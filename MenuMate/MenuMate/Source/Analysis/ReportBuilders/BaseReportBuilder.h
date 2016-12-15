#ifndef BaseReportBuilderH
#define BaseReportBuilderH

#include "IReportBuilder.h"
#include "BaseReport.h"
#include "IReportSectionFactory.h"
#include "ReportEnums.h"
#include "Printout.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class BaseReportBuilder : public IReportBuilder
{
public:
	BaseReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction);
    BaseReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction, TDateTime* startTime, TDateTime* endTime);

	//We are implementing template here, since we need to invoke report creation here but want individual derived classes to implement the report creation.
	IReport* BuildReport();

protected:
	//This method will be overridden in the dervied classes to prepare the report.
	virtual IReport* PrepareAndCompileSections() = 0;
    virtual int AddReportSectionToReport(IReport* report, ReportSectionType reportSectionType, bool isEnabled);

	//We need to encapsulate the private member variables..
	TGlobalSettings* GetGlobalSettings();
	Database::TDBTransaction* GetDatabaseTransaction();

	ReportType GetCurrentReportType();

	IReportSectionFactory* _reportSectionFactory;

private:
	TPrintout* _printOut;
	ReportType _reportType;

	TGlobalSettings* _globalSettings;
	Database::TDBTransaction*  _dbTransaction;
    bool ValidateReportSection(ReportSectionType reportSectionType);
    TDateTime* _startTime;
    TDateTime* _endTime;
};

#endif

