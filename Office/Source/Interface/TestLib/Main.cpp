//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
const AnsiString StartAdjustError	= "Unable to Start DLL adjustments";
const AnsiString AdjustError			= "Stock DLL Adjust Stock error";
const AnsiString EndAdjustError		= "Unable to committ DLL adjustments";
//---------------------------------------------------------------------------
using namespace StockInterface;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	TStockInterface StockInterface("localhost:C:\\DataBackups\\Jack Flash\\Stock.gdb");
	try
	{
		StockInterface.Initialise();
		AdjustStock(StockInterface);
		GetCosts(StockInterface);
	}
	catch(Exception &E)
	{
		Log(false, E.Message);
	}
}
//---------------------------------------------------------------------------
void TForm1::AdjustStock(TStockInterface &StockInterface)
{
	try
	{
		StockInterface.StartAdjustments();
/*		for (int i=0; i<8; i+=2)
		{
			Log(StockInterface.AdjustStock(IntToStr(i), "Bar", -1, saSale), AdjustError);
		}
		for (int i=1; i<8; i+=2)
		{
			Log(StockInterface.AdjustStock(IntToStr(i), "Bar", -1, saWriteOff), AdjustError);
		}*/
		Log(StockInterface.AdjustStock("222", "RESTAURANT", -.1234, saSale), AdjustError);
		Log(StockInterface.EndAdjustments(), EndAdjustError);
	}
	catch(Exception &E)
	{
		Log(false, E.Message);
		Log(StockInterface.RollbackAdjustments(), EndAdjustError);
	}
}
//---------------------------------------------------------------------------
void TForm1::GetCosts(TStockInterface &StockInterface)
{
	try
	{
		TStockDetails StockDetails;

		bool Eof = !StockInterface.GetFirstStockCost(StockDetails);
		while (!Eof)
		{
			SaveCost(StockDetails);
			Eof = !StockInterface.GetNextStockCost(StockDetails);
		}
	}
	catch(Exception &E)
	{
		Log(false, E.Message);
	}
}
//---------------------------------------------------------------------------
void TForm1::SaveCost(TStockDetails &StockDetails)
{
	RichEdit1->Lines->Add(AnsiString()	+
			StockDetails.StockCode			+ " " +
			StockDetails.Location			+ " " +
			StockDetails.Unit					+ " " +
			StockDetails.Description		+ " " +
			FloatToStr(StockDetails.Cost));
}
//---------------------------------------------------------------------------
inline void TForm1::Log(bool Success, AnsiString Message)
{
	if (!Success)
	{
		Application->MessageBox(Message.c_str(), "Error", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------

