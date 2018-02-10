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
    TTouchTime *TouchTime1;
    TTouchBtn *btnCancel;
    TTouchBtn *btnClose;
    void __fastcall btnCloseMouseClick(TObject *Sender);
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmServingTime(TComponent* Owner);
    TDateTime Time1;
    TDateTime Time2;
    eTimeType TimeType;

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmServingTime *frmServingTime;
//---------------------------------------------------------------------------
#endif
