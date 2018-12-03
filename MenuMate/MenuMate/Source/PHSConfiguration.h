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
enum ePMS { phoenix = 1, siHot, oracle, mews};
enum eVertSel {Outlet = 1, Service, AccountingCategory };
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
    TLabel *Label1;
    TTouchBtn *tbRevenueCodes;
    TTouchBtn *tbServingTime;
    TTouchBtn *tbRevenueCentre;
    TCheckBox *cbEnableCustomerJourney;
    TCheckBox *cbMakeOracleServer;
    TLabel *Label2;
    TTouchBtn *tbOracleInterfaceIP;
    TTouchBtn *tbOracleInterfacePort;
    TTouchBtn *tbTimeOut;
    TLabel *lblNoTaxToSihot;
    TCheckBox *cbNoTaxToSihot;
    TLabel *lblEnableItemDetailsPosting;
    TCheckBox *cbEnableItemDetailsPosting;
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
    void __fastcall tbServiceChargeMouseClick(TObject *Sender);
    void __fastcall cbEnableCustomerJourneyClick(TObject *Sender);
    void __fastcall tbRevenueCodesClick(TObject *Sender);
    void __fastcall tbServingTimeMouseClick(TObject *Sender);
    void __fastcall tbRevenueCentreMouseClick(TObject *Sender);
    void __fastcall cbMakePOSServer(TObject *Sender);
    void __fastcall tbOracleInterfacePortMouseClick(TObject *Sender);
    void __fastcall tbOracleInterfaceIPMouseClick(TObject *Sender);
    void __fastcall tbTimeOutMouseClick(TObject *Sender);
    void __fastcall cbNoTaxToSihotClick(TObject *Sender);
    void __fastcall cbEnableItemDetailsPostingClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmPHSConfiguration(TComponent* Owner);
   void UpdateGUI();
   void InitializePMS();
   void LogPMSEnabling();
   bool CanEnablePOSServer();
   void InitializePMSDefaultPayment();
   bool DefaultPaymentInitRequired();
   void InitDefaultPaymentInDB();
   AnsiString GetDropDownResult(eVertSel selectionType);
   UnicodeString ShowKeyBoard(int maxLength,UnicodeString value,UnicodeString caption);
   int ShowNumPad(UnicodeString caption,int value);
   void SyncMewsDetailsFromCloud();
   AnsiString ShowVerticalSelection(eVertSel selectionType);
   AnsiString GetMewsName(AnsiString code,eVertSel selectionType);
public:		// User declarations
   ePMS PMSType;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPHSConfiguration *frmPHSConfiguration;
//---------------------------------------------------------------------------
#endif
