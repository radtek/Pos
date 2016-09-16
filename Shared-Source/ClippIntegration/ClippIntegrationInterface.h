//---------------------------------------------------------------------------

#ifndef ClippIntegrationInterfaceH
#define ClippIntegrationInterfaceH

//---------------------------------------------------------------------------

#include <StdCtrls.hpp>
#include "ClippIntegrationWSDL.h"
#include "GlobalSettings.h"
#include "LogsDB.h"
#include <map>
#include <vector>

typedef ArrayOfClippMessage CLIPP_MESSAGE_LIST;
typedef ArrayOfDiscount CLIPP_DISCOUNT_LIST;

//This enum defines the type of message we are trying to retrieve through Clipp interface..
//0 stands for outsatnding messages in queue which needs to be processed on POS
//1 stands for already processed messages by POS..
enum MMClippMessageVersionType
{
    Current = 0,
    Previous = 1
};

//This enum declares the message type i.e. whether a tab has been opened, or closed....
enum MMClippMessageType
{
  TabOpen,
  TabStatus,
  TabUpdate,
  TabPay,
  TabClose
};

//MM version of Service Clipp Discounts, it has been declared to make sure we dont carry the WSDL client every where..
class MMClippDiscount
{
public:
    MMClippDiscount(Currency amount, UnicodeString description, UnicodeString discountType, UnicodeString reference):
                    Amount(amount),
                    Description(description),
                    DiscountType(discountType),
                    Reference(reference)
    { }

    Currency Amount;
    UnicodeString Description;
    UnicodeString DiscountType;
    UnicodeString Reference;
};

//Defining the vector to hold the vector of discounts..
typedef std::vector<MMClippDiscount> CLIPP_DISCOUNT_VECTOR;

//MM version of Service ClippMessage, it has been declared to make sure we dont carry the WSDL client every where..
class MMClippMessage
{
public:
    MMClippMessage(TDateTime createdDate, __int64 id, UnicodeString  messageId, MMClippMessageType messageType,
                    UnicodeString clippPaymentRef, UnicodeString clippTabRef, UnicodeString clippUserRef, Currency discountAmount,
                    UnicodeString firstName, UnicodeString lastName, UnicodeString photoUrl, UnicodeString posTabRef,
                    Currency processingFeeAmount, Currency tabLimit, Currency tipAmount, Currency totalPaymentAmount) :
                    CreatedDate(createdDate),
                    Id(id),
                    MessageId(messageId),
                    MessageType(messageType),
                    ClippPaymentRef(clippPaymentRef),
                    ClippTabRef(clippTabRef),
                    ClippUserRef(clippUserRef),
                    DiscountAmount(discountAmount),
                    FirstName(firstName),
                    LastName(lastName),
                    PhotoUrl(photoUrl),
                    PosTabRef(posTabRef),
                    ProcessingFeeAmount(processingFeeAmount),
                    TabLimit(tabLimit),
                    TipAmount(tipAmount),
                    TotalPaymentAmount(totalPaymentAmount)
    { }

    TDateTime CreatedDate;
    __int64 Id;
    UnicodeString MessageId;
    MMClippMessageType MessageType;
    UnicodeString ClippPaymentRef;
    UnicodeString ClippTabRef;
    UnicodeString ClippUserRef;
    Currency DiscountAmount;
    CLIPP_DISCOUNT_VECTOR Discounts;
    UnicodeString FirstName;
    UnicodeString LastName;
    UnicodeString PhotoUrl;
    UnicodeString PosTabRef;
    Currency ProcessingFeeAmount;
    Currency TabLimit;
    Currency TipAmount;
    Currency TotalPaymentAmount;
};

//Defining the vector to hold the vector of ClippMessage..
typedef std::vector<MMClippMessage> CLIPP_MESSAGE_VECTOR;

//Defining the vector to hold the map of MessageType with ClippMessage..
//This will help in differentiating the messages recieved from Clipp..
typedef std::map<MMClippMessageType, CLIPP_MESSAGE_VECTOR> CLIPP_MESSAGE_MAP;

class MMClippItem;

//Defining the vector to hold the vector of MMItem..
typedef std::vector<MMClippItem> ITEM_VECTOR;

class MMClippItem
{
public:
    MMClippItem(UnicodeString category, UnicodeString description, Currency discountAmount, UnicodeString discountDescription,
            UnicodeString group, Currency price, Currency quantity, bool isTaxInclusive, Currency totalAmount, UnicodeString venueItemRef):
            Category(category),
            Description(description),
            DiscountAmount(discountAmount),
            DiscountDescription(discountDescription),
            Group(group),
            Price(price),
            Quantity(quantity),
            IsTaxInclusive(isTaxInclusive),
            TotalAmount(totalAmount),
            VenueItemRef(venueItemRef)
    { }

    UnicodeString Category;
    UnicodeString Description;
    Currency DiscountAmount;
    UnicodeString DiscountDescription;
    UnicodeString Group;
    Currency Price;
    Currency Quantity;
    ITEM_VECTOR SubItems;
    bool IsTaxInclusive;
    Currency TotalAmount;
    UnicodeString VenueItemRef;
};

class MMClippPayment
{
public:
    MMClippPayment(Currency amount, UnicodeString description, UnicodeString venuePaymentRef):
                Amount(amount),
                Description(description),
                VenuePaymentRef(venuePaymentRef)
    { }

    Currency Amount;
    UnicodeString Description;
    UnicodeString VenuePaymentRef;
};

//Defining the vector to hold the vector of MMPayment..
typedef std::vector<MMClippPayment> PAYMENT_VECTOR;

class MMClippSundry
{
public:
    MMClippSundry(Currency amount, UnicodeString description, UnicodeString venueSundryRef):
                Amount(amount),
                Description(description),
                VenueSundryRef(venueSundryRef)
    { }

    Currency Amount;
    UnicodeString Description;
    UnicodeString VenueSundryRef;
};

//Defining the vector to hold the vector of MMSundry..
typedef std::vector<MMClippSundry> SUNDRY_VECTOR;

class MMClippTabDetail
{
public:
    MMClippTabDetail(UnicodeString clippTabRef, UnicodeString messageId, Currency includedTaxAmount, Currency outstandingAmount,
                TDateTime recieptDate, Currency tabLimit, Currency totalAmount, UnicodeString venueRecieptId, UnicodeString venueTabRef) :
                ClippTabRef(clippTabRef),
                MessageId(messageId),
                IncludedTaxAmount(includedTaxAmount),
                OutstandingAmount(outstandingAmount),
                RecieptDate(recieptDate),
                TabLimit(tabLimit),
                TotalAmount(totalAmount),
                VenueRecieptId(venueRecieptId),
                VenueTabRef(venueTabRef)
    { }

    Currency Amount;
    UnicodeString ClippTabRef;
    UnicodeString MessageId;
    Currency IncludedTaxAmount;
    ITEM_VECTOR Items;
    Currency OutstandingAmount;
    PAYMENT_VECTOR Payments;
    TDateTime RecieptDate;
    SUNDRY_VECTOR Sundries;
    Currency TabLimit;
    Currency TotalAmount;
    UnicodeString VenueRecieptId;
    UnicodeString VenueTabRef;
};

//Defining the vector to hold the vector of MMSundry..
typedef std::vector<MMClippTabDetail> CLIPP_TAB_DETAIL_VECTOR;

class MMClippMessageDetail
{
public:
    MMClippMessageDetail(UnicodeString clippPaymentRef, UnicodeString clippTabRef, UnicodeString clippUserRef, Currency discountAmount,
                    UnicodeString firstName, UnicodeString lastName, UnicodeString photoUrl, UnicodeString posTabRef,
                    Currency processingFeeAmount, Currency tabLimit, Currency tipAmount, Currency totalPaymentAmount) :
                    ClippPaymentRef(clippPaymentRef),
                    ClippTabRef(clippTabRef),
                    ClippUserRef(clippUserRef),
                    DiscountAmount(discountAmount),
                    FirstName(firstName),
                    LastName(lastName),
                    PhotoUrl(photoUrl),
                    PosTabRef(posTabRef),
                    ProcessingFeeAmount(processingFeeAmount),
                    TabLimit(tabLimit),
                    TipAmount(tipAmount),
                    TotalPaymentAmount(totalPaymentAmount)
    { }
    UnicodeString ClippPaymentRef;
    UnicodeString ClippTabRef;
    UnicodeString ClippUserRef;
    Currency DiscountAmount;
    CLIPP_DISCOUNT_VECTOR Discounts;
    UnicodeString FirstName;
    UnicodeString LastName;
    UnicodeString PhotoUrl;
    UnicodeString PosTabRef;
    Currency ProcessingFeeAmount;
    Currency TabLimit;
    Currency TipAmount;
    Currency TotalPaymentAmount;
};

class MMPaymentDetailRequest
{
public:
    MMPaymentDetailRequest(Currency amount, UnicodeString clippMessageId, UnicodeString clippTabRef, UnicodeString errorData):
                Amount(amount),
                ClippMessageId(clippMessageId),
                ClippTabRef(clippTabRef),
                ErrorData(errorData)
    { }

    Currency Amount;
    UnicodeString ClippMessageId;
    UnicodeString ClippTabRef;
    UnicodeString ErrorData;
};

class MMPaymentDetailResponse
{
public:
    MMPaymentDetailResponse(UnicodeString clippPaymentRef, UnicodeString clippTabRef, UnicodeString posTabRef,
                            Currency processingFeeAmount, Currency totalPaymentAmount):
                ProcessingFeeAmount(processingFeeAmount),
                ClippPaymentRef(clippPaymentRef),
                ClippTabRef(clippTabRef),
                PosTabRef(posTabRef),
                TotalPaymentAmount(totalPaymentAmount)
    { }

    UnicodeString ClippPaymentRef;
    UnicodeString ClippTabRef;
    UnicodeString PosTabRef;
    Currency ProcessingFeeAmount;
    Currency TotalPaymentAmount;
};

class MMClippErrorDetail
{
public:
    MMClippErrorDetail(UnicodeString clippTabRef, UnicodeString messageId, Currency totalAmount, UnicodeString errorCode,
                        UnicodeString errorDescription):
                MessageId(messageId),
                TotalAmount(totalAmount),
                ClippTabRef(clippTabRef),
                ErrorCode(errorCode),
                ErrorDescription(errorDescription)
    { }

    UnicodeString ClippTabRef;
    UnicodeString MessageId;
    Currency TotalAmount;

    UnicodeString ErrorCode;
    UnicodeString ErrorDescription;
};

//---------------------------------------------------------------------------
// This class will act as an interface to the ClippIntegration wcf service
// purpose of creating this class so it will hide the complexities of
// consuming a web service within Embacadero.
//---------------------------------------------------------------------------

class TClippIntegrationInterface
{
public:
    // Constructor and Destructor
    TClippIntegrationInterface();
    ~TClippIntegrationInterface();

    //These methods encapsulate the Service Methods..
    CLIPP_MESSAGE_MAP GetClippMessages(MMClippMessageVersionType clippMessageType);
    void SendTabDetails(MMClippTabDetail mmClippTabDetail);
    void SendTabDetails(UnicodeString clippTabRef, UnicodeString recievedMessageId, UnicodeString errorCode,
                                                            UnicodeString errorDescription, Currency totalPaymentAmount);
    MMClippMessageDetail CloseTab(MMClippTabDetail mmClippTabDetail);
    void CloseTab(UnicodeString clippTabRef, UnicodeString recievedMessageId, UnicodeString errorCode,
                                                            UnicodeString errorDescription, Currency totalPaymentAmount);
    MMPaymentDetailResponse RequestTabPayment(MMPaymentDetailRequest mmPaymentDetailRequest);
    bool ForceCloseAllTabs();

private:
    //Fields..
    _di_IClippIntergrationWebService _clippIntegrationWebClient;
    bool _useWsdl;

    //Methods..
    void InitializeWebClient();
    CLIPP_MESSAGE_MAP ProcessMessagesIntoMap(CLIPP_MESSAGE_LIST clippMessages);
    MMClippMessageType CreateMMClippMessageType(ClippMessageType clippMessageType);

    //These methods are factory methods to convert wsdl specific domain objects to mm specific domain objects..
    MMClippMessage CreateMMClippMessage(ClippMessage* clippMessage);
    MMClippTabDetail CreateMMClippTabDetail(ClippTabDetailRequest* clippTabDetailRequest);
    MMClippDiscount CreateMMClippDiscount(NS_ClippIntegrationWSDL::Discount* discount);
    MMClippItem CreateMMClippItem(Item* item);
    MMClippPayment CreateMMClippPayment(NS_ClippIntegrationWSDL::Payment* payment);
    MMClippSundry CreateMMClippSundry(Sundry* sundry);
    MMClippMessageDetail CreateMMClippMessageDetail(ClippMessageDetail* clippMessageDetail);
    MMPaymentDetailResponse CreateMMPaymentDetailResponse(PaymentDetailResponse* paymentDetailResponse);

    //These methods are factory methods to convert mm specific domain objects to wsdl specific domain objects..
    ClippTabDetailRequest* CreateClippTabDetailRequest(MMClippTabDetail mmClippTabDetail);
    ClippErrorDetail* CreateClippErrorDetail(UnicodeString clippTabRef, UnicodeString recievedMessageId, UnicodeString errorCode,
                                                            UnicodeString errorDescription, Currency totalPaymentAmount = 0.0);
    NS_ClippIntegrationWSDL::Discount* CreateDiscount(MMClippDiscount mmClippDiscount);
    Item* CreateItem(MMClippItem mmClippItem);
    NS_ClippIntegrationWSDL::Payment* CreatePayment(MMClippPayment mmClippPayment);
    Sundry* CreateSundry(MMClippSundry mmClippSundry);
    ClippMessageDetail* CreateClippMessageDetail(MMClippMessageDetail mmClippMessageDetail);
    PaymentDetailRequest* CreatePaymentDetailRequest(MMPaymentDetailRequest mmPaymentDetailRequest);

    //This method is for sanitizing the clipp tab ref to tab key..
    UnicodeString GetTabKeyByClippTabRef(UnicodeString clippTabRef);
};

#endif



