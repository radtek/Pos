//---------------------------------------------------------------------------

#ifndef ForcedSidesH
#define ForcedSidesH
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
class TItemBtn : public TTouchBtn//TCustomStaticText
{
private:
public:
	__fastcall TItemBtn(Classes::TComponent* AOwner);
	TItemSide *ptrSide;
	TItemCompleteSub *ptrSubItem;
	bool Selected;
};

class TfrmSelectForcedSides : public TZForm
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
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
     MESSAGE_HANDLER(item_management::network_messaging::refresh_item_display,
                     Messages::TMessage,
                     ForwardItemDefinitionRefresh);
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
   int ButtonWidth;
	int ButtonHeight;

   void ForwardItemDefinitionRefresh(Messages::TMessage &message);

	__fastcall TfrmSelectForcedSides(TComponent* Owner,Database::TDBTransaction &inDBTransaction);
	Database::TDBTransaction &DBTransaction;
public:		// User declarations

   TListSideContainer *ForcedSidesList;
	TItemComplete *MasterItem;
   TItemBtn *NextButton;
	TItemBtn *CloseButton;   
};

#endif
