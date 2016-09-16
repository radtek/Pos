//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectGlass.h"
#include "SelectDish.h"
#include "ItemSize.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

void
TfrmSelectGlass::ForwardItemDefinitionRefresh(
  Messages::TMessage &)
const
{
   frmSelectDish->Perform(
     item_management::network_messaging::refresh_item_display, 0, 0);

   const_cast<TfrmSelectGlass *>(this)->Preprocess(is_set_menu);
}

__fastcall TfrmSelectGlass::TfrmSelectGlass(TComponent* Owner)
	: TZForm(Owner), is_set_menu(false)
{
   Sizes = new TListSizeContainer;
}

__fastcall TfrmSelectGlass::~TfrmSelectGlass()
{
   delete Sizes;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectGlass::FormShow(TObject *Sender)
{
	FormResize(NULL);
   SelectedGlass = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectGlass::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectGlass::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
      	if((double)Screen->Width / Screen->Height < 1.4)
        {
   		ScaleBy(Screen->Width, Temp);
        }
   }

}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectGlass::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	SetMenuMask = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectGlass::FormHide(TObject *Sender)
{
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------



void __fastcall TfrmSelectGlass::CancelClick(TObject *Sender)
{
   SelectedGlass = -1;
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------


int TfrmSelectGlass::Preprocess(bool PickingSetMenuItem)
{
	UINT i;
   int ButtonsEnabled = 0;
   int AutoSelectedGlass = -1;
   bool ProductCountEnabled = false;

   is_set_menu = PickingSetMenuItem;

   tgridItemSizes->ColCount = 0;
	tgridItemSizes->RowCount = 0;

   tgridItemSizes->ColCount = 5;
   tgridItemSizes->RowCount = 5;

   for (int i = 0; i < tgridItemSizes->RowCount ; i++)
   {
      for (int j = 0; j < tgridItemSizes->ColCount; j++)
      {
         TItemSize *Size = Sizes->SizeGet((i*5) + j);
         if(Size != NULL)
         {
            tgridItemSizes->Buttons[i][j]->Visible = true;
            tgridItemSizes->Buttons[i][j]->Caption = Size->GetDecoratedName();
            tgridItemSizes->Buttons[i][j]->DisabledColor = clBtnFace;
            tgridItemSizes->Buttons[i][j]->FontColor = clWhite;
            tgridItemSizes->Buttons[i][j]->Tag = (i*5) + j;

            if( ((Size->SetMenuMask & SetMenuMask) && PickingSetMenuItem) // Set Menu ItemSize Matches.
                || ( Size->Available_As_Standard && !PickingSetMenuItem)  ) // Not a set Menu and Available as Standard.
            {
               tgridItemSizes->Buttons[i][j]->Enabled =
                 Size->IsEnabled();
               tgridItemSizes->Buttons[i][j]->Visible = true;
               tgridItemSizes->Buttons[i][j]->Color = clNavy;
               ButtonsEnabled++;
               AutoSelectedGlass = (i*5) + j;
			   ProductCountEnabled = ProductCountEnabled || Size->IsProductCountEnabled();
            }
            else
            {
               tgridItemSizes->Buttons[i][j]->Enabled = false;
               tgridItemSizes->Buttons[i][j]->Visible = false;
               tgridItemSizes->Buttons[i][j]->Color = clSilver;
            }
         }
         else
         {
            tgridItemSizes->Buttons[i][j]->Enabled = false;
            tgridItemSizes->Buttons[i][j]->Visible = false;
            tgridItemSizes->Buttons[i][j]->Color = clSilver;
         }
      }
   }

   if(ButtonsEnabled == 1 && !ProductCountEnabled)
   {
      SelectedGlass = AutoSelectedGlass;
   }
   else
   {
      SelectedGlass = -1;
   }
   return SelectedGlass;
}

void __fastcall TfrmSelectGlass::tgridItemSizesMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, TGridButton *GridButton,
      int X, int Y)
{
	SelectedGlass = GridButton->Tag;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

