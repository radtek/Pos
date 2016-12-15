#ifndef ReportManagerH
#define ReportManagerH

#include <memory>
#include "ZedReport.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "XReport.h"
#include "IReportBuilder.h"
#include "ReportBuilderFactory.h"
#include "ReportEnums.h"
#include "ConsolidatedZedReport.h"

class ReportManager
{
public:
    ReportManager();
    ~ReportManager();

    //We can not expose an interface with a call hierarchy, as in to first call settings and then pass transaction and finally call the build report.
    //The caller has to invoke a single method pass all required data and sit back to recieve the processed results.
    ZedReport* GetZedReport(TGlobalSettings* settings, Database::TDBTransaction*  dbTransaction);
    XReport* GetXReport(TGlobalSettings* settings, Database::TDBTransaction*  dbTransaction);
    ConsolidatedZedReport* GetConsolidatedZedReport(TGlobalSettings* settings, Database::TDBTransaction*  dbTransaction, TDateTime* StartTime, TDateTime* EndTime);

private:
    void CreateReportBuilderInstance(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction);
    void CreateReportBuilderInstanceForConsolidatedZed(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction, TDateTime* startTime,  TDateTime* endTime);

    std::auto_ptr<IReportBuilder> _reportBuilder;
    std::auto_ptr<ReportBuilderFactory> _reportBuilderFactory;
};

#endif