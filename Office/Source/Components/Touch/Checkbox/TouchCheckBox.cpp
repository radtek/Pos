//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TouchCheckBox.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TTouchCheckBox *)
{
	new TTouchCheckBox(NULL);
}
//---------------------------------------------------------------------------
__fastcall TTouchCheckBox::TTouchCheckBox(TComponent* Owner)
	: TCustomPanel(Owner)
{
   ParentCtl3D					= false;
   Ctl3D							= false;
   BorderStyle					= bsSingle;
   BevelInner					= bvNone;
   BevelOuter					= bvRaised;
   BevelWidth					= 2;
   Width							= 153;
   Height						= 81;
   TCustomPanel::Caption	= "";
	Font->Name					= "Arial";
   Font->Size					= 13;
   Font->Style					= TFontStyles() << fsBold;
   Font->Color					= clBlack;

	FCheckBox					= new TTouchCheck(this);
   FCheckBox->Parent			= this;
   FCheckBox->Align			= alLeft;
   FCheckBox->Alignment		= taLeftJustify;
   FCheckBox->Caption		= "";
   FCheckBox->Width			= GetSystemMetrics(SM_CXMENUCHECK) * 2;

	FLabel 						= new TChkTouchLabel(this);
   FLabel->Parent				= this;
	FLabel->AutoSize			= false;
	FLabel->Align				= alClient;
   FLabel->Alignment			= taCenter;
   FLabel->Layout				= tlCenter;
   FLabel->WordWrap			= true;
   FLabel->Caption			= "";
   FLabel->ParentFont		= true;
   FLabel->ShowAccelChar	= false;
}
//---------------------------------------------------------------------------
namespace Touchcheckbox
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TTouchCheckBox)};
		RegisterComponents("Touch Controls", classes, 0);
	}
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchCheckBox::Click()
{
	FCheckBox->TCustomCheckBox::Toggle();
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchCheckBox::SetCaption(AnsiString S)
{
//	FCheckBox->Caption=S;
	FLabel->Caption = S;
};
//---------------------------------------------------------------------------
inline AnsiString __fastcall TTouchCheckBox::GetCaption()
{
//	return FCheckBox->Caption;
   return FLabel->Caption;
};
//---------------------------------------------------------------------------
inline void __fastcall TTouchCheckBox::SetChecked(bool C)
{
	FCheckBox->Checked = C;
};
//---------------------------------------------------------------------------
inline bool __fastcall TTouchCheckBox::GetChecked()
{
	return FCheckBox->Checked;
};
//---------------------------------------------------------------------------
inline void __fastcall TTouchCheckBox::SetEnabled(bool E)
{
   FCheckBox->TCheckBox::Enabled = E;
   FLabel->TLabel::Enabled = E;
   TCustomPanel::Enabled = E;
};
//---------------------------------------------------------------------------
inline bool __fastcall TTouchCheckBox::GetEnabled()
{
	return TCustomPanel::Enabled;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TTouchCheck::TTouchCheck(TComponent* Owner)
	: TCheckBox(Owner)
{
	Caption = "";
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchCheck::MouseDown(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	((TTouchCheckBox *)Parent)->MouseDown(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchCheck::MouseUp(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	((TTouchCheckBox *)Parent)->MouseUp(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchCheck::MouseMove(Classes::TShiftState Shift, int X, int Y)
{
	((TTouchCheckBox *)Parent)->MouseMove(Shift,X,Y);
}
//---------------------------------------------------------------------------
inline void __fastcall TTouchCheck::Click()
{
	((TTouchCheckBox *)Parent)->TCustomPanel::Click();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TChkTouchLabel::TChkTouchLabel(TComponent* Owner)
	: TLabel(Owner)
{
	Caption = "";
}

//---------------------------------------------------------------------------
void __fastcall TChkTouchLabel::MouseDown(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	((TChkTouchLabel *)Parent)->MouseDown(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TChkTouchLabel::MouseUp(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	((TChkTouchLabel *)Parent)->MouseUp(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TChkTouchLabel::MouseMove(Classes::TShiftState Shift, int X, int Y)
{
	((TChkTouchLabel *)Parent)->MouseMove(Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TChkTouchLabel::Click()
{
	((TChkTouchLabel *)Parent)->Click();
}
//---------------------------------------------------------------------------
