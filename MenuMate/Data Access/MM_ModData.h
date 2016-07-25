//---------------------------------------------------------------------------

#ifndef MM_ModDataH
#define MM_ModDataH
//---------------------------------------------------------------------------
#include "ZipBuilder.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <DBXpress.hpp>
#include <FMTBcd.hpp>
#include <SqlExpr.hpp>
#include <IBServices.hpp>
#include <DBXInterbase.hpp>
#include <WideStrings.hpp>

//---------------------------------------------------------------------------
class TmodData : public TDataModule
{
__published:	// IDE-managed Components
	TSQLConnection *SQLConnection;
	TSQLQuery *SQLQueryA;
	TSQLQuery *SQLQueryB;
	TSQLQuery *SQLQueryC;
	TSQLConnection *SQLConnection1;
	TSQLQuery *SQLQuery1;
	TSQLQuery *SQLQuery2;
	TSQLQuery *SQLQuery3;
	TIBRestoreService *IBRestore;
	TIBBackupService *IBCopy;
	TSQLQuery *SQLQuery4;
	TZipBuilder *Zip;
private:
	void __fastcall SetEnabled(bool value);
	bool __fastcall GetEnabled();	// User declarations
public:		// User declarations
	__fastcall TmodData(TComponent* Owner);
	bool OpenPrimaryDB();
	void ClosePrimaryDB();
	bool OpenSecondaryDB();
	void CloseSecondaryDB();
   void MakeDBImage(AnsiString BackupPath,TLabel *Display);
   void FormatDB(TDateTime StartTime, TDateTime EndTime);
	AnsiString CompressDB(AnsiString BackupPath,AnsiString PickUpPath);
//   void CopyTable(AnsiString TableName,TSQLQuery *Query);

	UnicodeString InterbaseIP;
	UnicodeString DatabasePath;
	UnicodeString InterbaseIP1;
   UnicodeString DatabasePath1;

};
//---------------------------------------------------------------------------
extern PACKAGE TmodData *modData;
//---------------------------------------------------------------------------
#endif
