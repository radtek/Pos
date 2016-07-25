#ifndef RenameH
#define RenameH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>

#include "Main.h"
#include "Message.h"

//---------------------------------------------------------------------------
class TRenForm : public TForm {
	__published:	// IDE-managed Components
		TLabel		*Label1;
		TLabel		*Label2;
		TLabel		*Label3;
		TLabel		*Label4;
		TComboBox	*OldCombo;
		TEdit			*NewEdit;
		TEdit			*DTEdit;
		TPanel		*Panel1;
		TButton		*AddBtn;
		TButton		*RemoveBtn;
		TButton		*DTAllBtn;
		TBitBtn		*OkBitBtn;
		TBitBtn		*CancelBitBtn;
  TStringGrid *StringGrid1;

		void __fastcall FormShow( TObject *Sender);
		void __fastcall AddBtnClick( TObject *Sender);
		void __fastcall OkBitBtnClick( TObject *Sender);
		void __fastcall OldComboClick( TObject *Sender);
		void __fastcall DTAllBtnClick( TObject *Sender );
		void __fastcall RemoveBtnClick( TObject *Sender);
		void __fastcall CancelBitBtnClick( TObject *Sender);
  void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
  void __fastcall StringGrid1SelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);

   private:			// User declarations
		void __fastcall ClearZipRenList( void );

   public:			// User declarations
		TList         *ZipRenameList;
    unsigned long  GDateTime;

		__fastcall TRenForm( TComponent *Owner );
    __fastcall ~TRenForm(void);
};

//---------------------------------------------------------------------------
extern PACKAGE TRenForm *RenForm;
#endif
