#include "XCommissionTipsDetailsReportSection.h"

#include "Commission.h"
#include "DeviceRealTerminal.h"
#include "StaffHours.h"

XCommissionTipsDetailsReportSection::XCommissionTipsDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmCommissionTipsDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}


XCommissionTipsDetailsReportSection::~XCommissionTipsDetailsReportSection()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void XCommissionTipsDetailsReportSection::GetOutput(TPrintout* printOut)
{
    AddTitle(printOut, "Commission/Tips");
    printOut->PrintFormat->NewLine();

    IReportSectionDisplayTraits* reportSectionDisplayTrait = GetTextFormatDisplayTrait();
    if(reportSectionDisplayTrait)
    {
        reportSectionDisplayTrait->ApplyTraits(printOut);
    }

    TForm* currentForm = Screen->ActiveForm;
    TCommissionController CommissionController(currentForm,  *_dbTransaction);

    TCommissionCache Commission = CommissionController.Get();

    double TotalCommission = 0;
    TCommissionContainer::iterator itCommission = Commission.begin();
    for (; itCommission != Commission.end(); itCommission++)
    {

        int days = (int)itCommission->second.GetDateTo() - (int)itCommission->second.GetDateFrom();
        if((double)itCommission->second.GetCommission())
        {
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 16/20;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;

            printOut->PrintFormat->Line->Columns[0]->Text = itCommission->second.GetName() + " Total:";
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( itCommission->second.GetCommission() );
            TotalCommission = TotalCommission + itCommission->second.GetCommission();
            printOut->PrintFormat->AddLine();

            AnsiString str1 = itCommission->second.GetDateFrom().FormatString("dd/mm/yy");
            AnsiString str2 = itCommission->second.GetDateTo().FormatString("dd/mm/yy");
            AnsiString str3 = str1 + " - " + str2;


            printOut->PrintFormat->Line->Columns[1]->Width = 0;
            printOut->PrintFormat->Line->Columns[0]->Text = str3;
            printOut->PrintFormat->Line->Columns[1]->Text = "";
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "";
            printOut->PrintFormat->AddLine();
        }
    }
    printOut->PrintFormat->AddLine();
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 16/20;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;
    printOut->PrintFormat->Line->Columns[0]->Text = "Total Commission:";
    printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( TotalCommission );
    printOut->PrintFormat->AddLine();
}

/*void XCommissionTipsDetailsReportSection::GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime)
{
    AddTitle(printOut, "Commission/Tips");
    printOut->PrintFormat->NewLine();

    IReportSectionDisplayTraits* reportSectionDisplayTrait = GetTextFormatDisplayTrait();
    if(reportSectionDisplayTrait)
    {
        reportSectionDisplayTrait->ApplyTraits(printOut);
    }

    TForm* currentForm = Screen->ActiveForm;
    TCommissionController CommissionController(currentForm,  *_dbTransaction);

    TCommissionCache Commission = CommissionController.Get();

    double TotalCommission = 0;
    TCommissionContainer::iterator itCommission = Commission.begin();
    for (; itCommission != Commission.end(); itCommission++)
    {

        int days = (int)itCommission->second.GetDateTo() - (int)itCommission->second.GetDateFrom();
        if((double)itCommission->second.GetCommission())
        {
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 16/20;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;

            printOut->PrintFormat->Line->Columns[0]->Text = itCommission->second.GetName() + " Total:";
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( itCommission->second.GetCommission() );
            TotalCommission = TotalCommission + itCommission->second.GetCommission();
            printOut->PrintFormat->AddLine();

            AnsiString str1 = itCommission->second.GetDateFrom().FormatString("dd/mm/yy");
            AnsiString str2 = itCommission->second.GetDateTo().FormatString("dd/mm/yy");
            AnsiString str3 = str1 + " - " + str2;


            printOut->PrintFormat->Line->Columns[1]->Width = 0;
            printOut->PrintFormat->Line->Columns[0]->Text = str3;
            printOut->PrintFormat->Line->Columns[1]->Text = "";
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->Text = "";
            printOut->PrintFormat->AddLine();
        }
    }
    printOut->PrintFormat->AddLine();
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 16/20;
    printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width;
    printOut->PrintFormat->Line->Columns[0]->Text = "Total Commission:";
    printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( TotalCommission );
    printOut->PrintFormat->AddLine();
}*/
