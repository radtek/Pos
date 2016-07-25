//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "StockInterface.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TRichEdit *RichEdit1;
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	void AdjustStock(StockInterface::TStockInterface &StockInterface);
	void GetCosts(StockInterface::TStockInterface &StockInterface);
	void SaveCost(StockInterface::TStockDetails &StockDetails);
	inline void Log(bool Success, AnsiString Message);
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
