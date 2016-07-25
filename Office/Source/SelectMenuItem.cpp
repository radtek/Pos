//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>
#include "SelectMenuItem.h"
#include "Utilities.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TSelectMenuItem::TSelectMenuItem()
{
}
//---------------------------------------------------------------------------
void TSelectMenuItem::Clear()
{
	fCourses.clear();
	fItems.clear();
	fSelected.clear();
}
//---------------------------------------------------------------------------
int TSelectMenuItem::AddCourse(TCourse Course)
{
	fCourses.push_back(Course);
	return fCourses.size()-1;
}
//---------------------------------------------------------------------------
int TSelectMenuItem::AddItem(int CourseIndex, TItem Item)
{
	fItems[CourseIndex].push_back(Item);
	return fItems[CourseIndex].size()-1;
}
//---------------------------------------------------------------------------
int TSelectMenuItem::SelectedCount()
{
	return fSelected.size();
}
//---------------------------------------------------------------------------
void *TSelectMenuItem::Selected(int Index)
{
	return fSelected[Index].Data;
}
//---------------------------------------------------------------------------
bool TSelectMenuItem::Execute()
{
	std::auto_ptr<TfrmSelectMenuItem> Form(new TfrmSelectMenuItem(NULL));

	Form->vtvItems->Clear();
	Form->vtvItems->NodeDataSize = sizeof(TData);

	PVirtualNode		CourseNode	= NULL;
	PVirtualNode		ItemNode		= NULL;
	TData					*NodeData	= NULL;

	for (unsigned int CourseIndex=0; CourseIndex<fCourses.size(); CourseIndex++)
	{
		CourseNode										= Form->vtvItems->AddChild(NULL);
		Form->vtvItems->CheckType[CourseNode]	= ctTriStateCheckBox;
		CourseNode->CheckState						= csUncheckedNormal;
		NodeData											= static_cast<TData *>(Form->vtvItems->GetNodeData(CourseNode));
		*NodeData										= fCourses[CourseIndex];
		for (unsigned int ItemIndex=0; ItemIndex<fItems[CourseIndex].size(); ItemIndex++)
		{
			ItemNode										= Form->vtvItems->AddChild(CourseNode);
			Form->vtvItems->CheckType[ItemNode]	= ctTriStateCheckBox;
			ItemNode->CheckState						= csUncheckedNormal;
			NodeData										= static_cast<TData *>(Form->vtvItems->GetNodeData(ItemNode));
			*NodeData									= fItems[CourseIndex][ItemIndex];
		}
	}
	if (Form->ShowModal() == mrOk)
	{
		fSelected.clear();
		PVirtualNode Node = Form->vtvItems->GetFirst();
		while (Node)
		{
			if (Form->vtvItems->GetNodeLevel(Node) == 1)
			{
				if (Node->CheckState == csCheckedNormal)
				{
					TData *NodeData = static_cast<TData *>(Form->vtvItems->GetNodeData(Node));
					fSelected.push_back(*NodeData);
				}
			}
			Node = Form->vtvItems->GetNext(Node);
		}
		return fSelected.size() > 0;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TfrmSelectMenuItem::TfrmSelectMenuItem(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectMenuItem::vtvItemsGetImageIndex(
      TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind,
      TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
	switch (Kind)
	{
		case ikState:
		{
			ImageIndex = -1;
			break;
		}
		case ikNormal:
		{
			if (Sender->GetNodeLevel(Node) == 0)
			{
				ImageIndex = ICON_CLOSED_FOLDER_INDEX;
			}
			else
			{
				ImageIndex = ICON_NOTEPAD_INDEX;
			}
			break;
		}
		case ikSelected:
		{
			if (Sender->GetNodeLevel(Node) == 0)
			{
				ImageIndex = ICON_OPEN_FOLDER_INDEX;
			}
			else
			{
				ImageIndex = ICON_NOTEPAD_INDEX;
			}
			break;
			}
		default:
		{
			ImageIndex = -1;
			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectMenuItem::vtvItemsGetText(
      TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
      TVSTTextType TextType, WideString &CellText)
{
	TSelectMenuItem::TData *NodeData = static_cast<TSelectMenuItem::TData *>(Sender->GetNodeData(Node));
	if (NodeData)
	{
		CellText = NodeData->Name;
	}
	else
	{
		CellText = "";
	}
}
//---------------------------------------------------------------------------

