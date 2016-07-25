#ifndef BaseReportSectionH
#define BaseReportSectionH

#include <vector>
#include "IReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "IReportSectionDisplayStrategy.h"
#include "ReportEnums.h"

class BaseReportSection : public IReportSection
{
public:
	BaseReportSection(ReportType reportType, ReportSectionType reportSectionType, Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);

    IReportSectionDisplayStrategy*  GetReportSectionStrategy();
    std::vector<IReportSectionDisplayTraits*> GetReportSectionTraits();

    virtual bool GetIsEnabled();
    virtual void SetIsEnabled(bool isEnabled);

protected:
    Database::TDBTransaction* _dbTransaction;
    TGlobalSettings* _globalSettings;

    void AddTitle(TPrintout* printOut, AnsiString title);
    void AddSubTitle(TPrintout *printOut, UnicodeString title);
    IReportSectionDisplayTraits* GetTextFormatDisplayTrait();

    virtual void SetReportSectionStrategy(IReportSectionDisplayStrategy* reportSectionDisplayStrategy);
    virtual void SetReportSectionTraits(std::vector<IReportSectionDisplayTraits*> reportSectionDisplayTraits);

private:
    bool _isEnabled;

	ReportType _reportType;
	ReportSectionType _reportSectionType;

    IReportSectionDisplayStrategy* _reportSectionDisplayStrategy;
    std::vector<IReportSectionDisplayTraits*> _reportSectionDisplayTraits;
};

#endif
