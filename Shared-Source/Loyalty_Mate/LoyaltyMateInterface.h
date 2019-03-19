//---------------------------------------------------------------------------

#ifndef LoyaltyMateInterfaceH
#define LoyaltyMateInterfaceH
//---------------------------------------------------------------------------

#include <StdCtrls.hpp>
#include "MMContactInfo.h"
#include "Membership.h"
#include "LogsDB.h"
#include "MM_DBCore.h"
#include "ManagerSyndCode.h"
#include "DBTierLevel.h"
#include "LoyaltyMateWSDL.h"
#include "Discount.h"
#include "LoyaltyMateUtilities.h"
#include "OnlineOrderingAttributes.h"

//---------------------------------------------------------------------------
// enums and service reponse classes that maps the information from responses from wcf service
//---------------------------------------------------------------------------


enum  MMDiscountType
{
  Percent,
  Dollar,
  Combo,
  ItemMode,
  SetPrice
};

enum MMDisplayOption
{
  FixedDescriptionAndAmount,
  PromptForDescription,
  PromptForAmount,
  PromptForDescriptionAndAmount
};

enum MMImplicationType
{
  Discount,
  Surcharge,
  Point
};

enum MMProductPriority
{
  LowestPriceFirst,
  HighestPriceFirst,
  NotApllicable
};


typedef std::pair <AnsiString, AnsiString> TNamePair;
using Database::TDBControl;
using Database::TDBTransaction;



//---------------------------------------------------------------------------
// this class will act as an interface to the loyalty_mate wcf service
// purpose of creating this class so it will hide the complexities of
// consuming a web service within Embacadero.
//---------------------------------------------------------------------------
class TLoyaltyMateInterface
{
    private:
        // Private Members
        _di_IWCFServiceLoyaltyMate loyaltymateClient; // WCF client
        int siteId; // site id for the menumate database
        bool FAutoSync;
    public:

        // Constructor and Destructor
        TLoyaltyMateInterface();
        ~TLoyaltyMateInterface();

        // sets the site id
        void RefreshSiteID();

        // creates a member in the cloud
        // returns a globally unique identifier so the POS can store it on the card and in the database for future use
        MMLoyaltyServiceResponse CreateMember(TSyndCode syndicateCode,TMMContactInfo &contactInfo,AnsiString &uuid );

        // updates member details using a uuid
        MMLoyaltyServiceResponse UpdateMember(TSyndCode syndicateCode,AnsiString uuid,const TMMContactInfo* const contactInfo);

        // deletes a member in the cloud
        MMLoyaltyServiceResponse DeleteMember(TSyndCode  syndicateCode,AnsiString uuid );

        // retrieve member details using a uuid
        MMLoyaltyServiceResponse GetMemberDetails(TSyndCode syndicateCode,AnsiString uuid,TMMContactInfo &contactInfo,bool replacePoints = false );

        // retrieve member details using a uuid
        MMLoyaltyServiceResponse GetMemberDetailsByCode(TSyndCode syndicateCode,AnsiString memberCode,TMMContactInfo &contactInfo,bool replacePoints = false );

        // retrieve member details using a uuid
        MMLoyaltyServiceResponse GetMemberDetailsByEmail(TSyndCode syndicateCode,AnsiString memberEmail,TMMContactInfo &contactInfo,bool replacePoints = false );

        // posts members transactions on to the cloud
        MMLoyaltyServiceResponse PostTransaction(TSyndCode syndicateCode,AnsiString uuid,Currency pointsBalance,Currency pointsDelta,
                                                 TDateTime occurredDate,int pointsType,AnsiString inInvoiceNumber);

        // Get gift voucher balance
        MMLoyaltyServiceResponse GetGiftVoucherBalance(TSyndCode syndicateCode,AnsiString giftVoucherNumber,TGiftCardDetail &GiftCardDetail);

        // Get pocket voucher Detail
        MMLoyaltyServiceResponse GetPocketVoucherDetail(TSyndCode syndicateCode,AnsiString pocketVoucherNumber,TVoucherDetail &VoucherDetail);

        MMLoyaltyServiceResponse ProcessVoucherTransaction(TSyndCode syndicateCode,TVoucherUsageDetail& inVoucherUsageDetail);

        MMLoyaltyServiceResponse ReleaseVouchers(TSyndCode syndicateCode,TReleasedVoucherDetail inReleasedVoucherDetail);

        // Public Properties
        __property bool AutoSync =
        {
	        read = FAutoSync, write = FAutoSync
        };

        MMLoyaltyServiceResponse SyncCompanyDetails(TSyndCode syndicateCode);
        MMLoyaltyServiceResponse UpdateMemberCardCode(TSyndCode syndicateCode,AnsiString uniqueId,AnsiString memberCardCode);
        void SyncCompanyInfo(CompanyInfo* companyInfo);
        void SyncTierLevels(Database::TDBTransaction &DBTransaction,DynamicArray<TierLevelInfo*> tierLevels);
        void SyncDiscounts(Database::TDBTransaction &DBTransaction,DynamicArray<DiscountInfo*> discounts);
        void ReadPocketVoucherInfo(VoucherInfo* inVoucherInfo,TVoucherDetail& VoucherDetail);
        void ReadGiftCardInfo(GiftCardInfo* inVoucherInfo,TGiftCardDetail& GiftCardDetail);
        void ReadMemberVouchers(DynamicArray<VoucherInfo*> memberVouchers,TMMContactInfo& inContactInfo);
        //for online ordering menu syncing.
        MMLoyaltyServiceResponse SendMenu(TSiteMenuInfo menuInfo);
        MMLoyaltyServiceResponse SendTaxSettings(TSiteTaxSettingsInfo taxSettingsInfo);
        MMLoyaltyServiceResponse PostOnlineOrderInvoiceInfo(TSiteOrderModel siteOrderModel);
        MMLoyaltyServiceResponse SyncOnlineOrderingDetails(TSyndCode syndicateCode,int siteCode);
        bool UnsetOrderingDetails(TSyndCode syndicateCode,int siteCode);
        void SendEmail(AnsiString emailBody);
        void AddLoyaltyLogs(std::auto_ptr<TStringList> loyaltyLogs);
    private:
        // initiates the Loyaltymate WCF Client
        void InitLMClient();

        // initiates the Site ID. Read the value from the DB
        void InitSiteID();

        // initiates the AutoSync property.
        // AutoSync = true means both the External Key and the Points
        // are synced with their values in the cloud automatically
        void InitAutoSync();


        // searches through menumate contact table to get the site id
        int GetCurrentSiteId();

        // creates the contact name with Loyalty WCF format
        AnsiString CreateWCFName( const TMMContactInfo* const contactInfo );

        void CreateWcfContactInfo(MemberInfo* inMemberInfo,TMMContactInfo& inContactInfo);

        // reads the contact info from the WCF response
        void ReadContactInfo(LoyaltyMemberResponse* inWCFResponse,TMMContactInfo& outContactInfo, bool replacePoints = false );

        // reads the contact info list from the WCF member info
        void ReadContactInfo(MemberInfo* inMemberInfo,TMMContactInfo& contactInfo,bool replacePoints = false );

        // syncs loyaltymate attr coming with the cloud with the db's
		void SyncLoyaltymateAttrs(const TMMContactInfo* const inContactInfo );

        // updates loyaltymate attr in the db's
	   bool UpdateLoyaltymateAttrs(const TMMContactInfo* const inContactInfo );

        // add loyaltymate attr to the db's
		void AddLoyaltymateAttrs(const TMMContactInfo* const inContactInfo );

        // creates MMResponse from LoyaltyResponse
        MMLoyaltyServiceResponse CreateMMResponse(LoyaltyResponse* inWCFResponse );

        // creates MMResponse from LoyaltyMemberResponse
        MMLoyaltyServiceResponse CreateMMResponse(LoyaltyMemberResponse* inWCFResponse );

        MMLoyaltyServiceResponse CreateMMResponse(LoyaltyCompanyResponse* inWCFResponse );

        MMLoyaltyServiceResponse CreateMMResponse(LoyaltyGiftCardResponse* inWCFResponse );

        MMLoyaltyServiceResponse CreateMMResponse(LoyaltyVoucherResponse* inWCFResponse );

        MMLoyaltyServiceResponse CreateMMResponse(VoucherTransactionResponse* inWCFResponse );
        MMLoyaltyServiceResponse CreateMMResponse(LoyaltyOnlineOrderingResponse*  inWCFResponse);

        // creates MMResponse from an exception message
        MMLoyaltyServiceResponse CreateExceptionFailedResponse(AnsiString inMessage );

		// generates a table key
		int GenerateTableKey( const AnsiString inGeneratorName,TDBControl* const inDBControl );

		// executes an SQL query
		int ExecuteQuery(const AnsiString  inGeneratorName,TDBControl* const inDBControl );

        int GetTierCloudId(int tierLevel);
        int GetTierLevelFromCloudId(int cloudId);
        void ReadTierInfo(TierLevelInfo* inTierInfo,TTierLevel *TierLevel);
        void ReadDiscountInfo(Database::TDBTransaction &DBTransaction,DiscountInfo* inDiscountInfo,TDiscount& discount);
        void ExtractModeAndAmount(DiscountInfo* inDiscountInfo,TDiscount& discount);
        void CreateVoucherPaymentType(Database::TDBTransaction &DBTransaction);
        void CreateGiftVoucherPaymentType(Database::TDBTransaction &DBTransaction);
        void  DisableSyncSetting(Database::TDBTransaction &DBTransaction);
        RequestInfo* CreateRequest(AnsiString requestKey);
        //Online ordering
        AnsiString GetSyndCodeForOnlineOrdering();
        OrderItemModel* CreateOrderItemModel(TOrderItemModel itemModel);
        OrderItemSizeModel* CreateOrderItemSizeModel(TOrderItemSizeModel orderItemSizeModel);
        OrderItemSizeDiscountModel* CreateOrderItemSizeDiscountModel(TOrderItemSizeDiscountModel itemSizeDiscountModel);
        OrderItemSizeTaxProfileModel* CreateOrderItemSizeTaxProfileModel(TOrderItemSizeTaxProfileModel itemSizeTaxProfileModel);
        OrderInvoiceTransactionModel* CreateOrderInvoiceTransaction(TOrderInvoiceTransactionModel orderinvoiceTransaction);
        InvoiceTransactionModel* CreateOrderInvoiceTransaction(TInvoiceTransactionModel invoiceTransactionModel);
        AnsiString GetEmailBody(AnsiString syndicateCode, AnsiString email, AnsiString uuid);
        AnsiString GetLogFileName();
        //void AddLoyaltyLogs(std::auto_ptr<TStringList> loyaltyLogs);
};

#endif
