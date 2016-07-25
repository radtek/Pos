//---------------------------------------------------------------------------


#pragma hdrstop

#include "Deal.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TDeal::TDeal()
{
	name = "";
	priority = 0;
	categoryKey = 0;
	dealID = 0;
}

TDeal::TDeal(UnicodeString inName, int inPriority, int inCategoryKey, int inDealID)
{
	name = inName;
	priority = inPriority;
    //priority = 0;
	categoryKey = inCategoryKey;
	dealID = inDealID;
}

TDeal::~TDeal()
{
	for (StoredDealLevelSet::iterator it = dealLevels.begin(); it != dealLevels.end(); it++)
	{
        delete *it;
    }
}
//---------------------------------------------------------------------------

UnicodeString TDeal::GetName()
{
	return name;
}

int TDeal::GetPriority()
{
    return priority;
    //return 0;
}

int TDeal::GetCategoryKey()
{
    return categoryKey;
}

int TDeal::GetDealID()
{
    return dealID;
}

StoredDealLevelSet TDeal::GetStoredDealLevels()
{
	return dealLevels;
}

//---------------------------------------------------------------------------

void TDeal::SetName(UnicodeString inName)
{
    name = inName;
}

void TDeal::SetPriority(int inPriority)
{
    priority = inPriority;
    //priority = 0;
}

void TDeal::SetCategoryKey(int inCategoryKey)
{
	categoryKey = inCategoryKey;
}

void TDeal::SetDealID(int inDealID)
{
    dealID = inDealID;
}

void TDeal::SetStoredDealLevels(StoredDealLevelSet inDealLevels)
{
	dealLevels = inDealLevels;
}

//---------------------------------------------------------------------------
