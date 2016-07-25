using System;
using SalesForceCommunication.Domain;
using SalesForceCommunication.SalesforceReference;

namespace SalesForceCommunication.Utilities
{
    public class AccountFieldsMapper
    {
        public AccountFieldsDetails Map(Account account)
        {
            return new AccountFieldsDetails
            {
                MobiToGo = account.Mobi2Go__c,
                Stock    = account.Stock_Activated__c,
                Thorlink = account.Thorlink_Activated__c,
                Xero     = account.Xero_Activated__c,
                ChefMate = account.ChefMate__c,
                PalmMate  = account.PalmMate__c,
                Loyalty = account.Loyalty__c,
            };
        }
    }
}
