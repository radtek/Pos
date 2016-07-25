//---------------------------------------------------------------------------

#ifndef IntaMateTransportH
#define IntaMateTransportH

#include <IdTCPClient.hpp>
#include <memory>
#include <tinyxml.h>
#include "POS_XMLBase.h"
//---------------------------------------------------------------------------
#define defaultConnectTimeOut       1000
#define defaultReadTimeOut          60000
#define defaultRetryCount           3
#define defaultSleepOnException     300


class IIM_Transport
{
   public :
      virtual void Process(TPOS_XMLBase &doc) = 0;
};

class TIM_Transport_TCPIP
{
   private:
   int ConnectTimeOut;
   int ReadTimeOut;
   int SleepOnException;
   int RetryCount;

   void SendAndFetch(TPOS_XMLBase &Packet, AnsiString HostAddress, int Port);
   public :
      TIM_Transport_TCPIP();

		void GetStatus(AnsiString IP, int Port, int ReadTimeOut);
      AnsiString ConnectionStatus;
      bool OffLine;

      void Process(AnsiString inIP, int inPort,TPOS_XMLBase &POSXML);
      void TimeOuts(int inConnectTimeOut,int inReadTimeOut,int inRetryCount, int inSleepOnException);
};

class TIM_Transport_Failed
{
   void AddFailedRecord(TPOS_XMLBase &POSXML);
   public :
		void Process(TPOS_XMLBase &POSXML,UnicodeString FileName);
};

class TIM_Transport_Accepted
{
	void CleanSuccessfullRecords();
   public :
		void Process(TPOS_XMLBase &POSXML,UnicodeString FileName);
};


class TIM_Transport_Cache : public TThread
{
   void __fastcall TIM_Transport_Cache::Execute();

   void AddCacheRecord(TPOS_XMLBase &POSXML);

   std::auto_ptr<TIM_Transport_Failed> IM_Failed;
   std::auto_ptr<TIM_Transport_Accepted> IM_Accepted;

   public :
		TIM_Transport_Cache(AnsiString inIP,int inPort, int inReadTimeOut);
		__fastcall ~TIM_Transport_Cache();

		std::auto_ptr<TIM_Transport_TCPIP>  IM_TCPIP;
		AnsiString IP;
		int        Port;
		int        ReadTimeOut;
};



#endif
