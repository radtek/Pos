using System;
using MenumateServices.DTO.SalesForce;
using SalesForceCommunication;
using SalesForceCommunication.Domain;
using System.Net;
using System.Diagnostics;
using System.Security.Cryptography.X509Certificates;

namespace MenumateServices.WCFServices
{
    public class SalesForceIntegrationWebService : ISalesForceIntegrationWebService
    {
        private readonly SalesForceInterface _salesForceInterface;
        private readonly SalesForceCredentialFactory _salesForceCredentialFactory;


        public SalesForceIntegrationWebService()
        {
            _salesForceInterface = new SalesForceInterface();
            _salesForceCredentialFactory = new SalesForceCredentialFactory();
        }


        public bool UpdateSalesForceVersion(string versionNumber, string accountName)
        {
            try
            {
                //We need to login to sales force to initiate the process..
                //Get the credentials for logging into sales force..
                var credentials = _salesForceCredentialFactory.Create();

                if (_salesForceInterface.Login(credentials))
                {
                    //Update the sales force version of POS..
                    var isEnabled = _salesForceInterface.UpdateSalesForceVersion(accountName, versionNumber);

                    //Since we have completed all the processing we will log out..
                    _salesForceInterface.Logout();

                    return isEnabled;
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In UpdateSalesForceVersion Salesforce", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 12, short.MaxValue);
                ServiceLogger.LogException("Exception in UpdateSalesForceVersion", exception);
            }

            return false;
        }

        public bool EnableMobiToGo(string accountName)
        {
            try
            {
                //We need to login to sales force to initiate the process..
                //Get the credentials for logging into sales force..
                var credentials = _salesForceCredentialFactory.Create();

                if (_salesForceInterface.Login(credentials))
                {
                    //Update the Mobi to Go Status..
                    var isEnabled = _salesForceInterface.UpdateMobiToGoStatus(accountName, true);

                    //Since we have completed all the processing we will log out..
                    _salesForceInterface.Logout();

                    return isEnabled;
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In EnableMobiToGo Salesforce", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 13, short.MaxValue);
                ServiceLogger.LogException("Exception in EnableMobiToGo", exception);
            }

            return false;
        }

        public bool DisableMobiToGo(string accountName)
        {
            try
            {
                //We need to login to sales force to initiate the process..
                //Get the credentials for logging into sales force..
                var credentials = _salesForceCredentialFactory.Create();

                if (_salesForceInterface.Login(credentials))
                {
                    //Update the Mobi to Go Status..
                    var isEnabled = _salesForceInterface.UpdateMobiToGoStatus(accountName, false);

                    //Since we have completed all the processing we will log out..
                    _salesForceInterface.Logout();

                    return isEnabled;
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In DisableMobiToGo Salesforce", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in DisableMobiToGo", exception);
            }

            return false;
        }

        public bool UpdateSalesForceModulesStatus(SalesForceModuleStatus salesForceModuleStatus)
        {
            try
            {
                //We need to login to sales force to initiate the process..
                //Get the credentials for logging into sales force..
                var credentials = _salesForceCredentialFactory.Create();

                if (_salesForceInterface.Login(credentials))
                {
                    //Update the sales fields with new values..
                    var isSuccess = _salesForceInterface.UpdateSalesForceModulesStatus(salesForceModuleStatus);

                    //Since we have completed all the processing we will log out..
                    _salesForceInterface.Logout();

                    return isSuccess;
                }
            }
            catch (Exception exception)
            {
                EventLog.WriteEntry("In UpdateSalesForceModulesStatus Salesforce", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 15, short.MaxValue);
                ServiceLogger.LogException("Exception in UpdateSalesForceModulesStatus", exception);
            }

            return false;
        }
    }
}