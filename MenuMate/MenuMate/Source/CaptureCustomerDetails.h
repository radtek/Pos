//---------------------------------------------------------------------------

#ifndef CaptureCustomerDetailsH
#define CaptureCustomerDetailsH
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
struct SCDPWDCustomerDetails
{
    UnicodeString CustomerName;
    UnicodeString Address;
    UnicodeString TinNo;
    UnicodeString BusinessStyle;
    UnicodeString SC_PWD_ID;
};
//---------------------------------------------------------------------------
class TfrmCaptureCustomerDetails : public TZForm
{
    friend TZForm;
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
    void __fastcall CaptureCustomerDetails(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
    UnicodeString CustomerInfoPointers[5];
    void DisplayCustomerData();
public:		// User declarations
    __fastcall TfrmCaptureCustomerDetails(TComponent* Owner);
    SCDPWDCustomerDetails customerDetails;
};
//------------------------------------------------------------------------------------------
#endif
