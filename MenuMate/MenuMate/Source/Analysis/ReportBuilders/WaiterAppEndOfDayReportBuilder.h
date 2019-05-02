#ifndef WaiterAppEndOfDayReportBuilderH
#define WaiterAppEndOfDayReportBuilderH

#include "ReportEnums.h"
#include "BaseReportBuilder.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class WaiterAppEndOfDayReportBuilder : public BaseReportBuilder
{
public:
	WaiterAppEndOfDayReportBuilder(ReportType reportType, TGlobalSettings* globalSettings, Database::TDBTransaction*  dbTransaction);
	~WaiterAppEndOfDayReportBuilder();

protected:
	//This function is implemented to help derived concrete classes to reuse the functionality as is if they dont intend to override some of the functionality
	//If they need to they can very well do it in PrepareAndCompileSections() pure virtual function carried from the BaseReportBuilder
	void AddSectionsToReport(IReport* report);

    virtual void AddOrderDetailsSection(IReport* report);

};
#endif
