//---------------------------------------------------------------------------

#ifndef SetupPurchaseOrdersH
#define SetupPurchaseOrdersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmSetupPurchaseOrders : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel4;
	TLabel *Label14;
	TLabel *Label16;
	TPanel *Panel5;
	TLabel *lbeTitle;
	TMemo *memInstructions;
	TMemo *memDeliveryAddress;
	TBitBtn *btnSave;
	TBitBtn *btnCancel;
	TLabel *Label1;
	TMemo *memCompanyDetails;
	TLabel *Label2;
	TBevel *Bevel1;
	TLabel *Label3;
	TBevel *Bevel2;
	TLabel *Label4;
    TCheckBox *cbShowItemCost;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall cbShowItemCostClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSetupPurchaseOrders(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSetupPurchaseOrders *frmSetupPurchaseOrders;
//---------------------------------------------------------------------------
#endif
