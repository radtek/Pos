//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MM_DBCore.h"
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchPages.h"

//---------------------------------------------------------------------------
enum TMessageType { msError, msWarning, msComplete };
//---------------------------------------------------------------------------
class TIBTools : public TForm
{
__published:	// IDE-managed Components
	TPanel *BackupPanel;
	TPanel *BackupCaption;
	TPanel *BackupSettingsPanel;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label6;
	TTouchBtn *chkIgnoreChecksums;
	TTouchBtn *chkIgnoreLimbo;
	TTouchBtn *chkNonTransportable;
	TTouchBtn *chkMetadataOnly;
	TTouchBtn *chkNoGarbageCollection;
	TTouchBtn *SweepButton;
	TTouchBtn *RepairButton;
	TTouchBtn *BackupButton;
	TPanel *RepairPanel;
	TTouchBtn *TouchBtn2;
	TPanel *RepairCaption;
	TTouchBtn *CheckButton;
	TMemo *RepairMemo;
	TPanel *RestorePanel;
	TTouchBtn *TouchBtn1;
	TPanel *RestoreCaption;
	TPanel *RestoreSettingsPanel;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label12;
	TLabel *Label9;
	TLabel *Label10;
	TTouchBtn *chkDeactivateIndexes;
	TTouchBtn *chkNoValidityCheck;
	TTouchBtn *chkOneRelationAtATime;
	TTouchBtn *chkUseAllSpace;
	TTouchBtn *chkNoShadow;
	TTouchBtn *RestoreIBButton;
	TTouchBtn *TouchBtn3;
	TLabel *Label5;
	TLabel *lbeVersion;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BackupButtonMouseClick(TObject *Sender);
	void __fastcall SweepButtonMouseClick(TObject *Sender);
	void __fastcall RepairButtonMouseClick(TObject *Sender);
	void __fastcall CheckButtonMouseClick(TObject *Sender);
	void __fastcall DoRepairButtonMouseClick(TObject *Sender);
	void __fastcall CancelButtonMouseClick(TObject *Sender);
	void __fastcall RestoreIBButtonMouseClick(TObject *Sender);
private:	// User declarations
	Database::TDBControl		FDBControl;
	Database::TDBUtilities	FDBUtilities;

	UnicodeString		ComputerName;

	UnicodeString		RepairFileName;
	UnicodeString		RepairBackupFileName;
	UnicodeString		RepairRestoreFileName;

	void __fastcall ServiceComplete(bool Success, UnicodeString ErrorMsg, Database::TIBServiceType ServiceType);
	void __fastcall ServiceStatus(UnicodeString Msg, Database::TIBServiceType ServiceType);

	void EnableControls(bool Enable);
	/*bool ZipFile(AnsiString FileName, AnsiString ArchiveName);*/

	void Message(const UnicodeString &Msg, TMessageType MessageType);

	void Execute();
public:		// User declarations
	__fastcall TIBTools(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TIBTools *IBTools;
//---------------------------------------------------------------------------
#endif
