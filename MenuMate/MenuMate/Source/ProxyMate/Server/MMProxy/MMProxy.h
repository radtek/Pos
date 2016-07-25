//---------------------------------------------------------------------------

#ifndef MMProxyH
#define MMProxyH

//---------------------------------------------------------------------------

#include <vcl.h>

#include "MMConnectorServer.h"

//---------------------------------------------------------------------------

extern UnicodeString MenumatePath;

struct TTableStatus
{
	unsigned __int32 Number;            // Table Number
	bool             Occupied;          // Occupied
	unsigned __int8  LastServingCourse; // Last Serving Course:
										// 	Entree  = 1
										//	Mains   = 2
										//	Dessert = 3
										//	Billed  = 4
};

struct TLocationStatus
{
		 unsigned __int32 Location;   // Location DB's ID
	std::vector<TTableStatus> Tables; // Table Number
};

struct TFPTableData
{
	unsigned __int32 Location;  // Location DB's ID
	unsigned __int32 Table;     // Table Number
	UnicodeString    TableName; // Table Name
	unsigned __int8  Action;    // Action
								// Selected = 1
								// To Bill = 2
								// To Edit = 3
								// To View Details = 4
							    // To Transfer data = 5

};

const _MAX_SERVER_CONNECTORS = 1;

enum TMMConnectorTypes
{
   mmcFloorPlan,
   mmcTablePicker
};

//:::::::::::::::::::::::::::
//   TMMProxyServer
//:::::::::::::::::::::::::::
class TMMProxyServer
{
public:
	TMMProxyServer();
	~TMMProxyServer();

	static TMMProxyServer& Instance()
	{
		static TMMProxyServer s_instance;

		return s_instance;
	}

	TMMConnectorServer* GetConnector( TMMConnectorTypes inConnectorType);

protected:
	std::vector<TMMConnectorServer*> _connectors;
};

//---------------------------------------------------------------------------

#endif
