//---------------------------------------------------------------------------


#pragma hdrstop

#include "DealLevel.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TDealLevel::TDealLevel(Currency inQty, Currency inPrice, int inDealLevelID)
{
	qty = inQty;
	price = inPrice;
	dealLevelID = inDealLevelID;
}
//---------------------------------------------------------------------------

Currency TDealLevel::GetQty() const
{
    return qty;
}

Currency TDealLevel::GetPrice() const
{
    return price;
}

int TDealLevel::GetDealLevelID() const
{
    return dealLevelID;
}

//---------------------------------------------------------------------------

void TDealLevel::SetQty(Currency inQty)
{
    qty = inQty;
}

void TDealLevel::SetPrice(Currency inPrice)
{
    price = inPrice;
}

void TDealLevel::SetDealLevelID(int inDealLevelID)
{
	dealLevelID = inDealLevelID;
}

//---------------------------------------------------------------------------

bool TDealLevel::operator<(const IDealLevel& rhs)
{
	return qty < rhs.GetQty();
}

bool TDealLevel::operator>(const IDealLevel& rhs)
{
    return qty > rhs.GetQty();
}

bool TDealLevel::operator<=(const IDealLevel& rhs)
{
	return qty <= rhs.GetQty();
}

bool TDealLevel::operator>=(const IDealLevel& rhs)
{
	return qty >= rhs.GetQty();
}

bool TDealLevel::operator==(const IDealLevel& rhs)
{
	return qty == rhs.GetQty();
}

bool TDealLevel::operator!=(const IDealLevel& rhs)
{
	return qty != rhs.GetQty();
}

//---------------------------------------------------------------------------