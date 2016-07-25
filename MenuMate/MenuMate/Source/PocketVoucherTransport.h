//---------------------------------------------------------------------------

#ifndef PocketVoucherTransportH
#define PocketVoucherTransportH
//---------------------------------------------------------------------------

#include <IdTCPClient.hpp>
#include <memory>
#include <tinyxml.h>
#include "PocketMateXML.h"
#include "IdHTTP.hpp"

//---------------------------------------------------------------------------
#define defaultConnectTimeOut       1000
#define defaultReadTimeOut          60000
#define defaultRetryCount           3
#define defaultSleepOnException     300


class IXML_Transport
{
   public :
      virtual void Process(TXMLBase &doc) = 0;
};

class TXML_Transport_TCPIP
{
   private:
   int ConnectTimeOut;
   int ReadTimeOut;
   int SleepOnException;
   int RetryCount;

   void SendAndFetch(TXMLBase &Packet, AnsiString HostAddress, int Port);
   public :
      TXML_Transport_TCPIP();

      AnsiString ConnectionStatus;
      bool OffLine;

      void Process(AnsiString inIP, int inPort,TXMLBase &POSXML);
      void TimeOuts(int inConnectTimeOut,int inReadTimeOut,int inRetryCount, int inSleepOnException);
};

class TXML_Transport_Http
{
   private:
   int ConnectTimeOut;
   int ReadTimeOut;
   int SleepOnException;
   int RetryCount;

   void SendAndFetch(TXMLBase &Packet, AnsiString HostAddress, int Port);
   public :
      TXML_Transport_Http();

      AnsiString ConnectionStatus;
      bool OffLine;

      void Process(AnsiString inIP, int inPort,TXMLBase &POSXML);
      void TimeOuts(int inConnectTimeOut,int inReadTimeOut,int inRetryCount, int inSleepOnException);
};

#endif

