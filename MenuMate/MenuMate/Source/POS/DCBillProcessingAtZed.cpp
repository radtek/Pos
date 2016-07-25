//---------------------------------------------------------------------------


#pragma hdrstop

#include "DCBillProcessingAtZed.h"
#include "DrinkCommandManager.h"
#include "Item.h"
#include "DeviceRealTerminal.h"
#include "DBOrder.h"
#include "DBSecurity.h"
#include "ItemSize.h"
#include "ItemRecipe.h"
#include "MMContactInfo.h"
#include "enumTab.h"
#include "ContactMemberApplied.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
TDCBillProcessingAtZed::TDCBillProcessingAtZed()
{
    TDrinkCommandData::Instance ().ItemDetailsReceived.RegisterForEvent(OnItemDetailsReceivedZED);
    TDrinkCommandData::Instance ().OrderComplete.RegisterForEvent(OnOrderComplete);

     OrdersList1.reset(new TList());
}
//--------------------------------------------------------------------------
TDCBillProcessingAtZed::~TDCBillProcessingAtZed()
{
    TDrinkCommandData::Instance().ItemDetailsReceived.DeregisterForEvent(OnItemDetailsReceivedZED);
    TDrinkCommandData::Instance().OrderComplete.DeregisterForEvent(OnOrderComplete);
}
void TDCBillProcessingAtZed::FetchAllDCOpenSessions()
{
    CollectedOpenSessionDetails = TDrinkCommandData::Instance().GetAllDCOpenSessions();
    AnsiString session_uuid = "";
    int i = 0;
    for(std::vector<TOpenSessionDetails>::iterator it = CollectedOpenSessionDetails.begin();
                it != CollectedOpenSessionDetails.end();++it)
    {
          i++;
          card_no = it->Card_Id;
          AnsiString session_id = it->Session_Id;
          session_uuid += session_id;
          std::auto_ptr<TDrinkCommandManager> dcM(new TDrinkCommandManager());
          AnsiString path = TGlobalSettings::Instance().DrinkCommandServerPath;
          int port = TGlobalSettings::Instance().DrinkCommandServerPort;
          dcM->AskDCSummary(path , port ,session_id);
    }
    i = 0;
}
void TDCBillProcessingAtZed::OnItemDetailsReceivedZED(TSystemEvents *Sender)
{//      if(CardIdStored.Pos(card_no) == 0)//      {        MakeItem(Sender->ItemKey, Sender->ItemSizeKey , Sender->QtyValue , Sender->BillValue);//      }}void TDCBillProcessingAtZed::OnOrderComplete(TSystemEvents *Sender){
      if(CardIdStored.Pos(card_no) == 0)
      {
        ProceedForPayment();
      }
}
void TDCBillProcessingAtZed::MakeItem(long itemKey, long sizesKey , AnsiString qtyValue , AnsiString billValue){
    SizesKey = sizesKey;
    TItem *Item = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(itemKey);
	if (Item)
	{
        dc_item = true;
        bool Proceed = true;
        SelectedOrderMenuName = Item->MenuName;

		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
		DBTransaction.StartTransaction();

        TItemComplete *SetMenuDish = new TItemComplete;
//
        SetMenuDish->ItemOrderedFrom = Item;
        SetMenuDish->TableNo = 000;
        SetMenuDish->SeatNo = 0;
        SetMenuDish->Item = Item->Item;
        SetMenuDish->ItemKitchenName = Item->ItemKitchenName;
        SetMenuDish->ItemKey = Item->ItemKey;
        SetMenuDish->Item_ID = Item->Item_ID;
        SetMenuDish->MenuName = Item->MenuName;
        // TODO 4 -o Michael -c Printing : Remove When Printing Engine Finished
        SetMenuDish->MenuKey = Item->MenuKey;
        SetMenuDish->ItemType = Item->ItemType;
        SetMenuDish->PrintChitNumber = Item->PrintChitNumber;
        SetMenuDish->Terminal = "DC_Term";
        SetMenuDish->TimeStamp = Now();
        SetMenuDish->TransNo = TDBOrder::GetNextTransNumber(DBTransaction);
        SetMenuDish->Note = "";
        SetMenuDish->Course = Item->Course;
        SetMenuDish->CourseKitchenName = Item->CourseKitchenName;
        SetMenuDish->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));

        int ServingCourseKey = Item->Owner->DefaultServingCourseKey;
        SetMenuDish->ServingCourse = TDeviceRealTerminal::Instance().Menus->GetServingCourse(ServingCourseKey);

        // Sort Recipes
        TItemSize* itemSize = Item->Sizes->SizeGetByItemSizeKey(sizesKey);

        if(itemSize)
        {
            SetMenuDish->SalesRecipesToApply->RecipeCopyList(itemSize->Recipes);
            SetMenuDish->Categories->CategoryCopyList(itemSize->Categories);

            SetMenuDish->GSTPercent = itemSize->GSTPercent;
            SetMenuDish->TaxProfiles = itemSize->TaxProfiles;
        }

        // Sort Cost.
        SetMenuDish->Cost = 0;
        if (Item->Sizes->SizeGetByItemSizeKey(sizesKey)->Recipes->Count > 0)
        {
            for (UINT i = 0; i < Item->Sizes->SizeGetByItemSizeKey(sizesKey)->Recipes->Count; i++)
            {
                SetMenuDish->Cost += Item->Sizes->SizeGetByItemSizeKey(sizesKey)->Recipes->RecipeGet(i)->Cost;
            }
        }
        else
        {
            SetMenuDish->Cost = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->Cost; // Default cost if no recipes.
        }

        SetMenuDish->SetPriceLevelCustom(0); // Set Menu Items have no price. Just a cost.
        SetMenuDish->Size = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->Name;

        SetMenuDish->ClaimAvailability();

        SetMenuDish->SetMenuMask = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->SetMenuMask;
        SetMenuDish->SetMenu = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->SetMenuItem;
        SetMenuDish->SetMenuMaster = false;
        SetMenuDish->SizeKitchenName = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->SizeKitchenName;
        SetMenuDish->ThirdPartyKey = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->ThirdPartyKey;
        SetMenuDish->ThirdPartyCode = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->ThirdPartyCode;
        SetMenuDish->PLU = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->PLU;

        SetMenuDish->PointsPercent = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->PointsPercent;


        int priceLevel = atoi(billValue.c_str()) ;
        int qtyDC = atoi(qtyValue.c_str());
        double priceLeveld =  (priceLevel / 100.0);
        double price = double((priceLeveld * 1000) / (qtyDC));

        SetMenuDish->PriceLevel0 = price;
        SetMenuDish->PriceLevel1 = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->HappyPrice;

        SetMenuDish->SetPriceLevel0();
        SetMenuDish->SetPriceLevel1();

        SetMenuDish->MemberFreeSaleCount = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->MemberFreeSaleCount;
        SetMenuDish->MemberFreeSaleDiscount = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->MemberFreeSaleDiscount;
        SetMenuDish->LocationFreeSaleCount = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->LocationFreeSaleCount;
        SetMenuDish->LocationFreeSaleDiscount = Item->Sizes->SizeGetByItemSizeKey(sizesKey)->LocationFreeSaleDiscount;

        SetMenuDish->SetMenuGroup = SetMenuGroupNo;

        SetMenuDish->OrderType = NormalOrder;
        SetMenuDish->HappyHour = false;
		int SecRefNumber =  0;
//        TDBSecurity::GetNextSecurityRef(PaymentTransaction);
		SetMenuDish->TabName = "DC_ITEMS_ZED";
        SetMenuDish->IdName = TGlobalSettings::Instance().TabPrintName;
        SetMenuDish->IdNumber = TGlobalSettings::Instance().TabPrintPhone;
//        Order->TabType = TabType;
        SetMenuDish->RoomNo = 0;
        SetMenuDish->TableNo = 0;
        SetMenuDish->SeatNo = 0;
        SetMenuDish->OrderType = NormalOrder;

        SetMenuDish->TimeStamp = Now();
        SetMenuDish->Terminal = TDeviceRealTerminal::Instance().ID.Name;
        SetMenuDish->OrderedLocation = TDeviceRealTerminal::Instance().ID.Location;
        SetMenuDish->Security->SetSecurityRefNumber(SecRefNumber);
//        TSecurityReference SecRef;
//        SecRef.UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
//        SecRef.Event = SecurityTypes[secOrderedBy];
//        SecRef.From = TDeviceRealTerminal::Instance().User.Name;
//        SecRef.To = TDeviceRealTerminal::Instance().User.Initials;
//        SecRef.TimeStamp = Now();
//        SetMenuDish->Security->SecurityUpdate(secOrderedBy, SecRef);
//
        double qtyValF = 0.0;
        if(price != 0)
        {
               qtyValF = double(qtyDC/1000.0);
        }
        else
        {
            qtyValF = 0;
        }

        SetMenuDish->SetQty(StrToCurr(qtyValF));

        DBTransaction.Commit();
        OrdersList1->Clear();
        OrdersList1->Add(SetMenuDish);
        //delete SetMenuDish;
	}
}
void TDCBillProcessingAtZed::ProceedForPayment()
{
    CardIdStored += ",";
    CardIdStored += card_no;
    ApplyMembership();
    std::auto_ptr<TList>OrdersList(new TList);
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
    TPaymentTransaction PaymentTransaction(DBTransaction);
    PaymentTransaction.Membership.Assign(Membership);
    PaymentTransaction.Orders->Assign(OrdersList1.get());
    TotalCosts(PaymentTransaction);
    PaymentTransaction.SalesType = eCash;
    PaymentTransaction.Recalc();
    PaymentTransaction.ChitNumber.Clear();
    PaymentTransaction.Type = eTransOrderSet;
    bool paymentComplete = TDeviceRealTerminal::Instance().PaymentSystem->ProcessTransaction(PaymentTransaction);
    if(paymentComplete)
    {
        TDeviceRealTerminal::Instance().ManagerStock->UpdateStock(DBTransaction, OrdersList.get(), TDeviceRealTerminal::Instance().User.Name);
    }
    DBTransaction.Commit();
}
void TDCBillProcessingAtZed::ApplyMembership()
{
    TMMContactInfo TempUserInfo;
    TempUserInfo.ProxStr = card_no;
    TempUserInfo.CardStr = card_no;
    TLoginSuccess LoginResult = lsDenied;
    eMemberSource MemberSource = emsManual;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
    TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->IsCard(DBTransaction, TempUserInfo);
    DBTransaction.Commit();
    Membership.Assign(TempUserInfo, MemberSource);
}
void TDCBillProcessingAtZed::TotalCosts(TPaymentTransaction PaymentTransaction)
{
    InitialMoney.Clear();
	TDeviceRealTerminal::Instance().PaymentSystem->PaymentsLoadTypes(PaymentTransaction);
	std::auto_ptr<TList>OrdersList(new TList);
    PaymentTransaction.ProcessPoints();
}

