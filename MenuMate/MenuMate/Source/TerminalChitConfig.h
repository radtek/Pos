//---------------------------------------------------------------------------

#ifndef TerminalChitConfigH
#define TerminalChitConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <ExtCtrls.hpp>
#include "MM_DBCore.h"
#include "ZForm.h"
#include "typedefChits.h"

#include "ManagerChitNumber.h"

//---------------------------------------------------------------------------
class TfrmTerminalChitConfig : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnl1;
	TPanel *pnl2;
	TTouchGrid *tgridDeviceList;
	TPanel *pnl3;
	TPanel *pnl4;
	TTouchGrid *tgridProfileList;
	TPanel *pnl5;
	TPanel *pnlButtons;
	TTouchBtn *tbtnOk;
	void __fastcall tbtnOkMouseClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall tgridDeviceListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall tgridProfileListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);



protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	 BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	 END_MESSAGE_MAP(TForm)
private:	// User declarations
	Database::TDBTransaction &DBTransaction;
	void UpdateDeviceDisplay();
	void UpdateProfileListDisplay();
	void SaveDeviceChitProfiles();
public:		// User declarations
	__fastcall TfrmTerminalChitConfig(TComponent* Owner,Database::TDBTransaction &inDBTransaction);
	__int64 CurrentDeviceKey;

 	DeviceChitsList DevicesChitProfile; // Device Key /  typChitDefaultPairList
};
//---------------------------------------------------------------------------
#endif
