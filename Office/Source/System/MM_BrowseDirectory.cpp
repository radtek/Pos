//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <shlobj.h>
#include "MM_BrowseDirectory.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
const int SH_FOLDERS_ARRAY[] = {
	CSIDL_DESKTOP,
	-1,
	CSIDL_PROGRAMS,
	CSIDL_CONTROLS,
	CSIDL_PRINTERS,
	CSIDL_PERSONAL,
	CSIDL_FAVORITES,
	CSIDL_STARTUP,
	CSIDL_RECENT,
	CSIDL_SENDTO,
	CSIDL_BITBUCKET,
	CSIDL_STARTMENU,
	CSIDL_DESKTOPDIRECTORY,
	CSIDL_DRIVES,
	CSIDL_NETWORK,
	CSIDL_NETHOOD,
	CSIDL_FONTS,
	CSIDL_TEMPLATES};

const int NUMBER_OF_BROWSE_INFO_FLAGS = 10;
const int BROWSE_FLAG_ARRAY[] = {
	BIF_BROWSEFORCOMPUTER,
	BIF_BROWSEFORPRINTER,
	BIF_DONTGOBELOWDOMAIN,
	BIF_RETURNFSANCESTORS,
	BIF_RETURNONLYFSDIRS,
	BIF_NEWDIALOGSTYLE,
	BIF_EDITBOX,
	BIF_NONEWFOLDERBUTTON,
	BIF_BROWSEINCLUDEFILES,
	BIF_STATUSTEXT };
//---------------------------------------------------------------------------
TFolderDialog::TFolderDialog(HWND Handle, TDialogSettings DialogSettings) :
		FParentHandle(Handle),
		FSelectedFolder(DialogSettings.InitialPath),
		FLabelCaption(DialogSettings.Caption),
		FFlags(DialogSettings.Flags),
		FRootFolder(DialogSettings.RootFolder)
{
	if (!IsWindow(FParentHandle))
	{
		throw Exception("Invalid WIndows Handle");
	}
}
//---------------------------------------------------------------------------
bool TFolderDialog::Execute()
{
	bool Result	= false;

	FFolder = FSelectedFolder;

	BROWSEINFO		BrowseInfo;
	LPITEMIDLIST	ItemIDList = NULL;
	memset(&BrowseInfo, 0, sizeof(BrowseInfo));
	try
	{
		BrowseInfo.hwndOwner = FParentHandle;

		LPITEMIDLIST pidlRoot = NULL;
		if (FRootFolder != foDesktopExpanded)
		{
			SHGetSpecialFolderLocation(FParentHandle, SH_FOLDERS_ARRAY[FRootFolder], &pidlRoot);
		}
		BrowseInfo.pidlRoot			= pidlRoot;
		BrowseInfo.pszDisplayName	= "";//FDisplayName.c_str();	// No go in new gui (M$ bug). Don't bother
		BrowseInfo.lpszTitle			= FLabelCaption.c_str();
		BrowseInfo.ulFlags			= 0;

		for (int i=0; i<NUMBER_OF_BROWSE_INFO_FLAGS-1; i++)
		{
			if (FFlags.Contains(TBrowseInfoFlags(i)))
			{
				BrowseInfo.ulFlags |= BROWSE_FLAG_ARRAY[TBrowseInfoFlags(i)];
			}
		}
		BrowseInfo.lpfn	= BrowserCallbackProc;
		BrowseInfo.lParam	= reinterpret_cast<LPARAM>(this);
		BrowseInfo.iImage	= 0;
		ItemIDList			= SHBrowseForFolder(&BrowseInfo);

		Result = (ItemIDList != NULL);
		if (Result)
		{
			char TempPath[MAX_PATH+1];
			SHGetPathFromIDList(ItemIDList, TempPath);

			FSelectedFolder = TempPath;
		}
		else
		{
			FSelectedFolder = FFolder;
		}
	}
	__finally
	{
		CoTaskMemFree(ItemIDList);
		CoTaskMemFree((void *)BrowseInfo.pidlRoot);
	}
	return Result;
}
//---------------------------------------------------------------------------
int __stdcall TFolderDialog::BrowserCallbackProc(HWND HWindow, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TFolderDialog *FolderDialog = reinterpret_cast<TFolderDialog*>(lpData);

	switch(uMsg)
	{
		case BFFM_INITIALIZED:
		{
			if (DirectoryExists(FolderDialog->FSelectedFolder))
			{
				FolderDialog->SetSelectionPath(HWindow, FolderDialog->FSelectedFolder);
			}
//			else if (FolderDialog->PStartItemIDList)
//			{
//				FolderDialog->SetSelectionPIDL(HWindow, FolderDialog->PStartItemIDList);
//			}
		}
		case BFFM_SELCHANGED:
		{
//			FolderDialog->EnableOK(HWindow, false);

//			TSHFileInfo ShellFileInfo;
//			SHGetFileInfo(PChar(lParam), 0, &ShellFileInfo, sizeof(ShellFileInfo), SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_TYPENAME);
//			SendMessage(HWindow, BFFM_SETSTATUSTEXT, 0, Longint(ShellFileInfo.szDisplayName)); // No go in new gui
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
void TFolderDialog::SetSelectionPath(HWND HWindow, AnsiString Path)
{
	PItemIDList ItemIDList;
	GetIDListFromPath(Path, ItemIDList);
	SendMessage(HWindow, BFFM_SETSELECTION, false, long(ItemIDList));
}
//---------------------------------------------------------------------------
void TFolderDialog::SetSelectionPIDL(HWND HWindow, PItemIDList ItemIDList)
{
	SendMessage(HWindow, BFFM_SETSELECTION, false, long(ItemIDList));
}
//---------------------------------------------------------------------------
void TFolderDialog::EnableOK(HWND HWindow, bool Enable)
{
	SendMessage(HWindow, BFFM_ENABLEOK, 0, Enable);
}
//---------------------------------------------------------------------------
void TFolderDialog::GetIDListFromPath(AnsiString Path, PItemIDList &ItemIDList)
{
	LPSHELLFOLDER pShellFolder;
	ULONG CharsParsed;
	ULONG Attributes;

	if (SHGetDesktopFolder(&pShellFolder) == NOERROR)
	{
		if (DirectoryExists(Path))
		{
			pShellFolder->ParseDisplayName(0, NULL, StringToOleStr(Path), &CharsParsed, &ItemIDList, &Attributes);
			pShellFolder->Release();
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*//---------------------------------------------------------------------------
int __stdcall BrowseProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch(uMsg)
	{
		case BFFM_INITIALIZED:
		{
			TBrowseStruct *BrowseStruct = reinterpret_cast<TBrowseStruct*>(lpData);

			if (BrowseStruct)
			{
				if (BrowseStruct->CentreRect != TRect(0, 0, 0, 0))//BrowseStruct->Parent)
				{
					TRect r1, r2;
					GetWindowRect(hwnd, &r1);
					r2 = BrowseStruct->CentreRect;

					MoveWindow(
							hwnd,
							((r2.right + r2.left) - (r1.right - r1.left)) / 2,
							((r2.top + r2.bottom) - (r1.Bottom - r1.Top)) / 2,
							r1.Right - r1.Left,
							r1.Bottom - r1.Top,
							false);
				}
				if (!BrowseStruct->InitialDirectory.IsEmpty())
				{
					// the browse dialog will not correctly set the desired
					// path if there's a trailing '\' at the end of it when it's
					// not a root path
					if (BrowseStruct->InitialDirectory.Length() > 3)
					{
						while (BrowseStruct->InitialDirectory.IsPathDelimiter(BrowseStruct->InitialDirectory.Length()))
						{
							BrowseStruct->InitialDirectory.Delete(BrowseStruct->InitialDirectory.Length(), 1);
						}
					}
					LPITEMIDLIST pidl = NULL;
//					SHPathToPIDL((char *)lpData, &pidl);
					GetIDListFromPath(BrowseStruct->InitialDirectory, pidl);
					SendMessage(hwnd, BFFM_SETSELECTION, false, (LPARAM)pidl);
//					SendMessage(hwnd, BFFM_SETSELECTION, TRUE, reinterpret_cast<LPARAM>(BrowseStruct->InitialDirectory.c_str()));
				}
			}
			break;
		}
	}
//	LPMALLOC g_pMalloc;
	// Get the shell's allocator.
//	if (!SUCCEEDED(SHGetMalloc(&g_pMalloc)))
//	{
//		return false;
//	}
//	g_pMalloc->Free(pidl);
	return 0;
/*	char szDir[MAX_PATH];

	switch(uMsg)
	{
		case BFFM_INITIALIZED:
			SendMessage(hwnd, BFFM_SETSTATUSTEXT,0, (LPARAM)"Greetings");

			// Set the initial directory. If WPARAM is TRUE, then LPARAM is a
			// string that contains the path. If WPARAM is FALSE, then LPARAM
			// should be a lovely PIDL
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)"C:\\Delphi4");
			break;
		case BFFM_SELCHANGED:
			if(SHGetPathFromIDList((LPITEMIDLIST)lParam, szDir))
			{
//				Form1->Label3->Caption = szDir;
			}
			break;
		}
		return 0; */
/*}
//---------------------------------------------------------------------------
/*
{Author:	Poul Bak}
{Copyright © 1999-2001 : BakSoft-Denmark (Poul Bak). All rights reserved.}
{http://home11.inet.tele.dk/BakSoft/}
{Mailto: baksoft-denmark@dk2net.dk}
{}
{Component Version: 3.00.00.00}
{}
{PBFolderDialog is SHBrowseForFolder dialog with capability of creating new folders when browsing for a folder.}
{It can show path above the window.}
{The 'New folder'-button caption and a 'Label'-caption (shown above the path) are automatic localized (national language) detected every time the application runs.}
{Version 2.00.00.00 automatically calculates the width of the 'New folder' buttoon at runtime depending on the actual localized caption.}
{Now localized error-message if a new folder could not be created.}
{Version 3.00.00.00 does not use the Registry - Windows NT/2000 safe.}

{The component is based on Todd Fast's TBrowseFolder component.}
//
{Thanks to Gaetano Giunta for great bug-fixing in version 1.20.00.00}
{Thanks to Daniel Deycard for reporting a problem with using large fonts in Windows.}
{Thanks to Peter Aschbacher for great translation and bug-reporting.}
{Thanks to Taine G. for fixing a network-folder problem.}
{Thanks to Oliver Sturm for fixing a bug changing captions at runtime.}
{Thanks to Diederik Wierenga for fixing a display-path problem.}
{Thanks to Laurent Baudrillard for great translations.}
{"Too many cooks spoil the broth !"}
{Thanks to Bernard White for fixing a bug introduced in version 2.00 (Delphi 3).}
{Thanks to Peter Magor for fixing a bug if application has no mainform.}

unit PBFolderDialog;

interface

uses
	Windows, Messages, Classes, Forms, Dialogs, SysUtils, ActiveX, Shlobj,
	FileCtrl, Controls, Graphics, ShellApi, StdCtrls;

type
{Decides what foldertypes to accept and whether to show path.}
	TBrowseInfoFlags=(OnlyComputers, OnlyPrinters, OnlyDomains, OnlyAncestors,
		OnlyFileSystem, ShowPath);
{Decides what foldertypes to accept and whether to show path.}
	TBrowseInfoFlagSet=set of TBrowseInfoFlags;
{List of Foldernames used as the root-folder. Users can not browse to a folder above that level.}
	TSHFolders=(foDesktop,foDesktopExpanded,foPrograms,foControlPanel,foPrinters,foPersonal,foFavorites,foStartup,foRecent,
		foSendto,foRecycleBin,foStartMenu,foDesktopFolder,foMyComputer,foNetwork,foNetworkNeighborhood,
		foFonts,foTemplates);

	TPBFolderDialog = class;
{The event that is triggered when the dialog has initialized.}
	TBrowserInitializedEvent=procedure(Sender: TPBFolderDialog; DialogHandle: HWND) of object;
{The event that is triggered whenever a folder is selected.}
{Manually add 'ShlObj' to the uses clause of your forms unit, when you create a TSelectionChangedEvent.}
	TSelectionChangedEvent=procedure(Sender: TPBFolderDialog; DialogHandle: HWND; const ItemIDList: PItemIDList; const Folder: String) of object;

{Author:	Poul Bak}
{Copyright © 1999-2001 : BakSoft-Denmark (Poul Bak). All rights reserved.}
{http://home11.inet.tele.dk/BakSoft/}
{Mailto: baksoft-denmark@dk2net.dk}
{}
{Component Version: 2.20.00.00}
{}
{PBFolderDialog is SHBrowseForFolder dialog with capability of creating new folders when browsing for a folder.}
{It can show path above the window.}
{The 'New folder'-button caption and a 'Label'-caption (shown above the path) are automatic localized (national language) detected every time the application runs.}
{Version 2.00.00.00 automatically calculates the width of the 'New folder' buttoon at runtime depending on the actual localized caption.}
{Now localized error-message if a new folder could not be created.}
{Version 3.00.00.00 does not use the Registry - Windows NT/2000 safe.}
	TPBFolderDialog = class(TComponent)
	private
		FDialogHandle : HWnd;
		FNewFolderHandle : HWnd;
		FLabelCaption : String;
		FParentHandle : HWnd;
		FDisplayName : String;
		FImageIndex : Integer;
		FFolder : String;
		FSelectedFolder : String;
		FFlags : TBrowseInfoFlagSet;
		FRootFolder : TSHFolders;
		FNewFolderVisible : Boolean;
		FNewFolderEnabled : Boolean;
		FNewFolderCaption : String;
		FRestart : Boolean;
		FValidPath : Boolean;
		FVersion, FLocale : string;
		FNewFolderCaptions : TStringList;
		FLabelCaptions : TStringList;
		PStartItemIDList : PItemIDList;
		FInitialized : Boolean;
		FOnInitialized : TBrowserInitializedEvent;
		FOnSelectionChanged : TSelectionChangedEvent;
		function LocaleText(List : TStringList) : string;
		function MakeDisplayPath(Path : string; MaxL : integer) : string;
		function StoreNewFolderCaptions : Boolean;
		function StoreCurrentFolderCaptions : Boolean;
		procedure Dummy(Value: String);
		procedure SetNewFolderCaption(Value : String);
		procedure SetNewFolderEnabled(Value : Boolean);
		procedure SetNewFolderVisible(Value : Boolean);
		procedure SetNewFolderCaptions(Value : TStringList);
		procedure SetSelectedFolder(Value : String);
		procedure SetFlags(Value : TBrowseInfoFlagSet);
		procedure SetLabelCaptions(Value : TStringList);
		procedure LabelCaptionsChange (Sender: TObject);
		procedure NewFolderCaptionsChange (Sender: TObject);
	protected
	public
		constructor Create(AOwner : TComponent); override;
		procedure Loaded; override;
		destructor Destroy; override;
{Use the Execute function to browse for a folder. if the user presses 'Ok' the Folder-property will contain the path to the selected folder.}
{If the user presses 'Cancel' the Folder-property will not change.}
		function Execute : Boolean;
{Use this procedure to set the selected folder to an ItemIDList.}
{Use it from an event.}
		procedure SetSelectionPIDL(const Hwnd : HWND; const ItemIDList : PItemIDList);
{Use this procedure to set the selected folder to a path.}
{Use it from an event.}
		procedure SetSelectionPath(const Hwnd : HWND; const Path : String);
{Use this procedure to Enable/Disable the 'Ok'-button.}
{Use it from an event.}
		procedure EnableOK(const Hwnd : HWND; const Value : Boolean);
{Use this procedure to get an ItemIDList, when you know the path.}
		procedure GetIDListFromPath(Path: String; var ItemIDList: PItemIDList);
{This property gives the Window-title (when you open a folder in Explorer).}
{The DisplayName is normally the short foldername.}
		property DisplayName : String read FDisplayName;
{A system index to the image for the folder.}
		property ImageIndex : Integer read FImageIndex;
{The handle of the parent window (the form that called the dialog.}
		property ParentHandle : HWnd read FParentHandle write FParentHandle;
{The handle of the dialog.}
		property DialogHandle : HWnd read FDialogHandle write FDialogHandle;
{The handle of the 'New folder' button.}
		property NewFolderHandle : HWnd read FNewFolderHandle write FNewFolderHandle;
{The currently selected folder. You can access and set this path in one of the events.}
		property SelectedFolder : String read FSelectedFolder write SetSelectedFolder;
	published
{The Folder that is selected when the dialog opens and, when returned, contains the path to the folder the user selected.}
		property Folder : String read FFolder write FFolder;
{Decides what foldertypes to accept and whether to show path.}
		property Flags : TBrowseInfoFlagSet read FFlags write SetFlags;
{The root-folder. Users can not browse to a folder above that level.}
		property RootFolder : TSHFolders read FRootFolder write FRootFolder default foDesktopExpanded;
{Decides if the 'New folder' button shall be visible.}
		property NewFolderVisible : Boolean read FNewFolderVisible write SetNewFolderVisible default True;
{Decides if the 'New folder' button shall be enabled.}
		property NewFolderEnabled : Boolean read FNewFolderEnabled write SetNewFolderEnabled default True;
{The event that is triggered when the dialog has initialized.}
		property OnInitialized : TBrowserInitializedEvent read FOnInitialized write FOnInitialized;
{The event that is triggered whenever a folder is selected.}
{Manually add 'ShlObj' to the uses clause of your forms unit, when you create an OnSelectionChanged event.}
		property OnSelectionChanged : TSelectionChangedEvent read FOnSelectionChanged write FOnSelectionChanged;
{LabelCaptions is the localized caption-list for the caption above the browsewindow.}
{See the 'International codes.txt'-file to find the codes.}
{At runtime the text that fits the Windows-language is used.}
{If the Windows-localeversion is not found in the list the 'Default'-value is used.}
		property LabelCaptions : TStringList read FLabelCaptions
			write SetLabelCaptions stored StoreCurrentFolderCaptions;
{NewFolderCaptions is the localized caption-list for the caption of the 'New folder' button.}
{See the 'International codes.txt'-file to find the codes.}
{At runtime the text that fits the Windows-language is used.}
{If the Windows-localeversion is not found in the list the 'Default'-value is used.}
		property NewFolderCaptions : TStringList read FNewFolderCaptions
			write SetNewFolderCaptions stored StoreNewFolderCaptions;
//ReadOnly property.
		property Version : string read FVersion write Dummy stored False;
	end;

procedure Register;

implementation

const
	MinButtonWidth = 75;
	_BUTTON_ID=255;
	MAX_PATH_DISPLAY_LENGTH=50;
	NUMBER_OF_BROWSE_INFO_FLAGS=6;
	BROWSE_FLAG_ARRAY: array[TBrowseInfoFlags] of Integer = (BIF_BROWSEFORCOMPUTER,
		BIF_BROWSEFORPRINTER, BIF_DONTGOBELOWDOMAIN, BIF_RETURNFSANCESTORS,
		BIF_RETURNONLYFSDIRS, BIF_STATUSTEXT);
	SH_FOLDERS_ARRAY: array[TSHFolders] of Integer=
		(CSIDL_DESKTOP,-1,
		CSIDL_PROGRAMS,CSIDL_CONTROLS,CSIDL_PRINTERS,CSIDL_PERSONAL,CSIDL_FAVORITES,
		CSIDL_STARTUP,CSIDL_RECENT,CSIDL_SENDTO,CSIDL_BITBUCKET,CSIDL_STARTMENU,CSIDL_DESKTOPDIRECTORY,
		CSIDL_DRIVES,CSIDL_NETWORK,CSIDL_NETHOOD,CSIDL_FONTS,CSIDL_TEMPLATES);
	NEW_FOLDER_CAPTIONS =	'"Default=New folder","0009=New folder",' +
		'"0406=Ny mappe","0407=Neuer Ordner","0409=New folder",' +
		'"040C=Nouveau Dossier","0410=Nuova Cartella","0413=Nieuwe map",' +
		'"0415=Nowy folder","0416=Nova Pasta","0807=Neuer Ordner",' +
		'"0809=New folder","080C=Nouveau Dossier","0810=Nuova Cartella",' +
		'"0C07=Neuer Ordner","0C09=New folder","0C0C=Nouveau Dossier",' +
		'"1007=Neuer Ordner","1009=New folder","100C=Nouveau Dossier",' +
		'"1407=Neuer Ordner","1409=New folder","140C=Nouveau Dossier",' +
		'"1809=New folder","180C=Nouveau Dossier","1C09=New folder",' +
		'"2009=New folder","2809=New folder","2C09=New folder"';
	CURRENT_FOLDER_CAPTIONS = '"Default=Current folder:","0009=Current folder:",' +
		'"0406=Valgt mappe:","0407=Ausgewählter Ordner:","0409=Current folder:",' +
		'"040C=Dossier Courant:","0410=Cartella selezionata:","0413=Huidige map:",' +
		'"0415=Bie¿±cy folder","0416=Pasta Atual:","0807=Ausgewählter Ordner:",' +
		'"0809=Current folder:","080C=Dossier Courant:","0810=Cartella selezionata:",' +
		'"0C07=Ausgewählter Ordner:","0C09=Current folder:","0C0C=Dossier Courant:",' +
		'"1007=Ausgewählter Ordner:","1009=Current folder:","100C=Dossier Courant:",' +
		'"1407=Ausgewählter Ordner:","1409=Current folder:","140C=Dossier Courant:",' +
		'"1809=Current folder:","180C=Dossier Courant:","1C09=Current folder:",' +
		'"2009=Current folder:","2809=Current folder:","2C09=Current folder:"';

procedure CenterWindow(HWindow: HWND);
var
	Rect0: TRect;
begin
	GetWindowRect(HWindow,Rect0);
	SetWindowPos(HWindow,0,
		(Screen.Width div 2) - ((Rect0.Right-Rect0.Left) div 2),
		(Screen.Height div 2) - ((Rect0.Bottom - Rect0.Top) div 2),
		0,0,SWP_NOACTIVATE or SWP_NOSIZE or SWP_NOZORDER);
end;

function WndProc(HWindow: HWND; Msg: UINT; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall;
var
	Instance: TPBFolderDialog;
	NewFolder, TempFolder : String;
	Buffer : PChar;
begin
	Instance:=TPBFolderDialog(GetWindowLong(HWindow,GWL_USERDATA));
	if (Msg=WM_COMMAND) and (Lo(wParam)=_BUTTON_ID) then
	begin
		NewFolder := InputBox(Instance.FNewFolderCaption, '', '');
		if NewFolder <> '' then
		begin
			Instance.FRestart := True;
			if (NewFolder[1] <> '\')
				and (Instance.FSelectedFolder[Length(Instance.FSelectedFolder)] <> '\')
				then NewFolder := '\' + NewFolder;
			TempFolder := Instance.FSelectedFolder + NewFolder;
			try
				ForceDirectories(TempFolder);
				if DirectoryExists(TempFolder) then Instance.FSelectedFolder := TempFolder
				else Raise EExternalException.Create('');
			except
				Instance.FInitialized := False;
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER or FORMAT_MESSAGE_FROM_SYSTEM,
					nil, 82, 0, @Buffer, 256, nil);
				Application.MessageBox(Buffer, PChar(Instance.FNewFolderCaption),
					MB_OK+MB_ICONEXCLAMATION+MB_DEFBUTTON1+MB_APPLMODAL);
				LocalFree(Cardinal(@Buffer));
			end;
			Keybd_Event(VK_ESCAPE, 0, 0, 0);
			Keybd_Event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
		end;
		Result:=0;
	end
	else Result:=DefDlgProc(HWindow,Msg,wParam,lParam);
end;

procedure AddControls(HWindow: HWND; Instance: TPBFolderDialog);
var
	NewFolderWindowHandle : HWND;
	ControlCreateStyles : Integer;
	Height0, TextWidth : integer;
	Rect0, Rect1 : TRect;
	TopLeft : TPoint;
begin
	ControlCreateStyles := WS_CHILD or WS_CLIPSIBLINGS or WS_VISIBLE or WS_TABSTOP
		or BS_PUSHBUTTON;
	GetClientRect(HWindow, Rect0);
	Height0 := Rect0.Bottom - Rect0.Top;
	case Screen.PixelsPerInch of
		72, 96: NewFolderWindowHandle:=CreateWindow('Button', PChar(Instance.FNewFolderCaption),
			ControlCreateStyles, 12, Height0 - 36, MinButtonWidth, 23,
			HWindow, _BUTTON_ID, HInstance, nil);
		120: NewFolderWindowHandle:=CreateWindow('Button', PChar(Instance.FNewFolderCaption),
			ControlCreateStyles, 15, Height0 - 45, MinButtonWidth * 5 div 4, 28,
			HWindow, _BUTTON_ID, HInstance, nil);
		144: NewFolderWindowHandle:=CreateWindow('Button', PChar(Instance.FNewFolderCaption),
			ControlCreateStyles, 17, Height0 - 56, MinButtonWidth * 3 div 2, 35,
			HWindow, _BUTTON_ID, HInstance, nil);
		192: NewFolderWindowHandle:=CreateWindow('Button', PChar(Instance.FNewFolderCaption),
			ControlCreateStyles, 20, Height0 - 66, MinButtonWidth * 192 div 96,
			41, HWindow, _BUTTON_ID, HInstance, nil);
		else NewFolderWindowHandle:=CreateWindow('Button', PChar(Instance.FNewFolderCaption),
			ControlCreateStyles, 12 * Screen.PixelsPerInch div 96,
			Height0 - GetSystemMetrics(SM_CYCAPTION) - 17,
			MinButtonWidth * Screen.PixelsPerInch div 96, 23 * Screen.PixelsPerInch div 96,
			HWindow, _BUTTON_ID, HInstance, nil);
	end;
	with TBitmap.Create do
	begin
		TextWidth := Canvas.TextWidth(Instance.FNewFolderCaption);
		PostMessage(NewFolderWindowHandle, WM_SETFONT, Longint(Canvas.Font.Handle),
			MAKELPARAM(1,0));
		Free;
	end;
	GetClientRect(NewFolderWindowHandle, Rect1);
	Rect1.Right := Rect1.Left + TextWidth + 12;
	if (Rect1.Right < MinButtonWidth) and (Screen.PixelsPerInch = 72)
		then Rect1.Right := MinButtonWidth
	else if Rect1.Right < MinButtonWidth * Screen.PixelsPerInch div 96
		then Rect1.Right := MinButtonWidth * Screen.PixelsPerInch div 96;
	AdjustWindowRect(Rect1, ControlCreateStyles, False);
	GetWindowRect(NewFolderWindowHandle, Rect0);
	TopLeft := Rect0.TopLeft;
	ScreenToClient(HWindow, TopLeft);
	MoveWindow(NewFolderWindowHandle, TopLeft.x, TopLeft.y, Rect1.Right,
		Rect1.Bottom, True);
	EnableWindow(NewFolderWindowHandle,Instance.FNewFolderEnabled);
	SetWindowLong(HWindow,GWL_WNDPROC,Longint(@WndProc));
	Instance.FNewFolderHandle:=NewFolderWindowHandle;
end;

procedure BrowserCallbackProc(HWindow: HWND; uMsg: Integer; lParameter: LPARAM;
	lpPBFolderDialog: LPARAM); stdcall;
var
	Instance: TPBFolderDialog;
	Path: String;
	TempPath: array[0..MAX_PATH] of Char;
	SHFolderI: IShellFolder;
	lpDisplay : TStrRet;
	ShellFileInfo :  TSHFileInfo;
begin
	Instance:=TPBFolderDialog(lpPBFolderDialog);
	case uMsg of
		BFFM_INITIALIZED:
		begin
			Instance.DialogHandle:=HWindow;
			CenterWindow(HWindow);
			SetWindowLong(HWindow,GWL_USERDATA,lpPBFolderDialog);
			AddControls(HWindow,Instance);
			if IsWindow(Instance.FNewFolderHandle) then
			begin
				EnableWindow(Instance.FNewFolderHandle, Instance.FValidPath and Instance.FNewFolderEnabled); //G. Giunta 2000.03.24: enable button if path is valid and property is set
				ShowWindow(Instance.FNewFolderHandle,(SW_HIDE and Variant(not Instance.FNewFolderVisible)) or (SW_SHOW and Variant(Instance.FNewFolderVisible))); //G. Giunta 2000.03.24: show/hide new directory button
			end;
			if DirectoryExists(Instance.FFolder) then
				Instance.SetSelectionPath(HWindow,Instance.FFolder)
			else if (Instance.PStartItemIDList <> nil)
				then Instance.SetSelectionPIDL(HWindow, Instance.PStartItemIDList);
			Instance.FInitialized := True;
			if Assigned(Instance.OnInitialized) then
				Instance.OnInitialized(Instance,HWindow);
		end;
		BFFM_SELCHANGED:
		begin
			SHGetPathFromIDList(PItemIDList(lParameter),TempPath);
			if Instance.FInitialized then Instance.PStartItemIDList := PItemIDList(lParameter);
			Instance.FSelectedFolder := StrPas(TempPath);
			if (ShowPath in Instance.FFlags) then
			begin
				SetLength(Path,MAX_PATH);
				Path := Instance.MakeDisplayPath(StrPas(TempPath), MAX_PATH_DISPLAY_LENGTH);
				if Path = '' then
				begin
					if SHGetDeskTopFolder(SHFolderI) = NOERROR then
					begin
						SHFolderI.GetDisplayNameOf(PItemIDList(lParameter), SHGDN_FORPARSING, lpDisplay);
						if lpDisplay.uType = STRRET_CSTR then Path := StrPas(lpDisplay.cStr);
					end;
				end;
				if Path = '' then
				begin
						 SHGetFileInfo( PChar(lParameter),0,ShellFileInfo,sizeof(ShellFileInfo),SHGFI_PIDL or SHGFI_DISPLAYNAME or SHGFI_TYPENAME);
						 Path := StrPas(ShellFileInfo.szDisplayName);
						 Instance.FSelectedFolder := StrPas(ShellFileInfo.szDisplayName);
				end;
				SendMessage(HWindow,BFFM_SETSTATUSTEXT,0,Longint(PChar(Path)));
			end;
			Instance.FValidPath := (Instance.FSelectedFolder <> '');
			if (OnlyFileSystem in Instance.FFlags) then Instance.EnableOK(HWindow, Instance.FValidPath)
			else Instance.EnableOK(HWindow, True);
			if IsWindow(Instance.FNewFolderHandle) then
			 EnableWindow(Instance.FNewFolderHandle, Instance.FValidPath and Instance.FNewFolderEnabled); //G. Giunta 2000.03.24: enable button if path is valid and property is set
			if Assigned(Instance.OnSelectionChanged) then
				Instance.OnSelectionChanged(Instance,HWindow,PItemIDList(lParameter),Instance.SelectedFolder);
		end;
	end;
end;

constructor TPBFolderDialog.Create(AOwner: TComponent);
begin
	inherited Create(AOwner);
	SetLength(FDisplayName,MAX_PATH);
	SetLength(FFolder,MAX_PATH);
	FParentHandle:=0;
	FRootFolder:=foDesktopExpanded;
	FNewFolderVisible:=True;
	FNewFolderEnabled:=True;
	FFlags:=[ShowPath];
	FFolder:='';
	FSelectedFolder:='';
	PStartItemIDList := nil;
	FInitialized := False;
	FValidPath := True;
	FVersion := '3.00.00.00';
	FLocale := IntToHex(GetSystemDefaultLangID, 4);
	FNewFolderCaptions := TStringList.Create;
	FNewFolderCaptions.OnChange := NewFolderCaptionsChange;
	FNewFolderCaptions.CommaText := NEW_FOLDER_CAPTIONS;
	FLabelCaptions := TStringList.Create;
	FLabelCaptions.OnChange := LabelCaptionsChange;
	FLabelCaptions.CommaText := CURRENT_FOLDER_CAPTIONS;
end;

procedure TPBFolderDialog.Loaded;
begin
	SetNewFolderCaption(LocaleText(FNewFolderCaptions));
	FLabelCaption := LocaleText(FLabelCaptions);
end;

destructor TPBFolderDialog.Destroy;
begin
	FNewFolderCaptions.Free;
	FLabelCaptions.Free;
	inherited Destroy;
end;

procedure TPBFolderDialog.GetIDListFromPath(Path: String; var ItemIDList: PItemIDList);
var
	ShellFolderInterface: IShellFolder;
	CharsParsed: ULONG;
	Attributes: ULONG;
begin
	if SHGetDesktopFolder(ShellFolderInterface)=NOERROR then
	begin
		if DirectoryExists(Path) then
			ShellFolderInterface.ParseDisplayName(0,nil,StringToOleStr(Path),CharsParsed,ItemIDList,Attributes);
	end;
end;

procedure TPBFolderDialog.SetSelectionPIDL(const Hwnd: HWND; const ItemIDList: PItemIDList);
begin
	SendMessage(Hwnd,BFFM_SETSELECTION,Ord(FALSE),Longint(ItemIDList));
end;

procedure TPBFolderDialog.SetSelectionPath(const Hwnd: HWND; const Path: String);
var
	ItemIDList : PItemIDList;
begin
	GetIDListFromPath(Path, ItemIDList);
	SendMessage(Hwnd,BFFM_SETSELECTION,Ord(FALSE),Longint(ItemIDList));
end;

procedure TPBFolderDialog.EnableOK(const Hwnd: HWND; const Value: Boolean);
begin
	SendMessage(Hwnd,BFFM_ENABLEOK,0,Ord(Value));
end;

procedure TPBFolderDialog.SetNewFolderVisible(Value: Boolean);
begin
	if FNewFolderVisible <> Value then
	begin
		FNewFolderVisible:=Value;
		if IsWindow(FNewFolderHandle) then
			ShowWindow(FNewFolderHandle,(SW_HIDE and not Variant(Value)) or (SW_SHOW and Variant(Value)));
	end;
end;

procedure TPBFolderDialog.SetNewFolderEnabled(Value: Boolean);
begin
	FNewFolderEnabled:=Value;
	if (IsWindow(FNewFolderHandle)) then EnableWindow(FNewFolderHandle,Value and FValidPath); //G. Giunta 2000.03.24: enable button if path is valid and property is set
end;

procedure TPBFolderDialog.SetNewFolderCaption(Value: String);
begin
	FNewFolderCaption:=Value;
	if (IsWindow(FNewFolderHandle)) then SetWindowText(FNewFolderHandle,PChar(Value));
end;

procedure TPBFolderDialog.SetSelectedFolder(Value: String);
begin
	SetSelectionPath(DialogHandle,Value);
end;

procedure TPBFolderDialog.SetFlags(Value : TBrowseInfoFlagSet);
begin
	FFlags := Value;
end;

procedure TPBFolderDialog.SetLabelCaptions(Value : TStringList);
begin
	if FLabelCaptions.Text <> Value.Text then
	begin
		FLabelCaptions.Assign(Value);
	end;
end;

procedure TPBFolderDialog.SetNewFolderCaptions(Value : TStringList);
begin
	if FNewFolderCaptions.Text <> Value.Text then
	begin
		FNewFolderCaptions.Assign(Value);
	end;
end;

procedure TPBFolderDialog.LabelCaptionsChange (Sender: TObject);
begin
	FLabelCaption := LocaleText(FLabelCaptions);
end;

function TPBFolderDialog.StoreCurrentFolderCaptions : Boolean;
begin
	Result := (FLabelCaptions.CommaText <> CURRENT_FOLDER_CAPTIONS);
end;

procedure TPBFolderDialog.NewFolderCaptionsChange (Sender: TObject);
begin
	SetNewFolderCaption(LocaleText(FNewFolderCaptions));
end;

function TPBFolderDialog.StoreNewFolderCaptions : Boolean;
begin
	Result := (FNewFolderCaptions.CommaText <> NEW_FOLDER_CAPTIONS);
end;

procedure TPBFolderDialog.Dummy(Value: String);
begin
//	Read only !
end;

function TPBFolderDialog.LocaleText(List : TStringList) : string;
begin
	if List.Count = 0 then Result := ''
	else
	begin
		if List.IndexOfName(FLocale) <> -1 then Result := List.Values[FLocale]
		else if List.IndexOfName('Default') <> -1 then Result := List.Values['Default']
		else Result := List.Values[List.Names[0]];
	end;
end;

function TPBFolderDialog.MakeDisplayPath(Path : string; MaxL : integer) : string;
var
	t, Pos0, NumBack : integer;
begin
	Result := '';
	if (Length(Path) <= MaxL) or (MaxL < 6) or (Pos('\', Path) = 0) then Result := Copy(Path, 1, MaxL)
	else
	begin
		NumBack := 0;
		for t := 3 to Length(Path) do if (Path[t] = '\') then inc(NumBack);
		if NumBack < 2 then Result := Copy(Path, 1, MaxL)
		else
		begin
			Pos0 := Pos('\', Path);
			if Pos0 < 3 then
			begin
				Result := '\\';
				Path := Copy(Path, 3, Length(Path) - 2);
			end;
			Pos0 := Pos('\', Path);
			Result := Result + Copy(Path, 1, Pos0) + '...';
			repeat
				Path := Copy(Path, Pos0 + 1, Length(Path) - Pos0);
				Pos0 := Pos('\', Path);
			until ((Length(Result + Path) + 1) <= MaxL) or (Pos0 = 0);
			if ((Length(Result + Path) + 1) <= MaxL) then Result := Result + '\' + Path
			else Result := Copy(Result + '\' + Path, 1, MaxL - 3) + '...';
		end;
	end;
end;

function TPBFolderDialog.Execute: Boolean;
var
	BrowseInfo: TBrowseInfo;
	ItemIDList: PItemIDList;
	i: Integer;
	TempPath: array[0..MAX_PATH] of Char;
begin
	FSelectedFolder := FFolder;
	ItemIDList := nil;
	try
		if IsWindow(FParentHandle) then BrowseInfo.hwndOwner:=FParentHandle
		else if (Owner is TWinControl) then BrowseInfo.hwndOwner:=TWinControl(Owner).Handle
		else if Assigned(Application.MainForm) then BrowseInfo.hwndOwner:=Application.MainForm.Handle
		else BrowseInfo.hwndOwner:=Application.Handle;
		if FRootFolder=foDesktopExpanded then BrowseInfo.pidlRoot:=nil
		else SHGetSpecialFolderLocation(Application.Handle,SH_FOLDERS_ARRAY[FRootFolder],BrowseInfo.pidlRoot);
		BrowseInfo.pszDisplayName:=PChar(FDisplayName);
		BrowseInfo.lpszTitle:=PChar(FLabelCaption);
		BrowseInfo.ulFlags:=0;
		for i:=0 to NUMBER_OF_BROWSE_INFO_FLAGS-1 do
		begin
			if( TBrowseInfoFlags(i) in FFlags)
				then	BrowseInfo.ulFlags:=BrowseInfo.ulFlags
				or DWord(BROWSE_FLAG_ARRAY[TBrowseInfoFlags(i)]);
		end;
		BrowseInfo.lpfn:=@BrowserCallbackProc;
		BrowseInfo.lParam:=Longint(Self);
		BrowseInfo.iImage:=0;
		FSelectedFolder := FFolder;
		PStartItemIDList := nil;
		repeat
			FInitialized := False;
			FRestart := False;
			FFolder := FSelectedFolder;
			{SHBrowseForFolder; return is nil if user cancels}
			ItemIDList:=SHBrowseForFolder(BrowseInfo);
		until not FRestart;
		Result:=(ItemIDList <> nil);
		if Result then
		begin
			SHGetPathFromIDList(ItemIDList,TempPath);
			FFolder:=StrPas(TempPath);
			FSelectedFolder:=FFolder;
			FImageIndex:=BrowseInfo.iImage;
		end;
	finally
		CoTaskMemFree(ItemIDList);
		CoTaskMemFree(BrowseInfo.pidlRoot);
	end;
end;

procedure Register;
begin
	RegisterComponents('PB', [TPBFolderDialog]);
end;

end.
*/







/*
I guess I will post the answer to my own question! I finally figured out a
way to get what I want but the WIN32 Programmer's help file says that what I
did was not quite appropriate! Oh well... Since there didn't seem to be a
better way offered, I made the slightly inappropriate fix to TOpenDialog by
making a TSelectDialog. I hide the Filter and FileName controls inside the
ExplorerHook function.

When I use TSelectDialog, I set the Filename to '~' and the Filter to 'Hide
all files|~'. This way only directories are visible. When the Execute method
returns, I strip off the '~' from Filename. Voila! I have the directory!

Maybe somebody can tell me how to do this in a cleaner fashion since I
really only needed to change the ExplorerHook function of TOpenDialog. All
of the other stuff was simply copying everything from TOpenDialog so that
TSelectDialog would work using my ExplorerHook function.

----------------------------------
Here's my code:

type
  TSelectDialog = class(TOpenDialog)
  private
    FCtl3D: Boolean;
    FDefaultExt: string;
    FTitle: string;
    FInitialDir: string;
    FOptions: TOpenOptions;
    FFiles: TStrings;
    FFileName: TFileName;
    FFilter: string;
    FFilterIndex: Integer;
    function DoExecute(Func: Pointer): Bool;
    procedure SetInitialDir(const Value: string);
  public
    property Files: TStrings read FFiles;
    function Execute: Boolean; override;
    constructor Create(AOwner: TComponent); override;
  published
    property Ctl3D: Boolean read FCtl3D write FCtl3D default True;
    property DefaultExt: string read FDefaultExt write FDefaultExt;
    property InitialDir: string read FInitialDir write SetInitialDir;
    property Options: TOpenOptions read FOptions write FOptions default [];
    property FileName: TFileName read FFileName write FFileName;
    property Filter: string read FFilter write FFilter;
    property FilterIndex: Integer read FFilterIndex write FFilterIndex
default 1;
    property Title: string read FTitle write FTitle;
  end;

{ Center the given window on the screen }
procedure CenterWindow(Wnd: HWnd);
var
  Rect: TRect;
begin
  GetWindowRect(Wnd, Rect);
  SetWindowPos(Wnd, 0,
    (GetSystemMetrics(SM_CXSCREEN) - Rect.Right + Rect.Left) div 2,
    (GetSystemMetrics(SM_CYSCREEN) - Rect.Bottom + Rect.Top) div 3,
    0, 0, SWP_NOACTIVATE or SWP_NOSIZE or SWP_NOZORDER);
end;

function ExplorerHook(Wnd: HWnd; Msg: UINT; WParam: WPARAM; LParam: LPARAM):
UINT; stdcall;
var
  hDlg: HWnd;
begin
  Result := 0;
  if (Msg = WM_NOTIFY) and (POFNotify(LParam)^.hdr.code = CDN_INITDONE) then
  begin
    hDlg := GetWindowLong(Wnd, GWL_HWNDPARENT);
    SendMessage(hDlg, CDM_HIDECONTROL, 1090, 0);  {Filename}
    SendMessage(hDlg, CDM_HIDECONTROL, 1152, 0);  {Filename label}
    SendMessage(hDlg, CDM_HIDECONTROL, 1089, 0);  {Filetypes}
    SendMessage(hDlg, CDM_HIDECONTROL, 1136, 0);  {Filetypes label}
    CenterWindow(hDlg);
  end;
end;

constructor TSelectDialog.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FFiles := TStringList.Create;
  FFilterIndex := 1;
end;

procedure TSelectDialog.SetInitialDir(const Value: string);
var
  L: Integer;
begin
  L := Length(Value);
  if (L > 1) and (Value[L] = '\') and (Value[L - 1] <> ':') then Dec(L);
  FInitialDir := Copy(Value, 1, L);
end;

function TSelectDialog.DoExecute(Func: Pointer): Bool;
const
  MultiSelectBufferSize = 8192;
  OpenOptions: array [TOpenOption] of Longint = (
    OFN_READONLY, OFN_OVERWRITEPROMPT, OFN_HIDEREADONLY,
    OFN_NOCHANGEDIR, OFN_SHOWHELP, OFN_NOVALIDATE, OFN_ALLOWMULTISELECT,
    OFN_EXTENSIONDIFFERENT, OFN_PATHMUSTEXIST, OFN_FILEMUSTEXIST,
    OFN_CREATEPROMPT, OFN_SHAREAWARE, OFN_NOREADONLYRETURN,
    OFN_NOTESTFILECREATE, OFN_NONETWORKBUTTON, OFN_NOLONGNAMES,
    OFN_EXPLORER, OFN_NODEREFERENCELINKS);
var
  Option: TOpenOption;
  P: PChar;
  CDefaultExt: array[0..3] of Char;
  OpenFilename: TOpenFilename;

  function AllocFilterStr(const S: string): PChar;
  var
    P: PChar;
  begin
    Result := nil;
    if S <> '' then
    begin
      Result := StrCopy(StrAlloc(Length(S) + 2), PChar(S));
      P := Result;
      while P^ <> #0 do
      begin
        if P^ = '|' then P^ := #0;
        Inc(P);
      end;
      Inc(P);
      P^ := #0;
    end;
  end;

  function FindExtension(P: PChar): PChar;
  begin
    Result := '';
    while P^ <> #0 do
    begin
      if P^ = '.' then Result := P + 1 else
        if P^ = '\' then Result := '';
      Inc(P);
    end;
  end;

  function ExtractFileName(P: PChar; var S: string): PChar;
  var
    Separator: Char;
  begin
    Separator := #0;
    if (ofAllowMultiSelect in FOptions) and
      ((ofOldStyleDialog in FOptions) or not NewStyleControls) then
      Separator := ' ';
    Result := P;
    while (Result[0] <> #0) and (Result[0] <> Separator) do Inc(Result);
    SetString(S, P, Result - P);
    if Result[0] = Separator then Inc(Result);
  end;

  procedure ExtractFileNames(P: PChar);
  var
    DirName, FileName: string;
  begin
    P := ExtractFileName(P, DirName);
    P := ExtractFileName(P, FileName);
    if FileName = '' then
      FFiles.Add(DirName)
    else
    begin
      if DirName[Length(DirName)] <> '\' then
        DirName := DirName + '\';
      repeat
        if (FileName[1] <> '\') and ((Length(FileName) <= 3) or
          (FileName[2] <> ':') or (FileName[3] <> '\')) then
          FileName := DirName + FileName;
        FFiles.Add(FileName);
        P := ExtractFileName(P, FileName);
      until FileName = '';
    end;
  end;

begin
  FFiles.Clear;
  FillChar(OpenFileName, SizeOf(OpenFileName), 0);
  with OpenFilename do
  try
    lStructSize := SizeOf(TOpenFilename);
    hInstance := System.HInstance;
    lpstrFilter := AllocFilterStr(FFilter);
    nFilterIndex := FFilterIndex;
    if ofAllowMultiSelect in FOptions then
      nMaxFile := MultiSelectBufferSize else
      nMaxFile := MAX_PATH;
    GetMem(lpstrFile, nMaxFile + 2);
    FillChar(lpstrFile^, nMaxFile + 2, 0);
    StrLCopy(lpstrFile, PChar(FFileName), nMaxFile);
    lpstrInitialDir := PChar(FInitialDir);
    lpstrTitle := PChar(FTitle);
    HookCtl3D := FCtl3D;
    Flags := OFN_ENABLEHOOK;
    for Option := Low(Option) to High(Option) do
      if Option in FOptions then
        Flags := Flags or OpenOptions[Option];
    if NewStyleControls then
      Flags := Flags xor OFN_EXPLORER
    else
      Flags := Flags and not OFN_EXPLORER;
    if FDefaultExt <> '' then
    begin
      P := PChar(FDefaultExt);
      if (P^ = #0) and (Flags and OFN_EXPLORER = 0) then
        P := FindExtension(PChar(FFileName));
      lpstrDefExt := StrLCopy(CDefaultExt, P, 3)
    end;
    lpfnHook := ExplorerHook;
    hWndOwner := Application.Handle;
    Result := TaskModalDialog(Func, OpenFileName);
    if Result then
    begin
      if ofAllowMultiSelect in FOptions then
      begin
        ExtractFileNames(lpstrFile);
        FFileName := FFiles[0];
      end else
      begin
        ExtractFileName(lpstrFile, FFileName);
        FFiles.Add(FFileName);
      end;
      if (Flags and OFN_EXTENSIONDIFFERENT) <> 0 then
        Include(FOptions, ofExtensionDifferent) else
        Exclude(FOptions, ofExtensionDifferent);
      if (Flags and OFN_READONLY) <> 0 then
        Include(FOptions, ofReadOnly) else
        Exclude(FOptions, ofReadOnly);
      FFilterIndex := nFilterIndex;
    end;
  finally
    if lpstrFile <> nil then FreeMem(lpstrFile, nMaxFile + 2);
    if lpstrFilter <> nil then StrDispose(lpstrFilter);
  end;
end;

function TSelectDialog.Execute: Boolean;
begin
  Result := DoExecute(@GetOpenFileName);
end;


--------------------------------------------------------------------------------
*/
