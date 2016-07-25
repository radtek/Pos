//---------------------------------------------------------------------------
#ifndef TouchPagesH
#define TouchPagesH
//---------------------------------------------------------------------------
#include "TouchControls.h"
//---------------------------------------------------------------------------
namespace Touchpages
{
//---------------------------------------------------------------------------
enum TButtonPosition	{ bpLeft, bpRight, bpTop, bpBottom };
enum TTabPosition		{ tpTop, tpRight };
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TTabChangingEvent)(System::TObject* Sender, bool &AllowChange);
//---------------------------------------------------------------------------
class PACKAGE TTouchSheet : public Controls::TWinControl
{
friend class TTouchPages;
public:
	__fastcall virtual								TTouchSheet(Classes::TComponent* AOwner);
	__fastcall virtual								~TTouchSheet(void);

	__property TTouchPages*							PageControl		= {read=FPageControl, write=SetPageControl};
	__property int										TabIndex			= {read=GetTabIndex, nodefault};
	
__published:
	__property											BorderWidth		= {default=0};
	__property											Caption;
	__property											Color;
	__property bool									ParentColor		= {read=FParentColor, write=SetParentColor, default=false, stored=true };
	__property											Enabled			= {default=true};
	__property											Font;
	__property int										PageIndex		= {read=GetPageIndex, write=SetPageIndex, stored=false, nodefault};
	__property											ParentFont  	= {default=1};
	__property bool									TabVisible		= {read=FTabVisible, write=SetTabVisible, default=1};
	__property											Visible			= {stored=false, default=1};
	__property											OnMouseDown;
	__property											OnMouseMove;
	__property											OnMouseUp;

private:
	class TTouchPages*								FPageControl;
	bool													FTabVisible;
	bool													FTabShowing;
	int __fastcall										GetPageIndex(void);
	int __fastcall										GetTabIndex(void);
	void __fastcall									SetPageControl(TTouchPages* ATouchPages);
	void __fastcall									SetPageIndex(int Value);
	void __fastcall									SetTabShowing(bool Value);
	void __fastcall									SetTabVisible(bool Value);
	void __fastcall									UpdateTabShowing(void);

protected:
	virtual void __fastcall							ReadState(Classes::TReader* Reader);

	bool													FParentColor;
	void __fastcall									SetParentColor(bool Value);
	virtual void __fastcall							CMParentColorChanged(Messages::TMessage& Message);
	virtual void __fastcall							CMColorChanged(Messages::TMessage& Message);
	BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(CM_PARENTCOLORCHANGED, Messages::TMessage, CMParentColorChanged)
			MESSAGE_HANDLER(CM_COLORCHANGED, Messages::TMessage, CMColorChanged)
	END_MESSAGE_MAP(TWinControl)
};
//---------------------------------------------------------------------------
class PACKAGE TTouchPages : public TCustomTouchButtonControl
{
friend class TTouchSheet;
public:
	__fastcall								TTouchPages(TComponent* Owner);
	__fastcall virtual					~TTouchPages();
	TTouchSheet* __fastcall				FindNextPage(TTouchSheet* CurPage, bool GoForward, bool CheckTabVisible);
	void __fastcall						SelectNextPage(bool GoForward, bool CheckTabVisible = true);

	__property int							ActivePageIndex				= {read=GetActivePageIndex, write=SetActivePageIndex, nodefault};
	__property int							PageCount						= {read=GetPageCount, nodefault};
	__property TTouchSheet*				Pages[int Index]				= {read=GetPage};

__published:
	__property TTouchSheet*				ActivePage						= { read=FActivePage, write=SetActivePage };
	__property int							TabHeight						= { read=FTabHeight, write=SetTabHeight, default=48 };
	__property int							TabWidth							= { read=FTabWidth, write=SetTabWidth, default=100 };
	__property TColor						TabColor							= { read=FTabColor, write=SetTabColor };
	__property TTabPosition				TabPosition						= { read=FTabPosition, write=SetTabPosition };

	__property TTabChangingEvent		OnChanging						= { read=FOnChanging, write=FOnChanging };
	__property Classes::TNotifyEvent OnChange							= { read=FOnChange, write=FOnChange };

	__property								BevelOuter						= { default=bvRaised };

	__property								Align;
	__property								Anchors;
	__property								Font;
	__property								Color;
	__property								ParentFont;
	__property								ParentColor						= { default=true };
	__property								Visible;

protected:
	DYNAMIC bool __fastcall				CanChange(void);
	virtual bool __fastcall				CanShowTab(int TabIndex);
	DYNAMIC void __fastcall				Change(void);
	DYNAMIC void __fastcall				GetChildren(Classes::TGetChildProc Proc, Classes::TComponent* Root);
	DYNAMIC void __fastcall				SetChildOrder(Classes::TComponent* Child, int Order);
	virtual void __fastcall				ShowControl(Controls::TControl* AControl);

	TGridButton								*GetButtonXY(int X, int Y);
	TRect										GetButtonRect(const TGridButton *Button);
	virtual void __fastcall				Paint(void);

private:
	__property								BorderWidth;
	TList *									FPages;
	TTouchSheet*							FActivePage;

	int										FTabHeight;
	void __fastcall						SetTabHeight(int Height);
	int										FTabWidth;
	void __fastcall						SetTabWidth(int Width);
	TColor									FTabColor;
	void __fastcall						SetTabColor(TColor Color);

	TTabPosition							FTabPosition;
	void __fastcall						SetTabPosition(TTabPosition Position);
	DYNAMIC void __fastcall				MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);

	virtual void __fastcall				CustomAlignPosition(TControl* Control, int &NewLeft, int &NewTop,
													int &NewWidth, int &NewHeight, Types::TRect &AlignRect, const TAlignInfo &AlignInfo);

	TTabChangingEvent						FOnChanging;
	Classes::TNotifyEvent				FOnChange;
	void __fastcall						ChangeActivePage(TTouchSheet* Page);
	int __fastcall							GetActivePageIndex(void);
	TTouchSheet* __fastcall				GetPage(int Index);
	int __fastcall							GetPageCount(void);
	void __fastcall						SetActivePage(TTouchSheet* Page);
	void __fastcall						SetActivePageIndex(const int Value);
	void __fastcall						UpdateTab(TTouchSheet* Page);
	void __fastcall						SetTabs(Classes::TStrings* Value);
	void __fastcall						RemovePage(TTouchSheet* Page);
	void __fastcall						InsertPage(TTouchSheet* Page);

	void __fastcall						CMDesignHitTest(TCMDesignHitTest &Message);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(CM_DESIGNHITTEST, TCMDesignHitTest, CMDesignHitTest)
	END_MESSAGE_MAP(TCustomTouchButtonControl)
};
//---------------------------------------------------------------------------
} // namespace
using namespace Touchpages;
//---------------------------------------------------------------------------
#endif
