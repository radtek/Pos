//---------------------------------------------------------------------------

#ifndef ExportFormH
#define ExportFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "touchbtn.h"
#include "touchcontrols.h"
//---------------------------------------------------------------------------
class TfrmExport : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TTouchBtn *btnExit;
	TTouchBtn *btnEditMenus;
	TTouchBtn *TouchBtn1;
	TImage *Image1;
	TLabel *Label1;
	TLabel *Label2;
	void __fastcall btnEditMenusMouseClick(TObject *Sender);
	void __fastcall btnExitMouseClick(TObject *Sender);
	void __fastcall TouchBtn1MouseClick(TObject *Sender);
private:	// User declarations
	bool Registered;
	unsigned int Modules;
public:		// User declarations
	__fastcall TfrmExport();
};
//---------------------------------------------------------------------------
#endif
