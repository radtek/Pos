//---------------------------------------------------------------------------

#ifndef IMManagerH
#define IMManagerH
//---------------------------------------------------------------------------
#include "PaymentTransaction.h"
#include "IntaMateTransport.h"
#include "POS_XMLBase.h"

class TIMManager
{
   private:
		AnsiString IP;
		int        Port;
		int        ReadTimeOut;
		TIM_Transport_Cache *IM_Cache;
	public:
		int  POSID;
		bool Registered;
		bool Enabled;
		bool OffLine;
		AnsiString LastErrorMessage;
		TIMManager();
		~TIMManager();
		void Initialise(AnsiString inIP, int inPort, int inPosID,int inTimeOut);
      void Export(TPaymentTransaction &PaymentTransaction);
      void Export(TPOS_XMLBase &Data);
      void Status();
};

#endif




