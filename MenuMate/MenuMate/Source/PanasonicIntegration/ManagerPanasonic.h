//---------------------------------------------------------------------------

#ifndef ManagerPanasonicH
#define ManagerPanasonicH
#include <Classes.hpp>
#include "DeviceRealTerminal.h"
#include "DBPanasonic.h"

//---------------------------------------------------------------------------

class TPanasonicThread : public TThread
{
    private:
        //TTimer* panasonicThreadTimer;
        void GetMemberNameAndCustomerID(Database::TDBTransaction &dbTransaction, UnicodeString invoiceNo, int &contactKey, UnicodeString &memberName);
        void ConvertTransactionInfoToPanasonicInfo(Database::TDBTransaction &dbTransaction );
        void ConvertTransactionInfoToPanasonicItemList(TDBPanasonic &dbPanasonic, Database::TDBTransaction &dbTransaction, int arcBill_Key);
        void ConverTransactionInfoToTransactionDBServerInfo(TDBPanasonic &dbPanasonic, Database::TDBTransaction &dbTransaction);
        UnicodeString GetPOSVersionInfo(Database::TDBTransaction &dbTransaction);
        void UpdateArcBillAndDayArcBill(Database::TDBTransaction &dbTransaction, int arcBillKey);
        int GetSiteId(Database::TDBTransaction &dbTransaction);
        TDateTime GetStartDateTime(Database::TDBTransaction &dbTransaction, int arcBillKey);
        bool HasAllProperties(AnsiString propertyString,AnsiString allProperties);
    protected:
        virtual void __fastcall Execute();
    public:
        void SendDataToPanasonicServer();
        // Constructor
        TPanasonicThread();
};

class TManagerPanasonic
{
    private:
        static TManagerPanasonic* instance;
        TPanasonicThread* panasonicThread;
        TTimer* panasonicThreadTimer;
        bool panasonicThreadTerminated;

        // Constructor and Destructor
        TManagerPanasonic();
        ~TManagerPanasonic();

        void __fastcall PanasonicTheadTerminate( TObject* sender );
        void InitiatePanasonicThread();
        void StartPanasonicThread();
        void StartPanasonicThreadTimer();
        void StopPanasonicThreadTimer();
        void __fastcall OnPanasonicThreadTimerTick(TObject *Sender);
        bool IsPendingRecordForPostingToServerExist();

    public:

        static TManagerPanasonic* Instance()
        {
            if(instance == NULL)
                instance = new TManagerPanasonic();

            return instance;
        }

        void TriggerTransactionSync();
        void PrepareTenderTypes();
        void PrepareTransactionTypes();
};
#endif
