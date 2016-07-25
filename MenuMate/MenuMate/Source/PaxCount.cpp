//---------------------------------------------------------------------------


#pragma hdrstop

#include "PaxCount.h"
#include "Timer.h"
#include "MMTouchNumpad.h"
#include "MMMessageBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TPaxCountController::TPaxCountController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction) : frmListManager(new TfrmListManager(inDisplayOwner)), DBTransaction(inDBTransaction)
{
	DisplayOwner = inDisplayOwner;
}

void TPaxCountController::Run(void)
{
	frmListManager->Select.RegisterForEvent(OnSelect);
	frmListManager->Close.RegisterForEvent(OnClose);
	frmListManager->Delete.RegisterForEvent(OnDelete);
	frmListManager->tbtnDelete->Hide();
	frmListManager->tbtnDelete->Caption = "Cancel";
	frmListManager->tbtnAdd->Hide();
	frmListManager->tbtnEdit->Hide();
	frmListManager->SetCaption("Pax Count");
	PopulateListManager();
}

void TPaxCountController::Show(void)
{
	frmListManager->ShowModal();
	frmListManager->Select.DeregisterForEvent(OnSelect);
	frmListManager->Edit.DeregisterForEvent(OnClose);
	frmListManager->Delete.DeregisterForEvent(OnDelete);
}

void TPaxCountController::PopulateListManager(void)
{
	try
	{
		frmListManager->sgDisplay->Cols[0]->Clear();
		frmListManager->sgDisplay->Cols[1]->Clear();
		frmListManager->sgDisplay->ColCount = 2;

		double PhotoPacks = LoadData(ePhotoPacks);
		double Prints = LoadData(ePrints);
		double Total = LoadData(eTotal);
		int Transactions = LoadData(eTransactionCount);

		PaxCount.SetPhotoPacksSold(PhotoPacks);
		PaxCount.SetPrintsSold(Prints);
		PaxCount.SetProductsSold(Total);
		PaxCount.SetTransactionCount(Transactions);

		UpdateDisplay();
	}
	catch(Exception & E)
	{
		ShowMessage(E.Message);
	}

}

void TPaxCountController::UpdateDisplay(void)
{
	frmListManager->sgDisplay->Cols[0]->Clear();
	frmListManager->sgDisplay->Cols[1]->Clear();

/*	frmListManager->sgDisplay->Cols[0]->AddObject("Photo Packs Sold", (TObject*)0);
	frmListManager->sgDisplay->Cols[1]->AddObject(PaxCount.GetPhotoPacksSold(), (TObject*)0);
	frmListManager->sgDisplay->Cols[0]->AddObject("Prints Sold", (TObject*)1);
	frmListManager->sgDisplay->Cols[1]->AddObject(PaxCount.GetPrintsSold(), (TObject*)1);
	frmListManager->sgDisplay->Cols[0]->AddObject("Products Sold", (TObject*)2);
	frmListManager->sgDisplay->Cols[1]->AddObject(PaxCount.GetProductsSold(), (TObject*)2);
	frmListManager->sgDisplay->Cols[0]->AddObject("Transaction Count", (TObject*)3);
	frmListManager->sgDisplay->Cols[1]->AddObject(PaxCount.GetTransactionCount(), (TObject*)3);
*/

	PaxCount.GetPhotoPacksSold();
	PaxCount.GetPrintsSold();
	PaxCount.GetProductsSold();
	PaxCount.GetTransactionCount();

	frmListManager->sgDisplay->Cols[0]->AddObject("PAX Date From", (TObject*)4);
	frmListManager->sgDisplay->Cols[1]->AddObject(PaxCount.GetDateFrom().FormatString("dd/mm/yy"), (TObject*)4);
	frmListManager->sgDisplay->Cols[0]->AddObject("PAX Date To", (TObject*)5);
	frmListManager->sgDisplay->Cols[1]->AddObject(PaxCount.GetDateTo().FormatString("dd/mm/yy"), (TObject*)5);
	frmListManager->sgDisplay->Cols[0]->AddObject("PAX Number", (TObject*)6);
	frmListManager->sgDisplay->Cols[1]->AddObject(PaxCount.GetPaxNumber(), (TObject*)6);
}

void TPaxCountController::OnSelect(int Row, int Col)
{
	if(Col)
	{

		std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (DisplayOwner));
		TDateTime Date;
		unsigned short th, tm, ts, tms;
		unsigned short dd, dm, dy;

		switch(Row)
		{
			case 0:
					PaxCount.GetDateTo().DecodeDate(&dy, &dm, &dd);
					Date = DisplayDatePicker(PaxCount.GetDateFrom());

					if(Date > TDateTime(dy, dm, dd, 0, 0, 0, 0))
						break;
					PaxCount.SetDateFrom(Date);
					break;
			case 1:
					PaxCount.GetDateFrom().DecodeDate(&dy, &dm, &dd);
					Date = DisplayDatePicker(PaxCount.GetDateTo());

					if(Date < TDateTime(dy, dm, dd, 0, 0, 0, 0))
						break;
					PaxCount.SetDateTo(Date);
					break;
			case 2:
					frmTouchNumpad->Mode = pmNumber;
					frmTouchNumpad->Caption = "Enter the start count";
					frmTouchNumpad->btnDiscount->Visible = false;
					frmTouchNumpad->btnSurcharge->Visible = false;
					frmTouchNumpad->btnOk->Visible = true;
					frmTouchNumpad->ShowModal();
					PaxCount.SetPaxNumber(frmTouchNumpad->INTResult);
					break;
		}
		UpdateDisplay();
	}
}

TDateTime TPaxCountController::DisplayDatePicker(TDateTime inDate)
{
	std::auto_ptr <TfrmClockTime> frmClockTime(new TfrmClockTime(DisplayOwner, inDate));
	frmClockTime->TimeDisplay->Hide();
	frmClockTime->Height = 250;
	if(frmClockTime->ShowModal() == mrOk)
	{
		unsigned short dd, dm, dy;
		inDate.DecodeDate(&dy, &dm, &dd);
		return TDateTime(dy, dm, dd, 0, 0, 0, 0);
	}
	else
		return inDate;
}

void TPaxCountController::OnClose(int Row, int Col)
{
	AnsiString Messages[2] = {"Your Pax number is zero, would you like to continue?",
									  "Your Pax number is " + (AnsiString)PaxCount.GetPaxNumber() + ", would you like to continue?"};
	bool t = (bool)PaxCount.GetPaxNumber();

	if(MessageBox(Messages[t], "", MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
	{
		frmListManager->ModalResult = mrOk;
	}

}

void TPaxCountController::OnDelete(int Row, int Col)
{
	frmListManager->ModalResult = mrCancel;
}

double TPaxCountController::LoadData(ePaxType type)
{
	try
	{

		const AnsiString PaxType[3] = {"", "PACK", "PRINT"};
		const AnsiString message[3] = {"", "THIRDPARTYCODES.CODE = :CODE AND ", "THIRDPARTYCODES.CODE = :CODE AND "};
		TDateTime ZedTime = GetDateTime();

		if(type != eTransactionCount)
		{
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"SELECT "
				"DAYARCHIVE.QTY "
			"FROM "
				"DAYARCHIVE JOIN THIRDPARTYCODES ON "
				"DAYARCHIVE.THIRDPARTYCODES_KEY = THIRDPARTYCODES.THIRDPARTYCODES_KEY "
			"WHERE "
				+ message[(int)type] +
				"DAYARCHIVE.TIME_STAMP >= :TIME_STAMP;";

			if((int)type)
				IBInternalQuery->ParamByName("CODE")->AsString = PaxType[(int)type];

			IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = ZedTime;
			IBInternalQuery->ExecQuery();
			double count = 0;
			for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				count = count + IBInternalQuery->FieldByName("QTY")->AsFloat;
			}
			return count;
		}

		else
		{
 			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"SELECT COUNT(TOTAL) FROM DAYARCBILL "
			"WHERE TIME_STAMP >= :TIME_STAMP;";

			IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = ZedTime;
			IBInternalQuery->ExecQuery();
			double count = 0;
			for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				count = count + IBInternalQuery->FieldByName("COUNT")->AsInteger;
			}
			return count;
		}


	}
	catch(Exception &E)
	{
		throw;
	}
}

TDateTime TPaxCountController::GetDateTime(void)
{
	TDateTime now = Now();
	unsigned short h, m, s, ms;
	unsigned short dd, dm, dy;
	now.DecodeTime(&h, &m, &s, &ms);
	now.DecodeDate(&dy, &dm, &dd);

	if(h < 5)
	{
		h = 5;
		now = TDateTime(dy, dm, dd, h, 0, 0, 0);
		now.operator--();
	}
	else
	{
		h = 5;
		now = TDateTime(dy, dm, dd, h, 0, 0, 0);
	}
	return now;
}

void TPaxCountController::Save(AnsiString inDeviceName)
{
		int PaxCountkey;
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PAXCOUNT, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		PaxCountkey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT " "* FROM ZEDS " "WHERE " "TERMINAL_NAME = :TERMINAL_NAME AND " "TIME_STAMP IS NULL";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = inDeviceName;
		IBInternalQuery->ExecQuery();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"INSERT INTO PAXCOUNT (" "PAXCOUNT_KEY," "PAX_NUMBER," "DATE_FROM," "DATE_TO)"
			"VALUES (" ":PAXCOUNT_KEY," ":PAX_NUMBER," ":DATE_FROM," ":DATE_TO);";
		IBInternalQuery->ParamByName("PAXCOUNT_KEY")->AsInteger = PaxCountkey;
		IBInternalQuery->ParamByName("PAX_NUMBER")->AsInteger = PaxCount.GetPaxNumber();
		IBInternalQuery->ParamByName("DATE_FROM")->AsDate = PaxCount.GetDateFrom();
		IBInternalQuery->ParamByName("DATE_TO")->AsDate = PaxCount.GetDateTo();
		IBInternalQuery->ExecQuery();
}

TPaxCount TPaxCountController::Get(void)
{
	return PaxCount;
}
