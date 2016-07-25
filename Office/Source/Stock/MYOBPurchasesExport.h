//---------------------------------------------------------------------------

#ifndef MYOBPurchasesExportH
#define MYOBPurchasesExportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
#include "VirtualTrees.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <DB.hpp>
#include <ExtCtrls.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <Dialogs.hpp>
#include <IBSQL.hpp>
#include <vector>
#include <set>
#include <map>
#include <string>


//---------------------------------------------------------------------------
class TGLInvoiceData
{
public:
	int			BatckKey;
	TDateTime	Date;
	AnsiString	GLCode;
	AnsiString	Category;

    AnsiString  Description;
    double      OrderQty;

	Currency	TotalCost;
	Currency	TotalGST;
	Currency	GSTPercent;
};
//---------------------------------------------------------------------------
class TInvoiceNodeData
{
public:
	WideString	Text;
	TDateTime	Date;
	AnsiString	Reference;
	AnsiString	SupplierName;
	AnsiString	SupplierID;
	AnsiString	Export1, Export2, Export3;
	Currency		TotalCost;
	std::vector<TGLInvoiceData> GLData;
};
//---------------------------------------------------------------------------
class TfrmMYOBPurchasesExport : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel4;
	TPanel *Panel7;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TPanel *Panel8;
	TBitBtn *BitBtn5;
	TPageControl *pcReportProperties;
	TTabSheet *tsStartTime;
	TLabel *Label1;
	TMonthCalendar *mcStartDate;
	TMonthCalendar *mcEndDate;
	TLabel *Label2;
	TPanel *pnlEndTime;
	TCSpinButton *csEndHour;
	TCSpinButton *csEndMinute;
	TCSpinButton *csStartMinute;
	TPanel *pnlStartTime;
	TCSpinButton *csStartHour;
	TLabel *lbeFrom;
	TLabel *lbeTo;
	TBitBtn *btnDateNext;
	TBitBtn *btnThisMonth;
	TBitBtn *btnWeek;
	TBitBtn *btnThisDay;
	TBevel *Bevel1;
	TTabSheet *tsInvoices;
	TVirtualStringTree *vtvInvoices;
	TBevel *Bevel2;
	TBitBtn *BitBtn1;
	TIBTransaction *Transaction;
	TLabel *lbeFilterRange;
	TBitBtn *btnFilterBack;
	TSaveDialog *SaveDialog;
	TIBSQL *qrInvoices;
	TIBSQL *qrExported;
    TIBSQL *qrInvoicesForXero;
	void __fastcall mcStartDateClick(TObject *Sender);
	void __fastcall csStartHourDownClick(TObject *Sender);
	void __fastcall csStartHourUpClick(TObject *Sender);
	void __fastcall csStartMinuteDownClick(TObject *Sender);
	void __fastcall csStartMinuteUpClick(TObject *Sender);
	void __fastcall csEndHourDownClick(TObject *Sender);
	void __fastcall csEndHourUpClick(TObject *Sender);
	void __fastcall csEndMinuteDownClick(TObject *Sender);
	void __fastcall csEndMinuteUpClick(TObject *Sender);
	void __fastcall btnDateNextClick(TObject *Sender);
	void __fastcall btnThisDayClick(TObject *Sender);
	void __fastcall btnWeekClick(TObject *Sender);
	void __fastcall btnThisMonthClick(TObject *Sender);
	void __fastcall tsInvoicesShow(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall vtvInvoicesGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
	void __fastcall vtvInvoicesGetImageIndex(TBaseVirtualTree *Sender,
          PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
          bool &Ghosted, int &ImageIndex);
	void __fastcall btnFilterBackClick(TObject *Sender);
	void __fastcall St(TObject *Sender);
private:	// User declarations
	TDateTime StartTime, EndTime;
	void LoadTree();
	void ShowDateTimes();

	void GetCBAInvoiceLines(TInvoiceNodeData *InvoiceData, TStringList *Lines, std::set<int> &ExportedList);
	void GetGreentreeInvoiceLines(TInvoiceNodeData *InvoiceData, TStringList *Lines, std::set<int> &ExportedList);

   void ShowInvoicesReport(std::set<int> &ExportedList);

	void MarkAsExported(std::set<int> &ExportedList);
	TDateTime LastDayOfMonth(TDateTime DateTime);

    /*
     * First layer is indexed by the Country name.
     * Second layer is indexed by the MYOB edition name.
     *
     * The value of the second layer is a pointer to an array of integers;
     * Each element of this array specifies an "export entity." These entities
     * specify what fields to export to MYOB.
     */
    std::map<std::string,
             std::map<std::string, const unsigned short *> > myob_export_lut;

    void initialize_myob_export_lut();

    const unsigned short *template_from_myob_version_string(
      const AnsiString &version_string);

    void write_myob_header_from_template(
      TStringList &add_header_to,
      const unsigned short *_template);

    void
    write_myob_invoice_from_template(
      TStringList &add_invoice_line_to,
      std::set<int> &exported_list,
      const TInvoiceNodeData &invoice_data,
      const unsigned short *template_pos);

public:		// User declarations
	__fastcall TfrmMYOBPurchasesExport(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmMYOBPurchasesExport *frmMYOBPurchasesExport;
//---------------------------------------------------------------------------
#endif
