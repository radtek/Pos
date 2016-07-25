//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UpdateCategories.h"
#include "Utilities.h"
#include "MenuEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma link "ThemeMgr"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
using namespace CategoryTranslation;
//---------------------------------------------------------------------------
__fastcall TfrmUpdateCategories::TfrmUpdateCategories(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::FormShow(TObject *Sender)
{
	PageControl2->ActivePage = tsSelectGroups;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::btnNext1Click(TObject *Sender)
{
	tvCategories->Items->Clear();
	tvCategories->Items->BeginUpdate();
	tvGroupedCategories->Items->Clear();
	tvGroupedCategories->Items->BeginUpdate();

	for (int i=0; i<chlbOldCategories->Items->Count; i++)
	{
		if (chlbOldCategories->Checked[i])
		{
			TCategoryTranslation *CategoryTranslation	= (TCategoryTranslation *)chlbOldCategories->Items->Objects[i];

			CategoryTranslation->CategoryType			= ctUnused;
		}
		else
		{
			TCategoryTranslation *CategoryTranslation	= (TCategoryTranslation *)chlbOldCategories->Items->Objects[i];

			CategoryTranslation->CategoryType			= ctCategory;

			TTreeNode *NewGroup								= tvCategories->Items->Add(NULL, CategoryTranslation->OldCategoryName);
			NewGroup->Data										= CategoryTranslation;
			NewGroup->ImageIndex								= ICON_BOX_INDEX;
			NewGroup->SelectedIndex							= ICON_BOX_INDEX;
		}
	}
	tvCategories->Items->EndUpdate();
	tvGroupedCategories->Items->EndUpdate();
	PageControl2->ActivePage = tsSelectCategories;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::tvGroupedCategoriesMouseDown(
		TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
		int Y)
{
	if (Button == mbLeft && tvGroupedCategories->Selected && tvGroupedCategories->Selected->Level == 1)
	{
		tvGroupedCategories->BeginDrag(true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::tvGroupedCategoriesDragDrop(
		TObject *Sender, TObject *Source, int X, int Y)
{
	TTreeView *SourceTree	= NULL;
	TTreeView *DestTree		= NULL;
	if ((SourceTree	= dynamic_cast<TTreeView *>(Source)) == NULL) return;
	if ((DestTree		= dynamic_cast<TTreeView *>(Sender)) == NULL) return;
	THitTests HT		=	DestTree->GetHitTestInfoAt(X, Y);

	if (HT.Contains(htOnItem) || HT.Contains(htOnIcon))
	{
		TTreeNode *DestItem = DestTree->GetNodeAt(X, Y);
		if (SourceTree == DestTree)
		{
			if (SourceTree == tvGroupedCategories)
			{
				if (DestItem->Level == 0)
				{
					SourceTree->Selected->MoveTo(DestItem, naAddChild);
				}
				else
				{
					SourceTree->Selected->MoveTo(DestItem, naInsert);
				}
			}
		}
		else
		{
			std::vector<TTreeNode *> NodesToDelete;
			TTreeNode *NewNode = NULL;
			for (int i=0; i<SourceTree->Items->Count; i++)
			{
				TTreeNode *SourceNode = SourceTree->Items->Item[i];
				if (SourceNode->Selected)
				{
					NodesToDelete.push_back(SourceNode);

					if (DestItem->Level == 0)
					{
						NewNode = DestTree->Items->AddChild(DestItem, SourceNode->Text);
					}
					else
					{
						NewNode = DestTree->Items->Insert(DestItem, SourceNode->Text);
					}
					NewNode->Data = SourceNode->Data;
				}
			}
			for (unsigned i=0; i<NodesToDelete.size(); i++)
			{
				SourceTree->Items->Delete(NodesToDelete[i]);
			}
			DestTree->Selected = NewNode;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::tvCategoriesDragDrop(TObject *Sender,
		TObject *Source, int X, int Y)
{
	TTreeView *SourceTree	= NULL;
	TTreeView *DestTree		= NULL;
	if ((SourceTree			= dynamic_cast<TTreeView *>(Source)) == NULL) return;
	if ((DestTree				= dynamic_cast<TTreeView *>(Sender)) == NULL) return;

	if (SourceTree == tvGroupedCategories)
	{
		TTreeNode *DestItem	= DestTree->GetNodeAt(X, Y);
		TTreeNode *NewNode	= NULL;
		if (DestItem)
		{
			NewNode = DestTree->Items->Insert(DestItem, SourceTree->Selected->Text);
		}
		else
		{
			NewNode = DestTree->Items->AddChild(NULL, SourceTree->Selected->Text);
		}
		NewNode->Data = SourceTree->Selected->Data;
		SourceTree->Items->Delete(SourceTree->Selected);
		DestTree->Selected = NewNode;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::tvGroupedCategoriesDragOver(
		TObject *Sender, TObject *Source, int X, int Y, TDragState State,
		bool &Accept)
{
	Accept = (Source == tvGroupedCategories || Source == tvCategories);
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::tvCategoriesDragOver(TObject *Sender,
		TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	Accept = (Source == tvGroupedCategories);
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::btnNext2Click(TObject *Sender)
{
	TCategoryTranslation *Breakdown	= new TCategoryTranslation;
	Breakdown->OldCategoryName			= "Breakdown Categories";
	Breakdown->CategoryType				= ctBreakdownGroup;

	TTreeNode *NewCategory				= tvGroupedCategories->Items->Add(NULL, Breakdown->OldCategoryName);
	NewCategory->Data						= Breakdown;
	NewCategory->ImageIndex				= ICON_CLOSED_FOLDER_INDEX;
	NewCategory->SelectedIndex			= ICON_OPEN_FOLDER_INDEX;

	UpdateCategories->CategoryTranslationList->Add(Breakdown);

	if (vtvRename->IsEditing())
	{
		vtvRename->EndEditNode();
	}
	vtvRename->Clear();
	vtvRename->NodeDataSize				= sizeof(TTreeData);

	TTreeData *NodeData					= NULL;
	PVirtualNode GroupNode				= NULL;
	PVirtualNode BreakdownGroupNode	= NULL;
	PVirtualNode CategoryNode			= NULL;

	TTreeNode *Node = tvGroupedCategories->Items->GetFirstNode();
	while (Node)
	{
		TCategoryTranslation *CategoryTranslation	= (TCategoryTranslation *)Node->Data;

		if (CategoryTranslation->CategoryType == ctGroup || CategoryTranslation->CategoryType == ctBreakdownGroup)
		{
			GroupNode   							= vtvRename->AddChild(NULL);
			NodeData	   							= (TTreeData *)vtvRename->GetNodeData(GroupNode);
			NodeData->CategoryTranslation		= CategoryTranslation;

			if (NodeData->CategoryTranslation->NewCategoryName == "")
			{
				NodeData->CategoryTranslation->NewCategoryName = NodeData->CategoryTranslation->OldCategoryName;
			}
			if (CategoryTranslation == Breakdown)
			{
				BreakdownGroupNode = GroupNode;
			}
		}
		else if (CategoryTranslation->CategoryType == ctCategory)
		{
			CategoryNode  							= vtvRename->AddChild(GroupNode);
			NodeData	   							= (TTreeData *)vtvRename->GetNodeData(CategoryNode);
			NodeData->CategoryTranslation		= CategoryTranslation;
			if (NodeData->CategoryTranslation->NewCategoryName == "")
			{
				NodeData->CategoryTranslation->NewCategoryName = NodeData->CategoryTranslation->OldCategoryName;
			}

			vtvRename->Expanded[GroupNode]	= true;
		}
		Node = Node->GetNext();
	}
	Node = tvCategories->Items->GetFirstNode();
	while (Node)
	{
		TCategoryTranslation *CategoryTranslation	= (TCategoryTranslation *)Node->Data;

//		CategoryTranslation->CategoryType = ctUnused;
		CategoryNode  							= vtvRename->AddChild(BreakdownGroupNode);
		NodeData	   							= (TTreeData *)vtvRename->GetNodeData(CategoryNode);
		NodeData->CategoryTranslation		= CategoryTranslation;
		if (NodeData->CategoryTranslation->NewCategoryName == "")
		{
			NodeData->CategoryTranslation->NewCategoryName = NodeData->CategoryTranslation->OldCategoryName;
		}

		vtvRename->Expanded[GroupNode]	= true;

		Node = Node->GetNext();
	}
	if (vtvRename->GetFirst())
	{
		vtvRename->FocusedNode = vtvRename->GetFirst();
		if (vtvRename->FocusedNode)
		{
			vtvRename->Selected[vtvRename->FocusedNode] = true;
		}
	}
	PageControl2->ActivePage = tsRename;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::Button4Click(TObject *Sender)
{
	TTreeNode *Node = tvGroupedCategories->Items->GetFirstNode();
	while (Node)
	{
		TCategoryTranslation *CategoryTranslation	= (TCategoryTranslation *)Node->Data;
		if (CategoryTranslation->CategoryType == ctGroup || CategoryTranslation->CategoryType == ctBreakdownGroup)
		{
			UpdateCategories->CategoryTranslationList->Delete(UpdateCategories->CategoryTranslationList->IndexOf(CategoryTranslation));
		}
		Node = Node->GetNext();
	}
	tvCategories->Items->Clear();
	tvGroupedCategories->Items->Clear();
	PageControl2->ActivePage = tsSelectGroups;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameExit(TObject *Sender)
{
	if (vtvRename->IsEditing())
	{
		vtvRename->EndEditNode();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameEnter(TObject *Sender)
{
	if (!vtvRename->IsEditing())
	{
		if (!vtvRename->FocusedNode)
		{
			vtvRename->FocusedNode = vtvRename->GetFirst();
		}
		if (vtvRename->FocusedNode)
		{
			vtvRename->FocusedColumn = 1;
			vtvRename->EditNode(vtvRename->FocusedNode, vtvRename->FocusedColumn);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameAfterPaint(
		TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions << toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameBeforePaint(
		TBaseVirtualTree *Sender, TCanvas *TargetCanvas)
{
	((TVirtualStringTree *)Sender)->TreeOptions->MiscOptions >> toGridExtensions;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameColumnDblClick(
		TBaseVirtualTree *Sender, TColumnIndex Column, TShiftState Shift)
{
	Sender->EditNode(Sender->FocusedNode, Sender->FocusedColumn);
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameCreateEditor(
		TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
		IVTEditLink *EditLink)
{
	TTreeData *NodeData				= (TTreeData *)Sender->GetNodeData(Node);

	if (NodeData->CategoryTranslation->CategoryType == ctGroup || NodeData->CategoryTranslation->CategoryType == ctBreakdownGroup)
	{
		if (UpdateCategories->CurrentCategories.size() > 0)
		{
			UpdateCategories->GetCurrentGroups(cbNewName->Items);
			cbNewName->Text					= NodeData->CategoryTranslation->NewCategoryName;
			cbNewName->Tag						= 1;
			edNewName->Tag						= 0;
			TPropertyEdit* PropertyLink	= new TPropertyEdit(Sender, Node, Column, cbNewName);
			PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		}
		else
		{
			edNewName->Text					= NodeData->CategoryTranslation->NewCategoryName;
			cbNewName->Tag						= 0;
			edNewName->Tag						= 1;
			TPropertyEdit* PropertyLink	= new TPropertyEdit(Sender, Node, Column, edNewName);
			PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		}
	}
	else if (NodeData->CategoryTranslation->CategoryType == ctCategory)
	{
		TTreeData *ParentData				= (TTreeData *)Sender->GetNodeData(Sender->NodeParent[Node]);
		UpdateCategories->GetCurrentCategories(ParentData->CategoryTranslation->NewCategoryName, cbNewName->Items);

		if (cbNewName->Items->Count > 0)
		{
			cbNewName->Text					= NodeData->CategoryTranslation->NewCategoryName;
			cbNewName->Tag						= 1;
			edNewName->Tag						= 0;
			TPropertyEdit* PropertyLink	= new TPropertyEdit(Sender, Node, Column, cbNewName);
			PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		}
		else
		{
			edNewName->Text					= NodeData->CategoryTranslation->NewCategoryName;
			cbNewName->Tag						= 0;
			edNewName->Tag						= 1;
			TPropertyEdit* PropertyLink	= new TPropertyEdit(Sender, Node, Column, edNewName);
			PropertyLink->QueryInterface(__uuidof(IVTEditLink), (void**)EditLink);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameEdited(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	TTreeData *NodeData = (TTreeData *)Sender->GetNodeData(Node);

	if (edNewName->Tag == 1)
	{
		NodeData->CategoryTranslation->NewCategoryName = edNewName->Text;
	}
	else if (cbNewName->Tag == 1)
	{
		NodeData->CategoryTranslation->NewCategoryName = cbNewName->Text;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameEditing(
		TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      bool &Allowed)
{
	Allowed = (Column == 1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameFocusChanged(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	if (Node && !Sender->IsEditing())
	{
		if (Column != 1)
		{
			Sender->FocusedColumn = 1;
		}
		else
		{
			Sender->EditNode(Node, Column);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameFocusChanging(
      TBaseVirtualTree *Sender, PVirtualNode OldNode, PVirtualNode NewNode,
		TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed)
{
	if (Sender->IsEditing())
	{
		Sender->EndEditNode();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameGetImageIndex(
		TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind,
		TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
	TTreeData *NodeData = (TTreeData *)Sender->GetNodeData(Node);
	if (NodeData && Column == ((TVirtualStringTree *)Sender)->Header->MainColumn)
	{
		if (Sender->GetNodeLevel(Node) == 0)
		{
			if (Kind == ikNormal)
			{
				ImageIndex = ICON_CLOSED_FOLDER_INDEX;
			}
			else if (Kind == ikSelected)
			{
				ImageIndex = ICON_OPEN_FOLDER_INDEX;
			}
			else
			{
				ImageIndex = -1;
			}
		}
		else
		{
			if (Kind == ikNormal || Kind == ikSelected)
			{
				ImageIndex = ICON_BOX_INDEX;
			}
			else
			{
				ImageIndex = -1;
			}
		}
	}
	else
	{
		ImageIndex = -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameGetText(
		TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
		TVSTTextType TextType, WideString &CellText)
{
	TTreeData *NodeData = (TTreeData *)Sender->GetNodeData(Node);
	if (NodeData && TextType == ttNormal)
	{
		if (Column == 0)
		{
			CellText = NodeData->CategoryTranslation->OldCategoryName;
		}
		else if (Column == 1)
		{
			CellText = NodeData->CategoryTranslation->NewCategoryName;
		}
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::vtvRenameKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if (!vtvRename->IsEditing() && vtvRename->FocusedNode)
	{
		if (Key == VK_RETURN)
		{
			vtvRename->EditNode(vtvRename->FocusedNode, vtvRename->FocusedColumn);
		}
		else if (Key == VK_RIGHT)
		{
			vtvRename->Expanded[vtvRename->FocusedNode] = true;
		}
		else if (Key == VK_LEFT)
		{
			vtvRename->Expanded[vtvRename->FocusedNode] = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::edNewNameKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN || Key == VK_DOWN || Key == VK_UP)
	{
		vtvRename->BeginUpdate();
		try
		{
			vtvRename->EndEditNode();
		}
		__finally
		{
			vtvRename->EndUpdate();
		}
		if (Key == VK_RETURN || Key == VK_DOWN)
		{
			if (vtvRename->GetNext(vtvRename->FocusedNode))
			{
				vtvRename->FocusedNode = vtvRename->GetNext(vtvRename->FocusedNode);
				vtvRename->Selected[vtvRename->FocusedNode] = true;
			}
			else
			{
				vtvRename->EditNode(vtvRename->FocusedNode, vtvRename->FocusedColumn);
			}
		}
		else if (Key == VK_UP)
		{
			if (vtvRename->GetPrevious(vtvRename->FocusedNode))
			{
				vtvRename->FocusedNode = vtvRename->GetPrevious(vtvRename->FocusedNode);
				vtvRename->Selected[vtvRename->FocusedNode] = true;
			}
			else
			{
				vtvRename->EditNode(vtvRename->FocusedNode, vtvRename->FocusedColumn);
			}
		}
	}
	else if (Key == VK_ESCAPE)
	{
		vtvRename->CancelEditNode();
		vtvRename->EditNode(vtvRename->FocusedNode, vtvRename->FocusedColumn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::edNewNameKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::edNewNameKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if (Key == VK_DOWN || Key == VK_UP)
	{
		edNewName->SelectAll();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::Button1Click(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::Button2Click(TObject *Sender)
{
	AnsiString NewGroupName = "";
	if (InputQuery("Group Name", "Enter a name for the category group (Max " + IntToStr(MAX_CATEGORY_GROUP_NAME_LENGTH) + " chars).", NewGroupName))
	{
		if (NewGroupName != "")
		{
			if (NewGroupName.Length() > MAX_CATEGORY_GROUP_NAME_LENGTH)
			{
				if (Application->MessageBox("The name is too long and will be trimmed.\rIs this OK?",
													 "Warning", MB_OKCANCEL + MB_ICONWARNING) != ID_OK)
				{
					return;
				}
				NewGroupName = NewGroupName.SubString(1, MAX_CATEGORY_GROUP_NAME_LENGTH);
			}
			if (UpdateCategories->FindOldCategory(NewGroupName))
			{
				Application->MessageBox("This group/category already exists.", "Warning", MB_OK + MB_ICONERROR);
				return;
			}
			TCategoryTranslation *CategoryTranslation	= new TCategoryTranslation;
			CategoryTranslation->OldCategoryName		= NewGroupName;
			CategoryTranslation->NewCategoryName		= NewGroupName;
			CategoryTranslation->CategoryType			= ctGroup;

			TTreeNode *NewCategory							= tvGroupedCategories->Items->Add(NULL, CategoryTranslation->OldCategoryName);
			NewCategory->Data									= CategoryTranslation;
			NewCategory->ImageIndex							= ICON_CLOSED_FOLDER_INDEX;
			NewCategory->SelectedIndex						= ICON_OPEN_FOLDER_INDEX;

			UpdateCategories->CategoryTranslationList->Add(CategoryTranslation);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::Button3Click(TObject *Sender)
{
	TTreeNode *Node = tvGroupedCategories->Selected;
	if (Node)
	{
		TCategoryTranslation *Group = (TCategoryTranslation *)Node->Data;

		if (Group->CategoryType == ctGroup || Group->CategoryType == ctBreakdownGroup)
		{
			for (int i=0; i<Node->Count; i++)
			{
				TTreeNode *NewNode = tvCategories->Items->AddChild(NULL, Node->Item[i]->Text);
				NewNode->Data = Node->Item[i]->Data;
				tvCategories->Items->Delete(Node->Item[i]);
				tvCategories->Selected = NewNode;
			}
			UpdateCategories->CategoryTranslationList->Delete(UpdateCategories->CategoryTranslationList->IndexOf(Group));
			delete Group;
			tvCategories->Items->Delete(Node);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::edNewNameChange(TObject *Sender)
{
	if (vtvRename->FocusedNode)
	{
		TTreeData *NodeData = (TTreeData *)vtvRename->GetNodeData(vtvRename->FocusedNode);
		if (NodeData->CategoryTranslation->CategoryType == ctGroup || NodeData->CategoryTranslation->CategoryType == ctBreakdownGroup)
		{
			if (edNewName->Text.Length() > MAX_CATEGORY_GROUP_NAME_LENGTH)
			{
				edNewName->Text = edNewName->Text.SubString(1, MAX_CATEGORY_GROUP_NAME_LENGTH);
				edNewName->SelStart = edNewName->Text.Length();
			}
		}
		else if (NodeData->CategoryTranslation->CategoryType == ctCategory)
		{
			if (edNewName->Text.Length() > MAX_CATEGORY_NAME_LENGTH)
			{
				edNewName->Text = edNewName->Text.SubString(1, MAX_CATEGORY_NAME_LENGTH);
				edNewName->SelStart = edNewName->Text.Length();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateCategories::cbNewNameChange(TObject *Sender)
{
	if (vtvRename->FocusedNode)
	{
		TTreeData *NodeData = (TTreeData *)vtvRename->GetNodeData(vtvRename->FocusedNode);
		if (NodeData->CategoryTranslation->CategoryType == ctGroup || NodeData->CategoryTranslation->CategoryType == ctBreakdownGroup)
		{
			if (cbNewName->Text.Length() > MAX_CATEGORY_GROUP_NAME_LENGTH)
			{
				cbNewName->Text		= cbNewName->Text.SubString(1, MAX_CATEGORY_GROUP_NAME_LENGTH);
				cbNewName->SelStart	= cbNewName->Text.Length();
			}
		}
		else if (NodeData->CategoryTranslation->CategoryType == ctCategory)
		{
			if (cbNewName->Text.Length() > MAX_CATEGORY_NAME_LENGTH)
			{
				cbNewName->Text		= cbNewName->Text.SubString(1, MAX_CATEGORY_NAME_LENGTH);
				cbNewName->SelStart	= cbNewName->Text.Length();
			}
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TCategoryTranslation::TCategoryTranslation() : System::TObject()
{
	CategoryType	= ctUndefined;
}
//---------------------------------------------------------------------------
__fastcall TCategoryTranslation::~TCategoryTranslation()
{
	CategoryType = ctUndefined;
}
//---------------------------------------------------------------------------
TUpdateCategories::TUpdateCategories()
{
	CategoryTranslationList	= new TList;
	CategoryGroups				= new TList;
}
//---------------------------------------------------------------------------
TUpdateCategories::~TUpdateCategories()
{
	for (int i=0; i<CategoryTranslationList->Count; i++)
	{
		TCategoryTranslation *CategoryTranslation = (TCategoryTranslation *)CategoryTranslationList->Items[i];
		delete CategoryTranslation;
	}
	delete CategoryTranslationList;
	for (int i=0; i<CategoryGroups->Count; i++)
	{
		TCategoryGroup *CategoryGroup = (TCategoryGroup *)CategoryGroups->Items[i];
		delete CategoryGroup;
	}
	delete CategoryGroups;
}
//---------------------------------------------------------------------------
void TUpdateCategories::AddOldCategory(AnsiString OldCategoryName)
{
	if (!FindOldCategory(OldCategoryName))
	{
		TCategoryTranslation *CategoryTranslation = new TCategoryTranslation;
		CategoryTranslation->OldCategoryName = OldCategoryName;
		CategoryTranslationList->Add(CategoryTranslation);
	}
}
//---------------------------------------------------------------------------
TCategoryTranslation *TUpdateCategories::FindOldCategory(AnsiString OldCategoryName)
{
	for (int i=0; i<CategoryTranslationList->Count; i++)
	{
		TCategoryTranslation *CategoryTranslation = (TCategoryTranslation *)CategoryTranslationList->Items[i];
		if (CategoryTranslation->OldCategoryName == OldCategoryName)
		{
			return CategoryTranslation;
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
int TUpdateCategories::GetCategoryPriority(AnsiString OldCategoryName)
{
	int Priority = -1;
	int GC = GroupCount();
	for (int i=GC-1; i>=0; i--)
	{
		int CC = CategoryCount(i);
		for (int j=CC-1; j>=0; j--)
		{
			Priority++;
			TCategoryTranslation *Category = GetCategory(i, j);

			if (Category->OldCategoryName == OldCategoryName)
			{
				TCategoryTranslation	*Group = GetGroup(i);
				if (Group->CategoryType == ctGroup)
				{
					return Priority;
				}
				else
				{
					return -1;
				}
			}
		}
	}
	return -1;
/*
	int Priority = -1;
	int GC = GroupCount();
	for (int i=0; i<GC; i++)
	{
		for (int j=0; j<CategoryCount(i); j++)
		{
			Priority++;
			TCategoryTranslation *Category = GetCategory(i, j);

			if (Category->OldCategoryName == OldCategoryName)
			{
				return Priority;
			}
		}
	}
	return -1;
*/
}
//---------------------------------------------------------------------------
TCategoryType TUpdateCategories::GetCategoryType(AnsiString OldCategoryName)
{
	for (int i=0; i<CategoryTranslationList->Count; i++)
	{
		TCategoryTranslation *CategoryTranslation = (TCategoryTranslation *)CategoryTranslationList->Items[i];
		if (CategoryTranslation->OldCategoryName == OldCategoryName)
		{
			return CategoryTranslation->CategoryType;
		}
	}
	return ctUndefined;
}
//---------------------------------------------------------------------------
AnsiString TUpdateCategories::GetNewCategoryName(AnsiString OldCategoryName)
{
	for (int i=0; i<CategoryTranslationList->Count; i++)
	{
		TCategoryTranslation *CategoryTranslation = (TCategoryTranslation *)CategoryTranslationList->Items[i];
		if (CategoryTranslation->OldCategoryName == OldCategoryName)
		{
			return CategoryTranslation->NewCategoryName;
		}
	}
	return "";
}
//---------------------------------------------------------------------------
TCategoryTranslation *TUpdateCategories::GetGroup(int GroupIndex)
{
	return ((TCategoryGroup *)CategoryGroups->Items[GroupIndex])->Group;
}
//---------------------------------------------------------------------------
int TUpdateCategories::GroupCount()
{
	return CategoryGroups->Count;
}
//---------------------------------------------------------------------------
TCategoryTranslation *TUpdateCategories::GetCategory(int GroupIndex, int CategoryIndex)
{
	return ((TCategoryGroup *)CategoryGroups->Items[GroupIndex])->GetCategory(CategoryIndex);
}
//---------------------------------------------------------------------------
int TUpdateCategories::CategoryCount(int GroupIndex)
{
	return ((TCategoryGroup *)CategoryGroups->Items[GroupIndex])->CategoryCount();
}
//---------------------------------------------------------------------------
void TUpdateCategories::AddGroup(TCategoryTranslation *CategoryGroup)
{
	for (int i=0; i<CategoryGroups->Count; i++)
	{
		if (((TCategoryGroup *)CategoryGroups->Items[i])->Group->NewCategoryName.UpperCase() == CategoryGroup->NewCategoryName.UpperCase())
		{
			return;
		}
	}
	TCategoryGroup *GroupContainer = new TCategoryGroup;
	GroupContainer->Group = CategoryGroup;
	CategoryGroups->Add(GroupContainer);
}
//---------------------------------------------------------------------------
void TUpdateCategories::AddCategory(AnsiString GroupName, TCategoryTranslation *CategoryGroup)
{
	TCategoryGroup *Group = NULL;
	for (int i=0; i<CategoryGroups->Count; i++)
	{
		TCategoryGroup *CurrentGroup = (TCategoryGroup *)CategoryGroups->Items[i];
		if (CurrentGroup->Group->NewCategoryName.UpperCase() == GroupName.UpperCase())
		{
			Group = CurrentGroup;
		}
/*		for (int j=0; j<CurrentGroup->CategoryCount(); j++)
		{
			if (CurrentGroup->GetCategory(j)->NewCategoryName.UpperCase() == CategoryGroup->NewCategoryName.UpperCase())
			{
				return;
			}
		}*/
	}
	if (Group)
	{
		Group->AddCategory(CategoryGroup);
	}
}
//---------------------------------------------------------------------------
void TUpdateCategories::AddCurrentGroup(AnsiString GroupName)
{
	CurrentCategories[GroupName] = TGroupSet();
}
//---------------------------------------------------------------------------
void TUpdateCategories::AddCurrentCategory(AnsiString GroupName, AnsiString CategoryName)
{
	CurrentCategories[GroupName].insert(CategoryName);
}
//---------------------------------------------------------------------------
void TUpdateCategories::GetCurrentGroups(TStrings *Groups)
{
	Groups->Clear();
	std::map<AnsiString, TGroupSet>::iterator iGroup;
	for (iGroup=CurrentCategories.begin(); iGroup!=CurrentCategories.end(); iGroup++)
	{
		Groups->Add((*iGroup).first);
	}
}
//---------------------------------------------------------------------------
void TUpdateCategories::GetCurrentCategories(AnsiString GroupName, TStrings *Categories)
{
	Categories->Clear();
	TGroupSet GroupSet = CurrentCategories[GroupName];

	TGroupSet::iterator iCategory;
	for (iCategory = GroupSet.begin(); iCategory != GroupSet.end(); iCategory++)
	{
		Categories->Add(*iCategory);
	}
}
//---------------------------------------------------------------------------
int TCategoryGroup::CategoryCount()
{
	return Categories.size();
}
//---------------------------------------------------------------------------
int TCategoryGroup::AddCategory(TCategoryTranslation *Category)
{
	Categories.push_back(Category);
	return Categories.size();
}
//---------------------------------------------------------------------------
TCategoryTranslation *TCategoryGroup::GetCategory(int CategoryIndex)
{
	return Categories[CategoryIndex];
}
//---------------------------------------------------------------------------
bool TUpdateCategories::Show()
{
	TModalResult ModalResult = mrCancel;
	try
	{
		Form = new TfrmUpdateCategories(NULL);
		Form->UpdateCategories = this;
		Form->chlbOldCategories->Items->Clear();
		for (int i=0; i<CategoryTranslationList->Count; i++)
		{
			TCategoryTranslation *CategoryTranslation = (TCategoryTranslation *)CategoryTranslationList->Items[i];

			Form->chlbOldCategories->Items->AddObject(CategoryTranslation->OldCategoryName, CategoryTranslation);
		}
		ModalResult = Form->ShowModal();

		if (ModalResult == mrOk)
		{
			AnsiString GroupName = "";
			PVirtualNode Node = Form->vtvRename->GetFirst();
			while(Node)
			{
				TTreeData *NodeData = (TTreeData *)Form->vtvRename->GetNodeData(Node);
				if (NodeData->CategoryTranslation->CategoryType == ctGroup || NodeData->CategoryTranslation->CategoryType == ctBreakdownGroup)
				{
					GroupName = NodeData->CategoryTranslation->NewCategoryName;
					AddGroup(NodeData->CategoryTranslation);
				}
				else if (NodeData->CategoryTranslation->CategoryType == ctCategory)
				{
					AddCategory(GroupName, NodeData->CategoryTranslation);
				}
				Node = Form->vtvRename->GetNext(Node);
			}
		}
	}
	__finally
	{
		delete Form;
		Form = NULL;
	}
	return (ModalResult == mrOk);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TUpdateArchiveCategories::TUpdateArchiveCategories(AnsiString DatabaseName, TProgressBar *ProgressBar) :
		Quit(true),
		fCompleted(false),
		IBDatabase(new TIBDatabase(NULL)),
		DBControl(IBDatabase.get()),
		sqlCategories(new TIBSQL(NULL)),
		sqlArchiveCount(new TIBSQL(NULL)),
		sqlArchive(new TIBSQL(NULL)),
		sqlUpdateArchive(new TIBSQL(NULL)),
		sqlFindCategory(new TIBSQL(NULL)),
		sqlFindCategoryGroup(new TIBSQL(NULL)),
		sqlAddCategory(new TIBSQL(NULL)),
		sqlAddCategoryGroup(new TIBSQL(NULL)),
		sqlSetCategory(new TIBSQL(NULL)),
		TThread(true)
{
	IBDatabase->Params->Add("user_name=SYSDBA");
	IBDatabase->Params->Add("password=masterkey");
	IBDatabase->DatabaseName	= DatabaseName;
	IBDatabase->LoginPrompt		= false;
	IBDatabase->Connected		= true;

	DBControl.RegisterQuery(sqlCategories);
	DBControl.RegisterQuery(sqlArchiveCount);
	DBControl.RegisterQuery(sqlArchive);
	DBControl.RegisterQuery(sqlUpdateArchive);
	DBControl.RegisterQuery(sqlFindCategory);
	DBControl.RegisterQuery(sqlFindCategoryGroup);
	DBControl.RegisterQuery(sqlAddCategory);
	DBControl.RegisterQuery(sqlAddCategoryGroup);
	DBControl.RegisterQuery(sqlSetCategory);

	sqlCategories->SQL->Text =
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name Category_Group_Name,"
			"ArcCategories.Category_Key,"
			"ArcCategories.Category "
		"From "
			"ArcCategories Left Join CategoryGroups On "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Order By "
			"CategoryGroups.Name,"
			"ArcCategories.Category";

	sqlArchive->SQL->Text =
		"Select "
			"Archive.Archive_Key,"
			"Archive.Category_Key,"
			"NewCategory.Category New_Category,"
			"OldCategories.Category Old_Category "
		"From "
			"Archive Inner Join ArcCategories NewCategory on "
				"Archive.Category_Key = NewCategory.Category_Key "
			"Left Join ArcCategory on "
				"Archive.Archive_Key = ArcCategory.Archive_Key "
			"Left Join ArcCategories OldCategories on "
				"ArcCategory.Category_Key = OldCategories.Category_Key "
		"Where "
			"NewCategory.Category = 'MM_TOTAL' "
		"Order By "
			"Archive.Archive_Key,"
			"OldCategories.Category";

	sqlArchiveCount->SQL->Text =
		"Select "
			"Count(Archive.Archive_Key) "
		"From "
			"Archive Inner Join ArcCategories NewCategory on "
				"Archive.Category_Key = NewCategory.Category_Key "
			"Left Join ArcCategory on "
				"Archive.Archive_Key = ArcCategory.Archive_Key "
			"Left Join ArcCategories OldCategories on "
				"ArcCategory.Category_Key = OldCategories.Category_Key "
		"Where "
			"NewCategory.Category = 'MM_TOTAL' "
		"Order By "
			"Archive.Archive_Key,"
			"OldCategories.Category";

	sqlUpdateArchive->SQL->Text =
		"Update "
			"Archive "
		"Set "
			"Category_Key = :Category_Key "
		"Where "
			"Archive_Key = :Archive_Key";

	sqlFindCategory->SQL->Text =
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name Category_Group_Name,"
			"ArcCategories.Category_Key,"
			"ArcCategories.Category "
		"From "
			"ArcCategories Left Join CategoryGroups On "
				"ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGroups_Key "
		"Where "
			"Upper(ArcCategories.Category) = :UpperName";

	sqlFindCategoryGroup->SQL->Text =
		"Select "
			"CategoryGroups.CategoryGroups_Key,"
			"CategoryGroups.Name Category_Group_Name "
		"From "
			"CategoryGroups "
		"Where "
			"Upper(CategoryGroups.Name) = :UpperName";

	sqlAddCategory->SQL->Text =
		"Insert Into ArcCategories ("
			"Category_Key,"
			"Category,"
			"CategoryGroups_Key) "
		"Values ("
			":Category_Key,"
			":Category,"
			":CategoryGroups_Key)";

	sqlAddCategoryGroup->SQL->Text =
		"Insert Into CategoryGroups ("
			"CategoryGroups_Key,"
			"Name,"
			"Visible) "
		"Values ("
			":CategoryGroups_Key,"
			":Name,"
			"'T')";

	sqlSetCategory->SQL->Text =
		"Update "
			"ArcCategories "
		"Set "
			"CategoryGroups_Key = :CategoryGroups_Key, "
			"Category = :CategoryName "
		"Where "
			"Category_Key = :Category_Key";

	fProgressBar = ProgressBar;
}
//---------------------------------------------------------------------------
__fastcall TUpdateArchiveCategories::~TUpdateArchiveCategories()
{
	Terminate();
	while (!Quit)
	{
	}
}
//---------------------------------------------------------------------------
bool TUpdateArchiveCategories::StartUpdate()
{
	DBControl.StartTransaction();

	sqlCategories->Close();
	int LastGroupKey = 0;
	for (sqlCategories->ExecQuery(); !sqlCategories->Eof; sqlCategories->Next())
	{
		if (LastGroupKey == 0 || sqlCategories->FieldByName("CategoryGroups_Key")->AsInteger != LastGroupKey)
		{
			UpdateCategories.AddCurrentGroup(sqlCategories->FieldByName("Category_Group_Name")->AsString);
			LastGroupKey = sqlCategories->FieldByName("CategoryGroups_Key")->AsInteger;
		}
		UpdateCategories.AddCurrentCategory(sqlCategories->FieldByName("Category_Group_Name")->AsString,
														sqlCategories->FieldByName("Category")->AsString);
		UpdateCategories.AddOldCategory(sqlCategories->FieldByName("Category")->AsString);
	}
	DBControl.CommitTransaction();
	if (UpdateCategories.Show())
	{
		InitializeCategoryLookup();
		FreeOnTerminate	= false;
		Quit					= false;
		Priority				= tpLower;
		Resume();
		return true;
	}
	else
	{
		Quit = true;
		return false;
	}
}
//---------------------------------------------------------------------------
void TUpdateArchiveCategories::PauseUpdate()
{
	Suspend();
}
//---------------------------------------------------------------------------
void __fastcall TUpdateArchiveCategories::Execute()
{
	try
	{
		DBControl.StartTransaction();
		try
		{
			if (fProgressBar)
			{
				sqlArchiveCount->Close();
				sqlArchiveCount->ExecQuery();
				fProgressBar->Min = 0;
				fProgressBar->Max = sqlArchiveCount->Fields[0]->AsInteger;
				fProgressBar->Position = 0;
				sqlArchiveCount->Close();
			}
			sqlArchive->Close();
			sqlArchive->ExecQuery();

			unsigned int Clicks = GetTickCount();

			while (!Terminated && !sqlArchive->Eof)
			{
				int LastCategoryPriority	= -1;
				int CurrentArchiveKey		= sqlArchive->FieldByName("Archive_Key")->AsInteger;
				AnsiString ReplacementName;
				while (sqlArchive->FieldByName("Archive_Key")->AsInteger == CurrentArchiveKey && !sqlArchive->Eof)
				{
					AnsiString OldCategory = sqlArchive->FieldByName("Old_Category")->AsString;
					CategoryTranslation::TCategoryType CategoryType = UpdateCategories.GetCategoryType(OldCategory);
					if (CategoryType == CategoryTranslation::ctCategory)
					{
						AnsiString NewCategoryName = UpdateCategories.GetNewCategoryName(OldCategory);
						int GroupIndex, CategoryIndex;

						int Priority = UpdateCategories.GetCategoryPriority(OldCategory);
						if (Priority > LastCategoryPriority && Priority > -1)
						{
							ReplacementName		= NewCategoryName;
							LastCategoryPriority	= Priority;
						}
					}
					sqlArchive->Next();
					++ProgressPos;
				}
				if (ReplacementName != "")
				{
					TiCategories iCategories = Categories.find(ReplacementName.UpperCase());
					if (iCategories != Categories.end())
					{
						sqlUpdateArchive->Close();
						sqlUpdateArchive->ParamByName("Archive_Key")->AsInteger = CurrentArchiveKey;
						sqlUpdateArchive->ParamByName("Category_Key")->AsInteger = iCategories->second.CategoryKey;
						sqlUpdateArchive->ExecQuery();
					}
					else
					{
						throw Exception("Unable to find category.");
					}
				}
				if (GetTickCount() > (Clicks + 1000))
				{
					Synchronize(UpdateGUI);
					Clicks = GetTickCount();
				}
			}
			DBControl.CommitTransaction();
			ProgressPos = 0;
			Synchronize(UpdateGUI);
		}
		catch (Exception &E)
		{
			DBControl.UndoTransaction();
			throw;
		}
	}
	__finally
	{
		Quit = true;
	}
	// Don't complete if an exception is thrown
	fCompleted = true;
}
//---------------------------------------------------------------------------
void __fastcall TUpdateArchiveCategories::UpdateGUI()
{
	if (fProgressBar)
	{
		fProgressBar->Position = ProgressPos;
	}
}
//---------------------------------------------------------------------------
void TUpdateArchiveCategories::InitializeCategoryLookup()
{
	DBControl.StartTransaction();
	try
	{
		for (int i=0; i<UpdateCategories.GroupCount(); i++)
		{
			CategoryTranslation::TCategoryTranslation *CatGroupTrans = UpdateCategories.GetGroup(i);

			for (int j=0; j<UpdateCategories.CategoryCount(i); j++)
			{
				CategoryTranslation::TCategoryTranslation *CatTrans = UpdateCategories.GetCategory(i, j);

				TCategory Category;
				Category.CategoryName = CatTrans->NewCategoryName;
				Category.Group.GroupName = CatGroupTrans->NewCategoryName;

				sqlFindCategoryGroup->Close();
				sqlFindCategoryGroup->ParamByName("UpperName")->AsString = Category.Group.GroupName.UpperCase();
				sqlFindCategoryGroup->ExecQuery();
				if (sqlFindCategoryGroup->Eof)
				{
					int GroupKey = DBControl.GetGeneratorVal("Gen_CategoryGroups");

					sqlAddCategoryGroup->Close();
					sqlAddCategoryGroup->ParamByName("CategoryGroups_Key")->AsInteger	= GroupKey;
					sqlAddCategoryGroup->ParamByName("Name")->AsString						= Category.Group.GroupName;
					sqlAddCategoryGroup->ExecQuery();

					Category.Group.GroupKey = GroupKey;
				}
				else
				{
					Category.Group.GroupKey = sqlFindCategoryGroup->FieldByName("CategoryGroups_Key")->AsInteger;
				}
				sqlFindCategory->Close();
				sqlFindCategory->ParamByName("UpperName")->AsString = Category.CategoryName.UpperCase();
				sqlFindCategory->ExecQuery();
				if (sqlFindCategory->Eof)
				{
					int CategoryKey = DBControl.GetGeneratorVal("Gen_ArcCategories");

					sqlAddCategory->Close();
					sqlAddCategory->ParamByName("Category_Key")->AsInteger			= CategoryKey;
					sqlAddCategory->ParamByName("Category")->AsString					= Category.CategoryName;
					sqlAddCategory->ParamByName("CategoryGroups_Key")->AsInteger	= Category.Group.GroupKey;
					sqlAddCategory->ExecQuery();

					Category.CategoryKey = CategoryKey;
				}
				else
				{
					Category.CategoryKey = sqlFindCategory->FieldByName("Category_Key")->AsInteger;
				}
				sqlSetCategory->Close();
				sqlSetCategory->ParamByName("Category_Key")->AsInteger			= Category.CategoryKey;
				sqlSetCategory->ParamByName("CategoryGroups_Key")->AsInteger	= Category.Group.GroupKey;
				sqlSetCategory->ParamByName("CategoryName")->AsString				= Category.CategoryName;
				sqlSetCategory->ExecQuery();

				Categories[Category.CategoryName.UpperCase()] = Category;
			}
		}
		DBControl.CommitTransaction();
	}
	catch (Exception &E)
	{
		DBControl.UndoTransaction();
		throw;
	}
}
//---------------------------------------------------------------------------

