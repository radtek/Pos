//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <set>
#include "MM_RemoteBrowseClient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
using namespace RemoteBrowser;
//---------------------------------------------------------------------------
void TRBClientInterface::Expand(TNodeSettings *NodeSettings, TRBNodes &Nodes)
{
	fRBClient->Expand(NodeSettings, Nodes);
}
//---------------------------------------------------------------------------
void TRBClientInterface::LoadIcons(const TRBNodes &Nodes, TIconSize IconSize)
{
	fRBClient->LoadIcons(Nodes, IconSize);
}
//---------------------------------------------------------------------------
void TRBClientInterface::InitialiseIcons(TIconSize IconSize)
{
	TImageList			*Icons;
	TFileImageIndexes	*FileImageIndexes;

	if (IconSize == isLarge)
	{
		Icons					= LargeIcons;
		FileImageIndexes	= &LargeFileImageIndexes;
	}
	else
	{
		Icons					= SmallIcons;
		FileImageIndexes	= &SmallFileImageIndexes;
	}
	FileImageIndexes->clear();

	if (Icons)
	{
		Icons->Clear();

		SHFILEINFO sfi;
		int Flags;
		std::auto_ptr<TIcon> Icon(new TIcon());

		// Get the icon for the My Computer folder.
		LPMALLOC lpMalloc = NULL;
		if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
		{
			LPITEMIDLIST lpidl;
			if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &lpidl)))
			{
				if (IconSize == isLarge)	Flags = SHGFI_PIDL | SHGFI_ICON | SHGFI_LARGEICON;
				else								Flags = SHGFI_PIDL | SHGFI_ICON | SHGFI_SMALLICON;

				if (SUCCEEDED(SHGetFileInfo((LPCTSTR)lpidl,
														0,
														&sfi,
														sizeof(SHFILEINFO),
														Flags)))
				{
					Icon->Handle = sfi.hIcon;
				}
			}
			lpMalloc->Free(lpidl);
			lpMalloc->Release();
		}
		MyComputerIconIndex = Icons->AddIcon(Icon.get());
/*
		// Get the drive icon.
		if (IconSize == isLarge)	Flags = SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_OPENICON | SHGFI_LARGEICON;
		else								Flags = SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON;

		if (SUCCEEDED(SHGetFileInfo(	"C:\\",
												FILE_ATTRIBUTE_DIRECTORY,
												&sfi,
												sizeof(sfi),
												Flags)))
		{
			Icon->ReleaseHandle();
			Icon->Handle = sfi.hIcon;
		}
		DefaultFixedDriveIconIndex	= Icons->AddIcon(Icon.get());
		FixedDriveIconIndex			= DefaultFixedDriveIconIndex;
		FloppyDriveIconIndex			= DefaultFixedDriveIconIndex;
		RemovableDriveIconIndex		= DefaultFixedDriveIconIndex;
		RemoteDriveIconIndex			= DefaultFixedDriveIconIndex;
		CDROMDriveIconIndex			= DefaultFixedDriveIconIndex;
		RAMDriveIconIndex				= DefaultFixedDriveIconIndex;

		// Get the closed folder icon. The path is fictitious but shows it is a folder that is required.
		if (IconSize == isLarge)	Flags = SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_LARGEICON;
		else								Flags = SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON;

		if (SUCCEEDED(SHGetFileInfo(	"C:\\DIRECTORY",
												FILE_ATTRIBUTE_DIRECTORY,
												&sfi,
												sizeof(sfi),
												Flags)))
		{
			Icon->ReleaseHandle();
			Icon->Handle = sfi.hIcon;
		}
		DefaultFolderClosedIconIndex	= Icons->AddIcon(Icon.get());
		FolderClosedIconIndex			= DefaultFolderClosedIconIndex;
		// Get the open folder icon.
		if (IconSize == isLarge)	Flags = SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_OPENICON | SHGFI_LARGEICON;
		else								Flags = SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON;

		if (SUCCEEDED(SHGetFileInfo(	"C:\\DIRECTORY",
												FILE_ATTRIBUTE_DIRECTORY,
												&sfi,
												sizeof(sfi),
												Flags)))
		{
			Icon->ReleaseHandle();
			Icon->Handle = sfi.hIcon;
		}
		DefaultFolderOpenIconIndex	= Icons->AddIcon(Icon.get());
		FolderOpenIconIndex			= DefaultFolderOpenIconIndex;
*/
		Icon->ReleaseHandle();
	} 
}
//---------------------------------------------------------------------------
bool TRBClientInterface::IconLoaded(TIconSize IconSize, int Index) const
{
	const TFileImageIndexes *Indexes	= &SmallFileImageIndexes;
	if (IconSize == isLarge)
	{
		Indexes = &LargeFileImageIndexes;
	}
	return Indexes->find(Index) != Indexes->end();
}
//---------------------------------------------------------------------------
int TRBClientInterface::AddIcon(TIconSize IconSize, int RemoteIndex, TIcon *Icon)
{
	TImageList			*Icons;
	TFileImageIndexes	*FileImageIndexes;

	if (IconSize == isLarge)
	{
		Icons					= LargeIcons;
		FileImageIndexes	= &LargeFileImageIndexes;
	}
	else
	{
		Icons					= SmallIcons;
		FileImageIndexes	= &SmallFileImageIndexes;
	}
	int LocalIndex = -1;
	if (Icons)
	{
		LocalIndex								= Icons->AddIcon(Icon);
		(*FileImageIndexes)[RemoteIndex]	= LocalIndex;
	}
	return LocalIndex;
}
//---------------------------------------------------------------------------
int TRBClientInterface::GetLocalIconIndex(TIconSize IconSize, int RemoteIndex)
{
	TFileImageIndexes	*FileImageIndexes;
	if (IconSize == isLarge)
	{
		FileImageIndexes	= &LargeFileImageIndexes;
	}
	else
	{
		FileImageIndexes	= &SmallFileImageIndexes;
	}
	if (FileImageIndexes->find(RemoteIndex) != FileImageIndexes->end())
	{
		return (*FileImageIndexes)[RemoteIndex];
	}
	else
	{
		return -1;
	}
}
//---------------------------------------------------------------------------
/*void TRBClientInterface::GetIcon(const TNodeSettings *NodeSettings, int RemoteIndex, TIcon *Icon, int &LocalIndex)
{
	TImageList *Icons				= SmallIcons;
	TFileImageIndexes *Indexes	= &SmallFileImageIndexes;

	if (NodeSettings->IconSize == isLarge)
	{
		Icons	= LargeIcons;
		Indexes		= &LargeFileImageIndexes;
	}
	TFileImageIndexes::iterator iImage = Indexes->find(RemoteIndex);
	if (iImage == Indexes->end())
	{
		if (Icon)
		{
			LocalIndex					= Icons->AddIcon(Icon);
			(*Indexes)[RemoteIndex]	= LocalIndex;
		}
	}
	else
	{
		LocalIndex = iImage->second;
	}
}*/
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*TRBClientGUI::TRBClientGUI() :
		TRBClientInterface(),
		Form(new TRemoteBrowseForm),
		fInitialisingPath(false)
{
	SmallIcons = Form->imlTreeImages;
}
//---------------------------------------------------------------------------
void TRBClientGUI::Initialise()
{
	fInitialisingPath = true;
	Form->TreeView1->Items->Clear();

	InitialiseIcons(isSmall);

	Form->TreeView1->OnExpanding = fTreeExpanding;
	Form->Label1->Caption = "You are currently browsing the machine \"" + fHostName + "\".";
}
//---------------------------------------------------------------------------
void TRBClientGUI::Loading()
{
	Screen->Cursor = crHourGlass;
}
//---------------------------------------------------------------------------
void TRBClientGUI::Loaded()
{
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
bool TRBClientGUI::Show()
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
void TRBClientGUI::LoadDrives()
{
	TTreeNode *ComputerNode			= Form->TreeView1->Items->AddChild(NULL, fHostName);
	ComputerNode->Data				= NULL;
	ComputerNode->ImageIndex		= MyComputerIconIndex;
	ComputerNode->SelectedIndex	= MyComputerIconIndex;

	TRBNodes Nodes;
	Expand(NULL, Nodes);

	ComputerNode->HasChildren = (Nodes.size() > 0);

	for (unsigned int i=0; i<Nodes.size(); i++)
	{
		TTreeNode *NewNode				= Form->TreeView1->Items->AddChild(ComputerNode, Nodes[i]->DisplayName);
		NewNode->Data						= (void *)Nodes[i];
		NewNode->HasChildren				= Nodes[i]->HasSubFolders;

		switch (Nodes[i]->NodeType)
		{
			case ntFloppyDrive:			if (Nodes[i]->Icon)
												{
													FloppyDriveIconIndex		= SmallIcons->AddIcon(Nodes[i]->Icon);
												}
												NewNode->ImageIndex			= FloppyDriveIconIndex;
												NewNode->SelectedIndex		= FloppyDriveIconIndex;
												break;
			case ntRemovableDrive:		if (Nodes[i]->Icon)
												{
													RemovableDriveIconIndex	= SmallIcons->AddIcon(Nodes[i]->Icon);
												}
												NewNode->ImageIndex			= RemovableDriveIconIndex;
												NewNode->SelectedIndex		= RemovableDriveIconIndex;
												break;
			case ntFixedDrive:			if (Nodes[i]->Icon)
												{
													FixedDriveIconIndex		= SmallIcons->AddIcon(Nodes[i]->Icon);
												}
												NewNode->ImageIndex			= FixedDriveIconIndex;
												NewNode->SelectedIndex		= FixedDriveIconIndex;
												break;
			case ntRemoteDrive:			if (Nodes[i]->Icon)
												{
													RemoteDriveIconIndex		= SmallIcons->AddIcon(Nodes[i]->Icon);
												}
												NewNode->ImageIndex			= RemoteDriveIconIndex;
												NewNode->SelectedIndex		= RemoteDriveIconIndex;
												break;
			case ntCDROMDrive:			if (Nodes[i]->Icon)
												{
													CDROMDriveIconIndex		= SmallIcons->AddIcon(Nodes[i]->Icon);
												}
												NewNode->ImageIndex			= CDROMDriveIconIndex;
												NewNode->SelectedIndex		= CDROMDriveIconIndex;
												break;
			case ntRAMDrive:				if (Nodes[i]->Icon)
												{
													RAMDriveIconIndex			= SmallIcons->AddIcon(Nodes[i]->Icon);
												}
												NewNode->ImageIndex			= RAMDriveIconIndex;
												NewNode->SelectedIndex		= RAMDriveIconIndex;
												break;
		}
	}
	ComputerNode->Expanded = true;
}
//---------------------------------------------------------------------------
void __fastcall TRBClientGUI::fTreeExpanding(TObject *Sender, TTreeNode *Node, bool &AllowExpansion)
{
	TNodeSettings *NodeSettings = ((TNodeSettings *)Node->Data);
	if (NodeSettings && Node->Count == 0)
	{
		TRBNodes Nodes;
		Expand(NodeSettings, Nodes);

		Node->HasChildren = (Nodes.size() > 0);

		for (unsigned int i=0; i<Nodes.size(); i++)
		{
			TTreeNode *NewNode			= Form->TreeView1->Items->AddChild(Node, Nodes[i]->DisplayName);
			NewNode->Data					= (void *)Nodes[i];
			NewNode->HasChildren			= Nodes[i]->HasSubFolders;

			if (Nodes[i]->NodeType == ntFolder)
			{
				NewNode->ImageIndex		= FolderClosedIconIndex;
				NewNode->SelectedIndex	= FolderOpenIconIndex;

				if (fInitialisingPath)
				{
					if (ExtractFilePath(fPath) == Nodes[i]->Path)
					{
						NewNode->MakeVisible();
						fInitialisingPath = false;
					}
					else if (fPath.AnsiPos(ExtractFilePath(Nodes[i]->Path)))
					{
						NewNode->MakeVisible();
					}
				}
			}
			else if (Nodes[i]->NodeType == ntFile)
			{
				TFileImageIndexes::iterator iIndex = SmallFileImageIndexes.find(Nodes[i]->FileExtension);
				if (iIndex == SmallFileImageIndexes.end())
				{
					int Index = SmallIcons->AddIcon(Nodes[i]->Icon);
					SmallFileImageIndexes[Nodes[i]->FileExtension] = Index;

					NewNode->ImageIndex		= Index;
					NewNode->SelectedIndex	= Index;
				}
				else
				{
					NewNode->ImageIndex		= iIndex->second;
					NewNode->SelectedIndex	= iIndex->second;
				}
			}
		}
	}
} */
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TRBClient::TRBClient(TRBClientInterface *RBClientInterface, const AnsiString &Caption,
	const AnsiString &IP, int Port, TBrowseFilter BrowseFilter, const AnsiString &ExtFilter) :
		fRBClientInterface(RBClientInterface),
		fTCPClient(new TIdTCPClient(NULL)),
		fExtFilter(ExtFilter),
		fBrowseFilter(BrowseFilter)
{
	fRBClientInterface->fRBClient	= this;
	fTCPClient->Host					= IP;
	fTCPClient->Port					= Port;

	fRBClientInterface->SetCaption(Caption);
}
//---------------------------------------------------------------------------
TRBClient::~TRBClient()
{
	fTCPClient->Disconnect();
}
//---------------------------------------------------------------------------
void TRBClient::SendTCPMessage(TSerialData &Data)
{
	Data.Compress(6);
	fTCPClient->WriteStream(Data.Stream(), true, true);
}
//---------------------------------------------------------------------------
bool TRBClient::ReceiveTCPMessage(TSerialData &Data)
{
	bool Success = false;

	fTCPClient->ReadStream(Data.Stream(), -1, false);
	Data.Decompress();

	Data >> Success;
	return Success;
}
//---------------------------------------------------------------------------
bool TRBClient::Execute()
{
	fRBClientInterface->Loading();
	try
	{
		fRBClientInterface->Initialise(fTCPClient->Host, Path);
		fRBClientInterface->Initialise();
		ResetServer();
		fRBClientInterface->LoadDrives();
	}
	__finally
	{
		fRBClientInterface->Loaded();
	}
	if (fRBClientInterface->Show())
	{
		Path = fRBClientInterface->Path();
		ResetServer();
		return true;
	}
	else
	{
		ResetServer();
		return false;
	}
}
//---------------------------------------------------------------------------
void TRBClient::Expand(TNodeSettings *NodeSettings, TRBNodes &Nodes)
{
	Nodes.clear();
	TSerialData OutData;
	TSerialData InData;

	if (NodeSettings)
	{
		OutData << MSG_RBRWS_FOLDERS;
		OutData << reinterpret_cast<void*>(this);
		OutData << NodeSettings->RemoteNode;
		OutData << fExtFilter;
	}
	else
	{
		OutData << MSG_RBRWS_DRIVES;
		OutData << reinterpret_cast<void*>(this);
		OutData << fBrowseFilter;
	}
	OutData << fRBClientInterface->fIconSize;

	fTCPClient->Connect();
	try
	{
		SendTCPMessage(OutData);

		TSerialData InData;
		if (ReceiveTCPMessage(InData))
		{
			int NodesCount = 0;
			InData >> NodesCount;
			for (int i=0; i<NodesCount; i++)
			{
				TNodeSettings *NodeSettings = new TNodeSettings;
				*NodeSettings << InData;
				Nodes.push_back(NodeSettings);
			}
		}
	}
	__finally
	{
		fTCPClient->Disconnect();
	}
}
//---------------------------------------------------------------------------
void TRBClient::LoadIcons(const TRBNodes &Nodes, TIconSize IconSize)
{
	std::set<int> IconIndexes;

	for (unsigned int i=0; i<Nodes.size(); i++)
	{
		if (!fRBClientInterface->IconLoaded(Nodes[i]->IconSize, Nodes[i]->IconIndex))
		{
			IconIndexes.insert(Nodes[i]->IconIndex);
		}
		if (!fRBClientInterface->IconLoaded(Nodes[i]->IconSize, Nodes[i]->SelectedIconIndex))
		{
			IconIndexes.insert(Nodes[i]->SelectedIconIndex);
		}
	}
	if (IconIndexes.size() > 0)
	{
		TSerialData OutData;
		TSerialData InData;

		OutData << MSG_RBRWS_ICONS;
		OutData << IconSize;
		OutData << static_cast<int>(IconIndexes.size());

		for (std::set<int>::iterator iIndexes=IconIndexes.begin(); iIndexes!=IconIndexes.end(); ++iIndexes)
		{
			OutData << *iIndexes;
		}
		fTCPClient->Connect();
		try
		{
			SendTCPMessage(OutData);

			TSerialData InData;
			if (ReceiveTCPMessage(InData))
			{
				for (std::vector<int>::size_type i=0; i<IconIndexes.size(); i++)
				{
					int RemoteIndex;
					InData >> RemoteIndex;

					std::auto_ptr<TIcon> Icon(new TIcon);
					InData >> Icon.get();

					fRBClientInterface->AddIcon(IconSize, RemoteIndex, Icon.get());
				}
			}
		}
		__finally
		{
			fTCPClient->Disconnect();
		}
	}
	for (unsigned int i=0; i<Nodes.size(); i++)
	{
		Nodes[i]->IconIndex = fRBClientInterface->GetLocalIconIndex(isLarge, Nodes[i]->IconIndex);
		Nodes[i]->SelectedIconIndex = fRBClientInterface->GetLocalIconIndex(isLarge, Nodes[i]->SelectedIconIndex);
	}
}
//---------------------------------------------------------------------------
void TRBClient::ResetServer()
{
	TSerialData OutData;
	TSerialData InData;

	OutData << MSG_RBRWS_RESET;
	OutData << reinterpret_cast<void*>(this);
	fTCPClient->Connect();
	try
	{
		SendTCPMessage(OutData);

		TSerialData InData;
		if (ReceiveTCPMessage(InData))
		{
		}
	}
	__finally
	{
		fTCPClient->Disconnect();
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TRemoteBrowseForm::TRemoteBrowseForm()
	: TForm((TComponent*)NULL)
{
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseForm::TreeView1Deletion(TObject *Sender,
		TTreeNode *Node)
{
	delete (TNodeSettings *)Node->Data;
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseForm::BitBtn1Click(TObject *Sender)
{
	if (TreeView1->Selected)
	{
		TNodeSettings *NodeSettings = ((TNodeSettings *)TreeView1->Selected->Data);
		Path = NodeSettings->Path;
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TRemoteBrowseForm::TreeView1DblClick(TObject *Sender)
{
	TPoint pt = TreeView1->ScreenToClient(Mouse->CursorPos);
	TTreeNode *Node = TreeView1->GetNodeAt(pt.x, pt.y);
	if (Node)
	{
		TNodeSettings *NodeSettings = ((TNodeSettings *)Node->Data);
		if (NodeSettings && NodeSettings->HasSubFolders)
		{
			Node->HasChildren = true;
			Node->Expanded = true;
		}
	}
}
//---------------------------------------------------------------------------

