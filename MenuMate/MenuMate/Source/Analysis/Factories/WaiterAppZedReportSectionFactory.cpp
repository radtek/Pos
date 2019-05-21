//---------------------------------------------------------------------------


#pragma hdrstop

#include "WaiterAppZedReportSectionFactory.h"
#include "ZWaiterAppOrderDetailsReportSection.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

WaiterAppZedReportSectionFactory::WaiterAppZedReportSectionFactory(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSectionFactory(dbTransaction, globalSettings)
{
}

WaiterAppZedReportSectionFactory::~WaiterAppZedReportSectionFactory()
{
}


//TODO: This file includes some sections from XReport, which needs to be refactored out to seperate Z specific classes.
//Will revisit this once we are into Analysis refactoring.
IReportSection* WaiterAppZedReportSectionFactory::CreateReportSection(ReportSectionType reportSectionType)
{
   IReportSection* reportSection;
   try
   {
        if(reportSectionType == mmWaiterAppOrderDetailsSection)
        {
            reportSection = new ZWaiterAppOrderDetailsReportSection(_dbTransaction, _globalSettings);
        }
        else
        {
            reportSection = NULL;
        }
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exscpetion in Bhasad",MB_OK);
    }
	return reportSection;
}
