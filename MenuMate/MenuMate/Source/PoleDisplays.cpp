//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PoleDisplays.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


void TPoleDisplayTypeGeneric::UpdatePoleDisplay(AnsiString TopLine,AnsiString BottomLine, TComPort *PoleDisplayPort)
{
	// This Code is Compatable with a Birch 20 char two line pole display.
	TMemoryStream *Stream = new TMemoryStream;
	try
	{
		TopLine += '\r';
		BottomLine += '\r';

		Stream->Clear();
		Stream->Position = 0;
		Stream->Write(TopLine.c_str(),TopLine.Length());
		Stream->Position = 0;
		PoleDisplayPort->Write((unsigned char *)(Stream->Memory),Stream->Size);

		Stream->Clear();

		Stream->Position = 0;
		Stream->Write(BottomLine.c_str(),BottomLine.Length());
		Stream->Position = 0;
		PoleDisplayPort->Write((unsigned char *)(Stream->Memory),Stream->Size);
	}
	__finally
	{
		delete Stream;
	}
}

void TPoleDisplayTypeBirch::UpdatePoleDisplay(AnsiString TopLine,AnsiString BottomLine,TComPort *PoleDisplayPort)
{
	// This Code is Compatable with a Birch 20 char two line pole display.
	TMemoryStream *Stream = new TMemoryStream;
	try
	{
		AnsiString CmdTopLn = "";
		AnsiString CmdBotLn = "";

		CmdTopLn = AnsiString(char(0x1B)) + "QA";
		CmdBotLn = AnsiString(char(0x1B)) + "QB";

		TopLine += '\r';
		BottomLine += '\r';

		Stream->Clear();
		Stream->Position = 0;
		Stream->Write(CmdTopLn.c_str(),CmdTopLn.Length());
		Stream->Write(TopLine.c_str(),TopLine.Length());
		Stream->Position = 0; PoleDisplayPort->Write((unsigned char *)(Stream->Memory),Stream->Size);
		Stream->Clear();
		Stream->Position = 0;
		Stream->Write(CmdBotLn.c_str(),CmdBotLn.Length());
		Stream->Write(BottomLine.c_str(),BottomLine.Length());
		Stream->Position = 0; PoleDisplayPort->Write((unsigned char *)(Stream->Memory),Stream->Size);
		Stream->Clear();
	}
	__finally
	{
		delete Stream;
	}
}

void TPoleDisplayTypeDSP810::UpdatePoleDisplay(AnsiString TopLine,AnsiString BottomLine,TComPort *PoleDisplayPort)
{
	// This Code is Compatable with a Birch 20 char two line pole display.
	TMemoryStream *Stream = new TMemoryStream;
	try
	{
		//char charCmdInit[] = {0x04,0x01,0x1B,'@',0x17};
		char charCmdClr[] = {0x04,0x01,'C',0x31,0x58,0x17};
		char charSelectCmdTopLn[] = {0x04,0x01,'P',0x31,0x17};
		char charSelectCmdBotLn[] = {0x04,0x01,'P',0x45,0x17};

      Stream->Clear();
		Stream->Position = 0;
		Stream->Write(&charCmdClr,sizeof(charCmdClr));
		Stream->Write(&charSelectCmdTopLn,sizeof(charSelectCmdTopLn));
		Stream->Write(TopLine.c_str(),TopLine.Length());
		Stream->Write(BottomLine.c_str(),BottomLine.Length());
		Stream->Position = 0; PoleDisplayPort->Write((unsigned char *)(Stream->Memory),Stream->Size); Stream->Clear();
	}
	__finally
	{
		delete Stream;
	}
}

void TPoleDisplayTypeDSP800F::UpdatePoleDisplay(AnsiString TopLine,AnsiString BottomLine,TComPort *PoleDisplayPort)
{
	// This Code is Compatable with a Birch 20 char two line pole display.
	TMemoryStream *Stream = new TMemoryStream;
	try
	{
		char charCmdInit[] = {0x1B,0x40};
		char charCmdMode[] = {0x1B,0x45,0x58,0x34};
		char charSelectCmdTopLn[] = {0x1B,0x55};
		char charSelectCmdBotLn[] = {0x1B,0x44};

		Stream->Clear();
		Stream->Position = 0;
		Stream->Write(&charCmdInit,sizeof(charCmdInit));
		Stream->Write(&charCmdMode,sizeof(charCmdMode));
		Stream->Write(&charSelectCmdTopLn,sizeof(charSelectCmdTopLn));
		Stream->Write(TopLine.c_str(),TopLine.Length());
		Stream->Write(&charSelectCmdBotLn,sizeof(charSelectCmdBotLn));
		Stream->Write(BottomLine.c_str(),BottomLine.Length());
		Stream->Position = 0; PoleDisplayPort->Write((unsigned char *)(Stream->Memory),Stream->Size); Stream->Clear();
	}
	__finally
	{
		delete Stream;
	}
}

