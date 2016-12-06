//---------------------------------------------------------------------------

#ifndef MessageMaintenanceH
#define MessageMaintenanceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Message.h>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------


class TfrmMessageMaintenance : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel9;
	TPanel *Panel10;
	TPanel *pnlLabel;
	TStringGrid *sgDisplay;
	TTouchBtn *btnAdd;
	TTouchBtn *btnEdit;
	TTouchBtn *btnDelete;
	TTouchBtn *TouchBtn1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall imgExitClick(TObject *Sender);
	void __fastcall btnAddMessageClick(TObject *Sender);
	void __fastcall btnEditMessageClick(TObject *Sender);
	void __fastcall btnDelMessageClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	 BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	 END_MESSAGE_MAP(TForm)
private:	// User declarations
	__fastcall TfrmMessageMaintenance(TComponent* Owner,Database::TDBControl &inDBControl);
	
	Database::TDBControl &DBControl;
	void ShowMessages();
    Currency GetDenominationValue(Database::TDBTransaction &DBTransaction, Currency denominationValue);
    Currency LoadDenominations(Database::TDBTransaction &DBTransaction);
    void GetHeaders(AnsiString& CurrentCaption, AnsiString& CurrentMessage);
    void SaveDenomination(Database::TDBTransaction &DBTransaction,int key, AnsiString inTitle, Currency inValue);
public:		// User declarations

	eMessageType MessageType;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmMessageMaintenance *frmMessageMaintenance;
//---------------------------------------------------------------------------
#endif
