//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>

#include <DB.hpp>
#include <FMTBcd.hpp>
#include <SqlExpr.hpp>
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchBtn.h"
#include "TouchNumpad.h"
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TButton *btnQuit;
	TButton *Button4;
	TOpenDialog *Open;
	TGroupBox *gbSource;
	TLabel *lbSource;
	TButton *btnSourceBrowse;
	TLabel *lbProg;
	TButton *Button3;
	TProgressBar *ProgBar;
	TGroupBox *gbDest;
	TLabel *lbDest;
	TButton *btnDestBrowse;
   TEdit *edContactKey;
   TLabel *Label1;
   TEdit *edDateFilter;
   TLabel *Label2;
   TButton *Button1;
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall btnQuitClick(TObject *Sender);
	void __fastcall btnSourceBrowseClick(TObject *Sender);
	void __fastcall btnDestBrowseClick(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
   void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
	bool OpenSource();
	bool OpenDest();
	void MergeData();
	bool InvoiceExists(TSQLQuery *Invoice);
	int  InvoiceCopy(TSQLQuery *Invoice);
	int  CopyArcBill(TSQLQuery *Invoice);
	int  CopyArcBillPay(TSQLQuery *Invoice, int ArcBillKey);
	int  CopyArcSurcharge(TSQLQuery *Invoice, int ArcBillKey);
	void CopyArchive(TSQLQuery *Invoice,int ArcBillKey);
	void CopyArchiveCats(int NewArchiveKey,int OldArchiveKey);
   int CopySecurityRef(int SourceSecurityRef);
	int CopyThirdPartyCodes(int ThridPartyCodeKey);
	int CopyCategory(int CatKey);
   int CopyServingCourse(int ServingCourseKey);
   int CopyArcRef(TSQLQuery *Invoice, int ArcBillKey);      
	int GetOrCreateCatGroup(AnsiString Category);
	AnsiString GetSourceCatGroupForCategory(AnsiString Category);		
	int GetOrCreateCat(AnsiString Category);	
	AnsiString SourceVersion();
	AnsiString DestVersion();
	void GetSourceDataBaseVersion(int &DBMajorVersion,int &DBMinorVersion);
	void GetDestDataBaseVersion(int &DBMajorVersion,int &DBMinorVersion);
	AnsiString DBBool(AnsiString BoolVal);	
	AnsiString JoinMateVersion();
   int GetTotalInvoices(AnsiString Filter);
   int GetTotalInvoices();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
