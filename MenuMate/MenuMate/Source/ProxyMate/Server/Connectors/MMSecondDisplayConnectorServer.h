//---------------------------------------------------------------------------

#ifndef MMSecondDisplayConnectorServerH
#define MMSecondDisplayConnectorServerH

#include <vcl.h>
#include <vector>

#include "MMConnectorServer.h"

//---------------------------------------------------------------------------

//TODO: Uncomment this is TMMProxyServer is used
//class TMMProxyServer;

//:::::::::::::::::::::::::::
//   TMMFloorPlanConnectorServer
//:::::::::::::::::::::::::::

// Sent to client
const EV_SD_ORDER_ITEMS = 0x0001;

// Received from client
//const event  = 0x0001;

struct TOrderItem;

//typedef void __fastcall (__closure *TUpdatedTablesEvent)(System::TObject* Sender, TLocationStatus* inUpdatedTables, int inCount );

class TMMSecondDisplayConnectorServer : public TMMConnectorServer
{
protected:
	__fastcall  TMMSecondDisplayConnectorServer();

public:
	__fastcall ~TMMSecondDisplayConnectorServer();

protected:
	//TUpdatedTablesEvent FOnUpdatedTablesEvent;

	void blockFromVector( std::vector<TOrderItem> inOrderItems, unsigned __int32* &outBlock, __int32& outCount );

	UnicodeString clientExecutableName();
	UnicodeString clientExecutableParam();

	bool processMessage( Messages::TMessage& Msg );

	//void processUpdatedTables();
	//void readUpdatedTables( std::vector<TLocationStatus> &inUpdatedTables );
	//void vectorFromBlock( unsigned __int32* inBlock, std::vector<TLocationStatus> &outUpdatedTables );
	//TLocationStatus* vectorToArray( std::vector<TLocationStatus> inUpdatedTables );

public:
    //TODO: Uncomment this is TMMProxyServer is used
	//friend TMMProxyServer;

	bool InitSecondEditor(
             unsigned __int32 inMaxItemsCount,
             unsigned __int32 inMaxOptionsCount.
             unsigned __int32 inMaxSidesCount );

	bool WillHandle( tagMSG &inMsg );

	void ShowOrderItems( std::vector<TOrderItem> inOrderItems );

	//__property TUpdatedTablesEvent OnUpdatedTablesEvent = { read=FOnUpdatedTablesEvent, write=FOnUpdatedTablesEvent };
};

//----------------------------------------------------------------------------

#endif
