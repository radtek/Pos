//---------------------------------------------------------------------------

#ifndef SelectCompanyH
#define SelectCompanyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "CSPIN.h"
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmSelectCompany : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TLabel *Label6;
	TPanel *Panel5;
	TBitBtn *BitBtn5;
	TBitBtn *BitBtn2;
	TBitBtn *btnCancel;
	TPanel *Panel6;
	TComboBox *cbCompanyNames;
	TLabel *Label1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
private:	// User declarations
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)
public:		// User declarations
	__fastcall TfrmSelectCompany(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectCompany *frmSelectCompany;
//---------------------------------------------------------------------------
#endif
