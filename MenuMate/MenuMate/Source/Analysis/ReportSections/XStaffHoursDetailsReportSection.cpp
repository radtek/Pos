#include "XStaffHoursDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "StaffHours.h"

XStaffHoursDetailsReportSection::XStaffHoursDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmStaffHoursDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}


XStaffHoursDetailsReportSection::~XStaffHoursDetailsReportSection()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void XStaffHoursDetailsReportSection::GetOutput(TPrintout* printOut)
{
    AddTitle(printOut, " Staff Hours");
    TForm* currentForm = Screen->ActiveForm;

    TStaffHoursController StaffHoursController(currentForm, *_dbTransaction);
    StaffHoursController.Run();
    TStaffHoursInterface StaffHours = StaffHoursController.Get();

    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    const Currency totalEarnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName);

    printOut->PrintFormat->NewLine();

    IReportSectionDisplayTraits* reportSectionDisplayTrait = GetTextFormatDisplayTrait();
    if(reportSectionDisplayTrait)
    {
        reportSectionDisplayTrait->ApplyTraits(printOut);
    }

    double TotalHours = 0, TotalWages = 0;

    TStaffHoursContainer::iterator itStaffHours = StaffHours.begin();
    for (; itStaffHours != StaffHours.end(); itStaffHours++)
    {
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 16/20;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;

        printOut->PrintFormat->Line->Columns[0]->Text = itStaffHours->second.GetContactName();
        printOut->PrintFormat->Line->Columns[1]->Text = "Unknown";
        if(!itStaffHours->second.GetLogOutNull())
        {
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( itStaffHours->second.GetHoursWorked() );
            TotalHours = RoundTo((TotalHours + itStaffHours->second.GetHoursWorked()), -2);
        }

        printOut->PrintFormat->AddLine();

        AnsiString str1 = itStaffHours->second.GetLoggedIn().FormatString("dd/mm/yy HH:nn");
        AnsiString str2 = itStaffHours->second.GetLoggedOut().FormatString("dd/mm/yy HH:nn");
        if(itStaffHours->second.GetLogOutNull())
        str2 = "???";
        AnsiString str3 = str1 + " - " + str2;

        printOut->PrintFormat->Line->Columns[1]->Width = 0;
        printOut->PrintFormat->Line->Columns[0]->Text = str3;
        printOut->PrintFormat->Line->Columns[1]->Text = "";

        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 16/20;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;

		TotalWages = TotalWages + itStaffHours->second.GetHourlyRate() * itStaffHours->second.GetHoursWorked();
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
