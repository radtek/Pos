//---------------------------------------------------------------------------

#ifndef ManagerClippIntegrationHelperH
#define ManagerClippIntegrationHelperH

//---------------------------------------------------------------------------

#include "DeviceRealTerminal.h"
#include "ClippIntegrationInterface.h"
#include <Extctrls.hpp>
#include "DBTab.h"
#include "DBClippTab.h"
#include "MMContactInfo.h"
#include "ContactMemberApplied.h"


class TManagerClippIntegrationHelper
{
private:

     TMMContactInfo MemberInfo;
     int contactsKey;
     eMemberSource MemberSource;
     TContactMemberApplied Membership;

    //Get the tab details from the database
    //Create the payment transaction from tab data..
    TPaymentTransaction* CreatePaymentTransaction(Database::TDBTransaction &dbTransaction, int tabKey, std::vector<MMClippDiscount> clippDiscounts);
    TPaymentTransaction CreatePartialPaymentTransaction(Database::TDBTransaction &dbTransaction, int tabKey, std::vector<MMClippDiscount> clippDiscounts, Currency tipAmount = 0.0 );

public:

    //Factory methods to create the entities from each other..
    TClippTab CreateClippTab(MMClippMessage mmClippMessage);

    void UpdateClippPaymentAmount(MMPaymentDetailResponse mmPaymentDetailResponse);

    //Factory methods to create the entities from each other..
    MMClippTabDetail CreateMMClippTabDetail(int tabKey, UnicodeString recievedMessageId, std::vector<MMClippDiscount> clippDiscounts, Currency tipAmount = 0.0);
    MMClippTabDetail CreateMMClippTabDetail(int tabKey, UnicodeString recievedMessageId, Currency tipAmount, Currency processingFeesAmount, std::vector<MMClippDiscount> clippDiscounts);

    //Factory method to create payment details request data from raw message recieved from clipp app..
    MMPaymentDetailRequest CreateMMPaymentDetailRequest(int tabKey, UnicodeString recievedMessageId,
                                                Currency totalPaymentAmount, Currency clippProcessingFees);

    //This method will finally close the tab upon validating the data recieved from the cipp app..
    void FinalizeClippTab(MMClippMessageDetail mmClippMessageDetail);

    //method to apply dynamic discount when discount is venue specific and applied when closing tab from app.
    void ApplyDiscountOnTabClose(TPaymentTransaction &clippTabTransaction, std::vector<MMClippDiscount> clippDiscounts, Currency *totalVenueDiscount);

    //calculate tax on venue discount if recalculate tax after applying discount setting is on
    void AddtaxOnDiscountToTotal(TPaymentTransaction &clippTabTransaction);

    //Add  tax with venue discount if recalculate tax after applying discount setting is on
    void CalculateTaxOnClippVenueDiscount(TPaymentTransaction &clippTabTransaction, Currency clippVenueDiscount);

     //Add servicecharge with  tax on venue discount According to tax settings
    void AddServiceChargeWithtax (TPaymentTransaction &clippTabTransaction);

     //Add tax on venue discount According to tax settings
    void AddTaxOnVenueDiscount (TPaymentTransaction &clippTabTransaction);

    //Create Dynamic Discount for clipp Venue
    TDiscount CreateMMDiscount(std::vector<MMClippDiscount>::iterator it);

    //Apply Processing fee as surcharge
    void ApplyProcessingFeeSurcharge(TPaymentTransaction &clippTabTransaction, Currency processingFeesAmount);

    //Create Clipp Processing fee surcharge
    TDiscount CreateClippSurcharge(Currency processingFeesAmount);
};

#endif
