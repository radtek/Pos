//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Reports.h"
#include "ManagerReports.h"
#include "DeviceRealTerminal.h"
#include "Analysis.h"
#include "Comms.h"
#include "ShowPrintout.h"
#include "GlobalSettings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmReports::TfrmReports(TComponent* Owner)
	: TZForm(Owner)
{

}

void __fastcall TfrmReports::btnCancelMouseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

int TfrmReports::GetBtnSelection(void)
{
    return BtnSelection;
}

//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnStockMouseClick(TObject *Sender)
{
		BtnSelection = 1;
		Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReports::btnTransactionMouseClick(TObject *Sender)
{
	BtnSelection = 2;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReports::btnTransferredMouseClick(TObject *Sender)
{
	BtnSelection = 3;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReports::btnConsumptionMouseClick(TObject *Sender)
{
	BtnSelection = 4;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReports::btnConsumpByMenuMouseClick(TObject *Sender)
{
	BtnSelection = 5;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReports::btnMemberPointsMouseClick(TObject *Sender)
{
	BtnSelection = 6;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReports::btnSiteSummaryMouseClick(TObject *Sender)
{
	BtnSelection = 7;
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnTabsTablesMouseClick(TObject *Sender)
{
	BtnSelection = 8;
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TfrmReports::BtnFloatAdjustmentsMouseClick(TObject *Sender)
{
	BtnSelection = 9;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReports::btnXReportMouseClick(TObject *Sender)
{
	BtnSelection = 10;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmReports::btnEJournalMouseClick(TObject *Sender)
{
	BtnSelection = 11;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnConsolidatedZedMouseClick(TObject *Sender)
{
	BtnSelection = 12;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnHavanaReportMouseClick(TObject *Sender)
{
	BtnSelection = 13;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmReports::btnbtnRoomPaymentReportMouseClick(TObject *Sender)
{
	BtnSelection = 14;
	Close();
}
//---------------------------------------------------------------------------

