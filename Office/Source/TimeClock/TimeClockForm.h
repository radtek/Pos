//---------------------------------------------------------------------------
#ifndef TimeClockFormH
#define TimeClockFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
namespace Payroll
{
	class TimeClockExporter;
	class AbstractPayroll;
}
//---------------------------------------------------------------------------
class TfrmTimeClockExport : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label5;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TButton *btnExport;
	TListView *ListView1;
	TButton *btnCancel;
	TButton *btnDepartmentReport;
	TButton *Button4;
	TDateTimePicker *dtpDate;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *cbHours;
	TComboBox *cbMinutes;
	TLabel *Label3;
	void __fastcall btnExportClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnDepartmentReportClick(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall DateTimeChange(TObject *Sender);
	
private:	// User declarations
	Payroll::TimeClockExporter* timeClock;

	void LoadTimeClock(Payroll::TimeClockExporter& payroll);
	Payroll::AbstractPayroll* GetConfiguredPayroll();

   TDateTime dateTimeFilter;
   void GetDateTimeFilter();
	void DisplayTimeClock();

public:		// User declarations
	__fastcall TfrmTimeClockExport();
	__fastcall ~TfrmTimeClockExport();
};
//---------------------------------------------------------------------------
#endif
