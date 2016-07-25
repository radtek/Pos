//---------------------------------------------------------------------------

#ifndef ChefMateSettingsH
#define ChefMateSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include "ZForm.h"

//---------------------------------------------------------------------------

#define _DEFAULT_CM_ACTIVE  true
#define _DEFAULT_CM_IP_ADDR "localhost"
#define _DEFAULT_CM_IP_PORT 56001
#define _DEFAULT_CM_DYNAMIC false

//---------------------------------------------------------------------------

class TfrmChefmateSettings : public TZForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TBevel *Bevel4;
	TBevel *Bevel3;
	TBevel *Bevel2;
	TPanel *Panel1;
	TTouchBtn *btnOK;
	TTouchBtn *btnCancel;
	TBevel *Bevel5;
	TGroupBox *GroupBox1;
	TLabel *lbIPAddr;
	TLabel *lbIPPort;
	TEdit *edIPAddr;
	TEdit *edIPPort;
	TLabel *lbDynamic;
	TGroupBox *GroupBox2;
	TLabel *lbActive;
	TLabel *Label3;
	TRadioButton *rbXeroRemote;
	TCheckBox *chkbActive;
	TCheckBox *chkbDynamic;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOKMouseClick(TObject *Sender);
	void __fastcall btnCancelMouseClick(TObject *Sender);
	void __fastcall edIPAddrClick(TObject *Sender);
	void __fastcall edIPPortClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall chkbActiveClick(TObject *Sender);
	void __fastcall chkbDynamicClick(TObject *Sender);

protected:
	bool          _getActive();
	UnicodeString _getIPAddr();
	unsigned int  _getIPPort();
	bool          _getDynamic();

	void _setActive(  bool inActive );
	void _setIPAddr(  UnicodeString inIPAddr );
	void _setIPPort(  UnicodeString inIPPort );
	void _setDynamic( bool inDynamic );

public:		// User declarations
	__fastcall TfrmChefmateSettings(TComponent* Owner);

	__property bool          Active  = { read = _getActive,  write = _setActive };
	__property UnicodeString IPAddr  = { read = _getIPAddr,  write = _setIPAddr };
	__property __int32       IPPort  = { read = _getIPPort,  write = _setIPPort };
	__property bool          Dynamic = { read = _getDynamic, write = _setDynamic };

	void Set(
			bool       inActive,
			AnsiString inIPAddr,
			__int32    inIPPort,
			bool       inDynamic );

private:	// User declarations
	bool          FValidated;
	bool          FActive;
	UnicodeString FIPAddr;
	unsigned int  FIPPort;
	bool          FDynamic;

	void initSettings();
	void refreshSettings();

	void enableAndClearEdit(
			bool           inEnabled,
			TEdit*         inEdit,
			UnicodeString  inText );

	UnicodeString checkIPAddr(    UnicodeString inIPAddr );
	UnicodeString checkIPPortStr( UnicodeString inIPPortStr );
	unsigned int  checkIPPort(    unsigned int inIPPort );

	bool editText(    UnicodeString inCaption, UnicodeString &outText );
	bool editNumText( UnicodeString inCaption, UnicodeString &outText );
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
