//---------------------------------------------------------------------------

#ifndef IntaMateConfigurationH
#define IntaMateConfigurationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmIntaMateConfiguration : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel24;
   TTouchBtn *tbPAddress;
	TPanel *Panel2;
   TTouchBtn *tbPortNumber;
   TTouchBtn *tbPOSID;
   TTouchBtn *btnOk;
	TTouchBtn *tbTimeOuts;
	TPanel *pnlLabel;
	void __fastcall tbPhoenixIPAddressClick(TObject *Sender);
	void __fastcall tbPhoenixPortNumberClick(TObject *Sender);
	void __fastcall tbPhoenixIDClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall tbTimeOutsMouseClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmIntaMateConfiguration(TComponent* Owner);
   void UpdateGUI();   
public:		// User declarations
   int POSID;
   AnsiString TCPIPAddress;
	int TCPPort;
	int TCPTimeOut_ms;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmIntaMateConfiguration *frmIntaMateConfiguration;
//---------------------------------------------------------------------------
#endif
