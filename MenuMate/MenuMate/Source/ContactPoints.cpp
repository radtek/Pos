//---------------------------------------------------------------------------


#pragma hdrstop

#include "ContactPoints.h"
#include "Stream.h"
#include "SmartCardAPI.h"
#include "rounding.h"
#include "GlobalSettings.h"
#include "StringTableRes.h"
#include "ItemMinorComplete.h"
#include <math.h>
#include <algorithm>
//---------------------------------------------------------------------------

#pragma package(smart_init)


TContactPoints::TContactPoints()
{
	Clear();
}


void TContactPoints::Assign(TContactPoints &inPoints)
{
   PointsStore = inPoints.PointsStore;
}

void TContactPoints::Clear()
{
    PointsStore.clear();
    PointsRules.Clear();
    //PointsRules += TGlobalSettings::Instance().SystemRules;
    ExportStatus = pesNone;
}

void TContactPoints::ClearBySource(TPointsAssignedSource inSource)
{
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();)
	{
		if(ptrPointsTransaction->first.Source == inSource)
		{
			PointsStore.erase(ptrPointsTransaction++);
      	}
        else
        {
        	ptrPointsTransaction++;
        }
   }
}

/* Remvoes all Points allocated by a certain type.
Will not remove points that came from the DB as they, well came from a source
that cannot be 'updated/deleted' by this interface.*/

void TContactPoints::ClearByAccountType(TPointsTypePair PointsToClear)
{
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();)
	{
		if(ptrPointsTransaction->first.AccType == PointsToClear &&
        	ptrPointsTransaction->first.Source != pasDatabase)
		{
			PointsStore.erase(ptrPointsTransaction++);
      	}
        else
        {
        	ptrPointsTransaction++;
        }
   }
}

/* Removes all points allocated since this structure was loaded.*/
void TContactPoints::ResetPoints()
{
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();)
	{
		if(ptrPointsTransaction->first.Source != pasDatabase)
		{
			PointsStore.erase(ptrPointsTransaction++);
      	}
        else
        {
        	ptrPointsTransaction++;
        }
   }
}

void TContactPoints::LoadFromStreamVer1(TMemoryStream *Stream)
{
   // IN version 1 the version stream was an interger in version 2 its gone, see
   // SMART_CARD_VERSION_TWO
   Currency fCurrentPoints = 0;

   int Ver1Version;
   StreamRead(Stream,Ver1Version);
   StreamRead(Stream,fCurrentPoints);

   TPointsTypePair typepair(pttEarned,ptstLoyalty);
   TPointsType type(pasDatabase, typepair,pesExported);
   Load(type,fCurrentPoints);

}

void TContactPoints::LoadFromStream(int CardVersion,TMemoryStream *Stream)
{
	switch(CardVersion)
	{
	  	case SMART_CARD_VERSION_TWO :
		case SMART_CARD_VERSION_THREE :
		{
			Currency fCurrentPoints = 0;
			StreamRead(Stream,fCurrentPoints);
            TPointsTypePair typepair(pttEarned,ptstLoyalty);
            TPointsType type(pasDatabase, typepair,pesExported);
            Load(type,fCurrentPoints);
		}break;
		case SMART_CARD_VERSION_FOUR :
		case SMART_CARD_VERSION_FIVE :
        case SMART_CARD_VERSION_SIX :
		{
			Currency fCurrentPoints = 0;
			Currency fLoadedPoints = 0;
			Currency fEarntPoints = 0;
			StreamRead(Stream,fCurrentPoints);
			StreamRead(Stream,fLoadedPoints);
			StreamRead(Stream,fEarntPoints);

            double balance = 0;

            if( fCurrentPoints != 0 )
            {
                // comparing current, earned and loaded points to fix points issues when upgrading from older menumate versions
                if( fCurrentPoints != 0
                    && fEarntPoints == 0
                    && fLoadedPoints == 0 )
                {
                    /* in previous versions 5.18, the smart card preloader writes the points into currentpoints section.
                    also the currentpoints section is used to store points balance for nz and aussi loyalty.
                    here we check whether we have some balance in the current points section
                    and only if the other points values are empty we mark it as the only balance for the card */
                    balance = fCurrentPoints;
                }
                else if( fCurrentPoints != fEarntPoints + fLoadedPoints )
                {
                     /* card is not synced, and has a variance between different points sections. Calculate the difference and depending on
                        whether this value is a plus or minus, the points will be either added or removed from the appropriate areas later */

                     balance = fCurrentPoints - (fEarntPoints + fLoadedPoints);
                }
                else if( fCurrentPoints == fEarntPoints + fLoadedPoints )
                {
                    /* perfectly synced card either from earlier versions of current versions.
                      clearing the current points value so it will not affect other sections in the system if used */

                    fCurrentPoints = 0;
                }

                /* adding the balance to appropriate points section
                    either earned or loaded sections depending on the loyalty type. */

                if( TGlobalSettings::Instance().EnableSeperateEarntPts )
                    fLoadedPoints += balance; // aussi only uses loaded points.
                else
                    fEarntPoints += balance;  // nz only uses earned points
            }

            TPointsTypePair typepair(pttEarned,ptstLoyalty);
            TPointsType type(pasDatabase, typepair,pesExported);
            Load(type,fEarntPoints);

            TPointsTypePair typepair2(pttPurchased,ptstAccount);
            TPointsType type2(pasDatabase, typepair2,pesExported);
            Load(type2,fLoadedPoints);
		}
		break;
	}
}
//---------------------------------------------------------------------------
void TContactPoints::SaveToStream(TMemoryStream *Stream)
{
    Currency Value = 0.0; // Before: Currency Value = getPointsBalance(); , Now: Ignore current points value as we no longer use that
	StreamWrite(Stream,Value);
    Value = getPointsBalance(ptstAccount);
	StreamWrite(Stream,Value);
    Value = getPointsBalance(ptstLoyalty);
	StreamWrite(Stream,Value);
}

Currency TContactPoints::GetPointsValue(TPointsType type)
{
    return PointsStore[type];
}

/* This function and the following one has a special case as we are getting
the Points by Source or Account that source may contain both + and - pttRedeemed values.
These values are stored as + numbers in Code but converted to - numbers
when put in the DB, as it makes reporting easier.
We need to check and if pttRedeemed value is greater than 0 if so  then we deduct it
Otherwise we jsut add the negitive number which has the equivilent result */

Currency TContactPoints::GetPointsValue(TPointsTypePair type)
{
    Currency TypeTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end(); ptrPointsTransaction++)
	{
		if(ptrPointsTransaction->first.AccType == type)
		{
                    if((ptrPointsTransaction->first.AccType.first == pttRedeemed ||
                        ptrPointsTransaction->first.AccType.first == pttRedeemedBD ||
                        ptrPointsTransaction->first.AccType.first == pttRedeemedFV ||
                        (ptrPointsTransaction->first.AccType.first == pttRefund && !TGlobalSettings::Instance().RefundingItems))
                        && (ptrPointsTransaction->second > 0))
                    {
                            TypeTotalValue -= ptrPointsTransaction->second;
                    }
                    else
                    {
                            TypeTotalValue += ptrPointsTransaction->second;
                    }
      	}
   }
   return TypeTotalValue;
}

Currency TContactPoints::GetPointsValue(TPointsAssignedSource inSource)
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if(ptrPointsTransaction->first.Source == inSource)
		{
            if((ptrPointsTransaction->first.AccType.first == pttRedeemed ||
                ptrPointsTransaction->first.AccType.first == pttRedeemedBD ||
                ptrPointsTransaction->first.AccType.first == pttRedeemedFV) &&
                (ptrPointsTransaction->second > 0))
            {
                    SourceTotalValue -= ptrPointsTransaction->second;
            }
            else
            {
            	SourceTotalValue += ptrPointsTransaction->second;
            }
      	}
   }
   return SourceTotalValue;
}

/* Excludes values loaded from the DB, so will return just whats new in the
   current sale, i.e what has been modified in the Points Stroe since it was
   loaded from the DB.*/
Currency TContactPoints::GetCurrentPointsValue(TPointsTypePair type)
{
    Currency TypeTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end(); ptrPointsTransaction++)
	{
		if(ptrPointsTransaction->first.Source != pasDatabase &&
                   ptrPointsTransaction->first.AccType == type)
		{
                    if((ptrPointsTransaction->first.AccType.first == pttRedeemed  ||
                        ptrPointsTransaction->first.AccType.first == pttRedeemedBD ||
                        ptrPointsTransaction->first.AccType.first == pttRedeemedFV ||
                        (ptrPointsTransaction->first.AccType.first == pttRefund && !TGlobalSettings::Instance().RefundingItems))
                        && (ptrPointsTransaction->second > 0 ))
                    {
                        TypeTotalValue -= ptrPointsTransaction->second;
                    }
                    else
                    {
                        TypeTotalValue += ptrPointsTransaction->second;
                    }
      	}
   }
   return TypeTotalValue;
}

Currency TContactPoints::getPointsBalance(TPointsTransactionAccountType Account)
{
    return  GetPointsValue(TPointsTypePair(pttEarned, Account)) +
            GetPointsValue(TPointsTypePair(pttFirstVisit, Account)) +
            GetPointsValue(TPointsTypePair(pttBirthdayBonus, Account)) +
            GetPointsValue(TPointsTypePair(pttPurchased, Account)) +
            GetPointsValue(TPointsTypePair(pttRedeemed, Account)) +
            GetPointsValue(TPointsTypePair(pttRefund, Account)) +
            GetPointsValue(TPointsTypePair(pttSync, Account))+
            GetPointsValue(TPointsTypePair(pttRedeemedBD, Account))+
            GetPointsValue(TPointsTypePair(pttRedeemedFV, Account));
}

Currency TContactPoints::getPointsEarned(TPointsTransactionAccountType Account)
{
	return GetPointsValue(TPointsTypePair(pttEarned,Account)) +
           GetPointsValue(TPointsTypePair(pttSync,Account));
}

Currency TContactPoints::getPointsRedeemed(TPointsTransactionAccountType Account)
{
	return GetPointsValue(TPointsTypePair(pttRedeemed,Account)) +
           GetPointsValue(TPointsTypePair(pttRedeemedBD,Account)) +
           GetPointsValue(TPointsTypePair(pttRedeemedFV,Account)) +
           GetPointsValue(TPointsTypePair(pttSync,Account));
}

Currency TContactPoints::getPointsPurchased(TPointsTransactionAccountType Account)
{
	return GetPointsValue(TPointsTypePair(pttPurchased,Account)) +
           GetPointsValue(TPointsTypePair(pttSync,Account));
}

Currency TContactPoints::getPointsRefunded(TPointsTransactionAccountType Account)
{
    return GetPointsValue(TPointsTypePair(pttRefund, Account)) +
           GetPointsValue(TPointsTypePair(pttSync, Account));
}

Currency TContactPoints::getPointsBalance(TPointsAssignedSource Source)
{
	return GetPointsValue(Source);
}

Currency TContactPoints::getPointsBalance(TPointsAssignedSource Source, TPointsTransactionAccountType Account)
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source == Source &&
        	     ptrPointsTransaction->first.AccType.second == Account)
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	        }
   }
   return SourceTotalValue;
}

Currency TContactPoints::getPointsEarned(TPointsAssignedSource Source)
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source == Source &&
        	(ptrPointsTransaction->first.AccType.first == pttEarned ||
        	ptrPointsTransaction->first.AccType.first == pttSync))
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	}
   }
   return SourceTotalValue;
}

Currency TContactPoints::getPointsRedeemed(TPointsAssignedSource Source)
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source == Source &&
        	(ptrPointsTransaction->first.AccType.first == pttRedeemed ||
             ptrPointsTransaction->first.AccType.first == pttRedeemedBD ||
             ptrPointsTransaction->first.AccType.first == pttRedeemedFV ||
             ptrPointsTransaction->first.AccType.first == pttSync))
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	}
   }
   return SourceTotalValue;
}

Currency TContactPoints::getPointsRefunded(TPointsAssignedSource Source)
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end(); ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source == Source &&
        	(ptrPointsTransaction->first.AccType.first == pttRefund))
		{
            if(!TGlobalSettings::Instance().RefundingItems)
            {
			    SourceTotalValue += ptrPointsTransaction->second;
            }
            else
            {
                  SourceTotalValue -= ptrPointsTransaction->second;
            }
      	}
   }
   return SourceTotalValue;
}

Currency TContactPoints::getPointsPurchased(TPointsAssignedSource Source)
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source == Source &&
        	(ptrPointsTransaction->first.AccType.first == pttPurchased ||
        	ptrPointsTransaction->first.AccType.first == pttSync))
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	}
   }
   return SourceTotalValue;
}

Currency TContactPoints::getPointsBalance()
{
    Currency Total = 0;
	std::map<TPointsType, Currency>::iterator ptr = PointsStore.begin();
    for(; ptr != PointsStore.end(); advance(ptr,1))
    {
        if((ptr->first.AccType.first == pttRedeemed ||
            ptr->first.AccType.first == pttRedeemedBD ||
            ptr->first.AccType.first == pttRedeemedFV ) &&
            ptr->second > 0 )
        {
			    Total -= ptr->second;
        }
        else
        {
    		Total += ptr->second;
        }

    }
	return Total;
}

Currency TContactPoints::getPointsEarned()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( (ptrPointsTransaction->first.AccType.first == pttEarned ||
        	ptrPointsTransaction->first.AccType.first == pttSync))
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	}
   }
   return SourceTotalValue;
}

Currency TContactPoints::getBirthDayRewardPoints()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if(ptrPointsTransaction->first.AccType.first == pttBirthdayBonus)
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	        }
   }
   return SourceTotalValue;
}

Currency TContactPoints::getFirstVisitPoints()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if(ptrPointsTransaction->first.AccType.first == pttFirstVisit)
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	        }
   }
   return SourceTotalValue;
}

Currency TContactPoints::getBirthDayRewardRedeemedPoints()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if(ptrPointsTransaction->first.AccType.first == pttRedeemedBD)
		{
			SourceTotalValue += ptrPointsTransaction->second;
        }
   }
   return SourceTotalValue;
}

Currency TContactPoints::getFirstVisitRedeemedPoints()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if(ptrPointsTransaction->first.AccType.first == pttRedeemedFV)
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	        }
   }
   return SourceTotalValue;
}

Currency TContactPoints::getPointsRedeemed()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
          if(ptrPointsTransaction->first.AccType.first == pttRedeemed   ||
             ptrPointsTransaction->first.AccType.first == pttRedeemedBD ||
             ptrPointsTransaction->first.AccType.first == pttRedeemedFV ||
        	 ptrPointsTransaction->first.AccType.first == pttSync)
	    {
	       SourceTotalValue += ptrPointsTransaction->second;
        }
   }
   return SourceTotalValue;
}

Currency TContactPoints::getPointsRefunded()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
          ptrPointsTransaction != PointsStore.end(); ptrPointsTransaction++)
    {
        if((ptrPointsTransaction->first.AccType.first == pttRefund))
        {
            if(!TGlobalSettings::Instance().RefundingItems)
            {
			    SourceTotalValue -= ptrPointsTransaction->second;
            }
            else
            {
                  SourceTotalValue += ptrPointsTransaction->second;
            }
        }
    }
    return SourceTotalValue;
}

Currency TContactPoints::getPointsPurchased()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if((ptrPointsTransaction->first.AccType.first == pttPurchased ||
        	ptrPointsTransaction->first.AccType.first == pttSync))
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	}
   }
   return SourceTotalValue;
}

Currency TContactPoints::getCurrentPointsEarned()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source != pasDatabase &&
        	(ptrPointsTransaction->first.AccType.first == pttEarned ||
            /* In a large number of cases pttSync will have a source of DB */
        	ptrPointsTransaction->first.AccType.first == pttSync))
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	}
   }
   return SourceTotalValue;
}

Currency TContactPoints::getCurrentPointsRedeemed()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source != pasDatabase &&
        	(ptrPointsTransaction->first.AccType.first == pttRedeemed ||
             ptrPointsTransaction->first.AccType.first == pttRedeemedBD ||
             ptrPointsTransaction->first.AccType.first == pttRedeemedFV ||
            /* In a large number of cases pttSync will have a source of DB */
        	ptrPointsTransaction->first.AccType.first == pttSync))
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	}
   }
   return SourceTotalValue;
}

Currency TContactPoints::getCurrentPointsRefunded()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source != pasDatabase &&
        	(ptrPointsTransaction->first.AccType.first == pttRefund) )
		{
            if(!TGlobalSettings::Instance().RefundingItems)
            {
			    SourceTotalValue -= ptrPointsTransaction->second;
            }
            else
            {
                  SourceTotalValue += ptrPointsTransaction->second;
            }
      	}
   }
   return SourceTotalValue;
}

Currency TContactPoints::getCurrentPointsRedeemed(TPointsTransactionAccountType Account)
{
	return GetCurrentPointsValue(TPointsTypePair(pttRedeemed,Account)) +
           GetCurrentPointsValue(TPointsTypePair(pttRedeemedBD,Account)) +
           GetCurrentPointsValue(TPointsTypePair(pttRedeemedFV,Account)) +
           GetCurrentPointsValue(TPointsTypePair(pttSync,Account));
}

Currency TContactPoints::getCurrentPointsPurchased()
{
    Currency SourceTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source != pasDatabase &&
        	(ptrPointsTransaction->first.AccType.first == pttPurchased ||
        	ptrPointsTransaction->first.AccType.first == pttSync))
		{
			SourceTotalValue += ptrPointsTransaction->second;
      	}
   }
   return SourceTotalValue;
}

Currency TContactPoints::getPointsHeldTotal()
{
    Currency HeldTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.ExportStatus == pesNone &&
        	ptrPointsTransaction->first.Source == pasDatabase)
		{
			HeldTotalValue += ptrPointsTransaction->second;
      	}
   }
   return HeldTotalValue;
}

Currency TContactPoints::getPointsHeldPurchased()
{
    Currency HeldTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.ExportStatus == pesNone &&
        	ptrPointsTransaction->first.Source == pasDatabase &&
            ptrPointsTransaction->first.AccType.first == pttPurchased)
		{
			HeldTotalValue += ptrPointsTransaction->second;
      	}
   }
   return HeldTotalValue;
}

Currency TContactPoints::getPointsHeldEarned()
{
    Currency HeldTotalValue = 0;
    for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.ExportStatus == pesNone &&
        	ptrPointsTransaction->first.Source == pasDatabase &&
            ptrPointsTransaction->first.AccType.first == pttEarned)
		{
			HeldTotalValue += ptrPointsTransaction->second;
      	}
   }
   return HeldTotalValue;
}

void TContactPoints::Load(TPointsType type,Currency Points)
{
    if (Points.Val % Currency(MIN_CURRENCY_VALUE).Val == 0)
	{
		PointsStore[type] = Points;
	}
	else
	{
		PointsStore[type] = RoundToNearest(Points, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
	}
}

/* This function doesn't just load, it accumlates the values into each type (+=)
  It's used is in taking Transactions loaded from the DB for held points
  and accumlating them for storage onto a smart cards. */
void TContactPoints::Add(std::vector <TPointsTransaction> &Transactions)
{
	  for (std::vector <TPointsTransaction> ::iterator ptrPointsTransaction = Transactions.begin();
		 ptrPointsTransaction != Transactions.end(); ptrPointsTransaction++)
		{

            TPointsTypePair Pair(ptrPointsTransaction->PointsTransactionType,ptrPointsTransaction->PointsTransactionAccountType);
            TPointsExportStatus Status = ptrPointsTransaction->ExportStatus;
            TPointsType Type(pasDatabase,Pair,Status);
            PointsStore[Type] += ptrPointsTransaction->Adjustment;
      	}
}

void TContactPoints::toHTML(TStringList *Report)
{
    UnicodeString Temp = LoadStr(TABLE_START);
    std::map<TPointsType, Currency>::iterator ptr = PointsStore.begin();
    for(; ptr != PointsStore.end(); advance(ptr,1))
    {
        UnicodeString TempRow = LoadStr(TABLE_ROW4);
        TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", ptr->first.Name());
        TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", FormatFloat("0.00", ptr->second));
        advance(ptr,1);
        if(ptr != PointsStore.end())
        {
            TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", ptr->first.Name());
            TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", FormatFloat("0.00", ptr->second));
        }
        else
        {
            TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", UnicodeString("&nbsp;"));
            TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", UnicodeString("&nbsp;"));
        }
        Report->Add(TempRow);
    }
    UnicodeString TempRow = LoadStr(TABLE_STOP);
    Report->Add(TempRow);
}

void TContactPoints::Recalc(TList *OrdersList,int memberType,bool isRefundTransaction)
{
     if(((TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate && memberType == 1) || (TGlobalSettings::Instance().MembershipType != MembershipTypeMenuMate )) && (!isRefundTransaction))
    {
        CalcEarned(OrdersList);
    }
	ReAssignRedeemed(OrdersList);
}

/*
	Used by CalcEarned for Points Sort.
*/
int __fastcall SortByPointsPercent(void *Item1,void *Item2)
{
    TItemMinorComplete *Order1 = (TItemMinorComplete *)Item1;
    TItemMinorComplete *Order2 = (TItemMinorComplete *)Item2;
    if (Order1->PointsPercent > Order2->PointsPercent)
    {
    	return -1;
    }
    else if (Order1->PointsPercent == Order2->PointsPercent)
    {
    	return -0;
    }
    else
    {
    	return 1;
    }
}

Currency TContactPoints::CalcLoadedPointsValue(TList *OrdersList)
{
  Currency TotalValue = 0;
  Currency LoadedPoints = getCurrentPointsRedeemed(ptstAccount);
  for (int i=0; i < OrdersList->Count; i++)
    {
      TItemMinorComplete *Order = (TItemMinorComplete *) OrdersList->Items[i];
      if(LoadedPoints > 0)
      {
         if(LoadedPoints > Order->PointRedeemed)
         {
            TotalValue += Order->AmountRedeemed;
         }
         else
         {
            TotalValue += ( LoadedPoints * (Order->AmountRedeemed / Order->PointRedeemed )) ;
         }
         LoadedPoints -= Order->PointRedeemed;
      }
    }
  return TotalValue;
}



/* Works though the Orders List and recalculates the amount of earned points
and updates the PointsStore.

This Order List is Flattened no special case for sides is required or should be called.*/
void TContactPoints::CalcEarned(TList *OrdersList)
{
    Currency TotalPointsEarned = 0;
    if(OrdersList->Count > 0)
    {
       /* You can earn point on either protion of ProductValue
           or if allowed (maybe some of the) points spent.*/
        Currency ProductValue = 0;
        Currency PointsRedeemed = 0;
        Currency TotalAssableValue = 0;
        Currency TotalPoints = 0;
        Currency RedeemPointsValue = 0;
        Currency LoadedPointsValue = 0;
        bool AllowedToEarnPoints = !PointsRules.Contains(eprNeverEarnsPoints);
        bool AllowedToEarnOnPointsSpend = PointsRules.Contains(eprEarnsPointsWhileRedeemingPoints);
        bool OnlyEarnsOnPointsSpend = PointsRules.Contains(eprOnlyEarnsPointsWhileRedeemingPoints);
        bool AllowedToEarnOnPointsSpend_Global = TGlobalSettings::Instance().SystemRules.Contains(eprEarnsPointsWhileRedeemingPoints);
        bool OnlyEarnsOnPointsSpend_Global = TGlobalSettings::Instance().SystemRules.Contains(eprOnlyEarnsPointsWhileRedeemingPoints);

        if(AllowedToEarnPoints)
        {
            std::auto_ptr<TList> SortedOrdersByPointsPercent(new TList);
            for (int i=0; i < OrdersList->Count; i++)
            {
                TItemMinorComplete *Order = (TItemMinorComplete *) OrdersList->Items[i];
                /* If the Order earns Points Added it */
                if(Order->PointsPercent != 0)
                {
                   SortedOrdersByPointsPercent->Add(Order);
                }
                RedeemPointsValue+= Order->AmountRedeemed;
            }
	    SortedOrdersByPointsPercent->Sort(&SortByPointsPercent);
            /* Add in the Cash portion of the spend.
            We do not have access to the payment types here, so we use the theritical
            maxium points you can earn less ALL redeemed points, then we add the
            redeemed points back on based on the points redeeming rules. */
            for (int i=0; i < SortedOrdersByPointsPercent->Count; i++)
            {
                TItemMinorComplete *Order = (TItemMinorComplete *) SortedOrdersByPointsPercent->Items[i];
                ProductValue += Order->TotalPriceAdjustment();
            }
            if(!TGlobalSettings::Instance().UseTierLevels)
            {
               RedeemPointsValue = fabs(getCurrentPointsRedeemed());
               LoadedPointsValue = fabs(getCurrentPointsRedeemed(ptstAccount));
            }
            else
            {
               LoadedPointsValue = CalcLoadedPointsValue(OrdersList);
            }

            /* If we are spliting point the the getCurrentPointsRedeemed() will
            return the discounted amount from the use of ptstLoyalty points.
            This has already been included in the above Order->TotalPriceAdjustment()
            so we exlude it here. */
            if(TGlobalSettings::Instance().EnableSeperateEarntPts)
            {
                TotalAssableValue = ProductValue - LoadedPointsValue;
            }
            else
            {
            	TotalAssableValue = ProductValue - RedeemPointsValue ;
            }

            if(AllowedToEarnOnPointsSpend || AllowedToEarnOnPointsSpend_Global)
            {
               PointsRedeemed += RedeemPointsValue;
            }

            if(OnlyEarnsOnPointsSpend || OnlyEarnsOnPointsSpend_Global)
            {
               /* You only earn based on ptstLoyalty points spent not cash
                     Loaded Points are in this as well so in this instance
                     are not treated as cash. */
                    PointsRedeemed = RedeemPointsValue;
                    TotalAssableValue = 0;
            }
            TotalAssableValue += PointsRedeemed;
            for (int i=0; i < SortedOrdersByPointsPercent->Count; i++)
                {
                    TItemMinorComplete *Order = (TItemMinorComplete *) SortedOrdersByPointsPercent->Items[i];
                    // Take the Total points spent.
                    // Spred them proportionally across all items that earn points.
                    // Calculate the points earned based on this spread.
                    // This has the effect of the customer using his points to firstly pay for
                    // items that earn him points as he only earns points on those points he redeems.
                    // If this is not done the customer would want to pay for his items that earn points
                    // with points in a speerate transaction and pay for everything else later.
                    if(fabs(TotalAssableValue) >= fabs(Order->TotalPriceAdjustment()))
                    {
                        Currency MaxPointsForThisItem;

                        bool isWeightedItem = Order->WeightedPrice.WeightedItem;
                        if(isWeightedItem && TGlobalSettings::Instance().EarntPointsOnWeight)
                        {
                            //Convert the weight to grams since the setting for loyalty points speaks points per gram.
                            double weight = double(Order->WeightedPrice.Weight.AsGrams());
                            MaxPointsForThisItem = double((weight * Order->PointsPercent) / 100.0);
                        }
                        else
                        {
                            MaxPointsForThisItem = double(Order->PointsPercent * double(Order->TotalPriceAdjustment()) / 100.0);
                        }

                        Order->PointsEarned = MaxPointsForThisItem;
                        TotalPointsEarned += Order->PointsEarned;
                        TotalAssableValue -= Order->TotalPriceAdjustment();
                    }
                    else if(TotalAssableValue > 0)
                    {
                        Order->PointsEarned = double(Order->PointsPercent * double(TotalAssableValue) / 100.0);
                        TotalPointsEarned += Order->PointsEarned;
                        TotalAssableValue -= TotalAssableValue;
                    }
                    else
                    {
                        Order->PointsEarned = 0;
                    }
                }
        }
        TPointsTypePair PointsToClear(pttEarned,ptstLoyalty);
        ClearByAccountType(PointsToClear);
        if(TotalPointsEarned == 0)
        {
            for (int i=0; i < OrdersList->Count; i++)
            {
                TItemMinorComplete *Order = (TItemMinorComplete *) OrdersList->Items[i];
                Order->PointsEarned = 0;
            }
        }
        else
        {
            Currency RoundedEarned = 0;
            if (TotalPointsEarned.Val % Currency(MIN_CURRENCY_VALUE).Val == 0)
            {
              RoundedEarned = TotalPointsEarned;
            }
            else
            {
              RoundedEarned = RoundToNearest(TotalPointsEarned, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
            }

            TPointsTypePair Pair(pttEarned,ptstLoyalty);
            TPointsType Type(pasUser,Pair,pesNone);
            Load(Type, RoundedEarned);
        }
   }
}

/*From the Points Store retrives the amount of points redeemed and spreads them
across all items prioritised based on whether those items earn them points
Order List is already Flattened, no special code for sides is required.*/
void TContactPoints::ReAssignRedeemed(TList *OrdersList)
{
   /* Redeeming.
   	1)	Total Points Redeemed by customer is proportionally spread across
   	all items on Receipt.
   */

   Currency TotalPrice = 0;

	for (int i = 0; i < OrdersList->Count ; i++)
	{
		TItemMinorComplete *Order = (TItemMinorComplete *)OrdersList->Items[i];
		TotalPrice += Order->TotalPriceAdjustment();
	}

   Currency TotalPointsRedeemed = getCurrentPointsRedeemed();

   if(TotalPointsRedeemed != 0 && TotalPrice != 0)
   {
      Currency PointsToDistribute = TotalPointsRedeemed;
      Currency PointsFraction =  PointsToDistribute/TotalPrice;
      // The PointsPerCent scaler is not allowed to be higher than 1
      for (int i=0; i < OrdersList->Count ; i++)
      {
         TItemMinorComplete *Order = (TItemMinorComplete *) OrdersList->Items[i];

         if( i ==  OrdersList->Count-1)
         {
            Order->Redeemed  =  PointsToDistribute;
         }
         else
         {
            if(TotalPrice != 0)
            {
               PointsFraction =  PointsToDistribute/TotalPrice;
            }
            else
            {
               PointsFraction = 0;
            }
            Order->Redeemed  = RoundToNearest(Order->Price() * PointsFraction,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
            PointsToDistribute -= Order->Redeemed;
            TotalPrice -= Order->TotalPriceAdjustment();
         }
      }
   }
   else
   {
      for (int i=0; i < OrdersList->Count; i++)
      {
         TItemMinorComplete *Order = (TItemMinorComplete *) OrdersList->Items[i];
         Order->Redeemed = 0;
      }
   }
}

TPointsExportStatus TContactPoints::getPointsStatus(int inStatus)
{
   TPointsExportStatus Status = pesNone;
   try
   {
		// enum TPointsExportStatus {pesNone, pesExported, pesCorrupted};
   		switch(inStatus)
                 {
                    case pesNone :
                    {
                        Status = pesNone;
                    }
                    break;
                    case pesExported :
                    {
                        Status = pesExported;
                    }
                    break;
                    case pesCorrupted :
                    {
                        Status = pesCorrupted;
                    }
                    break;
                    default :
        	    {
		      	Status = pesNone;
                    }
                    break;
        }
   }
   catch(Exception & E)
   {
	  throw;
   }
   return Status;

}

TPointsTypePair TContactPoints::GetPointsType(int AdjustmentType, int AdjustmentSubType)
{
   TPointsTransactionType Type = pttInvalid;
   TPointsTransactionAccountType SubType = ptstInvalid;
   try
   {
        //enum TPointsTransactionType {pttInvalid,pttSystem, pttReward, pttPurchased,pttEarned,pttRedeemed,pttSync/*pttEarntSpent,pttLoaded*/};
   		switch(AdjustmentType)
        {
        	case pttInvalid :
        	{
             	 Type = pttInvalid;
                }
                break;
                case pttRefund 	: { Type = pttRefund; } break;
                case pttRedeemedBD 	: { Type = pttRedeemedBD; } break;
                case pttRedeemedFV 	: { Type = pttRedeemedFV; } break;
        	case pttPurchased :
        	{
             	  Type = pttPurchased;
                }
                 break;
        	case pttEarned :
        	{
             	  Type = pttEarned;
                }
                break;
        	case pttRedeemed :
        	{
             	  Type = pttRedeemed;
                }
                break;
        	case pttSync :
        	{
             	  Type = pttSync;
                }
                break;
        	case pttBirthdayBonus :
        	{
             	  Type = pttBirthdayBonus;
                }
                break;
        	case pttFirstVisit :
        	{
             	  Type = pttFirstVisit;
                }
                break;
                default :
        	{
		  Type = pttInvalid;
                }
                break;
        }

        // enum TPointsTransactionAccountType {ptstInvalid, ptstHeld, ptstLoyalty, ptstAccount};
        switch(AdjustmentSubType)
        {
            case ptstInvalid :
            {
                SubType = ptstInvalid;
            }
            break;
            case ptstLoyalty :
            {
                SubType = ptstLoyalty;
            }
            break;
            case ptstAccount :
            {
                SubType = ptstAccount;
            }
            break;
            default :
            {
                SubType = ptstInvalid;
            }
            break;
        }

   }
   catch(Exception & E)
   {
	  throw;
   }
   return TPointsTypePair(Type,SubType);
}

TPointsStore TContactPoints::getExcludingSource(TPointsAssignedSource inSource)
{
    TPointsStore RetVal;
 	for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source != pasDatabase)
		{
			RetVal[ptrPointsTransaction->first] = ptrPointsTransaction->second;
      	}
   }
   return RetVal;
}

TPointsStore TContactPoints::getPointsStore()
{
	return PointsStore;
}

bool TContactPoints::HasTransactions()
{
    bool RetVal = false;
 	for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source != pasDatabase)
		{
         	RetVal = true;
      	}
   }
   return RetVal;
}

// checks whether there are any transactions with export status set to pesNone
bool TContactPoints::HasHeldTransactions()
{
    bool RetVal = false;
 	for (TPointsStore::iterator ptrPointsTransaction = PointsStore.begin();
		  ptrPointsTransaction != PointsStore.end();ptrPointsTransaction++)
	{
		if( ptrPointsTransaction->first.Source == pasDatabase
            && ptrPointsTransaction->first.ExportStatus == pesNone)
		{
         	RetVal = true;
      	}
   }
   return RetVal;
}

void TContactPoints::setExportStatus(TPointsExportStatus inExportStatus)
{
    ExportStatus = inExportStatus;
}


/* Auto Incrementors for FOR LOOPS and emum types */
TPointsExportStatus& advance(TPointsExportStatus& ExportStatus)
{
  switch(ExportStatus)
  {
    case pesNone 		: return ExportStatus = pesExported;
    case pesExported 	: return ExportStatus = pesCorrupted;
    case pesCorrupted	: return ExportStatus = pesNone;
  }
  return ExportStatus; // some compilers might warn otherwise
}

TPointsTransactionType& advance(TPointsTransactionType& TransType)
{
  switch(TransType)
  {
    case pttInvalid 	: return TransType = pttPurchased;
    case pttPurchased 	: return TransType = pttEarned;
    case pttEarned 	: return TransType = pttRedeemed;
    case pttRedeemed 	: return TransType = pttSync;
    case pttSync 	: return TransType = pttInvalid;
    case pttBirthdayBonus 	: return TransType = pttBirthdayBonus;
    case pttFirstVisit 	: return TransType = pttFirstVisit;
    case pttRefund 	: return TransType = pttRefund;
    case pttRedeemedBD 	: return TransType = pttRedeemedBD;
    case pttRedeemedFV 	: return TransType = pttRedeemedFV;
  }
  return TransType; // some compilers might warn otherwise
}

TPointsTransactionAccountType &advance(TPointsTransactionAccountType& AccountType)
{
  switch(AccountType)
  {
    case ptstInvalid : return AccountType = ptstLoyalty;
    case ptstLoyalty : return AccountType = ptstAccount;
    case ptstAccount : return AccountType = ptstInvalid;
  }
  return AccountType; // some compilers might warn otherwise
}

void TContactPoints::PadOutEmptyStoreTPointsType(TPointsStore &Store)
{
    for(TPointsTransactionType transType = pttPurchased; transType != pttSync; advance(transType) )
    {
        for(TPointsTransactionAccountType AccType = ptstLoyalty; AccType != ptstInvalid; advance(AccType)  )
        {
            for(TPointsExportStatus StatusType = pesNone; StatusType != pesCorrupted; advance(StatusType)  )
            {
	            TPointsTypePair Pair(transType,AccType);
    	            TPointsExportStatus Status = StatusType;
        	    TPointsType Type(pasDatabase,Pair,Status);
                    if(Store.find(Type) == Store.end())
                    {
                            Store[Type] = 0.00;
                    }
            }
        }
    }
}
