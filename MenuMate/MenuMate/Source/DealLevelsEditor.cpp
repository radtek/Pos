//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DealLevelsEditor.h"
#include "MMTouchNumpad.h"
#include "DealLevel.h"
#include <System.hpp>
#include <utility>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmDealLevels *frmDealLevels;
//---------------------------------------------------------------------------
__fastcall TfrmDealLevels::TfrmDealLevels(TComponent* Owner, Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealLevels::FormShow(TObject *Sender)
{
	sgDisplay->Cols[0]->AddObject("Quantity", 0);
	sgDisplay->Cols[1]->AddObject("Price", 0);
    sgDisplay->Row = 1;
}
//---------------------------------------------------------------------------

StoredDealLevelSet TfrmDealLevels::GetDealLevels()
{
    return DealLevelsList;
}
//---------------------------------------------------------------------------

void TfrmDealLevels::SetDealLevels (StoredDealLevelSet inDealLevels)
{
	DealLevelsList = inDealLevels;
    updateDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealLevels::sgDisplayDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	TStringGrid* StringGrid = static_cast<TStringGrid*>(Sender);
	assert(StringGrid != NULL);
	StringGrid->Canvas->FillRect(Rect);

	AnsiString text(StringGrid->Cells[ACol][ARow]);
	RECT RText = static_cast<RECT>(Rect);
	InflateRect(&RText, -3, -3);

	if (ARow == 0)
	{
		DrawText(StringGrid->Canvas->Handle, text.c_str(), text.Length(), &RText, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}
	else
	{
		DrawText(StringGrid->Canvas->Handle, text.c_str(), text.Length(), &RText, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
    }
}
//---------------------------------------------------------------------------

void TfrmDealLevels::add(Currency initQty, Currency initPrice)
{
	Currency qty   = currencyTouchPad("Enter Quantity");
	Currency price = currencyTouchPad("Enter Price");

	IStoredDealLevel *dealLevel = new TDealLevel(qty, price);
	std::pair<StoredDealLevelSet::iterator, bool> insertResult = DealLevelsList.insert(dealLevel);
	if (!insertResult.second)
	{
        (*insertResult.first)->SetPrice(price);
    }
	updateDisplay();
}
//---------------------------------------------------------------------------

void TfrmDealLevels::edit()
{
	StoredDealLevelSet::iterator it = getDealLevelFromSelection();
	if (it != NULL)
	{
		UnicodeString priceString = sgDisplay->Cells[1][sgDisplay->Row];
		//Currency qty   = currencyTouchPad("Enter Quantity", sgDisplay->Cells[0][sgDisplay->Row]);
		Currency price = currencyTouchPad("Enter Price", priceString.SubString(2, (priceString.Length() - 1)));
		//(*it)->SetQty(qty);
		(*it)->SetPrice(price);
        updateDisplay();
	}
}
//---------------------------------------------------------------------------

void TfrmDealLevels::remove()
{
	StoredDealLevelSet::iterator it = getDealLevelFromSelection();
	if (it != NULL)
	{
		delete *it;
		DealLevelsList.erase(it);
		updateDisplay();
	}
}
//---------------------------------------------------------------------------

void TfrmDealLevels::updateDisplay()
{
	UnicodeString qtyString;
	UnicodeString priceString;
	sgDisplay->RowCount = DealLevelsList.size() + 1;
	int gridRow = 1;
	for (StoredDealLevelSet::iterator it = DealLevelsList.begin(); it != DealLevelsList.end(); it++, gridRow++)
	{
		qtyString = CurrToStr((*it)->GetQty());
		priceString = UnicodeString::CurrToStrF((*it)->GetPrice(), UnicodeString::sffCurrency, 2);
		sgDisplay->Cells[0][gridRow] = qtyString;
		sgDisplay->Cells[1][gridRow] = priceString;
	}
	if (DealLevelsList.empty())
	{
		sgDisplay->RowCount = 2;
		sgDisplay->Cells[0][1] = "";
		sgDisplay->Cells[1][1] = "";
    }
}
//---------------------------------------------------------------------------

StoredDealLevelSet::iterator TfrmDealLevels::getDealLevelFromSelection()
{
	StoredDealLevelSet::iterator it = NULL;
	if (sgDisplay->Row > 0 && sgDisplay->Cells[0][sgDisplay->Row] != "")
	{
		it = DealLevelsList.begin();
		advance(it, sgDisplay->Row - 1);
	}
	return it;
}
//---------------------------------------------------------------------------

Currency TfrmDealLevels::currencyTouchPad(UnicodeString inCaption, Currency inInitValue)
{
	Currency retVal = inInitValue;
	std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = inCaption;
	frmTouchNumpad->btnOk->Visible = true;
	frmTouchNumpad->Mode = pmDecimal;
	frmTouchNumpad->CURInitial = inInitValue;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		retVal = frmTouchNumpad->CURResult;
	}
	return retVal;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealLevels::tbtnOkMouseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealLevels::tbtnAddMouseClick(TObject *Sender)
{
	add();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealLevels::tbtnEditMouseClick(TObject *Sender)
{
	edit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealLevels::tbtnDeleteMouseClick(TObject *Sender)
{
	remove();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDealLevels::sgDisplaySelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
	if (ARow == 0)
	{
    	CanSelect = false;
	}
}
//---------------------------------------------------------------------------

