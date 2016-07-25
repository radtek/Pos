//---------------------------------------------------------------------------
#pragma hdrstop

#include <dir.h>
#include <SysUtils.hpp>
#include "MYOBInvoiceExporter.h"
#include "Connections.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Accounts;
//---------------------------------------------------------------------------
MYOBInvoiceExporter::MYOBInvoiceExporter(const AnsiString& const filePath) :
	exportFilePath(filePath),
   fileHandle(0),
	prevCustomerName("")
{
}
//---------------------------------------------------------------------------
void MYOBInvoiceExporter::Open()
{
	if (fileHandle != 0)
	{
		Close();
	}

	AnsiString header;
    CurrentConnection.SalesInvoiceExportType = "Accounting";

		header =
          "Co./Last Name, "
          "First Name, "
          "Invoice  No., "
          "Date, "
          "Customer PO, "
          "Inclusive, "
          "Delivery Status, "
          "Detail Date, "
          "Description, "
          "Account #, "
          "Amount, "
          "Inc-Tax Amount, "
          "Job, "
          "Comment, "
          "Journal Memo, "
          "Promised Date, "
          "Referral Source, "
          "Tax Code, "
          "NON GST Amount, "
          "GST Amount, "
          "LCT Amount, "
          "Sale Status, ";

        if (CurrentConnection.AccountSystem != "MYOB AU SCSI") {
            header = header + "Currency Code, Exchange Rate, ";
        }

        header = header +
          "Terms - Payment is Due, "
          "- Discount Days, "
          "- Balance Due Days, "
          "- % Discount, "
          "- % Monthly Charge, "
          "Salesperson Last Name, "
          "Salesperson First Name, "
          "Amount Paid, "
          "Payment Method, "
          "Payment Notes, "
          "Name on Card, "
          "Card Number, "
          "Expiry Date, "
          "Authorisation Code, "
          "BSB, "
          "Account Number, "
          "Drawer/Account Name, "
          "Cheque Number, "
          "Category, "
          "Card ID, "
          "Record ID";

	header += "\r\n";

	fileHandle = FileCreate(exportFilePath);
	FileWrite(fileHandle, header.c_str(), header.Length());
}
//---------------------------------------------------------------------------
void MYOBInvoiceExporter::Export(const Accounts::Invoice& const invoice)
{
    std::list<AnsiString> fields;
    AnsiString currCustomerName = invoice.GetCustomer().GetAccountName();
    AnsiString records;

    bool is_scsi = CurrentConnection.AccountSystem == "MYOB AU SCSI";

    if (prevCustomerName != currCustomerName && is_scsi) {
        fields.push_back("\r\n" + currCustomerName);
        prevCustomerName = currCustomerName;
    } else fields.push_back(currCustomerName);

    fields.push_back("");
    fields.push_back(invoice.Reference);
    fields.push_back(invoice.CreatedDateTime.FormatString("dd/mm/yyyy"));
    fields.push_back("");
    fields.push_back("X");
    fields.push_back("A");
    fields.push_back(invoice.CreatedDateTime.FormatString("dd/mm/yyyy"));

    fields.push_back("Inv " + invoice.Reference + " " + invoice.Description);
    fields.push_back(CurrentConnection.SalesInvoiceExportAccount);
    fields.push_back(FormatFloat("$0.00##", invoice.TotalExcl));
    fields.push_back(FormatFloat("$0.00##", invoice.TotalInc));
    fields.push_back("");
    fields.push_back("");
    fields.push_back("Invoice " + invoice.Reference);
    fields.push_back("");
    fields.push_back("");
    fields.push_back("GST");
    fields.push_back("$0.00");
    fields.push_back(FormatFloat("$0.00##", invoice.TotalInc - invoice.TotalExcl));
    fields.push_back("$0.00");
    fields.push_back("I");

    if (is_scsi) {
        fields.push_back("");
        fields.push_back("");
    }
    fields.push_back("5");
    fields.push_back("1");
    fields.push_back("30");
    fields.push_back("0");
    fields.push_back("0");
    fields.push_back("");
    fields.push_back("");
    fields.push_back("$0.00");
    fields.push_back("");
    fields.push_back("");
    fields.push_back("");
    fields.push_back("");
    fields.push_back("");
    fields.push_back("");
    fields.push_back("");
    fields.push_back("");
    fields.push_back("");
    fields.push_back("");
    fields.push_back("");
    fields.push_back(invoice.GetCustomer().GetAccountID());
    fields.push_back("");

    std::list<AnsiString>::const_iterator i = fields.begin();
    records = *i++;
    while (i != fields.end())
        records = records + "," + *i++;
    records = records + "\r\n";



	FileWrite(fileHandle, records.c_str(), records.Length());
}
//---------------------------------------------------------------------------
void MYOBInvoiceExporter::Close()
{
	FileClose(fileHandle);
   fileHandle = 0;
}
//---------------------------------------------------------------------------
