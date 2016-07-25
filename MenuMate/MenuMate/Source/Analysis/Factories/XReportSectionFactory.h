#ifndef XReportSectionFactoryH
#define XReportSectionFactoryH

#include <memory>
#include "ReportEnums.h"
#include "IReportSection.h"
#include "BaseReportSectionFactory.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class XReportSectionFactory : public BaseReportSectionFactory
{
public:
	XReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~XReportSectionFactory();

	IReportSection* CreateReportSection(ReportSectionType reportSectionType);
};

#endif

