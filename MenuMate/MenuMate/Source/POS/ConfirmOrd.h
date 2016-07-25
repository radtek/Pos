//---------------------------------------------------------------------------
#ifndef ConfirmOrdH
#define ConfirmOrdH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "enum.h"
#include <ComCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <Grids.hpp>
#include <pngimage.hpp>
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdHTTP.hpp"
#include "IdTCPClient.hpp"
#include "IdTCPConnection.hpp"
#include <map>
#include "GlobalSettings.h"
//---------------------------------------------------------------------------
class TSaveOrdersTo
{
   public:
   TSaveOrdersTo() : PartyNameChanged(false),AllowPartyNameChanged(false) {};
	std::map<AnsiString,Variant> Location;
	typedef std::pair<AnsiString,Currency> StringValuePair;
	std::vector<StringValuePair> Values;
   bool PartyNameChanged;
   bool AllowPartyNameChanged;
};

class TfrmConfirmOrder : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlInfo;
	TLabel *lbeTitle;
   TLabel *lbTableNo;
   TLabel *lbParty;
	TPanel *pnlButtons;
	TPanel *pnlReceipt;
	TTouchBtn *btnBillUp;
	TTouchBtn *btnBillDown;
	TTouchBtn *tbtnSave;
	TTouchBtn *tbSavePrint;
	TTouchBtn *btnSaveAndBill;
	TTouchBtn *tbCancel;
	TLabel *LabelSeat;
   TTouchBtn *tbtnPartyName;
   TStringGrid *sgTotals;
	TRichEdit *tntReceipt;
	TPanel *pnlScroll;
	TTouchBtn *tchbtnScrollBillUp;
	TTouchBtn *tchbtnScrollBillDown;
    TImage *Image1;
    TIdHTTP *IdHTTP1;
    TLabel *lbTabLimit;
    TLabel *lbLimitBal;
   void __fastcall tbCancelClick(TObject *Sender);
   void __fastcall btnSaveClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
	void __fastcall tbSavePrintClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnBillDownMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnBillUpMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnSaveAndBillClick(TObject *Sender);
   void __fastcall tbtnPartyNameMouseClick(TObject *Sender);
	void __fastcall tchbtnScrollBillUpAutoRepeat(TObject *Sender);
	void __fastcall tchbtnScrollBillDownAutoRepeat(TObject *Sender);
    void __fastcall sgTotalsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
private:	// User declarations
	//__fastcall TfrmConfirmOrder(TComponent* Owner,TSaveOrdersTo &inOrderContainer);
	static TForm *WinOwner;
	void __fastcall CreateParams(Controls::TCreateParams &params);
     UnicodeString applicationDirectory;

public:		// User declarations
    __fastcall TfrmConfirmOrder(TComponent* Owner,TSaveOrdersTo &inOrderContainer);
	TfrmConfirmOrder static *Create(TForm* Owner,TSaveOrdersTo &inOrderContainer);
	TSaveOrdersTo &OrderContainer;
	TStringList *ReceiptDisplay;
};
//---------------------------------------------------------------------------
#endif

