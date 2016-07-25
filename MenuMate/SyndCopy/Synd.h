//---------------------------------------------------------------------------

#ifndef SyndH
#define SyndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MM_DBCore.h"
#include "ManagerSyndCode.h"
#include "IBSQL.hpp"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TEdit *Edit1;
	TComboBox *ComboBox1;
	TEdit *Edit2;
	TButton *Button3;
	TButton *btnDisplay;
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall btnDisplayClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	Database::TDBControl IBDatabase;
	TManagerSyndCode ManagerSyndicateCode;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
