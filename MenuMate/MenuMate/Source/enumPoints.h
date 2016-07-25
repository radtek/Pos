#ifndef enumPointsH
#define enumPointsH

#include <utility>
//---------------------------------------------------------------------------
/* Note : Please DO NOT Change the enums on this page unless you also update the
 advance(EnumType) function in ContactPoints.cpp . */


/* Switching Systems
	If the System switched from a rewards (earned) system to a rewards and accounts system
    no changes take place behind the scenes.
    Earned points no longer become avaiable for spend in the Payment Gui.
    Earned points are repersented by a System discount using the new "Max_Value" field, which
    will be set to the Total of the members Earned Points.

    The members points to redeem and purchase on the payment sceen repersent his
    ptstAccount points.

    Switching back.
    The system will no longer create the Earned Points discount.
    Points of any sub type will appear on the payment screen for use.
    Points of the ptstLoyalty will appear reguardless of Balance.
	Points of the ptstAccount type will appear only if the Balance is not Zero allowing them
    to be spent untill they are gone.
*/

/* Point Types : Alterations MenuMate v 5.17??

Old version of the enumeration.
enum TPointsTransactionType {pttInvalid,pttSystem, pttReward, pttPurchased,pttEarned,pttRedeemed,pttSync,pttEarntSpent,pttLoaded};

pttInvalid is there only to supply a default, Invalid constructor value.

pttSystem is not used by code, it was there for developers to use when adding points
	transactions manually to the DB to fix up someones account, as it's not used by code
    there is no point having it, they should use pttSync instead for manual DB adjustments and
    mark thiem as none exported to get them out to the smart cards.

pttReward was Reserved for future use when we have a rewards system. Having it in Code now
	is jsut a red hearing, and adds confusion, It has therefore been dropped.
    This field has therefore been removed.

pttPurchased Marks all points trasaction where cash was paid for those points.
pttEarned Marks all points trasaction where points were awrded for purchases.
pttRedeemed Marks all tranactions where points were used to pay for purchases.
pttSync Used by the system to "square up" the points transaction DB.
	This is mostly seen in smart card sites where the vaule on the card is different
    to the value in the DB because the cards was used on other sites.

pttEarntSpent Is not longer required and was removed.
	All pttEarntSpent has been coverted to pttRedeemed with a sub type of ptstLoyalty.


    7 -> 5 on colum ADJUSTMENT_TYPE  && ADJUSTMENT_SUBTYPE = ptstLoyalty (1) POINTSTRANSACTIONS

OlD ENUM
enum TPointsTransactionType {pttInvalid,pttSystem, pttReward, pttPurchased,pttEarned,pttRedeemed,pttSync,pttEarntSpent,pttLoaded};
NEW ENUM
enum TPointsTransactionType {pttInvalid,pttPurchased,pttEarned,pttRedeemed,pttSync};

The Followign SQL should fix up ther transaction table.
update POINTSTRANSACTIONS set ADJUSTMENT_SUBTYPE = 1;
update POINTSTRANSACTIONS set ADJUSTMENT_TYPE = 1 WHERE ADJUSTMENT_TYPE == 3;
update POINTSTRANSACTIONS set ADJUSTMENT_TYPE = 2 WHERE ADJUSTMENT_TYPE == 4;
update POINTSTRANSACTIONS set ADJUSTMENT_TYPE = 3 WHERE ADJUSTMENT_TYPE == 5;
update POINTSTRANSACTIONS set ADJUSTMENT_TYPE = 4 WHERE ADJUSTMENT_TYPE == 6;
update POINTSTRANSACTIONS set ADJUSTMENT_TYPE = 5 WHERE ADJUSTMENT_TYPE == 7;
update POINTSTRANSACTIONS set ADJUSTMENT_TYPE = 1, ADJUSTMENT_SUBTYPE = 2 WHERE ADJUSTMENT_TYPE == 8;

enum TPointsTransactionAccountType {ptstInvalid, ptstLoyalty, ptstAccount};


    // TODO 1 -o Michael -c Incomplete :
    This needs to be done by the 5.16/17? parser. this enum has been dropped.

pttLoaded Is not longer required and was removed.
All pttLoaded has been coverted to pttPurchased with a sub type of ptstAccount.

    // TODO 1 -o Michael -c Incomplete :
    This needs to be done by the 5.16/17? parser. this enum has been dropped.


The TOTAL_SPENT field in the CONTACTS table, looks like it been used not for
the total customers spend but to store the new points balance.

    // TODO 1 -o Michael -c Incomplete :
    The 5.16 Parser should remove the TOTAL_SPENT, EARNT_POINTS, LOADED_POINTS and INITIAL_EARNT_POINTS
    from the Contacts table. These values are now calculated by summing the appropreate fields
    in the POINTSTRANSACTIONS table or looking up the ARCHIVES.

    The Parser will need add a pttSync record to bring each member into line before these fields are dropped.
    Initially I suggest you add the pttSync record. Have a look where this is used in the menuamte code,
    i.e on Smartcard insertaion and do a simlar thing for all members in the DB.

    As of now the fields are kept for reporting in Back office. But once the back office
    reporting is moved to use the points transaction tables (it may already use them)
    these fields should be dropped to stop futher development that might use them by mistake.

	// TODO 1 -o Michael -c Incomplete : AT THE VERY LEAST you will need to add the pttSync
    to the points transaction table to ensure the totals match what the members currently
    have in there contacts tables.

    // TODO 1 -o Michael -c Incomplete : The BuildXMLListMembership also uses these feilds
    and should be updated for XML export to whereever.

/* The following repersents a Type of Points.

	All points are currently considered ptstLoyalty points.
    If you are running the AU Loyalty system you can also have ptstAccount points.
	If you are running the AU Loyalty system you can only spend ptstLoyalty via the discount interface.
    This Discount is automatcially created by the system when the AU Points setting is selected.

    Point with no type i.e ptstInvalid are treated as ptstLoyalty points.
    They are converted to this type by the Parser in 5.16
*/
enum TPointsTransactionType {pttInvalid, pttPurchased, pttEarned, pttRedeemed, pttSync, pttBirthdayBonus, pttFirstVisit, pttRefund,
                             pttRedeemedBD,pttRedeemedFV,pttReset};

enum TPointsTransactionAccountType {ptstInvalid, ptstLoyalty, ptstAccount};

typedef std::pair<TPointsTransactionType,TPointsTransactionAccountType> TPointsTypePair;

/* Used by the smart card system to mark if points have been successfully exported to the
members Card*/
enum TPointsExportStatus {pesNone, pesExported, pesCorrupted};

/* Used to determin where the points transaction came from.
pasDatabase : The original values loaded from the DB.
pasUser : Loaded by staff @ The Customer request aka manual purchase or redemtion.
pasDiscount : Loaded by the system because they are using Aussie loyalty and
 have redeemed some points via the discount interface.
pasCloud : Loaded from the cloud when assigning new cards using LoyaltyMate.*/
enum TPointsAssignedSource {pasDatabase, pasUser, pasDiscount, pasCloud};

/*Used to determine which reset point transaction should occur*/
enum TResetPoints{All, PurchaseEarn, Purchase, PurchaseRedeem, EarnRedeem, Earn, Redeem};
//---------------------------------------------------------------------------
#endif
