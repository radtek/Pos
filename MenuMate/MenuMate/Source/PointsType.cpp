//---------------------------------------------------------------------------


#pragma hdrstop

#include "PointsType.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

bool operator == (TPointsType &x,TPointsType &y)
{
   if (  (x.AccType.first == y.AccType.first) &&
   		 (x.AccType.second == y.AccType.second) &&
   		 (x.ExportStatus == y.ExportStatus) &&
         (x.Source == y.Source)
         )
      {
         return true;
      }
   return false;
};

bool operator < (const TPointsType &x,const TPointsType &y)
{
		bool Retval = false;
        if(x.ExportStatus > y.ExportStatus)
        { // The > is delibrate and there to but Held trasactions Last.
            Retval = true;
        }
        else if (x.ExportStatus == y.ExportStatus)
        {
            if(x.AccType.second < y.AccType.second)
            { // Account type.
                Retval = true;
            }
            else if (x.AccType.second == y.AccType.second)
            {
                if(x.AccType.first < y.AccType.first)
                {
                    Retval = true;
                }
                else if (x.AccType.first == y.AccType.first)
                {
                    if(x.Source < y.Source)
                    {
                        Retval = true;
                    }
                }
            }
        }
		return Retval;
};

TPointsType::TPointsType(TPointsAssignedSource inSouce, TPointsTypePair inAccType,TPointsExportStatus inExportStatus)
{
    Source = inSouce;
    AccType = inAccType;
    ExportStatus = inExportStatus;
}

UnicodeString TPointsType::Name() const
{
    return GetPointsTypeName(*this);
}

UnicodeString TPointsType::GetPointsTypeName(TPointsType PtsType)
{
   UnicodeString pre = "Unknown";
   UnicodeString post = "Unknown";
   UnicodeString status = "";
    // enum TPointsTransactionAccountType {ptstInvalid, ptstHeld, ptstLoyalty, ptstAccount};
    switch(PtsType.ExportStatus)
    {
        case pesNone :
        {
            if(PtsType.Source == pasDatabase)
                status = "Held";
        }
        break;
        case pesExported :
        {
            status = "";
        }
        break;
        case pesCorrupted :
        {
            status = "Corrupted";
        }
        break;
        default :
        {
            status = "Unknown ";
        }
        break;
    }

    switch(PtsType.AccType.second)
    {
        case ptstInvalid :
        {
            pre = "Unknown";
        }
        break;
        case ptstLoyalty :
        {
            pre = "Loyalty";
        }
        break;
        case ptstAccount :
        {
            pre = "Account";
        }
        break;
        default :
        {
            pre = "Unknown";
        }
        break;
    }

    //enum TPointsTransactionType {pttInvalid,pttSystem, pttReward, pttPurchased,pttEarned,pttRedeemed,pttSync/*pttEarntSpent,pttLoaded*/};
    switch(PtsType.AccType.first)
    {
        case pttInvalid :
        {
            post = "Unknown";
        }
        break;
        case pttRefund :
        {
            post = "Refunded";
        }
        break;
        case pttPurchased :
        {
            post = "Purchased";
        }
        break;
        case pttEarned :
        {
            post = "Earned";
        }
        break;
        case pttRedeemed :
        {
            post = "Redeemed";
        }
        break;
        case pttRedeemedBD :
        {
            post = "Redeemed BirthDay";
        }
        break;
        case pttRedeemedFV :
        {
            post = "Redeemed First Visit";
        }
        break;
        case pttSync :
        {
            post = "Sync";
        }
        break;
        case pttBirthdayBonus :
        {
            post = "Birthday Rewards";
        }
        break;
        case pttFirstVisit :
        {
            post = "First Visit";
        }
        break;
        default :
        {
            post = "Unknown";
        }
        break;
    }
   status = (status == "" ? UnicodeString("") : UnicodeString(status + " "));
   return status + pre + " " + post;
}


