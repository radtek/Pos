using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using MYOB.AccountRight.SDK;
using MYOB.AccountRight.SDK.Contracts;
using MYOB.AccountRight.SDK.Services;
using AccountingIntegration.Model;
using MYOB.AccountRight.SDK.Services.Contact;
using MYOB.AccountRight.SDK.Contracts.Version2.Contact;
using MYOB.AccountRight.SDK.Contracts.Version2;
using MYOB.AccountRight.SDK.Services.Sale;
using MYOB.AccountRight.SDK.Contracts.Version2.Sale;
using MYOB.AccountRight.SDK.Contracts.Version2.GeneralLedger;
using MYOB.AccountRight.SDK.Services.GeneralLedger;
using AccountingIntegration.MYOB;
using AccountingIntegration.Utility;

namespace AccountingIntegration.MYOB
{
    public class MYOBCompanyFiles
    {
        #region Delegates

        public delegate void SetDataSourceCallback(CompanyFile[] companyFiles);

        #endregion

        private bool _waitflag;
        private string CompanyName;
        private CompanyFile CompanyFile;
        private IApiConfiguration ConfigurationCloud;
        private IOAuthKeyService MyOAuthKeyService;
        private ICompanyFileCredentials Credentials;
        private MYOBInvoiceController _myobInvoiceContoller;
        string FileName;

        #region Public
        public MYOBCompanyFiles(Credentials inCredentials)
        {
            CompanyName = inCredentials.Organisation;
            _myobInvoiceContoller = new MYOBInvoiceController();
        }

        public void FetchFile(IApiConfiguration configurationCloud, IWebRequestFactory webRequestFactory, IOAuthKeyService keyService,string inFilename)
        {
            try
            {
                ServiceLogger.Log("In fetch File");
                FileName = inFilename;
                ConfigurationCloud = configurationCloud;
                MyOAuthKeyService = keyService;
                var cfsCloud = new CompanyFileService(configurationCloud, null, keyService);
                cfsCloud.GetRange(OnComplete, OnError);
                
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in FetchFile", ex);
            }
        }
        #endregion

        #region Private
        private void OnComplete(HttpStatusCode statusCode, CompanyFile[] companyFiles)
        {
            for (int i = 0; i < companyFiles.Count(); i++)
            {
                if (companyFiles[i].Name == CompanyName)
                {
                    CompanyFile = companyFiles[i];
                    break;
                }
            }
            LogintoCompanyFile();
        }

        private void OnError(Uri uri, Exception ex)
        {
            ServiceLogger.Log("Error Occurred" + ex.Message);
            ServiceLogger.Log("Error Occurred" + ex.StackTrace);
        }

        private void LogintoCompanyFile()
        {
            try
            {
                IApiConfiguration config = ConfigurationCloud;
                var configFilePath = ServiceInfo.GetFileURI(AccountingIntegration.Properties.Settings.Default.CompanyFileDetails);
                var credentials = XeroHelper.DeSerializeFromFilePath<CompanyFileCredentialsCF>(configFilePath);
                Credentials = new CompanyFileCredentials(credentials.CredentialsDetails.UserName, credentials.CredentialsDetails.Password);
                GetCompanyFileDetails(Credentials);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in LogintoCompanyFile", ex);
            }
        }

        private void GetCompanyFileDetails(ICompanyFileCredentials credentials)
        {
            try
            {
                Credentials = credentials;
                LoadCustomers();
            }
            catch (Exception)
            {
                ServiceLogger.Log("Exception in GetCompanyFileDetails");
            }
        }

        private void LoadCustomers()
        {
            try
            {
                var customerSvc = new CustomerService(ConfigurationCloud, null, MyOAuthKeyService);
                customerSvc.GetRange(CompanyFile, null, Credentials, OnCustomerComplete, OnError);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in LoadCustomers", ex);
            }
        }

        private void OnCustomerComplete(HttpStatusCode statusCode, PagedCollection<Customer> customers)
        {
            try
            {
                _myobInvoiceContoller.Customers = new Customer[(int)customers.Count];
                for (int i = 0; i < customers.Count; i++)
                {
                    _myobInvoiceContoller.Customers[i] = customers.Items[i];
                }
                LoadAccounts();
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in OnCustomerComplete", ex);
            }
        }

        private void LoadAccounts()
        {
            try
            {
                var accountSvc = new AccountService(ConfigurationCloud, null,
                                            MyOAuthKeyService);
                //accountSvc.GetRange(CompanyFile, "$filter=Classification eq 'Income' and IsHeader eq false", Credentials,
                //                    OnAccountComplete, OnError);
                accountSvc.GetRange(CompanyFile, "$filter=IsHeader eq false", Credentials,
                                    OnAccountComplete, OnError);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in LoadAcounts", ex);
            }
        }

        private void OnAccountComplete(HttpStatusCode statusCode, PagedCollection<Account> accounts)
        {
            try
            {
                _myobInvoiceContoller.Accounts = new Account[(int)accounts.Count];
                for (int i = 0; i < accounts.Count; i++)
                {
                    _myobInvoiceContoller.Accounts[i] = accounts.Items[i];
                }
                LoadJOB();
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in OnAccountComplete", ex);
            }
        }

        private void LoadJOB()
        {
            try
            {
                var jobSvc = new JobService(ConfigurationCloud, null,
                        MyOAuthKeyService);
                jobSvc.GetRange(CompanyFile, "$filter=IsHeader eq false", Credentials, OnJobComplete, OnError);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in LOADJOB", ex);
            }
        }

        private void OnJobComplete(HttpStatusCode statusCode, PagedCollection<Job> jobs)
        {
            try
            {
                _myobInvoiceContoller.Jobs = new Job[(int)jobs.Count];
                for (int i = 0; i < jobs.Count; i++)
                {
                    _myobInvoiceContoller.Jobs[i] = jobs.Items[i];
                }
                LoadTaxes();
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in OnJobComplete", ex);
            }
        }

        private void LoadTaxes()
        {
            try
            {
                var taxCodeSvc = new TaxCodeService(ConfigurationCloud, null,
                                            MyOAuthKeyService);
                taxCodeSvc.GetRange(CompanyFile, null, Credentials, OnTaxCodeComplete, OnError);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in LoadTaxes", ex);
            }
        }

        private void OnTaxCodeComplete(HttpStatusCode statusCode, PagedCollection<TaxCode> taxCodes)
        {
            try
            {
                _myobInvoiceContoller.TaxCodes = new TaxCode[(int)taxCodes.Count];
                for (int i = 0; i < taxCodes.Count; i++)
                {
                    _myobInvoiceContoller.TaxCodes[i] = taxCodes.Items[i];
                }
               _myobInvoiceContoller.ConfigurationCloud = ConfigurationCloud;
               _myobInvoiceContoller.MyOAuthKeyService = MyOAuthKeyService;
               _myobInvoiceContoller.Credentials = Credentials;
                _myobInvoiceContoller.PostInvoice(CompanyFile, FileName);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in OnTaxCodeComplete", ex);
            }
        }
        
        private void OnSaveComplete(HttpStatusCode statusCode, string newURI)
        {
            ServiceLogger.Log("Saved");
        }
        #endregion
    }
}
