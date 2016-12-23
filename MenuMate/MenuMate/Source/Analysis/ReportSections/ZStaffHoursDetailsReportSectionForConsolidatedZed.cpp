#include "ZStaffHoursDetailsReportSectionForConsolidatedZed.h"
#include "DeviceRealTerminal.h"
#include "StaffHours.h"

ZStaffHoursDetailsReportSectionForConsolidatedZed::ZStaffHoursDetailsReportSectionForConsolidatedZed(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmStaffHoursDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
    IsConsolidatedZed = true;
}


ZStaffHoursDetailsReportSectionForConsolidatedZed::~ZStaffHoursDetailsReportSectionForConsolidatedZed()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void ZStaffHoursDetailsReportSectionForConsolidatedZed::GetOutput(TPrintout* printOut)
{
    AddTitle(printOut, " Staff Hours");

    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    const Currency totalEarnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName, *_startTime, *_endTime);

    printOut->PrintFormat->NewLine();

    IReportSectionDisplayTraits* reportSectionDisplayTrait = GetTextFormatDisplayTrait();
    if(reportSectionDisplayTrait)
    {
        reportSectionDisplayTrait->ApplyTraits(printOut);
    }

    double TotalHours = 0, TotalWages = 0;

    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 16/20;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;

    TIBSQL *qrstaffHours = _dbTransaction->Query(_dbTransaction->AddQuery());
    qrstaffHours->Close();

    qrstaffHours->Close();
    qrstaffHours->SQL->Text =
                "select "
                "Contact_Type, "
                "Name, "
                "Payroll_ID, "
                "cast(Login_DateTime as timestamp) Login_DateTime, "
                "cast(Logout_DateTime as timestamp) Logout_DateTime, "
                "Breaks, "
                "case when (TOTALHOURS is null ) then (tt - bd) * 24 else TOTALHOURS End as TOTALHOURS, "
                "cast((tt - bd) as float) Days_Worked, "
                "Department, "
                "Zone, "
                "Modified, "
                "HOURLY_RATE "
                "from (select (ct.logout_datetime - ct.login_datetime) tt, "
                            "(cast(ct.breaks as timestamp) "
                            " - cast((current_date || ', 00:00:00.000') as timestamp)) bd, "
                            "C.Contact_Type, "
                            "C.Name, "
                            "C.Payroll_ID, "
                            "cast(ct.Login_DateTime as timestamp) Login_DateTime, "
                            "cast(ct.Logout_DateTime as timestamp) Logout_DateTime, "
                            "ct.Breaks, "
                            "ct.TOTALHOURS, "
                            "TCL.Name Department, "
                            "TCL.Code Zone, "
                            "ct.Modified, "
                            "c.HOURLY_RATE "
                            "from contacttime ct "
                                "Inner Join Contacts C On "
                                    "C.Contacts_Key = ct.Contacts_Key "
                                "Left Join TimeClockLocations TCL On "
                                    "ct.TimeClockLocations_Key = TCL.TimeClockLocations_Key "
                            "where ct.breaks is not null "
                                  "and ct.Logout_DateTime >= :startTime "
                                  "and ct.Logout_DateTime <= :endTime "
                                  "and ct.logout_datetime is not null "
                                  "and (C.Contact_Type = 0 "
                                  " or C.Contact_Type = 1) ) ";


    qrstaffHours->ParamByName("startTime")->AsDateTime = *_startTime;
    qrstaffHours->ParamByName("endTime")->AsDateTime = *_endTime;
    qrstaffHours->ExecQuery();

    for(; !qrstaffHours->Eof; qrstaffHours->Next())
    {
        printOut->PrintFormat->Line->Columns[0]->Text = qrstaffHours->FieldByName("Name")->AsString; //itStaffHours->second.GetContactName();

        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( qrstaffHours->FieldByName("TOTALHOURS")->AsCurrency );
        TotalHours = RoundTo((TotalHours + qrstaffHours->FieldByName("TOTALHOURS")->AsCurrency), -2);

        printOut->PrintFormat->AddLine();

        TDateTime t1 = qrstaffHours->FieldByName("Login_DateTime")->AsDateTime; //itStaffHours->second.GetLoggedIn().FormatString("dd/mm/yy HH:nn");
        TDateTime t2 = qrstaffHours->FieldByName("Logout_DateTime")->AsDateTime;///.FormatString("dd/mm/yy HH:nn");
        AnsiString str1 = t1.FormatString("dd/mm/yy HH:nn");
        AnsiString str2 = t2.FormatString("dd/mm/yy HH:nn");
        ///if(itStaffHours->second.GetLogOutNull())
        //str2 = "???";
        AnsiString str3 = str1 + " - " + str2;

        printOut->PrintFormat->Line->Columns[1]->Width = 0;
        printOut->PrintFormat->Line->Columns[0]->Text = str3;
        printOut->PrintFormat->Line->Columns[1]->Text = "";

        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 16/20;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;
		TotalWages = TotalWages + qrstaffHours->FieldByName("HOURLY_RATE")->AsCurrency * qrstaffHours->FieldByName("TOTALHOURS")->AsCurrency;
    }


    printOut->PrintFormat->Line->Columns[0]->Text = "";
    printOut->PrintFormat->Line->Columns[1]->Text = "";
    printOut->PrintFormat->AddLine();
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 16/20;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;
    printOut->PrintFormat->Line->Columns[0]->Text = "Total Hours";
    printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", TotalHours);
    printOut->PrintFormat->AddLine();
    printOut->PrintFormat->Line->Columns[0]->Text = "Total Wages";
    printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( TotalWages );
    printOut->PrintFormat->AddLine();
    if((double)totalEarnings)
    {
        printOut->PrintFormat->Line->Columns[0]->Text = "Wage Cost % ";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( 100 * TotalWages / totalEarnings );
        printOut->PrintFormat->AddLine();
    }
}
