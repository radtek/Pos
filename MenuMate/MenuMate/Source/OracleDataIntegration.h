//---------------------------------------------------------------------------

#ifndef OracleDataIntegrationH
#define OracleDataIntegrationH

#include "OracleDataIntegration.h"
#include "PaymentTransaction.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
///////////////  Room Request /////////////////////////////////
struct TRoomInquiryItem;
struct TPostRoomInquiry
{
    AnsiString InquiryInformation;
    AnsiString MaximumReturnedMatches;
    AnsiString SequenceNumber;
    AnsiString RequestType;
    AnsiString PaymentMethod;
    AnsiString Date;
    AnsiString Time;
    AnsiString RevenueCenter;
    AnsiString WaiterId;
    AnsiString WorkstationId;
};

struct TRoomInquiryResult
{
    AnsiString SequenceNumber;
    AnsiString HotelId;
    AnsiString PaymentMethod;
    AnsiString RevenueCenter;
    AnsiString WaiterId;
    AnsiString WorkstationId;
    AnsiString Date;
    AnsiString Time;
    std::vector<TRoomInquiryItem> TRoomInquiryItem;
};

struct TRoomInquiryItem
{
    AnsiString RoomNumber;
    AnsiString ReservationId;
    AnsiString LastName;
    AnsiString FirstName;
    AnsiString Title;
    AnsiString NoPost;
    AnsiString CreditLimit;
    AnsiString ProfileId;
    AnsiString HotelId;
};

struct TRoomInquiryAnswer
{
    AnsiString AnswerStatus;
    AnsiString ResponseText;
    AnsiString SequenceNumber;
    AnsiString HotelId;
    AnsiString Date;
    AnsiString Time;
    AnsiString PaymentMethod;
    AnsiString RevenueCenter;
    AnsiString WaiterId;
    AnsiString WorkstationId;
};


////////////// Sales Data //////////////////////////

struct TPostRequest
{
    AnsiString RoomNumber;
    AnsiString ReservationId;
    AnsiString ProfileId;
    AnsiString LastName;
    AnsiString HotelId;
    AnsiString RequestType;
    AnsiString InquiryInformation;
    AnsiString MatchfromPostList;
    AnsiString SequenceNumber;
    AnsiString TotalAmount;
    AnsiString CreditLimitOverride;
    AnsiString PaymentMethod;
    AnsiString Covers;
    AnsiString RevenueCenter;
    AnsiString ServingTime;
    AnsiString CheckNumber;
    std::vector<AnsiString> Subtotal1;
    std::vector<AnsiString> Discount;
    AnsiString Tip;
    std::vector<AnsiString> ServiceCharge;
    std::vector<AnsiString> Tax;
    AnsiString Date;
    AnsiString Time;
    AnsiString WaiterId;
    AnsiString WorkstationId;
};

struct TPostRequestAnswer
{
    AnsiString RoomNumber;
    AnsiString ReservationId;
    AnsiString LastName;
    AnsiString AnswerStatus;
    AnsiString ResponseText;
    AnsiString CheckNumber;
    AnsiString SequenceNumber;
    AnsiString HotelId;
    AnsiString Date;
    AnsiString Time;
    AnsiString PaymentMethod;
    AnsiString RevenueCenter;
    AnsiString WaiterId;
    AnsiString WorkstationId;
};
#endif
