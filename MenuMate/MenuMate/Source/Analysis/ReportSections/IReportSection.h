#ifndef IReportSectionH
#define IReportSectionH

#include <vector>
#include "Printout.h"
#include "IReportSectionDisplayStrategy.h"
#include "IReportSectionDisplayTraits.h"

class IReportSection
{
public:
	virtual void GetOutput(TPrintout* printout) = 0;

    //This will be used to identify whether a report section is enabled or not.
    virtual bool GetIsEnabled() = 0;
    virtual void SetIsEnabled(bool isEnabled) = 0;

    //This will be used to get report section strategies..
    virtual IReportSectionDisplayStrategy* GetReportSectionStrategy() = 0;

    //This will be used to display the report section traits..
    virtual std::vector<IReportSectionDisplayTraits*> GetReportSectionTraits() = 0;
    virtual TDateTime* GetStartTime() = 0;
    virtual TDateTime* GetEndTime() = 0;
    virtual void SetStartTime(TDateTime* startTime) = 0;
    virtual void SetEndTime(TDateTime* endTime) = 0;
    virtual void GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime) = 0;
};

#endif