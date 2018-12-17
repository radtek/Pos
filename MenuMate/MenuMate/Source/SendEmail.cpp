//---------------------------------------------------------------------------

#pragma hdrstop

#include "SendEmail.h"

#include <vcl.h>
#include <windows.h>
#include "MMMessageBox.h"
#include "IdAntiFreeze.hpp"

#include <memory>
#define semi_colon 0x3B
#define MAX_SIZE 1024


#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdExplicitTLSClientServerBase"
#pragma link "IdIOHandler"
#pragma link "IdIOHandlerSocket"
#pragma link "IdIOHandlerStack"
#pragma link "IdMessage"
#pragma link "IdMessageClient"
#pragma link "IdSMTP"
#pragma link "IdSMTPBase"
#pragma link "IdSSL"
#pragma link "IdSSLOpenSSL"
#pragma link "IdTCPClient"
#pragma link "IdTCPConnection"


//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
bool SendEmail::Send(const AnsiString& attachmentFileName, const AnsiString& subject, AnsiString& mailTo, const AnsiString& body, bool isLoyaltyEmail)
{
   AnsiString mailToName = "MM Office Generated";
   bool result = Send(attachmentFileName, subject, mailTo, mailToName, body, isLoyaltyEmail);

   return result;
}

AnsiString Get_FileName(AnsiString& filename );
//---------------------------------------------------------------------------

bool SendEmail::Send(const AnsiString& attachmentFileName, const AnsiString& subject, AnsiString& mailTo, const AnsiString& mailToName,
                    const AnsiString& body, bool isLoyaltyEmail)
{
   // for multiple files..
   TStringList *attachments = new TStringList;

    char *FileLine = attachmentFileName.c_str();
    AnsiString TrimmedFileLine;

    for(int i=0; i < attachmentFileName.Length() ; i++)
    {
       if(FileLine[i] == semi_colon)
       {
         attachments->Add(TrimmedFileLine);
         TrimmedFileLine = "";
       }
       else if(i == attachmentFileName.Length() - 1)
       {
          TrimmedFileLine += FileLine[i];
          attachments->Add(TrimmedFileLine);
       }
       else
       {
           TrimmedFileLine += FileLine[i];
       }
    }


    int numFile = attachments->Count;
    AnsiString *fileattachment = new AnsiString[numFile];
    AnsiString *filename = new AnsiString[numFile];

    for(int i=0; i < numFile ; i++)
    {
      fileattachment[i] = attachments->Strings[i];
      filename[i] = ::Get_FileName(fileattachment[i]).c_str();
    }


   if(!isLoyaltyEmail && (TGlobalSettings::Instance().EmailHost == "" || TGlobalSettings::Instance().EmailHostPort == 0 || TGlobalSettings::Instance().EmailId == "" || TGlobalSettings::Instance().EmailPassword == ""))
   {
      MessageBox("Unable to Send Email. \r"
      "Please Configure Email Settings Details. \r\r " "", "Information",
      MB_OK + MB_ICONERROR);
      return false;
   }

   std::auto_ptr<TIdAntiFreeze> IdAntiFreeze1(new TIdAntiFreeze(NULL));
   IdAntiFreeze1->IdleTimeOut = 250;

   std::auto_ptr<TIdSMTP> IdSMTP1(new TIdSMTP(NULL));
   TIdSSLIOHandlerSocketOpenSSL *IdSSLIOHandlerSocketOpenSSL1 = new TIdSSLIOHandlerSocketOpenSSL(IdSMTP1.get());

   IdSSLIOHandlerSocketOpenSSL1->SSLOptions->Method = sslvTLSv1;
   IdSSLIOHandlerSocketOpenSSL1->SSLOptions->Mode = sslmClient;

   IdSSLIOHandlerSocketOpenSSL1->IPVersion = Id_IPv4;
   IdSSLIOHandlerSocketOpenSSL1->ConnectTimeout = 30000;
   IdSSLIOHandlerSocketOpenSSL1->ReadTimeout = 30000;

   if(isLoyaltyEmail)
   {
       IdSSLIOHandlerSocketOpenSSL1->Host = "smtp.gmail.com";
       IdSSLIOHandlerSocketOpenSSL1->Port = 25;
       IdSMTP1->Host = "smtp.gmail.com";
       IdSMTP1->Username = "developmentmenumate@gmail.com";
       IdSMTP1->Password = "Menumate@1202";
       IdSMTP1->Port = 25;
   }
   else
   {
       IdSSLIOHandlerSocketOpenSSL1->Host = TGlobalSettings::Instance().EmailHost;
       IdSSLIOHandlerSocketOpenSSL1->Port = TGlobalSettings::Instance().EmailHostPort;
       IdSMTP1->Host = TGlobalSettings::Instance().EmailHost;
       IdSMTP1->Username = TGlobalSettings::Instance().EmailId;
       IdSMTP1->Password = TGlobalSettings::Instance().EmailPassword;
       IdSMTP1->Port = TGlobalSettings::Instance().EmailHostPort;
   }
   IdSMTP1->IOHandler = IdSSLIOHandlerSocketOpenSSL1;
   IdSMTP1->UseTLS = utUseImplicitTLS;
   IdSMTP1->ConnectTimeout = 30000;
   IdSMTP1->ReadTimeout = 40000;
   IdSMTP1->AuthType = satDefault;

   std::auto_ptr<TIdMessage> IdMessage1(new TIdMessage(NULL));

   IdMessage1->Clear();
   IdMessage1->Recipients->EMailAddresses = mailTo.c_str();
   IdMessage1->Priority = mpHigh;
   IdMessage1->Encoding = meMIME;
   IdMessage1->Subject = subject;
   IdMessage1->Body->Text = body;

   IdMessage1->AttachmentEncoding = "MIME";

   for(int i =0; i < numFile; i++)
   {
      TIdAttachmentFile *multipleattachment = new TIdAttachmentFile(IdMessage1->MessageParts, fileattachment[i]);
      multipleattachment->ContentType = GetMIMETypeFromFile(fileattachment[i]);
      multipleattachment->FileName = filename[i];
   }

   try
   {
      IdSMTP1->Connect();
      IdSMTP1->Send(IdMessage1.get());
   }
   catch(Exception &e)
   {
      MessageBox("Unable to Send Email. \r"
       "Please write down and report the following message to your service provider. \r\r " + e.Message, "Error",
       MB_OK + MB_ICONERROR);
     return false;
   }

   IdSMTP1->Disconnect();
   return true;
}


AnsiString Get_FileName(AnsiString& filename )
{
 AnsiString file_name="";
 char* temp_char = new char[filename.Length()+1];
 strcpy(temp_char, filename.c_str());
 int len = filename.Length();
 while(len--)
 {
   if(temp_char[len] == 'Z')
   {
     file_name = filename.SubString(len+1, (filename.Length()+1)-len);
     break;
   }
 }

 return file_name;
}
//---------------------------------------------------------------------------

