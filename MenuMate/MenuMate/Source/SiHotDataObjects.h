//---------------------------------------------------------------------------

#ifndef SiHotDataObjectsH
#define SiHotDataObjectsH
#include <memory>
#include <vector>
#include <Classes.hpp>
#include "PHSTCPIP.h"
//---------------------------------------------------------------------------
struct TRoomRequest
{
    UnicodeString TransactionNumber;
    UnicodeString RoomNumber;
    UnicodeString IPAddress;
    int PortNumber;
};
struct TGuestDetails
{
    UnicodeString FirstName;
    UnicodeString LastName;
    UnicodeString Limit;
    UnicodeString From;
    UnicodeString To;
    UnicodeString Title;
    UnicodeString Vip;
    UnicodeString VipCode;
    UnicodeString Vip2;
    UnicodeString VIP2Code;
    UnicodeString SalesCategory;
    UnicodeString SalesCategoryCode;
    UnicodeString GeneralRemark;
    UnicodeString FbRemark;
    UnicodeString KitchenRemark;
    UnicodeString Board;
    UnicodeString FirstDayBoard;
    UnicodeString LastDayBoard;
    UnicodeString Drink;
    UnicodeString PersonType;
    UnicodeString PG;
    UnicodeString ExtRef;
    UnicodeString MarcetCode;
    UnicodeString ResNo;
    UnicodeString ResSubNo;
    UnicodeString NoPax;
    UnicodeString CardInfo;
    UnicodeString UniqueId;
    UnicodeString EMail;
    UnicodeString Message;
    UnicodeString MessageText;
    UnicodeString AccountNumber;
    UnicodeString AccountActive;
    UnicodeString AccountOwner;
    UnicodeString Pciid;
    UnicodeString RoomNumber;
    UnicodeString Mandator;
};
struct TRoomResponse
{
    UnicodeString TransactionNumber;
    std::vector<TGuestDetails> GuestsInformation;
    bool IsSuccessful;
    UnicodeString ResponseMessage;
};
struct TSiHotService
{
    UnicodeString SuperCategory;
    UnicodeString SuperCategory_Desc;
    UnicodeString MiddleCategory;
    UnicodeString MiddleCategory_Desc;
    UnicodeString ArticleCategory;
    UnicodeString ArticleCategory_Desc;
    UnicodeString ArticleNo;
    UnicodeString ArticleNo_Desc;
    UnicodeString PricePerUnit;
    UnicodeString Amount;
    UnicodeString PriceTotal;
    UnicodeString VATPercentage;
    UnicodeString Billno;
    UnicodeString Cashno;
    UnicodeString Cashier;
    UnicodeString Source;
};
struct TSiHotPayments
{
    UnicodeString Type;
    UnicodeString Amount;
    UnicodeString Billno;
    UnicodeString Cashno;
    UnicodeString Cashier;
    UnicodeString Source;
    UnicodeString Description;
};
struct TRoomCharge
{
    UnicodeString TransactionNumber;
    UnicodeString AccountNumber;
    std::vector<TSiHotService> SiHotServices;
    std::vector<TSiHotPayments> SiHotPayments;
    UnicodeString Covers;
    UnicodeString CoverType;
    UnicodeString CashNo;
    UnicodeString LinkID;
    UnicodeString LinkText;
    UnicodeString Shift;
    UnicodeString Date;
    UnicodeString Time;
    UnicodeString IPAddress;
    int PortNumber;
};

struct TRoomChargeResponse
{
    bool IsSuccessful;
    AnsiString ResponseMessage;
};
struct TAccountDetails
{
    UnicodeString Balance;
    UnicodeString CreditLimit;
    UnicodeString RoomNumber;
    UnicodeString FirstName;
    UnicodeString LastName;
};
struct TSiHotAccounts
{
    UnicodeString AccountNumber;

    std::vector<TAccountDetails> AccountDetails;
};



#endif
