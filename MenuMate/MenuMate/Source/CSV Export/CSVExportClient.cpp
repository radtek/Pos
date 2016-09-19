//---------------------------------------------------------------------------

#pragma hdrstop

#include "CSVExportClient.h"

#include <map>
#include "CSV.h"
#include "GlobalSettings.h"
//--------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TCSVExportClient::~TCSVExportClient()
{

}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void TCSVExportClient::SendPointDetailsToCSVExport ( UnicodeString csvPath  )
{
    std::auto_ptr<TCSVExportTcpStreamSender> streamSender( new TCSVExportTcpStreamSender(TGlobalSettings::Instance().CSVExportIP, 47001 ) );

        UnicodeString file = ExtractFileName( csvPath );
        TCsv Csv;
        if (FileExists(csvPath))
        {
            Csv.LoadFromFile(csvPath);
        }
        streamSender->Send(Csv.fStrings);
}
//---------------------------------------------------------------------------
