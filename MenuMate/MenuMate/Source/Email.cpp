//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <IdMessageBuilder.hpp>

#include "Email.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
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
#pragma resource "*.dfm"

#define semi_colon 0x3B
#define MAX_SIZE 1024


TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void TForm2::SendEmail(const AnsiString& attachmentFileName,
		const AnsiString& subject,
		       AnsiString& mailTo,
                const AnsiString& mailToName)
{

  	TCHAR szFileName[_MAX_PATH];
	TCHAR szPath[_MAX_PATH];
	TCHAR szSubject[_MAX_PATH];
	TCHAR szRecip[_MAX_PATH];


	strcpy(szSubject, subject.c_str());
    strcpy(szRecip, "smtp:");
    strcat(szRecip, mailTo.c_str());

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
    for(int i=0; i < numFile ; i++)
    {
      fileattachment[i] = attachments->Strings[i];
    }

    AnsiString *filename = new AnsiString[numFile];
    for(int i=0; i < numFile ; i++)
    {
      filename[i] = get_filename(fileattachment[i]).c_str();
    }


    TStringList *emails = new TStringList;

    const char *Line = mailTo.c_str();
    AnsiString TrimmedLine;
    for(int i=0; i < mailTo.Length() ; i++)
    {
       if(Line[i] == semi_colon)
       {
         TrimmedLine = "smtp:" + TrimmedLine;
         emails->Add(TrimmedLine);
         TrimmedLine = "";
       }
       else if(i == mailTo.Length() - 1)
       {
          TrimmedLine += Line[i];
          TrimmedLine = "smtp:" + TrimmedLine;
          emails->Add(TrimmedLine);
       }
       else
       {
           TrimmedLine += Line[i];
       }
    }

    int numRecip = emails->Count;
    AnsiString *address = new AnsiString[numRecip];

    for(int i=0; i < numRecip ; i++)
    {
     address[i] = emails->Strings[i];
    }



   IdSMTP1->Host = "smtp.yahoo.com";//smtp.gmail.com";
   IdSMTP1->Username = "nitesh.menumate@yahoo.com";
   IdSMTP1->Password = "";

   //IdMessage1->Recipients = new IdMessage1[10];

   IdMessage1->Clear();


   IdMessage1->Recipients->Items[0]->Address = "nitesh.menumate@yahoo.com";

   //IdMessage1->Recipients->Items[0]->Address = "nitesh.menumate@yahoo.com";

   //TIdEMailAddressList *Recipients_list = new TIdEMailAddressList[10];

   IdMessage1->Subject = "MM-4104";
   IdMessage1->Body->Text = "Email Done.......";

   TIdMessageBuilderPlain* MyMessage = new TIdMessageBuilderPlain();
   MyMessage->Attachments->Add("C:/2015-06-01-POS3.txt");
   MyMessage->FillMessage(IdMessage1);
   try
   {
      IdSMTP1->Connect();
      IdSMTP1->Send(IdMessage1);
   }
   catch(Exception &e)
   {
     ShowMessage("Error" +e.Message);
   }
   IdSMTP1->Disconnect();
   delete MyMessage;
}

AnsiString get_filename(AnsiString& filename )
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


