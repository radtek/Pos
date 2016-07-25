//---------------------------------------------------------------------------

#ifndef ProxReaderH
#define ProxReaderH

#include <CPort.hpp>

#include "MM_DBCore.h"
//---------------------------------------------------------------------------
enum eProxFormat { eproxStd = 0, eproxWiegand = 1 };

class TProxReader
{
		private:
		TComPort *Port;
			bool Busy;
			bool CloseingPort;
			bool OpeningPort;
			void __fastcall GetData(TObject *Sender,int Count);
		public:
			AnsiString LastCard;
			int PortNumber;
         bool AsyncMode;
			TProxReader();
			virtual ~TProxReader();
			bool Open(int PortNum);
         eProxFormat ProxFormat;
			void Close();
         TComPort *GetTComPort();
         //TODO : Remove the DB Dependancy from this class.
         void Initialise(Database::TDBTransaction &DBTransaction);
         void SaveSettings(Database::TDBTransaction &DBTransaction);
};
#endif
