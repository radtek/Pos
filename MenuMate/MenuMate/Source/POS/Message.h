// ---------------------------------------------------------------------------

#ifndef MessageH
#define MessageH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "MM_DBCore.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"

// ---------------------------------------------------------------------------
enum eMessageType
{
   eMessage, eCancelReason, eCreditReason, eDiscountReason,
   eRunProgram, eCallAway, ePatronTypes, eSkimRefloat, eWriteOff, eCustomerTypes,ePaymentTypeDisplay,eHHProfile,eThorDiscountReason,
   eCashDrawer, eCashDenomination, eRevenueCodes, eServingTimes
};

enum eBtnControlType
{
   eBtnNormal, eMultiSelect, eSelectionComplete
};

class TfrmMessage : public TZForm
{
   friend TZForm;
__published: // IDE-managed Components
   TPanel *pnlCourses;
   TScrollBox *sbMessages;
   TTouchBtn *btnMsgUp;
   TTouchBtn *btnMsgDown;

   void __fastcall FormShow(TObject *Sender);
   void __fastcall BtnMesasgeClick(TObject *Sender);
   void __fastcall BtnCloseClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall pnlCoursesResize(TObject *Sender);
   void __fastcall btnMsgDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall btnMsgUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

protected:
   void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)END_MESSAGE_MAP(TComponent)

private: // User declarations
   __fastcall TfrmMessage(TComponent* Owner, Database::TDBControl &inIBDatabase);

   Database::TDBControl &DBControl;
   void DisplayPaymentList(Database::TDBTransaction &DBTransaction,int cordX,int cordY,int ButtonWidth,int PixelGap);

   void HHProfileList(Database::TDBTransaction &DBTransaction,int cordX,int cordY,int ButtonWidth,int PixelGap);

public: // User declarations

   AnsiString TextResult;
   AnsiString TextSelected;
   // Selected Button Caption.
   std::auto_ptr <TStringList> TextResults;
   bool allow_combo;
   eMessageType MessageType;
   long Key;
   bool ShowPointsAsDiscount;
};

class TMessageBtn : public TTouchBtn
{
private:
public:
   __fastcall TMessageBtn(Classes::TComponent * AOwner);
   AnsiString Title;
   AnsiString TextResult;
   eBtnControlType BtnType;
   bool ShowKeyboard;
   bool Selected;
};

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#endif
