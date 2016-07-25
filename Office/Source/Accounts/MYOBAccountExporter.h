//---------------------------------------------------------------------------
#ifndef MYOBAccountExporterH
#define MYOBAccountExporterH
//---------------------------------------------------------------------------
#include "Accounts.h"
//---------------------------------------------------------------------------
namespace Accounts
{
//---------------------------------------------------------------------------
class MYOBCustomerExporter : public Accounts::IExporter<Customer>
{
public:

   MYOBCustomerExporter(const AnsiString& const filePath);

   virtual void Open();
	virtual void Export(const Accounts::Customer& const customer);
   virtual void Close();

private:

	const AnsiString exportFilePath;

   int fileHandle;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
