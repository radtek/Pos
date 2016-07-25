//---------------------------------------------------------------------------
// Copyright (c) 2004 MenuMate Ltd.
//
// Version 3.3.0.0 Created 23/09/2004
//
//---------------------------------------------------------------------------
#pragma hdrstop
#include "StockInterface.h"
#include "MM_StockTransactions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Database;
//---------------------------------------------------------------------------
namespace StockInterface
{
	TvMMControls::~TvMMControls() {};
//---------------------------------------------------------------------------
	class TMMControls : public TvMMControls
	{
	public:
		TMMControls(const AnsiString &DatabaseName) :
				fDBControl(TDBSettings("",DatabaseName,true)),
				fDBTransaction(fDBControl),
				sqlStockCosts(new TIBSQL(NULL)),
				sqlOnHand(new TIBSQL(NULL)),
                sqlStockItem(new TIBSQL(NULL)),
				fStockSaleControl(NULL)
		{}

		virtual ~TMMControls() { delete fStockSaleControl; }

		Database::TDBControl			fDBControl;
		Database::TDBTransaction	fDBTransaction;
		Stock::TStockSale				*fStockSaleControl;

		Database::TcpIBSQL			sqlStockCosts;
		Database::TcpIBSQL			sqlOnHand;
        ///
        Database::TcpIBSQL          sqlStockItem;

		void PopulateStockDetails(TStockDetails &StockDetails, TcpIBSQL &sqlStock );
		bool fGetStockCost(TStockDetails &StockDetails);

	};
}
//---------------------------------------------------------------------------
using namespace StockInterface;
//---------------------------------------------------------------------------
TStockInterface::TStockInterface(AnsiString DatabaseName) :
		fControls(new TMMControls(DatabaseName))
{
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		Controls->fDBTransaction.RegisterQuery(Controls->sqlStockCosts);
		Controls->sqlStockCosts->SQL->Text =
			"Select "
				"Stock.Code,"
				"Stock.Stocktake_Unit,"
				"Stock.Description,"
				"Stock.GST_Percent,"
				"StockLocation.Location,"
				"StockLocation.Latest_Cost,"
				"StockLocation.Average_Cost,"
				"StockLocation.On_Hand, "
				"StockLocation.Min_Level, "
				"StockLocation.Max_Level "
			"From "
				"Stock inner join StockLocation on "
					"Stock.Stock_Key = StockLocation.Stock_Key "
			"Order By "
				"Code,"
				"Location";
		Controls->fDBTransaction.RegisterQuery(Controls->sqlOnHand);
		Controls->sqlOnHand->SQL->Text =
			"Select "
				"Stock.Code,"
				"Stock.Stocktake_Unit,"
				"Stock.Description,"
				"Stock.GST_Percent,"
				"StockLocation.Location,"
				"StockLocation.Latest_Cost,"
				"StockLocation.Average_Cost,"
				"StockLocation.On_Hand, "
				"StockLocation.Min_Level, "
				"StockLocation.Max_Level "
			"From "
				"Stock inner join StockLocation on "
					"Stock.Stock_Key = StockLocation.Stock_Key "
			"Where "
				"Stock.Code = :Code and "
				"StockLocation.Location = :Location";
		Controls->fDBTransaction.RegisterQuery(Controls->sqlStockItem);
		Controls->sqlStockItem->SQL->Text =
			"Select "
				"Stock.STOCK_KEY "
			"From "
				"Stock ";
	}
}
//---------------------------------------------------------------------------
TStockInterface::~TStockInterface()
{
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		try
		{
			Controls->fDBTransaction.CommitTransaction();
		}
		catch (...)
		{
		}
		if (Controls->fStockSaleControl)
		{
			try
			{
				Controls->fStockSaleControl->Close(false);
				delete Controls->fStockSaleControl;
			}
			catch (...)
			{
			}
			Controls->fStockSaleControl = NULL;
		}
	}
}
//---------------------------------------------------------------------------
void TStockInterface::Initialise()
{
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		Controls->fDBControl.Connect();
	}
}
//---------------------------------------------------------------------------
bool TStockInterface::StartAdjustments()
{
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		if (Controls->fStockSaleControl)
		{
			return false;
		}
		else
		{
			Controls->fStockSaleControl = new Stock::TStockSale(Controls->fDBControl);
			return true;
		}
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStockInterface::EndAdjustments()
{
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		if (!Controls->fStockSaleControl)
		{
			return false;
		}
		else
		{
			bool Success = Controls->fStockSaleControl->Close(true);
			try
			{
				delete Controls->fStockSaleControl;
			}
			__finally
			{
				Controls->fStockSaleControl = NULL;
			}
			return Success;
		}
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStockInterface::RollbackAdjustments()
{
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		if (!Controls->fStockSaleControl)
		{
			return false;
		}
		else
		{
			bool Success = Controls->fStockSaleControl->Close(false);
			try
			{
				delete Controls->fStockSaleControl;
			}
			__finally
			{
				Controls->fStockSaleControl = NULL;
			}
			return Success;
		}
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStockInterface::AdjustStock(AnsiString StockCode, AnsiString Location, double Qty, TStockAdjustType AdjustType, AnsiString Operator, AnsiString Reason, AnsiString DeviceName)
{
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		Stock::TSaleItemInfo 			SaleItemInfo;
		SaleItemInfo.StockCode			= StockCode;
		SaleItemInfo.Location			= Location;
		SaleItemInfo.Qty					= Qty;
        SaleItemInfo.DeviceName         = DeviceName;

		switch(AdjustType)
		{
			case saSale:		SaleItemInfo.AdjustmentType = Stock::ttSale;			break;
			case saWriteOff:	SaleItemInfo.AdjustmentType = Stock::ttWriteoff;	break;
            case saRefund:	       SaleItemInfo.AdjustmentType = Stock::ttRefund; 	break;
			default:				return false;
		}
	  return Controls->fStockSaleControl->AdjustStock(SaleItemInfo, Operator, Reason);
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStockInterface::GetFirstStockCost(TStockDetails &StockDetails)
{
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		Controls->fDBTransaction.StartTransaction();
		Controls->sqlStockCosts->Close();
		Controls->sqlStockCosts->ExecQuery();

		return Controls->fGetStockCost(StockDetails);
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStockInterface::CheckStockItem()
{
    bool returnVal = false;
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		Controls->fDBTransaction.StartTransaction();
		Controls->sqlStockItem->Close();
		Controls->sqlStockItem->ExecQuery();

        int key = Controls->sqlStockItem->FieldByName("STOCK_KEY")->AsInteger;

        if(key > 0)
           returnVal = true;
        else
           returnVal = false;

     }
     return returnVal;
}
//---------------------------------------------------------------------------
bool TStockInterface::GetNextStockCost(TStockDetails &StockDetails)
{
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		if (!Controls->sqlStockCosts->Open)
		{
			return false;
		}
		Controls->sqlStockCosts->Next();

		return Controls->fGetStockCost(StockDetails);
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool TStockInterface::GetStockDetails(const AnsiString &Code ,const AnsiString &Location ,TStockDetails &StockDetails )
{
	TMMControls *Controls = dynamic_cast<TMMControls*>(fControls.get());
	if (Controls)
	{
		Controls->fDBTransaction.StartTransaction();
		Controls->sqlOnHand->Close();
		Controls->sqlOnHand->ParamByName("Code")->AsString = Code;
		Controls->sqlOnHand->ParamByName("Location")->AsString = Location;
		Controls->sqlOnHand->ExecQuery();
		if (Controls->sqlOnHand->Eof)
		{
			StockDetails = TStockDetails();
			return false;
		}
		else
		{
			Controls->PopulateStockDetails(StockDetails,Controls->sqlOnHand);
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
//  TMMControls Classes
//---------------------------------------------------------------------------
void TMMControls::PopulateStockDetails(TStockDetails &StockDetails, TcpIBSQL &sqlStock )
{
	StockDetails.StockCode		= sqlStock->FieldByName("CODE")->AsString;
	StockDetails.Unit		    	= sqlStock->FieldByName("STOCKTAKE_UNIT")->AsString;
	StockDetails.Description	= sqlStock->FieldByName("DESCRIPTION")->AsString;
	StockDetails.Location		= sqlStock->FieldByName("LOCATION")->AsString;
	StockDetails.Cost			   = sqlStock->FieldByName("Average_Cost")->AsCurrency;
	StockDetails.GSTPercent	   = sqlStock->FieldByName("GST_Percent")->AsCurrency;
	StockDetails.OnHand        = sqlStock->FieldByName("ON_HAND")->AsDouble;
	StockDetails.MinLevel      = sqlStock->FieldByName("MIN_LEVEL")->AsDouble;
	StockDetails.MaxLevel      = sqlStock->FieldByName("MAX_LEVEL")->AsDouble;
}
//---------------------------------------------------------------------------
bool TMMControls::fGetStockCost(TStockDetails &StockDetails)
{
	if (sqlStockCosts->Eof)
	{
		StockDetails = TStockDetails();
		return false;
	}
	else
	{
		PopulateStockDetails(StockDetails,sqlStockCosts);
		return true;
	}
}
//---------------------------------------------------------------------------

