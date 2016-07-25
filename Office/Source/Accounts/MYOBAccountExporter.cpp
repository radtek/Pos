//---------------------------------------------------------------------------
#pragma hdrstop

#include "MYOBAccountExporter.h"
#include <dir.h>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include "Connections.h"

#include <iostream>

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Accounts;
//---------------------------------------------------------------------------
MYOBCustomerExporter::MYOBCustomerExporter(const AnsiString& const filePath) :
	exportFilePath(filePath),
   fileHandle(0)
{
}
//---------------------------------------------------------------------------
void MYOBCustomerExporter::Open()
{
    AnsiString type_template("MYOB ?? SCSI");

    bool is_scsi =
      CurrentConnection.AccountSystem.Length() == type_template.Length();

   if (fileHandle != 0)
   {
   	Close();
   }

	char szFileName[MAXFILE+4];

 	/*if (FileExists(exportFilePath))
   {
   	fnsplit(exportFilePath.c_str(), 0, 0, szFileName, 0);
   	strcat(szFileName, ".BAK");
   	RenameFile(exportFilePath, szFileName);
   }*/

   fileHandle = FileCreate(exportFilePath);

    AnsiString header =
      "Co./Last Name, "
      "First Name, "
      "Card ID, "
      "Card Status, ";

    if (!is_scsi)
        header = header + "Currency Code, ";

    header =
      header +
      "Addr 1 - Line 1, "
      "- Line 2, "
      "- Line 3, "
      "- Line 4, "
      "- City, "
      "- State, "
      "- Postcode, "
      "- Country, "
      "- Phone # 1, "
      "- Phone # 2, "
      "- Phone # 3, "
      "- Fax #, "
      "- Email, "
      "- WWW, "
      "- Contact Name, "
      "- Salutation, "
      "Addr 2 - Line 1, "
      "- Line 2, "
      "- Line 3, "
      "- Line 4, "
      "- City, "
      "- State, "
      "- Postcode, "
      "- Country, "
      "- Phone # 1, "
      "- Phone # 2, "
      "- Phone # 3, "
      "- Fax #, "
      "- Email, "
      "- WWW, "
      "- Contact Name, "
      "- Salutation, "
      "Addr 3 - Line 1, "
      "- Line 2, "
      "- Line 3, "
      "- Line 4, "
      "- City, "
      "- State, "
      "- Postcode, "
      "- Country, "
      "- Phone # 1, "
      "- Phone # 2, "
      "- Phone # 3, "
      "- Fax #, "
      "- Email, "
      "- WWW, "
      "- Contact Name, "
      "- Salutation, "
      "Addr 4 - Line 1, "
      "- Line 2, "
      "- Line 3, "
      "- Line 4, "
      "- City, "
      "- State, "
      "- Postcode, "
      "- Country, "
      "- Phone # 1, "
      "- Phone # 2, "
      "- Phone # 3, "
      "- Fax #, "
      "- Email, "
      "- WWW, "
      "- Contact Name, "
      "- Salutation, "
      "Addr 5 - Line 1, "
      "- Line 2, "
      "- Line 3, "
      "- Line 4, "
      "- City, "
      "- State, "
      "- Postcode, "
      "- Country, "
      "- Phone # 1, "
      "- Phone # 2, "
      "- Phone # 3, "
      "- Fax #, "
      "- Email, "
      "- WWW, "
      "- Contact Name, "
      "- Salutation, "
      "Picture, "
      "Notes, "
      "Identifiers, "
      "Custom List 1, "
      "Custom List 2, "
      "Custom List 3, "
      "Custom Field 1, "
      "Custom Field 2, "
      "Custom Field 3, "
      "Billing Rate, "
      "Terms - Payment is Due, "
      "- Discount Days, "
      "- Balance Due Days, "
      "- % Discount, "
      "- % Monthly Charge, "
      "Tax Code, "
      "Credit Limit, "
      "Tax ID No., "
      "Volume Discount %, "
      "Sales/Purchase Layout, "
      "Price Level, "
      "Payment Method, "
      "Payment Notes, "
      "Name on Card, "
      "Card Number, "
      "Expiry Date, "
      "BSB, "
      "Account Number, "
      "Account Name, "
      "A.B.N. , "
      "A.B.N. Branch, "
      "Account, "
      "Salesperson, "
      "Salesperson Card ID, "
      "Comment, "
      "Shipping Method, "
      "Printed Form, "
      "Freight Tax Code, "
      "Use Customer's Tax Code, "
      "Receipt Memo, "
      "Invoice/Purchase Order Delivery, "
      "Record ID\r\n";

	FileWrite(fileHandle, header.c_str(), header.Length());
}
//---------------------------------------------------------------------------
void MYOBCustomerExporter::Export(const Accounts::Customer& const customer)
{
	std::list<AnsiString> fields;


    fields.push_back(customer.GetAccountName());
    fields.push_back("");
    fields.push_back(customer.GetAccountID());
    fields.push_back("");
    fields.push_back("");
    AnsiString address = customer.GetAddress();
    char *adr = strtok(address.c_str(), "\r\n");


    int k = 0;
    while(adr != NULL)
    {
        fields.push_back(adr);
        adr = strtok(NULL, "\r\n");
        k++;
    }

    for(;k < 4; k++)
        fields.push_back("");
        

    // Write out a Max of 4 lines of address.
/*    for (int i = 0; i < AddressList->Count && i < 4; i++)
    {
        fields.push_back(AddressList->Strings[i]);
    }

    // Fill in the Blanks if the Address is less than 4 lines.
    for (int i = AddressList->Count; i < 4; i++)
    {
        fields.push_back("");
    }
*/



    for (int i = 0; i < 4; i++)
        fields.push_back("");

    fields.push_back(customer.GetPhone());
    fields.push_back("");
    fields.push_back("");
    fields.push_back("");
    fields.push_back(customer.GetEmail());

    for (int i = 0; i < 109; i++)
        fields.push_back("");

   std::list<AnsiString>::iterator i=fields.begin();
    AnsiString line;

    line = *i++;
    while (i != fields.end())
        line = line + "," + *i++;

   line = line + "\r\n";

	FileWrite(fileHandle, line.c_str(), line.Length());
}
//---------------------------------------------------------------------------
void MYOBCustomerExporter::Close()
{
   FileClose(fileHandle);
   fileHandle = 0;
}
//---------------------------------------------------------------------------
