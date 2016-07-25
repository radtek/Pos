//---------------------------------------------------------------------------

#ifndef EmailH
#define EmailH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
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
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
    TIdSMTP *IdSMTP1;
    TIdSSLIOHandlerSocketOpenSSL *IdSSLIOHandlerSocketOpenSSL1;
    TIdMessage *IdMessage1;
private:	// User declarations
public:		// User declarations
    __fastcall TForm2(TComponent* Owner);

    void SendEmail(const AnsiString& attachmentFileName,
		const AnsiString& subject,
		       AnsiString& mailTo,
                const AnsiString& mailToName);

   AnsiString get_filename(AnsiString& filename );
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
