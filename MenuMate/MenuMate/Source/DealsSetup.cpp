//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DealsSetup.h"
#include "DealEditor.h"
#include "Deal.h"
#include "DealManager.h"
#include <algorithm>
#include <iterator>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchBtn"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmDeals::TfrmDeals(TComponent* Owner, Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl)
{
	selectedDealIndex = -1;
	dealManager = TDealManager::Instance();
}
//---------------------------------------------------------------------------
__fastcall TfrmDeals::~TfrmDeals()
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmDeals::btnOkMouseClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDeals::FormShow(TObject *Sender)
{
	SetGridColors(tgridSelection);
	updateDealGrid();
}
//---------------------------------------------------------------------------
void TfrmDeals::addDeal()
{
	IStoredDeal *deal = new TDeal();
    TfrmDealEditorForm *frmDealEditorForm = new TfrmDealEditorForm(this,DBControl);
	frmDealEditorForm->SetDeal(deal);
	if (frmDealEditorForm->ShowModal() == mrOk)
	{
		dealManager->Add(deal);
		updateDealGrid();
	}
    delete frmDealEditorForm;
}
//---------------------------------------------------------------------------
void TfrmDeals::editDeal()
{
	if(selectedDealIndex != -1)
	{
		TfrmDealEditorForm *frmDealEditorForm = new TfrmDealEditorForm(this,DBControl);
		frmDealEditorForm->SetDeal(dealManager->GetDealAt(selectedDealIndex));
		frmDealEditorForm->ShowModal();
        dealManager->Update(selectedDealIndex);
		updateDealGrid();
        delete frmDealEditorForm;
	}

}
//---------------------------------------------------------------------------
void TfrmDeals::deleteDeal()
{
	if(selectedDealIndex != -1)
	{
        dealManager->Remove(selectedDealIndex);
		updateDealGrid();
	}
}
//---------------------------------------------------------------------------
void TfrmDeals::updateDealGrid()
{
	tgridSelection->RowCount = 0;
	const int size = dealManager->GetDealsSize();
	tgridSelection->RowCount = size;

	int i = 0;

	for (StoredDealVector::iterator it = dealManager->GetDealsBegin(); it != dealManager->GetDealsEnd(); it++, i++)
	{
        tgridSelection->Buttons[i][0]->Caption = (*it)->GetName();
        tgridSelection->Buttons[i][0]->Tag = i;
	}

	selectedDealIndex = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDeals::btnAddDealMouseClick(TObject *Sender)
{
	addDeal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDeals::btnEditDealMouseClick(TObject *Sender)
{
	editDeal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDeals::btnDeleteDealMouseClick(TObject *Sender)
{
	deleteDeal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDeals::tgridSelectionMouseClick(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, TGridButton *GridButton)
{
	selectedDealIndex = GridButton->Tag;
}
//---------------------------------------------------------------------------

