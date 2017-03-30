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
struct GuestDetails
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
    std::vector<GuestDetails> GuestsInformation;
    bool IsSuccessful;
    UnicodeString ResponseMessage;
};
struct TRoomCharge
{

};
struct TRoomChargeResponse
{

};


#endif
