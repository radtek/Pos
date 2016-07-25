//---------------------------------------------------------------------------

#ifndef RoomConfigurationH
#define RoomConfigurationH
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
class TfrmRoomConfiguration : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel24;
	TLabel *Label4;
	TCheckBox *cbImportRoomFiles;
	TLabel *Label1;
   TTouchBtn *tbGuestMaster;
   TTouchBtn *btnOk;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall tbGuestMasterClick(TObject *Sender);
	void __fastcall cbImportRoomFilesClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmRoomConfiguration(TComponent* Owner);
public:		// User declarations

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRoomConfiguration *frmRoomConfiguration;
//---------------------------------------------------------------------------
#endif
