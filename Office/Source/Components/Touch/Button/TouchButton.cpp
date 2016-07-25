//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TouchButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TTouchButton *)
{
	new TTouchButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TTouchButton::TTouchButton(TComponent* Owner)
	: TCustomPanel(Owner)
{
   ParentCtl3D					= false;
   Ctl3D							= false;
   BorderStyle					= bsSingle;
   BevelInner					= bvNone;
   BevelOuter					= bvRaised;
   BevelWidth					= 2;
   Width							= 153;
   Height						= 57;
   TCustomPanel::Caption	= "";
	Font->Name					= "Arial";
   Font->Size					= 13;
   Font->Style					= TFontStyles() << fsBold;
   Font->Color					= clBlack;
   UpColour						= TCustomPanel::Color;
   DownColour					= TCustomPanel::Color;
   FInitialDelay				= 500;
   FRepeatDelay				= 50;
   FAccelSteps					= 1;
   AutoRepeatCount			= 1;	// Start at 1 cos the first 1 has already gone with the first push.
	FLabel 						= new TTouchLabel(this);
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
namespace Touchbutton
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TTouchButton)};
		RegisterComponents("Touch Controls", classes, 0);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchButton::MouseDown(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	TCustomPanel::Color = FDownColour;
	TCustomPanel::Update(); 
	if (FTimer)
	{
		FTimer->Enabled = false;
		delete FTimer;
		FTimer = NULL;
	}
	TCustomPanel::MouseDown(Button,Shift,X,Y);


/*	if (FColourTimer)
	{
		TCustomPanel::Color = FDownColour;
		FColourTimer->Interval	= 250;
		FColourTimer->OnTimer	= OnColourTimer;
		FColourTimer->Enabled	= false;
	} */



	if (Button == mbLeft && FOnAutoRepeat)
	{
		FTimer = new TTimer(this);
      FTimer->Enabled = false;
      FTimer->Interval = FInitialDelay;
      FTimer->OnTimer = TimerAutoRepeat;
      AutoRepeatCount = 1;
      FTimer->Enabled = true;
   }
}
//---------------------------------------------------------------------------
void __fastcall TTouchButton::MouseUp(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	TCustomPanel::Color = FUpColour;
	if (FTimer)
   {
		FTimer->Enabled = false;
		delete FTimer;
		FTimer = NULL;
	}
	TCustomPanel::MouseUp(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
//void __fastcall TTouchButton::OnColourTimer(TObject *Sender)
//{
//	FColourTimer->Enabled = false;
//	TCustomPanel::Color = FUpColour;
//}
//---------------------------------------------------------------------------
void __fastcall TTouchButton::TimerAutoRepeat(TObject *Sender)
{
	FTimer->Enabled = false;
	if (AutoRepeatCount < FAccelSteps)
   {
   	FTimer->Interval = FInitialDelay - ((FInitialDelay - FRepeatDelay) * AutoRepeatCount / FAccelSteps);
      ++AutoRepeatCount;
   }
   else
   {
   	FTimer->Interval = FRepeatDelay;
   }
   FOnAutoRepeat(this,mbLeft,TShiftState(),0,0);
	if (FTimer)
	{
		FTimer->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchButton::SetEnabled(bool E)
{
   FLabel->Enabled			= E;
	TCustomPanel::Enabled	= E;
   TCustomPanel::Color		= E?FUpColour:clSilver;
	if (FTimer)
   {
   	FTimer->Enabled = false;
      delete FTimer;
      FTimer = NULL;
   }
}
//---------------------------------------------------------------------------
bool __fastcall TTouchButton::GetEnabled()
{
	return TCustomPanel::Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TTouchButton::SetColour(TColor C)
{
   FUpColour = C;
   FDownColour = C;
   TCustomPanel::Color = Enabled?C:clSilver;
}
//---------------------------------------------------------------------------
TColor __fastcall TTouchButton::GetColour()
{
	return TCustomPanel::Color;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TTouchLabel::TTouchLabel(TComponent* Owner)
	: TLabel(Owner)
{
	Caption = "";
}
//---------------------------------------------------------------------------
void __fastcall TTouchLabel::MouseDown(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	((TTouchButton *)Parent)->MouseDown(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TTouchLabel::MouseUp(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	((TTouchButton *)Parent)->MouseUp(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TTouchLabel::MouseMove(Classes::TShiftState Shift, int X, int Y)
{
	((TTouchButton *)Parent)->MouseMove(Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TTouchLabel::Click()
{
	((TTouchButton *)Parent)->Click();
}
//---------------------------------------------------------------------------
