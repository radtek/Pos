//---------------------------------------------------------------------------


#pragma hdrstop

#include "ZWaiterAppOrderDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"
#include "GlobalSettings.h"
#include "ReceiptUtility.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


ZWaiterAppOrderDetailsReportSection::ZWaiterAppOrderDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	: BaseReportSection(mmWaiterAppZReport, mmWaiterAppOrderDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}

ZWaiterAppOrderDetailsReportSection::~ZWaiterAppOrderDetailsReportSection()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void ZWaiterAppOrderDetailsReportSection::GetOutput(TPrintout* printOut)
{
    try
    {
        UnicodeString itemName = "";
        //Get Terminal Name For Waiter App Zed
        UnicodeString terminalName = dataCalculationUtilities->GetTerminalName(*_dbTransaction);

        AddTitle(printOut, terminalName);
        PrintHeader(Now().DateTimeString(), *printOut);
        printOut->PrintFormat->AddLine();
        PrintHeader("Total Orders Paid :", *printOut);
        printOut->PrintFormat->AddLine();
        PrintColumnHeader(*printOut);
        AddDoubleLine(*printOut);

        //Get The Object List Of Required Data
        TIBSQL *orderQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
        std::list<TWaiterAppOrderInfo> waiterAppOrderInfoList = dataCalculationUtilities->GetWaiterAppOrderListForPaidOrders(orderQuery, terminalName);

        //Printing Available Order Information
        for(std::list<TWaiterAppOrderInfo>::iterator it = waiterAppOrderInfoList.begin(); it != waiterAppOrderInfoList.end(); ++it)
        {
            TWaiterAppOrderInfo waiterAppOrderInfo;
            waiterAppOrderInfo.itemName = it->itemName;
            waiterAppOrderInfo.sizeName = it->sizeName;
            waiterAppOrderInfo.qty      = it->qty;
            waiterAppOrderInfo.amount   = it->amount;
            PrintItemRow(waiterAppOrderInfo, itemName, *printOut);
        }

        AddDoubleLine(*printOut);
        TDateTime prevZedTime  = dataCalculationUtilities->GetPreviousZedTimeForTerminal(*_dbTransaction,terminalName);
        waiterAppOrderInfoList = dataCalculationUtilities->GetWaiterAppOrderListForProcessedOrders(orderQuery, terminalName, prevZedTime);
        itemName = "";
        printOut->PrintFormat->AddLine();
        PrintHeader("Total Orders Processed :", *printOut);
        printOut->PrintFormat->AddLine();
        PrintColumnHeader(*printOut);
        AddDoubleLine(*printOut);

        //Printing Available Order Information
        for(std::list<TWaiterAppOrderInfo>::iterator it = waiterAppOrderInfoList.begin(); it != waiterAppOrderInfoList.end(); ++it)
        {
            TWaiterAppOrderInfo waiterAppOrderInfo;
            waiterAppOrderInfo.itemName = it->itemName;
            waiterAppOrderInfo.sizeName = it->sizeName;
            waiterAppOrderInfo.qty      = it->qty;
            waiterAppOrderInfo.amount   = it->amount;
            PrintItemRow(waiterAppOrderInfo, itemName, *printOut);
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void ZWaiterAppOrderDetailsReportSection::PrintHeader(UnicodeString textToPrint, TPrintout &printOut)
{
    try
    {
        printOut.PrintFormat->Line->ColCount = 1;
        printOut.PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut.PrintFormat->Line->Columns[0]->Width = printOut.PrintFormat->Width;
        printOut.PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut.PrintFormat->Line->Columns[0]->Text = textToPrint;

    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void ZWaiterAppOrderDetailsReportSection::PrintItemRow(TWaiterAppOrderInfo waiterAppOrderInfo, UnicodeString &itemName, TPrintout &printOut)
{
    try
    {
        printOut.PrintFormat->Line->ColCount = 4;
        if(itemName == "" || !SameStr(itemName, waiterAppOrderInfo.itemName))
        {
            itemName = waiterAppOrderInfo.itemName;
            printOut.PrintFormat->Line->Columns[0]->Text = itemName;
            printOut.PrintFormat->Line->Columns[0]->Width = printOut.PrintFormat->Width * 3/8;
        }
        else if(SameStr(itemName, waiterAppOrderInfo.itemName))
        {
            printOut.PrintFormat->Line->Columns[0]->Width = printOut.PrintFormat->Width * 3/8;
            printOut.PrintFormat->Line->Columns[0]->Text = "";
        }
            printOut.PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut.PrintFormat->Line->Columns[1]->Width = printOut.PrintFormat->Width * 1 / 4;
            printOut.PrintFormat->Line->Columns[1]->Text = waiterAppOrderInfo.sizeName;
            printOut.PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
            printOut.PrintFormat->Line->Columns[2]->Width = printOut.PrintFormat->Width * 1/8;
            printOut.PrintFormat->Line->Columns[2]->Text = dataFormatUtilities->FormatMMReportPoints(waiterAppOrderInfo.qty);
            printOut.PrintFormat->Line->Columns[2]->Alignment = taLeftJustify;
            printOut.PrintFormat->Line->Columns[3]->Width = printOut.PrintFormat->Width - printOut.PrintFormat->Line->Columns[0]->Width - printOut.PrintFormat->Line->Columns[1]->Width - printOut.PrintFormat->Line->Columns[2]->Width;
            printOut.PrintFormat->Line->Columns[3]->Text = dataFormatUtilities->FormatMMReportCurrency(waiterAppOrderInfo.amount);
            printOut.PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
            printOut.PrintFormat->AddLine();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void ZWaiterAppOrderDetailsReportSection::PrintColumnHeader(TPrintout &printOut)
{
    try
    {
        printOut.PrintFormat->Line->ColCount = 4;
        printOut.PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut.PrintFormat->Line->Columns[0]->Width = printOut.PrintFormat->Width * 3/8;
        printOut.PrintFormat->Line->Columns[0]->Text = "Item Name";
        printOut.PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut.PrintFormat->Line->Columns[1]->Width = printOut.PrintFormat->Width * 1 / 4;
        printOut.PrintFormat->Line->Columns[1]->Text = "Size Name";
        printOut.PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
        printOut.PrintFormat->Line->Columns[2]->Width = printOut.PrintFormat->Width * 1/8;
        printOut.PrintFormat->Line->Columns[2]->Text = "Qty";
        printOut.PrintFormat->Line->Columns[2]->Alignment = taLeftJustify;
        printOut.PrintFormat->Line->Columns[3]->Width = printOut.PrintFormat->Width - printOut.PrintFormat->Line->Columns[0]->Width - printOut.PrintFormat->Line->Columns[1]->Width - printOut.PrintFormat->Line->Columns[2]->Width;
        printOut.PrintFormat->Line->Columns[3]->Text = "Total";
        printOut.PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
        printOut.PrintFormat->AddLine();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void ZWaiterAppOrderDetailsReportSection::AddDoubleLine(TPrintout &printOut)
{
    try
    {
        printOut.PrintFormat->Line->ColCount = 1;
        printOut.PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut.PrintFormat->Line->Columns[0]->Width = printOut.PrintFormat->Width;
        printOut.PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut.PrintFormat->Line->Columns[0]->DoubleLine();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
