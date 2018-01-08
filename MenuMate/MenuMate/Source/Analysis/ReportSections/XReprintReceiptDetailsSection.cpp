#include "XReprintReceiptDetailsSection.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ReportUtilities.h"
#include "ManagerReports.h"
#include "MMMessageBox.h"

XReprintReceiptDetailsSection::XReprintReceiptDetailsSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmReprintReceiptDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}


XReprintReceiptDetailsSection::XReprintReceiptDetailsSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmReprintReceiptDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XReprintReceiptDetailsSection::~XReprintReceiptDetailsSection()
{
    delete dataFormatUtilities;
}
//------------------------------------------------------------------------------
void XReprintReceiptDetailsSection::GetOutput(TPrintout* printout)
{
    try
    {
        AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
        TIBSQL *qrInvoice = _dbTransaction->Query(_dbTransaction->AddQuery());
        AnsiString terminalNamePredicate = "";
        if(!IsConsolidatedZed)
            ReprintReceiptQueryForNormalZed(qrInvoice);
        else
            ReprintReceiptForConsolidatedZed(qrInvoice, _startTime,_endTime);
        AddTitle(printout, "Reprint Report");
        if(qrInvoice->RecordCount > 0)
        {
            printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
            printout->PrintFormat->Line->ColCount = 1;
            printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width;
            printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
            if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
            {
               printout->PrintFormat->Line->Columns[0]->DoubleLine();
               printout->PrintFormat->NewLine();
            }
            else
            {
               printout->PrintFormat->Line->Columns[0]->Text = "";
            }
            printout->PrintFormat->AddLine();
        }
        printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        double totalDiscount = 0;
        if(!IsConsolidatedZed)
        {
            printout->PrintFormat->Line->ColCount = 2;
            printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
            printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width / 3;
            printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width -
                                                             printout->PrintFormat->Line->Columns[1]->Width;
            printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
            printout->PrintFormat->Line->Columns[0]->Text = "Invoice Number";
            printout->PrintFormat->Line->Columns[1]->Text = "Total";
            printout->PrintFormat->AddLine();
            for(;!qrInvoice->Eof; qrInvoice->Next())
            {
                IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();
                if(reportSectionDisplayTraits)
                {
                    reportSectionDisplayTraits->ApplyTraits(printout);
                }
                printout->PrintFormat->Line->ColCount = 2;
                printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width / 3;
                printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
                printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width -
                                                                 printout->PrintFormat->Line->Columns[1]->Width;
                printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
                TDateTime dt = qrInvoice->FieldByName("TIME_STAMP")->AsDateTime;
                AnsiString column = dt.FormatString("hh:nn");
                column += " #";
                column += qrInvoice->FieldByName("Note")->AsString;
                printout->PrintFormat->Line->Columns[0]->Text = column;
                double amount = qrInvoice->FieldByName("TOTAL")->AsFloat;
                amount = RoundTo(amount,-2);
                totalDiscount += amount;
                AnsiString value = amount;
                printout->PrintFormat->Line->Columns[1]->Text = value;
                printout->PrintFormat->AddLine();
            }
        }
        else
        {
            printout->PrintFormat->Line->ColCount = 3;
            printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
            printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width / 3;
            printout->PrintFormat->Line->Columns[1]->Alignment = taCenter;
            printout->PrintFormat->Line->Columns[2]->Width = printout->PrintFormat->Width / 3;
            printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
            printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width -
                                                             printout->PrintFormat->Line->Columns[1]->Width -
                                                             printout->PrintFormat->Line->Columns[2]->Width;
            printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printout->PrintFormat->Line->Columns[0]->Text = "Invoice Number";
            printout->PrintFormat->Line->Columns[1]->Text = "Count";
            printout->PrintFormat->Line->Columns[2]->Text = "Total";
            printout->PrintFormat->AddLine();
            for(;!qrInvoice->Eof; qrInvoice->Next())
            {
                IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();
                if(reportSectionDisplayTraits)
                {
                    reportSectionDisplayTraits->ApplyTraits(printout);
                }
                printout->PrintFormat->Line->ColCount = 3;
                printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width / 3;
                printout->PrintFormat->Line->Columns[2]->Width = printout->PrintFormat->Width / 3;
                printout->PrintFormat->Line->Columns[1]->Alignment = taCenter;
                printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
                printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width -
                                                                 printout->PrintFormat->Line->Columns[1]->Width -
                                                                 printout->PrintFormat->Line->Columns[2]->Width;
                printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
                AnsiString column = "#";
                column += qrInvoice->FieldByName("Note")->AsString;
                printout->PrintFormat->Line->Columns[0]->Text = column;
                AnsiString count = "x";
                count += qrInvoice->FieldByName("INSTANCES")->AsInteger;
                printout->PrintFormat->Line->Columns[1]->Text = count;
                double amount = qrInvoice->FieldByName("TOTAL")->AsFloat;
                amount = RoundTo(amount,-2);
                totalDiscount += amount;
                AnsiString value = amount;
                printout->PrintFormat->Line->Columns[2]->Text = value;
                printout->PrintFormat->AddLine();
            }
        }
        printout->PrintFormat->Line->ColCount = 2;
        printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width / 3;
        printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
        printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width -
                                                         printout->PrintFormat->Line->Columns[1]->Width;
        printout->PrintFormat->Line->Columns[0]->Text = "";
        printout->PrintFormat->Line->Columns[1]->DoubleLine();
        printout->PrintFormat->AddLine();
        printout->PrintFormat->Line->ColCount = 2;
        printout->PrintFormat->Line->Columns[1]->Width = printout->PrintFormat->Width / 3;
        printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
        printout->PrintFormat->Line->Columns[0]->Width = printout->PrintFormat->Width -
                                                         printout->PrintFormat->Line->Columns[1]->Width;
        printout->PrintFormat->Line->Columns[0]->Text = "Total";
        AnsiString total = totalDiscount;
        printout->PrintFormat->Line->Columns[1]->Text = total;
        printout->PrintFormat->AddLine();
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
        throw;
    }
}
//------------------------------------------------------------------------------
void XReprintReceiptDetailsSection::SetSingleColumnPrinterFormat(TPrintout* printOut)
{
    printOut->PrintFormat->Line->ColCount = 1;
    printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
    printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
}
//------------------------------------------------------------------------------
void XReprintReceiptDetailsSection::ReprintReceiptQueryForNormalZed(TIBSQL* qrInvoice)
{
    try
    {
        qrInvoice->Close();

        UnicodeString query =
        "SELECT a.Note, a.SECURITY_EVENT, a.TIME_STAMP, d.TOTAL FROM SECURITY a "
        "INNER JOIN DAYARCBILL d "
        "on a.NOTE = d.INVOICE_NUMBER "
        "WHERE a.TIME_STAMP > "
        "(SELECT FIRST 1 b.TIME_STAMP FROM SECURITY b "
        "WHERE b.SECURITY_EVENT = 'Till Z Off' "
        "ORDER BY b.TIME_STAMP desc) "
        "and a.SECURITY_EVENT = 'Reprint Receipt' ";

        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            query += " AND a.TERMINAL_NAME = :TERMINAL_NAME  ";
            qrInvoice->SQL->Text = query;
            qrInvoice->ParamByName("TERMINAL_NAME")->AsString =
                                     TDeviceRealTerminal::Instance().ID.Name;
        }
        else
           qrInvoice->SQL->Text = query;

        qrInvoice->ExecQuery();
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
        throw;
    }
}
//------------------------------------------------------------------------------
void XReprintReceiptDetailsSection::ReprintReceiptForConsolidatedZed( TIBSQL* qrInvoice,
                                    TDateTime* startTime, TDateTime* endTime)
{
    try
    {
        qrInvoice->Close();
        UnicodeString query =
        "SELECT a.Note, a.SECURITY_EVENT,d.TOTAL, COUNT(a.SECURITY_KEY) INSTANCES FROM SECURITY a "
        "INNER JOIN ARCBILL d "
        "on a.NOTE = d.INVOICE_NUMBER "
        "WHERE "
        "a.SECURITY_EVENT = 'Reprint Receipt' "
        "AND a.TIME_STAMP >= :START_TIME AND a.TIME_STAMP <= :END_TIME ";

        if(!TGlobalSettings::Instance().EnableDepositBagNum)
        {
            query += " AND a.TERMINAL_NAME = :TERMINAL_NAME  ";
            query += "group by 1,2,3 ";
            qrInvoice->SQL->Text = query;
            qrInvoice->ParamByName("TERMINAL_NAME")->AsString =
                                     TDeviceRealTerminal::Instance().ID.Name;
        }
        else
        {
           query += "group by 1,2,3 ";
           qrInvoice->SQL->Text = query;
        }
        qrInvoice->ParamByName("START_TIME")->AsDateTime = *startTime;
        qrInvoice->ParamByName("END_TIME")->AsDateTime = *endTime;
        MessageBox(query,"query",MB_OK);
        qrInvoice->ExecQuery();
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
        throw;
    }
}
//------------------------------------------------------------------------------
