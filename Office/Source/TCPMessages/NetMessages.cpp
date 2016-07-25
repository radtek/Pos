//---------------------------------------------------------------------------
#pragma hdrstop

#include "NetMessages.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace NetMessage;
//---------------------------------------------------------------------------
void TNetSerialObj::SerializeOut(TSerialData &Data)
{
	Reference = NextReference();

	Data << fObjectID;
	Data << fVer;
	Data << Reference;

	Write(Data);
}
//---------------------------------------------------------------------------
bool TNetSerialObj::SerializeIn(TSerialData &Data)
{
	int TCPVer;
	Data >> TCPVer;
	if (TCPVer != fVer) return false;

	Data >> Reference;

	return Read(Data);
}
//---------------------------------------------------------------------------
TRefType TNetSerialObj::NextReference()
{
	static TRefType fNextReference;
	return ++fNextReference;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TNetManager::TNetManager(int ListenPort) :
//		fUDPClient(new TIdUDPClient(NULL)),TIdTCPClient
		fUDPClient(new TIdTCPClient(NULL)),
//		fUDPServer(new TIdUDPServer(NULL)),
		fUDPServer(new TIdTCPServer(NULL)),
		fIPWatch(new TIdIPWatch(NULL))
{
	fUDPClient->BroadcastEnabled	= true;
	fUDPClient->Active				= true;

	fUDPServer->OnUDPRead			= udpServerUDPRead;
	fUDPServer->DefaultPort			= ListenPort;		// Can only be set once
	fUDPServer->Active				= true;

	fIPWatch->WatchInterval			= 10000;
	fIPWatch->HistoryEnabled		= false;
	fIPWatch->Active					= true;
	fIPWatch->ForceCheck();
}
//---------------------------------------------------------------------------
void __fastcall TNetManager::udpServerUDPRead(TObject *Sender,
		TStream *AData, TIdSocketHandle *ABinding)
{
	// All exceptions in here are eaten by the TIdUDPServer thread!
	// Executed via Synchronise so in VCL context.
	if (AData->Size > 0)
	{
		TSerialData Data(AData);

		std::auto_ptr<TNetSerialObj>Message(TSerialObjFactoryBase<TNetSerialObj, TIDType>::CreateInstance(Data));
		if (Message.get())
		{
			Message->RecipientIP = ABinding->PeerIP;

			Data >> Message->SourceIP;
			Data >> Message->DeviceName;
			Data >> Message->Description;
			Data >> Message->Product;
			Data >> Message->Sent;

			Message->Execute();
		}
	}
}
//---------------------------------------------------------------------------
void TNetManager::Send(TNetSerialObj &Message, AnsiString HostAddress, int Port)
{
	TSerialData Data;
	Message.SerializeOut(Data);

	Message.SourceIP		= fIPWatch->CurrentIP;
	Message.DeviceName	= fIPWatch->LocalName;
	Message.Description	= "";
	Message.Product		= ProductID;
	Message.Sent			= Now();

	Data << Message.SourceIP;
	Data << Message.DeviceName;
	Data << Message.Description;
	Data << Message.Product;
	Data << Message.Sent;

	fUDPClient->Host = HostAddress;
	fUDPClient->Port = Port;
	fUDPClient->SendBuffer(Data.Data(), Data.Size());
}
//---------------------------------------------------------------------------
void TNetManager::Send(TNetSerialObj &Message, AnsiString HostAddress, const std::set<int> &Ports)
{
	for (std::set<int>::iterator iPorts = Ports.begin(); iPorts!=Ports.end(); iPorts++)
	{
		Send(Message, HostAddress, *iPorts);
	}
}
//---------------------------------------------------------------------------
AnsiString TNetManager::fGetBroadcastAddress()
{
	if (!fIPWatch->IsOnline) return "";
	return fIPWatch->CurrentIP.SubString(1, fIPWatch->CurrentIP.LastDelimiter(".")) + "255";
}
//---------------------------------------------------------------------------
void TNetManager::Broadcast(TNetSerialObj &Message, int Port)
{
	Send(Message, fGetBroadcastAddress(), Port);
}
//---------------------------------------------------------------------------
void TNetManager::Broadcast(TNetSerialObj &Message, const std::set<int> &Ports)
{
	for (std::set<int>::iterator iPorts = Ports.begin(); iPorts!=Ports.end(); iPorts++)
	{
		Send(Message, fGetBroadcastAddress(), *iPorts);
	}
}
//---------------------------------------------------------------------------

