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
struct SeniorCustomerDetails
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
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall edCustomerNameClick(TObject *Sender);
    void __fastcall edAddressClick(TObject *Sender);
    void __fastcall edTinClick(TObject *Sender);
    void __fastcall edBusinessStyleClick(TObject *Sender);
    void __fastcall edSCPWDNOClick(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmCaptureCustomerDetails(TComponent* Owner);
};
//------------------------------------------------------------------------------------------
extern PACKAGE TfrmCaptureCustomerDetails *frmCaptureCustomerDetails;
//---------------------------------------------------------------------------
#endif
