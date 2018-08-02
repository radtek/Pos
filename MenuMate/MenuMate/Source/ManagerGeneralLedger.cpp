//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerGeneralLedger.h"
#include "ItemComplete.h"
#include "ItemCompleteSub.h"
#include "DBThirdPartyCodes.h"
#include "ReferenceManager.h"
#include "DeviceRealTerminal.h"
#include "SaveLogs.h"
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
    std::auto_ptr<TStringList> logsList(new TStringList());
    logsList->Add("Going to compile data for request                              " + Now().FormatString("hh:nn:ss am/pm"));

	//if ( EnabledFor(PaymentTransaction))
    if(true)
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
                AnsiString value = ThirdPartyCode;
                logsList->Add("Payment used has name                                          " + Payment->Name);
                logsList->Add("ThirdPartyCode extracted from payment as                       " + value);
				/* points types should be ignored as they are handled though the membership system? */
				if(Payment->GetPaymentAttribute(ePayTypePoints))
				{
                   logsList->Add("Payment used is of type points                               ");
                   if(Payment->Name == "Comp")
                      ThirdPartyCode = CompPointsCode;
                   else if(Payment->Name == "Dining")
                      ThirdPartyCode = DiningPointsCode;
                   else
                      ThirdPartyCode = PointsCode;
				}
                logsList->Add("ThirdPartyCode is                                              " + (AnsiString)ThirdPartyCode);
				if(ThirdPartyCode != UN_CODED)
				{
                    logsList->Add("ThirdPartyCode is not equal to UN_CODED");
                    Currency paymentValue = Payment->GetPayTendered();
//                    RoundToNearest(Payment->GetPayTendered(),0.01,
//                                                    TGlobalSettings::Instance().MidPointRoundsDown);
					Categories[ThirdPartyCode] += paymentValue;
                    logsList->Add("Payment Value is                                               " + paymentValue);
					TotalRounding += Payment->GetRoundingTotal();
                    logsList->Add("TotalRounding is                                               " + TotalRounding);
				}
			}
		}

		if(TotalRounding != 0)
		{
            logsList->Add("RoundingCode is                                                " + RoundingCode);
			Categories[RoundingCode] += TotalRounding;
		}

		/* Adjustments for any change.*/
		TPayment *PaymentCashAcc = PaymentTransaction.PaymentFind(CASH);
		int CashAccountNumber = UN_CODED;
		if(PaymentCashAcc != NULL && PaymentCashAcc->PaymentThirdPartyID != "")
		{
			CashAccountNumber = StrToIntDef(PaymentCashAcc->PaymentThirdPartyID,UN_CODED);
            logsList->Add("CashAccountNumber is                                           " + CashAccountNumber);
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
                        Currency cashOut = Payment->GetCashOutTotal();
						Categories[CashAccountNumber] += -cashOut;
                        logsList->Add("cashOut is                                                     " + cashOut);
                        isCashOut = true;
					}
				}
			}


			if(PaymentTransaction.Money.Change != 0 && !PaymentTransaction.CreditTransaction && !isCashOut)
			{
                Currency change = PaymentTransaction.Money.Change;
                //RoundToNearest(PaymentTransaction.Money.Change,0.01,TGlobalSettings::Instance().MidPointRoundsDown);
				Categories[CashAccountNumber] -=  change;
                logsList->Add("change is                                                      " + change);
                //PaymentTransaction.Money.Change;
			}

			// Write out Payments to cover the "Cash Out" portion of the
			// change given above.

		}

        if(PaymentTransaction.Money.Discount != 0)
        {
            // Discounts are a Negitive so post the positive number.
            Currency discount = PaymentTransaction.Money.Discount;
        	Categories[DefaultDiscountCode] -= discount;
            logsList->Add("discount is                                                    " + discount);
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
            logsList->Add("Going to iterate Categories and assign them to Transaction");
            for (; Cats != Categories.end(); Cats++)
            {
                __int64 Value = RoundToNearest(Cats->second, 0.01, false).Val / 100;
                //__int64 Value = Cats->second.Val / 100;
                AnsiString valueStr = Value;
                AnsiString index = Cats->first;
                logsList->Add("value is                                                       " + valueStr);
                logsList->Add("for                                                            " + index);
                Transaction->AddPayment(Cats->first,Value);
            }
            logsList->Add("Sending for Process at                                         " + Now().FormatString("hh:nn:ss am/pm"));
            TSaveLogs::RecordCasinoLogs(logsList.get());
            logsList->Clear();
            MembershipGeneralLedgerTCP->Process(
                TGlobalSettings::Instance().MembershipDatabaseIP,
                TGlobalSettings::Instance().MembershipDatabasePort,
                *Transaction.get());
            if (!Transaction->Result == eMSAccepted)
            {
                logsList->Add("Transaction status is not Accepted                             ");
                logsList->Add("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
                TSaveLogs::RecordCasinoLogs(logsList.get());
                //RetVal = false;
                RetVal = true;
                //throw Exception("MCI Error processing transaction " + Transaction->ResultText);
            }
            else
            {
                RetVal = true;
                logsList->Add("Transaction status is Accepted                                         ");
            }
    	}
        else
        {
            RetVal = true;
            logsList->Add("Transaction status is Accepted as category size is 0                       ");
        }
	}
	else
	{
		RetVal = true;
        logsList->Add("Transaction status is Accepted as not enabled                       ");
	}
    logsList->Add("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    TSaveLogs::RecordCasinoLogs(logsList.get());
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
