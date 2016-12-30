#include "BlindBalanceCalculationStrategyForConsolidatedZed.h"
#include "DeviceRealTerminal.h"
#include "GlobalSettings.h"


BlindBalanceCalculationStrategyForConsolidatedZed::BlindBalanceCalculationStrategyForConsolidatedZed(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance, TDateTime* startTime, TDateTime* endTime)
	: BaseReportSectionDisplayStrategy(dbTransaction, globalSettings, startTime, endTime)
{
    _isMasterBalance = isMasterBalance;
}

void BlindBalanceCalculationStrategyForConsolidatedZed::BuildSection(TPrintout* printOut)
{
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    AnsiString terminalNamePredicate = "";
    if(!_globalSettings->EnableDepositBagNum)
    {
        terminalNamePredicate = " and ZEDS.TERMINAL_NAME = :TERMINAL_NAME ";
    }

    TIBSQL *ibInternalQuery = _dbTransaction->Query(_dbTransaction->AddQuery());
    TIBSQL *ibLoadDataQuery = _dbTransaction->Query(_dbTransaction->AddQuery());

    printOut->PrintFormat->Line->ColCount = 3;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 4/10;
	printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
	printOut->PrintFormat->Line->Columns[0]->Text =  "Payment Type";
	printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width / 3;
	printOut->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
	printOut->PrintFormat->Line->Columns[1]->Text = "Blind Balance ";
	printOut->PrintFormat->Line->Columns[2]->Width = printOut->PrintFormat->Width - printOut->PrintFormat->Line->Columns[0]->Width - printOut->PrintFormat->Line->Columns[1]->Width;
	printOut->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
	printOut->PrintFormat->Line->Columns[2]->Text = "Variance ";

	printOut->PrintFormat->AddLine();

    GetBlindBalanceDetailsForConsolidatedZed(ibInternalQuery, terminalNamePredicate);

    ibInternalQuery->ParamByName("startTime")->AsDateTime = *_startTime;
    ibInternalQuery->ParamByName("endTime")->AsDateTime = *_endTime;
    if(!_globalSettings->EnableDepositBagNum)
    {
        ibInternalQuery->ParamByName("TERMINAL_NAME")->AsString = deviceName;
    }
    LoadBlindBalanceDetails(printOut, ibInternalQuery, ibLoadDataQuery, deviceName);
}

void BlindBalanceCalculationStrategyForConsolidatedZed::GetBlindBalanceDetailsForConsolidatedZed(TIBSQL *ibInternalQuery, AnsiString terminalNamePredicate)
{
    ibInternalQuery->Close();
    ibInternalQuery->SQL->Text =
        "select SubQuery1.PAYMENT, sum(SubQuery1.Blindbalance_VALUE) BlindBalance " //--, count(*)Qty "
        "from( "
        "select BLINDBALANCE.PAYMENT, "
        "sum(BLINDBALANCE.BLIND_BALANCE) Blindbalance_VALUE, "
        "ZEDS.Z_KEY, ZEDS.TERMINAL_NAME "
        "from BLINDBALANCE inner join ZEDS on ZEDS.Z_KEY = BLINDBALANCE.Z_KEY "
         "where "
         " ZEDS.TIME_STAMP >= :startTime and ZEDS.TIME_STAMP <= :endTime "
         + terminalNamePredicate +
         "GROUP BY BLINDBALANCE.PAYMENT, ZEDS.Z_KEY, ZEDS.TERMINAL_NAME)SubQuery1 "
         "group by SubQuery1.PAYMENT ";

}

void BlindBalanceCalculationStrategyForConsolidatedZed::LoadBlindBalanceDetails(TPrintout* printOut, TIBSQL *ibInternalQuery, TIBSQL *ibLoadDataQuery, AnsiString deviceName)
{

    ibInternalQuery->ExecQuery();
    for (; !ibInternalQuery->Eof; ibInternalQuery->Next())
	{
		ibLoadDataQuery->Close();
		ibLoadDataQuery->SQL->Text = "select sum(dabp.subtotal) total "
                                        "       from arcbillpay dabp "
                                        "            left join arcbill dab on "
                                        "                 dabp.arcbill_key = dab.arcbill_key "
                                        "       where dab.TIME_STAMP >= :startTime and dab.TIME_STAMP <= :endTime and"
                                        " dabp.pay_type = :pay_type ";

        if (!_globalSettings->EnableDepositBagNum || _isMasterBalance)
		{
			ibLoadDataQuery->SQL->Text = ibLoadDataQuery->SQL->Text +
			"             and dab.terminal_name = :terminal_name "
			"       		  group by dabp.pay_type;";
			ibLoadDataQuery->ParamByName("terminal_name")->AsString = deviceName;
		}
		else
		{
			ibLoadDataQuery->SQL->Text = ibLoadDataQuery->SQL->Text + "       group by dabp.pay_type;";
		}

		printOut->PrintFormat->Line->Columns[0]->Text = ibInternalQuery->FieldByName("PAYMENT")->AsString;
		printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", ibInternalQuery->FieldByName("BlindBalance")->AsCurrency);

		ibLoadDataQuery->ParamByName("pay_type")->AsString = ibInternalQuery->FieldByName("PAYMENT")->AsString;
        ibLoadDataQuery->ParamByName("startTime")->AsDateTime = *_startTime;
        ibLoadDataQuery->ParamByName("endTime")->AsDateTime = *_endTime;
		ibLoadDataQuery->ExecQuery();

		double tempBalance = ibInternalQuery->FieldByName("BlindBalance")->AsCurrency - ibLoadDataQuery->FieldByName("total")->AsCurrency;

		printOut->PrintFormat->Line->Columns[2]->Text = FormatFloat("0.00", tempBalance);
		printOut->PrintFormat->AddLine();

		ibLoadDataQuery->Close();
	}
}
