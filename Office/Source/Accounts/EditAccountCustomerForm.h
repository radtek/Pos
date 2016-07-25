//---------------------------------------------------------------------------
#ifndef EditAccountCustomerFormH
#define EditAccountCustomerFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TfrmEditAccountCustomer : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label2;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TTouchBtn *btnExit;
	TTouchBtn *btnExport;
	TEdit *edAccountName;
	TEdit *edAccountID;
	TLabel *Label3;
	TLabel *lbeName;
	TLabel *Label5;
	void __fastcall btnExitMouseClick(TObject *Sender);
	void __fastcall btnExportMouseClick(TObject *Sender);
private:	// User declarations
protected:
	__fastcall TfrmEditAccountCustomer(TComponent* Owner);
public:		// User declarations
	static bool Execute(const AnsiString& Name, AnsiString& AccountName, AnsiString& AccountID);
};
//---------------------------------------------------------------------------
#endif
