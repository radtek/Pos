//---------------------------------------------------------------------------

#ifndef SendFormH
#define SendFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchControls.h"
#include "TouchGrid.h"
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "DeviceRealTerminal.h"
#include "TouchBtn.h"
//---------------------------------------------------------------------------

enum etype{eHeldOrder, eChitOrder, eTableOrder};

class SendOrder_t
{
public:
	void SetTime(TDateTime inTime){Time = inTime;}
	TDateTime GetTime(void){return Time;}
	void SetCaption(UnicodeString inCaption){Caption = inCaption;}
	UnicodeString GetCaption(void){return Caption;}
	void SetKey(int inKey){Key = inKey;}
	int GetKey(void){return Key;}
	void SetID(UnicodeString inID){ID = inID;}
	UnicodeString GetID(void){return ID;}
	void SetStatus(etype inStatus){Status = inStatus;}
	etype GetStatus(void){return Status;}
	void SetRelatedChitKey(int chitKey){_relatedChitKey=chitKey;}
	int GetRelatedChitKey(void){return _relatedChitKey;}
	void SetRelatedTableNumber(int tableNumber){_tableNumber=tableNumber;}
	int GetRelatedTableNumber(void){return _tableNumber;}
	void SetRelatedTabName(UnicodeString tabName){_tabName=tabName;}
	UnicodeString GetRelatedTabName(void){return _tabName;}


private:
	TItemComplete *Order;
	TDateTime Time;
	UnicodeString Caption;
	int Key;
	UnicodeString ID;
	etype Status;
	int _relatedChitKey;
	int _tableNumber;
	UnicodeString _tabName;
};


class TSendFormController
{
	private:
		//std::set<int> OrderKeys;
		std::multimap<UnicodeString, int> OrderKeys;
		std::set<int> UnindexedOrderKeys;

	public:
		void LoadHeldOrders(Database::TDBTransaction &DBTransaction, std::vector<SendOrder_t> &Orders);
		//void GetOrders(int inKey);
		void AddOrder(UnicodeString Caption, int HeldOrderKey);
		void RemoveOrder(UnicodeString Caption, int HeldOrderKey);
		bool EnableOkBtn();

		//int OrderKeysSize(void){return OrderKeys.size();}
		//std::set<int>::iterator begin(void){return OrderKeys.begin();}
		//std::set<int>::iterator end(void){return OrderKeys.end();}
		std::multimap<UnicodeString, int>::iterator begin(void){return OrderKeys.begin();}
		std::multimap<UnicodeString, int>::iterator end(void){return OrderKeys.end();}
};


class TfrmSendForm : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TStaticText *StaticText1;
	TTouchGrid *TouchGrid1;
	TTouchBtn *BtnOk;
	TTouchBtn *BtnAll;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TouchGrid1Click(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnAllClick(TObject *Sender);
private:	// User declarations
	void BtnOkEnabled(void);
	void SelectAll(void);
	Database::TDBControl &DBControl;
	std::auto_ptr<TSendFormController> SendFormController;
	void DisplayOrders(std::vector<SendOrder_t> &Orders);
public:		// User declarations
	__fastcall TfrmSendForm(TComponent* Owner,  Database::TDBControl &inDBControl);
//	std::auto_ptr <TList> GetOrders(void){return Orders;}
	std::auto_ptr<TList> Orders;
	TChitNumber ChitNumber;
	std::vector<TPatronType> Patrons;
//	void Get(std::auto_ptr <TList> O){Orders = O;};
};

class TChitInfo
{
private:
	int	fChitKey;
	TChitNumber fChitNumber;
	vector<int> HeldOrderKeysList;

public:
	TChitInfo(Database::TDBTransaction &DBTransaction, int ChitKey);
	~TChitInfo();

	__property int ChitKey =
	{
		read = fChitKey
	};

	__property TChitNumber ChitNumber =
	{
		read = fChitNumber
	};

	__property vector<int> HeldOrderKeys =
	{
		read = HeldOrderKeysList
	};
};

class TTableInfo
{
private:
	int	fTableNumber;
	UnicodeString fTableName;
	TDateTime fTimeStamp;
	vector<int> HeldOrderKeysList;

public:
	TTableInfo(Database::TDBTransaction &DBTransaction, int TableNumber, UnicodeString TableName, TDateTime TimeStamp);
	~TTableInfo();

	__property int TableNumber =
	{
		read = fTableNumber
	};

	__property UnicodeString TableName =
	{
		read = fTableName
	};

	__property TDateTime TimeStamp =
	{
		read = fTimeStamp
	};

	__property vector<int> HeldOrderKeys =
	{
		read = HeldOrderKeysList
	};
};

class TTabInfo
{
private:
	UnicodeString fTabName;
	vector<int> HeldOrderKeysList;

public:
	TTabInfo(Database::TDBTransaction &DBTransaction, int TableNumber, UnicodeString TabName);
	~TTabInfo();

	__property UnicodeString TabName =
	{
		read = fTabName
    };

	__property vector<int> HeldOrderKeys =
	{
		read = HeldOrderKeysList
	};
};

class THeldOrderInfo
{
private:
	int fHeldOrderKey;
	UnicodeString fDeviceName;
	TDateTime fTimeStamp;

public:
	THeldOrderInfo(int Key, UnicodeString DeviceName, TDateTime TimeStamp);
	~THeldOrderInfo();

	__property int HeldOrderKey =
	{
		read = fHeldOrderKey
	};

	__property UnicodeString DeviceName =
	{
		read = fDeviceName
	};

	__property TDateTime TimeStamp =
	{
		read = fTimeStamp
	};
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmSendForm *frmSendForm;
//---------------------------------------------------------------------------
#endif
