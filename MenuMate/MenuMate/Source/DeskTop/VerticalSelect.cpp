//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "VerticalSelect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchcontrols"
#pragma link "touchgrid"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmVerticalSelect::TfrmVerticalSelect(TComponent* Owner)
   : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmVerticalSelect::FormShow(TObject *Sender)
{
	FormResize(NULL);
	SetWindowPos(Handle, HWND_TOP	, Left, Top, Width, Height, 0);
	SetGridColors(tgridSelection);
   if(Items.size() == 0)
   {
      TVerticalSelection Close;
	  Close.Title = "Cancel";
      Close.CloseSelection = true;
	  Close.Properties["Color"] = CL_BUTTONCOLOUR_CANCEL;
      Close.Properties["Visible"] = "true";
      Items.push_back(Close);
   }
   UpdateSelectionList();
}
//---------------------------------------------------------------------------
void __fastcall TfrmVerticalSelect::FormResize(TObject *Sender)
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
   Left				= (Screen->Width - Width) / 2;
   Top				= (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void TfrmVerticalSelect::UpdateSelectionList()
{
	tgridSelection->ColCount = 0;
	tgridSelection->RowCount = 0;

	tgridSelection->VertScrollDistance = (tgridSelection->Height / 9) * 6;
	tgridSelection->DefaultRowHeight = tgridSelection->Height / 9;

	tgridSelection->ColCount = 0;
	tgridSelection->RowCount = Items.size();


	for (int i = 0; i < Items.size() ; i++)
   {
      TVerticalSelection Item = Items[i];
      // Reset the Selected Property.
	  Item.Properties["Selected"] = false;
      Items[i] = Item;

	  std::map<UnicodeString,Variant>::iterator Property;

      Property = Item.Properties.find("Visible");
      if(Property != Item.Properties.end())
			tgridSelection->Buttons[i][0]->Visible = Property->second;
      else
         tgridSelection->Buttons[i][0]->Visible = true;

      Property = Item.Properties.find("Caption");
      if(Property != Item.Properties.end())
         tgridSelection->Buttons[i][0]->Caption = Property->second;
      else
         tgridSelection->Buttons[i][0]->Caption = Item.Title;

      Property = Item.Properties.find("Color");
      if(Property != Item.Properties.end())
		tgridSelection->Buttons[i][0]->Color = static_cast<TColor>(static_cast<int>(Property->second));
		else tgridSelection->Buttons[i][0]->Color = clGreen;

		Property = Item.Properties.find("DisabledColor");
		if(Property != Item.Properties.end())
			tgridSelection->Buttons[i][0]->DisabledColor = static_cast<TColor>(static_cast<int>(Property->second));
      else
         tgridSelection->Buttons[i][0]->DisabledColor = clBtnFace;

      Property = Item.Properties.find("FontColor");
      if(Property != Item.Properties.end())
		tgridSelection->Buttons[i][0]->FontColor = static_cast<TColor>(static_cast<int>(Property->second));
		else tgridSelection->Buttons[i][0]->FontColor = GetItemButtonFontColour(tgridSelection->Buttons[i][0]->Color);

      Property = Item.Properties.find("LatchedColor");
      if(Property != Item.Properties.end())
			tgridSelection->Buttons[i][0]->LatchedColor = static_cast<TColor>(static_cast<int>(Property->second));
      else
         tgridSelection->Buttons[i][0]->LatchedColor = clNavy;

      Property = Item.Properties.find("LatchedFontColor");
      if(Property != Item.Properties.end())
			tgridSelection->Buttons[i][0]->LatchedFontColor = static_cast<TColor>(static_cast<int>(Property->second));
      else
         tgridSelection->Buttons[i][0]->LatchedFontColor = GetItemButtonFontColour(tgridSelection->Buttons[i][0]->LatchedColor);

	  Property = Item.Properties.find("Latched");
      if(Property != Item.Properties.end())
      {
			tgridSelection->Buttons[i][0]->Latched = Property->second;
            Item.Properties["Selected"] =  Property->second;
      		Items[i] = Item;
      }
      else
      {
         	tgridSelection->Buttons[i][0]->Latched = false;
      }

      Property = Item.Properties.find("Tag");
      if(Property != Item.Properties.end())
         tgridSelection->Buttons[i][0]->Tag = Property->second;
      if(Item.IsDisabled)
         tgridSelection->Buttons[i][0]->Enabled = false;
   }
}

TColor TfrmVerticalSelect::GetItemButtonFontColour(TColor inColor)
{
	TColor RetVal;
	double R,G,B;         // input RGB values
	long A = ColorToRGB(inColor);
	R = GetRValue(A);
	G = GetGValue(A);
	B = GetBValue(A);
	double L = (0.2126 * R) + (0.7152 * G) + (0.0722 * B);
	if (L > 128)
	{
		RetVal = clBlack;
	}
	else
	{
		RetVal = clWhite;
	}
	return RetVal;
}

void __fastcall TfrmVerticalSelect::tgridSelectionMouseClick(
      TObject *Sender, TMouseButton Button, TShiftState Shift,
      TGridButton *GridButton)
{
	// Find the Button Index.
	int RowIndex = tgridSelection->Row(GridButton);
	// Retrive the Item.
   TVerticalSelection Item = Items[RowIndex];

	// Find its selected Property
	std::map<UnicodeString,Variant>::iterator Property;
	Property = Item.Properties.find("Selected");

	if(Property != Item.Properties.end())
	{
		if(static_cast<bool>(Item.Properties["Selected"]))
		{
			Item.Properties["Selected"] = false;
			GridButton->Latched = false;
		}
		else
		{
			Item.Properties["Selected"] = true;
			GridButton->Latched = true;
		}
	}
	else
	{  // Cant find it just set it.
		Item.Properties["Selected"] = true;
		GridButton->Latched = true;
	}

   Items[RowIndex] = Item;

   if(Item.CloseSelection)
   {
      Close();
   }
}
//---------------------------------------------------------------------------
bool __fastcall TfrmVerticalSelect::GetFirstSelectedItem(TVerticalSelection &SelectedItem)
{
	bool RetVal = false;
	for (int i = 0; (i < Items.size()) && (RetVal == false); i++)
	{
		TVerticalSelection Item = Items[i];
		std::map<UnicodeString,Variant>::iterator Property;
		Property = Item.Properties.find("Selected");
		if(Property != Item.Properties.end() && static_cast<bool>(Item.Properties["Selected"]) == true)
		{
			SelectedItem = Item;
			RetVal = true;
		}
	}
	return RetVal;
}

