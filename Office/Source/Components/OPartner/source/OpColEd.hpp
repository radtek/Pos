// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpColEd.pas' rev: 6.00

#ifndef OpColEdHPP
#define OpColEdHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ToolWin.hpp>	// Pascal unit
#include <ImgList.hpp>	// Pascal unit
#include <OpShared.hpp>	// Pascal unit
#include <RTLConsts.hpp>	// Pascal unit
#include <VCLEditors.hpp>	// Pascal unit
#include <DesignEditors.hpp>	// Pascal unit
#include <DesignIntf.hpp>	// Pascal unit
#include <OpDesign.hpp>	// Pascal unit
#include <ComCtrls.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opcoled
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum OpColEd__1 { esCanUndo, esCanRedo, esCanCut, esCanCopy, esCanPaste, esCanDelete, esCanZOrder, esCanAlignGrid, esCanEditOle, esCanTabOrder, esCanCreationOrder, esCanPrint, esCanSelectAll };
#pragma option pop

typedef Set<OpColEd__1, esCanUndo, esCanSelectAll>  TEditState;

#pragma option push -b-
enum TEditAction { eaUndo, eaRedo, eaCut, eaCopy, eaPaste, eaDelete, eaSelectAll, eaPrint, eaBringToFront, eaSendToBack, eaAlignToGrid };
#pragma option pop

#pragma option push -b-
enum TNodeType { ntCollection, ntItem };
#pragma option pop

struct TNodeData;
typedef TNodeData *pNodeData;

#pragma pack(push, 4)
struct TNodeData
{
	TNodeType NodeType;
	Classes::TCollection* Collection;
	Classes::TCollectionItem* Item;
	int ItemId;
} ;
#pragma pack(pop)

class DELPHICLASS TfrmCollectionEditor;
class PASCALIMPLEMENTATION TfrmCollectionEditor : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
__published:
	Comctrls::TTreeView* trvCollection;
	Menus::TPopupMenu* pmnuCollection;
	Menus::TMenuItem* AddItem1;
	Menus::TMenuItem* DeleteItem1;
	Controls::TImageList* imglstCollection;
	Comctrls::TToolBar* ToolBar1;
	Comctrls::TToolButton* btnNewItem;
	Comctrls::TToolButton* btnDeleteItem;
	Comctrls::TToolButton* btnPrevLevel;
	Comctrls::TToolButton* btnNextLevel;
	Comctrls::TToolButton* ToolButton7;
	Comctrls::TToolButton* ToolButton4;
	Comctrls::TStatusBar* stsbrName;
	Menus::TPopupMenu* pmnuStatusBar;
	Menus::TMenuItem* Copy1;
	void __fastcall AddItem1Click(System::TObject* Sender);
	void __fastcall FormClose(System::TObject* Sender, Forms::TCloseAction &Action);
	void __fastcall trvCollectionChange(System::TObject* Sender, Comctrls::TTreeNode* Node);
	void __fastcall trvCollectionMouseUp(System::TObject* Sender, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	void __fastcall DeleteItem1Click(System::TObject* Sender);
	void __fastcall btnPrevLevelClick(System::TObject* Sender);
	void __fastcall btnNextLevelClick(System::TObject* Sender);
	Opshared::TOpNestedCollectionItem* __fastcall GetNodeItem(Comctrls::TTreeNode* Node);
	void __fastcall trvCollectionKeyDown(System::TObject* Sender, Word &Key, Classes::TShiftState Shift);
	void __fastcall FormCreate(System::TObject* Sender);
	void __fastcall Copy1Click(System::TObject* Sender);
	
private:
	Classes::TCollection* FCollection;
	Classes::TComponent* FComponent;
	Classes::TWndMethod FOldTreeProc;
	void __fastcall CustomTreeProc(Messages::TMessage &Message);
	void __fastcall FreeNodeData(void);
	void __fastcall ClearTree(void);
	Comctrls::TTreeNode* __fastcall AddTreeCollection(Comctrls::TTreeNode* Node, Classes::TCollection* Collection);
	void __fastcall AddItem(Classes::TCollection* Collection);
	Comctrls::TTreeNode* __fastcall AddTreeItem(Comctrls::TTreeNode* Node, Classes::TCollectionItem* NewItem);
	void __fastcall PopulateEditor(Comctrls::TTreeNode* Node, Opshared::TOpNestedCollection* Root);
	void __fastcall SetButtonState(void);
	Comctrls::TTreeNode* __fastcall FindNode(Classes::TCollection* Collection);
	void __fastcall CustomItemClick(System::TObject* Sender);
	
protected:
	Designintf::_di_IDesigner Designer;
	void __fastcall Activated(void);
	AnsiString __fastcall UniqueName(Classes::TComponent* Component);
	
public:
	__fastcall virtual ~TfrmCollectionEditor(void);
	/*         class method */ static void __fastcall CreateEditor(TMetaClass* vmt, Designintf::_di_IDesigner ADesigner, Classes::TComponent* Component, Classes::TCollection* Collection);
	/*         class method */ static void __fastcall FreeEditor(TMetaClass* vmt);
	void __fastcall ComponentDeleted(Classes::TComponent* Component);
	TEditState __fastcall GetEditState(void);
	void __fastcall EditAction(TEditAction Action);
	void __fastcall FormClosed(Forms::TCustomForm* AForm);
	void __fastcall FormModified(void);
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TfrmCollectionEditor(Classes::TComponent* AOwner) : Forms::TForm(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TfrmCollectionEditor(Classes::TComponent* AOwner, int Dummy) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TfrmCollectionEditor(HWND ParentWindow) : Forms::TForm(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Opcoled */
using namespace Opcoled;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpColEd
