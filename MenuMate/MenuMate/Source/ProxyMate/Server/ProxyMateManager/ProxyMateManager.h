//---------------------------------------------------------------------------

#ifndef ProxyMateManagerH
#define ProxyMateManagerH

//---------------------------------------------------------------------------

#include "MMAppConnectorServer.h"
#include "MMFloorPlanConnectorServer.h"
#include "MMTablePickerConnectorServer.h"

//:::::::::::::::::::::::::::
//   TProxyMateManager
//:::::::::::::::::::::::::::
class TProxyMateManager
{
protected:
					 bool          FClientUp;
	TMMAppConnectorServer*         FAppConnectorServer;
	TMMFloorPlanConnectorServer*   FFPConnector;
	TMMTablePickerConnectorServer* FTPConnector;

	bool initialized();

	void __fastcall mmAppConnectorClientUp(   TObject* Sender );
	void __fastcall mmAppConnectorClientDown( TObject* Sender );

public:
	static TProxyMateManager& Instance()
	{
		static TProxyMateManager s_instance;

		return s_instance;
	}

	TProxyMateManager();
	~TProxyMateManager();

	__property bool                           ClientUp           = { read=FClientUp           };
	__property TMMAppConnectorServer*         AppConnectorServer = { read=FAppConnectorServer };
	__property TMMFloorPlanConnectorServer*   FPConnector        = { read=FFPConnector        };
	__property TMMTablePickerConnectorServer* TPConnector        = { read=FTPConnector        };

	bool Init();
};

//---------------------------------------------------------------------------

#endif
