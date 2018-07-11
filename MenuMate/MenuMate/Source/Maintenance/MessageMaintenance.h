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
#include "ManagerPMSCodes.h"
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
    void __fastcall sgDisplayDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
    void __fastcall sgDisplaySelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);


protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	 BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	 END_MESSAGE_MAP(TForm)
private:	// User declarations
	__fastcall TfrmMessageMaintenance(TComponent* Owner,Database::TDBControl &inDBControl);
	int SelectedRow;
	Database::TDBControl &DBControl;
	void ShowMessages();
    Currency GetDenominationValue(Database::TDBTransaction &DBTransaction, Currency denominationValue);
    Currency LoadDenominations(Database::TDBTransaction &DBTransaction);
    void GetHeaders(AnsiString& CurrentCaption, AnsiString& CurrentMessage);
    void SaveDenomination(Database::TDBTransaction &DBTransaction,int key, AnsiString inTitle, Currency inValue);
    bool CheckDefaultPatronTypes();
    void LoadRevenueCodes(Database::TDBTransaction &DBTransaction);
    void AddMessage(TObject *Sender);
    void AddRevenueCode(TObject *Sender);
    void UpdateRevenueCode(Database::TDBTransaction &DBTransaction, int key);
    TManagerPMSCodes* managerPMSCodes;
    void AddServingTime(TObject *Sender);
    bool ValidateTimeSlot(TDateTime time, int key);
    void LoadServingDetails(Database::TDBTransaction &DBTransaction);
    void InsertMealSlotToDB(TTimeSlots slots);
    void UpdateMealDetails(Database::TDBTransaction &DBTransaction, int key);
    void LoadPMSPaymentTypes(Database::TDBTransaction &DBTransaction);
    void AddPMSPaymentType(TObject *Sender);
    void UpdatePMSPaymentType(Database::TDBTransaction &DBTransaction, int key) ;
    bool ValidatePMSPayTypeDeletion(Database::TDBTransaction &DBTransaction, int key);
    bool IsNotDuplicatePMSPayName(AnsiString name,bool forUpdate,int key);
public:		// User declarations

	eMessageType MessageType;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmMessageMaintenance *frmMessageMaintenance;
//---------------------------------------------------------------------------
#endif
