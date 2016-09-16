#include "XRefundDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ReportUtilities.h"

XRefundDetailsReportSection::XRefundDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmRefundDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XRefundDetailsReportSection::~XRefundDetailsReportSection()
{
    delete dataFormatUtilities;
}

void XRefundDetailsReportSection::GetOutput(TPrintout* printOut)
{
    if(!printOut->BlindBalanceUsed)
        return;
    TStringList *_creditServerList = new TStringList;
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;

    AnsiString terminalNamePredicate = "";
    if(!_globalSettings->EnableDepositBagNum)
    {
        terminalNamePredicate = " DAYARCHIVE.TERMINAL_NAME = '" + deviceName + "' AND ";
    }

    TIBSQL *creditQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
    creditQuery->Close();
    creditQuery->SQL->Text = "SELECT "
                                    "ITEM_NAME, "
                                    "PRICE, "
                                    "QTY, "
                                    "REDEEMED, "
                                    "ORDER_TYPE, "
                                    "CONTACTS.NAME, "
                                    "SECURITY.NOTE, "
                                    "SECURITY.TIME_STAMP "
                                "FROM DAYARCHIVE "
                                "LEFT JOIN SECURITY ON DAYARCHIVE.SECURITY_REF = SECURITY.SECURITY_REF "
                                "LEFT JOIN CONTACTS ON SECURITY.USER_KEY = CONTACTS.CONTACTS_KEY "
                                " WHERE " + terminalNamePredicate + " ORDER_TYPE = " + IntToStr(CreditNonExistingOrder) + " " "AND "
                                "SECURITY.SECURITY_EVENT = '" + SecurityTypes[secCredit] + "' "
                                 "OR "  "SECURITY.SECURITY_EVENT = '" + SecurityTypes[secWriteOff] + "' "  "ORDER BY " "CONTACTS.NAME";
    creditQuery->ExecQuery();

    for (; !creditQuery->Eof; creditQuery->Next())
    {
        int index = _creditServerList->IndexOf(creditQuery->FieldByName("NAME")->AsString);
        if (index == -1)
        {
            TStringList *_itemsList = new TStringList;
            index = _creditServerList->AddObject(creditQuery->FieldByName("NAME")->AsString, _itemsList);
            TCurrencyTotal *_item = new TCurrencyTotal;
            _item->Total = (creditQuery->FieldByName("PRICE")->AsCurrency * creditQuery->FieldByName("QTY")->AsCurrency);
            _item->Note = creditQuery->FieldByName("NOTE")->AsString;
            _item->TimeStamp = creditQuery->FieldByName("TIME_STAMP")->AsString;
             _item->Name = creditQuery->FieldByName("ITEM_NAME")->AsString;
             _item->StaffName = creditQuery->FieldByName("NAME")->AsString;
            ((TStringList*)_creditServerList->Objects[index])->AddObject(creditQuery->FieldByName("ITEM_NAME")->AsString, _item);
        }
        else
        {
            TCurrencyTotal *_item = new TCurrencyTotal;
            _item->Total = (creditQuery->FieldByName("PRICE")->AsCurrency * creditQuery->FieldByName("QTY")->AsCurrency );
            _item->Note = creditQuery->FieldByName("NOTE")->AsString;
            _item->TimeStamp = creditQuery->FieldByName("TIME_STAMP")->AsString;
             _item->Name = creditQuery->FieldByName("ITEM_NAME")->AsString;
             _item->StaffName = creditQuery->FieldByName("NAME")->AsString;
            ((TStringList*)_creditServerList->Objects[index])->AddObject(creditQuery->FieldByName("ITEM_NAME")->AsString, _item);
        }
    }


    if (_creditServerList->Count > 0)
    {
        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        printOut->PrintFormat->Line->Columns[0]->DoubleLine();
        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->NewLine();

        AddTitle(printOut, "Refund Report");

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;

        Currency totalCredit = 0;
        for (int i = 0; i < _creditServerList->Count; i++)
        {
            // Add User name...
            printOut->PrintFormat->Line->ColCount = 1;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
            printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut->PrintFormat->Add(_creditServerList->Strings[i]);

            // ...followed by the items credited by that user.
            printOut->PrintFormat->Line->ColCount = 3;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 5 / 9;
            printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
            printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 2 / 9;
            printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
            printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - (printOut->PrintFormat->Width * 7 / 9);
            printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

            for (int j = 0; j < ((TStringList*)_creditServerList->Objects[i])->Count; j++)
            {
                printOut->PrintFormat->Add(((TCurrencyTotal*)((TStringList*)
                                _creditServerList->Objects[i])->Objects[j])->TimeStamp.FormatString("dd/mm/yy hh:nn:ss") + "|" + " " +
                                ((TStringList*)_creditServerList->Objects[i])->Strings[j] + "|" +
                                dataFormatUtilities->FormatMMReportCurrency( ((TCurrencyTotal*)((TStringList*)_creditServerList->Objects[i])->Objects[j])->Total ) );

                if (((TCurrencyTotal*)((TStringList*)_creditServerList->Objects[i])->Objects[j])->Note != "")
                {
                    printOut->PrintFormat->Add("Note : " + ((TCurrencyTotal*)((TStringList*)_creditServerList->Objects[i])->Objects[j])->Note + "|");
                }
                printOut->PrintFormat->Add(((TCurrencyTotal*)((TStringList*)_creditServerList->Objects[i])->Objects[j])->Name + "|");  //MM-4385
                printOut->PrintFormat->Add("Staff name :" + ((TCurrencyTotal*)((TStringList*)_creditServerList->Objects[i])->Objects[j])->StaffName + "|");

                totalCredit += ((TCurrencyTotal*)((TStringList*)_creditServerList->Objects[i])->Objects[j])->Total;
                delete((TStringList*)_creditServerList->Objects[i])->Objects[j];
            }
            delete _creditServerList->Objects[i];
        }

        IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

        if(reportSectionDisplayTraits)
        {
            reportSectionDisplayTraits->ApplyTraits(printOut);
        }

        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->Line->Columns[1]->DoubleLine();
        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->Add("Total Refunds|" + dataFormatUtilities->FormatMMReportCurrency( totalCredit ) );

        printOut->PrintFormat->NewLine();
    }
    delete _creditServerList;
}
