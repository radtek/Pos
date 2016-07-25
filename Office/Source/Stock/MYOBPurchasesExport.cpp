//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>
#include <dateutils.hpp>
#include "MYOBPurchasesExport.h"
#include "StockData.h"
#include "Utilities.h"
#include "Connections.h"

#include "Reports.h"
#include "StockReportData.h"

#include "..\TinyXml\tinyxml.h"
#include <iostream.h>
#include "..\Helpers.h"

#include <Setup.h>
#include <sstream>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"

/*
* Used only for clarification into indices used later in the code.
* Likewise, __myob_countries is used as a range specifier in arrays
* found later in the code.
*/
enum myob_country {
	mc_australia,
	mc_new_zealand,
	/* New countries above this comment */
	__myob_countries
};

/*
* This enumeration serves to define the various fields that are
* exported to MYOB. They've been broken down into their own constants
* so that we can iterate through "template" arrays. These "template
* arrays" are nothing but an array of myob_export_entity. The elements
* of the array specify what will be output and in what order.
*
* Hopefully this'll make extending or tweaking the output easier than
* what was used previously.
*
* Make sure the last entry in any template array is
* "__myob_template_terminator."
*/
enum myob_export_entity {
	mee_co_last_name,
	mee_first_name,
	mee_addr_l1,
	mee_addr_l2,
	mee_addr_l3,
	mee_addr_l4,
	mee_inclusive,
	mee_purchase_no,
	mee_date,
	mee_supplier_invoice_id,
	mee_ship_via,
	mee_delivery_status,
	mee_description,
	mee_account_no,
	mee_amount,
	mee_inc_tax_amount,
	mee_job,
	mee_comment,
	mee_journal_memo,
	mee_shipping_date,
	mee_tax_code,
	mee_non_gst_amount,
	mee_gst_amount,
	mee_import_duty_amount,
	mee_freight_amount,
	mee_inc_tax_freight_amount,
	mee_freight_tax_code,
	mee_freight_non_gst_amount,
	mee_freight_gst_amount,
	mee_freight_import_duty_amount,
	mee_purchase_status,
	mee_currency_code,
	mee_exchange_rate,
	mee_terms_payment_is_due,
	mee_discount_days,
	mee_balance_due_days,
	mee_percent_discount,
	mee_amount_paid,
	mee_category,
	mee_card_id,
	mee_record_id,
	/*
	* Add new export-entity types above this comment
	*
	* __myob_export_entities is a sentinel; it keeps track of
	* how many entities there are.
	*
	* __myob_template_terminator is another sentinel; it marks
	* the end of a template.
	*/
	__myob_export_entities,
	__myob_template_terminator
};

/*
* What tax codes do we export when we're exporting in Australia?
* What about when we're in New Zealand? This table lets us determine
* what code to output and when easily.
*/
static const AnsiString myob_country_tax_codes[__myob_countries][2] = {
	// Inc.Tax  Excl.Tax
	{"GST",     "FRE"}, // Australia
	{"S",       "S"}    // New Zealand
};

/*
* This array defines the names of the fields exported to MYOB.
* It appears as the first line in the output file and it acts as a header.
*
* It must be kept in sync with the myob_export_entity enumeration as each
* element of that enumeration acts as an index into this array.
*/
static const AnsiString myob_export_headers[__myob_export_entities] = {
	"Co./Last Name",
	"First Name",
	"Addr 1 - Line 1",
	"- Line 2",
	"- Line 3",
	"- Line 4",
	"Inclusive",
	"Purchase #",
	"Date",
	"Supplier Invoice #",
	"Ship Via",
	"Delivery Status",
	"Description",
	"Account #",
	"Amount",
	"Inc-Tax Amount",
	"Job",
	"Comment",
	"Journal Memo",
	"Shipping Date",
	"Tax Code",
	"Non-GST Amount",
	"GST Amount",
	"Import Duty Amount",
	"Freight Amount",
	"Inc-Tax Freight Amount",
	"Freight Tax Code",
	"Freight Non-GST Amount",
	"Freight GST Amount",
	"Freight Import Duty Amount",
	"Purchase Status",
	"Currency Code",
	"Exchange Rate",
	"Terms - Payment is Due",
	" - Discount Days",
	" - Balance Due Days",
	"- % Discount",
	"Amount Paid",
	"Category",
	"Card ID",
	"Record ID"
};

/* Templates. If you create any new templates or modify the ones
* currently defined, make sure the first element is the country
* code - mc_australia or mc_new_zealand.
*
* Also, make sure that the last element is
* __myob_template_terminator.
*
* Naming is as follows:
*   myob_V_C_E where V = MYOB Version, C = Country, E = Edition.
*   E.g.
*     myob_v19_nz_se = MYOB Version 19, New Zealand, Standard.
*/
static const unsigned short myob_v19_au_pe[] = {
	mc_australia,
	mee_co_last_name,
	mee_first_name,
	mee_addr_l1,
	mee_addr_l2,
	mee_addr_l3,
	mee_addr_l4,
	mee_inclusive,
	mee_purchase_no,
	mee_date,
	mee_supplier_invoice_id,
	mee_ship_via,
	mee_delivery_status,
	mee_description,
	mee_account_no,
	mee_amount,
	mee_inc_tax_amount,
	mee_job,
	mee_comment,
	mee_journal_memo,
	mee_shipping_date,
	mee_tax_code,
	mee_non_gst_amount,
	mee_gst_amount,
	mee_import_duty_amount,
	mee_freight_amount,
	mee_inc_tax_freight_amount,
	mee_freight_tax_code,
	mee_freight_non_gst_amount,
	mee_freight_gst_amount,
	mee_freight_import_duty_amount,
	mee_purchase_status,
	mee_currency_code,
	mee_exchange_rate,
	mee_terms_payment_is_due,
	mee_discount_days,
	mee_balance_due_days,
	mee_percent_discount,
	mee_amount_paid,
	mee_category,
	mee_card_id,
	mee_record_id,
	__myob_template_terminator
};

static const unsigned short myob_v19_nz_pe[] = {
	mc_new_zealand,
	mee_co_last_name,
	mee_first_name,
	mee_addr_l1,
	mee_addr_l2,
	mee_addr_l3,
	mee_addr_l4,
	mee_inclusive,
	mee_purchase_no,
	mee_date,
	mee_supplier_invoice_id,
	mee_ship_via,
	mee_delivery_status,
	mee_description,
	mee_account_no,
	mee_amount,
	mee_inc_tax_amount,
	mee_job,
	mee_comment,
	mee_journal_memo,
	mee_shipping_date,
	mee_tax_code,
	mee_gst_amount,
	mee_import_duty_amount,
	mee_freight_amount,
	mee_inc_tax_freight_amount,
	mee_freight_tax_code,
	mee_freight_gst_amount,
	mee_freight_import_duty_amount,
	mee_purchase_status,
	mee_currency_code,
	mee_exchange_rate,
	mee_terms_payment_is_due,
	mee_discount_days,
	mee_balance_due_days,
	mee_percent_discount,
	mee_amount_paid,
	mee_category,
	mee_card_id,
	mee_record_id,
	__myob_template_terminator
};

static const unsigned short myob_v19_nz_se[] = {
	mc_new_zealand,
	mee_co_last_name,
	mee_first_name,
	mee_addr_l1,
	mee_addr_l2,
	mee_addr_l3,
	mee_addr_l4,
	mee_inclusive,
	mee_purchase_no,
	mee_date,
	mee_supplier_invoice_id,
	mee_ship_via,
	mee_delivery_status,
	mee_description,
	mee_account_no,
	mee_amount,
	mee_inc_tax_amount,
	mee_job,
	mee_comment,
	mee_journal_memo,
	mee_shipping_date,
	mee_tax_code,
	mee_gst_amount,
	mee_import_duty_amount,
	mee_freight_amount,
	mee_inc_tax_freight_amount,
	mee_freight_tax_code,
	mee_freight_gst_amount,
	mee_freight_import_duty_amount,
	mee_purchase_status,
	mee_terms_payment_is_due,
	mee_discount_days,
	mee_balance_due_days,
	mee_percent_discount,
	mee_amount_paid,
	mee_category,
	mee_card_id,
	mee_record_id,
	__myob_template_terminator
};

void
TfrmMYOBPurchasesExport::initialize_myob_export_lut()
{
	/*
	* The Australian "option" doesn't have any special edition
	* identifiers yet. We'll have to add them later and patch them in.
	*/
	myob_export_lut["AU"][""]         = myob_v19_au_pe;
	myob_export_lut["NZ"]["Standard"] = myob_v19_nz_se;
	myob_export_lut["NZ"]["Premier"]  = myob_v19_nz_pe;
}

const unsigned short *
TfrmMYOBPurchasesExport::template_from_myob_version_string(
const AnsiString &version_string)
{
	/* We're using istringstream here as it makes tokenizing easy! */
	std::string        country;
	std::istringstream strin(version_string.c_str());
	std::string        target;
	std::string        variant;

	/* Really must typedef this at some point ... */
	std::map<std::string,
	std::map<std::string,
	const unsigned short *> >::const_iterator i;
	std::map<std::string, const unsigned short *>::const_iterator j;

	strin >> target;
	strin >> country;
	strin >> variant;

	if (target != "MYOB"
			|| (i = myob_export_lut.find(country)) == myob_export_lut.end()
			|| (j = i->second.find(variant)) == i->second.end())
	return 0x0;

	return j->second;
}

void
TfrmMYOBPurchasesExport::write_myob_header_from_template(
TStringList &add_header_to,
const unsigned short *_template)
{
	AnsiString str;

	for (_template++; *_template != __myob_template_terminator; )
	str = str + myob_export_headers[*_template++] + "\t";

	add_header_to.Add((str[str.Length()] = '\n', str));
}

void
TfrmMYOBPurchasesExport::write_myob_invoice_from_template(
TStringList &add_invoice_line_to,
std::set<int> &exported_list,
const TInvoiceNodeData &invoice_data,
const unsigned short *template_beg)
{
	const unsigned short *template_pos;
	unsigned short country = *++template_beg;
	AnsiString     m;

	for (int i = 0, j = invoice_data.GLData.size(), k; i < j; i++) {
		k = invoice_data.GLData[i].GSTPercent == 0;
		template_pos = template_beg;

		for (m = ""; *template_pos != __myob_template_terminator;
		m = m + "\t") {
			switch (*template_pos++) {
			case mee_addr_l1:
			case mee_addr_l2:
			case mee_addr_l3:
			case mee_addr_l4:
			case mee_comment:
			case mee_first_name:
			case mee_freight_amount:
			case mee_freight_non_gst_amount:
			case mee_inclusive:
			case mee_inc_tax_freight_amount:
			case mee_non_gst_amount:
			case mee_purchase_no:
			case mee_shipping_date:
			case mee_amount_paid:
			case mee_category:
			case mee_card_id:
			case mee_record_id:
			case mee_ship_via:
				break;
			case mee_co_last_name:
				m = m + invoice_data.SupplierName;
				break;
			case mee_delivery_status:
				m = m + "P";
				break;
			case mee_date:
				m = m + invoice_data.Date.FormatString("dd/mm/yyyy");
				break;
			case mee_supplier_invoice_id:
				m = m + invoice_data.Reference;
				break;
			case mee_description:
				m = m + invoice_data.GLData[i].Category;
				break;
			case mee_account_no:
				m = m + invoice_data.GLData[i].GLCode;
				break;
			case mee_amount:
				m = m + invoice_data.GLData[i].TotalCost;
				break;
			case mee_inc_tax_amount:
				m = m + (invoice_data.GLData[i].TotalCost
				+ invoice_data.GLData[i].TotalGST);
				break;
			case mee_job:
				m = m + CurrentConnection.MYOBJobCode;
				break;
			case mee_journal_memo:
				m = m + "Purchases; " + invoice_data.SupplierName;
				break;
			case mee_tax_code:
				m = m + myob_country_tax_codes[country][k];
				break;
			case mee_gst_amount:
				m = m + (k ? 0.00 : invoice_data.GLData[i].TotalGST); 
				break;
			case mee_freight_tax_code:
				m = m + "GST";
				break;
			case mee_import_duty_amount:
			case mee_freight_gst_amount:
			case mee_freight_import_duty_amount:
				m = m + "$0.00";
				break;
			case mee_purchase_status:
				m = m + "B";
				break;
			case mee_terms_payment_is_due:
			case mee_discount_days:
			case mee_balance_due_days:
			case mee_percent_discount:
				m = m + "0";
				break;

			}
		}
		add_invoice_line_to.Add((m[m.Length()] = '\n', m));
		exported_list.insert(invoice_data.GLData[i].BatckKey);
	}
}

//---------------------------------------------------------------------------
__fastcall TfrmMYOBPurchasesExport::TfrmMYOBPurchasesExport(TComponent* Owner)
: TForm(Owner)
{
	initialize_myob_export_lut();

	StartTime = EncodeTime(5,0,0,0);
	EndTime = EncodeTime(5,0,0,0);
	mcStartDate->Date = int(Date()-1);
	mcEndDate->Date = int(mcStartDate->Date) + 1;
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::FormShow(TObject *Sender)
{
	pcReportProperties->ActivePage = tsStartTime;
}
//---------------------------------------------------------------------------
void TfrmMYOBPurchasesExport::ShowDateTimes()
{
	pnlStartTime->Caption	= StartTime.FormatString("HH:nn");
	pnlEndTime->Caption		= EndTime.FormatString("HH:nn");
	lbeFrom->Caption			= "From " + mcStartDate->Date.FormatString("ddddd") + " at " + StartTime.FormatString("HH:nn");
	lbeTo->Caption				= "To " + mcEndDate->Date.FormatString("ddddd") + " at " + EndTime.FormatString("HH:nn");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::mcStartDateClick(TObject *Sender)
{
	mcStartDate->MinDate = 0;
	mcEndDate->MinDate = 0;
	if (int(mcStartDate->Date) > int(mcEndDate->Date))
	{
		mcEndDate->Date = int(mcStartDate->Date);
	}
	mcEndDate->MinDate = int(mcStartDate->Date);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::csStartHourDownClick(
TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(StartTime, Hour, Min, Sec, MSec);

	if (short(--Hour) < 0) Hour = 23;

	StartTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::csStartHourUpClick(
TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(StartTime, Hour, Min, Sec, MSec);

	if (++Hour >= 24) Hour = 0;

	StartTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::csStartMinuteDownClick(
TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(StartTime, Hour, Min, Sec, MSec);

	if (short(--Min) < 0)
	{
		Min = 59;
		if (short(--Hour) < 0) Hour = 23;
	}
	StartTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::csStartMinuteUpClick(
TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(StartTime, Hour, Min, Sec, MSec);

	if (++Min >= 59)
	{
		Min = 0;
		if (++Hour >= 24) Hour = 0;
	}
	StartTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::csEndHourDownClick(
TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(EndTime, Hour, Min, Sec, MSec);

	if (short(--Hour) < 0) Hour = 23;

	EndTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::csEndHourUpClick(TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(EndTime, Hour, Min, Sec, MSec);

	if (++Hour >= 24) Hour = 0;

	EndTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::csEndMinuteDownClick(
TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(EndTime, Hour, Min, Sec, MSec);

	if (short(--Min) < 0)
	{
		Min = 59;
		if (short(--Hour) < 0) Hour = 23;
	}
	EndTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::csEndMinuteUpClick(
TObject *Sender)
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(EndTime, Hour, Min, Sec, MSec);

	if (++Min >= 59)
	{
		Min = 0;
		if (++Hour >= 24) Hour = 0;
	}
	EndTime = EncodeTime(Hour, Min, Sec, MSec);
	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::btnThisDayClick(TObject *Sender)
{
	mcEndDate->MinDate = int(mcStartDate->Date);
	mcEndDate->Date = int(mcStartDate->Date + 1);
	EndTime = StartTime;

	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::btnWeekClick(TObject *Sender)
{
	mcEndDate->MinDate = int(mcStartDate->Date);
	mcEndDate->Date = int(mcStartDate->Date + 7);
	EndTime = StartTime;

	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::btnThisMonthClick(TObject *Sender)
{
	unsigned short year, month, day;
	mcStartDate->Date.DecodeDate(&year, &month, &day);
	mcStartDate->Date		= int(EncodeDate(year, month, 1));
	mcEndDate->MinDate	= int(mcStartDate->Date);
	mcEndDate->Date		= int(IncMonth(mcStartDate->Date, 1));
	EndTime					= StartTime;

	ShowDateTimes();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::btnDateNextClick(TObject *Sender)
{
	ShowDateTimes();
	pcReportProperties->ActivePage = tsInvoices;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::tsInvoicesShow(TObject *Sender)
{
	TDateTime Start	= int(mcStartDate->Date) + StartTime;
	TDateTime End		= int(mcEndDate->Date) + EndTime;

	lbeFilterRange->Caption	= "From " + Start.FormatString("ddddd - HH:nn") + " to " + End.FormatString("ddddd - HH:nn");
	LoadTree();
}
//---------------------------------------------------------------------------
void TfrmMYOBPurchasesExport::LoadTree()
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	vtvInvoices->Clear();
	vtvInvoices->NodeDataSize = sizeof(TInvoiceNodeData);

	PVirtualNode		SupplierNode						= NULL;
	PVirtualNode		InvoiceNode; //							= NULL;
	TInvoiceNodeData	*NodeData; //							= NULL;

	AnsiString			LastSupplier;
	int					SupplierInvoiceCount				= 0;
	int					SupplierInvoiceCheckedCount	= 0;

	if (CurrentConnection.AccountSystem == "Xero") {
		
		qrInvoicesForXero->Close();

		qrInvoicesForXero->ParamByName("StartTime")->AsDateTime	= int(mcStartDate->Date) + StartTime;
		qrInvoicesForXero->ParamByName("EndTime")->AsDateTime		= int(mcEndDate->Date) + EndTime;
		qrInvoicesForXero->ExecQuery();

		while (!qrInvoicesForXero->Eof)
		{
			if (qrInvoicesForXero->FieldByName("Supplier_Name")->AsString != LastSupplier || !SupplierNode)
			{
				if (SupplierNode)
				{
					// Set the last suppliers checkbox appropriately
					if (SupplierInvoiceCount == 0 || SupplierInvoiceCheckedCount == 0)
					{
						SupplierNode->CheckState			= csUncheckedNormal;
					}
					else if (SupplierInvoiceCheckedCount < SupplierInvoiceCount)
					{
						SupplierNode->CheckState			= csMixedNormal;
					}
					else
					{
						SupplierNode->CheckState			= csCheckedNormal;
					}
				}
				SupplierNode									= vtvInvoices->AddChild(NULL);
				vtvInvoices->CheckType[SupplierNode]	= ctTriStateCheckBox;
				NodeData											= (TInvoiceNodeData *)vtvInvoices->GetNodeData(SupplierNode);
				NodeData->Text									= qrInvoicesForXero->FieldByName("Supplier_Name")->AsString;

				LastSupplier									= qrInvoicesForXero->FieldByName("Supplier_Name")->AsString;
				SupplierInvoiceCount							= 0;
				SupplierInvoiceCheckedCount				= 0;
			}
			InvoiceNode											= vtvInvoices->AddChild(SupplierNode);
			vtvInvoices->CheckType[InvoiceNode]			= ctTriStateCheckBox;
			InvoiceNode->CheckState							= (qrInvoicesForXero->FieldByName("Exported")->AsString == 'T' ||
			qrInvoicesForXero->FieldByName("Supplier_ID")->AsString == "")?
csUncheckedNormal:csCheckedNormal;
			NodeData												= (TInvoiceNodeData *)vtvInvoices->GetNodeData(InvoiceNode);

			NodeData->Text										= qrInvoicesForXero->FieldByName("Reference")->AsString;
			NodeData->Reference								= qrInvoicesForXero->FieldByName("Reference")->AsString;
			NodeData->Date										= qrInvoicesForXero->FieldByName("Created")->AsDateTime;
			NodeData->SupplierName							= qrInvoicesForXero->FieldByName("Supplier_Name")->AsString;
			NodeData->SupplierID								= qrInvoicesForXero->FieldByName("Supplier_ID")->AsString;
			NodeData->Export1									= qrInvoicesForXero->FieldByName("Export_1")->AsString;
			NodeData->Export2									= qrInvoicesForXero->FieldByName("Export_2")->AsString;
			NodeData->Export3									= qrInvoicesForXero->FieldByName("Export_3")->AsString;
			NodeData->TotalCost								= qrInvoicesForXero->FieldByName("Invoice_Total_Cost")->AsFloat;

			SupplierInvoiceCount++;
			if (InvoiceNode->CheckState == csCheckedNormal) SupplierInvoiceCheckedCount++;

			while (qrInvoicesForXero->FieldByName("Supplier_Name")->AsString == LastSupplier &&
			qrInvoicesForXero->FieldByName("Reference")->AsString == NodeData->Reference &&
			!qrInvoicesForXero->Eof)
			{
				TGLInvoiceData GLInvoiceData;
				GLInvoiceData.GLCode							= qrInvoicesForXero->FieldByName("GL_Code")->AsString;
				GLInvoiceData.Category							= qrInvoicesForXero->FieldByName("Stock_Category")->AsString;
				GLInvoiceData.Description                       = qrInvoicesForXero->FieldByName("Description")->AsString;
				GLInvoiceData.OrderQty                          = qrInvoicesForXero->FieldByName("Order_Qty")->AsFloat;
				GLInvoiceData.TotalCost							= qrInvoicesForXero->FieldByName("GL_Total_Cost")->AsFloat;
				GLInvoiceData.TotalGST							= qrInvoicesForXero->FieldByName("GL_Total_GST")->AsFloat;
				GLInvoiceData.GSTPercent						= qrInvoicesForXero->FieldByName("GST_Percent")->AsFloat;
				GLInvoiceData.BatckKey							= qrInvoicesForXero->FieldByName("Batch_Key")->AsInteger;
				GLInvoiceData.Date								= qrInvoicesForXero->FieldByName("Created")->AsDateTime;

				NodeData->GLData.push_back(GLInvoiceData);

				qrInvoicesForXero->Next();
			}
		}
		if (SupplierNode)
		{
			// Set the last suppliers checkbox appropriately
			if (SupplierInvoiceCount == 0 || SupplierInvoiceCheckedCount == 0)
			{
				SupplierNode->CheckState			= csUncheckedNormal;
			}
			else if (SupplierInvoiceCheckedCount < SupplierInvoiceCount)
			{
				SupplierNode->CheckState			= csMixedNormal;
			}
			else
			{
				SupplierNode->CheckState			= csCheckedNormal;
			}
		}
	}
	else {      // if (CurrentConnection.AccountSystem == "Xero")

		qrInvoices->Close();

		qrInvoices->ParamByName("StartTime")->AsDateTime	= int(mcStartDate->Date) + StartTime;
		qrInvoices->ParamByName("EndTime")->AsDateTime		= int(mcEndDate->Date) + EndTime;
		qrInvoices->ExecQuery();

		while (!qrInvoices->Eof)
		{
			if (qrInvoices->FieldByName("Supplier_Name")->AsString != LastSupplier || !SupplierNode)
			{
				if (SupplierNode)
				{
					// Set the last suppliers checkbox appropriately
					if (SupplierInvoiceCount == 0 || SupplierInvoiceCheckedCount == 0)
					{
						SupplierNode->CheckState			= csUncheckedNormal;
					}
					else if (SupplierInvoiceCheckedCount < SupplierInvoiceCount)
					{
						SupplierNode->CheckState			= csMixedNormal;
					}
					else
					{
						SupplierNode->CheckState			= csCheckedNormal;
					}
				}
				SupplierNode									= vtvInvoices->AddChild(NULL);
				vtvInvoices->CheckType[SupplierNode]	= ctTriStateCheckBox;
				NodeData											= (TInvoiceNodeData *)vtvInvoices->GetNodeData(SupplierNode);
				NodeData->Text									= qrInvoices->FieldByName("Supplier_Name")->AsString;

				LastSupplier									= qrInvoices->FieldByName("Supplier_Name")->AsString;
				SupplierInvoiceCount							= 0;
				SupplierInvoiceCheckedCount				= 0;
			}
			InvoiceNode											= vtvInvoices->AddChild(SupplierNode);
			vtvInvoices->CheckType[InvoiceNode]			= ctTriStateCheckBox;
			InvoiceNode->CheckState							= (qrInvoices->FieldByName("Exported")->AsString == 'T' ||
			qrInvoices->FieldByName("Supplier_ID")->AsString == "")?
csUncheckedNormal:csCheckedNormal;
			NodeData												= (TInvoiceNodeData *)vtvInvoices->GetNodeData(InvoiceNode);

			NodeData->Text										= qrInvoices->FieldByName("Reference")->AsString;
			NodeData->Reference								= qrInvoices->FieldByName("Reference")->AsString;
			NodeData->Date										= qrInvoices->FieldByName("Created")->AsDateTime;
			NodeData->SupplierName							= qrInvoices->FieldByName("Supplier_Name")->AsString;
			NodeData->SupplierID								= qrInvoices->FieldByName("Supplier_ID")->AsString;
			NodeData->Export1									= qrInvoices->FieldByName("Export_1")->AsString;
			NodeData->Export2									= qrInvoices->FieldByName("Export_2")->AsString;
			NodeData->Export3									= qrInvoices->FieldByName("Export_3")->AsString;
			NodeData->TotalCost								= qrInvoices->FieldByName("Invoice_Total_Cost")->AsFloat;

			SupplierInvoiceCount++;
			if (InvoiceNode->CheckState == csCheckedNormal) SupplierInvoiceCheckedCount++;

			while (qrInvoices->FieldByName("Supplier_Name")->AsString == LastSupplier &&
			qrInvoices->FieldByName("Reference")->AsString == NodeData->Reference &&
			!qrInvoices->Eof)
			{
				TGLInvoiceData GLInvoiceData;
				GLInvoiceData.GLCode							= qrInvoices->FieldByName("GL_Code")->AsString;
				GLInvoiceData.Category							= qrInvoices->FieldByName("Stock_Category")->AsString;
				//GLInvoiceData.Description                       = qrInvoices->FieldByName("Description")->AsString;
				//GLInvoiceData.OrderQty                          = qrInvoices->FieldByName("Order_Qty")->AsFloat;
				GLInvoiceData.TotalCost							= qrInvoices->FieldByName("GL_Total_Cost")->AsFloat;
				GLInvoiceData.TotalGST							= qrInvoices->FieldByName("GL_Total_GST")->AsFloat;
				GLInvoiceData.GSTPercent						= qrInvoices->FieldByName("GST_Percent")->AsFloat;
				GLInvoiceData.BatckKey							= qrInvoices->FieldByName("Batch_Key")->AsInteger;
				GLInvoiceData.Date								= qrInvoices->FieldByName("Created")->AsDateTime;

				NodeData->GLData.push_back(GLInvoiceData);

				qrInvoices->Next();
			}
		}
		if (SupplierNode)
		{
			// Set the last suppliers checkbox appropriately
			if (SupplierInvoiceCount == 0 || SupplierInvoiceCheckedCount == 0)
			{
				SupplierNode->CheckState			= csUncheckedNormal;
			}
			else if (SupplierInvoiceCheckedCount < SupplierInvoiceCount)
			{
				SupplierNode->CheckState			= csMixedNormal;
			}
			else
			{
				SupplierNode->CheckState			= csCheckedNormal;
			}
		}
	} // if (CurrentConnection.AccountSystem == "Xero")

	Transaction->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::vtvInvoicesGetText(
TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
TVSTTextType TextType, WideString &CellText)
{
	TInvoiceNodeData *NodeData = (TInvoiceNodeData *)Sender->GetNodeData(Node);
	if (NodeData)
	{
		if (Sender->GetNodeLevel(Node) == 0)
		{
			switch (Column)
			{
			case 0:	CellText = NodeData->Text;
				break;
			case 1:	CellText = "";
				break;
			case 2:	CellText = "";
				break;
			}

		}
		else
		{
			switch (Column)
			{
			case 0:	CellText = NodeData->Text;
				break;
			case 1:	CellText = NodeData->Date.FormatString("ddd, dd/mm/yyyy");
				break;
			case 2:	CellText = CurrToStrF(NodeData->TotalCost, ffFixed, CurrencyDecimals);
				break;
			}
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::vtvInvoicesGetImageIndex(
TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind,
TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
	if (Column == ((TVirtualStringTree *)Sender)->Header->MainColumn)
	{
		switch (Kind)
		{
		case ikState:
			{
				ImageIndex = -1;
				break;
			}
		case ikNormal:
			{
				if (Sender->GetNodeLevel(Node) == 0)
				{
					ImageIndex = ICON_CLOSED_FOLDER_INDEX;
				}
				else
				{
					ImageIndex = ICON_NOTEPAD_INDEX;
				}
				break;
			}
		case ikSelected:
			{
				if (Sender->GetNodeLevel(Node) == 0)
				{
					ImageIndex = ICON_OPEN_FOLDER_INDEX;
				}
				else
				{
					ImageIndex = ICON_NOTEPAD_INDEX;
				}
				break;
			}
		default:
			{
				ImageIndex = -1;
				break;
			}
		}
	}
	else
	{
		ImageIndex = -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMYOBPurchasesExport::btnFilterBackClick(
TObject *Sender)
{
	pcReportProperties->ActivePage = tsStartTime;
}

void __fastcall TfrmMYOBPurchasesExport::St(TObject *Sender) {
	
	if (CurrentConnection.AccountSystem == "" )
	{
		Application->MessageBox("You must select the account system from within Setup.", "Error", MB_ICONERROR + MB_OK);
	}
	else
	{

		std::auto_ptr<TStringList> ExportStrings(new TStringList());

		// setup local vars for Xero export
		// create xml document
		TiXmlDocument doc;

		// add root node
		TiXmlElement *rootNode = new TiXmlElement("Invoices");
		doc.LinkEndChild(rootNode);

		TCursor Save_Cursor = Screen->Cursor;
		Screen->Cursor = crHourGlass;
		try
		{
			std::set<int> ExportedList;
			int HeaderLineCount = 0;
			int MissingSupplierIDCount = 0;
			// Different Header Information for each MYOB export
			PVirtualNode Node = vtvInvoices->GetFirst();
			const unsigned short *myob_export_template;
			AnsiString acc_sys = CurrentConnection.AccountSystem;

			if ((myob_export_template =
						template_from_myob_version_string(acc_sys)))
			write_myob_header_from_template(
			*ExportStrings, myob_export_template);

			HeaderLineCount++;

			while (Node)
			{
				if (vtvInvoices->GetNodeLevel(Node) == 1)
				{
					if (Node->CheckState == csCheckedNormal)
					{
						TInvoiceNodeData *NodeData = (TInvoiceNodeData *)vtvInvoices->GetNodeData(Node);

						// Basically this means if no supplier code loaded for a supplier it wont be exported.
						if (NodeData->SupplierID != "")
						{
							if (myob_export_template) {
								write_myob_invoice_from_template(
								*ExportStrings,
								ExportedList,
								*NodeData,
								myob_export_template);

								if (NodeData->GLData.size() > 0)
								ExportStrings->Add("");
							}
							else if (CurrentConnection.AccountSystem == "CBA")
							{
								GetCBAInvoiceLines(NodeData, ExportStrings.get(), ExportedList);
							}
							else if (CurrentConnection.AccountSystem == "Greentree")
							{
								GetGreentreeInvoiceLines(NodeData, ExportStrings.get(), ExportedList);
							}
							else  if (CurrentConnection.AccountSystem == "Xero") {
								//
								// build a single invoice node for Xero
								//

								// add invoice node
								TiXmlElement *invoiceNode = new TiXmlElement("Invoice");
								rootNode->LinkEndChild(invoiceNode);

								// add invoice details

								// invoice type
								TiXmlElement *typeNode = new TiXmlElement("Type");
								TiXmlText *typeTextNode = new TiXmlText("ACCPAY");
								typeNode->LinkEndChild(typeTextNode);
								invoiceNode->LinkEndChild(typeNode);

								// contact info
								TiXmlElement *contactNode = new TiXmlElement("Contact");
								TiXmlElement *contactNameNode = new TiXmlElement("Name");
								TiXmlText *contactNameTextNode = new TiXmlText(NodeData->SupplierName.c_str());
								contactNameNode->LinkEndChild(contactNameTextNode);
								contactNode->LinkEndChild(contactNameNode);
								invoiceNode->LinkEndChild(contactNode);

								// invoice number
								TiXmlElement *invoiceNumberNode = new TiXmlElement("InvoiceNumber");
								TiXmlText *invoiceNumberTextNode = new TiXmlText(NodeData->Reference.c_str());
								invoiceNumberNode->LinkEndChild(invoiceNumberTextNode);
								invoiceNode->LinkEndChild(invoiceNumberNode);


								// date
								TiXmlElement *dateNode =
								new TiXmlElement("Date");
								TiXmlText *dateTextNode =
								new TiXmlText(NodeData->Date.FormatString("yyyy-mm-dd").c_str());
								dateNode->LinkEndChild(dateTextNode);
								invoiceNode->LinkEndChild(dateNode);

								// due date - leave blank for now and let Xero deal with it

								// invoice line item amounts stored excl gst
								TiXmlElement *lineAmountTypeNode = new TiXmlElement("LineAmountTypes");
								TiXmlText *lineAmountTypeTextNode = NULL;
								lineAmountTypeTextNode = new TiXmlText("Exclusive");
								lineAmountTypeNode->LinkEndChild(lineAmountTypeTextNode);
								invoiceNode->LinkEndChild(lineAmountTypeNode);

								// add line items node
								TiXmlElement *lineItemsNode = new TiXmlElement("LineItems");
								invoiceNode->LinkEndChild(lineItemsNode);

								// add line items
								for (unsigned int i=0; i < NodeData->GLData.size(); i++) {

									// add the line item node
									TiXmlElement *lineItemNode = new TiXmlElement("LineItem");
									lineItemsNode->LinkEndChild(lineItemNode);

									// description
									TiXmlElement *descNode = new TiXmlElement("Description");
									TiXmlText *descTextNode = new TiXmlText(NodeData->GLData[i].Description.c_str());
									descNode->LinkEndChild(descTextNode);
									lineItemNode->LinkEndChild(descNode);

									// qty
									// Hardcode qty to be "1.0" to force Xero to import
									TiXmlElement *qtyNode = new TiXmlElement("Quantity");
									TiXmlText *qtyTextNode = new TiXmlText("1.0");
									qtyNode->LinkEndChild(qtyTextNode);
									lineItemNode->LinkEndChild(qtyNode);

									// amount
									TiXmlElement *amtNode = new TiXmlElement("LineAmount");
									TiXmlText *amtTextNode = new TiXmlText(FormatFloat("0.00", NodeData->GLData[i].TotalCost).c_str());
									amtNode->LinkEndChild(amtTextNode);
									lineItemNode->LinkEndChild(amtNode);

									// account code/gl code
									if (NodeData->GLData[i].GLCode.Length() > 0) {
										TiXmlElement *accountCodeNode = new TiXmlElement("AccountCode");
										TiXmlText *accountCodeTextNode = new TiXmlText(NodeData->GLData[i].GLCode.c_str());
										accountCodeNode->LinkEndChild(accountCodeTextNode);
										lineItemNode->LinkEndChild(accountCodeNode);
									}

									ExportedList.insert(NodeData->GLData[i].BatckKey);
								}
							}
						}
						else
						{
							++MissingSupplierIDCount;
						}
					}
				}
				Node = vtvInvoices->GetNext(Node);
			}

			if (MissingSupplierIDCount > 0)
			{
				Application->MessageBox("One or more invoices are from suppliers without a supplier ID and were not exported.",
				"Warning", MB_ICONWARNING + MB_OK);
			}

			// do actual export
			if (CurrentConnection.AccountSystem == "Xero") {
				TiXmlPrinter printer;
				printer.SetStreamPrinting();
				doc.Accept(&printer);
				if (Helpers::SendInvoiceToXero(printer.CStr(), printer.Size() + sizeof(char))) {
					Application->MessageBox("Invoices sent to Xero OK", "Information", MB_OK);
					MarkAsExported(ExportedList);

					Node = vtvInvoices->GetFirst();
					while (Node)
					{
						Node->CheckState = csUncheckedNormal;
						Node = vtvInvoices->GetNext(Node);
					}
				}
				else {
					Application->MessageBox("Invoices NOT sent to Xero!", "Error", MB_OK | MB_ICONWARNING);
				}
			}
			else {
				if (ExportStrings->Count >= HeaderLineCount)
                {
                    //Show Reports for the exported invoices:
                    ShowInvoicesReport(ExportedList);

					SaveDialog->InitialDir = CurrentConnection.MYOBPath;

					if (SaveDialog->Execute())
					{
						ExportStrings->SaveToFile(SaveDialog->FileName);
						MarkAsExported(ExportedList);
					}
				}
			}
		}
		__finally
		{
			Screen->Cursor = Save_Cursor;
		}
	}
}

void TfrmMYOBPurchasesExport::ShowInvoicesReport(std::set<int> &ExportedList)
{
	dmStockReportData->StockTrans->StartTransaction();

	AnsiString InPredicateClause;
	try
	{
		for (std::set<int>::iterator iExportedList = ExportedList.begin(); iExportedList != ExportedList.end(); ++iExportedList)
		{
			if(InPredicateClause != "")
			InPredicateClause += ", ";

			InPredicateClause += *iExportedList;
		}
		dmStockReportData->SetupInvoices(InPredicateClause);
		if(frmReports->rvStock->SelectReport("repExportedInvoices", false))
		{
			frmReports->rvStock->Execute();
		}
	}
	__finally
	{
		dmStockReportData->StockTrans->Commit();
	}
}

void TfrmMYOBPurchasesExport::GetCBAInvoiceLines(TInvoiceNodeData *InvoiceData, TStringList *Lines, std::set<int> &ExportedList)
{
	const AnsiString Quote = "\"";
	const AnsiString Delimiter = ",";
	for (unsigned iGLData=0; iGLData<InvoiceData->GLData.size(); iGLData++)
	{
		Lines->Add(
		Quote + "APINV" + Quote + Delimiter +																			// Type
		Quote + "01" + Quote + Delimiter +																				// Branch
		Quote + Quote + Delimiter +																						// Warehouse
		Quote + InvoiceData->Reference + Quote + Delimiter +														// Invoice Number
		Quote + InvoiceData->Date.FormatString("dd/mm/yyyy") + Quote + Delimiter +							// Date
		Quote + InvoiceData->SupplierID + Quote + Delimiter +														// Supplier Code
		Quote + LastDayOfMonth(InvoiceData->Date).FormatString("dd/mm/yyyy") + Quote + Delimiter +	// Pay Date
		Quote + Quote + Delimiter +																						// Hold Code
		Quote + Quote + Delimiter +																						// Withholding tax
		Quote + "E" + Quote + Delimiter +																				// GST Indicator
		Quote + "G" + Quote + Delimiter +																				// S/G/F/J Indicator
		Quote + "01" + Quote + Delimiter +																				// Department
		Quote + InvoiceData->GLData[iGLData].GLCode + Quote + Delimiter +										// GL Account
		Quote + Quote + Delimiter +																						// CatIT/Subcode
		"1.00" + Delimiter +																									// Qty
		FormatFloat("0.0000", InvoiceData->GLData[iGLData].TotalCost) + Delimiter +						// Unit Cost exc.
		"0.00" + Delimiter +																									// Disc %
		"0.00" + Delimiter +																									// Disc Amount
		FormatFloat("0.00", InvoiceData->GLData[iGLData].GSTPercent) + Delimiter +							// Tax %
		FormatFloat("0.00", InvoiceData->GLData[iGLData].TotalGST) + Delimiter +							// Tax Amount
		FormatFloat("0.00", InvoiceData->GLData[iGLData].TotalCost) + Delimiter +							// Line Total exc.
		"0.00" + Delimiter +																									// Hold Amount
		Quote + InvoiceData->SupplierName + Quote + Delimiter +													// Supplier Name
		Quote + "N" + Quote);																								// Zero Tax Indicator
		/*			AnsiString("APINV") + "\t" +													// Type
			"\t" +																				// Branch
			"\t" +																				// Warehouse
			InvoiceData->Reference + "\t" +												// Invoice Number
			InvoiceData->Date.FormatString("dd/mm/yyyy") + "\t" +					// Date
			InvoiceData->SupplierID + "\t" +												// Supplier Code
			LastDayOfMonth(InvoiceData->Date) + "\t" +								// Pay Date
			"\t" +																				// Hold Code
			"\t" +																				// Withholding tax
			"N" + "\t" +																		// GST Indicator
			"G" + "\t" +																		// S/G/F/J Indicator
			"1" + "\t" +																		// Department
			InvoiceData->GLData[iGLData].GLCode + "\t" +								// GL Account
			"\t" +																				// CatIT/Subcode
			"1" + "\t" +																		// Qty
			FormatFloat("0.####", InvoiceData->GLData[iGLData].TotalCost +
			InvoiceData->GLData[iGLData].TotalGST) + "\t" +							// Unit Cost inc.
			"0" + "\t" +																		// Disc %
			"0" + "\t" +																		// Disc Amount
			"0" + "\t" +																		// Tax %
			"0" + "\t" +																		// Tax Amount
			FormatFloat("0.####", InvoiceData->GLData[iGLData].TotalCost +
			InvoiceData->GLData[iGLData].TotalGST) + "\t" +							// Line Total inc.
			"0" + "\t" +																		// Hold Amount
			InvoiceData->SupplierName); 													// Inclusive */
		ExportedList.insert(InvoiceData->GLData[iGLData].BatckKey);
	}
}

void TfrmMYOBPurchasesExport::GetGreentreeInvoiceLines(TInvoiceNodeData *InvoiceData, TStringList *Lines, std::set<int> &ExportedList)
{
	const AnsiString Quote = "\"";
	const AnsiString Delimiter = ",";

	for (unsigned iGLData=0; iGLData < InvoiceData->GLData.size(); iGLData++)
	{
		Lines->Add(
		"APINV" + Delimiter +																			// Type
		"01" + Delimiter +																				// Branch
		Delimiter +																						// Warehouse
		InvoiceData->Reference + Delimiter +														    // Invoice Number
		InvoiceData->Date.FormatString("dd/mm/yyyy") + Delimiter +							            // Date
		InvoiceData->SupplierID + Delimiter +														    // Supplier Code
		Delimiter +	                                                                                    // Pay Date
		Delimiter +																						// Hold Code
		Delimiter +																						// Withholding tax
		"N" + Delimiter +																				// GST Indicator
		"G" + Delimiter +																				// S/G/F/J Indicator
		"01" + Delimiter +																				// Department
		InvoiceData->GLData[iGLData].GLCode + Delimiter +										        // GL Code
		Delimiter +																						// CatIT/Subcode
		"1.00" + Delimiter +																							// Qty
		FormatFloat("0.00", InvoiceData->GLData[iGLData].TotalCost) + Delimiter +						                // Unit Cost exc.
		"0.00" + Delimiter +																									// Disc %
		"0.00" + Delimiter +																									// Disc Amount
		FormatFloat("0.00", InvoiceData->GLData[iGLData].GSTPercent) + Delimiter +							// Tax %
		FormatFloat("0.00", InvoiceData->GLData[iGLData].TotalGST) + Delimiter +							// Tax Amount
		FormatFloat("0.00", InvoiceData->GLData[iGLData].TotalCost)
         + Delimiter +							// Line Total exc.
		"0.00" + Delimiter +																									// Hold Amount
		InvoiceData->SupplierName);												// Supplier Name

		ExportedList.insert(InvoiceData->GLData[iGLData].BatckKey);
	}
}

//---------------------------------------------------------------------------
void TfrmMYOBPurchasesExport::MarkAsExported(std::set<int> &ExportedList)
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	for (std::set<int>::iterator iExportedList=ExportedList.begin(); iExportedList!=ExportedList.end(); ++iExportedList)
	{
		qrExported->Close();
		qrExported->ParamByName("BatchKey")->AsInteger = *iExportedList;
		qrExported->ExecQuery();
		qrExported->Close();
	}
	Transaction->Commit();
}
//---------------------------------------------------------------------------
TDateTime TfrmMYOBPurchasesExport::LastDayOfMonth(TDateTime DateTime)
{
	Word Year, Month, Day;
	DecodeDate(DateTime, Year, Month, Day);
	TDateTime NewDateTime = IncDay(IncMonth(EncodeDate(Year, Month, 1)), -1);
	if (NewDateTime <= DateTime)
	{
		NewDateTime = IncMonth(NewDateTime);
	}
	return NewDateTime;
}
//---------------------------------------------------------------------------
