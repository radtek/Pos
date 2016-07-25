//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMTablePickerConnectorServer.h"
#include <process.h>

#include "MMProxy.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::
//   TMMTablePickerConnectorServer
//:::::::::::::::::::::::::::
__fastcall  TMMTablePickerConnectorServer::TMMTablePickerConnectorServer()
{
}
//...........................................................................

__fastcall TMMTablePickerConnectorServer::~TMMTablePickerConnectorServer()
{

}
//...........................................................................

bool TMMTablePickerConnectorServer::InitTablePicker( unsigned __int32 inMaxLocations, unsigned __int32 inMaxTablesInLocation )
{
	int unitSize = sizeof( unsigned __int32 );

	__int64 serverCapacity = unitSize + ( inMaxLocations * unitSize ) + ( inMaxLocations * ( inMaxTablesInLocation * unitSize ) );
	__int64 clientCapacity = unitSize + ( 2 * unitSize );

	return Init( "MMTablePickerNZ", serverCapacity, clientCapacity );
}
//...........................................................................

bool TMMTablePickerConnectorServer::WillHandle( tagMSG &inMsg )
{
	return TMMConnectorServer::WillHandle( inMsg ) ||
			( ( inMsg.message == WM_MM_NEW_CLIENT_DATA ) && ( (inMsg.wParam == EV_TP_SELECTED_TABLE ) ) );
}
//...........................................................................

void TMMTablePickerConnectorServer::ShowTablePicker( std::vector<TLocationStatus> inCurrentTables )
{
			 __int32  count;
	unsigned __int32 *block;

	blockFromVector( inCurrentTables, block, count );

	_sharedMemoryController->Write( _initPosition, count );
	if( count > 0 )
	{
		//_sharedMemoryController->WriteBlock( _initPosition + sizeof( __int32 ), count, block );
		_sharedMemoryController->WriteBlock( _initPosition, count, block );
		delete[] block;
	}

	signalClient( WM_MM_NEW_SERVER_DATA, EV_TP_CURRENT_TABLES, 0 );
}
//...........................................................................

void TMMTablePickerConnectorServer::blockFromVector( std::vector<TLocationStatus> inCurrentTables, unsigned __int32* &outBlock, __int32& outCount )
{
	outCount = 1 + 1 + ( 2 * inCurrentTables.size() ); // [BlockSize] + [LocationCount] + ( LocationCount * ( [Location] + [TableCount] ) )

	std::vector<TLocationStatus>::iterator locationIT;
	std::vector<TTableStatus>::iterator    tableIT;

	for( locationIT = inCurrentTables.begin(); locationIT < inCurrentTables.end(); locationIT++ )
	{
		outCount += 3 * ( *locationIT ).Tables.size();  // TableNumber, Occupied, LastServingCourse
	}

	//...................................................

	// block = [BlockSize][LocationCount][Location0][TableCount][TableName0][Ocuppied0][LastServingCourse0]...[TableN-1]...[LocationM-1][TableCount][TableName0][Ocuppied0][LastServingCourse0]...[TableNameN-1][OcuppiedN-1][LastServingCourseN-1]...

	outBlock       = new unsigned __int32[outCount];
	int blockIndex = 0;

	outBlock[blockIndex++]  = outCount * sizeof( unsigned __int32 ); // [BlockSize]
	outBlock[blockIndex++]  = inCurrentTables.size();                // [LocationCount]

	for( locationIT = inCurrentTables.begin(); locationIT < inCurrentTables.end(); locationIT++ )
	{
		std::vector<TTableStatus> tables = ( *locationIT ).Tables;

		outBlock[blockIndex++] = ( *locationIT ).Location;         // [Location]
		outBlock[blockIndex++] = tables.size();                    // [TableCount]

		for( tableIT = tables.begin(); tableIT < tables.end(); tableIT++ )
		{
		   outBlock[blockIndex++] = ( *tableIT ).Number;            // [TableNumber]
		   outBlock[blockIndex++] = ( *tableIT ).Occupied;          // [Occupied]
		   outBlock[blockIndex++] = ( *tableIT ).LastServingCourse; // [LastServingCourse]
        }
	}
}
//...........................................................................

bool TMMTablePickerConnectorServer::processMessage( TMessage& Msg )
{
	bool result = TMMConnectorServer::processMessage( Msg );

	//:::::::::::::::::::::::::::::::::::::::

	if( !result )
	{
		if( Msg.Msg == WM_MM_NEW_CLIENT_DATA )
		{
			switch( Msg.WParam )
			{
				case EV_TP_SELECTED_TABLE:
				{
					processSelectedTable( Msg.LParam );
					result = true;
				} break;
			}
		}
	}

	//:::::::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

void TMMTablePickerConnectorServer::processSelectedTable( int inAction )
{
	if( FOnSelectedTableEvent != NULL )
	{
		FOnSelectedTableEvent( this, readSelectedTable( inAction ) );
	}
}
//...........................................................................

TFPTableData* TMMTablePickerConnectorServer::readSelectedTable( int inAction )
{
	TFPTableData* result = new TFPTableData();

	//::::::::::::::::::::::::::::::::::

	result->Location  = _sharedMemoryController->ReadInt32( _initPosition );
	result->Table     = _sharedMemoryController->ReadInt32( _initPosition + sizeof( unsigned __int32 ) );
	result->TableName = "";
	result->Action    = inAction;

	//::::::::::::::::::::::::::::::::::

	return result;
}
//...........................................................................

