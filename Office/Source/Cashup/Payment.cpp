//---------------------------------------------------------------------------
#pragma hdrstop

#include "Payment.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Cashup;
//---------------------------------------------------------------------------
Payment::Payment() :
	_id(0),
	_quantity(0),
	_calculated(0),
	_counted(0)
{
}
//---------------------------------------------------------------------------
Payment::Payment(const int id, const AnsiString& type, const int group,
		const int quantity, const Currency& amount, const Currency& counted) :

	_id(id),
    _type(type),
	_group(group),
	_quantity(quantity),
	_calculated(amount),
	_counted(counted)
{
}
//---------------------------------------------------------------------------
int Payment::GetId() const
{
	return _id;
}
//---------------------------------------------------------------------------
const AnsiString& Payment::GetType() const
{
	return _type;
}
//---------------------------------------------------------------------------
int Payment::GetGroup() const
{
	return _group;
}
//---------------------------------------------------------------------------
int Payment::GetQuantity() const
{
	return _quantity;
}
//---------------------------------------------------------------------------
const Currency& Payment::GetCalculatedValue() const
{
	return _calculated;
}
//---------------------------------------------------------------------------
const Currency& Payment::GetCountedValue() const
{
	return _counted;
}
//---------------------------------------------------------------------------
Currency Payment::GetVariance() const
{
	return _calculated - _counted;
}
//---------------------------------------------------------------------------
void Payment::SetCountedValue(const Currency& value)
{
	_counted = value;
}
//---------------------------------------------------------------------------
void Payment::operator = (const Payment& rhs)
{
	_id = rhs._id;
	_type = rhs._type;
	_group = rhs._group;
	_quantity = rhs._quantity;
	_calculated = rhs._calculated;
	_counted = rhs._counted;
}
//---------------------------------------------------------------------------
bool Payment::operator == (const Payment& rhs) const
{
	return _id == rhs._id;
}
//---------------------------------------------------------------------------
bool Payment::operator < (const Payment& rhs) const
{
	return _type < rhs._type;
}
//---------------------------------------------------------------------------
