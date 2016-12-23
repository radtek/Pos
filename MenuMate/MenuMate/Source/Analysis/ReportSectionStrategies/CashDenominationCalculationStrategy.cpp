#include "CashDenominationCalculationStrategy.h"
#include "DeviceRealTerminal.h"
#include "GlobalSettings.h"
#include "CashDenominationController.h"

CashDenominationCalculationStrategy::CashDenominationCalculationStrategy(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, bool isMasterBalance)
	: BaseReportSectionDisplayStrategy(dbTransaction, globalSettings)
{
    _isMasterBalance = isMasterBalance;
}

void CashDenominationCalculationStrategy::BuildSection(TPrintout* printOut)
{
    TCashDenominations cashDenominations;
    AnsiString bagId;
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;

    TForm* currentForm = Screen->ActiveForm;
    TCashDenominationController CashDenominationController(currentForm, *_dbTransaction, deviceName);
    if(CashDenominationController.Run())
    {
       _dbTransaction->Commit();
       _dbTransaction->StartTransaction();
    }
    else
    {
        printOut->ContinuePrinting = false;
        return;
    }

    cashDenominations = CashDenominationController.Get();
    bagId = CashDenominationController.GetBagID();
    TCashDenominationControllerInterface::Instance()->SetCashDenominations(cashDenominations,_isMasterBalance);
    TCashDenominationControllerInterface::Instance()->SetBagID(bagId);

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

    TCashDenominationContainer::iterator itCashDenominations = cashDenominations.begin();
	for (; itCashDenominations != cashDenominations.end(); itCashDenominations++)
	{
		printOut->PrintFormat->Line->Columns[0]->Text = itCashDenominations->second.Title;
		printOut->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", itCashDenominations->second.Quantity);
		printOut->PrintFormat->Line->Columns[2]->Text = FormatFloat("0.00", itCashDenominations->second.Total);
		printOut->PrintFormat->AddLine();
	}

    printOut->PrintFormat->Line->Columns[0]->Text = "";
    printOut->PrintFormat->Line->Columns[1]->Text = "";
    printOut->PrintFormat->Line->Columns[2]->Line();
    printOut->PrintFormat->AddLine();


    printOut->PrintFormat->Line->Columns[0]->Text = "Total";
    printOut->PrintFormat->Line->Columns[1]->Text = "";
    printOut->PrintFormat->Line->Columns[2]->Text = FormatFloat("0.00", cashDenominations.GetTotal());
    printOut->PrintFormat->AddLine();

}
