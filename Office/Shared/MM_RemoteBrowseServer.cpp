//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include <Vector>
#include <Shlobj.h>
//---------------------------------------------------------------------------
#include "MM_RemoteBrowseServer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace RemoteBrowser;
//---------------------------------------------------------------------------
TRBServer::TRBServer(int Port) :
		fTCPServer(new TIdTCPServer(NULL)),
		LargeImages(new TImageList(NULL)),
		SmallImages(new TImageList(NULL))
{
//	SentFileImages	= new TList;

	SHFILEINFO IconInfo;
	LargeImages->Handle = SHGetFileInfo(
			"",
			0,
			&IconInfo,
			sizeof(SHFILEINFO),
			SHGFI_ICON | SHGFI_LARGEICON | SHGFI_SYSICONINDEX);

	LargeImages->ShareImages = true;
	SmallImages->Handle = SHGetFileInfo(
			"",
			0,
			&IconInfo,
			sizeof(SHFILEINFO),
			SHGFI_ICON | SHGFI_SMALLICON | SHGFI_SYSICONINDEX);

	SmallImages->ShareImages = true;

	fTCPServer->OnExecute		= fTCPExecute;
//	fTCPServer->OnDisconnect	= fTCPDisconnect;
	fTCPServer->DefaultPort		= Port;
	fTCPServer->Active			= true;
}
//---------------------------------------------------------------------------
TRBServer::~TRBServer()
{
	fTCPServer->Active = false;

	if (!ClientTree.empty())
	{
		TClientTree::iterator iTree = ClientTree.begin();
		for (; iTree!=ClientTree.end(); ++iTree)
		{
			iTree->second.Node->Free();
			delete iTree->second.Node;
		}
	}
}
//---------------------------------------------------------------------------
/*void TRBServer::SendTCPMessage(TIdTCPServerConnection *Connection, TSerialData &Data)
{
	Data.Compress(8);
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
}*/
//---------------------------------------------------------------------------
void __fastcall TRBServer::fTCPExecute(TIdPeerThread *AThread)
{
	std::auto_ptr<TSyncTCPMessage> SyncTCPMessage(new TSyncTCPMessage(AThread, *this));
	SyncTCPMessage->Synchronize();
/*

	try
	{
		TSerialData OutData;
		TSerialData InData;

		AnsiString PeerIP;
		TIdIOHandlerSocket *IOHandler = dynamic_cast<TIdIOHandlerSocket *>(AThread->Connection->IOHandler);
		if (IOHandler)
		{
			PeerIP = IOHandler->Binding->PeerIP;
		}
		if (ReceiveTCPMessage(AThread->Connection, InData))
		{
			bool Success = false;

			int PacketType=0;
			InData >> PacketType;

			switch (PacketType)
			{
				case MSG_RBRWS_RESET:				Success = Reset(InData, OutData, PeerIP);					break;
				case MSG_RBRWS_DRIVES:				Success = GetDrives(InData, OutData, PeerIP);			break;
//				case MSG_RBRWS_FOLDERS:				Success = GetFolders(InData, OutData, PeerIP);			break;
				case MSG_RBRWS_NETWORK:				Success = GetNetworkFolders(InData, OutData, PeerIP);	break;
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
	}*/
}
//---------------------------------------------------------------------------
__fastcall TSyncTCPMessage::TSyncTCPMessage(TIdPeerThread *AThread, TRBServer &RBServer) :
			fRBServer(RBServer), TIdSync(AThread), fThread(AThread)
{
	TIdIOHandlerSocket *IOHandler = dynamic_cast<TIdIOHandlerSocket *>(AThread->Connection->IOHandler);
	if (IOHandler)
	{
		fPeerIP = IOHandler->Binding->PeerIP;
	}
	fThread->Connection->ReadStream(fInData.Stream(), -1, false);
	fInData.Decompress();
}
//---------------------------------------------------------------------------
__fastcall TSyncTCPMessage::~TSyncTCPMessage()
{
	fOutData.Compress(8);
	fThread->Connection->WriteStream(fOutData.Stream(), true, true);
	fThread->Connection->Disconnect();
}
//---------------------------------------------------------------------------
void __fastcall TSyncTCPMessage::DoSynchronize(void)
{
	bool Success = false;

	int PacketType=0;
	fInData >> PacketType;

	switch (PacketType)
	{
		case MSG_RBRWS_RESET:		Success = fRBServer.Reset(fInData, fOutData);		break;
		case MSG_RBRWS_DRIVES:		Success = fRBServer.GetDrives(fInData, fOutData);	break;
		case MSG_RBRWS_FOLDERS:		Success = fRBServer.GetFolders(fInData, fOutData);	break;
		case MSG_RBRWS_ICONS:		Success = fRBServer.GetIcons(fInData, fOutData);	break;
	}
	if (!Success)
	{
		fOutData << false; // Unsuccessful
	}
}
//---------------------------------------------------------------------------
bool TRBServer::Reset(const TSerialData &InData, TSerialData &OutData)
{
	TRemoteClient RemoteClient;
	InData >> RemoteClient;

	TClientTree::iterator iTree = ClientTree.find(RemoteClient);
	if (iTree != ClientTree.end())
	{
		iTree->second.Node->Free();
		ClientTree.erase(iTree);
	}
	OutData << true; // Successful
	return true;
}
//---------------------------------------------------------------------------
bool TRBServer::GetIcons(const TSerialData &InData, TSerialData &OutData)
{
	TIconSize IconSize;
	InData >> IconSize;

	int IconCount;           
	InData >> IconCount;

	OutData << true; // Successful
	for (int i=0; i<IconCount; ++i)
	{
		int IconIndex;
		InData >> IconIndex;
		std::auto_ptr<TIcon> Icon(new TIcon);
		if (IconSize == isLarge)
		{
			LargeImages->GetIcon(IconIndex, Icon.get());
		}
		else
		{
			SmallImages->GetIcon(IconIndex, Icon.get());
		}
		OutData << IconIndex;
		OutData << Icon.get();
	}
	return true;
}
//---------------------------------------------------------------------------
bool TRBServer::GetDrives(const TSerialData &InData, TSerialData &OutData)
{
	TRemoteClient RemoteClient;
	InData >> RemoteClient;

	TBrowseFilter BrowseFilter;
	InData >> BrowseFilter;

	TIconSize IconSize;
	InData >> IconSize;

	std::vector<TNodeSettings *> Nodes;

	DWORD CurrErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
	try
	{
		// Get a pointer to the shell's IMalloc interface
		// for later use -- it's required to use the shell
		// task allocator to free the Item identifiers and PIDLs.
		LPMALLOC lpMalloc;
		if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
		{
			// Grab a pointer to the root (desktop) folder
			LPSHELLFOLDER lpsf;
			if (SUCCEEDED(SHGetDesktopFolder(&lpsf)))
			{
				if (BrowseFilter & bfFixedDrives || BrowseFilter & bfRemovableDrives || BrowseFilter & bfCDROM)
				{
					LPITEMIDLIST lpidl;
					if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &lpidl)))
					{
						LPSHELLFOLDER lpsfSub;
						if (SUCCEEDED(lpsf->BindToObject(lpidl, NULL, IID_IShellFolder, reinterpret_cast<void **>(&lpsfSub))))
						{
							// refill the branch
//							IPTree[IP];
//							fFillBranch(IP, BrowseFilter, lpsfSub, lpidl, IconSize, IPTree[IP].Node, Nodes);
							fFillBranch(RemoteClient, BrowseFilter, "", lpsfSub, lpidl, IconSize, ClientTree[RemoteClient].Node, Nodes);
							// clean up
							lpsfSub->Release();
						}
					}
				}
				if (BrowseFilter & bfNetwork)
				{
					LPITEMIDLIST lpidl;
					if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_NETWORK, &lpidl)))
					{
						LPSHELLFOLDER lpsfSub;
						if (SUCCEEDED(lpsf->BindToObject(lpidl, NULL, IID_IShellFolder, reinterpret_cast<void **>(&lpsfSub))))
						{
							// refill the branch
							fFillBranch(RemoteClient, BrowseFilter, "", lpsfSub, lpidl, IconSize, ClientTree[RemoteClient].Node, Nodes);
							// clean up
							lpsfSub->Release();
						}
					}
				}
				lpsf->Release();
			}
			// clean up
			lpMalloc->Release();
		}
	}
	__finally
	{
		SetErrorMode(CurrErrorMode);
	}
	OutData << true; // Successful
	OutData << Nodes.size();
	for (unsigned int i=0; i<Nodes.size(); i++)
	{
		*Nodes[i] >> OutData;
		delete Nodes[i];
	}
	return true;
}
//---------------------------------------------------------------------------
void TRBServer::fFillBranch(TRemoteClient RemoteClient, TBrowseFilter BrowseFilter, AnsiString FileFilter,
		LPSHELLFOLDER lpsf, LPITEMIDLIST lpidl,
		TIconSize IconSize, TRBNode *ParentNode, std::vector<TNodeSettings *> &Nodes)
{
	// Get a pointer to the shell's IMalloc interface
	// for later use -- it's required to use the shell
	// task allocator to free the Item identifiers and PIDLs.
	LPMALLOC lpMalloc;
	if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
	{
		// Here we enumerate the contents of the folder that
		// lpsf is bound to.  To do this, we first grab a pointer
		// to the IEnumIDList interface.
		LPENUMIDLIST lpeidl;
		if (SUCCEEDED(lpsf->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &lpeidl)))
		{
			// Using the IEnumIDList interface's Next() method, we
			// iterates through the item IDs one at a time.  The resulting
			// item is stored in lpidlCurrent.
			LPITEMIDLIST lpidlCurrent;
			unsigned long ulFetched;
			while (lpeidl->Next(1, &lpidlCurrent, &ulFetched) == S_OK)
			{
				// create a fully qualified itemidlist
				LPITEMIDLIST lpidlFQ = fMergeIDLists(lpMalloc, lpidl, lpidlCurrent);
				if (lpidlFQ)
				{
					SHFILEINFO FileInfo;
					unsigned int uSHFlags =	SHGFI_SYSICONINDEX |
													SHGFI_DISPLAYNAME |
													SHGFI_ATTRIBUTES |
													SHGFI_PIDL;
					if (IconSize == isLarge)
					{
						uSHFlags |= SHGFI_LARGEICON;
					}
					else
					{
						uSHFlags |= SHGFI_SMALLICON;
					}
					// get file info
					SHGetFileInfo(reinterpret_cast<LPCSTR>(lpidlFQ), 0, &FileInfo, sizeof(SHFILEINFO), uSHFlags);

					if (FileInfo.dwAttributes & SFGAO_FILESYSTEM ||
						 FileInfo.dwAttributes & SFGAO_FILESYSANCESTOR ||
						 ((BrowseFilter & bfRemovableDrives || BrowseFilter & bfCDROM) &&
							(FileInfo.dwAttributes & SFGAO_REMOVABLE || FileInfo.dwAttributes == 0)))
					{
						if ((FileInfo.dwAttributes & SFGAO_FOLDER || FileInfo.dwAttributes & SFGAO_REMOVABLE || FileInfo.dwAttributes == 0) &&
								!(FileInfo.dwAttributes & SFGAO_LINK) && !(FileInfo.dwAttributes & SFGAO_STREAM)) // no zips as folders please
						{
							char Path[MAX_PATH];

							if (FileInfo.dwAttributes & SFGAO_LINK)
							{
								LPSHELLFOLDER lpsfDesktop;
								if (SUCCEEDED(SHGetDesktopFolder(&lpsfDesktop)))
								{
									IShellLink* link = NULL;
									if (SUCCEEDED(lpsfDesktop->BindToObject(lpidlFQ, NULL, IID_IShellLink, reinterpret_cast<void **>(&link))))
									{
										link->GetPath(Path, MAX_PATH, NULL, SLGP_UNCPRIORITY);
										link->Release();
									}
									lpsfDesktop->Release();
								}
							}
							else
							{
								SHGetPathFromIDList(lpidlFQ, Path);
							}

							UINT DriveType = GetDriveType(Path);
							bool AddDrive = true;

							if (DriveType == DRIVE_REMOVABLE			&& !(BrowseFilter&bfRemovableDrives))	AddDrive = false;
							else if (DriveType==DRIVE_FIXED			&& !(BrowseFilter&bfFixedDrives))		AddDrive = false;
							else if (DriveType==DRIVE_REMOTE			&& !(BrowseFilter&bfNetwork))				AddDrive = false;
							else if (DriveType==DRIVE_CDROM			&& !(BrowseFilter&bfCDROM))				AddDrive = false;
							else if ((DriveType==0||DriveType==1)	&& !(BrowseFilter&bfFolders))				AddDrive = false;

							if (AddDrive)
							{
								// add the node and assign the icon index
								// and the fully qualified pidl
								TNodeSettings *NodeSettings		= new TNodeSettings;
								NodeSettings->DisplayName			= FileInfo.szDisplayName;
								NodeSettings->NodeType				= ntFolder;
								NodeSettings->HasSubFolders		= FileInfo.dwAttributes & SFGAO_HASSUBFOLDER;
								NodeSettings->RemoteNode			= ParentNode->AddNode();
								NodeSettings->RemoteNode->lpidl	= lpidlFQ;
								NodeSettings->IconSize				= IconSize;
								NodeSettings->IconIndex				= FileInfo.iIcon;
								if (IconSize == isLarge)
								{
									uSHFlags = SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_OPENICON;
								}
								else
								{
									uSHFlags = SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON;
								}
								SHGetFileInfo((LPCSTR)lpidl, 0, &FileInfo, sizeof(SHFILEINFO), uSHFlags);
								NodeSettings->SelectedIconIndex	= FileInfo.iIcon;

	/*							char Path[MAX_PATH];
								SHGetPathFromIDList(lpidlFQ, Path);

								if (FileInfo.dwAttributes & SFGAO_LINK)
								{
									LPSHELLFOLDER lpsfDesktop;
									if (SUCCEEDED(SHGetDesktopFolder(&lpsfDesktop)))
									{
										IShellLink* link = NULL;
										if (SUCCEEDED(lpsfDesktop->BindToObject(lpidlFQ, NULL, IID_IShellLink, reinterpret_cast<void **>(&link))))
										{
											link->GetPath(Path, MAX_PATH, NULL, SLGP_UNCPRIORITY);
											link->Release();
										}
										lpsfDesktop->Release();
									}
								}*/
								NodeSettings->Path = Path;

								Nodes.push_back(NodeSettings);
							}
						}
						else if (!(FileInfo.dwAttributes & SFGAO_LINK))
						{
							if (FileFilter == "" || FileFilter == ExtractFileExt(FileInfo.szDisplayName).UpperCase())
							{
								char Path[MAX_PATH];
								SHGetPathFromIDList(lpidlFQ, Path);

								TNodeSettings *NodeSettings		= new TNodeSettings;
								NodeSettings->DisplayName			= FileInfo.szDisplayName;
								NodeSettings->NodeType				= ntFile;
								NodeSettings->HasSubFolders		= FileInfo.dwAttributes & SFGAO_HASSUBFOLDER;
								NodeSettings->RemoteNode			= ParentNode->AddNode();
								NodeSettings->RemoteNode->lpidl	= lpidlFQ;
								NodeSettings->IconSize				= IconSize;
								NodeSettings->IconIndex				= FileInfo.iIcon;
								NodeSettings->SelectedIconIndex	= FileInfo.iIcon;
								NodeSettings->Path					= Path;

								Nodes.push_back(NodeSettings);
							}
							else
							{
								lpMalloc->Free(lpidlFQ);
							}
						}
					}
				}
				// free lpidlCurrent filled by the Next() method
				lpMalloc->Free(lpidlCurrent);
			}
			// release the pointer to the IEnumIDList interface
			lpeidl->Release();
		}
		// release the pointer to the IMalloc interface
		lpMalloc->Release();
	}
}
//---------------------------------------------------------------------------
bool TRBServer::GetFolders(const TSerialData &InData, TSerialData &OutData)
{
	std::vector<TNodeSettings *> FolderNodes;

	TRemoteClient RemoteClient;
	InData >> RemoteClient;

	unsigned int RemoteNodeHandle;
	InData >> RemoteNodeHandle;

	AnsiString FileFilter;
	InData >> FileFilter;
	FileFilter = FileFilter.UpperCase();

	TIconSize IconSize;
	InData >> IconSize;

	TRBNode *RemoteNode = reinterpret_cast<TRBNode*>(RemoteNodeHandle);

	// Get a pointer to the IShellFolder interface
	// bound to the root (desktop) folder.
	LPSHELLFOLDER lpsf;
	if (SUCCEEDED(SHGetDesktopFolder(&lpsf)))
	{
		// extract the associated pidl
		LPITEMIDLIST lpidl = RemoteNode->lpidl;
		// if not the desktop pidl
		if (lpidl->mkid.cb)
		{
			// bind the IShellFolder pointer to the
			// path indicated by the node's pidl
			LPSHELLFOLDER lpsfSub;
			if (SUCCEEDED(lpsf->BindToObject(lpidl, NULL, IID_IShellFolder, reinterpret_cast<void **>(&lpsfSub))))
			{
				// refill the branch
				fFillBranch(RemoteClient, bfAll, FileFilter, lpsfSub, lpidl, IconSize, RemoteNode, FolderNodes);
				// clean up
				lpsfSub->Release();
			}
		}
		// clean up
		lpsf->Release();
	}
	OutData << true; // Successful
	OutData << FolderNodes.size();
	for (unsigned int i=0; i<FolderNodes.size(); i++)
	{
		*FolderNodes[i] >> OutData;
		delete FolderNodes[i];
	}
	return true;
}
//---------------------------------------------------------------------------
/*LPITEMIDLIST TRBServer::fPIDLGetFromPath(const AnsiString &aPathName)
{
// Convert the fully-qualified file path aPathname to a fully-qualified PIDL
	LPITEMIDLIST aPidl = NULL;

	LPMALLOC lpMalloc;
	if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
	{
		// Grab a pointer to the root (desktop) folder
		LPSHELLFOLDER lpsf;
		if (SUCCEEDED(SHGetDesktopFolder(&lpsf)))
		{
			WideString		OLEPath = aPathName;
			ULONG				dwAttributes;
			ULONG				chEaten;

			if SUCCEEDED(lpsf->ParseDisplayName(0, NULL, OLEPath.c_bstr(), &chEaten, &aPidl, &dwAttributes))
			{
			}
		}
		// clean up
		lpMalloc->Release();
	}
	return aPidl;
}
//---------------------------------------------------------------------------
void __fastcall TRBServer::fFillWithCSIDL(int CSIDL, bool SubItems, std::vector<TNodeSettings *> &FolderNodes)
{
	// Get a pointer to the shell's IMalloc interface
	// for later use -- it's required to use the shell
	// task allocator to free the Item identifiers and PIDLs.
	LPMALLOC lpMalloc;
	if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
	{
		// Grab a pointer to the root (desktop) folder
		LPSHELLFOLDER lpsf;
		if (SUCCEEDED(SHGetDesktopFolder(&lpsf)))
		{
			// Get the piddle of the Drives (My Computer)
			LPITEMIDLIST lpidlSpecial;
			if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL, &lpidlSpecial)))
			{
				char folder[MAX_PATH] = {0};
				SHGetPathFromIDList(lpidlSpecial, folder);

				SHFILEINFO FileInfo;
				unsigned int uSHFlags =	SHGFI_SYSICONINDEX |
												SHGFI_DISPLAYNAME |
												SHGFI_ATTRIBUTES |
												SHGFI_PIDL;

				// get file info
				SHGetFileInfo((LPCSTR)lpidlSpecial, 0, &FileInfo, sizeof(SHFILEINFO), uSHFlags);

				TNodeSettings *NodeSettings	= new TNodeSettings;

				NodeSettings->DisplayName		= FileInfo.szDisplayName;
				NodeSettings->Path				= folder;

				NodeSettings->NodeType			= ntNetwork;

				FolderNodes.push_back(NodeSettings);

				if (SubItems)
				{
					// Bind the shell folder to the virtual Drives
					// piddle (LPITEMIDLIST)
					LPSHELLFOLDER lpsfSpecial;
					if (SUCCEEDED(lpsf->BindToObject(lpidlSpecial, NULL, IID_IShellFolder, (LPVOID *)&lpsfSpecial)))
					{
						// call the FillList() helper function
						fFillList(lpsfSpecial, lpidlSpecial, FolderNodes);
						lpsfSpecial->Release();
					}
				}
				// clean up
				lpMalloc->Free(lpidlSpecial);
				lpsf->Release();
			}
		}
		// clean up
		lpMalloc->Release();
	}
} */
//---------------------------------------------------------------------------
// PIDL "piddle" helper routine
//---------------------------------------------------------------------------
LPITEMIDLIST TRBServer::fGetNextItem(LPITEMIDLIST pidl)
{
	unsigned short nLen = pidl->mkid.cb;
	if (nLen == 0) return NULL;
	return reinterpret_cast<LPITEMIDLIST>((LPBYTE)pidl + nLen);
}
//---------------------------------------------------------------------------
// PIDL "piddle" helper routine
//---------------------------------------------------------------------------
unsigned short TRBServer::fGetPIDLSize(LPITEMIDLIST lpidl)
{
	unsigned short cb = 0;
	while (lpidl)
	{
		cb = static_cast<unsigned short>(cb + lpidl->mkid.cb);
		lpidl = fGetNextItem(lpidl);
	}
	return static_cast<unsigned short>(cb + 2);
}
//---------------------------------------------------------------------------
// PIDL "piddle" helper routine
//---------------------------------------------------------------------------
LPITEMIDLIST TRBServer::fMergeIDLists(LPMALLOC lpMalloc, LPITEMIDLIST lpidl1, LPITEMIDLIST lpidl2)
{
	int cb1, cb2;

	if (lpidl1) cb1 = fGetPIDLSize(lpidl1);
	if (cb1) cb1 = cb1 - 2;


	if (lpidl2) cb2 = fGetPIDLSize(lpidl2);
	if (cb2) cb2 = cb2 - 2;

	int total_size = cb1 + cb2 + 2;

	LPITEMIDLIST lpidlNew = reinterpret_cast<LPITEMIDLIST>(lpMalloc->Alloc(total_size));
	if (lpidlNew)
	{
		ZeroMemory(lpidlNew, total_size);
		CopyMemory(lpidlNew, lpidl1, cb1);
		CopyMemory((reinterpret_cast<LPBYTE>(lpidlNew)) + cb1, lpidl2, cb2);
		FillMemory((reinterpret_cast<LPBYTE>(lpidlNew)) + cb1 + cb2, 2, 0);
		return lpidlNew;
    } 
    return NULL; 
}/*
//---------------------------------------------------------------------------
// FillList to fill the ComboBox with icons / display names
void __fastcall TRBServer::fFillList(LPSHELLFOLDER lpsf, LPITEMIDLIST lpidl, std::vector<TNodeSettings *> &FolderNodes)
{
	// Get a pointer to the shell's IMalloc interface
	// for later use -- it's required to use the shell
	// task allocator to free the Item identifiers and PIDLs.
	LPMALLOC lpMalloc;
	if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
	{
		// Here we enumerate the contents of the folder that
		// lpsf is bound to.  To do this, we first grab a pointer
		// to the IEnumIDList interface.
		LPENUMIDLIST lpeidl;
		if (SUCCEEDED(lpsf->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &lpeidl)))
		{
			// Using the IEnumIDList interface's Next() method, we
			// iterates through the item IDs one at a time.  The resulting
			// item is stored in lpidlCurrent.
			LPITEMIDLIST lpidlCurrent;
			while (lpeidl->Next(1, &lpidlCurrent, NULL) == S_OK)
			{
				// create a fully qualified itemidlist
				LPITEMIDLIST lpidlFQ = fMergeIDLists(lpidl, lpidlCurrent);
				if (lpidlFQ)
				{
					SHFILEINFO FileInfo;
					unsigned int uSHFlags =	SHGFI_SYSICONINDEX |
													SHGFI_DISPLAYNAME |
													SHGFI_ATTRIBUTES |
													SHGFI_PIDL;

					// get file info
					SHGetFileInfo((LPCSTR)lpidlFQ, 0, &FileInfo, sizeof(SHFILEINFO), uSHFlags);

					char folder[MAX_PATH] = {0};
					SHGetPathFromIDList(lpidlFQ, folder);

					TNodeSettings *NodeSettings	= new TNodeSettings;

					NodeSettings->DisplayName		= FileInfo.szDisplayName;
					NodeSettings->Path				= folder;

					NodeSettings->NodeType			= ntNetworkFolder;

					FolderNodes.push_back(NodeSettings);

					// free the fullly qualified itemidlist
					lpMalloc->Free(lpidlFQ);
				}
				// free lpidlCurrent filled by the Next() method
				lpMalloc->Free(lpidlCurrent);
			}
			// release the pointer to the IEnumIDList interface
			lpeidl->Release();
		}
		// release the pointer to the IMalloc interface
		lpMalloc->Release();
	}
}
//---------------------------------------------------------------------------
// OnDrawItem event handler
/*void __fastcall TForm1::ComboBox1DrawItem(TWin­Control *Control, int Index,
    TRect &Rect, TOwnerDrawState State) 
{ 
    int IconIndex = 
        reinterpret_cast<int>(ComboBox­1->Items->Objects[Index]); 

    if (State.Contains(odSelected)) 
    { 
		  ComboBox1->Canvas->Brush->Color = clHighlight;
        ComboBox1->Canvas->Font->Color = clHighlightText; 
		  ImageList1->DrawingStyle = dsSelected;
    } 
    else 
    { 
        ComboBox1->Canvas->Brush->Colo­r = ComboBox1->Color; 
        ComboBox1->Canvas->Font->Color = ComboBox1->Font->Color; 
        ImageList1->DrawingStyle = dsNormal; 
    } 
    ComboBox1->Canvas->FillRect(Re­ct); 
    ImageList1->Draw(ComboBox1->Ca­nvas, Rect.Left, Rect.Top, IconIndex); 


    Rect.Left = Rect.Left + ImageList1->Width + 5; 
    ComboBox1->Canvas->TextRect(Re­ct, Rect.Left + 1, Rect.Top + 1, 
                                ComboBox1->Items->Strings[Inde­x]); 



} 
//---------------------------------------------------------------------------
// OnMeasureItem event handler
void __fastcall TForm1::ComboBox1MeasureItem(T­WinControl *Control, int Index, 
    int &Height) 
{ 
    Height = ImageList1->Height + 3; 


} 
*/
