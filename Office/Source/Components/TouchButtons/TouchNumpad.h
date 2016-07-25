//---------------------------------------------------------------------------
#ifndef TouchNumpadH
#define TouchNumpadH
//---------------------------------------------------------------------------
#include "TouchControls.h"
//---------------------------------------------------------------------------
namespace Touchnumpad
{
//---------------------------------------------------------------------------
enum TNumpadKey {nk0, nk1, nk2, nk3, nk4, nk5, nk6, nk7, nk8, nk9, nkCustom, nkClear };
//---------------------------------------------------------------------------
enum TNumpadDisplayMode { dmCurrency, dmNumeric, dm12HrTime, dm24HrTime, dmPIN };
//---------------------------------------------------------------------------
typedef Currency		TNumeric;
typedef AnsiString	TPIN;
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TGetNumericTextEvent)(System::TObject* Sender, TNumeric Value, AnsiString &Text);
typedef void __fastcall (__closure *TGetTimeTextEvent)(System::TObject* Sender, TTime Value, AnsiString &Text);
//---------------------------------------------------------------------------
class TTouchNumpad;
class PACKAGE TNumpadDisplay : public TCustomPanel
{
friend TTouchNumpad;
public:
	__fastcall TNumpadDisplay(TComponent* Owner);

	TTime										Time();
	TNumeric									Numeric();
	TPIN										PIN();

	void										SetTime(TTime Time);
	void										SetNumeric(TNumeric Numeric);
	void										SetPIN(TPIN Pin);
	void										Clear();

__published:
	__property								Align;
	__property								Anchors;
	__property								Font;
	__property								Color;
	__property								ParentFont;
	__property								ParentColor;
	__property								Visible;
	__property								BevelInner						= {default=0};
	__property								BevelOuter						= {default=0};
	__property								BevelWidth						= {default=1};
	__property								BorderWidth						= {default=0};
	__property								BorderStyle						= {default=1};

	__property								OnClick;
	__property								OnDblClick;
	__property TGetNumericTextEvent	OnGetNumericText				= { read=FOnGetNumericText, write=FOnGetNumericText };
	__property TGetTimeTextEvent		OnGetTimeText					= { read=FOnGetTimeText, write=FOnGetTimeText };

	__property TNumpadDisplayMode		NumpadDisplayMode				= { read=FNumpadDisplayMode, write=SetNumpadDisplayMode };
	__property int							DecimalPlaces					= { read=FDecimalPlaces, write=SetDecimalPlaces, default=2 };

protected:
	TTouchNumpad *							FTouchNumpad;
	void										SetTouchNumpad(TTouchNumpad *TouchNumpad);
	void										KeyPressed(TNumpadKey Key);
	void										UpdateCurrency(TNumpadKey Key);
	AnsiString								GetCurrencyText();
	void										UpdateNumeric(TNumpadKey Key);
	AnsiString								GetNumericText();
	void										Update12HrTime(TNumpadKey Key);
	AnsiString								Get12HrTimeText();
	void										Update24HrTime(TNumpadKey Key);
	AnsiString								Get24HrTimeText();
	void										UpdatePIN(TNumpadKey Key);
	AnsiString								GetPINText();

	TNumpadDisplayMode					FNumpadDisplayMode;
	void __fastcall						SetNumpadDisplayMode(TNumpadDisplayMode Mode);

	void __fastcall						FOnNumpadClick(TObject *Sender, TNumpadKey Key);

	int										FDecimalPlaces;
	void __fastcall						SetDecimalPlaces(int DecimalPlaces);
	AnsiString								FText;

	bool										FInitialText;

	TGetNumericTextEvent					FOnGetNumericText;
	TGetTimeTextEvent						FOnGetTimeText;
	AnsiString __fastcall				GetText();
	void										AdjustColors(TPanelBevel Bevel, TColor &TopColor, TColor &BottomColor);
	virtual void __fastcall				Paint();
};
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TKeyboardMouseEvent)(System::TObject* Sender, TNumpadKey Key);
//---------------------------------------------------------------------------
enum TNumberingType { ntKeypadNumeric, ntTelephoneNumeric };
//---------------------------------------------------------------------------
const AnsiString KeypadNumericKeyText[]		= { "7", "8", "9", "4", "5", "6", "1", "2", "3", "0" };
const AnsiString TelephoneNumericKeyText[]	= { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };
//---------------------------------------------------------------------------
class PACKAGE TTouchNumpad : public TCustomTouchButtonControl
{
public:
	__fastcall TTouchNumpad(TComponent* Owner);

__published:
	__property								BorderStyle						= { default=bsSingle };
	__property int							GridBorderWidth				= { read=FGridBorderWidth, write=SetGridBorderWidth, default=4 };
	__property int							ButtonGapWidth					= { read=FBtnGapWidth, write=SetBtnGapWidth };
	__property int							ButtonGapHeight				= { read=FBtnGapHeight, write=SetBtnGapHeight };
	__property TColor						ButtonColor						= { read=FButtonColor, write=SetButtonColor };
	__property TNumberingType			NumberingType					= { read=FNumberingType, write=SetNumberingType, default=ntKeypadNumeric };

	__property bool						CustomKey						= { read=FCustomKey, write=SetCustomKey };
	__property AnsiString				CustomKeyCaption				= { read=FCustomKeyCaption, write=SetCustomKeyCaption };

	__property TNumpadDisplay *		NumpadDisplay					= { read=FNumpadDisplay, write=SetNumpadDisplay };

	__property TKeyboardMouseEvent	OnClick							= { read=FOnClick, write=FOnClick };
protected:
	TGridButton								*GetButtonXY(int X, int Y);
	TRect										GetButtonRect(const TGridButton *Button);
	void __fastcall						Paint(void);

	void										SetCaptions();

	AnsiString								FCustomKeyCaption;
	void __fastcall						SetCustomKeyCaption(AnsiString Caption);
	bool										FCustomKey;
	void __fastcall						SetCustomKey(bool CustomKey);

	TNumberingType							FNumberingType;
	void __fastcall						SetNumberingType(TNumberingType Value);

	int										FGridBorderWidth;
	int										FBtnGapWidth;
	int										FBtnGapHeight;
	void	__fastcall						SetGridBorderWidth(int GridBorderWidth);
	void	__fastcall						SetBtnGapWidth(int Width);
	void	__fastcall						SetBtnGapHeight(int Width);

	TColor									FButtonColor;
	void	__fastcall						SetButtonColor(TColor Value);

	DYNAMIC void __fastcall				MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);

	TKeyboardMouseEvent					FOnClick;

	TNumpadDisplay *						FNumpadDisplay;
	void __fastcall						SetNumpadDisplay(TNumpadDisplay *NumpadDisplay);
};
//---------------------------------------------------------------------------
} // namespace
using namespace Touchnumpad;
//---------------------------------------------------------------------------
#endif
