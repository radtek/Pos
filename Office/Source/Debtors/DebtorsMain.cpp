//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DebtorsMain.h"
#include "Debtors.h"
#include "SelectSupplier.h"
#include "DebtorInvoices.h"
#include "SelectDebtor.h"
#include "MMReportData.h"
#include "Reports.h"
#include "Connections.h"
#include "SelectDate.h"
#include "SetupPurchaseOrders.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmDebtorsMain *frmDebtorsMain;
//---------------------------------------------------------------------------
__fastcall TfrmDebtorsMain::TfrmDebtorsMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorsMain::btnEditStockClick(TObject *Sender)
{
	bool Result = frmDebtors->HasMembershipInstalled(true);
	if (Result)
	{
		return;
	}
	frmDebtors->DebtorsMode = mmEdit;
	frmDebtors->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorsMain::btnPrintInvoiceClick(TObject *Sender)
{
	frmSelectDebtor->Reset = true;
	if (frmSelectDebtor->ShowModal() == mrOk)
	{
		frmDebtorInvoices->DebtorKey = frmSelectDebtor->DebtorKey;
		frmDebtorInvoices->ShowModal();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorsMain::btnStatementClick(TObject *Sender)
{
	frmSelectDebtor->Reset = true;
	if (frmSelectDebtor->ShowModal() == mrOk)
	{
		frmSelectDate->Calendar->Date = Now();
		frmSelectDate->lbeTitle->Caption = "Select the statement date.";
		if (frmSelectDate->ShowModal() != mrOk)
		{
			return;
		}
		TDateTime StatementDate = frmSelectDate->Calendar->Date;

		Word Year, Month, Day;
		TDateTime DueDate = Date();
		DecodeDate(DueDate, Year, Month, Day);

		if (Day >= 20)
		{
			DueDate = IncMonth(DueDate);
			DecodeDate(DueDate, Year, Month, Day);
		}
		Day = 20;
		DueDate = EncodeDate(Year, Month, Day);

		frmSelectDate->Calendar->Date = DueDate;
		frmSelectDate->lbeTitle->Caption = "Select the due date.";
		if (frmSelectDate->ShowModal() != mrOk)
		{
			return;
		}
		DueDate = frmSelectDate->Calendar->Date;
		dmMMReportData->MMTrans->StartTransaction();
		try
		{
			dmMMReportData->SetupDebtorInvoiceStatement(frmSelectDebtor->DebtorKey, StatementDate);
			if (frmReports->rvMenuMate->SelectReport("repDebtorInvoiceStatement", false))
			{
				frmReports->rvMenuMate->SetParam("RemittanceDate", DueDate.DateString());
				frmReports->rvMenuMate->SetParam("PrintDate", StatementDate.FormatString(LongDateFormat));
				frmReports->rvMenuMate->SetParam("GSTNumber", CurrentConnection.GSTNumber);
				frmReports->rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
				frmReports->rvMenuMate->SetParam("CompanyAddress", CurrentConnection.ReportHeader);
				frmReports->rvMenuMate->Execute();
			}
			else
			{
				Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
			}
		}
		__finally
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorsMain::btnAllStatementsClick(TObject *Sender)
{
	frmSelectDate->Calendar->Date = Now();
	frmSelectDate->lbeTitle->Caption = "Select the statement date.";
	if (frmSelectDate->ShowModal() != mrOk)
	{
		return;
	}
	TDateTime StatementDate = frmSelectDate->Calendar->Date;

	Word Year, Month, Day;
	TDateTime DueDate = Date();
	DecodeDate(DueDate, Year, Month, Day);

	if (Day >= 20)
	{
		DueDate = IncMonth(DueDate);
		DecodeDate(DueDate, Year, Month, Day);
	}
	Day = 20;
	DueDate = EncodeDate(Year, Month, Day);

	frmSelectDate->Calendar->Date = DueDate;
	frmSelectDate->lbeTitle->Caption = "Select the due date.";
	if (frmSelectDate->ShowModal() != mrOk)
	{
		return;
	}
	DueDate = frmSelectDate->Calendar->Date;

	dmMMReportData->MMTrans->StartTransaction();
	try
	{
		dmMMReportData->SetupDebtorInvoiceStatement(0, StatementDate);
		if (frmReports->rvMenuMate->SelectReport("repDebtorInvoiceStatement", false))
		{
			//frmReports->rvMenuMate->SetParam("RemittanceDate", DueDate.DateString());
			//frmReports->rvMenuMate->SetParam("PrintDate", StatementDate.FormatString(LongDateFormat));
			//frmReports->rvMenuMate->SetParam("CompanyDetails", CurrentConnection.ReportHeader);
			frmReports->rvMenuMate->SetParam("RemittanceDate", DueDate.DateString());
			frmReports->rvMenuMate->SetParam("PrintDate", StatementDate.FormatString(LongDateFormat));
			frmReports->rvMenuMate->SetParam("GSTNumber", CurrentConnection.GSTNumber);
			frmReports->rvMenuMate->SetParam("CompanyAddress", CurrentConnection.ReportHeader);
			frmReports->rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
			frmReports->rvMenuMate->Execute();
		}
		else
		{
			Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
		}
	}
	__finally
	{
		dmMMReportData->MMTrans->Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorsMain::btnBalanceClick(TObject *Sender)
{
	dmMMReportData->MMTrans->StartTransaction();
	try
	{
		dmMMReportData->SetupDebtorSummary();
		if (frmReports->rvMenuMate->SelectReport("repDebtorSummary", false))
		{
			frmReports->rvMenuMate->SetParam("CompanyDetails", CurrentConnection.ReportHeader);
			frmReports->rvMenuMate->Execute();
		}
		else
		{
			Application->MessageBox("Report not found!", "Error", MB_OK + MB_ICONERROR);
		}
	}
	__finally
	{
		dmMMReportData->MMTrans->Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorsMain::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorsMain::BitBtn1Click(TObject *Sender)
{
	frmSetupPurchaseOrders->ShowModal();
}
//---------------------------------------------------------------------------

