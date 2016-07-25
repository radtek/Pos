//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SendForm.h"
#include "DBHeldOrder.h"
#include "DBActiveChit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchBtn"
#pragma resource "*.dfm"
TfrmSendForm *frmSendForm;
//---------------------------------------------------------------------------
__fastcall TfrmSendForm::TfrmSendForm(TComponent* Owner, Database::TDBControl &inDBControl)
	: TZForm(Owner), SendFormController(new TSendFormController), DBControl(inDBControl)//, OrdersList(new TList)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSendForm::FormShow(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	std::vector<SendOrder_t> SendOrders;
	SendFormController->LoadHeldOrders(DBTransaction, SendOrders);
	DisplayOrders(SendOrders);
	Orders.reset(new TList);
	Patrons.clear();
}
//---------------------------------------------------------------------------
 void TfrmSendForm::DisplayOrders(std::vector<SendOrder_t> &Orders)
{
	const int size = Orders.size();
	TouchGrid1->RowCount = size;
	for(int i = 0; i < size; i++)
	{
		UnicodeString caption = Orders.at(i).GetCaption();
		if (Orders.at(i).GetID() != "Chit" && Orders.at(i).GetID() != "Table")
		{
			caption += " " + Orders.at(i).GetTime().TimeString();
		}
		TouchGrid1->Buttons[i][0]->Caption = caption;
		TouchGrid1->Buttons[i][0]->Tag = Orders.at(i).GetKey();
		TouchGrid1->Buttons[i][0]->Color = clGreen;
	}
}
//---------------------------------------------------------------------------

void TSendFormController::LoadHeldOrders(Database::TDBTransaction &DBTransaction, std::vector<SendOrder_t> &Orders)
{
	vector<TChitInfo> ChitOrders = TDBHeldOrder::GetChitInfoOfHeldOrders(DBTransaction);

	for(std::vector<TChitInfo>::iterator ChitInfo = ChitOrders.begin(); ChitInfo != ChitOrders.end(); ChitInfo++)
	{
		std::vector<int> HeldOrderKeys = ChitInfo->HeldOrderKeys;
		for(std::vector<int>::iterator OrderKey = HeldOrderKeys.begin(); OrderKey !=  HeldOrderKeys.end(); OrderKey++)
		{
			SendOrder_t temp;
			temp.SetKey(*OrderKey);
			temp.SetCaption(ChitInfo->ChitNumber.GetChitNumber());
			temp.SetID("Chit");
			temp.SetStatus(eChitOrder);
			temp.SetRelatedChitKey(ChitInfo->ChitKey);
			Orders.push_back(temp);
		}
	}

	vector<TTableInfo> TableOrders = TDBHeldOrder::GetTableInfoOfHeldOrders(DBTransaction);

	for(std::vector<TTableInfo>::iterator TableInfo = TableOrders.begin(); TableInfo != TableOrders.end(); TableInfo++)
	{
		std::vector<int> HeldOrderKeys = TableInfo->HeldOrderKeys;
		for(std::vector<int>::iterator OrderKey = HeldOrderKeys.begin(); OrderKey !=  HeldOrderKeys.end(); OrderKey++)
		{
			SendOrder_t temp;
			temp.SetKey(*OrderKey);
			//temp.SetTime(TableInfo->TimeStamp);
			temp.SetCaption((TableInfo->TableName));
			temp.SetID("Table");
			temp.SetStatus(eTableOrder);
			temp.SetRelatedTableNumber(TableInfo->TableNumber);
			Orders.push_back(temp);
		}
	}

	vector<TTabInfo> TabOrders = TDBHeldOrder::GetTabInfoOfHeldOrders(DBTransaction);

	for(std::vector<TTabInfo>::iterator TabInfo = TabOrders.begin(); TabInfo != TabOrders.end(); TabInfo++)
	{
		std::vector<int> HeldOrderKeys = TabInfo->HeldOrderKeys;
		for(std::vector<int>::iterator OrderKey = HeldOrderKeys.begin(); OrderKey !=  HeldOrderKeys.end(); OrderKey++)
		{
			SendOrder_t temp;
			temp.SetKey(*OrderKey);
			temp.SetCaption("Tab " + TabInfo->TabName);
			temp.SetID("Tab");
			temp.SetStatus(eTableOrder);
			temp.SetRelatedTabName(TabInfo->TabName);
			Orders.push_back(temp);
		}
	}

	vector<THeldOrderInfo> HeldOrders = TDBHeldOrder::GetKeyInfoOfUnindexedHeldOrders(DBTransaction);

	for(std::vector<THeldOrderInfo>::iterator HeldOrderInfo = HeldOrders.begin(); HeldOrderInfo != HeldOrders.end(); HeldOrderInfo++)
	{
		SendOrder_t temp;
		temp.SetKey(HeldOrderInfo->HeldOrderKey);
		temp.SetTime(HeldOrderInfo->TimeStamp);
		temp.SetCaption(HeldOrderInfo->DeviceName);
		temp.SetID("POS");
		temp.SetStatus(eHeldOrder);
		Orders.push_back(temp);
		UnindexedOrderKeys.insert(HeldOrderInfo->HeldOrderKey);
	}
}

/*
    void TSendFormController::GetOrders(int inKey)
    {
    	std::set<int>::iterator it;
		it = OrderKeys.find(inKey);
    	if(it == OrderKeys.end())
    		OrderKeys.insert(inKey);
    	else
    		OrderKeys.erase(it);
    	// This fails if a order has both a chit number and a table number and both are selected.
		// This shouldn't happen in the release but it still has to be noted as a possible issue.
	}
*/

void TSendFormController::AddOrder(UnicodeString Caption, int HeldOrderKey)
{
	OrderKeys.insert(std::pair<UnicodeString, int>(Caption,HeldOrderKey));
}

void TSendFormController::RemoveOrder(UnicodeString Caption, int HeldOrderKey)
{
	std::multimap<UnicodeString, int>::iterator it;
	for (it = OrderKeys.find(Caption); it != OrderKeys.end() && it->first == Caption; it++)
	{
		if (it->second == HeldOrderKey)
		{
			OrderKeys.erase(it);
			break;
        }
	}
}

bool TSendFormController::EnableOkBtn()
{
	bool EnableOkBtn = OrderKeys.size() > 0;
	UnicodeString LastCaption = "";
	bool KeyBeenIndexed = false;
	std::multimap<UnicodeString, int>::iterator it;
	for(it = OrderKeys.begin(); EnableOkBtn && it != OrderKeys.end(); it++)
	{
		KeyBeenIndexed = KeyBeenIndexed || UnindexedOrderKeys.find(it->second) == UnindexedOrderKeys.end();
		if (KeyBeenIndexed)
		{
			EnableOkBtn = LastCaption == "" || LastCaption == it->first;
		}
        LastCaption = it->first;
    }
	return EnableOkBtn;
}

void __fastcall TfrmSendForm::TouchGrid1Click(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, TGridButton *GridButton)
{
	if(GridButton->Latched)
	{
		SendFormController->AddOrder(GridButton->Caption, GridButton->Tag); // Add to orders here
	}
	else
	{
		SendFormController->RemoveOrder(GridButton->Caption, GridButton->Tag);				// Remove from orders here
    }
	//SendFormController->GetOrders(GridButton->Tag);
	BtnOkEnabled();
}
//---------------------------------------------------------------------------



void __fastcall TfrmSendForm::btnOkClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	std::set<int> ProcessedKeys;
	for(std::multimap<UnicodeString, int>::iterator it = SendFormController->begin(); it != SendFormController->end(); it++)
	{
		if (ProcessedKeys.find(it->second) == ProcessedKeys.end())
		{
			TDBHeldOrder::GetHeldOrdersFromKey(DBTransaction, Orders.get(), ChitNumber, Patrons, it->second);
			TDBHeldOrder::DeleteHeldOrderByKey(DBTransaction, it->second);
			ProcessedKeys.insert(it->second);
		}
	}
	DBTransaction.Commit();
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSendForm::btnAllClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void TfrmSendForm::SelectAll(void)
{
	const int size = TouchGrid1->RowCount;
	for(int i = 0; i < size; i++)
	{
		//SendFormController->GetOrders(TouchGrid1->Buttons[i][0]->Tag);
		//TouchGrid1->Buttons[i][0]->Latched = true;
	}
}
//---------------------------------------------------------------------------

void TfrmSendForm::BtnOkEnabled(void)
{
	BtnOk->Enabled = SendFormController->EnableOkBtn();
}

//---------------------------------------------------------------------------
// Support Classes
//---------------------------------------------------------------------------

TChitInfo::TChitInfo(Database::TDBTransaction &DBTransaction, int ChitKey)
{
	fChitKey = ChitKey;
	fChitNumber = TDBActiveChit::LoadChitNumberFromKey(DBTransaction, ChitKey);
	HeldOrderKeysList = TDBHeldOrder::GetHeldOrderKeysFromChitKey(DBTransaction, ChitKey);
}

TChitInfo::~TChitInfo()
{
}
//---------------------------------------------------------------------------

TTableInfo::TTableInfo(Database::TDBTransaction &DBTransaction, int TableNumber, UnicodeString TableName, TDateTime TimeStamp)
{
	fTableNumber = TableNumber;
	fTableName = TableName;
	fTimeStamp = TimeStamp;
	HeldOrderKeysList = TDBHeldOrder::GetHeldOrderKeysFromTableNumber(DBTransaction, TableNumber);
}

TTableInfo::~TTableInfo()
{
}
//---------------------------------------------------------------------------

TTabInfo::TTabInfo(Database::TDBTransaction &DBTransaction, int TabKey, UnicodeString TabName)
{
	fTabName = TabName;
	HeldOrderKeysList = TDBHeldOrder::GetHeldOrderKeysFromTabKey(DBTransaction, TabKey);
}

TTabInfo::~TTabInfo()
{
}
//---------------------------------------------------------------------------

THeldOrderInfo::THeldOrderInfo(int Key, UnicodeString DeviceName, TDateTime TimeStamp)
{
	fHeldOrderKey = Key;
	fDeviceName = DeviceName;
	fTimeStamp = TimeStamp;
}

THeldOrderInfo::~THeldOrderInfo()
{
}
