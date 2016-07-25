//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MM_RemoteBrowser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
using namespace RemoteBrowser;
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TNodeSettings::TNodeSettings(const TNodeSettings &Rhs) : Icon(NULL)
{
	operator = (Rhs);
}
//---------------------------------------------------------------------------
TNodeSettings & TNodeSettings::operator = (const TNodeSettings &Rhs)
{
	DisplayName		= Rhs.DisplayName;
	Path				= Rhs.Path;
	HasSubFolders	= Rhs.HasSubFolders;
	NodeType			= Rhs.NodeType;
	FileExtension	= Rhs.FileExtension;
	if (Rhs.Icon)
	{
		Icon			= new TIcon;
		Icon->Assign(Rhs.Icon);
	}
	return *this;
}
//---------------------------------------------------------------------------
void TNodeSettings::operator >> (TSerialData &SerialData) const
{
	SerialData << DisplayName;
	SerialData << Path;
	SerialData << HasSubFolders;
	SerialData << NodeType;
	SerialData << FileExtension;
	if (Icon)
	{
		SerialData << true;
		SerialData << Icon;
	}
	else
	{
		SerialData << false;
	}
}
//---------------------------------------------------------------------------
TNodeSettings& TNodeSettings::operator << (const TSerialData &SerialData)
{
	SerialData >> DisplayName;
	SerialData >> Path;
	SerialData >> HasSubFolders;
	SerialData >> NodeType;
	SerialData >> FileExtension;

	bool ContainsIcon;
	SerialData >> ContainsIcon;
	if (ContainsIcon)
	{
		Icon = new TIcon();
		SerialData >> Icon;
	}
	return *this;
}
//---------------------------------------------------------------------------
TRBServer::TRBServer(int Port) : fTCPServer(new TIdTCPServer(NULL)),
		SentFixedDriveIcon(false),
		SentFloppyDriveIcon(false),
		SentRemovableDriveIcon(false),
		SentRemoteDriveIcon(false),
		SentCDROMDriveIcon(false),
		SentRAMDriveIcon(false)
{
	fTCPServer->OnExecute		= fTCPExecute;
	fTCPServer->DefaultPort		= Port;
	fTCPServer->Active			= true;
}
//---------------------------------------------------------------------------
TRBServer::~TRBServer()
{
	fTCPServer->Active = false;
}
//---------------------------------------------------------------------------
void TRBServer::SendTCPMessage(TIdTCPServerConnection *Connection, TSerialData &Data)
{
	Data.Compress(6);
//	Data.Encrypt();
	Connection->WriteStream(Data.Stream(), true, true);
}
//---------------------------------------------------------------------------
bool TRBServer::ReceiveTCPMessage(TIdTCPServerConnection *Connection, TSerialData &Data)
{
	Connection->ReadStream(Data.Stream(), -1, false);
	Data.Decompress();
//	Data.Stream()->Position = 0;
	return true;//Data.Decrypt();
}
//---------------------------------------------------------------------------
void __fastcall TRBServer::fTCPExecute(TIdPeerThread *AThread)
{
	TSerialData OutData;
	TSerialData InData;

	try
	{
		if (ReceiveTCPMessage(AThread->Connection, InData))
		{
			bool Success = false;

			int PacketType=0;
			InData >> PacketType;

			switch (PacketType)
			{
				case MSG_RBRWS_DRIVES:		Success = GetDrives(InData, OutData);		break;
				case MSG_RBRWS_FOLDERS:		Success = GetFolders(InData, OutData);		break;
				case MSG_RBRWS_FILES:		Success = GetFiles(InData, OutData);		break;
			}
			if (!Success)
			{
				OutData << false; // Unsuccessful
			}
			SendTCPMessage(AThread->Connection, OutData);
		}
	}
	__finally
	{
		AThread->Connection->Disconnect();
	}
}
//---------------------------------------------------------------------------
bool TRBServer::GetDrives(const TSerialData &InData, TSerialData &OutData)
{
	SendFileImages.clear();
	SentFixedDriveIcon		= false;
	SentFloppyDriveIcon		= false;
	SentRemovableDriveIcon	= false;
	SentRemoteDriveIcon		= false;
	SentCDROMDriveIcon		= false;
	SentRAMDriveIcon			= false;

	TNodeType NodeTypes;
	InData >> NodeTypes;

	std::vector<TNodeSettings> Nodes;

	char DriveBuffer[MAX_PATH] = {0};
	char NameBuffer[MAX_PATH] = {0};
	int Index = 0;

	if (GetLogicalDriveStrings(MAX_PATH, DriveBuffer))
	{
		while (DriveBuffer[Index])
		{
			char *Drive = DriveBuffer + Index;

			TNodeSettings NodeSettings;

			NodeSettings.DisplayName	= AnsiString(Drive).SubString(1, 2);
			NodeSettings.Path				= Drive;
			NodeSettings.HasSubFolders	= true;

			bool *Sent = NULL;

			switch (GetDriveType(Drive))
			{
				case DRIVE_REMOVABLE:	NodeSettings.NodeType	= ntRemovableDrive;	Sent = &SentRemovableDriveIcon;	break;
				case DRIVE_FIXED:			NodeSettings.NodeType	= ntFixedDrive;		Sent = &SentFixedDriveIcon;		break;
				case DRIVE_REMOTE:		NodeSettings.NodeType	= ntRemoteDrive;		Sent = &SentRemoteDriveIcon;		break;
				case DRIVE_CDROM:			NodeSettings.NodeType	= ntCDROMDrive;		Sent = &SentCDROMDriveIcon;		break;
				case DRIVE_RAMDISK:		NodeSettings.NodeType	= ntRAMDrive;			Sent = &SentRAMDriveIcon;			break;
				default:						NodeSettings.NodeType	= ntUnknown;			Sent = NULL;							break;
			}
			NameBuffer[0] = NULL;

			// Get a description for the drive
			DWORD CurrErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
			try
			{
				if (NodeSettings.NodeType == ntRemovableDrive)
				{
					// Default to ...
					strcpy(NameBuffer, "Removable Disk");

					// Find out if it's a floppy drive
					AnsiString drive = "\\\\.\\" + AnsiString(Drive).SubString(1, 2);
					HANDLE HDrive = CreateFile(drive.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);

					if (HDrive != INVALID_HANDLE_VALUE )
					{
						DISK_GEOMETRY Geometry[20];
						DWORD BytesReturned;
						if (DeviceIoControl(HDrive, IOCTL_STORAGE_GET_MEDIA_TYPES, NULL, 0, Geometry, sizeof(Geometry), &BytesReturned, NULL))
						{
							if (Geometry[0].MediaType >= F5_1Pt2_512 && Geometry[0].MediaType <=F5_160_512)
							{
								NodeSettings.NodeType = ntFloppyDrive;
								Sent = &SentFloppyDriveIcon;
								strcpy(NameBuffer, "Floppy");
							}
						}
					}
				}
				else if (NodeSettings.NodeType == ntRemoteDrive)
				{
					// Mapped netword drive. Find where it maps to.
					DWORD Length = MAX_PATH-1;
					AnsiString drive = AnsiString(Drive).SubString(1, 2);
					WNetGetConnection(drive.c_str(), NameBuffer, &Length);
				}
				else
				{
					char FSBuffer[16] = {0};
					// Any other drive...
					GetVolumeInformation(Drive, NameBuffer, MAX_PATH-1, NULL, NULL, NULL, FSBuffer, 15);
					if (NodeSettings.NodeType == ntFixedDrive)
					{
						if (FSBuffer[0] == NULL)
						{
							NodeSettings.NodeType = ntUnknown;
						}
						else if (NameBuffer[0] == NULL)
						{
							strcpy(NameBuffer, "Hard Drive");
						}
					}
					else if (NodeSettings.NodeType == ntCDROMDrive && FSBuffer[0] == NULL)
					{
						strcpy(NameBuffer, "CD-ROM");
					}
				}
				if (Sent && !*Sent)
				{
					SHFILEINFO sfi;
					if (SUCCEEDED(SHGetFileInfo(Drive,
									FILE_ATTRIBUTE_NORMAL,
									&sfi,
									sizeof(sfi),
									SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON)))
					{
						std::auto_ptr<TIcon>			Icon(new TIcon());
						Icon->Handle					= sfi.hIcon;

						NodeSettings.Icon				= new TIcon;
						NodeSettings.Icon->Assign(Icon.get());

						*Sent = true;
					}
				}
			}
			__finally
			{
				SetErrorMode(CurrErrorMode);
			}
			// Does this drive have a description?
			if (NameBuffer[0] != NULL)
			{
				NodeSettings.DisplayName = AnsiString(NameBuffer) + " (" + NodeSettings.DisplayName + ")";
			}
			if (NodeSettings.NodeType != ntUnknown && NodeSettings.NodeType & NodeTypes)
			{
				Nodes.push_back(NodeSettings);
			}
			while (DriveBuffer[Index++]) {}
		}
	}
	OutData << true; // Successful
	OutData << Nodes.size();
	for (unsigned int i=0; i<Nodes.size(); i++)
	{
		Nodes[i] >> OutData;
	}
	return false;
}
//---------------------------------------------------------------------------
bool TRBServer::GetFolders(const TSerialData &InData, TSerialData &OutData)
{
	std::vector<TNodeSettings> FolderNodes;
	std::vector<TNodeSettings> FileNodes;

	AnsiString Path, FileFilter;
	InData >> Path;
	InData >> FileFilter;
	if (FileFilter != "")
	{
		FileFilter = "." + FileFilter.UpperCase();
	}

	Path = IncludeTrailingBackslash(Path);

	WIN32_FIND_DATA FileData;
	HANDLE hSearch;
	DWORD dwAttrs;

	BOOL fFinished = FALSE;
 
	DWORD CurrErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
	try
	{
		hSearch = FindFirstFile((Path + "*.*").c_str(), &FileData);
		if (hSearch != INVALID_HANDLE_VALUE)
		{
			while (!fFinished)
			{
				dwAttrs = GetFileAttributes((Path + FileData.cFileName).c_str());

				if (dwAttrs & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (AnsiString(FileData.cFileName) != "." && AnsiString(FileData.cFileName) != "..")
					{
						TNodeSettings NodeSettings;

						NodeSettings.DisplayName	= FileData.cFileName;
						NodeSettings.Path				= Path + FileData.cFileName;

						NodeSettings.NodeType		= ntFolder;
						NodeSettings.HasSubFolders	= true;

						FolderNodes.push_back(NodeSettings);
					}
				}
				else if (dwAttrs & (FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_ARCHIVE))
				{
					if (FileFilter == "" || FileFilter == ExtractFileExt(FileData.cFileName).UpperCase())
					{
						TNodeSettings NodeSettings;

						NodeSettings.DisplayName	= FileData.cFileName;
						NodeSettings.Path				= Path + FileData.cFileName;

						NodeSettings.NodeType		= ntFile;
						NodeSettings.HasSubFolders	= false;

						NodeSettings.FileExtension	= ExtractFileExt(NodeSettings.Path).UpperCase();

						if (SendFileImages.find(NodeSettings.FileExtension) == SendFileImages.end())
						{
							SHFILEINFO sfi;
							if (SUCCEEDED(SHGetFileInfo(FileData.cFileName,
											FILE_ATTRIBUTE_NORMAL,
											&sfi,
											sizeof(sfi),
											SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON)))
							{
								std::auto_ptr<TIcon>			Icon(new TIcon());
								Icon->Handle					= sfi.hIcon;

								NodeSettings.Icon				= new TIcon;
								NodeSettings.Icon->Assign(Icon.get());

								SendFileImages.insert(NodeSettings.FileExtension);
							}
						}
						FileNodes.push_back(NodeSettings);
					}
				}

				if (!FindNextFile(hSearch, &FileData))
				{
					if (GetLastError() == ERROR_NO_MORE_FILES)
					{
						fFinished = true;
					}
				}
			}
		}
		FindClose(hSearch);
	}
	__finally
	{
		SetErrorMode(CurrErrorMode);
	}
	OutData << true; // Successful
	OutData << FolderNodes.size() + FileNodes.size();
	for (unsigned int i=0; i<FolderNodes.size(); i++)
	{
		FolderNodes[i] >> OutData;
	}
	for (unsigned int i=0; i<FileNodes.size(); i++)
	{
		FileNodes[i] >> OutData;
	}
	return true;
}
//---------------------------------------------------------------------------
bool TRBServer::GetFiles(const TSerialData &InData, TSerialData &OutData)
{
	return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TRBClient::TRBClient(const AnsiString &IP, int Port, TNodeType NodeTypes, const AnsiString &Filter) :
		Form(new TRemoteBrowseForm()), fTCPClient(new TIdTCPClient(NULL)),
		fFilter(Filter),	fNodeTypes(NodeTypes), fExecuting(false)

{
	fTCPClient->Host = IP;
	fTCPClient->Port = Port;
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
//	Data.Encrypt();
	fTCPClient->WriteStream(Data.Stream(), true, true);
}
//---------------------------------------------------------------------------
bool TRBClient::ReceiveTCPMessage(TSerialData &Data)
{
	bool Success = false;

	fTCPClient->ReadStream(Data.Stream(), -1, false);
	Data.Decompress();
//	Data.Stream()->Position = 0;
//	if (Data.Decrypt())
//	{
//		Data.Decompress();
		Data >> Success;
//	}
	return Success;
}
//---------------------------------------------------------------------------
bool TRBClient::Execute()
{
	Screen->Cursor = crHourGlass;
	try
	{
		fExecuting = true;
		Form->imlTreeImages->Clear();
		Form->TreeView1->Items->Clear();
		FileImageIndexes.clear();

		Form->Label1->Caption = "You are currently browsing the machine \"" + fTCPClient->Host + "\".";

		Form->TreeView1->OnExpanding = TreeExpanding;

		// Ask Window$ nicely for some pretty icons for our tree.
		SHFILEINFO sfi;
		std::auto_ptr<TIcon> Icon(new TIcon());

		// Get the icon for the My Computer folder.
		LPMALLOC lpMalloc = NULL;
		if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
		{
			LPITEMIDLIST lpidl;
			if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &lpidl)))
			{
				if (SUCCEEDED(SHGetFileInfo((LPCTSTR)lpidl,
														0,
														&sfi,
														sizeof(SHFILEINFO),
														SHGFI_PIDL | SHGFI_ICON | SHGFI_SMALLICON)))
				{
					Icon->Handle = sfi.hIcon;
				}
			}
			lpMalloc->Free(lpidl);
			lpMalloc->Release();
		}
		MyComputerIconIndex = Form->imlTreeImages->AddIcon(Icon.get());

		// Get the drive icon.
		if (SUCCEEDED(SHGetFileInfo(	"C:\\",
												FILE_ATTRIBUTE_DIRECTORY,
												&sfi,
												sizeof(sfi),
												SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON)))
		{
			Icon->ReleaseHandle();
			Icon->Handle = sfi.hIcon;
		}
		DefaultFixedDriveIconIndex	= Form->imlTreeImages->AddIcon(Icon.get());
		FixedDriveIconIndex			= DefaultFixedDriveIconIndex;
		FloppyDriveIconIndex			= DefaultFixedDriveIconIndex;
		RemovableDriveIconIndex		= DefaultFixedDriveIconIndex;
		RemoteDriveIconIndex			= DefaultFixedDriveIconIndex;
		CDROMDriveIconIndex			= DefaultFixedDriveIconIndex;
		RAMDriveIconIndex				= DefaultFixedDriveIconIndex;

		// Get the closed folder icon. The path is fictitious but shows it is a folder that is required.
		if (SUCCEEDED(SHGetFileInfo(	"C:\\DIRECTORY",
												FILE_ATTRIBUTE_DIRECTORY,
												&sfi,
												sizeof(sfi),
												SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON)))
		{
			Icon->ReleaseHandle();
			Icon->Handle = sfi.hIcon;
		}
		DefaultFolderClosedIconIndex	= Form->imlTreeImages->AddIcon(Icon.get());
		FolderClosedIconIndex			= DefaultFolderClosedIconIndex;
		// Get the open folder icon.
		if (SUCCEEDED(SHGetFileInfo(	"C:\\DIRECTORY",
												FILE_ATTRIBUTE_DIRECTORY,
												&sfi,
												sizeof(sfi),
												SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON)))
		{
			Icon->ReleaseHandle();
			Icon->Handle = sfi.hIcon;
		}
		DefaultFolderOpenIconIndex	= Form->imlTreeImages->AddIcon(Icon.get());
		FolderOpenIconIndex			= DefaultFolderOpenIconIndex;
		// Get the txt file icon.
	/*	if (SUCCEEDED(SHGetFileInfo(	"TXTFILE.TXT",
	//	if (SUCCEEDED(SHGetFileInfo(	"DEFAULTICON.FORTHIS",
												FILE_ATTRIBUTE_NORMAL,
												&sfi,
												sizeof(sfi),
												SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON)))
		{
			Icon->ReleaseHandle();
			Icon->Handle = sfi.hIcon;
		}
		DefaultFileIconIndex = Form->imlTreeImages->AddIcon(Icon.get());*/
		Icon->ReleaseHandle();

		GetDrives();
	}
	__finally
	{
		Screen->Cursor = crDefault;
	}
	if (Form->ShowModal() == mrOk)
	{
		Path = Form->Path;
		return true;
	}
	else return false;
}
//---------------------------------------------------------------------------
void TRBClient::GetDrives()
{
	TTreeNode *ComputerNode			= Form->TreeView1->Items->AddChild(NULL, fTCPClient->Host);
	ComputerNode->Data				= NULL;
	ComputerNode->ImageIndex		= MyComputerIconIndex;
	ComputerNode->SelectedIndex	= MyComputerIconIndex;

	TSerialData OutData;
	TSerialData InData;

	OutData << MSG_RBRWS_DRIVES;
	OutData << fNodeTypes;

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
				TNodeSettings *NodeSettings	= new TNodeSettings;
				*NodeSettings << InData;

				TTreeNode *NewNode				= Form->TreeView1->Items->AddChild(ComputerNode, NodeSettings->DisplayName);
				NewNode->Data						= (void *)NodeSettings;
				NewNode->HasChildren				= NodeSettings->HasSubFolders;

				switch (NodeSettings->NodeType)
				{
					case ntFloppyDrive:			if (NodeSettings->Icon)
														{
															FloppyDriveIconIndex		= Form->imlTreeImages->AddIcon(NodeSettings->Icon);
														}
														NewNode->ImageIndex			= FloppyDriveIconIndex;
														NewNode->SelectedIndex		= FloppyDriveIconIndex;
														break;
					case ntRemovableDrive:		if (NodeSettings->Icon)
														{
															RemovableDriveIconIndex	= Form->imlTreeImages->AddIcon(NodeSettings->Icon);
														}
														NewNode->ImageIndex			= RemovableDriveIconIndex;
														NewNode->SelectedIndex		= RemovableDriveIconIndex;
														break;
					case ntFixedDrive:			if (NodeSettings->Icon)
														{
															FixedDriveIconIndex		= Form->imlTreeImages->AddIcon(NodeSettings->Icon);
														}
														NewNode->ImageIndex			= FixedDriveIconIndex;
														NewNode->SelectedIndex		= FixedDriveIconIndex;
														break;
					case ntRemoteDrive:			if (NodeSettings->Icon)
														{
															RemoteDriveIconIndex		= Form->imlTreeImages->AddIcon(NodeSettings->Icon);
														}
														NewNode->ImageIndex			= RemoteDriveIconIndex;
														NewNode->SelectedIndex		= RemoteDriveIconIndex;
														break;
					case ntCDROMDrive:			if (NodeSettings->Icon)
														{
															CDROMDriveIconIndex		= Form->imlTreeImages->AddIcon(NodeSettings->Icon);
														}
														NewNode->ImageIndex			= CDROMDriveIconIndex;
														NewNode->SelectedIndex		= CDROMDriveIconIndex;
														break;
					case ntRAMDrive:				if (NodeSettings->Icon)
														{
															RAMDriveIconIndex			= Form->imlTreeImages->AddIcon(NodeSettings->Icon);
														}
														NewNode->ImageIndex			= RAMDriveIconIndex;
														NewNode->SelectedIndex		= RAMDriveIconIndex;
														break;
				}
			}
		}
	}
	__finally
	{
		fTCPClient->Disconnect();
	}
	ComputerNode->Expanded = true;
}
//---------------------------------------------------------------------------
void __fastcall TRBClient::TreeExpanding(TObject *Sender, TTreeNode *Node, bool &AllowExpansion)
{
	TNodeSettings *NodeSettings = ((TNodeSettings *)Node->Data);

//	if ((NodeSettings->NodeType == ntFixedDrive || NodeSettings->NodeType == ntFolder) && NodeSettings->HasSubFolders && Node->Count == 0)
	if (NodeSettings/* && NodeSettings->HasSubFolders*/ && Node->Count == 0)
	{
		Screen->Cursor = crHourGlass;
		try
		{
			TSerialData OutData;
			TSerialData InData;

			OutData << MSG_RBRWS_FOLDERS;
			OutData << NodeSettings->Path;
			OutData << fFilter;

			fTCPClient->Connect();
			try
			{
				SendTCPMessage(OutData);

				TSerialData InData;
				if (ReceiveTCPMessage(InData))
				{
					int NodesCount = 0;
					InData >> NodesCount;
					if (NodesCount == 0)
					{
						Node->HasChildren = false;
					}
					else
					{
						for (int i=0; i<NodesCount; i++)
						{
							TNodeSettings *NodeSettings = NULL;
							TTreeNode *NewNode = NULL;

							try
							{
								NodeSettings = new TNodeSettings;
								*NodeSettings << InData;

								NewNode							= Form->TreeView1->Items->AddChild(Node, NodeSettings->DisplayName);
								NewNode->Data					= (void *)NodeSettings;
								NewNode->HasChildren			= NodeSettings->HasSubFolders;

								if (NodeSettings->NodeType == ntFolder)
								{
									NewNode->ImageIndex		= FolderClosedIconIndex;//ICON_CLOSED_FOLDER_INDEX;
									NewNode->SelectedIndex	= FolderOpenIconIndex;//ICON_OPEN_FOLDER_INDEX;

									if (fExecuting)
									{
										if (ExtractFilePath(Path) == NodeSettings->Path)
										{
											NewNode->MakeVisible();
											fExecuting = false;
										}
										else if (Path.AnsiPos(ExtractFilePath(NodeSettings->Path)))
										{
											NewNode->MakeVisible();
										}
									}
								}
								else if (NodeSettings->NodeType == ntFile)
								{
									TFileImageIndexes::iterator iIndex = FileImageIndexes.find(NodeSettings->FileExtension);
									if (iIndex == FileImageIndexes.end())
									{
										int Index = Form->imlTreeImages->AddIcon(NodeSettings->Icon);
										FileImageIndexes[NodeSettings->FileExtension] = Index;

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
							catch (...)
							{
								delete NodeSettings;
								delete NewNode;
							}
						}
					}
				}
			}
			__finally
			{
				fTCPClient->Disconnect();
			}
		}
		__finally
		{
			Screen->Cursor = crDefault;
		}
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

