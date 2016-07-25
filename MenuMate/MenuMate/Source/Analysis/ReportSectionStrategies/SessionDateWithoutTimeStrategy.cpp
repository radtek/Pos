#include "SessionDateWithoutTimeStrategy.h"

SessionDateWithoutTimeStrategy::SessionDateWithoutTimeStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSectionDisplayStrategy(dbTransaction, globalSettings)
{
}

void SessionDateWithoutTimeStrategy::BuildSection(TPrintout* printOut)
{
    UnicodeString currentDate = Date();

    SYSTEMTIME SystemTime;
    ::GetLocalTime(&SystemTime);
    int nPreviousMth;

    if(TGlobalSettings::Instance().EndOfDay > 0 && SystemTime.wHour < TGlobalSettings::Instance().EndOfDay)
    {
        if((SystemTime.wDay - 1) == 0)
        {
           nPreviousMth = (SystemTime.wMonth -1==0)?12:SystemTime.wMonth-1;
           int day = CalculateLastDayOfMonth(nPreviousMth);
           TDateTime sessionDate(SystemTime.wYear, nPreviousMth, day);
           currentDate = sessionDate;
        }
        else
        {
            TDateTime sessionDate(SystemTime.wYear, SystemTime.wMonth, (SystemTime.wDay - 1));
            currentDate = sessionDate;
        }
    }
    else
    {
        currentDate =  Date();
    }

    TDateTime sessionDate = currentDate;
    printOut->PrintFormat->Line->Columns[0]->Text = "Session Date: " + sessionDate.FormatString("d mmm yyyy");
    printOut->PrintFormat->AddLine();
}

int SessionDateWithoutTimeStrategy::CalculateLastDayOfMonth(int month)
{
    SYSTEMTIME SystemTime;

    int lastdayofmonth;
    int leap_year;

    switch(month)
    {
    case 1:
        lastdayofmonth = 31;
		break;
	case 2:
        leap_year = SystemTime.wYear % 4;
        if(leap_year == 0)
        {
           lastdayofmonth = 29;
        }
        else
        {
           lastdayofmonth = 28;
        }
		break;
	case 3:
        lastdayofmonth = 31;
		break;
	case 4:
        lastdayofmonth = 30;
		break;
	case 5:
        lastdayofmonth = 31;
		break;
	case 6:
        lastdayofmonth = 30;
		break;
	case 7:
        lastdayofmonth = 31;
		break;
	case 8:
        lastdayofmonth = 31;
		break;
	case 9:
        lastdayofmonth = 30;
		break;
	case 10:
        lastdayofmonth = 31;
		break;
	case 11:
        lastdayofmonth = 30;
		break;
	case 12:
        lastdayofmonth = 31;
		break;
    }
    return lastdayofmonth;
}
