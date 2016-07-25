//---------------------------------------------------------------------------
#pragma hdrstop

#include "MM_StockCore.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Stock;
//---------------------------------------------------------------------------
TStockControl::TStockControl(TIBDatabase *IBDatabase) : fDBTransaction(IBDatabase),
			sqlFindLocation(			new TIBSQL(NULL)),
			sqlAddLocation(			new TIBSQL(NULL)),
			sqlGetOrderItemDetails(	new TIBSQL(NULL)),
			sqlGetStockLocations(	new TIBSQL(NULL))
{
	fDBTransaction.RegisterQuery(sqlFindLocation);
	fDBTransaction.RegisterQuery(sqlAddLocation);
	fDBTransaction.RegisterQuery(sqlGetOrderItemDetails);
	fDBTransaction.RegisterQuery(sqlGetStockLocations);

	sqlFindLocation->SQL->Text =
		"Select "
			"Stock_Key "
		"From "
			"StockLocation "
		"Where "
			"Stock_Key = :Stock_Key And "
			"Location = :Location";

	sqlAddLocation->SQL->Text =
		"Insert Into StockLocation ("
			"Stock_Key,"
			"Location,"

			"Max_Level,"
			"Min_Level,"
			"On_Hand,"
			"Average_Cost,"
			"Latest_Cost,"

			"Initialised,"
			"Stocktake,"
			"Variance "
			")"
		"Values ("
			":Stock_Key,"
			":Location, "

			":Max_Level,"
			":Min_Level,"
			":On_Hand,"
			":Average_Cost,"
			":Latest_Cost,"

			"'F',"
			"0,"
			"0 "
			")";

	sqlGetOrderItemDetails->SQL->Text =
		"Select "
			"Supplier_Unit_Qty,"
			"Supplier_Unit_Size,"
			"Qty_Received,"
			"Qty_On_Order "
		"From "
			"PurchaseStock "
		"Where "
			"Purchase_Stock_LK = :Purchase_Stock_Key";

	sqlGetStockLocations->SQL->Text =
		"Select Distinct "
			"Location "
		"From "
			"StockLocation "
		"Order By "
			"Location";
}
//---------------------------------------------------------------------------
/*TStockControl::~TStockControl()
{
}*/
//---------------------------------------------------------------------------
bool TStockControl::CreateLocations(std::set<int> const& StockKeys, AnsiString Location)
{
	fDBTransaction.StartTransaction();
	try
	{
		std::set<int>::iterator iStockKeys;
		for (iStockKeys = StockKeys.begin(); iStockKeys != StockKeys.end(); iStockKeys++)
		{
			fCreateStockLocation(*iStockKeys, Location);
		}
		fDBTransaction.CommitTransaction();
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TStockControl::GetLocations(std::set<AnsiString>& Locations)
{
	fDBTransaction.StartTransaction();
	try
	{
		sqlGetStockLocations->Close();
		for (sqlGetStockLocations->ExecQuery(); !sqlGetStockLocations->Eof; sqlGetStockLocations->Next())
		{
			Locations.insert(sqlGetStockLocations->FieldByName("Location")->AsString);
		}
		fDBTransaction.CommitTransaction();
	}
	catch (Exception &E)
	{
		fDBTransaction.UndoTransaction();
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
void TStockControl::fCreateStockLocation(int StockKey, AnsiString Location)
{
	sqlFindLocation->Close();
	sqlFindLocation->ParamByName("Stock_Key")->AsInteger	= StockKey;
	sqlFindLocation->ParamByName("Location")->AsString		= Location.UpperCase();
	sqlFindLocation->ExecQuery();
	if (sqlFindLocation->Eof)
	{
		sqlAddLocation->Close();
		sqlAddLocation->ParamByName("Stock_Key")->AsInteger	= StockKey;
		sqlAddLocation->ParamByName("Location")->AsString		= Location;
		sqlAddLocation->ParamByName("Max_Level")->AsDouble		= 0;
		sqlAddLocation->ParamByName("Min_Level")->AsDouble		= 0;
		sqlAddLocation->ParamByName("On_Hand")->AsDouble		= 0;
		sqlAddLocation->ParamByName("Average_Cost")->AsDouble	= 0;
		sqlAddLocation->ParamByName("Latest_Cost")->AsDouble	= 0;
		sqlAddLocation->ExecQuery();
	}
	sqlFindLocation->Close();
} 
//---------------------------------------------------------------------------

