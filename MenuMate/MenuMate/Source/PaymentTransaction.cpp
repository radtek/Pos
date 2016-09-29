//---------------------------------------------------------------------------


#pragma hdrstop

#include "PaymentTransaction.h"
#include "GlobalSettings.h"
#include "ManagerDiscount.h"
#include "ItemComplete.h"
#include "ItemSizeCategory.h"
#include "DeviceRealTerminal.h"
#include "ManagerPatron.h"

#include "FreebieManager.h"
#include "MMTouchKeyboard.h"
#include "GUIDiscount.h"
#include "MMMessageBox.h"
#include <math.h>
#include <set>


//---------------------------------------------------------------------------

#pragma package(smart_init)

int __fastcall PaymentCompare(void *Item1,void *Item2);

TPaymentTransaction::TPaymentTransaction(Database::TDBTransaction &inDBTransaction)
: DBTransaction(inDBTransaction) , PaymentList(new TList)
{
	Type = eTransUnknown; // eNormal,ePickNMix,eCashSale,eSplitPayment,
	SalesType = eTab;
    TypeOfSale= RegularSale;
	Orders = new TList;
	InvoiceNumber = "Undefined";
	Money.Clear();
	SplitMoney.Clear();
	InvoiceKey = 0;
	RoomNumber = 0;
	BookingID = 0;
	Patrons.clear();
	TotalAdjustment = 0;
	DiscountReason = "";
	Phoenix.AccountNumber = "";
	Phoenix.AccountName = "";
	Phoenix.FolderNumber = 0;
	CreditTransaction = false;
	PaymentComp = &PaymentCompare;
	Membership.Clear();
	StaffMember.Clear();
	RequestPartialPayment = 0;
	IsCopy = false;
	IsBirtdayProcessed = false;
	IsFirstVisitProcessed = false;
	Birthdaypoints = 0;
	RedeemPointsInformation = new TRedeemPointsInformation;
	RedeemWeightInformation = new TRedeemPointsInformation;
    RedeemPocketVoucherInformation = new TRedeemPocketVoucherInformation;
    RedeemGiftVoucherInformation = new TRedeemGiftVoucherInformation;
	IsQuickPayTransaction = false;
	QuickPaymentName = "";
	SplittedItemKey = 0;
    WebOrderKey = 0;
    PartyName = "";
    TaxOnClippDiscount = 0;
    ServiceChargeWithTax = 0;
    IsVouchersProcessed = false;
}

__fastcall TPaymentTransaction::~TPaymentTransaction()
{
    delete Orders;
    PaymentsClear();
}

TPaymentTransaction::TPaymentTransaction(const TPaymentTransaction &OtherTransaction)
: DBTransaction(OtherTransaction.DBTransaction) , PaymentList(new TList)
{
	Type = OtherTransaction.Type;
	SalesType = OtherTransaction.SalesType;
	Orders = new TList;
	for (int i = 0; i < OtherTransaction.Orders->Count; i++)
	{
		TItemComplete *Order = (TItemComplete *)OtherTransaction.Orders->Items[i];		// Does Not Call Copy Constructor
		Orders->Add(Order);
	}
	InvoiceNumber = OtherTransaction.InvoiceNumber;
	Money = OtherTransaction.Money;
	SplitMoney = OtherTransaction.SplitMoney;
	InvoiceKey = OtherTransaction.InvoiceKey;
	RoomNumber = OtherTransaction.RoomNumber;
	BookingID  = OtherTransaction.BookingID;
	Patrons = OtherTransaction.Patrons;
	TotalAdjustment = OtherTransaction.TotalAdjustment;
	DiscountReason = OtherTransaction.DiscountReason;
	Phoenix.AccountNumber = OtherTransaction.Phoenix.AccountNumber;
	Phoenix.AccountName = OtherTransaction.Phoenix.AccountName;
	Phoenix.FolderNumber = OtherTransaction.Phoenix.FolderNumber;
	CreditTransaction = OtherTransaction.CreditTransaction;
	PaymentComp = &PaymentCompare;
	Membership = OtherTransaction.Membership;
	StaffMember = OtherTransaction.StaffMember;
	RequestPartialPayment 	= OtherTransaction.RequestPartialPayment;
	IsCopy = true;

	for (int i = 0; i < OtherTransaction.PaymentList->Count; i++)
	{
		TPayment *Payment = (TPayment *)OtherTransaction.PaymentList->Items[i];
		PaymentList->Add(Payment);
	}
    RedeemPointsInformation =  OtherTransaction.RedeemPointsInformation;
    RedeemWeightInformation = OtherTransaction.RedeemWeightInformation;
    IsQuickPayTransaction = OtherTransaction.IsQuickPayTransaction;
    QuickPaymentName = OtherTransaction.QuickPaymentName;
    WebOrderKey = OtherTransaction.WebOrderKey;
    PartyName =  OtherTransaction.PartyName;
    RedeemPocketVoucherInformation =  OtherTransaction.RedeemPocketVoucherInformation;
    RedeemGiftVoucherInformation = OtherTransaction.RedeemGiftVoucherInformation;
    IsVouchersProcessed = OtherTransaction.IsVouchersProcessed;
}

TPaymentTransaction& TPaymentTransaction::operator=(const TPaymentTransaction &OtherTransaction)
{
	delete Orders;
	PaymentsClear();
	Type 					= OtherTransaction.Type;
	SalesType 				= OtherTransaction.SalesType;
	Orders 					= new TList;
	for (int i = 0; i < OtherTransaction.Orders->Count; i++)
	{
		TItemComplete *Order = (TItemComplete *)OtherTransaction.Orders->Items[i];		// Does Not Call Copy Constructor
		Orders->Add(Order);
	}
	InvoiceNumber = OtherTransaction.InvoiceNumber;
	Money 	= OtherTransaction.Money;
	SplitMoney = OtherTransaction.SplitMoney;
	InvoiceKey = OtherTransaction.InvoiceKey;
	RoomNumber = OtherTransaction.RoomNumber;
	BookingID = OtherTransaction.BookingID;
	Patrons = OtherTransaction.Patrons;
	TotalAdjustment = OtherTransaction.TotalAdjustment;
	DiscountReason = OtherTransaction.DiscountReason;
	Phoenix.AccountNumber = OtherTransaction.Phoenix.AccountNumber;
	Phoenix.AccountName = OtherTransaction.Phoenix.AccountName;
	Phoenix.FolderNumber = OtherTransaction.Phoenix.FolderNumber;
	CreditTransaction = OtherTransaction.CreditTransaction;
	PaymentComp = &PaymentCompare;
	Membership = OtherTransaction.Membership;
	StaffMember = OtherTransaction.StaffMember;
	RequestPartialPayment = OtherTransaction.RequestPartialPayment;
	IsCopy = true;
	PaymentList = std::auto_ptr<TList> (new TList);
	for (int i = 0; i < OtherTransaction.PaymentList->Count; i++)
	{
		TPayment *Payment = (TPayment *)OtherTransaction.PaymentList->Items[i];
		PaymentList->Add(Payment);
	}
    RedeemPointsInformation =  OtherTransaction.RedeemPointsInformation;
    RedeemWeightInformation = OtherTransaction.RedeemWeightInformation;
    IsQuickPayTransaction = OtherTransaction.IsQuickPayTransaction;
    QuickPaymentName = OtherTransaction.QuickPaymentName;
    WebOrderKey = OtherTransaction.WebOrderKey;
    PartyName = OtherTransaction.PartyName;
    RedeemPocketVoucherInformation =  OtherTransaction.RedeemPocketVoucherInformation;
    RedeemGiftVoucherInformation = OtherTransaction.RedeemGiftVoucherInformation;
    IsVouchersProcessed = OtherTransaction.IsVouchersProcessed;
}

int __fastcall PaymentCompare(void *Item1,void *Item2)
{
	TPayment* Payment1 = (TPayment*)Item1;
	TPayment* Payment2 = (TPayment*)Item2;

	if(Payment1->DisplayOrder > Payment2->DisplayOrder)
   {
      return 1;
	}
	else if(Payment1->DisplayOrder == Payment2->DisplayOrder)
	{
		if(Payment1->Name > Payment2->Name)
      {
         return 1;
      }
		else if(Payment1->Name == Payment2->Name)
      {
         return 0;
      }
      else
      {
         return -1;
		}
   }
   else
   {
      return -1;
   }
}

void TPaymentTransaction::PaymentsReset()
{
	// This will Reset the Contact Points Redeemed
   Membership.Member.Points.ResetPoints();

	for ( int i = 0 ; i <  PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		Payment->Reset();
	}
}

int TPaymentTransaction::PaymentAdd(TPayment *in)
{
	in->Owner = this;
	return PaymentList->Add(in);
}

int TPaymentTransaction::PaymentIndex(TPayment *in)
{
   return PaymentList->IndexOf(in);
}

void TPaymentTransaction::PaymentDelete(TPayment *in)
{
	if(in != NULL)
	{
		if(PaymentList->IndexOf(in) != -1)
		{
			if(in->Owner == this)
			{
				PaymentList->Remove(in);
				delete (TPayment *)in;
				in = NULL;
			}
			else
			{
				PaymentList->Remove(in);
			}
		}
	}
}

TPayment * TPaymentTransaction::PaymentFind(UnicodeString in)
{
	for ( int i = 0 ; i <  PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		if(Payment->Name == in)
		{
			return Payment;
		}
	}
	return NULL;
}

int TPaymentTransaction::GetRefundPointPaymentIndex()
{
    for ( int i = 0 ; i < PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		if(Payment->RefundPoints)
		{
			return i;
		}
	}
	return -1;
}

TPayment* TPaymentTransaction::GetRefundPointsPayment()
{
    for ( int i = 0 ; i < PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		if(Payment->RefundPoints)
		{
			return Payment;
		}
	}
	return NULL;
}

TPayment * TPaymentTransaction::PaymentFindByProperty(int ePayTypeProperty)
{
	for ( int i = 0 ; i <  PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		if(Payment->Properties & ePayTypeProperty)
		{
			return Payment;
		}
	}
	return NULL;
}

void TPaymentTransaction::PaymentsClear()
{
	while(PaymentList->Count > 0)
	{
		PaymentDelete(PaymentGet(0));
	}
}

TPayment *TPaymentTransaction::PaymentGet(int Index)
{
	if(PaymentList->Count > Index)
	{
		return (TPayment *)PaymentList->Items[Index];
	}
	return NULL;
}

void TPaymentTransaction::PaymentsSort()
{
	PaymentList->Sort(PaymentComp);
}

int TPaymentTransaction::VisiablePaymentsCount()
{
   int RetVal = 0;
	for ( int i = 0 ; i <  PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		if(Payment->Visible)
		{
         RetVal ++;
      }
   }
   return RetVal++;
}

bool TPaymentTransaction::TransElectronicPayment()
{
	for ( int i = 0 ; i < PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		if((Payment->Properties & ePayTypeElectronicTransaction) && (Payment->GetCashOut() != 0|| Payment->GetPay() != 0) )
		{
			return true;
		}
	}
	return false;
}

bool TPaymentTransaction::TransInvoicePayment()
{
	for ( int i = 0 ; i < PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		if(((Payment->Properties & ePayTypeInvoiceExport) || (Payment->Properties & ePayTypeChargeToAccount)) && (Payment->GetCashOut() != 0|| Payment->GetPay() != 0) )
		{
			return true;
		}
	}
	return false;
}

bool TPaymentTransaction::TransIntegratedEFTPOS()
{
	for ( int i = 0 ; i <  PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		if((Payment->Properties & ePayTypeElectronicTransaction) &&
			(Payment->Properties & ePayTypeIntegratedEFTPOS) &&
			(Payment->GetCashOut() != 0|| Payment->GetPay() != 0) )
		{
			return true;
		}
	}
	return false;
}

bool TPaymentTransaction::TransVerifyCheque()
{
	for ( int i = 0 ; i < PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		if((Payment->Properties & ePayTypeChequeVerify) && (Payment->GetCashOut() != 0|| Payment->GetPay() != 0) )
		{
			return true;
		}
	}
	return false;
}

bool TPaymentTransaction::TransOpenCashDraw()
{
	for ( int i = 0 ; i <  PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
		if((Payment->Properties & ePayTypeOpensCashDrawer) && (Payment->GetCashOut() != 0 || Payment->GetPay() != 0))
		{
			return true;
		}
	}
	return false;
}

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

void TPaymentTransaction::ProcessPoints()
{
   // Put all the orders in a list includeing there sides so there is no special
   // code for sides.
   std::auto_ptr<TList> PointsOrdersList(new TList);
   bool isRefundTransaction = false;
   if(Orders != NULL)
   {
      for (int i=0; i < Orders->Count; i++)
      {
         TItemMinorComplete *Order = (TItemMinorComplete *) Orders->Items[i];
         PointsOrdersList->Add(Order);

         if(Order->SubOrders)
         {
         for(int j=0; j<Order->SubOrders->Count ; j++)
         {
            TItemMinorComplete *CurrentSubOrder = (TItemMinorComplete *)Order->SubOrders->Items[j];
            PointsOrdersList->Add(CurrentSubOrder);
         }
         }
      }
   }

   if(Membership.Member.ContactKey)
   {
      // Calculate the points being Earned first.
      // If the transaction is a credit these will be spread across the items as
      // it will be a negitive number and there will be no points redeemed.

		Membership.Member.Points.ResetPoints();

        /* Clear out and Points Based Discounts */
        Membership.Member.Points.ClearBySource(pasDiscount);
        /* Find any Points based discounts here and add the transaction.*/

        if(Orders != NULL)
        {
            Currency TotalPointsDiscount = 0;
            for (int i=0; i < PointsOrdersList->Count; i++)
            {
                TItemComplete *Order = (TItemComplete *) PointsOrdersList->Items[i];
                for (std::vector <TDiscount> ::iterator ptrDiscounts = Order->Discounts.begin(); ptrDiscounts != Order->Discounts.end();
                std::advance(ptrDiscounts, 1))
                {
                    if(ptrDiscounts->Source == dsMMMebersPoints)
                    {
                        TotalPointsDiscount += fabs(ptrDiscounts->Amount);
                    }
                }
            }

            if(TotalPointsDiscount != 0 && !TGlobalSettings::Instance().PontsSpentCountedAsRevenue)
            {
                TPointsTypePair Pair(pttRedeemed,ptstLoyalty);
                TPointsType Type(pasDiscount,Pair,pesNone);
                Membership.Member.Points.ClearBySource(pasDiscount);
                Membership.Member.Points.Load(Type, TotalPointsDiscount);
            }
        }

        for ( int i = 0 ; i < PaymentsCount(); i++ )
        {
            TPayment *Payment = PaymentGet(i);
            if(Payment->Properties & ePayTypePoints)
            {
                TPointsTypePair Pair(pttEarned,ptstLoyalty);
                TPointsTransactionAccountType AccountType = ptstLoyalty;
                Currency Redeemed = Payment->GetPay();
                Currency Purchased = Payment->GetAdjustment();
                Currency Refunded = Payment->GetRefundPointsValue();

                if(Redeemed < 0)
                {
                   Refunded = -1 * Redeemed;
                   Redeemed = 0;
                }

                if(TGlobalSettings::Instance().EnableSeperateEarntPts)
                {
                	AccountType = ptstAccount;
                }

                if(TGlobalSettings::Instance().PontsSpentCountedAsRevenue)
                   Redeemed = RedeemPointsInformation->TotalPoints;

                /* Add the Redeemed amount*/
                if(Redeemed != 0 && Payment->Name != "Dining")
                {
                  SetRedeemPoints(Redeemed);
                }

                /* Add the Purcashed Amounts amount*/
                if(Purchased != 0)
                {
                    TPointsTypePair Pair(pttPurchased,AccountType);
                    TPointsType Type(pasUser,Pair,pesNone);
                    Membership.Member.Points.Load(Type, Purchased);
                }

                if(Refunded != 0)
                {
                    if(Refunded < 0)
                     Refunded = -1*Refunded;
                    TPointsTypePair Pair(pttRefund, AccountType);
                    TPointsType Type(pasUser, Pair, pesNone);
                    Membership.Member.Points.Load(Type, Refunded);
                    isRefundTransaction = true;
                }
            }
        }
        Membership.Member.Points.Recalc(PointsOrdersList.get(),Membership.Member.MemberType,isRefundTransaction);

	}
	else
	{ // No Membership so No Points.
		Membership.Member.Points.Clear();
        if(TGlobalSettings::Instance().IsRunRateBoardEnabled && !TGlobalSettings::Instance().IsMemberSalesOnlyEnabled)
        {
            Membership.Member.Points.Recalc(PointsOrdersList.get(), 1 , false);
        }
	}
}

void TPaymentTransaction::SetRedeemPoints(Currency PointsRedeemed)
{
    if(TGlobalSettings::Instance().ShowScreenToSelectItemForPoint)
    {
       PointsRedeemed = RedeemPointsInformation->TotalPoints;
    }
    else if(TGlobalSettings::Instance().UseTierLevels && Membership.Member.MemberType == 1)
    {
      PointsRedeemed = RedeemPointsInformation->TotalPoints + RedeemWeightInformation->TotalPoints ;
    }
    SetRedeemBDayPoints(PointsRedeemed);
    SetRedeemFVPoints(PointsRedeemed);
    if( PointsRedeemed > 0)
     {
        TPointsTransactionAccountType AccountType = ptstLoyalty;
        if(TGlobalSettings::Instance().EnableSeperateEarntPts)
        {
          AccountType = ptstAccount;
        }
        TPointsTypePair Pair(pttRedeemed,AccountType);
        TPointsType Type(pasUser,Pair,pesNone);
        Membership.Member.Points.Load(Type, PointsRedeemed);
    }
}

void TPaymentTransaction::SetRedeemBDayPoints(Currency &PointsRedeemed)
{
   Currency redeemedBirthDayPoints = 0;
   Currency birthDayPoints = 0;
   if(!TGlobalSettings::Instance().LoyaltyMateEnabled)
   {
     birthDayPoints = Membership.Member.Points.getBirthDayRewardPoints() + Membership.Member.Points.getBirthDayRewardRedeemedPoints();
   }
   else
   {
     birthDayPoints = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableBDPoint;
   }
   redeemedBirthDayPoints = PointsRedeemed <= birthDayPoints ? PointsRedeemed :birthDayPoints;
   PointsRedeemed -= redeemedBirthDayPoints;
   TPointsTypePair Pair(pttRedeemedBD,ptstLoyalty);
   TPointsType Type(pasUser,Pair,pesNone);
   if(redeemedBirthDayPoints > 0)
      Membership.Member.Points.Load(Type, redeemedBirthDayPoints);
}

void TPaymentTransaction::SetRedeemFVPoints(Currency &PointsRedeemed)
{
  if( PointsRedeemed > 0)
   {
       Currency firstVisitPoints = 0;
       Currency redeemedFirstVisitPoints = 0;
       if(!TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
          firstVisitPoints = Membership.Member.Points.getFirstVisitPoints() + Membership.Member.Points.getFirstVisitRedeemedPoints();
        }
       else
       {
           firstVisitPoints = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->AvailableFVPoint;
       }
       redeemedFirstVisitPoints = PointsRedeemed <= firstVisitPoints ? PointsRedeemed :firstVisitPoints;
       PointsRedeemed -= redeemedFirstVisitPoints;
       TPointsTypePair Pair(pttRedeemedFV,ptstLoyalty);
       TPointsType Type(pasUser,Pair,pesNone);
       if(firstVisitPoints > 0)
       Membership.Member.Points.Load(Type, redeemedFirstVisitPoints);
   }
}

void TPaymentTransaction::Recalc()
{
   if(Orders != NULL)
   {
      for (int i=0; i < Orders->Count; i++)
      {
         TItemComplete *Order = (TItemComplete *) Orders->Items[i];
		 Order->Loyalty_Key = Membership.Member.ContactKey;
         if(SalesType != eAccount)
         { // Do not do this for invoices.
		 	Order->ResetPrice();
         }

         for(int j=0; j<Order->SubOrders->Count ; j++)
         {
            TItemMinorComplete *CurrentSubOrder = (TItemMinorComplete *)Order->SubOrders->Items[j];
            CurrentSubOrder->Loyalty_Key = Membership.Member.ContactKey;
            if(SalesType != eAccount)
            { // Do not do this for invoices.
               CurrentSubOrder->ResetPrice();
            }
         }
         ManagerDiscount->AddDiscountsByTime(DBTransaction, Order);
      }

      // Sort out Rewards Free Drinks and stuff Here.
      TManagerFreebie::IsPurchasing(DBTransaction,Orders);

	  ApplyDiscounts();
   }
   Money.Recalc(*this);
}

void __fastcall TPaymentTransaction::ApplyDiscounts()
{
   for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();
        ptrDiscount != Discounts.end();ptrDiscount++)
   {

      TDiscount CurrentDiscount  = *ptrDiscount;
	  DiscountReason = ptrDiscount->Description;
      ManagerDiscount->ClearDiscount(Orders,CurrentDiscount);
      ManagerDiscount->AddDiscount(Orders,CurrentDiscount);
   }

   Money.Recalc(*this);
}
//---------------------------------------------------------------------------
void TPaymentTransaction::BuildXMLPaymentTypes(TPOS_XMLBase &Data)
{
   // Update the IntaMate ID with the Invoice Number.
   Data.Doc.Clear();

	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
   Data.Doc.LinkEndChild( decl );

   // Insert DOCTYPE definiation here.
   TiXmlElement * List = new TiXmlElement( xmlEleListPaymentTypes );
	List->SetAttribute(xmlAttrID, AnsiString(Data.IntaMateID).c_str() );
	List->SetAttribute(xmlAttrSiteID, TGlobalSettings::Instance().SiteID);

	for ( int i = 0 ; i <  PaymentsCount(); i++ )
	{
		TPayment *Payment = PaymentGet(i);
	  TiXmlElement *ElePaymnet = new TiXmlElement( xmlElePayment );
	  ElePaymnet->SetAttribute(xmlAttrID, _T("") );
	  ElePaymnet->SetAttribute(xmlAttrName, Payment->Name.t_str() );
	  ElePaymnet->SetAttribute(xmlAttrCode, Payment->PaymentThirdPartyID.t_str() );
	  List->LinkEndChild( ElePaymnet );
   }
   Data.Doc.LinkEndChild( List );
}

void TPaymentTransaction::ApplyMembership(TMMContactInfo inMember, eMemberSource inMemberSource)
{
   ManagerDiscount->ClearMemberDiscounts(Orders);
   ManagerDiscount->ClearThorVouchersDiscounts(Orders);
   Membership.Assign(inMember, inMemberSource);
   ApplyMembershipDiscounts();
   Recalc();
}

void TPaymentTransaction::ApplyMembership(TContactMemberApplied inMemberApplied)
{
   ManagerDiscount->ClearMemberDiscounts(Orders);
   Membership.Assign(inMemberApplied);
   ApplyMembershipDiscounts();
   Recalc();
}

void TPaymentTransaction::ApplyMembershipDiscounts()
{
     for (std::set <int> ::iterator ptrDiscountKey = Membership.Member.AutoAppliedDiscounts.begin();
            ptrDiscountKey != Membership.Member.AutoAppliedDiscounts.end(); ptrDiscountKey++)
        {
            LoadDiscount(*ptrDiscountKey,dsMMMembership);
        }
}

void TPaymentTransaction::RemoveMembership()
{
	// Clear the Member Discounts from the orders.
   ManagerDiscount->ClearMemberDiscounts(Orders);
   ManagerDiscount->ClearThorVouchersDiscounts(Orders);

   //clear member discounts from discount list.
	for (std::vector <TDiscount> ::iterator ptrDiscount = Discounts.begin(); ptrDiscount != Discounts.end();)
	{
	  TDiscount CurrentDiscount = *ptrDiscount;
      if(CurrentDiscount.Source == dsMMMembership)
      {
			Discounts.erase(ptrDiscount);
			ptrDiscount = Discounts.begin();
      }
      else
      {
			std::advance(ptrDiscount, 1);
      }
	}

   //Remove membership.
	Membership.Clear();
    Recalc();
}

void TPaymentTransaction::DeleteOrders()
{
	while (Orders->Count != 0)
	{
		delete Orders->Items[0];
		Orders->Delete(0);
   }
}

void TPaymentTransaction::LoadDiscount(int DiscountKey, TDiscountSource DiscountSource)
{
	bool ProcessDiscount = true;
    bool bailout = false;
	TDiscount CurrentDiscount;
	CurrentDiscount.DiscountKey = DiscountKey;

	ManagerDiscount->GetDiscount(DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);
	CurrentDiscount.Source = DiscountSource;
    if(CurrentDiscount.Source == dsMMMembership)
       {
               CurrentDiscount.IsThorBill = TGlobalSettings::Instance().MembershipType == MembershipTypeThor && TGlobalSettings::Instance().IsThorlinkSelected;
               if(CurrentDiscount.IsThorBill)
               {
                    ManagerDiscount->GetThorlinkDiscount(DBTransaction,CurrentDiscount);
               }
       }
	if (CurrentDiscount.Type == dtPromptDescription || CurrentDiscount.Type == dtPromptDescriptionAmount)
	{
		std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (Screen->ActiveForm));
		do
		{
			frmTouchKeyboard->MaxLength = 200;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = true;
			frmTouchKeyboard->MustHaveValue = true;
			frmTouchKeyboard->KeyboardText = CurrentDiscount.Description;
			frmTouchKeyboard->Caption = "Please enter a discount / surcharge Description";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				CurrentDiscount.Description = frmTouchKeyboard->KeyboardText;
			}
			else
			{
				bailout = true;
			}
		}
		while (frmTouchKeyboard->KeyboardText == "" && !bailout);

		if (bailout)
		{
			ProcessDiscount = false;
		}
	}

	if (ProcessDiscount && (CurrentDiscount.Type == dtPromptAmount || CurrentDiscount.Type == dtPromptDescriptionAmount))
	{
		std::auto_ptr <TfrmDiscount> frmDiscount(TfrmDiscount::Create <TfrmDiscount> (Screen->ActiveForm));
		frmDiscount->Mode = CurrentDiscount.Mode;
		frmDiscount->CURInitial = CurrentDiscount.Amount;
		frmDiscount->PERCInitial = CurrentDiscount.PercentAmount;

		if (frmDiscount->ShowModal() == mrOk)
		{
			CurrentDiscount.Mode = frmDiscount->Mode;
			if (frmDiscount->Mode == DiscModeCurrency)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
					TGlobalSettings::Instance().MidPointRoundsDown);

				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
			else if (frmDiscount->Mode == DiscModeSetPrice)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
					TGlobalSettings::Instance().MidPointRoundsDown);
				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
            else if (frmDiscount->Mode == DiscModeItem)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
					TGlobalSettings::Instance().MidPointRoundsDown);
				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
			else
			{
				CurrentDiscount.PercentAmount = frmDiscount->PERCResult;
			}
		}
		else
		{
			ProcessDiscount = false;
		}
	}

	if (ProcessDiscount)
	{
      Discounts.push_back(CurrentDiscount);
      ManagerDiscount->SetDiscountAmountDB(DBTransaction, Orders);
	}
}
//---------------------------------------------------------------------------
void TPaymentTransaction::CalculatePatronCountFromMenu()
{
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

	Patrons.clear();
	TManagerPatron::Instance().GetPatronTypes(DBTransaction, Patrons);

	DBTransaction.Commit();

	int DefaultPatronCount = 0;
	for (int i = 0; i < Orders->Count; i++)
	{
		TItemMinorComplete *Item = (TItemMinorComplete*)Orders->Items[i];
		DefaultPatronCount += Item->PatronCount();
	}

	std::vector<TPatronType>::iterator ptrPatronType;
	for (ptrPatronType = Patrons.begin(); ptrPatronType != Patrons.end(); ptrPatronType++)
	{
		if (ptrPatronType->Default)
		{
			ptrPatronType->Count = DefaultPatronCount;
			break;
		}
	}
}
//---------------------------------------------------------------------------
bool TPaymentTransaction::HasOrders()
{
	return Orders != NULL && Orders->Count > 0;
}
//---------------------------------------------------------------------------
void TPaymentTransaction::ReCalculateAmounts()
{
	Money.Recalc( *this );
}
//---------------------------------------------------------------------------
bool TPaymentTransaction::CopyFrom( const TPaymentTransaction *OtherTransaction )
{
    copyBasicDetailsFrom(OtherTransaction);
    copyOrdersListFrom(OtherTransaction->Orders);
    copyPaymentsDetails(OtherTransaction->PaymentList.get());
}
//---------------------------------------------------------------------------
void TPaymentTransaction::copyBasicDetailsFrom( const TPaymentTransaction *OtherTransaction )
{
    Type                    = OtherTransaction->Type;
    SalesType               = OtherTransaction->SalesType;
    InvoiceNumber 			= OtherTransaction->InvoiceNumber;
    Money 					= OtherTransaction->Money;
    SplitMoney 				= OtherTransaction->SplitMoney;
    InvoiceKey 				= OtherTransaction->InvoiceKey;
    RoomNumber		 		= OtherTransaction->RoomNumber;
    BookingID 				= OtherTransaction->BookingID;
    Patrons 				= OtherTransaction->Patrons;
    TotalAdjustment 		= OtherTransaction->TotalAdjustment;
    DiscountReason 			= OtherTransaction->DiscountReason;
    Phoenix.AccountNumber 	= OtherTransaction->Phoenix.AccountNumber;
    Phoenix.AccountName 	= OtherTransaction->Phoenix.AccountName;
    Phoenix.FolderNumber 	= OtherTransaction->Phoenix.FolderNumber;
    CreditTransaction 		= OtherTransaction->CreditTransaction;
    PaymentComp 			= &PaymentCompare;
    Membership 				= OtherTransaction->Membership;
    StaffMember 			= OtherTransaction->StaffMember;
    RequestPartialPayment 	= OtherTransaction->RequestPartialPayment;
    RedeemPointsInformation =  OtherTransaction->RedeemPointsInformation;
    RedeemWeightInformation = OtherTransaction->RedeemWeightInformation;
    IsQuickPayTransaction = OtherTransaction->IsQuickPayTransaction;
    QuickPaymentName = OtherTransaction->QuickPaymentName;
    ChitNumber              = OtherTransaction->ChitNumber;
    PartyName = OtherTransaction->PartyName;
    IsVouchersProcessed = OtherTransaction->IsVouchersProcessed;
    IsCopy					= true;
}
//---------------------------------------------------------------------------
void TPaymentTransaction::copyOrdersListFrom(TList* OrdersList)
{
	Orders = new TList;
	for (int i = 0; i < OrdersList->Count; i++)
	{
		TItemComplete *Order = (TItemComplete *)OrdersList->Items[i];
        TItemComplete *CopiedOrder = new TItemComplete();
        Order->Assign(CopiedOrder);
      //  *CopiedOrder = *Order;  // copy order details
		Orders->Add(CopiedOrder);
	}
}
//---------------------------------------------------------------------------
void TPaymentTransaction::copyPaymentsDetails(TList* PaymentsList)
{
	for (int i = 0; i < PaymentsList->Count; i++)
	{
		TPayment *Payment = (TPayment *)PaymentsList->Items[i];
        TPayment *CopiedPayment = new TPayment(this);

        *CopiedPayment = *Payment;  // copy payment details

		PaymentList->Add(CopiedPayment);
	}
}
//---------------------------------------------------------------------------
bool TPaymentTransaction::CheckThorVoucherExistAsDiscount(AnsiString voucher_code)
{
    bool discountExists = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Clear();
    IBInternalQuery->SQL->Text =            " SELECT "
                                            " NAME "
                                            " FROM "
                                            "  DISCOUNTS "
                                            " WHERE "
                                            " DISCOUNT_ID  = :DISCOUNT_ID ";
    IBInternalQuery->ParamByName("DISCOUNT_ID")->AsString = voucher_code;
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount != 0)
    {
        discountExists = true;
    }
    DBTransaction.Commit();

    return discountExists;
}

//---------------------------------------------------------------------------
void TPaymentTransaction::makeLogFile(UnicodeString str)
{
    AnsiString fileName = ExtractFilePath(Application->ExeName) + "EFTPOSCashDrawer_Logs.txt" ;
    TStrings * List = new TStringList();
    if (FileExists(fileName) )
    {
      List->LoadFromFile(fileName);
    }
    List->Add(" "+ str +  "\n");
    List->SaveToFile(fileName );
}

