//---------------------------------------------------------------------------

#ifndef PHSConfigurationH
#define PHSConfigurationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
enum ePMS { phoenix = 1, siHot};
class TfrmPHSConfiguration : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel24;
	TLabel *Label4;
   TTouchBtn *tbPhoenixIPAddress;
	TPanel *Panel2;
   TTouchBtn *tbPhoenixPortNumber;
   TTouchBtn *tbPhoenixID;
   TTouchBtn *tbPaymentDefCat;
   TTouchBtn *tbItemDefCat;
   TTouchBtn *tbPointCat;
   TTouchBtn *tbCreditCat;
   TTouchBtn *tbDefTransAccount;
   TTouchBtn *tbSurchargeCat;
   TTouchBtn *tbRoundingCategory;
   TTouchBtn *TouchBtn1;
   TTouchBtn *btnOk;
   TTouchBtn *tbTipAccount;
   TTouchBtn *tbExpensesAccount;
   TTouchBtn *tbServiceCharge;
   TTouchBtn *tbLoyaltyPurchase;
	void __fastcall tbPhoenixIPAddressClick(TObject *Sender);
	void __fastcall tbPhoenixPortNumberClick(TObject *Sender);
	void __fastcall tbPhoenixIDClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall tbPaymentDefCatClick(TObject *Sender);
	void __fastcall tbItemDefCatClick(TObject *Sender);
	void __fastcall tbPointCatClick(TObject *Sender);
	void __fastcall tbCreditCatClick(TObject *Sender);
	void __fastcall tbDefTransAccountClick(TObject *Sender);
	void __fastcall tbSurchargeCatClick(TObject *Sender);
	void __fastcall tbRoundingCategoryClick(TObject *Sender);
   void __fastcall TouchBtn1MouseClick(TObject *Sender);
    void __fastcall tbTipAccountClick(TObject *Sender);
    void __fastcall tbExpensesAccountClick(TObject *Sender);
    void __fastcall tbServiceChargeAutoRepeat(TObject *Sender);
    void __fastcall tbLoyaltyPurchaseAutoRepeat(TObject *Sender);
    void __fastcall tbLoyaltyPurchaseMouseClick(TObject *Sender);
    void __fastcall tbServiceChargeMouseClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmPHSConfiguration(TComponent* Owner);
   void UpdateGUI();
   void InitializePMS();
public:		// User declarations
   ePMS PMSType;

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPHSConfiguration *frmPHSConfiguration;
//---------------------------------------------------------------------------
#endif
