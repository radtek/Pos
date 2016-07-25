//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PrinterControlPanel.h"
#include "PrinterMaintenance.h"
#include "ManagerVariable.h"
#include <Memory>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
 
//---------------------------------------------------------------------------
__fastcall TfrmPrinterControlPanel::TfrmPrinterControlPanel(TComponent* Owner,Database::TDBControl &inDBControl)
	: TZForm(Owner) , DBControl(inDBControl)
{
}

//---------------------------------------------------------------------------
void __fastcall TfrmPrinterControlPanel::FormShow(TObject *Sender)
{
	FormResize(Sender);
    UpdateDisplay();
}

void TfrmPrinterControlPanel::UpdateDisplay()
{
 	if(PrintingByCourse())
    {
        tbPrintingOrder->ButtonColor = clGreen;
    }
    else
    {
        tbPrintingOrder->ButtonColor = clGray;
    }

    if(PrintingByCat())
    {
        tbtnPrintingByCat->ButtonColor = clGreen;
    }
    else
    {
		tbtnPrintingByCat->ButtonColor = clGray;
    }
}

bool TfrmPrinterControlPanel::PrintingByCourse()
{
    bool RetVal = false;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT * FROM PRNORDER";
    IBInternalQuery->ExecQuery();
    RetVal = IBInternalQuery->RecordCount;
    DBTransaction.Commit();
    return RetVal;
}

bool TfrmPrinterControlPanel::PrintingByCat()
{
    bool RetVal = false;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT * FROM PRNCAT";
    IBInternalQuery->ExecQuery();
    RetVal = IBInternalQuery->RecordCount;
    DBTransaction.Commit();
    return RetVal;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterControlPanel::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;

       		if((double)Screen->Width / Screen->Height < 1.4)
        	{
      			ScaleBy(Screen->Width, Temp);
		}
	}
     //   this->Height = Screen->Height;
	Left				= (Screen->Width - Width) / 2;
	Top				= (Screen->Height - Height) / 2;
	pnlOptions->Left = pnlControl->Left + pnlControl->Width + pnlControl->Left;
	Width = pnlOptions->Left + pnlOptions->Width + pnlControl->Left;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterControlPanel::tbPrintingOrderMouseClick(
      TObject *Sender)
{
	std::auto_ptr<TfrmPrinterMaintenance> PrinterMaintenance(TfrmPrinterMaintenance::Create<TfrmPrinterMaintenance>(this,DBControl));
	PrinterMaintenance->pgControl->ActivePage = PrinterMaintenance->tsPrinting;
	PrinterMaintenance->ShowModal();
	UpdateDisplay();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterControlPanel::tbtnCallAwayPrintingMouseClick(
		TObject *Sender)
{
	std::auto_ptr<TfrmPrinterMaintenance> PrinterMaintenance(TfrmPrinterMaintenance::Create<TfrmPrinterMaintenance>(this,DBControl));
	PrinterMaintenance->pgControl->ActivePage = PrinterMaintenance->tsServingCoursePrinting;
	PrinterMaintenance->ShowModal();
	UpdateDisplay();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterControlPanel::TouchBtn4MouseClick(
		TObject *Sender)
{
	std::auto_ptr<TfrmPrinterMaintenance> PrinterMaintenance(TfrmPrinterMaintenance::Create<TfrmPrinterMaintenance>(this,DBControl));
	PrinterMaintenance->pgControl->ActivePage = PrinterMaintenance->tsKitchenFormat;
	PrinterMaintenance->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterControlPanel::TouchBtn5MouseClick(
		TObject *Sender)
{
	std::auto_ptr<TfrmPrinterMaintenance> PrinterMaintenance(TfrmPrinterMaintenance::Create<TfrmPrinterMaintenance>(this,DBControl));
	PrinterMaintenance->pgControl->ActivePage = PrinterMaintenance->tsPrinters;
	PrinterMaintenance->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterControlPanel::TouchBtn1MouseClick(
		TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterControlPanel::TouchBtn2MouseClick(
		TObject *Sender)
{
	std::auto_ptr<TfrmPrinterMaintenance> PrinterMaintenance(TfrmPrinterMaintenance::Create<TfrmPrinterMaintenance>(this,DBControl));
	PrinterMaintenance->pgControl->ActivePage = PrinterMaintenance->tsDevicePrinterProfiles;
	PrinterMaintenance->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterControlPanel::TouchBtn3MouseClick(
		TObject *Sender)
{
	std::auto_ptr<TfrmPrinterMaintenance> PrinterMaintenance(TfrmPrinterMaintenance::Create<TfrmPrinterMaintenance>(this,DBControl));
	PrinterMaintenance->pgControl->ActivePage = PrinterMaintenance->tsReceiptOptions;
	PrinterMaintenance->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TfrmPrinterControlPanel::BreakdownCatMouseclick(TObject *Sender)
{
	std::auto_ptr<TfrmPrinterMaintenance> PrinterMaintenance(TfrmPrinterMaintenance::Create<TfrmPrinterMaintenance>(this,DBControl));
	PrinterMaintenance->pgControl->ActivePage = PrinterMaintenance->tsBreakdownCats;
	PrinterMaintenance->ShowModal();
  	UpdateDisplay();
}
//---------------------------------------------------------------------------


