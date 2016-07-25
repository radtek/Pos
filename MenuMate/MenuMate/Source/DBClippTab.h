//---------------------------------------------------------------------------

#ifndef DBClippTabH
#define DBClippTabH

#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBTable.hpp>
#include <set>
#include "MM_DBCore.h"
#include "enumTab.h"

//---------------------------------------------------------------------------

class TClippPayment
{
private:
    int _clippTabPaymentKey;
    int _clippTabKey;
    UnicodeString _clippPaymentRef;
    Currency _amount;
    TMMClippPaymentType _clippPaymentType;

    void SetClippTabPaymentKey(int clippTabPaymentKey);
    void SetClippTabKey(int clippTabKey);
    void SetClippPaymentRef(UnicodeString clippPaymentRef);
    void SetAmount(Currency amount);
    void SetClippPaymentType(TMMClippPaymentType clippPaymentType);

public:
    __property int ClippTabPaymentKey = {read = _clippTabPaymentKey, write = SetClippTabPaymentKey};
    __property int ClippTabKey = {read = _clippTabKey, write = SetClippTabKey};
    __property UnicodeString ClippPaymentRef = {read = _clippPaymentRef, write = SetClippPaymentRef};
    __property Currency Amount = {read = _amount, write = SetAmount};
    __property TMMClippPaymentType ClippPaymentType = {read = _clippPaymentType, write = SetClippPaymentType};
};

typedef std::vector<TClippPayment> CLIPP_PAYMENTS;

class TClippTab
{
private:
    int _clippTabKey;
    int _tabKey;
    TDateTime _tabCreatedDate;
    UnicodeString _clippTabRef;
    UnicodeString _clippUserRef;
    UnicodeString _customerFirstName;
    UnicodeString _customerLastName;
    UnicodeString _customerPhotoUrl;
    bool _isTabClosed;
    UnicodeString _tabName;
    TMMTabType _tabType;
    Currency _tabLimit;
    Currency _tabCredit;
    CLIPP_PAYMENTS _clippPayments;

    void SetTabKey(int tabKey);
    void SetClippTabKey(int clippTabKey);
    void SetTabCreatedDate(TDateTime tabCreatedDate);
    void SetClippTabRef(UnicodeString clippTabRef);
    void SetClippUserRef(UnicodeString clippUserRef);
    void SetCustomerFirstName(UnicodeString customerFirstName);
    void SetCustomerLastName(UnicodeString customerLastName);
    void SetCustomerPhotoUrl(UnicodeString customerPhotoUrl);
    void SetIsTabClosed(bool isTabClosed);
    void SetTabName(UnicodeString tabName);
    void SetTabType(TMMTabType tabType);
    void SetTabLimit(Currency tabLimit);
    void SetTabCredit(Currency tabCredit);
    void SetClippPayments(CLIPP_PAYMENTS clippPayments);

public:

    __property int ClippTabKey = {read = _clippTabKey, write = SetClippTabKey};
    __property int TabKey = {read = _tabKey, write = SetTabKey};
    __property TDateTime TabCreatedDate = {read = _tabCreatedDate, write = SetTabCreatedDate};
    __property UnicodeString ClippTabRef = {read = _clippTabRef, write = SetClippTabRef};
    __property UnicodeString ClippUserRef = {read = _clippUserRef, write = SetClippUserRef};
    __property UnicodeString CustomerFirstName = {read = _customerFirstName, write = SetCustomerFirstName};
    __property UnicodeString CustomerLastName = {read = _customerLastName, write = SetCustomerLastName};
    __property UnicodeString CustomerPhotoUrl = {read = _customerPhotoUrl, write = SetCustomerPhotoUrl};
    __property bool IsTabClosed = {read = _isTabClosed, write = SetIsTabClosed};
    __property UnicodeString TabName = {read = _tabName, write = SetTabName};
    __property TMMTabType TabType = {read = _tabType, write = SetTabType};
    __property Currency TabLimit = {read = _tabLimit, write = SetTabLimit};
    __property Currency TabCredit = {read = _tabCredit, write = SetTabCredit};
    __property CLIPP_PAYMENTS ClippPayments = {read = _clippPayments, write = SetClippPayments};
};

class TDBClippTab
{
public:

    static int CreateAndSetClippTabDetails(Database::TDBTransaction &dbTransaction, TClippTab clippTab);
    static int CreateClippTabPayment(Database::TDBTransaction &dbTransaction, int tabKey, UnicodeString clippPaymentRef, Currency clippPayment,
                            TMMClippPaymentType clippPaymentType);

    static bool CloseClippTab(Database::TDBTransaction &DBTransaction, int tabKey);


    static TClippTab* GetClippTabDetailsByTabKey(Database::TDBTransaction &dbTransaction, int tabKey);
    static TClippTab* GetClippTabDetailsByClippTabKey(Database::TDBTransaction &dbTransaction, int clippTabKey);

    static CLIPP_PAYMENTS GetAllClippPayments(Database::TDBTransaction &dbTransaction, int tabKey);
    static CLIPP_PAYMENTS GetTipPayments(Database::TDBTransaction &dbTransaction, int tabKey);
    static CLIPP_PAYMENTS GetProcessingFeesPayments(Database::TDBTransaction &dbTransaction, int tabKey);
    static CLIPP_PAYMENTS GetClippPayments(Database::TDBTransaction &dbTransaction, int tabKey);

    static Currency GetAllClippPaymentsTotal(Database::TDBTransaction &dbTransaction, int tabKey);
    static Currency GetTipPaymentsTotal(Database::TDBTransaction &dbTransaction, int tabKey);
    static Currency GetProcessingFeesPaymentsTotal(Database::TDBTransaction &dbTransaction, int tabKey);
    static Currency GetClippPaymentsTotal(Database::TDBTransaction &dbTransaction, int tabKey);

    static void GetOpenClippTabs(Database::TDBTransaction &dbTransaction, TStringList* tabList, TMMTabType tabType = TabClipp);

    static int GetClippTabKeyByClippTabRef(Database::TDBTransaction &dbTransaction, UnicodeString clippTabRef);
    static int GetTabKeyByClippTabRef(Database::TDBTransaction &dbTransaction, UnicodeString clippTabRef);
    static UnicodeString GetClippTabRefByTabKey(Database::TDBTransaction &dbTransaction, int tabKey);

    static int GetClippTabKeyByTabKey(Database::TDBTransaction &dbTransaction, int tabKey);
    static int GetTabKeyByClippTabKey(Database::TDBTransaction &dbTransaction, int clippTabKey);
    static int GetClippTabByTabKey(Database::TDBTransaction &dbTransaction, int tabKey);
    static bool IsTabClosed(Database::TDBTransaction &dbTransaction, int tabKey);
    static bool IsTabExist(Database::TDBTransaction &dbTransaction, int tabKey);

    static UnicodeString GetClippCustomerPhotoURl(Database::TDBTransaction &dbTransaction, int tabKey);
    static UnicodeString GetCustomerName(Database::TDBTransaction &dbTransaction, int tabKey);
    static bool IsProcessingFeeAlreadyApplied(Database::TDBTransaction &dbTransaction, int tabKey, int PaymentType);
    static void UpdateProcessingFeeAmount(Database::TDBTransaction &dbTransaction, Currency processingFee, int tabKey, int PaymentType);
    static void UpdateClippTabTabKey(Database::TDBTransaction &dbTransaction, int sourceTabKey, int destTabKey);
};

#endif
