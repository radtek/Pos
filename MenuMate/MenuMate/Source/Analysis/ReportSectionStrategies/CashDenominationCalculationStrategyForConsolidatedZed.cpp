#include "CashDenominationCalculationStrategyForConsolidatedZed.h"
#include "DeviceRealTerminal.h"
#include "GlobalSettings.h"
#include "CashDenominationController.h"

CashDenominationCalculationStrategyForConsolidatedZed::CashDenominationCalculationStrategyForConsolidatedZed(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance, TDateTime* startTime, TDateTime* endTime)
	: BaseReportSectionDisplayStrategy(dbTransaction, globalSettings, startTime, endTime)
{
   _isMasterBalance = isMasterBalance;
   IsConsolidatedStartegy = true;
}

void CashDenominationCalculationStrategyForConsolidatedZed::BuildSection(TPrintout* printOut)
{
    TCashDenominations cashDenominations;
    AnsiString bagId;
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;

    printOut->PrintFormat->Line->ColCount = 3;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 4/10;
	printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
	printOut->PrintFormat->Line->Columns[0]->Text =  "Denomination";
	printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
	printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
	printOut->PrintFormat->Line->Columns[1]->Text = "Quantity Count ";
	printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width - printOut->PrintFormat->Line->Columns[1]->Width;
	printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
	printOut->PrintFormat->Line->Columns[2]->Text = "Banking ";
	printOut->PrintFormat->AddLine();
    GetCashDenominationQueryForConsolidatedZed(printOut, deviceName);
}

void CashDenominationCalculationStrategyForConsolidatedZed::GetCashDenominationQueryForConsolidatedZed(TPrintout* printOut, AnsiString deviceName)
{
    TIBSQL *ibInternalQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
    ibInternalQuery->Close();
    ibInternalQuery->SQL->Text = " select SubQuery1.DENOMINATION_TITLE, sum(SubQuery1.Banking) Banking, sum(SubQuery1.Quantity) qty "
                                 "from( "
                                 " select ZED_CASHDENOMINATIONS.DENOMINATION_TITLE, "
                                 " sum(ZED_CASHDENOMINATIONS.DENOMINATION_VALUE * ZED_CASHDENOMINATIONS.DENOMINATION_QUANTITY) Banking, "
                                 " sum(ZED_CASHDENOMINATIONS.DENOMINATION_QUANTITY) Quantity, "
                                 " ZEDS.Z_KEY, ZEDS.TERMINAL_NAME "
                                 " from ZED_CASHDENOMINATIONS "
                                 " inner join ZEDS on ZEDS.Z_KEY = ZED_CASHDENOMINATIONS.Z_KEY "
                                 " where "
                                 " ZEDS.TIME_STAMP >= :startTime and ZEDS.TIME_STAMP <= :endTime ";

    if (!_globalSettings->EnableDepositBagNum || _isMasterBalance)
    {
        ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text +
        "             and ZEDS.TERMINAL_NAME = :terminal_name and ZED_CASHDENOMINATIONS.TERMINAL_NAME != '' "
        "       		  GROUP BY ZED_CASHDENOMINATIONS.DENOMINATION_TITLE, ZEDS.Z_KEY, ZEDS.TERMINAL_NAME)SubQuery1 "
        "              group by SubQuery1.DENOMINATION_TITLE; ";
        ibInternalQuery->ParamByName("terminal_name")->AsString = deviceName;
    }
    else
    {
        ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + " GROUP BY ZED_CASHDENOMINATIONS.DENOMINATION_TITLE, ZEDS.Z_KEY, ZEDS.TERMINAL_NAME)SubQuery1 "
                                                                   "              group by SubQuery1.DENOMINATION_TITLE; ";
    }

    ibInternalQuery->ParamByName("startTime")->AsDateTime = *_startTime;
    ibInternalQuery->ParamByName("endTime")->AsDateTime = *_endTime;
    ibInternalQuery->ExecQuery();

    double total = 0.00;

	for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
	{
		printOut->PrintFormat->Line->Columns[0]->Text = ibInternalQuery->FieldByName("DENOMINATION_TITLE")->AsString ;
		printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", ibInternalQuery->FieldByName("QTY")->AsDouble);
		printOut->PrintFormat->Line->Columns[2]->Text = FormatFloat("0.00", ibInternalQuery->FieldByName("Banking")->AsDouble);
		printOut->PrintFormat->AddLine();
        total += ibInternalQuery->FieldByName("Banking")->AsDouble;
	}
    ibInternalQuery->Close();

    printOut->PrintFormat->Line->Columns[0]->Text = "";
    printOut->PrintFormat->Line->Columns[1]->Text = "";
    printOut->PrintFormat->Line->Columns[2]->Line();
    printOut->PrintFormat->AddLine();


    printOut->PrintFormat->Line->Columns[0]->Text = "Total";
    printOut->PrintFormat->Line->Columns[1]->Text = "";
    printOut->PrintFormat->Line->Columns[2]->Text = FormatFloat("0.00", total);
    printOut->PrintFormat->AddLine();
}

