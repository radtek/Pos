//---------------------------------------------------------------------------

#ifndef UtilitiesH
#define UtilitiesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <SysUtils.hpp>
#include "VirtualTrees.hpp"
//---------------------------------------------------------------------------
#define ICON_BOX_INDEX								0
#define ICON_NOTEPAD_INDEX							1
#define ICON_PIE_INDEX								2
#define ICON_MONEY_INDEX							3
#define ICON_EXCLAMATION_INDEX					4
#define ICON_EXCLAMATION_RED_INDEX				5
#define ICON_CROSS_RED_INDEX						6
#define ICON_CLOSED_FOLDER_INDEX					7
#define ICON_OPEN_FOLDER_INDEX					8
#define ICON_FAVORITES_FOLDER_INDEX				9
#define ICON_RECYCLE_FOLDER_INDEX				10

#define STATE_IMAGE_CROSS							1

#define STATE_IMAGE_UNCHECKED						2
#define STATE_IMAGE_CHECKED						3
#define STATE_IMAGE_GRAY_UNCHECKED				4
#define STATE_IMAGE_GRAY_CHECKED					5

#define STATE_IMAGE_SMALL_UNCHECKED				6
#define STATE_IMAGE_SMALL_CHECKED				7
#define STATE_IMAGE_SMALL_GRAY_UNCHECKED		8
#define STATE_IMAGE_SMALL_GRAY_CHECKED			9

#define MAX_MENU_ITEMS_IN_EVALUATION_MODE		20
//---------------------------------------------------------------------------
class TdmUtilities : public TDataModule
{
__published:	// IDE-managed Components
	TImageList *imlTreeImages;
	TImageList *imlTreeStateImages;
private:	// User declarations
public:		// User declarations
	__fastcall TdmUtilities(TComponent* Owner);

	AnsiString GetComputerIP(AnsiString ComputerName);
	AnsiString GetPCName();
	bool StrToBoolean(AnsiString s);
	void DeleteWildCardFile(AnsiString FileName);
	AnsiString FindWildCardFile(AnsiString FileName);
};
//---------------------------------------------------------------------------
class TRetrieveNamesThread : public TThread
{
private:
	void __fastcall EnumNetRes(NETRESOURCE* nr, TStrings *ComputerNames);
	void __fastcall AddName();
	AnsiString ComputerName;
protected:
	void __fastcall Execute();
public:
	__fastcall TRetrieveNamesThread(bool CreateSuspended);
	TStrings *ComputerNames;
};
//---------------------------------------------------------------------------
class TPropertyEdit : public IVTEditLink
{
	ULONG						FRefCount;			//  used by our own implementation of the interface
	
	bool						FStopping;
	TBaseVirtualTree		*FTree;
	TVirtualNode			*FNode;
	int						FColumn;
	TWinControl				*FEditControl;
protected:
public:
	// Constructor/Destructor
	__fastcall				TPropertyEdit(TBaseVirtualTree* Tree, TVirtualNode* Node, int Column, TWinControl *Control);
	virtual __fastcall	~TPropertyEdit(void);
	// IUnknown members
	virtual ULONG		__stdcall AddRef();
	virtual ULONG		__stdcall Release();
	virtual HResult	__stdcall QueryInterface(const GUID& IID, void** ppv);
	// IVTEditLink members
	virtual bool		__stdcall BeginEdit(void);
	virtual bool		__stdcall CancelEdit(void);
	virtual bool		__stdcall EndEdit(void);
	virtual bool		__stdcall PrepareEdit(TBaseVirtualTree* Tree, PVirtualNode Node, int Column);
	virtual TRect		__stdcall GetBounds(void);
	virtual void		__stdcall SetBounds(const TRect R);
	virtual void		__stdcall ProcessMessage(Messages::TMessage &Message);
};
//---------------------------------------------------------------------------
extern PACKAGE TdmUtilities *dmUtilities;
/*

TStringEditLink = class(TInterfacedObject, IVTEditLink)
  private
    FEdit: TVTEdit;                  // A normal custom edit control.
    FTree: TCustomVirtualStringTree; // A back reference to the tree calling.
    FNode: PVirtualNode;             // The node to be edited.
    FColumn: TColumnIndex;           // The column of the node.
    FAlignment: TAlignment;
    FTextBounds: TRect;              // Smallest rectangle around the text.
    FStopping: Boolean;              // Set to True when the edit link requests stopping the edit action.
    procedure SetEdit(const Value: TVTEdit);
  public
    constructor Create;
    destructor Destroy; override;

    function BeginEdit: Boolean; virtual; stdcall;
    function CancelEdit: Boolean; virtual; stdcall;
    property Edit: TVTEdit read FEdit write SetEdit;
    function EndEdit: Boolean; virtual; stdcall;
	 function GetBounds: TRect; virtual; stdcall;
    function PrepareEdit(Tree: TBaseVirtualTree; Node: PVirtualNode; Column: TColumnIndex): Boolean; virtual; stdcall;
    procedure ProcessMessage(var Message: TMessage); virtual; stdcall;
    procedure SetBounds(R: TRect); virtual; stdcall;
  end;
*/
//---------------------------------------------------------------------------
#endif
