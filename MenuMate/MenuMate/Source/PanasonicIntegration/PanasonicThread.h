//---------------------------------------------------------------------------

#ifndef PanasonicThreadH
#define PanasonicThreadH
#include <Classes.hpp>
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

class TPanasonicThread : public TThread
{
    private:
        UnicodeString GetMemberName(Database::TDBTransaction &dbTransaction, int contactKey);
        void ConvertTransactionInfoToPanasonicInfo(Database::TDBTransaction &dbTransaction );
        void ConvertTransactionInfoToPanasonicItemList(Database::TDBTransaction &dbTransaction, int arcBill_Key);
        void ConverTransactionInfoToTransactionDBServerInfo(Database::TDBTransaction &dbTransaction);
        UnicodeString GetPOSVersionInfo(Database::TDBTransaction &dbTransaction);
        void UpdateArcBillAndDayArcBill(Database::TDBTransaction &dbTransaction, int arcBillKey);
        int GetSiteId(Database::TDBTransaction &dbTransaction);
    protected:
        virtual void __fastcall Execute();
    public:
        void SendDataToPanasonicServer();
        // Constructor
        TPanasonicThread();
};
#endif
