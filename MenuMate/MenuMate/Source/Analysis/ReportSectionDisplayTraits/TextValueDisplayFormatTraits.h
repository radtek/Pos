#ifndef TextValueDisplayFormatTraitsH
#define TextValueDisplayFormatTraitsH

#include "BaseReportSectionDisplayTraits.h"

class TextValueDisplayFormatTraits : public BaseReportSectionDisplayTraits
{
public:
    TextValueDisplayFormatTraits::TextValueDisplayFormatTraits(ReportSectionTraitType reportSectionTraitType);
	void ApplyTraits(TPrintout* printOut);
};

#endif
