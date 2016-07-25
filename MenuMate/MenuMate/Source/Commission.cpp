//---------------------------------------------------------------------------


#pragma hdrstop

#include "Commission.h"
#include "DBContacts.h"
#include "GUIDiscount.h"
#include "Timer.h"
#include "MMMessageBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TCommissionController::TCommissionController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction) : frmListManager(new TfrmListManager(inDisplayOwner)), DBTransaction(inDBTransaction)
{
	DisplayOwner = inDisplayOwner;
	TotalCommission = 0;
}

void TCommissionController::Run(int ZedKey)
{
	frmListManager->Select.RegisterForEvent(OnSelect);
	 frmListManager->Close.RegisterForEvent(OnClose);
	frmListManager->tbtnDelete->Hide();
	frmListManager->tbtnAdd->Hide();
	frmListManager->tbtnEdit->Hide();
	frmListManager->SetCaption("Commission");
	frmListManager->Width = 950;
	PopulateListManager(ZedKey);
	frmListManager->ShowModal();
    frmListManager->Select.DeregisterForEvent(OnSelect);
	 frmListManager->Edit.DeregisterForEvent(OnClose);
}

void TCommissionController::OnSelect(int Row, int Col)
{
	if(Row)
	 {

		TCommissionContainer::iterator itCommissionCache = CommissionCache.begin();

		  TDateTime temp;
		  switch(Col)
		  {

			case 1:
					 temp = DisplayDatePicker(itCommissionCache->second.GetDateFrom());
					 for(; itCommissionCache != CommissionCache.end(); std::advance(itCommissionCache, 1))
					 {
						if(temp >= itCommissionCache->second.GetDateTo())
							break;
						if(temp != itCommissionCache->second.GetDateFrom())
							itCommissionCache->second.SetisDateFromNull(false);
						itCommissionCache->second.SetDateFrom(temp);
					 }
					 break;
				case 2:
					 temp = DisplayDatePicker(itCommissionCache->second.GetDateTo());
					 for(; itCommissionCache != CommissionCache.end(); std::advance(itCommissionCache, 1))
					 {
						if(temp <= itCommissionCache->second.GetDateFrom())
							break;
						if(temp != itCommissionCache->second.GetDateTo())
							itCommissionCache->second.SetisDateToNull(false);
						itCommissionCache->second.SetDateTo(temp);
					 }
					 break;
				case 4:
					itCommissionCache = CommissionCache.find(Row);
					std::auto_ptr<TfrmDiscount> frmCommission(TfrmDiscount::Create<TfrmDiscount>(Screen->ActiveForm));
					frmCommission->Caption = "Commission";
					frmCommission->tbToggleAmount->Hide();
					frmCommission->pnlToggle->Hide();
					 frmCommission->CURInitial = itCommissionCache->second.GetCommission();
					frmCommission->ShowModal();
					 itCommissionCache->second.SetCommission(frmCommission->CURResult);
                break;
        }
        UpdateDisplay();
    }

}

void TCommissionController::OnClose(int Row, int Col)
{
	bool t = (bool)TotalCommission;
    for(TCommissionContainer::iterator itCommissionCache = CommissionCache.begin(); itCommissionCache != CommissionCache.end(); std::advance(itCommissionCache, 1))
	 {
    	if((itCommissionCache->second.GetisDateFromNull() || itCommissionCache->second.GetisDateToNull()) && TotalCommission)
		{
        	MessageBox("Error, please select a date for " + itCommissionCache->second.GetName(), "Error", MB_OKCANCEL + MB_ICONQUESTION);
			return;
		}
	 }


	AnsiString Messages[2] = {"Your total commission is zero, would you like to continue?",
									  "The total commission comes to " + FormatFloat("$0.00", TotalCommission) + ", would you like to continue?"};

	if(MessageBox(Messages[t], "", MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
	{
		frmListManager->ModalResult = mrOk;
	}

}

TDateTime TCommissionController::DisplayDatePicker(TDateTime Date)
{
	std::auto_ptr<TfrmClockTime> frmClockTime(new TfrmClockTime(DisplayOwner, Date));
	frmClockTime->TimeDisplay->Hide();
	frmClockTime->Height = 250;
	frmClockTime->ShowModal();
		return Date;
}

void TCommissionController::PopulateListManager(int ZedKey)
{
	try
	{
		frmListManager->sgDisplay->Cols[0]->Clear();
		frmListManager->sgDisplay->Cols[1]->Clear();
        frmListManager->sgDisplay->Cols[2]->Clear();
		  frmListManager->sgDisplay->Cols[3]->Clear();
		  frmListManager->sgDisplay->Cols[4]->Clear();
		frmListManager->sgDisplay->ColCount = 5;
        std::vector<TCommission> Staff;
        TDBContacts::GetStaff(DBTransaction, Staff, ZedKey);

        for(std::vector<TCommission>::const_iterator ptr = Staff.begin(); ptr != Staff.end(); std::advance(ptr,1))
        {
        	if(CommissionCache.find(ptr->GetPosition()) == CommissionCache.end())
            	CommissionCache.UpdateCache(ptr->GetPosition(), *ptr);
        }
        UpdateDisplay();
	}
	catch(Exception & E)
	{
		ShowMessage(E.Message);
	}
}

void TCommissionController::UpdateDisplay(void)
{
		frmListManager->sgDisplay->Cols[0]->Clear();
		frmListManager->sgDisplay->Cols[1]->Clear();
        frmListManager->sgDisplay->Cols[2]->Clear();
		  frmListManager->sgDisplay->Cols[3]->Clear();
		  frmListManager->sgDisplay->Cols[4]->Clear();

	frmListManager->sgDisplay->Cols[0]->AddObject("Staff Name", (TObject*)0);
	frmListManager->sgDisplay->Cols[1]->AddObject("Date From", (TObject*)0);
	frmListManager->sgDisplay->Cols[2]->AddObject("Date To", (TObject*)0);
	frmListManager->sgDisplay->Cols[3]->AddObject("Num Of Days", (TObject*)0);
	frmListManager->sgDisplay->Cols[4]->AddObject("Commission", (TObject*)0);

	 frmListManager->sgDisplay->RowCount = CommissionCache.size() + 2;
	 int i = 1;
	 double Total = 0;
    for(TCommissionContainer::iterator itCommissionCache = CommissionCache.begin(); itCommissionCache != CommissionCache.end(); itCommissionCache++, i++)
    {
    	frmListManager->sgDisplay->Cols[0]->AddObject(itCommissionCache->second.GetName(), (TObject*)i);
		itCommissionCache->second.GetisDateFromNull() == true ? frmListManager->sgDisplay->Cols[1]->AddObject("", (TObject*)i) :
																frmListManager->sgDisplay->Cols[1]->AddObject(itCommissionCache->second.GetDateFrom().FormatString("dd/mm/yyyy"), (TObject*)i);
		itCommissionCache->second.GetisDateToNull() == true ? frmListManager->sgDisplay->Cols[2]->AddObject("", (TObject*)i) :
																frmListManager->sgDisplay->Cols[2]->AddObject(itCommissionCache->second.GetDateTo().FormatString("dd/mm/yyyy"), (TObject*)i);
		itCommissionCache->second.GetisDateToNull() || itCommissionCache->second.GetisDateFromNull() == true ?
																	  frmListManager->sgDisplay->Cols[3]->AddObject("", (TObject*)i) :
																	  frmListManager->sgDisplay->Cols[3]->AddObject((int)itCommissionCache->second.GetDateTo() - (int)itCommissionCache->second.GetDateFrom(), (TObject*)i);

		frmListManager->sgDisplay->Cols[4]->AddObject(FormatFloat("$0.00", itCommissionCache->second.GetCommission()), (TObject*)i);
		Total = Total + itCommissionCache->second.GetCommission();
	 }
	 TotalCommission = Total;
	 frmListManager->sgDisplay->Cols[0]->AddObject("Total", (TObject*)(i + 1));
	 frmListManager->sgDisplay->Cols[4]->AddObject(FormatFloat("$0.00", Total), (TObject*)(i + 1));
}

TCommissionCache TCommissionController::Get(void)
{
	return CommissionCache;
}

TCommissionContainer::iterator TCommissionCache::begin()
{
	return CommissionContainer.begin();
}

TCommissionContainer::iterator TCommissionCache::end()
{
	return CommissionContainer.end();
}

TCommissionContainer::iterator TCommissionCache::find(int Key)
{
	return CommissionContainer.find(Key);
}

int TCommissionCache::size()
{
	return CommissionContainer.size();
}

void TCommissionCache::UpdateCache(int Key, TCommission inCommission)
{
	CommissionContainer[Key] = inCommission;
}

bool TCommissionCache::Empty(void)
{
	TCommissionContainer::iterator itCommission = CommissionContainer.begin();
	for (itCommission = CommissionContainer.begin(); itCommission != CommissionContainer.end(); itCommission++)
	{
		if (itCommission->second.GetCommission() != 0)
		{
			return false;
		}
	}
	return true;
}



