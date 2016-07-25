#ifndef IReportH
#define IReportH

#include <vector>
#include "IReportSection.h"
#include "Printout.h"
#include <Forms.hpp>
#include "Timer.h"
#include <DateUtils.hpp>

struct delete_ptr
{
    template <class T>
    void operator()(T* p)
    {
        delete p;
    }
};

class ReportSections
{
public:
    ReportSections(TPrintout* printOut):_printOut(printOut){ }

    void operator()(IReportSection* reportSection)
    {
        if(reportSection && reportSection->GetIsEnabled())
        {

            reportSection->GetOutput(_printOut);
        }
    }

private:
    TPrintout* _printOut;
};

class IReport
{
public:
	virtual unsigned int AddSection(IReportSection* reportSection) = 0;

	virtual IReportSection* InsertSectionAtIndex(unsigned int index, IReportSection* reportSection) = 0;

	virtual IReportSection* RelocateSection(unsigned int sourceIndex, unsigned int targetIndex) = 0;

	virtual IReportSection* SectionAtIndex(unsigned int index) = 0;

	virtual void DeleteSectionAtIndex(unsigned int index) = 0;
	virtual void DeleteSection(IReportSection* reportSection) = 0;

	virtual void EnableSectionAtIndex(unsigned int index) = 0;
	virtual void EnableSection(IReportSection* reportSection) = 0;

	virtual void DisableSectionAtIndex(unsigned int index) = 0;
	virtual void DisableSection(IReportSection* reportSection) = 0;

	virtual unsigned int SectionCount() = 0;

	virtual std::vector<IReportSection*> Sections() = 0;
	virtual std::vector<IReportSection*> EnabledSections() = 0;
	virtual std::vector<IReportSection*> DisabledSections() = 0;

    virtual AnsiString GetReportName() = 0;

    virtual int DisplayAndPrint(TMemoryStream* memoryStream = NULL) = 0;
};

#endif
