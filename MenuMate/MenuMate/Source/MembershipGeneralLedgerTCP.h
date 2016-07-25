//---------------------------------------------------------------------------

#ifndef MembershipGeneralLedgerTCPH
#define MembershipGeneralLedgerTCPH

#include <IdTCPClient.hpp>
#include <memory>
#include <tinyxml.h>
#include "MembershipExternalXML.h"
//---------------------------------------------------------------------------
#define defaultConnectTimeOut       1000
#define defaultReadTimeOut          60000
#define defaultRetryCount           3
#define defaultSleepOnException     300

class TMembershipGeneralLedgerTCP
{
	private:

	int ConnectTimeOut;
	int ReadTimeOut;
	int RetryCount;
   int ExceptSleepTime;

	void SendAndFetch(TMSXMLBase &Packet, AnsiString HostAddress, int Port);
   public :
      TMembershipGeneralLedgerTCP();

		void GetStatus(AnsiString IP, int Port, int ReadTimeOut);
      AnsiString ConnectionStatus;
      bool OffLine;

	   void Process(AnsiString inIP, int inPort,TMSXMLBase &XML);
      void TimeOuts(int inConnectTimeOut,int inReadTimeOut,int inRetryCount, int inSleepOnException);
};

#endif
