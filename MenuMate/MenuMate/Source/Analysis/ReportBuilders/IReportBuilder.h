#ifndef IReportBuilderH
#define IReportBuilderH

#include "IReport.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"

class IReportBuilder
{
public:
	virtual IReport* BuildReport() = 0;
};

#endif

