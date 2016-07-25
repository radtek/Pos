//---------------------------------------------------------------------------
#ifndef MM_RemoteBrowseClientH
#define MM_RemoteBrowseClientH
//---------------------------------------------------------------------------
#ifndef NO_WIN32_LEAN_AND_MEAN
#error 'Conditional Defines' in 'Directories/Conditionals' under Project/Options must include NO_WIN32_LEAN_AND_MEAN and _WINSOCKAPI_.
#endif
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ImgList.hpp>
#include <StdCtrls.hpp>
#include <IdTCPClient.hpp>
#include <map>
//---------------------------------------------------------------------------
#include "MM_RemoteBrowse.h"
//---------------------------------------------------------------------------
namespace RemoteBrowser
{
//---------------------------------------------------------------------------
class TRemoteBrowseForm : public TForm
{
__published:	// IDE-managed Components
	TTreeView *TreeView1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TImageList *imlTreeImages;
	TImageList *imlTreeStateImages;
	TLabel *Label1;
	void __fastcall TreeView1Deletion(TObject *Sender, TTreeNode *Node);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall TreeView1DblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TRemoteBrowseForm();
	TTreeNode *PopulateNode(TNodeSettings *NodeSettings, TTreeNode *ParentNode);

	AnsiString Path;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
enum TBrowseType { btFiles, btFolders };
//---------------------------------------------------------------------------
class TRBClientInterface
{
public:
	TRBClientInterface(TBrowseType BrowseType=btFiles) : fRBClient(NULL), LargeIcons(NULL), SmallIcons(NULL), fIconSize(isLarge), fBrowseType(BrowseType) {}
	virtual ~TRBClientInterface() {};

	virtual void			Loading() {};		// Use to show hourglass etc.
	virtual void			Loaded() {};		// Hide hour glass
	virtual void			Initialise()=0;	// Clear display
	virtual void			LoadDrives()=0;
	virtual void			SetCaption(const AnsiString &Caption)=0;

	virtual bool			Show()=0;
	virtual AnsiString	Path() { return fPath; }
	void						Initialise(AnsiString Host, AnsiString Path) { fHostName = Host; fPath = Path; }

protected:
	friend class			TRBClient;
	class TRBClient		*fRBClient;

//	typedef std::map<AnsiString, int> TFileImageIndexes;
	typedef std::map<int, int> TFileImageIndexes; // Maps IP to

	TImageList				*LargeIcons;
	TImageList				*SmallIcons;
	TIconSize				fIconSize;
	void						InitialiseIcons(TIconSize IconSize);
	bool						IconLoaded(TIconSize IconSize, int Index) const;
	int						AddIcon(TIconSize IconSize, int RemoteIndex, TIcon *Icon);
	void						LoadIcons(const TRBNodes &Nodes, TIconSize IconSize);
	int						GetLocalIconIndex(TIconSize IconSize, int RemoteIndex);

	TFileImageIndexes		LargeFileImageIndexes;
	TFileImageIndexes		SmallFileImageIndexes;
//	void						GetIcon(const TNodeSettings *NodeSettings, int RemoteIndex, TIcon *Icon, int &LocalIndex);

	AnsiString				fPath;
	AnsiString				fHostName;

	TBrowseType				fBrowseType;

	void						Expand(TNodeSettings *NodeSettings, TRBNodes &Nodes);

	int						DefaultFixedDriveIconIndex;
	int						DefaultFolderOpenIconIndex;
	int						DefaultFolderClosedIconIndex;

	int						MyComputerIconIndex;

	int						FixedDriveIconIndex;
	int						FloppyDriveIconIndex;
	int						RemovableDriveIconIndex;
	int						RemoteDriveIconIndex;
	int						CDROMDriveIconIndex;
	int						RAMDriveIconIndex;

	int						FolderOpenIconIndex;
	int						FolderClosedIconIndex;
};
//---------------------------------------------------------------------------
class TRBClient
{
public:
	TRBClient(class TRBClientInterface *RBClientInterface, const AnsiString &Caption, const AnsiString &Host, int Port, TBrowseFilter BrowseFilter = bfAll, const AnsiString &ExtFilter = "");
	~TRBClient();

	bool				Execute();
	AnsiString		Path;

private:
	TRBClientInterface *fRBClientInterface;
	std::auto_ptr<TIdTCPClient> fTCPClient;

	friend void		TRBClientInterface::Expand(TNodeSettings *NodeSettings, TRBNodes &Nodes);
	void				Expand(TNodeSettings *NodeSettings, TRBNodes &Nodes);
	friend void		TRBClientInterface::LoadIcons(const TRBNodes &Nodes, TIconSize IconSize);
	void				LoadIcons(const TRBNodes &Nodes, TIconSize IconSize);
	void				ResetServer();

	void				SendTCPMessage(TSerialData &Data);
	bool				ReceiveTCPMessage(TSerialData &Data);

	AnsiString		fExtFilter;

	TBrowseFilter	fBrowseFilter;
};
//---------------------------------------------------------------------------
/*class TRBClientGUI : public TRBClientInterface
{
public:
	TRBClientGUI();

	virtual void		Loading();
	virtual void		Loaded();
	virtual void		Initialise();
	virtual void		LoadDrives();

	virtual bool		Show();

private:
	const std::auto_ptr<TRemoteBrowseForm> Form;

	void __fastcall	fTreeExpanding(TObject *Sender, TTreeNode *Node, bool &AllowExpansion);
	bool					fInitialisingPath;
};*/
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
