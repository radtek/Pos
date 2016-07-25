//---------------------------------------------------------------------------

#ifndef SecurityPortH
#define SecurityPortH

#include <CPort.hpp>
#include <list>

#include "MM_DBCore.h"
//---------------------------------------------------------------------------
class TSecurityPort
	{
		private:
		TComPort *Port;
			bool Busy;
			bool CloseingPort;
			bool OpeningPort;

			void __fastcall GetData(TObject *Sender,int Count);
         void __fastcall PortTxEmpty(TObject *Sender);
      	std::list<PAsync> writeAsyncPtrList;
		public:
			TSecurityPort();
			virtual ~TSecurityPort();

			int PortNumber;
         bool AsyncMode;
			UnicodeString LastData;

			void __fastcall SetData(AnsiString Data);
			bool Open(int PortNum);
			void Close();
         TComPort *GetTComPort();
         void Initialise(Database::TDBTransaction &DBTransaction);
         void SaveSettings(Database::TDBTransaction &DBTransaction);
	};
#endif
