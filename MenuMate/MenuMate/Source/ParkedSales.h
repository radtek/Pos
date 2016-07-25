//---------------------------------------------------------------------------

#ifndef ParkedSalesH
#define ParkedSalesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ZForm.h"
#include "ManagerParkedSales.h"


//---------------------------------------------------------------------------
class TfrmParkedSales : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *pnlParkedSales;
	TScrollBox *sbSales;
	TTouchBtn *btnSalesUp;
	TTouchBtn *btnSeatsDown;
	TStaticText *lbeTitle;
   TTouchBtn *TouchBtn1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall tbCloseClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall pnlParkedSalesResize(TObject *Sender);
	void __fastcall btnSeatsDownMouseClick(TObject *Sender);
	void __fastcall btnSalesUpMouseClick(TObject *Sender);
private:	// User declarations
	void __fastcall TouchButtonSaleClick(TObject *Sender);
public:		// User declarations
	__fastcall TfrmParkedSales(TComponent* Owner);
   std::auto_ptr<TManagerParkedSales> ParkedSales;
   int SelectedParkedSaleTabKey;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmParkedSales *frmParkedSales;
//---------------------------------------------------------------------------
#endif
