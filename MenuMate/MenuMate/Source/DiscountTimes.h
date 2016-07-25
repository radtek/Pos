//---------------------------------------------------------------------------

#ifndef DiscountTimesH
#define DiscountTimesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchTime.h"
#include <ExtCtrls.hpp>
#include "ZForm.h"
//---------------------------------------------------------------------------
#include "ManagerDiscount.h"


class TfrmDiscountTimes : public TZForm
{
__published:	// IDE-managed Components
   TPanel *Panel2;
   TTouchBtn *TouchBtn1;
   TPanel *Panel1;
   TPanel *Panel3;
   TGroupBox *GroupBox1;
   TTouchBtn *btnDelete;
   TPanel *Panel28;
   TPanel *pnlLabel;
   TTouchGrid *tgridSelection;
   TPanel *Panel4;
   TTouchGrid *tgDay;
   TPanel *Panel5;
   TPanel *Panel7;
   TTouchGrid *tgYear;
   TTouchGrid *tgMonth;
   TTouchGrid *TouchGridButtons;
   TTouchTime *TouchTime;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall tgMonthMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton);
   void __fastcall TouchGridButtonsMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall TouchBtn1MouseClick(TObject *Sender);
   void __fastcall btnDeleteMouseClick(TObject *Sender);
   void __fastcall tgridSelectionMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
private:	      // User declarations
   void DrawCalendar();
   void __fastcall AddDiscountTime();
   void __fastcall UpdateDiscountList();      
public:		   // User declarations
   __fastcall TfrmDiscountTimes(TComponent* Owner,Database::TDBControl &inDBControl);
	Database::TDBControl &DBControl;
   int DiscountKey;
   TDiscountTimes DiscountTimes;
};

//---------------------------------------------------------------------------
#endif
