//---------------------------------------------------------------------------
#ifndef TCPH
#define TCPH

#define CHECKCONNECTIONS  (WM_APP + 450)
//---------------------------------------------------------------------------
#include "IBLogs.h"
#include "DeviceRealTerminal.h"
#include "enum.h"
#include "Requests.h"
#include "TouchButton.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdIPWatch.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdUDPServer.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------

enum TCPState {eBooting,eUnconfigured,eFindingHost,eFindingNewHost,eClosing};
class TCPPacket;

class TfrmTCP : public TForm
{
__published:	// IDE-managed Components
   TImageList *NetImages;
	TIdIPWatch *IPWatch;
	void __fastcall udpServerUDPRead(TObject *Sender, TStream *AData,
          TIdSocketHandle *ABinding);
private:	// User declarations
public:		// User declarations
	TIdUDPClient *udp;
	TIdUDPServer *udpServer;

   __fastcall TfrmTCP(TComponent* Owner);
   __fastcall ~TfrmTCP();

  TCPState State;
//  TStringList *PossibleHosts;
  bool RequestClose;
  void SendToCheckMate(TRequest *Request);
  void SendToAll(TTinyRequest *Request);
  void SendTo(TRequest *Request,AnsiString IP);
  void Action(TMemoryStream *InboundStream,AnsiString FromIP, bool SamePc);
protected:
};


//---------------------------------------------------------------------------
extern PACKAGE TfrmTCP *frmTCP;
//---------------------------------------------------------------------------
#endif

