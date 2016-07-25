//---------------------------------------------------------------------------

#ifndef GetBarcodeH
#define GetBarcodeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "Barcodereader.h"
//---------------------------------------------------------------------------
class TfrmGetBarcode : public TForm
{
__published:	// IDE-managed Components
	TTimer *tiSwipe;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TBitBtn *btnCancel;
	TLabel *lbeTitle;
	TBitBtn *btnManual;
	TEdit *edBarcode;
	TPanel *Panel5;
		TImage *Image1;
	TEdit *edDummy;
	void __fastcall btnManualClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall tiSwipeTimer(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
protected:
	void __fastcall WMCardSwipe(TMessage& Message);
	BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(UWM_CARDSWIPE, TMessage, WMCardSwipe)
	END_MESSAGE_MAP(TForm)
private:	// User declarations
	void __fastcall WMLoadMenu(TMessage& Message);
	bool Done;
	bool Swiping;
	AnsiString TempBarcode;
	TMMBarcodeReader *MMBarcodeReader;
public:		// User declarations
	__fastcall TfrmGetBarcode(TComponent* Owner);
	AnsiString Barcode;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmGetBarcode *frmGetBarcode;
//---------------------------------------------------------------------------
#endif
