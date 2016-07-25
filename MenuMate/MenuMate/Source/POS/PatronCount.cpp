//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PatronCount.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchBtn"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmPatronCount::TfrmPatronCount(TComponent* Owner,Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPatronCount::FormShow(TObject *Sender)
{
	FormResize(NULL);

	pnl00->Visible = false;
	btnCLR->Left = pnl2->Left;
	btnCLR->Width = pnl3->Left + pnl3->Width - pnl2->Left;
	lbeEnteredValue->Caption = IntToStr(wrkIntAmount);

	Patrons.clear();

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerPatron::Instance().GetPatronTypes(DBTransaction,Patrons);
	DBTransaction.Commit();

	for (std::vector<TPatronType>::iterator ptrInitPatron = InitPatrons.begin();
		  ptrInitPatron != InitPatrons.end();
		  ptrInitPatron++)
	{
		for (std::vector<TPatronType>::iterator ptrPatron = Patrons.begin();
			  ptrPatron != Patrons.end();
			  ptrPatron++)
		{
			if(ptrInitPatron->Name == ptrPatron->Name)
			{
				ptrPatron->Count = ptrInitPatron->Count;
			}
		}
	}

	btnPartonTypes->RowCount = Patrons.size();

	std::vector<TPatronType>::iterator ptrPatronTypes = Patrons.begin();
	int Row = 0;
	for (ptrPatronTypes = Patrons.begin() ;ptrPatronTypes != Patrons.end() ; ptrPatronTypes++,Row++)
	{
		btnPartonTypes->Buttons[Row][0]->Caption = ptrPatronTypes->Name + "\r" + IntToStr(ptrPatronTypes->Count);
		btnPartonTypes->Buttons[Row][0]->Color = clBlue;
		btnPartonTypes->Buttons[Row][0]->FontColor = clWhite;
		btnPartonTypes->Buttons[Row][0]->LatchedColor = clGreen;
		btnPartonTypes->Buttons[Row][0]->LatchedFontColor = clWhite;
		
		if(ptrPatronTypes->Default == true)
		{ // This button should be down.
			btnPartonTypes->Buttons[Row][0]->Latched = true;
			CurrentButton = btnPartonTypes->Buttons[Row][0];
			if (ptrPatronTypes->Count == 0)
			{
				lbeEnteredValue->ParentColor = true;
				lbeEnteredValue->Font->Color = clBlack;
			}
			else
			{
				lbeEnteredValue->Color = clHighlight;
				lbeEnteredValue->Font->Color = clHighlightText;
				lbeEnteredValue->Caption = IntToStr(ptrPatronTypes->Count);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPatronCount::FormHide(TObject *Sender)
{
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPatronCount::btnNumberMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TLabel *Btn = (TLabel *)Sender;

	if (lbeEnteredValue->Color == clHighlight)
	{
		wrkIntAmount = StrToInt(Btn->Caption);
		lbeEnteredValue->Caption = IntToStr(wrkIntAmount);
	}
	else
	{
		if (IntToStr(wrkIntAmount).Length() < 9)
		{
			wrkIntAmount = wrkIntAmount * 10 + StrToInt(Btn->Caption);
			lbeEnteredValue->Caption = IntToStr(wrkIntAmount);
		}
	}
	lbeEnteredValue->ParentColor = true;
	lbeEnteredValue->Font->Color = clBlack;

	if(CurrentButton)
	{
		int Row = btnPartonTypes->Row(CurrentButton);
		Patrons[Row].Count = wrkIntAmount;
		CurrentButton->Caption = Patrons[Row].Name + "\r" + IntToStr(Patrons[Row].Count);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPatronCount::btnCLRMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	wrkIntAmount = 0;
	lbeEnteredValue->Caption = IntToStr(wrkIntAmount);

	lbeEnteredValue->ParentColor = true;
	lbeEnteredValue->Font->Color = clBlack;

	if(CurrentButton)
	{
		int Row = btnPartonTypes->Row(CurrentButton);
		Patrons[Row].Count = wrkIntAmount;
		CurrentButton->Caption = Patrons[Row].Name + "\r" + IntToStr(Patrons[Row].Count);
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmPatronCount::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPatronCount::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
   }
   Panel5->Width = Panel2->BorderWidth;
	ClientWidth = Panel2->Width;
	ClientHeight = Panel2->Height;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPatronCount::btnPartonTypesMouseClick(TObject *Sender,
		TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	CurrentButton = GridButton;
	if(CurrentButton)
	{
		int Row = btnPartonTypes->Row(CurrentButton);
		wrkIntAmount = Patrons[Row].Count;
		lbeEnteredValue->Caption = IntToStr(wrkIntAmount);
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmPatronCount::btnOkMouseClick(TObject *Sender)
{
	ModalResult = mrOk;	
}
//---------------------------------------------------------------------------

