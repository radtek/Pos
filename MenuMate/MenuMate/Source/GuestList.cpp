//---------------------------------------------------------------------------

#pragma hdrstop

#include "GuestList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm" 
TfrmGuestList *frmGuestList;
//---------------------------------------------------------------------------
__fastcall TfrmGuestList::TfrmGuestList(TComponent* Owner): TZForm(Owner)
{
}
//---------------------------------------------------------------------------

void TfrmGuestList::DisplayGuests()
{
    const int size = GuestAccounts.size();
	const int cols = GuestListGrid->ColCount;
	GuestListGrid->RowCount = (size + (cols - 1)) / cols;
	UnicodeString chitOptionColor = "";
	for (int i = 0; i < size; i++)
	{
	   	AnsiString caption = GuestAccounts.at(i).AccountDetails.at(i).FirstName + " " + GuestAccounts.at(i).AccountDetails.at(i).LastName;
        caption = caption.c_str();
		GuestListGrid->Buttons[i/8][i%8]->Caption   = caption;
        GuestListGrid->Buttons[i/8][i%8]->Tag = i;

		GuestListGrid->Buttons[i/8][i%8]->Color 	   =  ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];

		GuestListGrid->Buttons[i/8][i%8]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
	}

	if (size % 8 != 0)
	{
		for (int i = size; i % 8 != 0; i++)
		{
			GuestListGrid->Buttons[i/8][i%8]->Enabled = false;
			GuestListGrid->Buttons[i/8][i%8]->Visible = false;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmGuestList::BtnCancelMouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGuestList::GuestListGridMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{
    int size = GuestAccounts.size();
    for(int index = 0; index < size; index++ )
    {
        if(index == GridButton->Tag)
        {
            AccountNumber = GuestAccounts.at(index).AccountNumber;
            break;
        }
    }
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmGuestList::FormShow(TObject *Sender)
{
	DisplayGuests();
}
//---------------------------------------------------------------------------

