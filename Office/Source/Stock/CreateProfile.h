//---------------------------------------------------------------------------

#ifndef CreateProfileH
#define CreateProfileH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <ImgList.hpp>
#include <IBDatabase.hpp>
#include <Buttons.hpp>
#include <IBSQL.hpp>
//---------------------------------------------------------------------------
enum TProfileMode { pmAddProfile, pmEditProfile };
//---------------------------------------------------------------------------
class TfrmCreateProfile : public TForm
{
__published:	// IDE-managed Components
	TTreeView *tvStockGroups;
	TIBQuery *qrStockGroup;
	TDataSource *dsStockGroup;
	TIBQuery *qrStock;
	TIBTransaction *Transaction;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TEdit *edProfileName;
	TLabel *Label1;
	TIBSQL *sqlAddLK;
	TIBSQL *qrDeleteProfile;
	TIBDataSet *dtProfile;
	TDataSource *dsProfile;
	TIBDataSet *dtProfileGroups;
	TBitBtn *btnDelete;
	TBitBtn *btnSelectAll;
	TIBQuery *qrStockCategory;
	TDataSource *dsStockCategory;
	void __fastcall tvStockGroupsMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
	void __fastcall btnSelectAllClick(TObject *Sender);
	void __fastcall tvStockGroupsKeyPress(TObject *Sender, char &Key);
private:	// User declarations
	void LoadTree();
	int SaveProfile();
	bool Registered;
public:		// User declarations
	__fastcall TfrmCreateProfile(TComponent* Owner);
	TProfileMode ProfileMode;
	int StocktakeProfileKey;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCreateProfile *frmCreateProfile;
//---------------------------------------------------------------------------
#endif
