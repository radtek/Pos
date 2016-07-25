//---------------------------------------------------------------------------

#ifndef TransferItemsH
#define TransferItemsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmTransferItem : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TPanel *Panel5;
   TLabel *Label7;
   TLabel *Label8;
   TLabel *Label1;
   TStringGrid *StringGrid1;
   TPanel *Panel6;
   TPanel *Panel4;
   TPanel *Panel3;
   TPanel *Panel2;
   TPanel *stCancel;
   TPanel *stOk;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label6;
	TStringGrid *StringGrid2;
	TTouchBtn *btnDishUp;
	TTouchBtn *btnDishDown;
	TTouchBtn *btnDestDishUp;
	TTouchBtn *btnDestDishDown;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall stCancelClick(TObject *Sender);
	void __fastcall stOkClick(TObject *Sender);
	void __fastcall btnDishUpMouseDown(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, int X, int Y);
	void __fastcall btnDishUpMouseUp(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, int X, int Y);
	void __fastcall btnDishDownMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnDestDishUpMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnDestDishDownMouseDown(TObject *Sender,
			 TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall StringGrid2DrawCell(TObject *Sender, int ACol, int ARow,
			 TRect &Rect, TGridDrawState State);
	void __fastcall StringGrid1Click(TObject *Sender);
	void __fastcall StringGrid2Click(TObject *Sender);
private:	// User declarations
	__fastcall TfrmTransferItem(TComponent* Owner);

	void __fastcall ShowItems();
	TWinControl *ScrollingControl;
	TControl *ButtonClicked;
	bool ScrollUp;
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)
public:		// User declarations

   TList *SourceItems;
   TList *DestItems;
   AnsiString From;
   AnsiString To;
   int sourceItemCount;
   bool isItemTransferred;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmTransferItem *frmTransferItem;
//---------------------------------------------------------------------------
#endif
