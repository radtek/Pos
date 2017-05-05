//---------------------------------------------------------------------------

#ifndef NewPaymentTypeH
#define NewPaymentTypeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "CGRID.h"
#include "ListPaymentSystem.h"
#include "MM_DBCore.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ComCtrls.hpp>
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmNewPaymentType : public TZForm
{

	friend TZForm;
__published:	// IDE-managed Components
        TPanel *pnlButtons;
        TTouchBtn *tbPayment;
        TTouchBtn *tbSecurity;
        TTouchBtn *tbInterfaces;
        TTouchBtn *tbtnOk;
        TPageControl *Pages;
        TTabSheet *tsPayment;
        TPanel *Panel1;
        TLabel *Label5;
        TLabel *Label1;
        TCColorGrid *ccgColour;
        TTouchBtn *tbExchange;
        TGroupBox *GroupBox3;
        TCheckBox *cbSurcharge;
        TCheckBox *cbIsTip;
        TTouchBtn *tbSurchargeTaxRate;
        TGroupBox *GroupBox4;
        TCheckBox *cbTaxFree;
        TTouchBtn *tbGroupNumber;
        TGroupBox *GroupBox5;
        TCheckBox *cbIsCash;
        TCheckBox *cbOpendrawer;
        TGroupBox *GroupBox6;
        TTouchBtn *btnName;
        TTouchBtn *tbPosition;
        TTouchBtn *tbRounding;
        TPanel *Panel10;
        TTabSheet *tsSecurity;
        TTabSheet *tsInterfaces;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *Panel4;
        TPanel *Panel5;
        TGroupBox *GroupBox7;
        TCheckBox *cbCashOut;
        TCheckBox *cbIntegrated;
        TCheckBox *cbAllowReversal;
        TCheckBox *cbElectronicTransaction;
        TCheckBox *tbChequeVerify;
        TCheckBox *cbAllowTips;
        TGroupBox *GroupBox8;
        TCheckBox *cbCheckAccepted;
        TCheckBox *cbCheckSig;
        TGroupBox *GroupBox2;
        TCheckBox *cbCSVPaymentType;
//        TCheckBox *tbInvoiceInterface;
        TTouchBtn *TouchBtn2;
        TGroupBox *GroupBox1;
        TCheckBox *cbSec1;
        TCheckBox *cbSec2;
        TCheckBox *cbSec3;
        TCheckBox *cbReqNote;
        TCheckBox *cbAllowManPan;
        TGroupBox *GroupBox9;
        TTouchBtn *tbtnSecondaryPMSPort;
        TTouchBtn *tbtnSecondaryIPAddress;
        TTouchBtn *tbThirdPartyID;
        TCheckBox *tbRoomPayment;
        TCheckBox *cbGetVoucherDetails;
        TCheckBox *cbGetCardDetails;
        TGroupBox *GroupBox10;
        TCheckBox *cbPocketVoucher;
        //TTouchBtn *tbtnFixedVoucherCode;
        TTouchBtn *tbtnVoucherMerchant;
        TCheckBox *cbPVAcceptedMsg;
        TCheckBox *CheckBoxExport;
        TTouchBtn *tbtnUniUser;
        TTouchBtn *tbtnUniPass;
        TCheckBox *tbChargeToXero;
        TBevel *Bevel1;
        TGroupBox *GroupBox11;
        TTouchBtn *tbRMSWriteLocation;
        TTouchBtn *tbRMSReadLocation;
        TCheckBox *cbRMSInterface;
        TTouchBtn *tbTabLink;
        TTouchBtn *tbGLCode;
        TCheckBox *cbAutoPopulateBlindBalance;
    TGroupBox *grpPaymentWallet;
    TCheckBox *cbWalletPayments;
    TTouchBtn *btnWalletType;
    TTouchBtn *btnWalletConfig;
        void __fastcall pnlOkClick(TObject *Sender);
        void __fastcall Panel20Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall btnNameClick(TObject *Sender);
        void __fastcall ccgColourChange(TObject *Sender);
        void __fastcall tbPositionClick(TObject *Sender);
        void __fastcall tbExchangeClick(TObject *Sender);
        void __fastcall cbSurchargeClick(TObject *Sender);
        void __fastcall cbIsTipClick(TObject *Sender);
        void __fastcall cbCashOutClick(TObject *Sender);
        void __fastcall tbRoomPaymentClick(TObject *Sender);
        void __fastcall tbGroupNumberClick(TObject *Sender);
        void __fastcall cbIsCashClick(TObject *Sender);
        void __fastcall cbIntegratedClick(TObject *Sender);
        void __fastcall tbChequeVerifyClick(TObject *Sender);
        void __fastcall cbElectronicTransactionClick(TObject *Sender);
        void __fastcall cbAllowReversalClick(TObject *Sender);
        void __fastcall tbThirdPartyIDClick(TObject *Sender);
        void __fastcall tbSurchargeTaxRateClick(TObject *Sender);
        void __fastcall tbRoundingMouseClick(TObject *Sender);
//        void __fastcall tbInvoiceInterfaceClick(TObject *Sender);
        void __fastcall tbPaymentMouseClick(TObject *Sender);
        void __fastcall tbSecurityMouseClick(TObject *Sender);
        void __fastcall tbInterfacesMouseClick(TObject *Sender);
        void __fastcall tbtnSecondaryIPAddressClick(TObject *Sender);
        void __fastcall tbtnSecondaryPMSPortClick(TObject *Sender);
        //void __fastcall tbtnFixedVoucherCodeMouseClick(TObject *Sender);
        void __fastcall tbtnVoucherMerchantMouseClick(TObject *Sender);
        void __fastcall cbGetVoucherDetailsClick(TObject *Sender);
        void __fastcall cbPocketVoucherClick(TObject *Sender);
        void __fastcall ExportMouseClick(TObject *Sender);
        void __fastcall tbtnUniUserMouseClick(TObject *Sender);
        void __fastcall tbtnUniPassMouseClick(TObject *Sender);
        void __fastcall tbChargeToXeroClick(TObject *Sender);
        void __fastcall cbRMSInterfaceClick(TObject *Sender);
        void __fastcall tbRMSReadLocationMouseClick(TObject *Sender);
        void __fastcall tbRMSWriteLocationMouseClick(TObject *Sender);
        void __fastcall tbTabLinkMouseClick(TObject *Sender);
        void __fastcall tbGLCodeMouseClick(TObject *Sender);
    void __fastcall btnWalletConfigMouseClick(TObject *Sender);
    void __fastcall btnWalletTypeMouseClick(TObject *Sender);
    void __fastcall cbWalletPaymentsClick(TObject *Sender);

private:	// User declarations
	__fastcall TfrmNewPaymentType(TComponent* Owner,Database::TDBControl &inDBControl,TListPaymentSystem *inPaymentSystem,int inPaymentKey = 0);
	static TForm *WinOwner;
	void __fastcall CreateParams(Controls::TCreateParams &params);
	void GetTabDetailDetails(Database::TDBTransaction &DBTransaction);
	Database::TDBControl &DBControl;
	TListPaymentSystem *PaymentSystem;
	TPayment Payment;
	void RedrawButtons(TObject * Sender);
    AnsiString GLCode;
public:		// User declarations
    static TfrmNewPaymentType *Create(TForm* Owner,Database::TDBControl &inDBControl,TListPaymentSystem *inPaymentSystem,int inPaymentKey);

    bool Editing;
    int PaymentKey;
    int PaymentPos;
    int PaymentGroup;
    AnsiString PaymentThirdPartyID;
    AnsiString VoucherMerchantID;
    AnsiString SecondaryPMSIPAddress;
    AnsiString UniUser;
    AnsiString UniPass;
    int SecondaryPMSPortNumber;
    double ExchangeRate;
    AnsiString Reason;
    double SurchargeAmount;
    double TaxRate;
    Currency RoundTo;
    bool SurchargeIsAPercentAdjust;
    bool Export;
    UnicodeString CVSReadLocation;
    UnicodeString CVSWriteLocation;
    TMMTabType  CurrentDestTabType;
    int TabKey;
    AnsiString TabName;
};
//---------------------------------------------------------------------------
#endif
