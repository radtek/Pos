//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TouchRadioButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TTouchRadioButton *)
{
	new TTouchRadioButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TTouchRadioButton::TTouchRadioButton(TComponent* Owner)
	: TCustomPanel(Owner)
{
   ParentCtl3D						= false;
   Ctl3D								= false;
   BorderStyle						= bsSingle;
   BorderWidth						= 8;
   BevelInner						= bvNone;
   BevelOuter						= bvRaised;
   BevelWidth						= 2;
   Width								= 153;
   Height							= 81;
   TCustomPanel::Caption		= "";
   ParentFont						= true;
/*	Font->Name						= "Arial";
   Font->Size						= 13;
   Font->Style						= TFontStyles() << fsBold;
   Font->Color						= clBlack;     */
	FRadioButton					= new TTouchRadio(this);
   FRadioButton->Parent			= this;
   FRadioButton->Align			= alClient;
   FRadioButton->Alignment		= taRightJustify;
   FRadioButton->ParentFont	= true;
   FRadioButton->Caption		= "";
//	FRadioButton->Enabled		= TCustomPanel::Enabled;
//	FRadioButton->Show();
}
//---------------------------------------------------------------------------
namespace Touchradiobutton
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TTouchRadioButton)};
		RegisterComponents("Touch Controls", classes, 0);
	}
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchRadioButton::Click()
{
	if (!FRadioButton->TRadioButton::GetChecked())
   	FRadioButton->TRadioButton::SetChecked(true);
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchRadioButton::SetCaption(AnsiString S)
{
	FRadioButton->Caption=S;
};
//---------------------------------------------------------------------------
inline AnsiString __fastcall TTouchRadioButton::GetCaption()
{
	return FRadioButton->Caption;
};
//---------------------------------------------------------------------------
inline void __fastcall TTouchRadioButton::SetChecked(bool C)
{
	FRadioButton->Checked=C;
};
//---------------------------------------------------------------------------
inline bool __fastcall TTouchRadioButton::GetChecked()
{
	return FRadioButton->Checked;
};
//---------------------------------------------------------------------------
inline void __fastcall TTouchRadioButton::SetEnabled(bool E)
{
   FRadioButton->TTouchRadio::Enabled=E;
   TCustomPanel::Enabled=E;
};
//---------------------------------------------------------------------------
inline bool __fastcall TTouchRadioButton::GetEnabled()
{
	return TCustomPanel::Enabled;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TTouchRadio::TTouchRadio(TComponent* Owner)
	: TRadioButton(Owner)
{
	Caption = "";
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchRadio::MouseDown(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	((TTouchRadioButton *)Parent)->MouseDown(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchRadio::MouseUp(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	((TTouchRadioButton *)Parent)->MouseUp(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchRadio::MouseMove(Classes::TShiftState Shift, int X, int Y)
{
	((TTouchRadioButton *)Parent)->MouseMove(Shift,X,Y);
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchRadio::Click()
{
	((TTouchRadioButton *)Parent)->TCustomPanel::Click();
	TWinControl *WinControl = Parent->Parent;
   for (int i=0; i<WinControl->ControlCount; i++)
   {
      if (Parent != WinControl->Controls[i])
      {
         TTouchRadioButton *TouchRadioButton;
         if ((TouchRadioButton = dynamic_cast<TTouchRadioButton *>(WinControl->Controls[i])) != 0)
         {
            TouchRadioButton->Checked = false;
         }
      }
   }
/*      if ((b2 = dynamic_cast<Base2 *>(b1)) != 0) {
          cout << "The resulting pointer is of type "

               << typeid(b2).name() << endl;
         }

   WinControl = dynamic_cast<Parent->Parent>
   Parent->Parent */
}
//---------------------------------------------------------------------------
