//---------------------------------------------------------------------------
#ifndef MM_RemoteBrowseServerH
#define MM_RemoteBrowseServerH
//---------------------------------------------------------------------------
#include <IdTCPServer.hpp>
#include <IdSync.hpp>
#include <Set>
#include <Map>
//---------------------------------------------------------------------------
#include "MM_RemoteBrowse.h"
//---------------------------------------------------------------------------
namespace RemoteBrowser
{
//---------------------------------------------------------------------------
class TRBNode
{
private:
	typedef std::vector<TRBNode *>	TRBNodes;
	TRBNodes									fNodes;
	TRBNode									*fParent;

	void fFree(LPMALLOC lpMalloc)
	{
		fFree(this, lpMalloc);

		for (TRBNodes::size_type i=0; i<fNodes.size(); i++)
		{
			fNodes[i]->fFree(lpMalloc);
		}
	}
	void fFree(TRBNode *Node, LPMALLOC lpMalloc)
	{
		if (lpidl)
		{
			lpMalloc->Free(lpidl);
			lpidl = NULL;
		}
	}
public:
	TRBNode() : lpidl(NULL) {}
	virtual ~TRBNode()
	{
		for (unsigned int i=0; i<fNodes.size(); i++) { delete fNodes[i]; fNodes.clear();	}
	}
	TRBNode *AddNode()
	{
		TRBNode *Node	= new TRBNode;
		Node->fParent	= this;
		fNodes.push_back(Node);
		return Node;
	}
	void Free()
	{
		LPMALLOC lpMalloc;
		if (SUCCEEDED(SHGetMalloc(&lpMalloc)))
		{
			fFree(lpMalloc);
			lpMalloc->Release();
		}
	}

	LPITEMIDLIST	lpidl;
	TRBNode			*operator [](TRBNodes::size_type Index) { return fNodes[Index]; }
};
//---------------------------------------------------------------------------
struct TRBRootNode
{
	TRBRootNode() : Node(new TRBNode) {}
	TRBNode *Node;
};
//---------------------------------------------------------------------------
class TRBServer
{
friend class TSyncTCPMessage;

	typedef void* TRemoteClient;
	typedef std::map<TRemoteClient, TRBRootNode> TClientTree;
	TClientTree ClientTree;
public:
	TRBServer(int Port);
	~TRBServer();

private:
	const std::auto_ptr<TImageList> LargeImages;
	const std::auto_ptr<TImageList> SmallImages;

	const std::auto_ptr<TIdTCPServer> fTCPServer;

	void __fastcall fTCPExecute(TIdPeerThread *AThread);

	bool Reset(const TSerialData &InData, TSerialData &OutData);
	bool GetDrives(const TSerialData &InData, TSerialData &OutData);
	bool GetFolders(const TSerialData &InData, TSerialData &OutData);
	bool GetIcons(const TSerialData &InData, TSerialData &OutData);

	void	fFillBranch(TRemoteClient RemoteClient, TBrowseFilter BrowseFilter, AnsiString FileFilter,
					LPSHELLFOLDER lpsf, LPITEMIDLIST lpidl,
					TIconSize IconSize, TRBNode *ParentNode, std::vector<TNodeSettings *> &Nodes);

	LPITEMIDLIST fGetNextItem(LPITEMIDLIST pidl);
	unsigned short fGetPIDLSize(LPITEMIDLIST lpidl);
	LPITEMIDLIST fMergeIDLists(LPMALLOC lpMalloc, LPITEMIDLIST lpidl1, LPITEMIDLIST lpidl2);

//	typedef std::vector<int> TImageIndexSet;
//	std::map<AnsiString, TImageIndexSet> SentFileImages;
//	TImageIndexSet SentFileImages;

//	TList *SentFileImages;
};
//---------------------------------------------------------------------------
class TSyncTCPMessage : public TIdSync
{
public:
	__fastcall TSyncTCPMessage(TIdPeerThread *AThread, TRBServer &RBServer);
	virtual __fastcall ~TSyncTCPMessage();

	virtual void __fastcall DoSynchronize(void);

protected:
	TRBServer				&fRBServer;

	TSerialData				fOutData;
	TSerialData				fInData;

	AnsiString				fPeerIP;
	TIdPeerThread			*fThread;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
