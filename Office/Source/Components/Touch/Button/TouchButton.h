//---------------------------------------------------------------------------
#ifndef TouchButtonH
#define TouchButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TTouchLabel : public TLabel
{
private:
	DYNAMIC void	__fastcall	MouseDown(TMouseButton Button, Classes::
										TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall	MouseUp(TMouseButton Button, Classes::
										TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall	MouseMove(Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall	Click();
public:
	__fastcall	TTouchLabel(TComponent* Owner);
};
//---------------------------------------------------------------------------
class PACKAGE TTouchButton : public TCustomPanel
{
	friend class TTouchLabel;
__published:
	__property	AnsiString	Caption								={read=GetCaption,write=SetCaption};
	__property	TColor		Color									={read=GetColour,write=SetColour};
	__property	TColor		UpColour								={read=FUpColour,write=SetUpColour};
	__property	TColor		DownColour							={read=FDownColour,write=SetDownColour};
   __property	bool			Enabled								={read=GetEnabled,write=SetEnabled,stored=IsEnabledStored,
   																			default=1};
	__property	int			InitialDelay						={read=FInitialDelay,write=FInitialDelay,default=500};
	__property	int			RepeatDelay							={read=FRepeatDelay,write=FRepeatDelay,default=50};
	__property	int			AccelSteps							={read=FAccelSteps,write=FAccelSteps,default=1};

   __property	Font;
   __property	ParentFont;
   __property	Visible;
   __property	BevelInner;
   __property	BevelOuter;
   __property	BorderStyle;
   __property	BorderWidth;
   __property	BevelWidth;
   __property	Ctl3D;
	__property	ParentCtl3D;

   __property	OnMouseDown;
   __property	OnMouseUp;
   __property	OnClick;
	__property	TMouseEvent OnAutoRepeat = {read=FOnAutoRepeat, write=FOnAutoRepeat};
//	__fastcall TTouchButton::OnColourTimer(TObject *Sender)
private:
	TTouchLabel	*FLabel;
   TTimer		*FTimer;
   TTimer		*FColourTimer;
	TColor		FUpColour;
   TColor		FDownColour;
	TMouseEvent	FOnAutoRepeat;
   int			FInitialDelay;
   int			FRepeatDelay;
   int			FAccelSteps;

   int			AutoRepeatCount;

   void			__fastcall	SetEnabled(bool E);//				{ 	TCustomPanel::Enabled=E;
   															//				if(E)	FLabel->Font->Color=FEnabledColour;
                                                  //          else	FLabel->Font->Color=FDisabledColour; }
   bool			__fastcall	GetEnabled();//						{ 	return TCustomPanel::Enabled; }
   void			__fastcall	SetCaption(AnsiString S)		{ 	FLabel->Caption=S; }
   AnsiString	__fastcall	GetCaption()						{ 	return FLabel->Caption; }
   void			__fastcall	SetColour(TColor C);
   TColor		__fastcall	GetColour();
   void			__fastcall	SetUpColour(TColor C)			{ 	FUpColour = C ; Color = C ; }
   void			__fastcall	SetDownColour(TColor C)			{ 	FDownColour=C; }
	void 			__fastcall 	TimerAutoRepeat(TObject *Sender);
protected:
	DYNAMIC void __fastcall	MouseUp(TMouseButton Button, Classes::
									TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall	MouseDown(TMouseButton Button, Classes::
									TShiftState Shift, int X, int Y);
public:
	__fastcall	TTouchButton(TComponent* Owner);
	__property	TTouchLabel *Label={read=FLabel};
};
//---------------------------------------------------------------------------
#endif
