//---------------------------------------------------------------------------

#ifndef ServingTimeH
#define ServingTimeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchTime.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
enum eTimeType{startTime, endTime};
class TfrmServingTime : public TZForm
{
__published:	// IDE-managed Components
    TPanel *pnlMain;
    //TTouchTime *TouchTime1;
    TTouchBtn *BtnServingTimeHour;
    TTouchBtn *BtnServingTimeMinute;
    TTouchBtn *BtnServingTimeSecond;
    TGroupBox *GroupBox1;
    TTouchBtn *btnCancel;
    TTouchBtn *btnClose;
    TLabel *lbeHour;
    TLabel *lbeMinute;
    TLabel *lbeSecond;
    void __fastcall btnCloseMouseClick(TObject *Sender);
   void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall BtnServingTimeHourMouseClick(TObject *Sender);
    void __fastcall BtnServingTimeMinuteMouseClick(TObject *Sender);
    void __fastcall BtnServingTimeSecondMouseClick(TObject *Sender);
private:	// User declarations
     bool ValidateData(UnicodeString &Title , int &value);
     void SetZeroToTime();
public:		// User declarations
    __fastcall TfrmServingTime(TComponent* Owner);
    TDateTime Time1;
    TDateTime Time2;
    eTimeType TimeType;
    int value;

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmServingTime *frmServingTime;
//---------------------------------------------------------------------------
#endif
