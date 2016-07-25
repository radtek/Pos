//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AccountsExportForm.h"
#include "EditAccountCustomerForm.h"
#include "MYOBInvoiceExporter.h"
#include "MYOBAccountExporter.h"
#include "Connections.h"
#include "MMData.h"
#include "MMRegistry.h"
#include "MM_DBCore.h"
#include "Consts.h"
#include "Connections.h"
#include "..\TinyXml\tinyxml.h"
#include "..\Helpers.h"
//---------------------------------------------------------------------------
#include <systdate.h>
#include <DateUtils.hpp>
#include <systdate.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
class ScreenInvoiceExporter : public Accounts::IExporter<Accounts::Invoice>
{
public:
	ScreenInvoiceExporter(TListView* lv) : listView(lv)
	{
 		listView->Items->Clear();
	}

	virtual void Export(const Accounts::Invoice& const invoice)
   {
		TListItem *listItem = listView->Items->Add();
		listItem->Caption = invoice.Exported() ? AnsiString("T") : AnsiString();

		listItem->SubItems->Add(invoice.GetCustomer().GetName());
		listItem->SubItems->Add(invoice.Reference);
		listItem->SubItems->Add(invoice.CreatedDateTime.FormatString("dd/mm/yyyy"));
		listItem->SubItems->Add(FormatFloat("$0.00", invoice.TotalInc));

   }

private:

	TListView* listView;
};
//---------------------------------------------------------------------------
// XeroInvoiceExporter
// Build an internal data structure with all invoices and their line items,
// then send to Xero.
class XeroInvoiceExporter : public Accounts::IExporter<Accounts::Invoice> {
        private:
                Ibsql::TIBSQL *_query;
                TiXmlDocument _doc;
                TiXmlElement *_rootNode;
        public:
                XeroInvoiceExporter() {
                    // add root node and link to document
					_rootNode = new TiXmlElement("Invoices");
					_doc.LinkEndChild(_rootNode);
                }

                virtual void OpenWithQuery(Ibsql::TIBSQL*q) {
                    _query = q;
                };


                virtual void Close() {
                    //write xml document out to XeroMate
                    TiXmlPrinter printer;
                    printer.SetStreamPrinting();
                    _doc.Accept(&printer);
                    if (!Helpers::SendInvoiceToXero(printer.CStr(), printer.Size() + sizeof(char))) {
                        Application->MessageBox("Invoice NOT sent to Xero!", "Error", MB_OK);
                    }
                }

                // create exactly one (1) Invoice node and link to root node
                virtual void Export(const Accounts::Invoice& const invoice) {
                    // add invoice node
					TiXmlElement *invoiceNode = new TiXmlElement("Invoice");
					_rootNode->LinkEndChild(invoiceNode);

                    //
					// add invoice details
                    //
                    
                    // invoice type
                    TiXmlElement *typeNode = new TiXmlElement("Type");
                    TiXmlText *typeTextNode = new TiXmlText("ACCREC");
                    typeNode->LinkEndChild(typeTextNode);
                    invoiceNode->LinkEndChild(typeNode);

                    // contact info
                    TiXmlElement *contactNode = new TiXmlElement("Contact");
                    TiXmlElement *contactNameNode = new TiXmlElement("Name");
                    TiXmlText *contactNameTextNode = new TiXmlText(invoice.GetCustomer().GetName().c_str());
                    contactNameNode->LinkEndChild(contactNameTextNode);
                    contactNode->LinkEndChild(contactNameNode);
                    invoiceNode->LinkEndChild(contactNode);

                    // invoice number
                    TiXmlElement *invoiceNumberNode = new TiXmlElement("InvoiceNumber");
                    TiXmlText *invoiceNumberTextNode = new TiXmlText(invoice.Reference.c_str());
                    invoiceNumberNode->LinkEndChild(invoiceNumberTextNode);
                    invoiceNode->LinkEndChild(invoiceNumberNode);

                    // date
                    TiXmlElement *dateNode = new TiXmlElement("Date");
                    TiXmlText *dateTextNode =
                        new TiXmlText(invoice.CreatedDateTime.FormatString("yyyy-mm-dd").c_str());
                    dateNode->LinkEndChild(dateTextNode);
                    invoiceNode->LinkEndChild(dateNode);

                    // due date - leave blank for now and let Xero deal with it

                    // line items incl/excl gst
                    //TiXmlElement *lineAmountTypeNode = new TiXmlElement("LineAmountTypes");
                    //TiXmlText *lineAmountTypeTextNode = NULL;
                    //if (rbExcludingGST->Checked) {
                    //    lineAmountTypeTextNode = new TiXmlText("Exclusive");
                    //}
                    //else {
                    //lineAmountTypeTextNode = new TiXmlText("Inclusive");
                    //}
                    //lineAmountTypeNode->LinkEndChild(lineAmountTypeTextNode);
                    //invoiceNode->LinkEndChild(lineAmountTypeNode);

                    // line items
                    //TiXmlElement *lineItemsNode = new TiXmlElement("LineItems");
                    //invoiceNode->LinkEndChild(lineItemsNode);

                    // TODO: build line items


                }
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TfrmAccountsExport::TfrmAccountsExport()
	: TForm((TComponent*)NULL)
{
	accountsExporter = new Accounts::AccountsExporter();

	cbEndHours->Items->Clear();
	for (int i=0; i<=23; ++i)
	{
		cbEndHours->Items->Add(FormatFloat("00", i));
   }

	cbEndMinutes->Items->Clear();
	for (int i=0; i<=59; ++i)
   {
		cbEndMinutes->Items->Add(FormatFloat("00", i));
	}

	dtpEndDate->Date = TDateTime(int(IncDay(Now(), 1)));
	cbEndHours->ItemIndex = cbEndHours->Items->IndexOf("05");
	cbEndMinutes->ItemIndex = cbEndMinutes->Items->IndexOf("00");

	cbStartHours->Items->Clear();
	for (int i=0; i<=23; ++i)
	{
		cbStartHours->Items->Add(FormatFloat("00", i));
   }

	cbStartMinutes->Items->Clear();
	for (int i=0; i<=59; ++i)
	{
		cbStartMinutes->Items->Add(FormatFloat("00", i));
	}

	dtpStartDate->Date = TDateTime(int(IncMonth(Now(), -1)));
	cbStartHours->ItemIndex = cbStartHours->Items->IndexOf("05");
	cbStartMinutes->ItemIndex = cbStartMinutes->Items->IndexOf("00");

	chbUseStartFilter->Checked = false;

	LoadSettings();


  	if (edAccountCode->Text == "")
  	{
  		PageControl1->ActivePage = tsSetup;
  	}
  	else
  	{
  		PageControl1->ActivePage = tsInvoices;
	}

}
//---------------------------------------------------------------------------
__fastcall TfrmAccountsExport::~TfrmAccountsExport()
{
	delete accountsExporter;
}
//---------------------------------------------------------------------------
void TfrmAccountsExport::LoadSettings()
{
	edAccountCode->Text = CurrentConnection.SalesInvoiceExportAccount;
	settingsChanged = false;
	btnSaveSettings->Enabled = settingsChanged;
}
//---------------------------------------------------------------------------
void TfrmAccountsExport::SaveSettings()
{
	CurrentConnection.SalesInvoiceExportAccount = edAccountCode->Text;
	CurrentConnection.SalesInvoiceExportType = "Accounting";

	AnsiString Key = OfficeKey + "\\" + CurrentConnection.CompanyName;
	RegistryWrite(Key + "\\MYOB", "SalesInvoiceExportAccount", CurrentConnection.SalesInvoiceExportAccount);
	RegistryWrite(Key + "\\MYOB", "SalesInvoiceExportType", CurrentConnection.SalesInvoiceExportType);

	LoadSettings();
}
//---------------------------------------------------------------------------
void TfrmAccountsExport::SettingsChanged()
{
	settingsChanged = true;
	btnSaveSettings->Enabled = settingsChanged;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::SettingsChangedEvent(TObject *Sender)
{
	SettingsChanged();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::btnSaveSettingsClick(TObject *Sender)
{
	SaveSettings();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::FormShow(TObject *Sender)
{
	RefreshAccounts();
   accountsExporter->BeginLoadCustomers(OnLoadCustomersUpdate, OnLoadCustomersComplete);
}
//---------------------------------------------------------------------------
void TfrmAccountsExport::GetDateTimeFilter()
{
	int endDate = dtpEndDate->Date;
	int endHour = 0;
	int endMinute = 0;

	try
   {
		endHour = cbEndHours->Text.ToInt();
	}
	catch (EConvertError&)
	{
		cbEndHours->Text = "00";
	}

	try
	{
		endMinute = cbEndMinutes->Text.ToInt();
	}
	catch (EConvertError&)
	{
		cbEndMinutes->Text = "00";
	}

	endDateTimeFilter = TDateTime(endDate);
	endDateTimeFilter += TDateTime(endHour, endMinute, 0, 0);

	if (chbUseStartFilter->Checked == false)
	{
		int startDate = dtpStartDate->Date;
		int startHour = 0;
		int startMinute = 0;

		try
		{
			startHour = cbStartHours->Text.ToInt();
		}
		catch (EConvertError&)
		{
			cbStartHours->Text = "00";
		}

		try
		{
			startMinute = cbStartMinutes->Text.ToInt();
		}
		catch (EConvertError&)
		{
			cbStartMinutes->Text = "00";
		}

		startDateTimeFilter = TDateTime(startDate);
		startDateTimeFilter += TDateTime(startHour, startMinute, 0, 0);
	}
	else
	{
		startDateTimeFilter = TDateTime(0);
	}
}
//---------------------------------------------------------------------------
void TfrmAccountsExport::DisplayInvoices()
{
	std::auto_ptr<Accounts::IExporter<Accounts::Invoice> > exporter(new ScreenInvoiceExporter(ListView1));
	accountsExporter->Export(*exporter);
}
//---------------------------------------------------------------------------
void TfrmAccountsExport::RefreshAccounts()
{
   GetDateTimeFilter();
	accountsExporter->LoadInvoices(startDateTimeFilter, endDateTimeFilter);
   DisplayInvoices();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::DateTimeChange(TObject *Sender)
{
	RefreshAccounts();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::btnExitMouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::btnExportMouseClick(TObject *Sender)
{

    if (CurrentConnection.AccountSystem == "Xero") {
      	std::auto_ptr<Accounts::IExporter<Accounts::Invoice> > exporter(new XeroInvoiceExporter());
       	accountsExporter->Export(*exporter);
       	RefreshAccounts();
    }
    else {

        std::auto_ptr<TSaveDialog> saveDialog(new TSaveDialog(NULL));

        saveDialog->FileName = "Invoices.txt";
        saveDialog->InitialDir = CurrentConnection.MYOBPath;

        if (saveDialog->Execute()) {
        	std::auto_ptr<Accounts::IExporter<Accounts::Invoice> > exporter(new Accounts::MYOBInvoiceExporter(saveDialog->FileName));
        	accountsExporter->Export(*exporter);
        	RefreshAccounts();
         }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::PageControl1Change(TObject *Sender)
{
   btnExport->Enabled = (PageControl1->ActivePage == tsInvoices);
   ProgressBar1->Position = 0;
   ProgressBar1->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::PageControl1Changing(TObject *Sender,
		bool &AllowChange)
{
   if (PageControl1->ActivePage == tsSetup) {
		if (settingsChanged)
		{
			int mbResult = Application->MessageBox("Save Settings?", "Settings have been changed. Do you wish to save these settings?", MB_YESNOCANCEL | MB_ICONQUESTION);

			if (mbResult == IDYES)
			{
				SaveSettings();
			}
			else if (mbResult == IDCANCEL)
			{
				AllowChange = false;
			}
			else
			{
				LoadSettings();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::OnLoadCustomersUpdate(
	const Accounts::Customer& customer,
	int percentComplete)
{
	ProgressBar1->Position = percentComplete;

	TListItem *listItem = lvCustomers->Items->Add();
	listItem->Data = (void*)customer.GetKey();
	listItem->Caption = customer.GetName();
	listItem->SubItems->Add(customer.GetAccountName());
	listItem->SubItems->Add(customer.GetAccountID());
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::OnLoadCustomersComplete()
{
	ProgressBar1->Visible = false;
   btnExportCustomers->Enabled = true;
   btnExportCustomers->Caption = "Export...";
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::btnExportCustomersMouseClick(
      TObject *Sender)
{
   std::auto_ptr<TSaveDialog> saveDialog(new TSaveDialog(NULL));

   saveDialog->FileName = "Customers.txt";
   saveDialog->InitialDir = CurrentConnection.MYOBPath;

   if (saveDialog->Execute())
   {
        std::auto_ptr<Accounts::IExporter<Accounts::Customer> > exporter(new Accounts::MYOBCustomerExporter(saveDialog->FileName));
	accountsExporter->Export(*exporter);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::btnEditCustomerMouseClick(
		TObject *Sender)
{
	TListItem* selected = lvCustomers->Selected;

	if (selected && selected->Data)
	{
		if (selected->Caption == "")
		{
			::MessageBox(NULL, "This customer has no name.\r\r"
				"This must be fixed from the POS before continuing.",
				"Error",
				MB_ICONERROR | MB_OK); 
		}
		else
		{
			AnsiString accountName = selected->SubItems->Strings[0];
			AnsiString accountID = selected->SubItems->Strings[1];

			if (TfrmEditAccountCustomer::Execute(selected->Caption, accountName, accountID))
			{
				const Accounts::Customer& edited = accountsExporter->EditCustomer((int)selected->Data,
					accountName, accountID);

				selected->SubItems->Strings[0] = edited.GetAccountName();
				selected->SubItems->Strings[1] = edited.GetAccountID();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::lvCustomersSelectItem(TObject *Sender,
		TListItem *Item, bool Selected)
{
	btnEditCustomer->Enabled = (Selected && Item != NULL) || !Selected;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAccountsExport::chbUseStartFilterClick(TObject *Sender)
{
	dtpStartDate->Enabled = !chbUseStartFilter->Checked;
	cbStartHours->Enabled = !chbUseStartFilter->Checked;
	cbStartMinutes->Enabled = !chbUseStartFilter->Checked;

	if (chbUseStartFilter->Checked == false)
	{
		if (int(dtpStartDate->Date) == 0)
		{
			dtpStartDate->Date = int(IncWeek(dtpEndDate->Date, -1));
		}
	}

	RefreshAccounts();
}
//---------------------------------------------------------------------------

