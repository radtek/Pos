//---------------------------------------------------------------------------

#ifndef ScalesH
#define ScalesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ZForm.h"
#include "WeighScale.h"
#include "Weight.h"
#include "SystemEvents.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ItemMinorComplete.h"
#include "PoleDisplay.h"
#include "MMTouchNumpad.h"
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------

enum eEntryMode
{
	amount, weight
};

class TfrmScales : public TZForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TPanel *Panel2;
   TTouchBtn *tbtnOk;
   TTouchBtn *tbtnCancel;
    TTouchBtn *tbtnAmount;
    TTouchBtn *tbtnWeight;
   TPanel *Panel3;
   TLabel *Label2;
   TLabel *lbUnitPrice;
   TPanel *Panel4;
   TLabel *Label4;
   TLabel *lbTotalPrice;
   TLabel *lbItemName;
   TLabel *lbPackagingWeight;
   TLabel *Label5;
   TLabel *Label3;
   TLabel *lbScalesWeight;
   TLabel *WeightLabel;
   TLabel *Label1;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall tbtnOkMouseClick(TObject *Sender);
   void __fastcall tbtnCancelMouseClick(TObject *Sender);
    void __fastcall tbtnAmountMouseClick(TObject *Sender);
    void __fastcall tbtnWeightMouseClick(TObject *Sender);
private:	// User declarations
   bool AutoConvertScalesWeight;
   TWeighScale *Scales;
   TPoleDisplay *PoleDisplay;
   TWeight &Weight;
   TWeight &TareWeight;
   TItemMinorComplete *Item;
   void OnWeightChanged(TSystemEvents *Sender);
   void OnStabilityChanged(TSystemEvents *Sender);
   bool SetWeight(Currency weight, eEntryMode entryMode);
   std::auto_ptr <TfrmTouchNumpad> OpenTouchNumPad(AnsiString caption, TPadMode padMode);
public:		// User declarations
   __fastcall TfrmScales(TComponent* Owner,TWeighScale *inScales,TPoleDisplay *inPoleDisplay,TWeight &inWeight,TWeight &inTareWeight,bool AutoConvertScalesWeight,TItemMinorComplete *inItem);
   __fastcall virtual ~TfrmScales();
   TModalResult GetWeight();
   Currency UnitPrice;
   void CalcTotal();
   bool IsWeighed;

};
//---------------------------------------------------------------------------
#endif
