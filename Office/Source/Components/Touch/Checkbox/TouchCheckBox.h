//---------------------------------------------------------------------------
#ifndef TouchCheckBoxH
#define TouchCheckBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TChkTouchLabel : public TLabel
{
private:
	DYNAMIC void	__fastcall	MouseDown(TMouseButton Button, Classes::
										TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall	MouseUp(TMouseButton Button, Classes::
										TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall	MouseMove(Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall	Click();
public:
	__fastcall	TChkTouchLabel(TComponent* Owner);
};
//---------------------------------------------------------------------------
class TTouchCheck : public TCheckBox
{
	friend class TTouchCheckBox;
private:
	DYNAMIC void	__fastcall			MouseDown(TMouseButton Button, Classes::
						TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall			MouseUp(TMouseButton Button, Classes::
						TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall			MouseMove(Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall			Click();
protected:
public:
	__fastcall					TTouchCheck(TComponent* Owner);
};
//---------------------------------------------------------------------------
class PACKAGE TTouchCheckBox : public TCustomPanel
{
	friend class TTouchCheck;
	friend class TTouchLabel;
__published:
	__property	AnsiString	Caption={read=GetCaption,write=SetCaption,stored=IsCaptionStored};
	__property	bool			Enabled={read=GetEnabled,write=SetEnabled,stored
									=IsEnabledStored, default=1};
	__property	bool			Checked={read=GetChecked,write=SetChecked};
   __property	Color;
   __property	Font;
   __property	ParentFont;
   __property	Visible;

   __property	BevelInner;
   __property	BevelOuter;
   __property	BorderStyle;
   __property	BorderWidth;
   __property	BevelWidth;
   __property	Ctl3D;

   __property	OnMouseDown;
   __property	OnMouseUp;
   __property	OnClick;
private:
	TTouchCheck		*FCheckBox;
	TChkTouchLabel	*FLabel;
	void				__fastcall	SetCaption(AnsiString S);
	AnsiString		__fastcall	GetCaption();

	void				__fastcall	SetChecked(bool C);
	bool				__fastcall	GetChecked();

	void				__fastcall	SetEnabled(bool E);
	bool				__fastcall	GetEnabled();
protected:
	DYNAMIC void	__fastcall	Click();
public:
	__fastcall		TTouchCheckBox(TComponent* Owner);
	__property		TTouchCheck	*CheckBox={read=FCheckBox};
};
//---------------------------------------------------------------------------
#endif
