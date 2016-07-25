//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChitList.h"
#include "ManagerChitNumber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"
TfrmChitList *frmChitList;
//---------------------------------------------------------------------------
__fastcall TfrmChitList::TfrmChitList(TComponent* Owner, chit_option_name_vector_t &ChitNames)
	: TZForm(Owner), ChitList(ChitNames)
{
}
//---------------------------------------------------------------------------

void TfrmChitList::DisplayChits()
{
	const int size = ChitList.size();
	const int cols = ChitListGrid->ColCount;
	ChitListGrid->RowCount = (size + (cols - 1)) / cols;
	UnicodeString chitOptionColor = "";
	for (int i = 0; i < size; i++)
	{
		AnsiString caption = ChitList.at(i).c_str();
		ChitListGrid->Buttons[i/8][i%8]->Caption   = caption;

		// check for chit option colors only when hold and send is enabled. Because without holding orders
		if(TGlobalSettings::Instance().EnableHoldSend)
			chitOptionColor = TManagerChitNumber::Instance().GetColorForChitOption(caption);
		else
          	chitOptionColor = "";

		if(chitOptionColor == "Green")
			ChitListGrid->Buttons[i/8][i%8]->Color 	   =  clGreen;
		else if(chitOptionColor == "Red")
			ChitListGrid->Buttons[i/8][i%8]->Color 	   =  clRed;
		else
			ChitListGrid->Buttons[i/8][i%8]->Color 	   =  ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];

		ChitListGrid->Buttons[i/8][i%8]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
	}

	if (size % 8 != 0)
	{
		for (int i = size; i % 8 != 0; i++)
		{
			ChitListGrid->Buttons[i/8][i%8]->Enabled = false;
			ChitListGrid->Buttons[i/8][i%8]->Visible = false;
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TfrmChitList::BtnCancelMouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmChitList::ChitListGridMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{
	chitName = GridButton->Caption;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmChitList::FormShow(TObject *Sender)
{
	DisplayChits();
}
//---------------------------------------------------------------------------
