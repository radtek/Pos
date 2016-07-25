//---------------------------------------------------------------------------

#ifndef UserMaintenanceH
#define UserMaintenanceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <DB.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "SystemEvents.h"
#include "Enum.h"
//---------------------------------------------------------------------------
enum TUserMode {umStaff, umLoyalty, umDeletedStaff, umDeletedLoyalty };
//---------------------------------------------------------------------------
class TfrmUserMaintenance : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TPanel *pnlMajor;
	TPanel *Panel28;
	TPanel *pnlLabel;
   TStringGrid *sgList;
   TPanel *Panel2;
   TTouchBtn *btnAdd;
   TTouchBtn *btnEdit;
   TTouchBtn *btnDelete;
   TPanel *Panel1;
   TTouchBtn *btnUserUp;
   TTouchBtn *btnUsrDown;
	TTouchBtn *btnClose;
	void __fastcall imgExitClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall sgListDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall btnEditUserClick(TObject *Sender);
	void __fastcall btnAddUserClick(TObject *Sender);
	void __fastcall btnDeleteUserClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall btnUserUpMouseClick(TObject *Sender);
   void __fastcall btnUsrDownMouseClick(TObject *Sender);

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CardSwipe(TMessage& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange);
	  MESSAGE_HANDLER(UWM_CARDSWIPE, TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	__fastcall TfrmUserMaintenance(TComponent* Owner,Database::TDBControl &inDBControl);
	
	void LoadUserList(Database::TDBTransaction &DBTransaction);
	Database::TDBControl &DBControl;
public:		// User declarations
	void OnSmartCardInserted(TSystemEvents *Sender);
   void OnSmartCardRemoved(TSystemEvents *Sender);

	TUserMode UserMode;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
