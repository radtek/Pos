#include "ReportSectionDisplayTraitsProvider.h"
#include "TextValueDisplayFormatTraits.h"
#include "PointsReportHeaderTrait.h"

ReportSectionDisplayTraitsProvider::ReportSectionDisplayTraitsProvider(TGlobalSettings* globalSettings)
{
	_globalSettings = globalSettings;
}


ReportSectionDisplayTraitsProvider::~ReportSectionDisplayTraitsProvider()
{
}

std::vector<IReportSectionDisplayTraits*> ReportSectionDisplayTraitsProvider::CreateSectionTraits(ReportType reportType,
                                                                                                    ReportSectionType reportSectionType)
{
	std::vector<IReportSectionDisplayTraits*> reportSectionDisplayTraits;

    IReportSectionDisplayTraits* textValueDisplayFormatTraits = new TextValueDisplayFormatTraits(mmTextFormatDisplayTrait);
    reportSectionDisplayTraits.push_back(textValueDisplayFormatTraits);

    switch(reportSectionType)
    {
        case mmPointsReportDetailsSection:
            {
                IReportSectionDisplayTraits* pointsReportHeaderTraits = new PointsReportHeaderTrait(mmPointsReportHeaderTrait);
                reportSectionDisplayTraits.push_back(pointsReportHeaderTraits);
            }
            break;
        default:
            break;
    }

	return reportSectionDisplayTraits;
}

