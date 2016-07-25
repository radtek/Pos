//---------------------------------------------------------------------------
#ifndef TouchRadioButtonH
#define TouchRadioButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TTouchRadio : public TRadioButton
{
	friend class TTouchRadioButton;
private:
	DYNAMIC void	__fastcall	MouseDown(TMouseButton Button, Classes::
										TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall	MouseUp(TMouseButton Button, Classes::
										TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall	MouseMove(Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void	__fastcall	Click();
protected:
public:
	__fastcall	TTouchRadio(TComponent* Owner);
};
//---------------------------------------------------------------------------
class PACKAGE TTouchRadioButton : public TCustomPanel
{
	friend class TTouchRadio;
__published:
	__property		AnsiString	Caption={read=GetCaption,write=SetCaption};
	__property		bool			Enabled={read=GetEnabled,write=SetEnabled,stored
										=IsEnabledStored, default=1};
	__property		bool			Checked={read=GetChecked,write=SetChecked};
	__property		Color;
	__property		Font;
	__property		ParentFont;
	__property		Visible;

	__property		OnMouseDown;
	__property		OnMouseUp;
	__property		OnClick;
private:
	TTouchRadio 	*FRadioButton;
	void				__fastcall	SetCaption(AnsiString S);
	AnsiString		__fastcall	GetCaption();

	void				__fastcall	SetChecked(bool C);
	bool				__fastcall	GetChecked();

	void				__fastcall	SetEnabled(bool E);
	bool				__fastcall	GetEnabled();
protected:
	DYNAMIC void	__fastcall	Click();
public:
	__fastcall		TTouchRadioButton(TComponent* Owner);
	__property		TTouchRadio *RadioButton={read=FRadioButton};
};
//---------------------------------------------------------------------------
#endif
