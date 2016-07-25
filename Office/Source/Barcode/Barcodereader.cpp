//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Barcodereader.h"

#define Max_Bytes 255
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TMMBarcodeReader::TMMBarcodeReader()
{
	Port											= new TComPort(NULL);
	Port->BaudRate								= br9600;
	Port->DataBits								= dbEight;
	Port->Parity->Bits						= prNone;
	Port->StopBits								= sbOneStopBit;
	Port->Parity->Check						= false;
	Port->Parity->Replace					= false;
	Port->Buffer->InputSize					= 16384;
	Port->Buffer->OutputSize				= 16384;
	Port->DiscardNull							= false;
	Port->OnRxChar								= GetData;
	Port->Timeouts->ReadInterval			= 200;
	Port->Timeouts->ReadTotalMultiplier	= 200;
	Port->Timeouts->ReadTotalConstant	= 300;
	CloseingPort								= false;
	OpeningPort									= false;
	Busy											= false;
}
//---------------------------------------------------------------------------
TMMBarcodeReader::~TMMBarcodeReader()
{
	delete Port;
}
//---------------------------------------------------------------------------
bool TMMBarcodeReader::Open(int PortNum)
{
	bool Success = false;

	if (!OpeningPort)
	{
		OpeningPort = true;
		try
		{
			if(PortNum != -1)
			{
				if(Port->Connected)
				{
					Port->Close();
				}
				Port->Port = "COM" + IntToStr(PortNum);
				Port->Open();
				Success = true;
			}
			else
			{
				if(Port->Connected)
				{
					Port->Close();
				}
			}
		}
		__finally
		{
			OpeningPort = false;
		}
	}
	return Success;
}
//---------------------------------------------------------------------------
void TMMBarcodeReader::Close()
{
	if(CloseingPort) return;
	CloseingPort = true;
	try
	{
		Port->Close();
	}
	__finally
	{
		CloseingPort = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMMBarcodeReader::GetData(TObject *Sender,int count)
{
	try
	{
		try
		{
			Port->ClearBuffer(false, true);
			int CurrentTime, TimeOut;
			unsigned char TempBuffer[Max_Bytes];
			ZeroMemory(TempBuffer, Max_Bytes);
			int BytesRead	= Port->Read(TempBuffer, Max_Bytes);
			LastBarCode		= AnsiString((const char *)(TempBuffer), BytesRead);
			LastBarCode		= LastBarCode.Trim();
//			PostMessage(Screen->ActiveForm->Handle, UWM_CARDSWIPE, (UINT)LastBarCode.c_str(), NULL);
			PostMessage(Screen->ActiveForm->Handle, UWM_CARDSWIPE, (UINT)&LastBarCode, NULL);
		}
		__finally
		{
			Port->ClearBuffer(true, false);
		}
	}
	catch(Exception &E)
	{
		Port->ClearBuffer(true, true);
		throw;
	}
}
//---------------------------------------------------------------------------

