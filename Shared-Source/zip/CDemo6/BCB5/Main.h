#ifndef mainH
#define mainH

//----------------------------------------------------------------------------
#include <vcl\ExtCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\Grids.hpp>
#include <vcl\Buttons.hpp>
#include <Menus.hpp>
#include <shlobj.h>
#include <ImgList.hpp>

#include "Message.h"
#include "ExtractFiles.h"
#include "AddFiles.h"
#include "ZipBuilder.h"

class TMainForm : public TForm {
	__published:	// IDE-managed Components
		TBevel			*Bevel1;
		TButton			*AddBut;
		TButton			*ExtractBut;
		TButton			*WriteBttn;
		TButton			*ReadBttn;
		TButton			*NewZipBut;
		TLabel			*ZipFName;
		TLabel			*Label1;
		TLabel			*Label2;
		TLabel			*Label4;
		TLabel			*Label5;
		TLabel			*Label6;
		TLabel			*FilesLabel;
		TPanel			*Panel2;
		TPanel			*Panel4;
		TPanel			*Panel3;
		TOpenDialog		*OpenDialog1;
		TEdit				*MaxVolSizeEdit;
		TEdit				*FreeDisk1Edit;
		TEdit				*MinFreeVolEdit;
		TCheckBox		*FormatCB;
		TMainMenu		*MainMenu1;
		TMenuItem		*Project1;
		TMenuItem		*Showlasterror1;
		TMenuItem		*File1;
		TMenuItem		*Exit1;
		TMenuItem		*DLLversioninfo1;
		TMenuItem		*Messages1;
  TStringGrid *StringGrid1;
  TZipBuilder *ZipBuilder1;

		void __fastcall NewZipButClick( TObject *Sender );
		void __fastcall ExtractButClick( TObject *Sender );
		void __fastcall AddButClick( TObject *Sender );
		void __fastcall ZipBuilder1DirUpdate( TObject *Sender );
		void __fastcall ZipBuilder1Message( TObject *Sender, int ErrCode, String Message );
//		void __fastcall ZipBuilder1Progress( TObject *Sender, ProgressType ProgrType, String Filename, __int64 FileSize );
		void __fastcall Showlasterror1Click( TObject *Sender );
		void __fastcall Exit1Click( TObject *Sender );
		void __fastcall WriteBttnClick( TObject *Sender );
		void __fastcall ReadBttnClick( TObject *Sender );
		void __fastcall DLLversioninfo1Click( TObject *Sender );
		void __fastcall FormResize( TObject *Sender );
		void __fastcall Messages1Click( TObject *Sender );
		void __fastcall MaxVolSizeEditChange( TObject *Sender );
		void __fastcall FreeDisk1EditChange( TObject *Sender );
		void __fastcall MinFreeVolEditChange( TObject *Sender );
    void __fastcall SortGrid(void);
    void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall StringGrid1SelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormActivate(TObject *Sender);
  void __fastcall ZipBuilder1ProgressDetails(TObject *Sender,
          TProgressDetails *Details);
	public:		// User declarations
		bool		DoIt;
		int		TotUncomp, TotComp;
		String	ZipOpenDir;
    String ExtractDir;
    bool ExpandDirs;
    bool OverWr;
    bool AllFiles;
    bool Canceled;
		__fastcall TMainForm(TComponent *Owner);

		void __fastcall SetZipFName( String aCaption );
		void __fastcall FillGrid( void );
		bool __fastcall AskDirDialog( const HWND FormHandle, String &DirPath );
		long __fastcall GetSpecialFolder( int aFolder, String &Location );
		void __fastcall SetZipTotals( void );
		bool __fastcall ZipOpenArchive( void );
		void __fastcall AddSpan( void );
};

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TMainForm *MainForm;

#endif

