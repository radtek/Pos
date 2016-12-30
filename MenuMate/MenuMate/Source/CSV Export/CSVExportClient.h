//---------------------------------------------------------------------------

#ifndef CSVExportClientH
#define CSVExportClientH

//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector>

#include "CSVExportTcpStreamSender.h"

//---------------------------------------------------------------------------

class TCSVExportClient
{
public:

    TCSVExportClient(){};
	~TCSVExportClient();

    void SendPointDetailsToCSVExport ( UnicodeString csvPath );
};

//---------------------------------------------------------------------------

#endif
