//---------------------------------------------------------------------------

#ifndef XeroServiceManagerH
#define XeroServiceManagerH
//---------------------------------------------------------------------------

#include "ServiceManager.h"

class TXeroServiceManager : public TServiceManager
{
	public:
		static TXeroServiceManager& Instance()
		{
			static TXeroServiceManager singleton;
			return singleton;
		}

		TXeroServiceManager();

		bool IsServiceInstalled();
		bool IsServiceRunning();
		bool IsServiceStopped();
		bool IsServicePaused();

	protected:
		AnsiString _serviceName;
};
//---------------------------------------------------------------------------
#endif
