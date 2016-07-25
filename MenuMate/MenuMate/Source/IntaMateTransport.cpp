//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "IntaMateTransport.h"
#include "MMLogging.h"

#include <DateUtils.hpp>
#include <system.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)


TIM_Transport_TCPIP::TIM_Transport_TCPIP()
{
   ConnectTimeOut    = defaultConnectTimeOut;
   ReadTimeOut       = defaultReadTimeOut;
   RetryCount        = defaultRetryCount;
   SleepOnException  = defaultSleepOnException;
   OffLine           = true;   
}

//---------------------------------------------------------------------------


void TIM_Transport_TCPIP::TimeOuts(int inConnectTimeOut,int inReadTimeOut,int inRetryCount, int inSleepOnException)
{
   ConnectTimeOut = inConnectTimeOut;
   ReadTimeOut    = inReadTimeOut;
   RetryCount     = inRetryCount;
   SleepOnException = inSleepOnException;
}
//---------------------------------------------------------------------------

void TIM_Transport_TCPIP::GetStatus(AnsiString IP, int Port, int ReadTimeOut)
{
   if(IP != "" && Port != 0)
   {
      std::auto_ptr<TPOS_XMLStatus> POSXML(new TPOS_XMLStatus());

      POSXML->Cache = false;
      POSXML->Build();
      ConnectionStatus = "";

      TimeOuts(500,ReadTimeOut,1,0);
      Process(IP,Port,*POSXML.get());
      POSXML->Parse();
      if(POSXML->Result == eIMAccepted)
      {
         OffLine = false;
      }
      else
      {
         OffLine = true;
         TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG, "Interface Offline Status Nak" + POSXML->ResultText);
      }
      ConnectionStatus = POSXML->ResultText;
		TimeOuts(defaultConnectTimeOut,ReadTimeOut,defaultRetryCount,defaultSleepOnException);
   }
   else
   {
      OffLine = true;
      ConnectionStatus = "Not Configured";
      TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG, "Interface Offline " + ConnectionStatus);
   }
}
//---------------------------------------------------------------------------

void TIM_Transport_TCPIP::Process(AnsiString inIP, int inPort,TPOS_XMLBase &Packet)
{
   try
   {
   	SendAndFetch(Packet, inIP, inPort);
	}
   catch(Exception &E)
   {
      Packet.Result = eIMCache;
      Packet.ResultText = E.Message;
      TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Serious SendAndFetch Error");
      TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,Packet.ResultText);
      ::Sleep(SleepOnException);
   }
}

//---------------------------------------------------------------------------

void TIM_Transport_TCPIP::SendAndFetch(TPOS_XMLBase &Packet, AnsiString HostAddress, int Port)
{
	bool Retry = false;
	int RetryCount = 0;
   do
	{
		std::auto_ptr<TIdTCPClient> fTCPClient(new TIdTCPClient(NULL));
		Retry = false;
		try
		{
			if(HostAddress != "")
			{
				Packet.Result = eIMCache;
				AnsiString Data = Packet.SerializeOut();

            AnsiString Length = "0000000000";
            int Size = 0;
            Size = Data.Length() -1;
            Length.sprintf("%10.10d",Size);
            Data = Length + Data;

				if(!fTCPClient->Connected())
				{
					fTCPClient->Host = HostAddress;
					fTCPClient->Port = Port;
					fTCPClient->BoundPort = 0;
					fTCPClient->ReadTimeout = ReadTimeOut;
					fTCPClient->ConnectTimeout = ConnectTimeOut;
					fTCPClient->Connect();
				}

				if(fTCPClient->Connected())
				{
					TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Write :" + Data);
					TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Write Length :" + IntToStr(Data.Length()));
					fTCPClient->IOHandler->WriteLn(Data);
					Data = "";
					//Data =  fTCPClient->IOHandler->ReadLnWait();

                   TByteDynArray Incomming;
                   fTCPClient->IOHandler->ReadBytes(Incomming, 10, false);
                   Data = AnsiString((char *)&Incomming[0],10);
                   TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Read Size: " + Data);
                   int DataLength = StrToIntDef(Data,0);
                   Data = "";
                   if(DataLength > 0)
                   {
                       Data = fTCPClient->IOHandler->ReadString(DataLength);
                   }
                   else
                   {
                        TByteDynArray Incomming;
                        fTCPClient->IOHandler->ReadBytes(Incomming, -1, false);
                        Data = AnsiString((char *)&Incomming[0],Incomming.Length);
                   }

					TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Read : " + Data);
					TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Read Length :" + IntToStr(Data.Length()));


					if(Data == "")
					{
						Packet.Result = eIMCache;
						Packet.ResultText = "IntaMate Server Failed To Respond.";
						TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,Packet.ResultText);
					}
					else
					{
						TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Read :" + Data);
						Packet.Result = eIMAccepted;
						Packet.SerializeIn(Data);
					}
				}
				else
				{
					Packet.Result = eIMCache;
					Packet.ResultText = "Failed To Connect to IntaMate Server.";
					TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,Packet.ResultText);
				}
			}
			else
			{
				Packet.Result = eIMCache;
				Packet.ResultText = "Invalid IntaMate Host Address :" + HostAddress;
				TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,Packet.ResultText);
			}
		}
		catch(Exception &E)
		{
			Retry = true;
			RetryCount++;
			Packet.Result = eIMCache;
			Packet.ResultText = E.Message;
			TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,Packet.ResultText);
			TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Packet.ResultText);
			::Sleep(SleepOnException);
		}
	}
	while(Retry == true && RetryCount < defaultRetryCount);
}
//---------------------------------------------------------------------------
void TIM_Transport_Failed::Process(TPOS_XMLBase &POSXML,UnicodeString FileName)
{
	if(POSXML.Cache)
   {
      if(!DirectoryExists(ExtractFilePath(Application->ExeName) + "IntaMate\\Failed"))
      {
         ForceDirectories(ExtractFilePath(Application->ExeName) + "IntaMate\\Failed");
      }

      if(FileName != "")
		{ // If the file failed to laod into the XML Doc jsut copy it.
			UnicodeString Dest = UnicodeString(ExtractFilePath(Application->ExeName) + "IntaMate\\Failed\\" + ExtractFileName(FileName));
			CopyFileW(FileName.w_str(), Dest.w_str(),false);
			DeleteFile(FileName);
		}
		else
		{
			POSXML.Doc.SaveFile(UnicodeString(ExtractFilePath(Application->ExeName) + "IntaMate\\Failed\\" + POSXML.IntaMateID + ".xml").t_str());
		}

      AddFailedRecord(POSXML);
		TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Failed :" + POSXML.IntaMateID);
   }
}

void TIM_Transport_Failed::AddFailedRecord(TPOS_XMLBase &POSXML)
{
   try
   {
      TiXmlDocument Cache;
      Cache.Clear();
      if(FileExists(ExtractFilePath(Application->ExeName) + "IntaMate\\IntaMate.Failed"))
      {
         Cache.LoadFile((ExtractFilePath(Application->ExeName) + "IntaMate\\IntaMate.Failed").t_str());
      }
      else
      {
			TiXmlDeclaration * decl = new TiXmlDeclaration( _T("1.0"), _T("UTF-8"), _T("") );
         Cache.LinkEndChild( decl );
			TiXmlElement * Transaction = new TiXmlElement( _T("FailedRecords") );
			Cache.LinkEndChild( Transaction );
      }

// Insert DOCTYPE definiation here.

      TiXmlHandle hDoc(&Cache);
      TiXmlElement* pElement;
		pElement = hDoc.FirstChild(_T("FailedRecords")).ToElement();
		if (pElement)
		{
			TiXmlElement * Transaction = new TiXmlElement( _T("Transaction") );
         Transaction->SetAttribute(xmlAttrID, AnsiString(POSXML.IntaMateID).c_str());
         Transaction->SetAttribute(xmlFileName, (POSXML.IntaMateID + ".xml").t_str());
         Transaction->SetAttribute(xmlErr, POSXML.ResultText.t_str());
         pElement->LinkEndChild( Transaction );
      }
      else
      {
   		TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Failed Index Failure: no FailedRecords Element " + POSXML.IntaMateID);
      }
      Cache.SaveFile((ExtractFilePath(Application->ExeName) + "IntaMate\\IntaMate.failed").t_str());
   }
   catch(Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Cached Failure: " + E.Message);
   }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TIM_Transport_Cache::TIM_Transport_Cache(AnsiString inIP,int inPort,int inReadTimeOut) :
                                             IM_TCPIP(new TIM_Transport_TCPIP),
                                             IM_Failed(new TIM_Transport_Failed),
                                             IM_Accepted(new TIM_Transport_Accepted),
                                             TThread(true)
{
   IP = inIP;
	Port = inPort;
	ReadTimeOut = inReadTimeOut;
	IM_TCPIP->TimeOuts(defaultConnectTimeOut,defaultReadTimeOut,defaultRetryCount,defaultSleepOnException);

	if(!DirectoryExists(ExtractFilePath(Application->ExeName) + "IntaMate\\Cache"))
   {
      ForceDirectories(ExtractFilePath(Application->ExeName) + "IntaMate\\Cache");
   }
}

__fastcall TIM_Transport_Cache::~TIM_Transport_Cache()
{
   Terminate();
}

void __fastcall TIM_Transport_Cache::Execute()
{
	try
	{
		try
		{
			do
			{
            if(IP != "" && Port != 0 && !Terminated)
            {
               if(IM_TCPIP->OffLine)
               {
                  IM_TCPIP->GetStatus(IP,Port,ReadTimeOut);
               }
               else
               {
                  if (DirectoryExists(ExtractFilePath(Application->ExeName) + "IntaMate\\Cache"))
                  {
                     std::auto_ptr<TStringList> Files(new TStringList);
                     TSearchRec sr;
                     int iAttributes = faAnyFile;

                     if (FindFirst(ExtractFilePath(Application->ExeName) + "IntaMate\\Cache\\*.xml", iAttributes, sr) == 0)
                     {
                        if (sr.Attr & iAttributes)
                        {
                           Files->Add(ExtractFilePath(Application->ExeName) + "IntaMate\\Cache\\" + sr.Name);
                        }
                        while (FindNext(sr) == 0)
                        {
                           if (sr.Attr & iAttributes)
                           {
                              Files->Add(ExtractFilePath(Application->ExeName) + "IntaMate\\Cache\\" + sr.Name);
                           }
                        }
                     }

                     FindClose(sr);

                     if (Files->Count > 0)
                     {
                        try
                        {
                           TPOS_XMLBase CacheData("");
                           for (int i=0; i<Files->Count; i++)
                           {
                              AnsiString FileName = Files->Strings[i];
                              if(FileExists(FileName))
                              {
                                 CacheData.Clear();
                                 CacheData.LoadFromFile(FileName);
                                 if(!CacheData.Doc.Error())
                                 {
                                    IM_TCPIP->Process(IP,Port,CacheData);
                                    if(CacheData.Result == eIMAccepted)
                                    {
                                       // Check for a good response.
                                       CacheData.Parse();
                                       if(CacheData.Result == eIMAccepted)
                                       {
                                          IM_Accepted->Process(CacheData,FileName);
                                       }
                                       else if(CacheData.Result == eIMCache)
                                       {
                                          //Transaction ID Mismatch, leave it to be resent.
														TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,CacheData.ResultText);
                                       }
                                       else
                                       {
                                          TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,CacheData.ResultText);
                                          IM_Failed->Process(CacheData,FileName);
                                       }
                                    }
                                    else if(CacheData.Result == eIMFailed)
                                    {  // Something bad happened with the Responce the Server sent.
                                       // Rather than failling it a re-send might be more appropreate.
                                       TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,CacheData.ResultText);
                                       IM_Failed->Process(CacheData,FileName);
                                    }
                                    else if(CacheData.Result == eIMCache)
                                    {
                                       IM_TCPIP->OffLine = true;
                                       TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Interface Offline :" + CacheData.ResultText);
                                    }
                                 }
                                 else
                                 {
                                    //It didn't load so copy this file.
                                    TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Error Loading Cache Transation :" + AnsiString(CacheData.Doc.ErrorDesc()));
                                    CacheData.ResultText = AnsiString(CacheData.Doc.ErrorDesc());
                                    IM_Failed->Process(CacheData,FileName);
                                 }
                              }
                              else
                              {
                                 TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Error Loading Cache Transation : File Not Found " + FileName);
                              }
                           }
                        }
                        catch (Exception &E)
                        {
                           TManagerLogs::Instance().Add(__FUNC__,SMARTCARDLOG,E.Message);
                        }
                     }
                  }
                  else
                  {
                     AnsiString Path = ExtractFilePath(Application->ExeName) + "IntaMate\\Cache";
                     CreateDir(Path);
                  }
               }
            }
            Sleep(10000);
			}
			while (!Terminated);
		}
		__finally
		{
		}
	}
	catch (Exception &E)
	{
      TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,E.Message);
	}
   ReturnValue = 1;
}

void TIM_Transport_Accepted::Process(TPOS_XMLBase &POSXML,UnicodeString FileName)
{
	if(POSXML.Cache)
   {
      if(!DirectoryExists(ExtractFilePath(Application->ExeName) + "IntaMate\\Successful"))
      {
         ForceDirectories(ExtractFilePath(Application->ExeName) + "IntaMate\\Successful");
      }

      if(FileName != "")
		{ // If the file failed to laod into the XML Doc jsut copy it.
			UnicodeString Dest = UnicodeString(ExtractFilePath(Application->ExeName) + "IntaMate\\Successful\\" + ExtractFileName(FileName));
			CopyFileW(FileName.w_str(), Dest.w_str(),false);
			DeleteFile(FileName);
		}
		else
		{
			POSXML.Doc.SaveFile(UnicodeString(ExtractFilePath(Application->ExeName) + "IntaMate\\Successful\\" + POSXML.IntaMateID + ".xml").t_str());
		}

      CleanSuccessfullRecords();
		TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Successful Send :" + POSXML.IntaMateID);
   }
}

void TIM_Transport_Accepted::CleanSuccessfullRecords()
{
   if (DirectoryExists(ExtractFilePath(Application->ExeName) + "IntaMate\\Successful"))
   {
      std::auto_ptr<TStringList> Files(new TStringList);
      TSearchRec sr;
      int iAttributes = faAnyFile;

      if (FindFirst(ExtractFilePath(Application->ExeName) + "IntaMate\\Successful\\*.xml", iAttributes, sr) == 0)
      {
         if (sr.Attr & iAttributes)
         {
            TDateTime Age = FileDateToDateTime(sr.Time);
            if(DaysBetween(Now(),Age) > 7)
            {
               Files->Add(ExtractFilePath(Application->ExeName) + "IntaMate\\Successful\\" + sr.Name);
            }
         }
         while (FindNext(sr) == 0)
         {
            if (sr.Attr & iAttributes)
            {
               TDateTime Age = FileDateToDateTime(sr.Time);
               if(DaysBetween(Now(),Age) > 7)
               {
                  Files->Add(ExtractFilePath(Application->ExeName) + "IntaMate\\Successful\\" + sr.Name);
               }
            }
         }
      }

      FindClose(sr);

      for (int i=0; i<Files->Count; i++)
      {
         AnsiString FileName = Files->Strings[i];
         DeleteFile(FileName);
      }
   }
}
