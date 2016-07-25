//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SimpleTCP.h"
#include "TCP.h"
#include "enum.h"
#include "Printing.h"
#include "Comms.h"
#include "Pause.h"

//---------------------------------------------------------------------------
#ifdef PalmMate
   #include "Palm.h"
#endif

#pragma package(smart_init)
#pragma link "TouchButton"
#pragma resource "*.dfm"
TfrmTCP *frmTCP;
//---------------------------------------------------------------------------
__fastcall TfrmTCP::TfrmTCP(TComponent* Owner)
   : TForm(Owner)
{
	// MenuMate Sends on 65001 Listens on 65001
	// PalmMate Sends on 65001 Listens on 65002
	// ChefMate Sends on 65001	Listens on 65004

   udp = new TIdUDPClient(NULL);
   udp->Port = 65001;
   udp->BroadcastEnabled = true;
   udp->Active = true;

   udpServer = new TIdUDPServer(NULL);
#ifdef PalmMate
   udpServer->DefaultPort = 65002;
#else
   udpServer->DefaultPort = 65001;
#endif
	udpServer->OnUDPRead = udpServerUDPRead;
   udpServer->Active = true;
}

__fastcall TfrmTCP::~TfrmTCP()
{
	delete udp;
	delete udpServer;
}

//---------------------------------------------------------------------------

void TfrmTCP::Action(TMemoryStream *InboundStream,AnsiString FromIP, bool SamePc)
{
//   while(Stream->Position != Stream->Size)
   {
      udpCommands TempPacketType;
      InboundStream->Read(&TempPacketType,sizeof(TempPacketType));
      InboundStream->Position -= sizeof(TempPacketType);
      switch(TempPacketType)
      {
         case udp_Unknown :
            if(Logs)TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"udp_Unknown TCP Request!");
         break;
         case udp_InfoSync :
         {
            TReqInfoSync *Request = new TReqInfoSync;
            try
            {
               Request->udpDecode(InboundStream);
               if( Request->InstructionType == udpCommand &&
               	 (!SamePc ||
               	  SamePc && Request->Product != TDeviceRealTerminal::Instance().ID.Product))
               {
                  Request->BroadcastRequest = false;
                  TDeviceRealTerminal::Instance().UpdateInfoSync(Request);
               }
            }
            __finally
            {
               delete Request;
            }
         }
         break;
         case udp_SeatName :
         {
            TReqSeatName *Request = new TReqSeatName;
            try
            {
               Request->udpDecode(InboundStream);
               if(Request->InstructionType == udpCommand &&
               	 (!SamePc ||
               	  SamePc && Request->Product != TDeviceRealTerminal::Instance().ID.Product))
               {
                  TDeviceRealTerminal::Instance().UpdateSeatName(Request);
               }
            }
            __finally
            {
               delete Request;
            }
         break;
         }
         case udp_SpecialHours :
         {
            TReqSpecialTimeCheck *Request = new TReqSpecialTimeCheck;
            try
            {
               Request->udpDecode(InboundStream);
               if(Request->InstructionType == udpCommand &&
                  (!SamePc ||
                   SamePc && Request->Product != TDeviceRealTerminal::Instance().ID.Product))
               {
                  TDeviceRealTerminal::Instance().UpdateSpecialTimeInfo(Request);
               }
               else if(Request->InstructionType == udpInfo)
               {
                  Request->GetLocal();
                  Request->InstructionType = udpCommand;
                  SendTo(Request,FromIP);
               }
            }
            __finally
            {
               delete Request;
            }
         break;
         }
         case udp_MenuChange :
         {
            TReqMenuChange *Request = new TReqMenuChange;
            try
            {
               Request->udpDecode(InboundStream);
               if(Request->InstructionType == udpCommand  &&
                  (!SamePc || SamePc && Request->Product != TDeviceRealTerminal::Instance().ID.Product))
               {
                  	if(Request->CompareToDataBase)
                     {
                        TDeviceRealTerminal::Instance().Menus.MenuAddedRemoved(Request);
                     }
                     else
                     {
                  		TDeviceRealTerminal::Instance().Menus.MenuChanged(Request);
                     }
               }
               else if(Request->InstructionType == udpInfo)
               {
                  Request->GetLocal();
                  Request->InstructionType = udpCommand;
                  SendTo(Request,FromIP);
               }
            }
            __finally
            {
               delete Request;
            }
         }
         break;
         case udp_MenuAltered :
         {
            TReqMenuAlter *Request = new TReqMenuAlter;
            try
            {
               Request->udpDecode(InboundStream);
               if(Request->InstructionType == udpCommand &&
                  (!SamePc ||
                   SamePc && Request->Product != TDeviceRealTerminal::Instance().ID.Product))
               {
                  TDeviceRealTerminal::Instance().Menus.UpdateMenuAltered(Request);
               }
               else if(Request->InstructionType == udpInfo)
               {
                  if(Logs)TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Request Type TReqMenuAlter Recived with Instruction Type udpInfo. Therefore was Ignored" );
               }
            }
            __finally
            {
               delete Request;
            }
         }
         break;
         case udp_ShutDown :
         {
            TReqShutDown *Request = new TReqShutDown;
            try
            {
               Request->udpDecode(InboundStream);
               if(Request->InstructionType == udpCommand &&
                  (!SamePc ||
                   SamePc && Request->Product != TDeviceRealTerminal::Instance().ID.Product))
               {
                  TDeviceRealTerminal::Instance().ReqShutDown(Request);
               }
               else if(Request->InstructionType == udpInfo)
               {
                  if(Logs)TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Request Type udp_ShutDown Recived with Instruction Type udpInfo. Therefore was Ignored" );
               }
            }
            __finally
            {
               delete Request;
            }
         }
         break;
         case udp_Message :
         {
            TReqMessage *Request = new TReqMessage;
            try
            {
               Request->udpDecode(InboundStream);
               if(Request->InstructionType == udpCommand &&
                  (!SamePc ||
                   SamePc && Request->Product != TDeviceRealTerminal::Instance().ID.Product))
               {
                  TDeviceRealTerminal::Instance().UpdateMessage(Request);
               }
               else if(Request->InstructionType == udpInfo)
               {
                  if(Logs)TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Request Type udp_Message Recived with Instruction Type udpInfo. Therefore was Ignored" );
               }
            }
            __finally
            {
               delete Request;
            }
         }
         break;
         case udp_Status :
         {
            #ifdef MenuMate
            TReqStatus *Request = new TReqStatus;
            try
            {
               Request->udpDecode(InboundStream);
               if(Request->InstructionType == udpCommand &&
                  (!SamePc ||
                   SamePc && Request->Product != TDeviceRealTerminal::Instance().ID.Product))
               {
                  Request->GetLocal();
                  Request->InstructionType = udpInfo;
                  SendTo(Request,FromIP);
                  // Respond with My Own Packet.
               }
            }
            __finally
            {
               delete Request;
            }
            #endif

            #ifdef AutoMate
            TReqStatus *Request = new TReqStatus;
            try
            {
               Request->udpDecode(InboundStream);
               if(Request->InstructionType == udpInfo)
               {
                  TDeviceRealTerminal::Instance().Net.AddDeviceImageFromStatus(Request);
               }
            }
            __finally
            {
               delete Request;
            }
            #endif
         }
         break;
         case udp_TabChanged :
         {
            TReqTabChanged *Request = new TReqTabChanged;
            try
            {
               Request->udpDecode(InboundStream);
               if(Request->InstructionType == udpCommand  &&
                  (!SamePc || SamePc && Request->Product != TDeviceRealTerminal::Instance().ID.Product))
               {
                  #ifdef MenuMate
	                  TDeviceRealTerminal::Instance().TabStateChanged();
                  #endif
                  #ifdef PalmMate
                     TDeviceRealTerminal::Instance().CheckTabsSetMenuList();
                  #endif
               }
            }
            __finally
            {
               delete Request;
            }
         }
         break;
      }
   }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


void TfrmTCP::SendToCheckMate(TRequest *Request)
{
   if(Request != NULL)
   {
      if(Request->InstructionType != udp_InstUnknown)
      {
         if(TGlobalSettings::Instance().AutoMateIP != "")
         {
            TMemoryStream *SentStream = new TMemoryStream;
            try
            {
               Request->udpEncode(SentStream);
               SentStream->Position = 0;
               udp->Port = 65003; // CheckMate ListenPort.
               udp->Host = TGlobalSettings::Instance().AutoMateIP;
               udp->SendBuffer(SentStream->Memory,SentStream->Size);
            }
            __finally
            {
               delete SentStream;
            }
         }
         else
         {
            if(Logs)TManagerLogs::Instance().Add(__FUNC__,CONFIGLOG,"AutoMate IP Address Not Set");
         }
      }
      else
      {
         if(Logs)TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Request Type " + Request->ClassName() +" Has Instruction Type udp_InstUnknown. Therefore was not Sent" );
      }
   }
}

void TfrmTCP::SendToAll(TTinyRequest *Request)
{
   if((Request != NULL) &&
   	(TGlobalSettings::Instance().BroadcastIP != "") &&
      (TGlobalSettings::Instance().BroadcastIP != "127.0.0.255") )
   {
      if(Request->InstructionType != udp_InstUnknown)
      {
         TMemoryStream *SentStream = new TMemoryStream;
         try
         {
         	try
            {
               Request->udpEncode(SentStream);
               SentStream->Position = 0;
               udp->Host = TGlobalSettings::Instance().BroadcastIP;
               udp->Port = 65001;
               udp->SendBuffer(SentStream->Memory,SentStream->Size);
               // Palm Mate
               SentStream->Position = 0;
               udp->Port = 65002;
               udp->SendBuffer(SentStream->Memory,SentStream->Size);
            }
            catch(Exception &Err)
            {
					if(Logs)TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message + "BroadCast IP : " + TGlobalSettings::Instance().BroadcastIP);
            }
         }
         __finally
         {
            delete SentStream;
         }
      }
      else
      {
         if(Logs)TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Request Type " + Request->ClassName() +" Has Instruction Type udp_InstUnknown. Therefore was not Sent" );
      }
   }
}

void TfrmTCP::SendTo(TRequest *Request,AnsiString IP)
{
   if(Request != NULL)
   {
      if(Request->InstructionType != udp_InstUnknown)
      {
         TMemoryStream *SentStream = new TMemoryStream;
         try
         {
            Request->udpEncode(SentStream);
            SentStream->Position = 0;
            udp->Host = IP;
				udp->Port = 65001;
            udp->SendBuffer(SentStream->Memory,SentStream->Size);
            // Palm Mate
            SentStream->Position = 0;
				udp->Port = 65002;
            udp->SendBuffer(SentStream->Memory,SentStream->Size);
         }
         __finally
         {
            delete SentStream;
         }
      }
      else
      {
         if(Logs)TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Request Type " + Request->ClassName() +" Has Instruction Type udp_InstUnknown. Therefore was not Sent" );
      }
   }
}


//---------------------------------------------------------------------------


void __fastcall TfrmTCP::udpServerUDPRead(TObject *Sender,
      TStream *AData, TIdSocketHandle *ABinding)
{
   try
   {
      // TODO 4 -o Michael -c Improvement: AutoMate Interface
      // Set up a dif port for AutoMate Req. This way MM running on
      // the smae machine as AM will still process UDP packets.
      bool SamePc = false;
      if(ABinding->PeerIP == IPWatch->CurrentIP)
      {
	      SamePc = true;
      }
      if(AData->Size > 0)
      {
         TMemoryStream *InboundStream = new TMemoryStream;
         try
         {
               InboundStream->CopyFrom(AData,0);
               InboundStream->Position = 0;
               Action(InboundStream,ABinding->PeerIP,SamePc);
         }
         __finally
         {
            delete InboundStream;
         }
      }
   }
   catch(Exception &Err)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message + "IP : " + ABinding->PeerIP + " Byte Count " + IntToStr(AData->Size) + " Port " + IntToStr(ABinding->PeerPort));
   }
}
//---------------------------------------------------------------------------

