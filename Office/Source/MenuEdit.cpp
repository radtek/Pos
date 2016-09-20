
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Filectrl.hpp>
#include <Math.h>
#include <memory>

#include "MenuEdit.h"
#include ".\MenuEditor\CourseItemPricesForm.h"
#include ".\MenuEditor\CourseItemPricesController.h"
#include "MMData.h"
#include "Utilities.h"
#include "Main.h"
//#include "Dialog.h"
#include "StockData.h"
#include "MMRegistry.h"
#include "Consts.h"
#include "SelectMenu.h"
#include "SelectStockItem.h"
#include "Connections.h"
#include "GetBarcode.h"
#include "ErrorWarningSettings.h"
#include "SelectCategory.h"
#include "SelectMenuItem.h"
#include "Registration.h"
#include "MM_CSV.h"
//#include "Administration.h"

#include "LoadMenu.h"
#include "SaveMenu.h"
#include <algorithm>

#include <cassert>

#include "tinyxml.h"
#include "TaxProfile.h"
#include "InputCategory.h"
#include <map>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchButton"
#pragma link "NumericEdit"
#pragma link "TntStdCtrls"
//#pragma link "mwCombTree"
//#pragma link "Access_2K_SRVR"
#pragma link "touchbutton"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
#include "InputCategory.h"
TfrmMenuEdit *frmMenuEdit;

const MENU_PIC 			= ICON_NOTEPAD_INDEX;
const CLOSECOURSE_PIC 	= ICON_CLOSED_FOLDER_INDEX;
const OPENCOURSE_PIC 	= ICON_OPEN_FOLDER_INDEX;
const DISH_PIC 			= ICON_PIE_INDEX;
const CLOSEOPTION_PIC 	= ICON_FAVORITES_FOLDER_INDEX;
const OPENOPTION_PIC 	= ICON_FAVORITES_FOLDER_INDEX;
const OPTION_PIC 			= ICON_BOX_INDEX;



//const PRINTER_PIC			= 7;
//const CLASS_PIC			= 4;
//const BOTTLE_PIC			= 8;
//const GLASS_PIC			= 9;

//const DISABLED_PIC		= STATE_IMAGE_CROSS;

//const SEAT_PIC				= 11;
//const TABLE_PIC			= 12;
//const NOTE_PIC				= 0;
//const SAVE_PIC				= 13;
const DOLLARS_PIC			= ICON_MONEY_INDEX;
//const CREDIT_PIC      	= 15;
//const PALM_PIC         = 16;
//const PC_PIC           = 17;

//const NIP_PIC        	= 2;
//const SPLASH_PIC        = 3;
const DISABLEDSTATE_PIC = STATE_IMAGE_CROSS;

#define MAX_SHORT_NAME_LENGTH 10
#define MAX_CATEGORY_LENGTH 30
#define MAX_CATEGORY_GROUP_LENGTH 25
#define DEFAULT_ITEM_COLOUR clWhite
#define MAX_MENU_NAME_LENGTH 15
#define MAX_SIZE_NAME_LENGTH 30
#define MAX_COURSE_NAME_LENGTH 25
#define MAX_SERVING_COURSE_NAME_LENGTH 25
#define MAX_ITEM_NAME_LENGTH 50
#define MAX_OPTION_NAME_LENGTH 28

#define MAX_MENU_ITEMS_IN_EVALUATION_MODE 20

#define TST_PROMO_MASTER(i) (unsigned long)(i & 0x80000000)
#define TST_PROMO_ITEM(i) 	 (unsigned long)(i & 0x7FFFFFFF)
#define SET_PROMO_MASTER(i) (unsigned long)(i | 0x80000000)
#define CLR_PROMO_MASTER(i) (unsigned long)(i & 0x7FFFFFFF)

//#define NUMBER_OF_COURSES (tvMenu->Items->GetFirstNode()->Count - 2)

#define DEFAULT_SIZE_STRING "Default"

#define SIZES_INDEX 0
#define CATEGORY_GROUPS_INDEX 1
#define SERVING_COURSES_INDEX 2
#define FIRST_COURSE_INDEX 3



#define OBM_CHECKBOXES      32759

bool ClearingTree=false;	// Stop ~TItemNode trying to check for ForcedSides. Needs to be global. Fudge!

TTreeNode *
TfrmMenuEdit::get_current_course()
{
	TTreeNode *current_node = tvMenu->Selected;

	switch (reinterpret_cast<TEditorNode *>(current_node->Data)->NodeType) {
	case COURSE_NODE:
		return current_node;
	case ITEM_NODE:
	case ITEM_SIZE_NODE:
	case OPTION_NODE:
	case OPTIONS_NODE:
		for (TTreeNode *i = current_node; i != NULL; i = i->Parent) {
			const TEditorNode &node =
			*reinterpret_cast<TEditorNode *>(i->Data);
			if (node.NodeType == COURSE_NODE)
			return i;
		}
		assert(NULL
		&& "Parent chain didn't include a course node!");
		break;
	default:
		assert(NULL
		&& "Unknown node type encountered.");
		//return NULL;
	}
}

std::vector<TTreeNode *>
TfrmMenuEdit::get_all_item_sizes_in_course(
TTreeNode &treeview_course_node)
{
	std::vector<TTreeNode *> collected_nodes;

	assert(
	reinterpret_cast<TEditorNode *>(
	treeview_course_node.Data)->NodeType == COURSE_NODE);

	for (TTreeNode *node = treeview_course_node.getFirstChild();
	node != NULL; node = treeview_course_node.GetNextChild(node)) {
		const TEditorNode &editor_node =
		*reinterpret_cast<TEditorNode *>(node->Data);

		if (editor_node.NodeType != ITEM_NODE)
		continue;

		for (TTreeNode *size = node->getFirstChild();
		size != NULL; size = node->GetNextChild(size))
		collected_nodes.push_back(size);
	}

	return collected_nodes;
}

//---------------------------------------------------------------------------
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	TTreeNode *Node1 = (TTreeNode*)lParam1;
	TTreeNode *Node2 = (TTreeNode*)lParam2;
	bool Ascending = (bool)lParamSort;
	int GT = AnsiStrIComp(Node1->Text.c_str(), Node2->Text.c_str());
	if (!Ascending)
	{
		return -GT;
	}
	return GT;
}
//---------------------------------------------------------------------------

//**************************************************

class TIterationAction {
public:
	TIterationAction() { };
	virtual ~TIterationAction() { };

	virtual bool operator()(TItemSizeNode *) = 0;
	virtual bool operator()(TItemSizeNode *, __int32){
		return false;
	}
};

class TMatchAction : public TIterationAction {
	AnsiString &pattern;
public:
	TMatchAction(AnsiString &m)
	: pattern(m) { };
	~TMatchAction() { };

	inline bool operator()(TItemSizeNode *item) {
		return (item->ThirdPartyCode.Length() != 0 &&
		item->ThirdPartyCode == pattern);
	};
};

class TAssignmentAction : public TIterationAction {
	int assignment_base;

protected:
	inline AnsiString &nextId(AnsiString &target) {
		return target = IntToStr(assignment_base++);
	};

	inline AnsiString nextId() {
		return IntToStr(assignment_base++);
	};

public:
	TAssignmentAction(int assignment_base)
	: assignment_base(assignment_base) { };
	~TAssignmentAction() { };
};

class TReplaceAction : public TAssignmentAction {
public:
	TReplaceAction(int assignment_base)
	: TAssignmentAction(assignment_base) { };

	~TReplaceAction() { };

	inline bool operator()(TItemSizeNode *item) {
		return item->ThirdPartyCode = nextId(), false;
	}

	inline bool operator()(TItemSizeNode *item, __int32 tpcKey){
		item->ThirdPartyCode = nextId();
		item->ThirdPartyCodeKey = tpcKey;
		return false;
	}
};

class TSkipAction : public TAssignmentAction {
public:
	TSkipAction(int assignment_base)
	: TAssignmentAction(assignment_base) { };
	~TSkipAction() { };

	inline bool operator()(TItemSizeNode *item) {
		if (item->ThirdPartyCode.Length())
		return false;
		else {
			AnsiString id;
			bool result;

			while ((result =
			frmMenuEdit->IterateOverMenuItems(
			TMatchAction(nextId(id)))));

			item->ThirdPartyCode =                                                                                                             +
			!result ? id : item->ThirdPartyCode;

			return result;
		}
	}
};

//**************************************************

__fastcall TfrmMenuEdit::TfrmMenuEdit(TComponent* Owner)
: TForm(Owner),
frmPreview(new TfrmPreview(NULL)),
new_menu_element_key_generator_(menu_key_generator_t::Instance()),
itemsList(new TStringList),
forcedSideList(new TList)
{
	new_menu_element_key_generator_->Reset(-2, -1);

	dmMMData->Registered(&Registered, &Modules);

	//btnAddRecipe->Enabled = !(Modules & eMenuMateDirect);

	sgRecipe->Cells[0][0] = "Code";
	sgRecipe->Cells[1][0] = "Description";
	sgRecipe->Cells[2][0] = "Unit";

	if (Win32Platform == VER_PLATFORM_WIN32_NT)
	{
		tntedKitchenName->Font->Name = "MS Shell Dlg 2";
	}
	else
	{
		tntedKitchenName->Font->Name = "MS Shell Dlg";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::FormCreate(TObject *Sender)
{
	LastDrinkColour = DEFAULT_ITEM_COLOUR;

	sgOptions->DefaultRowHeight = ComboBox1->Height - 1;
	ComboBox1->Visible = false;
	ComboBox1->Text = "Select a group";

	Graphics::TBitmap *bmp;
	FCheck	= new Graphics::TBitmap;
	FNoCheck	= new Graphics::TBitmap;
	bmp		= new Graphics::TBitmap;
	try
	{
		bmp->Handle = LoadBitmap(0, MAKEINTRESOURCE(OBM_CHECKBOXES));
		// bmp now has a 4x3 bitmap of divers state images
		// used by checkboxes and radiobuttons
		// the first subimage is the unchecked box
		FNoCheck->Width = bmp->Width / 4;
		FNoCheck->Height = bmp->Height / 3;
		FNoCheck->Canvas->CopyRect(FNoCheck->Canvas->ClipRect, bmp->Canvas, FNoCheck->Canvas->ClipRect);

		// the second subimage is the checked box
		FCheck->Width = bmp->Width / 4;
		FCheck->Height = bmp->Height / 3;
		FCheck->Canvas->CopyRect(FCheck->Canvas->ClipRect, bmp->Canvas, TRect( FCheck->Width, 0, 2*FCheck->Width, FCheck->Height ));
	}
	__finally
	{
		delete bmp;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::FormDestroy(TObject *Sender)
{
	delete FCheck;
	delete FNoCheck;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::FormShow(TObject *Sender)
{
	PageControl1->ActivePage		= tsMenu;
	pcItemDetails->ActivePage		= tsItemDetails;
	pcItemSizeDetails->ActivePage	= tsItemSizePrice;
	ClearingTree						= false;
    ServingCoursesList = new TStringList;
	DeletedServingCoursesInfo.DeletedServingCourseVector.clear();

    //store no of price Levels
    if (qrGetNoOfPriceLevels->Database->Connected && !qrGetNoOfPriceLevels->Transaction->InTransaction)
    {
        qrGetNoOfPriceLevels->Transaction->StartTransaction();
        qrGetNoOfPriceLevels->Close();
        qrGetNoOfPriceLevels->ExecQuery();
        if(qrGetNoOfPriceLevels->RecordCount>0)
        {
            noOfPriceLevels				=	 qrGetNoOfPriceLevels->Fields[0]->AsInteger;
            allPriceLevels=  noOfPriceLevels;
        }
        else
        {
            noOfPriceLevels				=  2;
             allPriceLevels=  noOfPriceLevels;
        }
        qrGetNoOfPriceLevels->Transaction->Commit();
    }

    //insert priceLevels Name in map
    LoadPriceLevelNames();

}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::WMSysCommand(TWMSysCommand& Message)
{
	if ((Message.CmdType & 0xFFF0) == SC_MINIMIZE)
	{
		Application->Minimize();
	}
	else
	{
		TForm::Dispatch(&Message);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::Execute()
{
	itemsList->Clear();
	itemsList->Sorted = true;

	lbeTitle->Caption = "Loading...";
	LastLocation = "";

	PostMessage(Handle, WM_LOADMENU, 0, 0);
	ShowModal();


   }
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::WMLoadMenu(TMessage& Message)
{
	Update();
	try
	{
		if (!LoadMenu())
		{
			Close();
		}
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
		Close();
	}
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::RetrieveMMStock(AnsiString *Code, AnsiString *Desc, AnsiString *Unit,Currency *ConversionFactor,
AnsiString *ReductionUnit, AnsiString DefaultNewName)
{
	if (!dmStockData->dbStock->Connected)
	{
		return false;
	}
	frmSelectStockItem->Reset = true;
	frmSelectStockItem->NewItemName = DefaultNewName;
	if (frmSelectStockItem->ShowModal() == mrOk)
	{
		if (Code) *Code	= frmSelectStockItem->StockCode;
		if (Desc) *Desc	= frmSelectStockItem->Description;
		if (Unit) *Unit	= frmSelectStockItem->Unit;
		if (ReductionUnit) *ReductionUnit	= frmSelectStockItem->ReductionUnit;
		if (ConversionFactor) *ConversionFactor	= frmSelectStockItem->ConversionFactor;
        LastLocation = frmSelectStockItem->Location;
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RelabelDrinkCosts()
{
	tvMenu->Items->BeginUpdate();
	TTreeNode *ClassesNode = tvMenu->Items->GetFirstNode();
	for (int i = FIRST_COURSE_INDEX; i<ClassesNode->Count; i++)
	{
		TTreeNode *ClassNode = ClassesNode->Item[i];
		if (((TEditorNode *)ClassNode->Data)->NodeType == COURSE_NODE)
		{
			for (int j=0; j<ClassNode->Count; j++)
			{
				TTreeNode *DrinkNode = ClassNode->Item[j];

				if (((TEditorNode *)DrinkNode->Data)->NodeType == ITEM_NODE)
				{
					for (int k=0; k<DrinkNode->Count; k++)
					{
						TTreeNode *DrinkCostNode = DrinkNode->Item[k];
						TItemSizeNode *DCData = (TItemSizeNode *)DrinkCostNode->Data;

						if (((TEditorNode *)DrinkCostNode->Data)->Enabled)
						{
							DrinkCostNode->Text =  CurrToStrF(DCData->Price, ffFixed, CurrencyDecimals) +
							" (" +
							DCData->LongDescription +
							")";
							DrinkCostNode->StateIndex = -1;
						}
						else
						{
							DrinkCostNode->Text = "Disabled (" + DCData->LongDescription + ")";
							DrinkCostNode->StateIndex = DISABLEDSTATE_PIC;
						}
						((TItemSizeNode *)DrinkCostNode->Data)->LongDescription = DCData->LongDescription;
					}
					DrinkNode->StateIndex = (((TEditorNode *)DrinkNode->Data)->Enabled)?-1:DISABLEDSTATE_PIC;
				}
			}
			if (ClassNode)
			{
				if (((TEditorNode *)ClassNode->Data)->Enabled)
				{
					ClassNode->StateIndex = -1;
				}
				else
				{
					ClassNode->StateIndex = DISABLEDSTATE_PIC;
				}
			}
		}
	}
	tvMenu->Items->EndUpdate();
}
//---------------------------------------------------------------------------
TMenuNode::TMenuNode()
{
	NodeType = MENU_NODE;
}
//---------------------------------------------------------------------------
TTreeNode *TMenuNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == COURSE_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "New Course");
		NewNode->Data = new TCourseNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	if (N == SIZES_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "Available Sizes");
		NewNode->Data = new TSizesNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	if (N == CATEGORIES_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "Available Categories");
		NewNode->Data = new TCategoryGroupsNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	if (N == SERVING_COURSES_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "Available Serving Courses");
		NewNode->Data = new TServingCoursesNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TMenuNode::CopyNode(TTreeNode *S)        //
{
	if (((TEditorNode *)S->Data)->NodeType == COURSE_NODE)
	{
		((TTreeView *)Owner->TreeView)->Items->BeginUpdate();
		bool Expand = S->Expanded;
		TTreeNode *NewNode;
		NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, S->Text);
		NewNode->Data = ((TEditorNode *)S->Data)->Duplicate();
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		// NewNode is the new Course
		// S is the course being copied.

		// Now copy all the Items.
		// Loop through the Items for the course being copied.
		for (int i=0; i<S->Count; i++)
		{
			TTreeNode *DrinkToCopy = S->Item[i];   // Could be a sizes node
			bool Expand2 = DrinkToCopy->Expanded;
			TTreeNode *NewDrink = ((TTreeView *)Owner->TreeView)->Items->AddChild(NewNode, DrinkToCopy->Text);
			NewDrink->Data = ((TEditorNode *)DrinkToCopy->Data)->Duplicate();
			((TEditorNode *)NewDrink->Data)->Owner = NewDrink;

			// Now copy all the DrinkCosts.
			for (int j=0; j<DrinkToCopy->Count; j++)
			{
				TTreeNode *DrinkCostToCopy = DrinkToCopy->Item[j];
				bool Expand3 = DrinkCostToCopy->Expanded;
				((TEditorNode *)NewDrink->Data)->CopyNode(DrinkCostToCopy);
				NewDrink->Expanded = Expand3;
			}
			NewDrink->Expanded = Expand2;
		}

		Owner->TreeView->Selected = NewNode;
		NewNode->Expanded = Expand;
		((TTreeView *)Owner->TreeView)->Items->EndUpdate();
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TMenuNode::Duplicate()
{
	TMenuNode *NewNodeType;
	NewNodeType = new TMenuNode;
	CopyDetails(NewNodeType);
	return NewNodeType;
}
//---------------------------------------------------------------------------
int TMenuNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return MENU_PIC;
}
//---------------------------------------------------------------------------
int TMenuNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return MENU_PIC;
}
//---------------------------------------------------------------------------
bool TMenuNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == COURSE_NODE)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											SIZES_NODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TSizesNode::TSizesNode()
{
	NodeType = SIZES_NODE;
}
//---------------------------------------------------------------------------
TTreeNode *TSizesNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == SIZE_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "New Size");
		NewNode->Data = new TSizeNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TSizesNode::CopyNode(TTreeNode *S)
{
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TSizesNode::Duplicate()
{
	TSizesNode *NewNodeType = new TSizesNode;
	CopyDetails(NewNodeType);
	return NewNodeType;
}
//---------------------------------------------------------------------------
int TSizesNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return CLOSEOPTION_PIC;
}
//---------------------------------------------------------------------------
int TSizesNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPENOPTION_PIC;
}
//---------------------------------------------------------------------------
bool TSizesNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == SIZE_NODE)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											SIZE_NODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TSizeNode::TSizeNode()
{
	NodeType = SIZE_NODE;
	KitchenName = "";
	Weighed = false;
}
//---------------------------------------------------------------------------
TTreeNode *TSizeNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == SIZE_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, "New Size");
		NewNode->Data = new TSizeNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TSizeNode::CopyNode(TTreeNode *S)
{
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TSizeNode::Duplicate()
{
	TSizeNode *NewNodeType = new TSizeNode;
	CopyDetails(NewNodeType);
	NewNodeType->KitchenName = KitchenName;
	NewNodeType->Weighed = Weighed;
	return NewNodeType;
}
//---------------------------------------------------------------------------
int TSizeNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPTION_PIC;
}
//---------------------------------------------------------------------------
int TSizeNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPTION_PIC;
}
//---------------------------------------------------------------------------
bool TSizeNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == SIZE_NODE)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											CATEGORIES_NODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TCategoryGroupsNode::TCategoryGroupsNode()
{
	NodeType = CATEGORIES_NODE;
}
//---------------------------------------------------------------------------
TTreeNode *TCategoryGroupsNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == CATEGORY_GROUP_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "New Group");
		NewNode->Data = new TCategoryGroupNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TCategoryGroupsNode::CopyNode(TTreeNode *S)
{
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TCategoryGroupsNode::Duplicate()
{
	TCategoryGroupsNode *NewNodeType = new TCategoryGroupsNode;
	CopyDetails(NewNodeType);
	return NewNodeType;
}
//---------------------------------------------------------------------------
int TCategoryGroupsNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return CLOSEOPTION_PIC;
}
//---------------------------------------------------------------------------
int TCategoryGroupsNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPENOPTION_PIC;
}
//---------------------------------------------------------------------------
bool TCategoryGroupsNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == CATEGORY_NODE)
	{
		return true;
	}
	return false;

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											SERVING_COURSES_NODE   cww
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TServingCoursesNode::TServingCoursesNode()
{
	NodeType = SERVING_COURSES_NODE;
}
//---------------------------------------------------------------------------
TTreeNode *TServingCoursesNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == SERVING_COURSE_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "New Course");
		NewNode->Data = new TServingCourseNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TServingCoursesNode::CopyNode(TTreeNode *S)
{
	return NULL;
}
//---------------------------------------------------------------------------

TEditorNode *TServingCoursesNode::Duplicate()
{
	TServingCoursesNode *NewNodeType = new TServingCoursesNode;
	CopyDetails(NewNodeType);
	return NewNodeType;
}
//---------------------------------------------------------------------------
int TServingCoursesNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return CLOSEOPTION_PIC;
}
//---------------------------------------------------------------------------
int TServingCoursesNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPENOPTION_PIC;
}
//---------------------------------------------------------------------------
bool TServingCoursesNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == SERVING_COURSE_NODE)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											CATEGORY_GROUP_NODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TCategoryGroupNode::TCategoryGroupNode()
{
	NodeType = CATEGORY_GROUP_NODE;
	Deleted = false;
}
//---------------------------------------------------------------------------
TTreeNode *TCategoryGroupNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == CATEGORY_GROUP_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, "New Group");
		NewNode->Data = new TCategoryGroupNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		return NewNode;
	}
	if (N == CATEGORY_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "New Categoy");

		SetDeleted(false);
		
		TCategoryNode *node = new TCategoryNode;
		NewNode->Data = node;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TCategoryGroupNode::CopyNode(TTreeNode *S)
{
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TCategoryGroupNode::Duplicate()
{
	TCategoryGroupNode *NewNodeType = new TCategoryGroupNode;
	CopyDetails(NewNodeType);
	NewNodeType->Deleted = Deleted;
	return NewNodeType;
}
//---------------------------------------------------------------------------
int TCategoryGroupNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return CLOSECOURSE_PIC;
}
//---------------------------------------------------------------------------
int TCategoryGroupNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return CLOSECOURSE_PIC;
}
//---------------------------------------------------------------------------
bool TCategoryGroupNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == CATEGORY_NODE)
	{
		return true;
	}
	else if (((TEditorNode *)N->Data)->NodeType == CATEGORY_GROUP_NODE)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
void TCategoryGroupNode::SetDeleted(bool deleted)
{
	Deleted = deleted;
	Owner->StateIndex = -1;
	if (Deleted)
	{
		Owner->StateIndex = DISABLEDSTATE_PIC;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											CATEGORY_NODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TCategoryNode::TCategoryNode()
{
	NodeType = CATEGORY_NODE;
	Deleted = false;
}
//---------------------------------------------------------------------------
TTreeNode *TCategoryNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == CATEGORY_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "New Categoy");
		TCategoryNode *node = new TCategoryNode();
		node->Owner = NewNode;
		NewNode->Data = node;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TCategoryNode::CopyNode(TTreeNode *S)
{
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TCategoryNode::Duplicate()
{
	TCategoryNode *NewNodeType = new TCategoryNode();
	CopyDetails(NewNodeType);
	NewNodeType->Deleted = Deleted;
	return NewNodeType;
}
//---------------------------------------------------------------------------
int TCategoryNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPTION_PIC;
}
//---------------------------------------------------------------------------
int TCategoryNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPTION_PIC;
}
//---------------------------------------------------------------------------
bool TCategoryNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == CATEGORY_NODE)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
void TCategoryNode::SetDeleted(bool deleted)
{
	Deleted = deleted;
	Owner->StateIndex = -1;
	if (Deleted)
	{
		Owner->StateIndex = DISABLEDSTATE_PIC;
	}
	else
	{
		((TCategoryGroupNode *)(Owner->Parent->Data))->SetDeleted(false);
	}
}
//---------------------------------------------------------------------------
//											SERVING_COURSE_NODE  cww
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TServingCourseNode::TServingCourseNode()
{
	NodeType = SERVING_COURSE_NODE;
	NewServingCourse = false;
}
//---------------------------------------------------------------------------
TTreeNode *TServingCourseNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == SERVING_COURSE_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, "New Course");
		NewNode->Data = new TServingCourseNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TServingCourseNode::CopyNode(TTreeNode *S)
{
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TServingCourseNode::Duplicate()
{
	TServingCourseNode *NewNodeType = new TServingCourseNode;
	CopyDetails(NewNodeType);
	return NewNodeType;
}
//---------------------------------------------------------------------------
int TServingCourseNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPTION_PIC;
}
//---------------------------------------------------------------------------
int TServingCourseNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPTION_PIC;
}
//---------------------------------------------------------------------------
bool TServingCourseNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == SERVING_COURSE_NODE)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------
void TServingCourseNode::Enable()
{
	Enabled = true;
	Owner->StateIndex = -1;
}

//---------------------------------------------------------------------------
void TServingCourseNode::Disable()
{
	Enabled = false;
	Owner->StateIndex = DISABLEDSTATE_PIC;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											COURSE_NODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TCourseNode::TCourseNode()
{
	NodeType = COURSE_NODE;
	Enabled = true;
	Location = 0xFFFF;
	KitchenName = "";
	ServingCourse = "";
	NoDefaultServingCourse = false;
}

TTreeNode *
TCourseNode::GetOptionsNode()
{
	TTreeNode *candidateOptionTree;
	TTreeNode *parent = Owner;

	if (!parent \
			|| !(candidateOptionTree = parent->GetLastChild()) \
			|| candidateOptionTree->Text != "Options")
	return NULL;

	return candidateOptionTree;
}

//---------------------------------------------------------------------------
TTreeNode *TCourseNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == ITEM_NODE)
	{
		TTreeNode *NewNode;
		// Insert at Sizes node
		TTreeNode *OptionsNode = Owner->GetLastChild();
		if (OptionsNode)
		{
			NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(OptionsNode, "New Item");
		}
		else
		{
			NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "");
		}
		NewNode->Data = new TItemNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	if (N == COURSE_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, "New Course");
		NewNode->Data = new TCourseNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	if (N == OPTIONS_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "Options");
		NewNode->Data = new TOptionsNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TCourseNode::CopyNode(TTreeNode *S)
{
	if (((TEditorNode *)S->Data)->NodeType == ITEM_NODE)
	{
        bool Expand = S->Expanded;
		TTreeNode *NewNode;
		TTreeNode *OptionsNode = Owner->GetLastChild();
        if (OptionsNode)
		{
			NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(OptionsNode, S->Text);        //
		}
		else
		{
			NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, S->Text);
		}

		NewNode->Data = ((TEditorNode *)S->Data)->Duplicate();
		((TEditorNode *)NewNode->Data)->Owner = NewNode;

		// Now copy all the DrinkCosts.
		for (int i=0; i<S->Count; i++)
		{
			((TEditorNode *)NewNode->Data)->CopyNode(S->Item[i]);
		}

		Owner->TreeView->Selected = NewNode;
		NewNode->Expanded = Expand;
		return NewNode;
	}
	if (((TEditorNode *)S->Data)->NodeType == COURSE_NODE)
	{
		((TTreeView *)Owner->TreeView)->Items->BeginUpdate();
		bool Expand = S->Expanded;
		TTreeNode *NewNode;
		NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, S->Text);
		NewNode->Data = ((TEditorNode *)S->Data)->Duplicate();
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		// NewNode is the new Course
		// S is the course being copied.

		// Now copy all the Items.
		// Loop through the Items for the course being copied.
		for (int i=0; i<S->Count; i++)
		{
			TTreeNode *DrinkToCopy = S->Item[i];   // Could be a Sizes or options node.
			bool Expand2 = DrinkToCopy->Expanded;
			TTreeNode *NewDrink = ((TTreeView *)Owner->TreeView)->Items->AddChild(NewNode, DrinkToCopy->Text);
			NewDrink->Data = ((TEditorNode *)DrinkToCopy->Data)->Duplicate();
			((TEditorNode *)NewDrink->Data)->Owner = NewDrink;

			// Now copy all the DrinkCosts. Could be Sizes. Could be options.
			for (int j=0; j<DrinkToCopy->Count; j++)
			{
				TTreeNode *DrinkCostToCopy = DrinkToCopy->Item[j];
				bool Expand3 = DrinkCostToCopy->Expanded;
				((TEditorNode *)NewDrink->Data)->CopyNode(DrinkCostToCopy);
				NewDrink->Expanded = Expand3;
			}
			NewDrink->Expanded = Expand2;
		}

		Owner->TreeView->Selected = NewNode;
		NewNode->Expanded = Expand;
		((TTreeView *)Owner->TreeView)->Items->EndUpdate();
		return NewNode;
	}
	if (((TEditorNode *)S->Data)->NodeType == OPTIONS_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, S->Text);
		NewNode->Data = ((TEditorNode *)S->Data)->Duplicate();
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TCourseNode::Duplicate()
{
	TCourseNode *NewNodeType = new TCourseNode;

	CopyDetails(NewNodeType);

	NewNodeType->Location               = Location;
	NewNodeType->KitchenName            = KitchenName;
	NewNodeType->HandheldName           = HandheldName;
	NewNodeType->ServingCourse          = ServingCourse;
	NewNodeType->ServingCourseKey       = ServingCourseKey;
	NewNodeType->NoDefaultServingCourse = NoDefaultServingCourse;

	return NewNodeType;
}
//---------------------------------------------------------------------------
void TCourseNode::Enable()
{
	Enabled = true;
	Owner->StateIndex = -1;
	for (int i=0; i<Owner->Count; i++)
	{
		TEditorNode *NodeData = (TEditorNode *)Owner->Item[i]->Data;
		if (NodeData->NodeType == ITEM_NODE || NodeData->NodeType == OPTIONS_NODE)
		{
			NodeData->Enable();
		}
	}
}
//---------------------------------------------------------------------------
void TCourseNode::Disable()
{
	Enabled = false;
	Owner->StateIndex = DISABLEDSTATE_PIC;
	for (int i=0; i<Owner->Count; i++)
	{
		TEditorNode *NodeData = (TEditorNode *)Owner->Item[i]->Data;
		if (NodeData->NodeType == ITEM_NODE || NodeData->NodeType == OPTIONS_NODE)
		{
			NodeData->Disable();
		}
	}
}
//---------------------------------------------------------------------------
int TCourseNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return CLOSECOURSE_PIC;
}
//---------------------------------------------------------------------------
int TCourseNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPENCOURSE_PIC;
}
//---------------------------------------------------------------------------
bool TCourseNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == ITEM_NODE ||
			((TEditorNode *)N->Data)->NodeType == COURSE_NODE)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											ITEM_NODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TItemNode::TItemNode()
{
	NodeType					= ITEM_NODE;
	Colour					= frmMenuEdit->LastDrinkColour;
	ItemOnlySide			= false;
	Enabled					= true;
	Condiments				= new TList;
	ForcedSides				= new TList;
	KitchenName				= "";
	PrinterOptions.PrintUnderlined 	= false;
	PrinterOptions.PrintBold 			= false;
	PrinterOptions.PrintColour 		= clBlack;
	PrinterOptions.PrintFont 			= 0;
	PrinterOptions.PrintDoubleWidth  = false;
	PrinterOptions.PrintDoubleHeight = false;
}
//---------------------------------------------------------------------------
__fastcall TItemNode::~TItemNode()
{
	delete Condiments;
	for (int i=0; i<ForcedSides->Count; i++)
	{
		TForcedSide *ForcedSide = (TForcedSide *)ForcedSides->Items[i];
		delete ForcedSide;
	}
	delete ForcedSides;

	if (!ClearingTree)	// Will slow down needlessly if deleteing all items.
	{
		// Delete forced sides that linked to this item.
		TTreeNode *MenuNode = ((TTreeView *)Owner->TreeView)->Items->GetFirstNode();
		for (int i = FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
		{
			TTreeNode *CourseNode = MenuNode->Item[i];
			if (((TEditorNode *)CourseNode->Data)->NodeType == COURSE_NODE)
			{
				for (int j=0; j<CourseNode->Count-1; j++)
				{
					TTreeNode *ItemNode = CourseNode->Item[j];
					for (int k=0; k<((TItemNode *)ItemNode->Data)->ForcedSides->Count;)
					{
						TForcedSide *ForcedSide = (TForcedSide *)((TItemNode *)ItemNode->Data)->ForcedSides->Items[k];
						if (ForcedSide->ItemNode == this)
						{
							delete ForcedSide;
							((TItemNode *)ItemNode->Data)->ForcedSides->Delete(k);
						}
						else
						{
							k++;
						}
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
TTreeNode *TItemNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == ITEM_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, "New Item");
		NewNode->Data = new TItemNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		return NewNode;
	}
	if (N == ITEM_SIZE_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "");
		NewNode->Data = new TItemSizeNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TItemNode::CopyNode(TTreeNode *S)
{
	if (((TEditorNode *)S->Data)->NodeType == ITEM_SIZE_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, S->Text);
		NewNode->Data = ((TEditorNode *)S->Data)->Duplicate();
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		return NewNode;
	}
	if (((TEditorNode *)S->Data)->NodeType == ITEM_NODE)
	{
		bool Expand = S->Expanded;
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, S->Text);
		NewNode->Data = ((TEditorNode *)S->Data)->Duplicate();
		((TEditorNode *)NewNode->Data)->Owner = NewNode;

		// Now copy all the DrinkCosts.
		for (int i=0; i<S->Count; i++)
		{
			((TEditorNode *)NewNode->Data)->CopyNode(S->Item[i]);
		}

		Owner->TreeView->Selected = NewNode;
		NewNode->Expanded = Expand;
		return NewNode;
	}
	if (((TEditorNode *)S->Data)->NodeType == OPTION_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner->Parent->GetLastChild(), S->Text);
		NewNode->Data = ((TEditorNode *)S->Data)->Duplicate();
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TItemNode::Duplicate()
{
	TItemNode *NewNodeType;
	NewNodeType								= new TItemNode;
	CopyDetails(NewNodeType);
	NewNodeType->Colour					= Colour;
	NewNodeType->ItemOnlySide			= ItemOnlySide;
	NewNodeType->KitchenName			= KitchenName;
	NewNodeType->HandheldName        = HandheldName;
	NewNodeType->DefaultStockRecipe	= DefaultStockRecipe;
	NewNodeType->PrinterOptions.PrintUnderlined	 = PrinterOptions.PrintUnderlined;
	NewNodeType->PrinterOptions.PrintBold			 = PrinterOptions.PrintBold;
	NewNodeType->PrinterOptions.PrintColour 	    = PrinterOptions.PrintColour;
	NewNodeType->PrinterOptions.PrintFont			 = PrinterOptions.PrintFont;
	NewNodeType->PrinterOptions.PrintDoubleHeight = PrinterOptions.PrintDoubleHeight;
	NewNodeType->PrinterOptions.PrintDoubleWidth  = PrinterOptions.PrintDoubleWidth;

	for (int i=0; i<Condiments->Count; i++)
	{
		NewNodeType->Condiments->Add(Condiments->Items[i]);
	}
	for (int i=0; i<ForcedSides->Count; i++)
	{
		TForcedSide *ForcedSide		= new TForcedSide;

		ForcedSide->Key	          = ((TForcedSide *)ForcedSides->Items[i])->Key;
		ForcedSide->ItemKey	      = ((TForcedSide *)ForcedSides->Items[i])->ItemKey;
		ForcedSide->MasterItemKey = ((TForcedSide *)ForcedSides->Items[i])->MasterItemKey;

		ForcedSide->CourseName	  = ((TForcedSide *)ForcedSides->Items[i])->CourseName;
		ForcedSide->ItemName	  = ((TForcedSide *)ForcedSides->Items[i])->ItemName;
		ForcedSide->ItemNode	  = ((TForcedSide *)ForcedSides->Items[i])->ItemNode;
		ForcedSide->GroupNumber	  = ((TForcedSide *)ForcedSides->Items[i])->GroupNumber;
		ForcedSide->MaxSelect	  = ((TForcedSide *)ForcedSides->Items[i])->MaxSelect;
		ForcedSide->SideGroupSkip = ((TForcedSide *)ForcedSides->Items[i])->SideGroupSkip;

		NewNodeType->ForcedSides->Add(ForcedSide);
	}
	return NewNodeType;
}
//---------------------------------------------------------------------------
void TItemNode::Enable()
{
	Enabled = true;
	Owner->StateIndex = -1;
	TTreeNode *TheClassNode = Owner->Parent;
	if (((TEditorNode *)TheClassNode->Data)->Enabled == false)
	{
		// Enable the course
		((TEditorNode *)TheClassNode->Data)->Enabled = true;
		TheClassNode->StateIndex = -1;
		//      // Enable options
		((TEditorNode *)TheClassNode->GetLastChild()->Data)->Enable();
	}
}
//---------------------------------------------------------------------------
void TItemNode::Disable()
{
	Enabled = false;
	Owner->StateIndex = DISABLEDSTATE_PIC;
	bool AllDisabled = true;
	for (int i=0; i<Owner->Parent->Count; i++)
	{
		if (((TEditorNode *)Owner->Parent->Item[i]->Data)->Enabled == true &&
				((TEditorNode *)Owner->Parent->Item[i]->Data)->NodeType == ITEM_NODE)
		{
			AllDisabled = false;
			break;
		}
	}
	if (AllDisabled)
	{
		// Disable the Course for this dish
		((TEditorNode *)Owner->Parent->Data)->Enabled = false;
		Owner->Parent->StateIndex = DISABLEDSTATE_PIC;
		//      // Disable the options
		((TEditorNode *)Owner->Parent->GetLastChild()->Data)->Disable();
	}
}
//---------------------------------------------------------------------------
int TItemNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return DISH_PIC;
}
//---------------------------------------------------------------------------
int TItemNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return DISH_PIC;
}
//---------------------------------------------------------------------------
bool TItemNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == ITEM_NODE)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											ITEM_SIZE_NODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TItemSizeNode::TItemSizeNode()
{
	NodeType  = ITEM_SIZE_NODE;
	Enabled	  = true;
	RecipeList	= new TList;
	CategoryList = new TStringList;
	Price = 0;
	MaxRetailPrice = 0;
	SpecialPrice = 0;
	GST	 = 0;
	Free = false;
	PointsPercent = 0;
	Cost = 0;
	CostGST	= 0;
	SetMenuMask	= 0;
	SettingMenuMask	= false;
	SetMenuStandardItem	= false;
	TareWeight = 0;
	MemberPurchaseCount	= 0;
	MemberPurchaseDiscount = 0;
	LocationPurchaseCount = 0;
	LocationPurchaseDiscount = 0;
	IsTaxExempt = false;
	HasServiceCharge = false;
	AvailableQuantity = DefaultQuantity = -1.0;
	WarningQuantity = 0.0;
	DisableWhenCountReachesZero = false;
	DefaultPatronCount = 0;
	//MM1351
	KitchenName = "";
    CostForPoints = 0;
}
//---------------------------------------------------------------------------
__fastcall TItemSizeNode::~TItemSizeNode()
{
	while(RecipeList->Count != 0)
	{
		TRecipe *Recipe = (TRecipe *)RecipeList->Items[0];
		delete Recipe;
		RecipeList->Delete(0);
	}
	delete RecipeList;
	delete CategoryList;
}
//---------------------------------------------------------------------------
TTreeNode *TItemSizeNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == ITEM_SIZE_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, "");
		NewNode->Data = new TItemSizeNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TItemSizeNode::CopyNode(TTreeNode *S)
{
	if (((TEditorNode *)S->Data)->NodeType == ITEM_SIZE_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, S->Text);
		NewNode->Data = ((TEditorNode *)S->Data)->Duplicate();
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TItemSizeNode::Duplicate()
{
	TItemSizeNode *NewNodeType;
	NewNodeType										= new TItemSizeNode;
	CopyDetails(NewNodeType);
	NewNodeType->Price							 = Price;
    //Duplicating prices
    NewNodeType->ItemSizePriceLevels			= ItemSizePriceLevels;
    NewNodeType->PriceLevelsName			= PriceLevelsName;
	NewNodeType->MaxRetailPrice = MaxRetailPrice;
	NewNodeType->SpecialPrice					 = SpecialPrice;
	NewNodeType->GST								 = GST;
	NewNodeType->Free								 = Free;
	NewNodeType->PointsPercent					 = PointsPercent;
	NewNodeType->IsTaxExempt                = IsTaxExempt;
	NewNodeType->HasServiceCharge           = HasServiceCharge;
	NewNodeType->AvailableQuantity          = AvailableQuantity;
	NewNodeType->DefaultQuantity            = DefaultQuantity;
	NewNodeType->WarningQuantity            = WarningQuantity;
	NewNodeType->DisableWhenCountReachesZero = DisableWhenCountReachesZero;
	NewNodeType->SizeKey                   = SizeKey;
	NewNodeType->Cost								= Cost;
	NewNodeType->CostGST							= CostGST;
	NewNodeType->TareWeight						= TareWeight;
	NewNodeType->Barcode							= Barcode;
	NewNodeType->MemberPurchaseCount			= MemberPurchaseCount;
	NewNodeType->MemberPurchaseDiscount		= MemberPurchaseDiscount;
	NewNodeType->LocationPurchaseCount		= LocationPurchaseCount;
	NewNodeType->LocationPurchaseDiscount	= LocationPurchaseDiscount;
	NewNodeType->CategoryList->Assign(CategoryList);
	std::vector<Menu::TNameAndKey>::iterator nameKeyIT;
	for( nameKeyIT = BCategoryList.begin(); nameKeyIT != BCategoryList.end(); nameKeyIT++ )
	{
		Menu::TNameAndKey *nameKey = nameKeyIT;
		NewNodeType->BCategoryList.push_back( Menu::TNameAndKey( nameKey->Key, nameKey->Name ) );
	}

	for (int i=0; i<RecipeList->Count; i++)
	{
		TRecipe *NewRecipe	   = new TRecipe;

		NewRecipe->StockCode   = ((TRecipe *)RecipeList->Items[i])->StockCode;
		NewRecipe->Description = ((TRecipe *)RecipeList->Items[i])->Description;
		NewRecipe->Unit		   = ((TRecipe *)RecipeList->Items[i])->Unit;
		NewRecipe->Quantity	   = ((TRecipe *)RecipeList->Items[i])->Quantity;
		NewRecipe->Location	   = ((TRecipe *)RecipeList->Items[i])->Location;
		NewRecipe->ConversionFactor	   = ((TRecipe *)RecipeList->Items[i])->ConversionFactor;
		NewRecipe->ReductionUnit	   = ((TRecipe *)RecipeList->Items[i])->ReductionUnit;
		NewNodeType->RecipeList->Add(NewRecipe);
	}
	NewNodeType->CategoryKey	    = CategoryKey;
	NewNodeType->Category		    = Category;
	NewNodeType->ThirdPartyCode	    = ThirdPartyCode;
	NewNodeType->ThirdPartyCodeKey	= ThirdPartyCodeKey;
	NewNodeType->CanBePaidForUsingPoints = CanBePaidForUsingPoints;
	NewNodeType->DefaultPatronCount = DefaultPatronCount;
    NewNodeType->CostForPoints = CostForPoints;
	// Modified by DevPH
	TaxProfileKeyIterator tpIT;

	for( tpIT = TaxProfileKeys.begin(); tpIT != TaxProfileKeys.end(); tpIT++ )
	{
		NewNodeType->TaxProfileKeys.insert( *tpIT );
	}

	return NewNodeType;
}
//---------------------------------------------------------------------------
void TItemSizeNode::Enable()
{
	Enabled = true;
	Owner->Text = CurrencyString + CurrToStrF(Price, ffFixed, CurrencyDecimals) +
	" (" +
	LongDescription +
	")";

	Owner->StateIndex = -1;
}
//---------------------------------------------------------------------------
void TItemSizeNode::Disable()
{
	Enabled = false;
	Owner->Text = "Disabled (" + LongDescription + ")";
	Owner->StateIndex = DISABLEDSTATE_PIC;
}
//---------------------------------------------------------------------------
int TItemSizeNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return DOLLARS_PIC;
}
//---------------------------------------------------------------------------
int TItemSizeNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return DOLLARS_PIC;
}
//---------------------------------------------------------------------------
bool TItemSizeNode::CanExcept(TTreeNode *N)
{
	return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											OPTIONS_NODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TOptionsNode::TOptionsNode()
{
	NodeType = OPTIONS_NODE;
	Enabled = true;

}
//---------------------------------------------------------------------------
TTreeNode *TOptionsNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == OPTION_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, "");
		NewNode->Data = new TOptionNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TOptionsNode::CopyNode(TTreeNode *S)
{
	if (((TEditorNode *)S->Data)->NodeType == OPTION_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->AddChild(Owner, S->Text);
		NewNode->Data = ((TEditorNode *)S->Data)->Duplicate();
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TOptionsNode::Duplicate()
{
	TOptionsNode *NewNodeType;
	NewNodeType = new TOptionsNode;
	CopyDetails(NewNodeType);
	return NewNodeType;
}
//---------------------------------------------------------------------------
void TOptionsNode::Enable()
{
	// Don't enable if the course is disabled.
	if (((TEditorNode *)Owner->Parent->Data)->Enabled == false)
	{
		Application->MessageBox("You must enable the course first.", "Warning", MB_OK + MB_ICONWARNING);
	}
	else
	{
		Enabled = true;
		Owner->StateIndex = -1;
		for (int i=0; i<Owner->Count; i++)
		{
			TEditorNode *NodeData = (TEditorNode *)Owner->Item[i]->Data;
			if (NodeData->NodeType == OPTION_NODE)
			{
				NodeData->Enable();
			}
		}
	}
}
//---------------------------------------------------------------------------
void TOptionsNode::Disable()
{
	Enabled = false;
	Owner->StateIndex = DISABLEDSTATE_PIC;
	for (int i=0; i<Owner->Count; i++)
	{
		TEditorNode *NodeData = (TEditorNode *)Owner->Item[i]->Data;
		if (NodeData->NodeType == OPTION_NODE)
		{
			NodeData->Disable();
		}
	}
}
//---------------------------------------------------------------------------
int TOptionsNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return CLOSEOPTION_PIC;
}
//---------------------------------------------------------------------------
int TOptionsNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return CLOSEOPTION_PIC;
}
//---------------------------------------------------------------------------
bool TOptionsNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == OPTION_NODE)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//											OPTION_NODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TOptionNode::TOptionNode()
{
	NodeType = OPTION_NODE;
	Enabled = true;
	GroupNumber = 0;
	CondimentsMask = 0;
	PlusOption = true;
	MinusOption = false;
	KitchenName = "";
	MaxSelect = 1;
	OptionGroupSkip = false;
	PrinterOptions.PrintUnderlined 	= false;
	PrinterOptions.PrintBold 			= false;
	PrinterOptions.PrintColour 		= clBlack;
	PrinterOptions.PrintFont 			= 0;
	PrinterOptions.PrintDoubleWidth  = false;
	PrinterOptions.PrintDoubleHeight = false;
}
//---------------------------------------------------------------------------
__fastcall TOptionNode::~TOptionNode()
{
	if (!ClearingTree && Owner->Parent)	// Will slow down needlessly if deleteing all items.
	// Clearing trees seems to kill parents before children. Fudge!
	{
		// Delete forced sides that linked to this item.
		TTreeNode *CourseNode = Owner->Parent->Parent;
		if (((TEditorNode *)CourseNode->Data)->NodeType == COURSE_NODE)
		{
			for (int j=0; j<CourseNode->Count-1; j++)
			{
				TTreeNode *ItemNode = CourseNode->Item[j];
				TItemNode *ItemData = (TItemNode *)ItemNode->Data;
				for( int group_number = 1; group_number <= 8; group_number++ )
				{
					std::map<AnsiString, TForcedOption>::iterator it = ItemData->ForcedOptions[group_number].find(LongDescription);
					if  (it != ItemData->ForcedOptions[group_number].end())
					{
						ItemData->ForcedOptions[group_number].erase(it);
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
TTreeNode *TOptionNode::AddNode(NodeTypes N, bool Expand)
{
	if (N == OPTION_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, "");
		NewNode->Data = new TOptionNode;
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		Owner->Expanded = Expand;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TOptionNode::CopyNode(TTreeNode *S)
{
	if (((TEditorNode *)S->Data)->NodeType == OPTION_NODE)
	{
		TTreeNode *NewNode = ((TTreeView *)Owner->TreeView)->Items->Insert(Owner, S->Text);
		NewNode->Data = ((TEditorNode *)S->Data)->Duplicate();
		((TEditorNode *)NewNode->Data)->Owner = NewNode;
		return NewNode;
	}
	return NULL;
}
//---------------------------------------------------------------------------
TEditorNode *TOptionNode::Duplicate()
{
	TOptionNode *NewNodeType;
	NewNodeType							= new TOptionNode;
	CopyDetails(NewNodeType);
	NewNodeType->GroupNumber		= GroupNumber;
	NewNodeType->CondimentsMask	= CondimentsMask;
	NewNodeType->PlusOption			= PlusOption;
	NewNodeType->MinusOption		= MinusOption;
	NewNodeType->KitchenName		= KitchenName;
	NewNodeType->HandheldName     = HandheldName;
	NewNodeType->OptionGroupSkip	= OptionGroupSkip;
	NewNodeType->MaxSelect     	= MaxSelect;
	NewNodeType->PrinterOptions.PrintUnderlined	 = PrinterOptions.PrintUnderlined; //cww
	NewNodeType->PrinterOptions.PrintBold			 = PrinterOptions.PrintBold;
	NewNodeType->PrinterOptions.PrintColour 	    = PrinterOptions.PrintColour;
	NewNodeType->PrinterOptions.PrintFont			 = PrinterOptions.PrintFont;
	NewNodeType->PrinterOptions.PrintDoubleHeight = PrinterOptions.PrintDoubleHeight;
	NewNodeType->PrinterOptions.PrintDoubleWidth  = PrinterOptions.PrintDoubleWidth;
	return NewNodeType;
}
//---------------------------------------------------------------------------
void TOptionNode::Enable()
{
	// Don't enable if the course is disabled.
	if (((TEditorNode *)Owner->Parent->Parent->Data)->Enabled == false)
	{
		Application->MessageBox("You must enable the course first.", "Warning", MB_OK + MB_ICONWARNING);
	}
	else
	{
		Enabled = true;
		Owner->StateIndex = -1;
		// Make sure the Options Folder is enabled.
		((TEditorNode *)Owner->Parent->Data)->Enabled = true;
		Owner->Parent->StateIndex = -1;
	}
}
//---------------------------------------------------------------------------
void TOptionNode::Disable()
{
	Enabled = false;
	Owner->StateIndex = DISABLEDSTATE_PIC;
	bool AllDisabled = true;
	for (int i=0; i<Owner->Parent->Count; i++)
	{
		if (((TEditorNode *)Owner->Parent->Item[i]->Data)->Enabled == true &&
				((TEditorNode *)Owner->Parent->Item[i]->Data)->NodeType == OPTION_NODE)
		{
			AllDisabled = false;
			break;
		}
	}
	if (AllDisabled)
	{
		// Disable the Options Folder for this option
		((TEditorNode *)Owner->Parent->Data)->Enabled = false;
		Owner->Parent->StateIndex = DISABLEDSTATE_PIC;
	}
}
//---------------------------------------------------------------------------
int TOptionNode::GetImageIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPTION_PIC;
}
//---------------------------------------------------------------------------
int TOptionNode::GetSelectedIndex(TTreeView *DropTree, TTreeNode *DropNode)
{
	return OPTION_PIC;
}
//---------------------------------------------------------------------------
bool TOptionNode::CanExcept(TTreeNode *N)
{
	if (((TEditorNode *)N->Data)->NodeType == OPTION_NODE)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
//						End of Class Definitions
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuGetImageIndex(TObject *Sender,
TTreeNode *Node)
{
	Node->ImageIndex = ((TEditorNode *)Node->Data)->GetImageIndex(NULL, NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuGetSelectedIndex(
TObject *Sender, TTreeNode *Node)
{
	if (Node->DropTarget && tvMenu->Selected)
	{
		Node->SelectedIndex = ((TEditorNode *)Node->Data)->GetSelectedIndex(tvMenu, tvMenu->Selected);
	}
	else
	{
		Node->SelectedIndex = ((TEditorNode *)Node->Data)->GetSelectedIndex(NULL, NULL);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshMenuDetails()
{
	if (tvMenu->Selected)
	{
		lbeTitle->Caption += "  " + ((TEditorNode *)tvMenu->Selected->Data)->LongDescription; // cww
		if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_SIZE_NODE)
		{
			selectedItemSizeNode = (TItemSizeNode *)tvMenu->Selected->Data;

			lbeTitle->Caption = "Item Size";
			RefreshItemSize( selectedItemSizeNode );
			PageControl1->ActivePage = tsItemSize;
			RefreshItemRecipe( selectedItemSizeNode );

			setTaxProfilesInAssignerForItemSize( selectedItemSizeNode, menuTaxProfileProvider );
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_NODE)
		{
			lbeTitle->Caption = "Item";
			PageControl1->ActivePage = tsItem;
			RefreshItem((TItemNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == COURSE_NODE)
		{
			lbeTitle->Caption = "Course";
			PageControl1->ActivePage = tsCourse;
			RefreshCourse((TCourseNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == MENU_NODE)
		{
			lbeTitle->Caption = "Menu";
			PageControl1->ActivePage = tsMenu;
			RefreshMenu((TMenuNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == SIZES_NODE)
		{
			lbeTitle->Caption = "Sizes";
			PageControl1->ActivePage = tsAvailableSizes;
			RefreshSizes((TMenuNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == SIZE_NODE)
		{
			lbeTitle->Caption = "Size";
			PageControl1->ActivePage = tsSize;
			RefreshSize((TSizeNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == SERVING_COURSES_NODE) // cww
		{
			lbeTitle->Caption = "Serving Courses";
			PageControl1->ActivePage = tsServingCourses;
			RefreshServingCourses((TServingCoursesNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == SERVING_COURSE_NODE) // cww
		{
			lbeTitle->Caption = "Serving Course";
			PageControl1->ActivePage = tsServingCourse;
			RefreshServingCourse((TServingCourseNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == CATEGORIES_NODE)
		{
			lbeTitle->Caption = "Category Groups";
			PageControl1->ActivePage = tsCategoryGroups;
			RefreshCategoryGroups((TCategoryGroupsNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == CATEGORY_NODE)
		{
			lbeTitle->Caption = "Category";
			PageControl1->ActivePage = tsCategory;
			RefreshCategory((TCategoryNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == CATEGORY_GROUP_NODE)
		{
			lbeTitle->Caption = "Categories";
			PageControl1->ActivePage = tsCategories;
			RefreshCategories((TCategoryGroupNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == OPTIONS_NODE)
		{
			lbeTitle->Caption = "Options";
			PageControl1->ActivePage = tsOptions;
			RefreshOptions((TOptionsNode *)tvMenu->Selected->Data);
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == OPTION_NODE)
		{
			lbeTitle->Caption = "Option";
			PageControl1->ActivePage = tsOption;
			RefreshOption((TOptionNode *)tvMenu->Selected->Data);
		}
		else
		{
			lbeTitle->Caption = "";
			PageControl1->ActivePage = NULL;
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshItemRecipe(TItemSizeNode *DrinkCostData)
{ //Label20->Caption="Recipe Quantity" ;
	TList *RecipeList = DrinkCostData->RecipeList;
	if (RecipeList->Count == 0)
	{
		sgRecipe->RowCount = 2;
		sgRecipe->Cells[0][1] = "";
		sgRecipe->Cells[1][1] = "";
		sgRecipe->Cells[2][1] = "";

		try
		{
			edLoc->OnChange = NULL;
			edRecipeQty->OnChange = NULL;

			edRecipeQty->Value = 0;
			edRecipeQty->Enabled = false;
			edRecipeQty->Color = clSilver;
			edQty->Caption = 0;
			edLoc->Text = "";
			edLoc->Enabled = false;
			edLoc->Color = clSilver;
		}
		__finally
		{
			edLoc->OnChange = edLocChange;
			edRecipeQty->OnChange = edLocChange;
		}

		sgRecipe->Enabled = false;
		sgRecipe->Color = clSilver;
		btnAddRecipeToSizes->Enabled = false;
		btnAddRecipesToAllSizes->Enabled = false;
		btnAddRecipeToItems->Enabled = false;
		edItemCost->Enabled = true;
		edCostGST->Enabled = true;


	}
	else
	{
		sgRecipe->Row = 1;
		TRecipe *Recipe = (TRecipe *) RecipeList->Items[0];
		try
		{
        /*****************MM-6270*******************/

         	if (dmStockData->dbStock->Connected && !StockTransaction->InTransaction)
					{
						StockTransaction->StartTransaction();
					}
        if( Recipe->ReductionUnit== "")
        {
            qrStockDetails->Close();
            qrStockDetails->ParamByName( "Code"     )->AsString	=  Recipe->StockCode;
            qrStockDetails->ParamByName( "Location" )->AsString = Recipe->Location;
            qrStockDetails->ExecQuery();
            if( !qrStockDetails->Eof )
            {
                Recipe->ReductionUnit = qrStockDetails->FieldByName( "REDUCTION_UNIT"   )->AsString;

            }
        }

        		if (dmStockData->dbStock->Connected)
		{      StockTransaction->Commit();

		}
        /**************************************/
			if(Recipe->ReductionUnit!=NULL && Recipe->ReductionUnit!= "" )
			Label20->Caption="Recipe Quantity (" +  Recipe->ReductionUnit +")" ;
			else
			Label20->Caption="Recipe Quantity";

			edLoc->OnChange = NULL;
			edRecipeQty->OnChange = NULL;
			edRecipeQty->Value = Recipe->Quantity * Recipe->ConversionFactor;
			edQty->Caption = Recipe->Quantity;
			edLoc->Text = Recipe->Location;
		}
		__finally
		{
			edLoc->OnChange = edLocChange;
			edRecipeQty->OnChange = edLocChange;
		}

		sgRecipe->RowCount = RecipeList->Count + 1;
		Currency TotalItemCost = 0;
		for (int i=1; i<RecipeList->Count+1; i++)
		{
			TRecipe *Recipe = (TRecipe *) RecipeList->Items[i-1];
			sgRecipe->Cells[0][i] = Recipe->StockCode;
			sgRecipe->Cells[1][i] = Recipe->Description;
			sgRecipe->Cells[2][i] = Recipe->Unit;
			TotalItemCost += GetStockCost(Recipe->StockCode, Recipe->Location) * Recipe->Quantity;
            if(i==1)
            {
				edLoc->Items->Clear();
                LoadRecipeLocations(Recipe->StockCode);

            }
		}
		edItemCost->Enabled = false;
		//		edItemCost->Color = clBtnFace;
		edItemCost->Value = TotalItemCost;//0;
		edCostGST->Enabled = false;

		double GP = 0;
        //populating items prices and g.p 's

        for(int key =1; key<= noOfPriceLevels; key++)
        {

            Currency PriceExc = (DrinkCostData->ItemSizePriceLevels[key].Price * 100.0) / (100.0);
            if (PriceExc != 0)
            {
                GP = (1 - (edItemCost->Value / PriceExc)) * 100;
            }
            sgItemsize->Cells[2][key] = CurrToStrF(GP, ffFixed, CurrencyDecimals);
           //nePriceGP->Value = GP;

            GP = 0;
        }    

		edRecipeQty->Enabled = true;
		edRecipeQty->Color = lbCategories->Color;
		edLoc->Enabled = true;
		edLoc->Color = lbCategories->Color;
		sgRecipe->Enabled = true;
		sgRecipe->Color = lbCategories->Color;
		btnAddRecipeToSizes->Enabled = true;
		btnAddRecipesToAllSizes->Enabled = true;
		btnAddRecipeToItems->Enabled = true;
	}
	//btnAddRecipe->Enabled = !(Modules & eMenuMateDirect);
	btnDeleteRecipe->Enabled = true;

	sgRecipe->ColWidths[0] = sgRecipe->ClientWidth * 4 / 16;
	sgRecipe->ColWidths[1] = sgRecipe->ClientWidth * 9 / 16;
	sgRecipe->ColWidths[2] = sgRecipe->ClientWidth * 3 / 16 - 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::edLocChange(TObject *Sender)
{
	if (tvMenu->Selected)
	{
		if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_SIZE_NODE)
		{
			TItemSizeNode *ItemSizeData = (TItemSizeNode *)tvMenu->Selected->Data;

			TList *RecipeList = ItemSizeData->RecipeList;

			TRecipe *Recipe = (TRecipe *) RecipeList->Items[sgRecipe->Row-1];

			Recipe->Location = edLoc->Text;
			if(Recipe->ConversionFactor <= 0)
			{
				edQty->Caption = edRecipeQty->Value;
			}
			else
			{
				edQty->Caption = edRecipeQty->Value/Recipe->ConversionFactor;
			}
			Recipe->Quantity = edQty->Caption.ToDouble();
			MenuEdited = true;

			Currency TotalItemCost = 0;
			for (int i=0; i<RecipeList->Count; i++)
			{
				TRecipe *Recipe = (TRecipe *) RecipeList->Items[i];
				TotalItemCost += GetStockCost(Recipe->StockCode, Recipe->Location) * Recipe->Quantity;
			}
			edItemCost->Value = TotalItemCost;

			double GP = 0;

            for(int key =1; key<= noOfPriceLevels; key++)
            {
                Currency PriceExc = (ItemSizeData->ItemSizePriceLevels[key].Price * 100.0) / (100.0 + ItemSizeData->GST);
                if (PriceExc != 0)
                {
                    GP = (1 - (TotalItemCost / PriceExc)) * 100;
                }
                sgItemsize->Cells[2][key] = CurrToStrF(GP, ffFixed, CurrencyDecimals);
                GP = 0;
            }
		}
	}
}
//---------------------------------------------------------------------------
Currency TfrmMenuEdit::GetStockCost(AnsiString StockCode, AnsiString Location)
{
	Currency Cost = 0;
	if (dmStockData->dbStock->Connected)
	{
		if (!dmStockData->trStock->InTransaction) dmStockData->trStock->StartTransaction();
		dmStockData->IBQuery2->Close();
		dmStockData->IBQuery2->SQL->Text =
		"Select "
		"Average_Cost "
		"From "
		"Stock Inner Join StockLocation On "
		"Stock.Stock_Key = StockLocation.Stock_Key "
		"Where "
		"Stock.Code = :Code And "
		"StockLocation.Location = :Location";
		dmStockData->IBQuery2->ParamByName("Code")->AsString = StockCode;
		dmStockData->IBQuery2->ParamByName("Location")->AsString = Location;
		dmStockData->IBQuery2->Open();
		if (!dmStockData->IBQuery2->IsEmpty())
		{
			Cost = dmStockData->IBQuery2->FieldByName("Average_Cost")->AsFloat;
		}
		dmStockData->IBQuery2->Close();
		dmStockData->trStock->Commit();
	}
	return Cost;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshItemSize(TItemSizeNode *ItemSizeData)
{
	try
	{
		cbFreeItem->OnClick								= NULL;
		chbSetMenuStandardItem->OnClick				= NULL;
		chbSetMenuMaster->OnClick						= NULL;
		chbSetMenuItem->OnClick							= NULL;
		bool weighed = false;

        /*if()
        {

        }*/

       std::map<AnsiString, TAvailableMenuSize >::iterator it = AllSizesForMenu.find(ItemSizeData->LongDescription);

       if(it != AllSizesForMenu.end())
       {
          if(it->second.Weighed)
          {
             weighed = true;
          }
       }
		/*TTreeNode *SizesNode = tvMenu->Items->GetFirstNode()->Item[SIZES_INDEX];
		for (int i=0; i<SizesNode->Count; i++)
		{
			TSizeNode *SizeData = (TSizeNode *)SizesNode->Item[i]->Data;
			if (SizeData->LongDescription == ItemSizeData->LongDescription)
			{
				if (SizeData->Weighed)
				{
					weighed = true;
				}
				break;
			}
		}*/

		GroupBox1->Caption = "Prices";

		lbeTareWeight->Visible = weighed;
		neTareWeight->Visible = weighed;
		neTareWeight->Value = ItemSizeData->TareWeight;

		if (ItemSizeData->Free)
		{
			cbFreeItem->Checked							= true;
			edItemMaxPrice->Enabled							= false;
            sgItemsize->Enabled                     = false;
		}
		else
		{
			cbFreeItem->Checked							= false;
			edItemMaxPrice->Enabled							= true;
            sgItemsize->Enabled                     = true;
		}

      sgItemsize->RowCount =  noOfPriceLevels +1;
        for (int rowIndex=1; rowIndex<sgItemsize->RowCount; rowIndex++)
        {
             for(int colIndex=0; colIndex< 3; colIndex++)
            {
                sgItemsize->Cells[colIndex][rowIndex] = "";
                sgItemsize->Objects[colIndex][rowIndex] = NULL;
            }
        }

        sgItemsize->Cells[1][0] = "Price";
        sgItemsize->Cells[2][0] = "G.P.";
        int rowIndex = 1;
        //populating priceLevelsName in the grid
        std::map<int,Menu::TPriceLevelsName>::const_iterator grpStart = PriceLevelsName.begin();
        std::map<int,Menu::TPriceLevelsName>::const_iterator grpEnds = PriceLevelsName.end();
        for ( ; grpStart != grpEnds; ++grpStart )
        {
            sgItemsize->Cells[0][rowIndex] = (*grpStart).second.PriceLevelName;
            rowIndex++;
        }

        std::map<int,Menu::TItemSizePriceLevel>::const_iterator grpIT = ItemSizeData->ItemSizePriceLevels.begin();
        std::map<int,Menu::TItemSizePriceLevel>::const_iterator grpEnd = ItemSizeData->ItemSizePriceLevels.end();
        Currency GP = 0;
        rowIndex = 1;
       
        for ( ; grpIT != grpEnd; ++grpIT )
        {

                sgItemsize->Cells[1][rowIndex] = CurrToStrF((*grpIT).second.Price, ffFixed, CurrencyDecimals);
                Currency PriceExc = ((*grpIT).second.Price * 100.0) / (100.0);
                if (PriceExc != 0)
                {
                    GP = (1 - (ItemSizeData->Cost / PriceExc)) * 100;
                }
                sgItemsize->Cells[2][rowIndex] = CurrToStrF(GP, ffFixed, CurrencyDecimals);
                GP = 0;
                for(int i=0; i<3; i++)
                {
                    sgItemsize->Objects[0][(*grpIT).second.PriceLevelKey]	= ItemSizeData;
                }
                rowIndex++;
        }

		edItemCost->Value									= fabs(ItemSizeData->Cost);
		edItemMaxPrice->Value									= ItemSizeData->MaxRetailPrice;
		edCostGST->Value									= ItemSizeData->CostGST;
		nePointsPercent->Value							= ItemSizeData->PointsPercent;
		edDefaultPatronCount->Text =
		IntToStr(ItemSizeData->DefaultPatronCount);
		cbCanBePaidForUsingPoints->Checked =
		ItemSizeData->CanBePaidForUsingPoints;
		edItemMaxPrice->Value = ItemSizeData->MaxRetailPrice;

		if (ItemSizeData->MemberPurchaseCount == -1)  // discount always
		{
			neMemberPurchaseCount->Value					= 0;
			cbMemberDiscountAlways->Checked           = true;
		}
		else
		{
			neMemberPurchaseCount->Value					= ItemSizeData->MemberPurchaseCount;
			cbMemberDiscountAlways->Checked           = false;
		}
		cbMemberDiscountAlwaysClick(cbMemberDiscountAlways);
		neMemberPurchaseDiscount->Value				= ItemSizeData->MemberPurchaseDiscount;

		if (ItemSizeData->LocationPurchaseCount == -1)  // discount always
		{
			neLocationPurchaseCount->Value				  = 0;
			cbLocationDiscountAlways->Checked           = true;
		}
		else
		{
			neLocationPurchaseCount->Value			  	= ItemSizeData->LocationPurchaseCount;
			cbLocationDiscountAlways->Checked         = false;
		}
		cbLocationDiscountAlwaysClick(cbLocationDiscountAlways);
		neLocationPurchaseDiscount->Value			= ItemSizeData->LocationPurchaseDiscount;

		if (TST_PROMO_MASTER(ItemSizeData->SetMenuMask))
		{
			chbSetMenuMaster->Enabled					= true;
			chbSetMenuMaster->Checked					= true;

			lbSetMenuItems->Visible						= true;
			lbSetMenuMasters->Visible					= false;
			chbSetMenuStandardItem->Visible			= false;
			chbSetMenuItem->Checked						= false;
			chbSetMenuItem->Enabled						= false;
			chbSetMenuStandardItem->Checked			= false;
			chbSetMenuStandardItem->Enabled			= false;
			FindSetMenuItems(ItemSizeData->SetMenuMask);
			ClearSetMenuMasters();
		}
		else
		{
			ClearSetMenuItems();
			chbSetMenuMaster->Checked					= false;

			lbSetMenuItems->Visible						= false;
			lbSetMenuMasters->Visible					= true;
			chbSetMenuStandardItem->Visible			= true;
			if (ItemSizeData->SetMenuMask)			// Set Menu Item
			{
				chbSetMenuMaster->Enabled				= false;
				chbSetMenuItem->Enabled					= true;
				chbSetMenuItem->Checked					= true;
				chbSetMenuStandardItem->Enabled		= true;
				chbSetMenuStandardItem->Checked		= ItemSizeData->SetMenuStandardItem;
				FindSetMenuMasters(ItemSizeData->SetMenuMask);
			}
			else 												// Standard Item
			{
				chbSetMenuMaster->Enabled				= true;
				chbSetMenuMaster->Checked				= false;
				chbSetMenuItem->Enabled					= true;
				if (ItemSizeData->SettingMenuMask)
				{
					chbSetMenuItem->Checked				= true;
					chbSetMenuStandardItem->Checked	= ItemSizeData->SetMenuStandardItem;
					chbSetMenuStandardItem->Enabled	= true;
				}
				else
				{
					chbSetMenuItem->Checked				= false;
					chbSetMenuStandardItem->Checked	= true;
					chbSetMenuStandardItem->Enabled	= false;
				}
				if (chbSetMenuItem->Checked)
				{
					FindSetMenuMasters(ItemSizeData->SetMenuMask);
				}
				else
				{
					ClearSetMenuMasters();
				}
			}
		}

		lbCategories->Items->Clear();
		for (std::vector<Menu::TNameAndKey>::const_iterator i =
		ItemSizeData->BCategoryList.begin();
		i != ItemSizeData->BCategoryList.end(); ++i) {
			const Menu::TNameAndKey &name_key = *i;
			const int insertion_index = lbCategories->Items->Add(name_key.Name);
			lbCategories->Items->Objects[insertion_index] =
			reinterpret_cast<TObject *>(name_key.Key);
		}

		cbCategory->Items->Clear();
		TTreeNode *CategoryGroupsNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
		for (int i=0; i<CategoryGroupsNode->Count; i++)
		{
			TCategoryGroupsNode *CategoryGroupData = (TCategoryGroupsNode *)CategoryGroupsNode->Item[i]->Data;
			cbCategory->Items->AddObject(CategoryGroupData->LongDescription, (TObject *)0);
			for (int j=0; j<CategoryGroupsNode->Item[i]->Count; j++)
			{
				TCategoryNode *CategoryData = (TCategoryNode *)CategoryGroupsNode->Item[i]->Item[j]->Data;
				if (!CategoryData->GetDeleted())
				{
					cbCategory->Items->AddObject(CategoryData->LongDescription, (TObject *)1);
				}
			}
		}
		cbCategory->ItemIndex = cbCategory->Items->IndexOf(ItemSizeData->Category);
		cbCategory->DropDownCount = 16;

		cb3rdPartyGroupCode->Items->Clear();
		std::auto_ptr<TStringList> ThirdPartyGroups(new TStringList());
		GetAll3rdPartyGroups(ThirdPartyGroups.get());
		for (int i=0; i<ThirdPartyGroups->Count; i+=2)
		{
			cb3rdPartyGroupCode->Items->Add(ThirdPartyGroups->Strings[i]);
		}

		cb3rdPartyGroupCode->Text = ItemSizeData->ThirdPartyCode;
        if(ItemSizeData->CanBePaidForUsingPoints)
        {
           nePriceForPoint->Enabled = true;
           nePriceForPoint->Value = ItemSizeData->CostForPoints; // add cost for points.
        }
        else
        {
           nePriceForPoint->Enabled = false;
           nePriceForPoint->Value = 0.0; // add cost for points.
        }

	}
	__finally
	{
		cbFreeItem->OnClick = cbFreeItemClick;

		chbSetMenuMaster->OnClick = chbSetMenuMasterClick;
		chbSetMenuItem->OnClick	  = chbSetMenuItemClick;

		chbSetMenuStandardItem->OnClick   = chbSetMenuStandardItemClick;
		ItemSizeData->SettingMenuMask	    = false;
		ItemSizeData->SetMenuStandardItem = chbSetMenuStandardItem->Checked;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbCategoryDrawItem(TWinControl *Control,
int Index, TRect &Rect, TOwnerDrawState State)
{
	TRect OldRect = Rect;

	cbCategory->Canvas->Brush->Color = cbCategory->Color;
	cbCategory->Canvas->FillRect(Rect);

	if ((State.Contains(odSelected) || State.Contains(odFocused)) && cbCategory->Items->Objects[Index] == (TObject *)1)
	{
		cbCategory->Canvas->Brush->Color = clHighlight;
		cbCategory->Canvas->Font->Color = clHighlightText;
	}
	else
	{
		cbCategory->Canvas->Brush->Color = cbCategory->Color;
		cbCategory->Canvas->Font->Color = cbCategory->Font->Color;
	}
	if (State.Contains(odComboBoxEdit) && cbCategory->Items->Objects[Index] == (TObject *)1)
	{
		int CentredTop = (Rect.Top + Rect.Bottom - cbCategory->Canvas->TextHeight(cbCategory->Items->Strings[Index])) / 2;
		if (CentredTop < 0) CentredTop = 0;
		cbCategory->Canvas->TextRect(Rect, Rect.Left+2, CentredTop, cbCategory->Items->Strings[Index]);
	}
	else
	{
		if (State.Contains(odComboBoxEdit))
		{
			cbCategory->Canvas->Font->Color = clGray;
		}
		int TextTopOffset = (Rect.Top + Rect.Bottom - cbCategory->Canvas->TextHeight(cbCategory->Items->Strings[Index])) / 2;
		int TextWidth = cbCategory->Canvas->TextWidth(cbCategory->Items->Strings[Index]);

		if (cbCategory->Items->Objects[Index] == (TObject *)0)
		{
			dmUtilities->imlTreeImages->Draw(cbCategory->Canvas, Rect.Left+1, Rect.Top+1, ICON_CLOSED_FOLDER_INDEX, dsTransparent, itImage);
			Rect.Left = Rect.Left + dmUtilities->imlTreeImages->Width + 2;
			if (Rect.Left + TextWidth + 4 < Rect.Right)
			{
				Rect.Right = Rect.Left + TextWidth + 4;
			}
			cbCategory->Canvas->TextRect(Rect, Rect.Left + 2, TextTopOffset, cbCategory->Items->Strings[Index]);
		}
		else
		{
			dmUtilities->imlTreeImages->Draw(cbCategory->Canvas, Rect.Left+dmUtilities->imlTreeImages->Width+4, Rect.Top+1, ICON_BOX_INDEX, dsTransparent, itImage);
			Rect.Left = Rect.Left + (dmUtilities->imlTreeImages->Width*2) + 6;
			if (Rect.Left + TextWidth + 4 < Rect.Right)
			{
				Rect.Right = Rect.Left + TextWidth + 4;
			}
			cbCategory->Canvas->TextRect(Rect, Rect.Left + 2, TextTopOffset, cbCategory->Items->Strings[Index]);

			cbCategory->Canvas->Pen->Style = psSolid;
			cbCategory->Canvas->Pen->Color = clGray;
			cbCategory->Canvas->Pen->Width = 1;

			cbCategory->Canvas->MoveTo(OldRect.Left + (dmUtilities->imlTreeImages->Width / 2), OldRect.Top);
			if (Index < cbCategory->Items->Count-1)
			{
				cbCategory->Canvas->LineTo(OldRect.Left + (dmUtilities->imlTreeImages->Width / 2), OldRect.Bottom);
				cbCategory->Canvas->MoveTo(OldRect.Left + (dmUtilities->imlTreeImages->Width / 2), OldRect.Top + ((OldRect.Bottom - OldRect.Top) / 2));
			}
			else
			{
				cbCategory->Canvas->LineTo(OldRect.Left + (dmUtilities->imlTreeImages->Width / 2), OldRect.Top + ((OldRect.Bottom - OldRect.Top) / 2));
			}
			cbCategory->Canvas->LineTo(OldRect.Left + dmUtilities->imlTreeImages->Width + 2, OldRect.Top + ((OldRect.Bottom - OldRect.Top) / 2));
		}
		if (State.Contains(odSelected))
		{
			cbCategory->Canvas->DrawFocusRect(Rect);
		}
	}
	if (State.Contains(odSelected))
	{
		cbCategory->Canvas->Pen->Style = psSolid;
		cbCategory->Canvas->Pen->Color = clGray;
		cbCategory->Canvas->Pen->Width = 1;
		cbCategory->Canvas->DrawFocusRect(OldRect);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshItem(TItemNode *ItemData, bool isItemTranfer)
{
    ItemData->IsDragandDrop = isItemTranfer;
	cbItemName->Text					= ItemData->LongDescription;
	ItemData->Owner->Text			= ItemData->LongDescription;
	tntedKitchenName->Text			= ItemData->KitchenName;
	tntedItemHandheldName->Text			= ItemData->HandheldName;
	tntedItemReceiptName->Text			= ItemData->ReceiptName;
	try
	{
		cbItemEnable->OnClick		= NULL;                                         
		cbItemEnable->Checked		= ItemData->Enabled;
	}
	__finally
	{
		cbItemEnable->OnClick		= cbItemEnableClick;
	}
	try
	{
		chbItemOnlySide->OnClick	= NULL;
		chbItemOnlySide->Checked	= ItemData->ItemOnlySide;
	}
	__finally
	{
		chbItemOnlySide->OnClick	= chbItemOnlySideClick;
	}
	btnItemColour->Color				= ItemData->Colour;
	cbUnderline->Checked          = ItemData->PrinterOptions.PrintUnderlined;  // cww
	cbBold->Checked               = ItemData->PrinterOptions.PrintBold;
	cbColour->Checked             = (TColor(ItemData->PrinterOptions.PrintColour) == clRed);
	cbColourClick(cbColour); // alter the checkbox caption
	cbLargeFont->Checked          = (ItemData->PrinterOptions.PrintFont == 1);
	cbDoubleWidth->Checked        = ItemData->PrinterOptions.PrintDoubleWidth;
	cbDoubleHeight->Checked       = ItemData->PrinterOptions.PrintDoubleHeight;
    sgItem->ColCount = noOfPriceLevels + 1;
	for (int rowIndex=1; rowIndex<sgItem->RowCount; rowIndex++)
	{
        for(int colIndex=0; colIndex < noOfPriceLevels; colIndex++ )
        {
		    sgItem->Cells[colIndex][rowIndex] = "";
            sgItem->Objects[colIndex][rowIndex] = NULL;
         }
	}
	sgItem->RowCount = 2;
	sgItem->Cells[0][0] = "Size";
  
	if (ItemData->Owner->Count == 0)
	{
		sgItem->Enabled = false;
	}
	else
	{
		sgItem->Enabled = true;
	}
	lbSizesUsed->Items->Clear();
	lbSizesUnused->Items->Clear();

	for (int j=0; j<ItemData->Owner->Count; j++)
	{
		if (((TEditorNode *)ItemData->Owner->Item[j]->Data)->NodeType == ITEM_SIZE_NODE)
		{
			if (j!=0)
			{
				sgItem->RowCount++;
				sgItem->Cells[0][sgItem->RowCount-1] = "";
			}
			TTreeNode *ItemSizeNode = ItemData->Owner->Item[j];
			TItemSizeNode *ItemSizeData;
			if (ItemSizeNode)
			{
				ItemSizeData = (TItemSizeNode *)ItemSizeNode->Data;
				sgItem->Cells[0][sgItem->RowCount-1]	= ItemSizeData->LongDescription;

                 //populating priceLevelsName in the grid
                std::map<int,Menu::TPriceLevelsName>::const_iterator itrstart = PriceLevelsName.begin();
                std::map<int,Menu::TPriceLevelsName>::const_iterator itrEnd =  PriceLevelsName.end();
                for ( ; itrstart != itrEnd; ++itrstart )
                {
                    sgItem->Cells[(*itrstart).second.PriceLevelKey][0] = (*itrstart).second.PriceLevelName;
                }

                 std::map<int,Menu::TItemSizePriceLevel>::const_iterator grpIT = ItemSizeData->ItemSizePriceLevels.begin();
                 std::map<int,Menu::TItemSizePriceLevel>::const_iterator grpEnd = ItemSizeData->ItemSizePriceLevels.end();

                 for ( ; grpIT != grpEnd; ++grpIT )
                 {
                     sgItem->Cells[(*grpIT).second.PriceLevelKey][sgItem->RowCount-1] = CurrToStrF((*grpIT).second.Price, ffFixed, CurrencyDecimals);
                     sgItem->Objects[(*grpIT).second.PriceLevelKey][sgItem->RowCount-1]	= ItemSizeData;
                 }

                lbSizesUsed->Items->AddObject(ItemSizeData->LongDescription, (TObject *)ItemSizeData->SizeKey);
			}
			else
			{
                 for(int i=1; i<= noOfPriceLevels; i++)
                {
				    sgItem->Objects[i][sgItem->RowCount-1]	= NULL;
                }    
			}
		}
	}
	///std::auto_ptr<TStringList> AllSizes(new TStringList());
	//GetAllSizes(AllSizes.get());

    //std::map<AnsiString, int>::iterator it;
    //std::map<AnsiString, TForcedOption>::iterator it = ItemData->ForcedOptions[group_number].find(LongDescription);
    /*if  (it != ItemData->ForcedOptions[group_number].end())
    {
        ItemData->ForcedOptions[group_number].erase(it);
    }*/

    for(std::map<AnsiString,  TAvailableMenuSize>::iterator it = AllSizesForMenu.begin(); it != AllSizesForMenu.end(); ++it)
    {
       bool Used = false;
       for (int j=0; j<ItemData->Owner->Count; j++)
       {
           TItemSizeNode *UsedItemSizeData = (TItemSizeNode *)ItemData->Owner->Item[j]->Data;
           if(it->first == UsedItemSizeData->LongDescription)
           {
              Used = true;
              break;
           }
           //it = AllSizesForMenu.find(UsedItemSizeData->LongDescription);
       }
       if(!Used)
       {
          lbSizesUnused->Items->AddObject(it->first, (TObject*)it->second.Key);
       }
    }

    /*bool Used = false;
    for(int i = 0; i < AllSizesForMenu.size(); i += 5)
    {
        for (int j=0; j<ItemData->Owner->Count; j++)
        {
            TItemSizeNode *UsedItemSizeData = (TItemSizeNode *)ItemData->Owner->Item[j]->Data;
            it = AllSizesForMenu.find(UsedItemSizeData->LongDescription);
            if (it != AllSizesForMenu.end())
            {
                Used = true;
                break;
                //lbSizesUnused->Items->AddObject(it->, AllSizes->Objects[i]);
            }
        }
        if (!Used)
        {
            lbSizesUnused->Items->AddObject(it, AllSizes->Objects[i]);
        }
   }*/

	/*for (int i=0; i<AllSizes->Count; i+=5)
	{
		bool Used = false;
		for (int j=0; j<ItemData->Owner->Count; j++)
		{
			TItemSizeNode *UsedItemSizeData = (TItemSizeNode *)ItemData->Owner->Item[j]->Data;
			if (AllSizes->Strings[i] == UsedItemSizeData->LongDescription)
			{
				Used = true;
				break;
			}
		}
		if (!Used)
		{
			lbSizesUnused->Items->AddObject(AllSizes->Strings[i], AllSizes->Objects[i]);
		}
	}*/
	sgItem->ColWidths[0] = sgItem->ClientWidth / 3;
  //	sgItem->ColWidths[1] = (sgItem->ClientWidth - sgItem->ColWidths[0] - sgItem->GridLineWidth) / 2;
 //	sgItem->ColWidths[2] = sgItem->ClientWidth - sgItem->ColWidths[0] - sgItem->ColWidths[1] -
 //	(sgItem->GridLineWidth * 3);

	if (frmPreview->Tag == Screen->Width)
	{
		btnItemColour->Width = frmPreview->sbDishes->ClientWidth / 4;
		btnItemColour->Height = frmPreview->sbDishes->ClientHeight / 8;
	}
	btnItemColour->Caption = ItemData->LongDescription;

	double R,G,B;          /* input RGB values */
	R = GetRValue(btnItemColour->Color);
	G = GetGValue(btnItemColour->Color);
	B = GetBValue(btnItemColour->Color);
	double L = (0.2126 * R) + (0.7152 * G) + (0.0722 * B);
	//BrightNess := (0.3 * rgbRed) + (0.59 * rgbGreen) + (0.11 * rgbBlue);
	if (L > 128)
	{
		btnItemColour->Font->Color 		= clBlack;
	}
	else
	{
		btnItemColour->Font->Color 		= clWhite;
	}

	//rbCondimentGrp1->Checked = true;
	// Maiintains the same Radio Group Button accross items 
	TRadioButton *rbCondimentGrp = NULL;
	if (rbCondimentGrp1->Checked)      rbCondimentGrp = rbCondimentGrp1;
	else if (rbCondimentGrp2->Checked) rbCondimentGrp = rbCondimentGrp2;
	else if (rbCondimentGrp3->Checked) rbCondimentGrp = rbCondimentGrp3;
	else if (rbCondimentGrp4->Checked) rbCondimentGrp = rbCondimentGrp4;
	else if (rbCondimentGrp5->Checked) rbCondimentGrp = rbCondimentGrp5;
	else if (rbCondimentGrp6->Checked) rbCondimentGrp = rbCondimentGrp6;
	else if (rbCondimentGrp7->Checked) rbCondimentGrp = rbCondimentGrp7;
	else if (rbCondimentGrp8->Checked) rbCondimentGrp = rbCondimentGrp8;
	// 1st time thru
	if (rbCondimentGrp == NULL || rbCondimentGrp->Tag < 1 || rbCondimentGrp->Tag > 8 )
    rbCondimentGrp1->Checked = true;
	else
	rbCondimentGrpClick(rbCondimentGrp);

	//rbForcedSideGrp1->Checked = true;
	TRadioButton *rbForcedSideGrp = NULL;
	if (rbForcedSideGrp1->Checked)	rbForcedSideGrp = rbForcedSideGrp1;
	if (rbForcedSideGrp2->Checked)	rbForcedSideGrp = rbForcedSideGrp2;
	if (rbForcedSideGrp3->Checked)   rbForcedSideGrp = rbForcedSideGrp3;
	if (rbForcedSideGrp4->Checked)   rbForcedSideGrp = rbForcedSideGrp4;
	if (rbForcedSideGrp5->Checked)   rbForcedSideGrp = rbForcedSideGrp5;
	if (rbForcedSideGrp6->Checked)   rbForcedSideGrp = rbForcedSideGrp6;
	if (rbForcedSideGrp7->Checked)   rbForcedSideGrp = rbForcedSideGrp7;
	if (rbForcedSideGrp8->Checked)   rbForcedSideGrp = rbForcedSideGrp8;
	// 1st time thru
	if (rbForcedSideGrp == NULL || rbForcedSideGrp->Tag < 1 || rbForcedSideGrp->Tag > 8 )
	rbForcedSideGrp1->Checked = true;
	else
	rbForcedSideGrpClick(rbForcedSideGrp);

}
//---------------------------------------------------------------------------
void TfrmMenuEdit::GetAllSizes(TStringList *AllSizes)
{
	TTreeNode *SizesNode = tvMenu->Items->GetFirstNode()->Item[SIZES_INDEX];
	for (int i=0; i<SizesNode->Count; i++)
	{
		TSizeNode *SizeData = (TSizeNode *)SizesNode->Item[i]->Data;
		AllSizes->AddObject(SizeData->LongDescription, (TObject*)SizeData->Key);
		AllSizes->AddObject(UTF8Encode(SizeData->KitchenName), 0x0);
		AllSizes->AddObject(UTF8Encode(SizeData->HandheldName), 0x0);
		AllSizes->AddObject(UTF8Encode(SizeData->ReceiptName), 0x0);
		AllSizes->AddObject
		(SizeData->Weighed?AnsiString("True"):AnsiString("False"), 0x0);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::GetAllServingCourses(TStringList *AllServingCourses)
{
	TTreeNode *ServingCoursesNode  = tvMenu->Items->GetFirstNode()->Item[SERVING_COURSES_INDEX];
	//	bool SomeServingCoursesVisible = false;
	for (int i=0; i<ServingCoursesNode->Count; i++)
	{
		TServingCourseNode *ServingCourseData = (TServingCourseNode *)ServingCoursesNode->Item[i]->Data;
		AllServingCourses->Add(ServingCourseData->LongDescription);
		AllServingCourses->Add(UTF8Encode(ServingCourseData->KitchenName));
		switch (ServingCourseData->Enabled)
		{
		case true  : AllServingCourses->Add("T"); break;
		case false : AllServingCourses->Add("F"); break;
		}
		switch (ServingCourseData->Deleted)
		{
		case true  : AllServingCourses->Add("T"); break;
		case false : AllServingCourses->Add("F"); break;
		}
		switch (ServingCourseData->Selectable)
		{
		case true  : AllServingCourses->Add("T"); break;
		case false : AllServingCourses->Add("F"); break;
		}
		AllServingCourses->Add(IntToStr((int)ServingCourseData->Colour));
        //AllServingCourses->Add(IntToStr((int)ServingCourseData->Key));
	}
}

//---------------------------------------------------------------------------
void TfrmMenuEdit::GetAllCategories(TStringList *AllCategories)
{
	TTreeNode *CategoryGroupsNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
	for (int i=0; i<CategoryGroupsNode->Count; i++)
	{
		for (int j=0; j<CategoryGroupsNode->Item[i]->Count; j++)
		{
			TCategoryNode *CategoryData = (TCategoryNode *)CategoryGroupsNode->Item[i]->Item[j]->Data;
			AllCategories->Add(CategoryData->LongDescription);
		}
	}
}

//---------------------------------------------------------------------------
void TfrmMenuEdit::GetAllCategoriesWithKeys(std::vector<Menu::TNameAndKey> *AllCategoriesWithKeys)
{
	TTreeNode *CategoryGroupsNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
	for (int i=0; i<CategoryGroupsNode->Count; i++)
	{
		for (int j=0; j<CategoryGroupsNode->Item[i]->Count; j++)
		{
			TCategoryNode *CategoryData = (TCategoryNode *)CategoryGroupsNode->Item[i]->Item[j]->Data;
			Menu::TNameAndKey CategoryItem (CategoryData->Key, CategoryData->LongDescription);
			AllCategoriesWithKeys->push_back(CategoryItem);
		}
	}
}

//---------------------------------------------------------------------------
void TfrmMenuEdit::GetAll3rdPartyGroups(TStrings *ThirdPartyGroups)
{
	TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
	for (int i = FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
	{
		TTreeNode *CourseNode = MenuNode->Item[i];
		TCourseNode *CourseData = (TCourseNode *)CourseNode->Data;
		if (CourseData->NodeType == COURSE_NODE)
		{
			for (int j=0; j<CourseNode->Count; j++)
			{
				TTreeNode *ItemNode = CourseNode->Item[j];
				TItemNode *ItemData = (TItemNode *)ItemNode->Data;
				if (ItemData->NodeType == ITEM_NODE)
				{
					for (int k=0; k<ItemNode->Count; k++)
					{
						TTreeNode *ItemSizeNode = ItemNode->Item[k];
						TItemSizeNode *ItemSizeData = (TItemSizeNode *)ItemSizeNode->Data;
						if (ItemSizeData->NodeType == ITEM_SIZE_NODE)
						{
							if (ThirdPartyGroups->IndexOf(ItemSizeData->ThirdPartyCode) == -1 && ItemSizeData->ThirdPartyCode != WideString(""))
							{
								ThirdPartyGroups->Add(ItemSizeData->ThirdPartyCode);
								ThirdPartyGroups->Add("");
							}
						}
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbOptionMultiSelectClick(TObject *Sender)
{

   try
	{
		chbOptionMultiSelect->OnClick	  = NULL;
		chbOptionGroupSkip->OnClick     = NULL;
		if (chbOptionMultiSelect->Checked)
		{
			chbOptionGroupSkip->Enabled = false;
			chbOptionGroupSkip->Checked = true;
		}
		else
		{
			chbOptionGroupSkip->Enabled = true;
		}
		SetAllForcedOptions( chbOptionMultiSelect->Checked?0:1,chbOptionGroupSkip->Checked);
		MenuEdited = true;
	}
	__finally
	{
		chbOptionMultiSelect->OnClick	= chbOptionMultiSelectClick;
		chbOptionGroupSkip->OnClick	= chbOptionGroupSkipClick;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbOptionGroupSkipClick(TObject *Sender)
{
	SetAllForcedOptions( chbOptionMultiSelect->Checked?0:1,chbOptionGroupSkip->Checked);
	MenuEdited = true;



}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::rbCondimentGrpClick(TObject *Sender)
{  // Forced Options
	TTreeNode *ItemTreeNode = tvMenu->Selected;
	TItemNode *ItemData = (TItemNode *)ItemTreeNode->Data;
	TTreeNode *OptionsTreeNode = tvMenu->Selected->Parent->Item[tvMenu->Selected->Parent->Count - 1];
	chklstCondiments->Items->Clear();
	try
	{
		chbOptionMultiSelect->OnClick = NULL;
		chbOptionGroupSkip->OnClick   = NULL;
		chbOptionMultiSelect->Checked = false;
		chbOptionMultiSelect->Enabled = false;
		chbOptionGroupSkip->Checked	  = false;
		chbOptionGroupSkip->Enabled	  = false;


		for (int i=0; i<OptionsTreeNode->Count; i++)
		{
			if (((TEditorNode *)OptionsTreeNode->Item[i]->Data)->NodeType == OPTION_NODE)
			{
				TOptionNode *OptionData = (TOptionNode *)OptionsTreeNode->Item[i]->Data;
				int group_number = OptionData->GroupNumber;

				if (group_number == ((TComponent *)Sender)->Tag)
				{

					int Index = chklstCondiments->Items->AddObject(OptionData->LongDescription, OptionData);
					// multi Select
					if (OptionData->MaxSelect == 0)
					{
						chbOptionMultiSelect->Checked	= true;
						chbOptionGroupSkip->Checked		= true;
						OptionData->OptionGroupSkip     = true; // cww today
						chbOptionGroupSkip->Enabled		= false;
					}
					else
					{
						chbOptionGroupSkip->Checked		= OptionData->OptionGroupSkip;
						chbOptionGroupSkip->Enabled		= true;
					}
					chbOptionMultiSelect->Enabled		= true;
					if (ItemData->ForcedOptions[group_number].find(
								OptionData->LongDescription) \
							!= ItemData->ForcedOptions[group_number].end())
					{
                        if(ItemData->IsDragandDrop)
                        {
                           ItemData->ForcedOptions[group_number].erase(OptionData->LongDescription);
                        }
                        else
                        {
                           chklstCondiments->Checked[Index] = true;
                        }
					}

				}
			}
		}
	}
	__finally
	{
		chbOptionMultiSelect->OnClick				= chbOptionMultiSelectClick;
		chbOptionGroupSkip->OnClick   			= chbOptionGroupSkipClick;
	}
}
//---------------------------------------------------------------------------
void 	TfrmMenuEdit::SetAllForcedOptions( int  MaxSelect, bool OptionGroupSkip)
{
	int SelectedGroupNumber = -1;
	if (rbCondimentGrp1->Checked)      SelectedGroupNumber = 1;
	else if (rbCondimentGrp2->Checked) SelectedGroupNumber = 2;
	else if (rbCondimentGrp3->Checked) SelectedGroupNumber = 3;
	else if (rbCondimentGrp4->Checked) SelectedGroupNumber = 4;
	else if (rbCondimentGrp5->Checked) SelectedGroupNumber = 5;
	else if (rbCondimentGrp6->Checked) SelectedGroupNumber = 6;
	else if (rbCondimentGrp7->Checked) SelectedGroupNumber = 7;
	else if (rbCondimentGrp8->Checked) SelectedGroupNumber = 8;

	TTreeNode *OptionsTreeNode = tvMenu->Selected->Parent->Item[tvMenu->Selected->Parent->Count - 1];
	// Set Each of the Options to be the same Multi Select (MaxSelect) and Allow Skip for Group (OptionGroupSkip)
	for (int i=0; i<OptionsTreeNode->Count; i++)
	{
		if (((TEditorNode *)OptionsTreeNode->Item[i]->Data)->NodeType == OPTION_NODE)
		{
			TOptionNode *OptionData = (TOptionNode *)OptionsTreeNode->Item[i]->Data;
			if (OptionData->GroupNumber == SelectedGroupNumber)
			{
				AnsiString s = OptionData->LongDescription;
				OptionData->MaxSelect       = MaxSelect;
				OptionData->OptionGroupSkip = OptionGroupSkip;

			}
		}
	}
}
//---------------------------------------------------------------------------
void 	TfrmMenuEdit::SetForcedOptionToGroup(TOptionNode *ChangedOptionData )
{
	// this sets ChangedOptionData ( MaxSelect and OptionGroupSkip ) to be the same as others already in the Group
	bool FoundForcedOptionInGroup = false;
	TTreeNode *OptionsTreeNode = tvMenu->Selected->Parent;
	//TTreeNode *OptionsTreeNode = tvMenu->Selected->Parent->Item[tvMenu->Selected->Parent->Count - 1];
	for (int i=0; i<OptionsTreeNode->Count; i++)
	{
		if (((TEditorNode *)OptionsTreeNode->Item[i]->Data)->NodeType == OPTION_NODE)
		{
			TOptionNode *OptionData = (TOptionNode *)OptionsTreeNode->Item[i]->Data;
			if (OptionData->GroupNumber == ChangedOptionData->GroupNumber && OptionData->LongDescription != ChangedOptionData->LongDescription)
			{
				ChangedOptionData->MaxSelect       = OptionData->MaxSelect;
				ChangedOptionData->OptionGroupSkip = OptionData->OptionGroupSkip;
				FoundForcedOptionInGroup = true;
				break;
			}
		}
	}
	if (FoundForcedOptionInGroup == false)
	{  // Defaults if no other OPtions are in the group
		ChangedOptionData->MaxSelect       = 1;
		ChangedOptionData->OptionGroupSkip = false;
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshCourse(TCourseNode *CourseData)
{
	edCourseName->Text				= CourseData->LongDescription;
	CourseData->Owner->Text			= CourseData->LongDescription;
	tntedCourseKitchenName->Text	= CourseData->KitchenName;
	tntedCourseHandheldName->Text	= CourseData->HandheldName;
	tntedCourseReceiptName->Text	= CourseData->ReceiptName;
	cbNoDefaultServingCourse->Checked = CourseData->NoDefaultServingCourse;
	cbServingCourse->Enabled 		= !cbNoDefaultServingCourse->Checked;
	if (cbNoDefaultServingCourse->Checked)
	{
		//cbServingCourse->ItemIndex = 0;
		cbServingCourse->ItemIndex = -1;
		CourseData->ServingCourse    = "";
	}
	else
	{
		// Dont fill the combo box if Greyed out
		cbServingCourse->Items->Clear();
		//std::auto_ptr<TStringList> ServingCourseList(new TStringList());
		//GetAllServingCourses(ServingCourseList.get());
		for (int i=0; i<ServingCoursesList->Count; i+=6)
		{
			if (ServingCoursesList->Strings[i + 2] == "T")  // enabled ones only
			cbServingCourse->Items->Add(ServingCoursesList->Strings[i]);
		}
		if (CourseData->ServingCourse == "")
		cbServingCourse->ItemIndex = -1;
		else
		cbServingCourse->ItemIndex = cbServingCourse->Items->IndexOf(CourseData->ServingCourse);
	}
	try
	{
		cbClassEnable->OnClick	= NULL;
		cbClassEnable->Checked	= CourseData->Enabled;
	}
	__finally
	{
		cbClassEnable->OnClick	= cbClassEnableClick;
	}
    sgCourse->ColCount =  noOfPriceLevels + 2;
	for (int i=1; i<sgCourse->RowCount; i++)
	{
         for(int j=0; j<= noOfPriceLevels+1; j++)
        {
            sgCourse->Cells[j][i] = "";
            sgCourse->Objects[j][i] = NULL;
	    }
    }

	sgCourse->RowCount = 2;
	sgCourse->Cells[0][0] = "Item";
	sgCourse->Cells[1][0] = "Size";

	if (CourseData->Owner->Count < 2 || CourseData->Owner->Item[0]->Count == 0)
	{
		sgCourse->Enabled = false;
	}
	else
	{
		bool FirstItem = true; 

		for (int i=0; i<CourseData->Owner->Count; i++)
		{
			if (((TEditorNode *)CourseData->Owner->Item[i]->Data)->NodeType == ITEM_NODE)
			{
				if (!FirstItem)
				sgCourse->RowCount++;
				else
				FirstItem = false;
				TTreeNode *DrinkNode = CourseData->Owner->Item[i];
				TItemNode *DrinkData = (TItemNode *)DrinkNode->Data;
				sgCourse->Cells[0][sgCourse->RowCount-1] = DrinkData->LongDescription;
				for (int j=0; j<DrinkData->Owner->Count; j++)
				{
					if (((TEditorNode *)DrinkData->Owner->Item[j]->Data)->NodeType == ITEM_SIZE_NODE)
					{
						if (j!=0)
						{
							sgCourse->RowCount++;
							sgCourse->Cells[0][sgCourse->RowCount-1] = "";
						}
						TTreeNode *DrinkCostNode = DrinkData->Owner->Item[j];
						TItemSizeNode *DrinkCostData = (TItemSizeNode *)DrinkCostNode->Data;
						sgCourse->Cells[1][sgCourse->RowCount-1] = DrinkCostData->LongDescription;
                        int sizeKey = DrinkCostData->Key;

                        std::map<int,Menu::TPriceLevelsName>::const_iterator itrStart = PriceLevelsName.begin();
                        std::map<int,Menu::TPriceLevelsName>::const_iterator itrEnd = PriceLevelsName.end();
                        for ( ; itrStart != itrEnd; ++itrStart )
                        {
                            sgCourse->Cells[(*itrStart).second.PriceLevelKey+1][0] = (*itrStart).second.PriceLevelName;
                        }

                        //populating pricelevels columns
                        std::map<int,Menu::TItemSizePriceLevel>::const_iterator grpIT = DrinkCostData->ItemSizePriceLevels.begin();
                        std::map<int,Menu::TItemSizePriceLevel>::const_iterator grpEnd = DrinkCostData->ItemSizePriceLevels.end();

                         for ( ; grpIT != grpEnd; ++grpIT )
                         {
                            // sgCourse->Cells[(*grpIT).second.PriceLevelKey+1][0] = (*grpIT).second.PriceLevelName;
                             sgCourse->Cells[(*grpIT).second.PriceLevelKey+1][sgCourse->RowCount-1] = CurrToStrF((*grpIT).second.Price, ffFixed, CurrencyDecimals);
                             sgCourse->Objects[(*grpIT).second.PriceLevelKey+1][sgCourse->RowCount-1] = DrinkCostData;
                         }
					}
				}
			}
		}
		sgCourse->Enabled = true;
	}
	sgCourse->ColWidths[0] = sgCourse->ClientWidth * 2 / 5;
	sgCourse->ColWidths[1] = (sgCourse->ClientWidth - sgCourse->ColWidths[0] - sgCourse->GridLineWidth) * 2 / 5;
 //	sgCourse->ColWidths[2] = (sgCourse->ClientWidth - sgCourse->ColWidths[0] - sgCourse->ColWidths[1] -
 //	(sgCourse->GridLineWidth * 2)) / 2;
 //	sgCourse->ColWidths[3] = sgCourse->ClientWidth - sgCourse->ColWidths[0] - sgCourse->ColWidths[1] -
 //	sgCourse->ColWidths[2] - (sgCourse->GridLineWidth * 4);
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshMenu(TMenuNode *ClassesData)
{
	lbWarnings->Items->Clear();
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshServingCourses(TServingCoursesNode *ServingData) //cww
{
	lbAvailableServingCourses->Items->Clear();
	for (int i=0; i<ServingData->Owner->Count; i++)
	{
		TTreeNode *ServingCourseNode = ServingData->Owner->Item[i];
		TServingCourseNode *ServingCourseData = (TServingCourseNode *)ServingCourseNode->Data;
		// Set the treeview with the disabled pic
		switch (ServingCourseData->Enabled)
		{
		case true : ServingCourseData->Enable();
			cbServingCoursesEnable->Checked = true;
			break;
		case false: ServingCourseData->Disable();
			cbServingCoursesEnable->Checked = false;
			break;
		}
		// Enabled icon in the listbox - see onDrawItem
		lbAvailableServingCourses->Items->AddObject(ServingCourseData->LongDescription,(TObject *)ServingCourseData->Enabled);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshServingCourse(TServingCourseNode *ServingData)  //cww
{
	edServingCourseName->Text           = ServingData->LongDescription;
	tntedServingCourseKitchenName->Text = ServingData->KitchenName;
	cbTextColor->Selected               = ServingData->Colour;
	try
	{
		cbServingCourseEnable->OnClick		= NULL;
		cbServingCourseEnable->Checked      = false;
		cbServingCourseEnable->Checked		= ServingData->Enabled == true;
		cbSelectable->Checked	         	= ServingData->Selectable;
	}
	__finally
	{
		cbServingCourseEnable->OnClick		= cbServingCourseEnableClick;
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshSizes(TMenuNode *SizesData)
{
	lbAvailableSizes->Items->Clear();
    for(std::map<AnsiString, TAvailableMenuSize >::iterator it = AllSizesForMenu.begin(); it != AllSizesForMenu.end(); ++it)
    {
        lbAvailableSizes->Items->Add(it->first);
    }
	/*for (int i=0; i<SizesData->Owner->Count; i++)
	{
		TTreeNode *SizeNode = SizesData->Owner->Item[i];
		TSizeNode *SizeData = (TSizeNode *)SizeNode->Data;
		lbAvailableSizes->Items->Add(SizeData->LongDescription);
	}*/
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshSize(TSizeNode *SizeData)
{
	edSizeName->Text = SizeData->LongDescription;
	tntedSizeKitchenName->Text = SizeData->KitchenName;
	tntedSizeHandheldName->Text = SizeData->HandheldName;
	tntedSizeReceiptName->Text = SizeData->ReceiptName;
	try
	{
		chbSizeWeighed->OnClick = NULL;
		chbSizeWeighed->Checked = SizeData->Weighed;
	}
	__finally
	{
		chbSizeWeighed->OnClick = chbSizeWeighedClick;
	}

}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshCategories(TCategoryGroupNode *CategoriesData)
{
	lbAvailableCategories->Items->Clear();
	for (int i=0; i<CategoriesData->Owner->Count; i++)
	{
		TTreeNode *CategoryNode = CategoriesData->Owner->Item[i];
		TCategoryNode *CategoryData = (TCategoryNode *)CategoryNode->Data;
		lbAvailableCategories->Items->Add(CategoryData->LongDescription);
	}
	lbeCategoryGroup->Caption = CategoriesData->LongDescription;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshCategoryGroups(TCategoryGroupsNode *CategoryGroupsData)
{
	lbCategoryGroups->Items->Clear();
	for (int i=0; i<CategoryGroupsData->Owner->Count; i++)
	{
		TTreeNode *GroupNode = CategoryGroupsData->Owner->Item[i];
		TCategoryGroupNode *GroupData = (TCategoryGroupNode *)GroupNode->Data;
		lbCategoryGroups->Items->Add(GroupData->LongDescription);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshCategory(TCategoryNode *CategoryData)
{
	edCategoryName->Text = CategoryData->LongDescription;
	edGlCode->Text = CategoryData->GLCode;
	if (CategoryData->GetDeleted())
	{
		btnCategoryDelete->Caption = "Undelete Category";
	}
	else
	{
		btnCategoryDelete->Caption = "Delete Category";
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshOption(TOptionNode *OptionData)
{
	edOptionName->Text = OptionData->LongDescription;

	tntedOptionKitchenName->Text = OptionData->KitchenName;

	tntedOptionHandheldName->Text = OptionData->HandheldName;
	tntedOptionReceiptName->Text = OptionData->ReceiptName;

	cbPlusOption->Checked                = OptionData->PlusOption;
	cbMinusOption->Checked               = OptionData->MinusOption;
	cbUnderlineOptions->Checked          = OptionData->PrinterOptions.PrintUnderlined;  // cww
	cbBoldOptions->Checked               = OptionData->PrinterOptions.PrintBold;
	cbColourOptions->Checked             = (TColor(OptionData->PrinterOptions.PrintColour) == clRed);
	cbColourClick(cbColourOptions); // alter the checkbox caption
	cbLargeFontOptions->Checked          = (OptionData->PrinterOptions.PrintFont == 1);
	cbDoubleWidthOptions->Checked        = OptionData->PrinterOptions.PrintDoubleWidth;
	cbDoubleHeightOptions->Checked       = OptionData->PrinterOptions.PrintDoubleHeight;

	//	edOptionShortName->Text = OptionData->ShortDescription;
	try
	{
		cbOptionEnable->OnClick = NULL;
		cbOptionEnable->Checked = OptionData->Enabled;
	}
	__finally
	{
		cbOptionEnable->OnClick = cbOptionEnableClick;
	}
	cbOptionGroup->ItemIndex = OptionData->GroupNumber;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::RefreshOptions(TOptionsNode *OptionsData)
{
	try
	{
		cbOptionsEnable->OnClick = NULL;
		cbOptionsEnable->Checked = OptionsData->Enabled;
	}
	__finally
	{
		cbOptionsEnable->OnClick = cbOptionsEnableClick;
	}

	sgOptions->RowCount = 2;
	sgOptions->Cells[0][0] = "Option";
	sgOptions->Cells[1][0] = "Forced Option Group";
	sgOptions->Cells[2][0] = "";
	sgOptions->Cells[3][0] = "";
	sgOptions->Objects[0][1] = NULL;
	sgOptions->Cells[0][1] = "";
	sgOptions->Cells[1][1] = "";
	sgOptions->Cells[2][1] = "";
	sgOptions->Cells[3][1] = "";

	if (OptionsData->Owner->Count == 0)
	{
		sgOptions->Enabled = false;
	}
	else
	{
		sgOptions->Enabled = true;
		sgOptions->SetFocus(); // cww
		sgOptions->Col = 0;
		bool FirstItem = true;
		for (int i=0; i<OptionsData->Owner->Count; i++)
		{
			if (((TEditorNode *)OptionsData->Owner->Item[i]->Data)->NodeType == OPTION_NODE)
			{
				if (!FirstItem)
				{
					sgOptions->RowCount++;
				}
				else
				{
					FirstItem = false;
				}
				TTreeNode *OptionNode = OptionsData->Owner->Item[i];
				TOptionNode *OptionData = (TOptionNode *)OptionNode->Data;
				sgOptions->Cells[0][sgOptions->RowCount-1] = OptionData->LongDescription;
				if (OptionData->GroupNumber == 0)
				{
					sgOptions->Cells[1][sgOptions->RowCount-1] = "None";
				}
				else
				{
					sgOptions->Cells[1][sgOptions->RowCount-1] = IntToStr(OptionData->GroupNumber);
				}
				sgOptions->Objects[0][sgOptions->RowCount-1] = OptionData;
			}
		}
	}
	sgOptions->ColWidths[0] = sgOptions->ClientWidth / 2 - 1;
	sgOptions->ColWidths[1] = (sgOptions->ClientWidth - sgOptions->ColWidths[0]) * 3 / 4 - 2;
	sgOptions->ColWidths[2] = (sgOptions->ClientWidth - sgOptions->ColWidths[0] - sgOptions->ColWidths[1]) / 2 - 3;
	sgOptions->ColWidths[3] = sgOptions->ClientWidth - sgOptions->ColWidths[0] - sgOptions->ColWidths[1] - sgOptions->ColWidths[2] - 4;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::UpdateDuplicatedThirdPartyCodes(TTreeNode *ItemNode)
{
	for (int i=0; i<ItemNode->Count; i++)
	{
		if (((TEditorNode *)ItemNode->Item[i]->Data)->NodeType == ITEM_SIZE_NODE)
		{
			if (((TItemSizeNode *)ItemNode->Item[i]->Data)->ThirdPartyCodeKey < 0)
			{
				((TItemSizeNode *)ItemNode->Item[i]->Data)->ThirdPartyCodeKey = (*new_menu_element_key_generator_)();
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMenuEdit::removeForcedOptionFromCourseItems(
TTreeNode* currentCourseNode,
TOptionNode* changedOptionData,
int previousOptionGroup )
{
	for (int i=0; i<currentCourseNode->Count; i++)
	{
		if (((TEditorNode *)currentCourseNode->Item[i]->Data)->NodeType == ITEM_NODE)
		{
			TItemNode *ItemData = ( TItemNode* )currentCourseNode->Item[i]->Data;
			std::map<AnsiString, TForcedOption> &forcedOptions = ItemData->ForcedOptions[ previousOptionGroup ]; // todo: what if they change name and then the group ?
			AnsiString optionName = changedOptionData->LongDescription;

			if( forcedOptions.find( optionName ) != forcedOptions.end() )
			{
				// remove the assigned option from item's collection
				forcedOptions.erase( optionName );
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::tvMenuChange(TObject *Sender,
TTreeNode *Node)
{
	PageControl4->ActivePage = tsProperties;
	RefreshMenuDetails();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuChanging(TObject *Sender,
TTreeNode *Node, bool &AllowChange)
{
	if (Visible) tvMenu->SetFocus();
	if (tvMenu->Selected)
	{
		AllowChange = SaveNode(tvMenu->Selected);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuDeletion(TObject *Sender,
TTreeNode *Node)
{
	delete (TEditorNode *)Node->Data;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnNewCourseClick(TObject *Sender)
{
	TTreeNode *NewNode;
	if (tvMenu->Selected != NULL)
	{
		if (!SaveNode(tvMenu->Selected))
		{
			return;
		}

		tvMenu->Selected = tvMenu->Items->GetFirstNode();
		if (tvMenu->Selected != tvMenu->Items->GetFirstNode())
		{
			return;
		}
		NewNode = ((TEditorNode *)tvMenu->Selected->Data)->AddNode(COURSE_NODE, true);
		if (NewNode)
		{
			reinterpret_cast<TEditorNode *>(NewNode->Data)->Key =
			(*new_menu_element_key_generator_)();

			MenuEdited = true;
			InsertSize(DEFAULT_SIZE_STRING);
			((TEditorNode *)NewNode->Data)->AddNode(OPTIONS_NODE, false);
			tvMenu->Selected = NewNode;
			edCourseName->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnDeleteCourseClick(TObject *Sender)
{
	if (Application->MessageBox("Are you sure you want to delete this course and all of its items?",
				"Menu Editor",
				MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDOK)
	{
		Screen->Cursor = crHourGlass;
		try
		{
			tvMenu->Selected->Delete();
			Screen->Cursor = crDefault;
			MenuEdited = true;
		}
		catch (Exception &E)
		{
			Screen->Cursor = crDefault;
			throw;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgCourseDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	bool Draw = true;

	if (State.Contains(gdFixed)) Draw = false;
	if (ACol != 2 && ACol != 3) Draw = false;
	if (sgCourse->RowCount == 2 && sgCourse->Cells[0][1] == "" && sgCourse->Cells[1][1] == "") Draw = false;

	if (Draw)
	{
		Currency Value = 0;
		try
		{
			Value = StrToCurr(sgCourse->Cells[ACol][ARow]);
		}
		catch (EConvertError &E)
		{
			Value = 0;
			sgCourse->Cells[ACol][ARow] = "0.00";
		}
		sgCourse->Canvas->Brush->Color = sgCourse->Color;
		sgCourse->Canvas->FillRect(Rect);
		sgCourse->Canvas->TextRect(Rect, Rect.Left+1 ,Rect.Top+1, CurrToStrF(Value, ffFixed, CurrencyDecimals));
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgCourseEnter(TObject *Sender)
{
	if (sgCourse->Col == 0 || sgCourse->Row == 0)
	{
		TRect CellRect = sgCourse->CellRect(2, 1);
		TPoint Centre = TPoint(CellRect.Left + ((CellRect.Right - CellRect.Left) / 2),
		CellRect.Top + ((CellRect.Bottom - CellRect.Top) / 2));
		PostMessage(sgCourse->Handle, WM_LBUTTONDOWN, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
		PostMessage(sgCourse->Handle, WM_LBUTTONUP, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgCourseExit(TObject *Sender)
{
	try
	{
		sgCourse->OnSelectCell = NULL;
		sgCourse->Col = 0;
	}
	__finally
	{
		sgCourse->OnSelectCell = sgCourseSelectCell;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgCourseKeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{
	if (sgCourse->Col == 2 || sgCourse->Col == 3)
	{
		if (!((Key >= '0' && Key <= '9') || (Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9) ||
					Key == VK_DECIMAL || Key == '.' || Key == VK_BACK || Key == VK_LEFT || Key == VK_RIGHT))
		{
			Key = NULL;
		}
		else if ((Key == '.' || Key == VK_DECIMAL) && sgCourse->Cells[sgCourse->Col][sgCourse->Row].AnsiPos(".") != 0)
		{
			Key = NULL;
		}
		if (!sgCourse->Objects[sgCourse->Col][sgCourse->Row])
		{
			Key = NULL;
		}
	}
	else
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgCourseKeyUp(TObject *Sender, WORD &Key,
TShiftState Shift)
{
	if (sgCourse->Col == 2 || sgCourse->Col == 3)
	{
		if ((Key >= '0' && Key <= '9')  || (Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9) ||
				Key == VK_DECIMAL || Key == '.' || Key == VK_BACK)
		{
			if (Key != '.')
			{
				AnsiString Value = sgCourse->Cells[sgCourse->Col][sgCourse->Row];
				AnsiString Dec,Frac = "";
				int i = Value.AnsiPos(".");
				Dec = Value;
				// has a decimal point
				if (i > 0)
				{
					Dec = Value.SubString(1,i-1);
					Frac = Value.SubString(i+1,Value.Length() - i);
				}
				if (Dec.Length()  >= 7 || Frac.Length() >= 2 )
				{
					Key = NULL;
					sgCourse->Cells[sgCourse->Col][sgCourse->Row] = sgCourse->Cells[sgCourse->Col][sgCourse->Row];
				}
				if (sgCourse->Col == 2)
                {
                    for(i=3;i<sgCourse->ColCount;i++)
                    {
			            sgCourse->Cells[i][sgCourse->Row] = sgCourse->Cells[2][sgCourse->Row];
                    }
			    }
			}
		}
	} 

	if (Key == VK_RETURN) {
		int new_col = sgCourse->Col == 2;

		sgCourse->Col = new_col + 2;
		sgCourse->Row =
		sgCourse->Row + (sgCourse->Row
		!= sgCourse->RowCount - 1) * !new_col;
		sgCourse->SetFocus();
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgCourseSelectCell(TObject *Sender,
int ACol, int ARow, bool &CanSelect)
{
	if (ACol == 0 || ACol == 1 || sgCourse->Objects[ACol][ARow] == NULL)
	{
		CanSelect = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgCourseSetEditText(TObject *Sender,
int ACol, int ARow, const AnsiString Value)
{
	if (Value != "" && (ACol >= 2))
	{
		Currency CurVal;
		TItemSizeNode *NodeData = (TItemSizeNode *)sgCourse->Objects[ACol][ARow];
		if (NodeData)
		{
			try
			{
               if(CheckIfNumeric(Value))
               {
                    CurVal = StrToCurr(Value);
                    if ((ACol >= 2)
                            && (MenuEdited =
                                NodeData->ItemSizePriceLevels[ACol-1].Price != CurVal))
                    {
                        if(ACol == 2)
                        {
                             for(int i=1; i<sgCourse->ColCount-1; i++)
                            {
                                  NodeData->ItemSizePriceLevels[i].Price = CurVal;
                                  NodeData->Price = CurVal;
                                  NodeData->SpecialPrice = CurVal;
                            }
                        }
                        else
                        {
                                NodeData->ItemSizePriceLevels[ACol-1].Price = CurVal;
                                NodeData->SpecialPrice = CurVal;
                        }
                    }


				if (NodeData->Enabled)
				{
					NodeData->Owner->Text = CurrencyString + CurrToStrF(NodeData->ItemSizePriceLevels[1].Price, ffFixed, CurrencyDecimals) +
					" (" +
					NodeData->LongDescription +
					")";
					NodeData->Owner->StateIndex = -1;
				}
				else
				{
					NodeData->Owner->Text = "Disabled (" + NodeData->LongDescription + ")";
					NodeData->Owner->StateIndex = DISABLEDSTATE_PIC;
				}

                }
			}
			catch (EConvertError &E)
			{
				if (ACol >= 2)
				{
					sgCourse->Cells[sgCourse->Col][sgCourse->Row] = CurrToStrF(NodeData->ItemSizePriceLevels[ACol-1].Price, ffGeneral, CurrencyDecimals);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::edCourseNameChange(TObject *Sender)
{
	if (edCourseName->Text.Length() > MAX_COURSE_NAME_LENGTH)
	{
		edCourseName->Text = edCourseName->Text.SubString(1, MAX_COURSE_NAME_LENGTH);
		edCourseName->SelStart = edCourseName->Text.Length();
	}
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == COURSE_NODE)
	{
		TCourseNode *CourseData = (TCourseNode *)CurrentNodeData;
		CourseData->Owner->Text = edCourseName->Text;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbClassEnableKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		btnNewItem->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbBarClick(TObject *Sender)
{
	MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbBarKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		cbClassEnable->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbRestaurantKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		cbClassEnable->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbPalmKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		cbClassEnable/*btnCourseNewItem*/->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbClassEnableClick(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == COURSE_NODE)
	{
		TCourseNode *CourseData = (TCourseNode *)CurrentNodeData;
		if (cbClassEnable->Checked)
		{
			CourseData->Enable();
		}
		else
		{
			CourseData->Disable();
		}
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbItemNameChange(TObject *Sender)
{
	if (cbItemName->Text.Length() > MAX_ITEM_NAME_LENGTH)
	{
		cbItemName->Text = cbItemName->Text.SubString(1, MAX_ITEM_NAME_LENGTH);
		cbItemName->SelStart = cbItemName->Text.Length();
	}
	MenuEdited = true;
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_NODE)
	{
		TItemNode *DrinkData = (TItemNode *)CurrentNodeData;
		DrinkData->Owner->Text = cbItemName->Text;
		btnItemColour->Caption = cbItemName->Text;
		DrinkData->LongDescription = cbItemName->Text;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbItemNameDropDown(TObject *Sender)
{
	TItemNode *CurrentNodeData = (TItemNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_NODE)
	{
		AnsiString Desc, Code, Unit,ReductionUnit;
		Currency ConversionFactor;
		if (RetrieveMMStock(&Code, &Desc, &Unit,&ConversionFactor, &ReductionUnit, ""))
		{
			cbItemName->Items->Clear();
			cbItemName->Items->Add(Desc);
			cbItemName->Text						= Desc;
			CurrentNodeData->Owner->Text		= Desc;
			CurrentNodeData->LongDescription	= Desc;

			CurrentNodeData->DefaultStockRecipe.StockCode	= Code;
			CurrentNodeData->DefaultStockRecipe.Description	= Desc;
			CurrentNodeData->DefaultStockRecipe.Unit			= Unit;
			CurrentNodeData->DefaultStockRecipe.ConversionFactor = ConversionFactor;
			CurrentNodeData->DefaultStockRecipe.ReductionUnit = ReductionUnit;
			MenuEdited = true;
		}
	}
	if (Sender != NULL)
	{
		PostMessage(cbItemName->Handle, WM_CHAR, VK_ESCAPE, 0);
		cbItemName->Repaint();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbItemNameKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{
	if (Key == VK_DOWN)
	{
		cbItemNameDropDown(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbItemEnableClick(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_NODE)
	{
		TItemNode *DrinkData = (TItemNode *)CurrentNodeData;
		if (cbItemEnable->Checked)
		{
			DrinkData->Enable();
		}
		else
		{
			DrinkData->Disable();
		}
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbItemEnableKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		chbItemOnlySide->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::stDrinkColourClick(TObject *Sender)
{
	ColorDialog1->Color = btnItemColour->Color;
	if (ColorDialog1->Execute())
	{
		btnItemColour->Color = ColorDialog1->Color;
		double R,G,B;          /* input RGB values */
		R = GetRValue(btnItemColour->Color);
		G = GetGValue(btnItemColour->Color);
		B = GetBValue(btnItemColour->Color);
		double L = (0.2126 * R) + (0.7152 * G) + (0.0722 * B);
		if (L > 128)
		{
			btnItemColour->Font->Color 		= clBlack;
		}
		else
		{
			btnItemColour->Font->Color 		= clWhite;
		}
		LastDrinkColour = ColorDialog1->Color;
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnNewItemClick(TObject *Sender)
{
	MenuEdited = true;
	TTreeNode *NewNode;
	if (tvMenu->Selected != NULL)
	{
		if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_NODE)
		{
			tvMenu->Selected = tvMenu->Selected->Parent;
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_SIZE_NODE)
		{
			tvMenu->Selected = tvMenu->Selected->Parent->Parent;
		}
		if (((TEditorNode *)tvMenu->Selected->Data)->NodeType != COURSE_NODE)
		{
			Application->MessageBox("You must select the course first.", "Error", MB_OK + MB_ICONERROR);
			return;
		}
		NewNode = ((TEditorNode *)tvMenu->Selected->Data)->AddNode(ITEM_NODE, false);
		if (NewNode)
		{
			reinterpret_cast<TEditorNode *>(NewNode->Data)->Key =
			(*new_menu_element_key_generator_)();

			MenuEdited = true;
			tvMenu->Selected = NewNode;
			if (tvMenu->Selected == NewNode)
			{
				try
				{
					cbItemEnable->OnClick = NULL;
					cbItemEnable->Checked = true;
				}
				__finally
				{
					cbItemEnable->OnClick = cbItemEnableClick;
				}
				try
				{
					chbItemOnlySide->OnClick = NULL;
					chbItemOnlySide->Checked = false;
				}
				__finally
				{
					chbItemOnlySide->OnClick = chbItemOnlySideClick;
				}
				cbItemName->Text = ((TEditorNode *)NewNode->Data)->LongDescription;
				tntedKitchenName->Text = ((TItemNode *)NewNode->Data)->KitchenName;
				pcItemDetails->ActivePage = tsItemDetails;
				cbItemName->SetFocus();

				//if (rbFoodMenu->Checked)
				//{
				TTreeNode *NewSize = ((TEditorNode *)tvMenu->Selected->Data)->AddNode(ITEM_SIZE_NODE, true);

				int Index = InsertSize(DEFAULT_SIZE_STRING);
				TTreeNode *SizesNode = tvMenu->Items->GetFirstNode()->Item[SIZES_INDEX];

				((TEditorNode *)NewSize->Data)->LongDescription = ((TSizeNode *)SizesNode->Item[Index]->Data)->LongDescription;//DEFAULT_SIZE_STRING;
				((TItemSizeNode *)NewSize->Data)->KitchenName   = GetKitchenNameOfSize(((TSizeNode *)SizesNode->Item[Index]->Data)->LongDescription);
				((TItemSizeNode *)NewSize->Data)->Price			= 0;
				((TItemSizeNode *)NewSize->Data)->SpecialPrice	= 0;
				((TItemSizeNode *)NewSize->Data)->GST			= CurrentConnection.DefaultMenuGST;
				((TItemSizeNode *)NewSize->Data)->CostGST		= CurrentConnection.DefaultStockGST;

                 if (qrGetPriceLevelNames->Database->Connected && !qrGetPriceLevelNames->Transaction->InTransaction)
                {
                    qrGetPriceLevelNames->Transaction->StartTransaction();
                    qrGetPriceLevelNames->Close();
                    qrGetPriceLevelNames->ExecQuery();
                }
                    // when creating new item make default  price 0 and update pricelevels name according the values stored in db
                    for(int key =1; key<= noOfPriceLevels; key++)
                    {
                        Menu::TItemSizePriceLevel priceLevel;
                        priceLevel.PriceLevelKey = key;
                        priceLevel.Price = 0;

                        ((TItemSizeNode *)NewSize->Data)->ItemSizePriceLevels.insert(std::pair<int, Menu::TItemSizePriceLevel>(key,priceLevel) );

                        Menu::TPriceLevelsName priceLevelName;
                        priceLevelName.PriceLevelKey = key;
                        if(!qrGetPriceLevelNames->Eof)
                        priceLevelName.PriceLevelName = qrGetPriceLevelNames->FieldByName("PRICELEVEL_NAME")->AsString;
                        ((TItemSizeNode *)NewSize->Data)->PriceLevelsName.insert(std::pair<int, Menu::TPriceLevelsName>(key,priceLevelName) );
                        qrGetPriceLevelNames->Next();
                    }
				reinterpret_cast<TEditorNode *>(NewSize->Data)->Key =
				(*new_menu_element_key_generator_)();

				reinterpret_cast<TItemSizeNode *>(NewSize->Data)->SizeKey = ((TSizeNode *)SizesNode->Item[Index]->Data)->Key;
			}
			else
			{
				edCourseName->SetFocus();
				delete NewNode;
				return;
			}
		}
		RelabelDrinkCosts();
		RefreshItem((TItemNode *)tvMenu->Selected->Data);
		lbSizesUnused->ItemIndex = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnDeleteItemClick(TObject *Sender)
{
	if (Application->MessageBox("Are you sure you want to delete this item and all of its prices?",
				"Menu Editor",
				MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDOK)
	{
		tvMenu->Selected->Delete();
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemDrawCell(TObject *Sender, int ACol,
int ARow, TRect &Rect, TGridDrawState State)
{
	bool Draw = true;

	if (State.Contains(gdFixed)) Draw = false;
	if (ACol != 1 && ACol != 2) Draw = false;
	if (sgItem->RowCount == 2 && sgItem->Cells[0][1] == "") Draw = false;

	if (Draw)
	{
		Currency Value = 0;
		try
		{
			Value = StrToCurr(sgItem->Cells[ACol][ARow]);
		}
		catch (EConvertError &E)
		{
			Value = 0;
			sgItem->Cells[ACol][ARow] = "0.00";
		}
		sgItem->Canvas->FillRect(Rect);
		sgItem->Canvas->TextRect(Rect, Rect.Left+1 ,Rect.Top+1, CurrToStrF(Value, ffFixed, CurrencyDecimals));
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemEnter(TObject *Sender)
{
	if (sgItem->Col == 0 || sgItem->Row == 0)
	{
		TRect CellRect = sgItem->CellRect(1, 1);
		TPoint Centre = TPoint(CellRect.Left + ((CellRect.Right - CellRect.Left) / 2),
		CellRect.Top + ((CellRect.Bottom - CellRect.Top) / 2));
		PostMessage(sgItem->Handle, WM_LBUTTONDOWN, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
		PostMessage(sgItem->Handle, WM_LBUTTONUP, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemExit(TObject *Sender)
{
	try
	{
		sgItem->OnSelectCell = NULL;
		sgItem->Col = 0;
	}
	__finally
	{
		sgItem->OnSelectCell = sgItemSelectCell;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemKeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{
	if (sgItem->Col >= 1)
	{
		if (!((Key >= '0' && Key <= '9') || (Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9) ||
					Key == VK_DECIMAL || Key == '.' || Key == VK_BACK || Key == VK_LEFT || Key == VK_RIGHT))
		{
			Key = NULL;
		}
		else if ((Key == '.' || Key == VK_DECIMAL) && sgItem->Cells[sgItem->Col][sgItem->Row].AnsiPos(".") != 0)
		{
			Key = NULL;
		}
		if (!sgItem->Objects[sgItem->Col][sgItem->Row])
		{
			Key = NULL;
		}
	}
	else
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemKeyUp(TObject *Sender, WORD &Key,
TShiftState Shift)
{
	if (sgItem->Col >= 1)
	{
		if ((Key >= '0' && Key <= '9')  || (Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9) ||
				Key == VK_DECIMAL || Key == '.' || Key == VK_BACK)
		{
			if (Key != '.')
			{
				AnsiString Value = sgItem->Cells[sgItem->Col][sgItem->Row];
				AnsiString Dec,Frac = "";
				int i = Value.AnsiPos(".");
				Dec = Value;
				// has a decimal point
				if (i > 0)
				{
					Dec = Value.SubString(1,i-1);
					Frac = Value.SubString(i+1,Value.Length() - i);
				}
				if (Dec.Length()  >= 7 || Frac.Length() >= 2 )
				{
					Key = NULL;
					sgItem->Cells[sgItem->Col][sgItem->Row] = sgItem->Cells[sgItem->Col][sgItem->Row];
				}
				if (sgItem->Col == 1)
                {
                    for(i=2;i<sgItem->ColCount;i++)
                    {
			            sgItem->Cells[i][sgItem->Row] = sgItem->Cells[1][sgItem->Row];
                    }
                }
			}
		}
	}
	if (Key == VK_RETURN)
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		if (sgItem->Col == 1)
		{
			TRect CellRect = sgItem->CellRect(2, sgItem->Row);
			TPoint Centre = TPoint(CellRect.Left + ((CellRect.Right - CellRect.Left) / 2),
			CellRect.Top + ((CellRect.Bottom - CellRect.Top) / 2));
			PostMessage(sgItem->Handle, WM_LBUTTONDOWN, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
			PostMessage(sgItem->Handle, WM_LBUTTONUP, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
		}
		else
		{
			if (sgItem->Row != sgItem->RowCount-1)
			{
				TRect CellRect = sgItem->CellRect(1, sgItem->Row+1);
				TPoint Centre = TPoint(CellRect.Left + ((CellRect.Right - CellRect.Left) / 2),
				CellRect.Top + ((CellRect.Bottom - CellRect.Top) / 2));
				PostMessage(sgItem->Handle, WM_LBUTTONDOWN, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
				PostMessage(sgItem->Handle, WM_LBUTTONUP, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
			}
			else btnNewItem->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemSelectCell(TObject *Sender,
int ACol, int ARow, bool &CanSelect)
{
	CanSelect = (ACol != 0);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemSetEditText(TObject *Sender,
int ACol, int ARow, const AnsiString Value)
{
	if (Value != "" && (ACol >= 1))
	{
		Currency CurVal;
		TItemSizeNode *NodeData = (TItemSizeNode *)sgItem->Objects[ACol][ARow];
		if (NodeData)
		{
            
			try
			{
              if(CheckIfNumeric(Value))
               {

                        CurVal = StrToCurr(Value);
                        if (ACol == 1)
                        {
                            if (NodeData->ItemSizePriceLevels[ACol].Price != CurVal)
                            {
                                for(int i=1; i<sgItem->ColCount; i++)
                                {
                                      NodeData->ItemSizePriceLevels[i].Price = CurVal;
                                     NodeData->Price = CurVal;
                                     NodeData->SpecialPrice = CurVal;
                                     RelabelDrinkCosts();
                                }
                                MenuEdited = true;
                            }
                        }
                        else if (ACol >= 2)
                        {
                            if (NodeData->ItemSizePriceLevels[ACol].Price != CurVal)
                            {
                               NodeData->ItemSizePriceLevels[ACol].Price = CurVal;
                                NodeData->SpecialPrice = CurVal;
                                MenuEdited = true;
                                RelabelDrinkCosts();
                        }
                        if (NodeData->Enabled)
                        {
                            NodeData->Owner->Text = CurrencyString + CurrToStrF(NodeData->ItemSizePriceLevels[1].Price, ffFixed, CurrencyDecimals) +
                            " (" +
                            NodeData->LongDescription +
                            ")";
                            NodeData->Owner->StateIndex = -1;
                        }
                        else
                        {
                            NodeData->Owner->Text = "Disabled (" + NodeData->LongDescription + ")";
                            NodeData->Owner->StateIndex = DISABLEDSTATE_PIC;
                        }

                  }
			}
        }
			catch (EConvertError &E)
			{
				if (ACol == 1)
				{
					sgItem->Cells[ACol][ARow] = CurrToStrF(NodeData->ItemSizePriceLevels[ACol].Price, ffGeneral, CurrencyDecimals);
				}
			}
            sgItem->Refresh() ;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chklstCondimentsClickCheck(TObject *Sender)
{
	static TRadioButton *groupButtons[8] = {
		rbCondimentGrp1,
		rbCondimentGrp2,
		rbCondimentGrp3,
		rbCondimentGrp4,
		rbCondimentGrp5,
		rbCondimentGrp6,
		rbCondimentGrp7,
		rbCondimentGrp8
	};

	TItemNode *CurrentNodeData =
	reinterpret_cast<TItemNode *>(tvMenu->Selected->Data);
	if (CurrentNodeData->NodeType != ITEM_NODE)
	return;

	int currentGroupNumber = 0;
	for ( ; (currentGroupNumber < 8 \
	&& !groupButtons[currentGroupNumber]->Checked);
	currentGroupNumber++)
	;

	std::map<AnsiString, TForcedOption> &forcedOptions =
	CurrentNodeData->ForcedOptions[++currentGroupNumber];

	forcedOptions.clear();
	for (int i = 0, j = chklstCondiments->Items->Count; i < j; i++) {
		const TOptionNode &option =
		*reinterpret_cast<TOptionNode *>(chklstCondiments->Items->Objects[i]);
		if (chklstCondiments->Checked[i])
		forcedOptions[option.LongDescription] =
		TForcedOption(option.LongDescription,
		(*new_menu_element_key_generator_)(),
		option.Key,
		CurrentNodeData->Key);
	}
    CurrentNodeData->IsDragandDrop = false; // false value..
	MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbOptionsEnableClick(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == OPTIONS_NODE)
	{
		TOptionsNode *OptionsData = (TOptionsNode *)CurrentNodeData;
		if (cbOptionsEnable->Checked)
		{
			OptionsData->Enable();
		}
		else
		{
			OptionsData->Disable();
		}
	}
	MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnNewOptionClick(TObject *Sender)
{
	TTreeNode *NewNode;
	if (tvMenu->Selected != NULL)
	{
		if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == COURSE_NODE)
		{
			tvMenu->Selected = tvMenu->Selected->GetLastChild();
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_NODE)
		{
			tvMenu->Selected = tvMenu->Selected->Parent->GetLastChild();
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == OPTION_NODE)
		{
			tvMenu->Selected = tvMenu->Selected->Parent;
		}
		if (((TEditorNode *)tvMenu->Selected->Data)->NodeType != OPTIONS_NODE)
		{
			return;
		}
		NewNode = ((TEditorNode *)tvMenu->Selected->Data)->AddNode(OPTION_NODE, true);
		if (NewNode)
		{
			reinterpret_cast<TEditorNode *>(NewNode->Data)->Key =
			(*new_menu_element_key_generator_)();

			if (((TEditorNode *)NewNode->Parent->Data)->Enabled)
			{
				((TEditorNode *)NewNode->Data)->Enable();
			}
			else
			{
				((TEditorNode *)NewNode->Data)->Disable();
			}
			MenuEdited = true;
			tvMenu->Selected = NewNode;
			try
			{
				cbOptionEnable->OnClick = NULL;
				cbOptionEnable->Checked = ((TEditorNode *)NewNode->Data)->Enabled;
			}
			__finally
			{
				cbOptionEnable->OnClick = cbOptionEnableClick;
			}
			edOptionName->Text = ((TEditorNode *)NewNode->Data)->LongDescription;
			edOptionName->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgOptionsKeyPress(TObject *Sender,char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		if (sgOptions->Row == sgOptions->RowCount-1 && sgOptions->Col == 3)
		{
			// Last cell. Jump out.
			tvMenu/*btnOptionsNewOption*/->SetFocus();
		}
		else
		{
			sgOptions->OnMouseDown = NULL;
			try
			{
				TRect CellRect;
				if (sgOptions->Col < 3)
				{
					CellRect = sgOptions->CellRect(sgOptions->Col + 1, sgOptions->Row);
				}
				else if (sgOptions->Col == 3)
				{
					CellRect = sgOptions->CellRect(0, sgOptions->Row+1);
				}
				TPoint Centre = TPoint(CellRect.Left + ((CellRect.Right - CellRect.Left) / 2),
				CellRect.Top + ((CellRect.Bottom - CellRect.Top) / 2));
				SendMessage(sgOptions->Handle, WM_LBUTTONDOWN, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
				SendMessage(sgOptions->Handle, WM_LBUTTONUP, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
			}
			__finally
			{
				sgOptions->OnMouseDown = sgOptionsMouseDown;
			}
		}
	}
	else if (Key == VK_SPACE)
	{
		if (sgOptions->Col == 2 || sgOptions->Col == 3)
		{
			ToggleCheckbox(sgOptions->Col, sgOptions->Row);
			Key = NULL;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgOptionsKeyUp(TObject *Sender, WORD &Key,TShiftState Shift)
{
	TEditorNode *CurrentOptionsData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentOptionsData->NodeType == OPTIONS_NODE)
	{
		TOptionNode *OptionData = (TOptionNode *)CurrentOptionsData->Owner->Item[sgOptions->Row-1]->Data;
		OptionData->Owner->Text = sgOptions->Cells[0][sgOptions->Row];
		OptionData->LongDescription = sgOptions->Cells[0][sgOptions->Row];
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbOptionGroupKeyPress(TObject *Sender,char &Key)
{
	if (Key == VK_RETURN && !cbOptionGroup->DroppedDown)
	{
		Key = NULL;
		btnNewOption->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::edOptionNameChange(TObject *Sender)
{
	if (edOptionName->Text.Length() > MAX_OPTION_NAME_LENGTH)
	{
		edOptionName->Text = edOptionName->Text.SubString(1, MAX_OPTION_NAME_LENGTH);
		edOptionName->SelStart = edOptionName->Text.Length();
	}

	MenuEdited = true;

	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == OPTION_NODE)
	{
		TOptionNode *OptionData = (TOptionNode *)CurrentNodeData;
		OptionData->Owner->Text = edOptionName->Text;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::edOptionShortNameChange(TObject *Sender)
{
	edOptionShortName->Text = edOptionShortName->Text.SubString(1, MAX_SHORT_NAME_LENGTH);
	edOptionShortName->SelStart = edOptionShortName->Text.Length();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::edOptionShortNameKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		btnNewOption->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbOptionEnableClick(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == OPTION_NODE)
	{
		TOptionNode *OptionData = (TOptionNode *)CurrentNodeData;
		if (cbOptionEnable->Checked)
		{
			OptionData->Enable();
		}
		else
		{
			OptionData->Disable();
		}
	}
	MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbOptionGroupChange(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == OPTION_NODE)
	{
		TOptionNode *OptionData = (TOptionNode *)CurrentNodeData;

		int previousOptionGroup  = OptionData->GroupNumber;   
		OptionData->GroupNumber = cbOptionGroup->ItemIndex;

		SetForcedOptionToGroup(OptionData);
		removeForcedOptionFromCourseItems(
		tvMenu->Selected->Parent->Parent, 
		OptionData,
		previousOptionGroup );
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnDeleteOptionClick(TObject *Sender)
{
	if (Application->MessageBox("Are you sure you want to delete this option?",
				"Menu Editor",
				MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDOK)
	{
		tvMenu->Selected->Delete();
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbFreeItemClick(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *DrinkCostData = (TItemSizeNode *)CurrentNodeData;
		MenuEdited = true;
		if (cbFreeItem->Checked && !DrinkCostData->Free)
		{
			if (DrinkCostData->Price != 0 || DrinkCostData->SpecialPrice != 0)
			{
				if (Application->MessageBox("This will set the price and special price to $0.00.\r"
							"This prevents warnings being generated for a $0.00 priced item.",
							"Warning",
							MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == ID_CANCEL)
				{
					cbFreeItem->Checked = false;
					return;
				}
			}
			DrinkCostData->Free = true;
			DrinkCostData->Price = 0;
			DrinkCostData->SpecialPrice = 0;
			edItemMaxPrice->Value = 0;
			edItemMaxPrice->Enabled = false;
            for(int rowIndex =1; rowIndex<sgItemsize->RowCount; rowIndex++)
            {
               sgItemsize->Cells[1][rowIndex] = CurrToStrF(0, ffFixed, CurrencyDecimals);
               sgItemsize->Cells[2][rowIndex] = CurrToStrF(0, ffFixed, CurrencyDecimals);
               DrinkCostData->ItemSizePriceLevels[rowIndex].Price = CurrToStrF(0, ffFixed, CurrencyDecimals);
            }
            sgItemsize->Enabled = false;
			RelabelDrinkCosts();
		}
		else if (!cbFreeItem->Checked)
		{
			DrinkCostData->Free = false;
			edItemMaxPrice->Enabled = true;
            sgItemsize->Enabled = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbFreeItemKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::edItemPriceEnter(TObject *Sender)
{
	((TEdit *)Sender)->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::neTareWeightEnter(TObject *Sender)
{
	((TEdit *)Sender)->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::neTareWeightExit(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *DrinkCostData = (TItemSizeNode *)CurrentNodeData;
		if (neTareWeight->Value != DrinkCostData->TareWeight)
		{
			DrinkCostData->TareWeight = neTareWeight->Value;
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::edKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		SelectNext((TWinControl *)Sender, true, true);
	}
} 
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnDeleteSizeClick(TObject *Sender)
{
	if (Application->MessageBox("Are you sure you want to remove this size?",
				"Menu Editor",
				MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDOK)
	{
		tvMenu->Selected->Delete();
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnAddRecipeClick(TObject *Sender)
{
	TList *RecipeList = ((TItemSizeNode *)tvMenu->Selected->Data)->RecipeList;
	try
	{
		sgRecipe->RowCount++;
		sgRecipe->Cells[0][sgRecipe->RowCount - 1] = "";
		sgRecipe->Cells[1][sgRecipe->RowCount - 1] = "";
		sgRecipe->Cells[2][sgRecipe->RowCount - 1] = "";
		sgRecipe->Row = sgRecipe->RowCount - 1;
	}
	catch (EConvertError &E)
	{
		Application->MessageBox(E.Message.c_str(), "MenuMate", MB_OK + MB_ICONERROR);
		sgRecipe->RowCount--;
		RefreshMenuDetails();
		return;
	}

	AnsiString Code;
	AnsiString Desc;
	AnsiString Unit;
	AnsiString ReductionUnit;
	Currency ConversionFactor;
	TItemNode *ItemNode = (TItemNode *)tvMenu->Selected->Parent->Data;
	AnsiString NewStockItemDescription = ItemNode->LongDescription;
	if (RecipeList->Count == 0 && ItemNode->DefaultStockRecipe.StockCode != "")
	{
		TRecipe *Recipe		= new TRecipe;
		Recipe->StockCode		= ItemNode->DefaultStockRecipe.StockCode;
		Recipe->Description	= ItemNode->DefaultStockRecipe.Description;
		Recipe->Unit			= ItemNode->DefaultStockRecipe.Unit;
		Recipe->ConversionFactor = ItemNode->DefaultStockRecipe.ConversionFactor;
		Recipe->ReductionUnit = ItemNode->DefaultStockRecipe.ReductionUnit;
		Recipe->Quantity		= 0;

		if (CurrentConnection.SingleLocation)
		{
			Recipe->Location = CurrentConnection.DefaultLocation;
		}
		else
		{
			Recipe->Location = LastLocation;
		}
		edLoc->Text	= Recipe->Location;
		MenuEdited	= true;
		RecipeList->Add(Recipe);

		RefreshMenuDetails();

		sgRecipe->Row = sgRecipe->RowCount - 1;
		edRecipeQty->SetFocus();
		if(Recipe->ReductionUnit!=NULL && Recipe->ReductionUnit!= "" )
		Label20->Caption="Recipe Quantity (" +  Recipe->ReductionUnit +")" ;
		else
		Label20->Caption="Recipe Quantity";

		ItemNode->DefaultStockRecipe.StockCode		= "";
		ItemNode->DefaultStockRecipe.Description	= "";
		ItemNode->DefaultStockRecipe.Unit			= "";
		ItemNode->DefaultStockRecipe.ReductionUnit			= "";
		ItemNode->DefaultStockRecipe.ConversionFactor = 1;
	}
	else if (RetrieveMMStock(&Code, &Desc, &Unit, &ConversionFactor,&ReductionUnit, NewStockItemDescription))
	{
		TRecipe *Recipe = new TRecipe;
		RecipeList->Add(Recipe);
		Recipe->StockCode = Code;
		Recipe->Description = Desc;
		Recipe->Unit = Unit;
		Recipe->ConversionFactor = ConversionFactor;
		Recipe->ReductionUnit =  ReductionUnit;
		Recipe->Quantity = 0;
		if (CurrentConnection.SingleLocation)
		{
			Recipe->Location = CurrentConnection.DefaultLocation;
		}
		else
		{
			Recipe->Location = LastLocation;
		}
		edLoc->Text = Recipe->Location;
		MenuEdited = true;


		RefreshMenuDetails();

		sgRecipe->Row = sgRecipe->RowCount - 1;
		edRecipeQty->SetFocus();

		if(Recipe->ReductionUnit!=NULL && Recipe->ReductionUnit!= "" )
		Label20->Caption="Recipe Quantity (" +  Recipe->ReductionUnit +")" ;
		else
		Label20->Caption="Recipe Quantity";
	}
	else
	{
		RefreshMenuDetails();
	}
	sgRecipe->ColWidths[0] = sgRecipe->ClientWidth * 4 / 16;
	sgRecipe->ColWidths[1] = sgRecipe->ClientWidth * 9 / 16;
	sgRecipe->ColWidths[2] = sgRecipe->ClientWidth * 3 / 16 - 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnDeleteRecipeClick(TObject *Sender)
{
	if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_SIZE_NODE)
	{
		int LastRow = sgRecipe->Row;
		TList *RecipeList = ((TItemSizeNode *)tvMenu->Selected->Data)->RecipeList;
		if(RecipeList->Count > 0)
		{
			TRecipe *Recipe = (TRecipe *) RecipeList->Items[sgRecipe->Row-1];
			RecipeList->Delete(sgRecipe->Row-1);
			delete Recipe;

			sgRecipe->OnSelectCell = NULL;
			try
			{
				RefreshMenuDetails();

				Label20->Caption="Recipe Quantity";

			}
			__finally
			{
				sgRecipe->OnSelectCell = sgRecipeSelectCell;
			}
			int newSelectedRow = sgRecipe->RowCount - 1;

			if (LastRow < newSelectedRow)
			{
				sgRecipe->Row = LastRow;
			}
			else if (LastRow > newSelectedRow)
			{
				sgRecipe->Row = newSelectedRow;
			}
			sgRecipe->ColWidths[0] = sgRecipe->ClientWidth * 4 / 16;
			sgRecipe->ColWidths[1] = sgRecipe->ClientWidth * 9 / 16;
			sgRecipe->ColWidths[2] = sgRecipe->ClientWidth * 3 / 16 - 2;
		}
		MenuEdited = true;
		resetReceipeQtyLebel();
	}
}

void TfrmMenuEdit::resetReceipeQtyLebel()
{

	edRecipeQty->OnChange = NULL;
	try
	{

		if (tvMenu->Selected)
		{
			if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_SIZE_NODE)
			{
				TList *RecipeList = ((TItemSizeNode *)tvMenu->Selected->Data)->RecipeList;
				if (RecipeList->Count >= sgRecipe->Row)
				{
					TRecipe *Recipe = (TRecipe *) RecipeList->Items[sgRecipe->Row-1];
					Recipe->Location = edLoc->Text;
					LastLocation = Recipe->Location;
					Recipe->Quantity = edQty->Caption.ToDouble();
					if(Recipe->ReductionUnit!=NULL && Recipe->ReductionUnit!= "" )
					Label20->Caption="Recipe Quantity (" +  Recipe->ReductionUnit +")" ;
					else
					Label20->Caption="Recipe Quantity";

					
				}}}}
	__finally
	{
		edRecipeQty->OnChange = edLocChange;
	}
}

//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgRecipeSelectCell(TObject *Sender,
int ACol, int ARow, bool &CanSelect)
{
	edRecipeQty->OnChange = NULL;
	try
	{
		if (!(sgRecipe->RowCount == 2 &&
					sgRecipe->Cells[0][1] == "" &&
					sgRecipe->Cells[1][1] == "" &&
					sgRecipe->Cells[2][1] == ""))
		//   		sgRecipe->Cells[0][1] == "" &&
		//         sgRecipe->Cells[1][1] == ""))
		{
			if (tvMenu->Selected)
			{

				if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_SIZE_NODE)
				{
					TList *RecipeList = ((TItemSizeNode *)tvMenu->Selected->Data)->RecipeList;
					if (RecipeList->Count >= sgRecipe->Row)
					{
						TRecipe *Recipe = (TRecipe *) RecipeList->Items[sgRecipe->Row-1];
						Recipe->Location = edLoc->Text;
						LastLocation = Recipe->Location;
						Recipe->Quantity = edQty->Caption.ToDouble();
						if(Recipe->ReductionUnit!=NULL && Recipe->ReductionUnit!= "" )
						Label20->Caption="Recipe Quantity (" +  Recipe->ReductionUnit +")" ;
						else
						Label20->Caption="Recipe Quantity";

					}

					if(RecipeList->Count > 0)
					{
                        edLoc->Items->Clear();
						if (RecipeList->Count >= ARow)
						{
                            TRecipe *Recipe = (TRecipe *) RecipeList->Items[ARow-1];

                           
                            LoadRecipeLocations(Recipe->StockCode);
 
							edRecipeQty->Value = Recipe->Quantity*Recipe->ConversionFactor;
							edQty->Caption = Recipe->Quantity;
							edLoc->Text = Recipe->Location;
						}
						else
						{
							edRecipeQty->Value = 0;
							edQty->Caption = 0;
							edLoc->Text = "";
						}
					}
					else
					{
						edRecipeQty->Value = 0;
						edQty->Caption = 0;
						edLoc->Text = "";
					}
					// Save new Item costs.
					TItemSizeNode *DrinkCostData = (TItemSizeNode *)tvMenu->Selected->Data;

					Currency OldCost					= DrinkCostData->Price;
					Currency OldSpecialPrice		= DrinkCostData->SpecialPrice;
					double OldGST						= DrinkCostData->GST;
					double OldCostGST					= DrinkCostData->CostGST;
					DrinkCostData->MaxRetailPrice = edItemMaxPrice->Value;
					DrinkCostData->GST				= 0;
					DrinkCostData->CostGST			= edCostGST->Value;
					DrinkCostData->PointsPercent	= nePointsPercent->Value;

                    //update node's pricelevels according to grid values
                    for(int key =1;key<= noOfPriceLevels; key++)
                    {
                        DrinkCostData->ItemSizePriceLevels[key].Price = sgItemsize->Cells[1][key];
                    }

					if (OldCost != DrinkCostData->Price || OldSpecialPrice != DrinkCostData->SpecialPrice || OldGST != DrinkCostData->GST || OldCostGST != DrinkCostData->CostGST)
					{
						MenuEdited = true;
						RelabelDrinkCosts();
					}
				}
				else
				{
					edRecipeQty->Value = 0;
					edQty->Caption = 0;
					edLoc->Text = "";
				}
			}
			else
			{
				edRecipeQty->Value = 0;
				edQty->Caption = 0;
				edLoc->Text = "";
			}
		}
	}
	__finally
	{
		edRecipeQty->OnChange = edLocChange;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::edQtyKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN || Key == VK_TAB)
	{
		edLoc->SetFocus();
		if (btnAddRecipe->Enabled)
		{
			btnAddRecipe->SetFocus();
		}
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::WozedItemCategoryKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN || Key == VK_TAB)
	{
		Key = NULL;
		btnAddRecipe->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnAddRecipeToSizesClick(TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		TList *MasterRecipeList = ((TItemSizeNode *)CurrentTreeNode->Data)->RecipeList;
		if(MasterRecipeList->Count > 0)
		{
			TRecipe *MasterRecipe = (TRecipe *) MasterRecipeList->Items[sgRecipe->Row-1];
			if (CurrentTreeNode->Parent->Count > 0)
			{
				if (Application->MessageBox(("This will add " + MasterRecipe->Description + " to all the other sizes for this item.\rDo you wish to continue?").c_str(),
							"Warning",
							MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
				{
					return;
				}

				for (int i=0; i<CurrentTreeNode->Parent->Count; i++)
				{
					TItemSizeNode *CurrentDCNode = ((TItemSizeNode *)CurrentTreeNode->Parent->Item[i]->Data);
					if (CurrentDCNode != tvMenu->Selected->Data)
					{
						TList *RecipeList = CurrentDCNode->RecipeList;

						TRecipe *Recipe = new TRecipe;
						RecipeList->Add(Recipe);
						Recipe->StockCode		= MasterRecipe->StockCode;
						Recipe->Description	= MasterRecipe->Description;
						Recipe->Unit			= MasterRecipe->Unit;
						Recipe->Location		= MasterRecipe->Location;//edLoc->Text;
						Recipe->ConversionFactor = MasterRecipe->ConversionFactor;
						Recipe->ReductionUnit = MasterRecipe->ReductionUnit;
						Recipe->Quantity = edQty->Caption.ToDouble();
						LastLocation = Recipe->Location;
					}
				}
				MenuEdited = true;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSetLocationForAllItemsClick(TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		AnsiString Location = edLoc->Text;
		if (CurrentTreeNode->Parent->Count > 0)
		{
			if (Application->MessageBox(("This will set the locations of all recipe items in this course to " + Location + ".\rDo you wish to continue?").c_str(),
						"Warning",
						MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
			{
				return;
			}
			for (int j=0; j<CurrentTreeNode->Parent->Parent->Count; j++)
			{
				TEditorNode *CurrentItemNode = ((TEditorNode *)CurrentTreeNode->Parent->Parent->Item[j]->Data);
				if (CurrentItemNode->NodeType == ITEM_NODE)
				{
					for (int i=0; i<CurrentItemNode->Owner->Count; i++)
					{
						TItemSizeNode *CurrentSizeNode = ((TItemSizeNode *)CurrentItemNode->Owner->Item[i]->Data);
						TList *RecipeList = CurrentSizeNode->RecipeList;
						for (int k=0; k<RecipeList->Count; k++)
						{
							TRecipe *Recipe = (TRecipe *)RecipeList->Items[k];
							Recipe->Location = Location;
						}
					}
				}
			}
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnAddRecipeToItemsClick(TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		TList *MasterRecipeList = ((TItemSizeNode *)CurrentTreeNode->Data)->RecipeList;
		if(MasterRecipeList->Count > 0)
		{
			TRecipe *MasterRecipe = (TRecipe *) MasterRecipeList->Items[sgRecipe->Row-1];

			//         if (Application->MessageBox(("This will add " + sgRecipe->Cells[1][sgRecipe->Row] + " to all the other sizes in this course.\rDo you wish to continue?").c_str(),
			if (Application->MessageBox(("This will add " + MasterRecipe->Description + " to all the other sizes in this course.\rDo you wish to continue?").c_str(),
						"Warning",
						MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
			{
				return;
			}
			for (int j=0; j<CurrentTreeNode->Parent->Parent->Count; j++)
			{
				TEditorNode *CurrentDrinkNode = ((TEditorNode *)CurrentTreeNode->Parent->Parent->Item[j]->Data);
				if (CurrentDrinkNode->NodeType == ITEM_NODE)
				{
					for (int i=0; i<CurrentDrinkNode->Owner->Count; i++)
					{
						TItemSizeNode *CurrentDCNode = ((TItemSizeNode *)CurrentDrinkNode->Owner->Item[i]->Data);
						if (CurrentDCNode != tvMenu->Selected->Data)
						{
							TList *RecipeList = CurrentDCNode->RecipeList;

							TRecipe *Recipe = new TRecipe;
							RecipeList->Add(Recipe);
							Recipe->StockCode		= MasterRecipe->StockCode;
							Recipe->Description	= MasterRecipe->Description;
							Recipe->Unit			= MasterRecipe->Unit;
							Recipe->ConversionFactor = MasterRecipe->ConversionFactor;
							Recipe->ReductionUnit = MasterRecipe->ReductionUnit;
							Recipe->Quantity = edQty->Caption.ToDouble();
							Recipe->Location = MasterRecipe->Location;//edLoc->Text;
							LastLocation = Recipe->Location;
						}
					}
				}
			}
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbSizesUnusedKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		btnAddItemSizeClick(Sender);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnAddItemSizeClick(TObject *Sender)
{
	if (tvMenu->Selected)
	{
		SaveNode(tvMenu->Selected);
	}
	if (lbSizesUnused->ItemIndex > -1 && lbSizesUnused->Items->Count > 0)
	{
		if (tvMenu->Selected != NULL)
		{
			if (lbSizesUsed->Items->Count >= MAX_COURSE_SIZES)
			{
				Application->MessageBox(("You can only have " + IntToStr(MAX_COURSE_SIZES) + " sizes per item.").c_str(),
				"Error",
				MB_OK + MB_ICONERROR);
				return;
			}
			MenuEdited = true;

			if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_NODE)
			{
				TTreeNode *NewDrinkCost;
				if (lbSizesUnused->Items->Strings[lbSizesUnused->ItemIndex] == DEFAULT_SIZE_STRING)
				{
					TTreeNode *FirstItemSize = tvMenu->Selected->getFirstChild();
					if (FirstItemSize)
					{
						NewDrinkCost = ((TEditorNode *)FirstItemSize->Data)->AddNode(ITEM_SIZE_NODE, true);
					}
					else
					{
						NewDrinkCost = ((TEditorNode *)tvMenu->Selected->Data)->AddNode(ITEM_SIZE_NODE, true);
					}
				}
				else
				{
					NewDrinkCost = ((TEditorNode *)tvMenu->Selected->Data)->AddNode(ITEM_SIZE_NODE, true);
				}

				reinterpret_cast<TEditorNode *>(NewDrinkCost->Data)->Key = (*new_menu_element_key_generator_)();
				reinterpret_cast<TItemSizeNode *>(NewDrinkCost->Data)->SizeKey =reinterpret_cast<__int32>(lbSizesUnused->Items->Objects[lbSizesUnused->ItemIndex]);
				reinterpret_cast<TItemSizeNode *>(NewDrinkCost->Data)->SizeKey =reinterpret_cast<__int32>(lbSizesUnused->Items->Objects[lbSizesUnused->ItemIndex]);

				((TItemSizeNode *)NewDrinkCost->Data)->LongDescription	= lbSizesUnused->Items->Strings[lbSizesUnused->ItemIndex];
				((TItemSizeNode *)NewDrinkCost->Data)->KitchenName       = GetKitchenNameOfSize(lbSizesUnused->Items->Strings[lbSizesUnused->ItemIndex]);
				((TItemSizeNode *)NewDrinkCost->Data)->Price					= 0;
				((TItemSizeNode *)NewDrinkCost->Data)->SpecialPrice		= 0;
				((TItemSizeNode *)NewDrinkCost->Data)->GST					= CurrentConnection.DefaultMenuGST;
				((TItemSizeNode *)NewDrinkCost->Data)->CostGST				= CurrentConnection.DefaultStockGST;


                 if (qrGetPriceLevelNames->Database->Connected && !qrGetPriceLevelNames->Transaction->InTransaction)
                {
                    qrGetPriceLevelNames->Transaction->StartTransaction();
                    qrGetPriceLevelNames->Close();
                    qrGetPriceLevelNames->ExecQuery();
                }
                    // when creating new item size make default  price 0 and update pricelevels name according the values stored in db
                    for(int key =1; key<= noOfPriceLevels; key++)
                    {
                        Menu::TItemSizePriceLevel priceLevel;
                        priceLevel.PriceLevelKey = key;
                        priceLevel.Price = 0;

                        ((TItemSizeNode *)NewDrinkCost->Data)->ItemSizePriceLevels.insert(std::pair<int, Menu::TItemSizePriceLevel>(key,priceLevel) );

                        Menu::TPriceLevelsName priceLevelName;
                        priceLevelName.PriceLevelKey = key;

                        if(!qrGetPriceLevelNames->Eof)priceLevelName.PriceLevelName = qrGetPriceLevelNames->FieldByName("PRICELEVEL_NAME")->AsString;

                         ((TItemSizeNode *)NewDrinkCost->Data)->PriceLevelsName.insert(std::pair<int, Menu::TPriceLevelsName>(key,priceLevelName) );

                        qrGetPriceLevelNames->Next();
                    }

				RelabelDrinkCosts();
				RefreshItem((TItemNode *)tvMenu->Selected->Data);
				lbSizesUnused->ItemIndex = 0;
			}
		}
	}
}

AnsiString TfrmMenuEdit::GetKitchenNameOfSize(AnsiString SizeName)
{
	AnsiString kitchenName = "";
	TTreeNode *SizesNode = tvMenu->Items->GetFirstNode()->Item[SIZES_INDEX];
	for (int i=0; i<SizesNode->Count; i++)
	{
		TSizeNode *SizeData = (TSizeNode *)SizesNode->Item[i]->Data;
		if(SizeName == SizeData->LongDescription)
		{
			kitchenName = SizeData->KitchenName;
			break;
		}
	}
	return kitchenName;
}



//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnDeleteItemSizeClick(TObject *Sender)
{
	if (tvMenu->Selected)
	{
		SaveNode(tvMenu->Selected);
	}
	if (lbSizesUsed->ItemIndex > -1 && lbSizesUsed->Items->Count > 0)
	{
		if (Application->MessageBox("Are you sure you want to remove this size?",
					"Menu Editor",
					MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDOK)
		{
			tvMenu->Selected->Item[lbSizesUsed->ItemIndex]->Delete();
			MenuEdited = true;
			RefreshMenuDetails();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnMoveSizeUpClick(TObject *Sender)
{
	if (tvMenu->Selected)
	{
		SaveNode(tvMenu->Selected);
	}
	if (lbSizesUsed->ItemIndex >= 1 &&
			//		 lbSizesUsed->Items->Strings[lbSizesUsed->ItemIndex-1] != DEFAULT_SIZE_STRING &&
			tvMenu->Selected->Count > lbSizesUsed->ItemIndex)
	{
		TTreeNode *NodeToMove	= tvMenu->Selected->Item[lbSizesUsed->ItemIndex];
		TTreeNode *DestNode		= tvMenu->Selected->Item[lbSizesUsed->ItemIndex-1];
		MenuEdited					= true;
		NodeToMove->MoveTo(DestNode, naInsert);
		int OldIndex				= lbSizesUsed->ItemIndex;
		RefreshMenuDetails();
		lbSizesUsed->ItemIndex	= OldIndex - 1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnMoveSizeDownClick(TObject *Sender)
{
	if (tvMenu->Selected)
	{
		SaveNode(tvMenu->Selected);
	}
	if (lbSizesUsed->ItemIndex != -1 &&
			lbSizesUsed->ItemIndex < lbSizesUsed->Items->Count-1 &&
			//		 lbSizesUsed->Items->Strings[lbSizesUsed->ItemIndex] != DEFAULT_SIZE_STRING &&
			tvMenu->Selected->Count > lbSizesUsed->ItemIndex+1)
	{
		TTreeNode *NodeToMove	= tvMenu->Selected->Item[lbSizesUsed->ItemIndex+1];
		TTreeNode *DestNode		= tvMenu->Selected->Item[lbSizesUsed->ItemIndex];
		MenuEdited					= true;
		NodeToMove->MoveTo(DestNode, naInsert);
		int OldIndex				= lbSizesUsed->ItemIndex;
		RefreshMenuDetails();
		lbSizesUsed->ItemIndex	= OldIndex + 1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbSizesUsedClick(TObject *Sender)
{
	lbSizesUnused->ItemIndex = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbSizesUnusedClick(TObject *Sender)
{
	lbSizesUsed->ItemIndex = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnAddSizeClick(TObject *Sender)
{
	AnsiString NewGlassName = "";
	if (InputQuery("New Size", "Enter a name for the size (Max " + IntToStr(MAX_SIZE_NAME_LENGTH) + " chars).", NewGlassName))
	{
		if (NewGlassName != "")
		{
			if (NewGlassName.Length() > MAX_SIZE_NAME_LENGTH)
			{
				if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
							"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
				{
					return;
				}
				NewGlassName = NewGlassName.SubString(1, MAX_SIZE_NAME_LENGTH);
			}

			MenuEdited = true;
			int Index = InsertSize(NewGlassName);
			RefreshMenuDetails();
			lbAvailableSizes->ItemIndex = Index;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSizesEditClick(TObject *Sender)
{
	if (lbAvailableSizes->ItemIndex > -1)
	{
		AnsiString OldSizeName = lbAvailableSizes->Items->Strings[lbAvailableSizes->ItemIndex];
		AnsiString SizeName = OldSizeName;
		if (InputQuery("Change Size Name", "Enter a new name for the size (Max " + IntToStr(MAX_SIZE_NAME_LENGTH) + " chars).", SizeName))
		{
			if (SizeName != "")
			{
				if (SizeName.Length() > MAX_SIZE_NAME_LENGTH)
				{
					if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
								"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
					{
						return;
					}
					SizeName = SizeName.SubString(1, MAX_SIZE_NAME_LENGTH);
				}
				if (lbAvailableSizes->Items->IndexOf(SizeName) != -1)
				{
					if (lbAvailableSizes->Items->IndexOf(SizeName) != lbAvailableSizes->ItemIndex)
					{
						Application->MessageBox("A size of this name already exists.",
						"Error", MB_ICONERROR + MB_OK);
						return;
					}
				}
				MenuEdited = true;
				int NewIndex = EditSize(SizeName, lbAvailableSizes->ItemIndex);
                if(NewIndex > -1)
                {
                   AllSizesForMenu.erase(OldSizeName);
                   TAvailableMenuSize availablesize;
                   availablesize.Key = NewIndex;
                   AllSizesForMenu[SizeName] = availablesize;
                }


				TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
				for (int i = FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
				{
					TTreeNode *CourseNode = MenuNode->Item[i];
					for (int j=0; j<CourseNode->Count; j++)
					{
						TTreeNode *ItemNode = CourseNode->Item[j];
						for (int k=0; k<ItemNode->Count; k++)
						{
							TTreeNode *ItemSizeNode = ItemNode->Item[k];
							TEditorNode *ItemSizeData = (TEditorNode *)ItemSizeNode->Data;
							if (ItemSizeData->NodeType == ITEM_SIZE_NODE)
							{
								if (ItemSizeData->LongDescription == OldSizeName)
								{
									ItemSizeData->LongDescription = SizeName;
								}
							}
						}
					}
				}
				//				int OldIndex = lbAvailableSizes->ItemIndex;
				RelabelDrinkCosts();
				RefreshMenuDetails();
				lbAvailableSizes->ItemIndex = NewIndex;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSizesDeleteClick(TObject *Sender)
{
	if (lbAvailableSizes->ItemIndex > -1)
	{
		bool ItemSizeStillExists = false;
		TTreeNode *ExistingItemSize = NULL;

		TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
		for (int i =FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
		{
			TTreeNode *CourseNode = MenuNode->Item[i];
			for (int j=0; j<CourseNode->Count; j++)
			{
				TTreeNode *ItemNode = CourseNode->Item[j];
				for (int k=0; k<ItemNode->Count; k++)
				{
					TTreeNode *ItemSizeNode = ItemNode->Item[k];
					TEditorNode *ItemSizeData = (TEditorNode *)ItemSizeNode->Data;
					if (ItemSizeData->NodeType == ITEM_SIZE_NODE)
					{
						if (ItemSizeData->LongDescription == lbAvailableSizes->Items->Strings[lbAvailableSizes->ItemIndex])
						{
							ItemSizeStillExists = true;
							ExistingItemSize = ItemSizeNode;
							break;
						}
					}
				}
				if (ItemSizeStillExists) break;
			}
			if (ItemSizeStillExists) break;
		}
		if (ItemSizeStillExists)
		{
			Application->MessageBox("An item size still exists and must be deleted first.", "Error",
			MB_OK + MB_ICONERROR);
			tvMenu->Selected = ExistingItemSize;
		}
		else
		{
			//delete the data against the size
			DeleteSize(lbAvailableSizes->ItemIndex);
            AllSizesForMenu.erase(lbAvailableSizes->Items->Strings[lbAvailableSizes->ItemIndex]);
			// delete from the list box
			lbAvailableSizes->Items->Delete(lbAvailableSizes->ItemIndex);
			int OldIndex = lbAvailableSizes->ItemIndex;
			RelabelDrinkCosts();
			RefreshMenuDetails();
			lbAvailableSizes->ItemIndex = OldIndex;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::Expand1Click(TObject *Sender)
{
	if (tvMenu->Selected)
	{
		tvMenu->Selected->Expand(true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::Collapse1Click(TObject *Sender)
{
	if (tvMenu->Selected)
	{
		tvMenu->Selected->Collapse(true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::miDeleteDrinksClick(TObject *Sender)
{
	MenuEdited = true;
	if (tvMenu->Selected != NULL)
	{
		if (tvMenu->IsEditing())
		{
			return;
		}
		if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == COURSE_NODE)
		{
			if (Application->MessageBox("Are you sure you want to delete this course and all of its items?",
						"Menu Editor",
						MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDOK)
			{
				Screen->Cursor = crHourGlass;
				try
				{
					tvMenu->Selected->Delete();
					Screen->Cursor = crDefault;
				}
				catch (Exception &E)
				{
					Screen->Cursor = crDefault;
					throw;
				}
			}
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_NODE)
		{
			if (Application->MessageBox("Are you sure you want to delete this item and all of its prices?",
						"Menu Editor",
						MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDOK)
			{
				tvMenu->Selected->Delete();
			}
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_SIZE_NODE)
		{
			if (Application->MessageBox("Are you sure you want to delete this item size?",
						"Menu Editor",
						MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDOK)
			{
				tvMenu->Selected->Delete();
			}
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == OPTION_NODE)
		{
			if (Application->MessageBox("Are you sure you want to delete this option?",
						"Menu Editor",
						MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDOK)
			{
				tvMenu->Selected->Delete();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::miDuplicateDrinkClick(TObject *Sender)
{
	if (tvMenu->Selected)
	{
		TEditorNode *EditorNodeData = (TEditorNode *)tvMenu->Selected->Data;
		if (EditorNodeData->NodeType == COURSE_NODE)
		{
			if (tvMenu->Selected->Parent)
			{
				MenuEdited = true;
                TTreeNode *OldNode = tvMenu->Selected; //for forced options duplicate course

				TTreeNode *NewNode = ((TEditorNode *)tvMenu->Selected->Parent->Data)->CopyNode(tvMenu->Selected);
				((TEditorNode *)NewNode->Data)->LongDescription = "Copy of " + ((TEditorNode *)NewNode->Data)->LongDescription;
				NewNode->Text = ((TEditorNode *)NewNode->Data)->LongDescription;   
				for (int i=0; i<NewNode->Count; i++)
				{
                    TTreeNode *OldNodeItem = ((TTreeNode *)OldNode->Item[i]);
					TTreeNode *NewNodeItem = ((TTreeNode *)NewNode->Item[i]);
					if (((TEditorNode *)NewNodeItem->Data)->NodeType == ITEM_NODE)
					{
                        TItemNode *ItemData = (TItemNode *)NewNodeItem->Data;
                        TItemNode *OldItemData = (TItemNode *)OldNodeItem->Data;
                        ItemData->ForcedOptions = OldItemData->ForcedOptions;
						UpdateDuplicatedThirdPartyCodes(NewNodeItem);

					}
				}
				RefreshMenuDetails();
				edCourseName->SetFocus();
				
			}
		}
		else if (EditorNodeData->NodeType == ITEM_NODE)
		{
			if (tvMenu->Selected->Parent)
			{
				MenuEdited = true;
                TTreeNode *OldNode = tvMenu->Selected;
                TItemNode *OldItemData = (TItemNode *)OldNode->Data;
                
				TTreeNode *NewNode = ((TEditorNode *)tvMenu->Selected->Parent->Data)->CopyNode(tvMenu->Selected); //
                TItemNode *NewItemData = (TItemNode *)NewNode->Data;
                NewItemData->ForcedOptions = OldItemData->ForcedOptions;

				((TEditorNode *)NewNode->Data)->LongDescription = "Copy of " + ((TEditorNode *)NewNode->Data)->LongDescription;
				((TEditorNode *)NewNode->Data)->Key = (*new_menu_element_key_generator_)(); // assign a new key for the duplicated item
				NewNode->Text = ((TEditorNode *)NewNode->Data)->LongDescription;
				UpdateDuplicatedThirdPartyCodes(NewNode);
				RefreshMenuDetails();
				pcItemDetails->ActivePage = tsItemDetails;
				cbItemName->SetFocus();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{
	if (Key == VK_DELETE)
	{
		miDeleteDrinksClick(Sender);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::pmDrinksBevsPopup(TObject *Sender)
{
	miDeleteDrinks->Enabled = false;
	miDuplicateDrink->Enabled = false;


	if (tvMenu->Selected != NULL)
	{
		TEditorNode *NodeData = (TEditorNode *)tvMenu->Selected->Data;
		NodeTypes NodeType = NodeData->NodeType;

		switch (NodeType)
		{
		case COURSE_NODE:
		case ITEM_NODE:			miDeleteDrinks->Enabled = true;
			miDuplicateDrink->Enabled = true;
			break;
		case ITEM_SIZE_NODE:
		case OPTION_NODE:			miDeleteDrinks->Enabled = true;
			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button == mbRight)
	{
		if (tvMenu->GetNodeAt(X, Y))
		{
			tvMenu->Selected = tvMenu->GetNodeAt(X, Y);
		}
		else
		{
			tvMenu->Selected = tvMenu->Items->GetFirstNode();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuEditing(TObject *Sender,
TTreeNode *Node, bool &AllowEdit)
{
	AllowEdit = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnCheckClick(TObject *Sender)
{
	CompilingDrinkWarnings = true;

	tvMenu->Selected = tvMenu->Items->GetFirstNode();
	if (tvMenu->Selected != tvMenu->Items->GetFirstNode())
	{
		return;
	}

	lbWarnings->Items->BeginUpdate();
	try
	{
		lbWarnings->Items->Clear();

		PageControl4->ActivePage	= tsErrorsWarnings;
		int WarningCount				= 0;
		int ErrorCount					= 0;
		lbeErrorsCount->Caption		= " Errors: " + IntToStr(ErrorCount);
		lbeWarningsCount->Caption	= " Warnings: " + IntToStr(WarningCount);
		pbWarnings->Position			= 0;
		pbWarnings->Max				= tvMenu->Items->Count;
		Application->ProcessMessages();

		std::map<AnsiString /*Code*/, std::set<AnsiString /*Location*/> > stockLookup;
		try
		{
			if (dmStockData->dbStock->Connected)
			{
				if (!dmStockData->trStock->InTransaction) dmStockData->trStock->StartTransaction();
				dmStockData->IBQuery2->Close();
				dmStockData->IBQuery2->SQL->Text =
				"Select "
				"Code, "
				"Location "
				"From "
				"Stock Inner Join StockLocation On "
				"Stock.Stock_Key = StockLocation.Stock_Key "
				"Order by "
				"Code, Location";
				dmStockData->IBQuery2->Open();
				while(!dmStockData->IBQuery2->Eof)
				{
					stockLookup[dmStockData->IBQuery2->FieldByName("Code")->AsString].insert(dmStockData->IBQuery2->FieldByName("Location")->AsString);
					dmStockData->IBQuery2->Next();
				}
			}
		}
		catch (EIBError &E)
		{
			dmStockData->dbStock->ForceClose();
		}

		TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();

		bool
		CheckRecipes = true,
		CheckNoPrice = true,
		CheckNoSpecialPrice = true,
		CheckNoThirdPartyCode = true;
        //check pricelevelnames are not duplicates
        bool ispriceLevelNameSame = false;
        bool ispriceLevelNameEmpty = false;
        bool ispriceLevelExist = false;

        for(int index =1; index<=noOfPriceLevels;index++)
        {
            AnsiString Value = PriceLevelsName[index].PriceLevelName;
            int i = Value.AnsiPos(" ");
            int length = strlen(Value.c_str());
            if(i == 1 || length == 0)
            {
                ispriceLevelNameEmpty = true;
            }
            for(int nextIndex = index+1; nextIndex<=noOfPriceLevels; nextIndex++ )
            {
                if((PriceLevelsName[index].PriceLevelName).LowerCase() == (PriceLevelsName[nextIndex].PriceLevelName).LowerCase())
                {
                     ispriceLevelNameSame = true;
                     break;
                }
            }


        if (qrGetAllPriceLevelNames->Database->Connected && !qrGetAllPriceLevelNames->Transaction->InTransaction)
        {
            qrGetAllPriceLevelNames->Transaction->StartTransaction();

        }
        qrGetAllPriceLevelNames->Close();
         qrGetAllPriceLevelNames->ExecQuery();

        while(!qrGetAllPriceLevelNames->Eof)
        {
           if((PriceLevelsName[index].PriceLevelName).LowerCase() == (qrGetAllPriceLevelNames->FieldByName("PRICELEVEL_NAME")->AsString).LowerCase()
                    && PriceLevelsName[index].PriceLevelKey != qrGetAllPriceLevelNames->FieldByName("PRICELEVEL_KEY")->AsInteger )
            {
                 ispriceLevelExist = true;
                 break;
            }
           qrGetAllPriceLevelNames->Next();
        }
    }

        if(qrGetAllPriceLevelNames->Transaction->InTransaction)
        {
            qrGetAllPriceLevelNames->Transaction->Commit();
        }

		TMenuNode *menuData = (TMenuNode *)MenuNode->Data;

		if (!rbDrinkMenu->Checked && !rbFoodMenu->Checked)
		{
			ErrorCount++;
			int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(MenuNode->AbsoluteIndex) + ": Select menu type (Food/Beverage).", menuData);
			if (MenuNode->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
		}
		else
		{
			AnsiString Key;
			int Result;
			if (rbDrinkMenu->Checked)
			{
				Key = OfficeKey + "\\" + CurrentConnection.CompanyName + "\\Editor Checks\\Drinks";
				Result = 1;
			}
			else
			{
				Key = OfficeKey + "\\" + CurrentConnection.CompanyName + "\\Editor Checks\\Food";
				Result = 0;
			}
			RegistryRead(Key, "Recipes", Result);
			CheckRecipes = Result;

			Result = 1;
			RegistryRead(Key, "NoPrice", Result);
			CheckNoPrice = Result;

			Result = 1;
			RegistryRead(Key, "NoSpecialPrice", Result);
			CheckNoSpecialPrice = Result;

			Result = 0;
			RegistryRead(Key, "3rdPartyCode", Result);
			CheckNoThirdPartyCode = Result;
		}

		if (menuData->LongDescription.Length() > MAX_MENU_NAME_LENGTH)
		{
			ErrorCount++;
			int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(MenuNode->AbsoluteIndex) + ": The menu name is too long.", menuData);
			if (MenuNode->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
		}

		/* Serving Courses   */
		//ServingCoursesList = new TStringList;
		//GetAllServingCourses(ServingCoursesList);
		bool ServingCourseSelectable = false; // Indicates that at least 1 serving course is available?
		for (int i=0; i<ServingCoursesList->Count; i+=6)
		{
			if (ServingCourseSelectable == false)
			ServingCourseSelectable = dmUtilities->StrToBoolean(ServingCoursesList->Strings[i+4]);
		}
		if (NumberOfCourses() == 0)
		{
			ErrorCount++;
			int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(MenuNode->AbsoluteIndex) + ": There is nothing to save.", menuData);
			if (MenuNode->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
		}
		std::set<AnsiString> allCategories;
		TTreeNode *CategoryGroupsNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
		for (int i=0; i<CategoryGroupsNode->Count; i++)
		{
			TTreeNode *CategoriesNode = CategoryGroupsNode->Item[i];
			for (int j=0; j<CategoriesNode->Count; j++)
			{
				TCategoryNode *CategoryData = (TCategoryNode *)CategoriesNode->Item[j]->Data;
				if (!CategoryData->GetDeleted())
				{
					allCategories.insert(CategoryData->LongDescription.UpperCase());
				}
			}
		}

		/*std::set<AnsiString> allSizes;
		TTreeNode *SizesNode = tvMenu->Items->GetFirstNode()->Item[SIZES_INDEX];
		for (int i=0; i<SizesNode->Count; i++)
		{
			TSizeNode *SizeData = (TSizeNode *)SizesNode->Item[i]->Data;
			allSizes.insert(SizeData->LongDescription);
		}*/

		TTreeNode *ServingCoursesNode  = tvMenu->Items->GetFirstNode()->Item[SERVING_COURSES_INDEX];
		std::map<AnsiString /*Name*/, bool /*Enabled*/> servingCoursesLookup;
		for (int sc=0; sc<ServingCoursesNode->Count; sc++)
		{
			TServingCourseNode *ServingCourseData = (TServingCourseNode *)ServingCoursesNode->Item[sc]->Data;
			servingCoursesLookup[ServingCourseData->LongDescription] = ServingCourseData->Enabled;
		}
		std::map<AnsiString, TCourseNode*> CourseNames;
		for (int i = FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
		{
			TTreeNode *CourseNode = MenuNode->Item[i];
			TCourseNode *CourseData = (TCourseNode *)CourseNode->Data;

			if (CourseData->LongDescription == "")
			{
				ErrorCount++;
				int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(CourseData->Owner->AbsoluteIndex) + ": Course has no name.", CourseData);
				if (CourseData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
			}
			else if (CourseData->LongDescription.Length() > MAX_COURSE_NAME_LENGTH)
			{
				ErrorCount++;
				int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(CourseData->Owner->AbsoluteIndex) + ": Course name is too long.", CourseData);
				if (CourseData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
			}
			else
			{
				std::map<AnsiString, TCourseNode*>::iterator i = CourseNames.find(CourseData->LongDescription);
				if(i != CourseNames.end())
				{
					TCourseNode *OtherCourseData = i->second;

					ErrorCount++;
					int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(CourseData->Owner->AbsoluteIndex) + ": Duplicate course name with N" + IntToStr(OtherCourseData->Owner->AbsoluteIndex) + ".", CourseData);
					if (CourseData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
					// Show conflicting node
					ErrorCount++;
					Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(OtherCourseData->Owner->AbsoluteIndex) + ": Duplicate course name with N" + IntToStr(CourseData->Owner->AbsoluteIndex) + ".", OtherCourseData);
					if (OtherCourseData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
				}
				else
				{
					CourseNames[CourseData->LongDescription] = CourseData;
				}
			}
			// Check Always Prompt and 1 must be selectable from Serving Courses
			if (CourseData->NoDefaultServingCourse == true)
			{
				if (ServingCourseSelectable == false)
				{
					ErrorCount++;
					int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(CourseData->Owner->AbsoluteIndex) + ": No serving course's are selectable on the POS", CourseData);
					if (CourseData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
				}
			}
			else
			{
				std::map<AnsiString, bool>::iterator i = servingCoursesLookup.find(CourseData->ServingCourse);
				if(i == servingCoursesLookup.end())
				{
					ErrorCount++;
					int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(CourseData->Owner->AbsoluteIndex) + ": Course has no default serving course", CourseData);
					if (CourseData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
				}
				else if(i->second == false)
				{
					ErrorCount++;
					int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(CourseData->Owner->AbsoluteIndex) + ": This serving course is not available in this menu", CourseData);
					if (CourseData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
				}
			}
			std::map<AnsiString, TItemNode*> ItemNames;
			for (int j=0; j<CourseNode->Count; j++)
			{
				TTreeNode *ItemNode = CourseNode->Item[j];

				if (((TEditorNode *)ItemNode->Data)->NodeType == ITEM_NODE)
				{
					pbWarnings->Position = ItemNode->AbsoluteIndex;
					TItemNode *ItemData = (TItemNode *)ItemNode->Data;

					if (ItemData->LongDescription == "")
					{
						ErrorCount++;
						int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemData->Owner->AbsoluteIndex) + ": Item has no name.", ItemData);
						if (ItemData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
					}
					else if (ItemData->LongDescription.Length() > MAX_ITEM_NAME_LENGTH)
					{
						ErrorCount++;
						int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemData->Owner->AbsoluteIndex) + ": Item name is too long.", ItemData);
						if (ItemData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
					}
					else
					{
						// Check for duplicate names.
						std::map<AnsiString, TItemNode*>::iterator i = ItemNames.find(ItemData->LongDescription);
						if(i != ItemNames.end())
						{
							TItemNode *OtherItemData = i->second;//(TItemNode *)ItemNames->Objects[ItemIndex];

							ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemData->Owner->AbsoluteIndex) + ": Duplicate item name with N" + IntToStr(OtherItemData->Owner->AbsoluteIndex) + ".", ItemData);
							if (ItemData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
							// Show conflicting node
							ErrorCount++;
							Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(OtherItemData->Owner->AbsoluteIndex) + ": Duplicate item name with N" + IntToStr(ItemData->Owner->AbsoluteIndex) + ".", OtherItemData);
							if (OtherItemData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
						}
						else
						{
							ItemNames[ItemData->LongDescription] = ItemData;//->AddObject(ItemData->LongDescription, ItemData);
						}
					}
					if (ItemNode->Count == 0)
					{
						ErrorCount++;
						int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemData->Owner->AbsoluteIndex) + ": Item has no sizes.", ItemData);
						if (ItemData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
					}
					else if (ItemNode->Count > MAX_COURSE_SIZES)
					{
						ErrorCount++;
						int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemData->Owner->AbsoluteIndex) + ": Item has too many sizes.", ItemData);
						if (ItemData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
					}

					for (int k=0; k<ItemNode->Count; k++)
					{
						TTreeNode *ItemSizeNode = ItemNode->Item[k];
						TItemSizeNode *ItemSizeData = (TItemSizeNode *)ItemSizeNode->Data;
						if (ItemSizeData->LongDescription == "" || AllSizesForMenu.find(ItemSizeData->LongDescription) == AllSizesForMenu.end()) //allSizes.find(ItemSizeData->LongDescription) == allSizes.end())//!FindCategory(ItemSizeData->Category, NULL, NULL, &deleted))
						{
							ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item size doesen't exist.", ItemSizeData);
							if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
						}
						else if (ItemSizeData->LongDescription.Length() > MAX_SIZE_NAME_LENGTH)
						{
							ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item size name is too long.", ItemSizeData);
							if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
						}

						if (ItemSizeData->Category == "" || allCategories.find(ItemSizeData->Category.UpperCase()) == allCategories.end())//!FindCategory(ItemSizeData->Category, NULL, NULL, &deleted))
						{
							ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item has no category.", ItemSizeData);
							if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
						}
						else if (ItemSizeData->Category.Length() > MAX_CATEGORY_LENGTH)
						{
							ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Category name is too long.", ItemSizeData);
							if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
						}

						if (CheckNoPrice)
						{
							// Check price only if !free & enabled & ( (!SetMenuItem) | (StandardItem) )
							if (ItemSizeData->Enabled && !ItemSizeData->Free &&
									(!TST_PROMO_ITEM(ItemSizeData->SetMenuMask) || ItemSizeData->SetMenuStandardItem))
							{
								if (ItemSizeData->ItemSizePriceLevels[1].Price == 0)
								{
									WarningCount++;
									int Index = lbWarnings->Items->AddObject("[Warning] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item 's pricelevel 1 is $0.00.", ItemSizeData);
									if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
								}
							}
						}
						if (CheckNoSpecialPrice)
						{
							// Check price only if !free & enabled & ( (!SetMenuItem) | (StandardItem) )
							if (ItemSizeData->Enabled && !ItemSizeData->Free &&
									(!TST_PROMO_ITEM(ItemSizeData->SetMenuMask) || ItemSizeData->SetMenuStandardItem))
							{
                                for (int index = 2; index <= noOfPriceLevels; index++)
                                {
                                    if (ItemSizeData->ItemSizePriceLevels[index].Price == 0)
                                    {
                                        WarningCount++;
                                        int Index = lbWarnings->Items->AddObject("[Warning] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) +" "+": Item 's pricelevel" + " "+ index +" "+"is $0.00.", ItemSizeData);
                                       
                                        if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
                                    }
                                }
							}
						}
						if (CheckNoThirdPartyCode)
						{
							if (ItemSizeData->ThirdPartyCode == "")
							{
								ErrorCount++;
								int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Third party code not specified.", ItemSizeData);
								if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
							}
						}

						if (CheckRecipes)
						{
							if (ItemSizeData->RecipeList->Count == 0 && ItemSizeData->Enabled)
							{
								WarningCount++;
								int Index = lbWarnings->Items->AddObject("[Warning] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item has no recipe.", ItemSizeData);
								if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
							}
							for (int l=0; l<ItemSizeData->RecipeList->Count; l++)
							{
								TRecipe *Recipe = (TRecipe *)ItemSizeData->RecipeList->Items[l];
								if (Recipe->Quantity == 0)
								{
									WarningCount++;
									int Index = lbWarnings->Items->AddObject("[Warning] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item recipe has no quantity.", ItemSizeData);
									if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag)
									lbWarnings->ItemIndex = Index;
								}

								if (Recipe->Location == "")
								{
									ErrorCount++;
									int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item recipe has no location.", ItemSizeData);
									if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
								}
								else
								{
									int recipeIndex = edLoc->Items->IndexOf(Recipe->Location);
									if (recipeIndex == -1 || edLoc->Items->Strings[recipeIndex] != Recipe->Location) // case sensitive check
									{
										WarningCount++;
										int Index = lbWarnings->Items->AddObject("[Warning] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item recipe location may be incorrect.", ItemSizeData);
										if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
									}
								}
								if (dmStockData->dbStock->Connected)
								{
									std::map<AnsiString, std::set<AnsiString> >::iterator i = stockLookup.find(Recipe->StockCode);
									if(i == stockLookup.end())
									{
										ErrorCount++;
										int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item recipe stock code does not exist.", ItemSizeData);
										if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
									}
									else if(i->second.find(Recipe->Location) == i->second.end())
									{
										ErrorCount++;
										int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) +
										": Stock item (" + Recipe->StockCode + ") location (" + Recipe->Location + ") doesn't exist.", ItemSizeData);
										if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
									}
								}
							}
						}
						if (ItemSizeData->TaxProfileKeys.empty())
						{
							ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item has no tax profiles assigned.", ItemSizeData);
							if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
						}
						else if(!ItemSizeData->TaxProfileKeys.empty())
						{
							bool isProfitTaxAssigned = IsProfitTaxAssigned(ItemSizeData->TaxProfileKeys);

							if(isProfitTaxAssigned && ItemSizeData->MaxRetailPrice == 0.00)
							{
								ErrorCount++;
								int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item has tax profit assigned, but max price is not defined for the item.", ItemSizeData);
								if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag)
									lbWarnings->ItemIndex = Index;
							}
						}
                        if(ispriceLevelNameSame)
                        {
                             ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item has same price level names.", ItemSizeData);
							if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
                        }
                        if(ispriceLevelNameEmpty)
                        {
                             ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Item has price level name empty.", ItemSizeData);
							if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
                        }
                        if(ispriceLevelExist)
                        {
                             ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(ItemSizeData->Owner->AbsoluteIndex) + ": Please Change Price Level Name.", ItemSizeData);
							if (ItemSizeData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
                        }
					}
				}
				if (((TEditorNode *)ItemNode->Data)->NodeType == OPTIONS_NODE)
				{
					std::auto_ptr<TStringList> OptionNames(new TStringList());
					OptionNames->CaseSensitive = true;
					OptionNames->Sorted = true;
					for (int k = 0; k < ItemNode->Count; k++)
					{
						TTreeNode *OptionNode = ItemNode->Item[k];
						TEditorNode *OptionData = (TEditorNode *)OptionNode->Data;
						if (OptionData->LongDescription == "")
						{
							ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(OptionData->Owner->AbsoluteIndex) + ": Option has no name.", OptionData);
							if (OptionData->Owner->AbsoluteIndex == lbWarnings->Tag)
								lbWarnings->ItemIndex = Index;
						}
						else if (OptionData->LongDescription.Length() > MAX_OPTION_NAME_LENGTH)
						{
							ErrorCount++;
							int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(OptionData->Owner->AbsoluteIndex) + ": Option name is too long.", OptionData);
							if (OptionData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
						}
						else
						{
							// Check for duplicate names
							int OptionIndex = OptionNames->IndexOf(OptionData->LongDescription);
							if (OptionIndex != -1)
							{
								TOptionNode *OtherOptionData = (TOptionNode *)OptionNames->Objects[OptionIndex];

								ErrorCount++;
								int Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(OptionData->Owner->AbsoluteIndex) + ": Duplicate option name with N" + IntToStr(OtherOptionData->Owner->AbsoluteIndex) + ".", OptionData);
								if (OptionData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
								// Show conflicting node
								ErrorCount++;
								Index = lbWarnings->Items->AddObject("[Error] N" + IntToStr(OtherOptionData->Owner->AbsoluteIndex) + ": Duplicate option name with N" + IntToStr(OptionData->Owner->AbsoluteIndex) + ".", OtherOptionData);
								if (OtherOptionData->Owner->AbsoluteIndex == lbWarnings->Tag) lbWarnings->ItemIndex = Index;
							}
							else
							{
								OptionNames->AddObject(OptionData->LongDescription, OptionData);
							}
						}
					}
				}
				if (lbeErrorsCount->Caption != " Errors: " + IntToStr(ErrorCount)) lbeErrorsCount->Caption = " Errors: " + IntToStr(ErrorCount);
				if (lbeWarningsCount->Caption != " Warnings: " + IntToStr(WarningCount)) lbeWarningsCount->Caption = " Warnings: " + IntToStr(WarningCount);
				Application->ProcessMessages();
				if (tvMenu->Selected != tvMenu->Items->GetFirstNode()) break;
			}
			if (tvMenu->Selected != tvMenu->Items->GetFirstNode()) break;
		}

		lbeErrorsCount->Caption = " Errors: " + IntToStr(ErrorCount);
		lbeWarningsCount->Caption = " Warnings: " + IntToStr(WarningCount);
		DrinkErrorCount = ErrorCount;
		CompilingDrinkWarningsCompleted = (tvMenu->Selected == tvMenu->Items->GetFirstNode());
		CompilingDrinkWarnings = false;
		pbWarnings->Position = 0;
		if (dmStockData->dbStock->Connected)
		{
			if (dmStockData->trStock->InTransaction)
			{
				dmStockData->trStock->Commit();
			}
		}
	}
	__finally
	{
		lbWarnings->Items->EndUpdate();
	}
}

bool TfrmMenuEdit::IsProfitTaxAssigned(TaxProfileKeyList assignedTaxProfileKeys)
{
	TaxProfileKeyIterator it = assignedTaxProfileKeys.begin();
	int profitTaxDbKey = 0;

	TaxProfileController* taxProfileController = new TaxProfileController();

	for( ; it != assignedTaxProfileKeys.end(); it++ )
	{
		TaxProfile* profitTax = taxProfileController->GetTaxProfile(*it);
		if(profitTax  && profitTax->taxProfileType == ProfitTax)
		{
			return true;
		}
	}

	return false;
}



//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnLoadMenuClick(TObject *Sender)
{
	LoadMenu();
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::LoadServingCoursesPrior3Point4(TMenuNode *MenuData)
{
	TTreeNode			       *ServingCoursesNode;
	ServingCoursesNode	    = MenuData->AddNode(SERVING_COURSES_NODE);
	ServingCoursesNode->Text = "Available Serving Courses";
	if (dmMMData->dbMenuMate->Connected)
	{
		Menu::TMenuLoadDB	   		MenuLoader(dmMMData->dbMenuMate);  // no Menu Key
		Menu::TServingCoursesInfo	ServingCoursesInfo;
		if (MenuLoader.GetAvailableServingCourses(&ServingCoursesInfo))
		AddServingCourses(&ServingCoursesInfo,ServingCoursesNode);
	}
}

//---------------------------------------------------------------------------

void TfrmMenuEdit::GetThirdPartyCodesListFromFile(std::vector<Menu::TThirdPartyCodeInfo> *thirdPartyCodes, TLoadMenu *inLoadMenu)
{
	int tpcCount = inLoadMenu->ThirdPartyCodeCount();
	if( tpcCount > 0)
	{
		for(int i=0;i< tpcCount ; i++)
		{
			Menu::TThirdPartyCodeInfo tpcInfo;
			inLoadMenu->ThirdPartyCodeAtIndex(
			i,
			tpcInfo.Key,
			tpcInfo.Code,
			tpcInfo.Description,
			tpcInfo.Visible,
			tpcInfo.Code_Type);

			thirdPartyCodes->push_back(tpcInfo);
		}
	}
}

//---------------------------------------------------------------------------

AnsiString  TfrmMenuEdit::GetThirdPartyCodeFromKeyFromFile(std::vector<Menu::TThirdPartyCodeInfo> *thirdPartyCodes, __int32 tpcKey)
{
	AnsiString tpcCode = "";
	std::vector<Menu::TThirdPartyCodeInfo>::iterator tpcIT = thirdPartyCodes->begin();

	for( ; tpcIT != thirdPartyCodes->end(); tpcIT++ )
	{
		if( tpcIT->Key == tpcKey)
		{
			tpcCode = tpcIT->Code;
			break;
		}
	}

	return tpcCode;
}

//---------------------------------------------------------------------------
void TfrmMenuEdit::LoadCategories(CategoryTranslation::TUpdateCategories *UpdateCategories)
{
	TTreeNode *CatGpsNode			= tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
	TTreeNode *CategoryGroupNode	= NULL;
	for (int i=0; i<UpdateCategories->GroupCount(); i++)
	{
		CategoryTranslation::TCategoryTranslation *CategoryGroup = UpdateCategories->GetGroup(i);

		int GroupIndex;
		if (FindCategoryGroup(CategoryGroup->NewCategoryName, &GroupIndex))
		{
			TTreeNode *CatGpsNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
			CategoryGroupNode = CatGpsNode->Item[GroupIndex];
		}
		else
		{
			CategoryGroupNode			= ((TEditorNode *)CatGpsNode->Data)->AddNode(CATEGORY_GROUP_NODE, false);
			CategoryGroupNode->Text	= CategoryGroup->NewCategoryName;
			((TCategoryGroupNode *)CategoryGroupNode->Data)->LongDescription = CategoryGroupNode->Text;
		}
		for (int j=0; j<UpdateCategories->CategoryCount(i); j++)
		{
			CategoryTranslation::TCategoryTranslation *Category	= UpdateCategories->GetCategory(i, j);

			if (!FindCategory(Category->NewCategoryName, NULL, NULL, NULL))
			{
				TTreeNode *CategoryNode				= ((TEditorNode *)CategoryGroupNode->Data)->AddNode(CATEGORY_NODE, false);
				CategoryNode->Text					= Category->NewCategoryName;
				((TCategoryNode *)CategoryNode->Data)->LongDescription = CategoryNode->Text;
			}
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::LoadCategories(TCategoryList& CategoryList)
{
	qrCategories->Close();
	for (qrCategories->ExecQuery(); !qrCategories->Eof; qrCategories->Next())
	{
		TCategoryList::iterator iList = CategoryList.find(TCategoryBrief(qrCategories->FieldByName("Category_Group_Name")->AsString));
		if (iList != CategoryList.end())
		{
			std::set<TCategoryBrief>::iterator iCat = iList->second.find(TCategoryBrief(qrCategories->FieldByName("Category")->AsString, false));
			if (iCat != iList->second.end())
			{
				CategoryList[qrCategories->FieldByName("Category_Group_Name")->AsString].insert(TCategoryBrief(qrCategories->FieldByName("Category")->AsString, true));
			}
		}
	}
	TCategoryList::iterator iList;
	for (iList=CategoryList.begin(); iList!=CategoryList.end(); ++iList)
	{
		TTreeNode *groupTreeNode = ((TEditorNode *)tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX]->Data)->AddNode(CATEGORY_GROUP_NODE, false);
		groupTreeNode->Text = iList->first.Name;

		TCategoryGroupNode *groupData = (TCategoryGroupNode *)groupTreeNode->Data;
		groupData->LongDescription = iList->first.Name;

		std::set<TCategoryBrief>::iterator iCat;
		for (iCat=iList->second.begin(); iCat!=iList->second.end(); ++iCat)
		{
			TTreeNode *catTreeNode = groupData->AddNode(CATEGORY_NODE, false);
			catTreeNode->CustomSort(CompareFunc,1);
			catTreeNode->Text = iCat->Name;

			TCategoryNode *catData = (TCategoryNode *)catTreeNode->Data;
			catData->LongDescription = iCat->Name;
			catData->SetDeleted(iCat->Deleted);
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::LoadCategories()
{
	qrCategories->Close();
	int LastGroupKey = 0;
	TTreeNode *NewGroup = NULL;
	TTreeNode *CategoryNode = NULL;
	for (qrCategories->ExecQuery(); !qrCategories->Eof; qrCategories->Next())
	{
		if (LastGroupKey == 0 || qrCategories->FieldByName("CategoryGroups_Key")->AsInteger != LastGroupKey || NewGroup == NULL)
		{
			// returns the Category Node or Null
			CategoryNode = FindCategoryGroup(qrCategories->FieldByName("Category_Group_Name")->AsString);
			// create a NewGroup if not already there
			if (CategoryNode == NULL)
			{
				NewGroup = ((TEditorNode *)tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX]->Data)->AddNode(CATEGORY_GROUP_NODE, false);
				NewGroup->Text = qrCategories->FieldByName("Category_Group_Name")->AsString;
				((TCategoryGroupNode *)NewGroup->Data)->LongDescription = NewGroup->Text;
				LastGroupKey = qrCategories->FieldByName("CategoryGroups_Key")->AsInteger;
			}
		}
		if (!FindCategory(qrCategories->FieldByName("Category")->AsString, NULL, NULL, NULL))
		{
			TTreeNode *NewCategory;
			if (CategoryNode == NULL) // New Category_Group_Name
			{
				NewCategory = ((TEditorNode *)NewGroup->Data)->AddNode(CATEGORY_NODE, false);
				NewGroup->CustomSort(CompareFunc,1);
			}
			else // Existing Category_Group_Name - add onto this Node
			{
				NewCategory = ((TEditorNode *)CategoryNode->Data)->AddNode(CATEGORY_NODE, false);
				CategoryNode->CustomSort(CompareFunc,1);
			}
			NewCategory->Text = qrCategories->FieldByName("Category")->AsString;
			((TCategoryNode *)NewCategory->Data)->LongDescription = NewCategory->Text;
		}
	}
}

//---------------------------------------------------------------------------
AnsiString TfrmMenuEdit::ReadCSVWord(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape)
{
	AnsiString Temp = "";

	int StartPos			= Offset + 1;	// Not Zero based indexing.
	bool Escaped			= false;
	bool InsideQualifier	= false;
	bool Completed			= false;

	for (int i=StartPos; i<=Line.Length() && !Completed; i++)
	{
		if (Escape != NULL && Line[i] == Escape)
		{
			if (Escaped)
			{
				Temp += Escape;
				Escaped = false;
			}
			else
			{
				Escaped = true;
			}
		}
		else if (Qualifier != NULL && Line[i] == Qualifier)	// Check for "
		{
			if (Escaped)
			{
				Temp += Qualifier;
				Escaped = false;
			}
			else
			{
				InsideQualifier = !InsideQualifier;
			}
		}
		else if (Line[i] == Delimiter)	// Check for ,
		{
			if (Escaped)
			{
				Temp += Delimiter;
				Escaped = false;
			}
			else if (InsideQualifier)
			{
				Temp += Delimiter;
			}
			else
			{
				Completed = true;
			}
		}
		else
		{
			if (Escaped)
			{
				Temp += Escape;	// Shouldn't be escape. Assume the Escape char was part of the word.
				Escaped = false;
			}
			Temp += Line[i];
		}
		Offset++;
	}
	//   Offset++;
	return Temp.Trim();
}
//---------------------------------------------------------------------------
AnsiString TfrmMenuEdit::EscapeString(AnsiString Line, char Qualifier, char Escape)
{
	AnsiString Temp;
	for (int i=1; i<=Line.Length(); i++)
	{
		if (Line[i] == Qualifier || Line[i] == Escape)
		{
			Temp += Escape;
		}
		Temp += Line[i];
	}
	return Temp;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnCommitClick(TObject *Sender)
{
	TCursor Save_Cursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;
	try
	{
		tvMenu->Selected = tvMenu->Items->GetFirstNode();
		btnCheckClick(NULL);
		Screen->Cursor = Save_Cursor;
	}
	catch (Exception &E)
	{
		Screen->Cursor = Save_Cursor;
		throw;
	}
	if (tvMenu->Selected != tvMenu->Items->GetFirstNode())
	{
		return;
	}
	if (CompilingDrinkWarnings)
	{
		return;
	}
	if (CompilingDrinkWarningsCompleted) // Switched nodes while compiling.
	{
		if (DrinkErrorCount > 0)
		{
			Application->MessageBox("There are errors in the menu. Please resolve these first.", "MenuMate", MB_OK + MB_ICONERROR);
			return;
		}
	}
	else
	{
		return;
	}
	if (Application->MessageBox("Are your sure you want to force this menu? This will cause Palms to lose synch", "MenuMate", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
	{
		AnsiString FilePath = CurrentConnection.ServerPath + "\\Menu Import";
		AnsiString BackupFilePath = CurrentConnection.ServerPath + "\\Menu Backup";
		if (!DirectoryExists(FilePath))
		{
			if (!CreateDir(FilePath))
			{
				Application->MessageBox(("Cannot create directory " + FilePath).c_str(), "MenuMate", MB_OK + MB_ICONERROR);
				return;
			}
		}
		if (!DirectoryExists(BackupFilePath))
		{
			CreateDir(BackupFilePath);
		}
		TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
		TMenuNode *MenuData = (TMenuNode *)MenuNode->Data;

		Screen->Cursor = crHourGlass;
		try
		{
			// save the menu file as partial - so the pos wont recognize it until it is been completely saved
			SaveMenu(FilePath + "\\" + MenuData->LongDescription + ".partial",
						BackupFilePath + "\\" + "Backup of " + MenuData->LongDescription + " " + Now().FormatString("yymmddhhnnss") + ".csv");

			// rename is back to xml so the pos will import the file
			RenameFile( FilePath + "\\" + MenuData->LongDescription + ".partial", FilePath + "\\" + MenuData->LongDescription + ".xml" );
			Screen->Cursor = crDefault;
		}
		catch (Exception &E)
		{
			Screen->Cursor = crDefault;
			throw;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSavMenuClick(TObject *Sender)
{
	TCursor Save_Cursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;
	try
	{
		tvMenu->Selected = tvMenu->Items->GetFirstNode();
		btnCheckClick(NULL);
		Screen->Cursor = Save_Cursor;
	}
	catch (Exception &E)
	{
		Screen->Cursor = Save_Cursor;
		throw;
	}
	if (tvMenu->Selected != tvMenu->Items->GetFirstNode())
	{
		return;
	}
	if (CompilingDrinkWarnings)
	{
		return;
	}
	if (CompilingDrinkWarningsCompleted) // Switched nodes while compiling.
	{
		if (DrinkErrorCount > 0)
		{
			Application->MessageBox("There are errors in the menu. Please resolve these first.",
			"MenuMate",
			MB_OK + MB_ICONERROR);
			return;
		}
	}
	else
	{
		return;
	}
	TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
	TMenuNode *MenuData = (TMenuNode *)MenuNode->Data;

	SaveDialog1->Title = "Backup Menu";
	//  SaveDialog1->FileName = MenuData->LongDescription + ".csv";
	SaveDialog1->FileName = MenuData->LongDescription + ".xml";

	//	AnsiString SavePath = dmMMData->MenuSavePath + "\\Menus";
	AnsiString SavePath = CurrentConnection.MenuSavePath + "\\Menus";
	if (!DirectoryExists(SavePath))
	{
		if (!CreateDir(SavePath))
		{
			//			SavePath = dmMMData->MenuSavePath;
			SavePath = CurrentConnection.MenuSavePath;
		}
	}
	SaveDialog1->InitialDir = SavePath;
	if (SaveDialog1->Execute())
	{
		if (FileExists(SaveDialog1->FileName))
		{
			if (Application->MessageBox("The file already exists. Do you wish to replace it?",
						"Menu Editor",
						MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDCANCEL)
			{
				return;
			}
		}
		Screen->Cursor = crHourGlass;
		try
		{
			SaveMenu(SaveDialog1->FileName, "");
			OpenDialog1->FileName = SaveDialog1->FileName;
			Screen->Cursor = crDefault;
		}
		catch (Exception &E)
		{
			Screen->Cursor = crDefault;
			throw;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuDragOver(TObject *Sender,
TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	tvMenu->DragCursor = crArrow;
	TTreeView *TreeView = (TTreeView *)Source;
	if (GetKeyState(VK_LBUTTON) < 0)
	{
		if (Y <= 17 && TreeView->TopItem != TreeView->Items->GetFirstNode())
		{
			TreeView->DoubleBuffered = true;
			TreeView->Items->BeginUpdate();
			TreeView->Perform(WM_VSCROLL, SB_LINEUP, 0);
			TRect R(0, 0, TreeView->ClientWidth, 50);
			InvalidateRect(TreeView->Handle, &R, true);
			TreeView->Items->EndUpdate();
			TreeView->DoubleBuffered = false;
		}
		if (Y >= TreeView->ClientHeight-17)
		{
			TreeView->DoubleBuffered = true;
			TreeView->Items->BeginUpdate();
			TreeView->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
			TRect R(0, TreeView->ClientHeight - 50, TreeView->ClientWidth, TreeView->Height);
			InvalidateRect(TreeView->Handle, &R, true);
			TreeView->Items->EndUpdate();
			TreeView->DoubleBuffered = false;
		}
	}
	Accept = false;
	if (tvMenu->GetNodeAt(X, Y) == NULL) // Not actually dropping it anywhere
	{
		tvMenu->DropTarget = NULL;
	}
	if (tvMenu->DropTarget != NULL)
	{
		TEditorNode *Drag = (TEditorNode *)tvMenu->Selected->Data;  // Whats being dragged
		TEditorNode *Drop = (TEditorNode *)tvMenu->DropTarget->Data;  // What is being dropped on

		if (Drag->NodeType == ITEM_SIZE_NODE)
		{
			if (Drop->NodeType == ITEM_NODE)
			{
				Accept = (Drop == Drag->Owner->Parent->Data);
			}
			else if (Drop->NodeType == ITEM_SIZE_NODE)
			{
				Accept = (Drop->Owner->Parent == Drag->Owner->Parent);
			}
		}
		else if (Drag->NodeType == ITEM_NODE)
		{
			if (Drop->NodeType == ITEM_NODE || Drop->NodeType == COURSE_NODE || Drop->NodeType == OPTIONS_NODE)
			{
				Accept = true;
			}
		}
		// can only drop on a Category Group
		else if (Drag->NodeType == CATEGORY_NODE) // cww
		{
			if (Drop->NodeType == CATEGORY_GROUP_NODE)
			{
				Accept = true;
			}
		}
		else if (Drag->NodeType == SERVING_COURSE_NODE) // cww
		{
			if (Drop->NodeType == SERVING_COURSE_NODE)
			{
				Accept = true;
			}
		}
		else if (Drag->NodeType == COURSE_NODE)
		{
			if (Drop->NodeType == COURSE_NODE || Drop->NodeType == MENU_NODE)
			{
				Accept = true;
			}
		}
		else if (Drag->NodeType == OPTION_NODE)
		{
			if (Drop->NodeType == OPTION_NODE)
			{
				Accept = true;
				if (Drag->Owner->Parent->Data != Drop->Owner->Parent->Data)
				{
					tvMenu->DragCursor = crMultiDrag;
				}
			}
			else if (Drop->NodeType == OPTIONS_NODE)
			{
				Accept = true;
				if (Drag->Owner->Parent->Data != Drop)
				{
					tvMenu->DragCursor = crMultiDrag;
				}
			}
			else if (Drop->NodeType == COURSE_NODE)
			{
				Accept = true;
				if (Drag->Owner->Parent->Data != Drop)
				{
					tvMenu->DragCursor = crMultiDrag;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuDragDrop(TObject *Sender,
TObject *Source, int X, int Y)
{
	TTreeNode *DraggedNode		 	= ((TTreeView *)Source)->Selected;
	TTreeNode *DestNode				= ((TTreeView *)Sender)->DropTarget;

	TEditorNode *DraggedNodeData	= (TEditorNode *)DraggedNode->Data;
	TEditorNode *DestNodeData		= (TEditorNode *)DestNode->Data;

	MenuEdited = true;
	if (DraggedNodeData->NodeType == ITEM_SIZE_NODE)
	{
		if (DestNodeData->NodeType == ITEM_SIZE_NODE)
		{
			DraggedNode->MoveTo(DestNode, naInsert);
		}
		else if (DestNodeData->NodeType == ITEM_NODE)
		{
			DraggedNode->MoveTo(DestNode, naAddChild);
		}
	}
	// can only drop on a Category Group
	if (DraggedNodeData->NodeType == CATEGORY_NODE) //cww
	{
		if (DestNodeData->NodeType == CATEGORY_GROUP_NODE)
		{
			tvMenu->Items->BeginUpdate();
			DraggedNode->MoveTo(DestNode, naAddChild);

			TCategoryNode *catData = (TCategoryNode *)DraggedNodeData;
			TCategoryGroupNode *catGrpData = (TCategoryGroupNode *)DestNodeData;
			if (catGrpData->GetDeleted())
			{
				if (!catData->GetDeleted())
				{
					catGrpData->SetDeleted(false); // undelete the group if necessary
				}
			}
			// re-sort the Category Nodes
			DestNodeData->Owner->CustomSort(CompareFunc, 1);
			DestNodeData->Owner->Expand(false);
			tvMenu->Items->EndUpdate();
			//			DraggedNode->StateImage = -1;
			catData->SetDeleted(catData->GetDeleted()); // Make sure the icon is up to date
		}
	}
	if (DraggedNodeData->NodeType == SERVING_COURSE_NODE) //cww
	{
		if (DestNodeData->NodeType == SERVING_COURSE_NODE)
		{
			tvMenu->Items->BeginUpdate();
			DraggedNode->MoveTo(DestNode, naInsert);
			DraggedNode->StateIndex = -1;
			if (DraggedNodeData->Enabled == false)
			DraggedNode->StateIndex = DISABLEDSTATE_PIC;
			//RefreshServingCourses((TServingCoursesNode *)DestNode->Owner);
			tvMenu->Items->EndUpdate();

		}
	}
	if (DraggedNodeData->NodeType == OPTION_NODE)
	{
		TOptionNode *SourceOption = (TOptionNode *)DraggedNodeData;
		TOptionsNode *SourceOptions = (TOptionsNode *)DraggedNodeData->Owner->Parent->Data;
		if (DestNodeData->NodeType == OPTION_NODE)
		{
			TOptionsNode *DestOptions = (TOptionsNode *)DestNodeData->Owner->Parent->Data;
			if (SourceOptions == DestOptions)
			{
				DraggedNode->MoveTo(DestNode, naInsert);
			}
			else
			{
				TTreeNode *NewNode = SourceOption->CopyNode(SourceOption->Owner);
				NewNode->MoveTo(DestNode, naInsert);
				NewNode->Text = ((TEditorNode *)NewNode->Data)->LongDescription;
			}
		}
		else if (DestNodeData->NodeType == OPTIONS_NODE)
		{
			TOptionsNode *DestOptions = (TOptionsNode *)DestNodeData;
			if (SourceOptions == DestOptions)
			{
				DraggedNode->MoveTo(DestNode, naAddChild);
			}
			else
			{
				TTreeNode *NewNode = SourceOption->CopyNode(SourceOption->Owner);
				NewNode->MoveTo(DestNode, naAddChild);
				NewNode->Text = ((TEditorNode *)NewNode->Data)->LongDescription;
			}
		}
		// Drag Option_Node onto a Course
		else if (DestNodeData->NodeType == COURSE_NODE)
		{
			DestNode = DestNode->GetLastChild();
			DestNodeData = (TEditorNode *)DestNode->Data;

			TOptionsNode *DestOptions = (TOptionsNode *)DestNodeData;
			if (SourceOptions == DestOptions)
			{
				DraggedNode->MoveTo(DestNode, naAddChild);
			}
			else
			{
				TTreeNode *NewNode = SourceOption->CopyNode(SourceOption->Owner);
				NewNode->MoveTo(DestNode, naAddChild);
				NewNode->Text = ((TEditorNode *)NewNode->Data)->LongDescription;
			}
		}
	}
	else if (DraggedNodeData->NodeType == ITEM_NODE)
	{    TOptionNode *SourceOption = (TOptionNode *)DraggedNodeData;  //changes for forced options.
		TCourseNode *SourceCourse = (TCourseNode *)DraggedNodeData->Owner->Parent->Data;
		if (DestNodeData->NodeType == ITEM_NODE || DestNodeData->NodeType == OPTIONS_NODE)
		{
			TCourseNode *DestCourse = (TCourseNode *)DestNodeData->Owner->Parent->Data;
			DraggedNode->MoveTo(DestNode, naInsert);
           
              UpdateItemForForcedOptions();
            
		}
		else if (DestNodeData->NodeType == COURSE_NODE)
		{
			TCourseNode *DestCourse = (TCourseNode *)DestNodeData;
			if (DestNode->Count > 0)
			{
				DraggedNode->MoveTo(DestNode->Item[DestNode->Count-1], naInsert);
                UpdateItemForForcedOptions();                
			}
			else
			{
				DraggedNode->MoveTo(DestNode, naAddChild);
			}
		}
	}
	else if (DraggedNodeData->NodeType == COURSE_NODE)
	{    
		if (DestNodeData->NodeType == COURSE_NODE)
		{
			DraggedNode->MoveTo(DestNode, naInsert);
            //UpdateItemForForcedOptions();
		}
		else if (DestNodeData->NodeType == MENU_NODE)
		{
			DraggedNode->MoveTo(DestNode, naAddChild);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::FormCloseQuery(TObject *Sender,
bool &CanClose)
{
	if (MenuEdited)
	{
		if (Application->MessageBox("You have not saved. Are you sure you want to quit?",
					"Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) != IDOK)
		{
			CanClose = false;
		}
	}
	if (CanClose)
	{
		ClearTree();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnPreviewClick(TObject *Sender)
{
	if (SaveNode(tvMenu->Selected))
	{
		// Clear buttons
		while(frmPreview->sbDishes->ControlCount > 0)
		{
			delete frmPreview->sbDishes->Controls[0];
		}

		frmPreview->sbCourses->DisableAutoRange();
		frmPreview->sbCourses->VertScrollBar->Position = 0;
		TTreeNode *ClassesNode = tvMenu->Items->GetFirstNode();
		TTouchButton *CourseButton = NULL;

		frmPreview->CurrentCourseBtn = frmPreview->TouchButton1;
		TTreeNode *SelectedClass;
		if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == COURSE_NODE)
		{
			SelectedClass = tvMenu->Selected;
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_NODE)
		{
			SelectedClass = tvMenu->Selected->Parent;
		}
		else if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_SIZE_NODE)
		{
			SelectedClass = tvMenu->Selected->Parent->Parent;
		}

		int Count = 0;
		for (int i=FIRST_COURSE_INDEX; i<ClassesNode->Count && Count < 32; i++)
		{
			TTreeNode *ClassNode = ClassesNode->Item[i];
			TCourseNode *ClassData = (TCourseNode *)ClassNode->Data;

			if (ClassData->Location & LocBar || ClassData->Location & LocRestaurant || ClassData->Location & LocBottleStore)
			{
				CourseButton = (TTouchButton *)frmPreview->FindComponent("TouchButton" + IntToStr(Count+1));
				CourseButton->Visible = true;
				CourseButton->Caption = ClassData->LongDescription;
				CourseButton->Color = clMaroon;
				CourseButton->Tag = Count;

				// Add all of its Items
				int x=0, y=0, ButtonCount=0;
				TTouchButton *LastButtonAdded = NULL;

				TTreeNode *ClassNode = ClassesNode->Item[i];
				for (int j=0; j<ClassNode->Count; j++)
				{
					TTreeNode *DrinkNode = ClassNode->Item[j];
					TItemNode *DrinkData = (TItemNode *)DrinkNode->Data;
					if (DrinkData->NodeType == ITEM_NODE && !DrinkData->ItemOnlySide)
					{
						TTouchButton *NewButton = new TTouchButton(frmPreview.get());

						NewButton->Visible = false;
						NewButton->Color = (TColor)DrinkData->Colour;

						double R,G,B;          /* input RGB values */
						R = GetRValue(NewButton->Color);
						G = GetGValue(NewButton->Color);
						B = GetBValue(NewButton->Color);
						double L = (0.2126 * R) + (0.7152 * G) + (0.0722 * B);
						if (L > 128)
						{
							NewButton->Font->Color 		= clBlack;
						}
						else
						{
							NewButton->Font->Color 		= clWhite;
						}

						NewButton->Font->Name		= "Arial";
						NewButton->Font->Size		= frmPreview->TouchButton1->Font->Size;
						NewButton->Font->Style		= TFontStyles() << fsBold;
						NewButton->BorderStyle		= bsNone;
						NewButton->BevelInner		= bvNone;
						NewButton->BevelOuter		= bvRaised;
						NewButton->BevelWidth		= 2;
						NewButton->BorderWidth		= 2;
						NewButton->Enabled 			= DrinkData->Enabled;
						NewButton->Caption			= DrinkData->LongDescription;
						NewButton->Left				= x;
						NewButton->Top					= y;
						NewButton->Parent				= frmPreview->sbDishes;
						NewButton->Tag					= Count;
						if (LastButtonAdded)
						{
							NewButton->Width 			= (frmPreview->sbDishes->ClientWidth - (LastButtonAdded->Left + LastButtonAdded->Width)) /
							(4 - ButtonCount);
						}
						else
						{
							NewButton->Width			= frmPreview->sbDishes->ClientWidth / 4;
						}
						NewButton->Height				= frmPreview->sbDishes->ClientHeight / 8;
						x += NewButton->Width;
						if (ButtonCount == 3)
						{
							x = 0;
							y += NewButton->Height;
							ButtonCount = 0;
							LastButtonAdded = NULL;
						}
						else
						{
							ButtonCount++;
							LastButtonAdded = NewButton;
						}
					}
				}
				if (ClassNode == SelectedClass)
				{
					frmPreview->CurrentCourseBtn = CourseButton;
				}
				Count++;
			}
		}
		if (CourseButton) frmPreview->sbCourses->VertScrollBar->Range = CourseButton->Top + CourseButton->Height + 8;
		for (int i=Count; i<32; i++)
		{
			TTouchButton *CourseButton = (TTouchButton *)frmPreview->FindComponent("TouchButton" + IntToStr(i+1));
			CourseButton->Visible = false;
		}

		frmPreview->sbCourses->EnableAutoRange();
		frmPreview->stDrinksDishes->Caption = "Drinks";
		frmPreview->ShowModal();
	}
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::SaveNode(TTreeNode *TreeNode)
{
	TEditorNode *MenuNode = (TEditorNode *)TreeNode->Data;
	if (MenuNode)
	{
		if (MenuNode->NodeType == ITEM_SIZE_NODE)
		{
			// Save the new recipe settings.
			TList *RecipeList = ((TItemSizeNode *)MenuNode)->RecipeList;
			if (RecipeList->Count > 0)
			{
				if (RecipeList->Count >= sgRecipe->Row)
				{
					TRecipe *Recipe = (TRecipe *) RecipeList->Items[sgRecipe->Row-1];

					int OldQuantity = Recipe->Quantity;
					AnsiString OldLocation = Recipe->Location;

					Recipe->Location = edLoc->Text;
					LastLocation = Recipe->Location;
					if (Recipe->Location != "" && edLoc->Items->IndexOf(Recipe->Location) == -1)
					{
						edLoc->Items->Add(Recipe->Location);
					}
					Recipe->Quantity = edQty->Caption.ToDouble();
					if (OldQuantity != Recipe->Quantity || OldLocation != Recipe->Location)
					{
						MenuEdited = true;
					}
					sgRecipe->RowCount = 2;
					sgRecipe->Cells[0][1] = "";
					sgRecipe->Cells[1][1] = "";
					sgRecipe->Cells[2][1] = "";
				}
			}
			else
			{
				edRecipeQty->Value = 0;
				edQty->Caption = 0;
			}
			// Save new drink costs.
			TItemSizeNode *DrinkCostData = (TItemSizeNode *)MenuNode;
			Currency OldCost					= DrinkCostData->Cost;
			Currency OldCostGST				= DrinkCostData->CostGST;
			Currency OldPrice					= DrinkCostData->Price;
			Currency OldSpecialPrice		= DrinkCostData->SpecialPrice;
			Currency OldGST					= DrinkCostData->GST;

            Menu::TItemSizePriceLevel priceLevel;

	 	    DrinkCostData->Price				= sgItemsize->Cells[1][1];
			DrinkCostData->MaxRetailPrice = edItemMaxPrice->Value;
			DrinkCostData->Cost				= edItemCost->Value;
			DrinkCostData->CostGST			= edCostGST->Value;
			DrinkCostData->GST				= 0;
			DrinkCostData->PointsPercent	= nePointsPercent->Value;
			if (OldPrice != DrinkCostData->Price || OldSpecialPrice != DrinkCostData->SpecialPrice || OldCost != DrinkCostData->Cost || OldCostGST != DrinkCostData->CostGST || OldGST != DrinkCostData->GST)
			{
				MenuEdited = true;
				RelabelDrinkCosts();
			}
            DrinkCostData->CostForPoints = nePriceForPoint->Value;
		}
		else if (MenuNode->NodeType == COURSE_NODE)
		{
			TCourseNode *ClassData = (TCourseNode *)MenuNode;
			if (edCourseName->Text == "")
			{
				Application->MessageBox("Please enter a valid name for this course.", "MenuMate", MB_OK + MB_ICONERROR);
				edCourseName->SetFocus();
				return false;
			}
			else
			{
				if (ClassData->LongDescription != edCourseName->Text)
				{
					ClassData->LongDescription = edCourseName->Text;
					ClassData->Owner->Text = edCourseName->Text;
					MenuEdited = true;
				}
			}
			if (ClassData->Enabled != cbClassEnable->Checked)
			{
				if (cbClassEnable->Checked)
				{
					ClassData->Enable();
				}
				else
				{
					ClassData->Disable();
				}
				try
				{
					cbClassEnable->OnClick = NULL;
					cbClassEnable->Checked = ClassData->Enabled;
				}
				__finally
				{
					cbClassEnable->OnClick = cbClassEnableClick;
				}
			}
			ClassData->Location = LocPalm + LocRestaurant + LocBar;
		}
		else if (MenuNode->NodeType == ITEM_NODE)
		{
			TItemNode *DrinkData = (TItemNode *)MenuNode;
			if (cbItemName->Text == "")// && edItemShortName->Text == "")
			{
				Application->MessageBox("Please enter a valid name for this item.", "MenuMate", MB_OK + MB_ICONERROR);
				cbItemName->SetFocus();
				return false;
			}
			else
			{
				if (DrinkData->LongDescription != cbItemName->Text)
				{
					DrinkData->LongDescription = cbItemName->Text;
					DrinkData->Owner->Text = DrinkData->LongDescription;
					MenuEdited = true;
				}
				if (DrinkData->KitchenName != tntedKitchenName->Text)
				{
					DrinkData->KitchenName = tntedKitchenName->Text;
					MenuEdited = true;
				}
				if (DrinkData->Enabled != cbItemEnable->Checked)
				{
					if (cbItemEnable->Checked)
					{
						DrinkData->Enable();
					}
					else
					{
						DrinkData->Disable();
					}
					try
					{
						cbItemEnable->OnClick = NULL;
						cbItemEnable->Checked = DrinkData->Enabled;
					}
					__finally
					{
						cbItemEnable->OnClick = cbItemEnableClick;
					}
					MenuEdited = true;
				}
				if (DrinkData->ItemOnlySide != chbItemOnlySide->Checked)
				{
					DrinkData->ItemOnlySide = chbItemOnlySide->Checked;
					MenuEdited = true;
				}
				if (btnItemColour->Color != DrinkData->Colour)
				{
					DrinkData->Colour = btnItemColour->Color;
					MenuEdited = true;
				}
				if (DrinkData->PrinterOptions.PrintUnderlined != cbUnderline->Checked)
				{
					DrinkData->PrinterOptions.PrintUnderlined = cbUnderline->Checked;
					MenuEdited = true;
				}
				if (DrinkData->PrinterOptions.PrintBold != cbBold->Checked)
				{
					DrinkData->PrinterOptions.PrintBold = cbBold->Checked;
					MenuEdited = true;
				}
				if (DrinkData->PrinterOptions.PrintDoubleWidth != cbDoubleWidth->Checked)
				{
					DrinkData->PrinterOptions.PrintDoubleWidth = cbDoubleWidth->Checked;
					MenuEdited = true;
				}
				//colour is a TColor Red = Checked , Black = unChecked
				if (DrinkData->PrinterOptions.PrintColour == clBlack && cbColour->Checked )
				{
					DrinkData->PrinterOptions.PrintColour = clRed;
					MenuEdited = true;
				}
				else if (DrinkData->PrinterOptions.PrintColour == clRed && !cbColour->Checked )
				{
					DrinkData->PrinterOptions.PrintColour = clBlack;
					MenuEdited = true;
				}
				// Font is an integer 1 = Checked , 0 = unChecked
				if (DrinkData->PrinterOptions.PrintFont == 0 && cbLargeFont->Checked)
				{
					DrinkData->PrinterOptions.PrintFont = 1;
					MenuEdited = true;
				}
				else if (DrinkData->PrinterOptions.PrintFont == 1 && !cbLargeFont->Checked)
				{
					DrinkData->PrinterOptions.PrintFont = 0;
					MenuEdited = true;
				}
				if (DrinkData->PrinterOptions.PrintDoubleWidth != cbDoubleWidth->Checked)
				{
					DrinkData->PrinterOptions.PrintDoubleWidth = cbDoubleWidth->Checked;
					MenuEdited = true;
				}
				if (DrinkData->PrinterOptions.PrintDoubleHeight != cbDoubleHeight->Checked)
				{
					DrinkData->PrinterOptions.PrintDoubleHeight = cbDoubleHeight->Checked;
					MenuEdited = true;
				}
			}
		}
		else if (MenuNode->NodeType == OPTION_NODE)
		{
			TOptionNode *OptionData = (TOptionNode *)MenuNode;
			if (edOptionName->Text == "")// && edOptionShortName->Text == "")
			{
				Application->MessageBox("Please enter a valid name for this option.", "MenuMate", MB_OK + MB_ICONERROR);
				return false;
			}
			else
			{
				if (OptionData->LongDescription != edOptionName->Text)
				{
					OptionData->LongDescription = edOptionName->Text;
					OptionData->Owner->Text = OptionData->LongDescription;
					MenuEdited = true;
				}

				// checked in the onclick event for MenuEdited
				OptionData->PlusOption  = cbPlusOption->Checked;
				OptionData->MinusOption = cbMinusOption->Checked;

				if (OptionData->PrinterOptions.PrintUnderlined != cbUnderlineOptions->Checked)
				{
					OptionData->PrinterOptions.PrintUnderlined = cbUnderlineOptions->Checked;
					MenuEdited = true;
				}
				if (OptionData->PrinterOptions.PrintBold != cbBoldOptions->Checked)
				{
					OptionData->PrinterOptions.PrintBold = cbBoldOptions->Checked;
					MenuEdited = true;
				}
				if (OptionData->PrinterOptions.PrintDoubleWidth != cbDoubleWidthOptions->Checked)
				{
					OptionData->PrinterOptions.PrintDoubleWidth = cbDoubleWidthOptions->Checked;
					MenuEdited = true;
				}
				//colour is a TColor Red = Checked , Black = unChecked
				if (OptionData->PrinterOptions.PrintColour == clBlack && cbColourOptions->Checked )
				{
					OptionData->PrinterOptions.PrintColour = clRed;
					MenuEdited = true;
				}
				else if (OptionData->PrinterOptions.PrintColour == clRed && !cbColourOptions->Checked )
				{
					OptionData->PrinterOptions.PrintColour = clBlack;
					MenuEdited = true;
				}
				// Font is an integer 1 = Checked , 0 = unChecked
				if (OptionData->PrinterOptions.PrintFont == 0 && cbLargeFontOptions->Checked)
				{
					OptionData->PrinterOptions.PrintFont = 1;
					MenuEdited = true;
				}
				else if (OptionData->PrinterOptions.PrintFont == 1 && !cbLargeFontOptions->Checked)
				{
					OptionData->PrinterOptions.PrintFont = 0;
					MenuEdited = true;
				}
				if (OptionData->PrinterOptions.PrintDoubleWidth != cbDoubleWidthOptions->Checked)
				{
					OptionData->PrinterOptions.PrintDoubleWidth = cbDoubleWidthOptions->Checked;
					MenuEdited = true;
				}
				if (OptionData->PrinterOptions.PrintDoubleHeight != cbDoubleHeightOptions->Checked)
				{
					OptionData->PrinterOptions.PrintDoubleHeight = cbDoubleHeightOptions->Checked;
					MenuEdited = true;
				}
				if (OptionData->Enabled != cbOptionEnable->Checked)
				{
					if (cbOptionEnable->Checked)
					{
						OptionData->Enable();
					}
					else
					{
						OptionData->Disable();
					}
					MenuEdited = true;
					try
					{
						cbOptionEnable->OnClick = NULL;
						cbOptionEnable->Checked = OptionData->Enabled;
					}
					__finally
					{
						cbOptionEnable->OnClick = cbOptionEnableClick;
					}
				}
			}
		}
		else if (MenuNode->NodeType == OPTIONS_NODE)
		{
			TOptionsNode *OptionsData = (TOptionsNode *)MenuNode;
			if (OptionsData->Enabled != cbOptionsEnable->Checked)
			{
				if (cbOptionsEnable->Checked)
				{
					OptionsData->Enable();
				}
				else
				{
					OptionsData->Disable();
				}
				MenuEdited = true;
				try
				{
					cbOptionsEnable->OnClick = NULL;
					cbOptionsEnable->Checked = OptionsData->Enabled;
				}
				__finally
				{
					cbOptionsEnable->OnClick = cbOptionsEnableClick;
				}
			}
		}
		else if (MenuNode->NodeType == MENU_NODE)
		{
			if (!rbDrinkMenu->Checked && !rbFoodMenu->Checked)
			{
				Application->MessageBox("Please select if this is a food or beverage menu.", "Error",
				MB_OK + MB_ICONERROR);
				PageControl4->ActivePage = tsProperties;
				PageControl1->ActivePage = tsMenu;
				GroupBox5->SetFocus();
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::edItemCostChange(TObject *Sender)
{
	MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::edItemCostExit(TObject *Sender)
{
	float check_gst_value =
	StrToFloatDef(edItemCost->Value, -9999.0);
	TEditorNode *CurrentNodeData;
	TItemSizeNode *DrinkCostData;

	if (check_gst_value < 0)
	edItemCost->Value = 0.0;


	CurrentNodeData =
	reinterpret_cast<TEditorNode *>(tvMenu->Selected->Data);
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		DrinkCostData =
		reinterpret_cast<TItemSizeNode *>(CurrentNodeData);
		if (DrinkCostData->Cost != edItemCost->Value) {
			MenuEdited = false;
			DrinkCostData->Cost = edItemCost->Value;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnNewMenuClick(TObject *Sender)
{
	NewMenu();
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::NewMenu()
{
	if (MenuEdited)
	{
		if (Application->MessageBox("You have not saved. Are you sure you want to lose your changes?",
					"Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) != IDOK)
		{
			return false;
		}
	}
	AnsiString NewMenuName = "";
	if (InputQuery("Menu Name", "Enter a name for the menu (Max " + IntToStr(MAX_MENU_NAME_LENGTH) + " chars).", NewMenuName))
	{
		if (NewMenuName != "")
		{

			if (NewMenuName.Length() > MAX_MENU_NAME_LENGTH)
			{
				if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
							"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
				{
					return false;
				}
				NewMenuName = NewMenuName.SubString(1, MAX_MENU_NAME_LENGTH);
			}
			rbFoodMenu->Checked = false;
			rbDrinkMenu->Checked = false;
			chbAvailableOnPalm->Checked = true;

			resetMenuTaxProfileProvider( dmMMData->dbMenuMate );

			ClearTree();
			LoadLocations();
			//			tvMenu->Items->Clear();

			TTreeNode *MenuNode;
			MenuNode = tvMenu->Items->Add(NULL, NewMenuName);
			MenuNode->Data = new TMenuNode;
			((TMenuNode *)MenuNode->Data)->Owner = MenuNode;
			((TMenuNode *)MenuNode->Data)->LongDescription = NewMenuName;

			TTreeNode *SizesNode = ((TEditorNode *)MenuNode->Data)->AddNode(SIZES_NODE);
			SizesNode->Text = "Available Sizes";

			TTreeNode *CategoriesNode = ((TEditorNode *)MenuNode->Data)->AddNode(CATEGORIES_NODE);
			CategoriesNode->Text = "Available Categories";
			// add the global Categories/Groups
			if (dmMMData->dbMenuMate->Connected)
			{
				if (!MMTransaction->InTransaction) MMTransaction->StartTransaction();
				TCategoryList CategoryList;
				LoadCategories(CategoryList);
				MMTransaction->Commit();
			}
			// just add all global Serving Courses - prior to 3.4 didnt have serving courses
			LoadServingCoursesPrior3Point4(((TMenuNode *)MenuNode->Data));
			InsertSize(DEFAULT_SIZE_STRING);
			SizesNode->Expanded = false;
			MenuEdited = true;
			rbFoodMenu->Checked = false;
			rbDrinkMenu->Checked = false;
			chbAvailableOnPalm->Checked = true;

			tvMenu->Selected = MenuNode;
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnEditMenuClick(TObject *Sender)
{
	if (MenuEdited)
	{
		if (Application->MessageBox("You have not saved. Are you sure you want to lose your changes?",
					"Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) != IDOK)
		{
			return;
		}
	}
	AnsiString NewMenuName = "";
	if (InputQuery("Menu Name", "Enter a name for the menu (Max " + IntToStr(MAX_MENU_NAME_LENGTH) + " chars).", NewMenuName))
	{
		if (NewMenuName != "")
		{
			if (NewMenuName.Length() > MAX_MENU_NAME_LENGTH)
			{
				if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
							"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
				{
					return;
				}
				NewMenuName = NewMenuName.SubString(1, MAX_MENU_NAME_LENGTH);
			}
			TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
			TMenuNode *MenuData = (TMenuNode *)MenuNode->Data;
			MenuNode->Text = NewMenuName;
			MenuData->LongDescription = NewMenuName;
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbSetMenuMasterClick(TObject *Sender)
{
	TMenuNode *NodeData = (TMenuNode *)tvMenu->Selected->Data;
	if (NodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *ItemSize = (TItemSizeNode *)tvMenu->Selected->Data;
		if (chbSetMenuMaster->Checked)
		{
			if (unsigned long Temp = GetNextSetMenuMask())
			{
				ItemSize->SetMenuMask = SET_PROMO_MASTER(Temp);
			}
			else
			{
				Application->MessageBox("You can only have 24 specials.", "Error", MB_OK + MB_ICONERROR);
			}
		}
		else
		{
			for (int i=0; i<tvMenu->Items->Count; i++)
			{
				if (((TMenuNode *)tvMenu->Items->Item[i]->Data)->NodeType == ITEM_SIZE_NODE)
				{
					TItemSizeNode *TempItemSize = (TItemSizeNode *)tvMenu->Items->Item[i]->Data;
					if (TempItemSize->SetMenuMask && (!TST_PROMO_MASTER(TempItemSize->SetMenuMask)))
					{
						if (TempItemSize->SetMenuMask & CLR_PROMO_MASTER(ItemSize->SetMenuMask))
						{
							TempItemSize->SetMenuMask = 0;
						}
					}
				}
			}
			ItemSize->SetMenuMask = 0;
		}
		RefreshItemSize(ItemSize);
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
unsigned long TfrmMenuEdit::GetNextSetMenuMask()
{
	// Finds which bits are taken already.
	bool UsedMasks[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	for (int i=0; i<tvMenu->Items->Count; i++)
	{
		if (((TMenuNode *)tvMenu->Items->Item[i]->Data)->NodeType == ITEM_SIZE_NODE)
		{
			TItemSizeNode *ItemSize = (TItemSizeNode *)tvMenu->Items->Item[i]->Data;
			if (TST_PROMO_MASTER(ItemSize->SetMenuMask))
			{
				unsigned long SetMenuMask = CLR_PROMO_MASTER(ItemSize->SetMenuMask);
				int BitPos = 0;
				while (SetMenuMask >>= 1) ++BitPos;
				UsedMasks[BitPos] = true;
			}
		}
	}
	// returns first available if any, otherwise returns 0.
	unsigned long i;
	for (i=0; UsedMasks[i]; i++);
	if (i>=24) return 0;
	else return (1<<i);
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::FindSetMenuMasters(int SelectedSetMenuMask)
{
	ClearSetMenuMasters();
	for (int i=0; i<tvMenu->Items->Count; i++)
	{
		if (((TMenuNode *)tvMenu->Items->Item[i]->Data)->NodeType == ITEM_SIZE_NODE)
		{
			TItemSizeNode *ItemSize = (TItemSizeNode *)tvMenu->Items->Item[i]->Data;
			if (TST_PROMO_MASTER(ItemSize->SetMenuMask))
			{
				TItemNode *Item = (TItemNode *)ItemSize->Owner->Parent->Data;
				int Index = lbSetMenuMasters->Items->AddObject(Item->LongDescription + " (" + ItemSize->LongDescription + ")",
				((TObject *)CLR_PROMO_MASTER(ItemSize->SetMenuMask)));
				if (CLR_PROMO_MASTER(ItemSize->SetMenuMask) & SelectedSetMenuMask)
				{
					lbSetMenuMasters->Checked[Index] = true;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ClearSetMenuMasters()
{
	lbSetMenuMasters->Clear();
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::FindSetMenuItems(int SelectedSetMenuMask)
{
	ClearSetMenuItems();
	for (int i=0; i<tvMenu->Items->Count; i++)
	{
		if (((TMenuNode *)tvMenu->Items->Item[i]->Data)->NodeType == ITEM_SIZE_NODE)
		{
			TItemSizeNode *ItemSize = (TItemSizeNode *)tvMenu->Items->Item[i]->Data;
			if (!TST_PROMO_MASTER(ItemSize->SetMenuMask))
			{
				TItemNode *Item = (TItemNode *)ItemSize->Owner->Parent->Data;
				if (ItemSize->SetMenuMask & SelectedSetMenuMask)
				{
					lbSetMenuItems->Items->Add(Item->LongDescription + " (" + ItemSize->LongDescription + ")");
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ClearSetMenuItems()
{
	lbSetMenuItems->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbSetMenuItemClick(TObject *Sender)
{
	TMenuNode *NodeData = (TMenuNode *)tvMenu->Selected->Data;
	if (NodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *ItemSize = (TItemSizeNode *)tvMenu->Selected->Data;
		if (chbSetMenuItem->Checked)
		{
			ItemSize->SetMenuMask = 0;
			ItemSize->SetMenuStandardItem = true;
			ItemSize->SettingMenuMask = true;
			ClearSetMenuItems();
		}
		else
		{
			ItemSize->SetMenuMask = 0;
			ItemSize->SettingMenuMask = false;
		}
		MenuEdited = true;
		RefreshItemSize(ItemSize);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbSetMenuStandardItemClick(TObject *Sender)
{
	TMenuNode *NodeData = (TMenuNode *)tvMenu->Selected->Data;
	if (NodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *ItemSize = (TItemSizeNode *)tvMenu->Selected->Data;
		if (!chbSetMenuStandardItem->Checked)
		{
	 			ItemSize->SetMenuStandardItem = false;
			//			ItemSize->Price = 0;
			//			ItemSize->SpecialPrice = 0;
			//			ItemSize->Free = false;
		}
		else
		{
			ItemSize->SetMenuStandardItem = true;
			//			ItemSize->Free = false;
		}
		MenuEdited = true;
		RefreshItemSize(ItemSize);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbSetMenuMastersClick(TObject *Sender)
{
	if (lbSetMenuMasters->Items->Count > 0)
	{
		TMenuNode *NodeData = (TMenuNode *)tvMenu->Selected->Data;
		if (NodeData->NodeType == ITEM_SIZE_NODE)
		{
			TItemSizeNode *ItemSize = (TItemSizeNode *)tvMenu->Selected->Data;
			ItemSize->SetMenuMask = 0;
			for (int i=0; i<lbSetMenuMasters->Items->Count; i++)
			{
				if (lbSetMenuMasters->Checked[i])
				{
					ItemSize->SetMenuMask |= int(lbSetMenuMasters->Items->Objects[i]);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnScanBarcodeClick(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *DrinkCostData = (TItemSizeNode *)CurrentNodeData;
		frmGetBarcode->Barcode = DrinkCostData->Barcode;
		if (frmGetBarcode->ShowModal() == mrOk)
		{
			MenuEdited = true;
			DrinkCostData->Barcode = frmGetBarcode->Barcode;
		}
	}
} 
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnErrorWarningSettingsClick(TObject *Sender)
{
	frmErrorWarningSettings = new TfrmErrorWarningSettings(this);
	try
	{
		frmErrorWarningSettings->ShowModal();
	}
	__finally
	{
		delete frmErrorWarningSettings;
		frmErrorWarningSettings = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgOptionsSelectCell(TObject *Sender,int ACol, int ARow, bool &CanSelect)
{
	TRect R;
	if (ARow >= sgOptions->FixedRows)
	{
		if (ACol == 1)
		{
			R			= sgOptions->CellRect(ACol, ARow);
			R.Left	= R.Left + sgOptions->Left;
			R.Right	= R.Right + sgOptions->Left;
			R.Top		= R.Top + sgOptions->Top;
			R.Bottom	= R.Bottom + sgOptions->Top;

			ComboBox1->Left		= R.Left + 1;
			ComboBox1->Top			= R.Top + 1;
			ComboBox1->Width		= (R.Right + 1) - R.Left;
			ComboBox1->Height		= (R.Bottom + 1) - R.Top;
			ComboBox1->Visible	= true;
			ComboBox1->SetFocus();

			TOptionNode *OptionData = (TOptionNode *)sgOptions->Objects[0][ARow];
			ComboBox1->ItemIndex = OptionData->GroupNumber;
			if (OptionData->GroupNumber == 0)
			{
				ComboBox1->Text = "Select a group";
			}
			else
			{
				ComboBox1->Text = IntToStr(OptionData->GroupNumber);
			}
			sgOptions->Options == sgOptions->Options << goEditing;// << goAlwaysShowEditor;
			ComboBox1->SelectAll();
		}
		else if (ACol == 2 || ACol == 3)
		{
			sgOptions->Options == sgOptions->Options >> goEditing;// >> goAlwaysShowEditor;
		}
		else
		{
			sgOptions->Options == sgOptions->Options << goEditing;// << goAlwaysShowEditor;
			ComboBox1->SelectAll();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::ComboBox1Exit(TObject *Sender)
{
	TOptionNode *OptionData = (TOptionNode *)sgOptions->Objects[0][sgOptions->Row];
	if (OptionData)
	{
		if (OptionData->GroupNumber == 0)
		{
			sgOptions->Cells[sgOptions->Col][sgOptions->Row] = "None";
		}
		else
		{
			sgOptions->Cells[sgOptions->Col][sgOptions->Row] = IntToStr(OptionData->GroupNumber);
		}
		ComboBox1->Visible = false;

	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::ComboBox1Change(TObject *Sender)
{
	TOptionNode *OptionData = (TOptionNode *)sgOptions->Objects[0][sgOptions->Row];

	int previousOptionGroup = OptionData->GroupNumber;
	OptionData->GroupNumber =
	ComboBox1->Items->Strings[ComboBox1->ItemIndex] == "None"
	? 0
	: StrToInt( ComboBox1->Items->Strings[ComboBox1->ItemIndex] );

	SetForcedOptionToGroup(OptionData);
	removeForcedOptionFromCourseItems(
	tvMenu->Selected->Parent, 
	OptionData,
	previousOptionGroup );

	MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::ComboBox1Enter(TObject *Sender)
{
	ComboBox1->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::ComboBox1KeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{
	if (!ComboBox1->DroppedDown)
	{
		if (Key == VK_DOWN)
		{
			ComboBox1->DroppedDown = true;
		}
		if (Key == VK_UP)
		{
			ComboBox1->DroppedDown = true;
			if (ComboBox1->ItemIndex == -1)
			{
				ComboBox1->ItemIndex = 0;
			}
		}
	}
	if (Key == VK_RETURN)
	{
		PostMessage(sgOptions->Handle, WM_CHAR, Key, 0);
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::ComboBox1KeyPress(TObject *Sender, char &Key)
{
	if (!((Key >= '0' && Key <= '8') || (Key >= VK_NUMPAD0 && Key <= VK_NUMPAD8) || Key == VK_BACK))
	//	if ((Key < '0' || Key > '8') && Key != VK_BACK)
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgOptionsMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TPoint pt;
	int ACol, ARow;

	sgOptions->MouseToCell(X, Y, ACol, ARow);

	if ((ACol == 2 || ACol == 3) && ARow >= sgOptions->FixedRows)
	{
		ToggleCheckbox(ACol, ARow);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ToggleCheckbox(int ACol, int ARow)
{
	TOptionNode *OptionData = (TOptionNode *)sgOptions->Objects[0][ARow];
	if (ACol == 2)
	{
		if (OptionData->PlusOption == false)
		{
			OptionData->PlusOption = true;
		}
		else if (OptionData->MinusOption)
		{
			// Can't turn off if MinusOption isn't on. Must be one or the other or both.
			OptionData->PlusOption = false;
		}
		else
		{
			OptionData->PlusOption = false;
			OptionData->MinusOption = true;
			sgOptions->Objects[3][ARow] = sgOptions->Objects[3][ARow]; // Invalidates the cell
		}
		sgOptions->Objects[ACol][ARow] = sgOptions->Objects[ACol][ARow]; // Invalidates the cell
	}
	else if (ACol == 3)
	{
		if (OptionData->MinusOption == false)
		{
			OptionData->MinusOption = true;
		}
		else if (OptionData->PlusOption)
		{
			// Can't turn off if PlusOption isn't on. Must be one or the other or both.
			OptionData->MinusOption = false;
		}
		else
		{
			OptionData->MinusOption = false;
			OptionData->PlusOption = true;
			sgOptions->Objects[2][ARow] = sgOptions->Objects[2][ARow]; // Invalidates the cell
		}
		sgOptions->Objects[ACol][ARow] = sgOptions->Objects[ACol][ARow]; // Invalidates the cell
	}
	sgOptions->Canvas->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgOptionsDrawCell(TObject *Sender, int ACol,int ARow, TRect &Rect, TGridDrawState State)
{
	if (ACol == 2 || ACol == 3)
	{
		if (State.Contains(gdFixed))
		{
			if (ACol == 2)
			{
				sgOptions->Canvas->TextRect(Rect, (Rect.Right + Rect.Left - sgOptions->Canvas->TextWidth("+")) / 2, Rect.Top + 2,	"+");
			}
			else if (ACol == 3)
			{
				sgOptions->Canvas->TextRect(Rect, (Rect.Right + Rect.Left - sgOptions->Canvas->TextWidth("-")) / 2, Rect.Top + 2,	"-");
			}
		}
		else
		{
			sgOptions->Canvas->Brush->Color = TColor(0xE0E0E0);
			// checkboxes look better on a non-white background
			sgOptions->Canvas->FillRect(Rect);
			TOptionNode *OptionData = (TOptionNode *)sgOptions->Objects[0][ARow];

			bool Checked;

			if (OptionData != NULL)
			{
				if (ACol == 2)
				{
					Checked = (OptionData->PlusOption);
				}
				else if (ACol == 3)
				{
					Checked = (OptionData->MinusOption);
				}
				if (Checked)
				{
					sgOptions->Canvas->Draw((Rect.right + Rect.left - FCheck->Width) / 2,
					(Rect.bottom + Rect.top - FCheck->Height) / 2,
					FCheck );
				}
				else
				{
					sgOptions->Canvas->Draw((Rect.right + Rect.left - FNoCheck->Width) / 2,
					(Rect.bottom + Rect.top - FNoCheck->Height) / 2,
					FNoCheck );
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnAddCategoryClick(TObject *Sender)
{
	if (tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX]->Count == 0)
	{
		if (Application->MessageBox("There are no categories to add. Do you wish to create a category?",
					"Error", MB_YESNO + MB_ICONERROR) == ID_YES)
		{
			tvMenu->Selected = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
			PageControl1->ActivePage = tsCategoryGroups;
		}
	}
	else
	{
		std::vector<Menu::TNameAndKey> AllCategoriesWithKeys;
		std::auto_ptr<TfrmSelectCategory> frmSelectCategory(new TfrmSelectCategory(NULL));
		GetAllCategoriesWithKeys(&AllCategoriesWithKeys);
		frmSelectCategory->lbCategory->Items->Clear();
		for(std::vector<Menu::TNameAndKey>::iterator nameKeyIT = AllCategoriesWithKeys.begin(); nameKeyIT != AllCategoriesWithKeys.end(); nameKeyIT++ )
		{
			Menu::TNameAndKey *nameKey = nameKeyIT;
			frmSelectCategory->lbCategory->Items->Add(nameKey->Name);
		}
		if (frmSelectCategory->ShowModal() == mrOk && frmSelectCategory->lbCategory->ItemIndex != -1)
		{
			std::vector<Menu::TNameAndKey> *BCategoryList = &((TItemSizeNode *)tvMenu->Selected->Data)->BCategoryList;
			AnsiString CategoryString = frmSelectCategory->lbCategory->Items->Strings[frmSelectCategory->lbCategory->ItemIndex];
			if (lbCategories->Items->IndexOf(CategoryString) > -1)
			{
				lbCategories->ItemIndex = lbCategories->Items->IndexOf(CategoryString);
				return;
			}

			Menu::TNameAndKey *selectedCategory;
			bool hasValidBCategory = false;

			// find the selected category by looping though all categories
			for(std::vector<Menu::TNameAndKey>::iterator nameKeyIT = AllCategoriesWithKeys.begin(); nameKeyIT != AllCategoriesWithKeys.end(); nameKeyIT++ )
			{
				Menu::TNameAndKey *nameKey = nameKeyIT;
				if(nameKey->Name == CategoryString)
				{
					selectedCategory = nameKey;
					hasValidBCategory = true;
					break;
				}
			}

			if(hasValidBCategory)
			{
				BCategoryList->push_back(*selectedCategory);

				lbCategories->Items->Clear();
				std::vector<Menu::TNameAndKey>::iterator nameKeyIT;
				for( nameKeyIT = BCategoryList->begin(); nameKeyIT != BCategoryList->end(); nameKeyIT++ )
				{
					Menu::TNameAndKey *nameKey = nameKeyIT;
					lbCategories->Items->Add(nameKey->Name);
				}
				lbCategories->ItemIndex = lbCategories->Items->IndexOf(CategoryString);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall
TfrmMenuEdit::btnRemoveCategoryClick(
TObject *sender)
{
	int item_index = lbCategories->ItemIndex;

	if (item_index == -1)
	return;

	std::vector<Menu::TNameAndKey> &categories =
	reinterpret_cast<TItemSizeNode *>(tvMenu->Selected->Data)->BCategoryList;
	TStrings &visible_categories = *lbCategories->Items;
	AnsiString selected_category =
	visible_categories.Strings[lbCategories->ItemIndex];
	std::vector<Menu::TNameAndKey>::iterator i = categories.begin();

	for ( ; i->Name != selected_category && i != categories.end(); ++i)
	;

	categories.erase(i);
	visible_categories.Delete(item_index);

	lbCategories->SetFocus();
	lbCategories->ItemIndex =
	item_index - (item_index >= lbCategories->Items->Count);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnNewCategoryClick(TObject *Sender)
{
    TfrmInputCategory   *frmInputCategory=new TfrmInputCategory(this) ;
	frmInputCategory->CategoryName = "";
	frmInputCategory->GlCode = "";
	frmInputCategory->Caption = "New Category";
	AnsiString NewCategoryName = "";
	AnsiString NewGlCode = "";
	if(frmInputCategory->ShowModal() == mrOk)
	 {
		NewCategoryName = frmInputCategory->CategoryName;
		NewGlCode = frmInputCategory->GlCode;
		if (NewCategoryName != "")
		{
			if (NewCategoryName.Length() > MAX_CATEGORY_LENGTH)
			{
				if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
							"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
				{
					return;
				}
				NewCategoryName = NewCategoryName.SubString(1, MAX_CATEGORY_LENGTH);
			}

			int Index = InsertCategory((TCategoryGroupNode *)tvMenu->Selected->Data, NewCategoryName,NewGlCode);
			RefreshMenuDetails();
			MenuEdited = true;
			lbAvailableCategories->ItemIndex = Index;
		}
	 }
     delete frmInputCategory;
}
//---------------------------------------------------------------------------
int TfrmMenuEdit::InsertCategory(TCategoryGroupNode *CategoryGroup, AnsiString CategoryName, AnsiString GlCode)
{
	TTreeNode *CategoryGroupsNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
	int GroupIndex;
	int CategoryIndex;
	if (FindCategory(CategoryName, &GroupIndex, &CategoryIndex, NULL))
	{
		if (CategoryGroup == CategoryGroupsNode->Item[GroupIndex]->Data)
		{
			return CategoryIndex;
		}
		else
		{
			Application->MessageBox(AnsiString("This category already exists in " + CategoryGroupsNode->Item[GroupIndex]->Text + ".").c_str(),
			"Error", MB_OK + MB_ICONERROR);
			return -1;
		}
	}
	if (FindCategoryGroup(CategoryName, &GroupIndex))
	{
		Application->MessageBox("This category has already been used as a group name.", "Error", MB_OK + MB_ICONERROR);
		return -1;
	}
	TTreeNode *NewCategoryNode = CategoryGroup->AddNode(CATEGORY_NODE, false);
	NewCategoryNode->Text = CategoryName;
	((TCategoryNode *)NewCategoryNode->Data)->LongDescription = CategoryName;
	((TCategoryNode *)NewCategoryNode->Data)->GLCode = GlCode;
	reinterpret_cast<TEditorNode *>(NewCategoryNode->Data)->Key =
	(*new_menu_element_key_generator_)();

	tvMenu->Items->BeginUpdate();
	CategoryGroup->Owner->CustomSort(CompareFunc, 1);
	CategoryGroup->Owner->Expand(false);
	tvMenu->Items->EndUpdate();

	for (int i=0; i<CategoryGroup->Owner->Count; i++)
	{
		TCategoryNode *CategoryData = (TCategoryNode *)CategoryGroup->Owner->Item[i]->Data;
		if (CategoryData->LongDescription.UpperCase() == CategoryName.UpperCase())
		{
			return i;
		}
	}
	return -1;
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::FindCategory(AnsiString CategoryName, int *GroupIndex, int *CategoryIndex, bool *Deleted)
{
	if (GroupIndex)
	{
		*GroupIndex = -1;
	}
	if (CategoryIndex)
	{
		*CategoryIndex = -1;
	}
	TTreeNode *CategoryGroupsNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
	for (int i=0; i<CategoryGroupsNode->Count; i++)
	{
		TTreeNode *CategoriesNode = CategoryGroupsNode->Item[i];
		for (int j=0; j<CategoriesNode->Count; j++)
		{
			TCategoryNode *CategoryData = (TCategoryNode *)CategoriesNode->Item[j]->Data;
			if (CategoryData->LongDescription.UpperCase() == CategoryName.UpperCase())
			{
				if (GroupIndex)
				{
					*GroupIndex = i;
				}
				if (CategoryIndex)
				{
					*CategoryIndex = j;
				}
				if (Deleted)
				{
					*Deleted = CategoryData->GetDeleted();
				}
				return true;
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------------
TTreeNode* TfrmMenuEdit::FindCategoryGroup(AnsiString GroupName)
{
	TTreeNode *CategoryGroupsNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
	for (int i=0; i<CategoryGroupsNode->Count; i++)
	{
		TCategoryGroupNode *CategoryGroupData = (TCategoryGroupNode *)CategoryGroupsNode->Item[i]->Data;
		if (CategoryGroupData->LongDescription.UpperCase() == GroupName.UpperCase())
		{
			return CategoryGroupsNode->Item[i];;
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::FindCategoryGroup(AnsiString GroupName, int *GroupIndex)
{
	if (GroupIndex)
	{
		*GroupIndex = -1;
	}
	TTreeNode *CategoryGroupsNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
	for (int i=0; i<CategoryGroupsNode->Count; i++)
	{
		TCategoryGroupNode *CategoryGroupData = (TCategoryGroupNode *)CategoryGroupsNode->Item[i]->Data;
		if (CategoryGroupData->LongDescription.UpperCase() == GroupName.UpperCase())
		{
			if (GroupIndex)
			{
				*GroupIndex = i;
			}
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
int TfrmMenuEdit::InsertSize(AnsiString SizeName)
{
	TTreeNode *SizesNode = tvMenu->Items->GetFirstNode()->Item[SIZES_INDEX];
	for (int i=0; i<SizesNode->Count; i++)
	{
		TSizeNode *SizeData = (TSizeNode *)SizesNode->Item[i]->Data;
		if (SizeData->LongDescription.UpperCase() == SizeName.UpperCase())
		{
			return i;
		}
	}
	TTreeNode *NewSizeNode = ((TEditorNode *)SizesNode->Data)->AddNode(SIZE_NODE, false);
	NewSizeNode->Text = SizeName;
	((TSizeNode *)NewSizeNode->Data)->LongDescription = SizeName;

	reinterpret_cast<TEditorNode *>(NewSizeNode->Data)->Key =
	(*new_menu_element_key_generator_)();
    TAvailableMenuSize availablesize;
    availablesize.Key = reinterpret_cast<TEditorNode *>(NewSizeNode->Data)->Key;
    AllSizesForMenu[SizeName] = availablesize;

	tvMenu->Items->BeginUpdate();
	SizesNode->CustomSort(CompareFunc, 1);
	SizesNode->Expand(false);
	tvMenu->Items->EndUpdate();

    

	for (int i=0; i<SizesNode->Count; i++)
	{
		TSizeNode *SizeData = (TSizeNode *)SizesNode->Item[i]->Data;
		if (SizeData->LongDescription.UpperCase() == SizeName.UpperCase())
		{
			return i;
		}
	}
	return -1;
}
//---------------------------------------------------------------------------
int TfrmMenuEdit::InsertServingCourse(AnsiString ServingCourseName) //cww
{
	TTreeNode *ServingCoursesNode = tvMenu->Items->GetFirstNode()->Item[SERVING_COURSES_INDEX];
	for (int i=0; i<ServingCoursesNode->Count; i++)
	{
		TServingCourseNode *ServingData = (TServingCourseNode *)ServingCoursesNode->Item[i]->Data;
		if (ServingData->LongDescription.UpperCase() == ServingCourseName.UpperCase())
		{
			return i;
		}
	}
	TTreeNode *NewServingCourseNode = ((TEditorNode *)ServingCoursesNode->Data)->AddNode(SERVING_COURSE_NODE, false);
	NewServingCourseNode->Text = ServingCourseName;
	((TServingCourseNode *)NewServingCourseNode->Data)->LongDescription = ServingCourseName;
	((TServingCourseNode *)NewServingCourseNode->Data)->Enable();
	((TServingCourseNode *)NewServingCourseNode->Data)->NewServingCourse = true;
	((TServingCourseNode *)NewServingCourseNode->Data)->Selectable       = true;

	reinterpret_cast<TEditorNode *>(NewServingCourseNode->Data)->Key =
	(*new_menu_element_key_generator_)();

	tvMenu->Items->BeginUpdate();
	//ServingCoursesNode->CustomSort(CompareFunc, 1);
	ServingCoursesNode->Expand(false);
	tvMenu->Items->EndUpdate();

	for (int i=0; i<ServingCoursesNode->Count; i++)
	{
		TServingCourseNode *ServingData = (TServingCourseNode *)ServingCoursesNode->Item[i]->Data;
		if (ServingData->LongDescription.UpperCase() == ServingCourseName.UpperCase())
		{
			return i;
		}
	}
	return -1;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::DeleteCategory(int GroupIndex, int CategoryIndex)
{
	TTreeNode *CategoriesNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX]->Item[GroupIndex];
	TTreeNode *CategoryNode = CategoriesNode->Item[CategoryIndex];

	TCategoryNode *catData = (TCategoryNode *)CategoryNode->Data;
	if (catData)
	{
		catData->SetDeleted(true);
	}
	//	CategoriesNode->Item[CategoryIndex]->Delete();
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::DeleteCategoryGroup(int GroupIndex)
{
	TTreeNode *CategoriesNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX]->Item[GroupIndex];
	//	CategoriesNode->Delete();
	TCategoryGroupNode *grpData = (TCategoryGroupNode *)CategoriesNode->Data;
	if (grpData)
	{
		grpData->SetDeleted(true);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::DeleteSize(int Index)
{
	TTreeNode *SizesNode = tvMenu->Items->GetFirstNode()->Item[SIZES_INDEX];
	SizesNode->Item[Index]->Delete();
    
    
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::DeleteServingCourse(int Index)
{
	TTreeNode *ServingCoursesNode = tvMenu->Items->GetFirstNode()->Item[SERVING_COURSES_INDEX];
	AddToDeletedServingCourses((TServingCourseNode *)ServingCoursesNode->Item[Index]->Data);
	ServingCoursesNode->Item[Index]->Delete();
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::AddToDeletedServingCourses(TServingCourseNode *ServingCourseNode)
{
	if (ServingCourseNode->NewServingCourse == true)
	{ // the Serving Course has just been added in this menu and doesnt need to get to MenuMate
		return;
	}
	// dont add it if the name already exists
	for (unsigned i=0; i<DeletedServingCoursesInfo.DeletedServingCourseVector.size(); i++)
	{
		//ShowMessage(DeletedServingCoursesInfo.DeletedServingCoursesVector[i].ServingCourse_Name);
		if (UpperCase(DeletedServingCoursesInfo.DeletedServingCourseVector[i].ServingCourse_Name) == UpperCase(ServingCourseNode->LongDescription) )
		return;
	}
	Menu::TServingCourseInfo   DeletedServingCourseInfo;
	DeletedServingCourseInfo.ServingCourse_Name           = ServingCourseNode->LongDescription;
	DeletedServingCourseInfo.ServingCourse_Kitchen_Name   = ServingCourseNode->KitchenName;
	DeletedServingCourseInfo.Enabled                      = false;
	DeletedServingCourseInfo.Deleted                      = true;
	DeletedServingCourseInfo.Selectable                   = false;
	DeletedServingCourseInfo.DisplayOrder                 = ServingCourseNode->DisplayOrder;

	DeletedServingCoursesInfo.DeletedServingCourseVector.push_back(DeletedServingCourseInfo);
}

//---------------------------------------------------------------------------
void TfrmMenuEdit::DeleteFromServingCourses(const AnsiString ServingCourseName )
{
	std::vector<Menu::TServingCourseInfo>::iterator DeletedServingCourseIterator;
	DeletedServingCourseIterator = DeletedServingCoursesInfo.DeletedServingCourseVector.begin();
	while ( DeletedServingCourseIterator != DeletedServingCoursesInfo.DeletedServingCourseVector.end() )
	{
		if (UpperCase(DeletedServingCourseIterator->ServingCourse_Name) == UpperCase(ServingCourseName) )
		DeletedServingCoursesInfo.DeletedServingCourseVector.erase(DeletedServingCourseIterator);
		else
		++DeletedServingCourseIterator;
	}
}

//---------------------------------------------------------------------------
int TfrmMenuEdit::EditCategory(AnsiString CategoryName, int GroupIndex, int CategoryIndex)
{
	TTreeNode *CategoriesNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX]->Item[GroupIndex];
	tvMenu->Items->BeginUpdate();
	CategoriesNode->Item[CategoryIndex]->Text = CategoryName;
	((TMenuNode *)CategoriesNode->Item[CategoryIndex]->Data)->LongDescription = CategoryName;
	CategoriesNode->CustomSort(CompareFunc, 1);
	CategoriesNode->Expand(false);
	tvMenu->Items->EndUpdate();

	for (int i=0; i<CategoriesNode->Count; i++)
	{
		TCategoryNode *CategoryData = (TCategoryNode *)CategoriesNode->Item[i]->Data;
		if (CategoryData->LongDescription == CategoryName)
		{
			return i;
		}
	}
	return -1;
}
//---------------------------------------------------------------------------
int TfrmMenuEdit::EditCategoryGroup(AnsiString GroupName, int GroupIndex)
{
	TTreeNode *GroupsNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];

	tvMenu->Items->BeginUpdate();
	GroupsNode->Item[GroupIndex]->Text = GroupName;
	((TMenuNode *)GroupsNode->Item[GroupIndex]->Data)->LongDescription = GroupName;
	GroupsNode->CustomSort(CompareFunc, 1);
	GroupsNode->Expand(false);
	tvMenu->Items->EndUpdate();

	for (int i=0; i<GroupsNode->Count; i++)
	{
		TCategoryGroupNode *GroupData = (TCategoryGroupNode *)GroupsNode->Item[i]->Data;
		if (GroupData->LongDescription == GroupName)
		{
			return i;
		}
	}
	return -1;
}
//---------------------------------------------------------------------------
int TfrmMenuEdit::EditServingCourse(AnsiString ServingCourseName, int Index) //cww
{
	TTreeNode *ServingCoursesNode = tvMenu->Items->GetFirstNode()->Item[SERVING_COURSES_INDEX];
	for (int i=0; i<ServingCoursesNode->Count; i++)
	{
		TServingCourseNode *ServingCourseData  = (TServingCourseNode *)ServingCoursesNode->Item[i]->Data;
		if (ServingCourseData->LongDescription.UpperCase() == ServingCourseName.UpperCase() && i != Index)
		{
			return i;
		}
	}
	tvMenu->Items->BeginUpdate();
	ServingCoursesNode->Item[Index]->Text = ServingCourseName;
	((TMenuNode *)ServingCoursesNode->Item[Index]->Data)->LongDescription = ServingCourseName;
	//ServingCoursesNode->CustomSort(CompareFunc, 1);
	ServingCoursesNode->Expand(false);
	tvMenu->Items->EndUpdate();

	for (int i=0; i<ServingCoursesNode->Count; i++)
	{
		TServingCourseNode *ServingCourseData = (TServingCourseNode *)ServingCoursesNode->Item[i]->Data;
		if (ServingCourseData->LongDescription.UpperCase() == ServingCourseName.UpperCase())
		{
			return i;
		}
	}
	return -1;
}

//---------------------------------------------------------------------------
int TfrmMenuEdit::EditSize(AnsiString SizeName, int Index)
{
	TTreeNode *SizesNode = tvMenu->Items->GetFirstNode()->Item[SIZES_INDEX];
	for (int i=0; i<SizesNode->Count; i++)
	{
		TSizeNode *SizeData = (TSizeNode *)SizesNode->Item[i]->Data;
		if (SizeData->LongDescription.UpperCase() == SizeName.UpperCase() && i != Index)
		{
			return i;
		}
	}
	tvMenu->Items->BeginUpdate();
	SizesNode->Item[Index]->Text = SizeName;
	((TMenuNode *)SizesNode->Item[Index]->Data)->LongDescription = SizeName;
	SizesNode->CustomSort(CompareFunc, 1);
	SizesNode->Expand(false);
	tvMenu->Items->EndUpdate();

	for (int i=0; i<SizesNode->Count; i++)
	{
		TSizeNode *SizeData = (TSizeNode *)SizesNode->Item[i]->Data;
		if (SizeData->LongDescription.UpperCase() == SizeName.UpperCase())
		{
			return i;
		}
	}
	return -1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSyncCategoriesClick(TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		std::vector<Menu::TNameAndKey>  *MasterBreakdownCategoryList =  &((TItemSizeNode *)CurrentTreeNode->Data)->BCategoryList;

		//if(MasterCategoryList->Count > 0)
		//{
		if (Application->MessageBox("This will set the categories of all items in this course to be the same as this item.\rDo you wish to continue?",
					"Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
		{
			return;
		}
		for (int j=0; j<CurrentTreeNode->Parent->Parent->Count; j++)
		{
			TEditorNode *CurrentItemNode = ((TEditorNode *)CurrentTreeNode->Parent->Parent->Item[j]->Data);
			if (CurrentItemNode->NodeType == ITEM_NODE)
			{
				for (int i=0; i<CurrentItemNode->Owner->Count; i++)
				{
					TItemSizeNode *CurrentItemSizeNode = ((TItemSizeNode *)CurrentItemNode->Owner->Item[i]->Data);
					if (CurrentItemSizeNode != tvMenu->Selected->Data)
					{
						std::vector<Menu::TNameAndKey> *BCategoryList = &CurrentItemSizeNode->BCategoryList;
						BCategoryList->clear();
						BCategoryList->assign(MasterBreakdownCategoryList->begin(),MasterBreakdownCategoryList->end());
					}
				}
			}
		}
		MenuEdited = true;
		//}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnEditCategoryClick(TObject *Sender)
{
	if (lbAvailableCategories->ItemIndex > -1)
	{
		AnsiString OldCategoryName = lbAvailableCategories->Items->Strings[lbAvailableCategories->ItemIndex];
		AnsiString CategoryName = OldCategoryName;
		if (InputQuery("Change Category", "Enter a new name for the category (Max " + IntToStr(MAX_CATEGORY_LENGTH) + " chars).", CategoryName))
		{
			if (CategoryName != "")
			{
				if (CategoryName.Length() > MAX_CATEGORY_LENGTH)
				{
					if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
								"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
					{
						return;
					}
					CategoryName = CategoryName.SubString(1, MAX_CATEGORY_LENGTH);
				}
				int GroupIndex;
				int CategoryIndex;
				if (FindCategory(CategoryName, &GroupIndex, &CategoryIndex, NULL))
				{
					if (GroupIndex != tvMenu->Selected->Index || CategoryIndex != lbAvailableCategories->ItemIndex)
					{
						Application->MessageBox("A category of this name already exists.",
						"Error", MB_ICONERROR + MB_OK);
						return;
					}
				}
				if (FindCategoryGroup(CategoryName, &GroupIndex))
				{
					Application->MessageBox("This category has already been used as a group name.", "Error", MB_OK + MB_ICONERROR);
					return;
				}
				MenuEdited = true;

				GroupIndex = tvMenu->Selected->Index;

				int NewIndex = EditCategory(CategoryName, GroupIndex, lbAvailableCategories->ItemIndex);

				TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
				for (int i=FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
				{
					TTreeNode *CourseNode = MenuNode->Item[i];
					for (int j=0; j<CourseNode->Count; j++)
					{
						TTreeNode *ItemNode = CourseNode->Item[j];
						for (int k=0; k<ItemNode->Count; k++)
						{
							TTreeNode *ItemSizeNode = ItemNode->Item[k];
							TEditorNode *ItemSizeData = (TEditorNode *)ItemSizeNode->Data;
							if (ItemSizeData->NodeType == ITEM_SIZE_NODE)
							{
								if (((TItemSizeNode *)ItemSizeData)->Category == OldCategoryName)
								{
									((TItemSizeNode *)ItemSizeData)->Category = CategoryName;
								}
								for (int l=0; l<((TItemSizeNode *)ItemSizeData)->CategoryList->Count; l++)
								{
									if (((TItemSizeNode *)ItemSizeData)->CategoryList->Strings[l] == OldCategoryName)
									{
										((TItemSizeNode *)ItemSizeData)->CategoryList->Strings[l] = CategoryName;
									}
								}
							}
						}
					}
				}
				RefreshMenuDetails();
				lbAvailableCategories->ItemIndex = NewIndex;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnEditCategoryGroupClick(TObject *Sender)
{
	if (lbCategoryGroups->ItemIndex > -1)
	{
		AnsiString OldGroupName = lbCategoryGroups->Items->Strings[lbCategoryGroups->ItemIndex];
		AnsiString GroupName = OldGroupName;
		if (InputQuery("Change Category Group", "Enter a new name for the category group (Max " + IntToStr(MAX_CATEGORY_GROUP_LENGTH) + " chars).", GroupName))
		{
			if (GroupName != "")
			{
				if (GroupName.Length() > MAX_CATEGORY_GROUP_LENGTH)
				{
					if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
								"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
					{
						return;
					}
					GroupName = GroupName.SubString(1, MAX_CATEGORY_GROUP_LENGTH);
				}
				int GroupIndex;
				if (FindCategoryGroup(GroupName, &GroupIndex))
				{
					if (lbCategoryGroups->ItemIndex != GroupIndex)
					{
						lbCategoryGroups->ItemIndex = GroupIndex;
						Application->MessageBox("A category of this name already exists.",
						"Error", MB_ICONERROR + MB_OK);
						return;
					}
				}
				int CategoryIndex;
				if (FindCategory(GroupName, &GroupIndex, &CategoryIndex, NULL))
				{
					Application->MessageBox("This group has already been used as a category name.",
					"Error", MB_ICONERROR + MB_OK);
					return;
				}
				MenuEdited = true;

				int NewIndex = EditCategoryGroup(GroupName, lbCategoryGroups->ItemIndex);
				RefreshMenuDetails();
				lbCategoryGroups->ItemIndex = NewIndex;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnEditCategoryGroup2Click(TObject *Sender)
{
	AnsiString OldGroupName = lbeCategoryGroup->Caption;
	AnsiString GroupName = OldGroupName;
	if (InputQuery("Change Category Group", "Enter a new name for the category group (Max " + IntToStr(MAX_CATEGORY_GROUP_LENGTH) + " chars).", GroupName))
	{
		if (GroupName != "")
		{
			if (GroupName.Length() > MAX_CATEGORY_GROUP_LENGTH)
			{
				if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
							"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
				{
					return;
				}
				GroupName = GroupName.SubString(1, MAX_CATEGORY_GROUP_LENGTH);
			}
			int GroupIndex;
			if (FindCategoryGroup(GroupName, &GroupIndex))
			{
				if (tvMenu->Selected->Index != GroupIndex)
				{
					Application->MessageBox("A category of this name already exists.",
					"Error", MB_ICONERROR + MB_OK);
					return;
				}
			}
			int CategoryIndex;
			if (FindCategory(GroupName, &GroupIndex, &CategoryIndex, NULL))
			{
				Application->MessageBox("This group has already been used as a category name.",
				"Error", MB_ICONERROR + MB_OK);
				return;
			}
			MenuEdited = true;

			EditCategoryGroup(GroupName, tvMenu->Selected->Index);
			RefreshMenuDetails();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnDeleteCategoryClick(TObject *Sender)
{
	if (lbAvailableCategories->ItemIndex <= -1)
	{
		return;
	}
	TTreeNode *CategoryNode = tvMenu->Selected->Item[lbAvailableCategories->ItemIndex];
	TCategoryNode *catData = (TCategoryNode *)CategoryNode->Data;
	if (!catData)
	{
		return;
	}
	if (catData->GetDeleted())
	{
		catData->SetDeleted(false);
	}
	else
	{
		if (Application->MessageBox("This will remove this category from all items.\rDo you wish to continue?",
					"Warning", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
		{
			AnsiString CategoryToDelete = lbAvailableCategories->Items->Strings[lbAvailableCategories->ItemIndex];
			MenuEdited = true;

			TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
			for (int i=FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
			{
				TTreeNode *CourseNode = MenuNode->Item[i];
				for (int j=0; j<CourseNode->Count; j++)
				{
					TTreeNode *ItemNode = CourseNode->Item[j];
					for (int k=0; k<ItemNode->Count; k++)
					{
						TTreeNode *ItemSizeNode = ItemNode->Item[k];
						TEditorNode *ItemSizeData = (TEditorNode *)ItemSizeNode->Data;
						if (ItemSizeData->NodeType == ITEM_SIZE_NODE)
						{
							TItemSizeNode *sizeData = (TItemSizeNode *)ItemSizeData;
							int CategoryIndex = sizeData->CategoryList->IndexOf(CategoryToDelete);
							if (CategoryIndex > -1)
							{
								sizeData->CategoryList->Delete(CategoryIndex);
							}
						}
					}
				}
			}
			catData->SetDeleted(true);
		}
	}
	int OldIndex = lbAvailableCategories->ItemIndex;
	RefreshMenuDetails();
	lbAvailableCategories->SetFocus();
	lbAvailableCategories->ItemIndex = OldIndex;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnDeleteCategoryGroupClick(TObject *Sender)
{
	if (lbCategoryGroups->ItemIndex > -1)
	{
		bool empty = true;
		TTreeNode *CategoryGroupNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX]->Item[lbCategoryGroups->ItemIndex];
		for (int i=0; i<CategoryGroupNode->Count; ++i)
		{
			TCategoryNode *catData = (TCategoryNode *)CategoryGroupNode->Item[i]->Data;
			if (!catData->GetDeleted())
			{
				empty = false;
				break;
			}
		}
		if (!empty)
		{
			Application->MessageBox("You must delete all categories in the group first.", "Error", MB_OK + MB_ICONERROR);
			return;
		}
		MenuEdited = true;

		DeleteCategoryGroup(lbCategoryGroups->ItemIndex);
		lbCategoryGroups->Items->Delete(lbCategoryGroups->ItemIndex);
		int OldIndex = lbCategoryGroups->ItemIndex;
		RefreshMenuDetails();
		lbCategoryGroups->ItemIndex = OldIndex;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnDeleteCategoryGroup2Click(TObject *Sender)
{
	TTreeNode *CategoryGroupNode = tvMenu->Selected;
	bool empty = true;
	for (int i=0; i<CategoryGroupNode->Count; ++i)
	{
		TCategoryNode *catData = (TCategoryNode *)CategoryGroupNode->Item[i]->Data;
		if (!catData->GetDeleted())
		{
			empty = false;
			break;
		}
	}
	if (!empty)
	{
		Application->MessageBox("You must delete all categories in the group first.", "Error", MB_OK + MB_ICONERROR);
		return;
	}
	MenuEdited = true;

	DeleteCategoryGroup(tvMenu->Selected->Index);
	//	tvMenu->Selected->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::BitBtn1Click(TObject *Sender)
{
	for (int i=0; i<chklstCondiments->Count; i++)
	{
		chklstCondiments->Checked[i] = true;
	}
	chklstCondimentsClickCheck(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnAddSideClick(TObject *Sender)
{
	TSelectMenuItem SelectMenuItem;

	TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
	for (int i=FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
	{
		TTreeNode *CourseNode = MenuNode->Item[i];
		if (CourseNode->Count > 1)
		{
			TSelectMenuItem::TCourse Course;
			Course.Name = CourseNode->Text;
			Course.Data = static_cast<TCourseNode *>(CourseNode->Data);
			int CourseIndex = SelectMenuItem.AddCourse(Course);
			for (int j=0; j<CourseNode->Count-1; j++)
			{
				TTreeNode *ItemNode	= CourseNode->Item[j];
				TSelectMenuItem::TItem Item;
				Item.Name = ItemNode->Text;
				Item.Data = static_cast<TItemNode *>(ItemNode->Data);
				SelectMenuItem.AddItem(CourseIndex, Item);
			}
		}
	}
	if (SelectMenuItem.Execute())
	{
		TItemNode *ItemData = (TItemNode *)tvMenu->Selected->Data;
		for (int i=0; i<SelectMenuItem.SelectedCount(); i++)
		{
			TItemNode *SideData = static_cast<TItemNode *>(SelectMenuItem.Selected(i));

			bool DoAdd = true;
			for (int i=0; i<ItemData->ForcedSides->Count; i++)
			{
				if (((TForcedSide *)ItemData->ForcedSides->Items[i])->ItemNode == SideData)
				{
					Application->MessageBox((AnsiString("You already have \"") + SideData->LongDescription + "\" in group " +
					IntToStr(((TForcedSide *)ItemData->ForcedSides->Items[i])->GroupNumber) + ".").c_str(),
					"Error",
					MB_ICONERROR + MB_OK);
					DoAdd = false;
					break;
				}
			}
			for (int i=0; i<SideData->Owner->Count; i++)
			{
				if (TST_PROMO_MASTER(((TItemSizeNode *)SideData->Owner->Item[i]->Data)->SetMenuMask))
				{
					Application->MessageBox("This item is a set menu key and can not be added.",
					"Error",
					MB_ICONERROR + MB_OK);
					DoAdd = false;
					break;
				}
			}
			if (DoAdd)
			{
				int MaxSelect = 1;
				bool SideGroupSkip = true;
				if (lbForcedSides->Items->Count > 0)
				{
					TForcedSide *ForcedSide = (TForcedSide *)lbForcedSides->Items->Objects[0];
					if (ForcedSide)
					{
						MaxSelect		= ForcedSide->MaxSelect;
						SideGroupSkip	= ForcedSide->SideGroupSkip;
					}
				}
				TForcedSide *ForcedSide				= new TForcedSide;
				ForcedSide->ItemNode					= SideData;
				if (rbForcedSideGrp1->Checked)	ForcedSide->GroupNumber	= 1;
				if (rbForcedSideGrp2->Checked)	ForcedSide->GroupNumber	= 2;
				if (rbForcedSideGrp3->Checked)	ForcedSide->GroupNumber	= 3;
				if (rbForcedSideGrp4->Checked)	ForcedSide->GroupNumber	= 4;
				if (rbForcedSideGrp5->Checked)	ForcedSide->GroupNumber	= 5;
				if (rbForcedSideGrp6->Checked)	ForcedSide->GroupNumber	= 6;
				if (rbForcedSideGrp7->Checked)	ForcedSide->GroupNumber	= 7;
				if (rbForcedSideGrp8->Checked)	ForcedSide->GroupNumber	= 8;
				ForcedSide->Key =
				(*new_menu_element_key_generator_)();
				ForcedSide->ItemKey = SideData->Key;
				ForcedSide->MasterItemKey = ItemData->Key;
				ForcedSide->MaxSelect				= MaxSelect;
				ForcedSide->SideGroupSkip			= SideGroupSkip;

				ItemData->ForcedSides->Add(ForcedSide);
				MenuEdited = true;
				rbForcedSideGrpClick(NULL);
				lbForcedSides->ItemIndex = lbForcedSides->Items->Count-1;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::rbForcedSideGrpClick(TObject *Sender)
{
	if (tvMenu->Selected)
	{
		TTreeNode *ItemTreeNode = tvMenu->Selected;
		TEditorNode *NodeData = (TItemNode *)ItemTreeNode->Data;
		if (NodeData->NodeType == ITEM_NODE)
		{
			try
			{
				chbSideMultiSelect->OnClick				= NULL;
				chbSideGroupSkip->OnClick					= NULL;

				TItemNode *ItemData = (TItemNode *)NodeData;
				lbForcedSides->Items->Clear();
				chbSideMultiSelect->Checked				= false;
				chbSideMultiSelect->Enabled				= false;
				chbSideGroupSkip->Checked					= false;
				chbSideGroupSkip->Enabled					= false;
				for (int i=0; i<ItemData->ForcedSides->Count; i++)
				{
					TForcedSide *ForcedSide = (TForcedSide *)ItemData->ForcedSides->Items[i];
					if (ForcedSide->ItemNode)
					{
						TComponent *GroupRadioButton = FindComponent("rbForcedSideGrp" + IntToStr(ForcedSide->GroupNumber));
						if (GroupRadioButton && ((TRadioButton *)GroupRadioButton)->Checked)
						{
							lbForcedSides->Items->AddObject(ForcedSide->ItemNode->LongDescription, (TObject *)ForcedSide);
							if (ForcedSide->MaxSelect == 0)
							{
								chbSideMultiSelect->Checked	= true;
								chbSideGroupSkip->Checked		= true;
								chbSideGroupSkip->Enabled		= false;
							}
							else
							{
								chbSideGroupSkip->Checked		= ForcedSide->SideGroupSkip;
								chbSideGroupSkip->Enabled		= true;
							}
							//						chbSideMultiSelect->Checked		= (ForcedSide->MaxSelect == 0);
							chbSideMultiSelect->Enabled		= true;
						}
					}
				}
			}
			__finally
			{
				chbSideMultiSelect->OnClick				= chbSideMultiSelectClick;
				chbSideGroupSkip->OnClick					= chbSideGroupSkipClick;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbSideMultiSelectClick(TObject *Sender)
{
	TItemNode *ItemData = (TItemNode *)tvMenu->Selected->Data;
	for (int i=0; i<ItemData->ForcedSides->Count; i++)
	{
		TForcedSide *ForcedSide = (TForcedSide *)ItemData->ForcedSides->Items[i];
		if (rbForcedSideGrp1->Checked && ForcedSide->GroupNumber	== 1) ForcedSide->MaxSelect = chbSideMultiSelect->Checked?0:1;
		if (rbForcedSideGrp2->Checked && ForcedSide->GroupNumber	== 2) ForcedSide->MaxSelect = chbSideMultiSelect->Checked?0:1;
		if (rbForcedSideGrp3->Checked && ForcedSide->GroupNumber	== 3) ForcedSide->MaxSelect = chbSideMultiSelect->Checked?0:1;
		if (rbForcedSideGrp4->Checked && ForcedSide->GroupNumber	== 4) ForcedSide->MaxSelect = chbSideMultiSelect->Checked?0:1;
		if (rbForcedSideGrp5->Checked && ForcedSide->GroupNumber	== 5) ForcedSide->MaxSelect = chbSideMultiSelect->Checked?0:1;
		if (rbForcedSideGrp6->Checked && ForcedSide->GroupNumber	== 6) ForcedSide->MaxSelect = chbSideMultiSelect->Checked?0:1;
		if (rbForcedSideGrp7->Checked && ForcedSide->GroupNumber	== 7) ForcedSide->MaxSelect = chbSideMultiSelect->Checked?0:1;
		if (rbForcedSideGrp8->Checked && ForcedSide->GroupNumber	== 8) ForcedSide->MaxSelect = chbSideMultiSelect->Checked?0:1;
	}
	if (chbSideMultiSelect->Checked)
	{
		chbSideGroupSkip->Enabled = false;
		chbSideGroupSkip->Checked = true;
	}
	else
	{
		chbSideGroupSkip->Enabled = true;
	}
	MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbSideGroupSkipClick(TObject *Sender)
{
	TItemNode *ItemData = (TItemNode *)tvMenu->Selected->Data;
	for (int i=0; i<ItemData->ForcedSides->Count; i++)
	{
		TForcedSide *ForcedSide = (TForcedSide *)ItemData->ForcedSides->Items[i];
		if (rbForcedSideGrp1->Checked && ForcedSide->GroupNumber	== 1) ForcedSide->SideGroupSkip = chbSideGroupSkip->Checked;
		if (rbForcedSideGrp2->Checked && ForcedSide->GroupNumber	== 2) ForcedSide->SideGroupSkip = chbSideGroupSkip->Checked;
		if (rbForcedSideGrp3->Checked && ForcedSide->GroupNumber	== 3) ForcedSide->SideGroupSkip = chbSideGroupSkip->Checked;
		if (rbForcedSideGrp4->Checked && ForcedSide->GroupNumber	== 4) ForcedSide->SideGroupSkip = chbSideGroupSkip->Checked;
		if (rbForcedSideGrp5->Checked && ForcedSide->GroupNumber	== 5) ForcedSide->SideGroupSkip = chbSideGroupSkip->Checked;
		if (rbForcedSideGrp6->Checked && ForcedSide->GroupNumber	== 6) ForcedSide->SideGroupSkip = chbSideGroupSkip->Checked;
		if (rbForcedSideGrp7->Checked && ForcedSide->GroupNumber	== 7) ForcedSide->SideGroupSkip = chbSideGroupSkip->Checked;
		if (rbForcedSideGrp8->Checked && ForcedSide->GroupNumber	== 8) ForcedSide->SideGroupSkip = chbSideGroupSkip->Checked;
	}
	MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnRemoveSideClick(TObject *Sender)
{
	if (lbForcedSides->ItemIndex > -1)
	{
		TItemNode *ItemData = (TItemNode *)tvMenu->Selected->Data;

		int CurrentListBoxIndex = -1;
		for (int i=0; i<ItemData->ForcedSides->Count; i++)
		{
			TForcedSide *ForcedSide = (TForcedSide *)ItemData->ForcedSides->Items[i];
			TComponent *GroupRadioButton = FindComponent("rbForcedSideGrp" + IntToStr(ForcedSide->GroupNumber));
			if (GroupRadioButton && ((TRadioButton *)GroupRadioButton)->Checked)
			{
				CurrentListBoxIndex++;
				if (CurrentListBoxIndex == lbForcedSides->ItemIndex)
				{
					ItemData->ForcedSides->Delete(i);
					MenuEdited = true;
					break;
				}
			}
		}
		rbForcedSideGrpClick(NULL);
		lbForcedSides->ItemIndex = lbForcedSides->Items->Count-1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbForcedSidesMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button == mbRight)
	{
		int Index = lbForcedSides->ItemAtPos(Point(X,Y), true);
		if (Index > -1)
		{
			lbForcedSides->ItemIndex = Index;
			lbForcedSides->Selected[Index] = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnCloseClick(TObject *Sender)
{
    //insert priceLevels Name in map
     if (qrGetPriceLevelNames->Database->Connected && !qrGetPriceLevelNames->Transaction->InTransaction)
    {
        qrGetPriceLevelNames->Transaction->StartTransaction();
        qrGetPriceLevelNames->Close();
        qrGetPriceLevelNames->ExecQuery();

        if(qrGetPriceLevelNames->RecordCount>0)
        {
            for(int key =1; key<= noOfPriceLevels; key++)
            {
                Menu::TPriceLevelsName priceLevelName;
              //  priceLevelName.PriceLevelKey = key;
                if(!qrGetPriceLevelNames->Eof)
                PriceLevelsName[key].PriceLevelName = qrGetPriceLevelNames->FieldByName("PRICELEVEL_NAME")->AsString;
             //   PriceLevelsName[1].insert(std::pair<int, Menu::TPriceLevelsName>(key,priceLevelName) );
                qrGetPriceLevelNames->Next();
            }
        }
        qrGetPriceLevelNames->Transaction->Commit();
    }
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::GroupBox5Enter(TObject *Sender)
{
	if (!rbFoodMenu->Checked && !rbDrinkMenu->Checked)
	{
		rbFoodMenu->SetFocus();
		rbFoodMenu->Checked = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::rbFoodMenuKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		chbAvailableOnPalm->SetFocus();
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbItemOnlySideClick(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_NODE)
	{
		TItemNode *ItemData = (TItemNode *)CurrentNodeData;
		ItemData->ItemOnlySide = chbItemOnlySide->Checked;
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbItemOnlySideKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		pcItemDetails->ActivePage = tsItemSizes;
		if (sgItem->ColCount > 1 && sgItem->Objects[sgItem->Col][sgItem->Row])
		{
			sgItem->SetFocus();
			sgItem->Col = 1;
			sgItem->Row = 1;
		}
		else
		{
			lbSizesUnused->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::nePointsPercentExit(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *ItemSizeData = (TItemSizeNode *)CurrentNodeData;
		if (nePointsPercent->Value != ItemSizeData->PointsPercent)
		{
			ItemSizeData->PointsPercent = nePointsPercent->Value;
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnFindClick(TObject *Sender)
{
	FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::FindDialogFind(TObject *Sender)
{
	TTreeNode *CurItem;
	if (tvMenu->Selected)
	{
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = tvMenu->Selected->GetNext();
		}
		else
		{
			CurItem = tvMenu->Selected->GetPrev();
		}
	}
	else
	{
		CurItem = tvMenu->Items->GetFirstNode();
	}
	while (CurItem)
	{
		AnsiString FindText, NodeText;
		if (FindDialog->Options.Contains(frMatchCase))
		{
			FindText = FindDialog->FindText;
			NodeText = CurItem->Text;
		}
		else
		{
			FindText = FindDialog->FindText.UpperCase();
			NodeText = CurItem->Text.UpperCase();
		}
		if (FindDialog->Options.Contains(frWholeWord))
		{
			if (FindText == NodeText)
			{
				tvMenu->Selected = CurItem;
				break;
			}
		}
		else
		{
			if (NodeText.AnsiPos(FindText))
			{
				tvMenu->Selected = CurItem;
				break;
			}
		}
		if (FindDialog->Options.Contains(frDown))
		{
			CurItem = CurItem->GetNext();
		}
		else
		{
			CurItem = CurItem->GetPrev();
		}
	}
	if (!CurItem)
	{
		Application->MessageBox("No more occurances found.", "Search Complete", MB_ICONINFORMATION + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnForcedSideMoveUpClick(TObject *Sender)
{
	if (lbForcedSides->ItemIndex > 0)
	{
		TItemNode *ItemData = (TItemNode *)tvMenu->Selected->Data;

		int CurrentListBoxIndex = -1;
		int LastIndex = -1;
		for (int i=0; i<ItemData->ForcedSides->Count; i++)
		{
			TForcedSide *ForcedSide = (TForcedSide *)ItemData->ForcedSides->Items[i];
			TComponent *GroupRadioButton = FindComponent("rbForcedSideGrp" + IntToStr(ForcedSide->GroupNumber));
			if (GroupRadioButton && ((TRadioButton *)GroupRadioButton)->Checked)
			{
				CurrentListBoxIndex++;
				if (CurrentListBoxIndex == lbForcedSides->ItemIndex && CurrentListBoxIndex > 0 && LastIndex > -1)
				{
					ItemData->ForcedSides->Exchange(i, LastIndex);
					MenuEdited = true;
					break;
				}
				else
				{
					LastIndex = i;
				}
			}
		}
		int LastItemIndex = lbForcedSides->ItemIndex - 1;
		rbForcedSideGrpClick(NULL);
		if (LastItemIndex < lbForcedSides->Items->Count-1)
		{
			lbForcedSides->ItemIndex = LastItemIndex;
		}
		else
		{
			lbForcedSides->ItemIndex = lbForcedSides->Items->Count-1;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::miMoveSideUpClick(TObject *Sender)
{
	btnForcedSideMoveUpClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::nePointsPercentKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		pcItemSizeDetails->ActivePage = tsSizeCategories;
		cbCategory->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::kitchenNameChange(TObject *Sender)
{
	if (tntedKitchenName->Text.Length() > MAX_ITEM_NAME_LENGTH)
	{
		tntedKitchenName->Text = tntedKitchenName->Text.SubString(1, MAX_ITEM_NAME_LENGTH);
		tntedKitchenName->SelStart = tntedKitchenName->Text.Length();
	}
	MenuEdited = true;
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_NODE)
	{
		TItemNode *ItemData		= (TItemNode *)CurrentNodeData;
		ItemData->KitchenName	= tntedKitchenName->Text;
	}
}

void __fastcall TfrmMenuEdit::droidwareNameChange(TObject *Sender)
{
	WideString id_name = ((TTntEdit *)Sender)->Text;

	if (id_name.Length() > MAX_ITEM_NAME_LENGTH)
	id_name = id_name.SubString(1, MAX_ITEM_NAME_LENGTH);

	if (((TTntEdit *)Sender)->Tag)
	((TEditorNode *)tvMenu->Selected->Data)->ReceiptName = id_name;
	else ((TEditorNode *)tvMenu->Selected->Data)->HandheldName = id_name;

	MenuEdited = true;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tntedSizeKitchenNameChange(TObject *Sender)
{
	if (tntedSizeKitchenName->Text.Length() > MAX_SIZE_NAME_LENGTH)
	{
		tntedSizeKitchenName->Text = tntedSizeKitchenName->Text.SubString(1, MAX_SIZE_NAME_LENGTH);
		tntedSizeKitchenName->SelStart = tntedSizeKitchenName->Text.Length();
	}
	MenuEdited = true;
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == SIZE_NODE)
	{
		TSizeNode *SizeData		= (TSizeNode *)CurrentNodeData;
		SizeData->KitchenName	= tntedSizeKitchenName->Text;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbSizeWeighedClick(TObject *Sender)
{
	MenuEdited = true;
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == SIZE_NODE)
	{
		TSizeNode *SizeData = (TSizeNode *)CurrentNodeData;
		SizeData->Weighed = chbSizeWeighed->Checked;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tntedServingCourseKitchenNameChange(TObject *Sender)
{
	if (tntedServingCourseKitchenName->Text.Length() > MAX_SERVING_COURSE_NAME_LENGTH)
	{
		tntedServingCourseKitchenName->Text = tntedServingCourseKitchenName->Text.SubString(1, MAX_SERVING_COURSE_NAME_LENGTH);
		tntedServingCourseKitchenName->SelStart = tntedServingCourseKitchenName->Text.Length();
	}
	MenuEdited = true;
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == SERVING_COURSE_NODE)
	{
		TServingCourseNode *ServingCourseData	= (TServingCourseNode *)CurrentNodeData;
		ServingCourseData->KitchenName      	= tntedServingCourseKitchenName->Text;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbTextColorChange(TObject *Sender)
{
	MenuEdited = true;
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == SERVING_COURSE_NODE)
	{
		TServingCourseNode *ServingCourseData	= (TServingCourseNode *)CurrentNodeData;
		ServingCourseData->Colour = cbTextColor->Selected;
		//cbTextColor->Font->Color = cbTextColor->Selected;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tntedCourseKitchenNameChange(TObject *Sender)
{
	if (tntedCourseKitchenName->Text.Length() > MAX_COURSE_NAME_LENGTH)
	{
		tntedCourseKitchenName->Text = tntedCourseKitchenName->Text.SubString(1, MAX_COURSE_NAME_LENGTH);
		tntedCourseKitchenName->SelStart = tntedCourseKitchenName->Text.Length();
	}
	MenuEdited = true;
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == COURSE_NODE)
	{
		TCourseNode *CourseData	= (TCourseNode *)CurrentNodeData;
		CourseData->KitchenName		= tntedCourseKitchenName->Text;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbNoDefaultServingCourseClick(TObject *Sender)
{
	MenuEdited = true;
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	TCourseNode *CourseData	= (TCourseNode *)CurrentNodeData;
	if (CurrentNodeData->NodeType == COURSE_NODE)
	{
		CourseData->NoDefaultServingCourse	= cbNoDefaultServingCourse->Checked;
	}
	cbServingCourse->Enabled = !cbNoDefaultServingCourse->Checked;
	RefreshCourse(CourseData);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tntedOptionKitchenNameChange(TObject *Sender)
{
	if (tntedOptionKitchenName->Text.Length() > MAX_SIZE_NAME_LENGTH)
	{
		tntedOptionKitchenName->Text = tntedOptionKitchenName->Text.SubString(1, MAX_SIZE_NAME_LENGTH);
		tntedOptionKitchenName->SelStart = tntedOptionKitchenName->Text.Length();
	}
	MenuEdited = true;
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == OPTION_NODE)
	{
		TOptionNode *OptionData	= (TOptionNode *)CurrentNodeData;
		OptionData->KitchenName	= tntedOptionKitchenName->Text;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tntedKitchenNameKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		stDrinkColourClick(Sender);
		cbItemEnable->SetFocus();
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ClearTree()
{
	ClearingTree = true;
	try
	{
		tvMenu->Items->Clear();
		MenuEdited = false;
	}
	__finally
	{
		ClearingTree = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbWarningsDrawItem(TWinControl *Control,
int Index, TRect &Rect, TOwnerDrawState State)
{
	AnsiString Text = ((TListBox *)Control)->Items->Strings[Index];
	TCanvas *pCanvas = ((TListBox *)Control)->Canvas;
	//	pCanvas->FillRect(Rect); // clear the rectangle
	TRect ImgRect(Rect.Left, Rect.Top, Rect.Left + dmUtilities->imlTreeImages->Width + 4, Rect.Bottom);
	pCanvas->FillRect(TRect(Rect.Left + dmUtilities->imlTreeImages->Width + 3, Rect.Top, Rect.Right, Rect.Bottom));

	pCanvas->TextOut(Rect.Left + dmUtilities->imlTreeImages->Width + 6, Rect.Top, Text);

	pCanvas->Brush->Color = lbWarnings->Color;
	pCanvas->FillRect(ImgRect);
	if (Text.SubString(1, 9) == "[Warning]")
	{
		dmUtilities->imlTreeImages->Draw(pCanvas, Rect.Left + 2, Rect.Top, ICON_EXCLAMATION_INDEX, true);
	}
	else if (Text.SubString(1, 7) == "[Error]")
	{
		dmUtilities->imlTreeImages->Draw(pCanvas, Rect.Left + 2, Rect.Top, ICON_CROSS_RED_INDEX, true);
	}
	if (State.Contains(odSelected) && State.Contains(odFocused) && !State.Contains(odNoFocusRect))
	{
		//		pCanvas->DrawFocusRect(ImgRect);
		pCanvas->DrawFocusRect(Rect);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbWarningsDblClick(TObject *Sender)
{
	TEditorNode *DataNode = (TEditorNode *)lbWarnings->Items->Objects[lbWarnings->ItemIndex];
	if (DataNode != NULL)
	{
		lbWarnings->Tag = DataNode->Owner->AbsoluteIndex;
		tvMenu->SetFocus();
		tvMenu->Selected = DataNode->Owner;
		PageControl4->ActivePage = tsProperties;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSetItemsPointsClick(TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		if (Application->MessageBox("This will set the % Points Earned for all the other sizes in this course.\rDo you wish to continue?", "Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
		{
			return;
		}
		for (int j=0; j<CurrentTreeNode->Parent->Parent->Count; j++)
		{
			TEditorNode *CurrentItemNode = ((TEditorNode *)CurrentTreeNode->Parent->Parent->Item[j]->Data);
			if (CurrentItemNode->NodeType == ITEM_NODE)
			{
				for (int i=0; i<CurrentItemNode->Owner->Count; i++)
				{
					TItemSizeNode *CurrentSizeNode = ((TItemSizeNode *)CurrentItemNode->Owner->Item[i]->Data);
					if (CurrentSizeNode != tvMenu->Selected->Data)
					{
						CurrentSizeNode->PointsPercent = nePointsPercent->Value;
					}
				}
			}
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSetItemsSpendingPointsClick(TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		if (Application->MessageBox("This will set the ability to use Points on all the other sizes in this course.\rDo you wish to continue?", "Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
		{
			return;
		}
		for (int j=0; j<CurrentTreeNode->Parent->Parent->Count; j++)
		{
			TEditorNode *CurrentItemNode = ((TEditorNode *)CurrentTreeNode->Parent->Parent->Item[j]->Data);
			if (CurrentItemNode->NodeType == ITEM_NODE)
			{
				for (int i=0; i<CurrentItemNode->Owner->Count; i++)
				{
					TItemSizeNode *CurrentSizeNode = ((TItemSizeNode *)CurrentItemNode->Owner->Item[i]->Data);
					if (CurrentSizeNode != tvMenu->Selected->Data)
					{
						CurrentSizeNode->CanBePaidForUsingPoints = cbCanBePaidForUsingPoints->Checked;
					}
				}
			}
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::neMemberPurchaseCountExit(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *ItemSizeData = (TItemSizeNode *)CurrentNodeData;
		int MemberPurchaseCount = 0;
		// show as zero but store as -1
		if (cbMemberDiscountAlways->Checked)
		MemberPurchaseCount = -1;
		else
		MemberPurchaseCount = neMemberPurchaseCount->Value;
		if (MemberPurchaseCount != ItemSizeData->MemberPurchaseCount)
		{
			ItemSizeData->MemberPurchaseCount = MemberPurchaseCount;
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::neMemberPurchaseDiscountExit(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *ItemSizeData = (TItemSizeNode *)CurrentNodeData;
		if (neMemberPurchaseDiscount->Value != ItemSizeData->MemberPurchaseDiscount)
		{
			ItemSizeData->MemberPurchaseDiscount = neMemberPurchaseDiscount->Value;
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::neLocationPurchaseCountExit(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *ItemSizeData = (TItemSizeNode *)CurrentNodeData;
		int LocationPurchaseCount = 0;
		// show as zero but store as -1
		if (cbLocationDiscountAlways->Checked)
		LocationPurchaseCount = -1;
		else
		LocationPurchaseCount = neLocationPurchaseCount->Value;

		if (LocationPurchaseCount != ItemSizeData->LocationPurchaseCount)
		{
			ItemSizeData->LocationPurchaseCount = LocationPurchaseCount;
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::neLocationPurchaseDiscountExit(
TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *ItemSizeData = (TItemSizeNode *)CurrentNodeData;
		if (neLocationPurchaseDiscount->Value != ItemSizeData->LocationPurchaseDiscount)
		{
			ItemSizeData->LocationPurchaseDiscount = neLocationPurchaseDiscount->Value;
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbCategorySelect(TObject *Sender)
{
	TItemSizeNode *itemSizeData = (TItemSizeNode *)tvMenu->Selected->Data;

	if (cbCategory->ItemIndex > -1)
	{
		if (cbCategory->Items->Objects[cbCategory->ItemIndex] == (TObject *)1)
		{
			itemSizeData->Category = cbCategory->Items->Strings[cbCategory->ItemIndex];
		}
		else
		{
			itemSizeData->Category = "";
			//			cbCategory->ItemIndex = cbCategory->Items->IndexOf(((TItemSizeNode *)tvMenu->Selected->Data)->Category);
		}

		itemSizeData->CategoryKey = CategoryKeyWithName( itemSizeData->Category );
	}
	else
	{
		itemSizeData->Category = "";
	}
	MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSyncCategoryClick(
TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		if (Application->MessageBox("This will set the category of all items in this course to be the same as this item.\rDo you wish to continue?",
					"Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
		{
			return;
		}
		for (int j=0; j<CurrentTreeNode->Parent->Parent->Count; j++)
		{
			TEditorNode *CurrentItemNode = ((TEditorNode *)CurrentTreeNode->Parent->Parent->Item[j]->Data);
			if (CurrentItemNode->NodeType == ITEM_NODE)
			{
				for (int i=0; i<CurrentItemNode->Owner->Count; i++)
				{
					TItemSizeNode *CurrentItemSizeNode = ((TItemSizeNode *)CurrentItemNode->Owner->Item[i]->Data);
					if (CurrentItemSizeNode != tvMenu->Selected->Data)
					{
						CurrentItemSizeNode->Category = ((TItemSizeNode *)CurrentTreeNode->Data)->Category;
						CurrentItemSizeNode->CategoryKey =  ((TItemSizeNode *)CurrentTreeNode->Data)->CategoryKey;
					}
				}
			}
		}
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnNewCategoryGroupClick(TObject *Sender)
{
	AnsiString NewGroupName = "";
	if (InputQuery("New Category Group", "Enter a name for the category group (Max " + IntToStr(MAX_CATEGORY_GROUP_LENGTH) + " chars).", NewGroupName))
	{
		if (NewGroupName != "")
		{
			if (NewGroupName.Length() > MAX_CATEGORY_GROUP_LENGTH)
			{
				if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
							"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
				{
					return;
				}
				NewGroupName = NewGroupName.SubString(1, MAX_CATEGORY_GROUP_LENGTH);
			}
			int Index = InsertCategoryGroup(NewGroupName);
			RefreshMenuDetails();
			MenuEdited = true;
			lbCategoryGroups->ItemIndex = Index;
		}
	}
}
//---------------------------------------------------------------------------
int TfrmMenuEdit::InsertCategoryGroup(AnsiString GroupName)
{
	TTreeNode *CategoriesNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
	TCategoryGroupsNode *CategoriesData = (TCategoryGroupsNode *)CategoriesNode->Data;
	int GroupIndex;
	int CategoryIndex;
	if (FindCategoryGroup(GroupName, &GroupIndex))
	{
		return GroupIndex;
	}
	if (FindCategory(GroupName, &GroupIndex, &CategoryIndex, NULL))
	{
		Application->MessageBox("This group has already been used as a category name.", "Error", MB_OK + MB_ICONERROR);
		return -1;
	}
	TTreeNode *NewGroupNode = CategoriesData->AddNode(CATEGORY_GROUP_NODE, false);
	NewGroupNode->Text = GroupName;
	((TEditorNode *)NewGroupNode->Data)->LongDescription = GroupName;
	reinterpret_cast<TEditorNode *>(NewGroupNode->Data)->Key =
	(*new_menu_element_key_generator_)();

	tvMenu->Items->BeginUpdate();
	CategoriesNode->CustomSort(CompareFunc, 1);
	CategoriesNode->Expand(false);
	tvMenu->Items->EndUpdate();

	for (int i=0; i<CategoriesNode->Count; i++)
	{
		TEditorNode *GroupData = (TEditorNode *)CategoriesNode->Item[i]->Data;
		if (GroupData->LongDescription.UpperCase() == GroupName.UpperCase())
		{
			return i;
		}
	}
	return -1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cb3rdPartyGroupCodeChange(TObject *Sender)
{
	if (cb3rdPartyGroupCode->Text.Length() > MAX_ITEM_NAME_LENGTH)
	{
		cb3rdPartyGroupCode->Text = cb3rdPartyGroupCode->Text.SubString(1, MAX_3RD_PARTY_CODE_LENGTH);
		cb3rdPartyGroupCode->SelStart = cb3rdPartyGroupCode->Text.Length();
	}
	MenuEdited = true;
	if (tvMenu->Selected)
	{
		if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == ITEM_SIZE_NODE)
		{
			TItemSizeNode *ItemSizeData = (TItemSizeNode *)tvMenu->Selected->Data;
			ItemSizeData->ThirdPartyCode = cb3rdPartyGroupCode->Text;
			if(ItemSizeData->ThirdPartyCode == "")
			{
				ItemSizeData->ThirdPartyCodeKey = 0;
			}
			else if(ItemSizeData->ThirdPartyCodeKey >= 0)
			{
				ItemSizeData->ThirdPartyCodeKey = (*new_menu_element_key_generator_)();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSync3rdPartyGroupItemClick(
TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		AnsiString MasterThirdPartyCode = ((TItemSizeNode *)CurrentTreeNode->Data)->ThirdPartyCode;
		if(MasterThirdPartyCode != "")
		{
			if (Application->MessageBox(AnsiString("All sizes in this item will be set to '" +
							MasterThirdPartyCode + "'.\rDo you wish to continue?").c_str(),
						"Warning",
						MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
			{
				return;
			}
			for (int i=0; i<CurrentTreeNode->Parent->Count; i++)
			{
				TTreeNode *CurrentItemSizeNode	= CurrentTreeNode->Parent->Item[i];
				TItemSizeNode *CurrentItemSizeData	= (TItemSizeNode *)CurrentItemSizeNode->Data;

				if (CurrentItemSizeData->NodeType == ITEM_SIZE_NODE && CurrentItemSizeData != tvMenu->Selected->Data)
				{
					CurrentItemSizeData->ThirdPartyCode = MasterThirdPartyCode;
				}
			}
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSync3rdPartyGroupCourseClick(
TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		AnsiString MasterThirdPartyCode = ((TItemSizeNode *)CurrentTreeNode->Data)->ThirdPartyCode;
		if(MasterThirdPartyCode != "")
		{
			if (Application->MessageBox(AnsiString("WARNING!\r\rAll items in this course will be set to '" +
							MasterThirdPartyCode + "'.\rDo you wish to continue?").c_str(),
						"Warning",
						MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
			{
				return;
			}
			for (int j=0; j<CurrentTreeNode->Parent->Parent->Count; j++)
			{
				TEditorNode *CurrentItemNode = ((TEditorNode *)CurrentTreeNode->Parent->Parent->Item[j]->Data);
				if (CurrentItemNode->NodeType == ITEM_NODE)
				{
					for (int i=0; i<CurrentItemNode->Owner->Count; i++)
					{
						TItemSizeNode *CurrentItemSizeData = ((TItemSizeNode *)CurrentItemNode->Owner->Item[i]->Data);
						if (CurrentItemSizeData != tvMenu->Selected->Data)
						{
							CurrentItemSizeData->ThirdPartyCode = MasterThirdPartyCode;
						}
					}
				}
			}
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSync3rdPartyGroupMenuClick(
TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		AnsiString MasterThirdPartyCode = ((TItemSizeNode *)CurrentTreeNode->Data)->ThirdPartyCode;
		if(MasterThirdPartyCode != "")
		{
			if (Application->MessageBox(AnsiString("WARNING!\r\rAll items in this menu will be set to '" +
							MasterThirdPartyCode + "'.\rDo you wish to continue?").c_str(),
						"Warning",
						MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
			{
				return;
			}
			TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
			for (int i=FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
			{
				TTreeNode *CourseNode = MenuNode->Item[i];
				if (((TEditorNode *)CourseNode->Data)->NodeType == COURSE_NODE)
				{
					for (int j=0; j<CourseNode->Count; j++)
					{
						TTreeNode *ItemNode = CourseNode->Item[j];

						if (((TEditorNode *)ItemNode->Data)->NodeType == ITEM_NODE)
						{
							for (int k=0; k<ItemNode->Count; k++)
							{
								TTreeNode *ItemSizeNode = ItemNode->Item[k];
								if (ItemSizeNode->Data != tvMenu->Selected->Data)
								{
									TItemSizeNode *ItemSizeData = (TItemSizeNode *)ItemSizeNode->Data;

									ItemSizeData->ThirdPartyCode = MasterThirdPartyCode;
								}
							}
						}
					}
				}
			}
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::chbAvailableOnPalmKeyPress(TObject *Sender,
char &Key)
{
	if (Key == VK_RETURN)
	{
		btnNewCourse->SetFocus();
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSizeDeleteSizeClick(TObject *Sender) // cww
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == SIZE_NODE)
	{
		bool ItemSizeStillExists = false;
		TTreeNode *ExistingItemSize = NULL;

		TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
		for (int i=FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
		{
			TTreeNode *CourseNode = MenuNode->Item[i];
			for (int j=0; j<CourseNode->Count; j++)
			{
				TTreeNode *ItemNode = CourseNode->Item[j];
				for (int k=0; k<ItemNode->Count; k++)
				{
					TTreeNode *ItemSizeNode = ItemNode->Item[k];
					TEditorNode *ItemSizeData = (TEditorNode *)ItemSizeNode->Data;
					if (ItemSizeData->NodeType == ITEM_SIZE_NODE)
					{
						if (ItemSizeData->LongDescription == CurrentNodeData->LongDescription)
						{
							ItemSizeStillExists = true;
							ExistingItemSize = ItemSizeNode;
							break;
						}
					}
				}
				if (ItemSizeStillExists) break;
			}
			if (ItemSizeStillExists) break;
		}
		if (ItemSizeStillExists)
		{
			Application->MessageBox("An item size still exists and must be deleted first.", "Error",
			MB_OK + MB_ICONERROR);
			tvMenu->Selected = ExistingItemSize;
		}
		else
		{
			if (Application->MessageBox("This will delete the selected Size.\rDo you wish to continue?",
						"Warning", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)

			{
				tvMenu->Selected->Delete();
				MenuEdited = true;
				RelabelDrinkCosts();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbAvailableCategoriesKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{
	if (Key == VK_DELETE) btnDeleteCategoryClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbCategoriesKeyDown(TObject *Sender,
WORD &Key, TShiftState Shift)
{
	if (Key == VK_DELETE) btnRemoveCategoryClick(Sender);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnNewServingCourseClick(TObject *Sender) //cww
{
	AnsiString NewServingCourseName = "";
	if (InputQuery("New Serving Course", "Enter a name for the Serving Course (Max " + IntToStr(MAX_SERVING_COURSE_NAME_LENGTH) + " chars).", NewServingCourseName))
	{
		if (NewServingCourseName != "")
		{
			if (NewServingCourseName.Length() > MAX_SERVING_COURSE_NAME_LENGTH)
			{
				if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
							"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
				{
					return;
				}
				NewServingCourseName = NewServingCourseName.SubString(1, MAX_SERVING_COURSE_NAME_LENGTH);
			}
			MenuEdited = true;
			int Index = InsertServingCourse(NewServingCourseName);
			/* delete from the DeletedServingCoursesInfo if it exists
			ie they added after they have deleted one of the same name */
			DeleteFromServingCourses( NewServingCourseName );
			cbServingCoursesEnable->Enabled = true;
			//RelabelDrinkCosts();
			RefreshMenuDetails();
			lbAvailableServingCourses->ItemIndex = Index;
            ServingCoursesList->Clear();
            GetAllServingCourses(ServingCoursesList);
		}
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnServingCoursesEditClick(TObject *Sender)     //cww
{
	if (lbAvailableServingCourses->ItemIndex > -1)
	{
		AnsiString OldServingCourseName = lbAvailableServingCourses->Items->Strings[lbAvailableServingCourses->ItemIndex];
		AnsiString ServingCourseName    = OldServingCourseName;
		if (InputQuery("Change Serving Course Name", "Enter a new name for the Serving Course (Max " + IntToStr(MAX_SERVING_COURSE_NAME_LENGTH) + " chars).", ServingCourseName))
		{
			if (ServingCourseName != "")
			{
				if (ServingCourseName.Length() > MAX_SERVING_COURSE_NAME_LENGTH)
				{
					if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
								"Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
					{
						return;
					}
					ServingCourseName = ServingCourseName.SubString(1, MAX_SERVING_COURSE_NAME_LENGTH);
				}
				if (lbAvailableServingCourses->Items->IndexOf(ServingCourseName) != -1)
				{
					if (lbAvailableServingCourses->Items->IndexOf(ServingCourseName) != lbAvailableServingCourses->ItemIndex)
					{
						Application->MessageBox("A Serving Course of this name already exists.",
						"Error", MB_ICONERROR + MB_OK);
						return;
					}
				}
				MenuEdited = true;

				int NewIndex = EditServingCourse(ServingCourseName, lbAvailableServingCourses->ItemIndex);

				lbAvailableServingCourses->ItemIndex = NewIndex;
				//int OldIndex = lbAvailableServingCourses->ItemIndex;
				//RelabelDrinkCosts();
				RefreshMenuDetails();
				lbAvailableServingCourses->ItemIndex = NewIndex;
                ServingCoursesList->Clear();
                GetAllServingCourses(ServingCoursesList);

			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnServingCoursesDeleteClick(TObject *Sender)  // cww
{
	if (Application->MessageBox("This will delete the selected Serving Course.\rDo you wish to continue?",
				"Warning", MB_OKCANCEL + MB_ICONQUESTION) == ID_CANCEL)
	return;
	if (lbAvailableServingCourses->ItemIndex > -1)
	{
		// Delete the Serving Course Node
		DeleteServingCourse(lbAvailableServingCourses->ItemIndex);

		int OldIndex = lbAvailableServingCourses->ItemIndex;
		lbAvailableServingCourses->Items->Delete(lbAvailableServingCourses->ItemIndex);

		RefreshMenuDetails();
		lbAvailableServingCourses->SetFocus();
		if (OldIndex > lbAvailableServingCourses->Items->Count -1) OldIndex--;
		lbAvailableServingCourses->ItemIndex = OldIndex;
        ServingCoursesList->Clear();
        GetAllServingCourses(ServingCoursesList);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnUpServingCourseClick(TObject *Sender)
{ // Move up the List Box and TreeView
	if (lbAvailableServingCourses->ItemIndex != -1)
	{
		if (lbAvailableServingCourses->ItemIndex != 0)
		{
			int OldIndex = lbAvailableServingCourses->ItemIndex;
			// Available Serving Courses
			TTreeNode *ServingCoursesNode = tvMenu->Items->GetFirstNode()->Item[SERVING_COURSES_INDEX];
			// Index of the lbAvailableServingCourses->ItemIndex
			TTreeNode *ServingCourseNode =  ServingCoursesNode->Item[OldIndex];
			// If it has a Previous Child
			if (ServingCoursesNode->GetPrevChild(ServingCourseNode))
			{
				lbAvailableServingCourses->Items->Move(lbAvailableServingCourses->ItemIndex, lbAvailableServingCourses->ItemIndex - 1);
				int KeepStateIndex = ServingCourseNode->StateIndex;
				ServingCourseNode->MoveTo(ServingCoursesNode->GetPrevChild(ServingCourseNode), naInsert);
				RefreshServingCourses((TServingCoursesNode *)ServingCoursesNode->Data);
				// Bug in tree - must revert to -1 then show the correct int after that
				ServingCourseNode->StateIndex = -1;
				ServingCourseNode->StateIndex = KeepStateIndex;
				lbAvailableServingCourses->SetFocus();
				lbAvailableServingCourses->ItemIndex = OldIndex-1;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnDownServingCourseClick(TObject *Sender)
{ // Move down the List Box and TreeView
	if (lbAvailableServingCourses->ItemIndex != -1)
	{
		if (lbAvailableServingCourses->ItemIndex != lbAvailableServingCourses->Items->Count -1)
		{
			int oldindex = lbAvailableServingCourses->ItemIndex;
			// Available Serving Courses
			TTreeNode *ServingCoursesNode = tvMenu->Items->GetFirstNode()->Item[SERVING_COURSES_INDEX];
			// Index of the lbAvailableServingCourses->ItemIndex
			TTreeNode *ServingCourseNode =  ServingCoursesNode->Item[oldindex];
			// If it has a Previous Child
			if (ServingCoursesNode->GetNextChild(ServingCourseNode))
			{
				lbAvailableServingCourses->Items->Move(lbAvailableServingCourses->ItemIndex, lbAvailableServingCourses->ItemIndex + 1);
				// Get next Child and move this one up - Insert moves above the Line
				TTreeNode *NextChildNode = ServingCoursesNode->GetNextChild(ServingCourseNode);
				int KeepStateIndex = NextChildNode->StateIndex;
				NextChildNode->MoveTo(ServingCourseNode, naInsert);
				RefreshServingCourses((TServingCoursesNode *)ServingCoursesNode->Data);
				// Bug in TREE - must revert to -1 then show the correct int after that
				NextChildNode->StateIndex = -1;
				NextChildNode->StateIndex = KeepStateIndex;
				lbAvailableServingCourses->SetFocus();
				lbAvailableServingCourses->ItemIndex =  oldindex+1;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbServingCourseEnableClick(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == SERVING_COURSE_NODE)
	{
		TServingCourseNode *ServingCourseData = (TServingCourseNode *)CurrentNodeData;
		if (cbServingCourseEnable->Checked)
		{
			ServingCourseData->Enable();
		}
		else
		{
			ServingCourseData->Disable();
		}
		MenuEdited = true;
        ServingCoursesList->Clear();
        GetAllServingCourses(ServingCoursesList);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbSelectableClick(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == SERVING_COURSE_NODE)
	{
		TServingCourseNode *ServingCourseData = (TServingCourseNode *)CurrentNodeData;
		if (cbSelectable->Checked)
		{
			ServingCourseData->Selectable = true;
		}
		else
		{
			ServingCourseData->Selectable = false;
		}
		MenuEdited = true;
        ServingCoursesList->Clear();
        GetAllServingCourses(ServingCoursesList);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbAvailableServingCoursesDrawItem(TWinControl *Control,
int Index, TRect &Rect, TOwnerDrawState State)
{
	try
	{
		int ImageIndex = bool(lbAvailableServingCourses->Items->Objects[Index])?0:1;
		TRect DrawRect = Rect;
		cbServingCoursesEnable->OnClick = NULL;
		cbServingCoursesEnable->State = cbChecked;
		if (ImageIndex != 0)
		{
			dmUtilities->imlTreeStateImages->Draw(lbAvailableServingCourses->Canvas,DrawRect.Left,DrawRect.Top,ImageIndex);
			DrawRect.Left += 16;
			cbServingCoursesEnable->State = cbUnchecked;
		}
		lbAvailableServingCourses->Canvas->TextRect(DrawRect, DrawRect.Left + 3, DrawRect.Top, lbAvailableServingCourses->Items->Strings[Index].c_str());
	}
	__finally
	{
		cbServingCoursesEnable->OnClick = cbServingCoursesEnableClick;

	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbServingCoursesEnableClick(TObject *Sender)
{
	if (lbAvailableServingCourses->ItemIndex > -1 )
	{
		int OldIndex = lbAvailableServingCourses->ItemIndex;
		TTreeNode *ServingCoursesNode = tvMenu->Items->GetFirstNode()->Item[SERVING_COURSES_INDEX];
		TServingCourseNode *ServingCourseData = (TServingCourseNode *)ServingCoursesNode->Item[lbAvailableServingCourses->ItemIndex]->Data;
		if (cbServingCoursesEnable->Checked)
		{
			ServingCourseData->Enable();
		}
		else
		{
			ServingCourseData->Disable();
		}
		MenuEdited = true;
		RefreshServingCourses((TServingCoursesNode *)ServingCoursesNode->Data);
		lbAvailableServingCourses->SetFocus();
		lbAvailableServingCourses->ItemIndex = OldIndex;
        ServingCoursesList->Clear();
        GetAllServingCourses(ServingCoursesList);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbAvailableServingCoursesKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_DELETE) btnServingCoursesDeleteClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnServingCourseDeleteClick(TObject *Sender)
{
	if (Application->MessageBox("Are you sure you want to delete this Serving Course?",
				"Menu Editor",
				MB_OKCANCEL + MB_ICONQUESTION + MB_DEFBUTTON1) == IDOK)
	{
		//TServingCourseNode *ServingCourseNode = ((TServingCourseNode *)tvMenu->Selected->Data);
		AddToDeletedServingCourses((TServingCourseNode *)tvMenu->Selected->Data);
		tvMenu->Selected->Delete();
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuEndDrag(TObject *Sender,
TObject *Target, int X, int Y)
{
	//	TTreeNode *DraggedNode		 	= ((TTreeView *)Sender)->Selected;

	//	TServingCourseNode *MyData		= (TServingCourseNode *)DraggedNode->Data;

	//	if (MyData->Serving_Course_Visible)
	//    ShowMessage("Visible");
	//ShowMessage(IntToStr(GlobalStateIndex));

}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lbAvailableServingCoursesEnter(TObject *Sender)
{
	cbServingCoursesEnable->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::tvMenuEnter(TObject *Sender)
{
	if (PageControl1->ActivePage == tsServingCourses)
	cbServingCoursesEnable->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnCategoryDeleteClick(TObject *Sender)
{
	TCategoryNode *catData = (TCategoryNode *)tvMenu->Selected->Data;
	if (!catData)
	{
		return;
	}
	if (!catData->GetDeleted())
	{
		if (Application->MessageBox("This will remove this category from all items.\rDo you wish to continue?",
					"Warning", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
		{
			AnsiString CategoryToDelete = edCategoryName->Text;
			MenuEdited = true;

			// Deletes the Category from the Item Size
			TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
			for (int i=FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
			{
				TTreeNode *CourseNode = MenuNode->Item[i];
				for (int j=0; j<CourseNode->Count; j++)
				{
					TTreeNode *ItemNode = CourseNode->Item[j];
					for (int k=0; k<ItemNode->Count; k++)
					{
						TTreeNode *ItemSizeNode = ItemNode->Item[k];
						TEditorNode *ItemSizeData = (TEditorNode *)ItemSizeNode->Data;
						if (ItemSizeData->NodeType == ITEM_SIZE_NODE)
						{
							TItemSizeNode *sizeData = (TItemSizeNode *)ItemSizeData;
							int CategoryIndex = sizeData->CategoryList->IndexOf(CategoryToDelete);
							if (CategoryIndex > -1)
							{
								sizeData->CategoryList->Delete(CategoryIndex);
							}
						}
					}
				}
			}
			catData->SetDeleted(true);
		}
	}
	else
	{
		catData->SetDeleted(false);
	}
	RefreshMenuDetails();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbColourClick(TObject *Sender)
{
	if (((TCheckBox *)Sender)->Checked)
	{
		((TCheckBox *)Sender)->Caption = "Red";
		((TCheckBox *)Sender)->Font->Color = clRed;
	}
	else
	{
		((TCheckBox *)Sender)->Caption = "Black";
		((TCheckBox *)Sender)->Font->Color = clBlack;
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbServingCourseChange(TObject *Sender)
{
	if (cbServingCourse->Text.Length() > MAX_SERVING_COURSE_NAME_LENGTH)
	{
		cbServingCourse->Text = cbServingCourse->Text.SubString(1, MAX_SERVING_COURSE_NAME_LENGTH);
		cbServingCourse->SelStart = cbServingCourse->Text.Length();
	}
	MenuEdited = true;
	if (tvMenu->Selected)
	{
		if (((TEditorNode *)tvMenu->Selected->Data)->NodeType == COURSE_NODE)
		{
			TCourseNode *CourseData = (TCourseNode *)tvMenu->Selected->Data;
			// "None" is for display and not an actual Serving Course
			if (cbServingCourse->Text == "None")
			CourseData->ServingCourse = "";
			else
			CourseData->ServingCourse = cbServingCourse->Text;
			CourseData->ServingCourseKey = ServingCourseKeyWithName( CourseData->ServingCourse );
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSyncServingCourseClick(TObject *Sender)
{
	if (!cbServingCourseEnable->Checked)
	{
		Application->MessageBox("This Serving Course is not Enabled","Error",MB_OK + MB_ICONERROR );
		return;
	}
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == SERVING_COURSE_NODE)
	{
		if (Application->MessageBox("This will set this Serving Course to all of the Course's in the Menu.\rDo you wish to continue?",
					"Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
		{
			return;
		}
		TTreeNode *ClassesNode = tvMenu->Items->GetFirstNode();
		for (int i = FIRST_COURSE_INDEX; i<ClassesNode->Count; i++)
		{
			TTreeNode *CourseNode = ClassesNode->Item[i];
			if (((TEditorNode *)CourseNode->Data)->NodeType == COURSE_NODE)
			{
				TCourseNode *CourseData = ((TCourseNode *)CourseNode->Data);
				CourseData->ServingCourse = edServingCourseName->Text;
			}
		}
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSyncNoDefaultSCClick(TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == COURSE_NODE)
	{
		if (Application->MessageBox("This will set all of the Courses in the menu to these Serving Course settings.\rDo you wish to continue?",
					"Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
		{
			return;
		}
		TTreeNode *ClassesNode = tvMenu->Items->GetFirstNode();
		for (int i = FIRST_COURSE_INDEX; i<ClassesNode->Count; i++)
		{
			TTreeNode *CourseNode = ClassesNode->Item[i];
			if (((TEditorNode *)CourseNode->Data)->NodeType == COURSE_NODE)
			{
				TCourseNode *CourseData = ((TCourseNode *)CourseNode->Data);
				if (cbNoDefaultServingCourse->Checked == true)
				{
					CourseData->ServingCourse = "";
					CourseData->NoDefaultServingCourse = true;
				}
				else
				{
					CourseData->ServingCourse = cbServingCourse->Items->Strings[cbServingCourse->ItemIndex];
					CourseData->NoDefaultServingCourse = false;
				}
			}
		}
		MenuEdited = true;
	}
	// Refresh Current Item
	RefreshCourse((TCourseNode *)tvMenu->Selected->Data);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbMemberDiscountAlwaysClick(TObject *Sender)
{
	if (cbMemberDiscountAlways->Checked )
	neMemberPurchaseCount->Enabled = false;
	else
	neMemberPurchaseCount->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbLocationDiscountAlwaysClick(TObject *Sender)
{
	if (cbLocationDiscountAlways->Checked )
	neLocationPurchaseCount->Enabled = false;
	else
	neLocationPurchaseCount->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cbPlusOptionClick(TObject *Sender)
{
	if  (((TCheckBox *)Sender)->Name == cbPlusOption->Name)
	{
		if (cbPlusOption->Checked == false && cbMinusOption->Checked == false )
		cbMinusOption->Checked = true;
	}
	else
	{
		if (cbPlusOption->Checked == false && cbMinusOption->Checked == false )
		cbPlusOption->Checked = true;
	}
	MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::pcItemDetailsChange(TObject *Sender)
{
	if (pcItemDetails->ActivePageIndex == 2)
	lbForcedSides->ItemIndex = lbForcedSides->Items->Count-1;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSyncSideClick(TObject *Sender)
{
	// Forced Sides
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	TItemNode *SelectedItemData = (TItemNode *)tvMenu->Selected->Data;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_NODE)
	{
		if (Application->MessageBox("This will set the Forced Sides of all items in this course to be the same as this item.\rDo you wish to continue?", "Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
		{
			return;
		}
		// Course Node then each Item
		for (int j=0; j<CurrentTreeNode->Parent->Count; j++)
		{
			TItemNode *ItemData = ((TItemNode *)CurrentTreeNode->Parent->Item[j]->Data);
			if (ItemData->NodeType == ITEM_NODE && SelectedItemData->LongDescription != ItemData->LongDescription )
			{
				// delete all Forced Sides from Item
				for (int i=ItemData->ForcedSides->Count -1; i>-1 ; i--)
				{
					TForcedSide *ForcedSide = (TForcedSide *)ItemData->ForcedSides->Items[i];
					delete ForcedSide;
				}
				ItemData->ForcedSides->Clear();
				// add all Forced Sides from the Selected Item
				for (int i=0 ; i<SelectedItemData->ForcedSides->Count; i++)
				{
					TForcedSide *ForcedSide		= new TForcedSide;
					ForcedSide->Key    		    = ((TForcedSide *)SelectedItemData->ForcedSides->Items[i])->Key;
					ForcedSide->ItemKey    		= ((TForcedSide *)SelectedItemData->ForcedSides->Items[i])->ItemKey;
					ForcedSide->MasterItemKey   = ((TForcedSide *)SelectedItemData->ForcedSides->Items[i])->MasterItemKey;
					ForcedSide->CourseName		= ((TForcedSide *)SelectedItemData->ForcedSides->Items[i])->CourseName;
					ForcedSide->ItemName		= ((TForcedSide *)SelectedItemData->ForcedSides->Items[i])->ItemName;
					ForcedSide->ItemNode		= ((TForcedSide *)SelectedItemData->ForcedSides->Items[i])->ItemNode;
					ForcedSide->GroupNumber		= ((TForcedSide *)SelectedItemData->ForcedSides->Items[i])->GroupNumber;
					ForcedSide->MaxSelect		= ((TForcedSide *)SelectedItemData->ForcedSides->Items[i])->MaxSelect;
					ForcedSide->SideGroupSkip	= ((TForcedSide *)SelectedItemData->ForcedSides->Items[i])->SideGroupSkip;
					ItemData->ForcedSides->Add(ForcedSide);
				}
			}
		}
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSyncOptionsClick(TObject *Sender)
{
	// Forced Options or Condiments
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	TItemNode *SelectedItemData = (TItemNode *)tvMenu->Selected->Data;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_NODE)
	{
		if (Application->MessageBox("This will set the Forced Options of all items in this course to be the same as this item.\rDo you wish to continue?", "Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
		{
			return;
		}
		// Course Node then each Item
		for (int j=0; j<CurrentTreeNode->Parent->Count; j++)
		{
			TItemNode *ItemData = ((TItemNode *)CurrentTreeNode->Parent->Item[j]->Data);
			if (ItemData->NodeType == ITEM_NODE && SelectedItemData->LongDescription != ItemData->LongDescription )
			{
				// delete all Forced Options from Item
				for (int group_number = 0; group_number < 8; group_number++)
				{
					ItemData->ForcedOptions[group_number].clear();
				}
				// add all Forced Options from the Selected Item
				for (int group_number = 0; group_number < 8; group_number++)
				{
					std::map<AnsiString, TForcedOption>::iterator it = SelectedItemData->ForcedOptions[group_number].begin();
					for (; it != SelectedItemData->ForcedOptions[group_number].end(); it++)
					{
						TForcedOption forcedOption = TForcedOption(it->second.Name,
						(*new_menu_element_key_generator_)(),
						it->second.OptionFK,
						ItemData->Key);
						ItemData->ForcedOptions[group_number][it->first] = forcedOption;
					}
				}
			}
		}
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnSyncPrintingClick(TObject *Sender)
{
	// Forced Options or Condiments
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	TItemNode *SelectedItemData = (TItemNode *)tvMenu->Selected->Data;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_NODE)
	{
		if (Application->MessageBox("This will set the Printing Options of all items in this course to be the same as this item.\rDo you wish to continue?", "Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
		{
			return;
		}
		// Save to put any changes into TPrinterOptions
		SaveNode(CurrentTreeNode);
		// Course Node then each Item
		for (int j=0; j<CurrentTreeNode->Parent->Count; j++)
		{
			TItemNode *ItemData = ((TItemNode *)CurrentTreeNode->Parent->Item[j]->Data);
			if (ItemData->NodeType == ITEM_NODE && SelectedItemData->LongDescription != ItemData->LongDescription )
			{
				ItemData->PrinterOptions.PrintUnderlined   = SelectedItemData->PrinterOptions.PrintUnderlined;
				ItemData->PrinterOptions.PrintBold		   = SelectedItemData->PrinterOptions.PrintBold;
				ItemData->PrinterOptions.PrintColour	   = SelectedItemData->PrinterOptions.PrintColour;
				ItemData->PrinterOptions.PrintFont		   = SelectedItemData->PrinterOptions.PrintFont;
				ItemData->PrinterOptions.PrintDoubleWidth  = SelectedItemData->PrinterOptions.PrintDoubleWidth;
				ItemData->PrinterOptions.PrintDoubleHeight = SelectedItemData->PrinterOptions.PrintDoubleHeight;
			}
		}
		MenuEdited = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::btnCoursePricesClick(TObject *Sender)
{
	TTreeNode *currentTreeNode = tvMenu->Selected;
	if (((TEditorNode *)currentTreeNode->Data)->NodeType == COURSE_NODE)
	{
		std::vector<std::pair<TItemSizeNode*, TItemNode*> > prices;

		TCourseNode *courseData = (TCourseNode *)tvMenu->Selected->Data;
		for (int i=0; i<courseData->Owner->Count; i++)
		{
			if (((TEditorNode *)courseData->Owner->Item[i]->Data)->NodeType == ITEM_NODE)
			{
				TTreeNode *itemNode = courseData->Owner->Item[i];
				TItemNode *itemData = (TItemNode *)itemNode->Data;

				for (int j=0; j<itemData->Owner->Count; j++)
				{
					if (((TEditorNode *)itemData->Owner->Item[j]->Data)->NodeType == ITEM_SIZE_NODE)
					{
						TTreeNode *itemCostNode = itemData->Owner->Item[j];
						TItemSizeNode *itemCostData = (TItemSizeNode *)itemCostNode->Data;

						prices.push_back(std::pair<TItemSizeNode*, TItemNode*>(itemCostData, itemData));
					}
				}
			}
		}

		std::auto_ptr<MenuEditor::TfrmCourseItemPrices> form(new MenuEditor::TfrmCourseItemPrices());
		MenuEditor::CourseItemPricesController controller(form.get());
		form->SetController(&controller);
		controller.SetCurrentPrices(prices);
		if (form->Show() == mrOk)
		{
			MenuEdited = true;
			// Refresh Current Item
			RefreshCourse((TCourseNode *)tvMenu->Selected->Data);
		}
	}
}
//---------------------------------------------------------------------------

bool TfrmMenuEdit::IterateOverMenuItems(TIterationAction &action)
{
#define GetType(this) \
		((reinterpret_cast<TEditorNode *>((this)->Data))->NodeType)
#define ToItemSizeNode(this) \
		(reinterpret_cast<TItemSizeNode *>((this)))

	TTreeNode *root = tvMenu->Items->GetFirstNode();
	TTreeNode *course;
	TTreeNode *item;
	bool result = false;

	for (int i = FIRST_COURSE_INDEX; i < root->Count; i++)
	if (GetType(course = root->Item[i]) == COURSE_NODE)
	for (int j = 0; j < course->Count; j++)
	if (GetType(item = course->Item[j]) == ITEM_NODE)
	for (int k = 0; k < item->Count; k++) {
		if ((result = action(
						ToItemSizeNode(item->Item[k]->Data), (*new_menu_element_key_generator_)())))
		goto __escape;
	}

	__escape:
	return result;

#undef ToItemSizeNode
#undef GetType
}

void __fastcall TfrmMenuEdit::Button1Click(TObject *Sender)
{
#define ErrorCatchValue 0x7fffffff

	AnsiString codeBase;
	std::auto_ptr<TIterationAction> assignmentAction;
	int codeBaseAsInt;
	bool terminate = false;

	for (int state = 0; !terminate ; ) {
		switch (state) {
		case 0:
			if (!InputQuery("Enter a starting code",
						"Enter a starting number.", codeBase))
			return;
			codeBaseAsInt =
			StrToIntDef(codeBase, ErrorCatchValue);
			codeBase = "";
			state += codeBaseAsInt != ErrorCatchValue;
			break;
		case 1:
			if (Application->MessageBox(
						"Do you wish to replace existing codes?",
						"Replace existing codes?",
						MB_YESNO) == IDYES) {
				state++;
			} else {
				assignmentAction.reset(
				new TSkipAction(codeBaseAsInt));
				terminate++;
			}
			break;
		case 2:
			if (Application->MessageBox(
						"Are you sure? This will *erase* existing codes!",
						"Replace existing codes?",
						MB_YESNO) == IDYES)
			assignmentAction.reset(
			new TReplaceAction(codeBaseAsInt));
			else return;
			terminate++;
			break;
		}
	}

	IterateOverMenuItems(*assignmentAction);

#undef ErrorCatchValue
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::btnAddRecipesToAllSizesClick(TObject *Sender)
{
	TTreeNode *curr_item = tvMenu->Selected->Parent;
	TTreeNode *curr_size = curr_item->getFirstChild();
	TRecipe *dst_curr_recipe;
	TList *dst_rlist;
	TRecipe *src_curr_recipe;
	TList *src_rlist =
	((TItemSizeNode *)tvMenu->Selected->Data)->RecipeList;
	int i, j, k, r_present;

	do {
		dst_rlist =
		((TItemSizeNode *)curr_size->Data)->RecipeList;
		if (dst_rlist == src_rlist)
		continue;
		else if (!dst_rlist)
		dst_rlist = new TList();

		for (i = 0; i < src_rlist->Count; i++) {
			src_curr_recipe =
			(TRecipe *)src_rlist->Items[i];
			for (j = r_present = 0;
			j < dst_rlist->Count && !r_present; j++) {
				dst_curr_recipe =
				(TRecipe *)dst_rlist->Items[j],
				r_present =
				*src_curr_recipe == *dst_curr_recipe;
			}
			if (!r_present)
			dst_rlist->Add(new TRecipe(*src_curr_recipe));
		}
	} while ((curr_size = curr_item->GetNextChild(curr_size)));
}
//---------------------------------------------------------------------------

bool __fastcall TfrmMenuEdit::optionKeyInTheList( __int32 inOptionKey, std::vector<Menu::TNameAndKey> forcedOptions )
{
	std::vector<Menu::TNameAndKey>::iterator keyIT;

	for( keyIT = forcedOptions.begin(); keyIT != forcedOptions.end(); ++keyIT)
	{
		Menu::TNameAndKey nameAndKey = *keyIT;
		if( nameAndKey.Key == inOptionKey )
		{
			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------

__int32 __fastcall TfrmMenuEdit::CategoryKeyWithName( AnsiString inCategory )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::::::::

	TTreeNode   *cgTreeNode = tvMenu->Items->GetFirstNode()->getFirstChild()->getNextSibling()->getFirstChild();
	TEditorNode *cgData;

	while( ( result == 0 ) && ( cgTreeNode != NULL ) && ( ( ( TEditorNode * )cgTreeNode->Data )->NodeType == CATEGORY_GROUP_NODE ) )
	{
		cgData = ( TEditorNode * )cgTreeNode->Data;

		TTreeNode   *cTreeNode = cgTreeNode->getFirstChild();
		TEditorNode *cData;

		while( ( cTreeNode != NULL ) && ( ( ( TEditorNode * )cTreeNode->Data )->NodeType == CATEGORY_NODE ) )
		{
			cData = ( TEditorNode * )cTreeNode->Data;

			if( ( ( TCategoryNode* )cData )->LongDescription.UpperCase() == inCategory.UpperCase() )
			{
				result = ( ( TCategoryNode* )cData )->Key;
				break;
			}

			cTreeNode = cTreeNode->getNextSibling();
		}

		cgTreeNode = cgTreeNode->getNextSibling();
	}

	//:::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 __fastcall TfrmMenuEdit::ServingCourseKeyWithName( AnsiString inSCName )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::::::::

	TTreeNode   *scTreeNode = tvMenu->Items->GetFirstNode()->getFirstChild()->getNextSibling()->getNextSibling()->getFirstChild();
	TEditorNode *scData     = ( TEditorNode * )scTreeNode->Data;

	while( scData->NodeType == SERVING_COURSE_NODE )
	{
		if( ( ( TServingCourseNode* )scData )->LongDescription.UpperCase() == inSCName.UpperCase() )
		{
			result = ( ( TServingCourseNode* )scData )->Key;
			break;
		}

		scTreeNode = scTreeNode->getNextSibling();
		scData     = ( TEditorNode * )scTreeNode->Data;
	}

	//:::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

bool TfrmMenuEdit::LoadMenu()
{
	if( MenuEdited )
	{
		AnsiString text    = "You have not saved. Are you sure you want to lose your changes?";
		AnsiString caption = "Warning";
		__int32    flags   = MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2;

		if( Application->MessageBox( text.c_str(), caption.c_str(), flags ) != IDOK )
		{
			return false;
		}
	}

	//....................................

	bool result = false;

	//::::::::::::::::::::::::::::::::::::

	TButtonResult btnResult = ShowLoadMenuDialog();

	initMenuTaxProfileProvider();

	switch( btnResult )
	{
	case brLoadFromDB:   result = LoadFromDB();   break;
	case brLoadFromFile: result = LoadFromFile(); break;
	case brNewMenu:      result = LoadNew();      break;
	}

	//::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
TButtonResult TfrmMenuEdit::ShowLoadMenuDialog()
{
	TButtonResult result = brNone;
	//:::::::::::::::::::::::::::::::::::
	std::auto_ptr<TfrmMenuLoad> frmMenuLoad( new TfrmMenuLoad( NULL ) );
	frmMenuLoad->ShowDBLoad = ( dmMMData->dbMenuMate->Connected );
	result = frmMenuLoad->Execute();
	//:::::::::::::::::::::::::::::::::::
	return result;
}
//---------------------------------------------------------------------------

void TfrmMenuEdit::initMenuTaxProfileProvider()
{
	if( menuTaxProfileProvider == NULL )
	{
		menuTaxProfileProvider = new TMenuTaxProfileProvider();
	}
	else
	{
		menuTaxProfileProvider->Reset();
	}
}
//---------------------------------------------------------------------------
// This function must be called everytime a menu is loaded from the database
void TfrmMenuEdit::resetMenuTaxProfileProvider( TIBDatabase* inDatabase )
{
	menuTaxProfileProvider->Reset( inDatabase );

	resetTaxProfileAssigner( menuTaxProfileProvider );
}
//---------------------------------------------------------------------------
// This function must be called everytime a menu is loaded from a xml file
void TfrmMenuEdit::resetMenuTaxProfileProvider( TLoadMenu* inLoadMenu )
{
	menuTaxProfileProvider->Reset( inLoadMenu );

	resetTaxProfileAssigner( menuTaxProfileProvider );
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::resetTaxProfileAssigner( TMenuTaxProfileProvider *inMenuTaxProfileProvider )
{
	if( menuTaxProfileAssigner != NULL )
	{
		delete menuTaxProfileAssigner;
	}

	//::::::::::::::::::::::::::::::::::::::

	TaxProfileList allTPs;
	TStrings      *unassignedTPs = lbUnassigned->Items;
	TStrings      *assignedTPs   = lbAssigned->Items;
	inMenuTaxProfileProvider->ReadAllTaxProfiles( allTPs );
	menuTaxProfileAssigner = new TTaxProfileAssigner( allTPs, unassignedTPs, assignedTPs );
	ShowHideMaxRetailPrice(false);
	for(TaxProfileIterator it = allTPs.begin(); it != allTPs.end(); std::advance(it, 1))
	{
		if(( *it )->taxProfileType == ProfitTax)
			ShowHideMaxRetailPrice(true);
	}

	menuTaxProfileAssigner->OnAssignedTaxProfilesChanged = assignedTaxProfilesChanged;
}
//---------------------------------------------------------------------------
// This function must be called everytime an item size is selected in the Menu tree node
void TfrmMenuEdit::setTaxProfilesInAssignerForItemSize(TItemSizeNode* inItemSizeNode, TMenuTaxProfileProvider *inMenuTaxProfileProvider )
{
	TaxProfileList itemSizeTPs;
	inMenuTaxProfileProvider->ReadTaxProfilesWithKeys(inItemSizeNode->TaxProfileKeys, itemSizeTPs);

	// Calling this method will throw the event below: AssignedTaxProfilesChanged
	menuTaxProfileAssigner->ItemSizeTaxProfiles(  itemSizeTPs );
}

void TfrmMenuEdit::ShowHideMaxRetailPrice(bool displayFlag)
{
	lblMaxRetailPrice->Visible = displayFlag;
	edItemMaxPrice->Visible = displayFlag;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::assignedTaxProfilesChanged(TObject* sender, TStrings* const taxProfileContainer)
{
	selectedItemSizeNode->TaxProfileKeys.clear();
	int otherTaxes = 0;
	int profitTax = 0;
	try
	{
		for( int i = 0; i < taxProfileContainer->Count; i++ )
		{
			TaxProfile* tp = ( TaxProfile* )taxProfileContainer->Objects[i];
			if(tp->taxProfileType == ProfitTax)
				profitTax++;
			else if(tp->taxProfileType != ServiceCharge && tp->taxProfileType != ServiceChargeTax)
				otherTaxes++;
		}
		

		for( int i = 0; i < taxProfileContainer->Count; i++ )
		{
			TaxProfile* tp = ( TaxProfile* )taxProfileContainer->Objects[i];
			selectedItemSizeNode->TaxProfileKeys.insert(tp->GetTaxProfileDBKey());
		}
	}
	catch( Exception &exc )
	{
		Application->MessageBox( exc.Message.c_str(), "Warning", MB_OK + MB_ICONWARNING );
	}
}
//---------------------------------------------------------------------------

bool TfrmMenuEdit::LoadFromDB()
{
	bool result = false;

	//::::::::::::::::::::::::::::::::

	MenuEdited = false;

	result = LoadMenuFromDB();

	if( result )
	{
		DeletedServingCoursesInfo.DeletedServingCourseVector.clear();
	}
	else
	{
		Close();
	}

	//::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::LoadFromFile()
{
	bool result = false;

	//::::::::::::::::::::::::::::::::

	AnsiString loadPath = CurrentConnection.MenuSavePath + "\\Menus";

	if( !DirectoryExists( loadPath ) )
	{
		if( !CreateDir( loadPath ) )
		{
			loadPath = CurrentConnection.MenuSavePath;
		}
	}

	OpenDialog1->Title = "Load Menu File";
	OpenDialog1->InitialDir = loadPath;

	if( OpenDialog1->FileName == "" )
	{
		OpenDialog1->FileName = "*.xml";
	}
	if( OpenDialog1->Execute() )
	{
		DeletedServingCoursesInfo.DeletedServingCourseVector.clear();
		MenuEdited = false;

		if( LoadMenuFromFile( OpenDialog1->FileName ) )
		{
			result = true;
		}
		else
		{
			Close();
		}
	}

	//::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::LoadNew()
{
	bool result = false;

	//::::::::::::::::::::::::::::::::

	DeletedServingCoursesInfo.DeletedServingCourseVector.clear();
	Panel4->Visible = true;
	LoadProgress->Visible = false;

	result = NewMenu();

	//::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::
//  Load Menu from DB
//::::::::::::::::::::::::::::::::::::::::::

bool TfrmMenuEdit::LoadMenuFromDB()
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::::::

	if( InitMenuTransaction() )
	{
		__int32 menuKey = GetMenuDBKey();

		if( menuKey > 0 )
		{
			LoadProgress->Max = GetMenuDBCount( menuKey );

			if( LoadProgress->Max > 0 )
			{
				LoadProgress->Position = 0;
				LoadProgress->Visible  = true;
				Panel4->Visible        = false;

				Update();
			}
			if( menuKey > 0 )
			{
				TCursor Save_Cursor = Screen->Cursor;
				Screen->Cursor = crHourGlass;
				try
				{
					resetMenuTaxProfileProvider( dmMMData->dbMenuMate );

					result = LoadTree2( menuKey );
				}
				__finally
				{
					Screen->Cursor = Save_Cursor;
				}
			}
			Panel4->Visible = true;
			LoadProgress->Visible = false;
		}
		else
		{
			if( dmMMData->trMenuMate->InTransaction )
			{
				dmMMData->trMenuMate->Commit();
			}
		}
	}

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::InitMenuTransaction()
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::::::

	if( !dmMMData->trMenuMate->InTransaction )
	{
		dmMMData->trMenuMate->StartTransaction();
	}

	dmMMData->dtMenu->Active = true;

	if( !dmMMData->dtMenu->IsEmpty() )
	{
		dmMMData->trMenuMate->Commit();
		result = true;
	}
	else
	{
		Application->MessageBox( "There are no menus to load.", "Menu Editor Error", MB_OK + MB_ICONERROR );

		if( dmMMData->trMenuMate->InTransaction )
		{
			dmMMData->trMenuMate->Commit();
		}
	}

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
__int32 TfrmMenuEdit::GetMenuDBKey()
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::

	std::auto_ptr<TfrmSelectMenu> menuForm( new TfrmSelectMenu( NULL ) );

	if( menuForm->ShowModal() == mrOk )
	{
		result = menuForm->MenuKey;
	}
	else
	{
		if( dmMMData->trMenuMate->InTransaction )
		{
			dmMMData->trMenuMate->Commit();
		}
	}

	//:::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
__int32 TfrmMenuEdit::GetMenuDBCount( __int32 inMenuKey )
{
	__int32 result = 0;
	//:::::::::::::::::::::::::::::::
	if( !MMTransaction->InTransaction )
	{
		MMTransaction->StartTransaction();
	}
	qrMenuCount->Close();
	qrMenuCount->ParamByName( "Menu_Key" )->AsInteger = inMenuKey;
	qrMenuCount->ExecQuery();
	result = qrMenuCount->Fields[0]->AsInteger;
	if( MMTransaction->InTransaction )
	{
		MMTransaction->Commit();
	}
	//:::::::::::::::::::::::::::::::
	return result;
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::LoadTree2(int MenuKey)
{
	sgRecipe->RowCount		= 2;
	sgRecipe->Cells[0][1]	= "";
	sgRecipe->Cells[1][1]	= "";
	sgRecipe->Cells[2][1]	= "";
	edQty->Caption				= 0;
	edRecipeQty->Value				= 0;
	edRecipeQty->Enabled				= false;
	edRecipeQty->Color				= clSilver;
	edLoc->Text					= "";
	edLoc->Items->Clear();
	edLoc->Enabled				= false;
	edLoc->Color				= clSilver;
	bool LoadSuccess			= false;
	try
	{
		ClearTree();
		tvMenu->Items->BeginUpdate();
		Menu::TMenuLoadDB		     MenuLoader(dmMMData->dbMenuMate, MenuKey);
		Menu::TMenuInfo		     MenuInfo;
		Menu::TSizesInfo		     SizesInfo;
		Menu::TCategoriesInfo     CategoriesInfo;
		Menu::TLocationsInfo	     LocationsInfo;
		Menu::TServingCoursesInfo ServingCoursesInfo;
		if (MenuLoader.GetMenu(&MenuInfo, &SizesInfo, &CategoriesInfo, &LocationsInfo, &ServingCoursesInfo))
		{
			AnsiString FilePath = CurrentConnection.ServerPath + "\\Menu Import";
			if (DirectoryExists(FilePath) && FileExists(FilePath + "\\" + MenuInfo.Menu_Name + ".xml"))
			{
				tvMenu->Items->EndUpdate();
				Application->MessageBox("A committed menu has not yet been imported by MenuMate.", "Error",
				MB_OK + MB_ICONERROR);
			}
			else
			{
				try
				{
						LoadPriceLevelNames();
       
					LoadProgress->Max  = MenuLoader.Count();
					if (LoadProgress->Max > 0)
					{
						LoadProgress->Position	= 0;
						LoadProgress->Visible	= true;
						Panel4->Visible			= false;
						Update();
					}

					std::auto_ptr<TStringList> const ItemsList(new TStringList);
					std::auto_ptr<TList>	   const ForcedSideList(new TList);
					ItemsList->Sorted			= true;
					int ItemCount				= 0; // Limit for evaluation mode
					TTreeNode *MenuNode		= NULL;
					TTreeNode *CourseNode	= NULL;
					TTreeNode *ItemNode		= NULL;
					if (dmStockData->dbStock->Connected && !StockTransaction->InTransaction)
					{
						StockTransaction->StartTransaction();
					}
					MenuNode = AddMenu(&MenuInfo, &SizesInfo, &CategoriesInfo, &LocationsInfo, &ServingCoursesInfo);
					Menu::TCourseInfo CourseInfo;
					while (MenuLoader.GetNextCourse(&CourseInfo))
					{
						Update();	// Refresh the screen if necessary.
						CourseNode = AddMenuCourse(MenuNode, &CourseInfo);
						Menu::TItemInfo ItemInfo;
						while (MenuLoader.GetNextItem(&ItemInfo))
						{
							ItemNode = AddMenuItem( MenuLoader, CourseNode, &ItemInfo, ForcedSideList.get() );
							ItemsList->AddObject(CourseInfo.Course_Name + "%" + ItemInfo.Item_Name, (TObject *)ItemNode->Data);
							Menu::TItemSizeInfo ItemSizeInfo;

							while(MenuLoader.GetNextItemSize(&ItemSizeInfo))
							{
								AddMenuSize(ItemNode, &ItemSizeInfo);
								LoadProgress->Position = MenuLoader.Position();
							}
							ItemCount++;
						}
					}
					if (dmStockData->dbStock->Connected && StockTransaction->InTransaction)
					{
						StockTransaction->Commit();
					}
					for (int i=0; i<ForcedSideList->Count; i++)
					{
						TForcedSide *ForcedSide = (TForcedSide *)ForcedSideList->Items[i];
						int Index = ItemsList->IndexOf(ForcedSide->CourseName + "%" + ForcedSide->ItemName);
						if (Index > -1)
						{
							ForcedSide->ItemNode = (TItemNode *)ItemsList->Objects[Index];
						}
					}
					RelabelDrinkCosts();
					MenuNode->Expand(false);
					MenuEdited = false;
					tvMenu->Selected = tvMenu->Items->GetFirstNode();
					LoadSuccess = true;
				}
				catch (Exception &E)
				{
					Application->MessageBox((AnsiString("Please make sure this application is run in conjunction with MenuMate version 3.3.0.0 or greater.\r\r"
					"Message: ") + E.Message).c_str(),
					"Menu Editor Error", MB_OK + MB_ICONERROR);
					if (dmStockData->dbStock->Connected && StockTransaction->InTransaction)
					{
						try
						{
							StockTransaction->Rollback();
						}
						catch (...)
						{
						}
					}
					LoadProgress->Position = 0;
				}
			}
		}
	}
	__finally
	{
		tvMenu->Items->EndUpdate();
	}
	return LoadSuccess;
}
//---------------------------------------------------------------------------
TTreeNode *TfrmMenuEdit::AddMenu(
Menu::TMenuInfo *MenuInfo,
Menu::TSizesInfo *SizesInfo,
Menu::TCategoriesInfo *CategoriesInfo,
Menu::TLocationsInfo *LocationsInfo,
Menu::TServingCoursesInfo *ServingCoursesInfo)  // cww
{
	TTreeNode *MenuNode			= tvMenu->Items->Add(NULL, MenuInfo->Menu_Name);
	TMenuNode *MenuData			= new TMenuNode;
	MenuNode->Data				= MenuData;
	MenuData->Key				= MenuInfo->Key;
	MenuData->Owner				= MenuNode;
	MenuData->LongDescription	= MenuNode->Text;
	rbFoodMenu->Checked			= MenuInfo->Menu_Type == Menu::mtFoodMenu;
	rbDrinkMenu->Checked		= MenuInfo->Menu_Type == Menu::mtBeverageMenu;
	chbAvailableOnPalm->Checked	= MenuInfo->Palmable;
	TTreeNode *SizesNode		  = ((TEditorNode *)MenuNode->Data)->AddNode(SIZES_NODE);
	SizesNode->Text				  = "Available Sizes";
	TTreeNode *CategoriesNode	  = ((TEditorNode *)MenuNode->Data)->AddNode(CATEGORIES_NODE);
	CategoriesNode->Text		  = "Available Categories";
	TTreeNode *ServingCoursesNode = ((TEditorNode *)MenuNode->Data)->AddNode(SERVING_COURSES_NODE); //cww
	ServingCoursesNode->Text      = "Available Serving Courses";
	lbAvailableSizes->Clear();
	for (unsigned i=0; i<SizesInfo->Sizes.size(); i++)
	{
        TAvailableMenuSize availablesize;
		TTreeNode *SizeNode		  = ((TEditorNode *)SizesNode->Data)->AddNode(SIZE_NODE, false);
		SizeNode->Text			  = SizesInfo->Sizes[i].Size_Name;
		TSizeNode *SizeData		  = (TSizeNode *)SizeNode->Data;
		SizeData->Key             = SizesInfo->Sizes[i].Key;
		SizeData->LongDescription = SizesInfo->Sizes[i].Size_Name;
		SizeData->KitchenName	  = SizesInfo->Sizes[i].Size_Kitchen_Name;
		SizeData->HandheldName    = SizesInfo->Sizes[i].Size_Handheld_Name;
		SizeData->ReceiptName     = SizesInfo->Sizes[i].Size_Kitchen_Name;
		SizeData->Weighed		  = SizesInfo->Sizes[i].Weighed;
		SizeData->Size_ID		  = SizesInfo->Sizes[i].Size_ID;
		SizeData->PalmID		  = SizesInfo->Sizes[i].PalmID;
        availablesize.Key = SizeData->Key;
        availablesize.KitchenName = SizeData->KitchenName;
        availablesize.HandheldName = SizeData->HandheldName;
        availablesize.ReceiptName = SizeData->ReceiptName;
        availablesize.Weighed = SizeData->Weighed;
        availablesize.Size_ID = SizeData->Size_ID;
        availablesize.PalmID = SizeData->PalmID;
        AllSizesForMenu[SizeData->LongDescription] = availablesize;
        //AllSizesForMenu.insert(std::make_pair(SizeData->LongDescription, std::make_pair(SizeData->Key, SizeData->Weighed)));
        //AllSizesForMenu.insert(std::make_pair(SizeData->LongDescription, std::make_pair(SizeData->Key, SizeData->Weighed)));
	}
	for (unsigned i=0; i<CategoriesInfo->CategoryGroups.size(); i++)
	{
		TTreeNode *GroupNode = ((TEditorNode *)CategoriesNode->Data)->AddNode(CATEGORY_GROUP_NODE, false);
		TCategoryGroupNode *GroupData = (TCategoryGroupNode *)GroupNode->Data;
		GroupData->Key      	      = CategoriesInfo->CategoryGroups[i].Key;
		GroupData->LongDescription	  = CategoriesInfo->CategoryGroups[i].Category_Group_Name;
		GroupNode->Text				  = CategoriesInfo->CategoryGroups[i].Category_Group_Name;

		std::vector<Menu::TCategoryInfo> categories = CategoriesInfo->CategoryGroups[i].Categories;
		std::vector<Menu::TCategoryInfo>::iterator nkIT;
		for( nkIT = categories.begin(); nkIT != categories.end(); nkIT++)
		{
			TTreeNode *CategoryNode	= ((TEditorNode *)GroupNode->Data)->AddNode(CATEGORY_NODE, false);
			TCategoryNode    *CategoryData = (TCategoryNode *)CategoryNode->Data;
			Menu::TCategoryInfo category     = ( *nkIT );
			CategoryData->Key             = category.Key;
			CategoryData->LongDescription = category.Category_Name;
			CategoryNode->Text			  = category.Category_Name;
			CategoryData->GLCode = category.Category_GL_Code;
		}
	}
	LoadLocations(LocationsInfo);	// For the location dropdown in recipe
	AddServingCourses(ServingCoursesInfo,ServingCoursesNode);
	return MenuNode;
}
//---------------------------------------------------------------------------
void *TfrmMenuEdit::AddServingCourses(Menu::TServingCoursesInfo *ServingCoursesInfo,TTreeNode *ServingCoursesNode)
{
	for (unsigned i=0; i<ServingCoursesInfo->ServingCourses.size(); i++)
	{
		TTreeNode *ServingCourseNode = ((TEditorNode *)ServingCoursesNode->Data)->AddNode(SERVING_COURSE_NODE, false);
		ServingCourseNode->Text		 = ServingCoursesInfo->ServingCourses[i].ServingCourse_Name;
		// Set Disabled image in the tree as it is a Global and Menu List
		if( ServingCoursesInfo->ServingCourses[i].Enabled == false )
		{
			ServingCourseNode->StateIndex 	= DISABLEDSTATE_PIC;
		}
		TServingCourseNode *CourseData = (TServingCourseNode *)ServingCourseNode->Data;
		CourseData->Key              = ServingCoursesInfo->ServingCourses[i].Key;
		CourseData->LongDescription	 = ServingCoursesInfo->ServingCourses[i].ServingCourse_Name;
		CourseData->KitchenName	     = ServingCoursesInfo->ServingCourses[i].ServingCourse_Kitchen_Name;
		CourseData->Enabled          = ServingCoursesInfo->ServingCourses[i].Enabled;
		CourseData->Deleted 		 = false;
		CourseData->Colour   		 = ServingCoursesInfo->ServingCourses[i].Colour;
		CourseData->NewServingCourse = false;
		CourseData->Selectable  	 = ServingCoursesInfo->ServingCourses[i].Selectable;
	}
	return NULL;
}
//---------------------------------------------------------------------------
void *TfrmMenuEdit::AddServingCourse(Menu::TServingCoursesInfo *ServingCoursesInfo,TTreeNode *ServingCoursesNode,int i)
{
	TTreeNode *ServingCourseNode = ((TEditorNode *)ServingCoursesNode->Data)->AddNode(SERVING_COURSE_NODE, false);
	ServingCourseNode->Text		 = ServingCoursesInfo->ServingCourses[i].ServingCourse_Name;
	// Set Disabled image in the tree as it is a Global and Menu List
	if (ServingCoursesInfo->ServingCourses[i].Enabled == false)
	{
		ServingCourseNode->StateIndex 	= DISABLEDSTATE_PIC;
	}
	TServingCourseNode *CourseData = (TServingCourseNode *)ServingCourseNode->Data;
	CourseData->Key  	         = ServingCoursesInfo->ServingCourses[i].Key;
	CourseData->LongDescription	 = ServingCoursesInfo->ServingCourses[i].ServingCourse_Name;
	CourseData->KitchenName	     = ServingCoursesInfo->ServingCourses[i].ServingCourse_Kitchen_Name;
	CourseData->Enabled          = ServingCoursesInfo->ServingCourses[i].Enabled;
	CourseData->Deleted 		 = false;
	CourseData->Colour   		 = ServingCoursesInfo->ServingCourses[i].Colour;
	CourseData->NewServingCourse = false;
	CourseData->Selectable  	 = ServingCoursesInfo->ServingCourses[i].Selectable;
	return NULL;
}
//---------------------------------------------------------------------------
TTreeNode *TfrmMenuEdit::AddMenuCourse(TTreeNode *MenuNode, Menu::TCourseInfo *CourseInfo)
{
	TTreeNode *CourseNode				= ((TEditorNode *)MenuNode->Data)->AddNode(COURSE_NODE);
	CourseNode->Text					= CourseInfo->Course_Name;
	TCourseNode *CourseData				= (TCourseNode *)CourseNode->Data;
	CourseData->Key    		            = CourseInfo->Key;
	CourseData->LongDescription		    = CourseInfo->Course_Name;
	CourseData->KitchenName				= CourseInfo->Course_Kitchen_Name;
	CourseData->HandheldName			= CourseInfo->Course_Handheld_Name;
	CourseData->ReceiptName				= CourseInfo->Course_Receipt_Name;
	CourseData->ServingCourseKey        = CourseInfo->ServingCourse_Key;
	CourseData->ServingCourse			= CourseInfo->ServingCourse_Name;
	CourseData->NoDefaultServingCourse  = CourseInfo->No_Default_Serving_Course;
	TTreeNode *OptionsNode				= CourseData->AddNode(OPTIONS_NODE, false);
	OptionsNode->Text					= "Options";
	for (unsigned i=0; i<CourseInfo->Options.size(); i++)
	{
		TTreeNode *OptionNode			= ((TEditorNode *)OptionsNode->Data)->AddNode(OPTION_NODE, false);
		OptionNode->Text				= CourseInfo->Options[i].Option_Name;
		TOptionNode *OptionData			= (TOptionNode *)OptionNode->Data;
		OptionData->Key                 = CourseInfo->Options[i].Key;
		OptionData->LongDescription	    = CourseInfo->Options[i].Option_Name;
		OptionData->KitchenName			= CourseInfo->Options[i].Option_Kitchen_Name;
		OptionData->CondimentsMask		= CourseInfo->Options[i].Forced_Mask;
		OptionData->GroupNumber			= CourseInfo->Options[i].GroupNumber;
		OptionData->PlusOption			= CourseInfo->Options[i].PlusOption;
		OptionData->MinusOption			= CourseInfo->Options[i].MinusOption;
		OptionData->MaxSelect			= CourseInfo->Options[i].Max_Select;
		OptionData->HandheldName = CourseInfo->Options[i].Option_Handheld_Name;
		OptionData->ReceiptName  = CourseInfo->Options[i].Option_Receipt_Name;
		OptionData->OptionGroupSkip	= CourseInfo->Options[i].Allow_Skip;
		OptionData->PrinterOptions.PrintUnderlined   = CourseInfo->Options[i].Print_Underlined;
		OptionData->PrinterOptions.PrintBold		   = CourseInfo->Options[i].Print_Bold;
		OptionData->PrinterOptions.PrintColour		   = CourseInfo->Options[i].Print_Colour;
		OptionData->PrinterOptions.PrintFont		   = CourseInfo->Options[i].Print_Font;
		OptionData->PrinterOptions.PrintDoubleWidth  = CourseInfo->Options[i].Print_Double_Width;
		OptionData->PrinterOptions.PrintDoubleHeight = CourseInfo->Options[i].Print_Double_Height;
		OptionData->Enabled				= true;
	}
	return CourseNode;
}
//---------------------------------------------------------------------------

TTreeNode *TfrmMenuEdit::AddMenuItem( Menu::TMenuLoadDB& inMenuLoader,
TTreeNode*         inCourseNode,
Menu::TItemInfo*   inItemInfo,
TList*             inForcedSideList )
{
	TTreeNode *itemNode = CreateItemTreeNode( inCourseNode, inItemInfo, inForcedSideList );
	//::::::::::::::::::::::::::::::::::

	TItemNode *itemData	= ( TItemNode* )itemNode->Data;
	TTreeNode *courseOptions =
	reinterpret_cast<TCourseNode *>( inCourseNode->Data )->GetOptionsNode();
	if( courseOptions && courseOptions->HasChildren )
	{
		AddMenuItemForcedOptions( inMenuLoader, itemData, inItemInfo );
	}
	//::::::::::::::::::::::::::::::::::
	return itemNode;
}
//---------------------------------------------------------------------------

TTreeNode *TfrmMenuEdit::AddMenuItem( TLoadMenu*       inLoadMenu,
__int32          inItemHandle,
TTreeNode*       inCourseNode,
Menu::TItemInfo* inItemInfo,
TList*           inForcedSideList)
{
	TTreeNode *itemNode = CreateItemTreeNode( inCourseNode, inItemInfo, inForcedSideList );
	//::::::::::::::::::::::::::::::::::
	TItemNode *itemData	= ( TItemNode* )itemNode->Data;
	TTreeNode *courseOptions =
	reinterpret_cast<TCourseNode *>( inCourseNode->Data )->GetOptionsNode();
	if( courseOptions && courseOptions->HasChildren )
	{
		AddMenuItemForcedOptions( inLoadMenu, inItemHandle, itemData, inItemInfo );
	}

	//::::::::::::::::::::::::::::::::::

	return itemNode;
}
//---------------------------------------------------------------------------

TTreeNode* TfrmMenuEdit::CreateItemTreeNode( TTreeNode*        inCourseNode,
Menu::TItemInfo*  inItemInfo,
TList*            inForcedSideList )
{
	TTreeNode *itemNode		  = ((TEditorNode *)inCourseNode->Data)->AddNode(ITEM_NODE, false);
	//:::::::::::::::::::::::::::::::::::::::::::::
	itemNode->Text				  = inItemInfo->Item_Name;
	TItemNode *itemData		  = (TItemNode *)itemNode->Data;
	itemData->Key	           = inItemInfo->Key;
	itemData->LongDescription = inItemInfo->Item_Name;
	itemData->KitchenName	  = inItemInfo->Item_Kitchen_Name;
	itemData->HandheldName	  = inItemInfo->Item_Handheld_Name;
	itemData->ReceiptName	  = inItemInfo->Item_Receipt_Name;
	itemData->Colour			  = inItemInfo->Button_Colour;
	itemData->ItemOnlySide	  = inItemInfo->Exclusively_As_Side;
	itemData->PrinterOptions.PrintUnderlined   = inItemInfo->Print_Underlined;
	itemData->PrinterOptions.PrintBold		    = inItemInfo->Print_Bold;
	itemData->PrinterOptions.PrintColour	    = inItemInfo->Print_Colour;
	itemData->PrinterOptions.PrintFont		    = inItemInfo->Print_Font;
	itemData->PrinterOptions.PrintDoubleWidth  = inItemInfo->Print_Double_Width;
	itemData->PrinterOptions.PrintDoubleHeight = inItemInfo->Print_Double_Height;

	if( inItemInfo->Enabled )
		itemData->Enable();
	else
		itemData->Disable();

	for (unsigned i=0; i<inItemInfo->Sides.size(); i++)
	{
		TForcedSide *forcedSide	  = new TForcedSide;
		forcedSide->Key           = inItemInfo->Sides[i].Key;
		forcedSide->ItemKey       = inItemInfo->Sides[i].Item_Key;
		forcedSide->MasterItemKey = inItemInfo->Sides[i].Master_Item_Key;
		forcedSide->CourseName	  = inItemInfo->Sides[i].Course_Name;
		forcedSide->ItemName	     = inItemInfo->Sides[i].Item_Name;
		forcedSide->GroupNumber	  = inItemInfo->Sides[i].Group_Number;
		forcedSide->MaxSelect	  = inItemInfo->Sides[i].Max_Select;
		forcedSide->SideGroupSkip = inItemInfo->Sides[i].Allow_Skip;
		forcedSide->ItemNode	     = NULL;
		itemData->ForcedSides->Add( forcedSide );
		inForcedSideList->Add( forcedSide );
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::

	return itemNode;
}
//---------------------------------------------------------------------------

#define MAX_FORCED_OPTIONS_GROUP_NUMBER 8

void TfrmMenuEdit::AddMenuItemForcedOptions( Menu::TMenuLoadDB& inMenuLoader,
TItemNode*         inItemData,
Menu::TItemInfo*   inItemInfo )
{
	for( int groupNo = 1; groupNo <= MAX_FORCED_OPTIONS_GROUP_NUMBER; groupNo++ )
	{
		std::map<AnsiString, TForcedOption> &foGroup = inItemData->ForcedOptions[groupNo];
		std::vector<Menu::TItemOptionInfo> fOptions;
		std::vector<Menu::TItemOptionInfo>::const_iterator foIT;
		inMenuLoader.GetForcedItemOptionKeysWithGroupNumber( groupNo, inItemInfo->Key, fOptions );
		for( foIT = fOptions.begin(); foIT != fOptions.end(); foIT++ )
		{
			foGroup[foIT->Name()] = TForcedOption( foIT->Name(),
			foIT->ForcedOptionPK(),
			foIT->OptionFK(),
			foIT->ItemFK() );
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMenuEdit::AddMenuItemForcedOptions( TLoadMenu*       inLoadMenu,
__int32          inItemHandle,
TItemNode*       inItemData,
Menu::TItemInfo* inItemInfo )
{
	for( int groupNo = 1; groupNo <= MAX_FORCED_OPTIONS_GROUP_NUMBER; groupNo++ )
	{
		std::map<AnsiString, TForcedOption> &foGroup = inItemData->ForcedOptions[groupNo];
		std::vector<TForcedOption> fOptions;
		std::vector<TForcedOption>::const_iterator foIT;
		ReadItemForcedOptionsInfo( groupNo,
		inItemInfo->Key,
		inLoadMenu,
		inItemHandle,
		fOptions );

		for( foIT = fOptions.begin(); foIT != fOptions.end(); foIT++ )
		{
			foGroup[foIT->Name] = TForcedOption( foIT->Name,
			foIT->ForcedOptionPK,
			foIT->OptionFK,
			foIT->ItemFK );
		}
	}
}
//---------------------------------------------------------------------------

TTreeNode *TfrmMenuEdit::AddMenuSize(TTreeNode *ItemNode, Menu::TItemSizeInfo *ItemSizeInfo)
{
	TTreeNode *ItemSizeNode				   = ((TEditorNode *)ItemNode->Data)->AddNode(ITEM_SIZE_NODE, false);
	TItemSizeNode *ItemSizeData			   = (TItemSizeNode *)ItemSizeNode->Data;
	ItemSizeData->Key		               = ItemSizeInfo->Key;
	ItemSizeData->SizeKey = ItemSizeInfo->Size_Key;
	ItemSizeData->LongDescription		   = ItemSizeInfo->Size_Name;
	ItemSizeData->Free					   = ItemSizeInfo->Free;
	ItemSizeData->Price					   = ItemSizeInfo->Price;
	ItemSizeData->MaxRetailPrice		   = ItemSizeInfo->MaxRetailPrice;
	ItemSizeData->SpecialPrice			   = ItemSizeInfo->Special_Price;
	ItemSizeData->TareWeight			   = ItemSizeInfo->Tare_Weight;
	ItemSizeData->GST					   = ItemSizeInfo->GST_Percent;
	ItemSizeData->Cost					   = ItemSizeInfo->Cost;
	ItemSizeData->CostGST				   = ItemSizeInfo->Cost_GST_Percent;
	ItemSizeData->PointsPercent			   = ItemSizeInfo->Points_Percent;
	ItemSizeData->Barcode				   = ItemSizeInfo->Barcode;
	ItemSizeData->CategoryKey			   = ItemSizeInfo->CategoryKey;
	ItemSizeData->Category				   = ItemSizeInfo->Category;
	ItemSizeData->SetMenuMask			   = ItemSizeInfo->Setmenu_Mask;
	ItemSizeData->SetMenuStandardItem	   = ItemSizeInfo->Available_As_Standard;
	ItemSizeData->MemberPurchaseCount	   = ItemSizeInfo->Mem_Sale_Count;
	ItemSizeData->MemberPurchaseDiscount   = ItemSizeInfo->Mem_Discount_Percent;
	ItemSizeData->LocationPurchaseCount	   = ItemSizeInfo->Loc_Sale_Count;
	ItemSizeData->LocationPurchaseDiscount = ItemSizeInfo->Loc_Discount_Percent;
	ItemSizeData->ThirdPartyCodeKey		   = ItemSizeInfo->ThirdPartyCodes_Key;
	ItemSizeData->ThirdPartyCode		   = ItemSizeInfo->Third_Party_Code;
	ItemSizeNode->Text					   = FormatFloat(CurrencyString + "0.00", ItemSizeData->Price) +
	" (" + ItemSizeData->LongDescription + ")";
	ItemSizeData->PLU		               = ItemSizeInfo->PLU;
	ItemSizeData->HasServiceCharge = ItemSizeInfo->HasServiceCharge;
	ItemSizeData->IsTaxExempt = ItemSizeInfo->IsTaxExempt;
	ItemSizeData->AvailableQuantity = ItemSizeInfo->AvailableQuantity;
	ItemSizeData->DefaultQuantity = ItemSizeInfo->DefaultQuantity;
	ItemSizeData->WarningQuantity = ItemSizeInfo->WarningQuantity;
	ItemSizeData->DisableWhenCountReachesZero =
	ItemSizeInfo->DisableWhenCountReachesZero;
	ItemSizeData->CanBePaidForUsingPoints =
	ItemSizeInfo->CanBePaidForUsingPoints;
	ItemSizeData->DefaultPatronCount = 	ItemSizeInfo->DefaultPatronCount;
    ItemSizeData->CostForPoints = ItemSizeInfo->PriceForPoints;

    std::map<int,Menu::TItemSizePriceLevel>::const_iterator grpIT = ItemSizeInfo->ItemSizePriceLevels.begin();
    std::map<int,Menu::TItemSizePriceLevel>::const_iterator grpEnd = ItemSizeInfo->ItemSizePriceLevels.end();
    for ( ; grpIT != grpEnd; ++grpIT )
    {
       Menu::TItemSizePriceLevel priceLevel;
       priceLevel.PriceLevelKey = grpIT->second.PriceLevelKey;
       priceLevel.Price = grpIT->second.Price;
       priceLevel.PriceLevelName = grpIT->second.PriceLevelName;
       ItemSizeData->ItemSizePriceLevels.insert ( std::pair<int,Menu::TItemSizePriceLevel>(priceLevel.PriceLevelKey,priceLevel ) );

        if (grpIT->second.Price==0)
        {
         int i=0;
        }
    }


	for (unsigned i=0; i<ItemSizeInfo->Recipes.size(); i++)
	{
		TRecipe *Recipe	  = new TRecipe;
		Recipe->Key       = ItemSizeInfo->Recipes[i].Key;
		Recipe->StockCode = ItemSizeInfo->Recipes[i].Stock_Code;
		Recipe->Quantity  = ItemSizeInfo->Recipes[i].Qty;
		Recipe->Location  = ItemSizeInfo->Recipes[i].Stock_Location;
		ReadRecipeDescriptionAndUnit( Recipe->StockCode, Recipe->Location, Recipe->Description, Recipe->Unit, Recipe->ConversionFactor );

		if (edLoc->Items->IndexOf(Recipe->Location) == -1 && ItemSizeInfo->Recipes[i].Stock_Location != WideString(""))
		{
			edLoc->Items->Add(Recipe->Location);
		}
		ItemSizeData->RecipeList->Add(Recipe);
	}
	for (unsigned i=0; i<ItemSizeInfo->Categories.size(); i++)
	{
		ItemSizeData->BCategoryList.push_back( Menu::TNameAndKey( ItemSizeInfo->Categories[i].Key,
		ItemSizeInfo->Categories[i].Category
		) );
	}
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	TaxProfileKeyIterator tpIT;
	ItemSizeData->TaxProfileKeys.clear();

	for( tpIT = ItemSizeInfo->TaxProfileKeys.begin(); tpIT != ItemSizeInfo->TaxProfileKeys.end(); tpIT++ )
	{
		ItemSizeData->TaxProfileKeys.insert( *tpIT );
	}

	return ItemSizeNode;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadRecipeDescriptionAndUnit( AnsiString  inStockCode,    AnsiString  inLocation,
AnsiString& outDescription, AnsiString& outUnit, Currency& outConversionFactor )
{
	if( dmStockData->dbStock->Connected )
	{
		qrStockDetails->Close();
		qrStockDetails->ParamByName( "Code"     )->AsString	= inStockCode;
		qrStockDetails->ParamByName( "Location" )->AsString = inLocation;
		qrStockDetails->ExecQuery();
		if( !qrStockDetails->Eof )
		{
			outDescription = qrStockDetails->FieldByName( "Description"   )->AsString;
			outUnit		   = qrStockDetails->FieldByName( "Stocktake_Unit")->AsString;
             outConversionFactor  =  qrStockDetails->FieldByName( "CONVERSION_FACTOR")->AsCurrency;

		}
		else
		{
			outDescription = "Unknown";
			outUnit	   	   = "Unknown";
		}
		qrStockDetails->Close();
	}
	else
	{
		outDescription = "Unknown";
		outUnit	   	   = "Unknown";
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::LoadLocations(Menu::TLocationsInfo *LocationsInfo)
{
	for (unsigned i=0; i<LocationsInfo->Locations.size(); i++)
	{
		if (LocationsInfo->Locations[i] != WideString("") && edLoc->Items->IndexOf(LocationsInfo->Locations[i]) == -1)
		{
			edLoc->Items->Add(LocationsInfo->Locations[i]);
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::LoadLocations()
{
	if (dmMMData->dbMenuMate->Connected)
	{
		Menu::TMenuLoadDB Menu(dmMMData->dbMenuMate);
		Menu::TLocationsInfo LocationsInfo;
		Menu.GetAvailableLocations(&LocationsInfo);
		LoadLocations(&LocationsInfo);
	}
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::
//   Load Menu from a XML file
//::::::::::::::::::::::::::::::::::::::::::

bool TfrmMenuEdit::LoadMenuFromFile( AnsiString FileName )
{
	bool result = false;

	//::::::::::::::::::::::::::::::::::::

	InitControls();
	ClearTree();
	itemsList->Clear();
	forcedSideList->Clear();

	if( dmStockData->dbStock->Connected && !StockTransaction->InTransaction )
	{
		StockTransaction->StartTransaction();
	}

	try
	{
		TLoadMenu *loadMenu = new TLoadMenu( FileName );
		result = LoadTreeFromXML( loadMenu );
		new_menu_element_key_generator_.reset(
		menu_key_generator_t::Instance());

		new_menu_element_key_generator_->Reset(loadMenu->LastGeneratedID, -1);

		delete loadMenu;
	}
	catch( ExceptionLoadMenu &lmE )
	{
		ShowMessage( lmE.Message );
	}
	catch( Exception &e )
	{
		ShowMessage( e.Message );
	}

	//::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::InitControls()
{
	sgRecipe->RowCount		= 2;
	sgRecipe->Cells[0][1]	= "";
	sgRecipe->Cells[1][1]	= "";
	sgRecipe->Cells[2][1]	= "";
	edRecipeQty->Value = 0;
	edQty->Caption			= 0;
	edRecipeQty->Enabled			= false;
	edRecipeQty->Color			= clSilver;

	edLoc->Text				= "";
	edLoc->Items->Clear();
	edLoc->Enabled			= false;
	edLoc->Color			= clSilver;
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::LoadTreeFromXML( TLoadMenu *inLoadMenu )
{
	bool result = false;

	//::::::::::::::::::::::::::::::::::::

	InitProgressBar( 6 );   // 5 main nodes in the XML file: Menu, Sizes,
	//                               Category Groups,
	//                               Service Courses,
	//                               Courses
	// +1

	if( true )
	{
		TCursor Save_Cursor = Screen->Cursor;
		Screen->Cursor = crHourGlass;
		try
		{
			resetMenuTaxProfileProvider( inLoadMenu );

			result = CreateCourseNodes( inLoadMenu );
		}
		__finally
		{
			Screen->Cursor = Save_Cursor;
		}
	}

	Panel4->Visible       = true;
	LoadProgress->Visible = false;

	//::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::InitProgressBar( int inMax )
{
	LoadProgress->Max = inMax;

	if (LoadProgress->Max > 0)
	{
		LoadProgress->Position = 0;
		LoadProgress->Visible  = true;
		Panel4->Visible		   = false;
		Update();
	}
}
//---------------------------------------------------------------------------
TTreeNode* TfrmMenuEdit::CreateMenuNode( TLoadMenu *inLoadMenu )
{
	TTreeNode *result = NULL;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		Menu::TMenuInfo		     MenuInfo;
		Menu::TSizesInfo		     SizesInfo;
		Menu::TCategoriesInfo     CategoriesInfo;
		Menu::TLocationsInfo	     LocationsInfo;
		Menu::TServingCoursesInfo ServingCoursesInfo;

		ReadMenuInfo(           inLoadMenu, MenuInfo           ); LoadProgress->Position++;
		ReadSizesInfo(          inLoadMenu, SizesInfo          ); LoadProgress->Position++;
		ReadCategoriesInfo(     inLoadMenu, CategoriesInfo     ); LoadProgress->Position++;
		ReadServingCoursesInfo( inLoadMenu, ServingCoursesInfo ); LoadProgress->Position++;

		if( dmMMData->dbMenuMate->Connected )
		{
			Menu::TMenuLoadDB Menu( dmMMData->dbMenuMate );
			Menu.GetAvailableLocations( &LocationsInfo );
		}
		LoadProgress->Position++;

		result = AddMenu( &MenuInfo, &SizesInfo, &CategoriesInfo, &LocationsInfo, &ServingCoursesInfo );
		LoadProgress->Position++;
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadMenuInfo( TLoadMenu *inLoadMenu, Menu::TMenuInfo &inMenuInfo )
{
	inMenuInfo.Menu_Name = inLoadMenu->MenuName;
	inMenuInfo.Menu_Type = inLoadMenu->MenuType;
	inMenuInfo.Palmable  = inLoadMenu->AvailableOnPalm;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadSizesInfo( TLoadMenu *inLoadMenu, Menu::TSizesInfo &inSizesInfo )
{
	__int32 sizeCount = inLoadMenu->SizeCount();
	inSizesInfo.Sizes.clear();

	WideString sizeName = "";
	WideString sizeKitchenName = "";
	WideString sizeHandheldName = "";
	WideString sizeReceiptName = "";

	for( __int32 i = 0; i < sizeCount; i++ )
	{
		Menu::TSizeInfo sizeInfo;

		inLoadMenu->SizeAtIndex( i, sizeInfo.Key,
		sizeName,
		sizeKitchenName,
		sizeHandheldName,
		sizeReceiptName,
		sizeInfo.Weighed,
		sizeInfo.Size_ID,
		sizeInfo.PalmID );

		sizeInfo.Size_Name = WideString( sizeName );
		sizeInfo.Size_Kitchen_Name = WideString( sizeKitchenName );
		sizeInfo.Size_Handheld_Name = WideString( sizeHandheldName );
		sizeInfo.Size_Receipt_Name = WideString( sizeReceiptName );

		inSizesInfo.Sizes.push_back( sizeInfo );
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadCategoriesInfo( TLoadMenu *inLoadMenu, Menu::TCategoriesInfo &inCategoriesInfo )
{
	__int32 categoryGroupCount = inLoadMenu->CategoryGroupCount();
	inCategoriesInfo.CategoryGroups.clear();

	WideString categoryGroupName = "";

	for( __int32 i = 0; i < categoryGroupCount; i++ )
	{
		Menu::TCategoryGroupInfo categoryGroupInfo;
		bool deleted;

		__int32 categoryGroupID = inLoadMenu->CategoryGroupAtIndex(
		i,
		categoryGroupInfo.Key,
		categoryGroupName,
		deleted );

		categoryGroupInfo.Category_Group_Name =  categoryGroupName;

		//:::::::::::::::::::::::::::::::::

		__int32 categoryCount = inLoadMenu->CategoryCount( categoryGroupID );
		categoryGroupInfo.Categories.clear();

		for( __int32 j = 0; j < categoryCount; j++ )
		{
			Menu::TCategoryInfo category;
			category.Key = 0;
			WideString Category_Name = "";
			WideString Category_GL_Code = "";
			inLoadMenu->CategoryAtIndex( j, categoryGroupID, category.Key, Category_Name, Category_GL_Code, deleted );
			category.Category_Name =  Category_Name;
			category.Category_GL_Code =  Category_GL_Code;
			categoryGroupInfo.Categories.push_back(category  );
		}

		//:::::::::::::::::::::::::::::::::

		inCategoriesInfo.CategoryGroups.push_back( categoryGroupInfo );
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadServingCoursesInfo( TLoadMenu *inLoadMenu, Menu::TServingCoursesInfo &inServingCoursesInfo )
{
	__int32 servingCourseCount = inLoadMenu->ServingCourseCount();
	inServingCoursesInfo.ServingCourses.clear();
	DeletedServingCoursesInfo.DeletedServingCourseVector.clear();

	WideString servingCourseName = "";
	WideString servingCourseKitchenName = "";

	for( __int32 i = 0; i < servingCourseCount; i++ )
	{
		Menu::TServingCourseInfo servingCourseInfo;

		inLoadMenu->ServingCourseAtIndex( i,
		servingCourseInfo.Key,
		servingCourseName,
		servingCourseKitchenName,
		servingCourseInfo.Enabled,
		servingCourseInfo.Deleted,
		servingCourseInfo.Selectable,
		servingCourseInfo.Colour,
		servingCourseInfo.DisplayOrder );

		servingCourseInfo.ServingCourse_Name = UTF8Decode( servingCourseName );
		servingCourseInfo.ServingCourse_Kitchen_Name = UTF8Decode( servingCourseKitchenName );

		if( servingCourseInfo.Deleted )
		{
			DeletedServingCoursesInfo.DeletedServingCourseVector.push_back( servingCourseInfo );
		}
		else
		{
			inServingCoursesInfo.ServingCourses.push_back( servingCourseInfo );
		}
	}
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::CreateCourseNodes( TLoadMenu* inLoadMenu )
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		ClearTree();

		tvMenu->Items->BeginUpdate();

		TTreeNode *menuNode = CreateMenuNode( inLoadMenu );

		StartStockTransaction();

		__int32 courseCount = inLoadMenu->CourseCount();

		WideString courseName = "";
		WideString courseKitchenName = "";
		WideString courseHandheldName = "";
		WideString courseReceiptName = "";
		WideString courseServingCourseName = "";

		for( __int32 i = 0; i < courseCount; i++ )
		{
			Menu::TCourseInfo courseInfo;
			bool courseInfoEnabled;

			__int32 courseID = inLoadMenu->CourseAtIndex( i,
			courseInfo.Key,
			courseName,
			courseKitchenName,
			courseHandheldName,
			courseReceiptName,
			courseInfo.View_Location,
			courseInfoEnabled,
			courseInfo.ServingCourse_Key,
			courseServingCourseName,
			courseInfo.No_Default_Serving_Course );

			courseInfo.Course_Name = courseName;
			courseInfo.Course_Kitchen_Name = courseKitchenName;
			courseInfo.Course_Handheld_Name = courseHandheldName;
			courseInfo.Course_Receipt_Name = courseReceiptName;
			courseInfo.ServingCourse_Name = courseServingCourseName;

			ReadCourseOptionsInfo( inLoadMenu, courseID, courseInfo );

			result = CreateItemNodes( inLoadMenu, courseID, courseInfo.Course_Name, AddMenuCourse( menuNode, &courseInfo ) );

			RelabelDrinkCosts();
			menuNode->Expand(false);
			MenuEdited = false;

			tvMenu->Selected = tvMenu->Items->GetFirstNode();
		}

		tvMenu->Items->EndUpdate();

		if(courseCount)
		result = AssignForcedSidesReferences();
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::::::

	return result;
}

//---------------------------------------------------------------------------
bool TfrmMenuEdit::AssignForcedSidesReferences()
{
	bool result = false;

	try
	{
		/*
		* We do this to set the forced side's ItemNode property. This is
		* needed so that Office can easily access the side item's properties.
		*/
		for( int i=0; i<forcedSideList->Count; i++ )
		{
			TForcedSide *forcedSide = ( TForcedSide * )forcedSideList->Items[i];
			//int index = itemsList->IndexOf( forcedSide->CourseName + "%" + forcedSide->ItemName );
			int index = itemsList->IndexOf( forcedSide->ItemKey );
			if( index > -1 )
			{
				forcedSide->ItemNode = ( TItemNode * )itemsList->Objects[index];
			}
		}
		result = true;
	}
	catch( ... )
	{
	}

	return result;
}

//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadCourseOptionsInfo( TLoadMenu* inLoadMenu, __int32 inCourseID, Menu::TCourseInfo& inCourseInfo )
{
	__int32 optionCount = inLoadMenu->OptionCount( inCourseID );

	WideString optionName = "";
	WideString optionKitchenName = "";
	WideString optionHandheldName = "";
	WideString optionReceiptName = "";

	for( __int32 i = 0; i < optionCount; i++ )
	{
		Menu::TCourseOptionInfo optionInfo;
		bool optionEnabled;

		__int32 optionColor = 0;
		__int32 courseID = inLoadMenu->OptionAtIndex( i,
		inCourseID,
		optionInfo.Key,
		optionName,
		optionKitchenName,
		optionHandheldName,
		optionReceiptName,
		optionInfo.Forced_Mask,
		optionInfo.Flags,
		optionEnabled,
		optionInfo.Print_Underlined,
		optionInfo.Print_Bold,
		optionColor,
		optionInfo.Print_Font,
		optionInfo.Print_Double_Width,
		optionInfo.Print_Double_Height );

		optionInfo.Option_Name = optionName; //UTF8Decode(optionName);
		optionInfo.Option_Kitchen_Name = optionKitchenName; //UTF8Decode(optionKitchenName);
		optionInfo.Option_Handheld_Name = optionHandheldName; //UTF8Decode(optionHandheldName);
		optionInfo.Option_Receipt_Name = optionReceiptName; //UTF8Decode(optionReceiptName);

		optionInfo.Print_Colour = StringToColor( AnsiString( optionColor ) );

		/* Break the option's mask into it's component pieces. */
		inLoadMenu->DisassembleOptionMask(optionInfo.Flags,
		optionInfo.GroupNumber,
		optionInfo.Max_Select,
		optionInfo.PlusOption,
		optionInfo.MinusOption,
		optionInfo.Allow_Skip);

		inCourseInfo.Options.push_back( optionInfo );
	}
}
//---------------------------------------------------------------------------
bool TfrmMenuEdit::CreateItemNodes( TLoadMenu *inLoadMenu, __int32 inCourseHandle, WideString inCourseName, TTreeNode *inCourseNode )   //WideString
{
	bool result = false;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		__int32 itemCount = inLoadMenu->ItemCount( inCourseHandle );

		WideString itemName = "";
		WideString itemKitchenName = "";
		WideString itemHandheldName = "";
		WideString itemReceiptName = "";

		for( __int32 i = 0; i < itemCount; i++ )
		{
			Menu::TItemInfo itemInfo;

			__int32 itemHandle = inLoadMenu->ItemAtIndex( i,
			inCourseHandle,
			itemInfo.Key,
			itemName,
			itemKitchenName,
			itemHandheldName,
			itemReceiptName,
			itemInfo.Button_Colour,
			itemInfo.Display_Sizes,
			itemInfo.Enabled,
			itemInfo.Exclusively_As_Side,
			itemInfo.Print_Underlined,
			itemInfo.Print_Bold,
			itemInfo.Print_Colour,
			itemInfo.Print_Font,
			itemInfo.Print_Double_Width,
			itemInfo.Print_Double_Height );

			itemInfo.Item_Name = itemName;
			itemInfo.Item_Kitchen_Name = itemKitchenName;
			itemInfo.Item_Handheld_Name = itemHandheldName;
			itemInfo.Item_Receipt_Name = itemReceiptName;

			ReadItemForcedSidesInfo( inCourseName, inLoadMenu, itemHandle, itemInfo );

			// The Forced Option list is populated in AddMenuItem function as it
			// depends on the Group Number in each Tree Node
			TTreeNode *itemNode = AddMenuItem( inLoadMenu,
			itemHandle,
			inCourseNode,
			&itemInfo,
			forcedSideList.get());

			itemsList->AddObject( itemInfo.Key, ( TObject * )itemNode->Data );
			result = CreateItemSizeNodes( inLoadMenu, itemHandle, itemNode );

			result = true;
		}
	}
	catch( ... )
	{
	}

	//::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadItemForcedSidesInfo( const WideString &inCourseName, TLoadMenu *inLoadMenu, __int32 inItemID, Menu::TItemInfo &inItemInfo )
{
	__int32 sideCount = inLoadMenu->ForcedSideCount( inItemID );
	inItemInfo.Sides.clear();

	WideString forceSideName = "";

	for( __int32 i = 0; i < sideCount; i++ )
	{
		Menu::TItemSideInfo sideInfo;

		inLoadMenu->ForcedSideAtIndex( i,
		inItemID,
		sideInfo.Key,
		sideInfo.Item_Key,
		forceSideName,
		sideInfo.Group_Number,
		sideInfo.Max_Select,
		sideInfo.Allow_Skip );

		sideInfo.Course_Name     = inCourseName;
		sideInfo.Master_Item_Key = inItemInfo.Key;
		sideInfo.Item_Name =  forceSideName;

		inItemInfo.Sides.push_back( sideInfo );
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadItemForcedOptionsInfo( __int32    inGroupNumber,
__int32    inItemKey,
TLoadMenu *inLoadMenu,
__int32    inItemHandle,
std::vector<TForcedOption> &infOptions )
{
	WideString forcedOptionName = "";
	__int32    forcedOptionPK;
	__int32    optionFK;
	__int32    groupNumber;

	__int32 foCount = inLoadMenu->ForcedOptionCount( inItemHandle );

	infOptions.clear();

	for( int i = 0; i < foCount; i++ )
	{
		inLoadMenu->ForcedOptionAtIndex( i,
		inItemHandle,
		forcedOptionPK,
		optionFK,
		forcedOptionName,
		groupNumber );

		if( groupNumber == inGroupNumber )
		{
			infOptions.push_back( TForcedOption( forcedOptionName,
			forcedOptionPK,
			optionFK,
			inItemKey ) );
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadItemSizeTaxProfilesInfo( TLoadMenu *inLoadMenu,
__int32    inItemSizeID,
Menu::TItemSizeInfo &inItemSizeInfo )
{
	__int32 taxProfileCount = inLoadMenu->TaxProfileCount( inItemSizeID );

	inItemSizeInfo.TaxProfileKeys.clear();

	for( __int32 i = 0; i < taxProfileCount; i++ )
	{
		__int32 tpKey;

		inLoadMenu->ItemSizeTaxProfileAtIndex( i, inItemSizeID, tpKey );

		inItemSizeInfo.TaxProfileKeys.insert( tpKey );
	}
}
//---------------------------------------------------------------------------

bool TfrmMenuEdit::CreateItemSizeNodes( TLoadMenu *inLoadMenu, __int32 inItemID, TTreeNode *inItemNode )
{
	bool result = false;
	try
	{
		__int32 itemSizeCount = inLoadMenu->ItemSizeCount( inItemID );
		std::vector<Menu::TThirdPartyCodeInfo> thirdPartyCodes;  // existing third party codes from xml file
		WideString itemSizeName = "";

		if(itemSizeCount > 0)
		{
			GetThirdPartyCodesListFromFile( &thirdPartyCodes, inLoadMenu );
		}

		for( __int32 i = 0; i < itemSizeCount; i++ )
		{
			Menu::TItemSizeInfo itemSizeInfo;
			bool itemSizeEnabled;

			__int32 itemSizeID = inLoadMenu->ItemSizeAtIndex( i,
			inItemID,
			itemSizeInfo.Key,
			itemSizeInfo.Size_Key,
			itemSizeName,
			itemSizeInfo.Size_Kitchen_Name,
			itemSizeInfo.Free,
			itemSizeInfo.Price,
			itemSizeInfo.MaxRetailPrice,
			itemSizeInfo.Special_Price,
			itemSizeInfo.GST_Percent,
			itemSizeInfo.Cost,
			itemSizeInfo.Cost_GST_Percent,
			itemSizeInfo.Points_Percent,
			itemSizeInfo.Mem_Sale_Count,
			itemSizeInfo.Mem_Discount_Percent,
			itemSizeInfo.Loc_Sale_Count,
			itemSizeInfo.Loc_Discount_Percent,
			itemSizeInfo.No_Recipe,
			itemSizeInfo.IsTaxExempt,
			itemSizeInfo.HasServiceCharge,
			itemSizeInfo.Setmenu_Mask,
			itemSizeInfo.Available_As_Standard,
			itemSizeInfo.Barcode,
			itemSizeEnabled,
			itemSizeInfo.CategoryKey,
			itemSizeInfo.Category,
			itemSizeInfo.ThirdPartyCodes_Key,
			itemSizeInfo.Tare_Weight,
			itemSizeInfo.PLU,
			itemSizeInfo.AvailableQuantity,
			itemSizeInfo.DefaultQuantity,
			itemSizeInfo.WarningQuantity,
			itemSizeInfo.DisableWhenCountReachesZero,
			itemSizeInfo.CanBePaidForUsingPoints,
			itemSizeInfo.DefaultPatronCount,
            itemSizeInfo.PriceForPoints);

			itemSizeInfo.Third_Party_Code = GetThirdPartyCodeFromKeyFromFile(&thirdPartyCodes, itemSizeInfo.ThirdPartyCodes_Key);
			itemSizeInfo.Size_Name = itemSizeName;

			ReadItemSizeBCategoriesInfo( inLoadMenu, itemSizeID, itemSizeInfo );
			ReadItemSizeReceipesInfo(    inLoadMenu, itemSizeID, itemSizeInfo );
            ReadItemSizeTaxProfilesInfo( inLoadMenu, itemSizeID, itemSizeInfo );

            ReadItemSizePriceLevel( inLoadMenu, itemSizeID, itemSizeInfo );

			result = AddMenuSize( inItemNode, &itemSizeInfo );
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
///--------------------------------------------------------
void TfrmMenuEdit::ReadItemSizePriceLevel( TLoadMenu *inLoadMenu, __int32 inItemSizeID, Menu::TItemSizeInfo& inItemSizeInfo )
{
   	__int32 pricelevelCount = inLoadMenu->PriceLevelCount( inItemSizeID );

      inItemSizeInfo.ItemSizePriceLevels.clear();

   //   if(pricelevelCount ==0)
   //   {
          AnsiString priceLevelName = "Price Level ";
          for (int index =1;index<=noOfPriceLevels;index++)
         {
              Menu::TItemSizePriceLevel itemSizePriceLevel;

              itemSizePriceLevel.PriceLevelKey=index;
              itemSizePriceLevel.Price=inItemSizeInfo.Price;
              itemSizePriceLevel.PriceLevelName= priceLevelName + ""+ index;
          inItemSizeInfo.ItemSizePriceLevels.insert ( std::pair<int,Menu::TItemSizePriceLevel> (  itemSizePriceLevel.PriceLevelKey,itemSizePriceLevel ) );
         }

    //  }
}
__int32 TLoadMenu::PriceLevelCount( __int32 inItemSizeHandle )
{
	TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;

	return childCount( loadElement( "PriceLevels", itemSizeElem ) );
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadItemSizeBCategoriesInfo( TLoadMenu *inLoadMenu, __int32 inItemSizeID, Menu::TItemSizeInfo& inItemSizeInfo )
{
	__int32 bcCount = inLoadMenu->BCategoryCount( inItemSizeID );

	inItemSizeInfo.Categories.clear();

	for( __int32 i = 0; i < bcCount; i++ )
	{
		Menu::TSizeCategoryInfo sizeCategoryInfo;

		inLoadMenu->BCategoryAtIndex(
		i,
		inItemSizeID,
		sizeCategoryInfo.Key,
		sizeCategoryInfo.Category );

		inItemSizeInfo.Categories.push_back( sizeCategoryInfo );
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::ReadItemSizeReceipesInfo( TLoadMenu *inLoadMenu, __int32 inItemSizeID, Menu::TItemSizeInfo& inItemSizeInfo )
{
	__int32 srCount = inLoadMenu->ItemSizeReceipeCount( inItemSizeID );

	inItemSizeInfo.Recipes.clear();

	for( __int32 i = 0; i < srCount; i++ )
	{
		Menu::TSizeRecipeInfo sizeRecipeInfo;

		inLoadMenu->ItemSizeReceipeAtIndex(
		i,
		inItemSizeID,
		sizeRecipeInfo.Key,
		sizeRecipeInfo.Stock_Code,
		sizeRecipeInfo.Qty,
		sizeRecipeInfo.Stock_Location );

		inItemSizeInfo.Recipes.push_back( sizeRecipeInfo );
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::StartStockTransaction()
{
	if( dmStockData->dbStock->Connected && !StockTransaction->InTransaction )
	{
		StockTransaction->StartTransaction();
	}
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::
//   Save Menu to a XML file
//::::::::::::::::::::::::::::::::::::::::::

void TfrmMenuEdit::SaveMenu( AnsiString inFileName, AnsiString inBackupFileName )
{
	try
	{
		// Menu Data
		TTreeNode *menuTreeNode = tvMenu->Items->GetFirstNode();
		TMenuNode *menuData     = (TMenuNode *)menuTreeNode->Data;

		//........................................

		AnsiString menuVersion = "6.00";

		std::auto_ptr<TSaveMenu> saveMenuAutoPtr(new TSaveMenu(menuData->Key, menuVersion, inFileName, inBackupFileName, *new_menu_element_key_generator_) );
		TSaveMenu *saveMenu = saveMenuAutoPtr.get();

		//........................................

		saveMenu->MenuName        = menuData->LongDescription;
		saveMenu->MenuType        = rbFoodMenu->Checked ? Menu::mtFoodMenu : Menu::mtBeverageMenu;
		saveMenu->AvailableOnPalm = chbAvailableOnPalm->Checked;

		//........................................

		// Category Groups
		menuTreeNode = tvMenu->Items->GetFirstNode()->Item[CATEGORY_GROUPS_INDEX];
		SaveMenuCategoryGroups( saveMenu, menuTreeNode );
        //saveMenu->Commit();
		// Sizes
		menuTreeNode = tvMenu->Items->GetFirstNode()->Item[SIZES_INDEX];
		SaveMenuSizes( saveMenu, menuTreeNode );
        //saveMenu->Commit();

		// Serving Courses
		menuTreeNode = tvMenu->Items->GetFirstNode()->Item[SERVING_COURSES_INDEX];
		SaveMenuServingCourses( saveMenu, menuTreeNode );
        //saveMenu->Commit();

        // 3rd Party Groups
		menuTreeNode = tvMenu->Items->GetFirstNode();
		SaveMenuThirdPartyGroups( saveMenu, menuTreeNode );
        //saveMenu->Commit();

		// Tax Profiles
		std::set<TaxProfile*> taxProfiles;
		menuTaxProfileProvider->ReadAllTaxProfiles( taxProfiles );
		SaveMenuTaxProfiles( saveMenu, &taxProfiles );
        //saveMenu->Commit();
        // Courses
		menuTreeNode = tvMenu->Items->GetFirstNode();
		SaveMenuCourses( saveMenu, menuTreeNode );
		//:::::::::::::::::::::::::::::::::::::::
		saveMenu->Commit();

		Application->MessageBox( "Menu Saved", "Save", MB_OK + MB_ICONINFORMATION );
	}
	catch( Exception &exc )
	{
		AnsiString errorMsg = "Failed to save the Menu. " + exc.Message;
		Application->MessageBox( errorMsg.c_str(), "Error", MB_OK + MB_ICONERROR );
	}

	MenuEdited = false;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuCategoryGroups( TSaveMenu* inSaveMenu, TTreeNode* inCategoryGroupsNode )
{
	for( int i=0; i < inCategoryGroupsNode->Count; i++ )
	{
		TTreeNode          *categoryGroupNode = inCategoryGroupsNode->Item[i];
		TCategoryGroupNode *categoryGroupData = ( TCategoryGroupNode * )categoryGroupNode->Data;

		__int32 categoryGroupID = inSaveMenu->SaveCategoryGroup( categoryGroupData->Key,
		UTF8Encode( categoryGroupData->LongDescription ),
		categoryGroupData->GetDeleted() );
		for( int j=0; j < categoryGroupNode->Count; j++ )
		{
			TCategoryNode *categoryData = ( TCategoryNode * )categoryGroupNode->Item[j]->Data;

			inSaveMenu->SaveCategory( categoryGroupID,
			categoryData->Key,
			UTF8Encode( categoryData->LongDescription ),
            UTF8Encode( categoryData->GLCode.Trim() ),
			categoryData->GetDeleted()
			);
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuSizes( TSaveMenu* inSaveMenu, TTreeNode* inSizesTreeNode )
{

    /*for(std::map<AnsiString,  TAvailableMenuSize>::iterator it = AllSizesForMenu.begin(); it != AllSizesForMenu.end(); ++it)
    {
        inSaveMenu->SaveSize( it->second.Key,
		UTF8Encode( it->first ),
		UTF8Encode( it->second.KitchenName  ),
		UTF8Encode( it->second.HandheldName ),
		UTF8Encode( it->second.ReceiptName  ),
		it->second.Weighed,
		it->second.Size_ID,
		it->second.PalmID
		);
    } */
	for( int i = 0; i < inSizesTreeNode->Count; i++ )
	{
		TSizeNode *SizeData = ( TSizeNode * )inSizesTreeNode->Item[i]->Data;

		inSaveMenu->SaveSize( SizeData->Key,
		UTF8Encode( SizeData->LongDescription ),
		UTF8Encode( SizeData->KitchenName  ),
		UTF8Encode( SizeData->HandheldName ),
		UTF8Encode( SizeData->ReceiptName  ),
		SizeData->Weighed,
		SizeData->Size_ID,
		SizeData->PalmID
		);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuServingCourses( TSaveMenu* inSaveMenu, TTreeNode* inServingCoursesTreeNode )
{
    //ServingCoursesList
    /*for( int i = 0; i < ServingCoursesList->Count; i+=7 )
    {

        /*bool isEnabled = false;
        bool isdeleted = false;
        StrToBoolean(ServingCoursesList->Strings[i + 2])

        if( == true)
        {
           isEnabled = true;
        }
        if(ServingCoursesList->Strings[i + 3] == true)
        {
           isdeleted = true;
        }
 		inSaveMenu->SaveServingCourse
        (
          StrToInt(ServingCoursesList->Strings[i + 6]),
		  UTF8Encode( ServingCoursesList->Strings[i] ),
		  UTF8Encode( ServingCoursesList->Strings[i + 1] ),
		  StrToBool(ServingCoursesList->Strings[i + 2]),
		  StrToBool(ServingCoursesList->Strings[i + 3]),
		  StrToBool(ServingCoursesList->Strings[i + 4]),
		  StringToColor(ServingCoursesList->Strings[i + 5]),
         i );

    }*/

	   /*	TServingCourseNode *ServingCourseData = (TServingCourseNode *)ServingCoursesNode->Item[i]->Data;
		AllServingCourses->Add(ServingCourseData->LongDescription);
		AllServingCourses->Add(UTF8Encode(ServingCourseData->KitchenName));
		switch (ServingCourseData->Enabled)
		{
		case true  : AllServingCourses->Add("T"); break;
		case false : AllServingCourses->Add("F"); break;
		}
		switch (ServingCourseData->Deleted)
		{
		case true  : AllServingCourses->Add("T"); break;
		case false : AllServingCourses->Add("F"); break;
		}
		switch (ServingCourseData->Selectable)
		{
		case true  : AllServingCourses->Add("T"); break;
		case false : AllServingCourses->Add("F"); break;
		}
		AllServingCourses->Add(IntToStr((int)ServingCourseData->Colour));
        AllServingCourses->Add(IntToStr((int)ServingCourseData->Key));  */

	for( int i = 0; i < inServingCoursesTreeNode->Count; i++ )
	{
		TServingCourseNode *servingCourseData = ( TServingCourseNode * )inServingCoursesTreeNode->Item[i]->Data;
		servingCourseData->DisplayOrder = i;

		inSaveMenu->SaveServingCourse( servingCourseData->Key,
		UTF8Encode( servingCourseData->LongDescription ),
		UTF8Encode( servingCourseData->KitchenName ),
		servingCourseData->Enabled,
		servingCourseData->Deleted,
		servingCourseData->Selectable,
		servingCourseData->Colour,
		servingCourseData->DisplayOrder
		);
	}

	// Deleted Serving Courses that need to be notified to the POS
	for (unsigned i=0; i<DeletedServingCoursesInfo.DeletedServingCourseVector.size(); i++)
	{
		inSaveMenu->SaveServingCourse( DeletedServingCoursesInfo.DeletedServingCourseVector[i].Key,
		DeletedServingCoursesInfo.DeletedServingCourseVector[i].ServingCourse_Name,
		DeletedServingCoursesInfo.DeletedServingCourseVector[i].ServingCourse_Kitchen_Name,
		false,   // Enabled
		true,    // Deleted
		false,   // Selectable
		clBlack,  // Color
		-1
		);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuThirdPartyGroups( TSaveMenu* inSaveMenu, TTreeNode* inMenuNode )
{
	Menu::TMenuLoadDB MenuLoader( dmMMData->dbMenuMate );
	std::vector<Menu::TThirdPartyCodeInfo> thirdPartyCodes;
	TfrmMenuEdit::GetAllThirdPartyGroupsFromTree( thirdPartyCodes );

	std::vector<Menu::TThirdPartyCodeInfo>::iterator tpcIT;

	for( tpcIT = thirdPartyCodes.begin(); tpcIT != thirdPartyCodes.end(); tpcIT++ )
	{
		Menu::TThirdPartyCodeInfo tpcInfo = *tpcIT;

		inSaveMenu->SaveThirdPartyCode( tpcIT->Key,
		tpcIT->Code,
		tpcIT->Description,
		tpcIT->Visible,
		tpcIT->Code_Type );
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::GetAllThirdPartyGroupsFromTree(std::vector<Menu::TThirdPartyCodeInfo> &thirdPartyCodes)
{
	TTreeNode *MenuNode = tvMenu->Items->GetFirstNode();
	thirdPartyCodes.clear();
	for (int i = FIRST_COURSE_INDEX; i<MenuNode->Count; i++)
	{
		TTreeNode *CourseNode = MenuNode->Item[i];
		TCourseNode *CourseData = (TCourseNode *)CourseNode->Data;
		if (CourseData->NodeType == COURSE_NODE)
		{
			for (int j=0; j<CourseNode->Count; j++)
			{
				TTreeNode *ItemNode = CourseNode->Item[j];
				TItemNode *ItemData = (TItemNode *)ItemNode->Data;
				if (ItemData->NodeType == ITEM_NODE)
				{
					for (int k=0; k<ItemNode->Count; k++)
					{
						TTreeNode *ItemSizeNode = ItemNode->Item[k];
						TItemSizeNode *ItemSizeData = (TItemSizeNode *)ItemSizeNode->Data;
						if (ItemSizeData->NodeType == ITEM_SIZE_NODE)
						{
							if(ItemSizeData->ThirdPartyCode != "")
							{
								Menu::TThirdPartyCodeInfo tpcInfo;
								tpcInfo.Key = ItemSizeData->ThirdPartyCodeKey;
								tpcInfo.Code = ItemSizeData->ThirdPartyCode;
								tpcInfo.Visible = 1;
								tpcInfo.Code_Type = 0;
								thirdPartyCodes.push_back(tpcInfo);
							}
						}
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuTaxProfiles( TSaveMenu* inSaveMenu, std::set<TaxProfile*>* const inTaxProfilesList )
{
	std::set<TaxProfile*>::iterator taxProfileIT;

	for( taxProfileIT = inTaxProfilesList->begin(); taxProfileIT != inTaxProfilesList->end(); taxProfileIT++ )
	{
		inSaveMenu->SaveTaxProfile( ( *taxProfileIT )->GetTaxProfileDBKey(),
		( *taxProfileIT )->taxProfileName,
		( *taxProfileIT )->taxPercentage,
		( *taxProfileIT )->taxProfileType,
		( *taxProfileIT )->taxPriority );
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuCourses( TSaveMenu* inSaveMenu, TTreeNode* inMenuNode )
{
	for( int i = FIRST_COURSE_INDEX; i<inMenuNode->Count; i++ )
	{
		TTreeNode   *courseNode = inMenuNode->Item[i];
		TCourseNode *courseData = (TCourseNode *)courseNode->Data;

		__int32 courseID = SaveMenuCourse( inSaveMenu, courseData );

		SaveMenuCourseItems( inSaveMenu, courseID, courseNode );
	}
}
//---------------------------------------------------------------------------
__int32 TfrmMenuEdit::SaveMenuCourse( TSaveMenu* inSaveMenu, TCourseNode *inCourseData )
{
	return inSaveMenu->SaveMenuCourse( inCourseData->Key,
	UTF8Encode( inCourseData->LongDescription ),
	UTF8Encode( inCourseData->KitchenName ),
	UTF8Encode( inCourseData->HandheldName ),
	UTF8Encode( inCourseData->ReceiptName ),
	inCourseData->Location,
	inCourseData->Enabled,
	inCourseData->ServingCourseKey,
	UTF8Encode( inCourseData->ServingCourse ),
	inCourseData->NoDefaultServingCourse
	);
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuCourseItems( TSaveMenu* inSaveMenu, __int32 inCourseID, TTreeNode *inCourseNode )
{
	for( int j = 0; j < inCourseNode->Count; j++ )
	{
		TTreeNode *itemNode = inCourseNode->Item[j];
		TItemNode *itemData = (TItemNode *)itemNode->Data;

		if( ( ( TEditorNode * )itemNode->Data )->NodeType == ITEM_NODE)
		{
			__int32 itemID = SaveMenuCourseItem( inSaveMenu, inCourseID, itemData );

			SaveMenuItemForcedSides(   inSaveMenu, itemID, itemData );
			SaveMenuItemForcedOptions( inSaveMenu, itemID, itemData );
			SaveMenuItemSizes(         inSaveMenu, itemID, itemNode );
		}
		else
		if( ( ( TEditorNode * )itemNode->Data )->NodeType == OPTIONS_NODE )
		{
			SaveMenuItemOptions( inSaveMenu, inCourseID, itemNode );
		}
	}
}
//---------------------------------------------------------------------------
__int32 TfrmMenuEdit::SaveMenuCourseItem( TSaveMenu* inSaveMenu, __int32 inCourseID, TItemNode *inItemData )
{
	__int32 result = inSaveMenu->SaveItem( inCourseID,
	inItemData->Key,
	UTF8Encode( inItemData->LongDescription ),
	UTF8Encode( inItemData->KitchenName ),
	UTF8Encode( inItemData->HandheldName ),
	UTF8Encode( inItemData->ReceiptName ),
	inItemData->Colour,
	"True",
	inItemData->Enabled,
	inItemData->ItemOnlySide,
	inItemData->PrinterOptions.PrintUnderlined,
	inItemData->PrinterOptions.PrintBold,
	inItemData->PrinterOptions.PrintColour,
	inItemData->PrinterOptions.PrintFont,
	inItemData->PrinterOptions.PrintDoubleWidth,
	inItemData->PrinterOptions.PrintDoubleHeight
	);


	return result;
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuItemForcedSides( TSaveMenu* inSaveMenu, __int32 inItemID, TItemNode* inItemData )
{
	for( int i = 0; i < inItemData->ForcedSides->Count; i++ )
	{
		TForcedSide *forcedSide = (TForcedSide *)inItemData->ForcedSides->Items[i];
		if( forcedSide->ItemNode )
		{
			inSaveMenu->SaveForcedSide( inItemID,
			forcedSide->Key,
			forcedSide->ItemKey,
			UTF8Encode(forcedSide->ItemNode->LongDescription),
			forcedSide->GroupNumber,
			forcedSide->MaxSelect,
			forcedSide->SideGroupSkip
			);
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuItemForcedOptions( TSaveMenu* inSaveMenu, __int32 inItemHandle, TItemNode* inItemData )
{
	std::map<int, std::map<AnsiString, TForcedOption> >::const_iterator grpIT = \
	inItemData->ForcedOptions.begin();
	std::map<int, std::map<AnsiString, TForcedOption> >::const_iterator grpEnd = \
	inItemData->ForcedOptions.end();

	for ( ; grpIT != grpEnd; ++grpIT )
	{
		std::map<AnsiString, TForcedOption>::const_iterator foIT  = grpIT->second.begin();
		std::map<AnsiString, TForcedOption>::const_iterator foEnd = grpIT->second.end();

		for ( ; foIT != foEnd; ++foIT )
		{
			inSaveMenu->SaveMenuItemForcedOption(
			inItemHandle,
			foIT->second.ForcedOptionPK,   // Forced Option's Primary Key
			foIT->second.OptionFK,         // Option's Foreign Key
			UTF8Encode(foIT->first),                   // Option's Description
			grpIT->first );                // Group Number
		}
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuItemSizes( TSaveMenu* inSaveMenu, __int32 inItemID, TTreeNode *inItemNode )
{
	for( int i=0; i < inItemNode->Count; i++ )
	{
		TTreeNode *itemSizeNode = inItemNode->Item[i];
		TItemSizeNode   *dcData = (TItemSizeNode *)itemSizeNode->Data;
		dcData->KitchenName =  GetKitchenNameOfSize(dcData->LongDescription);
		// check for PLU code. if it is zero, allocate a new one
		if(dcData->PLU == 0)
		{
			dcData->PLU = (*new_menu_element_key_generator_)();
		}

		__int32 itemSizeID = SaveMenuItemSize( inSaveMenu, inItemID, dcData );

        SaveMenuItemSizePriceLevels( inSaveMenu, itemSizeID, dcData );
		SaveMenuBreakdownCategories( inSaveMenu, itemSizeID, dcData );
		SaveMenuItemSizeRecipes(     inSaveMenu, itemSizeID, dcData );
		SaveMenuItemSizeTaxProfiles( inSaveMenu, itemSizeID, dcData );

	}
}
//---------------------------------------------------------------------------
__int32 TfrmMenuEdit::SaveMenuItemSize( TSaveMenu* inSaveMenu, __int32 inItemID, TItemSizeNode* inDCData )
{
	bool tmp = ( !inDCData->SetMenuMask || TST_PROMO_MASTER( inDCData->SetMenuMask ) ) ?
	true : inDCData->SetMenuStandardItem;

	return inSaveMenu->SaveItemSize( inItemID,
	inDCData->Key,
	inDCData->SizeKey,
	UTF8Encode(inDCData->LongDescription),
	//MM1351 This suppose to be the kithcen name
	inDCData->KitchenName,
	inDCData->Free,
	inDCData->Price,
	inDCData->MaxRetailPrice,
	inDCData->SpecialPrice,
	inDCData->GST,
	fabs( inDCData->Cost ),
	inDCData->CostGST,
	inDCData->PointsPercent,

	inDCData->MemberPurchaseCount,
	inDCData->MemberPurchaseDiscount,
	inDCData->LocationPurchaseCount,
	inDCData->LocationPurchaseDiscount,
	false,  // NoRecipe
	inDCData->IsTaxExempt,
	inDCData->HasServiceCharge,
	inDCData->SetMenuMask,
	tmp,    // item's visibility on the pos
	inDCData->Barcode,
	inDCData->Enabled,
	inDCData->CategoryKey,
	UTF8Encode(inDCData->Category),
	inDCData->ThirdPartyCodeKey,
	inDCData->TareWeight,
	inDCData->PLU,
	inDCData->AvailableQuantity,
	inDCData->DefaultQuantity,
	inDCData->WarningQuantity,
	inDCData->DisableWhenCountReachesZero,
	inDCData->CanBePaidForUsingPoints,
	inDCData->DefaultPatronCount,
	inDCData->CostForPoints);
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuBreakdownCategories( TSaveMenu* inSaveMenu, __int32 inItemSizeID, TItemSizeNode* inDCData )
{
	std::vector<Menu::TNameAndKey>::iterator nameAndKeyIT;
	for( nameAndKeyIT = inDCData->BCategoryList.begin(); nameAndKeyIT != inDCData->BCategoryList.end(); nameAndKeyIT++ )
	{
		Menu::TNameAndKey *nameAndKey = nameAndKeyIT;
		inSaveMenu->SaveBCategory( inItemSizeID, nameAndKey->Key, nameAndKey->Name );
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuItemSizeRecipes( TSaveMenu* inSaveMenu, __int32 inItemSizeID, TItemSizeNode* inDCData )
{
	for( int i = 0; i < inDCData->RecipeList->Count; i++)
	{
		TRecipe *recipe = ( TRecipe * )inDCData->RecipeList->Items[i];

		inSaveMenu->SaveItemSizeRecipe( inItemSizeID,
		recipe->Key,
		recipe->StockCode,
		recipe->Quantity,
		recipe->Location
		);
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuItemSizeTaxProfiles( TSaveMenu* inSaveMenu, __int32 inItemSizeID, TItemSizeNode* inDCData )
{
	std::set<TaxProfile*> taxProfilesApplied;
	std::set<TaxProfile*>::iterator taxProfileIT;

	menuTaxProfileProvider->ReadTaxProfilesWithKeys( inDCData->TaxProfileKeys, taxProfilesApplied );

	for( taxProfileIT = taxProfilesApplied.begin(); taxProfileIT != taxProfilesApplied.end(); taxProfileIT++ )
	{
		inSaveMenu->SaveItemSizeTaxProfile( inItemSizeID,
		( *taxProfileIT )->GetTaxProfileDBKey(),
		( *taxProfileIT )->taxProfileName );
	}
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SaveMenuItemOptions( TSaveMenu* inSaveMenu, __int32 inCourseID, TTreeNode* inItemNode )
{
	for (int k=0; k<inItemNode->Count; k++)	// Actually Options Node
	{
		TTreeNode   *optionNode = inItemNode->Item[k];
		TOptionNode *optionData = (TOptionNode *)optionNode->Data;
		optionData->CondimentsMask = 0;

		__int32 flags =
		inSaveMenu->AssembleOptionMask(optionData->GroupNumber,
		optionData->MaxSelect,
		optionData->PlusOption,
		optionData->MinusOption,
		optionData->OptionGroupSkip);

		inSaveMenu->SaveOption( inCourseID,
		optionData->Key,
		UTF8Encode(optionData->LongDescription),
		UTF8Encode(optionData->KitchenName),
		UTF8Encode(optionData->HandheldName),
		UTF8Encode(optionData->ReceiptName),
		optionData->CondimentsMask,
		flags,
		optionData->Enabled,
		optionData->PrinterOptions.PrintUnderlined,
		optionData->PrinterOptions.PrintBold,
		optionData->PrinterOptions.PrintColour,
		optionData->PrinterOptions.PrintFont,
		optionData->PrinterOptions.PrintDoubleWidth,
		optionData->PrinterOptions.PrintDoubleHeight
		);
	}
}
//---------------------------------------------------------------------------

void __fastcall
TfrmMenuEdit::cbCanBePaidForUsingPointsClicked(
TObject *)
{
	TItemSizeNode *data =
	reinterpret_cast<TItemSizeNode *>(tvMenu->Selected->Data);

	data->CanBePaidForUsingPoints = cbCanBePaidForUsingPoints->Checked;
    if(cbCanBePaidForUsingPoints->Checked)
    {
       nePriceForPoint->Enabled = true;
    }
    else
    {
       nePriceForPoint->Enabled = false;
       nePriceForPoint->Value = 0.0;
    }
	MenuEdited = true;
}
//---------------------------------------------------------------------------

void __fastcall
TfrmMenuEdit::OnLeaveDefaultPatronCount(
TObject *)
{
	TItemSizeNode &size_node =
	*reinterpret_cast<TItemSizeNode *>(tvMenu->Selected->Data);
	int default_patron_count =
	StrToIntDef(edDefaultPatronCount->Text, -1);

	if (default_patron_count < 0) {
		Application->MessageBox("You must enter a positive integer.",
		"Bad patron count!",
		MB_ICONERROR | MB_OK);
		edDefaultPatronCount->Text =
		IntToStr(size_node.DefaultPatronCount);
		return;
	}

	MenuEdited = size_node.DefaultPatronCount != default_patron_count;
	size_node.DefaultPatronCount = default_patron_count;
}


void __fastcall
TfrmMenuEdit::SyncDefaultPatronCountClicked(
TObject *)
{
	std::vector<TTreeNode *> item_size_nodes =
	get_all_item_sizes_in_course(*get_current_course());
	int default_patron_count =
	StrToInt(edDefaultPatronCount->Text);

	for (std::vector<TTreeNode *>::iterator i = item_size_nodes.begin();
	i != item_size_nodes.end(); ++i) {
		TItemSizeNode &size_node =
		*reinterpret_cast<TItemSizeNode *>((*i)->Data);

		size_node.DefaultPatronCount = default_patron_count;
		MenuEdited |= size_node.DefaultPatronCount != default_patron_count;
	}
}
//---------------------------------------------------------------------------

// Dummy test of transferring TaxProfiles when add or remove is clicked

void __fastcall TfrmMenuEdit::btnAddTPMouseClick(TObject *Sender)
{
	menuTaxProfileAssigner->Transfer( lbUnassigned );

	/*   testTax->Transfer(lbUnassigned);     */
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::btnRemoveTPMouseClick(TObject *Sender)
{
	menuTaxProfileAssigner->Transfer( lbAssigned );

	/*   testTax->Transfer(lbAssigned);        */
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::lbUnassignedDblClick(TObject *Sender)
{
	menuTaxProfileAssigner->Transfer( lbUnassigned );
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::lbAssignedDblClick(TObject *Sender)
{
	menuTaxProfileAssigner->Transfer( lbAssigned );
}
//---------------------------------------------------------------------------


void __fastcall TfrmMenuEdit::btnSyncTPClick(TObject *Sender)
{
	TTreeNode *CurrentTreeNode = tvMenu->Selected;
	TStrings* taxProfileContainer = lbAssigned->Items;
	if (((TEditorNode *)CurrentTreeNode->Data)->NodeType == ITEM_SIZE_NODE)
	{
		if (Application->MessageBox("This will set the Tax Profile for all the other sizes in this course.\rDo you wish to continue?", "Warning",
					MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == IDCANCEL)
		{
			return;
		}
		for (int j=0; j<CurrentTreeNode->Parent->Parent->Count; j++)
		{
			TEditorNode *CurrentItemNode = ((TEditorNode *)CurrentTreeNode->Parent->Parent->Item[j]->Data);
			if (CurrentItemNode->NodeType == ITEM_NODE)
			{
				for (int i=0; i<CurrentItemNode->Owner->Count; i++)
				{
					TItemSizeNode *CurrentSizeNode = ((TItemSizeNode *)CurrentItemNode->Owner->Item[i]->Data);
					if (CurrentSizeNode != tvMenu->Selected->Data)
					{
						CurrentSizeNode->TaxProfileKeys.clear();
						for( int i = 0; i < taxProfileContainer->Count; i++ )
						{
							TaxProfile* tp = ( TaxProfile* )taxProfileContainer->Objects[i];
							CurrentSizeNode->TaxProfileKeys.insert( tp->GetTaxProfileDBKey() );
						}
					}
				}
			}
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::edCostGSTExit(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		TItemSizeNode *ItemSizeData = (TItemSizeNode *)CurrentNodeData;

		if (ItemSizeData->CostGST != edCostGST->Value)
		{
			ItemSizeData->CostGST = edCostGST->Value;
			MenuEdited = true;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::edGlCodeChange(TObject *Sender)
{
	MenuEdited = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::edGlCodeEnter(TObject *Sender)
{
	((TEdit *)Sender)->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::edGlCodeExit(TObject *Sender)
{
	TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
	if (CurrentNodeData->NodeType == CATEGORY_NODE)
	{
		TCategoryNode *CategoryData = (TCategoryNode *)CurrentNodeData;
		if (edGlCode->Text != CategoryData->GLCode)
		{
			CategoryData->GLCode = edGlCode->Text;
			MenuEdited = true;
		}
	}
}


 void TfrmMenuEdit::CreateXmlFromCsvFile(std::vector<TimportMenu> allCsvMenus,bool itemSizeUniqueCase)
  {
      	AnsiString menuVersion = "6.00";
        AnsiString  FilePath=CurrentConnection.ServerPath + "\\Menu Import";
        if (!DirectoryExists(FilePath))
		{
			if (!CreateDir(FilePath))
			{
				Application->MessageBox(("Cannot create directory " + FilePath).c_str(), "MenuMate", MB_OK + MB_ICONERROR);
				return;
			}
		}

      for (int menuNo=0;menuNo< allCsvMenus.size();menuNo++)
      {

       //........................................
	   	std::auto_ptr<TSaveMenu> saveMenuAutoPtr(new TSaveMenu(allCsvMenus[menuNo].menuKey, menuVersion,FilePath + "\\" +allCsvMenus[menuNo].menuName + ".xml", allCsvMenus[menuNo].menuName, 0) );

	  	TSaveMenu *saveMenu = saveMenuAutoPtr.get();
		saveMenu->MenuName        = allCsvMenus[menuNo].menuName;
	 	saveMenu->MenuType        =  allCsvMenus[menuNo].menuType;
		saveMenu->AvailableOnPalm = 0;

		//........................................

		// Category Groups
	   	CsvSaveMenuCategoryGroups( saveMenu, allCsvMenus[menuNo] );

	 	// Sizes
	 	CsvSaveMenuSizes( saveMenu, allCsvMenus[menuNo] );

	   // Serving Courses
        CsvSaveMenuServingCourses( saveMenu, allCsvMenus[menuNo] );

		// Tax Profiles
   		CsvSaveMenuTaxProfiles( saveMenu, allCsvMenus[menuNo] );

		// Courses
         CsvSaveMenuCourses( saveMenu, allCsvMenus[menuNo],itemSizeUniqueCase );

		//:::::::::::::::::::::::::::::::::::::::

		saveMenu->Commit();
     }
     	Application->MessageBox( "Menu Saved", "Save", MB_OK + MB_ICONINFORMATION );

  }


  void TfrmMenuEdit::CsvSaveMenuCategoryGroups( TSaveMenu *inSaveMenu, TimportMenu menu )
  {
     for( int groupNo=0; groupNo < menu.allCategoryGroups.size(); groupNo++ )
	{

		__int32 categoryGroupID = inSaveMenu->SaveCategoryGroup(  menu.allCategoryGroups[groupNo].categoryGroupKey,
		                                                          UTF8Encode( menu.allCategoryGroups[groupNo].categoryGroupName ),
                                                                  "T" );

	  	for( int categoryNo=0; categoryNo < menu.allCategories.size(); categoryNo++ )
		{
            if ( menu.allCategoryGroups[groupNo].categoryGroupKey ==  menu.allCategories[categoryNo].categoryGroupKey)
            {
                inSaveMenu->SaveCategory( categoryGroupID,                           //group key
                                          menu.allCategories[categoryNo].categoryKey,//category key
                                          menu.allCategories[categoryNo].categoryName,
                                          "", //GL code
                                          0    //isdeleted
                                        );

            }

		}

	}

  }



  void TfrmMenuEdit::CsvSaveMenuSizes( TSaveMenu* inSaveMenu, TimportMenu menu )
  {
       for( int sizeNo = 0; sizeNo < menu.allMenuSize.size(); sizeNo++ )
        {

            inSaveMenu->SaveSize( menu.allMenuSize[sizeNo].sizeKey,  //sizeKey
            UTF8Encode( menu.allMenuSize[sizeNo].sizeName ),      //size Name
            UTF8Encode( " "  ),    // Kitchcen Name
            UTF8Encode( " "),      //Handheld Name
            UTF8Encode( " "  ),    //Recipe Name
            false,                // weight
            0,    //sizeID
            0
            );
        }


  }

 void  TfrmMenuEdit::CsvSaveMenuServingCourses( TSaveMenu* inSaveMenu, TimportMenu menu )
  {
     for( int courseNo = 0; courseNo < menu.allServingCourse.size(); courseNo++ )
    	{

            inSaveMenu->SaveServingCourse( menu.allServingCourse[courseNo].servingCourseKey,    // serving course key
            UTF8Encode( menu.allServingCourse[courseNo].servingCourseName ),
            UTF8Encode( " " ),
            1,  //enabled
            0,  //deleted
            1,  //selectable
            51200, //color
            courseNo     //display order
            );
    	}


    }

  void TfrmMenuEdit::CsvSaveMenuTaxProfiles( TSaveMenu* inSaveMenu, TimportMenu menu )
  {
     
   for ( int taxProfile=0; taxProfile< menu.allTaxProfiles.size() ;taxProfile++ )
	{
	  	inSaveMenu->SaveTaxProfile( (__int32)menu.allTaxProfiles[taxProfile].taxProfileKey,
		menu.allTaxProfiles[taxProfile].taxName,    // taxname
		menu.allTaxProfiles[taxProfile].taxPercent, //tax percent
		menu.allTaxProfiles[taxProfile].taxType,  // type
		0 ); //Prioroty
	}


   }


 void TfrmMenuEdit::CsvSaveMenuCourses( TSaveMenu* inSaveMenu, TimportMenu menu,bool itemSizeUniqueCase )
   {
        // loop for saving courses
        for( int courseNo = 0; courseNo < menu.course.size(); courseNo++ )
        {


            __int32 xmlCourseID = CsvSaveMenuSingleCourse( inSaveMenu, menu.course[courseNo] );

              // loop for saving Items
             for( int itemNo = 0; itemNo < menu.allItem.size(); itemNo++ )
             {

                    if(menu.allItem[itemNo].courseKey == menu.course[courseNo].courseKey)
                    {
                         __int32 xmlItemID = CsvSaveMenuCourseItem( inSaveMenu,xmlCourseID ,menu.allItem[itemNo] );  //save the items


                          //loop for saving  item size
                         for( int itemSizeNo = 0; itemSizeNo < menu.allItemSizeInMenu.size(); itemSizeNo++ )
                          {

                              int itemSizeCourseKey=menu.allItemSizeInMenu[itemSizeNo].courseKey;
                              int itemKeyOfSize=menu.allItemSizeInMenu[itemSizeNo].itemKey;

                             if ( (itemSizeCourseKey == menu.course[courseNo].courseKey) && (itemKeyOfSize == menu.allItem[itemNo].itemKey ) )
                             {
                                  CsvSaveMenuItemSizes(inSaveMenu, xmlItemID, menu.allItemSizeInMenu[itemSizeNo],itemSizeUniqueCase);
                             }

                          }


                    }
              }
          }
  }

 __int32  TfrmMenuEdit::CsvSaveMenuSingleCourse(TSaveMenu* inSaveMenu,Tcourse  course )
   {
      return inSaveMenu->SaveMenuCourse((__int32)course.courseKey   ,   //key
        UTF8Encode( course.courseName),   //name
        UTF8Encode( " " ),        //kitvhen
        UTF8Encode( " " ),        //handheld
        UTF8Encode( " "  ),  //receipt name
        (__int32)65535,
        1,                      //enabled
        (__int32)course.corrospondingServingCourseKey ,  //serving course Key
       // course.corrospondingServingCourseName , //serving Course Name
          " " , //serving Course Name
           0 //  default serving course
        );

   }



   __int32 TfrmMenuEdit::CsvSaveMenuCourseItem( TSaveMenu* inSaveMenu, __int32 xmlCourseID ,Titem item )
   {
        __int32 result = inSaveMenu->SaveItem(xmlCourseID,
            (__int32)item.itemKey,
            UTF8Encode( item.itemName ),
            UTF8Encode( " "),   //Kitchen name
            UTF8Encode( " " ),     // handheld name
            UTF8Encode( " " ),     // receipt name
            clRed,   //color
            "True",   //display sizes
            1,       //is enabled
            0,       //itemonly side
            0,  //printer underlined
            0,  // is print Bold
            clRed,
            0,
            0,   //double width
            0    //double height
            );


        return result;

   }

    void  TfrmMenuEdit::CsvSaveMenuItemSizes(TSaveMenu* inSaveMenu,__int32 xmlItemID, TitemSize itemSize ,bool itemSizeUniqueCase)
    {
        double productCountDown=false;
       if(itemSizeUniqueCase)
       {
         productCountDown =1;
       }
       else
       {
         productCountDown  =-1;
       }

    __int32  xmlitemSizeID = inSaveMenu->SaveItemSize( xmlItemID,
                            itemSize.itemSizeKey,
                            itemSize.sizeFKey,
                            UTF8Encode(itemSize.itemSizeName),
                            //MM1351 This suppose to be the kithcen name
                            " ",
                            0,    //free
                            itemSize.itemSizePrice, //price
                            itemSize.itemSizePrice,  //maxretail price
                            itemSize.itemSizePrice,  //special price
                            15,          //GST
                            0,    //COST
                            15,
                            0,    //POINT PERCENT

                            0,   //MemberPurchaseCount
                            0,//MemberPurchaseDiscount
                            0,  //LocationPurchaseCount
                            0,   //LocationPurchaseDiscount
                            false,  // NoRecipe
                            0,
                            1,
                            0, //SetMenuMask
                            1,    // item's visibility on the pos
                            " ",   //BARCODE
                            1, //Enabled
                            itemSize.categoryKey, //CATEGORY KEY
                            UTF8Encode(itemSize.categoryName), //categoryName
                            0,   //thirdPartyCodesKey
                            0,  //TareWeight
                            0,//plu
                            productCountDown, //AvailableQuantity
                            -1,   //DefaultQuantity
                            0,  //WarningQuantity
                            0,   //DisableWhenCountReachesZero
                            0, //CanBePaidForUsingPoints
                            0,  //DefaultPatronCount
                            itemSize.itemSizePrice // Price for points
                            );

                    //save the item recipe
                     CsvSaveMenuItemSizeRecipes(     inSaveMenu, xmlitemSizeID, itemSize );

                     //save the tax profile
                     CsvSaveMenuItemSizeTaxProfiles( inSaveMenu, xmlitemSizeID, itemSize );

                   //save the pricelevels
                   CsvSaveMenuItemSizePriceLevels( inSaveMenu, xmlitemSizeID, itemSize );
    }

void TfrmMenuEdit::CsvSaveMenuItemSizeTaxProfiles( TSaveMenu* inSaveMenu, __int32 xmlitemSizeID, TitemSize itemSize )
{

    for ( int taxNo=0;taxNo <itemSize.itemSizeTaxes.size(); taxNo++)
    {
          inSaveMenu->SaveItemSizeTaxProfile( xmlitemSizeID,itemSize.itemSizeTaxes[taxNo].taxProfileKey,itemSize.itemSizeTaxes[taxNo].taxName);
    }


}
void  TfrmMenuEdit::CsvSaveMenuItemSizePriceLevels(  TSaveMenu* inSaveMenu, __int32 xmlitemSizeID, TitemSize itemSize )
{ 
        AnsiString priceLevelName = "PriceLevel ";
            // allPriceLevels
        for (int index =1; index<=frmMenuEdit->allPriceLevels; index++)
        {
          inSaveMenu->SaveItemSizePriceLevels( xmlitemSizeID,index,priceLevelName + ""+ index,itemSize.itemSizePrice);
        }
}


void TfrmMenuEdit::CsvSaveMenuItemSizeRecipes(  TSaveMenu* inSaveMenu, __int32 xmlitemSizeID, TitemSize itemSize )
{
     for( int recipeNo=0;recipeNo < itemSize.itemSizeRecipe.size(); recipeNo ++)
     {
        	inSaveMenu->SaveItemSizeRecipe( xmlitemSizeID,
		itemSize.itemSizeRecipe[recipeNo].recipeKey,
		itemSize.itemSizeRecipe[recipeNo].stockCode,
		itemSize.itemSizeRecipe[recipeNo].qty,
		itemSize.itemSizeRecipe[recipeNo].location
		);
     }

}

void TfrmMenuEdit::LoadRecipeLocations(AnsiString stockcode)
{
     
                 if (qrLocations->Database->Connected && !qrLocations->Transaction->InTransaction)
                {
                    qrLocations->Transaction->StartTransaction();
                }
                qrLocations->Close();
                qrLocations->ParamByName( "CODE")->AsString	=  stockcode;
                for (qrLocations->ExecQuery(); !qrLocations->Eof; qrLocations->Next())
                {
                    edLoc->Items->Add(qrLocations->FieldByName("LOCATION")->AsString);

                }
}


 void TfrmMenuEdit::SaveMenuItemSizePriceLevels( TSaveMenu* inSaveMenu, __int32 inItemSizeID, TItemSizeNode* inDCData )
 {
        std::map<int ,Menu::TItemSizePriceLevel> pricelevels=inDCData->ItemSizePriceLevels;
        for (std::map<int ,Menu::TItemSizePriceLevel>::iterator it=pricelevels.begin(); it!=pricelevels.end(); ++it)
        {
           inSaveMenu->SaveItemSizePriceLevels( inItemSizeID,
            (*it).second.PriceLevelKey,
            PriceLevelsName[(*it).second.PriceLevelKey].PriceLevelName,
            (*it).second.Price);
        }
 }
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemSizeDrawCell(TObject *Sender, int ACol,
int ARow, TRect &Rect, TGridDrawState State)
{
	bool Draw = true;

	if (State.Contains(gdFixed)) Draw = false;
	if ((ACol == 1 || ACol == 2) &&(ARow!=0))
    {
         Draw = true;
    }
    else
    {
       Draw = false;
    }
        
	if (sgItemsize->RowCount == 2 && sgItemsize->Cells[0][1] == "") Draw = false;

	if (Draw)
	{
		Currency Value = 0;
		try
		{
            if(ACol == 2 )
            {
                sgItemsize->Canvas->Font->Color = clGray;
            }
			Value = StrToCurr(sgItemsize->Cells[ACol][ARow]);
              
		}
		catch (EConvertError &E)
		{
		 	Value = 0;
			sgItemsize->Cells[ACol][ARow] = "0.00";
		}
		sgItemsize->Canvas->FillRect(Rect);
		sgItemsize->Canvas->TextRect(Rect, Rect.Left+1 ,Rect.Top+1, CurrToStrF(Value, ffFixed, CurrencyDecimals));
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemSizeEnter(TObject *Sender)
{
    	if (sgItemsize->Col == 0 || sgItemsize->Row == 0)
	{
		TRect CellRect = sgItemsize->CellRect(1, 1);
		TPoint Centre = TPoint(CellRect.Left + ((CellRect.Right - CellRect.Left) / 2),
		CellRect.Top + ((CellRect.Bottom - CellRect.Top) / 2));
		PostMessage(sgItemsize->Handle, WM_LBUTTONDOWN, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
		PostMessage(sgItemsize->Handle, WM_LBUTTONUP, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemSizeExit(TObject *Sender)
{
    	try
	{
		sgItemsize->OnSelectCell = NULL;
		sgItemsize->Col = 0;
	}
	__finally
	{
		sgItemsize->OnSelectCell = sgItemSizeSelectCell;
	}
}
//------------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemSizeKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
    if (sgItemsize->Col == 1)
	{
		if ((Key >= '0' && Key <= '9')  || (Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9) ||
				Key == VK_DECIMAL || Key == '.' || Key == VK_BACK || Key == 229)
		{
			if (Key != '.')
			{

				AnsiString Value = sgItemsize->Cells[sgItemsize->Col][sgItemsize->Row];
				AnsiString Dec,Frac = "";
				int i = Value.AnsiPos(".");
				Dec = Value;
				// has a decimal point
				if (i > 0)
				{
					Dec = Value.SubString(1,i-1);
					Frac = Value.SubString(i+1,Value.Length() - i);
				}
				if (Dec.Length()  >= 9 || Frac.Length() >= 2 )
				{
				  	Key = NULL;
				   	sgItemsize->Cells[sgItemsize->Col][sgItemsize->Row] = sgItemsize->Cells[sgItemsize->Col][sgItemsize->Row];
				}
			 	if (sgItemsize->Row == 1)
                {
                    for(i=2;i<sgItemsize->RowCount;i++)
                    {
			            sgItemsize->Cells[1][i] = sgItemsize->Cells[1][1];
                    }
			    }
            }
		}
	}
    else if(sgItemsize->Col == 0)
    {
        AnsiString Value = sgItemsize->Cells[sgItemsize->Col][sgItemsize->Row];
        if(Value == "")
        {
            PriceLevelsName[sgItemsize->Row].PriceLevelName = Value;
        }    

        if(strlen(Value.c_str()) >= 50)
        {
             Key = NULL;

			 sgItemsize->Cells[sgItemsize->Col][sgItemsize->Row] = Value.SubString(1, 50);//sgItemsize->Cells[sgItemsize->Col][sgItemsize->Row];
             
        }
    }

	if (Key == VK_RETURN)
	{
		Key = NULL;
	}    
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemSizeSelectCell(TObject *Sender,
int ACol, int ARow, bool &CanSelect)
{
   	CanSelect = (ACol != 2);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemSizeEditText(TObject *Sender,
int ACol, int ARow, const AnsiString Value)
{  
 	if (Value != "" && (ARow >= 1))
	{
       	Currency CurVal;
        Currency GP;
        TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
        if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
        {
            TItemSizeNode *DrinkCostData = (TItemSizeNode *)CurrentNodeData;
			
			try
			{ 			
                if(ACol == 1)
                {

                  if(CheckIfNumeric(Value) )
                  {
                     CurVal = StrToCurr(Value);
			
                        if(ARow == 1)
                        {
                            if (CurVal != DrinkCostData->Price)
                            {
                                for(int i=1; i<sgItemsize->RowCount;i++)
                                {
                                      DrinkCostData->ItemSizePriceLevels[i].Price = CurVal;
                                    Currency PriceExc = (CurVal* 100.0) / (100.0);
                                    if (PriceExc != 0)
                                    {
                                        GP = (1 - (edItemCost->Value / PriceExc)) * 100;
                                    }
                                    sgItemsize->Cells[2][i] = CurrToStrF(GP, ffFixed, CurrencyDecimals);
                                    GP = 0;
                                    DrinkCostData->Price = CurVal;
                                    MenuEdited = true;
                                    RelabelDrinkCosts();
                                }
                            }
                        }
                        else
                        {
                            if(CurVal != DrinkCostData->ItemSizePriceLevels[ARow].Price)
                            {
                                DrinkCostData->ItemSizePriceLevels[ARow].Price = CurVal;
                                Currency PriceExc = (CurVal* 100.0) / (100.0);
                                if (PriceExc != 0)
                                {
                                    GP = (1 - (edItemCost->Value / PriceExc)) * 100;
                                }
                                sgItemsize->Cells[2][ARow] = CurrToStrF(GP, ffFixed, CurrencyDecimals);
                                GP = 0;
                                DrinkCostData->SpecialPrice = CurVal;
                                MenuEdited = true;
                                RelabelDrinkCosts();
                            }
                        }

                   }
                 }
              else if(Value != "" && (ACol == 0))
             {
                TEditorNode *CurrentNodeData = (TEditorNode *)tvMenu->Selected->Data;
                if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
                {
                    
                    TItemSizeNode *DrinkCostData = (TItemSizeNode *)CurrentNodeData;
                    PriceLevelsName[ARow].PriceLevelName = Value;
                    MenuEdited = true;
                    RelabelDrinkCosts();
                 }
            }

         }
        catch (EConvertError &E)
        {
            if (ACol == 1)
            {
                sgItemsize->Cells[ACol][ARow] = CurrToStrF(DrinkCostData->ItemSizePriceLevels[ACol].Price, ffGeneral, CurrencyDecimals);
            }
            if(ACol == 0)
            {
                sgItemsize->Cells[ACol][ARow] = PriceLevelsName[ARow].PriceLevelName;
            }
    }
       sgItemsize->Refresh() ;
	 
    } 
}   
    
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemSizeKeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{
   	if (sgItemsize->Col == 1)
	{
		if (!((Key >= '0' && Key <= '9') || (Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9) ||
					Key == VK_DECIMAL || Key == '.' || Key == VK_BACK || Key == VK_LEFT || Key == VK_RIGHT))
		{
			Key = NULL;
		}
		else if ((Key == '.' || Key == VK_DECIMAL) && sgItemsize->Cells[sgItemsize->Col][sgItemsize->Row].AnsiPos(".") != 0)
		{
			Key = NULL;
		}
		if (!sgItemsize->Objects[sgItemsize->Col][sgItemsize->Row])
		{
			Key = NULL;
		}
	}
	else
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::sgItemSizeKeyPress(TObject *Sender, char &Key)
{
 	if (Key == VK_RETURN)
	{
		Key = NULL;
		if (sgItemsize->Col == 1)
		{
			TRect CellRect = sgItemsize->CellRect(2, sgItemsize->Row);
			TPoint Centre = TPoint(CellRect.Left + ((CellRect.Right - CellRect.Left) / 2),
			CellRect.Top + ((CellRect.Bottom - CellRect.Top) / 2));
			PostMessage(sgItemsize->Handle, WM_LBUTTONDOWN, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
			PostMessage(sgItemsize->Handle, WM_LBUTTONUP, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
		}
		else
		{
		 	if (sgItemsize->Row != sgItemsize->RowCount-1)
			{
				TRect CellRect = sgItemsize->CellRect(1, sgItemsize->Row+1);
				TPoint Centre = TPoint(CellRect.Left + ((CellRect.Right - CellRect.Left) / 2),
				CellRect.Top + ((CellRect.Bottom - CellRect.Top) / 2));
				PostMessage(sgItemsize->Handle, WM_LBUTTONDOWN, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
				PostMessage(sgItemsize->Handle, WM_LBUTTONUP, 0, (Centre.x&0xFFFF)+(Centre.y<<16));
			}
			else btnNewItem->SetFocus();   
		}
	}
}
//---------------------------------------------------------------------------
 void TfrmMenuEdit:: LoadPriceLevelNames()
 {
  if (qrGetPriceLevelNames->Database->Connected && !qrGetPriceLevelNames->Transaction->InTransaction)
    {
        qrGetPriceLevelNames->Transaction->StartTransaction();
        qrGetPriceLevelNames->Close();
        qrGetPriceLevelNames->ExecQuery();

        if(qrGetPriceLevelNames->RecordCount>0)
        {
            PriceLevelsName.clear();
            for(int key =1; key<= noOfPriceLevels; key++)
            {
                Menu::TPriceLevelsName priceLevelName;
                priceLevelName.PriceLevelKey = key;
                if(!qrGetPriceLevelNames->Eof)
                priceLevelName.PriceLevelName = qrGetPriceLevelNames->FieldByName("PRICELEVEL_NAME")->AsString;
                PriceLevelsName.insert(std::pair<int, Menu::TPriceLevelsName>(key,priceLevelName) );
                qrGetPriceLevelNames->Next();
            }
        }    
        qrGetPriceLevelNames->Transaction->Commit();
    }
}


 bool TfrmMenuEdit:: CheckIfNumeric(AnsiString Value)
 {
    std::string  word= Value.c_str();





  if( Value.Trim() == "-" )return false;


   if (word.find_first_not_of("01234567890.-") != std::string::npos)
    {
      return false;







    }
   return true;

 }

void __fastcall TfrmMenuEdit::nePriceForPointExit(TObject *Sender)
{
	float check_gst_value =
	StrToFloatDef(nePriceForPoint->Value, -9999.0);
	TEditorNode *CurrentNodeData;
	TItemSizeNode *DrinkCostData;


	if (check_gst_value < 0)
	nePriceForPoint->Value = 0.0;

	CurrentNodeData =
	reinterpret_cast<TEditorNode *>(tvMenu->Selected->Data);
	if (CurrentNodeData->NodeType == ITEM_SIZE_NODE)
	{
		DrinkCostData =
		reinterpret_cast<TItemSizeNode *>(CurrentNodeData);
		if (DrinkCostData->CostForPoints != nePriceForPoint->Value) {
			MenuEdited = false;
			DrinkCostData->CostForPoints = nePriceForPoint->Value;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::nePriceForPointChange(TObject *Sender)
{
  MenuEdited = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::nePriceForPointEnter(TObject *Sender)
{
  ((TEdit *)Sender)->SelectAll();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TfrmMenuEdit::UpdateItemForForcedOptions()
{
    RefreshItem((TItemNode *)tvMenu->Selected->Data, true);// refresh item when drag down..
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


