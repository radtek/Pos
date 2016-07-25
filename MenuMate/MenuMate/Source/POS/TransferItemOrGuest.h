//---------------------------------------------------------------------------

#ifndef TransferItemOrGuestH
#define TransferItemOrGuestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>

#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmTransferItemOrGuest : public TZForm
{
  friend TZForm;
__published:	// IDE-managed Components

	TPanel *Panel1;
    TPanel *Panel2;
    TTouchBtn *btnClose;
    TTouchBtn *btnOK;

    TLabel *lbeFrom;
    TLabel *lbeTo;
    TLabel *lbeItemName;
    TTouchBtn *btnDestination;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnOKMouseClick(TObject *Sender);
    void __fastcall btnCloseMouseClick(TObject *Sender);
    void __fastcall btnDestinationClick(TObject *Sender);
    TTouchBtn *btnQuantity;
    void __fastcall btnQuantityClick(TObject *Sender);


private:	// User declarations

  unsigned __int32 getMaxSeatCount();


public:		// User declarations
   __fastcall TfrmTransferItemOrGuest(TComponent* Owner);
   __fastcall ~TfrmTransferItemOrGuest();
   TList *SourceItems;
   TStringList *DestItems;
   AnsiString From; // table name
   AnsiString SourceTable;
   AnsiString DestTableName;
   AnsiString To;
   long dest_tab_key;
   bool SelectedItem;
   int index_cnt;
   int tab_key;
   UnicodeString GuestName;
   int ItmQty;
   double GetItemQty;
//---------------------------------------------------------------------------
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
