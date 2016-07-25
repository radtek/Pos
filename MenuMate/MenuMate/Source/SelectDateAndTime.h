//---------------------------------------------------------------------------

#ifndef SelectDateAndTimeH
#define SelectDateAndTimeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "TouchTime.h"

//---------------------------------------------------------------------------
class TfrmSelectDateTime : public TForm
{
__published:	// IDE-managed Components
    TTouchBtn *btnOk;
    TTouchBtn *tbtnCancel;
    TPanel *Panel1;
    TLabel *Label1;
    TTouchBtn *BtnDate;
    TLabel *Label2;
    TTouchBtn *BtnMonth;
    TTouchBtn *BtnYear;
    TLabel *Label3;
    TTouchTime *TimeDisplay;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnOkMouseClick(TObject *Sender);
    void __fastcall tbtnCancelMouseClick(TObject *Sender);
    void __fastcall BtnDateMouseClick(TObject *Sender);
    void __fastcall BtnMonthMouseClick(TObject *Sender);
    void __fastcall BtnYearMouseClick(TObject *Sender);
    //
    TTouchBtn *BtnHour;
    TTouchBtn *BtnMinute;
    TTouchBtn *BtnAMPM;
    TLabel *LblHour;
    TLabel *LblMinute;
    TLabel *LblAMPM;
    void __fastcall BtnHourMouseClick(TObject *Sender);
    void __fastcall BtnMinuteMouseClick(TObject *Sender);
    void __fastcall BtnAMPMMouseClick(TObject *Sender);

private:	// User declarations
    TDateTime DeliveryDate;
    bool ValidateData(AnsiString &Title, int &value);

public:		// User declarations
   //   __fastcall TfrmSelectDateTime(TComponent* Owner);
	__fastcall TfrmSelectDateTime(TComponent* Owner);
    UnicodeString PopUpDatePicker(AnsiString Title, UnicodeString &value);
        TDateTime SelectedTime;
        bool IsCancelButtonVisible;
        int MinutesToAdd;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
