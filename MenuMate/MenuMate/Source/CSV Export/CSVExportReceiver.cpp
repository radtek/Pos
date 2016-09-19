// ---------------------------------------------------------------------------

#pragma hdrstop

#include "CSVExportReceiver.h"
#include "ExportCSV.h"

#include <IdContext.hpp>
#include <Forms.hpp>
#include "GlobalSettings.h"
#include <SysUtils.hpp>
#include <IOUtils.hpp>
#include "CSV.h"
#include "DeviceRealTerminal.h"

#pragma package(smart_init)
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdContext"
#pragma link "IdCustomTCPServer"
#pragma link "IdTCPServer"

TCSVExportReceiver::TCSVExportReceiver() : fTCPServer(new TIdTCPServer(NULL))
{
}

TCSVExportReceiver::~TCSVExportReceiver()
{
}

void TCSVExportReceiver::Initialise(UnicodeString inDBIP, UnicodeString inDBPath, int inWebPort)
{
	try
	{
		CSVDir = inDBPath + "\\Accounting Export";

		if (!DirectoryExists(CSVDir))
		{
			ForceDirectories(CSVDir);
		}

	    if (inDBIP != "" && inDBPath != "")
        {
            fTCPServer->DefaultPort = inWebPort;
            fTCPServer->OnExecute = OnExecute;
            fTCPServer->OnConnect = OnConnect;
            fTCPServer->OnDisconnect = OnDisconnect;
            fTCPServer->Active = true;
            TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "CSV Export Enabled.");
        }
        else
        {
            TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "CSV Export Disabled. No DB Path or IP Address");
        }
	}
	catch(Exception & E)
	{
        fTCPServer->Active = false;
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "CSV Export Initialise Failed.");
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

void __fastcall TCSVExportReceiver::OnConnect(TIdContext *AContext)
{
	UnicodeString PeerIP = AContext->Connection->Socket->Binding->PeerIP;
}

void __fastcall TCSVExportReceiver::OnDisconnect(TIdContext *AContext)
{
	UnicodeString PeerIP = AContext->Connection->Socket->Binding->PeerIP;
}

void __fastcall TCSVExportReceiver::OnExecute(TIdContext *AContext)
{
	try
	{
        UnicodeString Format     = ".csv";
        TSearchRec sr;
        int iAttributes = faAnyFile;
        TExportCSV exportCSV;
        UnicodeString TerminalName = TDeviceRealTerminal::Instance().ID.Name;
		UnicodeString PeerIP = AContext->Connection->Socket->Binding->PeerIP;
		AnsiString Data = "";
		AContext->Connection->IOHandler->CheckForDataOnSource(100);
		AContext->Connection->IOHandler->ReadTimeout = 5000;
        TMemoryStream* recStream = new TMemoryStream;
        AContext->Connection->IOHandler->ReadStream(recStream, -1, false);
        if(recStream->Size > 0)
        {
            if( FindFirst( CSVDir + "*.CSV", iAttributes, sr ) == 0 )
            {
                do
                {
                    if (sr.Attr & iAttributes)
                    {
                        exportCSV.MoveCSVFile( CSVDir + sr.Name, CSVDir + "Archive CSV\\" );
                    }
                }
                while(FindNext(sr) == 0);
            }
           AnsiString FileName = CSVDir + "\\" +  TerminalName + Now().FormatString("ddmmyyyyhhnnss") + Format;
           recStream->SaveToFile(FileName);
        }
        delete recStream;
		AContext->Connection->Disconnect();
	}
	catch(Exception & E)
	{
		AContext->Connection->Disconnect();
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}


