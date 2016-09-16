//---------------------------------------------------------------------------


#pragma hdrstop

#include "XRemovalDetailsReportSection.h"

//---------------------------------------------------------------------------

XRemovalDetailsReportSection::XRemovalDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmShowRemovalSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XRemovalDetailsReportSection::~XRemovalDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XRemovalDetailsReportSection::GetOutput(TPrintout* printOut)
{
    if(!printOut->BlindBalanceUsed)
        return;
    TStringList *_removalsServerList = new TStringList;
    DataCalculationUtilities dataCalcUtils;

    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    TDateTime prevZedTime = dataCalcUtils.GetPreviousZedTimeForTerminal(
                                                                *_dbTransaction,
                                                                deviceName);

    AnsiString terminalNamePredicate = "";
    if(!_globalSettings->EnableDepositBagNum)
    {
        terminalNamePredicate = " SECURITY.TERMINAL_NAME = '" + deviceName + "' AND ";
    }

    TIBSQL* removalsQuery = _dbTransaction->Query( _dbTransaction->AddQuery());
    removalsQuery->Close();
    removalsQuery->SQL->Text =
                            "SELECT  "
                                    "b.TIME_STAMP, "
                                    "b.ITEM_NAME,  "
                                    "b.PRICE,  "
                                    "d.NAME  "

                                "FROM  "
                                    "SECURITY a  "
                                "INNER JOIN  "
                                    "CANCELITEMS b "
                                "ON "
                                    "a.SECURITY_REF = b.SECURITY_REF  "
                                "INNER JOIN  "
                                " CONTACTS d "
                                "ON "
                                "a.USER_KEY = d.CONTACTS_KEY  "
                                "WHERE  "
                                    "a.SECURITY_EVENT = :SECURITY_EVENT "
                                    "AND b.TIME_STAMP > :PrevZedTime  "

                                    "Order By  "
                                    "1  "   ;

    removalsQuery->ParamByName("PrevZedTime")->AsDateTime = prevZedTime;
    removalsQuery->ParamByName("SECURITY_EVENT")->AsString = UnicodeString(SecurityTypes[secCheckRemoval]);
    removalsQuery->ExecQuery();

    for (; !removalsQuery->Eof; removalsQuery->Next())
    {
        int Index = _removalsServerList->IndexOf(removalsQuery->FieldByName("NAME")->AsString);
        if (Index == -1)
        {
            TStringList *ItemsList = new TStringList;
            Index = _removalsServerList->AddObject(removalsQuery->FieldByName("NAME")->AsString, ItemsList);
            TCurrencyTotal *Item = new TCurrencyTotal;
            Item->Total = removalsQuery->FieldByName("PRICE")->AsCurrency;
            Item->TimeStamp = removalsQuery->FieldByName("TIME_STAMP")->AsString;
            Item->Name = removalsQuery->FieldByName("ITEM_NAME")->AsString;

            ((TStringList*)_removalsServerList->Objects[Index])->AddObject(removalsQuery->FieldByName("ITEM_NAME")->AsString, Item);
        }
        else
        {
            TCurrencyTotal *Item = new TCurrencyTotal;
            Item->Total = removalsQuery->FieldByName("PRICE")->AsCurrency;
            Item->TimeStamp = removalsQuery->FieldByName("TIME_STAMP")->AsString;
             Item->Name = removalsQuery->FieldByName("ITEM_NAME")->AsString;
            ((TStringList*)_removalsServerList->Objects[Index])->AddObject(removalsQuery->FieldByName("ITEM_NAME")->AsString, Item);
        }
    }

    if ( _removalsServerList->Count > 0 )
    {
        AddTitle(printOut, "Removal Report");

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;

        Currency TotalCanceled = 0;
        for (int i = 0; i < _removalsServerList->Count; i++)
        {
            // Add User name...
            printOut->PrintFormat->Line->ColCount = 1;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
            printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut->PrintFormat->Add(_removalsServerList->Strings[i]);

            // ...followed by the items canceled by that user.
            printOut->PrintFormat->Line->ColCount = 3;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 3 / 9;
            printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 4 / 9;
            printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
            printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 7 / 9);
            printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

            for (int j = 0; j < ((TStringList*)_removalsServerList->Objects[i])->Count; j++)
            {
                printOut->PrintFormat->Add(
                            ((TCurrencyTotal*)((TStringList*)_removalsServerList->Objects[i])->Objects[j])->TimeStamp.FormatString("dd/mm/yy hh:nn:ss")
                            + "|" + " " + ((TStringList*)_removalsServerList->Objects[i])->Strings[j] + "|"
                            + dataFormatUtilities->FormatMMReportCurrency( ((TCurrencyTotal*)((TStringList*)_removalsServerList->Objects[i])->Objects[j])->Total ) );
                printOut->PrintFormat->Add(((TCurrencyTotal*)((TStringList*)_removalsServerList->Objects[i])->Objects[j])->Name + "|");
                TotalCanceled += ((TCurrencyTotal*)((TStringList*)_removalsServerList->Objects[i])->Objects[j])->Total;
                delete((TStringList*)_removalsServerList->Objects[i])->Objects[j];
            }
            delete _removalsServerList->Objects[i];
        }

        IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

        if(reportSectionDisplayTraits)
        {
            reportSectionDisplayTraits->ApplyTraits(printOut);
        }

        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->Line->Columns[1]->DoubleLine();
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "Total Removals";
        printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency( TotalCanceled );
        printOut->PrintFormat->AddLine();
    }
    delete _removalsServerList;
}

#pragma package(smart_init)