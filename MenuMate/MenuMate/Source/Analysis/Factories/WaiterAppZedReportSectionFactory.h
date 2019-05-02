//---------------------------------------------------------------------------

#ifndef WaiterAppZedReportSectionFactoryH
#define WaiterAppZedReportSectionFactoryH

#include <memory>
#include "ReportEnums.h"
#include "IReportSection.h"
#include "BaseReportSectionFactory.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class WaiterAppZedReportSectionFactory : public BaseReportSectionFactory
{
public:
	WaiterAppZedReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~WaiterAppZedReportSectionFactory();

	IReportSection* CreateReportSection(ReportSectionType reportSectionType);
};
#endif
