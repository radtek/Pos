//---------------------------------------------------------------------------
#ifndef MM_RemoteBrowserH
#define MM_RemoteBrowserH

//---------------------------------------------------------------------------
#ifndef NO_WIN32_LEAN_AND_MEAN
#error 'Conditional Defines' in 'Directories/Conditionals' under Project/Options must include NO_WIN32_LEAN_AND_MEAN and _WINSOCKAPI_.
#endif
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPServer.hpp>
#include <IdTCPClient.hpp>
#include <ImgList.hpp>
#include <vector>
#include <memory>
#include <set>
#include <map>
//---------------------------------------------------------------------------
#include "SerialData.h"
//---------------------------------------------------------------------------
namespace RemoteBrowser
{
//---------------------------------------------------------------------------
const int MSG_RBRWS_DRIVES		= 0x01000001;
const int MSG_RBRWS_FOLDERS	= 0x01000002;
const int MSG_RBRWS_FILES		= 0x01000003;
//---------------------------------------------------------------------------
enum TNodeType {

	ntUnknown			= 0x00,
	ntFloppyDrive		= 0x01,
	ntRemovableDrive	= 0x02,
	ntFixedDrive		= 0x04,
	ntRemoteDrive		= 0x08,
	ntCDROMDrive		= 0x10,
	ntRAMDrive			= 0x20,
	ntFolder				= 0x40,
	ntFile				= 0x80,

	ntAll					= 0xFF }; // Used only to specify to server to return all possible drives.
//---------------------------------------------------------------------------
class TNodeSettings
{
public:
	TNodeSettings() : Icon(NULL) {}
	TNodeSettings(const TNodeSettings &Rhs);
	~TNodeSettings() { delete Icon; }

	AnsiString							DisplayName;
	AnsiString							Path;
	bool									HasSubFolders;
	TNodeType							NodeType;
	AnsiString							FileExtension;

	TIcon *								Icon;

	TNodeSettings & operator = (const TNodeSettings &Rhs);

	void operator >> (TSerialData &SerialData) const;
	TNodeSettings& operator << (const TSerialData &SerialData);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
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
//	AnsiString &FPath;
public:		// User declarations
	__fastcall TRemoteBrowseForm();
	TTreeNode *PopulateNode(TNodeSettings *NodeSettings, TTreeNode *ParentNode);

	AnsiString Path;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TRBServer
{
public:
	TRBServer(int Port);
	~TRBServer();

private:
	std::auto_ptr<TIdTCPServer> fTCPServer;

	void __fastcall fTCPDisconnect(TIdPeerThread *AThread);
	void __fastcall fTCPExecute(TIdPeerThread *AThread);

	void SendTCPMessage(TIdTCPServerConnection *Connection, TSerialData &Data);
	bool ReceiveTCPMessage(TIdTCPServerConnection *Connection, TSerialData &Data);

	bool GetDrives(const TSerialData &InData, TSerialData &OutData);
	bool GetFolders(const TSerialData &InData, TSerialData &OutData);
	bool GetFiles(const TSerialData &InData, TSerialData &OutData);

	std::set<AnsiString>	SendFileImages;

	bool	SentFixedDriveIcon;
	bool	SentFloppyDriveIcon;
	bool	SentRemovableDriveIcon;
	bool	SentRemoteDriveIcon;
	bool	SentCDROMDriveIcon;
	bool	SentRAMDriveIcon;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TRBClient
{
public:
	TRBClient(const AnsiString &Host, int Port, TNodeType NodeTypes = ntAll, const AnsiString &ExtFilter = "");
	~TRBClient();

	bool Execute();

	AnsiString Path;

private:
	const std::auto_ptr<TRemoteBrowseForm> Form;
	std::auto_ptr<TIdTCPClient> fTCPClient;

	void GetDrives();

	void SendTCPMessage(TSerialData &Data);
	bool ReceiveTCPMessage(TSerialData &Data);

	void __fastcall TreeExpanding(TObject *Sender, TTreeNode *Node, bool &AllowExpansion);

	bool fExecuting;
	AnsiString fFilter;

	TNodeType fNodeTypes;

	int	DefaultFixedDriveIconIndex;
	int	DefaultFolderOpenIconIndex;
	int	DefaultFolderClosedIconIndex;
//	int	DefaultFileIconIndex;

	int	MyComputerIconIndex;

	int	FixedDriveIconIndex;
	int	FloppyDriveIconIndex;
	int	RemovableDriveIconIndex;
	int	RemoteDriveIconIndex;
	int	CDROMDriveIconIndex;
	int	RAMDriveIconIndex;

	int	FolderOpenIconIndex;
	int	FolderClosedIconIndex;

	typedef std::map<AnsiString, int> TFileImageIndexes;
	TFileImageIndexes FileImageIndexes;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
