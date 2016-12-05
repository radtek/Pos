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

TBlindBalanceControllerInterface* TBlindBalanceControllerInterface::instance = NULL;

TBlindBalanceControllerInterface::TBlindBalanceControllerInterface()
{

}
//---------------------------------------------------------------------------
TBlindBalanceControllerInterface::~TBlindBalanceControllerInterface()
{
    //
}

void TBlindBalanceControllerInterface::SetBalances(TBlindBalances balances)
{
  Balances = balances;
}
void TBlindBalanceControllerInterface::SetBagID(AnsiString bagId)
{
  BagId = bagId;
}
TBlindBalances TBlindBalanceControllerInterface::GetBalances()
{
  return Balances;
}
AnsiString TBlindBalanceControllerInterface::GetBagID()
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
	frmListManager->tbtnAdd->Caption = "Add Another Payment Type";
	frmListManager->tbtnAdd->Height = 80;
    frmListManager->tbtnAdd->Visible = false;
	frmListManager->Close.RegisterForEvent(OnClose);
	frmListManager->Add.RegisterForEvent(OnAdd);
	frmListManager->Edit.RegisterForEvent(OnEdit);
	frmListManager->Select.RegisterForEvent(OnEdit);
	frmListManager->Delete.RegisterForEvent(OnDelete);
	frmListManager->tbtnEdit->Hide();
	frmListManager->tbtnDelete->Caption = "Cancel";
	PopulateListManager();
	frmListManager->SetCaption("Blind Balance Values");
	frmListManager->pnlLabel->Visible = false;
    frmListManager->sgDisplay->Height = 434 + 34;
    frmListManager->sgDisplay->Top = 7;
	if(frmListManager->ShowModal() == mrCancel)
		return false;
	frmListManager->Close.DeregisterForEvent(OnClose);
	frmListManager->Add.DeregisterForEvent(OnAdd);
	frmListManager->Edit.DeregisterForEvent(OnEdit);
	frmListManager->Select.DeregisterForEvent(OnEdit);
	frmListManager->Delete.DeregisterForEvent(OnDelete);
	return true;
}

void TCashDenominationController::OnClose(int Index, int ColIndex)
{
	UnicodeString Total = FormatFloat("0.00", BlindBalances.GetTotal());

	if(BlindBalances.GetTotal() == 0.0)
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

		MasterBalance = BlindBalances;
		if(TGlobalSettings::Instance().EnableDepositBagNum && !TGlobalSettings::Instance().EnableDontClearZedData)
		{
			LoadBlindBalances();
			if(frmTouchKeyboard->ShowModal() == mrOk )
			{
				BlindBalances.BagID = frmTouchKeyboard->KeyboardText;
				UpdateBlindBalances(frmTouchKeyboard->KeyboardText);
			}
			else
				return;
		}
		frmListManager->ModalResult = mrOk;
}

void TCashDenominationController::OnAdd(int Index, int ColIndex)
{
	// Balance Name.
	/*AnsiString BalanceName = "";
	Currency BalanceAmount = 0;
	std::auto_ptr<TfrmTouchKeyboard>frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(Screen->ActiveForm));
	frmTouchKeyboard->MaxLength = 20;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = "";
	frmTouchKeyboard->Caption = "Enter the Blind Balance Name ( i.e Cash )";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		BalanceName = frmTouchKeyboard->KeyboardText;

		std::auto_ptr<TfrmTouchNumpad>frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(DisplayOwner));
		frmTouchNumpad->Caption = "Enter the Blind Balance Amount";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->Mode = pmCurrency;
		frmTouchNumpad->CURInitial = 0;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			BlindBalances.UpdateBlindBalance(BalanceName, frmTouchNumpad->CURResult);
			PopulateListManager();
		}
	}*/
}

void TCashDenominationController::OnEdit(int Index, int ColIndex)
{
	//if (BlindBalances.IndexValid(Index + 1))
	//{
		std::auto_ptr<TfrmTouchNumpad>frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(DisplayOwner));
		frmTouchNumpad->Caption = "Enter Quantity Count";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->Mode = pmNumber;
		frmTouchNumpad->CURInitial = BlindBalances.GetBlindBalance(Index + 1);
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			BlindBalances.UpdateBlindBalance(Index, frmTouchNumpad->INTResult);
			PopulateListManager();
		}
	//}
}

void TCashDenominationController::OnDelete(int Index, int ColIndex)
{
/*	if (BlindBalances.IndexValid(Index))
	{
		if (MessageBox("Are you sure you wish to remove " + BlindBalances.BalanceName(Index) + "'s Balance?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
		{
			BlindBalances.RemoveBlindBalance(Index);
			PopulateListManager();
		}
	}
*/
		frmListManager->ModalResult = mrCancel;

}

void TCashDenominationController::PopulateListManager()
{
	try
	{
        frmListManager->sgDisplay->ColCount = 3;
		frmListManager->sgDisplay->Cols[0]->Clear();
		frmListManager->sgDisplay->Cols[1]->Clear();
        frmListManager->sgDisplay->Cols[2]->Clear();
        frmListManager->sgDisplay->Cols[0]->Text ="Denomination";
        frmListManager->sgDisplay->Cols[1]->Text ="Quantity Count";
        frmListManager->sgDisplay->Cols[2]->Text ="Banking";


        std::vector<TCashDenominationDetails> DenominationDetails;

        PopulateDenominationValues(DBTransaction, DenominationDetails);
		// Load up all the current! payment types.
        //std::vector<TPayment> Payments;     std::vector<TPayment> &Payments
		//TDeviceRealTerminal::Instance().PaymentSystem->PaymentsLoadTypes(DBTransaction,Payments);

		  for (std::vector<TCashDenominationDetails>::const_iterator ptr = DenominationDetails.begin(); ptr != DenominationDetails.end(); std::advance(ptr,1))
		  {
			if(BlindBalances.find(ptr->CashDenominationName) == BlindBalances.end())
			{
				BlindBalances.UpdateBlindBalance(ptr->CashDenominationName, 0, 0, ptr->DenominationValue);
			}

				/*if(ptr->Properties & ePayTypeAllowCashOut)
				{
					 if (BlindBalances.find(ptr->Name + " Cash Out") == BlindBalances.end())
					 {
						BlindBalances.UpdateBlindBalance(ptr->Name + " Cash Out", 0);
					 }
				}*/
		  }


		frmListManager->sgDisplay->RowCount = BlindBalances.size() + 1 + 1;

//		Currency Total;
		BlindBalances.ClearTotal();
		int j = 0;
		TBlindBalanceContainer::iterator itBlindBalances;
		TBlindBalanceSortedVector::iterator itSortedBlindBalances = BlindBalances.vectorbegin();
		for (; itSortedBlindBalances != BlindBalances.vectorend(); itSortedBlindBalances++, j++)
		{
			frmListManager->sgDisplay->Cols[0]->AddObject(*itSortedBlindBalances, (TObject*)j);
			itBlindBalances = BlindBalances.find(*itSortedBlindBalances);
			frmListManager->sgDisplay->Cols[1]->AddObject(FormatFloat("0.00", itBlindBalances->second.TransQty), (TObject*)j);
            frmListManager->sgDisplay->Cols[2]->AddObject(FormatFloat("0.00", itBlindBalances->second.BlindBalance), (TObject*)j);
			BlindBalances.SetTotal(itBlindBalances->second.BlindBalance, itBlindBalances->second.CashDenominationValue);
		}

		frmListManager->sgDisplay->Cols[0]->AddObject("Total", (TObject*)j);
		frmListManager->sgDisplay->Cols[2]->AddObject(FormatFloat("0.00", BlindBalances.GetTotal()), (TObject*)j);

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

void TCashDenominationController::LoadBlindBalances(void)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT * "
										  "FROM BLINDBALANCE B "
										  "WHERE DEPOSITBAG_ID = 'To Be Zed';";
	IBInternalQuery->ExecQuery();
	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		BlindBalances.UpdateBlindBalance(IBInternalQuery->FieldByName("PAYMENT")->AsString, IBInternalQuery->FieldByName("BLIND_BALANCE")->AsFloat, IBInternalQuery->FieldByName("BLIND_BALANCE")->AsFloat,IBInternalQuery->FieldByName("BLIND_BALANCE")->AsFloat);
	}
}

void TCashDenominationController::UpdateBlindBalances(AnsiString BagID)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "UPDATE BLINDBALANCE "
										  "SET DEPOSITBAG_ID = :DEPOSITBAG_ID "
										  "WHERE DEPOSITBAG_ID = 'To Be Zed' ";
	IBInternalQuery->ParamByName("DEPOSITBAG_ID")->AsString = BagID;
	IBInternalQuery->ExecQuery();
}

TBlindBalances TCashDenominationController::Get()
{
	return BlindBalances;
}

TBlindBalances TCashDenominationController::GetMaster()
{
	return MasterBalance;
}

void TBlindBalances::UpdateSystemBalance(AnsiString Payment, Currency Amount)
{
	//BlindBalances[Payment].SystemBalance = Amount;
}

void TBlindBalances::UpdateBlindBalance(AnsiString Payment, int qty, Currency Amount, double value)
{
	BlindBalances[Payment].BlindBalance += Amount;
    BlindBalances[Payment].TransQty = qty;
    BlindBalances[Payment].CashDenominationValue = value;
	BlindBalancesVector.push_back(Payment);
}

void TBlindBalances::UpdateSystemBalance(int Index, Currency Amount)
{
	/*TBlindBalanceContainer::iterator itBlindBalances = BlindBalances.begin();
	std::advance(itBlindBalances, Index);
	if (itBlindBalances != BlindBalances.end())
	{
		itBlindBalances->second.SystemBalance = Amount;
	}*/
}

void TBlindBalances::UpdateBlindBalance(int Index, Currency Amount)
{
	TBlindBalanceSortedVector::iterator itSortedBlindBalances = vectorbegin();
	TBlindBalanceContainer::iterator itBlindBalances;
	std::advance(itSortedBlindBalances, Index);
	if (itSortedBlindBalances != vectorend())
	{
		itBlindBalances = BlindBalances.find(*itSortedBlindBalances);
        itBlindBalances->second.TransQty = Amount;
		itBlindBalances->second.BlindBalance = itBlindBalances->second.TransQty * itBlindBalances->second.CashDenominationValue;
	}
}

Currency TBlindBalances::GetSystemBalance(int Index)
{
	/*Currency SysBal = 0;
	TBlindBalanceContainer::iterator itBlindBalances = BlindBalances.begin();
	std::advance(itBlindBalances, Index);
	if (itBlindBalances != BlindBalances.end())
	{
		SysBal = itBlindBalances->second.SystemBalance;
	}*/
}

Currency TBlindBalances::GetBlindBalance(int Index)
{
	Currency BlindBal = 0;
	TBlindBalanceSortedVector::iterator itSortedBlindBalances = vectorbegin();
	TBlindBalanceContainer::iterator itBlindBalances;
	std::advance(itSortedBlindBalances, Index);
	if (itSortedBlindBalances != vectorend())
	{
		itBlindBalances = BlindBalances.find(*itSortedBlindBalances);
		BlindBal = itBlindBalances->second.BlindBalance;
	}
	return BlindBal;
}

Currency TBlindBalances::GetBlindBalance(AnsiString Name)
{
	return BlindBalances[Name].BlindBalance;
}

AnsiString TBlindBalances::BalanceName(int Index)
{
	TBlindBalanceContainer::iterator itBlindBalances = BlindBalances.begin();
	std::advance(itBlindBalances, Index);
	if (itBlindBalances != BlindBalances.end())
	{
		return itBlindBalances->first;
	}
	return "";
}

Currency TBlindBalances::GetSystemBalance(AnsiString Name)
{
	//return BlindBalances[Name].SystemBalance;
}

bool TBlindBalances::IndexValid(int Index)
{
	TBlindBalanceContainer::iterator itBlindBalances = BlindBalances.begin();
	std::advance(itBlindBalances, Index);
	if (itBlindBalances != BlindBalances.end())
	{
		return true;
	}
	return false;
}

TBlindBalanceContainer::iterator TBlindBalances::GetIterator(int Index)
{
	TBlindBalanceContainer::iterator itBlindBalances = BlindBalances.begin();
	std::advance(itBlindBalances, Index);
	return itBlindBalances;
}

void TBlindBalances::RemoveBlindBalance(int Index)
{
	TBlindBalanceContainer::iterator itBlindBalances = GetIterator(Index);
	BlindBalances.erase(itBlindBalances);
}

void TBlindBalances::SetBalance(TBlindBalance &Balance)
{
	BlindBalances[Balance.CashDenominationName] = Balance;
}

TBlindBalanceContainer::iterator TBlindBalances::begin()
{
	return BlindBalances.begin();
}

TBlindBalanceContainer::iterator TBlindBalances::end()
{
	return BlindBalances.end();
}

TBlindBalanceContainer::iterator TBlindBalances::find(AnsiString Value)
{
	return BlindBalances.find(Value);
}

int TBlindBalances::size()
{
	return BlindBalances.size();
}

bool TBlindBalances::Empty()
{
	bool RetVal = true;
	TBlindBalanceContainer::iterator itBlindBalances = BlindBalances.begin();
	for (itBlindBalances = BlindBalances.begin(); itBlindBalances != BlindBalances.end(); itBlindBalances++)
	{
		if (itBlindBalances->second.BlindBalance != 0)
		{
			RetVal = false;
		}
	}
	return RetVal;
}

TBlindBalanceSortedVector::iterator TBlindBalances::vectorbegin()
{
	return BlindBalancesVector.begin();
}

TBlindBalanceSortedVector::iterator TBlindBalances::vectorend()
{
	return BlindBalancesVector.end();
}

void TBlindBalances::SetTotal(Currency sum, double qty)
{
	Total += qty * sum;
}

Currency TBlindBalances::GetTotal(void)
{
	return Total;
}

void TBlindBalances::ClearTotal(void)
{
	Total = 0;
}

AnsiString TCashDenominationController::GetBagID(void)
{
	return BlindBalances.BagID;
}

void TBlindBalance::operator = (TBlindBalance rhs)
{
	CashDenominationName = rhs.CashDenominationName;
	TransQty = rhs.TransQty;
    CashDenominationValue = rhs.CashDenominationValue;
	BlindBalance = rhs.BlindBalance;
}

void TCashDenominationController::PopulateDenominationValues(Database::TDBTransaction &DBTransaction, std::vector<TCashDenominationDetails> &DenominationDetails)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT * "
										  "FROM MESSAGES M "
										  "WHERE M.MESSAGE_TYPE = 14; ";
	IBInternalQuery->ExecQuery();
	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
          TCashDenominationDetails MessageDetails;
          MessageDetails.CashDenominationName = IBInternalQuery->FieldByName("TITLE")->AsString;
          MessageDetails.DenominationValue = StrToFloat(IBInternalQuery->FieldByName("MESSAGE_TEXT")->AsString);
          DenominationDetails.push_back(MessageDetails);
		//BlindBalances.UpdateBlindBalance(IBInternalQuery->FieldByName("PAYMENT")->AsString, IBInternalQuery->FieldByName("BLIND_BALANCE")->AsFloat);
	}
}


