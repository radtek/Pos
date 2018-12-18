//---------------------------------------------------------------------------
#ifndef SendEmailH
#define SendEmailH
#include <System.hpp>
#include <Forms.hpp>
#include <mapi.hpp>
#include <string.h>
#include <vector.h>


#include "GlobalSettings.h"

#include <IdMessageBuilder.hpp>
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdExplicitTLSClientServerBase.hpp"
#include "IdIOHandler.hpp"
#include "IdIOHandlerSocket.hpp"
#include "IdIOHandlerStack.hpp"
#include "IdMessage.hpp"
#include "IdMessageClient.hpp"
#include "IdSMTP.hpp"
#include "IdSMTPBase.hpp"
#include "IdSSL.hpp"
#include "IdSSLOpenSSL.hpp"
#include "IdTCPClient.hpp"
#include "IdTCPConnection.hpp"
#include "IdAttachmentFile.hpp"




//---------------------------------------------------------------------------
//namespace System { class AnsiString; }
//---------------------------------------------------------------------------
class SendEmail
{
	public:

    static bool Send(const AnsiString& attachmentFileName, const AnsiString& subject,  AnsiString& mailTo, const AnsiString& body, bool isLoyaltyEmail = false);
    static bool Send(const AnsiString& attachmentFileName, const AnsiString& subject,  AnsiString& mailTo, const AnsiString& mailToName, const AnsiString& body, bool isLoyaltyEmail);
    AnsiString Get_FileName(AnsiString& filename );

};
#endif
