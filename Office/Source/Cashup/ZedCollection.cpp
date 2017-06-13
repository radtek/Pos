//---------------------------------------------------------------------------
#pragma hdrstop

#include "ZedCollection.h"
#include "MMData.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Cashup;
//---------------------------------------------------------------------------
ZedCollection::~ZedCollection()
{
	Clear();
}
//---------------------------------------------------------------------------
void ZedCollection::BeginLoad(const TDateTime& startDateTimeFilter,
	const TDateTime& endDateTimeFilter, ISink<Zed>& zedSink)
{
	Clear();

	Database::TDBTransaction transaction(dmMMData->dbMenuMate);

	TIBSQL* query = transaction.Query(transaction.AddQuery());

	query->SQL->Text =
		"Select "
			"Zeds.Z_Key,"
			"Zeds.Time_Stamp,"
			"Zeds.Terminal_Name,"
			"Security.From_Val as Cashier,"
			"Sum(BlindBalance.Office_Balance - BlindBalance.System_Balance) as Total_Variance "
		"From "
			"Zeds Inner Join Security on "
				"Zeds.Security_Ref = Security.Security_Ref "
			"Left Join BlindBalance on "
				"Zeds.Z_Key = BlindBalance.Z_Key "
		"Where "
			"Zeds.Time_Stamp > :StartDateTime And "
			"Zeds.Time_Stamp <= :EndDateTime And "
			"Security.SECURITY_EVENT='Till Z Off' "

		"Group By "
			"Zeds.Z_Key,"
			"Zeds.Time_Stamp,"
			"Zeds.Terminal_Name,"
			"Security.From_Val "
		"Order By "
			"Zeds.Time_Stamp";

	transaction.Start();

	query->ParamByName("StartDateTime")->AsDateTime = startDateTimeFilter;
	query->ParamByName("EndDateTime")->AsDateTime = endDateTimeFilter;

	zedSink.Start();

	for (query->ExecQuery(); !query->Eof; query->Next())
	{
		int key = query->FieldByName("Z_Key")->AsInteger;

		CreateZ(key,
			query->FieldByName("Time_Stamp")->AsDateTime,
			query->FieldByName("Terminal_Name")->AsString,
			query->FieldByName("Cashier")->AsString,
			!query->FieldByName("Total_Variance")->IsNull,
			query->FieldByName("Total_Variance")->AsCurrency);

		zedSink.Sink(*GetZed(key));
	}

	zedSink.End();

	transaction.Commit();
}
//---------------------------------------------------------------------------
void ZedCollection::Clear()
{
	std::map<int, Zed*>::const_iterator i;

	for (i=_zeds.begin(); i!=_zeds.end(); ++i)
	{
		delete i->second;
	}

	_zeds.clear();
}
//---------------------------------------------------------------------------
Zed* ZedCollection::GetZed(int zedId) const
{
	std::map<int, Zed*>::const_iterator i = _zeds.find(zedId);

	if (i != _zeds.end())
	{
		return i->second;
	}
	else
	{
		return 0;
	}
}
//---------------------------------------------------------------------------
void ZedCollection::CreateZ(
	const int id,
	const TDateTime& createdDateTime,
	const AnsiString& terminalName,
	const AnsiString& cashier,
	const bool reconciled,
	const Currency& variance)
{
	if (_zeds.find(id) == _zeds.end())
	{
		_zeds[id] = new Zed(id, createdDateTime, terminalName, cashier, reconciled, variance);
	}
}
//---------------------------------------------------------------------------

