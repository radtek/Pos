//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include "touchbtn.h"
#include "touchcontrols.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
   TOpenDialog *dlgOpenGDB;
   TMemo *memLogs;
   TOpenDialog *dlgOpenXML;
   TTouchBtn *tbtnCheck;
   TTouchBtn *tbtnExtract;
   TTouchBtn *TouchBtn1;
   TTouchBtn *tbtnClear;
   TPanel *pnl;
   TCheckBox *cbCompareUniqueIndex;
   void __fastcall btnExtractClick(TObject *Sender);
   void __fastcall btnCheckClick(TObject *Sender);
   void __fastcall TouchBtn1MouseClick(TObject *Sender);
   void __fastcall tbtnClearMouseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TfrmMain(TComponent* Owner);
   void __fastcall TfrmMain::Log(AnsiString LogInfo);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
