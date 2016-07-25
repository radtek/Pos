//---------------------------------------------------------------------------

#ifndef DeskMainH
#define DeskMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "touchbtn.h"
#include "touchcontrols.h"
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <memory>
#include "ContactStaff.h"
#include "SystemConfigDeskMate.h"
#include "MM_DBCore.h"
#include "VerticalSelect.h"
#include "tinyxml.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "ManagerNet.h"
//---------------------------------------------------------------------------
class TRetrieveNamesThread;

class TfrmMain : public TForm
{
__published:	// IDE-managed Components
   TPanel *pButtons;
   TTouchBtn *TouchBtn1;
   TTouchBtn *TouchBtn5;
	TTouchBtn *tbtnOnlineUpdate;
   TTouchBtn *tbtnOnlineHelp;
   TTouchBtn *tbtnTills;
   TTouchBtn *tbtnDatabase;
	TLabel *lbeVersion;
   void __fastcall btnStartClick(TObject *Sender);
   void __fastcall btnCloseClick(TObject *Sender);
   void __fastcall tbtnOnlineUpdateMouseClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall tbtnOnlineHelpMouseClick(TObject *Sender);
   void __fastcall tbtnDatabaseMouseClick(TObject *Sender);
   void __fastcall tbtnOnlineHelpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall tbtnTillsMouseClick(TObject *Sender);
private:	// User declarations
	Database::TDBControl DBControl;
	void __fastcall IdleHandler(TObject *Sender, bool &Done);
	void OpenDB();
	void CloseDB();
   void XMLGUINav(TiXmlHandle &doc);
   TRetrieveNamesThread *RetrieveNamesThread;
   void __fastcall RetrieveNamesThreadTerminate(TObject *Sender);
   std::auto_ptr<TManagerNet> ManagerNet;   
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
	bool ShutDown(UINT uFlags);
	bool SetPrivilege(HANDLE hToken, LPCTSTR Privilege, bool bEnablePrivilege);
	void ShowLastErr();
   std::auto_ptr<TStringList> StringList;
   void TerminateMenuMate(AnsiString ComputerName,int Port);   

};

class TRetrieveNamesThread : public TThread
{
private:
   void __fastcall EnumNetRes(NETRESOURCE* nr, TStrings *ComputerNames);
   void __fastcall AddName();
   AnsiString ComputerName;
protected:
   void __fastcall Execute();
public:
   __fastcall TRetrieveNamesThread(bool CreateSuspended);
   TStrings *ComputerNames;
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
