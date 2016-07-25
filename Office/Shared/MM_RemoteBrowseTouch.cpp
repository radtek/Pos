//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MM_RemoteBrowseTouch.h"
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
using namespace RemoteBrowser;
//---------------------------------------------------------------------------
TRBClientTouchGUI::TRBClientTouchGUI(TBrowseType BrowseType) :
		TRBClientInterface(BrowseType),
		Form(new TRemoteBrowseTouchForm(*this)),
		fInitialisingPath(false),
		fCurrentLevel(NULL),
		Tree(NULL, NULL)
{
	LargeIcons		= Form->TreeImages;
	fCurrentLevel	= &Tree;
}
//---------------------------------------------------------------------------
void TRBClientTouchGUI::Initialise()
{
	fInitialisingPath	= true;
	InitialiseIcons(isLarge);
}
//---------------------------------------------------------------------------
void TRBClientTouchGUI::Loading()
{
	Screen->Cursor = crHourGlass;
}
//---------------------------------------------------------------------------
void TRBClientTouchGUI::Loaded()
{
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
bool TRBClientTouchGUI::Show()
{
	if (Form->ShowModal() == mrOk)
	{
		fPath = Form->Path;
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
void TRBClientTouchGUI::LoadDrives()
{
	TRBTreeNode *ComputerNode								= new TRBTreeNode(&Tree, new TNodeSettings);
//	TRBTreeNode *ComputerNode								= new TRBTreeNode;
//	ComputerNode->Parent										= &Tree;
//	ComputerNode->NodeSettings								= new TNodeSettings;

	ComputerNode->NodeSettings->NodeType				= ntMyComputer;
	ComputerNode->NodeSettings->DisplayName			= "Browsing " + fHostName;
	ComputerNode->NodeSettings->IconIndex				= MyComputerIconIndex;
	ComputerNode->NodeSettings->SelectedIconIndex	= MyComputerIconIndex;

	TRBTreeNode *InitialNode = ComputerNode;

	Tree.Nodes.push_back(ComputerNode);

	TRBNodes Nodes;
	Expand(NULL, Nodes);
	LoadIcons(Nodes, isLarge);

	for (unsigned int i=0; i<Nodes.size(); i++)
	{
		TRBTreeNode *DriveNode			= new TRBTreeNode(ComputerNode, Nodes[i]);
//		DriveNode->NodeSettings			= Nodes[i];

//		DriveNode->Parent					= ComputerNode;
		ComputerNode->Nodes.push_back(DriveNode);

		if (fInitialisingPath && fPath != "")
		{
			AnsiString DriveToFind = ExtractFileDrive(fPath);
			AnsiString CurrentPath = Nodes[i]->Path;
			if (fPath == CurrentPath)
			{
				InitialNode			= DriveNode;
				fInitialisingPath	= false;
			}
			else if (ExtractFileDrive(CurrentPath) == DriveToFind && DriveToFind != "")
			{
				InitialNode			= DriveNode;
			}
		}
	}
	fExpand(InitialNode);

	fInitialisingPath = false;
}
//---------------------------------------------------------------------------
void TRBClientTouchGUI::SetCaption(const AnsiString &Caption)
{
	Form->CaptionPanel->Caption = Caption;
}
//---------------------------------------------------------------------------
void TRBClientTouchGUI::fExpand(TRBTreeNode *RBTreeNode)
{
	Form->StatusCaption->Caption = " Loading...";
	Form->StatusCaption->Update();

	TRBTreeNode *AutoExpandNode = NULL;

	if (RBTreeNode->Nodes.size() == 0 && RBTreeNode->NodeSettings->RemoteNode)
	{
		TRBNodes Nodes;
		Expand(RBTreeNode->NodeSettings, Nodes);
		LoadIcons(Nodes, isLarge);
		if (Nodes.size() > 0)
		{
			for (unsigned int i=0; i<Nodes.size(); i++)
			{
				TRBTreeNode *NewNode			= new TRBTreeNode(RBTreeNode, Nodes[i]);
//				NewNode->NodeSettings		= Nodes[i];

				if (NewNode->NodeSettings->NodeType == ntFolder)
				{
					if (fInitialisingPath)
					{
						AnsiString PathToFind	= fPath;
						AnsiString CurrentPath	= Nodes[i]->Path;
						if (PathToFind != "" && PathToFind == CurrentPath)
						{
							AutoExpandNode = NewNode;
							fInitialisingPath = false;
						}
						else if (fPath.AnsiPos(CurrentPath)/* || (CurrentPath == "" && fPath.Length() >= 2 && fPath.SubString(1, 2) == "\\\\")*/)
						{
						// Will select a null path if it's a unc destination.
							AutoExpandNode = NewNode;
						}
					}
				}
//				NewNode->Parent = RBTreeNode;
				RBTreeNode->Nodes.push_back(NewNode);
			}
		}
	}
	if (AutoExpandNode)
	{
		fExpand(AutoExpandNode);
	}
	else 
	{
		fCurrentLevel = RBTreeNode;
		fUpdateGUI();
	}
}
//---------------------------------------------------------------------------
void TRBClientTouchGUI::fUpdateGUI()
{
	Form->FolderList->Items->Clear();
	Form->FilesList->Items->Clear();
	if (fCurrentLevel)
	{
		Form->FolderList->Items->BeginUpdate();
		Form->FilesList->Items->BeginUpdate();
		int BackIndex = -1;
		if (fCurrentLevel->Parent != NULL && fCurrentLevel->Parent->Parent != NULL/* && fCurrentLevel->Parent->NodeSettings != NULL*/)
		{
			if (fCurrentLevel->Parent->NodeSettings->Path == "")
			{
				BackIndex = Form->FolderList->Items->AddObject(fCurrentLevel->Parent->NodeSettings->DisplayName, reinterpret_cast<TObject *>(fCurrentLevel->Parent));
			}
			else
			{
				BackIndex = Form->FolderList->Items->AddObject(fCurrentLevel->Parent->NodeSettings->Path, reinterpret_cast<TObject *>(fCurrentLevel->Parent));
			}
			// Cant go back to the root node. The root is a single node representing the PC.
//			Form->FolderList->Items->AddObject(".. [" + fCurrentLevel->Parent->NodeSettings->DisplayName + "]", reinterpret_cast<TObject *>(fCurrentLevel->Parent));
		}
		TRBTreeNode::TRBTreeNodes::iterator iNode = fCurrentLevel->Nodes.begin();
		for (; iNode!=fCurrentLevel->Nodes.end(); ++iNode)
		{
			TRBTreeNode *Node = *iNode;
			if (Node->NodeSettings)
			{
				if (Node->NodeSettings->NodeType == ntFile)
				{
					Form->FilesList->Items->AddObject(Node->NodeSettings->DisplayName, reinterpret_cast<TObject *>(Node));
				}
				else
				{
					Form->FolderList->Items->AddObject(Node->NodeSettings->DisplayName, reinterpret_cast<TObject *>(Node));
				}
				if (Node == fCurrentLevel->LastVisible)
				{
					Form->FolderList->ItemIndex = Form->FolderList->Count - 1;
				}
			}
		}
		if (BackIndex > -1)
		{
			int PixelWidth = Form->FolderList->ClientWidth - (LargeIcons->Width + 8) - Form->FolderList->Canvas->TextWidth(".. []");
			AnsiString Caption = ".. [" + MinimizeName(Form->FolderList->Items->Strings[BackIndex], Form->FolderList->Canvas, PixelWidth) + "]";
			Form->FolderList->Items->Strings[BackIndex] = Caption;
		}
		Form->FolderList->Items->EndUpdate();
		Form->FilesList->Items->EndUpdate();
		Form->HostLabel->Caption = fHostName;
		if (fCurrentLevel->NodeSettings)
		{
			if (fCurrentLevel->NodeSettings->Path == "")
			{
				Form->StatusCaption->Caption = MinimizeName(" " + fCurrentLevel->NodeSettings->DisplayName, Form->Canvas, Form->StatusCaption->ClientWidth);
			}
			else
			{
				Form->StatusCaption->Caption = MinimizeName(" " + fCurrentLevel->NodeSettings->Path, Form->Canvas, Form->StatusCaption->ClientWidth);
			}
		}
		else
		{
			Form->StatusCaption->Caption = " Browsing " + fHostName;
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TRemoteBrowseTouchForm::TRemoteBrowseTouchForm(TRBClientTouchGUI &RBClientTouchGUI) :
		TForm(static_cast<TComponent*>(NULL)),
		fRBClientTouchGUI(RBClientTouchGUI)
{
	fResizeWindow.Load(this, Panel1);

	FolderList->ControlStyle >> csDoubleClicks;
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::WMSize(TWMSize& Message)
{
	TForm::Dispatch(&Message);

	if (fResizeWindow.Execute(Message.Width, Message.Height))
	{
		FolderList->ItemHeight	= FolderList->Height / 13;
		FilesList->ItemHeight	= FolderList->ItemHeight;
	}
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::FilesListClick(TObject *Sender)
{
	if (fRBClientTouchGUI.fBrowseType == btFiles && FilesList->ItemIndex > -1)
	{
		TRBTreeNode *Node = reinterpret_cast<TRBTreeNode *>(FilesList->Items->Objects[FilesList->ItemIndex]);
		StatusCaption->Caption = MinimizeName(" " + Node->NodeSettings->Path, Canvas, StatusCaption->ClientWidth);
	}
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::FolderListClick(TObject *Sender)
{
	TRBTreeNode *Node = reinterpret_cast<TRBTreeNode *>(FolderList->Items->Objects[FolderList->ItemIndex]);
	if (fRBClientTouchGUI.fCurrentLevel && Node)
	{
		Node->LastVisible = fRBClientTouchGUI.fCurrentLevel;
	}
	Screen->Cursor = crHourGlass;
	try
	{
		fRBClientTouchGUI.fExpand(Node);
	}
	__finally
	{
		Screen->Cursor = crDefault;
	}
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::TouchBtn8MouseClick(
		TObject *Sender)
{
	if (FolderList->Items->Count > 0 && fRBClientTouchGUI.fCurrentLevel && fRBClientTouchGUI.fCurrentLevel->NodeSettings->NodeType != ntMyComputer)
	{
		TRBTreeNode *Node = reinterpret_cast<TRBTreeNode *>(FolderList->Items->Objects[0]);
		if (Node)
		{
			Node->LastVisible = fRBClientTouchGUI.fCurrentLevel;

			Screen->Cursor = crHourGlass;
			try
			{
				fRBClientTouchGUI.fExpand(Node);
			}
			__finally
			{
				Screen->Cursor = crDefault;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::TouchBtn3MouseClick(
		TObject *Sender)
{
	if (fRBClientTouchGUI.fCurrentLevel && fRBClientTouchGUI.fCurrentLevel->Parent)
	{
		fRBClientTouchGUI.fCurrentLevel->DeleteChildren();
		try
		{
			fRBClientTouchGUI.fExpand(fRBClientTouchGUI.fCurrentLevel);
		}
		__finally
		{
			Screen->Cursor = crDefault;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::TouchBtn1MouseClick(
		TObject *Sender)
{
	if (fRBClientTouchGUI.fBrowseType == btFiles && FilesList->ItemIndex > -1)
	{
		TRBTreeNode *Node = reinterpret_cast<TRBTreeNode *>(FilesList->Items->Objects[FilesList->ItemIndex]);
		Path = Node->NodeSettings->Path;
		ModalResult = mrOk;
	}
	else if (fRBClientTouchGUI.fBrowseType == btFolders)
	{
		Path = fRBClientTouchGUI.fCurrentLevel->NodeSettings->Path;
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::FolderListDrawItem(
		TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
{
	TListBox *ListBox = dynamic_cast<TListBox *>(Control);
	if (ListBox)
	{

		TRect TextRect = TRect(Rect.Left + fRBClientTouchGUI.LargeIcons->Width + 8, Rect.Top, Rect.Right, Rect.Bottom);
		ListBox->Canvas->FillRect(TextRect); // clear the rectangle
		TRect IconRect = TRect(Rect.Left, Rect.Top, TextRect.Left, Rect.Bottom);

		TRBTreeNode *Node = reinterpret_cast<TRBTreeNode *>(ListBox->Items->Objects[Index]);
		ListBox->Canvas->TextOut(TextRect.Left + 4, TextRect.Top, ListBox->Items->Strings[Index]);

		ListBox->Canvas->Brush->Color = ListBox->Color;
		ListBox->Canvas->FillRect(IconRect); // clear the rectangle

		int ImageTop = IconRect.Top + ((IconRect.Height() - fRBClientTouchGUI.LargeIcons->Height) / 2);
		if (ImageTop < IconRect.Top)
		{
			ImageTop = IconRect.Top;
		}
		if (State.Contains(odSelected))
		{
			fRBClientTouchGUI.LargeIcons->Draw(ListBox->Canvas, IconRect.Left + 4, ImageTop, Node->NodeSettings->SelectedIconIndex);
		}
		else
		{
			fRBClientTouchGUI.LargeIcons->Draw(ListBox->Canvas, IconRect.Left + 4, ImageTop, Node->NodeSettings->IconIndex);
		}

		if (State.Contains(odFocused))
		{
			ListBox->Canvas->DrawFocusRect(Rect);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::TouchBtn2MouseClick(
      TObject *Sender)
{
	ModalResult = mrCancel;	
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::TouchBtn6MouseClick(
      TObject *Sender)
{
	PostMessage(FilesList->Handle, WM_VSCROLL, SB_LINEUP, 0);
}  
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::TouchBtn7MouseClick(
		TObject *Sender)
{
	PostMessage(FilesList->Handle, WM_VSCROLL, SB_LINEDOWN, 0);
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::TouchBtn5MouseClick(
		TObject *Sender)
{
	PostMessage(FolderList->Handle, WM_VSCROLL, SB_LINEUP, 0);
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseTouchForm::TouchBtn4MouseClick(
		TObject *Sender)
{
	PostMessage(FolderList->Handle, WM_VSCROLL, SB_LINEDOWN, 0);
}
//---------------------------------------------------------------------------

