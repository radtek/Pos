//---------------------------------------------------------------------------

#ifndef VirtualPrintersInterfaceH
#define VirtualPrintersInterfaceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
#include "VirtualPrinterManager.h"
#include "ZForm.h"

class TfrmVirtualPrintersInterface : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TPanel *pnl1;
   TLabel *Label17;
   TPanel *pnl2;
   TPanel *pnlAdd;
	TPanel *pnlEdit;
   TPanel *pnlDelete;
	TListBox *lbVirtualPrinters;
	TPanel *btnOk;
	TPanel *pnlReassign;
   void __fastcall pnlAddClick(TObject *Sender);
   void __fastcall pnlEditClick(TObject *Sender);
   void __fastcall pnlDeleteClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall pnlReassignClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmVirtualPrintersInterface(TComponent* Owner,Database::TDBControl &inDBControl);
	
	void RefreshList();
	bool GetNewPrinter(AnsiString Message,TPrinterVirtual &PrinterVirtual);
	Database::TDBControl &DBControl;
public:		// User declarations

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmVirtualPrintersInterface *frmVirtualPrintersInterface;
//---------------------------------------------------------------------------
#endif
