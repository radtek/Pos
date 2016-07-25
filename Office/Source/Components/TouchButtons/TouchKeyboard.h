//---------------------------------------------------------------------------
#ifndef TouchKeyboardH
#define TouchKeyboardH
//---------------------------------------------------------------------------
#include "TouchControls.h"
//---------------------------------------------------------------------------
namespace Touchkeyboard
{
//---------------------------------------------------------------------------
struct TBtnText
{
	TBtnText(AnsiString UA, char UC, AnsiString SA, char SC, bool US=true) :
			UnshiftedCaption(UA), UnshiftedChar(UC), ShiftedCaption(SA), ShiftedChar(SC), UseShiftedWhenLocked(US) {}

	AnsiString	UnshiftedCaption;
	char			UnshiftedChar;

	AnsiString	ShiftedCaption;
	char			ShiftedChar;

	bool			UseShiftedWhenLocked;
};
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TKeyboardMouseEvent)(System::TObject* Sender, char Char);
//---------------------------------------------------------------------------
class PACKAGE TTouchKeyboard : public TCustomTouchButtonControl
{
public:
	__fastcall TTouchKeyboard(TComponent* Owner);

__published:
	__property								BevelOuter						= { default=bvLowered };
	__property int							GridBorderWidth				= { read=FGridBorderWidth, write=SetGridBorderWidth, default=4 };
	__property int							ButtonGapWidth					= { read=FBtnGapWidth, write=SetBtnGapWidth };
	__property int							ButtonGapHeight				= { read=FBtnGapHeight, write=SetBtnGapHeight };
	__property TColor						ButtonColor						= { read=FButtonColor, write=SetButtonColor };

	__property bool						Shift								= { read=FShift, write=SetShift };
	__property bool						CapsLock							= { read=FCapsLock, write=SetCapsLock };

	__property TKeyboardMouseEvent	OnClick							= { read=FOnClick, write=FOnClick };
protected:
	TGridButton								*GetButtonXY(int X, int Y);
	TRect										GetButtonRect(const TGridButton *Button);
	void __fastcall						Paint(void);

	void										SetCaptions();
	const TBtnText &						GetBtnText(int ButtonIndex);

	bool										FShift;
	bool										FCapsLock;
	void	__fastcall						SetShift(bool Value);
	void	__fastcall						SetCapsLock(bool Value);

	int										FGridBorderWidth;
	int										FBtnGapWidth;
	int										FBtnGapHeight;
	void	__fastcall						SetGridBorderWidth(int GridBorderWidth);
	void	__fastcall						SetBtnGapWidth(int Width);
	void	__fastcall						SetBtnGapHeight(int Width);

	TColor									FButtonColor;
	void	__fastcall						SetButtonColor(TColor Value);

	DYNAMIC void __fastcall				MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall				MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);

	TKeyboardMouseEvent					FOnClick;
};
//---------------------------------------------------------------------------
} // namespace
using namespace Touchkeyboard;
//---------------------------------------------------------------------------
#endif
