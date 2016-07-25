//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <system.hpp>
#include <MMLogging.h>
#include <Classes.hpp>

#include "PocketVoucherTransport.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

TXML_Transport_TCPIP::TXML_Transport_TCPIP()
{
   ConnectTimeOut    = defaultConnectTimeOut;
   ReadTimeOut       = defaultReadTimeOut;
   RetryCount        = defaultRetryCount;
   SleepOnException  = defaultSleepOnException;
   OffLine           = true;
}

//---------------------------------------------------------------------------


void TXML_Transport_TCPIP::TimeOuts(int inConnectTimeOut,int inReadTimeOut,int inRetryCount, int inSleepOnException)
{
   ConnectTimeOut = inConnectTimeOut;
   ReadTimeOut    = inReadTimeOut;
   RetryCount     = inRetryCount;
   SleepOnException = inSleepOnException;
}
//---------------------------------------------------------------------------

void TXML_Transport_TCPIP::Process(AnsiString inIP, int inPort,TXMLBase &XML)
{
   SendAndFetch(XML, inIP, inPort);
}

//---------------------------------------------------------------------------

void TXML_Transport_TCPIP::SendAndFetch(TXMLBase &Packet, AnsiString HostAddress, int Port)
{
	bool Retry = false;
	int RetryCount = 0;
	std::auto_ptr<TIdTCPClient> fTCPClient(new TIdTCPClient(NULL));

   do
	{
		Retry = false;
		try
		{
			if(HostAddress != "")
			{
				Packet.Result = eXMLFailed;
				AnsiString Data = Packet.SerializeOut();
				if(!fTCPClient->Connected())
				{
					fTCPClient->Host = HostAddress;
					fTCPClient->Port = Port;
					fTCPClient->BoundPort = 0;
               fTCPClient->ReadTimeout = ReadTimeOut;
               //fTCPClient->ConnectTimeout = ConnectTimeOut; Not in C++Builder 6
					fTCPClient->Connect();
				}

				if(fTCPClient->Connected())
				{
					TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,"Write Line :" + Data);
					TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,"Write Length :" + IntToStr(Data.Length()));
//					fTCPClient->IOHandler->WriteLn(Data); Not in C++Builder 6
					fTCPClient->IOHandler->WriteLn(Data);
					Data = "";
//					Data = fTCPClient->IOHandler->ReadLnWait(); Not in C++Builder 6
					Data = fTCPClient->IOHandler->ReadLn(Idglobal::LF,ReadTimeOut , 255);

//               std::auto_ptr<TMemoryStream> AStream(new TMemoryStream);
//               fTCPClient->ReadStream(AStream.get(), -1, True);
//               Data = AnsiString((char *)AStream->Memory,AStream->Size);
					if(Data =="")
					{
						Packet.Result = eXMLFailed;
						Packet.ResultText = "IntaMate Server Failed To Respond.";
						TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,Packet.ResultText);
					}
					else
					{
						TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,"Read :" + Data);
						Packet.Result = eXMLAccepted;
						Packet.SerializeIn(Data);
					}

					if(fTCPClient->Connected())
					{
						fTCPClient->Disconnect();
					}
				}
				else
				{
					Packet.Result = eXMLFailed;
					Packet.ResultText = "Failed To Connect to IntaMate Server.";
					TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,Packet.ResultText);
				}
			}
			else
			{
				Packet.Result = eXMLFailed;
				Packet.ResultText = "Invalid IntaMate Host Address :" + HostAddress;
				TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,Packet.ResultText);
			}
		}
		catch(Exception &E)
		{
	      	fTCPClient->Disconnect();
			Retry = true;
			RetryCount++;
			Packet.Result = eXMLFailed;
			Packet.ResultText = E.Message;
			TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,Packet.ResultText);
			::Sleep(SleepOnException);
		}
	}
	while(Retry == true && RetryCount < RetryCount);
}

TXML_Transport_Http::TXML_Transport_Http()
{
   ConnectTimeOut    = defaultConnectTimeOut;
   ReadTimeOut       = defaultReadTimeOut;
   RetryCount        = defaultRetryCount;
   SleepOnException  = defaultSleepOnException;
   OffLine           = true;
}

//---------------------------------------------------------------------------


void TXML_Transport_Http::TimeOuts(int inConnectTimeOut,int inReadTimeOut,int inRetryCount, int inSleepOnException)
{
   ConnectTimeOut = inConnectTimeOut;
   ReadTimeOut    = inReadTimeOut;
   RetryCount     = inRetryCount;
   SleepOnException = inSleepOnException;
}
//---------------------------------------------------------------------------

void TXML_Transport_Http::Process(AnsiString inIP, int inPort,TXMLBase &XML)
{
   SendAndFetch(XML, inIP, inPort);
}

//---------------------------------------------------------------------------

void TXML_Transport_Http::SendAndFetch(TXMLBase &Packet, AnsiString HostAddress, int Port)
{
	bool Retry = false;
	int RetryCount = 0;

   do
	{
		Retry = false;
		try
		{
            if(HostAddress == "")
            {
               Packet.Result = eXMLFailed;
            }
            else
            {
               std::auto_ptr<TStringStream> Data (new TStringStream(Packet.SerializeOut()));
               std::auto_ptr<TStringList> Result (new TStringList());
					std::auto_ptr<TIdHTTP> fhttp (new TIdHTTP(NULL));
               fhttp->Request->ContentType = "application/x-www-form-urlencoded";
               fhttp->Request->UserAgent = "MenuMate";
               TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,"Post Data :" + Data->DataString);

               std::auto_ptr<TFileStream> FileData (new TFileStream("XMLOut.xml",fmCreate));
               FileData->CopyFrom(Data.get(), 0);

               Result->Text = fhttp->Post(HostAddress,Data.get());

               Result->SaveToFile("XMLin.xml");

               if(Result->Text == "")
               {
                  Packet.Result = eXMLFailed;
                  Packet.ResultText = "IntaMate Server Failed To Respond.";
                  TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,Packet.ResultText);
               }
               else
               {
                  TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,"Read :" + Result->Text);
                  Packet.Result = eXMLAccepted;
                  Packet.SerializeIn(Result->Text);
               }
            }
		}
		catch(Exception &E)
		{
			Retry = true;
			RetryCount++;
			Packet.Result = eXMLFailed;
			Packet.ResultText = E.Message;

         std::auto_ptr<TFileStream> FileData (new TFileStream("XMLin.xml",fmCreate));
         FileData->Write(E.Message.c_str(), E.Message.Length());

			TManagerLogs::Instance().Add(__FUNC__,XMLINTERFACELOG,Packet.ResultText);
			::Sleep(SleepOnException);
		}
	}
	while(Retry == true && RetryCount < RetryCount);
}

