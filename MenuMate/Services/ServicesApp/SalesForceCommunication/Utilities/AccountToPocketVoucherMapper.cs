using System;
using SalesForceCommunication.Domain;
using SalesForceCommunication.SalesforceReference;

namespace SalesForceCommunication.Utilities
{
    public class AccountToPocketVoucherMapper
    {
        public PocketVoucherDetail Map(Account account)
        {
            return new PocketVoucherDetail
            {
                PocketVoucher = account.PocketVouchers__c,
                Url = account.PocketVouchers_URL__c,
                UserName = account.PocketVouchers_User__c,
                Password = account.PocketVouchers_Password__c,
                MerchantId = account.PocketVouchers_MerchantID__c,
                DateUpdated = account.PocketVouchers_Date_Updated__c
            };
        }
    }
}