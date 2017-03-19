//---------------------------------------------------------------------------


#pragma hdrstop

#include "XWriteOffDetailsReportSection.h"

//---------------------------------------------------------------------------

XWriteOffDetailsReportSection::XWriteOffDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmRefundDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XWriteOffDetailsReportSection::XWriteOffDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmRefundDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XWriteOffDetailsReportSection::~XWriteOffDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XWriteOffDetailsReportSection::GetOutput(TPrintout* printOut)
{
    try
    {
        DataCalculationUtilities dataCalcUtils;

        AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
        TDateTime prevZedTime = dataCalcUtils.GetPreviousZedTimeForTerminal(
                                                                    *_dbTransaction,
                                                                    deviceName);

        TIBSQL* writeOffQuery = _dbTransaction->Query( _dbTransaction->AddQuery());
        writeOffQuery->Close();
        if(IsConsolidatedZed)
        {
           GetWriteOffDataforConsolidatedZed(writeOffQuery);
        }
        else
        {
           GetWriteOffDataforNormalZed(writeOffQuery);
           writeOffQuery->ParamByName("TIME_STAMP")->AsDateTime = prevZedTime;
        }
        writeOffQuery->ParamByName("TERMINALNAME")->AsString = deviceName;
        writeOffQuery->ExecQuery();

        if (writeOffQuery->RecordCount)
        {
            Currency TotalWriteOff = 0;

            AddTitle(printOut, "Write Off Audit");
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->ColCount = 3;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width *9/32;
            printOut->PrintFormat->Line->Columns[0]->Text = "Write off#";
            printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 2/4;
            printOut->PrintFormat->Line->Columns[1]->Text = "Item";
            printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width - printOut->PrintFormat->Line->Columns[1]->Width;
            printOut->PrintFormat->Line->Columns[2]->Text = "Amount";
            printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
            printOut->PrintFormat->AddLine();

            for(; !writeOffQuery->Eof; writeOffQuery->Next())
            {
                printOut->PrintFormat->Line->Columns[0]->Text = writeOffQuery->FieldByName("WRITEOFF_KEY")->AsString;
                printOut->PrintFormat->Line->Columns[1]->Text = writeOffQuery->FieldByName("ITEM")->AsString;
                printOut->PrintFormat->Line->Columns[2]->Text = FloatToStrF(writeOffQuery->FieldByName("AMOUNT")->AsFloat, ffNumber,
                18, CurrencyDecimals);

                TotalWriteOff += writeOffQuery->FieldByName("AMOUNT")->AsFloat;
                printOut->PrintFormat->AddLine();

            }

            IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

            if(reportSectionDisplayTraits)
            {
                reportSectionDisplayTraits->ApplyTraits(printOut);
            }

            printOut->PrintFormat->Line->Columns[0]->Text = "";
            printOut->PrintFormat->Line->Columns[1]->DoubleLine();
            printOut->PrintFormat->AddLine();

            printOut->PrintFormat->Line->Columns[0]->Text = "Total WriteOff";
            printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(TotalWriteOff);
            printOut->PrintFormat->AddLine();
        }
    }
     catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void XWriteOffDetailsReportSection::GetWriteOffDataforNormalZed(TIBSQL* writeOffQuery)
{
    try
    {
        writeOffQuery->SQL->Text =
                    "SELECT "
                        "a.WRITEOFF_KEY, "
                        "a.ITEM, "
                        "a.STAFF, "
                        "a.AMOUNT, "
                        "a.TIME_STAMP, "
                        "a.REASONS, "
                        "a.CATEGORY_KEY, "
                        "a.GROUP_KEY, "
                        "a.SUPPLIER_KEY, "
                        "a.MENU_KEY, "
                        "a.COURSE_KEY, "
                        "a.ITEM_KEY, "
                        "a.PARENT_KEY, "
                        "a.SIZE_NAME, "
                        "a.TERMINALNAME "
                    "FROM "
                        "WRITEOFF a "
                    "WHERE "
                        "a.TIME_STAMP > :TIME_STAMP "
                        "AND a.TERMINALNAME = :TERMINALNAME;";
    }
     catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}


void XWriteOffDetailsReportSection::GetWriteOffDataforConsolidatedZed(TIBSQL* writeOffQuery)
{
    try
    {
        writeOffQuery->SQL->Text =
                    "SELECT "
                        "a.WRITEOFF_KEY, "
                        "a.ITEM, "
                        "a.STAFF, "
                        "a.AMOUNT, "
                        "a.TIME_STAMP, "
                        "a.REASONS, "
                        "a.CATEGORY_KEY, "
                        "a.GROUP_KEY, "
                        "a.SUPPLIER_KEY, "
                        "a.MENU_KEY, "
                        "a.COURSE_KEY, "
                        "a.ITEM_KEY, "
                        "a.PARENT_KEY, "
                        "a.SIZE_NAME, "
                        "a.TERMINALNAME "
                    "FROM "
                        "WRITEOFF a "
                    "WHERE "
                        "a.TIME_STAMP >= :startTime and  a.TIME_STAMP <= :endTime "
                        "AND a.TERMINALNAME = :TERMINALNAME;";
        writeOffQuery->ParamByName("startTime")->AsDateTime = *_startTime;
        writeOffQuery->ParamByName("endTime")->AsDateTime = *_endTime;
    }
     catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
#pragma package(smart_init)
