//---------------------------------------------------------------------------
#ifndef TouchTimeH
#define TouchTimeH
//---------------------------------------------------------------------------
#include "TouchControls.h"
//---------------------------------------------------------------------------
namespace Touchtime
{
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TTimeClickEvent)(System::TObject* Sender);
//---------------------------------------------------------------------------
class PACKAGE TTouchTime : public TCustomTouchButtonControl
{
public:
	__fastcall TTouchTime(TComponent* Owner);
	__fastcall virtual ~TTouchTime();

__published:
	__property								BevelOuter						= { default=bvLowered };
	__property								ParentFont						= { default=false };
	__property int							GridBorderWidth				= { read=FGridBorderWidth, write=SetGridBorderWidth, default=4 };
	__property int							ButtonGapWidth					= { read=FBtnGapWidth, write=SetBtnGapWidth };
	__property int							ButtonGapHeight				= { read=FBtnGapHeight, write=SetBtnGapHeight };

	__property TGridButton				*ScrollHourUp					= { read=FHourUpButton, write=FHourUpButton };
	__property TGridButton				*ScrollHourDown				= { read=FHourDownButton, write=FHourDownButton };
	__property TGridButton				*ScrollMinuteUp				= { read=FMinuteUpButton, write=FMinuteUpButton };
	__property TGridButton				*ScrollMinuteDown				= { read=FMinuteDownButton, write=FMinuteDownButton };

	__property TColor						ArrowColor						= { read=FArrowColor, write=SetArrowColor, default=clBlack };
	__property TColor						ArrowBorderColor				= { read=FArrowBorderColor, write=SetArrowBorderColor, default=clBlack };
	__property int							ArrowScale						= { read=FArrowScale, write=SetArrowScale, default=30 };

	__property int							InitialDelay					= { read=FInitialDelay, write=FInitialDelay };
	__property int							RepeatDelay						= { read=FRepeatDelay, write=FRepeatDelay };

	__property int							ButtonWidth						= { read=FButtonWidth, write=SetButtonWidth };
	__property bool						AutoScale						= { read=FAutoScale, write=FAutoScale };

	__property TTime						Time								= { read=FTime, write=SetTime };
	__property AnsiString				DisplayFormat					= { read=FDisplayFormat, write=SetDisplayFormat };

	__property TNotifyEvent				OnTimeClick						= { read=FOnTimeClick, write=FOnTimeClick };
	__property TNotifyEvent				OnChange							= { read=FOnChange, write=FOnChange };
protected:
	TTime										FTime;
	void __fastcall						SetTime(TTime Time);
	AnsiString								FDisplayFormat;
	void __fastcall						SetDisplayFormat(AnsiString DisplayFormat);
	TGridButton								*GetButtonXY(int X, int Y) { return NULL; }
	TRect										GetButtonRect(const TGridButton *Button) { return TRect(0, 0, 0, 0); }
	void __fastcall						Paint(void);
	TRect										TextRect();

	const std::auto_ptr<TFont>			FTimeFont;
	Graphics::TFont* __fastcall		GetTimeFont();
	void __fastcall						SetTimeFont(Graphics::TFont* Value);
	void __fastcall						FOnFontChange(TObject* Sender);

	TGridButton								*FHourUpButton;
	TGridButton								*FHourDownButton;
	TGridButton								*FMinuteUpButton;
	TGridButton								*FMinuteDownButton;

	const std::auto_ptr<TTimer>		FScrollTimer;
	void __fastcall						ScrollTimeout(TObject *Sender);
	int										FInitialDelay;
	int										FRepeatDelay;

	int										FButtonWidth;
	void __fastcall						SetButtonWidth(int ButtonWidth);
	bool										FAutoScale;
	Extended									FScrollButtonRatio;

	TRect										ScrollButtonsRect[4];
	void										CalculateScrollButtonPos();
	TGridButton *							GetScrollButton(int X, int Y);
	int										GetMaxButtonSize();

	int										FGridBorderWidth;
	int										FBtnGapWidth;
	int										FBtnGapHeight;
	void	__fastcall						SetGridBorderWidth(int GridBorderWidth);
	void	__fastcall						SetBtnGapWidth(int Width);
	void	__fastcall						SetBtnGapHeight(int Width);

//	TColor									FButtonColor;
//	void	__fastcall						SetButtonColor(TColor Value);
	TColor									FArrowColor;
	TColor									FArrowBorderColor;
	int										FArrowScale;
	void	__fastcall						SetArrowColor(TColor Value);
	void	__fastcall						SetArrowBorderColor(TColor Value);
	void	__fastcall						SetArrowScale(int Value);

	DYNAMIC void __fastcall				MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall				MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall				MouseMove(Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall				WMCancelMode(TWMCancelMode& Message);

	TNotifyEvent							FOnTimeClick;
	TNotifyEvent							FOnChange;

private:
	TRect										FImageRect;

	TGridButton *							FCurrentClickedScrollButton;
	bool										FMouseInCurrentScrollButton;
	bool										FScroll;
	TGridButton								*FCurrentScrollButton;
	void										ApplyScroll();
	int										TimeText(TRect &Rect, HDC Handle, UnicodeString Str, bool Draw);
};
//---------------------------------------------------------------------------
} // namespace
using namespace Touchtime;
//---------------------------------------------------------------------------
#endif
