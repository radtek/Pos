//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <SysUtils.hpp>
#include <tchar.h>
#include <memory>
#include <vcl\Clipbrd.hpp>

//---------------------------------------------------------------------------

#pragma argsused
int _tmain(int argc, _TCHAR* argv[])
{
	UnicodeString Indent = "  ";
	char Quote = '"';
	char EndChar[] = ";";

	 TClipboard *cb = Clipboard();

	if(cb->HasFormat( CF_TEXT ) )
	{
		std::auto_ptr<TStringList> Data(new TStringList);
		Data->Text = cb->AsText;
		for (int i = 0; i < Data->Count; i++) {
			Data->Strings[i] = Indent +  AnsiQuotedStr( TrimRight(Data->Strings[i]) + " ", Quote ) + ((i == Data->Count-1) ? ";" : "");
		}
		cb->SetTextBuf(Data->Text.c_str());
	}
}
//---------------------------------------------------------------------------
