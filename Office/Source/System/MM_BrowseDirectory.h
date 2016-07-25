//---------------------------------------------------------------------------
/*enum TBorderIcon = { biSystemMenu, biMinimize, biMaximize, biHelp };
typedef Set<TBorderIcon, biSystemMenu, biHelp> TBorderIcons;
__property TBorderIcons BorderIcons = {read=FBorderIcons, write=SetBorderIcons, stored=IsForm, default=7}
*/

#ifndef MM_BrowseDirectoryH
#define MM_BrowseDirectoryH
//---------------------------------------------------------------------------
bool SelDir(AnsiString &Directory, AnsiString StartDir, AnsiString Caption, HWND Parent = NULL);
//---------------------------------------------------------------------------
//	Decides what foldertypes to accept and whether to show path.
enum TBrowseInfoFlags {
	OnlyComputers,
	OnlyPrinters,
	OnlyDomains,
	OnlyAncestors,
	OnlyFileSystem,

	NewDialogStyle,
	EditBox,
	NoNewFolderButton,
	IncludeFiles,
	ShowPath };
//	Decides what foldertypes to accept and whether to show path.
typedef Set<TBrowseInfoFlags, OnlyComputers, ShowPath> TBrowseInfoFlagSet;
// List of Foldernames used as the root-folder. Users can not browse to a folder above that level.
enum TSHFolders {
	foDesktop,
	foDesktopExpanded,
	foPrograms,
	foControlPanel,
	foPrinters,
	foPersonal,
	foFavorites,
	foStartup,
	foRecent,
	foSendto,
	foRecycleBin,
	foStartMenu,
	foDesktopFolder,
	foMyComputer,
	foNetwork,
	foNetworkNeighborhood,
	foFonts,
	foTemplates };
//---------------------------------------------------------------------------
struct TDialogSettings
{
	TDialogSettings() : RootFolder(foDesktopExpanded) { Flags << ShowPath << NewDialogStyle << EditBox << NoNewFolderButton; }

	TDialogSettings(
			AnsiString iInitialPath,
			AnsiString iCaption = "",
			TBrowseInfoFlagSet iFlags = TBrowseInfoFlagSet() << ShowPath << NewDialogStyle << EditBox << NoNewFolderButton) :
					RootFolder(foDesktopExpanded),
					InitialPath(iInitialPath),
					Flags(iFlags),
					Caption(iCaption) {}

	AnsiString 				InitialPath;
//	AnsiString 				DisplayName; // No go in new GUI (M$ Bug). Don't bother.
	AnsiString 				Caption;
	TBrowseInfoFlagSet	Flags;
	TSHFolders				RootFolder;
};
//---------------------------------------------------------------------------
class TFolderDialog
{
public:
	TFolderDialog(HWND Handle, TDialogSettings DialogSettings);
	
	bool						Execute();
	AnsiString				Selection() { return FSelectedFolder; }
private:
	static int __stdcall	BrowserCallbackProc(HWND HWindow, UINT uMsg, LPARAM lParam, LPARAM lpData);

	void						SetSelectionPath(HWND HWindow, AnsiString Path);
	void						SetSelectionPIDL(HWND HWindow, PItemIDList ItemIDList);
	void						GetIDListFromPath(AnsiString Path, PItemIDList &ItemIDList);
	void						EnableOK(HWND HWindow, bool Enable);

	TBrowseInfoFlagSet	FFlags;
	TSHFolders				FRootFolder;
	AnsiString				FLabelCaption;
	HWND						FParentHandle;
//	AnsiString				FDisplayName;
	AnsiString				FFolder;
	AnsiString				FSelectedFolder;
};
//---------------------------------------------------------------------------
#endif
