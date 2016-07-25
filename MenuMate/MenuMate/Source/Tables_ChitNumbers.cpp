//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Tables_ChitNumbers.h"
#include "DBHeldOrder.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchBtn"
#pragma resource "*.dfm"
TfrmTables_ChitNumbers *frmTables_ChitNumbers;
//---------------------------------------------------------------------------
__fastcall TfrmTables_ChitNumbers::TfrmTables_ChitNumbers(TComponent* Owner, Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl), SendFormController(new TSendFormController)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmTables_ChitNumbers::FormShow(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	std::vector<SendOrder_t> SendOrders;
	SendFormController->LoadHeldOrders(DBTransaction, SendOrders);
	DisplayOrders(TouchGrid1, SendOrders, eTableOrder);
	DisplayOrders(TouchGrid2, SendOrders, eChitOrder);
	DisplayOrders(TouchGrid3, SendOrders, eHeldOrder);
	Orders.reset(new TList);
	Patrons.clear();
}
//---------------------------------------------------------------------------

void TfrmTables_ChitNumbers::DisplayOrders(std::vector<SendOrder_t> &Orders)
{
	const int size = Orders.size();
	TouchGrid1->ColCount = size;
	for(int i = 0; i < size; i++)
	{
		const AnsiString caption = Orders.at(i).GetCaption() + " " + Orders.at(i).GetTime().TimeString();
		TouchGrid1->Buttons[0][i]->Caption = caption;
		TouchGrid1->Buttons[0][i]->Tag = Orders.at(i).GetKey();
		TouchGrid1->Buttons[0][i]->Color = clGreen;
	}
}

void TfrmTables_ChitNumbers::DisplayOrders(TTouchGrid *Grid, std::vector<SendOrder_t> &Orders, etype type)
{
	std::vector<SendOrder_t> temp;
	std::vector<SendOrder_t>::iterator it = Orders.begin();
	for(it; it != Orders.end(); it++)
	{
		if(it->GetStatus() == type)
			temp.push_back(*it);
	}

	const int size = temp.size();
	Grid->ColCount = (size + 1) / 2;
	for(int i = 0; i < size; i++)
	{
		AnsiString caption = temp.at(i).GetCaption();
		if (type == eHeldOrder)
		{
			caption += " " + temp.at(i).GetTime().TimeString();
		}
		Grid->Buttons[i%2][i/2]->Caption 			= caption;
		Grid->Buttons[i%2][i/2]->Tag 				= temp.at(i).GetKey();
		Grid->Buttons[i%2][i/2]->Color 				= ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		Grid->Buttons[i%2][i/2]->FontColor 			= ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		Grid->Buttons[i%2][i/2]->LatchedColor 		= ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
		Grid->Buttons[i%2][i/2]->LatchedFontColor 	= ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
	}

	if (size % 2 == 1)
	{
		Grid->Buttons[1][size/2]->Enabled 	= false;
		Grid->Buttons[1][size/2]->Visible 	= false;
	}
}
void __fastcall TfrmTables_ChitNumbers::TouchGrid1MouseClick(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, TGridButton *GridButton)
{
	GridBtnPressed(GridButton);
}
//---------------------------------------------------------------------------
void __fastcall TfrmTables_ChitNumbers::TouchGrid2MouseClick(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, TGridButton *GridButton)
{
	GridBtnPressed(GridButton);
}
//---------------------------------------------------------------------------
void __fastcall TfrmTables_ChitNumbers::TouchGrid3MouseClick(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, TGridButton *GridButton)
{
	if(GridButton->Latched)
	{
		SendFormController->AddOrder(GridButton->Caption, GridButton->Tag); 	// Add to orders here
	}
	else
	{
		SendFormController->RemoveOrder(GridButton->Caption, GridButton->Tag);	// Remove from orders here
	}
	BtnOkEnabled();
}
//---------------------------------------------------------------------------

void TfrmTables_ChitNumbers::GridBtnPressed(TGridButton *GridButton)
{
	if(GridButton->Latched)
	{
		SendFormController->AddOrder(GridButton->Caption, GridButton->Tag); 	// Add to orders here
	}
	else
	{
		SendFormController->RemoveOrder(GridButton->Caption, GridButton->Tag);	// Remove from orders here
	}
	BtnOkEnabled();
}

void TfrmTables_ChitNumbers::BtnOkEnabled(void)
{
	BtnOk->Enabled = SendFormController->EnableOkBtn();
}

void __fastcall TfrmTables_ChitNumbers::BtnOkMouseClick(TObject *Sender)
{
//	const int size = SendFormController->OrderKeysSize();
//	Database::TDBTransaction DBTransaction(DBControl);
//	DBTransaction.StartTransaction();
//	for(std::set<int>::iterator it = SendFormController->begin(); it != SendFormController->end(); it++)
//	{
//		TDBHeldOrder::GetHeldOrdersFromKey(DBTransaction, Orders.get(), ChitNumber, *it);
//		TDBHeldOrder::DeleteHeldOrderByKey(DBTransaction, *it);
//	}
//	DBTransaction.Commit();
//	ModalResult = mrOk;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	std::set<int> ProcessedKeys;
	for(std::multimap<UnicodeString, int>::iterator it = SendFormController->begin(); it != SendFormController->end(); it++)
	{
		if (ProcessedKeys.find(it->second) == ProcessedKeys.end())
		{
			TDBHeldOrder::GetHeldOrdersFromKey(DBTransaction, Orders.get(), ChitNumber, Patrons, it->second);
			//	see the below comment.
			//TDBHeldOrder::DeleteHeldOrderByKey(DBTransaction, it->second);
			ProcessedKeys.insert(it->second);
		}
	}

	/*	delete processed orders from the database. Moved the line from above loop to here
		because one cycle will delete an held order and the other will try to access some data using
		the deleted active-chit-number key.
	*/

	for(std::set<int>::iterator it = ProcessedKeys.begin(); it != ProcessedKeys.end(); it++)
	{
        TDBHeldOrder::DeleteHeldOrderByKey(DBTransaction, *it);
    }


	DBTransaction.Commit();
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTables_ChitNumbers::BtnAllMouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void TfrmTables_ChitNumbers::SelectAll(void)
{
//	TTouchGrid *Grids[3] = {TouchGrid1, TouchGrid2, TouchGrid3};
//	for(int j = 0; j < 3; j++)
//	{
//		int size = Grids[j]->ColCount;
//		for(int i = 0; i < size; i++)
//		{
//			SendFormController->GetOrders(Grids[j]->Buttons[0][i]->Tag);
//			Grids[j]->Buttons[0][i]->Latched = true;
//		}
//	}
}

