//---------------------------------------------------------------------------

#ifndef FiscalPortH
#define FiscalPortH
#include <CPort.hpp>
#include <list>
#include "Processing.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
enum FiscalRequestType { eFiscalNone, eFiscalValidate, eFiscalReceipt, eFiscalCopy, eFiscalPractice, eFiscalProof };
class TFiscalPort
	{
		private:
		TComPort *Port;
			bool Busy;
			bool ClosingPort;
			bool OpeningPort;

			void __fastcall GetData(TObject *Sender,int Count);
         void __fastcall PortTxEmpty(TObject *Sender);
      	std::list<PAsync> writeAsyncPtrList;
        bool AnalyzeData(AnsiString Response);
        bool CheckValidationResponse(AnsiString inData);
        bool CheckPostResponse(AnsiString inData);
		public:
			TFiscalPort();
			virtual ~TFiscalPort();
            AnsiString LastError;
			int PortNumber;
            bool AsyncMode;
			UnicodeString LastData;
            FiscalRequestType CurrentRequestType;
			AnsiString SetFiscalData(AnsiString Data, FiscalRequestType requestType);
			bool Open(int PortNum);
			void Close();
            AnsiString GetFiscalData();
         TComPort *GetTComPort();
         void Initialise(Database::TDBTransaction &DBTransaction);
         void SaveSettings(Database::TDBTransaction &DBTransaction);
	};
#endif
