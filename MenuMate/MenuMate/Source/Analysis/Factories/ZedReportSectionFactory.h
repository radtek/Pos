#ifndef ZedReportSectionFactoryH
#define ZedReportSectionFactoryH

#include <memory>
#include "ReportEnums.h"
#include "IReportSection.h"
#include "BaseReportSectionFactory.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class ZedReportSectionFactory : public BaseReportSectionFactory
{
public:
	ZedReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~ZedReportSectionFactory();

	IReportSection* CreateReportSection(ReportSectionType reportSectionType);
};

#endif

