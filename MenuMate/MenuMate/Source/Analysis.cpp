// ----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <i_item_definition.hh>
#include <i_item_manager.hh>
#include <i_menu_manager.hh>
#include <item_management_booter.hh>
#include "ManagerCloudSync.h"
#include <FileCtrl.hpp>
#include <Memory>
#include "MMRegistry.h"
#include "Analysis.h"
#include "enum.h"
#include "DeviceRealTerminal.h"
#include "PointsTransaction.h"
#include "PrintFormat.h"
#include "Printout.h"
#include "Comms.h"
#include "MMLogging.h"
#include "CSV.h"
#include "Processing.h"
#include "MMTouchNumpad.h"
#include "Login.h"
#include "Secure.h"
#include "SelectZed.h"
#include "DBSaleTimes.h"
#include "MMMessageBox.h"
#include "SelectDish.h"
#include "Rooms.h"
#include "Eftpos.h"
#include "ManagerVariable.h"
#include "TableManager.h"
#include "ManagerStock.h"
#include "ShowPrintout.h"
#include "DropDown.h"
#include "ContactStaff.h"
#include "DBOrder.h"
#include "ManagerFloat.h"
#include "MallExportUpdateAdaptor.h"
#include "MallExportHourlyUpdate.h"
#include "MallExportTransactionUpdate.h"
#include "MallExportOtherDetailsUpdate.h"
#include "DBTables.h"
#include "DBThirdPartyCodes.h"
#include "DBSecurity.h"
#include "ManagerDiscount.h"
#include "ManagerPatron.h"
#include "DBGroups.h"
#include "SalesForceCommAtZed.h"
#include "CashDenominationController.h"
#include "ExportCSV.h"
#include "StringTools.h"
#include "MallFactory.h"
#include "FiscalPrinterAdapter.h"
#include "DbMegaworldMall.h"
#include "DBAdyen.h"

#include <string>
#include <cassert>
#include "MallExportManager.h"
#include "SendEmail.h"
#include <Dateutils.hpp>
   #include <wininet.h>
#include <dirent.h>

#include <Math.hpp>
#include "ServicesManager.h"
// ---------------------------------------------------------------------------
#pragma comment(lib, "wininet")
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
const UnicodeString StartAdjustError = "Unable to Start DLL adjustments";
const UnicodeString AdjustError = "Stock DLL Adjust Stock error";
const UnicodeString EndAdjustError = "Unable to committ DLL adjustments";

using namespace StockInterface;


__fastcall TfrmAnalysis::TfrmAnalysis(TComponent* Owner) : TZForm(Owner)
{
    zedLogsList = new TStringList();
}

TMMContactInfo TfrmAnalysis::lastAuthenticatedUser;

int ResetKey; //MM 4579

bool TfrmAnalysis::are_any_tables_or_tabs_outstanding()
const
{
	Database::TDBTransaction tr(
	TDeviceRealControl::ActiveInstance().DBControl);
	std::auto_ptr<TIBSQL> qr(new TIBSQL(NULL));
	int n_open_tabs;

	tr.RegisterQuery(qr);

	qr->SQL->Text = "select count(*) n_open_tabs "
	"       from tab "
	"            inner join orders on "
	"                  orders.tab_key = tab.tab_key;";
	tr.StartTransaction();
	qr->ExecQuery();
	n_open_tabs = qr->FieldByName("n_open_tabs")->AsInteger;
	tr.Commit();

	return n_open_tabs > 0;
}

void TfrmAnalysis::print_chit_statistics(TPrintout &po)
{
	TDeviceRealTerminal &drt = TDeviceRealTerminal::Instance();

	Database::TDBTransaction tr(drt.DBControl);
	TIBSQL *q = tr.Query(tr.AddQuery());
	TPrintFormat &pf = *po.PrintFormat;
	const UnicodeString terminal_name = drt.ID.Name;
	const bool midpoint_rounds_down =
	TGlobalSettings::Instance().MidPointRoundsDown;
	std::map<UnicodeString, Currency> chit_totals;
	std::map<UnicodeString, int> chit_patrons;
	int total_patrons = 0;
	Currency total_spend = 0;
	Currency average_total_spend = 0;

	// since we use the sql in keyword in below queries, we need to wrap the terminal name in quotes
	UnicodeString terminalNames = "'" + terminal_name + "'";


	// check whether this terminal is in master slave configuration
	bool isMaster = TGlobalSettings::Instance().EnableDepositBagNum;

	if(isMaster)
	{
		// if so check whether there are more terminal names in the dayarchive table
		// this is where the sales recorded in at the end of the day from master and slaves
		tr.StartTransaction();
		q->SQL->Text = "select distinct(terminal_name) "
		"       from dayarchive "
		"       where terminal_name <> :terminal_name;";
		q->ParamByName("terminal_name")->AsString = terminal_name;
		q->ExecQuery();
		if(!q->Eof)
		{
			while (!q->Eof) {
				terminalNames += ",'" + q->FieldByName("terminal_name")->AsString + "'";
				q->Next();
			}
		}
		q->Close();
		tr.Commit();
	}

	tr.StartTransaction();
	/* Calculate the average total spend. */
	q->SQL->Text = "select sum(price * qty) total_spend, "
	"       count(*) n_sales "
	"       from dayarchive "
	"       where terminal_name in ( " + terminalNames + " ) "
	"             and chit_name is not null;";
	//q->ParamByName("terminal_name")->Value = terminalNames;
	q->ExecQuery();
	total_spend = q->FieldByName("total_spend")->AsCurrency;
	average_total_spend = q->FieldByName("total_spend")->AsCurrency
	/ (q->FieldByName("n_sales")->AsCurrency
	+ (q->FieldByName("n_sales")->AsInteger == 0));
	q->Close();

	/* Calculate the averages for each chit type. */
	q->SQL->Text = "select chit_name, "
	"       sum(price * qty) average_spend "
	"       from dayarchive "
	"       where terminal_name in ( " + terminalNames + " ) "
	"             and chit_name is not null "
	"       group by chit_name;";
	//q->ParamByName("terminal_name")->Value = terminalNames;
	for (q->ExecQuery(); !q->Eof; q->Next()) {
		chit_totals[q->FieldByName("chit_name")->AsString] =
		q->FieldByName("average_spend")->AsCurrency;
	}
	q->Close();

	/* Determine the patron count for each chit type. */
	q->SQL->Text =
	"select dacp.chit_name, "
	"       sum(dac.patron_count) n_patrons "
	"       from daypatroncount dac "
	"            inner join ( "
	"              select da.arcbill_key, "
	"                     da.chit_name "
	"                     from daypatroncount dac "
	"                          inner join dayarchive da on "
	"                                da.arcbill_key = dac.arcbill_key "
	"                     where terminal_name in ( " + terminalNames + " ) "
	"                           and chit_name is not null "
	"                     group by da.arcbill_key, "
	"                              da.chit_name "
	"            ) dacp on "
	"                  dacp.arcbill_key = dac.arcbill_key "
	"       group by dacp.chit_name;";
	//q->ParamByName("terminal_name")->Value = terminalNames;
	for (q->ExecQuery(); !q->Eof; q->Next()) {
		const int n_patrons = q->FieldByName("n_patrons")->AsInteger;

		chit_patrons[q->FieldByName("chit_name")->AsString] += n_patrons;
		total_patrons += n_patrons;
	}
	q->Close();
	tr.Commit();

	AddSubSectionTitle(&po, "Chit Averages");

	pf.Line->ColCount = 3;

	for (std::map<UnicodeString, Currency>::const_iterator i =
	chit_totals.begin(); i != chit_totals.end(); ++i) {
		const int n_patron = chit_patrons[i->first];

		pf.Line->Columns[0]->Text = i->first;
		pf.Line->Columns[1]->Text = IntToStr(chit_patrons[i->first]);
		pf.Line->Columns[2]->Text =
		UnicodeString::CurrToStrF(
		RoundToNearest(i->second / Currency(n_patron + (n_patron == 0)),
		0.01,
		midpoint_rounds_down),
		UnicodeString::sffFixed,
		2);
		pf.AddLine();
	}
	pf.DrawLine();
	pf.Line->Columns[0]->Text = "Totals";
	pf.Line->Columns[1]->Text = IntToStr(total_patrons);
	pf.Line->Columns[2]->Text =
	UnicodeString::CurrToStrF(
	RoundToNearest(total_spend / Currency(total_patrons + (total_patrons == 0)), 0.01, midpoint_rounds_down),
	UnicodeString::sffFixed,
	2);
	pf.AddLine();
	pf.NewLine();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmAnalysis::FormCreate(TObject *Sender)
{
	ZedToArchive = new TMemoryStream;

	ZedCancel    = false;
	ZedCompleted = false;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmAnalysis::FormDestroy(TObject *Sender)
{
	delete ZedToArchive;
}
// ---------------------------------------------------------------------------
void TfrmAnalysis::DefaultItemQuantities(Database::TDBTransaction &tr)
{
    try
    {
        item_management::i_menu_manager &mm = item_management::kickstarter::get_menu_manager();
        item_management::i_item_manager &im = item_management::kickstarter::get_item_manager();
        TIBSQL *qr = tr.Query(tr.AddQuery());

        std::set<menu_key_type> menus_to_update = mm.get_loaded_menus();

        for (std::set<menu_key_type>::const_iterator i = menus_to_update.begin(); i != menus_to_update.end(); ++i)
        {
            qr->SQL->Text = "select i.item_key, "
            "       s.itemsize_key "
            "       from itemsize s "
            "            inner join item i on "
            "                  i.item_key = s.item_key "
            "            inner join course c on "
            "                  c.course_key = i.course_key "
            "       where c.menu_key = :menu_key "
            "             and s.available_quantity <> -1 "
            "             and s.default_quantity <> -1;";

            qr->ParamByName("menu_key")->AsInteger = *i;

            for (qr->ExecQuery(); !qr->Eof; qr->Next()) {
                const size_key_type sid = qr->FieldByName("itemsize_key")->AsInteger;
                item_management::i_item_definition &id =
                im.get_item_definition(qr->FieldByName("item_key")->AsInteger);

                id.set_available_quantity(sid, id.get_default_quantity(sid));
            }

            qr->Close();
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

TDateTime TfrmAnalysis::GetPrevZedTime(Database::TDBTransaction &DBTransaction)
{
    TDateTime PrevZedTime = Now();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT " "MAX(TIME_STAMP)TIME_STAMP FROM ZEDS " "WHERE " "TERMINAL_NAME = :TERMINAL_NAME";
        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();
        IBInternalQuery->ExecQuery();
        if (IBInternalQuery->RecordCount)
        {
            PrevZedTime = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    return PrevZedTime;
}

void TfrmAnalysis::print_weborder_statistics(Database::TDBTransaction &tr, TPrintFormat &pfmt)
{
    try
    {
        TDateTime  last_zed = GetPrevZedTime(tr);
        TIBSQL     *qr = tr.Query(tr.AddQuery());
        unsigned int cancelled_items   = 0;
        unsigned int processed_items   = 0;
        unsigned int unprocessed_items = 0;
        unsigned int total_items_received;
        unsigned int orders_processed;
        unsigned int orders_received;
        float kpi_items;
        float kpi_orders;

        qr->SQL->Text =
        "select distinct "
        "       da.archive_key, "
        "       da.order_type "
        "       from dayarchive da "
        "            inner join dayarcweb daw on "
        "                  daw.arcbill_key = da.arcbill_key "
        "            inner join weborders wo on "
        "                  wo.weborder_key = daw.weborder_key "
        "            inner join webdata wd on "
        "                  wd.weborder_key = wo.weborder_key "
        "       where "
        "             wd.data like 'Stock Purchase Order No.%';";
        qr->ExecQuery();

        for (int order_type; !qr->Eof; qr->Next()) {
            order_type = qr->FieldByName("order_type")->AsInteger;
            processed_items += order_type == 0;
            cancelled_items += order_type == 2;
        }

        qr->Close();
        qr->SQL->Text =
        "select sum(o.qty) unprocessed_items "
        "       from orders o "
        "            inner join weborders wo on "
        "                  wo.tab_key = o.tab_key "
        "            inner join webdata wd on "
        "                  wd.weborder_key = wo.weborder_key "
        "       where wo.order_date > :last_z_time "
        "             and wd.data like 'Stock Purchase Order No.%';";
        qr->ParamByName("last_z_time")->AsDateTime = last_zed;
        qr->ExecQuery();

        unprocessed_items = qr->FieldByName("unprocessed_items")->AsInteger;

        total_items_received =
        cancelled_items + unprocessed_items + processed_items;

        qr->Close();
        qr->SQL->Text =
        "select count(*) weborders_processed "
        "       from (select distinct "
        "                    daw.weborder_key "
        "                    from dayarcweb daw "
        "                         inner join weborders wo on "
        "                               wo.weborder_key = daw.weborder_key "
        "                         inner join webdata wd on "
        "                               wd.weborder_key = wo.weborder_key "
        "                    where wd.data like 'Stock Purchase Order No.%');";
        qr->ExecQuery();
        orders_processed = qr->FieldByName("weborders_processed")->AsInteger;

        qr->Close();
        qr->SQL->Text =
        "select count(*) weborders_received "
        "       from weborders wo "
        "            inner join webdata wd on "
        "                  wd.weborder_key = wo.weborder_key "
        "       where wo.order_date >= :last_z_time "
        "             and wo.status <> 0 "
        "             and wd.data like 'Stock Purchase Order No.%';";
        qr->ParamByName("last_z_time")->AsDateTime = last_zed;
        qr->ExecQuery();
        orders_received = qr->FieldByName("weborders_received")->AsInteger;

        qr->Close();

        kpi_orders =  orders_received ? (orders_processed * 100) / orders_received
        : 0.0;
        kpi_items =
        total_items_received ? (processed_items * 100) / total_items_received
        : 0.0;

        pfmt.Line->ColCount = 1;
        pfmt.Line->Columns[0]->Width = pfmt.Width;

        pfmt.Line->Columns[0]->DoubleLine();
        pfmt.AddLine();
        pfmt.Line->Columns[0]->Alignment = taCenter;
        pfmt.Line->Columns[0]->Text = "Stock Purchase Order Analysis";
        pfmt.AddLine();
        pfmt.Line->Columns[0]->Text = "";
        pfmt.AddLine();

        pfmt.Line->ColCount = 2;
        pfmt.Line->Columns[0]->Alignment =
        pfmt.Line->Columns[1]->Alignment = taLeftJustify;
        pfmt.Line->Columns[0]->Width =
        pfmt.Line->Columns[1]->Width = pfmt.Width >> 1;

        AnsiString column_names[6] = {
            "Orders Received",
            "Orders Processed",
            "Processed Analysis",
            "Line Items Received",
            "Line Items Processed",
            "Processed Analysis"
        };

        AnsiString float_fmt[2] = {"0", "%0.00"};

        pfmt.Line->Columns[1]->Alignment = taRightJustify;
        pfmt.Line->Columns[0]->Text = "Orders Received:";
        pfmt.Line->Columns[1]->Text = FormatFloat("0", orders_received);
        pfmt.AddLine();
        pfmt.Line->Columns[0]->Text = "Orders Processed:";
        pfmt.Line->Columns[1]->Text = FormatFloat("0", orders_processed);
        pfmt.AddLine();
        pfmt.Line->Columns[0]->Text = "Processed Analysis:";
        pfmt.Line->Columns[1]->Text = FormatFloat("%0.00", kpi_orders);
        pfmt.AddLine();
        pfmt.Line->Columns[0]->Text = "Line Items Received:";
        pfmt.Line->Columns[1]->Text = FormatFloat("0", total_items_received);
        pfmt.AddLine();
        pfmt.Line->Columns[0]->Text = "Line Items Processed:";
        pfmt.Line->Columns[1]->Text = FormatFloat("0", processed_items);
        pfmt.AddLine();
        pfmt.Line->Columns[0]->Text = "Processed Analysis:";
        pfmt.Line->Columns[1]->Text = FormatFloat("%0.00", kpi_items);
        pfmt.AddLine();

        pfmt.Line->ColCount = 1;
        pfmt.Line->Columns[0]->Width = pfmt.Width;
        pfmt.Line->Columns[0]->Text = "";
        pfmt.AddLine();
        pfmt.Line->Columns[0]->DoubleLine();
        pfmt.AddLine();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void TfrmAnalysis::AddSectionTitle(TPrintout *Printout, UnicodeString Title)
{
	Printout->PrintFormat->NewLine();
	Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
	Printout->PrintFormat->Line->ColCount = 1;
	Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
	Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
	Printout->PrintFormat->Line->Columns[0]->Text = "";
	Printout->PrintFormat->Line->Columns[0]->DoubleLine();
	Printout->PrintFormat->AddLine();

	Printout->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
	Printout->PrintFormat->Line->ColCount = 3;
	Printout->PrintFormat->Line->Columns[0]->Width = (Printout->PrintFormat->Width - Title.Length()) / 2;
	Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
	Printout->PrintFormat->Line->Columns[1]->Width = Title.Length();
	Printout->PrintFormat->Line->Columns[1]->Alignment = taCenter;
	Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width - Title.Length() -
	(Printout->PrintFormat->Width - Title.Length()) / 2;
	Printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
	Printout->PrintFormat->Line->Columns[0]->Text = "";
	Printout->PrintFormat->Line->Columns[1]->Text = Title;
	Printout->PrintFormat->Line->Columns[2]->Text = "";
	Printout->PrintFormat->AddLine();
}

void TfrmAnalysis::AddSubSectionTitle(TPrintout *Printout, UnicodeString Title)
{
	Printout->PrintFormat->NewLine();
	Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
	Printout->PrintFormat->Line->ColCount = 3;
	Printout->PrintFormat->Line->Columns[0]->Width = (Printout->PrintFormat->Width - Title.Length()) / 2;
	Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
	Printout->PrintFormat->Line->Columns[1]->Width = Title.Length();
	Printout->PrintFormat->Line->Columns[1]->Alignment = taCenter;
	Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width - Title.Length() -
	(Printout->PrintFormat->Width - Title.Length()) / 2;
	Printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

	Printout->PrintFormat->Line->Columns[0]->Line(Printout->PrintFormat->Line->Columns[0]->Width - 1);
	Printout->PrintFormat->Line->Columns[1]->Text = Title;
	Printout->PrintFormat->Line->Columns[2]->Line(Printout->PrintFormat->Line->Columns[2]->Width - 1);
	Printout->PrintFormat->AddLine();
}

void TfrmAnalysis::PrintoutFormatForTxtValue(TPrintout *Printout)
{
	Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
	Printout->PrintFormat->Line->ColCount = 2;
	Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
	Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
	Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
	Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
}

void TfrmAnalysis::PrintFloatAdjustments(Database::TDBTransaction &DBTransaction, UnicodeString DeviceName)
{
	try
	{
		TDateTime PrevZedTime = Now();

		bool PrinterExists = true;

		if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
		{
			PrinterExists = false;
		}

		std::auto_ptr <TPrintout> Printout(new TPrintout);

		if (!PrinterExists)
		{
			TPrinterPhysical DefaultScreenPrinter;
			DefaultScreenPrinter.NormalCharPerLine = 40;
			DefaultScreenPrinter.BoldCharPerLine = 40;
			Printout->Printer = DefaultScreenPrinter;
		}
		else
		{
			Printout->Printer = TComms::Instance().ReceiptPrinter;
		}

		Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *FloatDetailsQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		FloatDetailsQuery->Close();

		FloatDetailsQuery->SQL->Text =
		"Select "
		"Transaction_Type, "
		"Amount, "
		"Staff, "
		"Terminal_Name, "
		"Time_Stamp, "
		"Z_Key, "
		"Reasons, "
        "IS_FLOAT_WITHDRAWN_FROM_CASH "
		"From "
		"Refloat_Skim "
		"Where "
		"Z_Key = :Z_Key "
		"Order By "
		"3";

		IBInternalQuery->SQL->Text =
		"Select "
		"Distinct Z_Key, "
		"Terminal_Name "
		"From "
		"Zeds "
		"Where "
		"Time_Stamp is Null;";
		IBInternalQuery->ExecQuery();

		std::vector<int> Z_Keys;
		std::vector<UnicodeString> DeviceNames;

		if (IBInternalQuery->RecordCount)
		{
			AddSectionTitle(Printout.get(), "Float Audit (by Staff member)");
			Printout->PrintFormat->Line->ColCount = 4;

			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->Line->Columns[1]->DoubleLine();
			Printout->PrintFormat->Line->Columns[2]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				Z_Keys.push_back(IBInternalQuery->FieldByName("Z_Key")->AsInteger);
				DeviceNames.push_back(IBInternalQuery->FieldByName("Terminal_Name")->AsString);
			}

			IBInternalQuery->Close();

			while (!Z_Keys.empty())
			{
				Float total = 0;

				FloatDetailsQuery->ParamByName("Z_Key")->AsInteger = Z_Keys.back();
				Z_Keys.pop_back();

				Printout->PrintFormat->Line->ColCount = 1;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				Printout->PrintFormat->Line->Columns[0]->Text = DeviceNames.back();
				Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
				Printout->PrintFormat->AddLine();

				DeviceNames.pop_back();

				Printout->PrintFormat->Line->ColCount = 4;
				Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 1 / 4;
				Printout->PrintFormat->Line->Columns[0]->Text = "Operator";
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width * 1 / 4;
				Printout->PrintFormat->Line->Columns[1]->Text = "Time";
				Printout->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width * 1 / 4;
				Printout->PrintFormat->Line->Columns[2]->Text = "Reason";
				Printout->PrintFormat->Line->Columns[2]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[3]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[2]->Width - Printout->PrintFormat->Line->Columns[1]->Width - Printout->PrintFormat->Line->Columns[0]->Width;
				Printout->PrintFormat->Line->Columns[3]->Text = "Total";
				Printout->PrintFormat->Line->Columns[3]->Alignment = taRightJustify;
				Printout->PrintFormat->AddLine();

				FloatDetailsQuery->ExecQuery();

				for (; !FloatDetailsQuery->Eof; FloatDetailsQuery->Next())
				{
                    if(!(FloatDetailsQuery->FieldByName("Transaction_Type")->AsString == "Withdrawal" && FloatDetailsQuery->FieldByName("IS_FLOAT_WITHDRAWN_FROM_CASH")->AsString == "T"))
                    {
                        const UnicodeString SetFloat = "Set Float";
                        UnicodeString TransType = FloatDetailsQuery->FieldByName("Transaction_Type")->AsString == "Initial" ? SetFloat : FloatDetailsQuery->FieldByName("Reasons")->AsString;;


                        Printout->PrintFormat->Line->Columns[0]->Text = FloatDetailsQuery->FieldByName("STAFF")->AsString;
                        Printout->PrintFormat->Line->Columns[1]->Text = FloatDetailsQuery->FieldByName("TIME_STAMP")->AsDateTime.FormatString("HH:MM:SS ");
                        Printout->PrintFormat->Line->Columns[2]->Text = TransType;
                        Printout->PrintFormat->Line->Columns[3]->Text = FloatToStrF(FloatDetailsQuery->FieldByName("AMOUNT")->AsFloat, ffNumber, 18, CurrencyDecimals);



                        total += FloatDetailsQuery->FieldByName("AMOUNT")->AsFloat;
                        Printout->PrintFormat->AddLine();
                    }
				}

				FloatDetailsQuery->Close();

				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 1 / 2;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width * 1 / 4;
				Printout->PrintFormat->Line->Columns[2]->Width = 0;
				Printout->PrintFormat->Line->Columns[3]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[2]->Width - Printout->PrintFormat->Line->Columns[1]->Width - Printout->PrintFormat->Line->Columns[0]->Width;

				Printout->PrintFormat->Line->Columns[0]->Text = "";
				Printout->PrintFormat->Line->Columns[1]->Text = "";
				Printout->PrintFormat->Line->Columns[2]->Text = "";
				Printout->PrintFormat->Line->Columns[3]->Line();
				Printout->PrintFormat->AddLine();

				Printout->PrintFormat->Line->Columns[0]->Text = "Float Balance: ";
				Printout->PrintFormat->Line->Columns[1]->Text = "";
				Printout->PrintFormat->Line->Columns[2]->Text = "";
				Printout->PrintFormat->Line->Columns[3]->Text = FloatToStrF(total,ffNumber, 18, CurrencyDecimals);

				Printout->PrintFormat->AddLine();

				Printout->PrintFormat->Line->Columns[0]->DoubleLine();
				Printout->PrintFormat->Line->Columns[1]->DoubleLine();
				Printout->PrintFormat->Line->Columns[2]->DoubleLine();
				Printout->PrintFormat->Line->Columns[3]->DoubleLine();
				Printout->PrintFormat->AddLine();

				Printout->PrintFormat->Line->Columns[0]->Text = "";
				Printout->PrintFormat->Line->Columns[1]->Text = "";
				Printout->PrintFormat->Line->Columns[2]->Text = "";
				Printout->PrintFormat->Line->Columns[3]->Text = "";
				Printout->PrintFormat->AddLine();
			}
		}

		if (IBInternalQuery->RecordCount)
		{
			Printout->PrintFormat->NewLine();
			AddSectionTitle(Printout.get(), "Float Audit (by Time)");
			Printout->PrintFormat->Line->ColCount = 4;

			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->Line->Columns[1]->DoubleLine();
			Printout->PrintFormat->Line->Columns[2]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				Z_Keys.push_back(IBInternalQuery->FieldByName("Z_Key")->AsInteger);
				DeviceNames.push_back(IBInternalQuery->FieldByName("Terminal_Name")->AsString);
			}

			IBInternalQuery->Close();

			while (!Z_Keys.empty())
			{
				Float total = 0;
				FloatDetailsQuery->ParamByName("Z_Key")->AsInteger = Z_Keys.back();
				Z_Keys.pop_back();

				Printout->PrintFormat->Line->ColCount = 1;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				Printout->PrintFormat->Line->Columns[0]->Text = DeviceNames.back();
				Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
				Printout->PrintFormat->AddLine();

				DeviceNames.pop_back();

				Printout->PrintFormat->Line->ColCount = 2;
				Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 1 / 2;
				Printout->PrintFormat->Line->Columns[0]->Text = "Time";
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width;
				Printout->PrintFormat->Line->Columns[1]->Text = "Amount";
				Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
				Printout->PrintFormat->AddLine();

				FloatDetailsQuery->ExecQuery();

				for (; !FloatDetailsQuery->Eof; FloatDetailsQuery->Next())
				{
					Printout->PrintFormat->Line->Columns[0]->Text = FloatDetailsQuery->FieldByName("TIME_STAMP")->AsDateTime.FormatString("HH:MM:SS dd/mm/yyyy");
					Printout->PrintFormat->Line->Columns[1]->Text = FloatToStrF(FloatDetailsQuery->FieldByName("AMOUNT")->AsFloat, ffCurrency, 18, CurrencyDecimals);

					total += FloatDetailsQuery->FieldByName("AMOUNT")->AsFloat;
					Printout->PrintFormat->AddLine();
				}

				FloatDetailsQuery->Close();
				Printout->PrintFormat->Line->Columns[0]->Text = "";
				Printout->PrintFormat->Line->Columns[1]->Line();
				Printout->PrintFormat->AddLine();

				Printout->PrintFormat->Line->Columns[0]->Text = "Float Balance: ";
				Printout->PrintFormat->Line->Columns[1]->Text = FloatToStrF(total, ffNumber, 18, CurrencyDecimals);
				Printout->PrintFormat->AddLine();

				Printout->PrintFormat->Line->Columns[0]->DoubleLine();
				Printout->PrintFormat->Line->Columns[1]->DoubleLine();
				Printout->PrintFormat->AddLine();

				Printout->PrintFormat->Line->Columns[0]->Text = "";
				Printout->PrintFormat->Line->Columns[1]->Text = "";
				Printout->PrintFormat->AddLine();
			}
		}

		Printout->PrintFormat->PartialCut();

		std::auto_ptr <TfrmShowPrintout> (frmShowPrintout)(TfrmShowPrintout::Create <TfrmShowPrintout> (this));
		Printout->PrintToStream(frmShowPrintout->CurrentPrintout.get());

		frmShowPrintout->btnCancel->Caption = "Close";
		frmShowPrintout->btnClose->Visible = false;
		frmShowPrintout->btnClosePrint->Caption = "Print";
		frmShowPrintout->Execute();
		ExitCode = frmShowPrintout->ExitCode;

		UnicodeString Dir = ExtractFilePath(Application->ExeName) + ZDIR;
		if (!DirectoryExists(Dir))
		{
			CreateDir(Dir);
		}
		Printout->PrintToFile(Dir + "\\" + Now().FormatString("yyyy-mm-dd - hh-nn-ss") + "-" + DeviceName + LOGFILE);
		Printout->PrintToStream(ZedToArchive);

	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TfrmAnalysis::PrintBlindBalance(Database::TDBTransaction &DBTransaction, TBlindBalances Balances, AnsiString DeviceName)
{
	try
	{
        zedLogsList->Add("Blind Balance printing section starts ");
		std::auto_ptr <TPrintout> Printout(new TPrintout);
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
		{
			TPrinterPhysical DefaultScreenPrinter;
			DefaultScreenPrinter.NormalCharPerLine = 40;
			DefaultScreenPrinter.BoldCharPerLine = 40;
			Printout->Printer = DefaultScreenPrinter;
		}
		else
		{
			Printout->Printer = TComms::Instance().ReceiptPrinter;
		}



		Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;

		AddSectionTitle(Printout.get(), "Blind Balance");
		Printout->PrintFormat->Line->ColCount = 4;

		Printout->PrintFormat->Line->Columns[0]->DoubleLine();
		Printout->PrintFormat->Line->Columns[1]->DoubleLine();
		Printout->PrintFormat->Line->Columns[2]->DoubleLine();
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->NewLine();

		Printout->PrintFormat->Line->ColCount = 1;
		Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
		Printout->PrintFormat->Line->Columns[0]->Text = "";//DeviceNames.back();
		Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
		Printout->PrintFormat->AddLine();

		Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

		Printout->PrintFormat->Line->ColCount = 3;
		Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 4/10;
		Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
		Printout->PrintFormat->Line->Columns[0]->Text =  "Payment Type";
		Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width / 3;
		Printout->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
		Printout->PrintFormat->Line->Columns[1]->Text = "Blind Balance $";
		Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width;
		Printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
		Printout->PrintFormat->Line->Columns[2]->Text = "Variance $";
		Printout->PrintFormat->AddLine();

		TBlindBalanceContainer::iterator itBlindBalances = Balances.begin();
		for (itBlindBalances = Balances.begin(); itBlindBalances != Balances.end(); itBlindBalances++)
		{

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"select sum(dabp.subtotal) total "
			"       from dayarcbillpay dabp "
			"            left join dayarcbill dab on "
			"                 dabp.arcbill_key = dab.arcbill_key "
			"       where dabp.pay_type = :pay_type "
			"             and dab.terminal_name = :terminal_name "
			"       		  group by dabp.pay_type;";
			IBInternalQuery->ParamByName("terminal_name")->AsString = DeviceName;
			IBInternalQuery->ParamByName("pay_type")->AsString =
			itBlindBalances->first;
			IBInternalQuery->ExecQuery();

			Printout->PrintFormat->Line->Columns[0]->Text =
			itBlindBalances->first;
			Printout->PrintFormat->Line->Columns[1]->Text =
			FormatFloat("0.00", itBlindBalances->second.BlindBalance);

			itBlindBalances->second.SystemBalance =
			IBInternalQuery->FieldByName("total")->AsCurrency;
			double temp = itBlindBalances->second.BlindBalance
			- itBlindBalances->second.SystemBalance;

			Printout->PrintFormat->Line->Columns[2]->Text =
			FormatFloat("0.00", temp);
			Printout->PrintFormat->AddLine();
		}

		Printout->PrintFormat->PartialCut();

		std::auto_ptr <TfrmShowPrintout> (frmShowPrintout)(TfrmShowPrintout::Create <TfrmShowPrintout> (this));
		Printout->PrintToStream(frmShowPrintout->CurrentPrintout.get());

		frmShowPrintout->btnCancel->Caption = "Close";
		frmShowPrintout->btnClose->Visible = false;
		frmShowPrintout->btnClosePrint->Caption = "Print";
		frmShowPrintout->Execute();
		ExitCode = frmShowPrintout->ExitCode;

		UnicodeString Dir = ExtractFilePath(Application->ExeName) + ZDIR;
		if (!DirectoryExists(Dir))
		{
			CreateDir(Dir);
		}
		Printout->PrintToFile(Dir + "\\" + Now().FormatString("yyyy-mm-dd - hh-nn-ss") + "-" + DeviceName + LOGFILE);
		Printout->PrintToStream(ZedToArchive);
	}
	catch(Exception & E)
	{
        zedLogsList->Add("Blind Balance printing catch block ");
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
    zedLogsList->Add("Blind Balance printing section ends ");
}

void TfrmAnalysis::PrintWriteOff(Database::TDBTransaction &DBTransaction, UnicodeString DeviceName)
{
	try
	{

		TDateTime PrevZedTime = Now();

		bool PrinterExists = true;

		if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
		{
			PrinterExists = false;
		}

		std::auto_ptr <TPrintout> Printout(new TPrintout);

		if (!PrinterExists)
		{
			TPrinterPhysical DefaultScreenPrinter;
			DefaultScreenPrinter.NormalCharPerLine = 40;
			DefaultScreenPrinter.BoldCharPerLine = 40;
			Printout->Printer = DefaultScreenPrinter;
		}
		else
		{
			Printout->Printer = TComms::Instance().ReceiptPrinter;
		}

		Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;


		TIBSQL *IBTerminalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TDateTime ZedOffTime;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT max(TIME_STAMP) FROM ZEDS;";

		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			ZedOffTime = IBInternalQuery->FieldByName("MAX")->AsDateTime;
		}

		std::vector<UnicodeString> DeviceNames;


		IBTerminalQuery->Close();
		IBTerminalQuery->SQL->Text =
		"SELECT DISTINCT TERMINALNAME FROM WRITEOFF "
		"WHERE TIME_STAMP >= :TIME_STAMP;";
		IBTerminalQuery->ParamByName("TIME_STAMP")->AsDateTime = ZedOffTime;
		IBTerminalQuery->ExecQuery();



		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT * FROM WRITEOFF "
		"WHERE TERMINALNAME = :TERMINALNAME;";



		for (; !IBTerminalQuery->Eof; IBTerminalQuery->Next())
		{
			UnicodeString et = IBTerminalQuery->FieldByName("TERMINALNAME")->AsString;
			DeviceNames.push_back(IBTerminalQuery->FieldByName("TERMINALNAME")->AsString);
		}

		IBTerminalQuery->Close();


		Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		Printout->PrintFormat->Line->ColCount = 1;
		Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
		Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;


		AddSectionTitle(Printout.get(), "Write Off Audit");
		Printout->PrintFormat->Line->ColCount = 4;
		Printout->PrintFormat->Line->Columns[0]->DoubleLine();
		Printout->PrintFormat->Line->Columns[1]->DoubleLine();
		Printout->PrintFormat->Line->Columns[2]->DoubleLine();
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->NewLine();

		while (!DeviceNames.empty())
		{
			double TotalWriteOff = 0;
			IBInternalQuery->ParamByName("TERMINALNAME")->AsString = DeviceNames.back();
			IBInternalQuery->ExecQuery();

			Printout->PrintFormat->Line->ColCount = 1;
			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
			Printout->PrintFormat->Line->Columns[0]->Text = DeviceNames.back();
			Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->Line->Columns[0]->Text = "";

			DeviceNames.pop_back();

			Printout->PrintFormat->Line->ColCount = 3;
			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 9/32;
			Printout->PrintFormat->Line->Columns[0]->Text = "Write Off#";
			Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
			Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width *2/4;
			Printout->PrintFormat->Line->Columns[1]->Text = "Item";
			Printout->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
			Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width; //- Printout->PrintFormat->Line->Columns[3]->Width;
			Printout->PrintFormat->Line->Columns[2]->Text = "Value";
			Printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
			Printout->PrintFormat->AddLine();

			for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{

				Printout->PrintFormat->Line->Columns[0]->Text = IBInternalQuery->FieldByName("WRITEOFF_KEY")->AsString;
				Printout->PrintFormat->Line->Columns[1]->Text = IBInternalQuery->FieldByName("ITEM")->AsString;
				Printout->PrintFormat->Line->Columns[2]->Text = FloatToStrF(IBInternalQuery->FieldByName("AMOUNT")->AsFloat, ffCurrency, 18,
				CurrencyDecimals);
				TotalWriteOff += IBInternalQuery->FieldByName("AMOUNT")->AsFloat;
				Printout->PrintFormat->AddLine();
			}

			IBInternalQuery->Close();
			Printout->PrintFormat->Line->Columns[0]->Text = "";
			Printout->PrintFormat->Line->Columns[1]->Text = "";
			Printout->PrintFormat->Line->Columns[2]->Line();
			Printout->PrintFormat->AddLine();

			Printout->PrintFormat->Line->Columns[0]->Text = "Total:";
			Printout->PrintFormat->Line->Columns[1]->Text = "";
			Printout->PrintFormat->Line->Columns[2]->Text = FloatToStrF(TotalWriteOff, ffCurrency, 18, CurrencyDecimals);
			Printout->PrintFormat->AddLine();

			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->Line->Columns[1]->DoubleLine();
			Printout->PrintFormat->Line->Columns[2]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			Printout->PrintFormat->Line->Columns[0]->Text = "";
			Printout->PrintFormat->Line->Columns[2]->Text = "";
		}

		IBTerminalQuery->ExecQuery();

		for (; !IBTerminalQuery->Eof; IBTerminalQuery->Next())
		{
			UnicodeString et = IBTerminalQuery->FieldByName("TERMINALNAME")->AsString;
			DeviceNames.push_back(IBTerminalQuery->FieldByName("TERMINALNAME")->AsString);
		}

		IBTerminalQuery->Close();

		Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		Printout->PrintFormat->Line->ColCount = 1;
		Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
		Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;

		AddSectionTitle(Printout.get(), "Write Off Audit");
		Printout->PrintFormat->Line->ColCount = 4;
		Printout->PrintFormat->Line->Columns[0]->DoubleLine();
		Printout->PrintFormat->Line->Columns[1]->DoubleLine();
		Printout->PrintFormat->Line->Columns[2]->DoubleLine();
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->NewLine();

		while (!DeviceNames.empty())
		{
			IBInternalQuery->ParamByName("TERMINALNAME")->AsString = DeviceNames.back();
			IBInternalQuery->ExecQuery();

			Printout->PrintFormat->Line->ColCount = 1;
			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
			Printout->PrintFormat->Line->Columns[0]->Text = DeviceNames.back();
			Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->Line->Columns[0]->Text = "";

			DeviceNames.pop_back();

			Printout->PrintFormat->Line->ColCount = 3;
			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 9/32;
			Printout->PrintFormat->Line->Columns[0]->Text = "Write Off#";
			Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
			Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width *10/32;
			Printout->PrintFormat->Line->Columns[1]->Text = "Operator";
			Printout->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
			Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width; //- Printout->PrintFormat->Line->Columns[3]->Width;
			Printout->PrintFormat->Line->Columns[2]->Text = "Reason";
			Printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
			Printout->PrintFormat->AddLine();

			for(; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{

				Printout->PrintFormat->Line->Columns[0]->Text = IBInternalQuery->FieldByName("WRITEOFF_KEY")->AsString;
				Printout->PrintFormat->Line->Columns[1]->Text = IBInternalQuery->FieldByName("STAFF")->AsString;
				Printout->PrintFormat->Line->Columns[2]->Text = IBInternalQuery->FieldByName("REASONS")->AsString;
				Printout->PrintFormat->AddLine();
			}

			IBInternalQuery->Close();

			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->Line->Columns[1]->DoubleLine();
			Printout->PrintFormat->Line->Columns[2]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			Printout->PrintFormat->Line->Columns[0]->Text = "";
			Printout->PrintFormat->Line->Columns[2]->Text = "";
		}



		Printout->PrintFormat->PartialCut();

		std::auto_ptr <TfrmShowPrintout> (frmShowPrintout)(TfrmShowPrintout::Create <TfrmShowPrintout> (this));
		Printout->PrintToStream(frmShowPrintout->CurrentPrintout.get());

		if (TGlobalSettings::Instance().EnableBlindBalances)
		{
			frmShowPrintout->btnCancel->Visible = false;
		}
		else
		{
			frmShowPrintout->btnCancel->Visible = true;
			frmShowPrintout->btnCancel->Caption = "Cancel Zed";
		}
		frmShowPrintout->btnClose->Caption = "Close Till";
		frmShowPrintout->btnClosePrint->Caption = "Close Till and Print Zed";

		{
			frmShowPrintout->btnCancel->Caption = "Close";
			frmShowPrintout->btnClose->Visible = false;
			frmShowPrintout->btnClosePrint->Caption = "Print";
		}
		frmShowPrintout->Execute();
		ExitCode = frmShowPrintout->ExitCode;

		UnicodeString Dir = ExtractFilePath(Application->ExeName) + ZDIR;
		if (!DirectoryExists(Dir))
		{
			CreateDir(Dir);
		}
		Printout->PrintToFile(Dir + "\\" + Now().FormatString("yyyy-mm-dd - hh-nn-ss") + "-" + DeviceName + LOGFILE);
		Printout->PrintToStream(ZedToArchive);

	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

#define SetSiteSummaryFormatting() { \
		pfmt->Line->ColCount = 2; \
		pfmt->Line->Columns[0]->Width = \
		((pfmt->NormalWidth >> 1) + \
		(pfmt->Line->Columns[1]->Width = pfmt->NormalWidth >> 2)); \
		pfmt->Line->Columns[0]->Alignment = taLeftJustify; \
		pfmt->Line->Columns[1]->Alignment = taRightJustify; \
	}

void TfrmAnalysis::SiteSummaryReport(Database::TDBTransaction &transaction, TPrintout *pout, TDateTime last_zedded)
{
    try
    {
        static const AnsiString subsection_name[2] = {
            "Summary by POS Terminal",
            "Summary by Item Category"
        };

        static const AnsiString query_str(
        "select aggregated_archive.pos_terminal, "
        "       cg.name category_name, "
        "       aggregated_archive.category_total "
        "from (select terminal_name pos_terminal, "
        "             category_key, "
        "             sum((price *qty)) category_total "
        "      from archive where time_stamp >= :base_time "
        "                   group by pos_terminal, "
        "                            category_key "
        "      union "
        "      select terminal_name pos_terminal, "
        "             category_key, "
        "             sum((price * qty)) category_total "
        "      from dayarchive where time_stamp >= :base_time "
        "                      group by pos_terminal, "
        "                               category_key) aggregated_archive "
        "inner join arccategories ac "
        "      on ac.category_key = aggregated_archive.category_key "
        "inner join categorygroups cg "
        "      on cg.categorygroups_key = ac.categorygroups_key;");

        TPrintFormat *pfmt = pout->PrintFormat;
        float grand_total = 0;
        TDateTime fromDateTime;
        std::map<AnsiString, float> totals[2];
        std::map<AnsiString, float>::const_iterator walker;
        TIBSQL *query = transaction.Query(transaction.AddQuery());
        unsigned short year;
        unsigned short month;
        unsigned short day;
        unsigned short hour;
        unsigned short minute;
        unsigned short dummy;

        query->SQL->Text = query_str;

        /*
        * This chunk of code extracts the individual date components
            * and performs bitshifting as to translate the current time
            * into a single integer value that can be quickly and
        * efficiently compared and in a way that takes account for
        * the case when the time on the reporting POS is less than
            * 5am of a new day. The bit operation results to one if it
        * is earlier than 5am the current day, this result is
            * subtracted from the current datetime which reduces the
            * date by one - sourcing the data from the previous day.
        */

        Now().DecodeDate(&year, &month, &day);
        Now().DecodeTime(&hour, &minute, &dummy, &dummy);

        /* An example:
            * fromDateTime { y: 2011, m: 09, d:23, h:5, m:0 }
        * hour = 02, minute = 43.
        *
        * We want to translate the time into a single 16bit
        * value. We can achieve this by combining the two
        * components. We know how many bits each component
        * requires by the limits of X hours in a day and Y
        * minutes in an hour - 24 and 60 respectively but
        * practically, in this case, the limit for minutes
        * is 59. The number 24 requires only 5bits, the
        * number 54 requires 6 (2^5 = 32, 2^6 = 64).
        *
        * A worked example with the above values:
        * Hour 2, 0000_0000_0000_0010, shifted by 6.
        *         0000_0000_1000_0000.
        *               '----' hour encoded.
        * Minute 43, 0000_0000_0010_1011
        *                        '-----' minute encoded.
        * Combined:
        *            0000_0000_1010_1011 = 0xAB (171 dec).
        *
        * 5am is 0000_0001_0100_0000 = 0x140 (320 dec).
        */
        fromDateTime = TDateTime(year, month, day, 5, 0, 0, 0);
        fromDateTime -= ((hour << 6) | minute) < 0x140;

        query->ParamByName("base_time")->AsDateTime = fromDateTime;
        query->ExecQuery();

        /* We write and display the totals via table indices as it
        avoids an if statement the loop body. Branches are expensive. */
        for (float i; !query->Eof; grand_total += i, query->Next()) {
            i = query->FieldByName("category_total")->AsFloat;
            totals[0][
            query->FieldByName("pos_terminal")->AsString] += i;
            totals[1][
            query->FieldByName("category_name")->AsString] += i;
        }
        query->Close();

        /* Defined simply to shorten the formatting expression as to fit within the
    80 margin limit. Keeps things readable. */
    #define FriendlyFormatTime(date_time) \
            ((date_time).FormatString("yyyy/mm/dd h:mm am/pm"))

        AddSectionTitle(pout, "Site Summary");
        pfmt->Line->ColCount = 1;
        pfmt->Line->Columns[0]->Alignment = taCenter;
        pfmt->Line->Columns[0]->Width = pfmt->NormalWidth;
        pfmt->Line->Columns[0]->Text =
        FriendlyFormatTime(query->ParamByName("base_time")->AsDateTime) +
        " to " + FriendlyFormatTime(TDateTime::CurrentDateTime());
        pfmt->AddLine();

    #undef FriendlyFormatTime

        for (int i = 0; i < 2; i++) {
            AddSubSectionTitle(pout, subsection_name[i]);
            SetSiteSummaryFormatting();
            for (walker = totals[i].begin();
            walker != totals[i].end(); pfmt->AddLine(), walker++) {
                pfmt->Line->Columns[0]->Text = "  " + walker->first;
                pfmt->Line->Columns[1]->Text =
                FormatFloat("0.00", walker->second);
            }
            pfmt->Line->Columns[0]->Text = "";
            pfmt->Line->Columns[1]->Line();
            pfmt->AddLine();
            pfmt->Line->Columns[0]->Text = "  Total";
            pfmt->Line->Columns[1]->Text = FormatFloat("0.00",
            grand_total);
            pfmt->AddLine();
        }
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

#undef SetSiteSummaryFormatting

void TfrmAnalysis::PaxCountReport(Database::TDBTransaction &transaction, TPrintout *Printout, TPaxCount &PaxCount, Currency &Total, TStaffHoursInterface &inStaffHours, TCommissionCache &inCommission, TPrinterReadingsInterface &inPrinterReadings)
{
	int days = (int)(PaxCount.GetDateTo() - PaxCount.GetDateFrom());
	AddSectionTitle(Printout, " Pax Count");
	Printout->PrintFormat->NewLine();
	PrintoutFormatForTxtValue(Printout);
	Printout->PrintFormat->Line->Columns[0]->Text = "Photo Packs Sold";
	Printout->PrintFormat->Line->Columns[1]->Text = (AnsiString)PaxCount.GetPhotoPacksSold();
	Printout->PrintFormat->AddLine();
	Printout->PrintFormat->Line->Columns[0]->Text = "Prints Sold";
	Printout->PrintFormat->Line->Columns[1]->Text = (AnsiString)PaxCount.GetPrintsSold();
	Printout->PrintFormat->AddLine();
	Printout->PrintFormat->Line->Columns[0]->Text = "Products Sold";
	Printout->PrintFormat->Line->Columns[1]->Text = (AnsiString)PaxCount.GetProductsSold();
	Printout->PrintFormat->AddLine();
	Printout->PrintFormat->Line->Columns[0]->Text = "Transaction Count";
	Printout->PrintFormat->Line->Columns[1]->Text = (AnsiString)PaxCount.GetTransactionCount();
	Printout->PrintFormat->AddLine();
	Printout->PrintFormat->Line->Columns[0]->Text = "Date From";
	Printout->PrintFormat->Line->Columns[1]->Text = PaxCount.GetDateFrom().FormatString("dd/mm/yy");
	Printout->PrintFormat->AddLine();
	Printout->PrintFormat->Line->Columns[0]->Text = "Date To";
	Printout->PrintFormat->Line->Columns[1]->Text = PaxCount.GetDateTo().FormatString("dd/mm/yy");
	Printout->PrintFormat->AddLine();
	Printout->PrintFormat->Line->Columns[0]->Text = "Pax Number";
	Printout->PrintFormat->Line->Columns[1]->Text = (AnsiString)PaxCount.GetPaxNumber();
	Printout->PrintFormat->AddLine();
	Printout->PrintFormat->Line->Columns[0]->Text = "";
	Printout->PrintFormat->Line->Columns[1]->Text = "";
	Printout->PrintFormat->AddLine();

	if(!inPrinterReadings.Empty() && inPrinterReadings.TotalCopies())
	{
		Printout->PrintFormat->Line->Columns[0]->Text = "Spend Per Print";
		Printout->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", Total/inPrinterReadings.TotalCopies());
		Printout->PrintFormat->AddLine();

		Printout->PrintFormat->Line->Columns[0]->Text = "Hit Rate";
		Printout->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", PaxCount.GetProductsSold()/inPrinterReadings.TotalCopies());
		Printout->PrintFormat->AddLine();
	}

	if(PaxCount.GetTransactionCount())
	{
		Printout->PrintFormat->Line->Columns[0]->Text = "Average Sale Value";
		Printout->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", Total/PaxCount.GetTransactionCount());
		Printout->PrintFormat->AddLine();
	}

	if(!inStaffHours.Empty())
	{
		double wages = 0;
		double commission = 0;
		TStaffHoursContainer::iterator itStaffHours = inStaffHours.begin();
		for (; itStaffHours != inStaffHours.end(); itStaffHours++)
		{
			if(!itStaffHours->second.GetLogOutNull())
			wages = wages + (itStaffHours->second.GetHoursWorked() * itStaffHours->second.GetHourlyRate());
		}

		/*			TCommissionContainer::iterator itCommission = inCommission.begin();
			for (; itCommission != inCommission.end(); itCommission++)
			{
				commission = commission + (itCommission->second.GetCommission() /
													((int)itCommission->second.GetDateTo() - (int)itCommission->second.GetDateFrom()));
			}
*/
		wages = wages + commission;
		Printout->PrintFormat->Line->Columns[0]->Text = "Wage %";
		if((double)Total)
		Printout->PrintFormat->Line->Columns[1]->Text = FormatFloat("%0.00", 100 * wages / Total);
		Printout->PrintFormat->AddLine();
	}

	if(PaxCount.GetPaxNumber())
	{
		Printout->PrintFormat->Line->Columns[0]->Text = "Capture Rate";
		Printout->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", (double)inPrinterReadings.TotalCopies()/PaxCount.GetPaxNumber());
		Printout->PrintFormat->AddLine();

		Printout->PrintFormat->Line->Columns[0]->Text = "Spend per Head";
		Printout->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", Total/PaxCount.GetPaxNumber());
		Printout->PrintFormat->AddLine();
	}

}

void TfrmAnalysis::WriteBlindBalance(Database::TDBTransaction &DBTransaction, TPrintout *Printout, AnsiString Title, TBlindBalances Balances, AnsiString DeviceName, bool t)
{
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        AddSectionTitle(Printout, Title);
        Printout->PrintFormat->NewLine();
        PrintoutFormatForTxtValue(Printout);
        Printout->PrintFormat->Line->ColCount = 3;
        Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 4/10;
        Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        Printout->PrintFormat->Line->Columns[0]->Text =  "Payment Type";
        Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width / 3;
        Printout->PrintFormat->Line->Columns[1]->Alignment = taLeftJustify;
        Printout->PrintFormat->Line->Columns[1]->Text = "Blind Balance $";
        Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]->Width - Printout->PrintFormat->Line->Columns[1]->Width;
        Printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;
        Printout->PrintFormat->Line->Columns[2]->Text = "Variance $";
        Printout->PrintFormat->AddLine();
        TBlindBalanceContainer::iterator itBlindBalances = Balances.begin();
        for (itBlindBalances = Balances.begin(); itBlindBalances != Balances.end(); itBlindBalances++)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "select sum(dabp.subtotal) total "
            "       from dayarcbillpay dabp "
            "            left join dayarcbill dab on "
            "                 dabp.arcbill_key = dab.arcbill_key "
            "       where dabp.pay_type = :pay_type ";
            if (!TGlobalSettings::Instance().EnableDepositBagNum || t)
            {
                IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                "             and dab.terminal_name = :terminal_name "
                "       		  group by dabp.pay_type;";
                IBInternalQuery->ParamByName("terminal_name")->AsString = DeviceName;
            }
            else
            {
                IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
                "       group by dabp.pay_type;";
            }
            Printout->PrintFormat->Line->Columns[0]->Text =
            itBlindBalances->first;
            Printout->PrintFormat->Line->Columns[1]->Text =
            FormatFloat("0.00", itBlindBalances->second.BlindBalance);

            IBInternalQuery->ParamByName("pay_type")->AsString =
            itBlindBalances->first;
            IBInternalQuery->ExecQuery();

            itBlindBalances->second.SystemBalance =
            IBInternalQuery->FieldByName("total")->AsCurrency;
            double temp = itBlindBalances->second.BlindBalance
            - itBlindBalances->second.SystemBalance;

            Printout->PrintFormat->Line->Columns[2]->Text =
            FormatFloat("0.00", temp);
            Printout->PrintFormat->AddLine();

            IBInternalQuery->Close();
        }
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TfrmAnalysis::PrintConsumption(Database::TDBTransaction &DBTransaction)
{
	UnicodeString DeviceName = GetTerminalName();

	bool PrinterExists = true;

	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		PrinterExists = false;
	}

	std::auto_ptr <TPrintout> Printout(new TPrintout);

	if (!PrinterExists)
	{
		TPrinterPhysical DefaultScreenPrinter;
		DefaultScreenPrinter.NormalCharPerLine = 40;
		DefaultScreenPrinter.BoldCharPerLine = 40;
		Printout->Printer = DefaultScreenPrinter;
	}
	else
	{
		Printout->Printer = TComms::Instance().ReceiptPrinter;
	}

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		TDateTime StartTime = Date() + EncodeTime(5, 0, 0, 0); // Today 5am.
		TDateTime EndTime = (Date() + 1) + EncodeTime(5, 0, 0, 0); // Tommorow 5am.

		if (Now() < StartTime) // We are between Midnight and 5am
		{
			StartTime = (Date() - 1) + EncodeTime(5, 0, 0, 0); // Yesterday 5am.
			EndTime = Date() + EncodeTime(5, 0, 0, 0); // Today 5am.
		}

		Printout->PrintFormat->DocumentName = "Consumption Report";

		Printout->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
		Printout->PrintFormat->Line->ColCount = 1;
		Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
		Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
		Printout->PrintFormat->Line->Columns[0]->Text = "Consumption Summary Report";
		Printout->PrintFormat->AddLine();

		Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
		Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
		Printout->PrintFormat->NewLine();

		Printout->PrintFormat->Line->Columns[0]->Text = "Report Covers ALL Tills";
		Printout->PrintFormat->AddLine();

		Printout->PrintFormat->Line->Columns[0]->Text = "From " + StartTime.FormatString("dd/mmm h:nn:ss am/pm")
		+ " To " + EndTime.FormatString("dd/mmm h:nn:ss am/pm");
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text = "Printed At " + Now().FormatString("dd/mmm h:nn:ss am/pm");
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text = DeviceName;
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->Line->Columns[0]->Text = lastAuthenticatedUser.Name;
		Printout->PrintFormat->AddLine();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select " "ArcCategories.Category Group_Name, " "CategoryGroups.Name Course_Name, "
		"Cast(Archive.Item_Name as VarChar(50)) Item_Name, " "Archive.Size_Name, " "Sum(Archive.Qty) Item_Count, "
		"Cast(Sum((Archive.Price* Archive.Qty)) as Numeric(15,4)) Price, "
		"Cast(Sum((Archive.Cost * Archive.Qty) * (Archive.Cost_GST_Percent + 100) / 100) as Numeric(15,4)) Cost " "From "
		"Security Left Join Archive on " "Security.Security_Ref = Archive.Security_Ref "
		"Left Join ArcCategories on " "Archive.Category_Key = ArcCategories.Category_Key " "Left Join CategoryGroups on "
		"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key " "Where " "(Archive.Order_Type = 3 or "
		"Archive.Order_Type = 0) and " "Security.Time_Stamp >= :StartTime and " "Security.Time_Stamp <= :EndTime and "
		"Security.Security_Event = 'Ordered By' " "Group By " "ArcCategories.Category," "CategoryGroups.Name," "Archive.Item_Name,"
		"Archive.Size_Name " "Having " "Count(Archive.Archive_Key) > 0 "

		"Union All "

		"Select " "ArcCategories.Category Group_Name, " "CategoryGroups.Name Course_Name, "
		"Cast(DayArchive.Item_Name as VarChar(50)) Item_Name, " "DayArchive.Size_Name, " "Sum(DayArchive.Qty) Item_Count, "
		"Cast(Sum((DayArchive.Price * DayArchive.Qty )) as Numeric(15,4)) Price, "
		"Cast(Sum((DayArchive.Cost * DayArchive.Qty) * (DayArchive.Cost_GST_Percent + 100) / 100) as Numeric(15,4)) Cost " "From "
		"Security Left Join DayArchive on " "Security.Security_Ref = DayArchive.Security_Ref " "Left Join ArcCategories on "
		"DayArchive.Category_Key = ArcCategories.Category_Key " "Left Join CategoryGroups on "
		"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key " "Where " "(DayArchive.Order_Type = 3 or "
		"DayArchive.Order_Type = 0) and " "Security.Time_Stamp >= :StartTime and " "Security.Time_Stamp <= :EndTime and "
		"Security.Security_Event = 'Ordered By' " "Group By " "ArcCategories.Category, " "CategoryGroups.Name, " "DayArchive.Item_Name, "
		"DayArchive.Size_Name " "Having " "Count(DayArchive.Archive_Key) > 0 "

		"Union All "

		"Select " "ArcCategories.Category Group_Name, " "CategoryGroups.Name Course_Name, " "Orders.Item_Name, " "Orders.Size_Name, "
		"Sum(Orders.Qty) Item_Count, " "Cast(Sum((Orders.Zed_Price * Orders.Qty))  as Numeric(15,4)) Price,"
		"Cast(Sum((Orders.Cost * Orders.Qty) * (Orders.Cost_GST_Percent + 100) / 100) as Numeric(15,4)) Cost " "From " "Security Left Join Orders on "
		"Security.Security_Ref = Orders.Security_Ref " "Left Join ArcCategories on " "Orders.Category_Key = ArcCategories.Category_Key "
		"Left Join CategoryGroups on " "ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key " "Where "
		"(Orders.Order_Type = 3 or " "Orders.Order_Type = 0) and " "Security.Time_Stamp >= :StartTime and "
		"Security.Time_Stamp <= :EndTime and " "Security.Security_Event = 'Ordered By' " "Group By " "ArcCategories.Category, "
		"CategoryGroups.Name, " "Orders.Item_Name, " "Orders.Size_Name " "Having " "Count(Orders.Order_Key) > 0 " "Order By " "1,2,3,4";

		IBInternalQuery->ParamByName("StartTime")->AsDateTime = StartTime;
		IBInternalQuery->ParamByName("EndTime")->AsDateTime = EndTime;
		IBInternalQuery->ExecQuery();

		TCategoryGroups CategoryGroup;
		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			TCategoryGroupDetails CategoryGroupDetails = CategoryGroup[IBInternalQuery->FieldByName("GROUP_NAME")->AsString];

			TCatTotal ItemTotals = CategoryGroupDetails.Details[IBInternalQuery->FieldByName("COURSE_NAME")->AsString];

			TBaseTotal Details;
			if (UpperCase(IBInternalQuery->FieldByName("SIZE_NAME")->AsString) == "DEFAULT")
			{
				Details = ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString];
			}
			else
			{
				Details = ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString + " " + IBInternalQuery->FieldByName
				("SIZE_NAME")->AsString];
			}

			Details.Qty += IBInternalQuery->FieldByName("ITEM_COUNT")->AsFloat;
			Details.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			Details.Total += IBInternalQuery->FieldByName("PRICE")->AsCurrency;

			if (UpperCase(IBInternalQuery->FieldByName("SIZE_NAME")->AsString) == "DEFAULT")
			{
				ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString] = Details;
			}
			else
			{
				ItemTotals.ItemDetails[IBInternalQuery->FieldByName("ITEM_NAME")->AsString + " " + IBInternalQuery->FieldByName("SIZE_NAME")
				->AsString] = Details;
			}

			ItemTotals.Totals.Qty += IBInternalQuery->FieldByName("ITEM_COUNT")->AsFloat;
			ItemTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			ItemTotals.Totals.Total += IBInternalQuery->FieldByName("PRICE")->AsCurrency;

			CategoryGroupDetails.Details[IBInternalQuery->FieldByName("COURSE_NAME")->AsString] = ItemTotals;
			CategoryGroupDetails.Totals.Qty += IBInternalQuery->FieldByName("ITEM_COUNT")->AsFloat;
			CategoryGroupDetails.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			CategoryGroupDetails.Totals.Total += IBInternalQuery->FieldByName("PRICE")->AsCurrency;

			CategoryGroup[IBInternalQuery->FieldByName("GROUP_NAME")->AsString] = CategoryGroupDetails;
		}

		if (CategoryGroup.size() > 0)
		{
			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
			Printout->PrintFormat->Line->ColCount = 1;
			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
			Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
			Printout->PrintFormat->Line->Columns[0]->DoubleLine();
			Printout->PrintFormat->AddLine();
			Printout->PrintFormat->NewLine();

			AddSectionTitle(Printout.get(), "Consumption Report for Zed Period");

			Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 3 / 5;
			Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
			Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width * 1 / 5;
			Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
			Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]
			->Width - Printout->PrintFormat->Line->Columns[1]->Width;
			Printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

			Printout->PrintFormat->Line->Columns[0]->Text = "Group / Category / Item";
			Printout->PrintFormat->Line->Columns[1]->Text = "Qty";
			Printout->PrintFormat->Line->Columns[2]->Text = "Total";
			Printout->PrintFormat->AddLine();

			Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

			TCategoryGroups::iterator itCatGroups;
			for (itCatGroups = CategoryGroup.begin(); itCatGroups != CategoryGroup.end(); itCatGroups++)
			{
				if (itCatGroups != CategoryGroup.begin())
				{
					Printout->PrintFormat->Line->Columns[0]->Text = "";
					Printout->PrintFormat->Line->Columns[1]->Text = "";
					Printout->PrintFormat->Line->Columns[2]->Text = "";
					Printout->PrintFormat->AddLine();
				}
				// Print the Cat Group.
				Printout->PrintFormat->Add(itCatGroups->first + "|" + FormatFloat("0.00", itCatGroups->second.Totals.Qty) + "|" + FormatFloat
				("0.00", itCatGroups->second.Totals.Total));

				TCategoryGroupDetails CategoryGroupDetails = itCatGroups->second;
				TCategoryTotals::iterator itCategoryTotals;

				for (itCategoryTotals = CategoryGroupDetails.Details.begin(); itCategoryTotals != CategoryGroupDetails.Details.end();
				itCategoryTotals++)
				{
					Printout->PrintFormat->Add(" " + itCategoryTotals->first + "|" + FormatFloat("0.00",
					itCategoryTotals->second.Totals.Qty) + "|" + FormatFloat("0.00", itCategoryTotals->second.Totals.Total));

					TItemTotals::iterator itItemTotals;
					for (itItemTotals = itCategoryTotals->second.ItemDetails.begin();
					itItemTotals != itCategoryTotals->second.ItemDetails.end(); itItemTotals++)
					{
						Printout->PrintFormat->Add("  " + itItemTotals->first + "|" + FormatFloat("0.00",
						itItemTotals->second.Qty) + "|" + FormatFloat("0.00", itItemTotals->second.Total));
					}
				}
			}
		}

		Printout->PrintFormat->PartialCut();

		std::auto_ptr <TfrmShowPrintout> (frmShowPrintout)(TfrmShowPrintout::Create <TfrmShowPrintout> (this));
		Printout->PrintToStream(frmShowPrintout->CurrentPrintout.get());
		frmShowPrintout->Execute();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
// ---------------------------------------------------------------------------
void TfrmAnalysis::UpdateArchive(Database::TDBTransaction &DBTransaction, TMembership *Membership, UnicodeString DeviceName, int zedKey)
{
    zedLogsList->Add("Update archive code execution starts: ");
	long NewBillingKey;
	try
	{
		if (StockMasterPath != "")
		{
			__int64 StartTime = ::GetTickCount();
			while (FileExists(StockMasterPath + "MMSTART"))
			{
				::Sleep(500);
				if (::GetTickCount() > StartTime + 60000)
				{
					MessageBox("A Z-report cannot be completed at this time. Please try again later.", "Error", MB_OK + MB_ICONERROR);
					throw Exception("StockMaster lock timeout.");
				}
				Application->ProcessMessages();
			}
		}
		TCsv Csv;


		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery2 = DBTransaction.Query(DBTransaction.AddQuery());

		TIBSQL *IBDayArcBill = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayArcBillPay = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayArchive = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayArchiveDiscounts = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayArchiveTaxes = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayArcSurcharge = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayArcRef = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayPartonCount = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBDayWebArchive = DBTransaction.Query(DBTransaction.AddQuery());

		TIBSQL *IBArchive = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArchiveDiscounts = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArchiveTaxes = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcBill = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcBillPay = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcSurcharge = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcRef = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBPatronCount = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBWebArchive = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBZedQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBMallQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBMallSalesTypeQuery = DBTransaction.Query(DBTransaction.AddQuery());

		UnicodeString ExportFile = StockMasterPath + "MMTR_" + FormatFloat("00000",TGlobalSettings::Instance().SiteID) + "_" + DeviceName + ".csv";
		try
		{
			if (StockMasterPath != "" && FileExists(ExportFile))
			{
				Csv.LoadFromFile(ExportFile);
			}


            IBDayArcBill->Close();
            IBDayArcBill->SQL->Text = "Select * from DAYARCBILL a where a.TERMINAL_NAME = :TERMINAL_NAME ";
            IBDayArcBill->ParamByName("TERMINAL_NAME")->AsString = DeviceName;

            IBDayArcBillPay->Close();
            IBDayArcBillPay->SQL->Text = "select * from DAYARCBILLPAY where ARCBILL_KEY = :ARCBILL_KEY";

			IBDayArcRef->Close();
			IBDayArcRef->SQL->Text = "select * from DAYARCREF where ARCBILL_KEY = :ARCBILL_KEY";

			IBDayArchive->Close();
			IBDayArchive->SQL->Text = "select * from DAYARCHIVE where ARCBILL_KEY = :ARCBILL_KEY";

			IBDayArchiveDiscounts->Close();
			IBDayArchiveDiscounts->SQL->Text = "select * from DAYARCORDERDISCOUNTS where ARCHIVE_KEY = :ARCHIVE_KEY";

			IBDayArchiveTaxes->Close();
			IBDayArchiveTaxes->SQL->Text = "select * from DAYARCORDERTAXES where ARCHIVE_KEY = :ARCHIVE_KEY";

			IBDayArcSurcharge->Close();
			IBDayArcSurcharge->SQL->Text = "select * from DAYARCSURCHARGE where ARCBILL_KEY = :ARCBILL_KEY";

			IBDayPartonCount->Close();
			IBDayPartonCount->SQL->Text = "select * from DAYPATRONCOUNT where ARCBILL_KEY = :ARCBILL_KEY";

			IBDayWebArchive->Close();
			IBDayWebArchive->SQL->Text = "select * from DAYARCWEB where DAYARCWEB.ARCBILL_KEY = :ARCBILL_KEY";

            int Zedkey = zedKey;

			IBArchive->Close();
			IBArchive->SQL->Text = "INSERT INTO ARCHIVE " "(ARCHIVE.ARCHIVE_KEY, ARCHIVE.ARCBILL_KEY, ARCHIVE.TERMINAL_NAME, "
			"ARCHIVE.MENU_NAME, ARCHIVE.COURSE_NAME, ARCHIVE.ITEM_NAME, "
			"ARCHIVE.ITEM_SHORT_NAME, ARCHIVE.ITEM_ID, ARCHIVE.ITEM_CATEGORY, "
			"ARCHIVE.SIZE_NAME, ARCHIVE.TABLE_NUMBER, ARCHIVE.TABLE_NAME, " "ARCHIVE.SEAT_NUMBER, ARCHIVE.SERVER_NAME, ARCHIVE.TAB_NAME, "
			"ARCHIVE.LOYALTY_NAME, ARCHIVE.ORDER_TYPE, ARCHIVE.TIME_STAMP, "
			"ARCHIVE.TIME_STAMP_BILLED, ARCHIVE.ORDER_LOCATION, ARCHIVE.PRICE, "
			"ARCHIVE.COST, ARCHIVE.HAPPY_HOUR, "
			"ARCHIVE.NOTE, ARCHIVE.SECURITY_REF, ARCHIVE.TIME_KEY, ARCHIVE.GST_PERCENT, "
			"ARCHIVE.COST_GST_PERCENT, ARCHIVE.QTY, ARCHIVE.DISCOUNT, "
			"ARCHIVE.REDEEMED, ARCHIVE.POINTS_PERCENT, ARCHIVE.POINTS_EARNED, "
			"ARCHIVE.LOYALTY_KEY,ARCHIVE.THIRDPARTYCODES_KEY,ARCHIVE.CATEGORY_KEY,ARCHIVE.DISCOUNT_REASON, "
			"ARCHIVE.PRICE_LEVEL0, ARCHIVE.PRICE_LEVEL1,ARCHIVE.SERVINGCOURSES_KEY,ARCHIVE.PLU,ARCHIVE.CHIT_NAME,ARCHIVE.CHIT_OPTION,ARCHIVE.BASE_PRICE, "
            "ARCHIVE.DISCOUNT_WITHOUT_TAX,ARCHIVE.TAX_ON_DISCOUNT, PRICE_INCL, PRICE_ADJUST, ONLINE_CHIT_TYPE, ORDER_GUID ) "

			"VALUES " "(:ARCHIVE_KEY, :ARCBILL_KEY, :TERMINAL_NAME, :MENU_NAME, :COURSE_NAME, " ":ITEM_NAME, :ITEM_SHORT_NAME, :ITEM_ID, :ITEM_CATEGORY, "
                    " :SIZE_NAME, " ":TABLE_NUMBER, :TABLE_NAME, :SEAT_NUMBER, :SERVER_NAME, :TAB_NAME, " ":LOYALTY_NAME, :ORDER_TYPE, :TIME_STAMP, "
                    ":TIME_STAMP_BILLED, " ":ORDER_LOCATION, :PRICE, :COST, :HAPPY_HOUR, " ":NOTE, :SECURITY_REF, :TIME_KEY, :GST_PERCENT, :COST_GST_PERCENT, "
                     ":QTY, :DISCOUNT, :REDEEMED, :POINTS_PERCENT, :POINTS_EARNED, " ":LOYALTY_KEY, :THIRDPARTYCODES_KEY, :CATEGORY_KEY, :DISCOUNT_REASON," ":PRICE_LEVEL0, "
                     ":PRICE_LEVEL1, :SERVINGCOURSES_KEY, :PLU, :CHIT_NAME, :CHIT_OPTION,:BASE_PRICE,:DISCOUNT_WITHOUT_TAX,:TAX_ON_DISCOUNT,:PRICE_INCL, "
                     ":PRICE_ADJUST, :ONLINE_CHIT_TYPE, :ORDER_GUID ); ";

			IBArcBill->Close();
			IBArcBill->SQL->Text = "insert into \"ARCBILL\" "
			"(\"ARCBILL\".\"ARCBILL_KEY\", \"ARCBILL\".\"TERMINAL_NAME\", \"ARCBILL\".\"STAFF_NAME\", "
			"\"ARCBILL\".\"TIME_STAMP\", \"ARCBILL\".\"TOTAL\", \"ARCBILL\".\"DISCOUNT\", \"ARCBILL\".\"PATRON_COUNT\", "
			"\"ARCBILL\".\"RECEIPT\", \"ARCBILL\".\"SECURITY_REF\", \"ARCBILL\".\"SALES_TYPE\", "
			"\"ARCBILL\".\"BILLED_LOCATION\", \"ARCBILL\".\"INVOICE_NUMBER\", \"ARCBILL\".\"INVOICE_KEY\","
			"\"ARCBILL\".\"ORDER_TYPE_MESSAGE\", \"ARCBILL\".\"CONTACTS_KEY\", \"ARCBILL\".\"ROUNDING_ADJUSTMENT\","
            "\"ARCBILL\".\"ORDER_IDENTIFICATION_NUMBER\", \"ARCBILL\".\"Z_KEY\" , \"ARCBILL\".\"REFUND_REFRECEIPT\", "
            " \"ARCBILL\".\"IS_POSTED_TO_PANASONIC_SERVER\", \"ARCBILL\".\"CASH_DRAWER_OPENED\" , \"ARCBILL\".\"EFTPOS_SERVICE_ID\") "
            "values "
			"(:\"ARCBILL_KEY\", :\"TERMINAL_NAME\", :\"STAFF_NAME\", :\"TIME_STAMP\", :\"TOTAL\", "
			":\"DISCOUNT\", :\"PATRON_COUNT\", :\"RECEIPT\", :\"SECURITY_REF\", :\"SALES_TYPE\", "
			":\"BILLED_LOCATION\", :\"INVOICE_NUMBER\", :\"INVOICE_KEY\", :\"ORDER_TYPE_MESSAGE\","
            " :\"CONTACTS_KEY\", :\"ROUNDING_ADJUSTMENT\", :\"ORDER_IDENTIFICATION_NUMBER\", :\"Z_KEY\", "
            " :\"REFUND_REFRECEIPT\", :\"IS_POSTED_TO_PANASONIC_SERVER\", :\"CASH_DRAWER_OPENED\", :\"EFTPOS_SERVICE_ID\")";

			IBArcBillPay->Close();
			IBArcBillPay->SQL->Text = "insert into \"ARCBILLPAY\" ("
			"\"ARCBILLPAY\".\"ARCBILLPAY_KEY\", \"ARCBILLPAY\".\"ARCBILL_KEY\", \"ARCBILLPAY\".\"PAY_TYPE\", "
			"\"ARCBILLPAY\".\"SUBTOTAL\", \"ARCBILLPAY\".\"VOUCHER_NUMBER\", \"ARCBILLPAY\".\"CASH_OUT\", "
			"\"ARCBILLPAY\".\"NOTE\", \"ARCBILLPAY\".\"TAX_FREE\", \"ARCBILLPAY\".\"GROUP_NUMBER\", "
			"\"ARCBILLPAY\".\"PROPERTIES\", \"ARCBILLPAY\".\"PAY_TYPE_DETAILS\",\"ARCBILLPAY\".\"ROUNDING\", "
            "\"ARCBILLPAY\".\"PAYMENT_CARD_TYPE\",\"ARCBILLPAY\".\"PAY_GROUP\",\"ARCBILLPAY\".\"CHARGED_TO_XERO\",\"ARCBILLPAY\".\"TIP_AMOUNT\") "
            "values "
			"(:\"ARCBILLPAY_KEY\", :\"ARCBILL_KEY\", :\"PAY_TYPE\", :\"SUBTOTAL\", :\"VOUCHER_NUMBER\", "
			":\"CASH_OUT\", :\"NOTE\", :\"TAX_FREE\", :\"GROUP_NUMBER\", :\"PROPERTIES\", :\"PAY_TYPE_DETAILS\", :\"ROUNDING\", "
            ":\"PAYMENT_CARD_TYPE\", :\"PAY_GROUP\", :\"CHARGED_TO_XERO\",:\"TIP_AMOUNT\") ";

			IBArcSurcharge->Close();
			IBArcSurcharge->SQL->Text = "insert into \"ARCSURCHARGE\" "
			"(\"ARCSURCHARGE\".\"SURCHARGE_KEY\", \"ARCSURCHARGE\".\"ARCBILL_KEY\", \"ARCSURCHARGE\".\"PAY_TYPE\", "
			"\"ARCSURCHARGE\".\"PAY_TYPE_DETAILS\", \"ARCSURCHARGE\".\"SUBTOTAL\", \"ARCSURCHARGE\".\"NOTE\", "
			"\"ARCSURCHARGE\".\"TAX_FREE\", \"ARCSURCHARGE\".\"PROPERTIES\", \"ARCSURCHARGE\".\"GROUP_NUMBER\",\"ARCSURCHARGE\".\"ROUNDING\") "
			"values " "(:\"SURCHARGE_KEY\", :\"ARCBILL_KEY\", :\"PAY_TYPE\", :\"PAY_TYPE_DETAILS\", "
			":\"SUBTOTAL\", :\"NOTE\", :\"TAX_FREE\", :\"PROPERTIES\", :\"GROUP_NUMBER\", :\"ROUNDING\") ";

			IBArcRef->Close();
			IBArcRef->SQL->Text = "insert into \"ARCREF \" " "(\"ARCREF_KEY\", \"ARCBILL_KEY\", \"REFS_KEY\",\"REFERENCE\") " "values "
			"(:\"ARCREF_KEY\", :\"ARCBILL_KEY\", :\"REFS_KEY\", :\"REFERENCE\") ";

			IBPatronCount->Close();
			IBPatronCount->SQL->Text = "insert into \"PATRONCOUNT \" "
			"(\"PATRONCOUNT_KEY\", \"ARCBILL_KEY\", \"PATRON_TYPE\",\"PATRON_COUNT\") " "values "
			"(:\"PATRONCOUNT_KEY\", :\"ARCBILL_KEY\", :\"PATRON_TYPE\", :\"PATRON_COUNT\") ";

			IBArchiveDiscounts->Close();
			IBArchiveDiscounts->SQL->Text =
			"INSERT INTO ARCORDERDISCOUNTS (" "ARCORDERDISCOUNTS_KEY," "ARCHIVE_KEY," "DISCOUNT_KEY," "NAME," "DESCRIPTION,"
			"MENU," "PERCENTAGE," "AMOUNT," "DISCOUNT_TYPE," "DISCOUNT_MODE," "ROUNDING," "CATEGORY_KEY," "PROPERTIES," "PRIORITY,"
			"DISCOUNT_ID," "APPEARANCE_ORDER," "SOURCE," "DISCOUNTED_VALUE, MEMBERS_ONLY, MEMBERS_EXEMPT,DISCOUNT_GROUPNAME) " "VALUES (" ":ARCORDERDISCOUNTS_KEY," ":ARCHIVE_KEY,"
			":DISCOUNT_KEY," ":NAME," ":DESCRIPTION," ":MENU," ":PERCENTAGE," ":AMOUNT," ":DISCOUNT_TYPE," ":DISCOUNT_MODE," ":ROUNDING,"
			":CATEGORY_KEY," ":PROPERTIES," ":PRIORITY," ":DISCOUNT_ID," ":APPEARANCE_ORDER," ":SOURCE," ":DISCOUNTED_VALUE, :MEMBERS_ONLY, :MEMBERS_EXEMPT, :DISCOUNT_GROUPNAME);";

			IBArchiveTaxes->Close();
			IBArchiveTaxes->SQL->Text =
			"INSERT INTO ARCORDERTAXES (" "ARCORDERTAXES_KEY," "ARCHIVE_KEY," "TAX_NAME," "TAX_VALUE," "TAX_TYPE) "
			"VALUES (" ":ARCORDERTAXES_KEY," ":ARCHIVE_KEY," ":TAX_NAME," ":TAX_VALUE," ":TAX_TYPE);";

			IBWebArchive->Close();
			IBWebArchive->SQL->Text = "insert into \"ARCWEB \" "
			"(\"WEBORDER_KEY\", \"ARCBILL_KEY\") values (:\"WEBORDER_KEY\", :\"ARCBILL_KEY\") ";

            zedLogsList->Add("query object created and initialized with query. ");

            if(TGlobalSettings::Instance().mallInfo.MallId)
            {
                IBMallQuery->Close();
                IBMallQuery->SQL->Text = "UPDATE MALLEXPORT_SALES a SET A.ARCBILL_KEY = :ARCBILL_KEY WHERE A.ARCBILL_KEY = :DAYARCBILL_KEY "
                                         "AND A.DEVICE_KEY = :DEVICE_KEY AND A.DATE_CREATED = :DATE_CREATED ";

                IBMallSalesTypeQuery->Close();
                IBMallSalesTypeQuery->SQL->Text = "UPDATE MALL_SALES_BY_SALES_TYPE a SET A.ARCBILL_KEY = :ARCBILL_KEY WHERE A.ARCBILL_KEY = :DAYARCBILL_KEY "
                                                  "AND A.DEVICE_KEY = :DEVICE_KEY AND A.DATE_CREATED = :DATE_CREATED ";
                zedLogsList->Add("mall query object created and initialized with query. ");
            }

            try
            {
         	    IBDayArcBill->ExecQuery();

                zedLogsList->Add("Data processing starts. Befor the loop of dayarcbill iteration.");
			    for (; !IBDayArcBill->Eof; IBDayArcBill->Next())
                {
                        IBInternalQuery->Close();
                        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCBILL, 1) FROM RDB$DATABASE";
                        IBInternalQuery->ExecQuery();
                        int ArcBillKey = IBInternalQuery->Fields[0]->AsInteger;
                        try
                        {

                        // Copy all the daily ArcBill Fields to the main archive
                            IBArcBill->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
                            IBArcBill->ParamByName("Z_KEY")->AsInteger = Zedkey;
                            for (int i = 1; i < IBDayArcBill->FieldCount; i++)
                            {
                                UnicodeString FieldName = IBDayArcBill->Fields[i]->Name;
                                if (FieldName.UpperCase() == "RECEIPT")
                                {
                                    if (IBDayArcBill->FieldByName("RECEIPT")->IsNull)
                                    {
                                        IBArcBill->ParamByName("RECEIPT")->Clear();
                                    }
                                    else
                                    {
                                        std::auto_ptr <TMemoryStream> ReceiptToArchive(new TMemoryStream);
                                        IBDayArcBill->FieldByName("RECEIPT")->SaveToStream(ReceiptToArchive.get());
                                        ReceiptToArchive->Position = 0;
                                        IBArcBill->ParamByName("RECEIPT")->LoadFromStream(ReceiptToArchive.get());
                                    }
                                }
                                else
                                {
                                    VariantDBAssign(IBDayArcBill->FieldByName(FieldName), IBArcBill->ParamByName(FieldName));
                                }
                            }

                            IBArcBill->ExecQuery();
                            zedLogsList->Add("Invoice number : " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString + " data moved from dayarcbill to arcbill: IBArcBill->ExecQuery() ");

                    }
                    catch(Exception & E)
                    {
                        zedLogsList->Add("Exception while moving from dayarcbill to arcbill: IBArcBill->ExecQuery() and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString );
                        throw;
                    }

                    try
                    {
                        if (StockMasterPath != "")
                        {
                            Csv.Add("H," + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString + "," + IBDayArcBill->FieldByName("TIME_STAMP")
                            ->AsDateTime.FormatString("d/m/yyyy") + "," + IBDayArcBill->FieldByName("TIME_STAMP")->AsDateTime.FormatString("H:N:S")
                            + "," + IBDayArcBill->FieldByName("STAFF_NAME")->AsString + "," + IBDayArcBill->FieldByName("DISCOUNT")
                            ->AsString + "," + IBDayArcBill->FieldByName("PATRON_COUNT")->AsString);
                        }
                    }
                    catch(Exception & E)
                    {
                        zedLogsList->Add("Exception while adding invoice number to csv IBArcBill->ExecQuery() and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                        throw;
                    }
                    int dayArcBillKeyRef = 0;
                    try
                    {
                        IBDayArcBillPay->Close();
                        IBDayArcBillPay->ParamByName("ARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                        dayArcBillKeyRef = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                        IBDayArcBillPay->ExecQuery();
                        for (; !IBDayArcBillPay->Eof; IBDayArcBillPay->Next())
                        {
                            // Copy all the daily ArcBillPay Fields to the main archive. Don't copy field 1 as it is the
                            // link to dtArcBill
                            IBInternalQuery->Close();
                            IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCBILLPAY, 1) FROM RDB$DATABASE";
                            IBInternalQuery->ExecQuery();
                            int ArcBillPayKey = IBInternalQuery->Fields[0]->AsInteger;

                            for (int i = 2; i < IBDayArcBillPay->FieldCount; i++)
                            {
                                UnicodeString FieldName = IBDayArcBillPay->Fields[i]->Name;
                                VariantDBAssign(IBDayArcBillPay->FieldByName(FieldName), IBArcBillPay->ParamByName(FieldName));
                                // IBArcBillPay->ParamByName(FieldName)->AsVariant =
                                // IBDayArcBillPay->FieldByName(FieldName)->AsVariant;
                            }
                            IBArcBillPay->ParamByName("ARCBILLPAY_KEY")->AsInteger = ArcBillPayKey;
                            IBArcBillPay->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
                            IBArcBillPay->ExecQuery();

                            if (StockMasterPath != "")
                            {
                                Csv.Add(UnicodeString("T,") + IBDayArcBillPay->FieldByName("PAY_TYPE")->AsString + "," + IBDayArcBillPay->FieldByName
                                ("SUBTOTAL")->AsString);
                            }
                        }
                        zedLogsList->Add("Invoice number : " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString + " : data moved from dayarcbillpay to arcbillpay: IBArcBill->ExecQuery() ");
                    }
                    catch(Exception & E)
                    {
                        zedLogsList->Add("Exception while moving from dayarcbillpay to arcbillpay: IBArcBillPay->ExecQuery() and invoice number is " + IBArcBillPay->FieldByName("INVOICE_NUMBER")->AsString);
                        throw;
                    }

                    try
                    {
                        IBDayArcRef->Close();
                        IBDayArcRef->ParamByName("ARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                        IBDayArcRef->ExecQuery();
                        for (; !IBDayArcRef->Eof; IBDayArcRef->Next())
                        {
                            // Copy all the daily ArcBillPay Fields to the main archive. Don't copy field 1 as it is the
                            // link to dtArcBill
                            IBInternalQuery->Close();
                            IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCREF, 1) FROM RDB$DATABASE";
                            IBInternalQuery->ExecQuery();
                            int ArcRefKey = IBInternalQuery->Fields[0]->AsInteger;

                            for (int i = 2; i < IBDayArcRef->FieldCount; i++)
                            {
                                UnicodeString FieldName = IBDayArcRef->Fields[i]->Name;
                                VariantDBAssign(IBDayArcRef->FieldByName(FieldName), IBArcRef->ParamByName(FieldName));

                                /* IBArcRef->ParamByName(FieldName)->AsVariant =
                        IBDayArcRef->FieldByName(FieldName)->AsVariant; */
                            }
                            IBArcRef->ParamByName("ARCREF_KEY")->AsInteger = ArcRefKey;
                            IBArcRef->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
                            IBArcRef->ExecQuery();
                        }
                        zedLogsList->Add("Invoice number : " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString + " : data moved from dayarcbillref to arcbillref: IBArcRef->ExecQuery() ");
                    }
                    catch(Exception & E)
                    {
                        zedLogsList->Add("Exception while moving from dayarcref to arcref: IBArcRef->ExecQuery() and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                        throw;
                    }

                    try
                    {
                        IBDayArcSurcharge->Close();
                        IBDayArcSurcharge->ParamByName("ARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                        IBDayArcSurcharge->ExecQuery();
                        for (; !IBDayArcSurcharge->Eof; IBDayArcSurcharge->Next())
                        {
                            IBInternalQuery->Close();
                            IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCSURCHARGE, 1) FROM RDB$DATABASE";
                            IBInternalQuery->ExecQuery();
                            int ArcSurchargeKey = IBInternalQuery->Fields[0]->AsInteger;

                            // Copy all the daily ArcBillPay Fields to the main archive. Don't copy field 1 as it is the
                            // link to dtArcBill
                            for (int i = 2; i < IBDayArcSurcharge->FieldCount; i++)
                            {
                                UnicodeString FieldName = IBDayArcSurcharge->Fields[i]->Name;
                                VariantDBAssign(IBDayArcSurcharge->FieldByName(FieldName), IBArcSurcharge->ParamByName(FieldName));

                                /* IBArcSurcharge->ParamByName(FieldName)->AsVariant =
                        IBDayArcSurcharge->FieldByName(FieldName)->AsVariant; */
                            }
                            IBArcSurcharge->ParamByName("SURCHARGE_KEY")->AsInteger = ArcSurchargeKey;
                            IBArcSurcharge->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
                            IBArcSurcharge->ExecQuery();

                            if (StockMasterPath != "")
                            {
                                Csv.Add(UnicodeString("T,") + IBDayArcSurcharge->FieldByName("PAY_TYPE")->AsString + "," + IBDayArcSurcharge->FieldByName
                                ("SUBTOTAL")->AsString);
                            }
                        }
                        zedLogsList->Add("Invoice number : " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString + " : data moved from dayarcsurcharge to arcsurcharge: IBArcSurcharge->ExecQuery() ");
                    }
                    catch(Exception & E)
                    {
                        zedLogsList->Add("Exception while moving from dayarcsurcharge to arcsurcharge : IBArcSurcharge->ExecQuery() and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                        throw;
                    }

                    try
                    {
                        IBDayPartonCount->Close();
                        IBDayPartonCount->ParamByName("ARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                        IBDayPartonCount->ExecQuery();
                        for (; !IBDayPartonCount->Eof; IBDayPartonCount->Next())
                        {
                            IBInternalQuery->Close();
                            IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PATRONCOUNT, 1) FROM RDB$DATABASE";
                            IBInternalQuery->ExecQuery();
                            int ArcPatronCountKey = IBInternalQuery->Fields[0]->AsInteger;

                            // Copy all the daily ArcBillPay Fields to the main archive. Don't copy field 1 as it is the
                            // link to dtArcBill
                            for (int i = 2; i < IBDayPartonCount->FieldCount; i++)
                            {
                                UnicodeString FieldName = IBDayPartonCount->Fields[i]->Name;
                                VariantDBAssign(IBDayPartonCount->FieldByName(FieldName), IBPatronCount->ParamByName(FieldName));

                                /* IBPatronCount->ParamByName(FieldName)->AsVariant =
                        IBDayPartonCount->FieldByName(FieldName)->AsVariant; */
                            }
                            IBPatronCount->ParamByName("PATRONCOUNT_KEY")->AsInteger = ArcPatronCountKey;
                            IBPatronCount->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
                            IBPatronCount->ExecQuery();

                            /* if (UseStockMaster)
                    { // This will write out the Patron count its on disable because
                    // it 'might' break other people already useing this file.
                    // Given each line has a header Type char it 'shouldnt though'.
                    Csv.Add(UnicodeString("P,") +
                    IBDayPartonCount->FieldByName("PATRON_TYPE")->AsString + "," +
                    IBDayPartonCount->FieldByName("PATRON_COUNT")->AsString);
                    } */
                        }
                        zedLogsList->Add("Invoice number : " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString + " : patron data moved: IBPatronCount->ExecQuery() ");
                    }
                    catch(Exception & E)
                    {
                        zedLogsList->Add("Exception while moving patron count data : IBPatronCount->ExecQuery() and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                        throw;
                    }

                    try
                    {
                        IBDayWebArchive->Close();
                        IBDayWebArchive->ParamByName("ARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                        IBDayWebArchive->ExecQuery();
                        for (; !IBDayWebArchive->Eof; IBDayWebArchive->Next())
                        {
                            // Copy all the daily ArcBillPay Fields to the main archive. Don't copy field 1 as it is the
                            // link to dtArcBill
                            VariantDBAssign(IBDayWebArchive->FieldByName("WEBORDER_KEY"), IBWebArchive->ParamByName("WEBORDER_KEY"));
                            IBWebArchive->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
                            IBWebArchive->ExecQuery();
                        }
                        zedLogsList->Add("Invoice number : " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString + " : data moved from dayarcweb to arcweb: IBWebArchive->ExecQuery() ");
                    }
                    catch(Exception & E)
                    {
                        zedLogsList->Add("Exception while moving dayarcweb to arc web : IBWebArchive->ExecQuery() and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                        throw;
                    }

                    try
                    {
                    // Find all the archive entries connected to this arcbill.
                    IBDayArchive->Close();
                    IBDayArchive->ParamByName("ARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                    IBDayArchive->ExecQuery();
                    for (; !IBDayArchive->Eof; IBDayArchive->Next())
                    {
                                IBInternalQuery->Close();
                                IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCHIVE, 1) FROM RDB$DATABASE";
                                IBInternalQuery->ExecQuery();
                                int ArchiveKey = IBInternalQuery->Fields[0]->AsInteger;
                                try
                                {
                                    for (int i = 2; i < IBDayArchive->FieldCount; i++)
                                    {
                                        UnicodeString FieldName = IBDayArchive->Fields[i]->Name;
                                        if(ParamExists(IBArchive, FieldName))
                                        {
                                            VariantDBAssign(IBDayArchive->FieldByName(FieldName), IBArchive->ParamByName(FieldName));
                                        }
                                        /* IBArchive->ParamByName(FieldName)->AsVariant =
                                IBDayArchive->FieldByName(FieldName)->AsVariant; */
                                    }
                                    IBArchive->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
                                    IBArchive->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
                                    IBArchive->ExecQuery();
                               }
                               catch(Exception & E)
                               {
                                    zedLogsList->Add("Exception while moving dayarchive to archive IBArchive->ExecQuery(); and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                                    throw;
                               }

                            try
                            {
                                IBDayArchiveDiscounts->Close();
                                IBDayArchiveDiscounts->ParamByName("ARCHIVE_KEY")->AsInteger = IBDayArchive->FieldByName("ARCHIVE_KEY")->AsInteger;
                                IBDayArchiveDiscounts->ExecQuery();
                                for (; !IBDayArchiveDiscounts->Eof; IBDayArchiveDiscounts->Next())
                                {
                                    IBInternalQuery->Close();
                                    IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCORDERDISCOUNTS, 1) FROM RDB$DATABASE";
                                    IBInternalQuery->ExecQuery();
                                    int ARCORDERDISCOUNTS_KEY = IBInternalQuery->Fields[0]->AsInteger;

                                    for (int i = 2; i < IBDayArchiveDiscounts->FieldCount; i++)
                                    {
                                        UnicodeString FieldName = IBDayArchiveDiscounts->Fields[i]->Name;
                                        VariantDBAssign(IBDayArchiveDiscounts->FieldByName(FieldName), IBArchiveDiscounts->ParamByName(FieldName));
                                    }
                                    IBArchiveDiscounts->ParamByName("ARCORDERDISCOUNTS_KEY")->AsInteger = ARCORDERDISCOUNTS_KEY;
                                    IBArchiveDiscounts->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
                                    IBArchiveDiscounts->ExecQuery();
                                }
                            }
                            catch(Exception & E)
                            {
                                zedLogsList->Add("Exception while moving dayarcorderdiscounts to arcorderdiscounts : IBArchiveDiscounts->execquery() and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                                throw;
                            }

                            try
                            {
                                IBDayArchiveTaxes->Close();
                                IBDayArchiveTaxes->ParamByName("ARCHIVE_KEY")->AsInteger = IBDayArchive->FieldByName("ARCHIVE_KEY")->AsInteger;
                                IBDayArchiveTaxes->ExecQuery();
                                for (; !IBDayArchiveTaxes->Eof; IBDayArchiveTaxes->Next())
                                {
                                    IBInternalQuery->Close();
                                    IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCORDERTAXES, 1) FROM RDB$DATABASE";
                                    IBInternalQuery->ExecQuery();
                                    int ARCORDERTAXES_KEY = IBInternalQuery->Fields[0]->AsInteger;

                                    for (int i = 2; i < IBDayArchiveTaxes->FieldCount; i++)
                                    {
                                        UnicodeString FieldName = IBDayArchiveTaxes->Fields[i]->Name;
                                        VariantDBAssign(IBDayArchiveTaxes->FieldByName(FieldName), IBArchiveTaxes->ParamByName(FieldName));
                                    }
                                    IBArchiveTaxes->ParamByName("ARCORDERTAXES_KEY")->AsInteger = ARCORDERTAXES_KEY;
                                    IBArchiveTaxes->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
                                    IBArchiveTaxes->ExecQuery();
                                }
                            }
                            catch(Exception & E)
                            {
                                zedLogsList->Add("Exception while moving dayarcordertaxes to arcordertaxes and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                                throw;
                            }

                            try
                            {
                                IBInternalQuery->Close();
                                IBInternalQuery->SQL->Text = "SELECT CATEGORY_KEY FROM DAYARCCATEGORY WHERE ARCHIVE_KEY = :ARCHIVE_KEY";
                                IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = IBDayArchive->FieldByName("ARCHIVE_KEY")->AsInteger;
                                IBInternalQuery->ExecQuery();
                                for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
                                {
                                    IBInternalQuery2->Close();
                                    IBInternalQuery2->SQL->Text =
                                    "INSERT INTO ARCCATEGORY (" "ARCHIVE_KEY," "CATEGORY_KEY) " "VALUES (" ":ARCHIVE_KEY,"
                                    ":CATEGORY_KEY);";
                                    IBInternalQuery2->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
                                    IBInternalQuery2->ParamByName("CATEGORY_KEY")->AsInteger = IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
                                    IBInternalQuery2->ExecQuery();
                                }
                            }
                            catch(Exception & E)
                            {
                                zedLogsList->Add("Exception while moving dayarccategory to Arccategory and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                                throw;
                            }

                            try
                            {
                                if (StockMasterPath != "")
                                {
                                    bool SetMunuItem = (IBDayArchive->FieldByName("PRICE")->AsCurrency == 0);
                                    UnicodeString ITEM_ID = IBDayArchive->FieldByName("ITEM_ID")->AsString;
                                    UnicodeString SIZE_NAME = IBDayArchive->FieldByName("SIZE_NAME")->AsString;
                                    UnicodeString TIME_STAMPDMY = IBDayArchive->FieldByName("TIME_STAMP")->AsDateTime.FormatString("d/m/yyyy");
                                    UnicodeString TIME_STAMPHMS = IBDayArchive->FieldByName("TIME_STAMP")->AsDateTime.FormatString("hh:nn:ss");
                                    UnicodeString SERVER_NAME = IBDayArchive->FieldByName("SERVER_NAME")->AsString;
                                    UnicodeString ORDER_TYPE = IBDayArchive->FieldByName("ORDER_TYPE")->AsString;
                                    UnicodeString ORDER_LOCATION = IBDayArchive->FieldByName("ORDER_LOCATION")->AsString;
                                    UnicodeString LOYALTY_NAME = Membership->GetContactName(DBTransaction, IBDayArchive->FieldByName("LOYALTY_KEY")->AsInteger);
                                    UnicodeString MEMBER_NUMBER = Membership->GetContactNumber(DBTransaction, IBDayArchive->FieldByName("LOYALTY_KEY")->AsInteger);
                                    UnicodeString SITE_ID = FormatFloat("00000",TGlobalSettings::Instance().SiteID);
                                    UnicodeString DISCOUNT = FormatFloat("0.00", IBDayArchive->FieldByName("DISCOUNT")->AsCurrency);
                                    UnicodeString REDEEMED = FormatFloat("0.00", IBDayArchive->FieldByName("REDEEMED")->AsCurrency);
                                    UnicodeString POINTS_PERCENT = FloatToStr(IBDayArchive->FieldByName("POINTS_PERCENT")->AsFloat);
                                    UnicodeString POINTS_EARNED = FormatFloat("0.00", IBDayArchive->FieldByName("POINTS_EARNED")->AsCurrency);
                                    UnicodeString THIRDPARTYCODES_KEY = TDBThirdPartyCodes::GetThirdPartyCodeByKey(DBTransaction,IBDayArchive->FieldByName("THIRDPARTYCODES_KEY")->AsInteger);
                                    UnicodeString PRICE = FormatFloat("0.00", IBDayArchive->FieldByName("PRICE")->AsCurrency * IBDayArchive->FieldByName("QTY")->AsCurrency);
                                    UnicodeString HAPPY_HOUR = IBDayArchive->FieldByName("HAPPY_HOUR")->AsString.UpperCase() == "F" ? "0" : "1";
                                    UnicodeString PLU = IBDayArchive->FieldByName("PLU")->AsString;

                                    Csv.Add(UnicodeString("P,") + ITEM_ID + "," + SIZE_NAME + "," + "," + TIME_STAMPDMY + "," + TIME_STAMPHMS +
                                    "," + SERVER_NAME + "," + ORDER_TYPE + "," + ORDER_LOCATION + "," + HAPPY_HOUR + "," + LOYALTY_NAME + "," + SITE_ID +
                                    "," + MEMBER_NUMBER + "," + DISCOUNT + "," + REDEEMED + "," + POINTS_PERCENT + "," + POINTS_EARNED + "," +
                                    THIRDPARTYCODES_KEY + "," + (SetMunuItem ? "1" : "0") + "," + PRICE + "," + PLU);
                                }
                            }
                            catch(Exception & E)
                            {
                                zedLogsList->Add("Exception adding row to csv and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                                throw;
                            }

                        }
                        zedLogsList->Add("Invoice number : " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString + " : data moved from dayarchive to archive ");
                    }
                    catch(Exception & E)
                    {
                        zedLogsList->Add("Exception in moving from dayarchive to archive parent catch and invoice number is " + IBDayArcBill->FieldByName("INVOICE_NUMBER")->AsString);
                        throw;
                    }

                    //Update MallExport's arcbill key if zed is done..
                    if(TGlobalSettings::Instance().mallInfo.MallId)
                    {
                        IBMallQuery->Close();
                        IBMallQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
                        IBMallQuery->ParamByName("DAYARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                        IBMallQuery->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
                        IBMallQuery->ParamByName("DATE_CREATED")->AsDateTime = IBDayArcBill->FieldByName("TIME_STAMP")->AsDateTime;
                        IBMallQuery->ExecQuery();

                        IBMallSalesTypeQuery->Close();
                        IBMallSalesTypeQuery->ParamByName("ARCBILL_KEY")->AsInteger = ArcBillKey;
                        IBMallSalesTypeQuery->ParamByName("DAYARCBILL_KEY")->AsInteger = IBDayArcBill->FieldByName("ARCBILL_KEY")->AsInteger;
                        IBMallSalesTypeQuery->ParamByName("DEVICE_KEY")->AsInteger = TDeviceRealTerminal::Instance().ID.ProfileKey;
                        IBMallSalesTypeQuery->ParamByName("DATE_CREATED")->AsDateTime = IBDayArcBill->FieldByName("TIME_STAMP")->AsDateTime;
                        IBMallSalesTypeQuery->ExecQuery();
                    }
                }
            }
            catch(Exception & E)
	        {
                zedLogsList->Add("Exception in overall data movement from day's table to arc's table.");
                MakeZEDLogFile(zedLogsList);
                zedLogsList->Clear();
                throw;
            }

            try
            {
                zedLogsList->Add("Deleting data from dayarcbill");
                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text = "DELETE FROM DAYARCBILL where TERMINAL_NAME = :TERMINAL_NAME";
                IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
                IBInternalQuery->ExecQuery();
                zedLogsList->Add("Data deleted from dayarcbill");
            }
            catch(Exception & E)
	        {
                zedLogsList->Add("Exception in deleting data from dayarcbill");
                throw;
            }

                if (StockMasterPath != "")
                {
                    Csv.SaveToFile(ExportFile);
                }

                if((TGlobalSettings::Instance().MallIndex == 7 && !DuplicateEntryInTable) ||
                    (TGlobalSettings::Instance().MallIndex != 0 && TGlobalSettings::Instance().MallIndex != 9 && TGlobalSettings::Instance().MallIndex != 7))
                {
                    zedLogsList->Add("updating zed table for mall");
                    UpdateArcMallExport(DBTransaction);
                }

		}
		__finally
		{
			if (StockMasterPath != "")
			{
				DeleteFile(StockMasterPath + "MMSTART");
                zedLogsList->Add("Deleting stock file");
                MakeZEDLogFile(zedLogsList);
                zedLogsList->Clear();
			}
		}
	}
	catch(Exception & E)
	{
        zedLogsList->Add("Exception in data moving in updateArchive() function last catch()");
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
        MakeZEDLogFile(zedLogsList);
        zedLogsList->Clear();
		throw E;
	}
}

void TfrmAnalysis::ProcessStock(Database::TDBTransaction &DBTransaction)
{
	UnicodeString StockDB =TGlobalSettings::Instance().StockInterbaseIP + ":" +TGlobalSettings::Instance().StockDatabasePath;
	if (StockDB == ":")
	{
		MessageBox("Your Stock DB Server and Path are blank. Stock will not be updated until this is corrected.", "Error",
		MB_OK + MB_ICONWARNING);
	}
	else
	{
		try
		{
			TStockInterface StockInterface(StockDB);
			try
			{
				StockInterface.Initialise();
				AdjustStock(DBTransaction, StockInterface);
				GetCosts(DBTransaction, StockInterface);
			}
			catch(Exception & E)
			{
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message + " Full DB Path " + StockDB);
				throw;
			}
		}
		catch(Exception & E)
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message + " Full DB Path " + StockDB);
			throw;
		}
	}
}

void TfrmAnalysis::AdjustStock(Database::TDBTransaction &DBTransaction, StockInterface::TStockInterface &StockInterface)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	TIBSQL *IBStockTrackQuery = DBTransaction.Query(DBTransaction.AddQuery());

	try
	{
		if (!StockInterface.StartAdjustments())
		{
			throw Exception(StartAdjustError);
		}
		else
		{
			int LastStockKey;

			IBStockTrackQuery->Close();
			IBStockTrackQuery->ParamCheck = true;
			IBStockTrackQuery->SQL->Text = "SELECT MAX(STOCK_KEY) STOCK_KEY " "FROM " "STOCKTRACK ";
			IBStockTrackQuery->ExecQuery();
			LastStockKey = IBStockTrackQuery->FieldByName("STOCK_KEY")->AsInteger;

			IBStockTrackQuery->Close();
			IBStockTrackQuery->ParamCheck = true;
			IBStockTrackQuery->SQL->Clear();
			IBStockTrackQuery->SQL->Text =
			"SELECT SUM(QTY) QTY, " "STOCK_CODE, STOCK_LOCATION, DEVICE_NAME, ADJUSTMENT_TYPE, STAFF, REASON " "FROM " "STOCKTRACK "
			"WHERE STOCK_KEY <= :STOCK_KEY " "GROUP BY STOCK_CODE, STOCK_LOCATION, DEVICE_NAME, ADJUSTMENT_TYPE, STAFF, REASON";
			IBStockTrackQuery->ParamByName("STOCK_KEY")->AsInteger = LastStockKey;

			IBStockTrackQuery->ExecQuery();
			for (; !IBStockTrackQuery->Eof; IBStockTrackQuery->Next())
			{
				double Qty = RoundTo(IBStockTrackQuery->FieldByName("QTY")->AsFloat, -5);
				UnicodeString StockCode = IBStockTrackQuery->FieldByName("STOCK_CODE")->AsString;
				UnicodeString StockLocation = IBStockTrackQuery->FieldByName("STOCK_LOCATION")->AsString;
				int AdjustmentType = IBStockTrackQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger;
				AnsiString Operator = IBStockTrackQuery->FieldByName("STAFF")->AsString;
				AnsiString Reason = IBStockTrackQuery->FieldByName("REASON")->AsString;
				AnsiString DeviceName = IBStockTrackQuery->FieldByName("DEVICE_NAME")->AsString;

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCHIVESTOCKTRACK, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();
				int LocalKey = IBInternalQuery->Fields[0]->AsInteger;

				IBInternalQuery->Close();
				IBInternalQuery->ParamCheck = true;
				IBInternalQuery->SQL->Clear();
				IBInternalQuery->SQL->Text =
				"INSERT INTO ARCHIVESTOCKTRACK (" "STOCK_KEY, " "STOCK_CODE, " "STOCK_LOCATION, " "QTY, " "TIME_STAMP, "
				"EXPORTED, " "CONFIRMED, " "DEVICE_NAME, " "ADJUSTMENT_TYPE) " "VALUES (" ":STOCK_KEY, " ":STOCK_CODE, "
				":STOCK_LOCATION, " ":QTY, " ":TIME_STAMP, " ":EXPORTED, " ":CONFIRMED, " ":DEVICE_NAME, " ":ADJUSTMENT_TYPE);";
				IBInternalQuery->ParamByName("STOCK_KEY")->AsInteger = LocalKey;
				IBInternalQuery->ParamByName("STOCK_CODE")->AsString = StockCode;
				IBInternalQuery->ParamByName("STOCK_LOCATION")->AsString = IBStockTrackQuery->FieldByName("STOCK_LOCATION")->AsString;
				IBInternalQuery->ParamByName("DEVICE_NAME")->AsString = IBStockTrackQuery->FieldByName("DEVICE_NAME")->AsString;
				IBInternalQuery->ParamByName("QTY")->AsFloat = Qty;
				IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = IBStockTrackQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger;
				IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
				IBInternalQuery->ParamByName("EXPORTED")->AsString = "T";
				IBInternalQuery->ParamByName("CONFIRMED")->AsString = "T";
				IBInternalQuery->ExecQuery();

				try
				{
					// Adjust stock levels via the DLL
					if (!StockInterface.AdjustStock(StockCode.t_str(), StockLocation.t_str(), -Qty, AdjustmentType, Operator, Reason, DeviceName))
					{
						TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Stock DLL Adjust Stock Failed on " + StockCode + " Qty :" + FloatToStrF(Qty, ffGeneral, 3,
						2) + " Loc : " + StockLocation + " Adj Type: " + IntToStr(AdjustmentType));
					}
				}
				catch(Exception & E)
				{
					TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Stock Adjust Failed : " + E.Message + StockCode + " Qty :" + FloatToStrF(Qty, ffGeneral, 3,
					2) + " Loc : " + StockLocation + " Adj Type: " + IntToStr(AdjustmentType));
				}
			}

			// All copied. Empty the temporary table.
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "DELETE FROM STOCKTRACK WHERE STOCK_KEY <= :LAST_STOCK_KEY";
			IBInternalQuery->ParamByName("LAST_STOCK_KEY")->AsInteger = LastStockKey;
			IBInternalQuery->ExecQuery();

			if (!StockInterface.EndAdjustments())
			{
				throw Exception(EndAdjustError);
			}

			IBStockTrackQuery->Close();
		}
	}
	catch(Exception & E)
	{
		StockInterface.RollbackAdjustments();
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message + "Stock Track");
		TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
		throw;
	}
}
// ---------------------------------------------------------------------------
void TfrmAnalysis::GetCosts(Database::TDBTransaction &DBTransaction, StockInterface::TStockInterface &StockInterface)
{
	try
	{
		StockInterface::TStockDetails StockCostDetails;

		bool Eof = !StockInterface.GetFirstStockCost(StockCostDetails);
		while (!Eof)
		{
			TDeviceRealTerminal::Instance().ManagerStock->UpdateStockCost(DBTransaction, StockCostDetails.StockCode, StockCostDetails.Location,
			StockCostDetails.Unit, StockCostDetails.Description, StockCostDetails.Cost, StockCostDetails.GSTPercent);

			TDeviceRealTerminal::Instance().UpdateCurrentRecipes(StockCostDetails.StockCode, StockCostDetails.Location, StockCostDetails.Cost,
			StockCostDetails.GSTPercent);

			TDeviceRealTerminal::Instance().ManagerStock->UpdateItemCosts(DBTransaction, StockCostDetails.StockCode, StockCostDetails.Cost,
			StockCostDetails.GSTPercent);

			Eof = !StockInterface.GetNextStockCost(StockCostDetails);
		}

		// Recalculate all ITEMSIZE COST & COST_GST_PERCENT for all
		// ITEMSIZE's that have Receipes that are attached to Stock Codes.

	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmAnalysis::TableTabSummaryReport()
{
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);

	std::auto_ptr<TfrmProcessing>
	(Processing)(TfrmProcessing::Create<TfrmProcessing>(this));
	Processing->Message = "Generating Table Tab Summary ...";
	Processing->Show();

	tr.StartTransaction();
	std::auto_ptr<TManagerReports>(rm)(
	new TManagerReports((TForm *)(this)));
	rm->PrintTableTabSummaryReport(tr);
	tr.Commit();

	Processing->Close();
}

TLoginSuccess TfrmAnalysis::AuthenticateReportsAccess(TReportSource reportSource)
{
	static AnsiString reportName[3] = {"access Flash Reports.","perform Zeds.","access Reports."};

	TLoginSuccess loginResult;
	std::auto_ptr<TContactStaff> staff;
	TMMContactInfo newCI;
	Database::TDBTransaction trans(TDeviceRealTerminal::Instance().DBControl);

	trans.StartTransaction();
	staff.reset(new TContactStaff(trans));
	loginResult = staff->Login(NULL, trans, newCI, SecurityCheck::CheckZeds);
	trans.Commit();

	switch (loginResult) {
	case lsPINIncorrect:
		MessageBox("The login was unsucceessful.", "Error",
		MB_OK | MB_ICONERROR);
		break;
	case lsDenied:
	case lsAccountBlocked:
		MessageBox("You do not have permission to "
		+ reportName[reportSource], "Error", MB_OK | MB_ICONERROR);
		break;
	case lsAccepted:
		lastAuthenticatedUser = newCI;
	case lsCancel:
		break;
	}

	return loginResult;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmAnalysis::btnReportsClick(void)
{
	if (AuthenticateReportsAccess(TReportSource::CashDrawer) == lsAccepted)
    {
		std::auto_ptr <TfrmDropDownFunc> frmDropDown(
		TfrmDropDownFunc::Create <TfrmDropDownFunc>(this));	
        frmDropDown->AddButton("X Report", &ReportXReport);
		frmDropDown->AddButton("Float Adjustments", &ReportFloatAdjustments);
		frmDropDown->AddButton("Write Off Audit", &ReportWriteOff);
		frmDropDown->AddButton("Consumption", &ReportConsumption);
		frmDropDown->AddButton("Table & Tab Summary", &TableTabSummaryReport);
		frmDropDown->AddButton("Site Summary", &ReportSiteSummary);
		if(TGlobalSettings::Instance().UseItalyFiscalPrinter)
            frmDropDown->AddButton("Fiscal Printer Settlement", &FiscalPrinterSettlement);

		if (frmDropDown->ShowModal() == mrOk)
        {
			frmDropDown->FunctionToCall();
		}
	}
}

void __fastcall TfrmAnalysis::ReportConsumption()
{
	Database::TDBTransaction DBTransaction(
	TDeviceRealTerminal::Instance().DBControl);
	std::auto_ptr <TfrmProcessing> Processing(
	TfrmProcessing::Create <TfrmProcessing>(this));
	Processing->Message = "Please Wait...";
	Processing->Show();

	DBTransaction.StartTransaction();
	std::auto_ptr <TManagerReports> (ReportManager)(new TManagerReports((TForm*)this));
	ReportManager->PrintConsumption(DBTransaction);
	DBTransaction.Commit();
	Processing->Close();
}

void __fastcall TfrmAnalysis::ReportFloatAdjustments()
{
	Database::TDBTransaction DBTransaction(
	TDeviceRealTerminal::Instance().DBControl);
	UnicodeString DeviceName = GetTerminalName();

	std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
	Processing->Message = "Please Wait...";
	Processing->Show();
	DBTransaction.StartTransaction();
	PrintFloatAdjustments(DBTransaction, DeviceName);
	DBTransaction.Commit();
	Processing->Close();
}

void __fastcall TfrmAnalysis::ReportWriteOff(void)
{
	Database::TDBTransaction DBTransaction(
	TDeviceRealTerminal::Instance().DBControl);
	UnicodeString DeviceName = GetTerminalName();

	std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
	Processing->Message = "Please Wait...";
	Processing->Show();
	TTransactionInfo TransactionInfo;
	DBTransaction.StartTransaction();
    TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfo(DBTransaction, DeviceName);
	PrintWriteOff(DBTransaction, DeviceName);
	DBTransaction.Commit();
	Processing->Close();
}

void __fastcall TfrmAnalysis::ReportSiteSummary()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);

	UnicodeString DeviceName = GetTerminalName();
	std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
	Processing->Message = "Please Wait...";
	Processing->Show();

	DBTransaction.StartTransaction();

	bool PrinterExists;

	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		PrinterExists = false;
	}

	std::auto_ptr <TPrintout> Printout(new TPrintout);

	if (!PrinterExists)
	{
		TPrinterPhysical DefaultScreenPrinter;
		DefaultScreenPrinter.NormalCharPerLine = 40;
		DefaultScreenPrinter.BoldCharPerLine = 40;
		Printout->Printer = DefaultScreenPrinter;
	}
	else
	{
		Printout->Printer = TComms::Instance().ReceiptPrinter;
	}

	SiteSummaryReport(DBTransaction, Printout.get(),
	GetPrevZedTime(DBTransaction));
	DBTransaction.Commit();
	Processing->Close();

	Printout->PrintFormat->PartialCut();

	std::auto_ptr <TfrmShowPrintout> (frmShowPrintout)(TfrmShowPrintout::Create <TfrmShowPrintout> (this));
	Printout->PrintToStream(frmShowPrintout->CurrentPrintout.get());

	if (TGlobalSettings::Instance().EnableBlindBalances)
	{
		frmShowPrintout->btnCancel->Visible = false;
	}
	else
	{
		frmShowPrintout->btnCancel->Visible = true;
		frmShowPrintout->btnCancel->Caption = "Cancel Zed";
	}
	frmShowPrintout->btnClose->Caption = "Close Till";
	frmShowPrintout->btnClosePrint->Caption = "Close Till and Print Zed";

	frmShowPrintout->btnCancel->Caption = "Close";
	frmShowPrintout->btnClose->Visible = false;
	frmShowPrintout->btnClosePrint->Caption = "Print";

	frmShowPrintout->Execute();
	ExitCode = frmShowPrintout->ExitCode;

	UnicodeString Dir = ExtractFilePath(Application->ExeName) + ZDIR;
	if (!DirectoryExists(Dir))
	{
		CreateDir(Dir);
	}
	Printout->PrintToFile(Dir + "\\" + Now().FormatString("yyyy-mm-dd - hh-nn-ss") + "-" + DeviceName + LOGFILE);
	Printout->PrintToStream(ZedToArchive);
}

void __fastcall TfrmAnalysis::ReportXReport()
{
    TTransactionInfoProcessor::Instance().RemoveEntryFromMap(TDeviceRealTerminal::Instance().ID.Name);
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
	Processing->Message = "Please Wait...";
	Processing->Show();
	DBTransaction.StartTransaction();
	ReportManager reportManager;
	XReport* xReport = reportManager.GetXReport(&TGlobalSettings::Instance(), &DBTransaction);
	xReport->DisplayAndPrint();
	DBTransaction.Commit();
	Processing->Close();
}

const TMMContactInfo &TfrmAnalysis::GetLastAuthenticatedUser()
{
	return lastAuthenticatedUser;
}

TPrintout* TfrmAnalysis::SetupPrintOutInstance()
{
	bool printerExists = true;

	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		printerExists = false;
	}

	TPrintout* printOut = new TPrintout;

	if (!printerExists)
	{
		TPrinterPhysical defaultScreenPrinter;
		defaultScreenPrinter.NormalCharPerLine = 40;
		defaultScreenPrinter.BoldCharPerLine = 40;

		printOut->Printer = defaultScreenPrinter;
	}
	else
	{
		printOut->Printer = TComms::Instance().ReceiptPrinter;
	}

	printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
	printOut->PrintFormat->Line->ColCount = 1;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
	printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;

	return printOut;
}
// ------------------------------------------------------------------------------
void __fastcall TfrmAnalysis::btnZReportClick(void)
{
    bool canGoBeyond = RestartFireBirdService();
    if(!canGoBeyond)
       return;

    if(!TDeviceRealTerminal::Instance().OpenDatabases())
    {
        MessageBox("Till not closed at this time. \r"
           "Please write down and report the following message to your service provider. \r\r ", "Error",
            MB_OK + MB_ICONERROR);
		zedLogsList->Clear();
		zedLogsList->Add("Error while opening Database.");
        zedLogsList->Add("Till not closed at this time. Please write down and report the following message to your service provider.");
        MakeZEDLogFile(zedLogsList);
        return;
    }
    zedLogsList->Clear();
    zedLogsList->Add("-----------------------------Zed report button clicked.--------------------------------------------------------------" );
    zedLogsList->Add("ZED starts at: " + Now().FormatString("dd/mm/yy hh:nn:ss" ));
    ZedToArchive->Clear();
    ZedToArchive->Position = 0;
    int Zedkey = 0;
    TCashDenominationControllerInterface::Instance()->ResetCashDenominations();
    // call to new class to get orders of DC and bill them off by storing
    if(TGlobalSettings::Instance().DrinkCommandServerPort != 0 && TGlobalSettings::Instance().DrinkCommandServerPath.Length() != 0
      && TGlobalSettings::Instance().IsDrinkCommandEnabled)
    {
        TMMProcessingState State(Screen->ActiveForm, "Handling DC Sessions", "Please Wait");
        TDeviceRealTerminal::Instance().ProcessingController.Push(State);
        std::auto_ptr<TDCBillProcessingAtZed> dcBill(new TDCBillProcessingAtZed());
        dcBill->FetchAllDCOpenSessions();
        TDeviceRealTerminal::Instance().ProcessingController.Pop();
    }
    zedLogsList->Add("After Drink command code execution." );
	TGlobalSettings &gs = TGlobalSettings::Instance();
	ZedCancel    = false;
	ZedCompleted = false;
    int _key =0;
    //int ResetKey;
	bool PrinterExists = true;

	if (AuthenticateReportsAccess(TReportSource::PrintZed) != lsAccepted)
	return;
	else if (gs.StopIfOpenTableAndTabs && are_any_tables_or_tabs_outstanding())
	{
		MessageBox("Zed unable to run. There are currently open tables or tabs. Please close these first.", "Stop Action", MB_OK + MB_ICONINFORMATION);
		return;
	}
	else if(gs.WarnIfOpenTableAndTabs && are_any_tables_or_tabs_outstanding())
	{
		if (MessageBox("There are open tabs. Are you sure you want to close the till?", "Close the till?", MB_YESNO | MB_ICONWARNING) == ID_NO)
		return;
	}
	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		PrinterExists = false;
	}

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

	if (!PrinterExists)
	{
		MessageBox("A printer has not been selected. " "Choose a receipt printer from [Maintenance][Interfaces] \r"
		"[Printer Maintenance][Setup Receipt Docket Format] \r" "This terminal is NOT closed!", "Error", MB_OK + MB_ICONWARNING);
	}
	else
	{
		bool UpdateingStock = false;
        if(TGlobalSettings::Instance().MallIndex == 7)
        {
            TIBSQL *IBSelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
            IBSelectQuery->Close();
            IBSelectQuery->SQL->Text = "SELECT A.ME_HOURLY_KEY "
                                          "FROM MALLEXPORT_HOURLY A";

            IBSelectQuery->ExecQuery();

            if(!IBSelectQuery->RecordCount)
            {
                MessageBox("Zed Cannot be Processed as there is No Sales Data ", "Zed Processing",MB_OK + MB_ICONERROR);
                return;
            }

        }
		std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
		Processing->Message = "Please Wait...";
		Processing->Show();

        std::vector<TXeroInvoiceDetail> XeroInvoiceDetails;
        std::vector<TMYOBInvoiceDetail> MYOBInvoiceDetails;
        UnicodeString DeviceName = GetTerminalName();
        zedLogsList->Add("Processing zed for" + DeviceName + " Terminal");
        bool CompleteZed;

		try
		{
			TBlindBalances Balances;
			AnsiString BagID;
			TStaffHoursInterface StaffHours;
			TCommissionCache Commission;
			TPrinterReadingsInterface PrinterReading;
			TPaxCount PaxCount;
			int z_key;

			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT MAX(Z_KEY) Z_KEY FROM ZEDS";
			IBInternalQuery->ExecQuery();
			z_key = IBInternalQuery->Fields[0]->AsInteger;

		   if (TGlobalSettings::Instance().EnablePrinterCounts)
			{
                zedLogsList->Add("inside EnablePrinterCounts enable check ");
				TPrinterReadingController PrinterReadings(this, DBTransaction);
				PrinterReadings.Run(z_key);
				PrinterReading = PrinterReadings.Get();
                zedLogsList->Add("After EnablePrinterCounts enable check ");
			}

			if (TGlobalSettings::Instance().EnablePaxCount)
			{
                zedLogsList->Add("inside EnablePaxCount enable check ");
				TPaxCountController PaxCountController(this, DBTransaction);
				PaxCountController.Run();
				PaxCountController.Show();
				PaxCount = PaxCountController.Get();
                zedLogsList->Add("After EnablePaxCount enable check ");
			}
            TTransactionInfo TransactionInfo;
            TTransactionInfoProcessor::Instance().RemoveEntryFromMap(DeviceName);
            zedLogsList->Add("Befor calling  GetTransactionInfo(DBTransaction, DeviceName)");
            TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfo(DBTransaction, DeviceName);
            zedLogsList->Add("After calling  GetTransactionInfo(DBTransaction, DeviceName)");

            Processing->Close();
            //Processing->Refresh(); // add to refresh...
			Processing->Message = "Processing...";
			Processing->Show();

			DataCalculationUtilities* dataCalculationUtilities;
            TDateTime trans_date = dataCalculationUtilities->CalculateSessionTransactionDate(Now());
			ReportManager reportManager;
			ZedReport* zedReport = reportManager.GetZedReport(&TGlobalSettings::Instance(), &DBTransaction);
            zedLogsList->Add("Before form show of close till.");
             CompleteZed = zedReport->DisplayAndPrint(ZedToArchive);

             zedLogsList->Add("After  form of close till processed and zed completed falg is " + CompleteZed);
			// Mall Export added variabnle
			ZedCompleted =  CompleteZed;
			ZedCancel    = !CompleteZed;
			if (CompleteZed)
			{
                zedLogsList->Add("If complete zed flag is true after processing of close till form");
                //Get terminal earning
                Currency TotalEarnings = dataCalculationUtilities->GetTotalEarnings(DBTransaction, DeviceName);
                zedLogsList->Add("After total earning calulation :");
                //Get Accumulated zed
                Currency AccumulatedZedTotal = dataCalculationUtilities->GetAccumulatedZedTotal(DBTransaction);
                AccumulatedZedTotal +=  TotalEarnings;
                zedLogsList->Add("After calculating Accumulated zed total: ");

              OpenCashDrawer();
              zedLogsList->Add("After opencash drawer");
              if(TGlobalSettings::Instance().EnableBlindBalances)
                {
                    zedLogsList->Add("Blind balance section start.");
                   Balances = TBlindBalanceControllerInterface::Instance()->GetBalances();
                   BagID = TBlindBalanceControllerInterface::Instance()->GetBagID();
                   zedLogsList->Add("Blind balance section end.");
                }

                //For MYOB And XERO
                if(TGlobalSettings::Instance().PostZToAccountingSystem && TGlobalSettings::Instance().IsXeroEnabled)
                {
                    zedLogsList->Add("zero section start.");
                    XeroInvoiceDetails = CalculateAccountingSystemData(DBTransaction);
                    zedLogsList->Add("zero section end.");
                }
                else if(TGlobalSettings::Instance().PostZToAccountingSystem && TGlobalSettings::Instance().IsMYOBEnabled)
                {
                    zedLogsList->Add("Myob section start.");
                    MYOBInvoiceDetails = CalculateMYOBData(DBTransaction, Balances);
                    zedLogsList->Add("Myob section ends.");
                }

Zed:
				TMallExportUpdateAdaptor exportUpdateAdaptor;
                zedLogsList->Add("Befor GetFinancialDetails(DBTransaction, TransactionInfo, DeviceName) starts ");
				TFinancialDetails FinancialDetails = GetFinancialDetails(DBTransaction, TransactionInfo, DeviceName);
                zedLogsList->Add("After GetFinancialDetails(DBTransaction, TransactionInfo, DeviceName)  ");
                zedLogsList->Add("Before UpdateExportTablesOnZed(FinancialDetails, &DBTransaction)  ");
				exportUpdateAdaptor.UpdateExportTablesOnZed(FinancialDetails, &DBTransaction);
                zedLogsList->Add("After UpdateExportTablesOnZed(FinancialDetails, &DBTransaction)  ");
				TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
				if(TGlobalSettings::Instance().EnableDontClearZedData)
				{
                    try
                    {
                        zedLogsList->Add("if EnableDontClearZedData setting is on ");
                        IBInternalQuery->Close();
                        IBInternalQuery->SQL->Text = "SELECT MAX(Z_KEY) Z_KEY FROM ZEDS";
                        IBInternalQuery->ExecQuery();
                        Zedkey = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;
                        BagID = "To Be Zed";
                        UpdateBlindBlances(DBTransaction, Zedkey, Balances, BagID);
                        UpdateCommissionDatabase(DBTransaction, Zedkey, Commission);
                        UpdatePrinterReadingsDatabase(DBTransaction, Zedkey, PrinterReading);
                        if(TGlobalSettings::Instance().EnablePaxCount)
                        UpdatePaxCountDatabase(DBTransaction, Zedkey, PaxCount);
                        if (TGlobalSettings::Instance().EnableBlindBalances)
                        PrintBlindBalance(DBTransaction, Balances, DeviceName);
                        DBTransaction.Commit();
                        return;
                    }
                    catch(Exception & E)
                    {
                        DBTransaction.Rollback();
                        zedLogsList->Add("Catch block of TGlobalSettings::Instance().EnableDontClearZedData ");
                        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
                        throw;
                    }

				}
				else
				{
                    zedLogsList->Add("Else block if EnableDontClearZedData setting is off ");
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = "SELECT " "* FROM ZEDS " "WHERE " "TERMINAL_NAME = :TERMINAL_NAME AND " "TIME_STAMP IS NULL";
					IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
					IBInternalQuery->ExecQuery();
					if (IBInternalQuery->RecordCount == 0)
					{
                        zedLogsList->Add("if zeds table contains no record having time stamp as null for that terminal. ");
						int CurrentSecurityRef = TDBSecurity::GetNextSecurityRef(DBTransaction);
						TDBSecurity::ProcessSecurity(DBTransaction, CurrentSecurityRef,
						lastAuthenticatedUser.ContactKey, SecurityTypes[secZedded],
						lastAuthenticatedUser.Name,
						lastAuthenticatedUser.Initials, Now(), DeviceName);
						IBInternalQuery->Close();
						IBInternalQuery->SQL->Text = "SELECT MAX(Z_KEY) Z_KEY FROM ZEDS";
						IBInternalQuery->ExecQuery();
						int PrevZedKey = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;


						IBInternalQuery->Close();
						IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ZED, 1) FROM RDB$DATABASE";
						IBInternalQuery->ExecQuery();
						z_key = Zedkey = IBInternalQuery->Fields[0]->AsInteger;
                        int EMAIL_STATUS = 0;
						IBInternalQuery->Close();
                        zedLogsList->Add("After updating security table. Now will update zeds table.");                                                         //
						IBInternalQuery->SQL->Text =
						"INSERT INTO ZEDS ( Z_KEY, INITIAL_FLOAT, TERMINAL_NAME, TIME_STAMP, REPORT, SECURITY_REF, EMAIL_STATUS, TRANS_DATE, TERMINAL_EARNINGS, ZED_TOTAL ) "
						"VALUES (:Z_KEY, :INITIAL_FLOAT, :TERMINAL_NAME, :TIME_STAMP, :REPORT, :SECURITY_REF, :EMAIL_STATUS, :TRANS_DATE, :TERMINAL_EARNINGS, :ZED_TOTAL ); ";
						IBInternalQuery->ParamByName("Z_KEY")->AsInteger = Zedkey;
						IBInternalQuery->ParamByName("INITIAL_FLOAT")->AsCurrency = 0;
						IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
						IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
                        IBInternalQuery->ParamByName("TERMINAL_EARNINGS")->AsCurrency = TotalEarnings;
                        IBInternalQuery->ParamByName("ZED_TOTAL")->AsCurrency = AccumulatedZedTotal;
						ZedToArchive->Position = 0;

						IBInternalQuery->ParamByName("REPORT")->LoadFromStream( FormattedZed(ZedToArchive));
						IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = CurrentSecurityRef;
                        IBInternalQuery->ParamByName("EMAIL_STATUS")->AsInteger = EMAIL_STATUS;
                        IBInternalQuery->ParamByName("TRANS_DATE")->AsDateTime = trans_date;
						IBInternalQuery->ExecQuery();
                         zedLogsList->Add("After updating zeds table.");
						UpdateBlindBlances(DBTransaction, Zedkey, Balances, BagID);
						UpdateCommissionDatabase(DBTransaction, Zedkey, Commission);
						UpdatePrinterReadingsDatabase(DBTransaction, Zedkey, PrinterReading);
						if (TGlobalSettings::Instance().EnablePaxCount)
					     	UpdatePaxCountDatabase(DBTransaction, Zedkey, PaxCount);
                        _key = Zedkey;
                        UpdateCancelItemsTable();
                        ///changes here for staff hour...
                        UpdateZedStaffHoursEnable(DBTransaction, Zedkey);
					}
					else
					{
                        zedLogsList->Add("if zeds table contains record having time stamp as null for that terminal. ");
						Zedkey = z_key = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;
						int CurrentSecurityRef = IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger;

						TDBSecurity::ProcessSecurity(DBTransaction, CurrentSecurityRef,
						lastAuthenticatedUser.ContactKey, SecurityTypes[secZedded],
						lastAuthenticatedUser.Name,
						lastAuthenticatedUser.Initials, Now(), DeviceName);

                        zedLogsList->Add("After updating security table. Now will update zeds table.");
						IBInternalQuery->Close();
						IBInternalQuery->SQL->Text =
						"UPDATE ZEDS " "SET " "TIME_STAMP	= :TIME_STAMP, " "REPORT	= :REPORT, " "TERMINAL_EARNINGS = :TERMINAL_EARNINGS, " "ZED_TOTAL = :ZED_TOTAL , TRANS_DATE = :TRANS_DATE  " "WHERE " "Z_KEY = :Z_KEY";
						IBInternalQuery->ParamByName("Z_KEY")->AsInteger = Zedkey;
						ZedToArchive->Position = 0;
						IBInternalQuery->ParamByName("REPORT")->LoadFromStream( FormattedZed(ZedToArchive));
						IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
                        IBInternalQuery->ParamByName("TRANS_DATE")->AsDateTime = trans_date;
                        IBInternalQuery->ParamByName("TERMINAL_EARNINGS")->AsCurrency = TotalEarnings;
                        IBInternalQuery->ParamByName("ZED_TOTAL")->AsCurrency = AccumulatedZedTotal;
						IBInternalQuery->ExecQuery();
                        zedLogsList->Add("After updating zeds table.");
						UpdateBlindBlances(DBTransaction, Zedkey, Balances, BagID);
						UpdateCommissionDatabase(DBTransaction, Zedkey, Commission);
						UpdatePrinterReadingsDatabase(DBTransaction, Zedkey, PrinterReading);
                        ///changes here for staff hour...
                        UpdateZedStaffHoursEnable(DBTransaction, Zedkey);

					}
				}
                TManagerChitNumber::Instance().ResetChitNumber(DBTransaction);
                zedLogsList->Add("After chit reset. ");
                Processing->Message = "Processing End Of Day...";
                UpdateSalesForce(); //update sales force
                ClearParkedSale(DBTransaction); // clear parked sale
                Processing->Close();
				Processing->Message = "Updating Clock...";
				Processing->Show();

				TDeviceRealTerminal::Instance().UpdateClockSyncInfo();
                zedLogsList->Add("After updating UpdateClockSyncInfo ");
                Processing->Close();
				Processing->Message = "Updating Archives...";
				Processing->Show();
                zedLogsList->Add("Before calling UpdateArchive ");
                UpdateArchive(IBInternalQuery, DBTransaction, DeviceName, Zedkey);  // update archive..
                zedLogsList->Add("After calling UpdateArchive and it was executed. ");
                Processing->Close();
				Processing->Message = "Archiving Report...";
				Processing->Show();
                Processing->Close();
				Processing->Message = "Updating Stock...";
			  	Processing->Show();
                zedLogsList->Add("Before update stock ");
                UpdateStock(UpdateingStock);  //update stock.
                zedLogsList->Add("After update stock ");
			}
            Processing->Close();
			Processing->Message = "Committing Data...";
			Processing->Show();

			if (CompleteZed)
            {
                zedLogsList->Add("Inside Zed completed flag");
			   DefaultItemQuantities(DBTransaction);
               zedLogsList->Add("After  DefaultItemQuantities execution");
               UpdateContactTimeZedStatus(DBTransaction);
               if(TGlobalSettings::Instance().CashDenominationEntry)
                {
                  TCashDenominationControllerInterface::Instance()->SaveDenominations(DBTransaction,z_key,DeviceName);
                  zedLogsList->Add("After  Cashdenomination execution");
                }
            }
            //create CSV
            if(TGlobalSettings::Instance().IsEnabledPeachTree && CompleteZed)
            {
                TExportCSV exportCSV;
                exportCSV.PostDateToCSV();
            }

			DBTransaction.Commit();
             zedLogsList->Add("Transaction committed.");
            PostDataToXeroAndMyOB(XeroInvoiceDetails, MYOBInvoiceDetails, CompleteZed); //post data to xero and Myob

            Processing->Close();
            //Processing->Refresh();
			Processing->Message = "Completed.";
		    Processing->Show();
			if(CompleteZed)
			{
                //MM-6401 : Block to reset points.
                bool resetPoint = TGlobalSettings::Instance().PointPurchased || TGlobalSettings::Instance().PointEarned || TGlobalSettings::Instance().PointRedeem ;
                if(TGlobalSettings::Instance().ConfirmResetPoint)
                {
                    if (MessageBox("Do you want to reset points now?", "Reset Point", MB_YESNO | MB_ICONWARNING) == ID_YES)
                    {
                        resetPoint = true;
                    }
                    else
                    {
                        resetPoint = false;
                    }
                }
                if(resetPoint)
                {
                    Processing->Message = "Resetting Points";
                    Processing->Refresh();
                    ResetPoints();
                }
                if(TGlobalSettings::Instance().EmailZedReports)
                {

                      //Processing->Refresh();
                      Processing->Close();
                      Processing->Message = "Sending Emails...";
                      Processing->Show();
                      EmailZedReport(z_key);
                      //Processing->Refresh();
                      Processing->Close();
                      Processing->Message = "Completed";
                      Processing->Show();
                      zedLogsList->Add("Z report emailed.");
                }
            }

			if(CompleteZed)
			{
                UpdateMallExportDetails();

                //Method for mall Design According to newly pattern

                if(TGlobalSettings::Instance().mallInfo.MallId)
                {
                    bool isMasterterminal = TGlobalSettings::Instance().EnableDepositBagNum;
                    if(TGlobalSettings::Instance().mallInfo.MallId == 1)
                    {
                        UpdateZKeyForMallExportSales(isMasterterminal, 33);
                    }
                    else if(TGlobalSettings::Instance().mallInfo.MallId == 2 || TGlobalSettings::Instance().mallInfo.MallId == 3)
                    {
                        isMasterterminal = true;
                        UpdateZKeyForMallExportSales(isMasterterminal, 19);
                        if(TGlobalSettings::Instance().mallInfo.MallId == 3)
                            UpdateStallCodeForEviaMall(2);
                    }
                    //Instantiation is happenning in a factory based on the active mall in database
                    TMallExport* mallExport = TMallFactory::GetMallType();
                    mallExport->Export();
                    delete mallExport;
                }

                //Settle th adyen eftpos bills
                if(TGlobalSettings::Instance().EnableEftPosAdyen)
                {
                    SettleEFTPOSBills();
                }
            }
      }
      catch(Exception & E)
      {
            DBTransaction.Rollback();
            if (UpdateingStock)
            {
                Database::TDBTransaction TransCloseStock(TDeviceRealTerminal::Instance().DBControl);
                TransCloseStock.StartTransaction();
                UpdateStockComplete(TransCloseStock);
                TransCloseStock.Commit();
            }
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
            TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
            MessageBox("Till not closed at this time. \r"
           "Please write down and report the following message to your service provider. \r\r " + E.Message, "Error",
            MB_OK + MB_ICONERROR);
            zedLogsList->Add("Till not closed at this time. Please write down and report the following message to your service provider.");
            MakeZEDLogFile(zedLogsList);
      }
        if(CompleteZed)
        {
            // For Mall Export
            SyncCompanyDetails();
            // For Mall Export
            UpdateDLFMall();
            zedLogsList->Add("Z Completed at " + Now().FormatString("dd/mm/yy hh:nn:ss" ));
            MakeZEDLogFile(zedLogsList);

        }
        frmSecurity->LogOut();
        Processing->Close();
	}
}

 void TfrmAnalysis::SaveCompValueinDBStrUnique(vmVariables vmVar, UnicodeString CompName)
{    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    TManagerVariable::Instance().SetProfileStr(DBTransaction, TManagerVariable::Instance().DeviceProfileKey, vmVar, CompName);
    DBTransaction.Commit();}

 void TfrmAnalysis::CompleteDLFMallExport()
{            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 0) FROM RDB$DATABASE";
            IBInternalQuery->ExecQuery();
            int	InvoiceNumber = IBInternalQuery->Fields[0]->AsInteger;
            DBTransaction.Commit();
            AnsiString cmd_code= "1";
            AnsiString file_stat ="Closed";                                        //length 6      //OPENED  OR CLOSED
            AnsiString tenant_No = TGlobalSettings::Instance().TenantNo;      //length 18
            AnsiString pos_No = TDeviceRealTerminal::Instance().ID.Name;      //length 6
            AnsiString Receipt_No=InvoiceNumber;                                     //length 10
            int Tran_File_No=TGlobalSettings::Instance().BatchNo;      //length 4
            AnsiString date = Now().FormatString("yyyymmdd");               //length 8
            AnsiString time = Now().FormatString("hh:mm:ss");               //length 8
            AnsiString user_ID=TDeviceRealTerminal::Instance().User.Name;     //length 8
            AnsiString Sale_date=Now().FormatString("yyyymmdd");                                         //length 8

            AnsiString  finalValue= cmd_code+"|" +file_stat+"|"+tenant_No+"|"+pos_No+"|"+Receipt_No+"|"+ Tran_File_No+ "|" +
            date  +"|" +time+"|" +user_ID+"|" +Sale_date;
            if(TGlobalSettings::Instance().DLFMallFileName!=""&&TGlobalSettings::Instance().DLFMallFileName!= "[null]" )
            {
                AnsiString mallPath = TGlobalSettings::Instance().MallPath;
                AnsiString LocalPath = mallPath + "\\DLFMall\\" +  TGlobalSettings::Instance().DLFMallFileName;

                TStrings * List = new TStringList();
                if (FileExists(LocalPath ))
                {
                   List->LoadFromFile(LocalPath );
                }
                else
                {

                }
            List->Add(finalValue);
            List->SaveToFile(LocalPath );
            delete List;

            }
            UnicodeString HostName = TGlobalSettings::Instance().FTPServer;
            UnicodeString UserPath = TGlobalSettings::Instance().FTPPath;
            UnicodeString UserName = TGlobalSettings::Instance().FTPUserName;
            UnicodeString UserPassword = TGlobalSettings::Instance().FTPPassword;


            UnicodeString LocalPathFileName= TGlobalSettings::Instance().MallPath+ "\\DLFMall\\"+TGlobalSettings::Instance().DLFMallFileName; //"C:/trip.txt";//
            UnicodeString LocalFileName=TGlobalSettings::Instance().DLFMallFileName;
            int FCount=0;
            const char * hostName = HostName.t_str();
            const char * userPath = UserPath.t_str();
            const char * userName = UserName.t_str();
            const char * userPassword = UserPassword.t_str();
            FileSubmit(hostName, userName, userPassword, userPath, LocalPathFileName, LocalFileName, FCount);
}
//---------------------------------------------------------------------------
// This method is used to send the file from the local directory to the FTP server
// provided by the client
void TfrmAnalysis::FileSubmit(const char * hostName, const char * userName,
                                           const char * userPassword, const char * userPath,
                                           UnicodeString LocalPathFileName, UnicodeString LocalFileName,
                                           int FCount)
{
    const char * pathFileName = LocalPathFileName.t_str();
    const char * fileName = LocalFileName.t_str();

    HINTERNET hInternet;
    HINTERNET hFtpSession;
    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL)
    {
        if (FCount == 0)
        {
            MessageBox( "Connection to FTP Server failed. Please check internet connection", "Connection Failed!", MB_OK );
        }
    }
    else
    {
        hFtpSession = InternetConnect(hInternet, hostName, INTERNET_DEFAULT_FTP_PORT, userName, userPassword, 1, INTERNET_FLAG_PASSIVE, 0);
        if (hFtpSession == NULL)
        {
            if (FCount == 0)
            {
                MessageBox( "Cannot connect to FTP Server. Please try again!", "Connection to FTP Host Unsuccessful.", MB_OK );
            }
        }
        else
        {
            // Changing dicrectory is necessary
            FtpSetCurrentDirectory( hFtpSession, userPath );
            //  FtpCreateDirectory( hFtpSession, "/Menumate/Beta/PosDroid/DLF" );

            if (!FtpPutFile(hFtpSession, pathFileName, fileName, FTP_TRANSFER_TYPE_BINARY, 0))
            {

                if (FCount == 0)
                {

                    MessageBox( "File was not successfully uploaded!", "File Transfer Failed!", MB_OK );
                }
            }

            if (FCount == 0)
            {
                MessageBox( "File was sent successfully!", "File Transfer Success!", MB_OK );
            }
        }
        InternetCloseHandle(hFtpSession);
    }
}
//---------------------------------------------------------------------------
/***********************DLF MALL END*********************************/
std::vector<TXeroInvoiceDetail> TfrmAnalysis::CalculateAccountingSystemData(Database::TDBTransaction &DBTransaction)
{
    try
    {
        std::vector<TXeroInvoiceDetail> XeroInvoiceDetails;
        TDateTime preZTime = GetPrevZedTime(DBTransaction);
        TDateTime currentDate = Now();
        TDateTime nextDay = Now();                                                //IncDay(GetMinDayArchiveTime(DBTransaction, preZTime),1);
        AnsiString CompanyName = GetCompanyName(DBTransaction);
        AnsiString TerminalName = GetTerminalName();
        AnsiString terminalNamePredicate = "";
        double cashWithdrawal = 0;
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBInternalQueryTotal = DBTransaction.Query(DBTransaction.AddQuery());
         TIBSQL *IBInternalQueryTabRefundCredit = DBTransaction.Query(DBTransaction.AddQuery());

         TIBSQL *IBInternalQuerySurcharge =  DBTransaction.Query(DBTransaction.AddQuery());
        if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
        {

          terminalNamePredicate = " and a.TERMINAL_NAME = :TERMINAL_NAME ";
        }
        AnsiString paymentDetails = "";
        GetPaymentDetails(paymentDetails, terminalNamePredicate);
        IBInternalQuery->SQL->Text = paymentDetails;

        IBInternalQueryTotal->SQL->Text =   "Select e.CATEGORY,e.GL_CODE,CAST(f.TAX AS DECIMAL(18,2)) TAX,CAST((e.PRICE-f.TAX) AS DECIMAL(18,2)) PRICE  from "
                                            "( "
                                            "Select c.CATEGORY,c.GL_CODE, Sum(a.PRICE * a.QTY) PRICE from  DAYARCHIVE a "
                                            "left join ARCCATEGORIES c on a.CATEGORY_KEY = c.CATEGORY_KEY "
                                            "where a.ARCBILL_KEY in (Select distinct a.ARCBILL_KEY from DAYARCBILL a left join DAYARCBILLPAY b on a.ARCBILL_KEY = b.ARCBILL_KEY  "
                                            "where b.NOTE <> 'Total Change.' and b.CHARGED_TO_XERO <> 'T' and a.TIME_STAMP > :STARTTIME and  a.TIME_STAMP <= :ENDTIME  " + terminalNamePredicate + ") "
                                            "group by c.CATEGORY,c.GL_CODE  "
                                            ") e "
                                            "left join  "
                                            "(  "
                                            "Select c.CATEGORY,c.GL_CODE,Sum(a.TAX_VALUE) TAX from DAYARCORDERTAXES a "
                                            "left join DAYARCHIVE b on a.ARCHIVE_KEY = b.ARCHIVE_KEY "
                                            "left join ARCCATEGORIES c on b.CATEGORY_KEY = c.CATEGORY_KEY "
                                            "where b.ARCBILL_KEY in ( Select distinct a.ARCBILL_KEY from DAYARCBILL a left join DAYARCBILLPAY b on a.ARCBILL_KEY = b.ARCBILL_KEY  "
                                            "where b.NOTE <> 'Total Change.' and b.CHARGED_TO_XERO <> 'T' and a.TIME_STAMP > :STARTTIME and  a.TIME_STAMP <= :ENDTIME  " + terminalNamePredicate + ") "
                                            "group by c.CATEGORY,c.GL_CODE "
                                            ") f "
                                            "on e.CATEGORY = f.CATEGORY ";

   IBInternalQuerySurcharge->SQL->Text =   "Select SUM(SUBTOTAL) SUBTOTAL FROM  DAYARCSURCHARGE ds  WHERE ds.ARCBILL_KEY "
                                            "in (Select distinct a.ARCBILL_KEY from DAYARCBILL a inner join DAYARCBILLPAY b "
                                            "on a.ARCBILL_KEY = b.ARCBILL_KEY "
                                            "where b.CHARGED_TO_XERO <> 'T' "
                                            "and a.TIME_STAMP > :STARTTIME and  a.TIME_STAMP <= :ENDTIME " + terminalNamePredicate + " group by a.ARCBILL_KEY) "
                                            "and ds.PROPERTIES like '%15%' ";




       IBInternalQuerySurcharge->ParamByName("STARTTIME")->AsDateTime = preZTime;
       IBInternalQuerySurcharge->ParamByName("ENDTIME")->AsDateTime   =  nextDay;
       if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
       {

            IBInternalQuerySurcharge->ParamByName("TERMINAL_NAME")->AsString = TerminalName;  // add terminal param..

        }

        IBInternalQuerySurcharge->Close();
        IBInternalQuerySurcharge->ExecQuery();

        bool canContinue = true;

        double TabCreditReceived = 0;
        double TabRefundReceived = 0;
        double catTotal = 0;
        double payTotal = 0;
        double floatAmount = 0;
        double PurchasedPoints = 0;
        double PurchasedVoucher = 0;
        double PurchasedGiftCard = 0;
        double TipAmount = 0;
        double tip = 0.0;
        double SurchargeAmount=0;
        UnicodeString floatGlCode = "";
        UnicodeString tabCreditReceivedGLCode = "200";
        UnicodeString tabCreditRefundGlCode = "200";

        TXeroInvoiceDetail XeroInvoiceDetail;
        IBInternalQuery->Close();
        IBInternalQueryTotal->Close();
        IBInternalQueryTabRefundCredit->Close();
        IBInternalQuery->ParamByName("STARTTIME")->AsDateTime = preZTime;
        IBInternalQuery->ParamByName("ENDTIME")->AsDateTime = nextDay;
        IBInternalQueryTotal->ParamByName("STARTTIME")->AsDateTime = preZTime;
        IBInternalQueryTotal->ParamByName("ENDTIME")->AsDateTime = nextDay;
        if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
        {
            IBInternalQueryTotal->ParamByName("TERMINAL_NAME")->AsString = TerminalName;  // add terminal param..
            IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
        }
        IBInternalQueryTotal->ExecQuery();

        for (; !IBInternalQueryTotal->Eof; IBInternalQueryTotal->Next())
        {
          AnsiString AccountCode = "200";
          if(IBInternalQueryTotal->FieldByName("GL_CODE")->AsString != NULL && IBInternalQueryTotal->FieldByName("GL_CODE")->AsString != "")
            AccountCode = IBInternalQueryTotal->FieldByName("GL_CODE")->AsString;
          catTotal += IBInternalQueryTotal->FieldByName("PRICE")->AsFloat + IBInternalQueryTotal->FieldByName("TAX")->AsFloat;
          if(IBInternalQueryTotal->FieldByName("PRICE")->AsFloat != 0)
           {
             AddInvoiceItem(XeroInvoiceDetail,IBInternalQueryTotal->FieldByName("CATEGORY")->AsString, IBInternalQueryTotal->FieldByName("PRICE")->AsFloat
                            , AccountCode, IBInternalQueryTotal->FieldByName("TAX")->AsFloat);
           }
        }

        GetTabCreditReceivedRefunded(DBTransaction, TabCreditReceived, TabRefundReceived, preZTime, nextDay);

        TabCreditReceived = RoundTo(TabCreditReceived, -2);
        TabRefundReceived = RoundTo(TabRefundReceived, -2);

        if(TGlobalSettings::Instance().TabDepositCreditRefundedGLCode != NULL || TGlobalSettings::Instance().TabDepositCreditRefundedGLCode != "")
        {
            tabCreditRefundGlCode = TGlobalSettings::Instance().TabDepositCreditRefundedGLCode;
        }
        if(TGlobalSettings::Instance().TabDepositCreditReceivedGLCode != NULL || TGlobalSettings::Instance().TabDepositCreditReceivedGLCode != "")
        {
            tabCreditReceivedGLCode = TGlobalSettings::Instance().TabDepositCreditReceivedGLCode;
        }
        if(TabRefundReceived!= 0)
        {
            catTotal -= TabRefundReceived;
            AddInvoiceItem(XeroInvoiceDetail,"Tab Deposit/Credit  Refunded", -1 * TabRefundReceived,tabCreditRefundGlCode,0);
        }
        if(TabCreditReceived != 0)
        {
            catTotal -= TabCreditReceived;
            AddInvoiceItem(XeroInvoiceDetail,"Tab Deposit/Credit Received", -1 * TabCreditReceived,tabCreditReceivedGLCode,0);
        }

        GetPointsAndVoucherData(DBTransaction,PurchasedPoints,PurchasedVoucher,preZTime,nextDay, PurchasedGiftCard);
        
        if(PurchasedPoints != 0)
        {
          catTotal += PurchasedPoints;
          AddInvoiceItem(XeroInvoiceDetail,"Points Purchased", PurchasedPoints,TGlobalSettings::Instance().PointsPurchasedGLCode,0);
        }
        if(PurchasedVoucher != 0)
        {
          catTotal += PurchasedVoucher;
          AddInvoiceItem(XeroInvoiceDetail,"Voucher Purchased", PurchasedVoucher,TGlobalSettings::Instance().VoucherPurchasedGLCode,0);
        }
		
		if(PurchasedGiftCard != 0)
        {
          catTotal += PurchasedGiftCard;
          AddInvoiceItem(XeroInvoiceDetail,"Gift Card Purchased", PurchasedGiftCard,TGlobalSettings::Instance().GiftCardGLCode,0);
        }

        bool addFloatAdjustmentToPayments = false;
        bool addEachPaymentNode = true;
        // get the final float amount (Float Deposit - float withdrwal)
        GetFloatAmounts(DBTransaction, floatAmount,terminalNamePredicate);
        AnsiString cashGlCode= GetCashGlCode(DBTransaction);
        floatGlCode = TGlobalSettings::Instance().FloatGLCode;

        floatAmount = RoundTo(floatAmount, -2);
        catTotal = RoundTo(catTotal, -2);

        if(floatGlCode != "" && floatGlCode != NULL && floatAmount != 0)
        {
             if(floatAmount<0)
             {
                    AddInvoiceItem(XeroInvoiceDetail,"Float WithDrawal", floatAmount, floatGlCode, 0);
             }
             else
                  AddInvoiceItem(XeroInvoiceDetail,"Float Deposit", floatAmount, floatGlCode, 0);

             if(TGlobalSettings::Instance().PostMoneyAsPayment)
             {
                    // if cash withdrawn is greator than all categories total then add one node in item having value equal to difference btw them
                    if((floatAmount + catTotal ) < 0)
                    {
                       AddInvoiceItem(XeroInvoiceDetail,"Float Adjustment",(-1)* (floatAmount + catTotal ), cashGlCode, 0);
                       addEachPaymentNode = false;
                    }
                    else
                    {
                       addFloatAdjustmentToPayments = true;
                       addEachPaymentNode = true;
                    }
              }
             else
              {
                     // in case of when payment is post to glcodes and if float amount is not equal to zero then make float adjustment against it
                   AddInvoiceItem(XeroInvoiceDetail,"Float Adjustment",-1 * floatAmount, cashGlCode, 0);
              }
        }
        if(TGlobalSettings::Instance().FloatWithdrawFromCash && !TGlobalSettings::Instance().PostMoneyAsPayment)
        {
            cashWithdrawal = 0;
            cashWithdrawal = GetCashWithdrawal(DBTransaction);
            UnicodeString glCodecashWithdrawal = "";
            if(TGlobalSettings::Instance().CashWithdrawalGLCode.Trim().Length() == 0 || TGlobalSettings::Instance().CashWithdrawalGLCode == NULL)
                glCodecashWithdrawal = "416";
            else
                glCodecashWithdrawal = TGlobalSettings::Instance().CashWithdrawalGLCode;

            if(cashWithdrawal != 0)
            {
                AddInvoiceItem(XeroInvoiceDetail,"Cash WithDrawal", RoundTo((cashWithdrawal),-2),glCodecashWithdrawal, 0);
            }
        }

        IBInternalQuery->ExecQuery();
        double cashAmount = 0.0;
        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
          if(IBInternalQuery->FieldByName("Amount")->AsFloat != 0)
           {
              AnsiString AccountCode = cashGlCode;
              TPayment payment;
              if(IBInternalQuery->FieldByName("GL_CODE")->AsString != NULL && IBInternalQuery->FieldByName("GL_CODE")->AsString != "")
                AccountCode = IBInternalQuery->FieldByName("GL_CODE")->AsString;
              if(IBInternalQuery->FieldByName("PAY_TYPE")->AsString == "Points")
                 AccountCode = TGlobalSettings::Instance().PointsSpentGLCode;
              payment.SetPaymentAttribute(ePayTypeCash);
              payTotal += IBInternalQuery->FieldByName("Amount")->AsFloat;

              if( TGlobalSettings::Instance().PostMoneyAsPayment)
              {
                 if(addFloatAdjustmentToPayments && addEachPaymentNode)
                 {
                    if(IBInternalQuery->FieldByName("PROPERTIES")->AsString.Pos(payment.GetPropertyString()) != 0)
                        cashAmount += IBInternalQuery->FieldByName("Amount")->AsFloat;
                    else
                        AddInvoicePayment(XeroInvoiceDetail,IBInternalQuery->FieldByName("PAY_TYPE")->AsString,
                                            IBInternalQuery->FieldByName("Amount")->AsFloat ,AccountCode,0);
                 }
                 else if(!addFloatAdjustmentToPayments && addEachPaymentNode)
                 {
                        AddInvoicePayment(XeroInvoiceDetail,IBInternalQuery->FieldByName("PAY_TYPE")->AsString,
                                            IBInternalQuery->FieldByName("Amount")->AsFloat ,AccountCode,0);
                 }

              }
              else
              {
                double paymentAmount = IBInternalQuery->FieldByName("Amount")->AsFloat;
                if(IBInternalQuery->FieldByName("PAY_TYPE")->AsString == "Cash" && TGlobalSettings::Instance().FloatWithdrawFromCash &&
                   cashWithdrawal != 0)
                {
                    paymentAmount = paymentAmount + cashWithdrawal;
                }
                AddInvoiceItem(XeroInvoiceDetail,IBInternalQuery->FieldByName("PAY_TYPE")->AsString,
                             -1 * paymentAmount, AccountCode,0);
              }
           }

           if(IBInternalQuery->FieldByName("Tip")->AsFloat != 0)
            TipAmount += IBInternalQuery->FieldByName("Tip")->AsFloat;
        }

        //Add payment Tip for DPS EftPOS
        if(TipAmount != 0)
        {
            AddInvoiceItem(XeroInvoiceDetail,"EftPos Tip",TipAmount,TGlobalSettings::Instance().EftPosTipGLCode,0);
            catTotal += TipAmount;
        }
        // Get tips through paymnt type as Tip
        UnicodeString tipGLCode = "";
        tip = GetTipAmount(DBTransaction,preZTime,tipGLCode);
        if(tip > 0.0)
        {
            catTotal += tip;
            AddInvoiceItem(XeroInvoiceDetail,"TIP", RoundTo(tip, -2), tipGLCode,0);
        }
        if(addFloatAdjustmentToPayments && TGlobalSettings::Instance().PostMoneyAsPayment)
             AddInvoicePayment(XeroInvoiceDetail,"Cash", ( cashAmount + floatAmount) , cashGlCode,0);
        else if(!addFloatAdjustmentToPayments && TGlobalSettings::Instance().PostMoneyAsPayment && cashAmount > 0.0)
             AddInvoicePayment(XeroInvoiceDetail,"Cash", ( cashAmount) , cashGlCode,0);

           //   SurchargeAmount = IBInternalQuerySurcharge->FieldByName("SUBTOTAL")->AsFloat;


        bool isSurchargeGLCodePresent = false;
        if(TGlobalSettings::Instance().SurchargeGLCode !=""  &&  TGlobalSettings::Instance().SurchargeGLCode !=NULL &&  TGlobalSettings::Instance().SurchargeGLCode !=0)
        {
           isSurchargeGLCodePresent = true;
           if(IBInternalQuerySurcharge->FieldByName("SUBTOTAL")->AsFloat !=0)
            {
                SurchargeAmount = IBInternalQuerySurcharge->FieldByName("SUBTOTAL")->AsFloat;
                AddInvoiceItem(XeroInvoiceDetail,"Surcharge",SurchargeAmount,TGlobalSettings::Instance().SurchargeGLCode,0);
            }
        }
        if(isSurchargeGLCodePresent)
        {
           catTotal += SurchargeAmount;
        }

        if(catTotal - payTotal)
        {
          AddInvoiceItem(XeroInvoiceDetail,"ROUNDING",-1 * (catTotal - payTotal),TGlobalSettings::Instance().RoundingGLCode,0);
        }

        AnsiString daystr = Now().FormatString("ddmmyy") + " " + Now().FormatString("HHMMss") ;
        AnsiString refName = TGlobalSettings::Instance().EnableDepositBagNum ? CompanyName : TerminalName ;
        XeroInvoiceDetail.InvoiceReference = refName +" Sales";
        XeroInvoiceDetail.InvoiceNumber = refName + " " + daystr;
        XeroInvoiceDetail.InvoiceDate = GetMaxDayArchiveTime(DBTransaction);//preZTime;
        if(TerminalName != "")
            XeroInvoiceDetail.ContactName = TerminalName;
        else
            XeroInvoiceDetail.ContactName = TDeviceRealTerminal::Instance().User.Name + " " + TDeviceRealTerminal::Instance().User.Surname;

        if(XeroInvoiceDetail.XeroCategoryDetails.size() > 0 || XeroInvoiceDetail.XeroPayTypeDetails.size() > 0)
            XeroInvoiceDetails.push_back(XeroInvoiceDetail);

        if(double(nextDay) >= double(currentDate))
        {
            canContinue = false;
        }
        else
        {
            preZTime = nextDay;
            nextDay =  IncDay(nextDay,1);
        }
        return XeroInvoiceDetails;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void TfrmAnalysis::GetQueriesForMYOB(AnsiString &Tax,AnsiString &zeroTax,AnsiString terminalNamePredicate)
{
    try
    {
        AnsiString fetchCategory =
         " Select Sum(a.PRICE * a.QTY) PRICE,Sum(a.BASE_PRICE * a.QTY) BASEPRICE, Sum(a.DISCOUNT ) DISCOUNT ,c.GL_CODE "
         " from  DAYARCHIVE a "
         " left join ARCCATEGORIES c on a.CATEGORY_KEY = c.CATEGORY_KEY "
         " where a.ARCBILL_KEY in (Select distinct a.ARCBILL_KEY from DAYARCBILL a left join DAYARCBILLPAY b on a.ARCBILL_KEY = b.ARCBILL_KEY "
         " where b.NOTE <> 'Total Change.' and a.TIME_STAMP > :STARTTIME and  a.TIME_STAMP <= :ENDTIME  " + terminalNamePredicate + " ) " ;

         AnsiString nonZeroTaxFilter =
         " and a.ARCHIVE_KEY in (Select Archive_KEY FROM DAYARCORDERTAXES where TAX_VALUE <> 0 GROUP BY Archive_Key) " ;
         AnsiString ZeroTaxFilter =
         " and a.ARCHIVE_KEY in (Select Archive_KEY FROM DAYARCORDERTAXES where TAX_VALUE = 0 GROUP BY Archive_Key) and a.PRICE <> 0 " ;
         AnsiString groupBy =
         " group by c.GL_CODE ";

         Tax = fetchCategory + nonZeroTaxFilter + groupBy;
         zeroTax = fetchCategory + ZeroTaxFilter + groupBy;
     }
     catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void TfrmAnalysis::GetPaymentDetails(AnsiString &paymentDetails, AnsiString terminalNamePredicate)
{
    try
    {
        AnsiString paymentDetailsPrimitive =
             " SELECT  b.PAY_TYPE,c.PROPERTIES,cast( Sum(b.SUBTOTAL) as numeric(17,2)) Amount,cast(Sum(b.TIP_AMOUNT) as numeric(17,2)) Tip,c.GL_CODE  From DAYARCBILL a "
             " Left join DAYARCBILLPAY b on a.ARCBILL_KEY = b.ARCBILL_KEY "
             " Left join PAYMENTTYPES c on b.PAY_TYPE = c.PAYMENT_NAME "
             " where "
             " b.PAY_TYPE <> 'Credit'  and b.SUBTOTAL <> 0 "
             " and a.TIME_STAMP > :STARTTIME and  a.TIME_STAMP <= :ENDTIME  " ;

        AnsiString optionalXero = " and b.CHARGED_TO_XERO <> 'T'" ;
        AnsiString paymentDetailsFinal = ""
              + terminalNamePredicate +
             " group by b.PAY_TYPE,c.PROPERTIES ,c.GL_CODE ";
        if(TGlobalSettings::Instance().IsXeroEnabled)
          paymentDetails = paymentDetailsPrimitive + optionalXero + paymentDetailsFinal;
        else if(TGlobalSettings::Instance().IsMYOBEnabled)
          paymentDetails = paymentDetailsPrimitive + paymentDetailsFinal;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
//-------------------------------------------------------------------------------
AnsiString TfrmAnalysis::GetMYOBJobCode(Database::TDBTransaction &DBTransaction)
{
    UnicodeString text = "";
    UnicodeString key = "Software\\IQWorks\\MenuMate\\Office\\"+GetCompanyName(DBTransaction)+"\\MYOB";
    RegistryRead(key, "MYOBJobCode", text);
    return text;
}
//------------------------------------------------------------------------------
void TfrmAnalysis::CalculateNextday(TDateTime &nextDay)
{
    if(TGlobalSettings::Instance().EndOfDay > 0)
    {
       nextDay =   RecodeTime(nextDay,TGlobalSettings::Instance().EndOfDay,0,0,0);
    }
    else
    {
       nextDay =   RecodeTime(nextDay,23,59,59,999);
    }
}

std::vector<TMYOBInvoiceDetail> TfrmAnalysis::CalculateMYOBData(Database::TDBTransaction &DBTransaction, TBlindBalances Balances)
{
    try
    {
        std::vector<TMYOBInvoiceDetail> MYOBInvoiceDetails;
        std::map<AnsiString, AnsiString> CollectCategoryName;
        std::map<AnsiString, AnsiString>::iterator ii;
        AnsiString jobCode = GetMYOBJobCode(DBTransaction);
        TDateTime preZTime = GetPrevZedTime(DBTransaction);
        TDateTime currentDate = Now();
        TDateTime nextDay = IncDay(GetMinDayArchiveTime(DBTransaction, preZTime),1);
        AnsiString CompanyName = GetCompanyName(DBTransaction);
        AnsiString TerminalName = GetTerminalName();
        AnsiString terminalNamePredicate = "";
        CalculateNextday(nextDay);
        TIBSQL *IBInternalQueryTabRefundCredit = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBInternalQueryTax = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBInternalQueryZeroTax = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL *IBInternalQueryGenerator = DBTransaction.Query(DBTransaction.AddQuery());

        GetCategoryNameAndGLCode(DBTransaction, CollectCategoryName, preZTime, nextDay);

        if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
        {
            terminalNamePredicate = " and a.TERMINAL_NAME = :TERMINAL_NAME ";  // add terminal filter for xero invoice.
        }
        AnsiString categoriesWithTax = "";
        AnsiString categoriesWithoutTax = "";
        GetQueriesForMYOB(categoriesWithTax,categoriesWithoutTax,terminalNamePredicate);

        IBInternalQueryTax->SQL->Text = categoriesWithTax;
        IBInternalQueryZeroTax->SQL->Text = categoriesWithoutTax;
        AnsiString paymentDetails = "";
        GetPaymentDetails(paymentDetails, terminalNamePredicate);
        IBInternalQuery->SQL->Text = paymentDetails;

        double cashBlindBalance = 0.00;
        double cashVariance = 0.00;

        //Get Cash Bline Balance
        if(TGlobalSettings::Instance().EnableBlindBalances)
            cashBlindBalance = GetCashBlindBalance(Balances);

       bool canContinue = true;
       IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_MYOBINVOICE_NUMBER, 1) FROM RDB$DATABASE ";
       IBInternalQueryGenerator->Close();
       IBInternalQueryGenerator->ExecQuery();
       int val = IBInternalQueryGenerator->Fields[0]->AsInteger;
       while(canContinue)
        {
          double TabCreditReceived = 0;
          double TabRefundReceived = 0;
          double catTotal = 0;
          double payTotal = 0;
          double floatAmount = 0;
          double PurchasedPoints = 0;
          double PurchasedVoucher = 0;
          double PurchasedGiftCard = 0;
          double TipAmount = 0;
          UnicodeString AccountCode = "";

          TMYOBInvoiceDetail MYOBInvoiceDetail;
          IBInternalQueryTax->Close();
          IBInternalQueryZeroTax->Close();
          IBInternalQueryTabRefundCredit->Close();
          IBInternalQuery->Close();

          IBInternalQueryTax->ParamByName("STARTTIME")->AsDateTime = preZTime;
          IBInternalQueryTax->ParamByName("ENDTIME")->AsDateTime = nextDay;
          IBInternalQueryZeroTax->ParamByName("STARTTIME")->AsDateTime = preZTime;
          IBInternalQueryZeroTax->ParamByName("ENDTIME")->AsDateTime = nextDay;
          IBInternalQuery->ParamByName("STARTTIME")->AsDateTime = preZTime;
          IBInternalQuery->ParamByName("ENDTIME")->AsDateTime = nextDay;
          if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
          {
             IBInternalQueryTax->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
             IBInternalQueryZeroTax->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
             IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName; // add terminal param..
          }
          IBInternalQueryTax->ExecQuery();
          IBInternalQueryZeroTax->ExecQuery();

          for (; !IBInternalQueryTax->Eof; )
          {
              AnsiString categoryName;
              AnsiString glCode = IBInternalQueryTax->FieldByName("GL_CODE")->AsString;
              ii = CollectCategoryName.find(glCode);
              if(ii != CollectCategoryName.end())
              {
                 categoryName = ii->second;
              }
              AnsiString taxStatus = "NonZeroTax";
              double price = 0.0;
              catTotal += IBInternalQueryTax->FieldByName("PRICE")->AsFloat;
              double discount = 0.0;
              price = RoundTo(IBInternalQueryTax->FieldByName("PRICE")->AsFloat, -2);
              discount = RoundTo(IBInternalQueryTax->FieldByName("DISCOUNT")->AsFloat, -2);

              //Check if category name is null then send "sales" in it.
              if(categoryName == "" || categoryName == NULL)
              {
                categoryName = "Sales";
              }

              if(TGlobalSettings::Instance().ReCalculateTaxPostDiscount)
              {
                  AddMYOBInvoiceItem(MYOBInvoiceDetail,glCode.Trim(),categoryName,price,0.0,jobCode,taxStatus);
              }
              else
              {
                  double actualDiscount = 0.0;
                  actualDiscount = discount;
                  if(actualDiscount > 0.0)
                  {
                    actualDiscount = actualDiscount * -1;
                  }
                  AddMYOBInvoiceItem(MYOBInvoiceDetail,glCode.Trim(),categoryName,(price-actualDiscount),0.0,jobCode,taxStatus);
                  if(actualDiscount != 0.0)
                  {
                     if(actualDiscount < 0.0)
                     {
                        actualDiscount = actualDiscount * -1;
                     }
                     AddMYOBInvoiceItem(MYOBInvoiceDetail,glCode.Trim(),"Discount without Tax",-actualDiscount,0.0,jobCode,"ZeroTax");
                  }
              }
              IBInternalQueryTax->Next();
          }

          for (; !IBInternalQueryZeroTax->Eof; )
          {
              AnsiString categoryName;
              AnsiString glCode = IBInternalQueryZeroTax->FieldByName("GL_CODE")->AsString;
              ii = CollectCategoryName.find(glCode);
              if(ii != CollectCategoryName.end())
              {
                 categoryName = ii->second;
              }

              AnsiString taxStatus = "ZeroTax";
              double taxRate = 0.0;
              double price = 0.0;
              double discount = 0.0;

              price = RoundTo(IBInternalQueryZeroTax->FieldByName("PRICE")->AsFloat, -2);
              catTotal += price;

              //Check if category name is null then send "sales" in it.
              if(categoryName == "" || categoryName == NULL)
              {
                categoryName = "Sales";
              }
              AddMYOBInvoiceItem(MYOBInvoiceDetail,glCode.Trim(),categoryName,price,0.0,jobCode,taxStatus);
              IBInternalQueryZeroTax->Next();
          }

          AnsiString tabCreditRefundGlCode   = "";
          AnsiString tabCreditReceivedGLCode = "";
          if(TGlobalSettings::Instance().TabDepositCreditRefundedGLCode != NULL || TGlobalSettings::Instance().TabDepositCreditRefundedGLCode != "")
          {
                tabCreditRefundGlCode = TGlobalSettings::Instance().TabDepositCreditRefundedGLCode;
          }
          if(TGlobalSettings::Instance().TabDepositCreditReceivedGLCode != NULL || TGlobalSettings::Instance().TabDepositCreditReceivedGLCode != "")
          {
                tabCreditReceivedGLCode = TGlobalSettings::Instance().TabDepositCreditReceivedGLCode;
          }
          GetTabCreditReceivedRefunded(DBTransaction, TabCreditReceived, TabRefundReceived, preZTime, nextDay);

          TabCreditReceived = RoundTo(TabCreditReceived, -4);
          TabRefundReceived = RoundTo(TabRefundReceived, -4);

          if(RoundTo((TabRefundReceived),-2)!= 0.00)
          {
            catTotal -= TabRefundReceived;
            AddMYOBInvoiceItem(MYOBInvoiceDetail,tabCreditRefundGlCode,"Tab Deposit/Credit  Refunded", -1 * RoundTo((TabRefundReceived),-2),0.0,jobCode,"ZeroTax");
         }
          if(RoundTo((TabCreditReceived),-2) != 0.00)
          {
             catTotal -= TabCreditReceived;
             AddMYOBInvoiceItem(MYOBInvoiceDetail,tabCreditReceivedGLCode,"Tab Deposit/Credit Received", -1 * RoundTo((TabCreditReceived),-2),0.0,jobCode,"ZeroTax");
          }

          GetPointsAndVoucherData(DBTransaction,PurchasedPoints,PurchasedVoucher,preZTime,nextDay, PurchasedGiftCard);
          if(RoundTo((PurchasedPoints),-2) != 0.00)
           {
              catTotal += PurchasedPoints;
              AnsiString accountCode = jobCode;
              AddMYOBInvoiceItem(MYOBInvoiceDetail,TGlobalSettings::Instance().PointsPurchasedGLCode,"Points Purchased", RoundTo((PurchasedPoints),-2),0.0,accountCode,"ZeroTax");
           }
          if(RoundTo((PurchasedVoucher),-2) != 0.00)
           {
              catTotal += PurchasedVoucher;
              AnsiString accountCode = jobCode;
              AddMYOBInvoiceItem(MYOBInvoiceDetail,TGlobalSettings::Instance().VoucherPurchasedGLCode,"Voucher Purchased", RoundTo((PurchasedVoucher),-2),0.0,accountCode,"ZeroTax");
           }

           bool addFloatAdjustmentToPayments = false;
           bool addEachPaymentNode = true;
          // get the final float amount (Float Deposit - float withdrwal)
           GetFloatAmounts(DBTransaction, floatAmount,terminalNamePredicate);
           AnsiString cashGlCode= GetCashGlCode(DBTransaction);
           floatAmount = RoundTo(floatAmount, -2);
           catTotal = RoundTo(catTotal, -2);
           bool addAdjustmentNode = true;
           if(RoundTo((floatAmount),-2)  != 0.00)
           {
               if(floatAmount<0)
                   AddMYOBInvoiceItem(MYOBInvoiceDetail,TGlobalSettings::Instance().FloatGLCode,"Float WithDrawal", RoundTo((floatAmount),-2),0.0, jobCode, "ZeroTax");
               else
                   AddMYOBInvoiceItem(MYOBInvoiceDetail,TGlobalSettings::Instance().FloatGLCode,"Float Deposit", RoundTo((floatAmount),-2),0.0, jobCode, "ZeroTax");
               AddMYOBInvoiceItem(MYOBInvoiceDetail,TGlobalSettings::Instance().FloatGLCode,"Float Adjustment",-1 * RoundTo((floatAmount),-2),0.0, jobCode, "ZeroTax");
           }
           double cashWithdrawal = 0;
           if(TGlobalSettings::Instance().FloatWithdrawFromCash)
           {
              cashWithdrawal = GetCashWithdrawal(DBTransaction);
              UnicodeString glCodecashWithdrawal = "";

                if(TGlobalSettings::Instance().CashWithdrawalGLCode.Trim().Length() == 0 || TGlobalSettings::Instance().CashWithdrawalGLCode == NULL)
                    glCodecashWithdrawal = "6-3300";
                else
                    glCodecashWithdrawal = TGlobalSettings::Instance().CashWithdrawalGLCode;

              if(cashWithdrawal != 0)
              {
                AddMYOBInvoiceItem(MYOBInvoiceDetail,glCodecashWithdrawal,"Cash WithDrawal", RoundTo((cashWithdrawal),-2),0.0, jobCode, "ZeroTax");
              }
           }
         IBInternalQuery->ExecQuery();
         for (; !IBInternalQuery->Eof; )
          {
              AnsiString AccountCode = cashGlCode;
              double amountValue = 0.0;

              if(IBInternalQuery->FieldByName("GL_CODE")->AsString != NULL && IBInternalQuery->FieldByName("GL_CODE")->AsString != "")
                AccountCode = IBInternalQuery->FieldByName("GL_CODE")->AsString;

              if(IBInternalQuery->FieldByName("PAY_TYPE")->AsString == "Points")
              {
                 AccountCode = TGlobalSettings::Instance().PointsSpentGLCode;
              }

              if(IBInternalQuery->FieldByName("PAY_TYPE")->AsString != "Cash")
              {
                amountValue = -RoundTo(IBInternalQuery->FieldByName("Amount")->AsFloat, -2);
                payTotal += RoundTo(IBInternalQuery->FieldByName("Amount")->AsFloat, -2);
              }
              else
              {
                 if(!TGlobalSettings::Instance().EnableBlindBalances)
                 {
                    amountValue = RoundTo(IBInternalQuery->FieldByName("Amount")->AsFloat, -2);
                    payTotal += RoundTo(amountValue, -2);
                 }
                 else
                 {
                    cashVariance = IBInternalQuery->FieldByName("Amount")->AsFloat - cashBlindBalance + cashWithdrawal;
                    amountValue = RoundTo(cashBlindBalance , -2);
                    payTotal += RoundTo(amountValue - cashWithdrawal, -2);
                 }
              }

              if(!addFloatAdjustmentToPayments && addEachPaymentNode)
              {
                 AddMYOBInvoiceItem(MYOBInvoiceDetail,AccountCode,IBInternalQuery->FieldByName("PAY_TYPE")->AsString, amountValue ,0.0,jobCode,"ZeroTax");
              }

              if(IBInternalQuery->FieldByName("Tip")->AsFloat != 0)
                 TipAmount += RoundTo(IBInternalQuery->FieldByName("Tip")->AsFloat, -2);

              IBInternalQuery->Next();
          }

         //Add payment Tip for DPS EftPOS
         if(TipAmount != 0)
         {
           AddMYOBInvoiceItem(MYOBInvoiceDetail,TGlobalSettings::Instance().EftPosTipGLCode,"EftPos Tip",TipAmount,0.0,jobCode,"ZeroTax");
           catTotal += TipAmount;
         }

         if(addFloatAdjustmentToPayments && TGlobalSettings::Instance().PostMoneyAsPayment)
           AddMYOBInvoiceItem(MYOBInvoiceDetail,cashGlCode,"Cash", ( payTotal + floatAmount) ,0.0,jobCode,"ZeroTax");

         UnicodeString cashVarianceGLCode = TGlobalSettings::Instance().CashVarianceGLCode;

         //if CashVariance Gl COde is Blank then it will post to default GL Code.
         if(TGlobalSettings::Instance().CashVarianceGLCode == "" && TGlobalSettings::Instance().EnableBlindBalances)
            cashVarianceGLCode = "6-3400";

          // If Cash Variance GL Code And Round GL Code both are same then adjust variance amount in to rounding.
         if(TGlobalSettings::Instance().RoundingGLCode != cashVarianceGLCode && TGlobalSettings::Instance().EnableBlindBalances)
         {
            payTotal += cashVariance;

            //If Rounding GL Code and  CashVarianceGLCode both are different cash variance roe will be created.
            if(RoundTo((cashVariance), -2) != 0)
                AddMYOBInvoiceItem(MYOBInvoiceDetail,cashVarianceGLCode,"Cash Variance",-1 * RoundTo((cashVariance), -2),0.0,jobCode,"ZeroTax");
         }

          if(RoundTo((catTotal - payTotal), -2))
           {
              AnsiString accountCode = jobCode;
              UnicodeString varianceString = "";
              if(RoundTo((catTotal - payTotal - cashVariance), -2) == 0)
                   varianceString = "Cash Variance";
              else
                   varianceString = "Rounding";
              AddMYOBInvoiceItem(MYOBInvoiceDetail,TGlobalSettings::Instance().RoundingGLCode,varianceString,-1 * RoundTo((catTotal - payTotal), -2),0.0,jobCode,"ZeroTax");
           }
           AnsiString daystr = preZTime.FormatString("ddmmyy") + " " + Now().FormatString("HHMMss") ;
           AnsiString refName = TGlobalSettings::Instance().EnableDepositBagNum ? CompanyName : TerminalName ;

           MYOBInvoiceDetail.InvoiceReference = refName +" Sales";
           MYOBInvoiceDetail.InvoiceNumber = val;
           MYOBInvoiceDetail.InvoiceDate = preZTime;
           MYOBInvoiceDetail.CustomerName = CompanyName;
           MYOBInvoiceDetail.JobCode = jobCode;

           if(MYOBInvoiceDetail.MYOBCategoryDetails.size() > 0)
           {
             MYOBInvoiceDetails.push_back(MYOBInvoiceDetail);
           }

          if(double(nextDay) >= double(currentDate))
           {
             canContinue = false;
           }
          else
           {
             preZTime = nextDay;
             nextDay =  IncDay(nextDay,1);
           }
        }
         return MYOBInvoiceDetails;
    }
    catch(Exception& ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ex.Message);
    }

}

void TfrmAnalysis::AddInvoiceItem(TXeroInvoiceDetail &XeroInvoiceDetail,AnsiString Description,double unitAmount,AnsiString AccountCode,
                  double taxAmount)
{
    TXeroCategoryDetail XeroCategoryDetail;
    XeroCategoryDetail.Description = Description;
    XeroCategoryDetail.UnitAmount = unitAmount;
    XeroCategoryDetail.AccountCode = AccountCode;
    XeroCategoryDetail.Quantity = 1;
    XeroCategoryDetail.TaxAmount = taxAmount;
    XeroInvoiceDetail.XeroCategoryDetails.push_back(XeroCategoryDetail);
}

void TfrmAnalysis::AddInvoicePayment(TXeroInvoiceDetail &XeroInvoiceDetail,AnsiString Description,double unitAmount,AnsiString AccountCode,
                  double taxAmount)
{
    TXeroPayTypeDetail XeroPayTypeDetail;
    XeroPayTypeDetail.Description = Description;
    XeroPayTypeDetail.UnitAmount = unitAmount;
    XeroPayTypeDetail.AccountCode = AccountCode;
    XeroPayTypeDetail.TaxAmount = taxAmount;
    XeroInvoiceDetail.XeroPayTypeDetails.push_back(XeroPayTypeDetail);
}

void TfrmAnalysis::AddMYOBInvoiceItem(TMYOBInvoiceDetail &MYOBInvoiceDetail,AnsiString glCode,AnsiString Description,double unitAmount,double taxAmount,AnsiString AccountCode,AnsiString taxStatus)
{
    TMYOBCategoryDetail MYOBCategoryDetail;
    MYOBCategoryDetail.Description = Description;
    MYOBCategoryDetail.GLCode = glCode;
    MYOBCategoryDetail.Amount = unitAmount;
    MYOBCategoryDetail.TaxAmount = taxAmount;
    MYOBCategoryDetail.TaxStatus = taxStatus;
    MYOBInvoiceDetail.MYOBCategoryDetails.push_back(MYOBCategoryDetail);
}

void TfrmAnalysis::GetPointsAndVoucherData(Database::TDBTransaction &DBTransaction,double &PurchasedPoints, double &PurchasedVoucher,
                                 TDateTime startTime,TDateTime endTime, double &purchasedGiftCard)
{
    UnicodeString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
    {
        terminalNamePredicate = " AND a.TERMINAL_NAME = :TERMINAL_NAME ";
    }
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =    "select cast(sum(f.SUBTOTAL) as numeric(17,2)) PointsTotal from "
                                        "(Select distinct a.ARCBILL_KEY from DAYARCBILL a "
                                        "left join DAYARCBILLPAY b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                                        "where b.NOTE <> 'Total Change.' and b.CHARGED_TO_XERO <> 'T' and "
                                        "a.TIME_STAMP > :STARTTIME and  a.TIME_STAMP <= :ENDTIME " + terminalNamePredicate + " ) e "
                                        "left join "
                                        "(select c.ARCBILL_KEY,c.SUBTOTAL from DAYARCSURCHARGE c where "
                                        "c.PAY_TYPE = 'Points') f on e.ARCBILL_KEY = f.ARCBILL_KEY ";
        IBInternalQuery->ParamByName("STARTTIME")->AsDateTime = startTime;
        IBInternalQuery->ParamByName("ENDTIME")->AsDateTime = endTime;
        if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
        {
            IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();
        }
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
        {
          PurchasedPoints = IBInternalQuery->FieldByName("PointsTotal")->AsFloat;
        }
        std::vector<TPayment> Payments;
        TDeviceRealTerminal::Instance().PaymentSystem->PaymentsLoadTypes(DBTransaction,Payments);
        for (std::vector <TPayment> ::iterator ptrPayment = Payments.begin(); ptrPayment != Payments.end(); ptrPayment++)
            {
                if(ptrPayment->GetPaymentAttribute(ePayTypeGetVoucherDetails))
                 {
                    IBInternalQuery->Close();
                    IBInternalQuery->SQL->Text =    "select CAST(sum(f.SUBTOTAL) as numeric(17,2)) VoucherTotal, F.PAY_TYPE from "
                                                    "(Select distinct a.ARCBILL_KEY from DAYARCBILL a "
                                                    "left join DAYARCBILLPAY b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                                                    "where b.NOTE <> 'Total Change.' and b.CHARGED_TO_XERO <> 'T' and "
                                                    "a.TIME_STAMP > :STARTTIME and  a.TIME_STAMP <= :ENDTIME " + terminalNamePredicate + " ) e "
                                                    "left join "
                                                    "(select c.ARCBILL_KEY,c.SUBTOTAL, c.PAY_TYPE PAY_TYPE from DAYARCSURCHARGE c where "
                                                    "c.PAY_TYPE = :PAYTYPE) f on e.ARCBILL_KEY = f.ARCBILL_KEY "
                                                    "WHERE F.PAY_TYPE = :PAYTYPE  GROUP BY 2 ";
                    IBInternalQuery->ParamByName("STARTTIME")->AsDateTime = startTime;
                    IBInternalQuery->ParamByName("ENDTIME")->AsDateTime = endTime;
                    IBInternalQuery->ParamByName("PAYTYPE")->AsString = ptrPayment->Name;
                    if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
                    {
                        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();
                    }
                    IBInternalQuery->ExecQuery();

                    if(TGlobalSettings::Instance().PostZToAccountingSystem && TGlobalSettings::Instance().IsXeroEnabled &&
                        !IBInternalQuery->Eof && IBInternalQuery->FieldByName("PAY_TYPE")->AsString == "Gift Card" )
                    {
                        purchasedGiftCard += IBInternalQuery->FieldByName("VoucherTotal")->AsFloat;
                    }
                    else if(!IBInternalQuery->Eof)
                    {
                        PurchasedVoucher += IBInternalQuery->FieldByName("VoucherTotal")->AsFloat;
                    }
                 }
            }
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

AnsiString TfrmAnalysis::GetCompanyName(Database::TDBTransaction &DBTransaction)
{
    try
    {
        AnsiString CompanyName = "";
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT a.COMPANY FROM REGISTRATION a where a.TERMINAL_NAME = :TERMINAL_NAME";
        IBInternalQuery->Close();
        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
           CompanyName = IBInternalQuery->FieldByName("COMPANY")->AsString;
         }
        return CompanyName;
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TfrmAnalysis::CreateXeroInvoiceAndSend(std::vector<TXeroInvoiceDetail>  &XeroInvoiceDetails)
{
   for (std::vector<TXeroInvoiceDetail>::iterator it = XeroInvoiceDetails.begin() ;
          it != XeroInvoiceDetails.end(); ++it)
     {
        if( TXeroInvoiceBuilder::Instance().BuildXeroInvoice( *it ) )
            {
                SendInvoiceToXero( TXeroInvoiceBuilder::Instance().XeroInvoice );
            }
     }
}

void TfrmAnalysis::CreateMYOBInvoiceAndSend(std::vector<TMYOBInvoiceDetail>  &TMYOBInvoiceDetails)
{
   for (std::vector<TMYOBInvoiceDetail>::iterator it = TMYOBInvoiceDetails.begin() ;
          it != TMYOBInvoiceDetails.end(); ++it)
     {
        if( TMYOBInvoiceBuilder::Instance().BuildMYOBInvoice( *it ) )
            {
                SendInvoiceToMYOB( TMYOBInvoiceBuilder::Instance().MYOBInvoice );
            }
     }
}
//-----------------------------------------------------------------------------
bool TfrmAnalysis::SendInvoiceToXero( TXeroInvoice* inXeroInvoice )
{
   bool result =  TXeroIntegration::Instance().SendInvoice( inXeroInvoice );
   return result;
}

bool TfrmAnalysis::SendInvoiceToMYOB( TMYOBInvoice* inMYOBInvoice )
{
   bool result =  TMYOBIntegration::Instance().SendInvoice( inMYOBInvoice );
   return result;
}
// ---------------------------------------------------------------------------
bool TfrmAnalysis::GetBlindBalences(Database::TDBTransaction &DBTransaction, TBlindBalances &Balance, AnsiString &DepositBagID, AnsiString DeviceName)
{
	/*if (TGlobalSettings::Instance().EnableBlindBalances)
	{
		TBlindBalanceController BlindBalanceController(this, DBTransaction, DeviceName);
		if(BlindBalanceController.Run() == false)
		return false;
		Balance = BlindBalanceController.Get();
		DepositBagID = BlindBalanceController.GetBagID();
	}*/
	return true;


}
// ---------------------------------------------------------------------------
void __fastcall TfrmAnalysis::FormShow(TObject *Sender)
{
	// Clear Zed Memory Stream.
	ZedToArchive->Clear();
	TouchGrid1->Buttons[5][0]->Enabled = EftPos->Enabled;
	if (!TouchGrid1->Buttons[5][0]->Enabled)
	{
		TouchGrid1->Buttons[5][0]->Color = clSilver;
	}
	else
	{
		TouchGrid1->Buttons[5][0]->Color = clRed;
	}


	TouchGrid1->Buttons[4][0]->Enabled = EftPos->Enabled && EftPos->EnquiryEnabled;

	if (!TouchGrid1->Buttons[4][0]->Enabled)
	{
		TouchGrid1->Buttons[4][0]->Color = clGreen;
	}
	else
	{
		TouchGrid1->Buttons[4][0]->Color = clRed;
	}

	// See if there are any palms

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

	if(TGlobalSettings::Instance().EnableDontClearZedData)
	{
		TouchGrid1->Buttons[7][0]->Caption = "End Of Day";
		TouchGrid1->Buttons[6][0]->Enabled = false;
	}

	UnicodeString RegKey = MMBaseKey "Printing";
	int ReceiptPrinterKey = TManagerVariable::Instance().GetInt(DBTransaction, vmReceiptPrinter, 0);
	if (ReceiptPrinterKey == 0)
	{
		MessageBox("No Receipt Printer selected. You will not be able to close off the till.", "Printing Warning", MB_OK + MB_ICONQUESTION);
	}

	StockMasterPath = TManagerVariable::Instance().GetStr(DBTransaction, vmStockMasterExportPath);
	if (StockMasterPath != "")
	{
		if (StockMasterPath[StockMasterPath.Length()] != '\\')
		StockMasterPath += "\\";
		if (FileExists(StockMasterPath + "MMSTART"))
		{
			DeleteFile(StockMasterPath + "MMSTART");
		}
	}
	DBTransaction.Commit();

	FormResize(Sender);

    CheckCANCELITEMS_KEY = false;
    DuplicateEntryInTable = false;

}
// ---------------------------------------------------------------------------
UnicodeString TfrmAnalysis::GetTerminalName()
{
	return TDeviceRealTerminal::Instance().ID.Name;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmAnalysis::btnReprintZClick(void)
{
	if (AuthenticateReportsAccess(TReportSource::PrintZed) != lsAccepted)
	return;

   try
    {
        TfrmSelectZed *frmSelectZed = new TfrmSelectZed(this, TDeviceRealTerminal::Instance().DBControl);
        frmSelectZed->Initialize(ePOSZED);
		frmSelectZed->ShowModal();
        delete frmSelectZed;
	}
   catch(Exception & E)
     {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmAnalysis::tbSetFloatClick(void)
{
    if(TGlobalSettings::Instance().EnableDontClearZedData)
    {
       MessageBox("Slave terminal can not perform Float function", "Warning", MB_OK + MB_ICONQUESTION);
       return;
    }
	try
	{
		TMMContactInfo TempUserInfo;
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
		bool AllowFloat = false;
		TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckSetFloat);
		if (Result == lsAccepted)
		{
			AllowFloat = true;
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access rights to set floats.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}

		if (AllowFloat)
		{
			std::auto_ptr <TManagerFloat> (FloatManager)(new TManagerFloat((TForm*)this));
			FloatManager->SetFloat(DBTransaction, TempUserInfo);
		}
		DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmAnalysis::btnAlterFloatClick(void)
{
	try
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));

		TMMContactInfo TempUserInfo;
		bool AllowAdjustment = false;
		TempUserInfo.Clear();
		TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckAdjustFloat);
		if (Result == lsAccepted)
		{
			AllowAdjustment = true;
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access rights to skim or re-float the till.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}

		if (AllowAdjustment)
		{
			std::auto_ptr <TManagerFloat> (FloatManager)(new TManagerFloat((TForm*)this));
			FloatManager->AlterFloat(DBTransaction, TempUserInfo);
		}
		DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmAnalysis::tbSettleUserClick(void)
{
	try
	{
		bool PrinterExists = true;

		if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
		{
			PrinterExists = false;
		}
		if (!PrinterExists)
		{
			MessageBox("A printer has not been selected. " "Choose a receipt printer from [Maintenance][Printer Maintenance] \r"
			"[Setup Receipt Docket Format]\r" "This terminal is NOT closed!", "Error", MB_OK + MB_ICONWARNING);
			return;
		}

		TMMContactInfo TempUserInfo;
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
		TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckZeds);
		if (Result == lsAccepted)
		{
			MessageBox("Select and login as the user to report on.", "Login Request", MB_OK + MB_ICONINFORMATION);

			TempUserInfo.Clear();
			TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckNone);
			if (Result == lsAccepted)
			{
				TDateTime EndTime = Now();
				std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
				Processing->Message = "Please Wait...";
				Processing->Show();

				std::auto_ptr <TPrintout> Printout(new TPrintout);

				Printout->Printer = TComms::Instance().ReceiptPrinter;

				Currency TotalInOrderedSales = 0;
				int OrderedSalesCount = 0;
				Currency TotalInBilledSales = 0;
				int SoldSalesCount = 0;
				Currency TotalInBilledTips = 0;
				int TipCount = 0;

				TDateTime StartTime = Staff->GetLastUserReportTime(DBTransaction, TempUserInfo.ContactKey);

				TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

				// Ordered By Data.
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "Select " "Contacts.Name, " "Count(Archive.Qty) Item_Count, "
				"Sum((Archive.Price * Archive.Qty )) Price, " "Sum(Archive.Cost * Archive.Qty) Cost " "From " "Security Left Join Archive on "
				"Security.Security_Ref = Archive.Security_Ref " "Left Join Contacts on " "Security.user_key = Contacts.Contacts_Key "
				"Where " "(Archive.Order_Type = 3 or " "Archive.Order_Type = 0) and " "Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp < :EndTime and " "Security.Security_Event = '" + UnicodeString(SecurityTypes[secOrderedBy])
				+ "' and " "Contacts.Contacts_Key = :Contacts_Key " "Group By " "Contacts.Name " "Having "
				"Count(Archive.Archive_Key) > 0 "

				"Union All "

				"Select " "Contacts.Name, " "Count(DayArchive.Qty) Item_Count, " "Sum(DayArchive.Price * DayArchive.Qty) Price, "
				"Sum(DayArchive.Cost * DayArchive.Qty) Cost " "From " "Security Left Join DayArchive on " "Security.Security_Ref = DayArchive.Security_Ref "
				"Left Join Contacts on " "Security.user_key = Contacts.Contacts_Key " "Where " "(DayArchive.Order_Type = 3 or "
				"DayArchive.Order_Type = 0) and " "Security.Time_Stamp >= :StartTime and " "Security.Time_Stamp < :EndTime and "
				"Security.Security_Event = '" + UnicodeString(SecurityTypes[secOrderedBy])
				+ "' and " "Contacts.Contacts_Key = :Contacts_Key " "Group By " "Contacts.Name " "Having "
				"Count(DayArchive.Archive_Key) > 0 "

				"Union All "

				"Select " "Contacts.Name, " "Count(Orders.Qty) Item_Count, " "Sum((Orders.Zed_Price  * Orders.Qty)) Price, "
				"Sum(Orders.Cost * Orders.Qty) Cost " "From " "Security Left Join Orders on " "Security.Security_Ref = Orders.Security_Ref "
				"Left Join Contacts on " "Security.user_key = Contacts.Contacts_Key " "Where " "Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp < :EndTime and " "Security.Security_Event = '" + UnicodeString(SecurityTypes[secOrderedBy])
				+ "' and " "Contacts.Contacts_Key = :Contacts_Key " "Group By " "Contacts.Name " "Having "
				"Count(Orders.Order_Key) > 0 ";

				IBInternalQuery->ParamByName("StartTime")->AsDateTime = StartTime;

				IBInternalQuery->ParamByName("EndTime")->AsDateTime = EndTime;

				IBInternalQuery->ParamByName("Contacts_Key")->AsInteger = TempUserInfo.ContactKey;
				IBInternalQuery->ExecQuery();
				// Ordered By Sold By Data.
				for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
				{
					OrderedSalesCount += IBInternalQuery->FieldByName("Item_Count")->AsInteger;
					TotalInOrderedSales += IBInternalQuery->FieldByName("PRICE")->AsCurrency;
				}

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "Select " "Contacts.Name, " "DAYARCSURCHARGE.PAY_TYPE,DAYARCSURCHARGE.TAX_FREE, "
				"DAYARCSURCHARGE.SUBTOTAL,DAYARCSURCHARGE.ROUNDING,DAYARCSURCHARGE.ARCBILL_KEY,DAYARCSURCHARGE.PROPERTIES " "From "
				"Security inner Join DayArcbill on " "Security.Security_Ref = DayArcbill.Security_Ref " "inner Join DAYARCSURCHARGE on "
				"DayArcbill.ARCBILL_KEY = DAYARCSURCHARGE.ARCBILL_KEY " "Left Join Contacts on "
				"Security.user_key = Contacts.Contacts_Key " "Where " "Security.Time_Stamp >= :StartTime and "
				"Security.Time_Stamp < :EndTime and " "Security.Security_Event = '" + UnicodeString(SecurityTypes[secBilledBy])
				+ "' and " "Contacts.Contacts_Key = :Contacts_Key "

				"Union All "

				"Select " "Contacts.Name, " "ARCSURCHARGE.PAY_TYPE,ARCSURCHARGE.TAX_FREE, "
				"ARCSURCHARGE.SUBTOTAL,ARCSURCHARGE.ROUNDING,ARCSURCHARGE.ARCBILL_KEY,ARCSURCHARGE.PROPERTIES " "From "
				"Security inner Join Arcbill on " "Security.Security_Ref = Arcbill.Security_Ref " "inner Join ARCSURCHARGE on "
				"Arcbill.ARCBILL_KEY = ARCSURCHARGE.ARCBILL_KEY " "Left Join Contacts on " "Security.user_key = Contacts.Contacts_Key "
				"Where " "Security.Time_Stamp >= :StartTime and " "Security.Time_Stamp < :EndTime and " "Security.Security_Event = '" +
				UnicodeString(SecurityTypes[secBilledBy]) + "' and " "Contacts.Contacts_Key = :Contacts_Key ";

				IBInternalQuery->ParamByName("StartTime")->AsDateTime = StartTime;
				IBInternalQuery->ParamByName("EndTime")->AsDateTime = EndTime;
				IBInternalQuery->ParamByName("Contacts_Key")->AsInteger = TempUserInfo.ContactKey;
				IBInternalQuery->ExecQuery();

				// Ordered By Sold By Data.
				std::map <UnicodeString, TSumPayments> PaymentValues; // Place to store Moeny.
				std::map <UnicodeString, TTransactionCount> TransactionsCount; // Place to store Count.

				// Ordered By Sold By Data.
				for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
				{
					AnsiString propertyString = IBInternalQuery->FieldByName("PROPERTIES")->AsString;
					if (TStringTools::Instance()->HasAllProperties(propertyString,"7,"))
					{
						TotalInBilledTips += IBInternalQuery->FieldByName("SUBTOTAL")->AsCurrency;
						TipCount++;
					}
				}

				// Ordered By Data.
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "Select " "Contacts.Name, "
				"DAYARCBILLPAY.PAY_TYPE,DAYARCBILLPAY.TAX_FREE,DAYARCBILLPAY.CASH_OUT, "
				"DAYARCBILLPAY.SUBTOTAL,DAYARCBILLPAY.ROUNDING,DAYARCBILLPAY.ARCBILL_KEY,DAYARCBILLPAY.PROPERTIES " "From "
				"Security left Join DayArcbill on " "Security.Security_Ref = DayArcbill.Security_Ref " "left Join DayArcbillPay on "
				"DayArcbill.ARCBILL_KEY = DayArcbillPay.ARCBILL_KEY " "Left Join Contacts on " "Security.user_key = Contacts.Contacts_Key "
				"Where " "Security.Time_Stamp >= :StartTime and " "Security.Time_Stamp < :EndTime and " "Security.Security_Event = '" +
				UnicodeString(SecurityTypes[secBilledBy]) + "' and " "Contacts.Contacts_Key = :Contacts_Key "

				"Union All "

				"Select " "Contacts.Name, " "ARCBILLPAY.PAY_TYPE,ARCBILLPAY.TAX_FREE,ARCBILLPAY.CASH_OUT, "
				"ARCBILLPAY.SUBTOTAL,ARCBILLPAY.ROUNDING,ARCBILLPAY.ARCBILL_KEY,ARCBILLPAY.PROPERTIES " "From "
				"Security left Join Arcbill on " "Security.Security_Ref = Arcbill.Security_Ref " "left Join ArcbillPay on "
				"Arcbill.ARCBILL_KEY = ArcbillPay.ARCBILL_KEY " "Left Join Contacts on " "Security.user_key = Contacts.Contacts_Key "
				"Where " "Security.Time_Stamp >= :StartTime and " "Security.Time_Stamp < :EndTime and " "Security.Security_Event = '" +
				UnicodeString(SecurityTypes[secBilledBy]) + "' and " "Contacts.Contacts_Key = :Contacts_Key ";

				IBInternalQuery->ParamByName("StartTime")->AsDateTime = StartTime;
				IBInternalQuery->ParamByName("EndTime")->AsDateTime = EndTime;
				IBInternalQuery->ParamByName("Contacts_Key")->AsInteger = TempUserInfo.ContactKey;
				IBInternalQuery->ExecQuery();
				// Ordered By Sold By Data.
				for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
				{
					if (IBInternalQuery->FieldByName("SUBTOTAL")->AsCurrency != 0)
					{
						SoldSalesCount++;
						TotalInBilledSales += IBInternalQuery->FieldByName("SUBTOTAL")->AsCurrency;

						UnicodeString PaymentName;
						PaymentName = IBInternalQuery->FieldByName("PAY_TYPE")->AsString;
						// If a payment type is ever made tax free 1/2 way though trading
						// The last payment type of its "type" will take presidence.
						TSumPayments CurrentPayment;
						CurrentPayment = PaymentValues[PaymentName];
						CurrentPayment.Name = PaymentName;
						if (IBInternalQuery->FieldByName("CASH_OUT")->AsString == "F" || IBInternalQuery->FieldByName("CASH_OUT")->AsString == "")
						{
							CurrentPayment.Total += IBInternalQuery->FieldByName("SUBTOTAL")->AsCurrency;
						}
						else
						{
							CurrentPayment.CashOut += IBInternalQuery->FieldByName("SUBTOTAL")->AsCurrency;
						}
						CurrentPayment.Rounding -= IBInternalQuery->FieldByName("ROUNDING")->AsCurrency;
						PaymentValues[CurrentPayment.Name] = CurrentPayment;

						// Count this Transaction.
						TTransactionCount ThisTransaction = TransactionsCount[PaymentName];
						ThisTransaction.Count++;
						TransactionsCount[PaymentName] = ThisTransaction;
					}
				}

				Printout->PrintFormat->DocumentName = "User Report : " + TempUserInfo.Name;

				Printout->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
				Printout->PrintFormat->Line->ColCount = 1;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
				Printout->PrintFormat->Line->Columns[0]->Text = TempUserInfo.Name + " User Report";
				Printout->PrintFormat->AddLine();

				Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->Columns[0]->Text = Now().FormatString("d mmm yyyy h:nn:ss am/pm");
				Printout->PrintFormat->AddLine();

				AddSectionTitle(Printout.get(), "Sales");

				Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width / 3;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width / 3;
				Printout->PrintFormat->Line->Columns[1]->Alignment = taCenter;
				Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]
				->Width - Printout->PrintFormat->Line->Columns[1]->Width;
				Printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

				Printout->PrintFormat->Add("Type | Total | Average ");

				Printout->PrintFormat->Line->ColCount = 1;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
				Printout->PrintFormat->Line->Columns[0]->Line();
				Printout->PrintFormat->AddLine();

				Printout->PrintFormat->Line->ColCount = 3;
				Printout->PrintFormat->Line->Columns[0]->Width = (Printout->PrintFormat->Width / 3);
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width / 3;
				Printout->PrintFormat->Line->Columns[1]->Alignment = taCenter;
				Printout->PrintFormat->Line->Columns[2]->Width = Printout->PrintFormat->Width - Printout->PrintFormat->Line->Columns[0]
				->Width - Printout->PrintFormat->Line->Columns[1]->Width;
				Printout->PrintFormat->Line->Columns[2]->Alignment = taRightJustify;

				if (OrderedSalesCount != 0)
				{
					Printout->PrintFormat->Add("Sales Ordered | " + FormatFloat("0.00", TotalInOrderedSales) + " | " + FormatFloat("0.00",
					TotalInOrderedSales / OrderedSalesCount));
				}
				if (SoldSalesCount != 0)
				{
					Printout->PrintFormat->Add("Sales Billed | " + FormatFloat("0.00", TotalInBilledSales) + " | " + FormatFloat("0.00",
					TotalInBilledSales / SoldSalesCount));
				}
				if (TotalInBilledTips != 0)
				{
					Printout->PrintFormat->Add("Tips Billed | " + FormatFloat("0.00", TotalInBilledTips) + " | " + FormatFloat("0.00",
					TotalInBilledTips / TipCount));
				}


				Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 1;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
				Printout->PrintFormat->Line->Columns[0]->DoubleLine();
				Printout->PrintFormat->AddLine();
				Printout->PrintFormat->NewLine();
                //-------------------------------------------------------------------------------------

				// TILL CASH IN DRAW REPORT.
				// ---------------------------------------------------------------------
				Currency GrandTotal = 0;
				Currency CashValue = 0;
				Currency ChangeValue = 0;

				AddSectionTitle(Printout.get(), "Cash In Drawer");

				Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 2;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
				Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

				std::map <UnicodeString, TSumPayments> ::iterator itCurrentPayment = PaymentValues.find(CASH);
				if (itCurrentPayment != PaymentValues.end())
				{
					GrandTotal += itCurrentPayment->second.Total;
					GrandTotal -= itCurrentPayment->second.CashOut; // There is never a Cash->Cashout value.
					CashValue = itCurrentPayment->second.Total;
				}

				Printout->PrintFormat->Line->Columns[0]->Text = "Cash";
				Printout->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", CashValue);
				Printout->PrintFormat->AddLine();

				// Find the Change Payment Type.
				itCurrentPayment = PaymentValues.find(CHANGE);
				if (itCurrentPayment != PaymentValues.end())
				{
					GrandTotal += itCurrentPayment->second.Total; // This will be a negitive number for change.
					ChangeValue += itCurrentPayment->second.Total; // This will be a negitive number for change.
				}

				Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 2;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
				Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

				Printout->PrintFormat->Line->Columns[0]->Text = "Change";
				Printout->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", ChangeValue);
				Printout->PrintFormat->AddLine();

				Printout->PrintFormat->NewLine();
				Printout->PrintFormat->Line->Columns[0]->Text = "";
				Printout->PrintFormat->Line->Columns[1]->Line();
				Printout->PrintFormat->AddLine();
				Printout->PrintFormat->Add("CASH IN DRAWER TOTAL|" + FormatFloat("0.00", GrandTotal));

				// Till GRAND TOTAL.
				for (itCurrentPayment = PaymentValues.begin(); itCurrentPayment != PaymentValues.end(); itCurrentPayment++)
				{
					if (itCurrentPayment->second.Name.UpperCase() != UpperCase(CHANGE) && itCurrentPayment->second.Name.UpperCase() != UpperCase
							(CASH))
					{
						TTransactionCount ThisTransaction = TransactionsCount[itCurrentPayment->second.Name];

					   AddSectionTitle(Printout.get(), itCurrentPayment->second.Name + " (" + IntToStr(ThisTransaction.Count) + ")");

						Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
						Printout->PrintFormat->Line->ColCount = 2;
						Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
						Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
						Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
						Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

						Printout->PrintFormat->Line->Columns[0]->Text = itCurrentPayment->second.Name;
						Printout->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", itCurrentPayment->second.Total);
						Printout->PrintFormat->AddLine();

						if (itCurrentPayment->second.CashOut != 0)
						{
							Printout->PrintFormat->Line->Columns[0]->Text = itCurrentPayment->second.Name + " Cash Out Payment";
							Printout->PrintFormat->Line->Columns[1]->Text = FormatFloat("0.00", itCurrentPayment->second.CashOut);
							Printout->PrintFormat->AddLine();
						}

						Printout->PrintFormat->Line->Columns[0]->Text = "";
						Printout->PrintFormat->Line->Columns[1]->Line();
						Printout->PrintFormat->AddLine();
						Printout->PrintFormat->Add("TOTAL TENDERED|" + FormatFloat("0.00",
						itCurrentPayment->second.Total + itCurrentPayment->second.CashOut));

						GrandTotal += itCurrentPayment->second.Total;
						GrandTotal += itCurrentPayment->second.CashOut;
					}
				}

				Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 2;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
				Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

				Printout->PrintFormat->NewLine();
				Printout->PrintFormat->Line->Columns[0]->Text = "";
				Printout->PrintFormat->Line->Columns[1]->DoubleLine();
				Printout->PrintFormat->AddLine();

				Printout->PrintFormat->Add("GRAND TOTAL|" + FormatFloat("0.00", GrandTotal));
               //  Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

                Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 1;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
				Printout->PrintFormat->Line->Columns[0]->DoubleLine();
				Printout->PrintFormat->AddLine();
				Printout->PrintFormat->NewLine();

			    Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 2;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
				Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;



                 // ---------------------------------------------------------------------------
				// Discount Report
				// ---------------------------------------------------------------------------

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "select "
				"ARCBILL_KEY, DISCOUNT, DAYARCBILL.TERMINAL_NAME, DAYARCBILL.TIME_STAMP, PATRON_COUNT,SECURITY.NOTE, TOTAL " "from "
				"DAYARCBILL LEFT JOIN SECURITY ON DAYARCBILL.SECURITY_REF = SECURITY.SECURITY_REF " "where " "DISCOUNT != 0 and "
				"Security.Time_Stamp >= :StartTime and " "Security.Time_Stamp < :EndTime and " "SECURITY.SECURITY_EVENT = '" + UnicodeString
				(SecurityTypes[secDiscountedBy]) + "' and " "SECURITY.USER_KEY = :Contacts_Key "

				"Union All "

				"select " "ARCBILL_KEY, DISCOUNT, ARCBILL.TERMINAL_NAME, ARCBILL.TIME_STAMP, PATRON_COUNT,SECURITY.NOTE, TOTAL " "from "
				"ARCBILL LEFT JOIN SECURITY ON ARCBILL.SECURITY_REF = SECURITY.SECURITY_REF " "where " "DISCOUNT != 0 and "
				"Security.Time_Stamp >= :StartTime and " "Security.Time_Stamp < :EndTime and " "SECURITY.SECURITY_EVENT = '" + UnicodeString
				(SecurityTypes[secDiscountedBy]) + "' and " "SECURITY.USER_KEY = :Contacts_Key ";

				IBInternalQuery->ParamByName("StartTime")->AsDateTime = StartTime;
				IBInternalQuery->ParamByName("EndTime")->AsDateTime = EndTime;
				IBInternalQuery->ParamByName("Contacts_Key")->AsInteger = TempUserInfo.ContactKey;
				IBInternalQuery->ExecQuery();

				TStringList *ItemsList = new TStringList;
				Currency DiscountTotal = 0;
				for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
				{
					TCurTotal *Item = new TCurTotal;
					Item->Total = IBInternalQuery->FieldByName("DISCOUNT")->AsCurrency;
					ItemsList->AddObject(IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime.FormatString("hh:nn ")
					+ IBInternalQuery->FieldByName("NOTE")->AsString, Item);

				}

				if (ItemsList->Count > 0)
				{
					Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
					Printout->PrintFormat->Line->ColCount = 1;
					Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
					Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
					Printout->PrintFormat->Line->Columns[0]->DoubleLine();
					Printout->PrintFormat->AddLine();
					Printout->PrintFormat->NewLine();

					AddSectionTitle(Printout.get(), "Discount Report");

					Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;

					// Currency TotalDiscount = 0;
					// ...followed by the items discounted by that user.
					Printout->PrintFormat->Line->ColCount = 2;
					Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 3 / 4;
					Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
					Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 3 / 4);
					Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

					for (int j = 0; j < ItemsList->Count; j++)
					{
						//TODO:
						Printout->PrintFormat->Add(ItemsList->Strings[j] + "|" + FormatFloat("0.00", ((TCurTotal*)(ItemsList->Objects[j]))->Total));
						//TODO:
						DiscountTotal += ((TCurTotal*)(ItemsList->Objects[j]))->Total;
						delete(TCurTotal*)ItemsList->Objects[j];
					}

					Printout->PrintFormat->Line->ColCount = 2;
					Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 3 / 4;
					Printout->PrintFormat->Line->Columns[0]->Alignment = taRightJustify;
					Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 3 / 4);
					Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

					Printout->PrintFormat->Line->Columns[0]->Text = "";
					Printout->PrintFormat->Line->Columns[1]->DoubleLine();
					Printout->PrintFormat->AddLine();
					Printout->PrintFormat->Add("TOTAL DISCOUNTS|" + FormatFloat("0.00", DiscountTotal));
				}
				delete ItemsList;
                Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 1;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
				Printout->PrintFormat->Line->Columns[0]->DoubleLine();
				Printout->PrintFormat->AddLine();
				Printout->PrintFormat->NewLine();

			    Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 2;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
				Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

                Currency Tax = 0, ServiceCharge = 0, ServiceChargeTax = 0;
                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text = "SELECT CAST(SUM( coalesce(T.Tax,0) )as numeric(17, 4)) Tax, "
                                             " Cast(SUM(coalesce(T.ServiceCharge,0)) as Numeric(17,4)) ServiceCharge,  "
                                             " Cast(SUM(coalesce(T.ServiceChargeTax,0)) as Numeric(17,4)) ServiceChargeTax  "
                                              "FROM DAYARCHIVE DA "
                                                    "INNER JOIN DAYARCBILL AR ON AR.ARCBILL_KEY = DA.ARCBILL_KEY  "
                                                     "LEFT JOIN SECURITY ON AR.SECURITY_REF = SECURITY.SECURITY_REF "
                                                    "Left Join Contacts on  Security.user_key = Contacts.Contacts_Key "
                                                 "LEFT JOIN "
                                                   " ( "
                                                      " SELECT "
                                                          "DAYARCORDERTAXES.ARCHIVE_KEY, "
                                                         " MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS Tax, "
                                                          "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
                                                          "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax "
                                                       "FROM  "
                                                          "( "
                                                            " SELECT "
                                                                "a.ARCHIVE_KEY,  "
                                                                 "a.TAX_TYPE, "
                                                                "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                                                             "FROM DAYARCORDERTAXES a "
                                                             "GROUP BY a.TAX_TYPE, a.ARCHIVE_KEY "
                                                            " ORDER BY 1 "
                                                          ") DAYARCORDERTAXES "
                                                       "GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY "
                                                   " ) T ON DA.ARCHIVE_KEY = T.ARCHIVE_KEY "

                                                 "AND "
                                                " DA.Time_Stamp >= :StartTime  and "
                                                " DA.Time_Stamp <  :EndTime  AND "
                                                 "Security.Security_Event = '" +
				                                   UnicodeString(SecurityTypes[secBilledBy]) + "' and " "Contacts.Contacts_Key = :Contacts_Key "


                                 "UNION ALL  "

                                     "SELECT cast(Sum( coalesce(T.Tax,0) )as numeric(17, 4)) Tax, "
                                     " Cast(SUM(COALESCE(T.ServiceCharge,0)) as Numeric(17,4)) ServiceCharge,  "
                                         " Cast(SUM(coalesce(T.ServiceChargeTax,0)) as Numeric(17,4)) ServiceChargeTax  "
                                          "FROM ARCHIVE DA "
                                                      "INNER JOIN ARCBILL AR ON AR.ARCBILL_KEY = DA.ARCBILL_KEY  "
                                                      "LEFT JOIN SECURITY ON AR.SECURITY_REF = SECURITY.SECURITY_REF "
                                                    "Left Join Contacts on  Security.user_key = Contacts.Contacts_Key "

                                             "LEFT JOIN "
                                               " ( "
                                                  " SELECT "
                                                      "ARCORDERTAXES.ARCHIVE_KEY, "
                                                     " MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS Tax, "
                                                      "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge, "
                                                      "MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax "
                                                   "FROM  "
                                                      "( "
                                                        " SELECT "
                                                            "a.ARCHIVE_KEY,  "
                                                             "a.TAX_TYPE, "
                                                            "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE "
                                                         "FROM ARCORDERTAXES a "
                                                         "GROUP BY a.TAX_TYPE, a.ARCHIVE_KEY "
                                                        " ORDER BY 1 "
                                                      ") ARCORDERTAXES "
                                                   "GROUP BY ARCORDERTAXES.ARCHIVE_KEY "
                                               " ) T ON DA.ARCHIVE_KEY = T.ARCHIVE_KEY "

                                             "AND "
                                            " DA.Time_Stamp >= :StartTime  and "
                                             " DA.Time_Stamp <  :EndTime  AND "
                                              "Security.Security_Event = '" +
                                              UnicodeString(SecurityTypes[secBilledBy]) + "' and " "Contacts.Contacts_Key = :Contacts_Key ";



                IBInternalQuery->ParamByName("StartTime")->AsDateTime = StartTime;
				IBInternalQuery->ParamByName("EndTime")->AsDateTime = EndTime;
                IBInternalQuery->ParamByName("Contacts_Key")->AsInteger = TempUserInfo.ContactKey;
                IBInternalQuery->ExecQuery();

                for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
                {
						Tax += IBInternalQuery->FieldByName("Tax")->AsCurrency;
                        ServiceCharge += IBInternalQuery->FieldByName("ServiceCharge")->AsCurrency;
                        ServiceChargeTax += IBInternalQuery->FieldByName("ServiceChargeTax")->AsCurrency;
                }
                if(Tax!=0)
                {
                     Printout->PrintFormat->Add("Tax Total|" + FormatFloat("0.00", Tax));
                }

                Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 1;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
				Printout->PrintFormat->Line->Columns[0]->DoubleLine();
				Printout->PrintFormat->AddLine();
				Printout->PrintFormat->NewLine();

			    Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 2;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
				Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
                if(ServiceChargeTax!=0)
                {
                    Printout->PrintFormat->Add("Service Charge Tax|" + FormatFloat("0.00", ServiceChargeTax));
                }


                if(ServiceCharge!=0)
                {
                    Printout->PrintFormat->Add("Service Charge|" + FormatFloat("0.00", ServiceCharge));
                }

                if (TotalInBilledTips != 0)
                 {
                    Printout->PrintFormat->Add("Tips Billed | " + FormatFloat("0.00", TotalInBilledTips));

                 }

			    Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
				Printout->PrintFormat->Line->ColCount = 2;
				Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
				Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
				Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
				Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;



                    Currency servictip = 0 ;
                    servictip = ServiceCharge + TotalInBilledTips ;

                    Printout->PrintFormat->Add("Service Charge And Tip total | "+ FormatFloat("0.00",servictip));

                    Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
                    Printout->PrintFormat->Line->ColCount = 2;
                    Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
                    Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
                    Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
                    Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

                    Printout->PrintFormat->Add("Total Billed|" + FormatFloat("0.00", GrandTotal));
                    Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
                    Printout->PrintFormat->Line->ColCount = 2;
                    Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
                    Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
                    Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
                    Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

                    Currency CashlessTips = 0;
                    CashlessTips =  GrandTotal - TotalInBilledTips;
                    Printout->PrintFormat->Add("Cashless Tips |" + FormatFloat("0.00", CashlessTips));
                    Printout->PrintFormat->Line->FontInfo.Height = fsNormalSize;
                    Printout->PrintFormat->Line->ColCount = 2;
                    Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width * 2 / 3;
                    Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
                    Printout->PrintFormat->Line->Columns[1]->Width = Printout->PrintFormat->Width - (Printout->PrintFormat->Width * 2 / 3);
                    Printout->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;



				Printout->PrintFormat->PartialCut();
				Printout->Print();

				if (MessageBox("Do you wish to Reset your information for you next shift Now?", "Reset user information",
							MB_YESNO + MB_ICONQUESTION) == IDYES)
				{
					Staff->SetLastUserReportTime(DBTransaction, TempUserInfo.ContactKey, EndTime);
				}
			}
			else if (Result == lsPINIncorrect)
			{
				MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
			}
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access to Z Reports.", "Error", MB_OK + MB_ICONERROR);
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
		}
		DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}


void __fastcall TfrmAnalysis::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		Height = Screen->Height;

		for(int i = 0; i < 10; i++)
		{
			TouchGrid1->RowHeights[i] = TouchGrid1->RowHeights[i] * (double)Screen->Height / 1024.0;
		}
	}


}
// ---------------------------------------------------------------------------
void __fastcall TfrmAnalysis::btnUpdateStockClick(void)
{
	try
	{
		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

		if (UpdateStockAllowed(DBTransaction))
		{
			if (MessageBox("This will update your stock levels. Do you wish to continue?", "Warning", MB_YESNO + MB_ICONQUESTION) == IDYES)
			{
				std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
				Processing->Message = "Please Wait...";
				Processing->Show();
				ProcessStock(DBTransaction);
				Processing->Close();
				MessageBox("Stock levels updated.", "Complete", MB_OK + MB_ICONWARNING);
			}
			UpdateStockComplete(DBTransaction);
		}
		else
		{
			if (MessageBox("The system reports that another till is currently printing its Z Report and updating the current stock levels."
						"\rIf you know this is not the case you can force a stock update now." "\rAre you certain no other till is doing a Z Report?"
						, "Warning", MB_YESNO + MB_ICONQUESTION) == IDYES)
			{
				std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
				Processing->Message = "Please Wait...";
				Processing->Show();
				ProcessStock(DBTransaction);
				Processing->Close();
				MessageBox("Stock levels updated.", "Complete", MB_OK + MB_ICONWARNING);
				UpdateStockComplete(DBTransaction);
			}
		}
		DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		Database::TDBTransaction TransCloseStock(TDeviceRealTerminal::Instance().DBControl);
		TransCloseStock.StartTransaction();
		UpdateStockComplete(TransCloseStock);
		TransCloseStock.Commit();

		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
		MessageBox("Stock has not been updated. \r" "Please write down and report the following message to your service provider. \r\r " +
		E.Message, "Error", MB_OK + MB_ICONERROR);
	}

}
// ---------------------------------------------------------------------------
bool TfrmAnalysis::UpdateStockAllowed(Database::TDBTransaction &DBTransaction)
{
    try
    {
        bool RetVal = false;
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_UPDATE_STOCK_ON_ZED, 1) FROM RDB$DATABASE";
        IBInternalQuery->ExecQuery();
        int key = IBInternalQuery->Fields[0]->AsInteger;

        if (key == 1)
        {
            RetVal = true;
        }
        return RetVal;
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TfrmAnalysis::UpdateStockComplete(Database::TDBTransaction &DBTransaction)
{
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SET GENERATOR GEN_UPDATE_STOCK_ON_ZED TO 0;";
        IBInternalQuery->ExecQuery();
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

TFinancialDetails TfrmAnalysis::GetFinancialDetails(Database::TDBTransaction &DBTransaction, TTransactionInfo &TransactionInfo, AnsiString DeviceName)
{
	TFinancialDetails FinancialDetails;
	try
	{
		AnsiString Text = "";
		AnsiString Text2 = "";
		if(!TGlobalSettings::Instance().EnableDepositBagNum)
		{
			Text = " DAYARCHIVE.TERMINAL_NAME = '" + DeviceName + "' AND ";
			Text2 = " DAYARCBILL.TERMINAL_NAME = '" + DeviceName + "' AND ";
		}

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		if(TGlobalSettings::Instance().EnableSeperateEarntPts)
		{
			/*Fixed this report. APPLYTOMEMBERS
		    Original Query. Ver 5.15 */

			IBInternalQuery->SQL->Text = "SELECT "
                                            " CATEGORYGROUPS.NAME,"
                                            " CATEGORYGROUPS.CATEGORYGROUPS_KEY,"
                                            " ARCCATEGORIES.CATEGORY,"
                                            " ARCCATEGORIES.CATEGORY_KEY, "
                                            " TAX.TAXTOTAL, "
                                            " 0 AS SALES_TAX_TOTAL, "
                                            " 0 AS SC_TAX_TOTAL, "
                                            " 0 AS SC_TOTAL, "
                                            " 0 AS LOCAL_TAX_TOTAL, "
                                            " DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME, "
                                            " SUM (DAYARCHIVE.PRICE * DAYARCHIVE.QTY)  TOTAL,"
                                            " SUM (DAYARCHIVE.PRICE * DAYARCHIVE.QTY) RAWTOTAL,"
                                            " SUM (DAYARCHIVE.COST * DAYARCHIVE.QTY) COST,"
                                            " SUM (DAYARCHIVE.QTY) TOTALQTY"
                                        " FROM CATEGORYGROUPS"
                                        " INNER JOIN ARCCATEGORIES ON CATEGORYGROUPS.CATEGORYGROUPS_KEY = ARCCATEGORIES.CATEGORYGROUPS_KEY"
                                        " INNER JOIN DAYARCHIVE ON ARCCATEGORIES.CATEGORY_KEY = DAYARCHIVE.CATEGORY_KEY"
                                        " LEFT JOIN DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY"
                                        " LEFT JOIN DISCOUNTS ON DAYARCORDERDISCOUNTS.DISCOUNT_KEY = DISCOUNTS.DISCOUNT_KEY"
                                        " INNER JOIN (SELECT ARCHIVE_KEY, SUM(TAX_VALUE) TAXTOTAL FROM DAYARCORDERTAXES WHERE TAX_TYPE = 0 OR TAX_TYPE = 3 OR TAX_TYPE = 4 GROUP BY ARCHIVE_KEY) TAX ON DAYARCHIVE.ARCHIVE_KEY = TAX.ARCHIVE_KEY"
                                        " INNER JOIN DAYARCBILL ON DAYARCHIVE.ARCBILL_KEY = DAYARCBILL.ARCBILL_KEY"
                                        " WHERE (( DAYARCBILL.DISCOUNT = 0 ) or DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' AND  DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE <> 0 ) AND "	+ Text +
                                        " ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    ORDER_TYPE = " + IntToStr(CreditNonExistingOrder) + ") "
                                        " GROUP BY CATEGORYGROUPS.NAME,CATEGORYGROUPS.CATEGORYGROUPS_KEY, ARCCATEGORIES.CATEGORY, ARCCATEGORIES.CATEGORY_KEY, TAX.TAXTOTAL, DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME ";
		}
		else
		{
			IBInternalQuery->SQL->Text =    "SELECT "
                                                 "CATEGORYGROUPS.NAME, "
                                                 "CATEGORYGROUPS.CATEGORYGROUPS_KEY, "
                                                 "ARCCATEGORIES.CATEGORY,      "
                                                 "ARCCATEGORIES.CATEGORY_KEY,  "
                                                 "DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME, "
                                                 "CAST (SUM (COALESCE(SALES_TAX.TAX_TOTAL,0)) AS NUMERIC(15, 4)) SALES_TAX_TOTAL, "
                                                 "CAST (SUM (COALESCE(SC_TAX.TAX_TOTAL,0)) AS NUMERIC(15, 4)) SC_TAX_TOTAL, "
                                                 "CAST (SUM (COALESCE(SC.SC_TOTAL,0)) AS NUMERIC(15, 4)) SC_TOTAL,  "
                                                 "CAST (SUM (COALESCE(LOCAL_TAX.LOCAL_TAX_TOTAL,0)) AS NUMERIC(15, 4)) LOCAL_TAX_TOTAL, "
                                                 "CAST (SUM ((CAST(DAYARCHIVE.PRICE AS NUMERIC(17,4)) * DAYARCHIVE.QTY)) AS NUMERIC(17, 4))  TOTAL, "
                                                 "CAST (SUM (CAST(DAYARCHIVE.PRICE AS NUMERIC(17,4)) * DAYARCHIVE.QTY) AS NUMERIC(17, 4)) RAWTOTAL, "
                                                 "CAST (SUM (CAST(CAST(DAYARCHIVE.COST AS NUMERIC(17,4)) * CAST(DAYARCHIVE.QTY AS NUMERIC(17,4)) AS NUMERIC(17,4))) AS NUMERIC(17, 4)) COST, "
                                                 "CAST (SUM (DAYARCHIVE.QTY) AS NUMERIC(15, 4)) TOTALQTY "
                                             "FROM  CATEGORYGROUPS "
                                             "INNER JOIN ARCCATEGORIES ON CATEGORYGROUPS.CATEGORYGROUPS_KEY = ARCCATEGORIES.CATEGORYGROUPS_KEY "
                                             "INNER JOIN DAYARCHIVE ON ARCCATEGORIES.CATEGORY_KEY = DAYARCHIVE.CATEGORY_KEY "
                                             "left JOIN DAYARCBILL ON DAYARCHIVE.ARCBILL_KEY = DAYARCBILL.ARCBILL_KEY       "
                                             "left join DAYARCORDERDISCOUNTS on DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
                                             "LEFT JOIN (SELECT ARCHIVE_KEY, SUM(COALESCE(TAX_VALUE,0)) TAX_TOTAL FROM DAYARCORDERTAXES WHERE TAX_TYPE = 0 GROUP BY ARCHIVE_KEY) SALES_TAX ON DAYARCHIVE.ARCHIVE_KEY = SALES_TAX.ARCHIVE_KEY "
                                             "LEFT JOIN (SELECT ARCHIVE_KEY, SUM(COALESCE(TAX_VALUE,0)) TAX_TOTAL FROM DAYARCORDERTAXES WHERE TAX_TYPE = 3 GROUP BY ARCHIVE_KEY) SC_TAX ON DAYARCHIVE.ARCHIVE_KEY = SC_TAX.ARCHIVE_KEY "
                                             "LEFT JOIN (SELECT ARCHIVE_KEY, SUM(COALESCE(TAX_VALUE,0)) SC_TOTAL FROM DAYARCORDERTAXES WHERE TAX_TYPE = 2 GROUP BY ARCHIVE_KEY) SC ON DAYARCHIVE.ARCHIVE_KEY = SC.ARCHIVE_KEY  "
                                             "LEFT JOIN (SELECT ARCHIVE_KEY, SUM(COALESCE(TAX_VALUE,0)) LOCAL_TAX_TOTAL FROM DAYARCORDERTAXES WHERE TAX_TYPE = 4 GROUP BY ARCHIVE_KEY) LOCAL_TAX ON DAYARCHIVE.ARCHIVE_KEY = LOCAL_TAX.ARCHIVE_KEY "
                                            " WHERE (( DAYARCBILL.DISCOUNT = 0 ) or DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' and DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE <> 0 ) and "	+ Text +
                                            " ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    ORDER_TYPE = " + IntToStr(CreditNonExistingOrder) + ") "
                                            " GROUP BY CATEGORYGROUPS.NAME,CATEGORYGROUPS.CATEGORYGROUPS_KEY, ARCCATEGORIES.CATEGORY, ARCCATEGORIES.CATEGORY_KEY,DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME ";
		}
		IBInternalQuery->ExecQuery();

		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
            // Retrive Grand Totals.
			TCategoryGroupDetails TotalSalesCategoryGroup = FinancialDetails.TotalSales.Details[IBInternalQuery->FieldByName("NAME")->AsString];
			TCatTotal TotalSalesDetailTotals = TotalSalesCategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString];

			// Retrive Category Group Totals.
			TCategoryGroupDetails CategoryGroup = FinancialDetails.BilledSales.Details[IBInternalQuery->FieldByName("NAME")->AsString];
			TCatTotal DetailTotals = CategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString];

			DetailTotals.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			DetailTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			DetailTotals.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;;
			DetailTotals.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
			DetailTotals.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
			DetailTotals.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
			DetailTotals.Totals.TaxContent = DetailTotals.Totals.SalesTaxContent + DetailTotals.Totals.ServiceChargeTaxContent;

			DetailTotals.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;

			CategoryGroup.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			CategoryGroup.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			CategoryGroup.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
			CategoryGroup.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
			CategoryGroup.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
			CategoryGroup.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
			CategoryGroup.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
			CategoryGroup.Totals.TaxContent = CategoryGroup.Totals.SalesTaxContent + CategoryGroup.Totals.ServiceChargeTaxContent;

			TotalSalesDetailTotals.Category_Key = IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
			TotalSalesDetailTotals.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			TotalSalesDetailTotals.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
			TotalSalesDetailTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			TotalSalesDetailTotals.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
			TotalSalesDetailTotals.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
			TotalSalesDetailTotals.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
			TotalSalesDetailTotals.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
			TotalSalesDetailTotals.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
			TotalSalesDetailTotals.Totals.TaxContent = TotalSalesDetailTotals.Totals.SalesTaxContent + TotalSalesDetailTotals.Totals.ServiceChargeTaxContent;

			TotalSalesCategoryGroup.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			TotalSalesCategoryGroup.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
			TotalSalesCategoryGroup.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			TotalSalesCategoryGroup.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
			TotalSalesCategoryGroup.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
			TotalSalesCategoryGroup.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
			TotalSalesCategoryGroup.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
			TotalSalesCategoryGroup.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
			TotalSalesCategoryGroup.Totals.TaxContent = TotalSalesCategoryGroup.Totals.SalesTaxContent + TotalSalesCategoryGroup.Totals.ServiceChargeTaxContent;

			TotalSalesCategoryGroup.Category_Group_Name = IBInternalQuery->FieldByName("NAME")->AsString;
			TotalSalesCategoryGroup.Category_Group_Key = IBInternalQuery->FieldByName("CATEGORYGROUPS_KEY")->AsInteger;

			if(IBInternalQuery->FieldByName("DISCOUNT_GROUPNAME")->AsString == "Complimentary")
			{
				FinancialDetails.ComplimentarySales.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
				FinancialDetails.ComplimentarySales.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
				FinancialDetails.ComplimentarySales.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
				FinancialDetails.ComplimentarySales.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
				FinancialDetails.ComplimentarySales.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
				FinancialDetails.ComplimentarySales.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
				FinancialDetails.ComplimentarySales.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
				FinancialDetails.ComplimentarySales.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
				FinancialDetails.ComplimentarySales.Totals.TaxContent = FinancialDetails.BilledSales.Totals.SalesTaxContent + FinancialDetails.BilledSales.Totals.ServiceChargeTaxContent;
			}
			else
			{
				FinancialDetails.BilledSales.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
				FinancialDetails.BilledSales.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
				FinancialDetails.BilledSales.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
				FinancialDetails.BilledSales.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
				FinancialDetails.BilledSales.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
				FinancialDetails.BilledSales.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
				FinancialDetails.BilledSales.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
				FinancialDetails.BilledSales.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
				FinancialDetails.BilledSales.Totals.TaxContent = FinancialDetails.BilledSales.Totals.SalesTaxContent + FinancialDetails.BilledSales.Totals.ServiceChargeTaxContent;
			}

			FinancialDetails.TotalSales.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			FinancialDetails.TotalSales.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
			FinancialDetails.TotalSales.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			FinancialDetails.TotalSales.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
			FinancialDetails.TotalSales.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
			FinancialDetails.TotalSales.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
			FinancialDetails.TotalSales.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
			FinancialDetails.TotalSales.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
			FinancialDetails.TotalSales.Totals.TaxContent = FinancialDetails.TotalSales.Totals.SalesTaxContent + FinancialDetails.TotalSales.Totals.ServiceChargeTaxContent;

			// Store Category Group Totals.
			//TODO:
			//CategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString] = DetailTotals;
			if(IBInternalQuery->FieldByName("DISCOUNT_GROUPNAME")->AsString == "Complimentary")
			{
				//TODO:
				//FinancialDetails.ComplimentarySales.Details[IBInternalQuery->FieldByName("NAME")->AsString] = CategoryGroup;
			}
			else
			{
				//TODO:
				//FinancialDetails.BilledSales.Details[IBInternalQuery->FieldByName("NAME")->AsString] = CategoryGroup;
			}
			// Store Grand Totals.
			TotalSalesCategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString] = TotalSalesDetailTotals;
			FinancialDetails.TotalSales.Details[IBInternalQuery->FieldByName("NAME")->AsString] = TotalSalesCategoryGroup;
		}
		IBInternalQuery->Close();
		TDateTime PrevZedTime = Now();
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT " "MAX(TIME_STAMP)TIME_STAMP FROM ZEDS " "WHERE " "TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount != 0)
		{
			PrevZedTime = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
		}

		if(!TGlobalSettings::Instance().EnableDepositBagNum)
		{
			Text = " TERMINAL_NAME = '" + DeviceName + "' AND ";
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT " " CATEGORYGROUPS.NAME," " ARCCATEGORIES.CATEGORY,"
		" SUM ((ORDERS.ZED_PRICE * ORDERS.QTY)) TOTAL,"
		" SUM (ORDERS.COST * ORDERS.QTY) COST,"
		" SUM (ORDERS.QTY) TOTALQTY,"
		" 0.00  AS TAXTOTAL "
		" FROM" " CATEGORYGROUPS" " INNER JOIN ARCCATEGORIES ON CATEGORYGROUPS.CATEGORYGROUPS_KEY = ARCCATEGORIES.CATEGORYGROUPS_KEY"
		" INNER JOIN ORDERS ON ARCCATEGORIES.CATEGORY_KEY = ORDERS.CATEGORY_KEY"
		" WHERE " + Text +
		" ORDERS.TIME_STAMP > :TIME_STAMP" " AND ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    ORDER_TYPE = " + IntToStr
		(CreditNonExistingOrder) + ") " " GROUP BY CATEGORYGROUPS.NAME, ARCCATEGORIES.CATEGORY";
		IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = PrevZedTime;
		IBInternalQuery->ExecQuery();
		// FinancialDetails.SavedSales
		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			// Retrive Grand Totals.
			TCategoryGroupDetails TotalSalesCategoryGroup = FinancialDetails.TotalSales.Details
			[IBInternalQuery->FieldByName("NAME")->AsString];
			TCatTotal TotalSalesDetailTotals = TotalSalesCategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString];
			// Retrive Category Group Totals.
			TCategoryGroupDetails CategoryGroup = FinancialDetails.SavedSales.Details[IBInternalQuery->FieldByName("NAME")->AsString];
			TCatTotal DetailTotals = CategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString];

			DetailTotals.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			DetailTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			DetailTotals.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
			DetailTotals.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;

			CategoryGroup.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			CategoryGroup.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			CategoryGroup.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
			CategoryGroup.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;


			TotalSalesDetailTotals.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			TotalSalesDetailTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			TotalSalesDetailTotals.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
			TotalSalesDetailTotals.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;

			TotalSalesCategoryGroup.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			TotalSalesCategoryGroup.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			TotalSalesCategoryGroup.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
			TotalSalesCategoryGroup.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;

			FinancialDetails.SavedSales.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			FinancialDetails.SavedSales.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			FinancialDetails.SavedSales.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
			FinancialDetails.SavedSales.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;

			FinancialDetails.TotalSales.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
			FinancialDetails.TotalSales.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
			FinancialDetails.TotalSales.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
			//TODO:
			//FinancialDetails.TotalSales.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;

			// Store Category Group Totals.
			//TODO:
			//CategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString] = DetailTotals;
			//TODO:
			//FinancialDetails.SavedSales.Details[IBInternalQuery->FieldByName("NAME")->AsString] = CategoryGroup;

			// Store Grand Totals.
			//TODO:
			//TotalSalesCategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString] = TotalSalesDetailTotals;
			//TODO:
			//FinancialDetails.TotalSales.Details[IBInternalQuery->FieldByName("NAME")->AsString] = TotalSalesCategoryGroup;
		}
		IBInternalQuery->Close();
		// Fix Bill ,Saved and Total totals.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT COUNT (DISTINCT DAYARCBILL.ARCBILL_KEY) QTYTOTAL " "FROM DAYARCBILL "
		"LEFT JOIN DAYARCHIVE ON DAYARCHIVE.ARCBILL_KEY = DAYARCBILL.ARCBILL_KEY "
		" WHERE " + Text2 +
		" ( DAYARCHIVE.ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    DAYARCHIVE.ORDER_TYPE = " + IntToStr
		(CreditNonExistingOrder) + ") ";
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount != 0)
		{
			FinancialDetails.BilledSales.Totals.Qty = IBInternalQuery->FieldByName("QTYTOTAL")->AsInteger;
		}

		// Fix Bill ,Saved and Total totals.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"select count(distinct(time_stamp)) QTY  from orders "
		" WHERE " + Text +
		" ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR ORDER_TYPE = " + IntToStr
		(CreditNonExistingOrder) + ") ";
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount != 0)
		{
			FinancialDetails.SavedSales.Totals.Qty = IBInternalQuery->FieldByName("QTY")->AsInteger;
		}

		FinancialDetails.TotalSales.Totals.Qty = FinancialDetails.SavedSales.Totals.Qty + FinancialDetails.BilledSales.Totals.Qty;

		Currency LoyaltyTotal = 0;
		float LoyaltyTotalQty = 0;
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "select " " SUM (DAYARCHIVE.PRICE) TOTAL,"
		" SUM (DAYARCHIVE.COST * DAYARCHIVE.QTY) COST," " SUM (DAYARCHIVE.QTY) TOTALQTY " " FROM " "DAYARCHIVE "
		" WHERE " + Text +
		" LOYALTY_KEY != 0 " " AND LOYALTY_KEY IS NOT NULL " " AND ( ORDER_TYPE = " + IntToStr(NormalOrder)
		+ " " " OR    ORDER_TYPE = " + IntToStr(CreditNonExistingOrder) + ") ";
		IBInternalQuery->ExecQuery();

		LoyaltyTotal += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
		LoyaltyTotalQty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "select " " SUM (ORDERS.ZED_PRICE) TOTAL,"
		" SUM (ORDERS.COST *ORDERS.QTY) COST," " SUM (ORDERS.QTY) TOTALQTY " " FROM ORDERS "
		" WHERE " + Text +
		" LOYALTY_KEY != 0 " " AND LOYALTY_KEY IS NOT NULL "
		" AND ORDERS.TIME_STAMP > :TIME_STAMP" " AND ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    ORDER_TYPE = " + IntToStr
		(CreditNonExistingOrder) + ") ";
		IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = PrevZedTime;
		IBInternalQuery->ExecQuery();

		LoyaltyTotal += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
		LoyaltyTotalQty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;

		TCalculatedTotals LoyaltySalesTotal(etcTotalLoyaltySales, LoyaltyTotal,0,0, LoyaltyTotalQty);
		TransactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTotalLoyaltySales]] = LoyaltySalesTotal;
		IBInternalQuery->Close();

		//Summa Gross Sales.
		/*TCalculatedTotals TotalRawSales(etcTotalRawSales, 0,0,0,0);
		GetSummaGrossSalesTotal(DBTransaction,DeviceName,TotalRawSales);
		TransactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTotalRawSales]] = TotalRawSales;

		//Summa Net sales.
		TCalculatedTotals TotalGrossSales(etcTotalGrossSales, 0,0,0,0);
		GetSummaNetSalesTotal(DBTransaction,DeviceName,TotalGrossSales);
		TransactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTotalGrossSales]] = TotalGrossSales;*/
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return FinancialDetails;
}

void TfrmAnalysis::GetSummaGrossSalesTotal(Database::TDBTransaction &DBTransaction,UnicodeString DeviceName, TCalculatedTotals &BaseSales)
{ // Sales Excluding Discounts
	try
	{
		// The Total Including Discounts.
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT SUM (DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) GROSS_TOTAL FROM DAYARCHIVE "
		"left join DAYARCORDERDISCOUNTS on  "
		"DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.RawTotal = IBInternalQuery->FieldByName("GROSS_TOTAL")->AsCurrency;
		}

		// The Total Qty.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT COUNT(DAYARCBILL.ARCBILL_KEY) FROM DAYARCBILL where DAYARCBILL.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Qty = IBInternalQuery->FieldByName("COUNT")->AsCurrency;
		}

		// Add Surcharges to the total.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT SUM (DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) DISCOUNT_TOTAL "
		"FROM DAYARCHIVE "
		"INNER JOIN DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		"WHERE DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' and "
		" DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE > 0 AND DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.RawTotal += IBInternalQuery->FieldByName("DISCOUNT_TOTAL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( ((((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY ) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_EXCL "
		" FROM DAYARCHIVE "
		"left join DAYARCORDERDISCOUNTS on "
		"DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'   "
		" or DAYARCHIVE.DISCOUNT = 0 and DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Total = IBInternalQuery->FieldByName("TOTAL_EXCL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( (((DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_SURCHARGE_EXCL "
		" FROM DAYARCORDERDISCOUNTS "
		" INNER JOIN DAYARCHIVE ON DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
		" WHERE (( DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'   "
		"  and DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE > 0) "
		"or DAYARCHIVE.DISCOUNT = 0) AND DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Total += IBInternalQuery->FieldByName("TOTAL_SURCHARGE_EXCL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( ((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) - ((((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_TAX "
		" FROM DAYARCHIVE "
		"left join DAYARCORDERDISCOUNTS on "
		"DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'   "
		" or DAYARCHIVE.DISCOUNT = 0 and DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.TaxContent = IBInternalQuery->FieldByName("TOTAL_TAX")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE -  ((DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0) ) TOTAL_SURCHARGE_TAX "
		" FROM DAYARCORDERDISCOUNTS "
		" INNER JOIN DAYARCHIVE ON DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
		" WHERE (( DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' and  "
		" DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE > 0) "
		"or DAYARCHIVE.DISCOUNT = 0) AND DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.TaxContent += IBInternalQuery->FieldByName("TOTAL_SURCHARGE_TAX")->AsCurrency;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TfrmAnalysis::GetSummaNetSalesTotal(Database::TDBTransaction &DBTransaction,UnicodeString DeviceName,TCalculatedTotals &BaseSales)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT SUM ((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) GROSS_TOTAL FROM DAYARCHIVE  "
		"left join DAYARCORDERDISCOUNTS on  "
		"DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.RawTotal = IBInternalQuery->FieldByName("GROSS_TOTAL")->AsCurrency;
		}

		// The Total Qty.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT COUNT(DAYARCBILL.ARCBILL_KEY) FROM DAYARCBILL where DAYARCBILL.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Qty = IBInternalQuery->FieldByName("COUNT")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT SUM (DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) TOTAL_ADJUSTMENTS "
		"FROM DAYARCHIVE "
		"INNER JOIN DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.RawTotal += IBInternalQuery->FieldByName("TOTAL_ADJUSTMENTS")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( ((((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_EXCL "
		" FROM DAYARCHIVE "
		"left join DAYARCORDERDISCOUNTS on  "
		"DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Total = IBInternalQuery->FieldByName("TOTAL_EXCL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( (((DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_ADJUSTMENTS_EXCL "
		" FROM DAYARCORDERDISCOUNTS "
		" INNER JOIN DAYARCHIVE ON DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Total += IBInternalQuery->FieldByName("TOTAL_ADJUSTMENTS_EXCL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( ((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) - (((DAYARCHIVE.PRICE * DAYARCHIVE.QTY) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_TAX "
		" FROM DAYARCHIVE "
		"left join DAYARCORDERDISCOUNTS on  "
		"DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.TaxContent = IBInternalQuery->FieldByName("TOTAL_TAX")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE -  ((DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0) ) TOTAL_ADJUSTMENTS_TAX "
		" FROM DAYARCORDERDISCOUNTS "
		" INNER JOIN DAYARCHIVE ON DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.TaxContent += IBInternalQuery->FieldByName("TOTAL_ADJUSTMENTS_TAX")->AsCurrency;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TfrmAnalysis::GetSummaGrossNet(Database::TDBTransaction &DBTransaction,int ArchiveKey, TCalculatedTotals &BaseSales)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT ((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) GROSS_TOTAL FROM DAYARCHIVE WHERE ARCHIVE_KEY = :ARCHIVE_KEY";
		IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.RawTotal += IBInternalQuery->FieldByName("GROSS_TOTAL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM (DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) SURCHARGE_TOTAL FROM DAYARCORDERDISCOUNTS "
		"WHERE DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE > 0 AND ARCHIVE_KEY = :ARCHIVE_KEY";
		IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.RawTotal += IBInternalQuery->FieldByName("SURCHARGE_TOTAL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" (((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) + DAYARCHIVE.DISCOUNT) TOTAL_NET "
		" FROM DAYARCHIVE WHERE ARCHIVE_KEY = :ARCHIVE_KEY";
		IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Total += IBInternalQuery->FieldByName("TOTAL_NET")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( (((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) + DAYARCHIVE.DISCOUNT) - ((((DAYARCHIVE.PRICE * DAYARCHIVE.QTY) + DAYARCHIVE.DISCOUNT) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_TAX "
		" FROM DAYARCHIVE WHERE ARCHIVE_KEY = :ARCHIVE_KEY";
		IBInternalQuery->ParamByName("ARCHIVE_KEY")->AsInteger = ArchiveKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.TaxContent += IBInternalQuery->FieldByName("TOTAL_TAX")->AsCurrency;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TfrmAnalysis::UpdateBlindBlances(Database::TDBTransaction &DBTransaction, int ZedKey, TBlindBalances &Balance, AnsiString &BagID)
{
    try
    {
        zedLogsList->Add("Before updating blind balance ");
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        for (TBlindBalanceContainer::iterator itBlindBalances = Balance.begin(); (itBlindBalances != Balance.end()) && ZedKey; itBlindBalances++)
        {
            int BlindBalancekey;
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_BLINDBALANCE_KEY, 1) FROM RDB$DATABASE";
            IBInternalQuery->ExecQuery();
            BlindBalancekey = IBInternalQuery->Fields[0]->AsInteger;

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "INSERT INTO BLINDBALANCE (" "BLINDBALANCE_KEY," "Z_KEY," "PAYMENT," "PAYMENT_GROUP," "PAYMENT_TRANS_QTY,"
            "BLIND_BALANCE," "SYSTEM_BALANCE," "OFFICE_BALANCE," "DEPOSITBAG_ID) " "VALUES (" ":BLINDBALANCE_KEY," ":Z_KEY," ":PAYMENT," ":PAYMENT_GROUP,"
            ":PAYMENT_TRANS_QTY," ":BLIND_BALANCE," ":SYSTEM_BALANCE," ":OFFICE_BALANCE," ":DEPOSITBAG_ID);";
            IBInternalQuery->ParamByName("BLINDBALANCE_KEY")->AsInteger = BlindBalancekey;
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
            IBInternalQuery->ParamByName("PAYMENT")->AsString = itBlindBalances->first;
            IBInternalQuery->ParamByName("PAYMENT_GROUP")->AsInteger = itBlindBalances->second.PaymentGroup;
            IBInternalQuery->ParamByName("PAYMENT_TRANS_QTY")->AsInteger = itBlindBalances->second.TransQty;
            IBInternalQuery->ParamByName("BLIND_BALANCE")->AsCurrency = itBlindBalances->second.BlindBalance;
            IBInternalQuery->ParamByName("SYSTEM_BALANCE")->AsCurrency = itBlindBalances->second.SystemBalance;
            IBInternalQuery->ParamByName("OFFICE_BALANCE")->AsCurrency = 0;
            IBInternalQuery->ParamByName("DEPOSITBAG_ID")->AsString = BagID;
            IBInternalQuery->ExecQuery();
        }
    }
    catch(Exception &E)
    {
        zedLogsList->Add("Exception caught in  UpdateBlindBlances()");
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    zedLogsList->Add("After updating blind balance ");
}

void TfrmAnalysis::UpdateCommissionDatabase(Database::TDBTransaction &DBTransaction, int ZedKey, TCommissionCache &Commission)
{
    try
    {
        zedLogsList->Add("Befor UpdateCommissionDatabase ");
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT * FROM COMMISSION WHERE COMMISSION_KEY < 0";
        //	IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount == 0 || TGlobalSettings::Instance().EnableDontClearZedData)
        {
            for (TCommissionContainer::iterator itCommission = Commission.begin(); itCommission != Commission.end(); itCommission++)
            {
                TDateTime date = itCommission->second.GetDateFrom();
                int days = (int)(itCommission->second.GetDateTo() - itCommission->second.GetDateFrom());

                for(int i = 0; i < days; i++)
                {
                    if(itCommission->second.GetCommission() == 0)
                    break;
                    int Commissionkey;
                    IBInternalQuery->Close();
                    IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_COMMISSION, 1) FROM RDB$DATABASE";
                    IBInternalQuery->ExecQuery();
                    Commissionkey = -IBInternalQuery->Fields[0]->AsInteger;


                    IBInternalQuery->Close();
                    IBInternalQuery->SQL->Text =
                    "INSERT INTO COMMISSION (" "COMMISSION_KEY," "STAFF_KEY," "STAFF_NAME," "ON_DATE," "AVG_COMMISSION,"
                    "DATEFROM," "DATETO," "TOTALCOMMISSION," "Z_KEY) " "VALUES (" ":COMMISSION_KEY," ":STAFF_KEY," ":STAFF_NAME," ":ON_DATE,"
                    ":AVG_COMMISSION," ":DATEFROM," ":DATETO," ":TOTALCOMMISSION," ":Z_KEY);";
                    IBInternalQuery->ParamByName("COMMISSION_KEY")->AsInteger = Commissionkey;
                    IBInternalQuery->ParamByName("STAFF_KEY")->AsInteger = itCommission->second.GetNumber();
                    IBInternalQuery->ParamByName("STAFF_NAME")->AsString = itCommission->second.GetName();
                    IBInternalQuery->ParamByName("ON_DATE")->AsDateTime = date;
                    IBInternalQuery->ParamByName("AVG_COMMISSION")->AsCurrency = itCommission->second.GetCommission() / days;
                    IBInternalQuery->ParamByName("DATEFROM")->AsDateTime = itCommission->second.GetDateFrom();
                    IBInternalQuery->ParamByName("DATETO")->AsDateTime = itCommission->second.GetDateTo();
                    IBInternalQuery->ParamByName("TOTALCOMMISSION")->AsCurrency = itCommission->second.GetCommission();
                    IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
                    IBInternalQuery->ExecQuery();
                    date.operator++();
                }
            }
            zedLogsList->Add("INSERTING INTO  COMMISSION ");
        }
        else
        {   zedLogsList->Add("updating  COMMISSION ");
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "UPDATE COMMISSION "
            "SET COMMISSION_KEY = -COMMISSION_KEY, "
            "Z_KEY = :Z_KEY "
            "WHERE COMMISSION_KEY < 0;";
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
            IBInternalQuery->ExecQuery();
        }
    }
    catch(Exception &E)
    {
        zedLogsList->Add("Exception caught in UpdateCommissionDatabase() ");
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    zedLogsList->Add("After  UpdateCommissionDatabase");
}

void TfrmAnalysis::UpdatePrinterReadingsDatabase(Database::TDBTransaction &DBTransaction, int ZedKey, TPrinterReadingsInterface &PrinterReading)
{
    try
    {
        zedLogsList->Add("Before Updating PRINTERREADINGS table ");
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT * FROM PRINTERREADINGS WHERE PRINTERREADINGS_KEY < 0;";
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount == 0)
        {

            for(TPrinterReadingsContainer::iterator itPrinterReadings = PrinterReading.begin(); itPrinterReadings != PrinterReading.end(); std::advance(itPrinterReadings, 1))
            {
                int PrinterReadingskey;
                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PRINTERREADINGS, 1) FROM RDB$DATABASE";
                IBInternalQuery->ExecQuery();
                PrinterReadingskey = -IBInternalQuery->Fields[0]->AsInteger;


                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text =
                "INSERT INTO PRINTERREADINGS (" "PRINTERREADINGS_KEY," "PRINTER_NAME," "START_NUMBER," "END_NUMBER," "COPIES,"
                " Z_KEY) " "VALUES (" ":PRINTERREADINGS_KEY," ":PRINTER_NAME," ":START_NUMBER," ":END_NUMBER," ":COPIES,"
                "" ":Z_KEY);";
                IBInternalQuery->ParamByName("PRINTERREADINGS_KEY")->AsInteger = PrinterReadingskey;
                IBInternalQuery->ParamByName("PRINTER_NAME")->AsString = itPrinterReadings->second.GetPrinterName();
                IBInternalQuery->ParamByName("START_NUMBER")->AsInteger = itPrinterReadings->second.GetStartNumber();
                IBInternalQuery->ParamByName("END_NUMBER")->AsInteger = itPrinterReadings->second.GetFinishNumber();
                IBInternalQuery->ParamByName("COPIES")->AsInteger = itPrinterReadings->second.GetCopies();
                IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
                IBInternalQuery->ExecQuery();
            }
            if(PrinterReading.size())
                zedLogsList->Add("After inserting record in PRINTERREADINGS table");
        }
        else if(IBInternalQuery->RecordCount == 0 || TGlobalSettings::Instance().EnableDontClearZedData)
        {
            for(TPrinterReadingsContainer::iterator itPrinterReadings = PrinterReading.begin(); itPrinterReadings != PrinterReading.end(); std::advance(itPrinterReadings, 1))
            {
                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text =
                "UPDATE PRINTERREADINGS "
                "SET START_NUMBER = :START_NUMBER, "
                "END_NUMBER = :END_NUMBER, "
                "COPIES = :COPIES "
                "WHERE PRINTERREADINGS_KEY < 0 AND PRINTER_NAME = :PRINTER_NAME;";
                IBInternalQuery->ParamByName("START_NUMBER")->AsInteger = itPrinterReadings->second.GetStartNumber();
                IBInternalQuery->ParamByName("END_NUMBER")->AsInteger = itPrinterReadings->second.GetFinishNumber();
                IBInternalQuery->ParamByName("COPIES")->AsInteger = itPrinterReadings->second.GetCopies();
                IBInternalQuery->ParamByName("PRINTER_NAME")->AsString = itPrinterReadings->second.GetPrinterName();
                IBInternalQuery->ExecQuery();
            }
            if(PrinterReading.size())
                zedLogsList->Add("After update record in PRINTERREADINGS table");
        }
        else
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "UPDATE PRINTERREADINGS "
            "SET PRINTERREADINGS_KEY = -PRINTERREADINGS_KEY, "
            "Z_KEY = :Z_KEY "
            "WHERE PRINTERREADINGS_KEY < 0;";
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
            IBInternalQuery->ExecQuery();
            zedLogsList->Add("update printerreading table else block.");
        }
    }
    catch(Exception &E)
    {
        zedLogsList->Add("Exception caught in UpdatePrinterReadingsDatabase() ");
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    zedLogsList->Add("After Updating PRINTERREADINGS table");
}

void TfrmAnalysis::UpdatePaxCountDatabase(Database::TDBTransaction &DBTransaction,int ZedKey, TPaxCount &inPaxCount)
{
    try
    {
        zedLogsList->Add("Before Updating PAXCOUNT table ");
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT * FROM PAXCOUNT WHERE Z_KEY = :Z_KEY";
        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->RecordCount)
        {
            int PaxCountkey;
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PAXCOUNT, 1) FROM RDB$DATABASE";
            IBInternalQuery->ExecQuery();
            PaxCountkey = IBInternalQuery->Fields[0]->AsInteger;

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            "INSERT INTO PAXCOUNT (" "PAXCOUNT_KEY," "PAX_NUMBER," "DATE_FROM," "DATE_TO," "Z_KEY)"
            "VALUES (" ":PAXCOUNT_KEY," ":PAX_NUMBER," ":DATE_FROM," ":DATE_TO," ":Z_KEY);";
            IBInternalQuery->ParamByName("PAXCOUNT_KEY")->AsInteger = PaxCountkey;
            IBInternalQuery->ParamByName("PAX_NUMBER")->AsInteger = inPaxCount.GetPaxNumber();
            IBInternalQuery->ParamByName("DATE_FROM")->AsDate = inPaxCount.GetDateFrom();
            IBInternalQuery->ParamByName("DATE_TO")->AsDate = inPaxCount.GetDateTo();
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
            IBInternalQuery->ExecQuery();
            zedLogsList->Add("Inserting records in PAXCOUNT table ");
        }
    }
    catch(Exception &E)
    {
        zedLogsList->Add("Exception caught in  UpdatePaxCountDatabase()");
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    zedLogsList->Add("After Updating PAXCOUNT table ");
}
bool __fastcall TfrmAnalysis::ParamExists(TIBSQL *IBSQL, AnsiString FieldName)
{
	bool RetVal = false;

	if(!IBSQL->Prepared)
	{
		IBSQL->Prepare();
	}
	AnsiString Names =  IBSQL->Params->Names;
	if(Names.Pos(FieldName) > 0)
	{
		RetVal = true;
	}
	/*for (int i = 0; i < IBSQL->Params->Names->Count; i++) {
		if(IBSQL->Params->Names[i] == FieldName)
		{
			RetVal = true;
		}
	}        */
	return RetVal;
}

void __fastcall TfrmAnalysis::VariantDBAssign(TIBXSQLVAR *Source, TIBXSQLVAR *Dest)
{
	Variant Data = Source->AsVariant;

	switch(Source->SQLType)
	{
	case SQL_TEXT:
	case SQL_VARYING:
		{
			AnsiString StrData = Source->AsString;
			Dest->AsString = StrData;
		}break;
	case varCurrency:
		{
			Dest->AsCurrency = Source->AsCurrency;
		}break;
	case SQL_DOUBLE:
	case SQL_FLOAT:
	case SQL_D_FLOAT:
		{
			Dest->AsDouble = Source->AsDouble;
		}break;
	case SQL_SHORT:
	case SQL_LONG:
		{
			Dest->AsLong = Source->AsLong;
		}break;
	case SQL_INT64:
		{
			Dest->AsDouble = Source->AsDouble;
		}break;
	case SQL_TYPE_DATE:
	case SQL_TYPE_TIME:
	case SQL_TIMESTAMP:
		{
			Dest->AsDateTime = Source->AsDateTime;
		}break;
	default:
		{
			Dest->AsVariant = Data;
		}
	}
}

void __fastcall TfrmAnalysis::GridMouseClick(TObject *Sender, TMouseButton Button,
TShiftState Shift, TGridButton *GridButton)
{
	switch(TouchGrid1->Row(GridButton))
	{
	case 0:
		tbSetFloatClick();
		break;
	case 1:
		btnAlterFloatClick();
		break;
	case 2:
		btnReportsClick();
		break;
	case 3:
		tbSettleUserClick();
		break;
	case 4:
		EftPos->DoSettlementEnquiry();
		break;
	case 5:
		EftPos->DoSettlementCutover();
		break;
	case 6:
		btnUpdateStockClick();
		break;
	case 7:
		btnZReportClick();
		break;
	case 8:
		btnReprintZClick();
		break;
	case 9:
		Close();
		break;
	}

}
//---------------------------------------------------------------------------
// This function is for saving the integer global variable
void TfrmAnalysis::SaveVariable(vmVariables vmVar, int CompName)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
        if(GlobalProfileKey == 0)
        {
            GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
        }
        TManagerVariable::Instance().SetProfileInt(DBTransaction, GlobalProfileKey, vmVar, CompName);
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void TfrmAnalysis::CheckCancelItemsTable()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {   zedLogsList->Add("checking cancel items table");
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text ="select count( a.ZED_PRINT), a.CANCELITEMS_KEY from CANCELITEMS a "
        "where ZED_PRINT < 1 group by a.CANCELITEMS_KEY";

        IBInternalQuery->ExecQuery();

        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           CANCELITEMS_KEY_ids.push_back(IBInternalQuery->FieldByName("CANCELITEMS_KEY")->AsInteger);
        }

        DBTransaction.Commit();

        if(CANCELITEMS_KEY_ids.size() > 0)
        {
          CheckCANCELITEMS_KEY = true;
        }
        else
        {
          CheckCANCELITEMS_KEY = false;
        }
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        zedLogsList->Add("exception caught checking cancel items table: CheckCancelItemsTable() ");
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    zedLogsList->Add("After cancel items table");
}
// ---------------------------------------------------------------------------
void TfrmAnalysis::UpdateCancelItemsTable()
{
    try
    {
         CheckCancelItemsTable();
         zedLogsList->Add("inside UpdateCancelItemsTable ");
         int zed_print = 1;
         if(CheckCANCELITEMS_KEY)
         {
            for(std::vector<int>::iterator it = CANCELITEMS_KEY_ids.begin(); it != CANCELITEMS_KEY_ids.end(); ++it)
            {
               Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
               DBTransaction.StartTransaction();

               TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
               IBInternalQuery->Close();
               IBInternalQuery->SQL->Text =
               "UPDATE CANCELITEMS " "SET " "ZED_PRINT	= :ZED_PRINT " "WHERE " "CANCELITEMS_KEY = :CANCELITEMS_KEY";
               IBInternalQuery->ParamByName("CANCELITEMS_KEY")->AsInteger = reinterpret_cast<int &>(*it);
               IBInternalQuery->ParamByName("ZED_PRINT")->AsInteger = zed_print;
               IBInternalQuery->ExecQuery();
               DBTransaction.Commit();
            }
         }
    }
    catch(Exception &E)
    {
        zedLogsList->Add("exception caught in  UpdateCancelItemsTable UpdateCancelItemsTable ");
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    zedLogsList->Add("After UpdateCancelItemsTable function execution.");
}

void TfrmAnalysis::GetReportData( int _key)
{
   try
   {

	  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	  DBTransaction.StartTransaction();
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT REPORT, TIME_STAMP FROM ZEDS WHERE Z_KEY = :Z_KEY";
	  IBInternalQuery->ParamByName("Z_KEY")->AsInteger = _key;
	  IBInternalQuery->ExecQuery();
      ZedToMail = new TMemoryStream;
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
          IBInternalQuery->FieldByName("REPORT")->SaveToStream(ZedToMail);
      }
      date_time = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
	  DBTransaction.Commit();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }

}

void TfrmAnalysis::UpdateEmailstatus(int z_key)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
      int EMAIL_STATUS = 1;

      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "UPDATE ZEDS " "SET " "EMAIL_STATUS	= :EMAIL_STATUS " "WHERE " "Z_KEY = :Z_KEY";
      IBInternalQuery->ParamByName("Z_KEY")->AsInteger = z_key;
      IBInternalQuery->ParamByName("EMAIL_STATUS")->AsInteger = EMAIL_STATUS;
      IBInternalQuery->ExecQuery();
      DBTransaction.Commit();
      EMAIL_STATUS = 0;
    }
    catch(Exception &E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void _fastcall TfrmAnalysis::ResetPoint(Database::TDBTransaction &DBTransaction, int ContactKey, TResetPoints check)
{
    /*
    1. Check last time a member use their point except refund
    2. Calculate with Reset time
    3. Write to table ResetPoint, date, memberNumber, how many point, date
    4. Erase the points from pointTransaction log
    5. Update contacts table for total spent, earnt point, loaded points column.
    */
    //Checking last time a member use their point except refund
    //DBTransaction.StartTransaction();
    //TTransactionInfo TransactionInfo;
    //UnicodeString DeviceName = GetTerminalName();
    //TransactionInfo.GetTransactionInfo(DBTransaction, DeviceName);
    double points;
    int caseTicked = 0;
    AnsiString boxTicked = "";
    AnsiString options = "";

	//DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    try
    {
    //Check if the contacts still have points
    IBInternalQuery->SQL->Text="select sum(a.ADJUSTMENT) from POINTSTRANSACTIONS a where A.CONTACTS_KEY = :CONTACTKEY";
    IBInternalQuery->ParamByName("CONTACTKEY")->AsString= ContactKey;
    IBInternalQuery->ExecQuery();
    points = IBInternalQuery->FieldByName("SUM") ->AsDouble;
    //If member has point than do the logic

    if(IBInternalQuery->FieldByName("SUM") ->AsDouble > 0.00)
    {

        IBInternalQuery->Close();
        //Check adjustment type
        //MM-6401 Deduct from count key only when member have used perticual adjustment type
        switch(check)
        {
            case All:
            {

                IBInternalQuery->SQL->Text="SELECT skip((select CASE WHEN count(*)> 0 THEN count(*) -1 else 0 END "
                "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE != '7' order by c.TIME_STAMP asc) b "
                "where b.CONTACTS_KEY = :CONTACTKEY))"
                "a.ADJUSTMENT_TYPE,a.POINTSTRANSACTIONS_KEY as keys  "
                "from (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE != '7' order by c.TIME_STAMP asc) a "
                "where A.CONTACTS_KEY = :CONTACTKEY";
                boxTicked = "3";
                break;
            }

            case PurchaseEarn:
            {

                IBInternalQuery->SQL->Text="SELECT skip((select CASE WHEN count(*)> 0 THEN count(*) -1 else 0 END "
                "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE != '7' and c.ADJUSTMENT_TYPE != '3' order by c.TIME_STAMP asc) b "
                "where b.CONTACTS_KEY = :CONTACTKEY))"
                "a.ADJUSTMENT_TYPE,a.POINTSTRANSACTIONS_KEY as keys  "
                "from (select * from POINTSTRANSACTIONS c WHERE c.ADJUSTMENT_TYPE != '7' and c.ADJUSTMENT_TYPE != '3' order by c.TIME_STAMP asc) a "
                "where A.CONTACTS_KEY = :CONTACTKEY";
                boxTicked = "2";
                options = "12";
                break;
            }

            case Purchase:
            {

                IBInternalQuery->SQL->Text="SELECT skip((select CASE WHEN count(*)> 0 THEN count(*) -1 else 0 END "
                "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE = '1' order by c.TIME_STAMP asc) b "
                "where b.CONTACTS_KEY = :CONTACTKEY))"
                "a.ADJUSTMENT_TYPE,a.POINTSTRANSACTIONS_KEY as keys  "
                "from (select * from POINTSTRANSACTIONS c where c.ADJUSTMENT_TYPE = '1' order by c.TIME_STAMP asc) a "
                "where A.CONTACTS_KEY = :CONTACTKEY";
                boxTicked = "1";
                caseTicked = 1;
                break;
            }

            case PurchaseRedeem:
            {

                IBInternalQuery->SQL->Text="SELECT skip((select CASE WHEN count(*)> 0 THEN count(*) -1 else 0 END "
                "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE != '7' and c.ADJUSTMENT_TYPE != '2' order by c.TIME_STAMP asc) b "
                "where b.CONTACTS_KEY = :CONTACTKEY)) "
                "a.ADJUSTMENT_TYPE,a.POINTSTRANSACTIONS_KEY as keys  "
                "from (select * from POINTSTRANSACTIONS c where c.ADJUSTMENT_TYPE != '7' and c.ADJUSTMENT_TYPE != '2' order by c.TIME_STAMP asc) a "
                "where A.CONTACTS_KEY = :CONTACTKEY ";
                boxTicked = "2";
                options = "13";
                break;
            }
            case EarnRedeem:
            {

                IBInternalQuery->SQL->Text="SELECT skip((select CASE WHEN count(*)> 0 THEN count(*) -1 else 0 END "
                "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE != '7' and c.ADJUSTMENT_TYPE != '1' order by c.TIME_STAMP asc) b "
                "where b.CONTACTS_KEY = :CONTACTKEY))"
                "a.ADJUSTMENT_TYPE,a.POINTSTRANSACTIONS_KEY as keys  "
                "from (select * from POINTSTRANSACTIONS c where c.ADJUSTMENT_TYPE != '7' and c.ADJUSTMENT_TYPE != '2' order by c.TIME_STAMP asc) a "
                "where A.CONTACTS_KEY = :CONTACTKEY";
                boxTicked = "2";
                options = "23";
                break;
            }
            case Earn:
            {

                IBInternalQuery->SQL->Text="SELECT skip((select CASE WHEN count(*)> 0 THEN count(*) -1 else 0 END "
                "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE = '2' order by c.TIME_STAMP asc) b "
                "where b.CONTACTS_KEY = :CONTACTKEY)) "
                "a.ADJUSTMENT_TYPE,a.POINTSTRANSACTIONS_KEY as keys  "
                "from (select * from POINTSTRANSACTIONS c where c.ADJUSTMENT_TYPE = '2' order by c.TIME_STAMP asc) a "
                "where A.CONTACTS_KEY = :CONTACTKEY ";
                boxTicked = "1";
                caseTicked = 2;
                break;

            }
            case Redeem:
            {

                IBInternalQuery->SQL->Text="SELECT skip((select CASE WHEN count(*)> 0 THEN count(*) -1 else 0 END "
                "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE = '3' order by c.TIME_STAMP asc) b "
                "where b.CONTACTS_KEY = :CONTACTKEY))"
                "a.ADJUSTMENT_TYPE, a.POINTSTRANSACTIONS_KEY as keys  "
                "from (select * from POINTSTRANSACTIONS c where c.ADJUSTMENT_TYPE = '3' order by c.TIME_STAMP asc) a "
                "where A.CONTACTS_KEY = :CONTACTKEY";
                boxTicked = "1";
                caseTicked = 3;
                break;

            }
        }
        IBInternalQuery->ParamByName("CONTACTKEY")->AsInteger= ContactKey;
        IBInternalQuery->ExecQuery();
        //Check date difference
        //MM-6401
        if(IBInternalQuery->FieldByName("keys")->AsInteger>0)
            CheckDate(DBTransaction,IBInternalQuery->FieldByName("keys")->AsInteger,ContactKey,points,check,caseTicked,boxTicked,options);
        //DBTransaction.Commit();
    }
    }
    catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void _fastcall TfrmAnalysis::CheckDate(Database::TDBTransaction &DBTransaction, int keys,int contacts, double points,TResetPoints check , int caseTicked, AnsiString boxTicked,AnsiString options)
{
    try
    {
        String Adjustment = "";
        TDateTime time ,time2;
        int adjustment_subtype = 0;

        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        if(TGlobalSettings::Instance().ResetDay != 0 && boxTicked == 3)                                              // If all three values are selected
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT DATEDIFF(day,a.TIME_STAMP,cast( 'Now' as date)) ,a.ADJUSTMENT_TYPE,a.ADJUSTMENT_SUBTYPE,a.TIME_STAMP FROM POINTSTRANSACTIONS a where a.CONTACTS_KEY = :CONTACTS and (a.ADJUSTMENT_TYPE = :ADJUSTMENT1 OR a.ADJUSTMENT_TYPE = :ADJUSTMENT2 OR a.ADJUSTMENT_TYPE = :ADJUSTMENT3 )order by a.TIME_STAMP desc";
            IBInternalQuery->ParamByName("CONTACTS")->AsInteger = contacts;
            IBInternalQuery->ParamByName("ADJUSTMENT1")->AsInteger = 1;
            IBInternalQuery->ParamByName("ADJUSTMENT2")->AsInteger = 2;
            IBInternalQuery->ParamByName("ADJUSTMENT3")->AsInteger = 3;
            IBInternalQuery->ExecQuery();

            adjustment_subtype = IBInternalQuery->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger;
            time2 = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            time = Now();
            __int64 hrs = HoursBetween(time,time2);

            if((IBInternalQuery->FieldByName("DATEDIFF")->AsInteger > TGlobalSettings::Instance().ResetDay) ||
               ((IBInternalQuery->FieldByName("DATEDIFF")->AsInteger == TGlobalSettings::Instance().ResetDay) &&
                (hrs >= 24 * TGlobalSettings::Instance().ResetDay)))
            {
                switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
                {
                    case 1:
                    {
                        Adjustment = "Purchased";
                        break;
                    }
                    case 2:
                    {
                        Adjustment = "Earned";
                        break;
                    }
                    case 3:
                    {
                        Adjustment = "Redeemed";
                        break;
                    }
                }
                //MM 5610
                //Write data to table Reset Points
                ResetKey = ResetKey+1;

                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text="INSERT INTO RESETPOINTS(RESET_KEY,TIMESTAMPS,CONTACTS_KEY,POINTS,ADJUSTMENT_TYPE)"
                    " VALUES ("
                    ":RESET_KEY,"
                    "(select current_timestamp from rdb$database),"
                    ":CONTACTS,"
                    ":points,"
                    ":ADJUSTMENT)";
                IBInternalQuery->ParamByName("RESET_KEY")->AsInteger = ResetKey;
                IBInternalQuery->ParamByName("CONTACTS")->AsInteger= contacts;
                IBInternalQuery->ParamByName("points")->AsInteger= points;
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsString= Adjustment;
                IBInternalQuery->ExecQuery();

                //    For putting data regarding Reset in PointsTransaction
                IBInternalQuery->Close();                                                      // Generation of PointsTransactionsKey using Generator
                IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_POINTSTRANSACTIONS, 1) FROM RDB$DATABASE";
                IBInternalQuery->ExecQuery();
                int PointsKey = IBInternalQuery->Fields[0]->AsInteger;

                std::auto_ptr<TPointsTransaction>  pttransaction (new TPointsTransaction());   // For generation of BLOB data

                pttransaction->PointsTransactionsKey = PointsKey;
                pttransaction->ContactKey = contacts;
                pttransaction->TimeStamp = Now();
                pttransaction->TimeStampExported = Now();
                pttransaction->PointsTransactionType = adjustment_subtype;
                pttransaction->PointsTransactionAccountType = adjustment_subtype;
                pttransaction->Adjustment = -points;
                pttransaction->InvoiceNumber = NULL;
                pttransaction->ExportStatus = pesExported;

                IBInternalQuery->Close();                                                    // Insert Query for putting an entry to depict reset of present points
                IBInternalQuery->SQL->Text =
                    "INSERT INTO POINTSTRANSACTIONS "
                    "("
                        "POINTSTRANSACTIONS_KEY,"
                        "CONTACTS_KEY,"
                        "TIME_STAMP,"
                        "TIME_STAMP_EXPORTED,"
                        "ADJUSTMENT_TYPE,"
                        "ADJUSTMENT_SUBTYPE,"
                        "ADJUSTMENT,"
                        "EXPORTED_STATUS,"
                        "MISC,"
                        "INVOICE_NUMBER"
                    ")"
                    " VALUES "
                    "("
                        ":POINTSTRANSACTIONS_KEY,"
                        ":CONTACTS_KEY,"
                        ":TIME_STAMP,"
                        ":TIME_STAMP_EXPORTED,"
                        ":ADJUSTMENT_TYPE,"
                        ":ADJUSTMENT_SUBTYPE,"
                        ":ADJUSTMENT,"
                        ":EXPORTED_STATUS,"
                        ":MISC,"
                        ":INVOICE_NUMBER"
                    ");";
                IBInternalQuery->ParamByName("POINTSTRANSACTIONS_KEY")->AsInteger = PointsKey;
                IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contacts;
                IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
                IBInternalQuery->ParamByName("TIME_STAMP_EXPORTED")->AsDateTime = Now();
                IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = pttReset;
                IBInternalQuery->ParamByName("ADJUSTMENT_SUBTYPE")->AsInteger = adjustment_subtype;
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsCurrency = -points;
                IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = pesExported;

                TMemoryStream *Streamcheck = pttransaction->GetAsStream();
                Streamcheck->Position = 0;
                IBInternalQuery->ParamByName("MISC")->LoadFromStream(Streamcheck);

                IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = "";
                IBInternalQuery->ExecQuery();

                //Update member data in contacts table
                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text="UPDATE Contacts SET TOTAL_SPENT=0,EARNT_POINTS =0, LOADED_POINTS=0  WHERE CONTACTS_KEY =:CONTACTS";
                IBInternalQuery->ParamByName("CONTACTS")->AsInteger = contacts;
                IBInternalQuery->ExecQuery();
                //DBTransaction.Commit();
            }
         }
        if(TGlobalSettings::Instance().ResetDay != 0 && boxTicked == 2)                        // If two options are selected to reset the points like
        {                                                                                      // Purchased & Earned / Redeemed & Earned / Purchased & Redeemed
            AnsiString option1 = options.SubString(1,1);
            AnsiString option2 = options.SubString(2,1);
            int option11 = atoi(option1.c_str());
            int option22 = atoi(option2.c_str());

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT DATEDIFF(day,a.TIME_STAMP,cast( 'Now' as date)) ,a.ADJUSTMENT_TYPE,a.ADJUSTMENT_SUBTYPE,a.TIME_STAMP FROM POINTSTRANSACTIONS a where a.CONTACTS_KEY = :CONTACTS and (a.ADJUSTMENT_TYPE = :ADJUSTMENT1 OR a.ADJUSTMENT_TYPE = :ADJUSTMENT2 )order by a.TIME_STAMP desc";
            IBInternalQuery->ParamByName("CONTACTS")->AsInteger = contacts;
            if(option11 == 1 && option22 == 2)
            {
                IBInternalQuery->ParamByName("ADJUSTMENT1")->AsInteger = 1;
                IBInternalQuery->ParamByName("ADJUSTMENT2")->AsInteger = 2;
            }
            if(option11 == 2 && option22 == 3)
            {
                IBInternalQuery->ParamByName("ADJUSTMENT1")->AsInteger = 2;
                IBInternalQuery->ParamByName("ADJUSTMENT2")->AsInteger = 3;
            }
            if(option11 == 1 && option22 == 3)
            {
                IBInternalQuery->ParamByName("ADJUSTMENT1")->AsInteger = 1;
                IBInternalQuery->ParamByName("ADJUSTMENT2")->AsInteger = 3;
            }
            IBInternalQuery->ExecQuery();

            adjustment_subtype = IBInternalQuery->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger ;
            time2 = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            time = Now();
            __int64 hrs = HoursBetween(time,time2);


            if((IBInternalQuery->FieldByName("DATEDIFF")->AsInteger > TGlobalSettings::Instance().ResetDay) ||
               ((IBInternalQuery->FieldByName("DATEDIFF")->AsInteger == TGlobalSettings::Instance().ResetDay) &&
                (hrs >= 24 * TGlobalSettings::Instance().ResetDay))||
                ((IBInternalQuery->FieldByName("DATEDIFF")->AsInteger <= TGlobalSettings::Instance().ResetDay)
                  && (IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger != option11 &&
                  IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger != option22)))
            {
                switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
                {
                    case 1:
                    {
                        Adjustment = "Purchased";
                        break;
                    }
                    case 2:
                    {
                        Adjustment = "Earned";
                        break;
                    }
                    case 3:
                    {
                        Adjustment = "Redeemed";
                        break;
                    }
                }
                //MM 5610
                //Write data to table Reset Points
                ResetKey = ResetKey+1;

                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text="INSERT INTO RESETPOINTS(RESET_KEY,TIMESTAMPS,CONTACTS_KEY,POINTS,ADJUSTMENT_TYPE)"
                    " VALUES ("
                    ":RESET_KEY,"
                    "(select current_timestamp from rdb$database),"
                    ":CONTACTS,"
                    ":points,"
                    ":ADJUSTMENT)";
                IBInternalQuery->ParamByName("RESET_KEY")->AsInteger = ResetKey;
                IBInternalQuery->ParamByName("CONTACTS")->AsInteger= contacts;
                IBInternalQuery->ParamByName("points")->AsInteger= points;
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsString= Adjustment;
                IBInternalQuery->ExecQuery();

                //    For putting data regarding Reset in PointsTransaction
                IBInternalQuery->Close();                                                      // Generation of PointsTransactionsKey using Generator
                IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_POINTSTRANSACTIONS, 1) FROM RDB$DATABASE";
                IBInternalQuery->ExecQuery();
                int PointsKey = IBInternalQuery->Fields[0]->AsInteger;

                std::auto_ptr<TPointsTransaction>  pttransaction (new TPointsTransaction());   // For generation of BLOB data

                pttransaction->PointsTransactionsKey = PointsKey;
                pttransaction->ContactKey = contacts;
                pttransaction->TimeStamp = Now();
                pttransaction->TimeStampExported = Now();
                pttransaction->PointsTransactionType = adjustment_subtype;
                pttransaction->PointsTransactionAccountType = adjustment_subtype;
                pttransaction->Adjustment = -points;
                pttransaction->InvoiceNumber = NULL;
                pttransaction->ExportStatus = pesExported;

                IBInternalQuery->Close();                                                      // Insert Query for putting an entry to depict reset of present points
                IBInternalQuery->SQL->Text =
                    "INSERT INTO POINTSTRANSACTIONS "
                    "("
                        "POINTSTRANSACTIONS_KEY,"
                        "CONTACTS_KEY,"
                        "TIME_STAMP,"
                        "TIME_STAMP_EXPORTED,"
                        "ADJUSTMENT_TYPE,"
                        "ADJUSTMENT_SUBTYPE,"
                        "ADJUSTMENT,"
                        "EXPORTED_STATUS,"
                        "MISC,"
                        "INVOICE_NUMBER"
                    ")"
                    " VALUES "
                    "("
                        ":POINTSTRANSACTIONS_KEY,"
                        ":CONTACTS_KEY,"
                        ":TIME_STAMP,"
                        ":TIME_STAMP_EXPORTED,"
                        ":ADJUSTMENT_TYPE,"
                        ":ADJUSTMENT_SUBTYPE,"
                        ":ADJUSTMENT,"
                        ":EXPORTED_STATUS,"
                        ":MISC,"
                        ":INVOICE_NUMBER"
                    ");";
                IBInternalQuery->ParamByName("POINTSTRANSACTIONS_KEY")->AsInteger = PointsKey;
                IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contacts;
                IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
                IBInternalQuery->ParamByName("TIME_STAMP_EXPORTED")->AsDateTime = Now();
                IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = pttReset;
                IBInternalQuery->ParamByName("ADJUSTMENT_SUBTYPE")->AsInteger = adjustment_subtype;
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsCurrency = -points;
                IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = pesExported;

                TMemoryStream *Streamcheck = pttransaction->GetAsStream();
                Streamcheck->Position = 0;
                IBInternalQuery->ParamByName("MISC")->LoadFromStream(Streamcheck);

                IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = "";
                IBInternalQuery->ExecQuery();

                //Update member data in contacts table
                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text="UPDATE Contacts SET TOTAL_SPENT=0,EARNT_POINTS =0, LOADED_POINTS=0  WHERE CONTACTS_KEY =:CONTACTS";
                IBInternalQuery->ParamByName("CONTACTS")->AsInteger = contacts;
                IBInternalQuery->ExecQuery();
                //DBTransaction.Commit();
            }
         }
        if(TGlobalSettings::Instance().ResetDay != 0 && boxTicked == 1)                                           // When only one option is selected for reseting the points
        {                                                                                                         // like Points Purchased or Points Redeemed or Points Earned
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT DATEDIFF(day,a.TIME_STAMP,cast( 'Now' as date)) ,a.ADJUSTMENT_TYPE,a.ADJUSTMENT_SUBTYPE,a.TIME_STAMP FROM POINTSTRANSACTIONS a where a.CONTACTS_KEY = :CONTACTS and a.ADJUSTMENT_TYPE = :ADJUSTMENT order by a.TIME_STAMP desc";
            IBInternalQuery->ParamByName("CONTACTS")->AsInteger = contacts;
            if(caseTicked == 1)
            {
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsInteger = 1;
            }
            if(caseTicked == 2)
            {
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsInteger = 2;
            }
            if(caseTicked == 3)
            {
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsInteger = 3;
            }
            IBInternalQuery->ExecQuery();

            adjustment_subtype = IBInternalQuery->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger;
            time2 = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            time = Now();
            __int64 hrs = HoursBetween(time,time2);

            if((IBInternalQuery->FieldByName("DATEDIFF")->AsInteger > TGlobalSettings::Instance().ResetDay) ||
               ((IBInternalQuery->FieldByName("DATEDIFF")->AsInteger == TGlobalSettings::Instance().ResetDay) &&
                (hrs >= 24 * TGlobalSettings::Instance().ResetDay))||
                ((IBInternalQuery->FieldByName("DATEDIFF")->AsInteger <= TGlobalSettings::Instance().ResetDay)
                  && IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger != caseTicked))
            {
                switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
                {
                    case 1:
                    {
                        Adjustment = "Purchased";
                        break;
                    }
                    case 2:
                    {
                        Adjustment = "Earned";
                        break;
                    }
                    case 3:
                    {
                        Adjustment = "Redeemed";
                        break;
                    }
                }
                //MM 5610
                //Write data to table Reset Points
                ResetKey = ResetKey+1;

                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text="INSERT INTO RESETPOINTS(RESET_KEY,TIMESTAMPS,CONTACTS_KEY,POINTS,ADJUSTMENT_TYPE)"
                    " VALUES ("
                    ":RESET_KEY,"
                    "(select current_timestamp from rdb$database),"
                    ":CONTACTS,"
                    ":points,"
                    ":ADJUSTMENT)";
                IBInternalQuery->ParamByName("RESET_KEY")->AsInteger = ResetKey;
                IBInternalQuery->ParamByName("CONTACTS")->AsInteger= contacts;
                IBInternalQuery->ParamByName("points")->AsInteger= points;
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsString= Adjustment;
                IBInternalQuery->ExecQuery();


                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_POINTSTRANSACTIONS, 1) FROM RDB$DATABASE";
                IBInternalQuery->ExecQuery();
                int PointsKey = IBInternalQuery->Fields[0]->AsInteger;

                std::auto_ptr<TPointsTransaction>  pttransaction (new TPointsTransaction());   // For generation of BLOB data

                pttransaction->PointsTransactionsKey = PointsKey;
                pttransaction->ContactKey = contacts;
                pttransaction->TimeStamp = Now();
                pttransaction->TimeStampExported = Now();
                pttransaction->PointsTransactionType = adjustment_subtype;
                pttransaction->PointsTransactionAccountType = adjustment_subtype;
                pttransaction->Adjustment = -points;
                pttransaction->InvoiceNumber = NULL;
                pttransaction->ExportStatus = pesExported;

                // Insert Query
                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text =
                    "INSERT INTO POINTSTRANSACTIONS "
                    "("
                        "POINTSTRANSACTIONS_KEY,"
                        "CONTACTS_KEY,"
                        "TIME_STAMP,"
                        "TIME_STAMP_EXPORTED,"
                        "ADJUSTMENT_TYPE,"
                        "ADJUSTMENT_SUBTYPE,"
                        "ADJUSTMENT,"
                        "EXPORTED_STATUS,"
                        "MISC,"
                        "INVOICE_NUMBER"
                    ")"
                    " VALUES "
                    "("
                        ":POINTSTRANSACTIONS_KEY,"
                        ":CONTACTS_KEY,"
                        ":TIME_STAMP,"
                        ":TIME_STAMP_EXPORTED,"
                        ":ADJUSTMENT_TYPE,"
                        ":ADJUSTMENT_SUBTYPE,"
                        ":ADJUSTMENT,"
                        ":EXPORTED_STATUS,"
                        ":MISC,"
                        ":INVOICE_NUMBER"
                    ");";
                IBInternalQuery->ParamByName("POINTSTRANSACTIONS_KEY")->AsInteger = PointsKey;
                IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contacts;
                IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
                IBInternalQuery->ParamByName("TIME_STAMP_EXPORTED")->AsDateTime = Now();
                IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = pttReset;
                IBInternalQuery->ParamByName("ADJUSTMENT_SUBTYPE")->AsInteger = adjustment_subtype;
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsCurrency = -points;
                IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = pesExported;

                TMemoryStream *Streamcheck = pttransaction->GetAsStream();
                Streamcheck->Position = 0;
                IBInternalQuery->ParamByName("MISC")->LoadFromStream(Streamcheck);

                IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = "";
                IBInternalQuery->ExecQuery();

                //Update member data in contacts table
                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text="UPDATE Contacts SET TOTAL_SPENT=0,EARNT_POINTS =0, LOADED_POINTS=0  WHERE CONTACTS_KEY =:CONTACTS";
                IBInternalQuery->ParamByName("CONTACTS")->AsInteger = contacts;
                IBInternalQuery->ExecQuery();
                //DBTransaction.Commit();
            }
         }
         if(TGlobalSettings::Instance().ResetDay == 0)
         {
               IBInternalQuery->Close();
               IBInternalQuery->SQL->Text = "SELECT DATEDIFF(day,a.TIME_STAMP,cast( 'Now' as date)) ,a.ADJUSTMENT_TYPE,a.ADJUSTMENT_SUBTYPE FROM POINTSTRANSACTIONS a where a.CONTACTS_KEY = :CONTACTS  order by a.TIME_STAMP desc";
               IBInternalQuery->ParamByName("CONTACTS")->AsInteger = contacts;
               IBInternalQuery->ExecQuery();

               switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
                {
                    case 1:
                    {
                        Adjustment = "Purchased";
                        break;
                    }
                    case 2:
                    {
                        Adjustment = "Earned";
                        break;
                    }
                    case 3:
                    {
                        Adjustment = "Redeemed";
                        break;
                    }
                }
                //MM 5610
                //Write data to table Reset Points
                ResetKey = ResetKey+1;

                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text="INSERT INTO RESETPOINTS(RESET_KEY,TIMESTAMPS,CONTACTS_KEY,POINTS,ADJUSTMENT_TYPE)"
                    " VALUES ("
                    ":RESET_KEY,"
                    "(select current_timestamp from rdb$database),"
                    ":CONTACTS,"
                    ":points,"
                    ":ADJUSTMENT)";
                IBInternalQuery->ParamByName("RESET_KEY")->AsInteger = ResetKey;
                IBInternalQuery->ParamByName("CONTACTS")->AsInteger= contacts;
                IBInternalQuery->ParamByName("points")->AsInteger= points;
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsString= Adjustment;
                IBInternalQuery->ExecQuery();

                IBInternalQuery->Close();                                                     // Generation of PointsTransactionsKey using Generator
                IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_POINTSTRANSACTIONS, 1) FROM RDB$DATABASE";
                IBInternalQuery->ExecQuery();
                int PointsKey = IBInternalQuery->Fields[0]->AsInteger;

                std::auto_ptr<TPointsTransaction>  pttransaction (new TPointsTransaction());   // For generation of BLOB data

                pttransaction->PointsTransactionsKey = PointsKey;
                pttransaction->ContactKey = contacts;
                pttransaction->TimeStamp = Now();
                pttransaction->TimeStampExported = Now();
                pttransaction->PointsTransactionType = adjustment_subtype;
                pttransaction->PointsTransactionAccountType = adjustment_subtype;
                pttransaction->Adjustment = -points;
                pttransaction->InvoiceNumber = NULL;
                pttransaction->ExportStatus = pesExported;

                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text =
                    "INSERT INTO POINTSTRANSACTIONS "
                    "("
                        "POINTSTRANSACTIONS_KEY,"
                        "CONTACTS_KEY,"
                        "TIME_STAMP,"
                        "TIME_STAMP_EXPORTED,"
                        "ADJUSTMENT_TYPE,"
                        "ADJUSTMENT_SUBTYPE,"
                        "ADJUSTMENT,"
                        "EXPORTED_STATUS,"
                        "MISC,"
                        "INVOICE_NUMBER"
                    ")"
                    " VALUES "
                    "("
                        ":POINTSTRANSACTIONS_KEY,"
                        ":CONTACTS_KEY,"
                        ":TIME_STAMP,"
                        ":TIME_STAMP_EXPORTED,"
                        ":ADJUSTMENT_TYPE,"
                        ":ADJUSTMENT_SUBTYPE,"
                        ":ADJUSTMENT,"
                        ":EXPORTED_STATUS,"
                        ":MISC,"
                        ":INVOICE_NUMBER"
                    ");";
                IBInternalQuery->ParamByName("POINTSTRANSACTIONS_KEY")->AsInteger = PointsKey;
                IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contacts;
                IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
                IBInternalQuery->ParamByName("TIME_STAMP_EXPORTED")->AsDateTime = Now();
                IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = pttReset;
                IBInternalQuery->ParamByName("ADJUSTMENT_SUBTYPE")->AsInteger = adjustment_subtype;
                IBInternalQuery->ParamByName("ADJUSTMENT")->AsCurrency = -points;
                IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = pesExported;

                TMemoryStream *Streamcheck = pttransaction->GetAsStream();
                Streamcheck->Position = 0;
                IBInternalQuery->ParamByName("MISC")->LoadFromStream(Streamcheck);

                IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = "";
                IBInternalQuery->ExecQuery();

                //Update member data in contacts table
                IBInternalQuery->Close();
                IBInternalQuery->SQL->Text="UPDATE Contacts SET TOTAL_SPENT=0,EARNT_POINTS =0, LOADED_POINTS=0  WHERE CONTACTS_KEY =:CONTACTS";
                IBInternalQuery->ParamByName("CONTACTS")->AsInteger = contacts;
                IBInternalQuery->ExecQuery();
                //DBTransaction.Commit();
            }
        }
        catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

bool  TfrmAnalysis::CheckContactsAdjustmentType(Database::TDBTransaction &DBTransaction, int contacts, int type)
{
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        switch(type)
            {
                case All:
                {

                    IBInternalQuery->SQL->Text="select count(*) "
                    "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE != '7' order by c.TIME_STAMP asc) b "
                    "where b.CONTACTS_KEY = :CONTACTKEY";

                    break;
                }

                case PurchaseEarn:
                {

                    IBInternalQuery->SQL->Text="select count(*) "
                    "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE != '7' and c.ADJUSTMENT_TYPE != '3' order by c.TIME_STAMP asc) b "
                    "where b.CONTACTS_KEY = :CONTACTKEY";
                    break;
                }

                case Purchase:
                {

                    IBInternalQuery->SQL->Text="select count(*) "
                    "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE = '1' order by c.TIME_STAMP asc) b "
                    "where b.CONTACTS_KEY = :CONTACTKEY";
                    break;
                }

                case PurchaseRedeem:
                {

                    IBInternalQuery->SQL->Text="select count(*) "
                    "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE != '7' and c.ADJUSTMENT_TYPE != '2' order by c.TIME_STAMP asc) b "
                    "where b.CONTACTS_KEY = :CONTACTKEY";
                    break;
                }
                case EarnRedeem:
                {

                    IBInternalQuery->SQL->Text="select count(*) "
                    "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE != '7' and c.ADJUSTMENT_TYPE != '1' order by c.TIME_STAMP asc) b "
                    "where b.CONTACTS_KEY = :CONTACTKEY";
                    break;
                }
                case Earn:
                {

                    IBInternalQuery->SQL->Text="select count(*) "
                    "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE = '2' order by c.TIME_STAMP asc) b "
                    "where b.CONTACTS_KEY = :CONTACTKEY";
                    break;

                }
                case Redeem:
                {

                    IBInternalQuery->SQL->Text="select count(*) "
                    "FROM (select * from POINTSTRANSACTIONS c  where c.ADJUSTMENT_TYPE = '3' order by c.TIME_STAMP asc) b "
                    "where b.CONTACTS_KEY = :CONTACTKEY";
                    break;

                }
            }

        IBInternalQuery->ParamByName("CONTACTKEY")->AsInteger= contacts;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->FieldByName("COUNT")->AsInteger > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
//---------------------------------------------------------------------------
void TfrmAnalysis::UpdateArcMallExport(Database::TDBTransaction &DBTransaction)
{

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery2 = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery3 = DBTransaction.Query(DBTransaction.AddQuery());

		TIBSQL *IBMallExport = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBMallExportHourly = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBMallExportTransaction = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBMallExportOtherDetails = DBTransaction.Query(DBTransaction.AddQuery());

		TIBSQL *IBArcMallExport = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcMallExportHourly = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcMallExportTransaction = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBArcMallExportOtherDetails = DBTransaction.Query(DBTransaction.AddQuery());

        UnicodeString MALLCODE = "";
        UnicodeString TENANT_NAME = "";
        UnicodeString CLASSCODE = "";
        UnicodeString TRADECODE = "";
        UnicodeString OUTLETNUMBER = "";
        Currency NEW_GRANDTOTAL = 0;
        Currency OLD_GRANDTOTAL = 0;
        UnicodeString SALESTYPE = "";
        Currency DEPARTMENTSUM = 0;
        Currency REGDISCOUNT_AMOUNT = 0;
        Currency EMPDISCOUNT_AMOUNT = 0;
        Currency SCDISCOUNT_AMOUNT = 0;
        Currency VIPDISCOUNT_AMOUNT = 0;
        Currency PWDDISCOUNT_AMOUNT = 0;
        Currency GPCDISCOUNT_AMOUNT = 0;
        Currency DISCOUNTG1_AMOUNT = 0;
        Currency DISCOUNTG2_AMOUNT = 0;
        Currency DISCOUNTG3_AMOUNT = 0;
        Currency DISCOUNTG4_AMOUNT = 0;
        Currency DISCOUNTG5_AMOUNT = 0;
        Currency DISCOUNTG6_AMOUNT = 0;
        Currency VAT_SALES = 0;
        Currency OTHERTAX = 0;
        Currency ADJUSTMENTS = 0;
        Currency POS_ADJ = 0;
        Currency NEG_ADJ = 0;
        Currency POS_ADJ_NONTAX = 0;
        Currency NEG_ADJ_NONTAX = 0;
        Currency DAILY_SALES = 0;
        Currency VOID_AMOUNT = 0;
        Currency REFUND_AMOUNT = 0;
        Currency TAX_INCLUSIVE_SALES = 0;
        Currency VATEXEMPT_SALES = 0;
        Currency CHARGECRED_SALES = 0;
        Currency CHARGECRED_SALES_TAX = 0;
        Currency GIFT_CHEQUE_SALES = 0;
        Currency DEBIT_CARD_SALES = 0;
        Currency OTHER_TENDER_SALES = 0;
        Currency MASTERCARD_SALES = 0;
        Currency VISA_SALES = 0;
        Currency AMEX_SALES = 0;
        Currency DINERS_SALES = 0;
        Currency JCB_SALES = 0;
        Currency OTHER_CARD_SALES = 0;
        Currency SCHARGE_AMOUNT = 0;
        Currency OTHER_CHARGES = 0;
        int FIRST_TRANSACTION = 0;
        int LAST_TRANSACTION = 0;
        int TRANSACTION_COUNT = 0;
        UnicodeString BEGINNING_INVOICE = "";
        UnicodeString ENDING_INVOICE = "";
        int CASH_COUNT = 0;
        int GC_COUNT = 0;
        int DEBIT_COUNT = 0;
        int OTHER_TENDER_COUNT = 0;
        int MASTERCARD_COUNT = 0;
        int VISA_COUNT = 0;
        int AMEX_COUNT = 0;
        int DINERS_COUNT = 0;
        int JCB_COUNT = 0;
        int OTHERCARD_COUNT = 0;
        UnicodeString TERMINAL_NAME = "";
        UnicodeString SERIALNUMBER = "";
        TDateTime TRANSACTIONTIME;
        TDateTime TRANSACTIONDATE;
        Currency GROSS_SALES = 0;
        int VOID_COUNT = 0;
        int REGDISCOUNT_COUNT = 0;
        int REFUND_COUNT = 0;
        int SCDISCOUNT_COUNT = 0;
        Currency EODOLD = 0;
        Currency EODNEW = 0;
        TDateTime DATE_VALUE;
        Currency NOVELTY_SALES = 0;
        Currency MISC_SALES = 0;
        Currency LOCALTAXES = 0;
        Currency PHARMA_SALES = 0;
        Currency NONPHARMA_SALES = 0;
        Currency SALESNOTSUBTORENT = 0;
        Currency REPTRANSACTION_AMOUNT = 0;
        int REPTRANSACTION_COUNT = 0;
        Currency CASH_SALES = 0;
        UnicodeString HOUR_VALUE = "";
        Currency TOTALDISCOUNT = 0;
        UnicodeString BEGINNING_OR = "";
        UnicodeString ENDING_OR = "";
        Currency LOCTAXEXEMPTDLY_SALES = 0;
        int FINEDINECUST_COUNT = 0;
        Currency TENDERSURCHARGES = 0;
        Currency NONVAT_SALES = 0;
        Currency CHECK_SALES = 0;
        Currency EPAY_SALES = 0;
        Currency NO_SALES = 0;
        int PREVEODCOUNTER = 0;
        int CURRENTEODCOUNTER = 0;
        int DISCOUNT_COUNT = 0;
        Currency CARD_SALES = 0;
        Currency OTHER_SALES = 0;
        Currency SALESTYPE_FOOD = 0;
        Currency SALESTYPE_NONFOOD = 0;
        Currency SALESTYPE_GROCERIES = 0;
        Currency SALESTYPE_MEDICINES = 0;
        Currency SALESTYPE_OTHERS = 0;
        UnicodeString MINUTE_VALUE = "";

		try
		{
            // For ARCMALLEXPORT
			IBMallExport->Close();
			IBMallExport->SQL->Text = "select * from MALLEXPORT";
            MallExportReadFromDB(IBMallExport->SQL->Text,DataRead);

            std::map<UnicodeString, UnicodeString>::iterator it;

            for(int i=0;i<DataRead.size();i++) {
                if(i==0) {
                    it = DataRead.find("MallCode");
                    MALLCODE = (it->second);
                } else if(i==1) {
                    it = DataRead.find("TenantNo");
                    TENANT_NAME = (it->second);
                } else if(i==2) {
                    it = DataRead.find("ClassCode");
                    CLASSCODE = (it->second);
                } else if(i==3) {
                    it = DataRead.find("TradeCode");
                    TRADECODE = (it->second);
                } else if(i==4) {
                    it = DataRead.find("OutletNumber");
                    OUTLETNUMBER = (it->second);
                } else if(i==5) {
                    it = DataRead.find("GrandTotal");
                    NEW_GRANDTOTAL = (it->second);
                } else if(i==6) {
                    it = DataRead.find("GrandTotalOld");
                    OLD_GRANDTOTAL = (it->second);
                } else if(i==7) {
                    it = DataRead.find("SalesType");
                    SALESTYPE = (it->second);
                } else if(i==8) {
                    it = DataRead.find("DepartmentSum");
                    DEPARTMENTSUM = (it->second);
                } else if(i==9) {
                    it = DataRead.find("TotalRegularDiscount");
                    REGDISCOUNT_AMOUNT = (it->second);
                } else if(i==10) {
                    it = DataRead.find("EmployeeDiscount");
                    EMPDISCOUNT_AMOUNT = (it->second);
                } else if(i==11) {
                    it = DataRead.find("TotalSeniorCitizenDiscount");
                    SCDISCOUNT_AMOUNT = (it->second);
                } else if(i==12) {
                    it = DataRead.find("VIPDiscount");
                    VIPDISCOUNT_AMOUNT = (it->second);
                } else if(i==13) {
                    it = DataRead.find("DisabilityDiscount");
                    PWDDISCOUNT_AMOUNT = (it->second);
                } else if(i==14) {
                    it = DataRead.find("GPCDiscount");
                    GPCDISCOUNT_AMOUNT = (it->second);
                } else if(i==15) {
                    it = DataRead.find("DiscountG1");
                    DISCOUNTG1_AMOUNT = (it->second);
                } else if(i==16) {
                    it = DataRead.find("DiscountG2");
                    DISCOUNTG2_AMOUNT = (it->second);
                } else if(i==17) {
                    it = DataRead.find("DiscountG3");
                    DISCOUNTG3_AMOUNT = (it->second);;
                } else if(i==18) {
                    it = DataRead.find("DiscountG4");
                    DISCOUNTG4_AMOUNT = (it->second);
                } else if(i==19) {
                    it = DataRead.find("DiscountG5");
                    DISCOUNTG5_AMOUNT = (it->second);
                } else if(i==20) {
                    it = DataRead.find("DiscountG6");
                    DISCOUNTG6_AMOUNT = (it->second);
                } else if(i==21) {
                    it = DataRead.find("TotalSaleTax");
                    VAT_SALES = (it->second);
                } else if(i==22) {
                    it = DataRead.find("OtherTax");
                    OTHERTAX = (it->second);
                } else if(i==23) {
                    it = DataRead.find("Adjustments");
                    ADJUSTMENTS = (it->second);
                } else if(i==24) {
                    it = DataRead.find("PositiveAdjustments");
                    POS_ADJ = (it->second);
                } else if(i==25) {
                    it = DataRead.find("NegativeAdjustments");
                    NEG_ADJ = (it->second);
                } else if(i==26) {
                    it = DataRead.find("PositiveAdjustmentsNonTax");
                    POS_ADJ_NONTAX = (it->second);
                } else if(i==27) {
                    it = DataRead.find("NegativeAdjustmentsNonTax");
                    NEG_ADJ_NONTAX = (it->second);
                } else if(i==28) {
                    it = DataRead.find("DailySales");
                    DAILY_SALES = (it->second);
                } else if(i==29) {
                    it = DataRead.find("TotalCancelled");
                    VOID_AMOUNT = (it->second);
                } else if(i==30) {
                    it = DataRead.find("TotalRefund");
                    REFUND_AMOUNT = (it->second);
                } else if(i==31) {
                    it = DataRead.find("TaxInclusiveSales");
                    TAX_INCLUSIVE_SALES = (it->second);
                } else if(i==32) {
                    it = DataRead.find("TaxExemptSales");
                    VATEXEMPT_SALES = (it->second);
                } else if(i==33) {
                    it = DataRead.find("ChargeCreditSales");
                    CHARGECRED_SALES = (it->second);
                } else if(i==34) {
                    it = DataRead.find("CashSales");
                    CHARGECRED_SALES_TAX = (it->second);
                } else if(i==35) {
                    it = DataRead.find("GiftChequeSales");
                    GIFT_CHEQUE_SALES = (it->second);
                } else if(i==36) {
                    it = DataRead.find("DebitCardSales");
                    DEBIT_CARD_SALES = (it->second);
                } else if(i==37) {
                    it = DataRead.find("OtherTender");
                    OTHER_TENDER_SALES = (it->second);
                } else if(i==38) {
                    it = DataRead.find("TotalMastercard");
                    MASTERCARD_SALES = (it->second);
                } else if(i==39) {
                    it = DataRead.find("TotalVisa");
                    VISA_SALES = (it->second);
                } else if(i==40) {
                    it = DataRead.find("TotalAmericanExpress");
                    AMEX_SALES = (it->second);
                } else if(i==41) {
                    it = DataRead.find("TotallDiners");
                    DINERS_SALES = (it->second);
                } else if(i==42) {
                    it = DataRead.find("TotalJCB");
                    JCB_SALES = (it->second);
                } else if(i==43) {
                    it = DataRead.find("TotalOtherCards");
                    OTHER_CARD_SALES = (it->second);
                } else if(i==44) {
                    it = DataRead.find("TotalServiceCharge");
                    SCHARGE_AMOUNT = (it->second);
                } else if(i==45) {
                    it = DataRead.find("OtherCharges");
                    OTHER_CHARGES = (it->second);
                } else if(i==46) {
                    it = DataRead.find("FirstTransaction");
                    FIRST_TRANSACTION = StrToInt(it->second);
                } else if(i==47) {
                    it = DataRead.find("LastTransaction");
                    LAST_TRANSACTION = StrToInt(it->second);
                } else if(i==48) {
                    it = DataRead.find("TransactionCount");
                    TRANSACTION_COUNT = StrToInt(it->second);
                } else if(i==49) {
                    it = DataRead.find("BeginningInvoiceNo");
                    BEGINNING_INVOICE = (it->second);
                } else if(i==50) {
                    it = DataRead.find("EndingInvoiveNo");
                    ENDING_INVOICE = (it->second);
                } else if(i==51) {
                    it = DataRead.find("CashTransactions");
                    CASH_COUNT = StrToInt(it->second);
                } else if(i==52) {
                    it = DataRead.find("GiftCardChequeTransactions");
                    GC_COUNT = StrToInt(it->second);
                } else if(i==53) {
                    it = DataRead.find("DebitCardTransactions");
                    DEBIT_COUNT = StrToInt(it->second);
                } else if(i==54) {
                    it = DataRead.find("OtherTenderTransctions");
                    OTHER_TENDER_COUNT = StrToInt(it->second);
                } else if(i==55) {
                    it = DataRead.find("MasterCardTransactions");
                    MASTERCARD_COUNT = StrToInt(it->second);
                } else if(i==56) {
                    it = DataRead.find("VisaTransactions");
                    VISA_COUNT = StrToInt(it->second);
                } else if(i==57) {
                    it = DataRead.find("AmericanExpressTransactions");
                    AMEX_COUNT = StrToInt(it->second);
                } else if(i==58) {
                    it = DataRead.find("DinersTransactions");
                    DINERS_COUNT = StrToInt(it->second);
                } else if(i==59) {
                    it = DataRead.find("JCBTransactions");
                    JCB_COUNT = StrToInt(it->second);
                } else if(i==60) {
                    it = DataRead.find("OtherCardTransactions");
                    OTHERCARD_COUNT = StrToInt(it->second);
                } else if(i==61) {
                    it = DataRead.find("TerminalNo");
                    TERMINAL_NAME = (it->second);
                } else if(i==62) {
                    it = DataRead.find("SerialNumber");
                    SERIALNUMBER = (it->second);
                } else if(i==63) {
                    it = DataRead.find("TransactionTime");
                    TRANSACTIONTIME = StrToDateTime(it->second);
                } else if(i==64) {
                    it = DataRead.find("TransactionDate");
                    TRANSACTIONDATE = StrToDateTime(it->second);
                } else if(i==65) {
                    it = DataRead.find("TotalGrossSale");
                    GROSS_SALES = (it->second);
                } else if(i==66) {
                    it = DataRead.find("TotalCancelledCount");
                    VOID_COUNT = StrToInt(it->second);
                } else if(i==67) {
                    it = DataRead.find("TotalRegularDiscountCount");
                    REGDISCOUNT_COUNT = StrToInt(it->second);
                } else if(i==68) {
                    it = DataRead.find("TotalRefundCount");
                    REFUND_COUNT = StrToInt(it->second);
                } else if(i==69) {
                    it = DataRead.find("TotalSeniorCitizenDiscountCount");
                    SCDISCOUNT_COUNT = StrToInt(it->second);
                } else if(i==70) {
                    it = DataRead.find("TotalEndOfDayOld");
                    EODOLD = (it->second);
                } else if(i==71) {
                    it = DataRead.find("TotalEndOfDay");
                    EODNEW = (it->second);
                } else if(i==72) {
                    it = DataRead.find("Date");
                    DATE_VALUE = StrToDateTime(it->second);
                } else if(i==73) {
                    it = DataRead.find("NoveltySales");
                    NOVELTY_SALES = (it->second);
                } else if(i==74) {
                    it = DataRead.find("MiscellaneusSales");
                    MISC_SALES = (it->second);
                } else if(i==75) {
                    it = DataRead.find("LocalTaxes");
                    LOCALTAXES = (it->second);
                } else if(i==76) {
                    it = DataRead.find("PharmaSales");
                    PHARMA_SALES = (it->second);
                } else if(i==77) {
                    it = DataRead.find("NonPharmaSales");
                    NONPHARMA_SALES = (it->second);
                } else if(i==78) {
                    it = DataRead.find("SalesNotSubjectToRent");
                    SALESNOTSUBTORENT = (it->second);
                } else if(i==79) {
                    it = DataRead.find("TotalReprintedTransc");
                    REPTRANSACTION_AMOUNT = (it->second);
                } else if(i==80) {
                    it = DataRead.find("TotalReprintedTranscCount");
                    REPTRANSACTION_COUNT = StrToInt(it->second);
                } else if(i==81) {
                    it = DataRead.find("CashSales");
                    CASH_SALES = (it->second);
                } else if(i==82) {
                    it = DataRead.find("Hour");
                    HOUR_VALUE = (it->second);
                } else if(i==83) {
                    it = DataRead.find("TotalDiscount");
                    TOTALDISCOUNT = (it->second);
                } else if(i==84) {
                    it = DataRead.find("BeginningORNo");
                    BEGINNING_OR = (it->second);;
                } else if(i==85) {
                    it = DataRead.find("EndingORNo");
                    ENDING_OR = (it->second);
                } else if(i==86) {
                    it = DataRead.find("LocalTaxExemptDailySales");
                    LOCTAXEXEMPTDLY_SALES = (it->second);
                } else if(i==87) {
                    it = DataRead.find("FineDiningCustomerCount");
                    FINEDINECUST_COUNT = StrToInt(it->second);
                } else if(i==88) {
                    it = DataRead.find("TenderSurcharges");
                    TENDERSURCHARGES = (it->second);
                } else if(i==89) {
                    it = DataRead.find("NonVatSales");
                    NONVAT_SALES = (it->second);
                } else if(i==90) {
                    it = DataRead.find("CheckSales");
                    CHECK_SALES = (it->second);
                } else if(i==91) {
                    it = DataRead.find("EPaySales");
                    EPAY_SALES = (it->second);
                } else if(i==92) {
                    it = DataRead.find("NoSales");
                    NO_SALES = (it->second);;
                } else if(i==93) {
                    it = DataRead.find("PrevEODCounter");
                    PREVEODCOUNTER = StrToInt(it->second);
                } else if(i==94) {
                    it = DataRead.find("CurrentEODCounter");
                    CURRENTEODCOUNTER = StrToInt(it->second);
                } else if(i==95) {
                    it = DataRead.find("DiscountCount");
                    DISCOUNT_COUNT = StrToInt(it->second);
                } else if(i==96) {
                    it = DataRead.find("CardSales");
                    CARD_SALES = (it->second);
                } else if(i==97) {
                    it = DataRead.find("OtherSales");
                    OTHER_SALES = (it->second);
                } else if(i==98) {
                    it = DataRead.find("SalesTypeFood");
                    SALESTYPE_FOOD = (it->second);
                } else if(i==99) {
                    it = DataRead.find("SalesTypeNon-Food");
                    SALESTYPE_NONFOOD = (it->second);
                } else if(i==100) {
                    it = DataRead.find("SalesTypeGroceries");
                    SALESTYPE_GROCERIES = (it->second);
                } else if(i==101) {
                    it = DataRead.find("SalesTypeMedicines");
                    SALESTYPE_MEDICINES = (it->second);
                } else if(i==102) {
                    it = DataRead.find("SalesTypeOthers");
                    SALESTYPE_OTHERS = (it->second);
                }
            }

			IBMallExport->ExecQuery();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCMALLEXPORT_KEY, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int ArcMallKey = IBInternalQuery->Fields[0]->AsInteger;

            TIBSQL *IBArcMallExport = DBTransaction.Query(DBTransaction.AddQuery());
			IBArcMallExport->SQL->Text = "INSERT INTO ARCMALLEXPORT "
            "( "
            "ARCMALLEXPORT.ARCMALLEXPORT_KEY, "
            "ARCMALLEXPORT.MALLCODE, ARCMALLEXPORT.TENANT_NAME, ARCMALLEXPORT.CLASSCODE, ARCMALLEXPORT.TRADECODE, ARCMALLEXPORT.OUTLETNUMBER, "
            "ARCMALLEXPORT.NEW_GRANDTOTAL, ARCMALLEXPORT.OLD_GRANDTOTAL, ARCMALLEXPORT.SALESTYPE, ARCMALLEXPORT.DEPARTMENTSUM, ARCMALLEXPORT.REGDISCOUNT_AMOUNT, "
            "ARCMALLEXPORT.EMPDISCOUNT_AMOUNT, ARCMALLEXPORT.SCDISCOUNT_AMOUNT, ARCMALLEXPORT.VIPDISCOUNT_AMOUNT, ARCMALLEXPORT.PWDDISCOUNT_AMOUNT, ARCMALLEXPORT.GPCDISCOUNT_AMOUNT, "
            "ARCMALLEXPORT.DISCOUNTG1_AMOUNT, ARCMALLEXPORT.DISCOUNTG2_AMOUNT, ARCMALLEXPORT.DISCOUNTG3_AMOUNT, ARCMALLEXPORT.DISCOUNTG4_AMOUNT, ARCMALLEXPORT.DISCOUNTG5_AMOUNT, "
            "ARCMALLEXPORT.DISCOUNTG6_AMOUNT, ARCMALLEXPORT.VAT_SALES, ARCMALLEXPORT.OTHERTAX, ARCMALLEXPORT.ADJUSTMENTS, ARCMALLEXPORT.POS_ADJ, "
            "ARCMALLEXPORT.NEG_ADJ, ARCMALLEXPORT.POS_ADJ_NONTAX, ARCMALLEXPORT.NEG_ADJ_NONTAX, ARCMALLEXPORT.DAILY_SALES, ARCMALLEXPORT.VOID_AMOUNT, "
            "ARCMALLEXPORT.REFUND_AMOUNT, ARCMALLEXPORT.TAX_INCLUSIVE_SALES, ARCMALLEXPORT.VATEXEMPT_SALES, ARCMALLEXPORT.CHARGECRED_SALES, ARCMALLEXPORT.CHARGECRED_SALES_TAX, "
            "ARCMALLEXPORT.GIFT_CHEQUE_SALES, ARCMALLEXPORT.DEBIT_CARD_SALES, ARCMALLEXPORT.OTHER_TENDER_SALES, ARCMALLEXPORT.MASTERCARD_SALES, ARCMALLEXPORT.VISA_SALES, "
            "ARCMALLEXPORT.AMEX_SALES, ARCMALLEXPORT.DINERS_SALES, ARCMALLEXPORT.JCB_SALES, ARCMALLEXPORT.OTHER_CARD_SALES, ARCMALLEXPORT.SCHARGE_AMOUNT, "
            "ARCMALLEXPORT.OTHER_CHARGES, ARCMALLEXPORT.FIRST_TRANSACTION, ARCMALLEXPORT.LAST_TRANSACTION, ARCMALLEXPORT.TRANSACTION_COUNT, ARCMALLEXPORT.BEGINNING_INVOICE, "
            "ARCMALLEXPORT.ENDING_INVOICE, ARCMALLEXPORT.CASH_COUNT, ARCMALLEXPORT.GC_COUNT, ARCMALLEXPORT.DEBIT_COUNT, ARCMALLEXPORT.OTHER_TENDER_COUNT, "
            "ARCMALLEXPORT.MASTERCARD_COUNT, ARCMALLEXPORT.VISA_COUNT, ARCMALLEXPORT.AMEX_COUNT, ARCMALLEXPORT.DINERS_COUNT, ARCMALLEXPORT.JCB_COUNT, "
            "ARCMALLEXPORT.OTHERCARD_COUNT, ARCMALLEXPORT.TERMINAL_NAME, ARCMALLEXPORT.SERIALNUMBER, ARCMALLEXPORT.TRANSACTIONTIME, ARCMALLEXPORT.TRANSACTIONDATE, "
            "ARCMALLEXPORT.GROSS_SALES, ARCMALLEXPORT.VOID_COUNT, ARCMALLEXPORT.REGDISCOUNT_COUNT, ARCMALLEXPORT.REFUND_COUNT, ARCMALLEXPORT.SCDISCOUNT_COUNT, "
            "ARCMALLEXPORT.EODOLD, ARCMALLEXPORT.EODNEW, ARCMALLEXPORT.DATE_VALUE, ARCMALLEXPORT.NOVELTY_SALES, ARCMALLEXPORT.MISC_SALES, "
            "ARCMALLEXPORT.LOCALTAXES, ARCMALLEXPORT.PHARMA_SALES, ARCMALLEXPORT.NONPHARMA_SALES, ARCMALLEXPORT.SALESNOTSUBTORENT, ARCMALLEXPORT.REPTRANSACTION_AMOUNT, "
            "ARCMALLEXPORT.REPTRANSACTION_COUNT, ARCMALLEXPORT.CASH_SALES, ARCMALLEXPORT.HOUR_VALUE, ARCMALLEXPORT.TOTALDISCOUNT, ARCMALLEXPORT.BEGINNING_OR, "
            "ARCMALLEXPORT.ENDING_OR, ARCMALLEXPORT.LOCTAXEXEMPTDLY_SALES, ARCMALLEXPORT.FINEDINECUST_COUNT, ARCMALLEXPORT.TENDERSURCHARGES, ARCMALLEXPORT.NONVAT_SALES, "
            "ARCMALLEXPORT.CHECK_SALES, ARCMALLEXPORT.EPAY_SALES, ARCMALLEXPORT.NO_SALES, ARCMALLEXPORT.PREVEODCOUNTER, ARCMALLEXPORT.CURRENTEODCOUNTER, "
            "ARCMALLEXPORT.DISCOUNT_COUNT, ARCMALLEXPORT.CARD_SALES, ARCMALLEXPORT.OTHER_SALES, ARCMALLEXPORT.SALESTYPE_FOOD, ARCMALLEXPORT.SALESTYPE_NONFOOD, "
            "ARCMALLEXPORT.SALESTYPE_GROCERIES, ARCMALLEXPORT.SALESTYPE_MEDICINES, ARCMALLEXPORT.SALESTYPE_OTHERS,ARCMALLEXPORT.Z_KEY ) "
			"VALUES "
            "( "
            ":ARCMALLEXPORT_KEY, "
            ":MALLCODE, :TENANT_NAME, :CLASSCODE, :TRADECODE, :OUTLETNUMBER, "
            ":NEW_GRANDTOTAL, :OLD_GRANDTOTAL, :SALESTYPE, :DEPARTMENTSUM, :REGDISCOUNT_AMOUNT, "
            ":EMPDISCOUNT_AMOUNT, :SCDISCOUNT_AMOUNT, :VIPDISCOUNT_AMOUNT, :PWDDISCOUNT_AMOUNT, :GPCDISCOUNT_AMOUNT, "
            ":DISCOUNTG1_AMOUNT, :DISCOUNTG2_AMOUNT, :DISCOUNTG3_AMOUNT, :DISCOUNTG4_AMOUNT, :DISCOUNTG5_AMOUNT, "
            ":DISCOUNTG6_AMOUNT, :VAT_SALES, :OTHERTAX, :ADJUSTMENTS, :POS_ADJ, "
            ":NEG_ADJ, :POS_ADJ_NONTAX, :NEG_ADJ_NONTAX, :DAILY_SALES, :VOID_AMOUNT, "
            ":REFUND_AMOUNT, :TAX_INCLUSIVE_SALES, :VATEXEMPT_SALES, :CHARGECRED_SALES, :CHARGECRED_SALES_TAX, "
            ":GIFT_CHEQUE_SALES, :DEBIT_CARD_SALES, :OTHER_TENDER_SALES, :MASTERCARD_SALES, :VISA_SALES, "
            ":AMEX_SALES, :DINERS_SALES, :JCB_SALES, :OTHER_CARD_SALES, :SCHARGE_AMOUNT, "
            ":OTHER_CHARGES, :FIRST_TRANSACTION, :LAST_TRANSACTION, :TRANSACTION_COUNT, :BEGINNING_INVOICE, "
            ":ENDING_INVOICE, :CASH_COUNT, :GC_COUNT, :DEBIT_COUNT, :OTHER_TENDER_COUNT, "
            ":MASTERCARD_COUNT, :VISA_COUNT, :AMEX_COUNT, :DINERS_COUNT, :JCB_COUNT, "
            ":OTHERCARD_COUNT, :TERMINAL_NAME, :SERIALNUMBER, :TRANSACTIONTIME, :TRANSACTIONDATE, "
            ":GROSS_SALES, :VOID_COUNT, :REGDISCOUNT_COUNT, :REFUND_COUNT, :SCDISCOUNT_COUNT, "
            ":EODOLD, :EODNEW, :DATE_VALUE, :NOVELTY_SALES, :MISC_SALES, "
            ":LOCALTAXES, :PHARMA_SALES, :NONPHARMA_SALES, :SALESNOTSUBTORENT, :REPTRANSACTION_AMOUNT, "
            ":REPTRANSACTION_COUNT, :CASH_SALES, :HOUR_VALUE, :TOTALDISCOUNT, :BEGINNING_OR, "
            ":ENDING_OR, :LOCTAXEXEMPTDLY_SALES, :FINEDINECUST_COUNT, :TENDERSURCHARGES, :NONVAT_SALES, "
            ":CHECK_SALES, :EPAY_SALES, :NO_SALES, :PREVEODCOUNTER, :CURRENTEODCOUNTER, "
            ":DISCOUNT_COUNT, :CARD_SALES, :OTHER_SALES, :SALESTYPE_FOOD, :SALESTYPE_NONFOOD, "
            ":SALESTYPE_GROCERIES, :SALESTYPE_MEDICINES, :SALESTYPE_OTHERS, :Z_KEY);";

            IBArcMallExport->ParamByName("ARCMALLEXPORT_KEY")->AsString = ArcMallKey;
            IBArcMallExport->ParamByName("MALLCODE")->AsString = MALLCODE;
            IBArcMallExport->ParamByName("MALLCODE")->AsString = MALLCODE;
            IBArcMallExport->ParamByName("TENANT_NAME")->AsString = TENANT_NAME;
            IBArcMallExport->ParamByName("CLASSCODE")->AsString = CLASSCODE;
            IBArcMallExport->ParamByName("TRADECODE")->AsString = TRADECODE;
            IBArcMallExport->ParamByName("OUTLETNUMBER")->AsString = OUTLETNUMBER;
            IBArcMallExport->ParamByName("NEW_GRANDTOTAL")->AsCurrency = NEW_GRANDTOTAL;
            IBArcMallExport->ParamByName("OLD_GRANDTOTAL")->AsCurrency = OLD_GRANDTOTAL;
            IBArcMallExport->ParamByName("SALESTYPE")->AsString = SALESTYPE;
            IBArcMallExport->ParamByName("DEPARTMENTSUM")->AsCurrency = DEPARTMENTSUM;
            IBArcMallExport->ParamByName("REGDISCOUNT_AMOUNT")->AsCurrency = REGDISCOUNT_AMOUNT;
            IBArcMallExport->ParamByName("EMPDISCOUNT_AMOUNT")->AsCurrency = EMPDISCOUNT_AMOUNT;
            IBArcMallExport->ParamByName("SCDISCOUNT_AMOUNT")->AsCurrency = SCDISCOUNT_AMOUNT;
            IBArcMallExport->ParamByName("VIPDISCOUNT_AMOUNT")->AsCurrency = VIPDISCOUNT_AMOUNT;
            IBArcMallExport->ParamByName("PWDDISCOUNT_AMOUNT")->AsCurrency = PWDDISCOUNT_AMOUNT;
            IBArcMallExport->ParamByName("GPCDISCOUNT_AMOUNT")->AsCurrency = GPCDISCOUNT_AMOUNT;
            IBArcMallExport->ParamByName("DISCOUNTG1_AMOUNT")->AsCurrency = DISCOUNTG1_AMOUNT;
            IBArcMallExport->ParamByName("DISCOUNTG2_AMOUNT")->AsCurrency = DISCOUNTG2_AMOUNT;
            IBArcMallExport->ParamByName("DISCOUNTG3_AMOUNT")->AsCurrency = DISCOUNTG3_AMOUNT;
            IBArcMallExport->ParamByName("DISCOUNTG4_AMOUNT")->AsCurrency = DISCOUNTG4_AMOUNT;
            IBArcMallExport->ParamByName("DISCOUNTG5_AMOUNT")->AsCurrency = DISCOUNTG5_AMOUNT;
            IBArcMallExport->ParamByName("DISCOUNTG6_AMOUNT")->AsCurrency = DISCOUNTG6_AMOUNT;
            IBArcMallExport->ParamByName("VAT_SALES")->AsCurrency = VAT_SALES;
            IBArcMallExport->ParamByName("OTHERTAX")->AsCurrency = OTHERTAX;
            IBArcMallExport->ParamByName("ADJUSTMENTS")->AsCurrency = ADJUSTMENTS;
            IBArcMallExport->ParamByName("POS_ADJ")->AsCurrency = POS_ADJ;
            IBArcMallExport->ParamByName("NEG_ADJ")->AsCurrency = NEG_ADJ;
            IBArcMallExport->ParamByName("POS_ADJ_NONTAX")->AsCurrency = POS_ADJ_NONTAX;
            IBArcMallExport->ParamByName("NEG_ADJ_NONTAX")->AsCurrency = NEG_ADJ_NONTAX;
            IBArcMallExport->ParamByName("DAILY_SALES")->AsCurrency = DAILY_SALES;
            IBArcMallExport->ParamByName("VOID_AMOUNT")->AsCurrency = VOID_AMOUNT;
            IBArcMallExport->ParamByName("REFUND_AMOUNT")->AsCurrency = REFUND_AMOUNT;
            IBArcMallExport->ParamByName("TAX_INCLUSIVE_SALES")->AsCurrency = TAX_INCLUSIVE_SALES;
            IBArcMallExport->ParamByName("VATEXEMPT_SALES")->AsCurrency = VATEXEMPT_SALES;
            IBArcMallExport->ParamByName("CHARGECRED_SALES")->AsCurrency = CHARGECRED_SALES;
            IBArcMallExport->ParamByName("CHARGECRED_SALES_TAX")->AsCurrency = CHARGECRED_SALES_TAX;
            IBArcMallExport->ParamByName("GIFT_CHEQUE_SALES")->AsCurrency = GIFT_CHEQUE_SALES;
            IBArcMallExport->ParamByName("DEBIT_CARD_SALES")->AsCurrency = DEBIT_CARD_SALES;
            IBArcMallExport->ParamByName("OTHER_TENDER_SALES")->AsCurrency = OTHER_TENDER_SALES;
            IBArcMallExport->ParamByName("MASTERCARD_SALES")->AsCurrency = MASTERCARD_SALES;
            IBArcMallExport->ParamByName("VISA_SALES")->AsCurrency = VISA_SALES;
            IBArcMallExport->ParamByName("AMEX_SALES")->AsCurrency = AMEX_SALES;
            IBArcMallExport->ParamByName("DINERS_SALES")->AsCurrency = DINERS_SALES;
            IBArcMallExport->ParamByName("JCB_SALES")->AsCurrency = JCB_SALES;
            IBArcMallExport->ParamByName("OTHER_CARD_SALES")->AsCurrency = OTHER_CARD_SALES;
            IBArcMallExport->ParamByName("SCHARGE_AMOUNT")->AsCurrency = SCHARGE_AMOUNT;
            IBArcMallExport->ParamByName("OTHER_CHARGES")->AsCurrency = OTHER_CHARGES;
            IBArcMallExport->ParamByName("FIRST_TRANSACTION")->AsInteger = FIRST_TRANSACTION;
            IBArcMallExport->ParamByName("LAST_TRANSACTION")->AsInteger = LAST_TRANSACTION;
            IBArcMallExport->ParamByName("TRANSACTION_COUNT")->AsInteger = TRANSACTION_COUNT;
            IBArcMallExport->ParamByName("BEGINNING_INVOICE")->AsString = BEGINNING_INVOICE;
            IBArcMallExport->ParamByName("ENDING_INVOICE")->AsString = ENDING_INVOICE;
            IBArcMallExport->ParamByName("CASH_COUNT")->AsInteger = CASH_COUNT;
            IBArcMallExport->ParamByName("GC_COUNT")->AsInteger = GC_COUNT;
            IBArcMallExport->ParamByName("DEBIT_COUNT")->AsInteger = DEBIT_COUNT;
            IBArcMallExport->ParamByName("OTHER_TENDER_COUNT")->AsInteger = OTHER_TENDER_COUNT;
            IBArcMallExport->ParamByName("MASTERCARD_COUNT")->AsInteger = MASTERCARD_COUNT;
            IBArcMallExport->ParamByName("VISA_COUNT")->AsInteger = VISA_COUNT;
            IBArcMallExport->ParamByName("AMEX_COUNT")->AsInteger = AMEX_COUNT;
            IBArcMallExport->ParamByName("DINERS_COUNT")->AsInteger = DINERS_COUNT;
            IBArcMallExport->ParamByName("JCB_COUNT")->AsInteger = JCB_COUNT;
            IBArcMallExport->ParamByName("OTHERCARD_COUNT")->AsInteger = OTHERCARD_COUNT;
            IBArcMallExport->ParamByName("TERMINAL_NAME")->AsString = TERMINAL_NAME;
            IBArcMallExport->ParamByName("SERIALNUMBER")->AsString = SERIALNUMBER;
            IBArcMallExport->ParamByName("TRANSACTIONTIME")->AsDateTime = TRANSACTIONTIME;
            IBArcMallExport->ParamByName("TRANSACTIONDATE")->AsDateTime = TRANSACTIONDATE;
            IBArcMallExport->ParamByName("GROSS_SALES")->AsCurrency = GROSS_SALES;
            IBArcMallExport->ParamByName("VOID_COUNT")->AsInteger = VOID_COUNT;
            IBArcMallExport->ParamByName("REGDISCOUNT_COUNT")->AsInteger = REGDISCOUNT_COUNT;
            IBArcMallExport->ParamByName("REFUND_COUNT")->AsInteger = REFUND_COUNT;
            IBArcMallExport->ParamByName("SCDISCOUNT_COUNT")->AsInteger = SCDISCOUNT_COUNT;
            IBArcMallExport->ParamByName("EODOLD")->AsCurrency = EODOLD;
            IBArcMallExport->ParamByName("EODNEW")->AsCurrency = EODNEW;
            IBArcMallExport->ParamByName("DATE_VALUE")->AsDateTime = DATE_VALUE;
            IBArcMallExport->ParamByName("NOVELTY_SALES")->AsCurrency = NOVELTY_SALES;
            IBArcMallExport->ParamByName("MISC_SALES")->AsCurrency = MISC_SALES;
            IBArcMallExport->ParamByName("LOCALTAXES")->AsCurrency = LOCALTAXES;
            IBArcMallExport->ParamByName("PHARMA_SALES")->AsCurrency = PHARMA_SALES;
            IBArcMallExport->ParamByName("NONPHARMA_SALES")->AsCurrency = NONPHARMA_SALES;
            IBArcMallExport->ParamByName("SALESNOTSUBTORENT")->AsCurrency = SALESNOTSUBTORENT;
            IBArcMallExport->ParamByName("REPTRANSACTION_AMOUNT")->AsCurrency = REPTRANSACTION_AMOUNT;
            IBArcMallExport->ParamByName("REPTRANSACTION_COUNT")->AsInteger = REPTRANSACTION_COUNT;
            IBArcMallExport->ParamByName("CASH_SALES")->AsCurrency = CASH_SALES;
            IBArcMallExport->ParamByName("HOUR_VALUE")->AsString = HOUR_VALUE;
            IBArcMallExport->ParamByName("TOTALDISCOUNT")->AsCurrency = TOTALDISCOUNT;
            IBArcMallExport->ParamByName("BEGINNING_OR")->AsString = BEGINNING_OR;
            IBArcMallExport->ParamByName("ENDING_OR")->AsString = ENDING_OR;
            IBArcMallExport->ParamByName("LOCTAXEXEMPTDLY_SALES")->AsCurrency = LOCTAXEXEMPTDLY_SALES;
            IBArcMallExport->ParamByName("FINEDINECUST_COUNT")->AsInteger = FINEDINECUST_COUNT;
            IBArcMallExport->ParamByName("TENDERSURCHARGES")->AsCurrency = TENDERSURCHARGES;
            IBArcMallExport->ParamByName("NONVAT_SALES")->AsCurrency = NONVAT_SALES;
            IBArcMallExport->ParamByName("CHECK_SALES")->AsCurrency = CHECK_SALES;
            IBArcMallExport->ParamByName("EPAY_SALES")->AsCurrency = EPAY_SALES;
            IBArcMallExport->ParamByName("NO_SALES")->AsCurrency = NO_SALES;
            IBArcMallExport->ParamByName("PREVEODCOUNTER")->AsInteger = PREVEODCOUNTER;
            IBArcMallExport->ParamByName("CURRENTEODCOUNTER")->AsInteger = CURRENTEODCOUNTER;
            IBArcMallExport->ParamByName("DISCOUNT_COUNT")->AsInteger = DISCOUNT_COUNT;
            IBArcMallExport->ParamByName("CARD_SALES")->AsCurrency = CARD_SALES;
            IBArcMallExport->ParamByName("OTHER_SALES")->AsCurrency = OTHER_SALES;
            IBArcMallExport->ParamByName("SALESTYPE_FOOD")->AsCurrency = SALESTYPE_FOOD;
            IBArcMallExport->ParamByName("SALESTYPE_NONFOOD")->AsCurrency = SALESTYPE_NONFOOD;
            IBArcMallExport->ParamByName("SALESTYPE_GROCERIES")->AsCurrency = SALESTYPE_GROCERIES;
            IBArcMallExport->ParamByName("SALESTYPE_MEDICINES")->AsCurrency = SALESTYPE_MEDICINES;
            IBArcMallExport->ParamByName("SALESTYPE_OTHERS")->AsCurrency = SALESTYPE_OTHERS;
            IBArcMallExport->ParamByName("Z_KEY")->AsInteger = 0;
			IBArcMallExport->ExecQuery();

            // For ARCMALLEXPORTHOURLY
			IBMallExportHourly->Close();
			IBMallExportHourly->SQL->Text = "select * from MALLEXPORT_HOURLY";

			IBMallExportHourly->ExecQuery();
			while(!IBMallExportHourly->Eof)
			{
                IBInternalQuery1->Close();
				IBInternalQuery1->SQL->Text = "SELECT GEN_ID(GEN_ARCME_HOURLY_KEY, 1) FROM RDB$DATABASE";
				IBInternalQuery1->ExecQuery();
				int ArcMallHourKey = IBInternalQuery1->Fields[0]->AsInteger;

                TIBSQL *IBArcMallExportHourly = DBTransaction.Query(DBTransaction.AddQuery());
	    		IBArcMallExportHourly->SQL->Text = "INSERT INTO ARCMALLEXPORTHOURLY "
                "( "
                "ARCMALLEXPORTHOURLY.AME_HOURLY_KEY, "
                "ARCMALLEXPORTHOURLY.TERMINAL_NAME, ARCMALLEXPORTHOURLY.TENANT_NAME, ARCMALLEXPORTHOURLY.DATE_VALUE, "
                "ARCMALLEXPORTHOURLY.TIME_VALUE, ARCMALLEXPORTHOURLY.AMOUNT_VALUE, ARCMALLEXPORTHOURLY.TRANSACTION_COUNT, "
                "ARCMALLEXPORTHOURLY.VAT_SALES, ARCMALLEXPORTHOURLY.TOTALDISCOUNT, ARCMALLEXPORTHOURLY.SCHARGE_AMOUNT, "
                "ARCMALLEXPORTHOURLY.REFUND_COUNT, ARCMALLEXPORTHOURLY.REFUND_AMOUNT, ARCMALLEXPORTHOURLY.VOID_COUNT, "
                "ARCMALLEXPORTHOURLY.VOID_AMOUNT, ARCMALLEXPORTHOURLY.SCDISCOUNT_AMOUNT, ARCMALLEXPORTHOURLY.MALLCODE, "
                "ARCMALLEXPORTHOURLY.PATRON_COUNT, ARCMALLEXPORTHOURLY.MINUTE_VALUE, ARCMALLEXPORTHOURLY.SCDISCOUNT_COUNT,ARCMALLEXPORTHOURLY.Z_KEY,ARCMALLEXPORTHOURLY.GIFT_CARD,ARCMALLEXPORTHOURLY.CHECK_SALES ) "
		    	"VALUES "
                "( "
                ":AME_HOURLY_KEY, "
                ":TERMINAL_NAME, :TENANT_NAME, :DATE_VALUE, :TIME_VALUE, :AMOUNT_VALUE, :TRANSACTION_COUNT, "
                ":VAT_SALES, :TOTALDISCOUNT, :SCHARGE_AMOUNT, :REFUND_COUNT, :REFUND_AMOUNT, "
                ":VOID_COUNT, :VOID_AMOUNT, :SCDISCOUNT_AMOUNT, :MALLCODE, :PATRON_COUNT, "
                ":MINUTE_VALUE, :SCDISCOUNT_COUNT, :Z_KEY, :Gift_Card, :Check_Sales );";


				for (int i = 1; i < IBMallExportHourly->FieldCount; i++)
				{
                    if(i==3)
                    {
                        UnicodeString Date = IBMallExportHourly->FieldByName(IBMallExportHourly->Fields[i]->Name)->AsString;
                        UnicodeString Hour = IBMallExportHourly->FieldByName(IBMallExportHourly->Fields[i+1]->Name)->AsString;
                        UnicodeString Month = "";
                        UnicodeString Day = "";
                        UnicodeString Year = "";
                        std::string DateSTR = Date.t_str();
                        int Y,M,D,H;
                        for(int j=0; j<Date.Length();j++)
                        {
                            if(j>=0 && j<=3)
                            {
                                Year += DateSTR[j];
                            }
                            else if(j>=4 && j<=5)
                            {
                                Month += DateSTR[j];
                            }
                            else
                            {
                                Day += DateSTR[j];
                            }
                        }
                        Y = StrToInt(Year);
                        M = StrToInt(Month);
                        D = StrToInt(Day);
                        H = StrToInt(Hour);

                        TDateTime SetDate = EncodeDate(Y, M, D);
                        TDateTime SetTime = EncodeTime(H,0,0,0);
                        TDateTime FieldName = SetDate + SetTime;
                        IBArcMallExportHourly->ParamByName(IBMallExportHourly->Fields[i]->Name)->AsDateTime = FieldName;
                    }
                    else
                    {
                        UnicodeString FieldName = IBMallExportHourly->Fields[i]->Name;
                        VariantDBAssign(IBMallExportHourly->FieldByName(FieldName), IBArcMallExportHourly->ParamByName(FieldName));
                    }
				}
				IBArcMallExportHourly->ParamByName("AME_HOURLY_KEY")->AsInteger = ArcMallHourKey;
                IBArcMallExportHourly->ParamByName("Z_KEY")->AsInteger = 0;
                IBArcMallExportHourly->ExecQuery();
                IBMallExportHourly->Next();
            }

            // For ARCMALLEXPORTTRANSACTION
			IBMallExportTransaction->Close();
			IBMallExportTransaction->SQL->Text = "select * from MALLEXPORT_TRANSACTION";

			IBMallExportTransaction->ExecQuery();
			while(!IBMallExportTransaction->Eof)
			{
                IBInternalQuery2->Close();
				IBInternalQuery2->SQL->Text = "SELECT GEN_ID(GEN_ARCME_TRANSACTION_KEY, 1) FROM RDB$DATABASE";
				IBInternalQuery2->ExecQuery();
				int ArcMallTransactionKey = IBInternalQuery2->Fields[0]->AsInteger;

                TIBSQL *IBArcMallExportTransaction = DBTransaction.Query(DBTransaction.AddQuery());
	    		IBArcMallExportTransaction->SQL->Text = "INSERT INTO ARCMALLEXPORTTRANSACTION "
                "( "
                "ARCMALLEXPORTTRANSACTION.AME_TRANSACTION_KEY, "
                "ARCMALLEXPORTTRANSACTION.DATE_VALUE, ARCMALLEXPORTTRANSACTION.TENANT_NAME, ARCMALLEXPORTTRANSACTION.CURRENT_DATE_VALUE, ARCMALLEXPORTTRANSACTION.TIME_VALUE, ARCMALLEXPORTTRANSACTION.TERMINAL_NAME, "
                "ARCMALLEXPORTTRANSACTION.INVOICE_NUMBER, ARCMALLEXPORTTRANSACTION.GROSS_SALES, ARCMALLEXPORTTRANSACTION.VAT_SALES, ARCMALLEXPORTTRANSACTION.NONVAT_SALES, ARCMALLEXPORTTRANSACTION.VATEXEMPT_SALES, "
                "ARCMALLEXPORTTRANSACTION.SCDISCOUNT_AMOUNT, ARCMALLEXPORTTRANSACTION.EMPDISCOUNT_AMOUNT, ARCMALLEXPORTTRANSACTION.PRODISCOUNT_AMOUNT, ARCMALLEXPORTTRANSACTION.OTHERDISCOUNT_AMOUNT, ARCMALLEXPORTTRANSACTION.REFUND_AMOUNT, "
                "ARCMALLEXPORTTRANSACTION.SCHARGE_AMOUNT, ARCMALLEXPORTTRANSACTION.OTHER_SURCHARGE, ARCMALLEXPORTTRANSACTION.NET_SALES, ARCMALLEXPORTTRANSACTION.OLD_GRANDTOTAL, ARCMALLEXPORTTRANSACTION.NEW_GRANDTOTAL, "
                "ARCMALLEXPORTTRANSACTION.ZCOUNT, ARCMALLEXPORTTRANSACTION.CASH_SALES_AMOUNT, ARCMALLEXPORTTRANSACTION.CARD_SALES_AMOUNT, ARCMALLEXPORTTRANSACTION.CHECK_SALES_AMOUNT, ARCMALLEXPORTTRANSACTION.GC_SALES_AMOUNT, "
                "ARCMALLEXPORTTRANSACTION.EPAY_SALES_AMOUNT, ARCMALLEXPORTTRANSACTION.OTHER_SALES_AMOUNT, ARCMALLEXPORTTRANSACTION.VOID_AMOUNT, ARCMALLEXPORTTRANSACTION.DEPOSIT_AMOUNT, ARCMALLEXPORTTRANSACTION.APPLIED_DEPOSIT_AMOUNT, "
                "ARCMALLEXPORTTRANSACTION.VAT_SALES_COUNT, ARCMALLEXPORTTRANSACTION.NONVAT_SALES_COUNT, ARCMALLEXPORTTRANSACTION.VATEXEMPT_SALES_COUNT, ARCMALLEXPORTTRANSACTION.SCDISCOUNT_COUNT, ARCMALLEXPORTTRANSACTION.EMPDISCOUNT_COUNT, "
                "ARCMALLEXPORTTRANSACTION.PRODISCOUNT_COUNT, ARCMALLEXPORTTRANSACTION.OTHERDISCOUNT_COUNT, ARCMALLEXPORTTRANSACTION.REFUND_COUNT, ARCMALLEXPORTTRANSACTION.SCHARGE_COUNT, ARCMALLEXPORTTRANSACTION.OTHER_SURCHARGE_COUNT, "
                "ARCMALLEXPORTTRANSACTION.CASH_COUNT, ARCMALLEXPORTTRANSACTION.CARD_COUNT, ARCMALLEXPORTTRANSACTION.CHECK_COUNT, ARCMALLEXPORTTRANSACTION.GC_COUNT, ARCMALLEXPORTTRANSACTION.EPAY_COUNT, "
                "ARCMALLEXPORTTRANSACTION.OTHER_COUNT, ARCMALLEXPORTTRANSACTION.VOID_COUNT, ARCMALLEXPORTTRANSACTION.TRANS_VOID_COUNT, ARCMALLEXPORTTRANSACTION.DEPOSIT_COUNT, ARCMALLEXPORTTRANSACTION.APPLIED_DEPOSIT_COUNT, "
                "ARCMALLEXPORTTRANSACTION.PATRON_COUNT, ARCMALLEXPORTTRANSACTION.TRANSACTION_COUNT, ARCMALLEXPORTTRANSACTION.NOSALE_COUNT, ARCMALLEXPORTTRANSACTION.SALES_FLAG, ARCMALLEXPORTTRANSACTION.VAT_PERCENTAGE ) "
		    	"VALUES "
                "( "
                ":AME_TRANSACTION_KEY, "
                ":DATE_VALUE, :TENANT_NAME, :CURRENT_DATE_VALUE, :TIME_VALUE, :TERMINAL_NAME, "
                ":INVOICE_NUMBER, :GROSS_SALES, :VAT_SALES, :NONVAT_SALES, :VATEXEMPT_SALES, "
                ":SCDISCOUNT_AMOUNT, :EMPDISCOUNT_AMOUNT, :PRODISCOUNT_AMOUNT, :OTHERDISCOUNT_AMOUNT, :REFUND_AMOUNT, "
                ":SCHARGE_AMOUNT, :OTHER_SURCHARGE, :NET_SALES, :OLD_GRANDTOTAL, :NEW_GRANDTOTAL, "
                ":ZCOUNT, :CASH_SALES_AMOUNT, :CARD_SALES_AMOUNT, :CHECK_SALES_AMOUNT, :GC_SALES_AMOUNT, "
                ":EPAY_SALES_AMOUNT, :OTHER_SALES_AMOUNT, :VOID_AMOUNT, :DEPOSIT_AMOUNT, :APPLIED_DEPOSIT_AMOUNT, "
                ":VAT_SALES_COUNT, :NONVAT_SALES_COUNT, :VATEXEMPT_SALES_COUNT, :SCDISCOUNT_COUNT, :EMPDISCOUNT_COUNT, "
                ":PRODISCOUNT_COUNT, :OTHERDISCOUNT_COUNT, :REFUND_COUNT, :SCHARGE_COUNT, :OTHER_SURCHARGE_COUNT, "
                ":CASH_COUNT, :CARD_COUNT, :CHECK_COUNT, :GC_COUNT, :EPAY_COUNT, "
                ":OTHER_COUNT, :VOID_COUNT, :TRANS_VOID_COUNT, :DEPOSIT_COUNT, :APPLIED_DEPOSIT_COUNT, "
                ":PATRON_COUNT, :TRANSACTION_COUNT, :NOSALE_COUNT, :SALES_FLAG, :VAT_PERCENTAGE);";

				for (int i = 1; i < IBMallExportTransaction->FieldCount; i++)
				{
                    if(i==3)
                    {
                        UnicodeString Date = IBMallExportTransaction->FieldByName(IBMallExportTransaction->Fields[i]->Name)->AsString;
                        UnicodeString Time = IBMallExportTransaction->FieldByName(IBMallExportTransaction->Fields[i+1]->Name)->AsString;
                        UnicodeString Month = "";
                        UnicodeString Day = "";
                        UnicodeString Year = "";
                        UnicodeString Hour = "";
                        UnicodeString Mins = "";
                        UnicodeString Sec = "";
                        std::string DateSTR = Date.t_str();
                        std::string TimeSTR = Time.t_str();
                        int Y,M,D,H,Mi,S;

                        for(int j=0; j<Date.Length();j++)
                        {
                            if(j>=0 && j<=3)
                            {
                                Year += DateSTR[j];
                            }
                            else if(j>=5 && j<=6)
                            {
                                Month += DateSTR[j];
                            }
                            else if(j>=8 && j<=9)
                            {
                                Day += DateSTR[j];
                            }
                        }

                        for(int k=0; k<Time.Length();k++)
                        {
                            if(k>=0 && k<=1)
                            {
                                Hour += TimeSTR[k];
                            }
                            else if(k>=3 && k<=4)
                            {
                                Mins += TimeSTR[k];
                            }
                            else if(k>=6 && k<=7)
                            {
                                Sec += TimeSTR[k];
                            }
                        }

                        Y = StrToInt(Year);
                        M = StrToInt(Month);
                        D = StrToInt(Day);
                        H = StrToInt(Hour);
                        Mi = StrToInt(Mins);
                        S = StrToInt(Sec);

                        TDateTime SetDate = EncodeDate(Y, M, D);
                        TDateTime SetTime = EncodeTime(H,Mi,S,0);
                        TDateTime FieldName = SetDate + SetTime;
                        IBArcMallExportTransaction->ParamByName(IBMallExportTransaction->Fields[i]->Name)->AsDateTime = FieldName;
                    }
                    else
                    {
                        UnicodeString FieldName = IBMallExportTransaction->Fields[i]->Name;
                        VariantDBAssign(IBMallExportTransaction->FieldByName(FieldName), IBArcMallExportTransaction->ParamByName(FieldName));
                    }
				}
				IBArcMallExportTransaction->ParamByName("AME_TRANSACTION_KEY")->AsInteger = ArcMallTransactionKey;
                IBArcMallExportTransaction->ExecQuery();
                IBMallExportTransaction->Next();
            }

            // For ARCMALLEXPORTOTHERDETAILS
			IBMallExportOtherDetails->Close();
			IBMallExportOtherDetails->SQL->Text = "select * from MALLEXPORTOTHERDETAILS";

			IBMallExportOtherDetails->ExecQuery();
			while(!IBMallExportOtherDetails->Eof)
			{
                IBInternalQuery3->Close();
				IBInternalQuery3->SQL->Text = "SELECT GEN_ID(GEN_AMEOD_OD_KEY, 1) FROM RDB$DATABASE";
				IBInternalQuery3->ExecQuery();
				int ArcMallOtherKey = IBInternalQuery3->Fields[0]->AsInteger;

                TIBSQL *IBArcMallExportOtherDetails = DBTransaction.Query(DBTransaction.AddQuery());
	    		IBArcMallExportOtherDetails->SQL->Text = "INSERT INTO ARCMALLEXPORTOTHERDETAILS "
                "( "
                "ARCMALLEXPORTOTHERDETAILS.AMEOD_OD_KEY, "
                "ARCMALLEXPORTOTHERDETAILS.DATE_VALUE, ARCMALLEXPORTOTHERDETAILS.TENANT_NAME, ARCMALLEXPORTOTHERDETAILS.DISCOUNT_TYPE, "
                "ARCMALLEXPORTOTHERDETAILS.DISCOUNT_PERC, ARCMALLEXPORTOTHERDETAILS.DISCOUNT_AMOUNT, ARCMALLEXPORTOTHERDETAILS.TRANSACTION_CODE, "
                "ARCMALLEXPORTOTHERDETAILS.REFUND_CANCEL_REASON, ARCMALLEXPORTOTHERDETAILS.REFUND_CANCEL_AMOUNT, ARCMALLEXPORTOTHERDETAILS.REFUND_CANCEL_TABKEY, "
                "ARCMALLEXPORTOTHERDETAILS.TRANSACTION_COUNT, ARCMALLEXPORTOTHERDETAILS.FINEDINECUST_COUNT, ARCMALLEXPORTOTHERDETAILS.SCDISCOUNT_COUNT, "
                "ARCMALLEXPORTOTHERDETAILS.PAYMENT_CODE, ARCMALLEXPORTOTHERDETAILS.PAYMENT_CODE_DESC, ARCMALLEXPORTOTHERDETAILS.PAYMENT_CLASS, "
                "ARCMALLEXPORTOTHERDETAILS.PAYMENT_CLASS_DESC, ARCMALLEXPORTOTHERDETAILS.PAYMENT_AMOUNT ) "
		    	"VALUES "
                "( "
                ":AMEOD_OD_KEY, "
                ":DATE_VALUE, :TENANT_NAME, :DISCOUNT_TYPE, :DISCOUNT_PERC, :DISCOUNT_AMOUNT, :TRANSACTION_CODE, "
                ":REFUND_CANCEL_REASON, :REFUND_CANCEL_AMOUNT, :REFUND_CANCEL_TABKEY, :TRANSACTION_COUNT, :FINEDINECUST_COUNT, "
                ":SCDISCOUNT_COUNT, :PAYMENT_CODE, :PAYMENT_CODE_DESC, :PAYMENT_CLASS, :PAYMENT_CLASS_DESC, "
                ":PAYMENT_AMOUNT );";

                IBArcMallExportOtherDetails->ParamByName("AMEOD_OD_KEY")->AsInteger = ArcMallOtherKey;
                IBArcMallExportOtherDetails->ParamByName("DATE_VALUE")->AsDateTime = IBMallExportOtherDetails->FieldByName("DATE_VALUE")->AsDateTime;
                IBArcMallExportOtherDetails->ParamByName("TENANT_NAME")->AsString = IBMallExportOtherDetails->FieldByName("TENANT_NAME")->AsString;
                IBArcMallExportOtherDetails->ParamByName("DISCOUNT_TYPE")->AsString = IBMallExportOtherDetails->FieldByName("DISCOUNT_TYPE")->AsString;
                IBArcMallExportOtherDetails->ParamByName("DISCOUNT_PERC")->AsCurrency = IBMallExportOtherDetails->FieldByName("DISCOUNT_PERC")->AsCurrency;
                IBArcMallExportOtherDetails->ParamByName("DISCOUNT_AMOUNT")->AsCurrency = IBMallExportOtherDetails->FieldByName("DISCOUNT_AMOUNT")->AsCurrency;
                IBArcMallExportOtherDetails->ParamByName("TRANSACTION_CODE")->AsString = IBMallExportOtherDetails->FieldByName("TRANSACTION_CODE")->AsString;
                IBArcMallExportOtherDetails->ParamByName("REFUND_CANCEL_REASON")->AsString = IBMallExportOtherDetails->FieldByName("REFUND_CANCEL_REASON")->AsString;
                IBArcMallExportOtherDetails->ParamByName("REFUND_CANCEL_AMOUNT")->AsCurrency = IBMallExportOtherDetails->FieldByName("REFUND_CANCEL_AMOUNT")->AsCurrency;
                IBArcMallExportOtherDetails->ParamByName("REFUND_CANCEL_TABKEY")->AsInteger = IBMallExportOtherDetails->FieldByName("REFUND_CANCEL_TABKEY")->AsInteger;
                IBArcMallExportOtherDetails->ParamByName("TRANSACTION_COUNT")->AsInteger = IBMallExportOtherDetails->FieldByName("TRANSACTION_COUNT")->AsInteger;
                IBArcMallExportOtherDetails->ParamByName("FINEDINECUST_COUNT")->AsInteger = IBMallExportOtherDetails->FieldByName("FINEDINECUST_COUNT")->AsInteger;
                IBArcMallExportOtherDetails->ParamByName("SCDISCOUNT_COUNT")->AsInteger = IBMallExportOtherDetails->FieldByName("SCDISCOUNT_COUNT")->AsInteger;
                IBArcMallExportOtherDetails->ParamByName("PAYMENT_CODE")->AsString = IBMallExportOtherDetails->FieldByName("PAYMENT_CODE")->AsString;
                IBArcMallExportOtherDetails->ParamByName("PAYMENT_CODE_DESC")->AsString = IBMallExportOtherDetails->FieldByName("PAYMENT_CODE_DESC")->AsString;
                IBArcMallExportOtherDetails->ParamByName("PAYMENT_CLASS")->AsString = IBMallExportOtherDetails->FieldByName("PAYMENT_CLASS")->AsString;
                IBArcMallExportOtherDetails->ParamByName("PAYMENT_CLASS_DESC")->AsString = IBMallExportOtherDetails->FieldByName("PAYMENT_CLASS_DESC")->AsString;
                IBArcMallExportOtherDetails->ParamByName("PAYMENT_AMOUNT")->AsCurrency = IBMallExportOtherDetails->FieldByName("PAYMENT_AMOUNT")->AsCurrency;
                IBArcMallExportOtherDetails->ExecQuery();
                IBMallExportOtherDetails->Next();
            }
		}
    	catch(Exception & E)
	    {
    		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		    TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
	    	throw E;
    	}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
		throw E;
	}
}
//---------------------------------------------------------------------------
void TfrmAnalysis::MallExportReadFromDB(UnicodeString DataQuery, std::map<UnicodeString, UnicodeString> &DataRead)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
   try
   {
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
        query->Close();

        query->SQL->Text = DataQuery;
        query->ExecQuery();

        while(!query->Eof)
        {
            MallExportKey = query->FieldByName("MALLEXPORT_KEY")->AsInteger;
            FieldName = query->FieldByName("FIELD_NAME")->AsString;
            StringValue = query->FieldByName("STRING_VALUE")->AsString;
            IntegerValue = query->FieldByName("INTEGER_VALUE")->AsInteger;
            CurrencyValue = query->FieldByName("CURRENCY_VALUE")->AsCurrency;
            TimeStampValue = query->FieldByName("TIMESTAMP_VALUE")->AsDateTime;

            DataRead[FieldName] = MallExportCheckValue(StringValue, IntegerValue, CurrencyValue, TimeStampValue, query);
            query->Next();
        }
        DBTransaction.Commit();
    }
	catch(Exception &E)
	{
		DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------
UnicodeString TfrmAnalysis::MallExportCheckValue(UnicodeString StringValue, int IntegerValue,
                                                     Currency CurrencyValue, TDateTime TimeStampValue, TIBSQL* query)
{

    UnicodeString MapVal = "";
    UnicodeString USTimeStampValue = TimeStampValue.DateTimeString();

    if (!(query->FieldByName("STRING_VALUE")->IsNull))
    {
        MapVal = StringValue;
    }
    else if (!(query->FieldByName("INTEGER_VALUE")->IsNull))
    {
        MapVal = IntToStr(IntegerValue);
    }
    else if (!(query->FieldByName("CURRENCY_VALUE")->IsNull))
    {
        MapVal = CurrToStrF((RoundToNearest(CurrencyValue, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);
    }
    else if (!(query->FieldByName("TIMESTAMP_VALUE")->IsNull))
    {
        MapVal = USTimeStampValue;
    }
    else
    {
        MapVal = "0.00";
    }

    return MapVal;
}
//---------------------------------------------------------------------------
void TfrmAnalysis::DeleteStaffHours(Database::TDBTransaction &DBTransaction)
{
   try
	{
        CheckUserLoggedOut(DBTransaction);
        for(std::vector<int>::iterator it = GetContactKeys.begin(); it != GetContactKeys.end(); ++it)
        {
            TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
                "delete from  ROUNDEDCONTACTTIME where CONTACTS_KEY = :CONTACTS_KEY";
            IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = reinterpret_cast<int &>(*it);
            IBInternalQuery->ExecQuery();
        }
	}
	catch(Exception &E)
	{
		throw;
	}
}
//---------------------------------------------------------------------------
void TfrmAnalysis::CheckUserLoggedOut(Database::TDBTransaction &DBTransaction)
{
   try
	{
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text ="select CONTACTS_KEY  from CONTACTTIME where  LOGOUT_DATETIME > LOGIN_DATETIME ";

        IBInternalQuery->ExecQuery();

        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           GetContactKeys.push_back(IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger);
        }
    }
	catch(Exception &E)
	{
		throw;
	}
}
//---------------------------------------------------------------------------
void TfrmAnalysis::UpdateZedStaffHoursEnable(Database::TDBTransaction &DBTransaction, int key)
{
   try
	{
        zedLogsList->Add("Before updating staff hour.");
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text ="UPDATE ZEDS SET STAFF_HOUR_ENABLE = :STAFF_HOUR_ENABLE WHERE Z_KEY = :Z_KEY ";

        if(TGlobalSettings::Instance().EnableStaffHours)
        {
          IBInternalQuery->ParamByName("STAFF_HOUR_ENABLE")->AsInteger = 1;
        }
        else
        {
          IBInternalQuery->ParamByName("STAFF_HOUR_ENABLE")->AsInteger = 0;
        }
        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = key;
        IBInternalQuery->ExecQuery();
    }
	catch(Exception &E)
	{
        zedLogsList->Add("exception in  updating staff hour.");
		throw;
	}
    zedLogsList->Add("After updating staff hour.");
}

TMemoryStream* TfrmAnalysis::FormattedZed(TMemoryStream *ZedToArchive)
{

		TMemoryStream* tempStream = new TMemoryStream;
		ZedToArchive->Position = 0;
		int prevStreamArrVal = 0;
		for (unsigned int i=0;i<ZedToArchive->Size;i++)
		{
			char c;
			ZedToArchive->Read(&c, 1);
			int StreamArrDecimalval= int(c);
		 if(StreamArrDecimalval > 0 && StreamArrDecimalval != 27 && StreamArrDecimalval != 8 && StreamArrDecimalval != 16 && StreamArrDecimalval != 24 && StreamArrDecimalval != 30 && StreamArrDecimalval < 127)
        	{


              		if(StreamArrDecimalval==33&&prevStreamArrVal==114)
				{
					tempStream->Position--;
				}
				else
				{
					tempStream->Write(&c, 1);

				}
				prevStreamArrVal =StreamArrDecimalval;


		}
       }
        return tempStream;
}
//------------------------------------------------------------------------------------------------------
void TfrmAnalysis::GetTabCreditReceivedRefunded(Database::TDBTransaction &DBTransaction,double &TabCreditReceived, double &TabRefundReceived,
                                 TDateTime startTime,TDateTime endTime)
{
    UnicodeString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
    {
        terminalNamePredicate = " AND a.TERMINAL_NAME = :TERMINAL_NAME ";
    }
    try
    {
       TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
       IBInternalQuery->SQL->Text = "SELECT "
                                                        "b.PAY_TYPE, "
                                                        "cast((CASE WHEN b.SUBTOTAL > 0 THEN  Sum(b.SUBTOTAL) else 0 END) as numeric(17, 4)) AS CreditRefund,  "
                                                        "cast((CASE WHEN b.SUBTOTAL < 0 THEN  Sum(b.SUBTOTAL) else 0 END) as numeric(17, 4)) AS CreditReceived, "
                                                        "c.GL_CODE "
                                                    "FROM DAYARCBILL a "
                                                         "Left join DAYARCBILLPAY b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                                                         "Left join PAYMENTTYPES c on b.PAY_TYPE = c.PAYMENT_NAME "
                                                    "WHERE "
                                                         " b.ARCBILL_KEY not in ( SELECT distinct a.ARCBILL_KEY FROM DAYARCBILLPAY a where a.CHARGED_TO_XERO = 'T' ) and "
                                                         " b.PAY_TYPE = 'Credit' AND "
                                                         " a.TIME_STAMP > :STARTTIME and  a.TIME_STAMP <= :ENDTIME " + terminalNamePredicate +
                                                    " GROUP BY  "
                                                        "b.PAY_TYPE,c.GL_CODE, b.SUBTOTAL ";

        IBInternalQuery->ParamByName("STARTTIME")->AsDateTime = startTime;
        IBInternalQuery->ParamByName("ENDTIME")->AsDateTime = endTime;
        if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
        {
            IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();
        }
        IBInternalQuery->ExecQuery();

           if(IBInternalQuery->RecordCount > 0)
           {
                for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
                {
                      if(IBInternalQuery->FieldByName("CreditRefund")->AsDouble != 0)
                     {
                        TabRefundReceived +=  IBInternalQuery->FieldByName("CreditRefund")->AsDouble;
                     }
                      if(IBInternalQuery->FieldByName("CreditReceived")->AsDouble != 0)
                     {
                        TabCreditReceived +=  IBInternalQuery->FieldByName("CreditReceived")->AsDouble;
                     }
                }
           }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
//------------------------------------------------------------------------------------------------------
void TfrmAnalysis::GetFloatAmounts(Database::TDBTransaction &DBTransaction,double &floatAmount,AnsiString terminalNamePredicate)
{
   try
   {
       TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
       IBInternalQuery->SQL->Text =   "SELECT "
                                            "a.z_Key, "
                                         //   "cast((CASE WHEN REFLOAT_SKIM.TRANSACTION_TYPE ='Initial' THEN refloat_skim.amount else 0 END) as numeric(17, 4)) AS Zed_InitialFloat, "
                                            "cast((CASE WHEN refloat_skim.amount > 0 and REFLOAT_SKIM.TRANSACTION_TYPE <> 'Initial' THEN refloat_skim.amount else 0 END) as numeric(17, 4)) AS Zed_Deposit, "
                                            "cast((CASE WHEN refloat_skim.amount < 0 THEN refloat_skim.amount else 0 END) as numeric(17, 4)) AS Zed_Withdraw, "
                                            "REFLOAT_SKIM.TRANSACTION_TYPE "
                                       "FROM ZEDS a "
                                       "left join refloat_skim on a.z_key = REFLOAT_SKIM.Z_KEY  "
                                       "WHERE  a.TIME_STAMP is null  and a.TERMINAL_NAME = :TERMINAL_NAME"  ;
        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount > 0)
        {
            for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
            {
    //                 if(IBInternalQuery->FieldByName("Zed_InitialFloat")->AsDouble != 0)
    //                 {
    //                    floatAmount += IBInternalQuery->FieldByName("Zed_InitialFloat")->AsDouble;
    //                 }
                  if(IBInternalQuery->FieldByName("Zed_Deposit")->AsDouble != 0)
                 {
                    floatAmount += IBInternalQuery->FieldByName("Zed_Deposit")->AsDouble;
                 }
                  if(IBInternalQuery->FieldByName("Zed_Withdraw")->AsDouble != 0)
                 {
                    floatAmount += IBInternalQuery->FieldByName("Zed_Withdraw")->AsDouble;
                 }
            }
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

}

AnsiString TfrmAnalysis::GetCashGlCode(Database::TDBTransaction &DBTransaction)
{
    try
    {
       AnsiString CashGlcode;

        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT a.GL_CODE FROM PAYMENTTYPES a where a.PAYMENT_NAME = :PAYMENT_NAME ";
        IBInternalQuery->ParamByName("PAYMENT_NAME")->AsString = "Cash";

        IBInternalQuery->ExecQuery();

       if(IBInternalQuery->RecordCount > 0)
       {
           CashGlcode = IBInternalQuery->FieldByName("GL_CODE")->AsString;
          if((CashGlcode == "" || CashGlcode == NULL) && TGlobalSettings::Instance().IsXeroEnabled)
           {
             CashGlcode="200";

           }

           return CashGlcode;
       }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

TDateTime TfrmAnalysis::GetMinDayArchiveTime(Database::TDBTransaction &DBTransaction, TDateTime PrevZedTime)
{
    TDateTime _dayArchivetime;
    try
    {
        if(PrevZedTime.Val == 0)
        {
            TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = " SELECT MIN(TIME_STAMP)TIME_STAMP FROM DAYARCHIVE WHERE TERMINAL_NAME = :TERMINAL_NAME ";
            IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();
            IBInternalQuery->ExecQuery();
            if (IBInternalQuery->RecordCount)
            {
                _dayArchivetime = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
            }
            if(_dayArchivetime.Val == 0)
            {
               _dayArchivetime = TDateTime::CurrentDateTime();
            }
        }
        else
        {
           _dayArchivetime = PrevZedTime;
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
	return _dayArchivetime;
}
//-----------------------------------------------------------------------------double TfrmAnalysis::GetTipAmount(Database::TDBTransaction &DBTransaction,TDateTime startTime,UnicodeString &tipGLCode){
    double amount = 0.0;
    UnicodeString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
    {
        terminalNamePredicate = " AND b.TERMINAL_NAME = :TERMINAL_NAME ";  // add terminal filter for xero invoice.
    }
    try
    {
        TPayment payment;
        payment.SetPaymentAttribute(ePayTypeCustomSurcharge);
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = " SELECT c.PROPERTIES,SUM(a.SUBTOTAL) as SUBTOTAL,c.GL_CODE FROM DAYARCSURCHARGE a LEFT JOIN "
                                     " DAYARCBILL b on a.ARCBILL_KEY = b.ARCBILL_KEY "
                                     " LEFT JOIN PAYMENTTYPES c on c.PAYMENT_NAME = a.PAY_TYPE "
                                     " WHERE b.TIME_STAMP > :STARTTIME AND b.TIME_STAMP <= :ENDTIME AND "
                                     " c.PROPERTIES = :PROPERTIES "
                                     + terminalNamePredicate +
                                     " GROUP BY 1,3 ";
        IBInternalQuery->ParamByName("STARTTIME")->AsDateTime = startTime;
        IBInternalQuery->ParamByName("ENDTIME")->AsDateTime = Now();
        IBInternalQuery->ParamByName("PROPERTIES")->AsString = payment.GetPropertyString();
        if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
        {
            IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();  // add terminal param..
        }
        IBInternalQuery->ExecQuery();
        if (IBInternalQuery->RecordCount)
        {
            amount += RoundTo(IBInternalQuery->FieldByName("SUBTOTAL")->AsFloat, -4);
        }
//        tipGLCode = GetGLCodeTip(DBTransaction);
        tipGLCode = IBInternalQuery->FieldByName("GL_CODE")->AsString;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    return amount;
}
//-----------------------------------------------------------------------------
TDateTime TfrmAnalysis::GetMaxDayArchiveTime(Database::TDBTransaction &DBTransaction)
{
    TDateTime _dayArchivetime;
    UnicodeString terminalNamePredicate = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
    {
        terminalNamePredicate = " WHERE TERMINAL_NAME = :TERMINAL_NAME ";  // add terminal filter for xero invoice.
    }
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = " SELECT MAX(TIME_STAMP) TIME_STAMP FROM DAYARCBILL " + terminalNamePredicate;
        if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
        {
            IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();  // add terminal param..
        }
        IBInternalQuery->ExecQuery();
        if (IBInternalQuery->RecordCount)
        {
            _dayArchivetime = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
        }
        if(_dayArchivetime.Val == 0)
        {
           _dayArchivetime = TDateTime::CurrentDateTime();
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
	return _dayArchivetime;
}
//-------------------------------------------------------------------------------

void TfrmAnalysis::SyncCompanyDetails()
{
   try
   {
        if (TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
            TManagerCloudSync ManagerCloudSync;
            ManagerCloudSync.SyncCompanyDetails();
        }
   }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }

}

void TfrmAnalysis::UpdateSalesForce()
{
  zedLogsList->Add("Before updating salesforce ");
   Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
   DBTransaction1.StartTransaction();
   try
   {
        AnsiString CompanyName = GetCompanyName(DBTransaction1);
        DBTransaction1.Commit();
        std::auto_ptr<TSalesForceCommAtZed> sfComm(new TSalesForceCommAtZed());
        sfComm->PVCommunication(CompanyName);
        sfComm->SalesForceCommunication(CompanyName);
    }
    catch(Exception & E)
    {   zedLogsList->Add("Exception in updating salesforce ");
        DBTransaction1.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
    zedLogsList->Add("After updating salesforce ");
}

void TfrmAnalysis::ClearParkedSale(Database::TDBTransaction &DBTransaction)
{
   try
   {
        if ((frmSelectDish->ParkedSales->GetCount(DBTransaction) > 0) &&
            (MessageBox("There are currently parked sales. Do you wish to remove them?", "Clear all parked sales.", MB_YESNO + MB_ICONQUESTION) == IDYES))
        {
            zedLogsList->Add("Clearing parked sale ");
            frmSelectDish->ClearAllParkedSales(DBTransaction);
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
}

void TfrmAnalysis::UpdateArchive(TIBSQL *IBInternalQuery, Database::TDBTransaction &DBTransaction, UnicodeString DeviceName, int zedKey)
{
   try
   {   zedLogsList->Add("Inside UpdateArchive() ");
        if(TGlobalSettings::Instance().EnableDepositBagNum)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text = "SELECT * FROM DEVICES WHERE PRODUCT = 'MenuMate';";
            IBInternalQuery->ExecQuery();
            for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
            {
                UpdateArchive(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get(), IBInternalQuery->FieldByName("DEVICE_NAME")->AsString, zedKey);
                DuplicateEntryInTable = true;
            }
            DuplicateEntryInTable = false;
        }
        else
        UpdateArchive(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get(), DeviceName, zedKey);
   }
    catch(Exception & E)
    {
        zedLogsList->Add("Exception in  UpdateArchive() 2");
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
        throw;
    }
    zedLogsList->Add("After successfull UpdateArchive() ");
}

void TfrmAnalysis::UpdateStock(bool UpdateingStock)
{
    Database::TDBTransaction DBStockTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBStockTransaction);
    DBStockTransaction.StartTransaction();
    try
    {
        if (UpdateStockAllowed(DBStockTransaction))
        {
            UpdateingStock = true;
            ProcessStock(DBStockTransaction);
            UpdateStockComplete(DBStockTransaction);
            UpdateingStock = false;
        }

        DBStockTransaction.Commit();
    }
    catch(Exception & E)
    {
        DBStockTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
}

void TfrmAnalysis::ResetPoints()
{
    Database::TDBTransaction DBTransactionResetPoints(TDeviceRealTerminal::Instance().DBControl);
    DBTransactionResetPoints.StartTransaction();
    try
    {
        TResetPoints check;
        TIBSQL *IBInternalQuery1 = DBTransactionResetPoints.Query(DBTransactionResetPoints.AddQuery());
        IBInternalQuery1->SQL->Text="select count(a.CONTACTS_KEY) from resetpoints a ";
        IBInternalQuery1->ExecQuery();
        ResetKey= IBInternalQuery1->FieldByName("COUNT")->AsInteger;
        IBInternalQuery1->Close();
        IBInternalQuery1->SQL->Text="select distinct a.CONTACTS_KEY from pointstransactions a ";
        IBInternalQuery1->ExecQuery();

        for (; !IBInternalQuery1->Eof; IBInternalQuery1->Next())
        {
            if(TGlobalSettings::Instance().PointPurchased)
            {
                if(TGlobalSettings::Instance().PointEarned)
                {
                    if(TGlobalSettings::Instance().PointRedeem)
                    {
                        check = All;

                    }
                    else
                    {
                        check =PurchaseEarn;
                    }
                }
                else
                {
                     if(TGlobalSettings::Instance().PointRedeem)
                    {
                        check = PurchaseRedeem;
                    }
                    else
                    {
                        check = Purchase;
                    }
                }
            }
            else
            {
                if(TGlobalSettings::Instance().PointEarned)
                {
                    if(TGlobalSettings::Instance().PointRedeem)
                    {
                        check = EarnRedeem;
                    }
                    else
                    {
                        check = Earn;
                    }
                }
                else
                {
                   if(TGlobalSettings::Instance().PointRedeem)
                    {
                        check = Redeem;

                    }

                }
            }
            if(TGlobalSettings::Instance().PointRedeem || TGlobalSettings::Instance().PointEarned || TGlobalSettings::Instance().PointPurchased)
            {
                ResetPoint(DBTransactionResetPoints, IBInternalQuery1->FieldByName("CONTACTS_KEY")->AsInteger,check);
            }
        }
        DBTransactionResetPoints.Commit();
    }
    catch(Exception &E)
    {
        DBTransactionResetPoints.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
}

void TfrmAnalysis::EmailZedReport(int z_key)
{
    try
    {
        bool SendEmailStatus = false;
        GetReportData(z_key);
        UnicodeString Dir = ExtractFilePath(Application->ExeName) + ZDIR;
        if (!DirectoryExists(Dir))
        {
            CreateDir(Dir);
        }
        UnicodeString filename = Dir + "\\" + "ZReport" +"-" + date_time.FormatString("yyyy-mm-dd - hh-mm-ss") +".txt";
        ZedToMail->SaveToFile(filename);
        ZedToMail->Clear();
        AnsiString emailIds = TGlobalSettings::Instance().SaveEmailId;
        UnicodeString emailSubject = CheckRegistered();
        SendEmailStatus = SendEmail::Send(filename, emailSubject, emailIds, "");
        if(SendEmailStatus)
        {
            UpdateEmailstatus(z_key);
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
}

void TfrmAnalysis::UpdateMallExportDetails()
{
    try
    {
        // For Mall Export
        if(TGlobalSettings::Instance().MallIndex != 0 && TGlobalSettings::Instance().MallIndex != 9 )
        {
           if(TGlobalSettings::Instance().MallIndex == 7)
           {
               int Zed_Key = UpdateZKeyInArcMallExportForMegaWorld();
               bool IsValueGreaterThanZero = TDbMegaWorldMall::CheckValueExistInOtherSalesField(Zed_Key);

               if(IsValueGreaterThanZero)
               {
                 bool Food_Type = false;
                 bool NonFood_Type = false;
                 bool Groceries_Type = false;
                 bool Medicines_Type = false;
                 bool Other_Type = false;
                 int Count = TDbMegaWorldMall::GetCountForSalesTypeValue(Zed_Key,Food_Type,NonFood_Type,Groceries_Type,Medicines_Type,Other_Type) ;
                 Currency Amount_Total = TDbMegaWorldMall::GetTotalSalesAmountFromArcMallExport(Zed_Key);
                 Currency Divisible_Amount = Amount_Total/Count;
                 TDbMegaWorldMall::UpdateSaleTypeValueInArcMallExport(Zed_Key,Divisible_Amount,Food_Type,NonFood_Type,Groceries_Type,Medicines_Type,Other_Type)  ;
               }
           }
           std::auto_ptr<TMallExportManager> MEM(new TMallExportManager());
           MEM->IMallManager->ZExport();
        }
        else
        {
            TGlobalSettings::Instance().ZCount += 1;
            SaveVariable(vmZCount, TGlobalSettings::Instance().ZCount);

        }
        TMallExportUpdateAdaptor exportUpdateAdaptor;
        TMallExportHourlyUpdate exportHourlyUpdate;
        TMallExportTransactionUpdate exportTransactionUpdate;
        TMallExportOtherDetailsUpdate exportOtherDetailsUpdate;
        exportUpdateAdaptor.ResetExportTables();
        exportHourlyUpdate.ResetHourlyExportTablesOnZed();
        exportTransactionUpdate.ResetTransactionExportTablesOnZed();
        exportOtherDetailsUpdate.ResetOtherDetailsExportTablesOnZed();

        zedLogsList->Add("updating .UpdateMallExportDetails");
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
        zedLogsList->Add("Exception in .UpdateMallExportDetails");
    }
}

void TfrmAnalysis::OpenCashDrawer()
{
    Database::TDBTransaction OpenCashDrawerTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(OpenCashDrawerTransaction);
    OpenCashDrawerTransaction.StartTransaction();
    try
    {
        if(TGlobalSettings::Instance().OpenCashDrawer)
        {
            std::auto_ptr <TManagerFloat> (FloatManager)(new TManagerFloat((TForm*)this));
            FloatManager->OpenCashDrawerAccordingToPrinter(OpenCashDrawerTransaction);
        }
        OpenCashDrawerTransaction.Commit();
    }
    catch(Exception & E)
    {
        OpenCashDrawerTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
}

void TfrmAnalysis::PostDataToXeroAndMyOB(std::vector<TXeroInvoiceDetail>  &XeroInvoiceDetails, std::vector<TMYOBInvoiceDetail>  &MYOBInvoiceDetails, bool CompleteZed)
{
    try
    {
        if(TGlobalSettings::Instance().PostZToAccountingSystem && CompleteZed && XeroInvoiceDetails.size() > 0 )
        {
             CreateXeroInvoiceAndSend(XeroInvoiceDetails);
        }
        else if(TGlobalSettings::Instance().PostZToAccountingSystem && CompleteZed && MYOBInvoiceDetails.size() > 0 )
        {
             CreateMYOBInvoiceAndSend(MYOBInvoiceDetails);
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
}

void TfrmAnalysis::UpdateDLFMall()
{
    try
    {
      if(TGlobalSettings::Instance().MallIndex == DLFMALL )
      {
            CompleteDLFMallExport();
            TGlobalSettings::Instance().DLFMallFileName ="";
            SaveCompValueinDBStrUnique(vmDLFMallFileName, TGlobalSettings::Instance().DLFMallFileName); // See Function Description
      }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
}

void TfrmAnalysis::UpdateContactTimeZedStatus(Database::TDBTransaction &DBTransaction)
{
   try
	{
        zedLogsList->Add("Before  UpdateContactTimeZedStatus");
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text ="UPDATE CONTACTTIME SET ZED_STATUS = 1 WHERE CONTACTTIME.LOGOUT_DATETIME is not null ";
        IBInternalQuery->ExecQuery();
    }
	catch(Exception &E)
	{
        zedLogsList->Add("Exception in UpdateContactTimeZedStatus");
		throw;
	}
    zedLogsList->Add("After  UpdateContactTimeZedStatus");
}

UnicodeString TfrmAnalysis::CheckRegistered()
{
    UnicodeString emailSubject = "Zed Report";
    bool Registered = false;
    UnicodeString pRegisteredName = "";
    TDeviceRealTerminal::Instance().Registered(&Registered,&pRegisteredName);
    if(Registered)
    {
        emailSubject = pRegisteredName;
    }

    return emailSubject;
}
///--------------------------------------------------------------------------------------------------
void TfrmAnalysis::UpdateZKeyForMallExportSales(bool isMasterTerminal, int fieldIndex)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    try
    {
        if(TGlobalSettings::Instance().mallInfo.MallId == 2)
            UpdateAccumulatedSales(DBTransaction);

        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT MAX(Z_KEY) Z_KEY FROM ZEDS";
        IBInternalQuery->ExecQuery();
        int ZedKey = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;

        int deviceKey = TDeviceRealTerminal::Instance().ID.ProfileKey;
        AnsiString terminalCondition = " AND DEVICE_KEY = :DEVICE_KEY ";

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "UPDATE MALLEXPORT_SALES SET MALLEXPORT_SALES.FIELD_VALUE = :Z_KEY WHERE MALLEXPORT_SALES.Z_KEY = :EXISTING_KEY "
                                        "AND  MALLEXPORT_SALES.FIELD_INDEX = :FIELD_INDEX ";

        if(!isMasterTerminal)
		{
			IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + terminalCondition ;
        }

        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
        IBInternalQuery->ParamByName("EXISTING_KEY")->AsInteger = 0;
        IBInternalQuery->ParamByName("FIELD_INDEX")->AsInteger = fieldIndex;

        if(!isMasterTerminal)
		{
            IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = deviceKey;
        }

        IBInternalQuery->ExecQuery();

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "UPDATE MALLEXPORT_SALES SET MALLEXPORT_SALES.Z_KEY = :Z_KEY WHERE MALLEXPORT_SALES.Z_KEY = :EXISTING_KEY ";

        if(!isMasterTerminal)
		{
			IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text + terminalCondition;
        }

        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
        IBInternalQuery->ParamByName("EXISTING_KEY")->AsInteger = 0;

        if(!isMasterTerminal)
		{
            IBInternalQuery->ParamByName("DEVICE_KEY")->AsInteger = deviceKey;
        }

        IBInternalQuery->ExecQuery();

		//For Mezzanine Sales if any.
        if(TGlobalSettings::Instance().mallInfo.MallId == 2)
        {   
            TIBSQL *IBMezzanineSalesQuery =   DBTransaction.Query(DBTransaction.AddQuery());
            IBMezzanineSalesQuery->Close();
            IBMezzanineSalesQuery->SQL->Text = "UPDATE MEZZANINE_SALES a SET A.Z_KEY = :Z_KEY WHERE A.Z_KEY = :EXISTING_KEY ";
            if(!isMasterTerminal)
            {
                IBMezzanineSalesQuery->SQL->Text = IBMezzanineSalesQuery->SQL->Text + "AND A.TERMINAL_NAME = :TERMINAL_NAME ";
                IBMezzanineSalesQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
            }
            IBMezzanineSalesQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
            IBMezzanineSalesQuery->ParamByName("EXISTING_KEY")->AsInteger = 0;
            IBMezzanineSalesQuery->ExecQuery();

        }

        DBTransaction.Commit();
    }
    catch(Exception & E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
}

double TfrmAnalysis::GetCashWithdrawal(Database::TDBTransaction &DBTransaction)
{
    double cashWithdrawal = 0;
    try
    {
        TIBSQL *IBWithdrawalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBWithdrawalQuery->Close();
        IBWithdrawalQuery->SQL->Text = "SELECT SUM(a.AMOUNT) as withdrawal FROM REFLOAT_SKIM a "
                                       "WHERE a.AMOUNT < 0 and a.TRANSACTION_TYPE = 'Withdrawal' and IS_FLOAT_WITHDRAWN_FROM_CASH = 'T' and "
                                       "a.TERMINAL_NAME = :TERMINAL_NAME and "
                                       "a.Z_KEY in (Select MAX(ZEDS.Z_KEY) FROM ZEDS where ZEDS.TIME_STAMP is null) ";
        IBWithdrawalQuery->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();
        IBWithdrawalQuery->ExecQuery();
        cashWithdrawal = IBWithdrawalQuery->FieldByName("withdrawal")->AsDouble;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    return cashWithdrawal;
}

void TfrmAnalysis::GetCategoryNameAndGLCode(Database::TDBTransaction &DBTransaction, std::map<AnsiString, AnsiString>&CollectCategoryName, TDateTime startTime, TDateTime endTime)
{
    try
    {
        AnsiString terminalNamePredicate = "";
        std::map<AnsiString, AnsiString>::iterator ii;
        if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
        {
            terminalNamePredicate = " and a.TERMINAL_NAME = :TERMINAL_NAME ";  // add terminal filter for xero invoice.
        }
        TIBSQL *IBInternalQueryCategory = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQueryCategory->Close();
        IBInternalQueryCategory->SQL->Text = "Select c.CATEGORY, c.GL_CODE from  DAYARCHIVE a "
                                       " left join ARCCATEGORIES c on a.CATEGORY_KEY = c.CATEGORY_KEY  "
                                       " where a.ARCBILL_KEY in (Select distinct a.ARCBILL_KEY from DAYARCBILL a left join DAYARCBILLPAY b on a.ARCBILL_KEY = b.ARCBILL_KEY   "
                                       " where b.NOTE <> 'Total Change.' and a.TIME_STAMP > :STARTTIME and  a.TIME_STAMP <= :ENDTIME  " + terminalNamePredicate + " ) "
                                       " group by c.CATEGORY, c.GL_CODE ";

        if(!TGlobalSettings::Instance().EnableDepositBagNum) // check for master -slave terminal
        {
            IBInternalQueryCategory->ParamByName("TERMINAL_NAME")->AsString = GetTerminalName();
        }
        IBInternalQueryCategory->ParamByName("STARTTIME")->AsDateTime = startTime;
        IBInternalQueryCategory->ParamByName("ENDTIME")->AsDateTime = endTime;
        IBInternalQueryCategory->ExecQuery();
        for (; !IBInternalQueryCategory->Eof; )
        {
             AnsiString glCode = IBInternalQueryCategory->FieldByName("GL_CODE")->AsString;
             ii = CollectCategoryName.find(glCode);
             if(ii != CollectCategoryName.end())
             {
                 CollectCategoryName[glCode] = "Sales";
             }
             else
             {
                 CollectCategoryName[glCode] = IBInternalQueryCategory->FieldByName("CATEGORY")->AsString;
             }
             IBInternalQueryCategory->Next();
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
//-------------------------------------------------------------------------------------------------------
double TfrmAnalysis::GetCashBlindBalance(TBlindBalances Balances)
{
    double cashBlineBalance = 0.00;
    for (TBlindBalanceContainer::iterator itBlindBalances = Balances.begin(); (itBlindBalances != Balances.end()); itBlindBalances++)
	{
        if(itBlindBalances->first == "Cash")
            cashBlineBalance += (double)itBlindBalances->second.BlindBalance;
    }
    return cashBlineBalance;
}
//-------------------------------------------------------------------------------------------------------
double TfrmAnalysis::GetOldAccumulatedSales(Database::TDBTransaction &dbTransaction, int fieldIndex)
{
    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dbTransaction.RegisterQuery(IBInternalQuery);
    double oldAccumulatedSales = 0.00;
    int zKey = 0;
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT MAX(Z_KEY) Z_KEY FROM MALLEXPORT_SALES a WHERE a.MALL_KEY = :MALL_KEY  ";
        IBInternalQuery->ParamByName("MALL_KEY")->AsInteger = 2;

        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount )
           zKey = IBInternalQuery->Fields[0]->AsInteger;

        if(zKey)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
                                        "SELECT a.FIELD_INDEX, A.FIELD, CAST(A.FIELD_VALUE AS NUMERIC(17,4))FIELD_VALUE "
                                        "FROM MALLEXPORT_SALES a "
                                        "WHERE  a.MALLEXPORT_SALE_KEY = "
                                            "(SELECT MAX(A.MALLEXPORT_SALE_KEY) FROM MALLEXPORT_SALES a WHERE A.FIELD_INDEX  = :FIELD_INDEX "
                                                " AND a.Z_KEY = :Z_KEY ) ";
            IBInternalQuery->ParamByName("FIELD_INDEX")->AsString = fieldIndex;
            IBInternalQuery->ParamByName("Z_KEY")->AsInteger =  zKey;
            IBInternalQuery->ExecQuery();

            if(IBInternalQuery->RecordCount)
                oldAccumulatedSales = IBInternalQuery->Fields[2]->AsCurrency;
        }
        //dbTransaction.Commit();
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return oldAccumulatedSales;
}
//-------------------------------------------------------------------------------------------------
void TfrmAnalysis::UpdateAccumulatedSales(Database::TDBTransaction &dbTransaction)
{
    try
    {
        double oldAccumulatedSales = GetOldAccumulatedSales(dbTransaction, 5);
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(IBInternalQuery);

        Database::TcpIBSQL UpdateQuery(new TIBSQL(NULL));
        dbTransaction.RegisterQuery(UpdateQuery);

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT A.MALLEXPORT_SALE_KEY, A.FIELD_INDEX, CAST(A.FIELD_VALUE AS NUMERIC(17,2))FIELD_VALUE "
                                     " FROM MALLEXPORT_SALES A "
                                     " WHERE (A.FIELD_INDEX = :FIELD_INDEX1 OR  A.FIELD_INDEX = :FIELD_INDEX2) AND A.Z_KEY = :Z_KEY "
                                     "ORDER BY 1 ASC ";

        IBInternalQuery->ParamByName("FIELD_INDEX1")->AsInteger = 4;
        IBInternalQuery->ParamByName("FIELD_INDEX2")->AsInteger = 5;
        IBInternalQuery->ParamByName("Z_KEY")->AsInteger =  0;
        IBInternalQuery->ExecQuery();

        UpdateQuery->Close();
        UpdateQuery->SQL->Text = "UPDATE MALLEXPORT_SALES A SET A.FIELD_VALUE = :FIELD_VALUE "
                                     "WHERE (A.FIELD_INDEX = :FIELD_INDEX ) AND A.Z_KEY = :Z_KEY AND A.MALLEXPORT_SALE_KEY = :MALLEXPORT_SALE_KEY ";

        for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            int index = IBInternalQuery->FieldByName("FIELD_INDEX")->AsInteger;
            if(index == 5)
            {
                oldAccumulatedSales += IBInternalQuery->Fields[2]->AsDouble;
            }
            UpdateQuery->ParamByName("FIELD_INDEX")->AsInteger = index;
            UpdateQuery->ParamByName("FIELD_VALUE")->AsString =  oldAccumulatedSales;
            UpdateQuery->ParamByName("Z_KEY")->AsInteger =  0;
            UpdateQuery->ParamByName("MALLEXPORT_SALE_KEY")->AsInteger =  IBInternalQuery->Fields[0]->AsInteger;
            UpdateQuery->ExecQuery();
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
//-------------------------------------------------------------------------------------------------------
void TfrmAnalysis::MakeZEDLogFile(TStringList *List)
{
    AnsiString directoryName = ExtractFilePath(Application->ExeName) + "/ZED Logs";
    if (!DirectoryExists(directoryName))
        CreateDir(directoryName);
    AnsiString name = Now().CurrentDate().FormatString("DDMMYYYY")+ ".txt";
    AnsiString fileName =  directoryName + "/" + name;

    if (FileExists(fileName) )
    {
        std::auto_ptr <TStringList> OldZedLogs(new TStringList);
        OldZedLogs->LoadFromFile(fileName);
        OldZedLogs->Add("---------------------------------------------------------------------------------------------------------------" );
        OldZedLogs->AddStrings(List);
        OldZedLogs->SaveToFile(fileName );
    }
    else
    {
        List->SaveToFile(fileName );
    }
}

//-------------------------------------------------------------------------------
void __fastcall TfrmAnalysis::FiscalPrinterSettlement()
{
    UnicodeString zPrinterResponse;
    try
    {
        std::auto_ptr<TFiscalPrinterAdapter> fiscalAdapter(new TFiscalPrinterAdapter());
        fiscalAdapter->FiscalZReportSettlement();
    }
    catch(Exception & E)
    {
       zPrinterResponse = "Exception found in FiscalPrinterSettlement()";
	}
}
//-------------------------------------------------------------------------------
void TfrmAnalysis::UpdateStallCodeForEviaMall(int fieldindex)
{
    std::list<TMallExportSettings> ::iterator itUISettings;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    UnicodeString Stallcode = "";
    try
    {
        for(itUISettings = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); itUISettings != TGlobalSettings::Instance().mallInfo.MallSettings.end(); itUISettings++)
        {

                if(itUISettings->ControlName == "edMallTenantNo")
                    Stallcode = itUISettings->Value;
        }
        UnicodeString StallCodeFormat =  "\"" + Stallcode + "\""   ;
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "UPDATE MALLEXPORT_SALES SET MALLEXPORT_SALES.FIELD_VALUE = :Stallcode WHERE MALLEXPORT_SALES.FIELD_INDEX = :FIELD_INDEX and MALLEXPORT_SALES.FIELD_VALUE = :FIELD_VALUE ";

        IBInternalQuery->ParamByName("FIELD_INDEX")->AsInteger = 2;
        IBInternalQuery->ParamByName("Stallcode")->AsString = StallCodeFormat;
        IBInternalQuery->ParamByName("FIELD_VALUE")->AsString = "";

        IBInternalQuery->ExecQuery();
        IBInternalQuery->Close();

        DBTransaction.Commit();
    }

    catch(Exception & E)
    {
    DBTransaction.Rollback();
    TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
}
// ------------------------------------------------------------------------------
bool TfrmAnalysis::RestartFireBirdService()
{
    if(!TGlobalSettings::Instance().RestartServiceAtZED)
        return true;

    bool retValue = false;
    TMMProcessingState State(Screen->ActiveForm, "Please Wait...", "Please Wait");
    TDeviceRealTerminal::Instance().ProcessingController.Push(State);
    try
    {
        std::auto_ptr<TServicesManager> servicesManager(new TServicesManager());
        retValue = servicesManager->RestartService("FirebirdGuardianDefaultInstance");
    }
    catch(Exception &Ex)
    {
       TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Ex.Message);
    }
    TDeviceRealTerminal::Instance().ProcessingController.Pop();
    return retValue;
}
// ------------------------------------------------------------------------------
int TfrmAnalysis::UpdateZKeyInArcMallExportForMegaWorld()
{
    int ZedKey = 0;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT MAX(Z_KEY) Z_KEY FROM ZEDS ";
        IBInternalQuery->ExecQuery();
        ZedKey = IBInternalQuery->FieldByName("Z_KEY")->AsInteger;

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "UPDATE ARCMALLEXPORTHOURLY SET ARCMALLEXPORTHOURLY.Z_KEY = :Z_KEY "
                                     "WHERE ARCMALLEXPORTHOURLY.Z_KEY  =:Z_VALUE ";

        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
        IBInternalQuery->ParamByName("Z_VALUE")->AsInteger = 0;

        IBInternalQuery->ExecQuery();
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "UPDATE ARCMALLEXPORT SET ARCMALLEXPORT.Z_KEY = :Z_KEY "
                                     "WHERE ARCMALLEXPORT.Z_KEY  =:Z_VALUE ";

        IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
        IBInternalQuery->ParamByName("Z_VALUE")->AsInteger = 0;

        IBInternalQuery->ExecQuery();

        DBTransaction.Commit();

    }
    catch(Exception & E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }

    return ZedKey;


}
//-------------------------------------------------------------------------------
void TfrmAnalysis::SettleEFTPOSBills()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.INVOICE_NO, a.ORIGINAL_REFERENCE FROM EFTPOSREFRENECE a "
                                     "WHERE a.IS_SETTLED = :IS_SETTLED ";
        IBInternalQuery->ParamByName("IS_SETTLED")->AsString = "F";
        IBInternalQuery->ExecQuery();

        for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            UnicodeString invoiceNumber = IBInternalQuery->FieldByName("INVOICE_NO")->AsString;
            UnicodeString originalReference = IBInternalQuery->FieldByName("ORIGINAL_REFERENCE")->AsString;
            TDBAdyen::ProcessTipForSelectedRecord(DBTransaction, invoiceNumber, originalReference);
        }
        DBTransaction.Commit();
    }
    catch(Exception & E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
}





