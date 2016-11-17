//---------------------------------------------------------------------------

#ifndef CaptureDiscountDetailsH
#define CaptureDiscountDetailsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmCaptureDiscountDetails : public TZForm
{
__published:	// IDE-managed Components
    TPanel *PanelDetails;
    TGroupBox *GroupBox1;
    TEdit *edCustomerName;
    TLabel *LabelCustomerName;
    TLabel *LabelAddress;
    TEdit *edAddress;
    TLabel *LabelTIN;
    TEdit *edTIN;
    TLabel *LabelBusinessStyle;
    TEdit *edBusinessStyle;
    TLabel *LabelSCPWDNO;
    TEdit *edSCPWDNO;
    TTouchBtn *btnOK;
    TTouchBtn *btnCancel;
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmCaptureDiscountDetails(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCaptureDiscountDetails *frmCaptureDiscountDetails;
//---------------------------------------------------------------------------
#endif
