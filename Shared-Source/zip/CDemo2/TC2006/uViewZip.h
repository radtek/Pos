#ifndef uViewZipH
#define uViewZipH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "ZipBuilder.h"

//---------------------------------------------------------------------------
class TfViewZip : public TForm {
	__published:	// IDE-managed Components
		TFileListBox		*FileListBox1;
		TDirectoryListBox	*DirectoryListBox1;
		TDriveComboBox		*DriveComboBox1;
		TEdit					*Edit1;
		TGroupBox			*GroupBox1;
		TMemo					*Memo;
		TButton				*btnTilfoj;
		TButton				*btnOpdater;
		TButton				*btnSlet;
		TButton				*btnAfslut;
		TImage				*Image1;
		TLabel				*Label1;
		void __fastcall FileListBox1DblClick( TObject *Sender );
		void __fastcall btnAfslutClick( TObject *Sender );
		void __fastcall btnTilfojClick( TObject *Sender );
		void __fastcall btnOpdaterClick( TObject *Sender );
		void __fastcall btnSletClick( TObject *Sender );
		void __fastcall FileListBox1Change( TObject *Sender );
		void __fastcall ZipBuilder1Message( TObject *Sender, int ErrCode, String Message );

	public:		// User declarations
	 	TZipBuilder			*ZipBuilder1;
		__fastcall TfViewZip( TComponent *Owner );
    __fastcall ~TfViewZip(void);
  private:
    char   *TEMPDir;
    char    TempDizPathSz[255];
    AnsiString  TempDizPath;
};

//---------------------------------------------------------------------------
extern PACKAGE TfViewZip *fViewZip;
#endif

