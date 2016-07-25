//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMSecondDisplayConnectorServer.h"

#include <process.h>

#include "MMProxy.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::
//   TMMSecondDisplayConnectorServer
//:::::::::::::::::::::::::::
__fastcall  TMMSecondDisplayConnectorServer::TMMSecondDisplayConnectorServer()
{
}
//...........................................................................

__fastcall TMMSecondDisplayConnectorServer::~TMMSecondDisplayConnectorServer()
{

}
//...........................................................................

bool TMMSecondDisplayConnectorServer::InitSecondDisplay(
             unsigned __int32 inMaxItemsCount,
             unsigned __int32 inMaxOptionsCount.
             unsigned __int32 inMaxSidesCount )
{
	int unitSize = sizeof( unsigned __int32 );

    //TODO: serverCapacity and clientCapacity
	//__int64 serverCapacity = unitSize + ( inMaxLocations * unitSize ) + ( inMaxLocations * ( inMaxTablesInLocation * unitSize ) );
	//__int64 clientCapacity = unitSize + ( inMaxLocations * unitSize ) + ( inMaxLocations * ( inMaxTablesInLocation * unitSize ) );

	return Init( "MMSecondDisplayNZ", serverCapacity, clientCapacity );
}
//...........................................................................

bool TMMSecondDisplayConnectorServer::WillHandle( tagMSG &inMsg )
{
    return false;
//	return TMMConnectorServer::WillHandle( inMsg ) ||
//			( ( inMsg.message == WM_MM_NEW_CLIENT_DATA ) && ( (inMsg.wParam == EV_SD_ORDER_ITEMS ) ) );
}
//...........................................................................

void TMMSecondDisplayConnectorServer::ShowSecondDisplay( std::vector<TItemOrders> inItemOrders )
{
			 __int32  count;
	unsigned __int32 *block;

	blockFromVector( inItemOrders, block, count );

	if( count > 0 )
	{
		_sharedMemoryController->WriteBlock( _initPosition, count, block );
		delete[] block;
	}

	signalClient( WM_MM_NEW_SERVER_DATA, EV_SD_ORDER_ITEMS, 0 );
}
//...........................................................................

void TMMSecondDisplayConnectorServer::blockFromVector(
                std::vector<TOrderItems> inOrderItems,
                       unsigned __int32* &outBlock,
                                __int32&  outCount )
{
    //TODO: blockFromVector
/*
	outCount = 1 + 1 + ( 2 * inCurrentTables.size() ); // [BlockSize] + [LocationCount] + ( LocationCount * ( [Location] + [TableCount] ) )

	std::vector<TLocationStatus>::iterator locationIT;
	std::vector<TTableStatus>::iterator    tableIT;

	for( locationIT = inCurrentTables.begin(); locationIT < inCurrentTables.end(); locationIT++ )
	{
		outCount +=  ( *locationIT ).Tables.size();
	}

	//...................................................

	// block = [BlockSize][LocationCount][Location0][TableCount][Table0]...[TableN-1]...[LocationM-1][TableCount][Table0]...[TableN-1]

	outBlock      = new unsigned __int32[outCount];
	int blockIndex = 0;

	outBlock[0] = outCount * sizeof( unsigned __int32 ); // [BlockSize]
	outBlock[1] = inCurrentTables.size();                // [LocationCount]

	for( locationIT = inCurrentTables.begin(); locationIT < inCurrentTables.end(); locationIT++ )
	{
		std::vector<TTableStatus> tables = ( *locationIT ).Tables;

		outBlock[blockIndex++] = ( *locationIT ).Location;         // [Location]
		outBlock[blockIndex++] = tables.size();                    // [TableCount]

		for( tableIT = tables.begin(); tableIT < tables.end(); tableIT++ )
		{
		   outBlock[blockIndex++] = tables.size();                  // [Table]
        }
	}
*/
}
//...........................................................................

bool TMMSecondDisplayConnectorServer::processMessage( TMessage& Msg )
{
    return true;

/*
 	bool result = TMMConnectorServer::processMessage( Msg );

	//:::::::::::::::::::::::::::::::::::::::

	if( !result )
	{
		if( Msg.Msg == WM_MM_NEW_CLIENT_DATA )
		{
			switch( Msg.WParam )
			{
				case EV_FP_UPDATED_TABLES:
				{
					processUpdatedTables();
					result = true;
				} break;
			}
		}
	}

	//:::::::::::::::::::::::::::::::::::::::

	return result;
*/
}
//...........................................................................

/*
void TMMSecondDisplayConnectorServer::processUpdatedTables()
{
	if( FOnUpdatedTablesEvent != NULL )
	{
		std::vector<TLocationStatus> updatedTables;

		readUpdatedTables( updatedTables );
		FOnUpdatedTablesEvent( this, vectorToArray( updatedTables ), updatedTables.size() );

		updatedTables.clear();
	}
}
//...........................................................................

void TMMSecondDisplayConnectorServer::readUpdatedTables( std::vector<TLocationStatus> &outUpdatedTables )
{
	// block = [BlockSize][LocationCount][Location0][TableCount][Table0]...[TableN-1]...[LocationM-1][TableCount][Table0]...[TableN-1]

	unsigned __int32  blockSize = _sharedMemoryController->ReadUInt32( _initPosition );
	unsigned __int32 *block     = new unsigned __int32[blockSize];
	unsigned __int32  count     = blockSize / sizeof( unsigned __int32 );

	_sharedMemoryController->ReadBlock( _initPosition, count, block );

	vectorFromBlock( block, outUpdatedTables );
}
//...........................................................................

void TMMSecondDisplayConnectorServer::vectorFromBlock( unsigned __int32* inBlock, std::vector<TLocationStatus> &outUpdatedTables )
{
	// block = [BlockSize][LocationCount][Location0][TableCount][Table0]...[TableN-1]...[LocationM-1][TableCount][Table0]...[TableN-1]

	unsigned __int32 blockIndex    = 1;
	unsigned __int32 locationCount = inBlock[blockIndex++];  		//[LocationCount]

	TLocationStatus  locationData;

	for( int i = 0; i < locationCount; i++ )
	{
		locationData.Location       = inBlock[blockIndex++]; 		//[Location]
		unsigned __int32 tableCount = inBlock[blockIndex++]; 		//[TableCount]

		locationData.Tables.clear();

		for( int j = 0; j < tableCount; j++ )
		{
			TTableStatus *tabledata      = new TTableStatus();
			tabledata->Number            = inBlock[blockIndex++];
			//tabledata->Occupied          = inBlock[blockIndex++];
			//tabledata->LastServingCourse = inBlock[blockIndex++];

			locationData.Tables.push_back( *tabledata ); //[Table]
		}

		outUpdatedTables.push_back( locationData );
	}
}
//...........................................................................

TLocationStatus* TMMSecondDisplayConnectorServer::vectorToArray( std::vector<TLocationStatus> inUpdatedTables )
{
	int count = inUpdatedTables.size();
	TLocationStatus* result = new TLocationStatus[count];

	int i = 0;
	std::vector<TLocationStatus>::iterator locationIT;
	for( locationIT = inUpdatedTables.begin(); locationIT < inUpdatedTables.end(); locationIT++ )
	{
		result[i++] = *locationIT;
	}

	return result;
}
//...........................................................................
*/
