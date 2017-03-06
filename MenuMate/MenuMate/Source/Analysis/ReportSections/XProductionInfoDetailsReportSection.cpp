#include "XProductionInfoDetailsReportSection.h"
#include "DBSaleTimes.h"

XProductionInfoDetailsReportSection::XProductionInfoDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmProductionInfoDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}


XProductionInfoDetailsReportSection::XProductionInfoDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmProductionInfoDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XProductionInfoDetailsReportSection::~XProductionInfoDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XProductionInfoDetailsReportSection::GetOutput(TPrintout* printOut)
{
    try
    {
        TDateTime total;
        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->DoubleLine();
        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->NewLine();

        AddTitle(printOut, "Production Info");

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 2;
        printOut->PrintFormat->Line->Columns[0]->Width = (printOut->PrintFormat->Width / 3) * 2;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
        printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

        printOut->PrintFormat->Line->Columns[0]->Text = "Average Sale Time (Minutes)";

        unsigned short hour = 0, minute = 0, second = 0, milliSecond = 0;
        if(IsConsolidatedZed)
        {
            total = TDBSaleTimes::GetSalesAverage(*_dbTransaction, *_startTime, *_endTime);
        }
        else
        {
            total = TDBSaleTimes::GetSalesAverage(*_dbTransaction);
        }
        total.DecodeTime(&hour, &minute, &second, &milliSecond);

        printOut->PrintFormat->Line->Columns[1]->Text = (IntToStr(hour) + ":" + IntToStr(minute) + ":" + IntToStr(second));
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "Average Make Time (Minutes)";

        hour = 0;
        minute = 0;
        second = 0;
        milliSecond = 0;
        if(IsConsolidatedZed)
        {
            total = TDBSaleTimes::GetMakeAverage(*_dbTransaction, *_startTime, *_endTime);
        }
        else
        {
            total = TDBSaleTimes::GetMakeAverage(*_dbTransaction);
        }
        total.DecodeTime(&hour, &minute, &second, &milliSecond);

        printOut->PrintFormat->Line->Columns[1]->Text = (IntToStr(hour) + ":" + IntToStr(minute) + ":" + IntToStr(second));
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->Text = "10 Longest Sale Times";
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 6;

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 20 / 100;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 10 / 100;
        printOut->PrintFormat->Line->Columns[1]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width * 10 / 100;
        printOut->PrintFormat->Line->Columns[2]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width * 20 / 100;
        printOut->PrintFormat->Line->Columns[3]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[4]->Width = printOut->PrintFormat->Width * 20 / 100;
        printOut->PrintFormat->Line->Columns[4]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[5]->Width = printOut->PrintFormat->Width * 20 / 100;
        printOut->PrintFormat->Line->Columns[5]->Alignment = taRightJustify;

        printOut->PrintFormat->Add("Doc#|Opr|Qty|Dur|+Avg|Val");

        UnicodeString Doc, Opr, Qty, Dur, Avg, Val;

        int counter = 0;
        int offset = 0;
        while(counter < 10)
        {
            bool isEOF;
            if(IsConsolidatedZed)
            {
                isEOF =  TDBSaleTimes::GetLongestSaleTransForConsolidatedZed(*_dbTransaction, offset, Doc, Opr, Qty, Dur, Avg, Val, *_startTime, *_endTime);
            }
            else
            {
                isEOF =  TDBSaleTimes::GetLongestSaleTrans(*_dbTransaction, offset, Doc, Opr, Qty, Dur, Avg, Val);
            }
            if (Doc != "")
            {
                printOut->PrintFormat->Add(Doc + "|" + Opr + "|" + Qty + "|" + Dur + "|" + Avg + "|" + Val);
                counter++;
            }
            offset++;
            if(isEOF)
            break;
        }

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->Line->Columns[0]->Text = "10 Longest Make Times";
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 6;

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 20 / 100;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 10 / 100;
        printOut->PrintFormat->Line->Columns[1]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width * 10 / 100;
        printOut->PrintFormat->Line->Columns[2]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[3]->Width = printOut->PrintFormat->Width * 20 / 100;
        printOut->PrintFormat->Line->Columns[3]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[4]->Width = printOut->PrintFormat->Width * 20 / 100;
        printOut->PrintFormat->Line->Columns[4]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[5]->Width = printOut->PrintFormat->Width * 20 / 100;
        printOut->PrintFormat->Line->Columns[5]->Alignment = taRightJustify;

        printOut->PrintFormat->Add("Doc#|Opr|Qty|Dur|+Avg|Val");

        for (int i = 0; i < 10; i++)
        {
            if(IsConsolidatedZed)
            {
               TDBSaleTimes::GetLongestMakeTransForConsolidatedZed(*_dbTransaction, i, Doc, Opr, Qty, Dur, Avg, Val, *_startTime, *_endTime);
            }
            else
            {
               TDBSaleTimes::GetLongestMakeTrans(*_dbTransaction, i, Doc, Opr, Qty, Dur, Avg, Val);
            }
            //TDBSaleTimes::GetLongestMakeTrans(*_dbTransaction, i, Doc, Opr, Qty, Dur, Avg, Val);
            if (Doc != "")
            {
                printOut->PrintFormat->Add(Doc + "|" + Opr + "|" + Qty + "|" + Dur + "|" + Avg + "|" + Val);
            }
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
