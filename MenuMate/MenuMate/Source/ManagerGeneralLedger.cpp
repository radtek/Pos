//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerGeneralLedger.h"
#include "ItemComplete.h"
#include "ItemCompleteSub.h"
#include "DBThirdPartyCodes.h"
#include "ReferenceManager.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

/* TODO -o Michael -c Improvement :
Show Notes on Members Select Dish display.
When a Order is processed save there card to the DB
When a Order is paid, remove the card from the DB.
Allow account to be pulled up manually if there is a card on the file.
Added a Gui for the Casino Site ID for the membership system.
*/

TManagerGeneralLedger::TManagerGeneralLedger() :
MembershipGeneralLedgerTCP(new TMembershipGeneralLedgerTCP())
{
	// Payment without Codes are not sent.
	UN_CODED = -1;
	DefaultDiscountCode = 190;
	PointsCode = 104;
        CompPointsCode = 190;
        DiningPointsCode = 199;
	RoundingCode = 127;
}

void TManagerGeneralLedger::Initialise(Database::TDBTransaction &DBTransaction)
{
    DefaultDiscountCode = TManagerVariable::Instance().GetInt(DBTransaction,vmExternalMembershipDiscountCode,DefaultDiscountCode);
    PointsCode = TManagerVariable::Instance().GetInt(DBTransaction,vmExternalMembershipPointsCode,PointsCode);
    RoundingCode = TManagerVariable::Instance().GetInt(DBTransaction,vmExternalMembershipSystemCode,RoundingCode);
}

bool TManagerGeneralLedger::EnabledFor(TPaymentTransaction &PaymentTransaction)
{
	return Enabled() && PaymentTransaction.Membership.Member.ContactKey != 0;
}

bool TManagerGeneralLedger::Enabled()
{
	return (TGlobalSettings::Instance().MembershipType == MembershipTypeExternal && TGlobalSettings::Instance().MembershipDatabaseIP != "");
}

bool TManagerGeneralLedger::ProcessTransaction(TPaymentTransaction &PaymentTransaction)
{

/* Info for requests */
	bool RetVal = false;
	if ( EnabledFor(PaymentTransaction))
	{

		AnsiString product_name = TDeviceRealTerminal::Instance().ID.Product.t_str();
		std::string TillName = TDeviceRealTerminal::Instance().ID.ComputerName.t_str();
		int TillID = TDeviceRealTerminal::Instance().ID.DeviceKey;
		int SiteID = TGlobalSettings::Instance().SiteID;
		int StaffID = TDeviceRealTerminal::Instance().User.ContactKey;
		AnsiString Card = "";

		if(PaymentTransaction.Membership.Member.CardStr != "")
		{
			Card = PaymentTransaction.Membership.Member.CardStr.t_str();
		}
		else
		{
			std::set <UnicodeString> ::iterator itCard = PaymentTransaction.Membership.Member.Cards.begin();
			if(itCard != PaymentTransaction.Membership.Member.Cards.end())
			{
				UnicodeString card = *itCard;
				Card = card.t_str();
			}
	   }

		TCategoryCurrency Categories;
		Currency TotalRounding = 0;
		for ( int i = 0 ; i <  PaymentTransaction.PaymentsCount(); i++ )
		{
			TPayment *Payment = PaymentTransaction.PaymentGet(i);

			if(Payment->GetPay() != 0 || Payment->GetAdjustment() != 0)
			{
				int ThirdPartyCode = StrToIntDef(Payment->PaymentThirdPartyID,UN_CODED);

				/* points types should be ignored as they are handled though the membership system? */
				if(Payment->GetPaymentAttribute(ePayTypePoints))
				{
                   if(Payment->Name == "Comp")
                      ThirdPartyCode = CompPointsCode;
                   else if(Payment->Name == "Dining")
                      ThirdPartyCode = DiningPointsCode;
                   else
                      ThirdPartyCode = PointsCode;
				}

				if(ThirdPartyCode != UN_CODED)
				{
					Categories[ThirdPartyCode] += Payment->GetPayTendered();
					TotalRounding += Payment->GetRoundingTotal();
				}
			}
		}

		if(TotalRounding != 0)
		{
			Categories[RoundingCode] += TotalRounding;
		}

		/* Adjustments for any change.*/
		TPayment *PaymentCashAcc = PaymentTransaction.PaymentFind(CASH);
		int CashAccountNumber = UN_CODED;
		if(PaymentCashAcc != NULL && PaymentCashAcc->PaymentThirdPartyID != "")
		{
			CashAccountNumber = StrToIntDef(PaymentCashAcc->PaymentThirdPartyID,UN_CODED);
		}

		if(CashAccountNumber != UN_CODED)
		{
          bool isCashOut = false;
          for ( int i = 0 ; i <  PaymentTransaction.PaymentsCount(); i++ )
			{
				TPayment *Payment = PaymentTransaction.PaymentGet(i);
				if(Payment->GetCashOutTotal() != 0)
				{
					// Fake an 'EFTPOS' Payement of the same amout as cash out to cover the cash out 'purchase'
					if(Payment != PaymentCashAcc)
					{
						Categories[CashAccountNumber] += -Payment->GetCashOutTotal();
                        isCashOut = true;
					}
				}
			}


			if(PaymentTransaction.Money.Change != 0 && !PaymentTransaction.CreditTransaction && !isCashOut)
			{
				Categories[CashAccountNumber] -= PaymentTransaction.Money.Change;
			}

			// Write out Payments to cover the "Cash Out" portion of the
			// change given above.

		}

        if(PaymentTransaction.Money.Discount != 0)
        {
            // Discounts are a Negitive so post the positive number.
        	Categories[DefaultDiscountCode] -= PaymentTransaction.Money.Discount;
        }

        if(Categories.size() > 0)
        {
            // int Covers = TManagerPatron::Instance().GetTotalPatrons(PaymentTransaction.Patrons);
            LastTransactionRef = GetRefNumber(PaymentTransaction.DBTransaction);

            std::auto_ptr<TMSXMLTransaction> Transaction(new TMSXMLTransaction());
            Transaction->SetTransNo(1);
            Transaction->SetTerminalID(TGlobalSettings::Instance().MembershipTillID);
            Transaction->SetTerminalName("MenuMate");
            Transaction->SetCard(Card);
            TCategoryCurrency::const_iterator Cats = Categories.begin();
            for (; Cats != Categories.end(); Cats++)
            {
                __int64 Value = RoundToNearest(Cats->second, 0.01, false).Val / 100;
                Transaction->AddPayment(Cats->first,Value);
            }
            MembershipGeneralLedgerTCP->Process(
                TGlobalSettings::Instance().MembershipDatabaseIP,
                TGlobalSettings::Instance().MembershipDatabasePort,
                *Transaction.get());
            if (!Transaction->Result == eMSAccepted)
            {
                RetVal = false;
                throw Exception("MCI Error processing transaction " + Transaction->ResultText);
            }
            else
            {
                RetVal = true;
            }
    	}
        else
        {
            RetVal = true;
        }
	}
	else
	{
		RetVal = true;
	}
	return RetVal;
}

/* We use the Phoenix Ref here as it is unlikly these two systems will
ever be used in conjuntion with each other and it saves a DB change which
at this time would be messy */
int TManagerGeneralLedger::GetRefNumber(Database::TDBTransaction &DBTransaction)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PHOENIX_REF, 1) FROM RDB$DATABASE";
	IBInternalQuery->ExecQuery();
	int Ref = IBInternalQuery->Fields[0]->AsInteger;
	return Ref;
}
