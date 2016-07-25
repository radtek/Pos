//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <Menus.hpp>
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdTCPClient.hpp"
#include "IdTCPConnection.hpp"
#include <memory>

#include "DeviceKiosk.h"
#include "DevicePC.h"

#define DESIGN_SCREEN_SIZE 1024
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TTimer *tiCardSwipe;
	TPopupMenu *PopupMenu1;
	TMenuItem *Close1;
	TMenuItem *Refresh1;
	TMenuItem *Refresh2;
	TPanel *Panel1;
	TWebBrowser *webDisplay;
	TButton *Button1;
	TMenuItem *PointsMode1;
	TMenuItem *MembershipMode1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *ShowEntrances1;
	TMenuItem *ShowEntrances2;
	TMenuItem *N3;
	TMenuItem *ERSClubMembership1;
	TMenuItem *ExternalMembership1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall tiCardSwipeTimer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Close1Click(TObject *Sender);
	void __fastcall Refresh2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall PointsMode1Click(TObject *Sender);
	void __fastcall MembershipMode1Click(TObject *Sender);
	void __fastcall webDisplayBeforeNavigate2(TObject *ASender, const IDispatch *pDisp, OleVariant &URL, OleVariant &Flags, OleVariant &TargetFrameName, OleVariant &PostData, OleVariant &Headers, WordBool &Cancel);
	void __fastcall ShowEntrances1Click(TObject *Sender);
	void __fastcall ERSClubMembership1Click(TObject *Sender);
	void __fastcall ExternalMembership1Click(TObject *Sender);

private:	// User declarations
	void __fastcall AppMessage(tagMSG &Msg, bool &Handled);
	void __fastcall IdleHandler(TObject *Sender, bool &Done);
	void __fastcall CardSwipe(TMessage& Message);

   void Navigate(TStringList *Html);
	AnsiString CardSwipeString;
	void __fastcall Refresh();
protected:
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(UWM_CARDSWIPE, TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)

public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
	__fastcall ~TfrmMain();

   std::auto_ptr<TDeviceKiosk> MyKiosk;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
