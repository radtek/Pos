//---------------------------------------------------------------------------
#pragma hdrstop

#include "Accounts.h"
//---------------------------------------------------------------------------
#include "MMData.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
#include <SyncObjs.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace Accounts
{
//---------------------------------------------------------------------------
class AccountsLoaderThread : public TThread
{
public:
	AccountsLoaderThread(AnsiString connectionString) :
   	dbSettings("", connectionString, true),
      TThread(true), OnUpdate(NULL), OnComplete(NULL)
   {
   	Priority = tpLower;
      FreeOnTerminate = true;
   }

   virtual __fastcall ~AccountsLoaderThread()
   {
   }

   AccountsExporter::TUpdateEvent OnUpdate;
   AccountsExporter::TCompletedEvent OnComplete;

   int totalRecords;
   int recordCount;

protected:
	void __fastcall Execute()
   {
      Database::TDBControl dbControl(dbSettings);

      dbControl.Connect();
      Database::TDBTransaction transaction(dbControl);

   	TIBSQL *query = transaction.Query(transaction.AddQuery());
      transaction.Start();

		query->SQL->Text = "Select Count(*) From Contacts Where (Contact_Type = 2 Or Contact_Type = 4)";
		query->ExecQuery();
		totalRecords = query->Fields[0]->AsInteger;
		recordCount = 0;

      query->Close();

		query->SQL->Text =
			"Select "
				"Contacts.Contacts_Key,"
				"Contacts.Name,"
				"Contacts.Member_Number,"
				"Contacts.Account_ID,"
				"Contacts.Account_Name,"
				"Contacts.Mailing_Address,"
				"Contacts.Phone,"
				"Contacts.Email "
			"From Contacts "
			"Where (Contact_Type = 2 Or Contact_Type = 4) "
			"Order By Contacts.Name";

		for (query->ExecQuery(); !query->Eof; query->Next())
      {
			Accounts::Customer customer(
				query->FieldByName("Contacts_Key")->AsInteger,
				query->FieldByName("Name")->AsString,
				query->FieldByName("Member_Number")->AsString,
				query->FieldByName("Account_ID")->AsString,
				query->FieldByName("Account_Name")->AsString,
				query->FieldByName("Mailing_Address")->AsString,
				query->FieldByName("Phone")->AsString,
				query->FieldByName("Email")->AsString
				);

         customers.insert(customer);
         currentCustomer = &customer;

      	recordCount++;
   		Synchronize(&Update);

         if (Terminated)
         {
         	break;
         }
      }

      transaction.Commit();
      dbControl.Disconnect();

   	Synchronize(&Completed);
   }
private:
	const TListView* listView;
   const Database::TDBSettings dbSettings;

   AccountsExporter::CustomerList customers;
   Customer* currentCustomer;

   void __fastcall Update()
   {
   	if (OnUpdate)
      {
      	OnUpdate(*currentCustomer, recordCount * 100 / totalRecords);
      }
   }

   void __fastcall Completed()
   {
   	if (OnComplete)
      {
      	OnComplete();
      }
   }
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
using namespace Accounts;
//---------------------------------------------------------------------------
AccountsExporter::AccountsExporter() :
   criticalSection(new TCriticalSection()),
   thread(NULL)
{
}
//---------------------------------------------------------------------------
AccountsExporter::~AccountsExporter()
{
   try
   {
   	criticalSection->Enter();
      try
      {
         if (thread != NULL)
         {
            thread->OnUpdate = NULL;
            thread->OnComplete = NULL;

         	thread->Terminate();

            thread = NULL;
         }
      }
      __finally
      {
         criticalSection->Leave();
      }

		Clear();
   }
   catch(Exception &e)
   {
   	// Ooops, shouldn't happen but logging would be good.
   }
}
//---------------------------------------------------------------------------
void AccountsExporter::Clear()
{
   try
   {
		for (Invoices::iterator i=_invoices.begin(); i!=_invoices.end(); ++i)
		{
			delete *i;
		}

		for (CustomerKeyMap::iterator i=_customerKeyMap.begin(); i!=_customerKeyMap.end(); ++i)
		{
			delete i->second;
		}
	}
   __finally
   {
		_customers.clear();
		_customerKeyMap.clear();
      _invoices.clear();
   }
}
//---------------------------------------------------------------------------
void AccountsExporter::LoadInvoices(const System::TDateTime& startDateTimeFilter,
	const System::TDateTime& endDateTimeFilter)
{
	Clear();

	Database::TDBTransaction transaction(dmMMData->dbMenuMate);

	TIBSQL* query = transaction.Query(transaction.AddQuery());

	query->SQL->Text =
		"Select "
			"Contacts.Contacts_Key,"
			"Contacts.Name,"
			"Contacts.Member_Number,"
			"Contacts.Account_ID,"
			"Contacts.Account_Name,"
			"Contacts.Mailing_Address,"
			"Contacts.Phone,"
			"Contacts.Email,"
			"Invoices.Invoice_number,"
			"Invoices.Total_Inc,"
			"Invoices.Total_Excl,"
			"Invoices.Creation_Date,"
			"Invoices.Exported_Time_Stamp,"
            "DayArchive.Item_Name,"
            "(DayArchive.Price * DayArchive.Qty + DayArchive.Discount) Price,"
            "DayArchive.GST_Percent "
		"From "
			"Invoices Left Join Contacts on "
            "Invoices.Contacts_key = Contacts.Contacts_key "
            "Inner join DayArcBill on "
            "Invoices.Invoice_Key = DayArcBill.Invoice_Key "
            "Left join DayArchive on "
            "DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key "
		"Where "
			"Invoices.Creation_Date > :CreationStartDateTime And "
			"Invoices.Creation_Date <= :CreationEndDateTime "

        "Union All "

		"Select "
			"Contacts.Contacts_Key,"
			"Contacts.Name,"
			"Contacts.Member_Number,"
			"Contacts.Account_ID,"
			"Contacts.Account_Name,"
			"Contacts.Mailing_Address,"
			"Contacts.Phone,"
			"Contacts.Email,"
			"Invoices.Invoice_number,"
			"Invoices.Total_Inc,"
			"Invoices.Total_Excl,"
			"Invoices.Creation_Date,"
			"Invoices.Exported_Time_Stamp,"
            "Archive.Item_Name,"
            "(Archive.Price * Archive.Qty + Archive.Discount) Price,"
            "Archive.GST_Percent "
		"From "
			"Invoices Left Join Contacts on "
            "Invoices.Contacts_key = Contacts.Contacts_key "
            "Inner join ArcBill on "
            "Invoices.Invoice_Key = ArcBill.Invoice_Key "
            "Left join Archive on "
            "ArcBill.ArcBill_Key = Archive.ArcBill_Key "
		"Where "
			"Invoices.Creation_Date > :CreationStartDateTime And "
			"Invoices.Creation_Date <= :CreationEndDateTime "
        "Order by 3";

	try
	{
		transaction.Start();

		query->ParamByName("CreationStartDateTime")->AsDateTime = startDateTimeFilter;
		query->ParamByName("CreationEndDateTime")->AsDateTime = endDateTimeFilter;

		AnsiString lastReference;
      Invoice* invoice = 0;

		for (query->ExecQuery(); !query->Eof; query->Next())
		{
		  //	if (lastReference != query->FieldByName("Invoice_number")->AsString || invoice == 0)
			{
			 	lastReference = query->FieldByName("Invoice_number")->AsString;

				invoice = CreateInvoice(
					Customer(
						query->FieldByName("Contacts_Key")->AsInteger,
						query->FieldByName("Name")->AsString,
						query->FieldByName("Member_Number")->AsString,
						query->FieldByName("Account_ID")->AsString,
						query->FieldByName("Account_Name")->AsString,
						query->FieldByName("Mailing_Address")->AsString,
						query->FieldByName("Phone")->AsString,
						query->FieldByName("Email")->AsString
					),
					lastReference,
                    query->FieldByName("Price")->AsCurrency,
                    ConvertToExcGST(query->FieldByName("Price")->AsCurrency, query->FieldByName("GST_Percent")->AsFloat),
					query->FieldByName("Creation_Date")->AsDateTime,
					query->FieldByName("Exported_Time_Stamp")->AsDateTime,
                    query->FieldByName("Item_Name")->AsString
					);
			}
		}

		transaction.Commit();
	}
	catch (Exception &E)
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
Currency AccountsExporter::ConvertToExcGST(Currency price, double GST)
{
    return (price / (GST/100.0 + 1));
}
//---------------------------------------------------------------------------
void AccountsExporter::BeginLoadCustomers(TUpdateEvent onUpdate, TCompletedEvent onComplete)
{
	criticalSection->Enter();

   try
   {
      if (thread == NULL)
      {
         OnCustomerLoaded = onUpdate;
         OnCustomerLoadComplete = onComplete;

         thread = new AccountsLoaderThread(dmMMData->dbMenuMate->DatabaseName);

         thread->OnUpdate = CustomerLoaded;
         thread->OnComplete = CustomerLoadComplete;

         thread->Resume();
      }
   }
   __finally
   {
      criticalSection->Leave();
   }
}
//---------------------------------------------------------------------------
void __fastcall AccountsExporter::CustomerLoaded(const Customer& customer, int percentComplete)
{
	AddCustomer(customer);
	if (OnCustomerLoaded) OnCustomerLoaded(customer, percentComplete);
}
//---------------------------------------------------------------------------
void AccountsExporter::AddCustomer(const Customer& customer)
{
	if (_customerKeyMap.find(customer.GetKey()) == _customerKeyMap.end())
	{
		Customer* newCustomer = new Customer(customer);
		_customerKeyMap[customer.GetKey()] = newCustomer;
		_customers.insert(*newCustomer);
	}
}
//---------------------------------------------------------------------------
void __fastcall AccountsExporter::CustomerLoadComplete()
{
   criticalSection->Enter();
   try
   {
     	thread = NULL;
   }
   __finally
   {
      criticalSection->Leave();
   }

	if (OnCustomerLoadComplete) OnCustomerLoadComplete();
}
//---------------------------------------------------------------------------
const Customer& AccountsExporter::EditCustomer(int key, const AnsiString& accountName, const AnsiString& accountID)
{
	Database::TDBTransaction transaction(dmMMData->dbMenuMate);

	TIBSQL* query = transaction.Query(transaction.AddQuery());

	query->SQL->Text =
		"Update Contacts Set "
			"Account_Name = :Account_Name,"
			"Account_ID = :Account_ID "
		"Where "
			"Contacts_Key = :Contacts_Key";

	try
	{
		transaction.Start();

		Customer* customer = _customerKeyMap[key];

		query->ParamByName("Contacts_Key")->AsInteger = key;
		query->ParamByName("Account_Name")->AsString = accountName;
		query->ParamByName("Account_ID")->AsString = accountID;

		query->ExecQuery();

		if (query->RowsAffected != 1)
		{
			throw Exception("Customer not found for update");
		}

		transaction.Commit();

		transaction.Start();

		query->SQL->Text =
			"Select "
				"Name,"
				"Member_Number,"
				"Account_ID,"
				"Account_Name,"
				"Mailing_Address,"
				"Phone,"
				"Email "
			"From "
				"Contacts "
			"Where "
				"(Contact_Type = 2 Or Contact_Type = 4) And "
				"Contacts_Key = :Contacts_Key";

		query->ParamByName("Contacts_Key")->AsInteger = key;
		query->ExecQuery();

		_customerKeyMap[key] = new Accounts::Customer(
			key,
			query->FieldByName("Name")->AsString,
			query->FieldByName("Member_Number")->AsString,
			query->FieldByName("Account_ID")->AsString,
			query->FieldByName("Account_Name")->AsString,
			query->FieldByName("Mailing_Address")->AsString,
			query->FieldByName("Phone")->AsString,
			query->FieldByName("Email")->AsString
			);

		_customers.erase(*customer);
		_customers.insert(*_customerKeyMap[key]);

		delete customer;

		transaction.Commit();

		return *_customerKeyMap[key];
	}
	catch (Exception &E)
	{
		transaction.Rollback();
		throw;
	}
}
//---------------------------------------------------------------------------
const Customer& AccountsExporter::GetCustomer(int key) const
{
	CustomerKeyMap::const_iterator i = _customerKeyMap.find(key);

	return *(i->second);
}
//---------------------------------------------------------------------------
void AccountsExporter::Export(IExporter<Invoice>& exporter)
{
	Database::TDBTransaction transaction(dmMMData->dbMenuMate);
	TIBSQL* query = transaction.Query(transaction.AddQuery());

	transaction.Start();

        if (CurrentConnection.AccountSystem == "Xero") {
                exporter.OpenWithQuery(query);
        }
        else {
                exporter.Open();
        }


	for (Invoices::const_iterator i=_invoices.begin(); i!=_invoices.end(); ++i) {
                exporter.Export(**i);
                MarkAsExported(query, **i);
        }

	exporter.Close();
	transaction.Commit();
}
//---------------------------------------------------------------------------
void AccountsExporter::MarkAsExported(TIBSQL* query, const Invoice& invoice)
{
	if (query->SQL->Text == "")
	{
		query->SQL->Text = "Update Invoices Set Exported_Time_Stamp = current_timestamp Where Invoice_number = :InvoiceNumber";
	}

	query->ParamByName("InvoiceNumber")->AsString = invoice.Reference;
	query->ExecQuery();
}
//---------------------------------------------------------------------------
void AccountsExporter::Export(IExporter<Customer>& exporter)
{
	exporter.Open();

	for (AccountsExporter::Customers::const_iterator i=_customers.begin(); i!=_customers.end(); ++i)
   {
		exporter.Export(*i);
   }

   exporter.Close();
}
//---------------------------------------------------------------------------
Invoice* AccountsExporter::CreateInvoice(
	const Customer& customer,
	const AnsiString& reference,
	const Currency& totalInc,
	const Currency& totalExcl,
	const TDateTime& createdDateTime,
	const TDateTime& exportedDateTime,
    const AnsiString& description)
{
	Invoice* invoice = new Invoice(*this, customer.GetKey(), reference, totalInc, totalExcl, createdDateTime, exportedDateTime, description);

	_invoices.push_back(invoice);

	this->AddCustomer(customer);

   return invoice;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
Invoice::Invoice(
		const AccountsExporter& exporter,
		int customerKey,
		const AnsiString& reference,
		const Currency& totalInc,
		const Currency& totalExcl,
		const TDateTime& createdDateTime,
		const TDateTime& exportedDateTime,
		const AnsiString& description) :

	Exporter(exporter),
	CustomerKey(customerKey),
	Reference(reference),
	TotalInc(totalInc),
   TotalExcl(totalExcl),
	CreatedDateTime(createdDateTime),
	ExportedDateTime(exportedDateTime),
	Description(description)
{
}
//---------------------------------------------------------------------------
const Customer& Invoice::GetCustomer() const
{
	return Exporter.GetCustomer(CustomerKey);
}
//---------------------------------------------------------------------------
bool Invoice::Exported() const
{
	return ExportedDateTime != TDateTime();
}
//---------------------------------------------------------------------------

