#include "BlindBalanceCalculationStrategy.h"
#include "DeviceRealTerminal.h"
#include "GlobalSettings.h"
#include "BlindBalanceController.h"


BlindBalanceCalculationStrategy::BlindBalanceCalculationStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance)
	: BaseReportSectionDisplayStrategy(dbTransaction, globalSettings)
{
    _isMasterBalance = isMasterBalance;
}

void BlindBalanceCalculationStrategy::BuildSection(TPrintout* printOut)
{
    TBlindBalances balance;
    AnsiString bagId;
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;

    TForm* currentForm = Screen->ActiveForm;
    TBlindBalanceController blindBalanceController(currentForm, *_dbTransaction, deviceName);

    if(printOut->CashDenominationUsed && blindBalanceController.Run())
    {
       _dbTransaction->Commit();
       _dbTransaction->StartTransaction();
    }
    else
    {
        printOut->BlindBalanceUsed = false;
        return;
    }


    balance = blindBalanceController.Get();
    bagId = blindBalanceController.GetBagID();
    TBlindBalanceControllerInterface::Instance()->SetBalances(balance);
    TBlindBalanceControllerInterface::Instance()->SetBagID(bagId);
    TIBSQL *ibInternalQuery = _dbTransaction->Query(_dbTransaction->AddQuery());

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

    TBlindBalanceContainer::iterator itBlindBalances = balance.begin();
	for (itBlindBalances = balance.begin(); itBlindBalances != balance.end(); itBlindBalances++)
	{
		ibInternalQuery->Close();
		ibInternalQuery->SQL->Text = "select sum(dabp.subtotal) total "
                                        "       from dayarcbillpay dabp "
                                        "            left join dayarcbill dab on "
                                        "                 dabp.arcbill_key = dab.arcbill_key "
                                        "       where dabp.pay_type = :pay_type ";

        if (!_globalSettings->EnableDepositBagNum || _isMasterBalance)
		{
			ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text +
			"             and dab.terminal_name = :terminal_name "
			"       		  group by dabp.pay_type;";
			ibInternalQuery->ParamByName("terminal_name")->AsString = deviceName;
		}
		else
		{
			ibInternalQuery->SQL->Text = ibInternalQuery->SQL->Text + "       group by dabp.pay_type;";
		}

		printOut->PrintFormat->Line->Columns[0]->Text = itBlindBalances->first;
		printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", itBlindBalances->second.BlindBalance);

		ibInternalQuery->ParamByName("pay_type")->AsString = itBlindBalances->first;
		ibInternalQuery->ExecQuery();

		itBlindBalances->second.SystemBalance = ibInternalQuery->FieldByName("total")->AsCurrency;
		double tempBalance = itBlindBalances->second.BlindBalance - itBlindBalances->second.SystemBalance;

		printOut->PrintFormat->Line->Columns[2]->Text = FormatFloat("0.00", tempBalance);
		printOut->PrintFormat->AddLine();

		ibInternalQuery->Close();
	}
}
