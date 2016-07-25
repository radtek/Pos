//---------------------------------------------------------------------------

#ifndef Tables_ChitNumbersH
#define Tables_ChitNumbersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchBtn.h"
#include "DeviceRealTerminal.h"
#include "SendForm.h"
#include "DBHeldOrder.h"
//---------------------------------------------------------------------------
/*
class TableChit_t
{
public:
	void SetTime(TDateTime inTime){Time = inTime;}
	TDateTime GetTime(void){return Time;}
	void SetStaff(UnicodeString inStaff){Staff = inStaff;}
	UnicodeString GetStaff(void){return Staff;}
	void SetKey(int inKey){Key = inKey;}
	int GetKey(void){return Key;}
	void SetID(UnicodeString inID){ID = inID;}
	UnicodeString GetID(void){return ID;}

private:
	TItemComplete *Order;
	TDateTime Time;
	UnicodeString Staff;
	int Key;
	UnicodeString ID;
};  */


class TController
{
	private:
		std::set<int> OrderKeys;
	public:
		virtual void LoadHeldOrders(Database::TDBTransaction &DBTransaction, std::vector<SendOrder_t> &Orders) = 0;
		void GetOrders(int inKey);
		int OrderKeysSize(void){return OrderKeys.size();}
		std::set<int>::iterator begin(void){return OrderKeys.begin();}
		std::set<int>::iterator end(void){return OrderKeys.end();}
};


class TfrmTables_ChitNumbers : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TTouchGrid *TouchGrid1;
	TTouchGrid *TouchGrid2;
	TTouchGrid *TouchGrid3;
	TTouchBtn *BtnAll;
	TTouchBtn *BtnOk;
    TPanel *Panel3;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TouchGrid1MouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
	void __fastcall TouchGrid2MouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
	void __fastcall TouchGrid3MouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
	void __fastcall BtnOkMouseClick(TObject *Sender);
	void __fastcall BtnAllMouseClick(TObject *Sender);
private:	// User declarations
	Database::TDBControl &DBControl;
	void DisplayOrders(TTouchGrid *Grid, std::vector<SendOrder_t> &Orders, etype type);
	std::auto_ptr<TSendFormController> SendFormController;
	void DisplayOrders(std::vector<SendOrder_t> &Orders);
	void BtnOkEnabled(void);
	void SelectAll(void);

	void GridBtnPressed(TGridButton *GridButton);
public:		// User declarations
	__fastcall TfrmTables_ChitNumbers(TComponent* Owner,  Database::TDBControl &inDBControl);
	std::auto_ptr<TList> Orders;
    TChitNumber ChitNumber;
	std::vector<TPatronType> Patrons;
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmTables_ChitNumbers *frmTables_ChitNumbers;
//---------------------------------------------------------------------------
#endif
