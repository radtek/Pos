//---------------------------------------------------------------------------

#pragma hdrstop

#include "SendEmail.h"
#include <System.hpp>
#include <Forms.hpp>
#include <mapi.hpp>
#include <string.h>

#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

#include <memory>


//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
bool SendEmail::Send(
		const AnsiString& attachmentFileName,
		const AnsiString& subject,
		const AnsiString& mailTo)
{
   AnsiString mailToName = "MM Office Generated";
   Send(attachmentFileName, subject, mailTo, mailToName);
}
//---------------------------------------------------------------------------

bool SendEmail::Send(
		const AnsiString& attachmentFileName,
		const AnsiString& subject,
		const AnsiString& mailTo,
                const System::AnsiString& mailToName)
{
	HINSTANCE hMAPI = ::LoadLibraryA("MAPI32.DLL");
	if (!hMAPI)
		return false;

	ULONG (PASCAL *SendMail)(ULONG, ULONG_PTR, MapiMessage*, FLAGS, ULONG);
		  (FARPROC&)SendMail = GetProcAddress(hMAPI, "MAPISendMail");

	if (!SendMail)
		return false;

	TCHAR szFileName[_MAX_PATH];
	TCHAR szPath[_MAX_PATH];
	TCHAR szSubject[_MAX_PATH];
	TCHAR szRecip[_MAX_PATH];
	strcpy(szFileName, attachmentFileName.c_str());
	strcpy(szPath, attachmentFileName.c_str());
	strcpy(szSubject, subject.c_str());
    strcpy(szRecip, "smtp:");
    strcat(szRecip, mailTo.c_str());

	MapiFileDesc fileDesc;
	::ZeroMemory(&fileDesc, sizeof(fileDesc));
	fileDesc.nPosition = (ULONG)-1;
	fileDesc.lpszPathName = szPath;
	fileDesc.lpszFileName = szFileName;


	MapiRecipDesc recipientInfo;
	::ZeroMemory(&recipientInfo, sizeof(recipientInfo));
	recipientInfo.ulRecipClass = MAPI_TO;
	recipientInfo.lpszName =  mailToName == "" ? "MM Office Generated" : mailToName.c_str();
	recipientInfo.lpszAddress = szRecip;

	MapiMessage message;
	::ZeroMemory(&message, sizeof(message));
	message.lpszSubject = szSubject;
	message.nFileCount = 1;
	message.lpFiles = &fileDesc;
	message.nRecipCount = 1;
	message.lpRecips = &recipientInfo;


	// Ok to send
	int nError = SendMail(0, (ULONG_PTR)Application->Handle,
						&message, MAPI_LOGON_UI|MAPI_DIALOG, 0);

	if (nError != SUCCESS_SUCCESS &&
		 nError != MAPI_USER_ABORT &&
		 nError != MAPI_E_LOGIN_FAILURE)
		return false;


	return true;
}
//---------------------------------------------------------------------------

