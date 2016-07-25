//---------------------------------------------------------------------------
#ifndef TouchGridH
#define TouchGridH
//---------------------------------------------------------------------------
#include "TouchControls.h"
//---------------------------------------------------------------------------
namespace Touchgrid
{
//---------------------------------------------------------------------------
//enum TScrollDirection { sdHoriz, sdVert };
enum TScrollDirection { sdLeft, sdRight, sdUp, sdDown };
typedef void __fastcall (__closure *TScrollCompleteEvent)(System::TObject* Sender, TScrollDirection Direction);
//---------------------------------------------------------------------------
class TTouchGrid;  
//---------------------------------------------------------------------------
class TGridScrollThread : public TThread
{
public:
	__fastcall			TGridScrollThread(bool CreateSuspended);
	void __fastcall	Execute();

	void __fastcall	UpdateGrid(void);
	void __fastcall	CompletedScroll(void);
	TTouchGrid			*TouchGrid;

	void					Scroll(TScrollDirection iDirection, int iDistance, int iTime);

	bool					Paused;
	bool					UseThread;

private:
	int					Jump;
	TScrollDirection	FDirection;
	int					FDistance;
	unsigned int		FTime;

	TScrollDirection	Direction;
	int					Distance;
	unsigned int		Time;

	int					LastPos;
	int					DistanceLeft;
	unsigned int		StartTime;
	unsigned int		CurrentTime;
	LARGE_INTEGER		liClockFrequency;
	LARGE_INTEGER		liStartTime;
	LARGE_INTEGER		liCurrentTime;
	__int64				ptClockFrequency;
	__int64				ptStartTime;
	__int64				ptCurrentTime;
};
//---------------------------------------------------------------------------
enum TVertScrollButtonPos	{ vsNone, vsAbove, vsBelow, vsAboveAndBelow/*, vsScrollBarLeft, vsScrollBarRight*/ };
enum THorizScrollButtonPos	{ hsNone, hsLeft, hsRight, hsLeftAndRight/*, hsScrollBarTop, hsScrollBarBottom*/ };
//---------------------------------------------------------------------------
struct _TGridSize
{
	_TGridSize(int S, Extended R) : Size(S), Ratio(R) {}
	int Size;
	Extended Ratio;
};
typedef _TGridSize TColWidth, TRowHeight;
//---------------------------------------------------------------------------
class PACKAGE TTouchGrid : public TCustomTouchButtonControl
{
friend TGridScrollThread;
public:
	__fastcall TTouchGrid(TComponent* Owner);
	__fastcall virtual ~TTouchGrid();

	__property TGridButton				*Buttons[int Row][int Col]	= { read=GetButton };
	bool										GetButtonPos(const TGridButton *const Button, int &Row, int &Col) const;
	int										Row(const TGridButton *const Button) const;
	int										Col(const TGridButton *const Button) const;
	__property int							ColWidths[int Index]			= { read=GetColWidths, write=SetColWidths };
	__property int							RowHeights[int Index]		= { read=GetRowHeights, write=SetRowHeights };

	void										InsertRow(int Index);
	void										DeleteRow(int Index);

	void										InsertCol(int Index);
	void										DeleteCol(int Index);

	void										Scroll(TScrollDirection Direction);

//	void										ScrollInView(const TGridButton *const Button);
	void										SetVisibleRow(int Row);
	void										SetVisibleCol(int Col);

	TGridButton *							Selected();	// Returns the first one found or NULL

__published:
	__property								BevelInner						= { default=0 };
	__property								BevelOuter						= { default=bvLowered };
	__property								BevelWidth;
	__property								Tag;

	__property								BorderWidth						= { default=4} ;

	__property 								DefaultButtonColor;
	__property 								DefaultButtonLatchedColor;
	__property 								LatchingType;
	__property 								LatchingProperties;

	__property 								OnMouseDown;
	__property 								OnMouseUp;
	__property								OnMouseClick;
	__property								OnMouseMove;

	__property bool						AutoScale						= { read=FAutoScale, write=FAutoScale, default=false };

	__property bool						GridBorder						= { read=FGridBorder, write=SetGridBorder, default=true };
	__property int							GridBorderWidth				= { read=FGridBorderWidth, write=SetGridBorderWidth, default=4 };
	__property int							RowCount							= { read=FRowCount, write=SetRowCount };
	__property int							ColCount							= { read=FColCount, write=SetColCount };

	__property bool						ParentGridColor				= { read=FParentGridColor, write=SetParentGridColor, default=true };
	__property TColor						GridColor						= { read=FGridColor, write=SetGridColor, default=clBtnFace };

	__property int							DefaultColWidth				= { read=FDefaultColWidth, write=SetDefaultColWidth };
	__property int							DefaultRowHeight				= { read=FDefaultRowHeight, write=SetDefaultRowHeight };

	__property int							ButtonGapWidth					= { read=FBtnGapWidth, write=SetBtnGapWidth };
	__property int							ButtonGapHeight				= { read=FBtnGapHeight, write=SetBtnGapHeight };

	__property bool						AutoButtonWidth				= { read=FAutoBtnWidth, write=SetAutoBtnWidth };
	__property bool						AutoButtonHeight				= { read=FAutoBtnHeight, write=SetAutoBtnHeight };

	__property TVertScrollButtonPos	VertScrollButtonPos			= { read=FVertScrollButtonPos, write=SetVertScrollButtonPos };
	__property THorizScrollButtonPos	HorizScrollButtonPos			= { read=FHorizScrollButtonPos, write=SetHorizScrollButtonPos };

	__property int							VertScrollButtonHeight		= { read=FVertScrollButtonHeight, write=SetVertScrollButtonHeight };
	__property int							HorizScrollButtonWidth		= { read=FHorizScrollButtonWidth, write=SetHorizScrollButtonWidth };

	__property TGridButton				*ScrollBtnUp					= { read=FUpButton, write=FUpButton };
	__property TGridButton				*ScrollBtnDown					= { read=FDownButton, write=FDownButton };
	__property TGridButton				*ScrollBtnLeft					= { read=FLeftButton, write=FLeftButton };
	__property TGridButton				*ScrollBtnRight				= { read=FRightButton, write=FRightButton };

	__property TColor						ArrowColor						= { read=FArrowColor, write=SetArrowColor, default=clBlack };
	__property TColor						ArrowBorderColor				= { read=FArrowBorderColor, write=SetArrowBorderColor, default=clBlack };
	__property int							ArrowScale						= { read=FArrowScale, write=SetArrowScale, default=30 };

	__property int							HorizScrollTime				= { read=FHorizScrollTime, write=FHorizScrollTime, default=100 };
	__property int							HorizScrollDistance			= { read=FHorizScrollDistance, write=FHorizScrollDistance, default=100 };
	__property int							VertScrollTime					= { read=FVertScrollTime, write=FVertScrollTime, default=100 };
	__property int							VertScrollDistance			= { read=FVertScrollDistance, write=FVertScrollDistance, default=100 };

	__property TScrollCompleteEvent	OnScrollComplete				= { read=FOnScrollComplete, write=FOnScrollComplete };
protected:
	TGridButton								*GetButtonXY(int X, int Y);
	TGridButton* __fastcall				GetButton(int Row, int Col);
	TRect										GetButtonRect(const TGridButton *Button);
	TRect										GetButtonRect(int Row, int Col);
	int										GetMaxButtonSize();

	std::vector<TColWidth>				FColWidths;
	std::vector<TRowHeight>				FRowHeights;
	int	__fastcall						GetColWidths(int Index);
	void	__fastcall						SetColWidths(int Index, int Value);
	int	__fastcall						GetRowHeights(int Index);
	void	__fastcall						SetRowHeights(int Index, int Value);
	void										CalculateAutoSizes();

	bool										FAutoScale;

	int										FRowCount;
	int										FColCount;
	void	__fastcall						SetRowCount(int Count);
	void	__fastcall						SetColCount(int Count);
	int										FDefaultColWidth;
	int										FDefaultRowHeight;
	void	__fastcall						SetDefaultColWidth(int Count);
	void	__fastcall						SetDefaultRowHeight(int Count);

	bool										FAutoBtnWidth;
	bool										FAutoBtnHeight;
	void	__fastcall						SetAutoBtnWidth(bool Auto);
	void	__fastcall						SetAutoBtnHeight(bool Auto);

	bool										FGridBorder;
	void	__fastcall						SetGridBorder(bool GridBorder);

	int										FGridBorderWidth;
	void	__fastcall						SetGridBorderWidth(int GridBorderWidth);

	TColor									FGridColor;
	void	__fastcall						SetGridColor(TColor Value);
	bool										FParentGridColor;
	void	__fastcall						SetParentGridColor(bool Value);

	TVertScrollButtonPos					FVertScrollButtonPos;
	void	__fastcall						SetVertScrollButtonPos(TVertScrollButtonPos VertScrollButtonPos);
	THorizScrollButtonPos				FHorizScrollButtonPos;
	void	__fastcall						SetHorizScrollButtonPos(THorizScrollButtonPos HorizScrollButtonPos);

	void										CalculateVertScrollButtonPos();
	void										CalculateHorizScrollButtonPos();

	int										FVertScrollButtonHeight;
	Extended									FVertScrollButtonRatio;
	void	__fastcall						SetVertScrollButtonHeight(int Height);
	int										FHorizScrollButtonWidth;
	Extended									FHorizScrollButtonRatio;
	void	__fastcall						SetHorizScrollButtonWidth(int Width);

	void										HorizSmoothScroll(TScrollDirection Direction, int Distance, int Time);
	void										VertSmoothScroll(TScrollDirection Direction, int Distance, int Time);
	void										VerifyScrollPos();
//	std::auto_ptr<TGridButton>			FUpButton;
//	std::auto_ptr<TGridButton>			FDownButton;
//	std::auto_ptr<TGridButton>			FLeftButton;
//	std::auto_ptr<TGridButton>			FRightButton;
	TGridButton								*FUpButton;
	TGridButton								*FDownButton;
	TGridButton								*FLeftButton;
	TGridButton								*FRightButton;

	TRect										ScrollButtonsRect[4];
	TGridButton *							GetScrollButton(int X, int Y);
	TGridButton *							FCurrentClickedScrollButton;
	bool										FMouseInCurrentScrollButton;
	bool										FScroll;
	TGridButton								*FCurrentScrollButton;
	void										ApplyScroll();

	TColor									FArrowColor;
	TColor									FArrowBorderColor;
	int										FArrowScale;
	void	__fastcall						SetArrowColor(TColor Value);
	void	__fastcall						SetArrowBorderColor(TColor Value);
	void	__fastcall						SetArrowScale(int Value);

	virtual void __fastcall				CMFontChanged(TMessage& Message);
	virtual void __fastcall				WMSize(TWMSize& Message);
//	BEGIN_MESSAGE_MAP
//			MESSAGE_HANDLER(CM_FONTCHANGED, TMessage, CMFontChanged)
//	END_MESSAGE_MAP(TCustomControl)

	DYNAMIC void __fastcall				MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall				MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall				MouseMove(Classes::TShiftState Shift, int X, int Y);
//	TGridButton* __fastcall				GetUpButton();
//	TGridButton* __fastcall				GetDownButton();
//	TGridButton* __fastcall				GetLeftButton();
//	TGridButton* __fastcall				GetRightButton();

	int										FHorizScrollTime;
	int										FHorizScrollDistance;
	int										FVertScrollTime;
	int										FVertScrollDistance;

	void __fastcall						UpButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall						UpButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall						DownButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall						DownButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall						LeftButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall						LeftButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall						RightButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall						RightButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall						ScrollButtonsMouseMove(TObject* Sender, TShiftState Shift, TGridButton *GridButton, int X, int Y);

	void __fastcall						GridCompleteScroll(TScrollDirection Direction, int Distance);

	bool	__fastcall						GetUseThread();
	void	__fastcall						SetUseThread(bool UseThread);

	void	__fastcall						SetBtnGapWidth(int Width);
	void	__fastcall						SetBtnGapHeight(int Width);
	int										FBtnGapWidth;
	int										FBtnGapHeight;

	int										FHorizPosition;
	int										FVertPosition;

	TGridScrollThread						*ScrollThread;
	int __fastcall							GetHorizMax();
	int __fastcall							GetHorizMin();
	int __fastcall							GetVertMax();
	int __fastcall							GetVertMin();

	TScrollCompleteEvent					FOnScrollComplete;

	TRect										PanelRect;
	TRect										GridOuterRect;
	TRect										GridInnerRect;
	virtual void __fastcall				Paint(void);

	void __fastcall						DefineProperties(TFiler *Filer);
	void __fastcall						ReadButtons(TStream* Stream);
	void __fastcall						WriteButtons(TStream* Stream);
};
//---------------------------------------------------------------------------
} // namespace
using namespace Touchgrid;
//---------------------------------------------------------------------------
#endif

