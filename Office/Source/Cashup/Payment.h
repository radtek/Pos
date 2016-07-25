//---------------------------------------------------------------------------
#ifndef PaymentH
#define PaymentH
//---------------------------------------------------------------------------
#include <system.hpp>
//---------------------------------------------------------------------------
namespace Cashup
{
//---------------------------------------------------------------------------
class Payment
{
public:
	Payment();

	Payment(const int id, const AnsiString& type, const int group,
		const int quantity, const Currency& amount, const Currency& counted);

	int GetId() const;
	const AnsiString& GetType() const;
	int GetGroup() const;
	int GetQuantity() const;
	const Currency& GetCalculatedValue() const;
	const Currency& GetCountedValue() const;

	Currency GetVariance() const;

	void SetCountedValue(const Currency& value);

	void operator = (const Payment& rhs);
	bool operator == (const Payment& rhs) const;
	bool operator < (const Payment& rhs) const;

private:

	int _id;
	AnsiString _type;
	int _group;
	int _quantity;
	Currency _calculated;

	Currency _counted;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
