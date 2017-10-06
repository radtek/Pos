//---------------------------------------------------------------------------

#ifndef BasePMSH
#define BasePMSH
#include "MM_DBCore.h"
#include "PHSTCPIP.h"
#include "SiHotDataObjects.h"
//---------------------------------------------------------------------------
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
        bool IsFastTenderEnabled;

        std::set<AnsiString> CodesTestedOk;
        public :
        bool Registered;
        bool nabled;
        TPhoenixHM(Database::TDBControl &inDBControl);
        void virtual GetRoomStatus(TPhoenixRoomStatusExt &RoomStatus,AnsiString PMSIPAddress,int PMSPort);
        bool virtual ExportData(TPaymentTransaction &PaymentTransaction, int StaffID);

        AnsiString virtual GetLastTransactionRef();

        void virtual Initialise();

        std::auto_ptr<TPhoenixNetTCPManager>	fPhoenixNet;
        bool virtual TestCode(AnsiString CodeToTest);
        void virtual ClearCodesTestedOk();
        bool virtual TestDefaultCodes();
        void virtual FindAndReplaceCode(AnsiString SourceCode, AnsiString DestCode);
        void virtual CheckCreditLimit(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort);
        void virtual ChargeRoom(TPhoenixRoomCharge &RoomCharge,AnsiString PMSIPAddress,int PMSPort);
        void virtual GetRoomStatus(std::vector<TSiHotAccounts> &siHotAccounts,AnsiString pmsIPAddress,int pmsPort);
};
//extern TBasePMS *BasePMS;
#endif
