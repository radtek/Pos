//---------------------------------------------------------------------------
#ifndef MYOBInvoiceExporterH
#define MYOBInvoiceExporterH
//---------------------------------------------------------------------------
#include "Accounts.h"
//---------------------------------------------------------------------------
namespace Accounts
{
//---------------------------------------------------------------------------
class MYOBInvoiceExporter : public Accounts::IExporter<Invoice>
{
public:

   MYOBInvoiceExporter(const AnsiString& const filePath);

   virtual void Open();
	virtual void Export(const Accounts::Invoice& const invoice);
   virtual void Close();

private:

	const AnsiString exportFilePath;
    AnsiString prevCustomerName;
   int fileHandle;

};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
