//---------------------------------------------------------------------------

#ifndef MMFloorPlanConnectorServerH
#define MMFloorPlanConnectorServerH

#include <vcl.h>
#include <vector>

#include "MMConnectorServer.h"

//---------------------------------------------------------------------------

class TMMProxyServer;

//:::::::::::::::::::::::::::
//   TMMFloorPlanConnectorServer
//:::::::::::::::::::::::::::

// Sent to client
const EV_FP_CURRENT_TABLES = 0x0001;

// Received from client
const EV_FP_UPDATED_TABLES  = 0x0001;

struct TLocationStatus;
struct TTableStatus;

typedef void __fastcall (__closure *TUpdatedTablesEvent)(System::TObject* Sender, TLocationStatus* inUpdatedTables, int inCount );

class TMMFloorPlanConnectorServer : public TMMConnectorServer
{
protected:
	__fastcall  TMMFloorPlanConnectorServer();

public:
	__fastcall ~TMMFloorPlanConnectorServer();

protected:
	TUpdatedTablesEvent FOnUpdatedTablesEvent;

	void blockFromVector( std::vector<TLocationStatus> inCurrentTables, unsigned __int32* &outBlock, __int32& outCount );

	UnicodeString clientExecutableName();
	UnicodeString clientExecutableParam();

	bool processMessage( Messages::TMessage& Msg );
	void processUpdatedTables();

	void readUpdatedTables( std::vector<TLocationStatus> &inUpdatedTables );
	void vectorFromBlock( unsigned __int32* inBlock, std::vector<TLocationStatus> &outUpdatedTables );
	TLocationStatus* vectorToArray( std::vector<TLocationStatus> inUpdatedTables );

public:
	friend TMMProxyServer;

	bool InitFloorPlan( unsigned __int32 inMaxLocations, unsigned __int32 inMaxTablesInLocation );

	bool WillHandle( tagMSG &inMsg );

	void ShowFloorPlan( std::vector<TLocationStatus> inCurrentTables );

	__property TUpdatedTablesEvent OnUpdatedTablesEvent = { read=FOnUpdatedTablesEvent, write=FOnUpdatedTablesEvent };
};

//----------------------------------------------------------------------------

#endif
