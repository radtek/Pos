//---------------------------------------------------------------------------
// Copyright (c) 2004 IQWorks Ltd.
//
// Version 3.3.0.0 Created 23/09/2004
//
//---------------------------------------------------------------------------
// Library for interfacing between MenuMate POS and MenuMate stock control
// system.
// Two primary functions:
//   1.  Apply stock rductions for POS sales and writeoffs,
//   2.  Retrieve current stock costings.
//
//---------------------------------------------------------------------------
// e.g.
//
//	using namespace StockInterface;
//	void __fastcall TForm1::Button1Click(TObject *Sender)
//	{
//		TStockInterface StockInterface("localhost:C:\\Code\\Office\\Office 3.3.0\\Exec\\Stock.gdb");
//		try
//		{
//			StockInterface.Initialise();
//			AdjustStock(StockInterface);
//			GetCosts(StockInterface);
//		}
//		catch(Exception &E)
//		{
//			Log(false, E.Message);
//		}
//	}
//
//	void TForm1::AdjustStock(TStockInterface &StockInterface)
//	{
//		try
//		{
//			if (StockInterface.StartAdjustments())
//			{
//				Log(StockInterface.AdjustStock("12432", "Bar", -1, saSale), AdjustError);
//				Log(StockInterface.AdjustStock("12432", "Bar", -1, saWriteOff), AdjustError);
//				Log(StockInterface.EndAdjustments(), EndAdjustError);
//			}
//			else
//			{
//				Log(false, StartAdjustError);
//			}
//		}
//		catch(Exception &E)
//		{
//			Log(false, E.Message);
//			Log(StockInterface.RollbackAdjustments(), EndAdjustError);
//		}
//	}
//
//	void TForm1::GetCosts(TStockInterface &StockInterface)
//	{
//		try
//		{
//			TStockCostDetails StockCostDetails;
//
//			bool Eof = !StockInterface.GetFirstStockCost(StockCostDetails);
//			while (!Eof)
//			{
//				SaveCost(StockCostDetails);
//				Eof = !StockInterface.GetNextStockCost(StockCostDetails);
//			}
//		}
//		catch(Exception &E)
//		{
//			Log(false, E.Message);
//		}
//	}
//---------------------------------------------------------------------------
#ifndef StockInterfaceH
#define StockInterfaceH
//---------------------------------------------------------------------------
#include <system.hpp>
#include <SysUtils.hpp>
#include <memory>
//---------------------------------------------------------------------------
namespace StockInterface
{
//---------------------------------------------------------------------------
struct TStockDetails
{
	TStockDetails() : OnHand(0) {}
	AnsiString	StockCode;
	AnsiString	Unit;
	AnsiString	Location;
	AnsiString	Description;
	Currency		Cost;
	Currency		GSTPercent;
	double		OnHand;
	double		MinLevel;
	double		MaxLevel;
};

//---------------------------------------------------------------------------
enum TStockAdjustType { saSale, saWriteOff};
//---------------------------------------------------------------------------
class TvMMControls
{
public:
	virtual ~TvMMControls()=0;
};
//---------------------------------------------------------------------------
class TStockInterface
{
public:
	TStockInterface(AnsiString DatabaseName);
	~TStockInterface();

	void Initialise();

	bool StartAdjustments();
	bool AdjustStock(AnsiString StockCode, AnsiString Location, double Qty, TStockAdjustType AdjustType);
	bool EndAdjustments();
	bool RollbackAdjustments();

	bool GetFirstStockCost(TStockDetails &StockDetails);
	bool GetNextStockCost(TStockDetails &StockDetails);
	bool GetStockDetails(const AnsiString &Code ,const AnsiString &Location ,TStockDetails &StockDetails );

private:
	std::auto_ptr<TvMMControls>	fControls;	// Interface into hidden functionality.
};
//---------------------------------------------------------------------------
} // namespace StockInterface
#endif

