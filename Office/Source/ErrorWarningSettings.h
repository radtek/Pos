//---------------------------------------------------------------------------

#ifndef ErrorWarningSettingsH
#define ErrorWarningSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmErrorWarningSettings : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *btnCancel;
	TBitBtn *Button2;
	TGroupBox *gbDrinksCheck;
	TCheckBox *chbDrinkRecipes;
	TCheckBox *chbDrinkNoPrice;
	TGroupBox *gbFoodCheck;
	TCheckBox *chbFoodRecipes;
	TCheckBox *chbFoodNoPrice;
	TCheckBox *chbDrinkNoSpecialPrice;
	TCheckBox *chbFoodNoSpecialPrice;
	TCheckBox *chbDrink3rdPartyError;
	TCheckBox *chbFood3rdPartyError;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmErrorWarningSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmErrorWarningSettings *frmErrorWarningSettings;
//---------------------------------------------------------------------------
#endif
