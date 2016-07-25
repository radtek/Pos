//---------------------------------------------------------------------------


#pragma hdrstop

#include "PrinterReadings.h"
#include "ManagerPhysicalPrinter.h"
#include "MMTouchNumpad.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TPrinterReadingController::TPrinterReadingController(TForm *inDisplayOwner, Database::TDBTransaction &inDBTransaction) : frmListManager(new TfrmListManager(inDisplayOwner)), DBTransaction(inDBTransaction)
{
	DisplayOwner = inDisplayOwner;
}

void TPrinterReadingController::Run(int ZedKey)
{
	 Initialise();
	 frmListManager->Select.RegisterForEvent(OnSelect);
	 frmListManager->Close.RegisterForEvent(OnClose);
	 frmListManager->tbtnDelete->Hide();
	 frmListManager->tbtnAdd->Hide();
	 frmListManager->tbtnEdit->Hide();
	 PopulateListManager(ZedKey);
	 frmListManager->ShowModal();
    frmListManager->Select.DeregisterForEvent(OnSelect);
	 frmListManager->Edit.DeregisterForEvent(OnClose);
}

void TPrinterReadingController::OnSelect(int Row, int Col)
{
	if(((Row > 0 && Row < 7) && (Col == 1 || Col == 2)) || (Row == 7 && Col == 3))
	{
		if(Col == 1 || Col == 2)
		{
			std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (DisplayOwner));
			frmTouchNumpad->Mode = pmNumber;
			frmTouchNumpad->Caption = "Enter the count";
			frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->btnSurcharge->Visible = false;
			frmTouchNumpad->btnOk->Visible = true;

			if (frmTouchNumpad->ShowModal() == mrOk)
				PrinterReadings.InsertCount(Row, Col, frmTouchNumpad->INTResult);
		}
		else
		{
			std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (DisplayOwner));
			frmTouchNumpad->Mode = pmNumber;
			frmTouchNumpad->Caption = "Enter the error count";
			frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->btnSurcharge->Visible = false;
			frmTouchNumpad->btnOk->Visible = true;

			if (frmTouchNumpad->ShowModal() == mrOk)
				PrinterReadings.InsertCount(Row, Col, frmTouchNumpad->INTResult);
		}

		UpdateDisplay();
	}
}

void TPrinterReadingController::OnClose(int Row, int Col)
{
	frmListManager->ModalResult = mrOk;
}

void TPrinterReadingController::Initialise(void)
{
 	frmListManager->btnClose->Caption = "Save";
	frmListManager->tbtnAdd->Caption = "Printer Readings";
	frmListManager->tbtnAdd->Height = 80;
	frmListManager->Width = 800;
	frmListManager->sgDisplay->Width = 620;
	frmListManager->sgDisplay->DefaultColWidth = frmListManager->sgDisplay->Width / 4;
	frmListManager->sgDisplay->ColCount = 4;
}


void TPrinterReadingController::PopulateListManager(int ZedKey)
{
	frmListManager->sgDisplay->Cols[0]->Clear();
	frmListManager->sgDisplay->Cols[1]->Clear();
	frmListManager->sgDisplay->Cols[2]->Clear();
	frmListManager->sgDisplay->Cols[3]->Clear();

	frmListManager->sgDisplay->Cols[0]->AddObject("Printer Name", (TObject*)0);
	frmListManager->sgDisplay->Cols[1]->AddObject("Starting Number", (TObject*)0);
	frmListManager->sgDisplay->Cols[2]->AddObject("Finish Number", (TObject*)0);
	frmListManager->sgDisplay->Cols[3]->AddObject("Num Of Prints", (TObject*)0);

	frmListManager->sgDisplay->RowCount = 8;

	for(int i = 1; i < 8; i++)
	{
		PrinterReadings.InsertToContainer(i, "Printer " + (AnsiString)i);
		if(i == 7)
			PrinterReadings.InsertToContainer(i, "Printer Errors");
		GetFromDB(i);
	}
	UpdateDisplay();
}

TPrinterReadingsInterface TPrinterReadingController::Get(void)
{
	return PrinterReadings;
}

void TPrinterReadingController::GetFromDB(int Printer)
{
	AnsiString temp = IntToStr(Printer);
	if(Printer == 7)
		temp = "Errors";


	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		"SELECT START_NUMBER, END_NUMBER, COPIES "
		"FROM PRINTERREADINGS "
		"WHERE PRINTERREADINGS_KEY < 0 AND PRINTER_NAME = :PRINTER_NAME;";
	IBInternalQuery->ParamByName("PRINTER_NAME")->AsString = "Printer " + temp;
	IBInternalQuery->ExecQuery();
	if(IBInternalQuery->RecordCount)
	{
		PrinterReadings.InsertCount(Printer, 1, IBInternalQuery->FieldByName("START_NUMBER")->AsInteger);
		PrinterReadings.InsertCount(Printer, 2, IBInternalQuery->FieldByName("END_NUMBER")->AsInteger);
		PrinterReadings.InsertCount(Printer, 3, IBInternalQuery->FieldByName("COPIES")->AsInteger);
	}
}


void TPrinterReadingController::UpdateDisplay(void)
{
	frmListManager->sgDisplay->Cols[0]->Clear();
	frmListManager->sgDisplay->Cols[1]->Clear();
	frmListManager->sgDisplay->Cols[2]->Clear();
	frmListManager->sgDisplay->Cols[3]->Clear();

	frmListManager->sgDisplay->Cols[0]->AddObject("Printer Name", (TObject*)0);
	frmListManager->sgDisplay->Cols[1]->AddObject("Starting Number", (TObject*)0);
	frmListManager->sgDisplay->Cols[2]->AddObject("Finish Number", (TObject*)0);
	frmListManager->sgDisplay->Cols[3]->AddObject("Num Of Prints", (TObject*)0);

	frmListManager->sgDisplay->RowCount = 8;

	int i = 1;
	for(TPrinterReadingsContainer::iterator itPrinterReadings = PrinterReadings.begin(); itPrinterReadings != PrinterReadings.end(); std::advance(itPrinterReadings, 1), i++)
	{
		if(itPrinterReadings->second.GetPrinterName() != "Printer Errors")
		{
			frmListManager->sgDisplay->Cols[0]->AddObject(itPrinterReadings->second.GetPrinterName(), (TObject*)i);
			frmListManager->sgDisplay->Cols[1]->AddObject(itPrinterReadings->second.GetStartNumber(), (TObject*)i);
			frmListManager->sgDisplay->Cols[2]->AddObject(itPrinterReadings->second.GetFinishNumber(), (TObject*)i);
			frmListManager->sgDisplay->Cols[3]->AddObject(itPrinterReadings->second.GetCopies(), (TObject*)i);
		}
		else
		{
			frmListManager->sgDisplay->Cols[0]->AddObject(itPrinterReadings->second.GetPrinterName(), (TObject*)i);
			frmListManager->sgDisplay->Cols[1]->AddObject("", (TObject*)i);
			frmListManager->sgDisplay->Cols[2]->AddObject("", (TObject*)i);
			frmListManager->sgDisplay->Cols[3]->AddObject(itPrinterReadings->second.GetCopies(), (TObject*)i);
		}
	}

//	frmListManager->sgDisplay->Cols[0]->AddObject("Printer Errors", (TObject*)i);
}


void TPrinterReadingsInterface::InsertToContainer(int row, AnsiString inPrinterReading)
{
	if(row != 0)
		PrinterReadingsContainer[row].SetPrinterName(inPrinterReading);
}

void TPrinterReadingsInterface::InsertCount(int row, int col, int count)
{

	switch(col)
	{
		case 1:
					PrinterReadingsContainer[row].SetStartNumber(count);
					break;
		case 2:
					PrinterReadingsContainer[row].SetFinishNumber(count);
					break;
		case 3:
					PrinterReadingsContainer[row].SetCopies(count);
					return;
	}
		PrinterReadingsContainer[row].SetCopies();



}

TPrinterReadingsContainer::iterator TPrinterReadingsInterface::begin(void)
{
	return PrinterReadingsContainer.begin();
}

TPrinterReadingsContainer::iterator TPrinterReadingsInterface::end(void)
{
	return PrinterReadingsContainer.end();
}

int TPrinterReadingsInterface::size(void)
{
	return PrinterReadingsContainer.size();
}

bool TPrinterReadingsInterface::Empty(void)
{
	TPrinterReadingsContainer::iterator itPrinterReading = PrinterReadingsContainer.begin();
	for (itPrinterReading = PrinterReadingsContainer.begin(); itPrinterReading != PrinterReadingsContainer.end(); itPrinterReading++)
	{
		if (itPrinterReading->second.GetCopies() != 0)
		{
			return false;
		}
	}
	return true;
}

int TPrinterReadingsInterface::TotalCopies(void)
{
	int Copies = 0;
	TPrinterReadingsContainer::iterator itPrinterReading = PrinterReadingsContainer.begin();
	for (itPrinterReading = PrinterReadingsContainer.begin(); itPrinterReading != PrinterReadingsContainer.end(); itPrinterReading++)
	{
		Copies = Copies + itPrinterReading->second.GetCopies();
		if(itPrinterReading->second.GetPrinterName() == "Printer Errors")
			Copies = Copies - 2*itPrinterReading->second.GetCopies();
	}
	return Copies;
}

void TPrinterReading::SetCopies(void)
{
	Copies = FinishNumber - StartNumber;
}
