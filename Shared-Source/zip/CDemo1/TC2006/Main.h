#ifndef mainH
#define mainH
#include <Classes.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include <shlobj.h>

#include "Message.h"
#include "ExtractFiles.h"
#include "AddFiles.h"
#include "SFX.h"
#include "Rename.h"
#include "ZipBuilder.h"


class TMainForm : public TForm
{
	__published:	// IDE-managed Components
		TBevel			*Bevel2;
		TBevel			*Bevel1;
		TButton			*CloseBut;
		TButton			*DeleteZipBut;
		TButton			*NewZipBut;
		TButton			*ZipOpenBut;
		TButton			*MsgBut;
		TButton			*ConvertBut;
		TButton			*DeleteBut;
		TButton			*AddBut;
		TButton			*ExtractBut;
		TButton			*TestBut;
		TCheckBox		*VerboseCB;
		TCheckBox		*TraceCB;
		TCheckBox		*UnattendedCB;
		TLabel			*FilesLabel;
		TLabel			*ZipFName;
		TLabel			*Label1;
		TLabel			*Label2;
		TLabel			*TimeLabel;
		TLabel			*Label4;
		TPanel			*Panel2;
		TPanel			*Panel4;
		TPanel			*Panel3;
		TPanel			*Panel1;
		TOpenDialog		*OpenDialog1;
		TMainMenu		*MainMenu1;
		TMenuItem		*Project1;
		TMenuItem		*Showlasterror1;
		TMenuItem		*File1;
		TMenuItem		*Exit1;
		TMenuItem		*Zipcomment1;
		TMenuItem		*DLLversioninfo1;
		TButton			*RenBut;
  TStringGrid *StringGrid1;

		void __fastcall CloseButClick( TObject *Sender );
		void __fastcall ZipOpenButClick( TObject *Sender );
		void __fastcall NewZipButClick( TObject *Sender );
		void __fastcall DeleteZipButClick( TObject *Sender );
		void __fastcall ExtractButClick( TObject *Sender );
		void __fastcall AddButClick( TObject *Sender );
		void __fastcall ZipBuilder1DirUpdate( TObject *Sender );
		void __fastcall ZipBuilder1Message( TObject *Sender, int ErrCode, String Message );
		void __fastcall DeleteButClick( TObject *Sender );
		void __fastcall TestButClick( TObject *Sender );
		void __fastcall MsgButClick( TObject *Sender );
		void __fastcall ConvertButClick(TObject *Sender );
		void __fastcall Showlasterror1Click( TObject *Sender );
		void __fastcall Exit1Click( TObject *Sender );
		void __fastcall UnattendedCBClick( TObject *Sender );
		void __fastcall Zipcomment1Click( TObject *Sender );
		void __fastcall DLLversioninfo1Click( TObject *Sender );
		void __fastcall FormResize( TObject *Sender );
		void __fastcall VerboseCBClick( TObject *Sender );
		void __fastcall TraceCBClick( TObject *Sender );
		void __fastcall RenButClick( TObject *Sender );
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
  void __fastcall StringGrid1SelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
  void __fastcall FormActivate(TObject *Sender);
  void __fastcall ZipBuilder1ProgressDetails(TObject *Sender,
          TProgressDetails *Details);
private: //user declarations
	void __fastcall ClearGrid(void);
	void __fastcall SortGrid(void);
public:		// User declarations
		bool AllFiles;
		bool ExpandDirs;
		bool OverWr;
		bool Canceled;
		bool DoIt;
		long s, f;
		AnsiString ExtractDir;
		unsigned __int64	TotUncomp, TotComp;
    TZipBuilder* ZipBuilder1;
		__fastcall TMainForm(TComponent *Owner);

		String ShowLTime( long s, long f );
		void __fastcall SetZipFName( String aCaption, bool AssignName );
		void __fastcall FillGrid( void );
		bool __fastcall AskDirDialog( const HWND FormHandle, String &DirPath );
		long __fastcall GetSpecialFolder( int aFolder, String &Location );
		void __fastcall SetZipTotals( void );
};

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TMainForm *MainForm;



#endif

