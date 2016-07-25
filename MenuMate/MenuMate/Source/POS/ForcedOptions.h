//---------------------------------------------------------------------------

#ifndef ForcedOptionsH
#define ForcedOptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "DeviceRealTerminal.h"

#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"

#include <item_management_network_messaging_fwd.hh>

//---------------------------------------------------------------------------
class TOptionBtn : public TTouchBtn//TCustomStaticText
{
private:
public:
	__fastcall TOptionBtn(Classes::TComponent* AOwner);
   TItemOption *ptrOption;
};

class TfrmSelectForcedOptions : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TPanel *pnlCondiments;
   TScrollBox *sbOptions;
   TPanel *pnlSide;
   TTouchBtn *btnMsgUp;
   TTouchBtn *btnMsgDown;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnCloseClick(TObject *Sender);
	void __fastcall BtnCancelClick(TObject *Sender);   
	void __fastcall btnMsgDownClick(TObject *Sender);
	void __fastcall btnMsgUpClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall pnlCoursesResize(TObject *Sender);
   void __fastcall BtnOptionClick(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
     MESSAGE_HANDLER(item_management::network_messaging::refresh_item_display,
                     Messages::TMessage,
                     ForwardItemDefinitionRefresh)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	__fastcall TfrmSelectForcedOptions(TComponent* Owner);

   void ForwardItemDefinitionRefresh(Messages::TMessage &message) const;
public:		// User declarations

   TListOptionContainer *ForcedOptionsList;
   TListOptionContainer *SelectedOptions;
   TOptionBtn *NextButton;
   TOptionBtn *CloseButton;
};
//---------------------------------------------------------------------------
#endif
