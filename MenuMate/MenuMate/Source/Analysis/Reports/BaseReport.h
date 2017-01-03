#ifndef BaseReportH
#define BaseReportH

#include <vector>

#include "IReport.h"
#include "IReportSection.h"

class BaseReport : public IReport
{
private:
    bool isEnabled;

public:
	BaseReport();
	~BaseReport();

	virtual unsigned int AddSection(IReportSection* reportSection);
	virtual IReportSection* InsertSectionAtIndex(unsigned int index, IReportSection* reportSection);

	virtual IReportSection* RelocateSection(unsigned int sourceIndex, unsigned int targetIndex);

	virtual IReportSection* SectionAtIndex(unsigned int index);

	virtual void DeleteSectionAtIndex(unsigned int index);
	virtual void DeleteSection(IReportSection* reportSection);

	virtual void EnableSectionAtIndex(unsigned int index);
	virtual void EnableSection(IReportSection* reportSection);

	virtual void DisableSectionAtIndex(unsigned int index);
	virtual void DisableSection(IReportSection* reportSection);

	virtual unsigned int SectionCount();

	virtual std::vector<IReportSection*> Sections();
	virtual std::vector<IReportSection*> EnabledSections();
	virtual std::vector<IReportSection*> DisabledSections();

protected:
	std::vector<IReportSection*> _sections;
    TPrintout* SetupPrintOutInstance();
    TPrintout* SetupPrintOutInstanceForConsolidatedZed();
};

#endif

