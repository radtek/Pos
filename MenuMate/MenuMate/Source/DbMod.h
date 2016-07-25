//---------------------------------------------------------------------------
#ifndef DbModH
#define DbModH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
//#include "VersInfo.hpp"
#include "ZForm.h"
#include <jpeg.hpp>
#include "pngimage.hpp"

//---------------------------------------------------------------------------
class TfrmDBMod : public TZForm
{                                                           
	friend TZForm;
__published:	// IDE-managed Components
	TLabel *Label1;
   TLabel *lbRegInfo;
   TLabel *lbeVersion;
   TImage *imgMenuMate;void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall imgMenuMateDblClick(TObject *Sender);

//	void __fastcall CreateParams( Controls::TCreateParams &Params );

private:	// User declarations
//   DBIResult Chk(DBIResult ErrorValue);
	__fastcall TfrmDBMod(TComponent* Owner);
	bool CancelRebuild;
	bool CanCancel;

public:		// User declarations

/*   bool OpenDB();
   void CloseDB();*/
   void Init();

   bool DoRebuild;
   bool VerifyArchive;
	void SetCaption(UnicodeString Caption);
   void SetRegCaption(UnicodeString Caption);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDBMod *frmDBMod;
//---------------------------------------------------------------------------
/*class TDBTableMod
{
	private:
		DBIResult fDbiDoRestructure(hDBIDb hTmpDb, char *TblName, FLDDesc FldDesc, char *NewTblName);
		DBIResult Chk(DBIResult ErrorValue);
   public:
      bool AddTable(TTable *ptrTable);
		bool AddField(TTable *ptrTable,TDatabase *Database,UnicodeString Name,TFieldType inFieldType,int Size = 0,bool Required=false);
      bool RebuildDB(TDatabase *Database);
		void OpenDB();
		void SetWriteAccess();
		void SetReadAccess();
      void CloseDB();
};     */
//---------------------------------------------------------------------------
#endif
 