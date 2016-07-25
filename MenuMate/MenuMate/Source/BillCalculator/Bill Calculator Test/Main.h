//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>

#include "MMBillCalculator.h"

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TPanel *Panel4;
    TPanel *pnMiddle;
    TPanel *pnInput;
    TPanel *pnSepMiddle;
    TPanel *pnOuput;
    TGroupBox *gbInput;
    TPanel *pnInputTop;
    TPanel *pnInputPrice;
    TPanel *pnInputPriceHeader;
    TPanel *pnInputPriceSep;
    TPanel *pnInputSep;
    TPanel *pnInputBottom;
    TPanel *pnInputDiscount;
    TPanel *pnInputDiscountHeader;
    TPanel *Panel8;
    TGroupBox *gbOutput;
    TPanel *pnOutputTop;
    TPanel *pnOutputAllPrices;
    TPanel *Panel9;
    TPanel *pnOutputBottom;
    TPanel *pnOutputResult;
    TPanel *pnOutputResultHeader;
    TSplitter *sptOutput;
    TPanel *pnOutputResultButtons;
    TButton *btnRefresh;
    TPanel *pnOutputAllPricesButtons;
    TButton *btnDeletePrice;
    TButton *btnEditPrice;
    TPageControl *pgcOutputResult;
    TTabSheet *tbsOutputDiscount;
    TTabSheet *tbsOutputTax;
    TTabSheet *tbsOutputServiceCharge;
    TTabSheet *tbsOutputAll;
    TButton *btnDetailResult;
    TListView *lvOuputDiscounts;
    TListView *lvOuputTaxes;
    TListView *lvOuputSC;
    TListView *lvOuputAll;
    TPanel *pnInputSC;
    TPanel *pnInputSCHeader;
    TPanel *pnInputTax;
    TPanel *pnInputTaxHeader;
    TStatusBar *StatusBar1;
    TPanel *pnInputButtons;
    TButton *btnCommitInput;
    TButton *btnNewInput;
    TPanel *pnInputDiscounBottom;
    TButton *btnEditDiscount;
    TPanel *pnInputDiscountButtons;
    TButton *btnNewDiscount;
    TButton *btnDelDiscount;
    TLabeledEdit *LabeledEdit1;
    TCheckBox *cbPriceTaxInclusive;
    TCheckBox *CheckBox1;
    TLabeledEdit *LabeledEdit2;
    TLabeledEdit *LabeledEdit3;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TPanel *pnInputDiscountTop;
    TLabeledEdit *ledDiscountName;
    TLabel *Label6;
    TLabeledEdit *ledDiscountValue;
    TLabel *Label7;
    TListView *ListView1;
    TRadioButton *rbDiscountContent;
    TRadioButton *rbDiscountPercentage;
    TButton *btnCommitDiscount;
    TPanel *Panel5;
    TButton *btnNewTax;
    TButton *btnCommitTax;
    TPanel *Panel6;
    TLabel *Label8;
    TLabel *Label9;
    TLabeledEdit *ledTaxName;
    TLabeledEdit *ledTaxValue;
    TRadioButton *rbSaleTaxType;
    TRadioButton *rbPurchaseTaxType;
    TListView *ListView2;
    TPanel *Panel7;
    TButton *btnEditTax;
    TButton *btnDelTax;
    TRadioButton *rbLocalTaxType;
    TLabel *Label10;
    TListView *ListView3;
    TListView *ListView4;
    TSplitter *Splitter1;
    TLabeledEdit *ledDiscountPriority;
    TLabel *Label11;
    TRadioGroup *rgDiscountType;
    TRadioGroup *rgDiscountMultipleCalcType;
    TRadioGroup *rgTaxMultipleCalcType;
    TMainMenu *MainMenu;
    TMenuItem *File1;
    TMenuItem *OpenItem;
    TMenuItem *NewItem;
    TMenuItem *N1;
    TMenuItem *SaveItem;
    TMenuItem *SaveAsItem;
    TMenuItem *N2;
    TMenuItem *ExitItem;
    TCheckBox *cbTaxExclusiveDiscount;
    TCheckBox *cbTaxBeforeDiscount;
    TCheckBox *CheckBox2;
    TCheckBox *CheckBox3;
    void __fastcall ExitItemClick(TObject *Sender);
    void __fastcall OpenItemClick(TObject *Sender);
    void __fastcall NewItemClick(TObject *Sender);
    void __fastcall SaveItemClick(TObject *Sender);
    void __fastcall SaveAsItemClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnCommitInputClick(TObject *Sender);

private:	// User declarations
    bool checkBeforeClosing();
    void saveProject();

	void commitDefault();
	void initDefaultPriceInfo( BillCalculator::TPriceInfo* const outPrinceInfo );
	void initDefaultTaxInfoList( BillCalculator::TAX_INFO_LIST& outTaxInfoList );
	void initDefaultDiscountInfoList( BillCalculator::DISCOUNT_INFO_LIST& outDiscountInfoList );

	void showBillCalcResult( const BillCalculator::TBillCalcResult* const inResult );
	void saveBillCalcResult(
				 UnicodeString inFileName,
		   const BillCalculator::TPriceInfo*      const inInput,
		   const BillCalculator::TBillCalcResult* const inResult );

public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
