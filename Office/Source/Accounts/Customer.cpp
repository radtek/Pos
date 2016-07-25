//---------------------------------------------------------------------------
#pragma hdrstop

#include "Customer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Accounts;
//---------------------------------------------------------------------------
Customer::Customer(
		int key,
   	const AnsiString& name,
   	const AnsiString& memberNumber,
   	const AnsiString& accountID,
      const AnsiString& accountName,
      const AnsiString& address,
      const AnsiString& phone,
      const AnsiString& email) :

	Key(key),
	Name(name),
	MemberNumber(memberNumber),
	AccountID(accountID),
	AccountName(accountName),
	Address(address),
	Phone(phone),
	Email(email)
{
}
//---------------------------------------------------------------------------
Customer::Customer(const Customer& customer)
{
	operator = (customer);
}
//---------------------------------------------------------------------------
int Customer::GetKey() const
{
	return Key;
}
//---------------------------------------------------------------------------
const AnsiString& Customer::GetName() const
{
	return Name;
}
//---------------------------------------------------------------------------
const AnsiString& Customer::GetAccountID() const
{
	if (AccountID == "") return MemberNumber;
   else return AccountID;
}
//---------------------------------------------------------------------------
const AnsiString& Customer::GetAccountName() const
{
	if (AccountName == "") return Name;
   else return AccountName;
}
//---------------------------------------------------------------------------
const AnsiString& Customer::GetAddress() const
{
    return Address;
}
//---------------------------------------------------------------------------
const AnsiString& Customer::GetPhone() const
{
	return Phone;
}
//---------------------------------------------------------------------------
const AnsiString& Customer::GetEmail() const
{
	return Email;
}
//---------------------------------------------------------------------------
bool Customer::operator < (const Customer& rhs) const
{
	if (GetAccountID() == rhs.GetAccountID()) return false;
	if (GetKey() == rhs.GetKey()) return false;

   return (GetAccountName() < rhs.GetAccountName());
}
//---------------------------------------------------------------------------
void Customer::operator = (const Customer& rhs)
{
	Name = rhs.Name;
	MemberNumber  = rhs.MemberNumber;
	AccountID = rhs.AccountID;
   AccountName = rhs.AccountName;
   Address = rhs.Address;
   Phone = rhs.Phone;
   Email = rhs.Email;

}
//---------------------------------------------------------------------------

