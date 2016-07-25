//---------------------------------------------------------------------------

#ifndef ThorlinkClientH
#define ThorlinkClientH
#include "ThorlinkWSDL.h"
#include <IdTCPClient.hpp>
#include <memory>
#include <tinyxml.h>
#include<list>
#include "ThorlinkDataObjects.h"
#include "MembershipExternalXML.h"
//---------------------------------------------------------------------------

struct TVoucherDetails
{
    UnicodeString           Id;
    int                   Type;
    UnicodeString         Code;
    AnsiString            PluCode;
    UnicodeString SecurityCode;
    UnicodeString         Name;
    double               Value;
    int                 Active;
    UnicodeString MerchantName;
    UnicodeString     ImageUrl;
    UnicodeString    StartDate;
    UnicodeString      EndDate;
    UnicodeString   ExpiryDate;
    int       AvailableInStore;

};

typedef std::list<TVoucherDetails> TDETAILS_LIST;

class TThorlinkClient
{
	private:
    _di_IWCFServiceThorlink thorclient;

   public :
      UnicodeString ResponseDescription ;
      UnicodeString TraderStatusName;
      UnicodeString TraderId;
      UnicodeString FirstName;
      UnicodeString LastName;
      UnicodeString BirthDate;
      UnicodeString EmailAddress;
      UnicodeString MobileNumber;
      UnicodeString Address;
      double LoyaltyBalance;
      TThorlinkClient();
      void initTLClient();
      bool OffLine;
      bool SetAuthorizationParameters(AnsiString AppKey,AnsiString SiteNo,AnsiString MerchantCode,AnsiString DeviceCode);
      void GetMemberInformation(AnsiString card);
      TPurchaseResponse GetTenderInformation(std::vector<TTenderDetails> tenderDetailsList,std::vector<TItemDetailsThor> itemsList);
      TPurchaseResponse GetRefundInformation(TRefundTransaction refundTransaction);
      DTO_TenderDetails* CreateTenderItem(TTenderDetails tender);
      DTO_ItemDetails* CreateItemThor(TItemDetailsThor itemThor);


      TDETAILS_LIST thorclientvouchers;

};


#endif