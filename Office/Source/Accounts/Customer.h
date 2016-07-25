//---------------------------------------------------------------------------
#ifndef CustomerH
#define CustomerH
//---------------------------------------------------------------------------
#include <System.hpp>
//---------------------------------------------------------------------------
namespace Accounts
{
//---------------------------------------------------------------------------
class Customer
{
public:

	Customer(
		int key,
   	const AnsiString& name,
      const AnsiString& memberNumber,
   	const AnsiString& accountID,
      const AnsiString& accountName,
      const AnsiString& address,
      const AnsiString& phone,
      const AnsiString& email
		);

	Customer(const Customer& customer);

   bool operator < (const Customer& rhs) const;
	void operator = (const Customer& rhs);

	int GetKey() const;
	const AnsiString& GetName() const;
	const AnsiString& GetAccountID() const;
	const AnsiString& GetAccountName() const;
   const AnsiString& GetAddress() const;
   const AnsiString& GetPhone() const;
   const AnsiString& GetEmail() const;

private:

	int Key;
	AnsiString Name;
   AnsiString MemberNumber;
	AnsiString AccountID;
	AnsiString AccountName;
   AnsiString Address;
   AnsiString Phone;
   AnsiString Email;
};
//---------------------------------------------------------------------------
} // Namespace
//---------------------------------------------------------------------------
#endif
