//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "NetMessageKillProc.h"
#include "Stream.h"
#include "Processing.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TNetMessageKillProc::TNetMessageKillProc()
{
	MessageType = udp_KillProc;
}

void TNetMessageKillProc::Encode(TMemoryStream *Stream)
{
	EncodeNetMessage(Stream);
	StreamWrite(Stream,Device);
	StreamWrite(Stream,User);
}

//---------------------------------------------------------------------------

void TNetMessageKillProc::Decode(TMemoryStream *Stream)
{
	DecodeNetMessage(Stream);
	StreamRead(Stream,Device);
	StreamRead(Stream,User);
}

void TNetMessageKillProc::Action()
{
   std::auto_ptr<TfrmProcessing>(frmProcessing)(TfrmProcessing::Create<TfrmProcessing>(Screen->ActiveForm));
   frmProcessing->Message = "Till Shutting Down in 15 Seconds";
   frmProcessing->ShowProgress = true;
   frmProcessing->CanCancel = true;
   frmProcessing->Position = 0;
	__int64 StartTime = ::GetTickCount();
	__int64 TickCount = StartTime + (1000 * 15);
   frmProcessing->Max = (1000 * 15);
   frmProcessing->Show();
	while (::GetTickCount() < TickCount && !frmProcessing->Cancelled)
   {
      Application->ProcessMessages();
      frmProcessing->Position = ::GetTickCount() - StartTime;
   }

   if(!frmProcessing->Cancelled)
   {
      Application->Terminate();
   }
}






