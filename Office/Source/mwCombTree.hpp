// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'mwCombTree.pas' rev: 6.00

#ifndef mwCombTreeHPP
#define mwCombTreeHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ImgList.hpp>	// Pascal unit
#include <ComCtrls.hpp>	// Pascal unit
#include <CommCtrl.hpp>	// Pascal unit
#include <Buttons.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Mwcombtree
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TmwScreenTreeView;
class PASCALIMPLEMENTATION TmwScreenTreeView : public Comctrls::TTreeView 
{
	typedef Comctrls::TTreeView inherited;
	
private:
	bool FMouseInControl;
	HIDESBASE MESSAGE void __fastcall CMMouseEnter(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMMouseLeave(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMMouseMove(Messages::TWMMouse &Message);
	HIDESBASE MESSAGE void __fastcall WMLButtonDown(Messages::TWMMouse &Message);
	
protected:
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	virtual void __fastcall CreateWnd(void);
public:
	#pragma option push -w-inl
	/* TCustomTreeView.Create */ inline __fastcall virtual TmwScreenTreeView(Classes::TComponent* AOwner) : Comctrls::TTreeView(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomTreeView.Destroy */ inline __fastcall virtual ~TmwScreenTreeView(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TmwScreenTreeView(HWND ParentWindow) : Comctrls::TTreeView(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TmwComboTree;
class PASCALIMPLEMENTATION TmwComboTree : public Stdctrls::TCustomEdit 
{
	typedef Stdctrls::TCustomEdit inherited;
	
__published:
	Buttons::TSpeedButton* Box;
	Comctrls::TTreeView* Tree;
	void __fastcall BoxClick(System::TObject* Sender);
	
private:
	int fImageIndex;
	int fStateIndex;
	Controls::TControlCanvas* FCanvas;
	HIDESBASE MESSAGE void __fastcall WMPaint(Messages::TWMPaint &Message);
	virtual void __fastcall WndProc(Messages::TMessage &Message);
	void __fastcall SetImageIndex(int NewValue);
	void __fastcall SetStateIndex(int NewValue);
	bool __fastcall ValidIndex(int Index);
	bool __fastcall ValidStateIndex(int Index);
	int __fastcall GetImageIndex(void);
	int __fastcall GetStateIndex(void);
	int __fastcall GetBoxWidth(void);
	void __fastcall SetBoxWidth(int NewValue);
	Graphics::TBitmap* __fastcall GetBoxGlyph(void);
	void __fastcall SetBoxGlyph(Graphics::TBitmap* NewValue);
	Imglist::TCustomImageList* __fastcall GetImages(void);
	void __fastcall SetImages(Imglist::TCustomImageList* NewValue);
	Comctrls::TTreeNodes* __fastcall GetItems(void);
	void __fastcall SetItems(Comctrls::TTreeNodes* NewValue);
	void __fastcall SetStateImages(Imglist::TCustomImageList* newValue);
	Imglist::TCustomImageList* __fastcall GetStateImages(void);
	void __fastcall SetTreeCursor(Controls::TCursor newValue);
	Controls::TCursor __fastcall GetTreeCursor(void);
	void __fastcall SetTreeEnabled(bool newValue);
	bool __fastcall GetTreeEnabled(void);
	void __fastcall SetTreeHeight(int newValue);
	int __fastcall GetTreeHeight(void);
	void __fastcall SetIndent(int newValue);
	int __fastcall GetIndent(void);
	void __fastcall SetTreeReadOnly(bool newValue);
	bool __fastcall GetTreeReadOnly(void);
	void __fastcall SetShowButtons(bool newValue);
	bool __fastcall GetShowButtons(void);
	void __fastcall SetTreeShowHint(bool newValue);
	bool __fastcall GetTreeShowHint(void);
	void __fastcall SetShowLines(bool newValue);
	bool __fastcall GetShowLines(void);
	void __fastcall SetShowRoot(bool newValue);
	bool __fastcall GetShowRoot(void);
	void __fastcall SetSortType(Comctrls::TSortType newValue);
	Comctrls::TSortType __fastcall GetSortType(void);
	void __fastcall SetTreeVisible(bool newValue);
	bool __fastcall GetTreeVisible(void);
	Comctrls::TTVChangedEvent __fastcall GetOnTreeChange();
	void __fastcall SetOnTreeChange(Comctrls::TTVChangedEvent NewValue);
	Comctrls::TTVChangingEvent __fastcall GetChanging();
	void __fastcall SetChanging(Comctrls::TTVChangingEvent NewValue);
	Classes::TNotifyEvent __fastcall GetOnTreeClick();
	void __fastcall SetOnTreeClick(Classes::TNotifyEvent NewValue);
	Comctrls::TTVExpandedEvent __fastcall GetOnCollapsed();
	void __fastcall SetOnCollapsed(Comctrls::TTVExpandedEvent NewValue);
	Comctrls::TTVCollapsingEvent __fastcall GetOnCollapsing();
	void __fastcall SetOnCollapsing(Comctrls::TTVCollapsingEvent NewValue);
	Comctrls::TTVCompareEvent __fastcall GetOnCompare();
	void __fastcall SetOnCompare(Comctrls::TTVCompareEvent NewValue);
	Classes::TNotifyEvent __fastcall GetOnTreeDblClick();
	void __fastcall SetOnTreeDblClick(Classes::TNotifyEvent NewValue);
	Comctrls::TTVExpandedEvent __fastcall GetOnDeletion();
	void __fastcall SetOnDeletion(Comctrls::TTVExpandedEvent NewValue);
	Comctrls::TTVEditedEvent __fastcall GetOnEdited();
	void __fastcall SetOnEdited(Comctrls::TTVEditedEvent NewValue);
	Comctrls::TTVEditingEvent __fastcall GetOnEditing();
	void __fastcall SetOnEditing(Comctrls::TTVEditingEvent NewValue);
	Classes::TNotifyEvent __fastcall GetOnTreeEnter();
	void __fastcall SetOnTreeEnter(Classes::TNotifyEvent NewValue);
	Classes::TNotifyEvent __fastcall GetOnTreeExit();
	void __fastcall SetOnTreeExit(Classes::TNotifyEvent NewValue);
	Comctrls::TTVExpandedEvent __fastcall GetOnExpanded();
	void __fastcall SetOnExpanded(Comctrls::TTVExpandedEvent NewValue);
	Comctrls::TTVExpandingEvent __fastcall GetOnExpanding();
	void __fastcall SetOnExpanding(Comctrls::TTVExpandingEvent NewValue);
	Comctrls::TTVExpandedEvent __fastcall GetOnGetImageIndex();
	void __fastcall SetOnGetImageIndex(Comctrls::TTVExpandedEvent NewValue);
	Comctrls::TTVExpandedEvent __fastcall GetOnGetSelectedIndex();
	void __fastcall SetOnGetSelectedIndex(Comctrls::TTVExpandedEvent NewValue);
	
protected:
	virtual void __fastcall CreateWindowHandle(const Controls::TCreateParams &Params);
	void __fastcall SetEditRect(void);
	virtual void __fastcall SetBounds(int Left, int Top, int Width, int Height);
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	virtual void __fastcall CreateWnd(void);
	
public:
	__fastcall virtual TmwComboTree(Classes::TComponent* AOwner);
	__fastcall virtual ~TmwComboTree(void);
	void __fastcall DrawImage(int Index, int Position);
	void __fastcall DrawStateImage(int Index, int Position);
	
__published:
	__property Font ;
	__property Text ;
	__property Imglist::TCustomImageList* Images = {read=GetImages, write=SetImages};
	__property int ImageIndex = {read=GetImageIndex, write=SetImageIndex, nodefault};
	__property int StateIndex = {read=GetStateIndex, write=SetStateIndex, nodefault};
	__property int BoxWidth = {read=GetBoxWidth, write=SetBoxWidth, nodefault};
	__property Graphics::TBitmap* BoxGlyph = {read=GetBoxGlyph, write=SetBoxGlyph};
	__property Comctrls::TTreeNodes* Items = {read=GetItems, write=SetItems};
	__property Imglist::TCustomImageList* StateImages = {read=GetStateImages, write=SetStateImages};
	__property Enabled  = {default=1};
	__property ReadOnly  = {default=0};
	__property ShowHint ;
	__property Visible  = {default=1};
	__property Controls::TCursor TreeCursor = {read=GetTreeCursor, write=SetTreeCursor, nodefault};
	__property bool TreeEnabled = {read=GetTreeEnabled, write=SetTreeEnabled, nodefault};
	__property int TreeHeight = {read=GetTreeHeight, write=SetTreeHeight, nodefault};
	__property int Indent = {read=GetIndent, write=SetIndent, nodefault};
	__property bool TreeReadOnly = {read=GetTreeReadOnly, write=SetTreeReadOnly, nodefault};
	__property bool ShowButtons = {read=GetShowButtons, write=SetShowButtons, nodefault};
	__property bool TreeShowHint = {read=GetTreeShowHint, write=SetTreeShowHint, nodefault};
	__property bool ShowLines = {read=GetShowLines, write=SetShowLines, nodefault};
	__property bool ShowRoot = {read=GetShowRoot, write=SetShowRoot, nodefault};
	__property Comctrls::TSortType SortType = {read=GetSortType, write=SetSortType, nodefault};
	__property bool TreeVisible = {read=GetTreeVisible, write=SetTreeVisible, nodefault};
	__property OnChange ;
	__property OnClick ;
	__property OnDblClick ;
	__property OnEnter ;
	__property OnExit ;
	__property Comctrls::TTVChangedEvent OnTreeChange = {read=GetOnTreeChange, write=SetOnTreeChange};
	__property Comctrls::TTVChangingEvent Changing = {read=GetChanging, write=SetChanging};
	__property Classes::TNotifyEvent OnTreeClick = {read=GetOnTreeClick, write=SetOnTreeClick};
	__property Comctrls::TTVExpandedEvent OnCollapsed = {read=GetOnCollapsed, write=SetOnCollapsed};
	__property Comctrls::TTVCollapsingEvent OnCollapsing = {read=GetOnCollapsing, write=SetOnCollapsing};
	__property Comctrls::TTVCompareEvent OnCompare = {read=GetOnCompare, write=SetOnCompare};
	__property Classes::TNotifyEvent OnTreeDblClick = {read=GetOnTreeDblClick, write=SetOnTreeDblClick};
	__property Comctrls::TTVExpandedEvent OnDeletion = {read=GetOnDeletion, write=SetOnDeletion};
	__property Comctrls::TTVEditedEvent OnEdited = {read=GetOnEdited, write=SetOnEdited};
	__property Comctrls::TTVEditingEvent OnEditing = {read=GetOnEditing, write=SetOnEditing};
	__property Classes::TNotifyEvent OnTreeEnter = {read=GetOnTreeEnter, write=SetOnTreeEnter};
	__property Classes::TNotifyEvent OnTreeExit = {read=GetOnTreeExit, write=SetOnTreeExit};
	__property Comctrls::TTVExpandedEvent OnExpanded = {read=GetOnExpanded, write=SetOnExpanded};
	__property Comctrls::TTVExpandingEvent OnExpanding = {read=GetOnExpanding, write=SetOnExpanding};
	__property Comctrls::TTVExpandedEvent OnGetImageIndex = {read=GetOnGetImageIndex, write=SetOnGetImageIndex};
	__property Comctrls::TTVExpandedEvent OnGetSelectedIndex = {read=GetOnGetSelectedIndex, write=SetOnGetSelectedIndex};
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TmwComboTree(HWND ParentWindow) : Stdctrls::TCustomEdit(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall Register(void);

}	/* namespace Mwcombtree */
using namespace Mwcombtree;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// mwCombTree
