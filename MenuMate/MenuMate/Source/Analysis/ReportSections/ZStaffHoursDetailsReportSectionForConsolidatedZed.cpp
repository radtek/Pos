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
    TForm* currentForm = Screen->ActiveForm;

    //TStaffHoursController StaffHoursController(currentForm, *_dbTransaction);
    //StaffHoursController.Run();
    //TStaffHoursInterface StaffHours = StaffHoursController.Get();

    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    const Currency totalEarnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName);

    printOut->PrintFormat->NewLine();

    IReportSectionDisplayTraits* reportSectionDisplayTrait = GetTextFormatDisplayTrait();
    if(reportSectionDisplayTrait)
    {
        reportSectionDisplayTrait->ApplyTraits(printOut);
    }

    double TotalHours = 0, TotalWages = 0;

    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
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
                                  "and ct.Logout_DateTime >= '20.12.2016, 10:03:02.000' "
                                  "and ct.Logout_DateTime < '24.12.2016, 10:03:02.000' "
                                  "and ct.logout_datetime is not null "
                                  "and (C.Contact_Type = 0 "
                                  " or C.Contact_Type = 1) ) "


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
