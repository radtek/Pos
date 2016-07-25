//---------------------------------------------------------------------------
#ifndef AccountsH
#define AccountsH
//---------------------------------------------------------------------------
#include "Customer.h"
#include <list>
#include <set>
#include <map>
#include <memory>
//---------------------------------------------------------------------------
namespace Syncobjs { class TCriticalSection; }
//---------------------------------------------------------------------------
namespace Ibsql { class TIBSQL; }
//---------------------------------------------------------------------------
namespace Accounts
{
//---------------------------------------------------------------------------
class AccountsExporter;
class Invoice
{
public:

	Invoice(
		const AccountsExporter& exporter,
		int customerKey,
		const AnsiString& reference,
		const Currency& totalExcl,
		const Currency& totalInc,
		const TDateTime& createdDateTime,
		const TDateTime& exportedDateTime,
		const AnsiString& description);

	const System::AnsiString Reference;
	const System::Currency TotalExcl;
	const System::Currency TotalInc;
	const System::TDateTime CreatedDateTime;
	const System::TDateTime ExportedDateTime;
	const System::AnsiString Description;

	const Customer& GetCustomer() const;

	bool Exported() const;

private:

	const int CustomerKey;
	const AccountsExporter& Exporter;
};
//---------------------------------------------------------------------------
template <typename T> class IExporter
{
public:

	virtual void Open() {};
    virtual void OpenWithQuery(Ibsql::TIBSQL*q) {};
	virtual void Export(const T& const value) = 0;
	virtual void Close() {};
};
//---------------------------------------------------------------------------
class AccountsExporter
{
public:

	AccountsExporter();

   ~AccountsExporter();

   typedef std::set<Accounts::Customer> CustomerList;
   typedef void __fastcall (__closure *TUpdateEvent)(const Customer& customer, int percentComplete);
   typedef void __fastcall (__closure *TCompletedEvent)();

   void LoadInvoices(const System::TDateTime& startDateTimeFilter, const System::TDateTime& endDateTimeFilter);
   void BeginLoadCustomers(TUpdateEvent onUpdate, TCompletedEvent onComplete);

   void Export(IExporter<Invoice>& exporter);
	void Export(IExporter<Customer>& exporter);

	const Customer& EditCustomer(int key,
		const AnsiString& accountName, const AnsiString& accountID);

	const Customer& GetCustomer(int key) const;

private:
	Invoice* CreateInvoice(
		const Customer& customer,
		const AnsiString& reference,
		const Currency& totalInc,
		const Currency& totalExcl,
		const TDateTime& createdDateTime,
		const TDateTime& exportedDateTime,
        const AnsiString& description);

	void AddCustomer(const Customer& customer);
	void MarkAsExported(Ibsql::TIBSQL* query, const Invoice& invoice);

    Currency ConvertToExcGST(Currency price, double GST);

	typedef std::list<Invoice*> Invoices;
	typedef std::set<Customer> Customers;
	typedef std::map<int, Customer*> CustomerKeyMap;

	void Clear();

   Invoices _invoices;
	Customers _customers;
	CustomerKeyMap _customerKeyMap;

   TUpdateEvent OnCustomerLoaded;
   TCompletedEvent OnCustomerLoadComplete;
   void __fastcall CustomerLoaded(const Customer& customer, int percentComplete);
   void __fastcall CustomerLoadComplete();
   const std::auto_ptr<Syncobjs::TCriticalSection> criticalSection;
   class AccountsLoaderThread* thread;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
