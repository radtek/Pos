//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <memory>

#include "Utilities.h"
#include <Winsock.h>
#include "Connections.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmUtilities *dmUtilities;
//---------------------------------------------------------------------------
__fastcall TdmUtilities::TdmUtilities(TComponent* Owner)
	: TDataModule(Owner)
{

	// Ask Window$ nicely for some pretty icons for our tree.
	SHFILEINFO sfi;
	std::auto_ptr<TIcon> Icon(new TIcon());

	// Get the closed folder icon. The path is fictitious but shows it is a folder that is required.
	if (SUCCEEDED(SHGetFileInfo("C:\\DIRECTORY",
											FILE_ATTRIBUTE_DIRECTORY,
											&sfi,
											sizeof(sfi),
											SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON)))
	{
		Icon->Handle = sfi.hIcon;
		imlTreeImages->AddIcon(Icon.get());
	}
	else
	{
		imlTreeImages->AddIcon(Icon.get());
	}
	// Get the open folder icon.
	if (SUCCEEDED(SHGetFileInfo("C:\\DIRECTORY",
											FILE_ATTRIBUTE_DIRECTORY,
											&sfi,
											sizeof(sfi),
											SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON)))
	{
		Icon->Handle = sfi.hIcon;
		imlTreeImages->AddIcon(Icon.get());
	}
	else
	{
		imlTreeImages->AddIcon(Icon.get());
	}

	// Get the icon for the favorites folder.
	LPMALLOC lpMalloc = NULL;
	if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
	{
		LPITEMIDLIST lpidl;
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_FAVORITES, &lpidl)))
		{
			if (SUCCEEDED(SHGetFileInfo((LPCTSTR)lpidl,
													0,
													&sfi,
													sizeof(SHFILEINFO),
													SHGFI_PIDL | SHGFI_ICON | SHGFI_SMALLICON)))
			{
				Icon->Handle = sfi.hIcon;
				imlTreeImages->AddIcon(Icon.get());
			}
			lpMalloc->Free(lpidl);
		}
		else
		{
			// Use the normal folder as a fallback.
			imlTreeImages->AddIcon(Icon.get());
		}
		lpMalloc->Release();
	}

	// Get the icon for the recycle folder.
	lpMalloc = NULL;
	if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
	{
		LPITEMIDLIST lpidl;
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_BITBUCKET, &lpidl)))
		{
			if (SUCCEEDED(SHGetFileInfo((LPCTSTR)lpidl,
													0,
													&sfi,
													sizeof(SHFILEINFO),
													SHGFI_PIDL | SHGFI_ICON | SHGFI_SMALLICON)))
			{
				Icon->Handle = sfi.hIcon;
				imlTreeImages->AddIcon(Icon.get());
			}
			lpMalloc->Free(lpidl);
		}
		else
		{
			// Use the normal folder as a fallback.
			imlTreeImages->AddIcon(Icon.get());
		}
		lpMalloc->Release();
	}

	std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap());
	Bitmap->Width = imlTreeStateImages->Width;
	Bitmap->Height = imlTreeStateImages->Height;

	// blank placeholder - StateImage Index 0 signals No Image. Image 0 is therefore useless.
	imlTreeStateImages->Insert(0, Bitmap.get(), NULL);

	RECT R = TRect(Bitmap->Canvas->ClipRect.left+1,
						Bitmap->Canvas->ClipRect.top+1,
						Bitmap->Canvas->ClipRect.right-1,
						Bitmap->Canvas->ClipRect.bottom-1);

	// unchecked
	unsigned int style = DFCS_BUTTONCHECK | DFCS_FLAT | DFCS_MONO;
	Bitmap->Canvas->Brush->Color = clFuchsia;
	Bitmap->Canvas->FillRect(TRect(Bitmap->Canvas->ClipRect.left,
											 Bitmap->Canvas->ClipRect.top,
											 Bitmap->Canvas->ClipRect.right,
											 Bitmap->Canvas->ClipRect.bottom));
	DrawFrameControl(Bitmap->Canvas->Handle, &R, DFC_BUTTON, style);
	imlTreeStateImages->AddMasked(Bitmap.get(), clFuchsia);

	// checked
	style = DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT | DFCS_MONO;
	Bitmap->Canvas->Brush->Color = clFuchsia;
	Bitmap->Canvas->FillRect(TRect(Bitmap->Canvas->ClipRect.left,
											 Bitmap->Canvas->ClipRect.top,
											 Bitmap->Canvas->ClipRect.right,
											 Bitmap->Canvas->ClipRect.bottom));
	DrawFrameControl(Bitmap->Canvas->Handle, &R, DFC_BUTTON, style);
	imlTreeStateImages->AddMasked(Bitmap.get(), clFuchsia);

	// grey unchecked
	style = DFCS_BUTTONCHECK | DFCS_INACTIVE | DFCS_FLAT | DFCS_MONO;
	Bitmap->Canvas->Brush->Color = clFuchsia;
	Bitmap->Canvas->FillRect(TRect(Bitmap->Canvas->ClipRect.left,
											 Bitmap->Canvas->ClipRect.top,
											 Bitmap->Canvas->ClipRect.right,
											 Bitmap->Canvas->ClipRect.bottom));
	DrawFrameControl(Bitmap->Canvas->Handle, &R, DFC_BUTTON, style);
	imlTreeStateImages->AddMasked(Bitmap.get(), clFuchsia);

	// grey checked
	style = DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_INACTIVE | DFCS_FLAT | DFCS_MONO;
	Bitmap->Canvas->Brush->Color = clFuchsia;
	Bitmap->Canvas->FillRect(TRect(Bitmap->Canvas->ClipRect.left,
											 Bitmap->Canvas->ClipRect.top,
											 Bitmap->Canvas->ClipRect.right,
											 Bitmap->Canvas->ClipRect.bottom));
	DrawFrameControl(Bitmap->Canvas->Handle, &R, DFC_BUTTON, style);
	imlTreeStateImages->AddMasked(Bitmap.get(), clFuchsia);

	// Smaller checkboxes

	R = TRect(	Bitmap->Canvas->ClipRect.left+2,
					Bitmap->Canvas->ClipRect.top+2,
					Bitmap->Canvas->ClipRect.right-2,
					Bitmap->Canvas->ClipRect.bottom-2);

	// unchecked
	style = DFCS_BUTTONCHECK | DFCS_FLAT | DFCS_MONO;
	Bitmap->Canvas->Brush->Color = clFuchsia;
	Bitmap->Canvas->FillRect(TRect(Bitmap->Canvas->ClipRect.left,
											 Bitmap->Canvas->ClipRect.top,
											 Bitmap->Canvas->ClipRect.right,
											 Bitmap->Canvas->ClipRect.bottom));
	DrawFrameControl(Bitmap->Canvas->Handle, &R, DFC_BUTTON, style);
	imlTreeStateImages->AddMasked(Bitmap.get(), clFuchsia);

	// checked
	style = DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT | DFCS_MONO;
	Bitmap->Canvas->Brush->Color = clFuchsia;
	Bitmap->Canvas->FillRect(TRect(Bitmap->Canvas->ClipRect.left,
											 Bitmap->Canvas->ClipRect.top,
											 Bitmap->Canvas->ClipRect.right,
											 Bitmap->Canvas->ClipRect.bottom));
	DrawFrameControl(Bitmap->Canvas->Handle, &R, DFC_BUTTON, style);
	imlTreeStateImages->AddMasked(Bitmap.get(), clFuchsia);

	// grey unchecked
	style = DFCS_BUTTONCHECK | DFCS_INACTIVE | DFCS_FLAT | DFCS_MONO;
	Bitmap->Canvas->Brush->Color = clFuchsia;
	Bitmap->Canvas->FillRect(TRect(Bitmap->Canvas->ClipRect.left,
											 Bitmap->Canvas->ClipRect.top,
											 Bitmap->Canvas->ClipRect.right,
											 Bitmap->Canvas->ClipRect.bottom));
	DrawFrameControl(Bitmap->Canvas->Handle, &R, DFC_BUTTON, style);
	imlTreeStateImages->AddMasked(Bitmap.get(), clFuchsia);

	// grey checked
	style = DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_INACTIVE | DFCS_FLAT | DFCS_MONO;
	Bitmap->Canvas->Brush->Color = clFuchsia;
	Bitmap->Canvas->FillRect(TRect(Bitmap->Canvas->ClipRect.left,
											 Bitmap->Canvas->ClipRect.top,
											 Bitmap->Canvas->ClipRect.right,
											 Bitmap->Canvas->ClipRect.bottom));
	DrawFrameControl(Bitmap->Canvas->Handle, &R, DFC_BUTTON, style);
	imlTreeStateImages->AddMasked(Bitmap.get(), clFuchsia);



 /*	TImageList *TmpList = new TImageList(NULL);
	try
	{
		TmpList->Assign(imlTreeImages);
		imlTreeImages->Handle = int(ImageList_Create(imlTreeImages->Width, imlTreeImages->Height, ILC_COLOR32 | ILC_MASK, 0, imlTreeImages->AllocBy));
		imlTreeImages->AddImages(TmpList);
	}
	__finally
	{
		delete TmpList;
	}
	for (int i=0; i<imlTreeImages->Count; i++)
	{
	} */
}
//---------------------------------------------------------------------------
AnsiString TdmUtilities::GetComputerIP(AnsiString ComputerName)
{
	AnsiString IP;
	// Only gets the first one. I guess this is ok.
	WSADATA sData;
	WSAStartup(0x101, &sData);
	hostent *pHostEnt = gethostbyname(ComputerName.c_str());
	if (pHostEnt && pHostEnt->h_addr_list[0])
	{
		IP = inet_ntoa(*(struct in_addr*)pHostEnt->h_addr_list[0]);
	}
/*	if (pHostEnt)
	{
		for( i=0; pHostEnt != NULL && pHostEnt->h_addr_list[i] != NULL; i++)
		{
			ComputerName = inet_ntoa(*(struct in_addr*)pHostEnt->h_addr_list[i]);
		}
	}*/
	WSACleanup();
	return IP;
}
//---------------------------------------------------------------------------
AnsiString TdmUtilities::GetPCName()
{
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
	char szBuf[MAX_COMPUTERNAME_LENGTH + 1];
	szBuf[0] = '\0';
	if (!GetComputerName(szBuf, &dwSize))
	{
		return "";
	}
	AnsiString PCName = AnsiString(szBuf,sizeof(szBuf));
	PCName = PCName.Trim();
	return PCName;
}
//---------------------------------------------------------------------------
bool TdmUtilities::StrToBoolean(AnsiString s)
{
	s = UpperCase(s);
	if (s == "T" || s == "1"  || s == "Y" || s == "TRUE")
		return true;
	else
		return false;
}
//---------------------------------------------------------------------------
void TdmUtilities::DeleteWildCardFile(AnsiString FileName)
{
	TSearchRec sr;
	int iAttributes = faAnyFile;

	if (FindFirst(FileName, iAttributes, sr) == 0)
	{
		do
		{
			if (sr.Attr & iAttributes)
			{
				//Strings->Add(sr.Name);     DeleteFile
				DeleteFile(sr.Name);
			}
		}
		while (FindNext(sr) == 0);
		FindClose(sr);
	}
}
//---------------------------------------------------------------------------
AnsiString TdmUtilities::FindWildCardFile(AnsiString FileName)
{
	TSearchRec sr;
	int iAttributes = faAnyFile;
	AnsiString ReturnFileName = "";
	if (FindFirst(FileName, iAttributes, sr) == 0)
	{
		do
		{
			if (sr.Attr & iAttributes)
			{
				ReturnFileName = CurrentConnection.MenuSavePath + "\\" + sr.Name;
			}
		}
		while (FindNext(sr) == 0);
		FindClose(sr);
	}
	return ReturnFileName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TRetrieveNamesThread::TRetrieveNamesThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TRetrieveNamesThread::Execute()
{
	EnumNetRes(NULL, ComputerNames);
}
//---------------------------------------------------------------------------
void __fastcall TRetrieveNamesThread::EnumNetRes(NETRESOURCE* nr, TStrings *ComputerNames)
{
	try
	{
		HANDLE hEnum;
		if (WNetOpenEnum(	RESOURCE_GLOBALNET,
								RESOURCETYPE_ANY,
								RESOURCEUSAGE_CONTAINER | RESOURCEUSAGE_CONNECTABLE,
								nr,
								&hEnum) != NO_ERROR)
		{
			return;
		}
		DWORD Count=-1;
		DWORD BufferSize=0;
		DWORD ReturnVal=0;

		NETRESOURCE *NetRes = (NETRESOURCE*)new char[1024];
		ZeroMemory(NetRes, sizeof(NETRESOURCE));
		for(;;)
		{
			ReturnVal = WNetEnumResource(hEnum, &Count, NetRes, &BufferSize);
			if(ReturnVal == ERROR_MORE_DATA)  //we set the size of NetRes
			{
				//we set the size of NetRes if there is more data
				Count=-1;
				delete[] NetRes;
				NetRes = (NETRESOURCE*)new char[BufferSize];
				ZeroMemory(NetRes,sizeof(NETRESOURCE));
				ReturnVal = WNetEnumResource(hEnum, &Count, NetRes, &BufferSize);
			}
			if(ReturnVal != NO_ERROR) break;
			// Add the resource to the ListBox
			for (unsigned int i=0; i<Count && !Terminated; i++)
			{
//				Application->ProcessMessages();
				if((NetRes[i].dwUsage & RESOURCEUSAGE_CONTAINER) == RESOURCEUSAGE_CONTAINER)
				{
					if(NetRes[i].dwDisplayType == RESOURCEDISPLAYTYPE_SERVER)
					{
						ComputerName = NetRes[i].lpRemoteName;
						ComputerName = ComputerName.SubString(3, ComputerName.Length()-2);
						Synchronize(AddName);
					}
					EnumNetRes(&NetRes[i], ComputerNames);
				}
			}
			if(ReturnVal == ERROR_NO_MORE_ITEMS)
			{
				delete[] NetRes;
				WNetCloseEnum(hEnum);
				//RaiseLastWin32Error();
			}
			if (Terminated)
			{
				break;
			}
		}
		delete[] NetRes;
		WNetCloseEnum(hEnum);
	}
	catch (Exception &E)
	{
		ShowMessage(E.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TRetrieveNamesThread::AddName()
{
	ComputerNames->Add(ComputerName);
}
//---------------------------------------------------------------------------
//===========================================================================
//----------------- TPropertyEdit -------------------------------------------
__fastcall TPropertyEdit::TPropertyEdit(TBaseVirtualTree* Tree, TVirtualNode* Node, int Column, TWinControl *Control)
{
	FRefCount				= 0;
	FTree						= Tree;
	FNode						= Node;
	FColumn					= Column;
	FEditControl			= Control;
	FStopping				= false;

	if (FEditControl)
	{
		FEditControl->Visible = false;
	}
}
//---------------------------------------------------------------------------
__fastcall TPropertyEdit::~TPropertyEdit(void)
{
	//  Release() will delete myself if counter == 0 !!!!!
}
//---------------------------------------------------------------------------
bool __stdcall TPropertyEdit::BeginEdit(void)
{
	if (!FStopping && FEditControl)
	{
		FEditControl->Show();
		FEditControl->SetFocus();
		return true;
	}
	return false;
/*	FEditing = true;
	if (FEditControl)
	{
		FEditControl->Show();
		FEditControl->SetFocus();
	}
	return true;*/
}
//---------------------------------------------------------------------------
bool __stdcall TPropertyEdit::CancelEdit(void)
{
	bool Result = FStopping;
	if (!FStopping)
	{
		FStopping = true;
		if (FEditControl)
		{
			FEditControl->Hide();
			FTree->SetFocus();
		}
//		FTree->CancelEditNode();
	}
	return Result;
/*	if (FEditing)
	{
		FEditing = false;

		FEditControl->Hide();
		FTree->SetFocus();
		FTree		= NULL;
		FColumn	= -1;
		FNode		= NULL;
	}
	return true;*/
}
//---------------------------------------------------------------------------
bool __stdcall TPropertyEdit::EndEdit(void)
{
	if (!FStopping)
	{
		FStopping	= true;
		FEditControl->Hide();
//		FTree->EndEditNode();
	}
	return true;
/*	if (FEditing)
	{
		FEditing	= false;
		FEditControl->Hide();
		FTree = NULL;
		FColumn = -1;
		FNode = NULL;
	}
	return true;*/
}
//---------------------------------------------------------------------------
bool __stdcall TPropertyEdit::PrepareEdit(TBaseVirtualTree* Tree, PVirtualNode Node, int Column)
{
	if (Node && FEditControl)
	{
		FTree							= Tree;
		FNode							= Node;
		FColumn						= Column;
		FEditControl->Parent		= FTree;
		FEditControl->Visible	= false;
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
TRect __stdcall TPropertyEdit::GetBounds(void)
{
	return FEditControl->BoundsRect;
}
//---------------------------------------------------------------------------
void __stdcall TPropertyEdit::SetBounds(const TRect R)
{
	FEditControl->BoundsRect = R;
}
//---------------------------------------------------------------------------
void __stdcall TPropertyEdit::ProcessMessage(Messages::TMessage &Message)
{
	FEditControl->WindowProc(Message);
}
//---------------------------------------------------------------------------
ULONG __stdcall TPropertyEdit::AddRef()                    //  Implement IUnknown interface
{
	return InterlockedIncrement((LPLONG)&FRefCount);        //  thread safe increment
}
//---------------------------------------------------------------------------
ULONG __stdcall TPropertyEdit::Release()                   //  Implement IUnknown interface
{
	InterlockedDecrement((LPLONG)&FRefCount);               //  thread safe decrement
	if (FRefCount == 0)                                     //  if last user then
	{
		delete this;                                        //    delete myself
		return 0;                                           //    => 0 !
	}
	return FRefCount;                                       //  return internal counter
}
//---------------------------------------------------------------------------
HResult  __stdcall TPropertyEdit::QueryInterface(const GUID& IID, void**ppv)
{

	if (IID == __uuidof(IUnknown))                          //  IUnknown as basic interface
	{
		*ppv = (IUnknown*)this;
	}
	else if (IID == __uuidof(IVTEditLink))                  //  i am self
	{
		*ppv = (IVTEditLink*)this;
	}
	else
	{
		*ppv = NULL;                                        //  unknowm interface
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();                          //  increment if used
	return S_OK;                                            //  Ok !
}
//===========================================================================

