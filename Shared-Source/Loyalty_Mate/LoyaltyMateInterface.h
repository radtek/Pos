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

//---------------------------------------------------------------------------
// enums and service reponse classes that maps the information from responses from wcf service
//---------------------------------------------------------------------------
enum MMLoyaltyResponseCode
{
  Successful,
  AuthenticationFailed,
  CreateMemberFailed,
  UpdateMemberFailed,
  DeleteMemberFailed,
  GetMemberFailed,
  PostTransactionFailed,
  CreateTierFailed,
  UpdateTierFailed,
  DeleteTierFailed,
  FailedDueToException,
  MemberNotExist,
};

typedef std::pair <AnsiString, AnsiString> TNamePair;
using Database::TDBControl;
using Database::TDBTransaction;

class MMLoyaltyServiceResponse
{
    public:
        bool                  IsSuccesful;
        AnsiString            Message;
        AnsiString            Description;
        MMLoyaltyResponseCode ResponseCode;
        AnsiString            UUID;

        MMLoyaltyServiceResponse(
                bool                  inSuccess,
                AnsiString            inMessage,
                AnsiString            inDescription,
                MMLoyaltyResponseCode inResponseCode,
                AnsiString            inUUID)
            : IsSuccesful(inSuccess),
              Message(inMessage),
              Description(inDescription),
              ResponseCode(inResponseCode),
              UUID(inUUID)
        {
        }
};

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

        TMembership *membership;

    public:

         // Constructor and Destructor
        TLoyaltyMateInterface();
        ~TLoyaltyMateInterface();

        // sets the site id
        void RefreshSiteID();

        // creates a member in the cloud
        // returns a globally unique identifier so the POS can store it on the card and in the database for future use
        MMLoyaltyServiceResponse CreateMember(
									TSyndCode      syndicateCode,
									TMMContactInfo contactInfo,
                                    AnsiString     &uuid );

        // updates member details using a uuid
        MMLoyaltyServiceResponse UpdateMember(
									TSyndCode             syndicateCode,
									AnsiString            uuid,
									const TMMContactInfo* const contactInfo);

        // deletes a member in the cloud
        MMLoyaltyServiceResponse DeleteMember(
                                    TSyndCode  syndicateCode,
                                    AnsiString uuid );

        // retrieve member details using a uuid
        MMLoyaltyServiceResponse GetMemberDetails(
                                    TSyndCode       syndicateCode,
                                    AnsiString      uuid,
                                    TMMContactInfo &contactInfo,
                                    bool replacePoints = false );

       // retrieve member details using a uuid
        MMLoyaltyServiceResponse GetMemberDetailsByCode(
                                    TSyndCode       syndicateCode,
                                    AnsiString      memberCode,
                                    TMMContactInfo &contactInfo,
                                    bool replacePoints = false );

       MMLoyaltyServiceResponse GetMemberDetailsByEmail(
                                    TSyndCode       syndicateCode,
                                    AnsiString      memberEmail,
                                    TMMContactInfo &contactInfo,
                                    bool replacePoints = false );

        // posts members transactions on to the cloud
        MMLoyaltyServiceResponse PostTransaction(
                                            TSyndCode syndicateCode,
                                            AnsiString uuid,
                                            Currency pointsBalance,
                                            Currency pointsDelta,
                                            TDateTime occurredDate,
                                            int pointsType);

        // Save contact info to DB. No need for calling this method
        // if AutoSync = true
        void SyncContactInfo( const TMMContactInfo* const inContactInfo );

        // Save points to DB. No need for calling this method
        // if AutoSync = true
        void SyncPoints( const TMMContactInfo* const inContactInfo );

        // Public Properties
        __property bool AutoSync =
        {
	        read = FAutoSync, write = FAutoSync
        };

        MMLoyaltyServiceResponse CreateTier(TSyndCode syndicateCode,TTierLevel *TierLevel);
        MMLoyaltyServiceResponse UpdateTier(TSyndCode syndicateCode,TTierLevel *TierLevel);
        MMLoyaltyServiceResponse DeleteTier(TSyndCode syndicateCode,TTierLevel *TierLevel);
        MMLoyaltyServiceResponse GetTier(TSyndCode syndicateCode,TTierLevel *TierLevel);
        MMLoyaltyServiceResponse SyncTierLevels(TSyndCode syndicateCode);
        MMLoyaltyServiceResponse UpdateMemberCardCode(TSyndCode syndicateCode,AnsiString uniqueId,AnsiString memberCardCode);
        void syncTierLevels(DynamicArray<TierLevelInfo*> tierLevels);
    private:
        // initiates the Loyaltymate WCF Client
        void initLMClient();

        // initiates the Membership object
        void initMembership();

        // initiates the Site ID. Read the value from the DB
        void initSiteID();

        // initiates the AutoSync property.
        // AutoSync = true means both the External Key and the Points
        // are synced with their values in the cloud automatically
        void initAutoSync();

        // initiates the Loyaltymate WCF Client
        void doneMembership();

        // searches through menumate contact table to get the site id
        int getCurrentSiteId();

        // creates the contact name with Loyalty WCF format
        AnsiString createWCFName( const TMMContactInfo* const contactInfo );

        void CreateWcfContactInfo(MemberInfo* inMemberInfo,TMMContactInfo& inContactInfo);

        // reads the contact info from the WCF response
        void readContactInfo(
					LoyaltyMemberResponse* inWCFResponse,
					TMMContactInfo&            outContactInfo,
                    bool                       replacePoints = false );

        // reads the contact info list from the WCF member info
        void readContactInfo(
					MemberInfo* inMemberInfo,
					TMMContactInfo& contactInfo,
                    bool            replacePoints = false );

        // syncs contact info coming with the cloud with the db's
        void syncContactInfo( const TMMContactInfo* const inContactInfo );

        // syncs loyaltymate attr coming with the cloud with the db's
		void syncLoyaltymateAttrs(
                         const TMMContactInfo* const inContactInfo );

        // updates loyaltymate attr in the db's
	   bool updateLoyaltymateAttrs(
                         const TMMContactInfo* const inContactInfo );

        // add loyaltymate attr to the db's
		void addLoyaltymateAttrs(
                         const TMMContactInfo* const inContactInfo );

        // syncs contact points coming with the cloud with the db's
	void syncContactPoints(
                         const TMMContactInfo* const inContactInfo );

        // creates MMResponse from LoyaltyResponse
        MMLoyaltyServiceResponse createMMResponse(
                                    LoyaltyResponse* inWCFResponse );

        // creates MMResponse from LoyaltyMemberResponse
        MMLoyaltyServiceResponse createMMResponse(
                                    LoyaltyMemberResponse* inWCFResponse );

        // creates MMResponse from LoyaltyMemberListResponse
        MMLoyaltyServiceResponse createMMResponse(
                                    LoyaltyMemberListResponse* inWCFResponse );

        MMLoyaltyServiceResponse createMMResponse(
                            LoyaltyTierResponse* inWCFResponse );

        MMLoyaltyServiceResponse createMMResponse(
                            LoyaltyPointsInfoResponse* inWCFResponse );

        MMLoyaltyServiceResponse createMMResponse(
                            LoyaltyTierListResponse* inWCFResponse );

        // creates MMResponse from an exception message
        MMLoyaltyServiceResponse createExceptionFailedResponse(
                                    AnsiString inMessage );

		// generates a table key
		int generateTableKey( const AnsiString inGeneratorName,
                              TDBControl* const inDBControl );

		// executes an SQL query
		int executeQuery(
                const AnsiString  inGeneratorName,
                TDBControl* const inDBControl );

       // add loyaltymate attr to the db's for tier level
        void UpdateTierCloudId(const TTierLevel* const inTierInfo );
        int GetTierCloudId(int tierLevel);
        int GetTierLevelFromCloudId(int cloudId);
        void readTierInfo(TierLevelInfo* inTierInfo,TTierLevel *TierLevel);
};

#endif
