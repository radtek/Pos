#ifndef MessageH
#define MessageH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "Main.h"

//---------------------------------------------------------------------------
class TMsgForm : public TForm {
	__published:	// IDE-managed Components
		TPanel		 *Panel2;
		TPanel		 *Panel1;
		TButton		 *DismissBut;
		TButton		 *CancelBut;
		TLabel		 *FileBeingZipped;
		TProgressBar *ProgressBar1;
		TRichEdit	 *RichEdit1;
		TStatusBar	 *StatusBar1;

		void __fastcall DismissButClick( TObject *Sender );
		void __fastcall CancelButClick( TObject *Sender );
		void __fastcall FormResize( TObject *Sender );

	public:			// User declarations
		TProgressBar *ProgressBar2;
		int			  FormInitialWidth;

		__fastcall TMsgForm( TComponent *Owner );
};

//---------------------------------------------------------------------------
extern PACKAGE TMsgForm *MsgForm;

#endif

