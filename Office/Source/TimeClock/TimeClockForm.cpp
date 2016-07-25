//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TimeClockForm.h"
//---------------------------------------------------------------------------
#include "MYOBPayroll.h"
#include "AcePayroll.h"
#include "ExcelPayroll.h"
#include "Connections.h"
#include "MMData.h"
#include "MMReportData.h"
#include "Reports.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
#include <memory>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
class ScreenPayroll : public Payroll::AbstractPayroll
{
public:

	ScreenPayroll(TListView* lv) : listView(lv)
   {
   	listView->Items->Clear();
   }

	virtual void Export()
	{
	}

	virtual void WorkPeriod(
		const Payroll::Worker& worker,
		const Payroll::WorkDay& workDay,
		const Payroll::WorkPeriod& workPeriod)
	{
		TListItem *listItem = listView->Items->Add();
		listItem->Caption = worker.ToString();
		listItem->SubItems->Add(workPeriod.GetDepartment());
		listItem->SubItems->Add(workDay.GetDateWorked().ToString());
		listItem->SubItems->Add(workPeriod.GetStart().FormatString("HH:mm:ss"));
		listItem->SubItems->Add(workPeriod.GetEnd().FormatString("dd/mm/yyyy HH:mm:ss"));
		listItem->SubItems->Add(workPeriod.Duration().ToString());
		listItem->SubItems->Add(workPeriod.GetBreak().FormatString("HH:mm:ss"));
	}

private:

	TListView* listView;
};
//---------------------------------------------------------------------------
__fastcall TfrmTimeClockExport::TfrmTimeClockExport()
	: TForm((TComponent*)NULL), timeClock(0)
{
   cbHours->Items->Clear();
   for (int i=0; i<=23; ++i)
   {
   	cbHours->Items->Add(FormatFloat("00", i));
   }

   cbMinutes->Items->Clear();
   for (int i=0; i<=59; ++i)
   {
   	cbMinutes->Items->Add(FormatFloat("00", i));
   }

   dtpDate->Date = TDateTime(int(Now()));
   cbHours->ItemIndex = cbHours->Items->IndexOf("05");
   cbMinutes->ItemIndex = cbMinutes->Items->IndexOf("00");

   GetDateTimeFilter();
	DisplayTimeClock();

	Label5->Caption = CurrentConnection.PayrollSystem;
}
//---------------------------------------------------------------------------
__fastcall TfrmTimeClockExport::~TfrmTimeClockExport()
{
	delete timeClock;
   timeClock = 0;
}
//---------------------------------------------------------------------------
void TfrmTimeClockExport::DisplayTimeClock()
{
	delete timeClock;
   timeClock = 0;

	timeClock = new Payroll::TimeClockExporter();
	LoadTimeClock(*timeClock);

	std::auto_ptr<Payroll::AbstractPayroll> payroll(new ScreenPayroll(ListView1));
	timeClock->Parse(*payroll);
	payroll->Export();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClockExport::btnExportClick(TObject *Sender)
{
	std::auto_ptr<Payroll::AbstractPayroll> payroll(GetConfiguredPayroll());

	if (Application->MessageBox(
		AnsiString("This will export payroll information to " + CurrentConnection.PayrollSystem +
		". You cannot undo this action. Continue?").c_str(), "Export?", MB_ICONQUESTION + MB_OKCANCEL) != ID_OK)
	{
		return;
	}

	timeClock->Parse(*payroll);

	try
	{
		class DBUpdateExported : public Payroll::UpdateExported
		{
		public:
			DBUpdateExported(TIBDatabase* db) : transaction(db)
			{
				query = transaction.Query(transaction.AddQuery());
				query->SQL->Text =
					"UPDATE CONTACTTIME SET EXPORTED = 'T' WHERE CONTACTTIME.CONTACTTIME_KEY = :Key";

				transaction.Start();
			}

			~DBUpdateExported()
			{
				transaction.Commit();
			}

			virtual void Update(int key)
			{
				try
				{
					query->ParamByName("Key")->AsInteger = key;
					query->ExecQuery();
				}
				catch (Exception& e)
				{
					transaction.Rollback();
					throw e;
				}
			}
		private:
				Database::TDBTransaction transaction;
				TIBSQL* query;
		};

		payroll->Export();
		timeClock->Close(&DBUpdateExported(dmMMData->dbMenuMate));

		Application->MessageBox("Export Complete.", "Complete", MB_ICONINFORMATION + MB_OK);
		ModalResult = mrOk;
	}
	catch (Exception &E)
	{
		Application->MessageBox("There was an error exporting to your payroll system.", "Error", MB_ICONERROR + MB_OK);
		throw;
	}
}
//---------------------------------------------------------------------------
Payroll::AbstractPayroll* TfrmTimeClockExport::GetConfiguredPayroll()
{
	bool pathConfigured = CurrentConnection.PayrollExportPath != "" &&
		DirectoryExists(CurrentConnection.PayrollExportPath);

	bool payrollSelected = true;

	Payroll::AbstractPayroll* payroll = 0;

	if (CurrentConnection.PayrollSystem == "Ace Payroll System")
	{
		payroll = new Payroll::AcePayroll(CurrentConnection.PayrollExportPath);
	}
	else if (CurrentConnection.PayrollSystem == "MYOB Payroll")
	{
		payroll = new Payroll::MYOBPayroll(CurrentConnection.PayrollExportPath);
	}
	else if (CurrentConnection.PayrollSystem == "Microsoft Excel Export")
	{
		pathConfigured = true;
		payroll = new Payroll::ExcelPayroll();
	}
	else
	{
		payrollSelected = false;
	}

	if (payrollSelected == false || pathConfigured == false || timeClock == 0)
	{
		throw Exception("This system has not been configured to use a payroll system.\r\r"
							 "Please select the path to export the file to from the Setup screen.");
	}

	return payroll;
}
//---------------------------------------------------------------------------
void TfrmTimeClockExport::LoadTimeClock(Payroll::TimeClockExporter& payroll)
{
	Database::TDBTransaction transaction(dmMMData->dbMenuMate);

	TIBSQL* query = transaction.Query(transaction.AddQuery());

	query->SQL->Text =
		"SELECT TimeClockLocations.Name as Department, CONTACTS.*, ROUNDEDCONTACTTIME.* FROM CONTACTS "
		"INNER JOIN ROUNDEDCONTACTTIME ON CONTACTS.CONTACTS_KEY = ROUNDEDCONTACTTIME.CONTACTS_KEY "
		"LEFT JOIN TimeClockLocations ON ROUNDEDCONTACTTIME.TimeClockLocations_Key = TimeClockLocations.TimeClockLocations_Key "
		"WHERE CONTACTS.PAYROLL_ID IS NOT NULL AND CONTACTS.PAYROLL_ID != '0' AND ROUNDEDCONTACTTIME.EXPORTED = 'F' AND "
      "ROUNDEDCONTACTTIME.ROUNDED_LOGOUT_DATETIME <= :LoginDateTime "
		"ORDER BY CONTACTS.NAME";

	try
	{
		transaction.Start();

      query->ParamByName("LoginDateTime")->AsDateTime = dateTimeFilter;

		AnsiString lastID;
		Payroll::Worker* worker = 0;

		for (query->ExecQuery(); !query->Eof; query->Next())
		{
			if (lastID != query->FieldByName("Payroll_ID")->AsString || worker == 0)
			{
				worker = &payroll.GetWorker(
					query->FieldByName("Payroll_ID")->AsString,
					query->FieldByName("Name")->AsString);
			}

			TDateTime start = query->FieldByName("Rounded_LOGIN_DATETIME")->AsDateTime;
			TDateTime end = query->FieldByName("Rounded_LOGOUT_DATETIME")->AsDateTime;
            TDateTime Break = query->FieldByName("BREAKS")->AsDateTime;

			if (end > start)
			{
				worker->AddWorkPeriod(start, end, Break, query->FieldByName("Department")->AsString);
				payroll.AddKey(query->FieldByName("ContactTime_Key")->AsInteger);
			}
		}
	}
	catch (Exception &E)
	{
		transaction.Rollback();
		Application->MessageBox("There was an error exporting to your payroll system.", "Error", MB_ICONERROR + MB_OK);
		throw;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClockExport::btnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;	
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClockExport::btnDepartmentReportClick(TObject *Sender)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}

	try
	{
		dmMMReportData->SetupWagesByDepatment(TDateTime(0), dateTimeFilter, NULL);
		frmReports->rvMenuMate->SelectReport("repWages", false);
		frmReports->rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
		frmReports->rvMenuMate->Execute();
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClockExport::Button4Click(TObject *Sender)
{
	if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
	{
		dmMMReportData->MMTrans->StartTransaction();
	}

	try
	{
		dmMMReportData->SetupWagesByStaff(TDateTime(0), dateTimeFilter, NULL);
		frmReports->rvMenuMate->SelectReport("repWagesByStaff", false);
		frmReports->rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
		frmReports->rvMenuMate->Execute();
	}
	__finally
	{
		if (dmMMReportData->MMTrans->DefaultDatabase->Connected)
		{
			dmMMReportData->MMTrans->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTimeClockExport::DateTimeChange(TObject *Sender)
{
	GetDateTimeFilter();
	LoadTimeClock(*timeClock);
	DisplayTimeClock();
}
//---------------------------------------------------------------------------
void TfrmTimeClockExport::GetDateTimeFilter()
{
	int date = dtpDate->Date;
   int hour = 0;
   int minute = 0;

   try
   {
   	hour = cbHours->Text.ToInt();
   }
   catch (EConvertError&)
   {
   	cbHours->Text = "00";
   }

   try
   {
   	minute = cbMinutes->Text.ToInt();
   }
   catch (EConvertError&)
   {
   	cbMinutes->Text = "00";
   }

   dateTimeFilter = TDateTime(date);
   dateTimeFilter += TDateTime(hour, minute, 0, 0);
}
//---------------------------------------------------------------------------

