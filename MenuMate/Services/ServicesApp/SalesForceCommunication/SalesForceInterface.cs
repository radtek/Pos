using System;
using System.Collections;
using System.Collections.Generic;
using SalesForceCommunication.Domain;
using SalesForceCommunication.SalesforceReference;
using SalesForceCommunication.Utilities;
using System.Net;


namespace SalesForceCommunication
{
    public class SalesForceInterface
    {
        private readonly SforceService _salesforceService;
        private LoginResult _loginResult;
        private readonly AccountToPocketVoucherMapper _accountToPocketVoucherMapper;
        private readonly AccountFieldsMapper _accountFieldsMapper;
        private readonly SalesForceCommunicationHelper _salesForceCommunicationHelper;

        public SalesForceInterface()
        {
            _loginResult = null;
            _salesforceService = new SforceService();
            _accountToPocketVoucherMapper = new AccountToPocketVoucherMapper();
            _accountFieldsMapper = new AccountFieldsMapper();
            _salesForceCommunicationHelper = new SalesForceCommunicationHelper(_salesforceService);
            ServicePointManager.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => { return true; };
        }

        public bool Login(SalesForceCredential salesForceCredential)
        {
            try
            {
                _loginResult = _salesforceService.login(salesForceCredential.UserName,
                    salesForceCredential.Password + salesForceCredential.SecurityToken);

                var result = !string.IsNullOrEmpty(_loginResult.sessionId)
                             && !string.IsNullOrEmpty(_loginResult.userId)
                             && _loginResult.userInfo != null;

                if (!result)
                    return false;

                _salesforceService.Url = _loginResult.serverUrl;
                _salesforceService.SessionHeaderValue = new SessionHeader { sessionId = _loginResult.sessionId };

                return true;
            }
            catch (Exception exception)
            {

                throw new Exception("There was an error logging into sales force.") { Source = exception.Source };
                
            }
        }

        public void Logout()
        {
            try
            {
                _salesforceService.logout();
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error logging out from sales force.") { Source = exception.Source };
            }
        }

        public PocketVoucherDetail GetPocketVoucherDetail(string accountName)
        {
            try
            {
                //if accountname does not contain single quotes we need to add to it for query execution..
                if (!accountName.StartsWith("'") && !accountName.EndsWith("'"))
                {
                    accountName = accountName.Insert(0, "'");
                    accountName = accountName.Insert((accountName.Length), "'");
                }

                var selectColumnNames = new List<string> { "PocketVouchers__c", "PocketVouchers_URL__c", "PocketVouchers_User__c", "PocketVouchers_Password__c", "PocketVouchers_MerchantID__c", "PocketVouchers_Date_Updated__c" };
                var filterClause = string.Format("Name = {0}", accountName);

                var queryResult = _salesForceCommunicationHelper.ExecuteSelectQuery(filterClause, selectColumnNames);

                if (queryResult != null && queryResult.records != null && queryResult.records.Length > 0)
                {
                    sObject[] records = queryResult.records;
                    var account = (Account)records[0];

                    if (account.PocketVouchers__c.HasValue)
                    {
                        var pocketVoucherDetail = _accountToPocketVoucherMapper.Map(account);
                        return pocketVoucherDetail;
                    }
                }
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error retrieving query from sales force.") { Source = exception.Source };
            }

            return null;
        }
        public AccountFieldsDetails GetAccountFieldsDetails(string accountName)
        {
            try
            {
                //if accountname does not contain single quotes we need to add to it for query execution..
                if (!accountName.StartsWith("'") && !accountName.EndsWith("'"))
                {
                    accountName = accountName.Insert(0, "'");
                    accountName = accountName.Insert((accountName.Length), "'");
                }

                var selectColumnNames = new List<string> { "Mobi2Go__c", "Stock_Activated__c", "Thorlink_Activated__c", "Xero_Activated__c", "ChefMate__c", "PalmMate__c", "Loyalty__c" };
                var filterClause = string.Format("Name = {0}", accountName);

                var queryResult = _salesForceCommunicationHelper.ExecuteSelectQuery(filterClause, selectColumnNames);

                if (queryResult != null && queryResult.records != null && queryResult.records.Length > 0)
                {
                    sObject[] records = queryResult.records;
                    var account = (Account)records[0];

                    //if (account.PocketVouchers__c.HasValue)
                    //{
                        var accountFieldDetails = _accountFieldsMapper.Map(account);
                        return accountFieldDetails;
                    //}
                }
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error retrieving query from sales force.") { Source = exception.Source };
            }

            return null;
        }

        public bool EnablePocketVouchers(string accountName)
        {
            try
            {
                //if accountname does not contain single quotes we need to add to it for query execution..
                if (!accountName.StartsWith("'") && !accountName.EndsWith("'"))
                {
                    accountName = accountName.Insert(0, "'");
                    accountName = accountName.Insert((accountName.Length), "'");
                }

                _salesForceCommunicationHelper.UpdateSalesForceColumnValue<bool?>(accountName, true, "PocketVouchers__c, ID", "PocketVouchers__c");
                return true;
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error enabling pocket vouchers on sales force.") { Source = exception.Source };
            }
        }

        public bool UpdatePocketVouchersDateUpdated(string accountName)
        {
            try
            {
                //if accountname does not contain single quotes we need to add to it for query execution..
                if (!accountName.StartsWith("'") && !accountName.EndsWith("'"))
                {
                    accountName = accountName.Insert(0, "'");
                    accountName = accountName.Insert((accountName.Length), "'");
                }
                IList<Tuple<string, object>> propertyWithNewValues = new List<Tuple<string, object>>();
                propertyWithNewValues.Add(new Tuple<string, object>("PocketVouchers_Date_Updated__c", DateTime.Today));
                propertyWithNewValues.Add(new Tuple<string, object>("PocketVouchers_Date_Updated__cSpecified", true));
                _salesForceCommunicationHelper.UpdateSalesForceColumnValues(accountName,
                propertyWithNewValues, "ID", "PocketVouchers_Date_Updated__c");
                return true;
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error enabling pocket vouchers on sales force.") { Source = exception.Source };
            }
        }
        public bool DisablePocketVouchers(string accountName)
        {
            try
            {
                //if accountname does not contain single quotes we need to add to it for query execution..
                if (!accountName.StartsWith("'") && !accountName.EndsWith("'"))
                {
                    accountName = accountName.Insert(0, "'");
                    accountName = accountName.Insert((accountName.Length), "'");
                }

                _salesForceCommunicationHelper.UpdateSalesForceColumnValue<bool?>(accountName, false, "PocketVouchers__c, ID", "PocketVouchers__c");
                return true;
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error enabling pocket vouchers on sales force.") { Source = exception.Source };
            }
        }

        public bool UpdateMobiToGoStatus(string accountName, bool status)
        {
            try
            {
                //if accountname does not contain single quotes we need to add to it for query execution..
                if (!accountName.StartsWith("'") && !accountName.EndsWith("'"))
                {
                    accountName = accountName.Insert(0, "'");
                    accountName = accountName.Insert((accountName.Length), "'");
                }

                _salesForceCommunicationHelper.UpdateSalesForceColumnValue<bool?>(accountName, status, "Mobi2Go__c, ID", "Mobi2Go__c");
                return true;
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error enabling or disabling mobi to go on sales force.") { Source = exception.Source };
            }
        }

        public bool UpdateSalesForceVersion(string accountName, string versionNumber)
        {
            try
            {
                //if accountname does not contain single quotes we need to add to it for query execution..
                if (!accountName.StartsWith("'") && !accountName.EndsWith("'"))
                {
                    accountName = accountName.Insert(0, "'");
                    accountName = accountName.Insert((accountName.Length), "'");
                }

                _salesForceCommunicationHelper.UpdateSalesForceColumnValue<string>(accountName, versionNumber, "Menumate_Version__c, ID", "Menumate_Version__c");
                return true;
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error updating menumate version on sales force.") { Source = exception.Source };
            }
        }
        public bool UpdateSalesForceModulesStatus(SalesForceModuleStatus salesForceModuleStatus)
        {
            try
            {
                //if accountname does not contain single quotes we need to add to it for query execution..
                if (!salesForceModuleStatus.AccountName.StartsWith("'") && !salesForceModuleStatus.AccountName.EndsWith("'"))
                {
                    salesForceModuleStatus.AccountName = salesForceModuleStatus.AccountName.Insert(0, "'");
                    salesForceModuleStatus.AccountName = salesForceModuleStatus.AccountName.Insert((salesForceModuleStatus.AccountName.Length), "'");
                }

                IList<Tuple<string, object>> propertyWithNewValues = new List<Tuple<string, object>>();
                var accountFieldsDetails = GetAccountFieldsDetails(salesForceModuleStatus.AccountName);
                propertyWithNewValues.Add(new Tuple<string, object>("Menumate_Version__c", salesForceModuleStatus.SalesForceVersion));
                _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                propertyWithNewValues, "ID", "Menumate_Version__c");
                propertyWithNewValues.Clear();
                //The condition compares the values of Field in Menumate and Salesforce and then updates the Field value 
                //accordingly in SalesForce
                if (accountFieldsDetails.MobiToGo != salesForceModuleStatus.IsMobiToGoActivated)
                {
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("Mobi2Go__c", salesForceModuleStatus.IsMobiToGoActivated));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "Mobi2Go__c");
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("Mobi2Go_Date_Updated__c", DateTime.Today));
                    propertyWithNewValues.Add(new Tuple<string, object>("Mobi2Go_Date_Updated__cSpecified", true));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "Mobi2Go_Date_Updated__c");
                    //UpdateSalesForceDateColumn(salesForceModuleStatus.AccountName, "Mobi2Go_Date_Updated__c");
                }
                //The condition compares the values of Field in Menumate and Salesforce and then updates the Field value 
                //accordingly in SalesForce
                if (accountFieldsDetails.Stock != salesForceModuleStatus.IsStockActivated)
                {
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("Stock_Activated__c", salesForceModuleStatus.IsStockActivated));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "Stock_Activated__c");
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("Stock_Date_Updated__c", DateTime.Today));
                    propertyWithNewValues.Add(new Tuple<string, object>("Stock_Date_Updated__cSpecified", true));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "Stock_Date_Updated__c");
                    //UpdateSalesForceDateColumn(salesForceModuleStatus.AccountName, "Stock_Date_Updated__c");
                }
                //The condition compares the values of Field in Menumate and Salesforce and then updates the Field value 
                //accordingly in SalesForce
                if (accountFieldsDetails.Thorlink != salesForceModuleStatus.IsThorlinkActivated)
                {
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("Thorlink_Activated__c", salesForceModuleStatus.IsThorlinkActivated));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "Thorlink_Activated__c");
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("Thorlink_Date_Updated__c", DateTime.Today));
                    propertyWithNewValues.Add(new Tuple<string, object>("Thorlink_Date_Updated__cSpecified", true));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "Thorlink_Date_Updated__c");
                    //UpdateSalesForceDateColumn(salesForceModuleStatus.AccountName, "Thorlink_Date_Updated__c");
                }
                //The condition compares the values of Field in Menumate and Salesforce and then updates the Field value 
                //accordingly in SalesForce
                if (accountFieldsDetails.Xero != salesForceModuleStatus.IsXeroActivated)
                {
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("Xero_Activated__c", salesForceModuleStatus.IsXeroActivated));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "Xero_Activated__c");
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("Xero_Date_Updated__c", DateTime.Today));
                    propertyWithNewValues.Add(new Tuple<string, object>("Xero_Date_Updated__cSpecified", true));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "Xero_Date_Updated__c");
                    //UpdateSalesForceDateColumn(salesForceModuleStatus.AccountName, "Xero_Date_Updated__c");
                }
                //The condition compares the values of Field in Menumate and Salesforce and then updates the Field value 
                //accordingly in SalesForce
                if (accountFieldsDetails.ChefMate != salesForceModuleStatus.IsChefMateActivated)
                {
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("ChefMate__c", salesForceModuleStatus.IsChefMateActivated));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "ChefMate__c");
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("ChefMate_Date_Updated__c", DateTime.Today));
                    propertyWithNewValues.Add(new Tuple<string, object>("ChefMate_Date_Updated__cSpecified", true));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "ChefMate_Date_Updated__c");
                    //UpdateSalesForceDateColumn(salesForceModuleStatus.AccountName, "ChefMate_Date_Updated__c");
                }
                //The condition compares the values of Field in Menumate and Salesforce and then updates the Field value 
                //accordingly in SalesForce
                if (accountFieldsDetails.PalmMate != salesForceModuleStatus.IsPalmMateActivated)
                {
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("PalmMate__c", salesForceModuleStatus.IsPalmMateActivated));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "PalmMate__c");
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("PalmMate_Date_Updated__c", DateTime.Today));
                    propertyWithNewValues.Add(new Tuple<string, object>("PalmMate_Date_Updated__cSpecified", true));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "PalmMate_Date_Updated__c");
                    //UpdateSalesForceDateColumn(salesForceModuleStatus.AccountName, "PalmMate_Date_Updated__c");
                }
                //The condition compares the values of Field in Menumate and Salesforce and then updates the Field value 
                //accordingly in SalesForce
                if (accountFieldsDetails.Loyalty != salesForceModuleStatus.IsLoyaltyActivated)
                {
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("Loyalty__c", salesForceModuleStatus.IsLoyaltyActivated));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "Loyalty__c");
                    propertyWithNewValues.Clear();
                    propertyWithNewValues.Add(new Tuple<string, object>("Loyalty_Date_Updated__c", DateTime.Today));
                    propertyWithNewValues.Add(new Tuple<string, object>("Loyalty_Date_Updated__cSpecified", true));
                    _salesForceCommunicationHelper.UpdateSalesForceColumnValues(salesForceModuleStatus.AccountName,
                    propertyWithNewValues, "ID", "Loyalty_Date_Updated__c");
                    //UpdateSalesForceDateColumn(salesForceModuleStatus.AccountName, "Loyalty_Date_Updated__c");
                }
                return true;
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error updating menumate version on sales force.") { Source = exception.Source };
            }
        }
    }
}
