//---------------------------------------------------------------------------

#ifndef MMTablePickerConnectorServerH
#define MMTablePickerConnectorServerH
#include <vcl.h>
#include <vector>

#include "MMConnectorServer.h"

//---------------------------------------------------------------------------

class TMMProxyServer;

//:::::::::::::::::::::::::::
//   TMMTablePickerConnectorServer
//:::::::::::::::::::::::::::

// Sent to client
const EV_TP_CURRENT_TABLES = 0x1001;

// Received from client
const EV_TP_SELECTED_TABLE  = 0x1001;

struct TLocationStatus;
struct TFPTableData;

typedef void __fastcall (__closure *TSelectedTableEvent)(System::TObject* Sender, TFPTableData* inData );

class TMMTablePickerConnectorServer : public TMMConnectorServer
{
protected:
	__fastcall  TMMTablePickerConnectorServer();

public:
	__fastcall ~TMMTablePickerConnectorServer();

protected:
	TSelectedTableEvent FOnSelectedTableEvent;

	void blockFromVector( std::vector<TLocationStatus> inCurrentTables, unsigned __int32* &outBlock, __int32& outCount );

	bool processMessage( Messages::TMessage& Msg );
	void processSelectedTable( int inAction );

	TFPTableData* readSelectedTable( int inAction );

public:
	friend TMMProxyServer;

	bool InitTablePicker( unsigned __int32 inMaxLocations, unsigned __int32 inMaxTablesInLocation );

	bool WillHandle( tagMSG &inMsg );

	void ShowTablePicker( std::vector<TLocationStatus> inCurrentTables );

	__property TSelectedTableEvent OnSelectedTableEvent = { read=FOnSelectedTableEvent, write=FOnSelectedTableEvent };
};

//----------------------------------------------------------------------------

#endif
