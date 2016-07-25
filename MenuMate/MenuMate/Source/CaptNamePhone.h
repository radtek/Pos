//---------------------------------------------------------------------------

#ifndef CaptNamePhoneH
#define CaptNamePhoneH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>

#include "ZForm.h"
#include "TouchKeyboard.h"
//---------------------------------------------------------------------------
class TfrmCaptNamePhone : public TZForm
{
__published:	// IDE-managed Components
    TEdit *edCustomerName;
    TEdit *edCustomerPhone;

    TTouchBtn *btnOk;
    TTouchBtn *TouchBtn1;
    TPanel *Panel1;
    TLabel *lblUsername;
    TLabel *lblPhoneNumber;
    void __fastcall btnOkMouseClick(TObject *Sender);
    void __fastcall TouchBtn1MouseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);


    TMMTouchKeyboard *TouchKeyboard1;
    void __fastcall TouchKeyboard1Click(TObject *Sender, char Char);

private:	// User declarations

   void CapturedAllData();
   void TextBoxValue(char valueChar);
   void PopulateTextBox(char valueChar, TEdit *edTextbox);

public:		// User declarations
    __fastcall TfrmCaptNamePhone(TComponent* Owner);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
