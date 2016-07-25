//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerParkedSales.h"
#include "DBTab.h"
#include "Classes.hpp"
#include "ParkedSale.h"
#include "DBOrder.h"
#include "DeviceRealTerminal.h"
#include "EnableFloorPlan.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TManagerParkedSales::TManagerParkedSales()
{
}

int TManagerParkedSales::GetCount(Database::TDBTransaction &DBTransaction)
{
	std::auto_ptr<TStringList> Data(new TStringList);
	TDBTab::GetTabs(DBTransaction,Data.get(), TabParkedSale);
   return Data->Count;
}

void TManagerParkedSales::GetParkedSales(Database::TDBTransaction &DBTransaction,TStringList *ParkedSales)
{
	TDBTab::GetTabs(DBTransaction,ParkedSales,TabParkedSale);
}


void TManagerParkedSales::Delete(Database::TDBTransaction &DBTransaction,int TabKey)
{
    try
    {
        if(TDBTab::GetTabExists(DBTransaction,TabKey))
        {
             TDBTab::ClearTab(DBTransaction,TabKey);
        }

    }
    catch(Exception & E)
    {
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

void TManagerParkedSales::SetParkedSale(Database::TDBTransaction &DBTransaction,TParkedSale *Sale)
{
   int TabKey = TDBTab::GetOrCreateTab(DBTransaction,0);
   TDBTab::SetTabType(DBTransaction,TabKey, TabParkedSale);

   if(Sale->StoreMembership)
   {
      TDBTab::SetTabOwner(DBTransaction,TabKey,Sale->Membership.Member.ContactKey,TabParkedSale);
   }

   UnicodeString TabName = Sale->ParkedBy + " " + Now().FormatString("dd-mm-yy hh:nn:ss am/pm");
	TDBTab::SetTabName(DBTransaction,TabKey,TabName );

   std::auto_ptr<TList> Orders(new TList);
	for (int i = 0; i < Sale->Orders.size() ; i++)
   {
		for (int j = 0; j < Sale->Orders[i]->Orders->Count; j ++)
      {
		 TItemComplete *Item = Sale->Orders[i]->Orders->Items[j];
         Item->TabType = TabParkedSale;
         Item->TabKey  = TabKey;
         Item->TabContainerName = Sale->SelectedTabContainerName;
		 Item->TabName = TabName;
		 Item->TransNo = TDBOrder::GetNextTransNumber(DBTransaction);
		 Item->TableNo = Sale->SelectedTable;
         Orders->Add(Item);
		}
   }

	TDBOrder::ProcessOrders(DBTransaction,Orders.get());
}

TParkedSale * TManagerParkedSales::GetFirstParkedSale(Database::TDBTransaction &DBTransaction)
{
	TParkedSale *Sale = new TParkedSale;
   return Sale;
}

TParkedSale * TManagerParkedSales::GetParkedSale(Database::TDBTransaction &DBTransaction,int TabKey)
{
	TParkedSale *Sale = NULL;

   std::auto_ptr<TList> Orders(new TList);
   std::set <__int64> ItemsTabs;
   ItemsTabs.insert(TabKey);
   TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, Orders.get(), ItemsTabs);

   /*We have to check that all the parked items laoded correctly in that
   they were linked back to the Menu Item that they come from.
   If we cant load them back then we can't unpark this order.
   This is a  double check here, we check for this beofre we show the parked list.*/

	bool AllItemsLinkedBackToMenus = Orders->Count > 0; // if there are no orders, there is an error with the park sale

	for (int i = 0; i < Orders->Count; i++)
	{
        TItemComplete *Item = (TItemComplete *)Orders->Items[i];
        if(Item->OriginalItemKey != 0)
        {
            Item->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(Item->OriginalItemKey);
        }

        if(Item->ItemOrderedFrom == NULL)
        {
            AllItemsLinkedBackToMenus = false;
        }
   }

   if(AllItemsLinkedBackToMenus)
   {
	  Sale = new TParkedSale;
      TMMContactInfo MembershipInfo;
	  eMemberSource MemberSource = emsManual;
	  MembershipInfo.ContactKey = TDBTab::GetTabOwner(DBTransaction,TabKey);
      Database::TDBTransaction MemberDBTransaction(TDeviceRealTerminal::Instance().DBControl);
	  TDeviceRealTerminal::Instance().RegisterTransaction(MemberDBTransaction);
	  MemberDBTransaction.StartTransaction();
      TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->FindMember(MemberDBTransaction, MembershipInfo);
      MemberDBTransaction.Commit();
      bool applyMemberShip = false;
      if (Result == lsAccepted)
      {
          applyMemberShip = true;
	      Sale->Membership.Assign(MembershipInfo, MemberSource);
		  Sale->StoreMembership = true;
      }

      unsigned __int32 maxSeatCount = getMaxSeatCount();

      for( unsigned __int32 i = 1; i <= maxSeatCount; i++)
      {
         TSeatOrders *Temp = new TSeatOrders(i);
         Sale->Orders.push_back(Temp);
      }

      for (int i = 0; i < Orders->Count; i++)
      {
         TItemComplete *Item = (TItemComplete *)Orders->Items[i];
         Item->TabKey  = TabKey;
         Sale->SelectedTabContainerName = Item->TabContainerName;
         Sale->SelectedTable = Item->TableNo;
         Sale->SelectedSeat = Item->SeatNo;
         Sale->Orders[Item->SeatNo]->Orders->Add(Item,Item->ItemOrderedFrom);
         if(applyMemberShip)
         	Sale->Orders[Item->SeatNo]->Orders->AppliedMembership = MembershipInfo;
       }

      }
   return Sale;
}

void TManagerParkedSales::DeleteAll(Database::TDBTransaction &DBTransaction)
{
  try
    {
 	  TDBTab::DeleteTabType(DBTransaction,TabParkedSale);
    }
    catch(Exception & E)
    {
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

unsigned __int32 TManagerParkedSales::getMaxSeatCount()
{
    return TEnableFloorPlan::Instance()->GetMaxSeatCount();
}
//.............................................................................

int TManagerParkedSales::SettempParkedSale(Database::TDBTransaction &DBTransaction,TParkedSale *Sale)
{
   int TabKey = TDBTab::GetOrCreateTab(DBTransaction,0);
   TDBTab::SetTabType(DBTransaction,TabKey, TabParkedSale);

   if(Sale->StoreMembership)
   {
      TDBTab::SetTabOwner(DBTransaction,TabKey,Sale->Membership.Member.ContactKey,TabParkedSale);
   }

   UnicodeString TabName = Sale->ParkedBy + "SubsidizedPay" ;
	TDBTab::SetTabName(DBTransaction,TabKey,TabName );

   std::auto_ptr<TList> Orders(new TList);
	for (int i = 0; i < Sale->Orders.size() ; i++)
   {
		for (int j = 0; j < Sale->Orders[i]->Orders->Count; j ++)
      {
		 TItemComplete *Item = Sale->Orders[i]->Orders->Items[j];
         Item->TabType = TabParkedSale;
         Item->TabKey  = TabKey;
         Item->TabContainerName = Sale->SelectedTabContainerName;
		 Item->TabName = TabName;
		 Item->TransNo = TDBOrder::GetNextTransNumber(DBTransaction);
		 Item->TableNo = Sale->SelectedTable;
         Orders->Add(Item);
		}
   }

	TDBOrder::ProcessOrders(DBTransaction,Orders.get());

    return TabKey;
}

