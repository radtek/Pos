// ---------------------------------------------------------------------------

#pragma hdrstop

#include "CashDenominationController.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"
#include "MMTouchKeyboard.h"
#include "DeviceRealTerminal.h"
#include "ListPaymentSystem.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

TCashDenominationControllerInterface* TCashDenominationControllerInterface::instance = NULL;

TCashDenominationControllerInterface::TCashDenominationControllerInterface()
{

}
//---------------------------------------------------------------------------
TCashDenominationControllerInterface::~TCashDenominationControllerInterface()
{
    //
}

void TCashDenominationControllerInterface::SetCashDenominations(TCashDenominations inCashDenominations)
{
  CashDenominations = inCashDenominations;
}

void TCashDenominationControllerInterface::SetBagID(AnsiString bagId)
{
  BagId = bagId;
}

TCashDenominations TCashDenominationControllerInterface::GetCashDenominations()
{
  return CashDenominations;
}

AnsiString TCashDenominationControllerInterface::GetBagID()
{
  return BagId;
}

TCashDenominationController::TCashDenominationController(TForm *inDisplayOwner, Database::TDBTransaction &inDBTransaction) : frmListManager(new TfrmListManager(inDisplayOwner)), DBTransaction(inDBTransaction)
{
	DisplayOwner = inDisplayOwner;
}

TCashDenominationController::TCashDenominationController(TForm *inDisplayOwner, Database::TDBTransaction &inDBTransaction, AnsiString DeviceName) : frmListManager(new TfrmListManager(inDisplayOwner)), DBTransaction(inDBTransaction), Terminal(DeviceName)
{
	DisplayOwner = inDisplayOwner;
}

bool TCashDenominationController::Run()
{
	frmListManager->btnClose->Caption = "Save";
    frmListManager->tbtnAdd->Visible = false;
	frmListManager->Close.RegisterForEvent(OnClose);
	frmListManager->Edit.RegisterForEvent(OnEdit);
	frmListManager->Select.RegisterForEvent(OnEdit);
    frmListManager->Delete.RegisterForEvent(OnDelete);
	frmListManager->tbtnEdit->Hide();
	frmListManager->tbtnDelete->Caption = "Cancel";
    PopulateDenominationValues(DBTransaction);
	PopulateListManager();
	frmListManager->SetCaption("Cash Denominations Values");
	frmListManager->pnlLabel->Visible = false;
    frmListManager->sgDisplay->Height = 434 + 34;
    frmListManager->sgDisplay->Top = 7;
	if(frmListManager->ShowModal() == mrCancel)
		return false;
	frmListManager->Close.DeregisterForEvent(OnClose);
	frmListManager->Edit.DeregisterForEvent(OnEdit);
	frmListManager->Select.DeregisterForEvent(OnEdit);
    frmListManager->Delete.DeregisterForEvent(OnDelete);
	return true;
}

void TCashDenominationController::OnClose(int Index, int ColIndex)
{
	UnicodeString Total = FormatFloat("0.00", CashDenominations.GetTotal());

	if(CashDenominations.GetTotal() == 0.0)
	{
		if(MessageBox("The total counted is " + Total + ", are you sure you wish to continue?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == IDCANCEL)
			return;
	}
	else
	{
		if(MessageBox("The total counted is " + Total + ", would you like to continue?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == IDCANCEL)
			return;
	}
    std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (DisplayOwner));
    frmTouchKeyboard->Caption = "Enter the deposit bag ID";
    MasterCashDenominations = CashDenominations;
    /*if(TGlobalSettings::Instance().EnableDepositBagNum && !TGlobalSettings::Instance().EnableDontClearZedData)
    {
        LoadBlindBalances();
        if(frmTouchKeyboard->ShowModal() == mrOk )
        {
            BlindBalances.BagID = frmTouchKeyboard->KeyboardText;
            UpdateBlindBalances(frmTouchKeyboard->KeyboardText);
        }
        else
            return;
    }*/
    frmListManager->ModalResult = mrOk;
}

void TCashDenominationController::OnEdit(int Index, int ColIndex)
{
    Index--;
	if (CashDenominations.IndexValid(Index))
     {
		std::auto_ptr<TfrmTouchNumpad>frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(DisplayOwner));
		frmTouchNumpad->Caption = "Enter Quantity";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->Mode = pmNumber;
		frmTouchNumpad->CURInitial = CashDenominations.GetDenominationQuantity(Index);
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			CashDenominations.UpdateDenominationQuantity(Index, frmTouchNumpad->INTResult);
			PopulateListManager();
		}
	}
}

void TCashDenominationController::OnDelete(int Index, int ColIndex)
{
  frmListManager->ModalResult = mrCancel;
}

void TCashDenominationController::PopulateListManager()
{
	try
	{
        int width = (int)(frmListManager->pnlMajor->Width /3);
        frmListManager->sgDisplay->ColCount = 3;
        frmListManager->sgDisplay->Cols[0]->Clear();
        frmListManager->sgDisplay->Cols[1]->Clear();
        frmListManager->sgDisplay->Cols[2]->Clear();
        frmListManager->sgDisplay->ColWidths[0] = width;
        frmListManager->sgDisplay->ColWidths[1] = width;
        frmListManager->sgDisplay->ColWidths[2] = width;

        frmListManager->sgDisplay->Cols[0]->Text ="Denomination";
        frmListManager->sgDisplay->Cols[1]->Text ="Quantity Count";
        frmListManager->sgDisplay->Cols[2]->Text ="Banking";

		frmListManager->sgDisplay->RowCount = CashDenominations.size() + 1 + 1;
		CashDenominations.ResetTotal();
		int j = 0;
		TCashDenominationContainer::iterator itCashDenomination = CashDenominations.begin();
		for (; itCashDenomination != CashDenominations.end(); advance(itCashDenomination,1))
		{
			frmListManager->sgDisplay->Cols[0]->AddObject(itCashDenomination->second.Title, (TObject*)j);
			frmListManager->sgDisplay->Cols[1]->AddObject(FormatFloat("0.00", itCashDenomination->second.Quantity), (TObject*)j);
            frmListManager->sgDisplay->Cols[2]->AddObject(FormatFloat("0.00", itCashDenomination->second.Total), (TObject*)j);
			CashDenominations.AddToTotal(itCashDenomination->second.Total);
            j++;
		}

		frmListManager->sgDisplay->Cols[0]->AddObject("Total", (TObject*)j);
		frmListManager->sgDisplay->Cols[2]->AddObject(FormatFloat("0.00", CashDenominations.GetTotal()), (TObject*)j);
	}
	catch(Exception & E)
	{
		ShowMessage(E.Message);
	}
}

bool TCashDenominationController::WarnOperator(void)
{
	if(MessageBox("The total counted is 0, are you sure you wish to continue?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
	{
		return false;
	}
	return true;
}

TCashDenominations TCashDenominationController::Get()
{
	return CashDenominations;
}

TCashDenominations TCashDenominationController::GetMaster()
{
	return MasterCashDenominations;
}

AnsiString TCashDenominationController::GetBagID(void)
{
	return CashDenominations.BagID;
}

void TCashDenominationController::PopulateDenominationValues(Database::TDBTransaction &DBTransaction)
{
    std::vector<TDenomination> denominations;
    TDBDenominations::LoadDenominations(DBTransaction,denominations);
    for(std::vector<TDenomination>::iterator it = denominations.begin(); it != denominations.end(); ++it)
    {
       TDenomination cashDenomination;
       cashDenomination.Key = it->Key;
       cashDenomination.Title = it->Title;
       cashDenomination.DenominationValue = it->DenominationValue;
       cashDenomination.Quantity = 0;
       cashDenomination.Total = 0;
       CashDenominations.SetCashDenomination(cashDenomination);
    }
}

void TCashDenominations::SetCashDenomination(TDenomination denomination)
{
   CashDenominations[denomination.Key] = denomination;
}

bool TCashDenominations::IndexValid(int Index)
{
	TCashDenominationContainer::iterator itCashDenominations = CashDenominations.begin();
	std::advance(itCashDenominations, Index);
	if (itCashDenominations != CashDenominations.end())
	{
		return true;
	}
	return false;
}

void TCashDenominations::UpdateDenominationQuantity(int key, int Amount)
{
    TCashDenominationContainer::iterator itCashDenominations = CashDenominations.begin();
	std::advance(itCashDenominations, key);
	if (itCashDenominations != CashDenominations.end())
	{
		itCashDenominations->second.Quantity = Amount;
        itCashDenominations->second.Total = itCashDenominations->second.DenominationValue * Amount;
	}
}

int TCashDenominations::GetDenominationQuantity(int key)
{
    TCashDenominationContainer::iterator itCashDenominations = CashDenominations.begin();
	std::advance(itCashDenominations, key);
	if (itCashDenominations != CashDenominations.end())
	{
		return itCashDenominations->second.Quantity;
	}
    return 0;
}

void TCashDenominations::ResetTotal()
{
	Total = 0;
}

Currency TCashDenominations::GetTotal()
{
	return Total;
}

void TCashDenominations::AddToTotal(Currency amount)
{
	Total += amount;
}

int TCashDenominations::size()
{
	return CashDenominations.size();
}

TCashDenominationContainer::iterator TCashDenominations::begin()
{
	return CashDenominations.begin();
}

TCashDenominationContainer::iterator TCashDenominations::end()
{
	return CashDenominations.end();
}








