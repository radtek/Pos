//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchButton.h"
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <IBDatabaseInfo.hpp>
#include <IBQuery.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TTouchButton *TouchButton1;
	TTouchButton *TouchButton2;
	TTouchButton *TouchButton3;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TTimer *Timer;
	void __fastcall TouchButton2Click(TObject *Sender);
	void __fastcall TouchButton1Click(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall TouchButton3Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
   AnsiString ImportFileName;
   TIBDatabase *IBDatabase;
	TIBTransaction *IBTransaction;
   TIBQuery *IBQuery;
   TIBQuery *IBQuery1;

   void CloseDB();
   void OpenDB();
	int GetNextSecurityRef();
	int ContactLookUp(int ContactID, AnsiString ContactName);
	void ProcessSecurity(int SecRef,int UserKey,
                        AnsiString Event,AnsiString FromVal,
                        AnsiString ToVal,TDateTime TimeStamp,
                        AnsiString TerminalName);
	void SetOrder(int TabKey,int ContactKey ,Currency InvoiceAmountInc,
								TDateTime TimeStamp,int SecurityRef);
   int UserKey;
	AnsiString ReadCSVWord(AnsiString Line, int& Offset, char Delimiter, char Qualifier, char Escape);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
