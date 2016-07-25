//---------------------------------------------------------------------------
#ifndef SendEmailH
#define SendEmailH
//---------------------------------------------------------------------------
namespace System { class AnsiString; }
//---------------------------------------------------------------------------
class SendEmail
{
	public: static bool Send(
		const System::AnsiString& attachmentFileName,
		const System::AnsiString& subject,
		const System::AnsiString& mailTo);

                static bool Send(
		const System::AnsiString& attachmentFileName,
		const System::AnsiString& subject,
		const System::AnsiString& mailTo,const System::AnsiString& mailToName);
};
#endif
