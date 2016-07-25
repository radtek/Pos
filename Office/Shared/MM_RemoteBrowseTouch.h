//---------------------------------------------------------------------------

#ifndef MM_RemoteBrowseTouchH
#define MM_RemoteBrowseTouchH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
#include <Set>
#include <List>
#include "MM_RemoteBrowseClient.h"
#include "ResizeWindow.h"
//---------------------------------------------------------------------------
namespace RemoteBrowser
{
//---------------------------------------------------------------------------
class TRBTreeNode
{
private:
	typedef std::vector<TRBTreeNode *> TRBTreeNodes;

public:
//	TRBTreeNode() : Parent(NULL), LastVisible(NULL) {}
	TRBTreeNode(TRBTreeNode *iParent, TNodeSettings *iSettings) : Parent(iParent), NodeSettings(iSettings), LastVisible(NULL) {}

	virtual ~TRBTreeNode()	{ DeleteChildren(); }
	void DeleteChildren()	{ for (unsigned int i=0; i<Nodes.size(); i++) delete Nodes[i]; Nodes.clear();	}

	TNodeSettings				*NodeSettings;
	TRBTreeNode					*Parent;
	TRBTreeNode					*LastVisible;

	TRBTreeNodes				Nodes;
};
//---------------------------------------------------------------------------
class TRBClientTouchGUI : public TRBClientInterface
{
public:
	TRBClientTouchGUI(TBrowseType BrowseType=btFiles);

	virtual void		Loading();
	virtual void		Loaded();
	virtual void		Initialise();
	virtual void		LoadDrives();
	virtual void		SetCaption(const AnsiString &Caption);

	virtual bool		Show();
private:
	const std::auto_ptr<class TRemoteBrowseTouchForm> Form;

	bool					fInitialisingPath;

	friend class		TRemoteBrowseTouchForm;
	void					fExpand(TRBTreeNode *RBTreeNode);
	TRBTreeNode			*fCurrentLevel;
	void					fUpdateGUI();

	TRBTreeNode			Tree;
};
//---------------------------------------------------------------------------
class TRemoteBrowseTouchForm : public TForm
{
	friend TRBClientTouchGUI;
__published:	// IDE-managed Components
	TImageList *TreeImages;
	TPanel *Panel1;
	TListBox *FolderList;
	TListBox *FilesList;
	TTouchBtn *TouchBtn1;
	TTouchBtn *TouchBtn2;
	TTouchBtn *TouchBtn4;
	TTouchBtn *TouchBtn5;
	TTouchBtn *TouchBtn6;
	TTouchBtn *TouchBtn7;
	TLabel *HostLabel;
	TTouchBtn *TouchBtn3;
	TTouchBtn *TouchBtn8;
	TPanel *CaptionPanel;
	TPanel *Panel3;
	TLabel *StatusCaption;
	void __fastcall FolderListClick(TObject *Sender);
	void __fastcall TouchBtn1MouseClick(TObject *Sender);
	void __fastcall FolderListDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
	void __fastcall TouchBtn2MouseClick(TObject *Sender);
	void __fastcall FilesListClick(TObject *Sender);
	void __fastcall TouchBtn6MouseClick(TObject *Sender);
	void __fastcall TouchBtn7MouseClick(TObject *Sender);
	void __fastcall TouchBtn5MouseClick(TObject *Sender);
	void __fastcall TouchBtn4MouseClick(TObject *Sender);
	void __fastcall TouchBtn8MouseClick(TObject *Sender);
	void __fastcall TouchBtn3MouseClick(TObject *Sender);

private:	// User declarations
	TRBClientTouchGUI	&fRBClientTouchGUI;

	void __fastcall	WMSize(TWMSize& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_SIZE, TWMSize, WMSize)
	END_MESSAGE_MAP(TComponent)

	TResizeWindow		fResizeWindow;

public:		// User declarations
	__fastcall			TRemoteBrowseTouchForm(TRBClientTouchGUI &RBClientTouchGUI);

	AnsiString			Path;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
