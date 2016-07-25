#ifndef ReportSectionDisplayTraitsProviderH
#define ReportSectionDisplayTraitsProviderH

#include <vector>
#include "GlobalSettings.h"
#include "IReportSectionDisplayTraits.h"
#include "ReportEnums.h"

class ReportSectionDisplayTraitsProvider
{
public:
	ReportSectionDisplayTraitsProvider(TGlobalSettings* globalSettings);
	~ReportSectionDisplayTraitsProvider();

	std::vector<IReportSectionDisplayTraits*> CreateSectionTraits(ReportType reportType, ReportSectionType reportSectionType);

private:
	TGlobalSettings* _globalSettings;
};

#endif
