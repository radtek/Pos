//---------------------------------------------------------------------------


#pragma hdrstop

#include "StaffHours.h"
#include "ManagerTimeClock.h"
#include "Contact.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"
#include "DeviceRealTerminal.h"
#include <exception>


//---------------------------------------------------------------------------

#pragma package(smart_init)

TStaffHoursController::TStaffHoursController(TForm *inDisplayOwner, Database::TDBTransaction &inDBTransaction) : frmListManager(new TfrmListManager(inDisplayOwner)), DBTransaction(inDBTransaction)
{
	DisplayOwner = inDisplayOwner;
}

void TStaffHoursController::Run(bool isStaffHoursEditable)
{
	frmListManager->btnClose->Caption = "Save";
	frmListManager->tbtnAdd->Caption = "Add Another Payment Type";
	frmListManager->tbtnAdd->Height = 80;
	frmListManager->Close.RegisterForEvent(OnClose);

    if(isStaffHoursEditable)
   	    frmListManager->Select.RegisterForEvent(OnSelect);

	frmListManager->tbtnEdit->Hide();
	frmListManager->tbtnDelete->Hide();
	frmListManager->tbtnAdd->Hide();
	ChangeListManagerDimensions();
	InitialiseListManager();
	frmListManager->SetCaption("Staff Hours");
	frmListManager->ShowModal();

	frmListManager->Close.DeregisterForEvent(OnClose);

    if(isStaffHoursEditable)
 	    frmListManager->Select.DeregisterForEvent(OnSelect);
}

void TStaffHoursController::OnClose(int Index, int ColIndex)
{
	bool t = (bool)TotalHours;
	AnsiString Messages[2] = {"Warning, your total hours is zero, would you like to continue?",
									  "The total staff hours comes to " + FormatFloat("0.00", TotalHours) + " hours, would you like to continue?"};

	if(MessageBox(Messages[t], "", MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
	{
		UpdateDatabase();
		frmListManager->ModalResult = mrOk;
	}
}

void TStaffHoursController::UpdateDatabase(void)
{
	std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
	std::vector<TStaffHours> Staff_Hours;

    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    dbTransaction.StartTransaction();
	for(TStaffHoursContainer::iterator itStaffHours = StaffHours.begin(); itStaffHours != StaffHours.end(); std::advance(itStaffHours,1))
	{
		ManagerTimeClock->SetLoggedInOutDetails(dbTransaction, itStaffHours);
	}
    dbTransaction.Commit();
}

void TStaffHoursController::OnSelect(int RowIndex, int ColIndex)
{
	if((ColIndex == 1 || ColIndex == 2 || ColIndex == 3) && RowIndex && (RowIndex != frmListManager->sgDisplay->RowCount - 1))
	{
		TStaffHours *SH = StaffHours.GetStaff(RowIndex);
		TDateTime date;
		unsigned short th, tm, ts, tms;
		switch(ColIndex)
		{
			case 1:
			{
				date = SH->GetLoggedIn();
				std::auto_ptr <TfrmClockTime> frmClockTime(new TfrmClockTime(DisplayOwner, date));
				if(frmClockTime->ShowModal() == mrOk)
				{
					if((date < SH->GetLoggedOut()) || SH->GetLogOutNull())
					{
						SH->SetLoggedIn(date);
						SH->SetHoursWorked();
					}
				}
				break;
			}
			case 2:
			{
				date = SH->GetLoggedOut();
				if(SH->GetLogOutNull())
					date = Now();
				std::auto_ptr <TfrmClockTime> frmClockTime(new TfrmClockTime(DisplayOwner, date));
				if(frmClockTime->ShowModal() == mrOk)
				{
					bool temp1 = SH->SetLoggedOut(date);
					SH->SetHoursWorked();
					SH->SetLogOutNull(!temp1 && SH->GetLogOutNull());
				}
				break;
			}
			case 3:
			{
				std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (DisplayOwner));
				frmTouchNumpad->Mode = pmNumber;
				frmTouchNumpad->Caption = "Enter the break time in minutes";
				frmTouchNumpad->btnDiscount->Visible = false;
				frmTouchNumpad->btnSurcharge->Visible = false;
				frmTouchNumpad->btnOk->Visible = true;

				if (frmTouchNumpad->ShowModal() == mrOk)
				{
					SH->SetBreak(frmTouchNumpad->INTResult);
					SH->SetHoursWorked();
				}
			}
		}
		PopulateListManager();
	}
}

void TStaffHoursController::InitialiseListManager(void)
{
	try
	{
		TContact Contact;
		std::vector<TStaffHours> Staff_Hours;
		std::auto_ptr<TManagerTimeClock> ManagerTimeClock( new TManagerTimeClock);
		DBTransaction.StartTransaction();
		TDateTime DateTime = getCurrentTime();

		Contact.Enabled = true;
		ManagerTimeClock->GetLoggedInDetails(DBTransaction, DateTime, Staff_Hours,
        TDeviceRealTerminal::Instance().ID.Name);
		frmListManager->sgDisplay->RowCount = Staff_Hours.size() + 2;
		InitialiseHeader();

		unsigned int i = 0;
		for(std::vector<TStaffHours>::iterator ptr = Staff_Hours.begin(); ptr != Staff_Hours.end(); std::advance(ptr,1))
		{
			AnsiString nm = Contact.GetContactName(DBTransaction, ptr->GetContactNumber());
			ptr->SetContactName(nm);
			ptr->SetHoursWorked();
			StaffHours.InsertToContainer(i + 1, *ptr);
			i++;
		}
		PopulateListManager();

	}
	catch(Exception & E)
	{
		ShowMessage(E.Message);
	}
}

void TStaffHoursController::PopulateListManager(void)
{
	try
	{
		for(int i = 0; i < 5; i++)
		{
			frmListManager->sgDisplay->Cols[i]->Clear();
		}
		InitialiseHeader();
		int i = 1;
		double Total = 0;
		for(TStaffHoursContainer::iterator itStaffHours = StaffHours.begin(); itStaffHours != StaffHours.end(); itStaffHours++, i++)
		{
			frmListManager->sgDisplay->Cols[0]->AddObject(itStaffHours->second.GetContactName(), (TObject*)i);
			frmListManager->sgDisplay->Cols[1]->AddObject(itStaffHours->second.GetLoggedIn().FormatString("dd/mm/yy HH:nn"), (TObject*)i);
			if(itStaffHours->second.GetLogOutNull())
			{
				frmListManager->sgDisplay->Cols[2]->AddObject("Still logged in", (TObject*)i);
				frmListManager->sgDisplay->Cols[4]->AddObject("Unknown", (TObject*)i);
			}
			else
			{
				frmListManager->sgDisplay->Cols[2]->AddObject(itStaffHours->second.GetLoggedOut().FormatString("dd/mm/yy HH:nn"), (TObject*)i);
				frmListManager->sgDisplay->Cols[4]->AddObject(FormatFloat("0.00", itStaffHours->second.GetHoursWorked()), (TObject*)i);
			}
            frmListManager->sgDisplay->Cols[3]->AddObject(itStaffHours->second.GetBreak(), (TObject*)i);
            Total = RoundTo((Total + itStaffHours->second.GetHoursWorked()), -2);
		}

		TotalHours = Total;
		frmListManager->sgDisplay->Cols[0]->AddObject("Total", (TObject*)(i + 1));
		frmListManager->sgDisplay->Cols[4]->AddObject(FormatFloat("0.00", Total), (TObject*)(i + 1));
	}
	catch(Exception & E)
	{
		ShowMessage(E.Message);
	}
}

void TStaffHoursController::ChangeListManagerDimensions(void)
{
	frmListManager->Width = 900;
	frmListManager->sgDisplay->Width = 700;
	frmListManager->sgDisplay->DefaultColWidth = frmListManager->sgDisplay->Width / 5;
	frmListManager->sgDisplay->ColCount = 5;
}

void TStaffHoursController::InitialiseHeader(void)
{
	frmListManager->sgDisplay->Cols[0]->AddObject("Staff Name", 0);
	frmListManager->sgDisplay->Cols[1]->AddObject("Clocked In", 0);
	frmListManager->sgDisplay->Cols[2]->AddObject("Clocked Out", 0);
	frmListManager->sgDisplay->Cols[3]->AddObject("Break (Minutes)", 0);
	frmListManager->sgDisplay->Cols[4]->AddObject("Hours Worked", 0);
}

TDateTime TStaffHoursController::getCurrentTime(void)
{
		unsigned short th, tm, ts, tms;
		unsigned short dd, dm, dy;
		TDateTime now = Now();
		now.DecodeTime(&th, &tm, &ts, &tms);
		now.DecodeDate(&dy, &dm, &dd);
		if(th < 5)
		{
			th = 5;
			now = TDateTime(dy, dm, dd, th, 0, 0, 0);
			now.operator--();
		}
		else
		{
			th = 5;
			now = TDateTime(dy, dm, dd, th, 0, 0, 0);
		}
	return now;
}

TStaffHoursInterface TStaffHoursController::Get(void)
{
	return StaffHours;
}

double TStaffHours::GetHoursWorked(void)
{
	return HoursWorked;
}

void TStaffHours::SetHoursWorked(void)
{
	if(LoggedOut > LoggedIn)
	{
		unsigned short LOh, LOm, LOs, LOms;
		unsigned short LIh, LIm, LIs, LIms;
		LoggedOut.DecodeTime(&LOh, &LOm, &LOs, &LOms);
		LoggedIn.DecodeTime(&LIh, &LIm, &LIs, &LIms);
		TDateTime tempLoggedOut = LoggedOut;
		double Days = 0, Hours;
		for(;(int)(tempLoggedOut - LoggedIn);tempLoggedOut.operator--())
		{
			Days++;
		}
		Hours = Days * 24;

		if(LIh > LOh)
		{
			Hours = Hours + (double)((24 - LIh) + LOh);
		}
		else if(LOh > LIh)
			Hours = Hours + (double)(LOh - LIh);


		if(LIm > LOm)
		{
			Hours = Hours + ((60 - LIm) + LOm)/60.0;
         Hours--;
		}
		else if(LOm > LIm)
		{
      	Hours = Hours + (LOm - LIm)/60.0;
		}
		double onBreak = Break / 60 + (Break % 60)/60.0;
		HoursWorked = (Hours - onBreak);
	}
    else
    {
      HoursWorked = 0;
    }
}

bool TStaffHours::SetLoggedOut(TDateTime inDate)
{
	if(inDate > LoggedIn)
	{
		LoggedOut = inDate;
		return true;
	}
	return false;
}

void TStaffHoursInterface::InsertToContainer(int row, TStaffHours inStaffHours)
{
	if(row != 0)
		StaffHoursContainer[row] = inStaffHours;
}

TStaffHoursContainer::iterator TStaffHoursInterface::begin(void)
{
	return StaffHoursContainer.begin();
}
TStaffHoursContainer::iterator TStaffHoursInterface::end(void)
{
	return StaffHoursContainer.end();
}

TStaffHours* TStaffHoursInterface::GetStaff(int row)
{
	return &StaffHoursContainer[row];
}

int TStaffHoursInterface::size(void)
{
	return StaffHoursContainer.size();
}

bool TStaffHoursInterface::Empty()
{
	TStaffHoursContainer::iterator itStaffHours = StaffHoursContainer.begin();
	for(itStaffHours = StaffHoursContainer.begin(); itStaffHours != StaffHoursContainer.end(); itStaffHours++)
	{
		if(itStaffHours->second.GetContactNumber() != 0)
			return false;
	}
	return true;
}

