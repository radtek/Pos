//---------------------------------------------------------------------------

#ifndef ManagerPanasonicH
#define ManagerPanasonicH
#include <Classes.hpp>
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

class TPanasonicThread : public TThread
{
    private:
        //TTimer* panasonicThreadTimer;
        UnicodeString GetMemberName(Database::TDBTransaction &dbTransaction, int contactKey);
        void ConvertTransactionInfoToPanasonicInfo(Database::TDBTransaction &dbTransaction );
        void ConvertTransactionInfoToPanasonicItemList(Database::TDBTransaction &dbTransaction, int arcBill_Key);
        void ConverTransactionInfoToTransactionDBServerInfo(Database::TDBTransaction &dbTransaction);
        UnicodeString GetPOSVersionInfo(Database::TDBTransaction &dbTransaction);
        void UpdateArcBillAndDayArcBill(Database::TDBTransaction &dbTransaction, int arcBillKey);
        int GetSiteId(Database::TDBTransaction &dbTransaction);
       // void __fastcall OnPanasonicThreadTimerTick(TObject *Sender);
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
};
#endif
