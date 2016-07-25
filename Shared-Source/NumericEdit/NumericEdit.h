//---------------------------------------------------------------------------

#ifndef NumericEditH
#define NumericEditH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
enum TNumericType { ntGeneral, ntCurrency, ntPercent };
//---------------------------------------------------------------------------
class PACKAGE TNumericEdit : public TCustomEdit
{
private:
	bool							FormattingText;
	TCanvas						*FCanvas;
	TPoint __fastcall 		GetTextMargins();

	double						FValue;
	TAlignment					FAlignment;
	int							FPrecision;
	int							FDecimalPlaces;
	TNumericType				FNumericType;
	bool							FFixedPoint;

	void __fastcall			SetValue(double Value);
	void __fastcall			SetAlignment(TAlignment Value);
	void __fastcall			SetPrecision(int Value);
	void __fastcall			SetDecimalPlaces(int Value);
	void __fastcall			SetNumericType(TNumericType Value);
	void __fastcall			SetFixedPoint(bool Value);

	void							Value2FormattedText();
	void							Value2EditText();
	void							Text2Value();

	DYNAMIC void __fastcall	DoEnter(void);
	DYNAMIC void __fastcall	DoExit(void);

	// Write settings of '0' to the DFM
	void __fastcall DefineProperties(TFiler *Filer);
	void __fastcall ReadProperty(TReader *Reader);
	void __fastcall WriteProperty(TWriter *Writer);
	bool __fastcall DoWrite(int Value);
protected:
//	void __fastcall			CreateParams(Controls::TCreateParams &Params);
	DYNAMIC void __fastcall	KeyPress(wchar_t &Key);
	DYNAMIC void __fastcall Change(void);
	__property					Text;

	void __fastcall WMPaint(TWMPaint& Message);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_PAINT, TWMPaint, WMPaint)
	END_MESSAGE_MAP(TComponent)

__published:
	__property double			Value				= {read=FValue,			write=SetValue,			default=0};
	__property TAlignment	Alignment		= {read=FAlignment,		write=SetAlignment,		default=1};
	__property int				Precision		= {read=FPrecision,		write=SetPrecision};
	__property int				DecimalPlaces	= {read=FDecimalPlaces,	write=SetDecimalPlaces,	stored=false};
	__property TNumericType	NumericType		= {read=FNumericType,	write=SetNumericType};
	__property bool			FixedPoint		= {read=FFixedPoint,		write=SetFixedPoint};

	__property Anchors  = {default=3};
	__property AutoSelect  = {default=1};
	__property AutoSize  = {default=1};
	__property BevelEdges  = {default=15};
	__property BevelInner  = {index=0, default=2};
	__property BevelKind  = {default=0};
	__property BevelOuter  = {index=1, default=1};
	__property BiDiMode ;
	__property BorderStyle  = {default=1};
	__property CharCase  = {default=0};
	__property Color  = {default=-2147483643};
	__property Constraints ;
	__property Ctl3D ;
	__property DragCursor  = {default=-12};
	__property DragKind  = {default=0};
	__property DragMode  = {default=0};
	__property Enabled  = {default=1};
	__property Font ;
	__property HideSelection  = {default=1};
	__property ImeMode  = {default=3};
	__property ImeName ;
	__property MaxLength  = {default=0};
	__property OEMConvert  = {default=0};
	__property ParentBiDiMode  = {default=1};
	__property ParentColor  = {default=0};
	__property ParentCtl3D  = {default=1};
	__property ParentFont  = {default=1};
	__property ParentShowHint  = {default=1};
	__property PasswordChar  = {default=0};
	__property PopupMenu ;
	__property ReadOnly  = {default=0};
	__property ShowHint ;
	__property TabOrder  = {default=-1};
	__property TabStop  = {default=1};
	__property Visible  = {default=1};
	__property OnChange ;
	__property OnClick ;
	__property OnContextPopup ;
	__property OnDblClick ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDock ;
	__property OnEndDrag ;
	__property OnEnter ;
	__property OnExit ;
	__property OnKeyDown ;
	__property OnKeyPress ;
	__property OnKeyUp ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnStartDock ;
	__property OnStartDrag ;
public:
	__fastcall TNumericEdit(TComponent* Owner);
	__fastcall virtual ~TNumericEdit(void);
};
//---------------------------------------------------------------------------
#endif
