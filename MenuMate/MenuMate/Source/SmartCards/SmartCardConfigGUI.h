//---------------------------------------------------------------------------

#ifndef SmartCardConfigGUIH
#define SmartCardConfigGUIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmSmartCardConfiguration : public TZForm
{
__published:	// IDE-managed Components
   TPanel *Panel24;
   TLabel *Label4;
   TPanel *Panel2;
   TTouchBtn *tbVerifactionCode;
   TTouchBtn *TouchBtn2;
   TTouchBtn *tbtnFormatCard;
   TLabel *Label2;
   TLabel *Label1;
   TTouchBtn *tbtnRestoreCard;
   TLabel *Label3;
   TTouchBtn *tbtnExploreCard;
   TTouchBtn *tbtnReassignCard;
   TLabel *Label5;
	TLabel *lblPreloadCard;
	TTouchBtn *tbtnPreloadCard;
   void __fastcall tbSetVerifactionCode(TObject *Sender);
   void __fastcall TouchBtn2MouseClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall tbtnFormatCardMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tbtnRestoreCardMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tbtnExploreCardMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall tbtnReassignCardMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall preload_card_clicked(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TfrmSmartCardConfiguration(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
