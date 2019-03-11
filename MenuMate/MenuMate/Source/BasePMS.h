//---------------------------------------------------------------------------

#ifndef BasePMSH
#define BasePMSH
#include "MM_DBCore.h"
#include "PHSTCPIP.h"
#include "SiHotDataObjects.h"
#include "ManagerPMSCodes.h"
#include "OracleDataObjects.h"
//---------------------------------------------------------------------------
enum TriggerLocation {eBoot = 1,eUI,eSelf};
class TBasePMS
{
    public:
        TBasePMS();
        virtual ~TBasePMS();
        void Intialise();
        int TCPPort;
        AnsiString TCPIPAddress;
        int POSID;
        bool Enabled;
        AnsiString LastTransactionRef;
        AnsiString GetRefNumber();
        AnsiString DefaultPaymentCategory;
        AnsiString DefaultItemCategory;
        AnsiString CreditCategory;
        AnsiString PointsCategory;
        AnsiString RoundingCategory;
        AnsiString DefaultTransactionAccount;
        AnsiString DefaultSurchargeAccount;
        AnsiString TipAccount;
        AnsiString ExpensesAccount;
        AnsiString ServiceChargeAccount;
        AnsiString RoundingAccountSiHot;
        AnsiString DefaultAccountNumber;
        AnsiString RoundingAccountNumber;
        AnsiString RevenueCentre;
        AnsiString ApiKey;
        std::set<AnsiString> CodesTestedOk;
        std::vector<TTimeSlots> Slots;
        std::map<int,TRevenueCodeDetails> RevenueCodesMap;
        int RoomServiceRevenueCenter;
        AnsiString RoomServiceMenu;
        public :
        bool Registered;
        TPhoenixHM(Database::TDBControl &inDBControl);
        std::auto_ptr<TPhoenixNetTCPManager>	fPhoenixNet;
        AnsiString virtual GetLastTransactionRef();
        void virtual Initialise();
        bool virtual TestCode(AnsiString CodeToTest);
        void virtual ClearCodesTestedOk();
        bool virtual TestDefaultCodes();
        void virtual FindAndReplaceCode(AnsiString SourceCode, AnsiString DestCode);
        void virtual CheckCreditLimit(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort);
        void virtual ChargeRoom(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort);
        void virtual GetRoomStatus(std::vector<TSiHotAccounts> &siHotAccounts,AnsiString pmsIPAddress,int pmsPort);
        void virtual GetRoomStatus(TPhoenixRoomStatusExt &RoomStatus,AnsiString PMSIPAddress,int PMSPort);
        bool virtual ExportData(TPaymentTransaction &PaymentTransaction, int StaffID);
        void virtual GetRoomStatus(AnsiString _roomNumber, TRoomInquiryResult &_roomResult);//std::auto_ptr<TRoomInquiryResult> _roomResult);
        void virtual LogPMSEnabling(TriggerLocation triggerType);
        void virtual UnsetPostingFlag();
        void virtual StoreTicketPost(UnicodeString invoiceNumber, TMemoryStream *receiptStream);
        void virtual ValidateMenuAvailabilityForRoomRevenue();
};
//extern TBasePMS *BasePMS;
#endif
