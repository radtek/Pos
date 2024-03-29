// ---------------------------------------------------------------------------

#pragma hdrstop

#include "BlindBalanceController.h"
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


TBlindBalanceController::TBlindBalanceController(TForm *inDisplayOwner, Database::TDBTransaction &inDBTransaction) : frmListManager(new TfrmListManager(inDisplayOwner)), DBTransaction(inDBTransaction)
{
	DisplayOwner = inDisplayOwner;
}

TBlindBalanceController::TBlindBalanceController(TForm *inDisplayOwner, Database::TDBTransaction &inDBTransaction, AnsiString DeviceName) : frmListManager(new TfrmListManager(inDisplayOwner)), DBTransaction(inDBTransaction), Terminal(DeviceName)
{
	DisplayOwner = inDisplayOwner;
}



bool TBlindBalanceController::Run()
{
	frmListManager->btnClose->Caption = "Save";
	frmListManager->tbtnAdd->Caption = "Add Another Payment Type";
	frmListManager->tbtnAdd->Height = 80;
	frmListManager->Close.RegisterForEvent(OnClose);
	frmListManager->Add.RegisterForEvent(OnAdd);
	frmListManager->Edit.RegisterForEvent(OnEdit);
	frmListManager->Select.RegisterForEvent(OnEdit);
	frmListManager->Delete.RegisterForEvent(OnDelete);
	frmListManager->tbtnEdit->Hide();
	frmListManager->tbtnDelete->Caption = "Cancel";
	PopulateListManager();
	frmListManager->SetCaption("Blind Balance Values");
	if(frmListManager->ShowModal() == mrCancel)
		return false;

	frmListManager->Close.DeregisterForEvent(OnClose);
	frmListManager->Add.DeregisterForEvent(OnAdd);
	frmListManager->Edit.DeregisterForEvent(OnEdit);
	frmListManager->Select.DeregisterForEvent(OnEdit);
	frmListManager->Delete.DeregisterForEvent(OnDelete);
	return true;
}

void TBlindBalanceController::OnClose(int Index, int ColIndex)
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

void TBlindBalanceController::OnAdd(int Index, int ColIndex)
{
	// Balance Name.
	AnsiString BalanceName = "";
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
	}
}

void TBlindBalanceController::OnEdit(int Index, int ColIndex)
{
	if (BlindBalances.IndexValid(Index))
	{
		std::auto_ptr<TfrmTouchNumpad>frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(DisplayOwner));
		frmTouchNumpad->Caption = "Set the Blind Balance Amount";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->Mode = pmCurrency;
		frmTouchNumpad->CURInitial = BlindBalances.GetBlindBalance(Index);
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			BlindBalances.UpdateBlindBalance(Index, frmTouchNumpad->CURResult);
			PopulateListManager();
		}
	}
}

void TBlindBalanceController::OnDelete(int Index, int ColIndex)
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

void TBlindBalanceController::PopulateListManager()
{
	try
	{
		frmListManager->sgDisplay->Cols[0]->Clear();
		frmListManager->sgDisplay->Cols[1]->Clear();


		// Load up all the current! payment types.
        std::vector<TPayment> Payments;
		TDeviceRealTerminal::Instance().PaymentSystem->PaymentsLoadTypes(DBTransaction,Payments);

		  for (std::vector<TPayment>::const_iterator ptr = Payments.begin(); ptr != Payments.end(); std::advance(ptr,1))
		  {
			if(BlindBalances.find(ptr->Name) == BlindBalances.end())
			{
				BlindBalances.UpdateBlindBalance(ptr->Name, 0);
			}

				if(ptr->Properties & ePayTypeAllowCashOut)
				{
					 if (BlindBalances.find(ptr->Name + " Cash Out") == BlindBalances.end())
					 {
						BlindBalances.UpdateBlindBalance(ptr->Name + " Cash Out", 0);
					 }
				}
		  }

		frmListManager->sgDisplay->ColCount = 2;
		frmListManager->sgDisplay->RowCount = BlindBalances.size() + 1;

//		Currency Total;
		BlindBalances.ClearTotal();
		int j = 0;
		TBlindBalanceContainer::iterator itBlindBalances;
		TBlindBalanceSortedVector::iterator itSortedBlindBalances = BlindBalances.vectorbegin();
		for (; itSortedBlindBalances != BlindBalances.vectorend(); itSortedBlindBalances++, j++)
		{
			frmListManager->sgDisplay->Cols[0]->AddObject(*itSortedBlindBalances, (TObject*)j);
			itBlindBalances = BlindBalances.find(*itSortedBlindBalances);
			frmListManager->sgDisplay->Cols[1]->AddObject(FormatFloat("0.00", itBlindBalances->second.BlindBalance), (TObject*)j);

			BlindBalances.SetTotal(itBlindBalances->second.BlindBalance);
		}

		frmListManager->sgDisplay->Cols[0]->AddObject("Total", (TObject*)j);
		frmListManager->sgDisplay->Cols[1]->AddObject(FormatFloat("0.00", BlindBalances.GetTotal()), (TObject*)j);

	}
	catch(Exception & E)
	{
		ShowMessage(E.Message);
	}
}

bool TBlindBalanceController::WarnOperator(void)
{
	if(MessageBox("The total counted is 0, are you sure you wish to continue?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
	{
		return false;
	}
	return true;
}

void TBlindBalanceController::LoadBlindBalances(void)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT * "
										  "FROM BLINDBALANCE B "
										  "WHERE DEPOSITBAG_ID = 'To Be Zed';";
	IBInternalQuery->ExecQuery();
	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		BlindBalances.UpdateBlindBalance(IBInternalQuery->FieldByName("PAYMENT")->AsString, IBInternalQuery->FieldByName("BLIND_BALANCE")->AsFloat);
	}
}

void TBlindBalanceController::UpdateBlindBalances(AnsiString BagID)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "UPDATE BLINDBALANCE "
										  "SET DEPOSITBAG_ID = :DEPOSITBAG_ID "
										  "WHERE DEPOSITBAG_ID = 'To Be Zed' ";
	IBInternalQuery->ParamByName("DEPOSITBAG_ID")->AsString = BagID;
	IBInternalQuery->ExecQuery();
}

TBlindBalances TBlindBalanceController::Get()
{
	return BlindBalances;
}

TBlindBalances TBlindBalanceController::GetMaster()
{
	return MasterBalance;
}

void TBlindBalances::UpdateSystemBalance(AnsiString Payment, Currency Amount)
{
	BlindBalances[Payment].SystemBalance = Amount;
}

void TBlindBalances::UpdateBlindBalance(AnsiString Payment, Currency Amount)
{
	BlindBalances[Payment].BlindBalance += Amount;
	BlindBalancesVector.push_back(Payment);
}

void TBlindBalances::UpdateSystemBalance(int Index, Currency Amount)
{
	TBlindBalanceContainer::iterator itBlindBalances = BlindBalances.begin();
	std::advance(itBlindBalances, Index);
	if (itBlindBalances != BlindBalances.end())
	{
		itBlindBalances->second.SystemBalance = Amount;
	}
}

void TBlindBalances::UpdateBlindBalance(int Index, Currency Amount)
{
	TBlindBalanceSortedVector::iterator itSortedBlindBalances = vectorbegin();
	TBlindBalanceContainer::iterator itBlindBalances;
	std::advance(itSortedBlindBalances, Index);
	if (itSortedBlindBalances != vectorend())
	{
		itBlindBalances = BlindBalances.find(*itSortedBlindBalances);
		itBlindBalances->second.BlindBalance = Amount;
	}
}

Currency TBlindBalances::GetSystemBalance(int Index)
{
	Currency SysBal = 0;
	TBlindBalanceContainer::iterator itBlindBalances = BlindBalances.begin();
	std::advance(itBlindBalances, Index);
	if (itBlindBalances != BlindBalances.end())
	{
		SysBal = itBlindBalances->second.SystemBalance;
	}
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
	return BlindBalances[Name].SystemBalance;
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
	BlindBalances[Balance.PaymentName] = Balance;
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

void TBlindBalances::SetTotal(Currency sum)
{
	Total += sum;
}

Currency TBlindBalances::GetTotal(void)
{
	return Total;
}

void TBlindBalances::ClearTotal(void)
{
	Total = 0;
}

AnsiString TBlindBalanceController::GetBagID(void)
{
	return BlindBalances.BagID;
}

void TBlindBalance::operator = (TBlindBalance rhs)
{
	PaymentName = rhs.PaymentName;
	PaymentGroup = rhs.PaymentGroup;
	TransQty = rhs.TransQty;
	BlindBalance = rhs.BlindBalance;
	SystemBalance = rhs.SystemBalance;
//	return t;
}


