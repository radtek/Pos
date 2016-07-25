//---------------------------------------------------------------------------
#ifndef TouchBtnH
#define TouchBtnH
//---------------------------------------------------------------------------
#include "TouchControls.h"
//---------------------------------------------------------------------------
namespace Touchbtn
{
//---------------------------------------------------------------------------
enum TArrowType { atNone, atLeft, atUp, atRight, atDown };
//---------------------------------------------------------------------------
class PACKAGE TTouchBtn : public TCustomTouchButtonControl
{
public:
	__fastcall						TTouchBtn(TComponent* Owner);
	__fastcall virtual			~TTouchBtn() {};

__published:
	__property UnicodeString		Caption					= { read=GetCaption, write=SetCaption };
	__property TColor				ButtonColor				= { read=GetBtnColor, write=SetBtnColor };
	__property TColor				LatchedColor			= { read=GetBtnLatchedColor, write=SetBtnLatchedColor };
	__property TColor				DisabledButtonColor	= { read=GetDisabledBtnColor, write=SetDisabledBtnColor };
	__property bool				Enabled					= { read=GetEnabled, write=SetEnabled, default=true };

	__property bool				Latched					= { read=GetLatched, write=SetLatched };
	__property bool				Highlighted				= { read=GetHighlighted, write=SetHighlighted };

	__property						LatchingType;
	__property						LatchingProperties;
	__property						Height					= { default=57 };
	__property						Width						= { default=185 };
	__property						Tag;

        __property Constraints;

	__property TArrowType		ArrowType				= { read=FArrowType, write=SetArrowType, default=atNone };

	__property TNotifyEvent		OnMouseClick			= { read=FBtnOnMouseClick, write=FBtnOnMouseClick };
	__property TNotifyEvent		OnAutoRepeat			= { read=FBtnOnAutoRepeat, write=FBtnOnAutoRepeat };
	__property TMouseEvent		OnMouseDown				= { read=FBtnOnMouseDown, write=FBtnOnMouseDown };
	__property TMouseEvent		OnMouseUp				= { read=FBtnOnMouseUp, write=FBtnOnMouseUp };
	__property TMouseMoveEvent	OnMouseMove				= { read=FBtnOnMouseMove, write=FBtnOnMouseMove };

	__property	int				InitialDelay			={read=FInitialDelay,write=FInitialDelay,default=500};
	__property	int				RepeatDelay				={read=FRepeatDelay,write=FRepeatDelay,default=50};
	__property	int				AccelSteps				={read=FAccelSteps,write=FAccelSteps,default=1};

protected:
	TGridButton						*GetButtonXY(int X, int Y);
	TRect								GetButtonRect(const TGridButton *Button);

	UnicodeString __fastcall		GetCaption();
	void __fastcall				SetCaption(UnicodeString Caption);

	bool __fastcall				GetEnabled();
	void __fastcall				SetEnabled(bool Value);

	TColor __fastcall				GetBtnColor();
	void __fastcall				SetBtnColor(TColor Color);

	TColor __fastcall				GetBtnLatchedColor();
	void __fastcall				SetBtnLatchedColor(TColor Color);

	TColor __fastcall				GetDisabledBtnColor();
	void __fastcall				SetDisabledBtnColor(TColor Color);

	bool __fastcall				GetHighlighted();
	void __fastcall				SetHighlighted(bool Highlighted);

	bool __fastcall				GetLatched();
	void __fastcall				SetLatched(bool Latched);

	TArrowType						FArrowType;
	TColor							FArrowColor;
	TColor							FArrowBorderColor;
	int								FArrowScale;
	void	__fastcall				SetArrowType(TArrowType Value);
	void	__fastcall				SetArrowColor(TColor Value);
	void	__fastcall				SetArrowBorderColor(TColor Value);
	void	__fastcall				SetArrowScale(int Value);

	virtual void __fastcall		WMSize(TWMSize& Message);
	virtual void __fastcall		Paint(void);

	TTimer							*FTimer;
	int								FInitialDelay;
	int								FRepeatDelay;
	int								FAccelSteps;
	int								FAutoRepeatCount;
	void __fastcall				FTimerAutoRepeat(TObject *Sender);


	TNotifyEvent					FBtnOnMouseClick;
	TNotifyEvent					FBtnOnAutoRepeat;
	TMouseEvent						FBtnOnMouseDown;
	TMouseEvent						FBtnOnMouseUp;
	TMouseMoveEvent				FBtnOnMouseMove;
	void __fastcall				BtnMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall				BtnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton, int X, int Y);
	void __fastcall				BtnMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton, int X, int Y);
	void __fastcall				BtnMouseMove(TObject *Sender, TShiftState Shift, TGridButton *GridButton, int X, int Y);
};
//---------------------------------------------------------------------------
} // namespace
using namespace Touchbtn;
//---------------------------------------------------------------------------
#endif
