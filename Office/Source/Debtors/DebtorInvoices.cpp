//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DebtorInvoices.h"
#include "Utilities.h"
#include "MMData.h"
#include "Connections.h"
#include "MMReportData.h"
#include "Reports.h"
#include <map>
#include <set>
#include <list>
#include <vector>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmDebtorInvoices *frmDebtorInvoices;
//---------------------------------------------------------------------------
__fastcall TfrmDebtorInvoices::TfrmDebtorInvoices(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorInvoices::FormShow(TObject *Sender)
{
	lbeReference->Caption = "";
	lbeInvoiceTotal->Caption = CurrencyString + CurrToStrF(0, ffFixed, CurrencyDecimals);
	lbePayments->Caption = CurrencyString + CurrToStrF(0, ffFixed, CurrencyDecimals);
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	PageControl->ActivePage = tsInvoice;
	LoadTree();
}
//---------------------------------------------------------------------------
class Payment
{
public:
	Payment(Currency total, AnsiString type, TDateTime time) : Total(total), Type(type), Time(time)
	{
	}

	Currency Total;
	AnsiString Type;
	TDateTime Time;
};
//---------------------------------------------------------------------------
class Invoice
{
public:
	typedef std::list< ::Payment> PaymentList;

	Invoice(int k, AnsiString n, Currency t, TDateTime dt) :
		Key(k), InvoiceNumber(n), Total(t), DateTime(dt), PaymentTotal(0) {}

	Invoice() {}

	int Key;
	AnsiString InvoiceNumber;
	Currency Total;
	Currency PaymentTotal;
	TDateTime DateTime;

	void AddPayment(const ::Payment& payment)
	{
		Payments.push_back(::Payment(payment));
	}
	const PaymentList& GetPayments() const
	{
		return Payments;
	}

	bool operator<(const Invoice& rhs) const
	{
		return InvoiceNumber < rhs.InvoiceNumber;
	}
	Invoice& operator = (const Invoice& rhs)
	{
		if (&rhs != this)
		{
			Key = rhs.Key;
			InvoiceNumber = rhs.InvoiceNumber;
			DateTime = rhs.DateTime;
			PaymentTotal = rhs.PaymentTotal;
			
			PaymentList::const_iterator i;
			for (i=rhs.Payments.begin(); i!=rhs.Payments.end(); ++i)
			{
				Payments.push_back(::Payment(*i));
			}
			Payments.sort();
		}
		return *this;
	}
private:
	PaymentList Payments;
};
//---------------------------------------------------------------------------
class InvoiceMonth
{
public:
	typedef std::list<Invoice> InvoiceList;

	InvoiceMonth(int y, int m) : Year(y), Month(m) {}
	InvoiceMonth() {}

	int Year;
	int Month;

	void AddInvoice(const Invoice& invoice)
	{
		Invoices.push_back(Invoice(invoice));
	}
	const InvoiceList& GetInvoices() const
	{
		return Invoices;
	}

	bool operator < (const InvoiceMonth& rhs) const
	{
		return Year < rhs.Year || (Year == rhs.Year && Month < rhs.Month);
	}
	bool operator == (const InvoiceMonth& rhs) const
	{
		return Year == rhs.Year && Month == rhs.Month;
	}

	InvoiceMonth& operator = (const InvoiceMonth& rhs)
	{
		if (&rhs != this)
		{
			Year = rhs.Year;
			Month = rhs.Month;
			InvoiceList::const_iterator i;
			for (i=rhs.Invoices.begin(); i!=rhs.Invoices.end(); ++i)
			{
				Invoices.push_back(Invoice(*i));
			}
			Invoices.sort();
		}
		return *this;
	}
private:
	InvoiceList Invoices;
};
//---------------------------------------------------------------------------
typedef std::list<InvoiceMonth> InvoiceMonthList;
typedef std::map<bool /*Closed*/, InvoiceMonthList> InvoiceList;
//---------------------------------------------------------------------------
void RenderInvoices(TTreeView* tvInvoices, AnsiString rootText, const InvoiceMonthList& invoiceMonthList)
{
	TTreeNode *rootNode = 0;
	InvoiceMonthList::const_iterator iMonthInvoices;
	for (iMonthInvoices=invoiceMonthList.begin(); iMonthInvoices!=invoiceMonthList.end(); ++iMonthInvoices)
	{
		if (rootNode == 0)
		{
			rootNode = tvInvoices->Items->Add(0, rootText);
			rootNode->ImageIndex = ICON_CLOSED_FOLDER_INDEX;
			rootNode->SelectedIndex = ICON_OPEN_FOLDER_INDEX;
			rootNode->Data = 0;
		}
		TTreeNode* monthNode = tvInvoices->Items->AddChild(rootNode,
			FormatDateTime("mmmm, yyyy", EncodeDate(iMonthInvoices->Year, iMonthInvoices->Month, 1)));

		InvoiceMonth::InvoiceList::const_iterator iInvoice;

		for (iInvoice=iMonthInvoices->GetInvoices().begin(); iInvoice!=iMonthInvoices->GetInvoices().end(); ++iInvoice)
		{
			TTreeNode* invoiceNode = tvInvoices->Items->AddChild(monthNode, iInvoice->InvoiceNumber);
			invoiceNode->ImageIndex = ICON_NOTEPAD_INDEX;
			invoiceNode->SelectedIndex = ICON_NOTEPAD_INDEX;
			invoiceNode->Data = (void *)new Invoice(*iInvoice);//(void *)(new AnsiString(iInvoice->InvoiceNumber));

			Invoice::PaymentList::const_iterator iPayment;

			if (iInvoice->GetPayments().size() > 0)
			{
				TTreeNode* paymentNode = tvInvoices->Items->AddChild(invoiceNode, "Paid: " +
					CurrencyString + CurrToStrF(iInvoice->PaymentTotal, ffFixed, CurrencyDecimals));

				paymentNode->ImageIndex = ICON_MONEY_INDEX;
				paymentNode->SelectedIndex = ICON_MONEY_INDEX;
				paymentNode->Data = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------
void TfrmDebtorInvoices::LoadTree()
{
	TCursor SaveCursor = Screen->Cursor;
	try
	{
		Screen->Cursor = crHourGlass;

		InvoiceList invoices;

		qrDebtorInvoices->SQL->Text =
			"Select "
				"Contacts.Contacts_Key,"
				"Contacts.Name,"
				"Invoices.Invoice_Number,"
				"Invoices.Invoice_Key,"
				"Invoices.Closed,"
				"Invoices.Total_Inc,"
				"Extract(Month from Invoices.Creation_Date) Invoice_Month,"
				"Extract(Year from Invoices.Creation_Date) Invoice_Year,"
				"Invoices.Creation_Date "
			"From "
				"Contacts Inner Join Invoices on "
				"Contacts.Contacts_Key = Invoices.Contacts_Key "
			"Where "
				"Contacts.Contacts_Key = :Contacts_Key "
			"Order By "
				"Invoices.Closed,"
				"Invoices.Creation_Date,"
				"Invoices.Invoice_Number ";

		qrDebtorInvoices->Close();
		qrDebtorInvoices->ParamByName("Contacts_Key")->AsInteger = DebtorKey;
		for (qrDebtorInvoices->Open(); !qrDebtorInvoices->Eof; qrDebtorInvoices->Next())
		{
			bool closed = qrDebtorInvoices->FieldByName("Closed")->AsString == "T";

			InvoiceMonthList& invoiceMonthList = invoices[closed];

			InvoiceMonth invoiceMonth(qrDebtorInvoices->FieldByName("Invoice_Year")->AsInteger,
				qrDebtorInvoices->FieldByName("Invoice_Month")->AsInteger);

			InvoiceMonthList::iterator iInvoiceMonth = std::find(invoiceMonthList.begin(), invoiceMonthList.end(), invoiceMonth);
			if (iInvoiceMonth == invoiceMonthList.end())
			{
				invoiceMonthList.push_front(invoiceMonth);
				iInvoiceMonth = invoiceMonthList.begin();
			}
			Invoice invoice(qrDebtorInvoices->FieldByName("Invoice_Key")->AsInteger,
				qrDebtorInvoices->FieldByName("Invoice_Number")->AsString,
				0,//qrDebtorInvoices->FieldByName("Total_Inc")->AsCurrency,
				qrDebtorInvoices->FieldByName("Creation_Date")->AsDateTime);

			Currency paymentTotal = 0;

			qrPayments->Close();
			qrPayments->ParamByName("InvoiceKey")->AsInteger = qrDebtorInvoices->FieldByName("Invoice_Key")->AsInteger;
			for (qrPayments->Open(); !qrPayments->Eof; qrPayments->Next())
			{
				::Payment payment(
					qrPayments->FieldByName("SubTotal")->AsCurrency,
					qrPayments->FieldByName("Pay_Type")->AsString,
					qrPayments->FieldByName("Time_Stamp")->AsDateTime
				);
				invoice.AddPayment(payment);

				paymentTotal += qrPayments->FieldByName("SubTotal")->AsCurrency;
			}
			invoice.PaymentTotal = paymentTotal;
			iInvoiceMonth->AddInvoice(invoice);
		}
		invoices[true].sort();
		invoices[false].sort();

		tvInvoices->Items->Clear();
		tvInvoices->Items->BeginUpdate();
		tvInvoices->OnChange = NULL;

		RenderInvoices(tvInvoices, "Unpaid Invoices", invoices[false]);
		RenderInvoices(tvInvoices, "Paid Invoices", invoices[true]);

		tvInvoices->Items->EndUpdate();

		lbeTitle->Caption = qrDebtorInvoices->FieldByName("Name")->AsString;
	}
	__finally
	{
		Screen->Cursor = SaveCursor;
		tvInvoices->OnChange = tvInvoicesChange;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorInvoices::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	if (Transaction->InTransaction) Transaction->Commit();
	FindDialog->CloseDialog();
}
//---------------------------------------------------------------------------
void TfrmDebtorInvoices::ResizeGrids()
{
	const ColCount2 = 4;
	TDBGrid *Grid = dbgDetails;
	Grid->TitleFont->Size = Grid->Font->Size;
	const PercArray2[ColCount2] = {12, 20, 55, 13};
	int WidthSoFar = 0;
	int PercLeft = 100;
	for (int i=0; i<Grid->Columns->Count; i++)
	{
		int ColWidth = (Grid->ClientWidth - WidthSoFar) * PercArray2[i] / PercLeft - 1;
		if (PercArray2[i] != 0)
		{
			Grid->Columns->Items[i]->Width = ColWidth;
			WidthSoFar += ColWidth + 1;
			PercLeft -= PercArray2[i];
		}
		if (i == Grid->Columns->Count-1)
		{
			Grid->Columns->Items[i]->Width += 1;
		}
	}
	// Payments Grid
	TDBGrid *PaymentsGrid = dbgPayments;
	PaymentsGrid->TitleFont->Size = PaymentsGrid->Font->Size;
	const PercArray1[ColCount2] = {23, 31, 26, 20};
	WidthSoFar = 0;
	PercLeft = 100;
	for (int i=0; i<PaymentsGrid->Columns->Count; i++)
	{
		int ColWidth = (PaymentsGrid->ClientWidth - WidthSoFar) * PercArray1[i] / PercLeft - 1;
		if (PercArray1[i] != 0)
		{
			PaymentsGrid->Columns->Items[i]->Width = ColWidth;
			WidthSoFar += ColWidth + 1;
			PercLeft -= PercArray1[i];
		}
		if (i == PaymentsGrid->Columns->Count-1)
		{
			PaymentsGrid->Columns->Items[i]->Width += 1;
		}
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorInvoices::tvInvoicesChange(TObject *Sender,
		TTreeNode *Node)
{
	Invoice* invoice = 0;

	if (Node->Level == 3) // Payment details
	{
		 invoice = ((Invoice*)Node->Parent->Data);
	}
	else if (Node->Level == 2) // Invoice details
	{
		 invoice = ((Invoice*)Node->Data);
	}

	if (invoice)//Node->Level == 2) // Invoice details
	{
		dbgPayments->DataSource = NULL;
		try
		{
          	qrtotalPriceTotal->SQL->Text =
           " Select "
"    Contacts.Name,Contacts.Mailing_Address,Invoices.Invoice_Number,Invoices.Creation_Date, "
"    Orders.PRICE_INCL Total_Inc,"
"    Invoices.Total_Excl, "
"    Orders.Size_Name, "
"    Cast(Orders.Item_Name as VarChar(50)) Item_Name,Sum(Orders.Qty) qty, "
"    Cast(Sum(Orders.Qty * Orders.PRICE + Orders.Discount ) as numeric(17,4)) Price1,"
"    Cast((Sum(Orders.Qty * Orders.Price + Orders.Discount)) as numeric(17, 4)) PriceExc,Orders.Discount,"
"    Cast((Orders.Price_Adjust) as numeric(17,4)) Price_Adjust, "
"    Orders.Time_Stamp Ordered_Time,HotelCustomer.Customer_Name,HotelCustomer.Room_Number  ,"
"   Cast( "
"        (  "
"          Sum(  "
"               cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +   "
"                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) "
"                      )    +  "
"     cast((  Sum(   "
"                  case when taxsetting.isPriceIncludetax =1 then   "
"                               cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))  "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4))  "
"                    else  "
"                              case when taxsetting.isRecalculateTaxAfterDiscount =1  then  "
"                                   cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                           cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                                             + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4)) "
"                                                                              ) )as numeric(17,4)) )as  numeric(17,4))  "
"                               else    "
"                                      cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4))  "
"                              end    "
"                 end    "
"            )  * cast ((COALESCE(TaxDetails.VAT,0))as numeric (17,4))/100 )as numeric(17,4))  +   "
"      sum( cast((  cast(( coalesce(ORDERS.PRICE,0)-  coalesce(ORDERS.MAXRETAILPRICE,0))as numeric(17,4)) *  cast((coalesce(TaxDetails.ProfitTax,0))as numeric(17,4))/100 )as numeric(17,4)) ) +  "
"        Sum(     "
"             case when taxsetting.isPriceIncludeServiceCharge =1 then   "
"                           cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                   cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                     + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))  "
"              else  "
"                  case when  taxsetting.isRecalculateServiceChargeAfterDiscount =1   then     "
"                                  cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "
"                           else   "
"                           cast (( (cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))   "
"                        end  "
"                end  "
"             )  +  "
"    cast((   Sum(  "
"             case when taxsetting.isPriceIncludeServiceCharge =1 then  "
"                           cast((  (cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))  "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "
"              else  "
"                  case when  taxsetting.isRecalculateServiceChargeAfterDiscount =1   then  "
"                                  cast((  (cast((  cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4)) )as  numeric(17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4))  "
"                              "
"                        else "
"                           cast (( (cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4)) )* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 )as numeric(17,4)) "
"                        end  "
"                end      "
"             ) *  cast((COALESCE(STAX.ServiceChargeTax,0))as numeric(17,4))/100 )as numeric(17,4))  "
"      ) as numeric(17,4))  price  , "
"	(CASE WHEN taxsetting.isRecalculateTaxAfterDiscount = 1 THEN     "
"     cast(  (Sum( cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))   "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4))  "
"                  )    "
"              )* cast ((COALESCE(TaxDetails.VAT,0))as numeric(17,4))/100 as numeric(17, 4)   "
"           )        "
"         else cast((Sum(   cast ((( cast((Orders.Qty)as numeric(17,4))  * cast(Orders.BASE_PRICE as numeric(17,4) )))as numeric (17,4))   ))*    cast((COALESCE(TaxDetails.VAT,0))as numeric(17,4))/100 as numeric(17, 4))  "
"   END ) As VAT  ,   "
"      cast((Sum(  cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))   "
"                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))  "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4))   ))* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 as numeric(17, 4)) ServiceCharge,  "
"                                    "
"                            "
"       cast(  cast((Sum(  cast((  cast((  (cast( (cast((Orders.Qty)as numeric(17,4)) * cast((ORDERS.PRICE)as numeric(17,4)))as numeric(17,4)) + coalesce(ORDERS.DISCOUNT,0)) / (1 + cast((( case when taxsetting.isPriceIncludetax =1 then 1 else 0 end )* (coalesce(TaxDetails.VAT,0)/100))as numeric (17,4)) +  "
"                       cast( ( (case when taxsetting.isPriceIncludeServiceCharge =1 then 1 else 0 end)*(coalesce(TaxDetails.ServiceCharge,0)/100) )as numeric(17,4))  "
"                         + cast(((case when taxsetting.isApplyTaxToServiceCharge =1  and taxsetting.isPriceIncludeServiceCharge =1 and taxsetting.isPriceIncludetax =1 and taxsetting.isPriceIncludetax =1  then 1 else 0 end) *  cast((    (coalesce(STAX.ServiceChargeTax,0)/100) *(coalesce(TaxDetails.ServiceCharge,0)/100))as numeric(17,4)) )as numeric(17,4))  "
"                                                          ) )as numeric(17,4)) )as  numeric(17,4))   ))* cast ((coalesce(TaxDetails.ServiceCharge,0))as numeric(17,4))/100 as numeric(17, 4))  "
"         * cast((COALESCE(STAX.ServiceChargeTax,0))as numeric(17,4))/100 as numeric(17, 4))  ServiceChargeTax   "
  " From   "
"            Contacts Inner Join Invoices on                                                                                                                                                                                                              "
"                Contacts.Contacts_Key = Invoices.Contacts_Key                                                                                                                                                                                            "
"			Inner Join Tab on                                                                                                                                                                                                                             "
"                Invoices.Invoice_Key = Tab.Invoice_Key                                                                                                                                                                                                   "
"			Left Join Orders on                                                                                                                                                                                                                           "
"                Tab.Tab_Key = Orders.Tab_Key                                                                                                                                                                                                             "
"            Left Join HotelCustomer on                                                                                                                                                                                                                   "
"                Invoices.Invoice_Key = HotelCustomer.Invoice_Key                                                                                                                                                                                         "
"            left join                                                                                                                                                                                                                                    "
"            (SELECT  cast(1 as int) keyvalue ,                                                                                                                                                                                                           "
"                    TAXPROFILE.ORDER_KEY,                                                                                                                                                                                                                "
"                    sum(CASE WHEN TAXPROFILE.TYPE = 0 THEN TAXPROFILE.TAX_RATE END) AS VAT ,                                                                                                                                                             "
"                    sum(CASE WHEN TAXPROFILE.TYPE = 2 THEN TAXPROFILE.TAX_RATE END) AS ServiceCharge,                                                                                                                                                    "
"                    sum(CASE WHEN TAXPROFILE.TYPE = 3 THEN TAXPROFILE.TAX_RATE END) AS ServiceChargeTax ,                                                                                                                                                 "
"                    sum(CASE WHEN TAXPROFILE.TYPE = 5 THEN TAXPROFILE.TAX_RATE END) AS ProfitTax   "
"                    FROM (SELECT                                                                                                                                                                                                                         "
"                    TFO.ORDER_KEY,  TAXPROFILES.TYPE,                                                                                                                                                                                                   "
"                    Cast(( sum(COALESCE(TAXPROFILES.RATE,0)) ) as Numeric(17,4)) TAX_RATE                                                                                                                                                                      "
"                    FROM TAXPROFILES_ORDERS TFO                                                                                                                                                                                                          "
"                    left join TAXPROFILES on TAXPROFILES.PROFILE_KEY=TFO.PROFILE_KEY                                                                                                                                                                     "
"                    group by TFO.ORDER_KEY,TAXPROFILES.TYPE,TAXPROFILES.RATE )  TAXPROFILE                                                                                                                                                               "
"                    GROUP BY TAXPROFILE.ORDER_KEY                                                                                                                                                                                                        "
"            ) TaxDetails on TaxDetails.ORDER_KEY=ORDERS.ORDER_KEY                                                                                                                                                                                        "
"          left join (SELECT VARS.keyvalue  ,MIN(CASE WHEN setting.INTEGER_VAL = 1 THEN VARS.ServiceChargeTax else cast(0 as int)   END) AS ServiceChargeTax                                                                                              "
" FROM (  SELECT      cast(1 as int) keyvalue   ,                                                                                                                                                                                                         "
" MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8007 THEN VARSPROFILE.NUMERIC_VAL END                                                                                                                                                                         "
" ) AS ServiceChargeTax      FROM VARSPROFILE   )    VARS                                                                                                                                                                                                 "
" left join (SELECT   cast(1 as int) keyvalue  ,a.INTEGER_VAL FROM VARSPROFILE a where a.VARIABLES_KEY=8005) setting on  setting.keyvalue=    VARS.keyvalue                                                                                               "
" group by 1   ) STAX on  STAX.keyvalue=TaxDetails.keyvalue                                                                                                                                                                                               "
"           left join ( SELECT                                                                                                                                                                                                                            "
"                cast(1 as int) keyvalue   ,                                                                                                                                                                                                              "
"                 MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8000 THEN VARSPROFILE.INTEGER_VAL END ) As isPriceIncludetax  ,                                                                                                                               "
"                 MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8001 THEN VARSPROFILE.INTEGER_VAL END ) As isPriceIncludeServiceCharge  ,                                                                                                                     "
"                 MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8002 THEN VARSPROFILE.INTEGER_VAL END ) As isRecalculateTaxAfterDiscount  ,                                                                                                                   "
"                 MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8003 THEN VARSPROFILE.INTEGER_VAL END ) As  isRecalculateServiceChargeAfterDiscount ,                                                                                                         "
"                 MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8005 THEN VARSPROFILE.INTEGER_VAL END ) As  isApplyTaxToServiceCharge                                                                                                                         "
"                 from VARSPROFILE) taxsetting on  taxsetting.keyvalue=TaxDetails.keyvalue                                                                                                                                                                "
"        Where                                                                                                                                                                                                                                            "
"			Invoices.Invoice_Number =  :Invoice_Number                                                                                                                                                                                                                "
"            group by                                                                                                                                                                                                                                     "
"                Contacts.Name,                                                                                                                                                                                                                           "
"                Contacts.Mailing_Address,                                                                                                                                                                                                                "
"                Invoices.Invoice_Number,                                                                                                                                                                                                                 "
"                Invoices.Creation_Date,                                                                                                                                                                                                                  "
"                Orders.PRICE_INCL,                                                                                                                                                                                                                       "
"                Invoices.Total_Excl,                                                                                                                                                                                                                     "
"                Orders.Size_Name,                                                                                                                                                                                                                        "
"                Orders.Item_Name ,                                                                                                                                                                                                                       "
"                Orders.Qty,                                                                                                                                                                                                                              "
"                Orders.Price , Orders.Discount ,                                                                                                                                                                                                         "
"                Orders.Price_Adjust,                                                                                                                                                                                                                     "
"                Orders.Time_Stamp ,                                                                                                                                                                                                                      "
"                HotelCustomer.Customer_Name,                                                                                                                                                                                                             "
"                HotelCustomer.Room_Number,                                                                                                                                                                                                               "
"                Orders.BASE_PRICE,                                                                                                                                                                                                                       "
"                Orders.DISCOUNT_WITHOUT_TAX,                                                                                                                                                                                                             "
"                TaxDetails.VAT,                                                                                                                                                                                                                          "
"                TaxDetails.ServiceCharge,                                                                                                                                                                                                                "
"                TaxDetails.ServiceChargeTax,                                                                                                                                                                                                             "
"                STAX.ServiceChargeTax ,                                                                                                                                                                                                                  "
"                Orders.order_key ,                                                                                                                                                                                                                       "
"                taxsetting.isRecalculateTaxAfterDiscount                                                                                                                                                                                                 "
"               union all                                                                                                                                                                            "
"     select                                                                                                                                                                                          "
"            Contacts.Name,                                                                                                                                                                           "
"			Contacts.Mailing_Address,                                                                                                                                                                "
"			Invoices.Invoice_Number,                                                                                                                                                                 "
"		    Invoices.Creation_Date,                                                                                                                                                                  "
"		    DayArchive.PRICE_INCL Total_Inc,                                                                                                                                                         "
"			Invoices.Total_Excl,                                                                                                                                                                     "
"		    DayArchive.Size_Name,                                                                                                                                                                    "
"		    Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,                                                                                                                                     "
"		    Sum(DayArchive.Qty) qty,                                                                                                                                                                 "
"		    Cast(Sum(DayArchive.Qty * DayArchive.Price + coalesce(DayArchive.Discount,0)) as numeric(17,4)) Price1,                                                                                  "
"			Cast(Sum(cast((DayArchive.Qty * DayArchive.Price + DayArchive.Discount) as Numeric(17,4))  ) as numeric(17,4)) PriceExc,                                                                 "
"		  	DayArchive.Discount,                                                                                                                                                                     "
"            Cast((DayArchive.Price_Adjust) as numeric(17,4)) Price_Adjust,                                                                                                                           "
"		    DayArchive.Time_Stamp Ordered_Time,                                                                                                                                                      "
"            HotelCustomer.Customer_Name,                                                                                                                                                             "
"            HotelCustomer.Room_Number,                                                                                                                                                               "
"            Cast(Sum(DayArchive.Qty * DayArchive.BASE_PRICE + (case when STAX.isRecalculateTaxAfterDiscount =1 then  coalesce(DAYARCHIVE.DISCOUNT_WITHOUT_TAX,0) else coalesce(DAYARCHIVE.DISCOUNT,0) end )) + sum(COALESCE(AOT.VAT,0)) + sum(COALESCE(AOT.ServiceCharge,0))                                       "
"	       + sum(COALESCE(AOT.ServiceChargeTax ,0))                                                                                                                                                  "
"           as Numeric(17,4)) Price ,                                                                                                                                                                 "
"          Cast(sum(COALESCE(AOT.VAT,0)) as Numeric(17,4)) VAT,                                    "
 "          Cast(sum(COALESCE(AOT.ServiceCharge,0)) as Numeric(17,4)) ServiceCharge,                "
 "                    Cast(sum(COALESCE(AOT.ServiceChargeTax,0)) as Numeric(17,4))ServiceChargeTax   "
"           From                                                                                                                                                                                      "
"			Contacts Inner Join Invoices on                                                                                                                                                          "
"            Contacts.Contacts_Key = Invoices.Contacts_Key                                                                                                                                            "
"			Inner Join DayArcBill on                                                                                                                                                                 "
"            Invoices.Invoice_Key = DayArcBill.Invoice_Key                                                                                                                                            "
"			Left Join DayArchive on                                                                                                                                                                  "
"            DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key                                                                                                                                          "
"           Left Join HotelCustomer on                                                                                                                                                                "
"        Invoices.Invoice_Key = HotelCustomer.Invoice_Key                                                                                                                                             "
"         LEFT JOIN (                                                                                                                                                                                 "
"    SELECT                                                                                                                                                                                           "
"                DAYARCORDERTAXES.ARCHIVE_KEY, cast(1 as int) keyvalue ,                                                                                                                              "            
"                MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT  ,                                                                                           "
"                MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge  ,                                                                                 "
"                MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax                                                                                 "
"				  FROM  DAYARCORDERTAXES                                                                                                                                                             "
"                GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )                                                                                                                                              "
"                AOT ON DAYARCHIVE.ARCHIVE_KEY = AOT.ARCHIVE_KEY   left join (SELECT      cast(1 as int) keyvalue   ,                                                                                 "
"                   MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8007 THEN VARSPROFILE.NUMERIC_VAL END) AS ServiceChargeTax ,                                                                             "
"                   MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8002 THEN VARSPROFILE.INTEGER_VAL END ) As isRecalculateTaxAfterDiscount  ,                                                             "
"                   MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8003 THEN VARSPROFILE.INTEGER_VAL END ) As  isRecalculateServiceChargeAfterDiscount                                                     "
"                FROM VARSPROFILE    ) STAX on  STAX.keyvalue=AOT.keyvalue                                                                                                                            "
"                Where                                                                                                                                                                                "
"			    invoices.Invoice_Number = :Invoice_Number                                                                                                                                            "
"						 group by                                                                                                                                                                    "
"		        Contacts.Name,                                                                                                                                                                       "
"                Contacts.Mailing_Address,                                                                                                                                                            "
"                Invoices.Invoice_Number,                                                                                                                                                             "
"                Invoices.Creation_Date,                                                                                                                                                              "
"                DayArchive.PRICE_INCL,                                                                                                                                                               "
"                Invoices.Total_Excl,                                                                                                                                                                 "
"                DayArchive.Size_Name,                                                                                                                                                                "
"                DayArchive.Item_Name ,                                                                                                                                                               "
"                DayArchive.Qty,                                                                                                                                                                      "
"                DayArchive.Price ,                                                                                                                                                                   "
"                 DayArchive.Discount ,                                                                                                                                                               "
"                DayArchive.Price_Adjust,                                                                                                                                                             "
"                DayArchive.Time_Stamp ,                                                                                                                                                              "
"                HotelCustomer.Customer_Name,                                                                                                                                                         "
"               HotelCustomer.Room_Number,                                                                                                                                                            "
"                DayArchive.BASE_PRICE,                                                                                                                                                               "
"                AOT.VAT,                                                                                                                                                                             "
"                 AOT.ServiceCharge,                                                                                                                                                                  "
"                 STAX.ServiceChargeTax                                                                                                                                                               "
"                 union all                                                                                                                                                                           "
" select                                                                                                                                                                                              "
"            Contacts.Name,                                                                                                                                                                           "
"			Contacts.Mailing_Address,                                                                                                                                                                "
"			Invoices.Invoice_Number,                                                                                                                                                                 "
"		    Invoices.Creation_Date,                                                                                                                                                                  "
"		    Archive.PRICE_INCL Total_Inc,                                                                                                                                                            "
"			Invoices.Total_Excl,                                                                                                                                                                     "
"		    Archive.Size_Name,                                                                                                                                                                       "
"		    Cast(Archive.Item_Name as VarChar(50)) Item_Name,                                                                                                                                        "
"		    Sum(Archive.Qty) qty,                                                                                                                                                                    "
"		    Cast(Sum(Archive.Qty * Archive.Price + coalesce(Archive.Discount,0)) as numeric(17,4)) Price1,                                                                                           "
"			Cast(Sum(cast((Archive.Qty * Archive.Price + Archive.Discount) as Numeric(17,4))  ) as numeric(17,4)) PriceExc,                                                                          "
"		  	Archive.Discount,                                                                                                                                                                        "
"           Cast((Archive.Price_Adjust) as numeric(17,4)) Price_Adjust,                                                                                                                               "
"		   Archive.Time_Stamp Ordered_Time,                                                                                                                                                          "
"           HotelCustomer.Customer_Name,                                                                                                                                                              "
"           HotelCustomer.Room_Number,                                                                                                                                                                "
"          Cast(Sum(Archive.Qty * Archive.BASE_PRICE + COALESCE(Archive.DISCOUNT,0)) + sum(COALESCE(AOT.VAT,0)) + sum(COALESCE(AOT.ServiceCharge,0))                                                  "
"	      + sum(COALESCE(AOT.ServiceChargeTax ,0))                                                                                                                                                   "
"           as Numeric(17,4)) Price ,                                                                                                                                                                 "
"          Cast(sum(COALESCE(AOT.VAT,0)) as Numeric(17,4)) VAT,                                    "
 "          Cast(sum(COALESCE(AOT.ServiceCharge,0)) as Numeric(17,4)) ServiceCharge,                "
"                    Cast(sum(COALESCE(AOT.ServiceChargeTax,0)) as Numeric(17,4))ServiceChargeTax   "
"         From                                                                                                                                                                                        "
"			Contacts Inner Join Invoices on                                                                                                                                                          "
"            Contacts.Contacts_Key = Invoices.Contacts_Key                                                                                                                                            "
"			Inner Join ArcBill on                                                                                                                                                                    "
"            Invoices.Invoice_Key = ArcBill.Invoice_Key                                                                                                                                               "
"			Left Join Archive on                                                                                                                                                                     "
"            ArcBill.ArcBill_Key = Archive.ArcBill_Key                                                                                                                                                "
"            Left Join HotelCustomer on                                                                                                                                                               "
"            Invoices.Invoice_Key = HotelCustomer.Invoice_Key                                                                                                                                         "
"            LEFT JOIN (                                                                                                                                                                              "
" SELECT                                                                                                                                                                                              "
"					   	ARCORDERTAXES.ARCHIVE_KEY, cast(1 as int) keyvalue ,                                                                                                                         "
"					   	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 0 THEN ARCORDERTAXES.TAX_VALUE END) AS VAT  ,                                                                                         "
"					   	MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 2 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceCharge  ,                                                                               "
"                        MIN(CASE WHEN ARCORDERTAXES.TAX_TYPE = 3 THEN ARCORDERTAXES.TAX_VALUE END) AS ServiceChargeTax                                                                               "
"				  FROM  ARCORDERTAXES                                                                                                                                                                "
"					   	GROUP BY ARCORDERTAXES.ARCHIVE_KEY )                                                                                                                                         "
"						AOT ON Archive.ARCHIVE_KEY = AOT.ARCHIVE_KEY   left join (SELECT      cast(1 as int) keyvalue   ,                                                                            "
"	       MIN(CASE WHEN VARSPROFILE.VARIABLES_KEY = 8007 THEN VARSPROFILE.NUMERIC_VAL END) AS ServiceChargeTax      FROM VARSPROFILE    ) STAX on  STAX.keyvalue=AOT.keyvalue                       "
"						 Where                                                                                                                                                                       "
"			invoices.Invoice_Number = :Invoice_Number                                                                                                                                                "
"						 group by                                                                                                                                                                    "
"		        Contacts.Name,                                                                                                                                                                       "
"                Contacts.Mailing_Address,                                                                                                                                                            "
"                Invoices.Invoice_Number,                                                                                                                                                             "
"                Invoices.Creation_Date,                                                                                                                                                              "
"                Archive.PRICE_INCL,                                                                                                                                                                  "
"                Invoices.Total_Excl,                                                                                                                                                                 "
"                Archive.Size_Name,                                                                                                                                                                   "
"                Archive.Item_Name ,                                                                                                                                                                  "
"                Archive.Qty,                                                                                                                                                                         "
"                Archive.Price ,                                                                                                                                                                      "
"                 Archive.Discount ,                                                                                                                                                                  "
"                Archive.Price_Adjust,                                                                                                                                                                "
"                Archive.Time_Stamp ,                                                                                                                                                                 "
"                HotelCustomer.Customer_Name,                                                                                                                                                         "
"               HotelCustomer.Room_Number,                                                                                                                                                            "
"                Archive.BASE_PRICE,                                                                                                                                                                  "
"                AOT.VAT,                                                                                                                                                                             "
"                 AOT.ServiceCharge,                                                                                                                                                                  "
"                 STAX.ServiceChargeTax                                                                                                                                                               "
"                 Order By                                                                                                                                                                            "
"			      8,7 ";

            	qrtotalPriceTotal->Close();
			qrtotalPriceTotal->ParamByName("Invoice_Number")->AsString = invoice->InvoiceNumber;
			qrtotalPriceTotal->Open();

			Currency total = 0;

			while (!qrtotalPriceTotal->Eof)
			{
				total += qrtotalPriceTotal->FieldByName("Price")->AsCurrency;
				qrtotalPriceTotal->Next();
			}
			invoice->Total = total;


		   qrInvoiceDetail->Close();
			qrInvoiceDetail->ParamByName("Invoice_Number")->AsString = invoice->InvoiceNumber;
			qrInvoiceDetail->Open();

		//	Currency total = 0;

		  /*	while (!qrInvoiceDetail->Eof)
			{
				total += qrInvoiceDetail->FieldByName("Price")->AsCurrency;
				qrInvoiceDetail->Next();
			}
			invoice->Total = total;   */

			qrInvoiceDetail->First();

			((TFloatField *)qrInvoiceDetail->FieldByName("Price"))->currency = true;
			qrInvoiceDetail->FieldByName("Qty")->Alignment = taLeftJustify;

			PageControl->ActivePage = tsInvoice;
		}
		__finally
		{
			dbgPayments->DataSource = dsPayments;
		}
	}
	if (Node->Level == 3)
	{
		PageControl->ActivePage = tsPayments;

		qrPayments->Close();
		qrPayments->ParamByName("InvoiceKey")->AsInteger = invoice->Key;
		qrPayments->Open();
		((TFloatField *)qrPayments->FieldByName("SubTotal"))->currency = true;
	}

	if (invoice != 0)
	{
		lbeReference->Caption = "Reference: " + invoice->InvoiceNumber + " - " +
			FormatDateTime("d mmmm yyyy", invoice->DateTime);

		lbeInvoiceTotal->Caption = CurrencyString + CurrToStrF(invoice->Total, ffFixed, CurrencyDecimals);
		lbePayments->Caption = CurrencyString + CurrToStrF(invoice->PaymentTotal, ffFixed, CurrencyDecimals);
	}
	else
	{
		qrInvoiceDetail->Close();
		lbeReference->Caption = "";
		qrPayments->Close();
	}

	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorInvoices::FormResize(TObject *Sender)
{
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorInvoices::btnPrintInvoiceClick(TObject *Sender)
{
	if (tvInvoices->Selected)
	{
		Invoice* invoice = 0;

		if (tvInvoices->Selected->Level == 3) // Payment details
		{
			 invoice = ((Invoice*)tvInvoices->Selected->Parent->Data);
		}
		else if (tvInvoices->Selected->Level == 2) // Invoice details
		{
			 invoice = ((Invoice*)tvInvoices->Selected->Data);
		}
		if (invoice != 0)
		{
			dmMMReportData->MMTrans->StartTransaction();
			try
			{
				dmMMReportData->SetupDebtorInvoice(invoice->InvoiceNumber);

				bool reportSelected;

				if (Sender == btnPrintInvoice)
				{
					reportSelected = frmReports->rvMenuMate->SelectReport("repDebtorInvoice", false);
				}
				else
				{
					reportSelected = frmReports->rvMenuMate->SelectReport("repDebtorInvoiceDetails", false);
				}

				if (reportSelected)
				{
					frmReports->rvMenuMate->SetParam("GSTNumber", CurrentConnection.GSTNumber);
					frmReports->rvMenuMate->SetParam("CompanyName", CurrentConnection.CompanyName);
					frmReports->rvMenuMate->SetParam("CompanyAddress", CurrentConnection.ReportHeader);
					frmReports->rvMenuMate->Execute();
				}
			}
			__finally
			{
				dmMMReportData->MMTrans->Commit();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorInvoices::btnFindClick(TObject *Sender)
{
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorInvoices::FindDialogFind(TObject *Sender)
{
	TTreeNode *CurItem;
	if (tvInvoices->Selected)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = tvInvoices->Selected->GetNext();
		}
		else
		{
			CurItem = tvInvoices->Selected->GetPrev();
		}
	}
	else
	{
		CurItem = tvInvoices->Items->GetFirstNode();
	}
	while (CurItem)
	{
		AnsiString FindText, NodeText;
		if (FindDialog->Options.Contains(frMatchCase))
		{
			FindText = FindDialog->FindText;
			NodeText = CurItem->Text;
		}
		else
		{
			FindText = FindDialog->FindText.UpperCase();
			NodeText = CurItem->Text.UpperCase();
		}
		if (FindDialog->Options.Contains(frWholeWord))
		{
			if (FindText == NodeText)
			{
				tvInvoices->Selected = CurItem;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				tvInvoices->Selected = CurItem;
				break;
			}
		}
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = CurItem->GetNext();
		}
		else
		{
			CurItem = CurItem->GetPrev();
		}
	}
	if (!CurItem)
	{
		Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmDebtorInvoices::tvInvoicesDeletion(TObject *Sender,	TTreeNode *Node)
{
	if (Node->Data)
	{
		if (Node->Level == 2)
		{
			Invoice *invoice = (Invoice *)Node->Data;
			delete invoice;
		}
	}
}
//---------------------------------------------------------------------------

