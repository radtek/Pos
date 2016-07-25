//---------------------------------------------------------------------------
#ifndef BackupH
#define BackupH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include "ZipBuilder.h"
#include "ZipBase.h"
//---------------------------------------------------------------------------
class TfrmBackup : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlBackup;
	TPageControl *PageControl1;
	TTabSheet *tsDataBackup;
	TTabSheet *tsSystemBackup;
	TBitBtn *btnOk;
	TEdit *edDestPath;
	TLabel *Label6;
	TBitBtn *btnSelectDestPath;
	TSaveDialog *SaveDialog1;
	TZipBuilder *ZipBuilder1;
	TTabSheet *tsSetup;
	TEdit *Edit1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *edDataBackupPath;
	TBitBtn *btnDataPath;
	TLabel *Label4;
	TEdit *Edit2;
	TLabel *Label5;
	TEdit *Edit3;
	TLabel *Label7;
	TBitBtn *BitBtn2;
	TListBox *ListBox1;
	TBitBtn *btnAddSysFile;
	TBitBtn *btnRemoveSysFile;
	TLabel *Label8;
	TEdit *edSysDestPath;
	TBitBtn *btnSelectSysDestPath;
	TBitBtn *btnSystemBackup;
	TProgressBar *ProgressBar;
	TLabel *lbeProgress;
	TLabel *lbeProgressCap;
	TGroupBox *GroupBox1;
	TBitBtn *btnFullDataBackup;
	TBitBtn *btnMMDataBackup;
	TBitBtn *btnStockDataBackup;
	TGroupBox *GroupBox2;
	TLabel *Label9;
	TEdit *edPickupPath;
	TBitBtn *btnSelectSourcePath;
	TBitBtn *btnPickupCopy;
	TLabel *Label11;
	TLabel *Label12;
	TOpenDialog *OpenDialog1;
	TRadioButton *radPickup;
	TRadioButton *radBackup;
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnDataPathClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall PageControl1Changing(TObject *Sender, bool &AllowChange);
	void __fastcall btnSelectDestPathClick(TObject *Sender);
	void __fastcall btnFullDataBackupClick(TObject *Sender);
	void __fastcall btnStockDataBackupClick(TObject *Sender);
	void __fastcall btnMMDataBackupClick(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall btnAddSysFileClick(TObject *Sender);
	void __fastcall ZipBuilder1Message(TObject *Sender, int ErrCode,
          AnsiString Message);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall btnRemoveSysFileClick(TObject *Sender);
	void __fastcall btnSystemBackupClick(TObject *Sender);
	void __fastcall btnSelectSysDestPathClick(TObject *Sender);
	void __fastcall Label11Click(TObject *Sender);
	void __fastcall Label12Click(TObject *Sender);
	void __fastcall radPickupClick(TObject *Sender);
	void __fastcall btnSelectSourcePathClick(TObject *Sender);
	void __fastcall btnPickupCopyClick(TObject *Sender);
	void __fastcall ZipBuilder1Progress(TObject *Sender,
          ProgressType ProgrType, AnsiString Filename, __int64 FileSize);
private:	// User declarations
	TEdit *CurrentPathEdit;
   bool FailedBackup;
   bool ArchiveProcessCancelled;
   bool CopyProcessCancelled;
	bool ArchiveFile(AnsiString FileName, AnsiString ArchiveName);
	bool ArchiveFiles(TStrings *FileNames, AnsiString ArchiveName);
	void DisableWhileBackingUp(bool Disable);

	AnsiString StockBackupPath, MMBackupPath, StockZipSourcePath, MMZipSourcePath, ArchiveDestPath, SystemDestPath, PickupSourcePath;
	AnsiString StockBackupKey, MMBackupKey, BackupKey, SystemBackupFilesKey, SystemBackupKey;

	void GetFilesList(AnsiString Path, TStrings *Strings);
public:		// User declarations
	__fastcall TfrmBackup(TComponent* Owner);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern PACKAGE TfrmBackup *frmBackup;
//---------------------------------------------------------------------------
#endif
