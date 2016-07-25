//---------------------------------------------------------------------------
#ifndef TouchControlsH
#define TouchControlsH
//---------------------------------------------------------------------------
#include <ExtCtrls.hpp>
#include <memory>
#include <vector>

#include "Color.h"
//---------------------------------------------------------------------------
#define BGR(b, g ,r)  ((DWORD) (((BYTE) (b) | ((WORD) (g) << 8)) | (((DWORD) (BYTE) (r)) << 16)))
#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
//---------------------------------------------------------------------------
namespace Touchcontrols
{
//---------------------------------------------------------------------------
class TCustomTouchButtonControl;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class PACKAGE TGridButton : public TPersistent
{
friend TCustomTouchButtonControl;

__published:
	__property TColor								Color							= { read=FColor, write=SetColor, stored=false };
	__property TColor								LatchedColor				= { read=FLatchedColor, write=SetLatchedColor, stored=false };
	__property TColor								DisabledColor				= { read=FDisabledColor, write=SetDisabledColor, stored=false };
	__property Graphics::TFont					*Font							= { read=GetFont, write=SetFont };
	__property bool								ParentFont					= { read=FParentFont, write=SetParentFont, default=true };
	__property TColor								FontColor					= { read=FFontColor, write=SetFontColor, stored=false };
	__property bool								ParentFontColor			= { read=FParentFontColor, write=SetParentFontColor, default=true };

	__property Graphics::TFont					*LatchedFont				= { read=GetLatchedFont, write=SetLatchedFont };
	__property bool								ParentLatchedFont			= { read=FParentLatchedFont, write=SetParentLatchedFont, default=true };
	__property TColor								LatchedFontColor			= { read=FLatchedFontColor, write=SetLatchedFontColor, stored=false };
	__property bool								ParentLatchedFontColor	= { read=FParentLatchedFontColor, write=SetParentLatchedFontColor, default=true };

	__property AnsiString						Caption						= { read=FCaption, write=SetCaption };
	__property bool								Enabled						= { read=FEnabled, write=SetEnabled, default=true };
	__property bool								Visible						= { read=FVisible, write=SetVisible, default=true };

	__property Graphics::TBitmap				*Glyph						= { read=GetGlyph, write=SetGlyph };
	__property int									GlyphAlpha					= { read=GetGlyphAlpha, write=SetGlyphAlpha };
	__property int									GlyphScale					= { read=FGlyphScale, write=SetGlyphScale, default=100 };
	__property bool								KeepGlyphAspect			= { read=FKeepGlyphAspect, write=SetGlyphAspect, default=true };
	__property bool								TransparentGlyph			= { read=FTransparentGlyph, write=SetTransparentGlyph, default=true };
	__property TColor								TransparentColor			= { read=FTransparentColor, write=SetTransparentColor, default=clBlack };

	__property int									Tag							= { read=FTag, write=FTag, default=0 };

public:
	__fastcall										TGridButton(TCustomTouchButtonControl* Owner);

	__property bool								Latched						= { read=FLatched, write=SetLatched };
	__property bool								Highlighted					= { read=FHighlighted, write=SetHighlighted };

protected:
	TCustomTouchButtonControl					*FOwner;

	void __fastcall								FOnFontChange(TObject* Sender);

	const std::auto_ptr<Graphics::TFont>	FFont;
	TColor											FFontColor;
	Graphics::TFont* __fastcall				GetFont();
	void __fastcall								SetFont(Graphics::TFont* Value);
	void __fastcall								SetFontColor(TColor Value);

	const std::auto_ptr<Graphics::TFont>	FLatchedFont;
	TColor											FLatchedFontColor;
	Graphics::TFont* __fastcall				GetLatchedFont();
	void __fastcall								SetLatchedFont(Graphics::TFont* Value);
	void __fastcall								SetLatchedFontColor(TColor Value);

	bool												FParentFont;
	void __fastcall								SetParentFont(bool Value);
	bool												FParentFontColor;
	void __fastcall								SetParentFontColor(bool Value);

	bool												FParentLatchedFont;
	void __fastcall								SetParentLatchedFont(bool Value);
	bool												FParentLatchedFontColor;
	void __fastcall								SetParentLatchedFontColor(bool Value);

	AnsiString										FCaption;
	void __fastcall								SetCaption(AnsiString Caption);

	const std::auto_ptr<TTimer>				FIndicatorTimer;
	void __fastcall								IndicatorTimeout(TObject *Sender);

	bool												FDown;
	bool												FHighlighted;
	void __fastcall								SetHighlighted(bool Highlighted);
	bool												FLatched;
	void __fastcall								SetLatched(bool Latched);

	TColor											FColor;
	void __fastcall								SetColor(TColor Color);
	TColor											FLatchedColor;
	void __fastcall								SetLatchedColor(TColor Color);
	TColor											FDisabledColor;
	void __fastcall								SetDisabledColor(TColor Color);

	bool												FEnabled;
	void __fastcall								SetEnabled(bool Enabled);
	bool												FVisible;
	void __fastcall								SetVisible(bool Visible);

	std::auto_ptr<Graphics::TBitmap>			const FGlyph;
	std::auto_ptr<Graphics::TBitmap>			const FStretchedGlyph;
	int												FGlyphAlpha;
	void __fastcall								SetGlyph(Graphics::TBitmap *Value);
	Graphics::TBitmap * __fastcall			GetGlyph();
	void												CreateStretchedGlyph();
	void __fastcall								SetGlyphAlpha(int Value);
	int __fastcall									GetGlyphAlpha();
	bool												FKeepGlyphAspect;
	void __fastcall								SetGlyphAspect(bool KeepAspect);
	TColor											FTransparentColor;
	void __fastcall								SetTransparentColor(TColor Color);
	bool												FTransparentGlyph;
	void __fastcall								SetTransparentGlyph(bool Transparent);
	int												FGlyphScale;
	void __fastcall								SetGlyphScale(int Scale);
	void												Invalidate();

	int												FTag;

private:
	void __fastcall								DefineProperties(TFiler *Filer);
	void __fastcall								ReadProperty1(TReader *Reader);
	void __fastcall								ReadProperty2(TReader *Reader);
	void __fastcall								ReadProperty3(TReader *Reader);
	void __fastcall								ReadProperty4(TReader *Reader);
	void __fastcall								ReadProperty5(TReader *Reader);
	void __fastcall								WriteProperty1(TWriter *Writer);
	void __fastcall								WriteProperty2(TWriter *Writer);
	void __fastcall								WriteProperty3(TWriter *Writer);
	void __fastcall								WriteProperty4(TWriter *Writer);
	void __fastcall								WriteProperty5(TWriter *Writer);
};
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TGridMouseEvent)		(System::TObject* Sender, TMouseButton Button, Classes::TShiftState Shift, TGridButton *GridButton, int X, int Y);
typedef void __fastcall (__closure *TGridClick)				(System::TObject* Sender, TMouseButton Button, Classes::TShiftState Shift, TGridButton *GridButton);
typedef void __fastcall (__closure *TGridMouseMoveEvent)	(System::TObject* Sender, Classes::TShiftState Shift, TGridButton *GridButton, int X, int Y);
//---------------------------------------------------------------------------
enum TLatchingType { ltNone, ltCheckbox, ltRadioButton };
//---------------------------------------------------------------------------
enum TLatchingKind { lkLatchColor, lkIndicator, lkStayDown };
typedef Set<TLatchingKind, lkLatchColor, lkStayDown>  TLatchingProperties;
//---------------------------------------------------------------------------
// Derive from TCustomPanel so we have BorderStyle
class PACKAGE TCustomTouchButtonControl : public TCustomPanel
{
friend TGridButton;
public:
	__fastcall TCustomTouchButtonControl(TComponent* Owner);
	__fastcall virtual ~TCustomTouchButtonControl();

	bool										DesignMode; //For internal use.

__published:
	__property								Align;
	__property								Anchors;
	__property								BorderWidth;
	__property								Font;
	__property								Color;
	__property								ParentFont;
	__property								ParentColor;
	__property								Visible;

	__property int							IndicatorDelay					= {read=FIndicatorDelay, write=FIndicatorDelay, default=300};
	__property bool						FontShadow						= {read=FFontShadow, write=SetFontShadow};
	__property bool						DisabledFontShadow			= {read=FDisabledFontShadow, write=SetDisabledFontShadow, default=true};

protected:
	__property TColor						DefaultButtonColor			= {read=FDefaultBtnColor, write=FDefaultBtnColor};
	__property TColor						DefaultButtonLatchedColor	= {read=FDefaultBtnLatchedColor, write=FDefaultBtnLatchedColor};
	__property TLatchingType			LatchingType					= { read=FLatchingType, write=SetLatchingType };
	__property TLatchingProperties	LatchingProperties			= {read=FLatchingProperties, write=FLatchingProperties};

	__property TGridMouseEvent 		OnMouseDown						= {read=FOnMouseDown, write=FOnMouseDown};
	__property TGridMouseEvent 		OnMouseUp						= {read=FOnMouseUp, write=FOnMouseUp};
	__property TGridClick		 		OnMouseClick					= {read=FOnMouseClick, write=FOnMouseClick};
	__property TGridMouseMoveEvent	OnMouseMove						= {read=FOnMouseMove, write=FOnMouseMove};

	std::auto_ptr<Graphics::TBitmap> const FImageBuffer;

	void										DrawCircle(Graphics::TBitmap *Bitmap, double CenterX, double CenterY, double Radius,
														double LineWidth, double Feather);

	std::vector<TGridButton *>			FButtons;
	DWORD										*FButtonBuffer;

	HCL										RGBToHCL(int col);
	int										HLCToRGB(HCL hcl);

	TColor									FDefaultBtnColor;
	TColor									FDefaultBtnLatchedColor;

	TGridMouseEvent						FOnMouseDown;
	TGridMouseEvent						FOnMouseUp;
	TGridClick								FOnMouseClick;
	TGridMouseMoveEvent					FOnMouseMove;

	TGridButton								*FCurrentClickedButton;
//	bool										FMouseInCurrentButton;
	DYNAMIC void __fastcall				MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall				MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall				MouseMove(Classes::TShiftState Shift, int X, int Y);

	void										SetButtonDown(TGridButton *GridButton);
	void										SetButtonUp(TGridButton *GridButton);
	void										SetButtonHighlight(TGridButton *GridButton, bool Highlight);

	bool										FFontShadow;
	inline void __fastcall				SetFontShadow(bool Enabled);
	bool										FDisabledFontShadow;
	inline void __fastcall				SetDisabledFontShadow(bool Enabled);

	TLatchingType							FLatchingType;
	virtual void __fastcall				SetLatchingType(TLatchingType LatchingType);
	TLatchingProperties					FLatchingProperties;
	int										FIndicatorDelay;

	virtual TRect							GetButtonRect(const TGridButton *Button)=0;
	virtual TGridButton					*GetButtonXY(int X, int Y) = 0;
	virtual void __fastcall				Paint(void) = 0;
	void										PaintButton(TGridButton *Button, TRect Rect, TRect ClipRect, Graphics::TBitmap *Bitmap, TColor BackgroundColor);
	void										PaintStandardButton(TGridButton *Button, TRect Rect, Graphics::TBitmap *Bitmap);
	void										PaintShadedButton(DWORD *Buffer, int ButtonWidth, int ButtonHeight, TColor ButtonColor, TColor BackgroundColor, bool ButtonHighlighted);
	void										PaintGradiantButton(TGridButton *Button, TRect Rect, Graphics::TBitmap *Bitmap);
	void										PaintRoundedButton(TGridButton *Button, TRect Rect, Graphics::TBitmap *Bitmap);

	virtual void __fastcall				WMEraseBkgnd(TMessage& Message);
	virtual void __fastcall				WMSize(TWMSize& Message);
	virtual void __fastcall				CMFontChanged(TMessage& Message);
	virtual void __fastcall				CMColorChanged(TMessage& Message);
	virtual void __fastcall				WMCancelMode(TWMCancelMode& Message);
	BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(WM_ERASEBKGND, TMessage, WMEraseBkgnd)
			MESSAGE_HANDLER(WM_SIZE, TWMSize, WMSize)
			MESSAGE_HANDLER(CM_FONTCHANGED, TMessage, CMFontChanged)
			MESSAGE_HANDLER(CM_COLORCHANGED, TMessage, CMColorChanged)
			MESSAGE_HANDLER(WM_CANCELMODE, TWMCancelMode, WMCancelMode);
	END_MESSAGE_MAP(TCustomControl)
};
//---------------------------------------------------------------------------
} // namespace
using namespace Touchcontrols;
//---------------------------------------------------------------------------
#endif
