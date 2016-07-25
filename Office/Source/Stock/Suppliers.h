//---------------------------------------------------------------------------

#ifndef SuppliersH
#define SuppliersH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBSQL.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <memory>
#include "AddContact.h"
//---------------------------------------------------------------------------
enum TContactsMode	{ cmEdit, cmSelect };
//---------------------------------------------------------------------------
class TfrmSuppliers : public TForm
{
__published:	// IDE-managed Components
	TIBTransaction *Transaction;
	TIBSQL *sqlAddLK;
	TIBQuery *qrContact;
	TPopupMenu *pmTreeView;
	TMenuItem *miNew;
	TMenuItem *N1;
	TMenuItem *miEdit;
	TMenuItem *miRename;
	TMenuItem *miDelete;
	TFindDialog *FindDialog;
	TIBSQL *qrUpdateContact;
	TIBSQL *qrFindContact;
	TPanel *Panel1;
	TPanel *Panel4;
	TTreeView *tvContacts;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TPageControl *pcContactsDetails;
	TTabSheet *tsContacts;
	TPanel *Panel7;
	TButton *btnEditStock;
	TButton *btnDeleteStock;
	TButton *btnAddStock;
	TButton *btnRenameStock;
	TListBox *lbContacts;
	TTabSheet *tsContact;
	TLabel *Label16;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label21;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *lbePhone;
	TLabel *lbeMobile;
	TLabel *lbeFax;
	TLabel *lbeEmail;
	TLabel *lbeContactName;
	TMemo *memLocationAddress;
	TMemo *memBillingAddress;
	TMemo *memNote;
	TPanel *Panel10;
	TBitBtn *btnClose;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TBitBtn *btnFind;
	TSplitter *Splitter1;
	TButton *btnAdd;
	TBitBtn *btnEdit;
	TBitBtn *btnDelete;
	TBitBtn *btnImport;
	TOpenDialog *odImport;
	TIBSQL *qrAddContact;
	TIBQuery *qrContactGroup;
	TIBQuery *qrAddContactGroup;
	TIBSQL *IBSQL1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall edFindChange(TObject *Sender);
	void __fastcall rbSearchClick(TObject *Sender);
	void __fastcall edFindKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall edFindKeyPress(TObject *Sender, char &Key);
	void __fastcall tvContactsChange(TObject *Sender, TTreeNode *Node);
	void __fastcall tvContactsMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall tvContactsEdited(TObject *Sender, TTreeNode *Node,
          AnsiString &S);
	void __fastcall tvContactsDblClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall miNewClick(TObject *Sender);
	void __fastcall miEditClick(TObject *Sender);
	void __fastcall miRenameClick(TObject *Sender);
	void __fastcall miDeleteClick(TObject *Sender);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
	void __fastcall btnAddStockClick(TObject *Sender);
	void __fastcall btnEditStockClick(TObject *Sender);
	void __fastcall btnRenameStockClick(TObject *Sender);
	void __fastcall btnDeleteStockClick(TObject *Sender);
	void __fastcall lbContactsDblClick(TObject *Sender);
	void __fastcall lbContactsDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
	void __fastcall tvContactsKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall tvContactsKeyPress(TObject *Sender, char &Key);
	void __fastcall pmTreeViewPopup(TObject *Sender);
	void __fastcall btnImportClick(TObject *Sender);
	void __fastcall Splitter1CanResize(TObject *Sender, int &NewSize,
          bool &Accept);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void ResizeGrids();
	void LoadTree();
	int ContactTreeLevel;
	TTreeNode *FindContact(AnsiString Text);

	bool Registered;
	unsigned int Modules;

	const std::auto_ptr<TfrmAddContact>frmAddContact;
public:		// User declarations
	__fastcall TfrmSuppliers(TComponent* Owner);
	TContactsMode ContactsMode;
	int ContactKey;
	AnsiString CompanyName;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSuppliers *frmSuppliers;
//---------------------------------------------------------------------------
#endif
