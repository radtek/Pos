//---------------------------------------------------------------------------

#ifndef AccountsExportFormH
#define AccountsExportFormH
//---------------------------------------------------------------------------
#include "Accounts.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include <jpeg.hpp>
#include "touchbtn.h"
#include "touchcontrols.h"
//---------------------------------------------------------------------------
namespace Accounts
{
	class AccountsExporter;
}
//---------------------------------------------------------------------------
class TfrmAccountsExport : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TPageControl *PageControl1;
	TTabSheet *tsInvoices;
	TTabSheet *tsCustomers;
	TListView *ListView1;
	TTouchBtn *btnExit;
	TTouchBtn *btnExport;
	TImage *Image1;
	TListView *lvCustomers;
	TTabSheet *TabSheet1;
	TLabel *Label4;
	TEdit *Edit1;
	TTouchBtn *TouchBtn2;
	TListBox *ListBox1;
	TTouchBtn *btnExportCustomers;
	TProgressBar *ProgressBar1;
	TTouchBtn *btnEditCustomer;
	TDateTimePicker *dtpEndDate;
	TComboBox *cbEndHours;
	TComboBox *cbEndMinutes;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TCheckBox *chbUseStartFilter;
	TLabel *Label5;
	TDateTimePicker *dtpStartDate;
	TLabel *Label6;
	TComboBox *cbStartHours;
	TComboBox *cbStartMinutes;
	TLabel *Label7;
	TTabSheet *tsSetup;
	TLabel *Label8;
	TButton *btnSaveSettings;
	TEdit *edAccountCode;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall DateTimeChange(TObject *Sender);
	void __fastcall btnExitMouseClick(TObject *Sender);
	void __fastcall btnExportMouseClick(TObject *Sender);
	void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall btnExportCustomersMouseClick(TObject *Sender);
	void __fastcall btnEditCustomerMouseClick(TObject *Sender);
	void __fastcall lvCustomersSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
	void __fastcall chbUseStartFilterClick(TObject *Sender);
	void __fastcall SettingsChangedEvent(TObject *Sender);
	void __fastcall PageControl1Changing(TObject *Sender, bool &AllowChange);
	void __fastcall btnSaveSettingsClick(TObject *Sender);
private:	// User declarations
	Accounts::AccountsExporter* accountsExporter;

	void LoadSettings();
	void SaveSettings();

	bool settingsChanged;
	void SettingsChanged();

	void RefreshAccounts();

	TDateTime startDateTimeFilter;
	TDateTime endDateTimeFilter;
	void GetDateTimeFilter();
	void DisplayInvoices();

   void __fastcall OnLoadCustomersUpdate(const Accounts::Customer& customer, int percentComplete);
	void __fastcall OnLoadCustomersComplete();

   
public:		// User declarations
	__fastcall TfrmAccountsExport();
   virtual __fastcall ~TfrmAccountsExport();
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmAccountsExport *frmAccountsExport;
//---------------------------------------------------------------------------
#endif
