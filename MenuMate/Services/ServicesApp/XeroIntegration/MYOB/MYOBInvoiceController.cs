using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;
using MYOB.AccountRight.SDK;
using MYOB.AccountRight.SDK.Contracts;
using MYOB.AccountRight.SDK.Services;
using AccountingIntegration.Model;
using MYOB.AccountRight.SDK.Services.Contact;
using MYOB.AccountRight.SDK.Contracts.Version2.Contact;
using MYOB.AccountRight.SDK.Contracts.Version2;
using MYOB.AccountRight.SDK.Services.Sale;
using MYOB.AccountRight.SDK.Contracts.Version2.Sale;
using MYOB.AccountRight.SDK.Contracts.Version2.Banking;
using MYOB.AccountRight.SDK.Services.Banking;
using MYOB.AccountRight.SDK.Contracts.Version2.GeneralLedger;
using MYOB.AccountRight.SDK.Services.GeneralLedger;
using AccountingIntegration.Utility;


namespace AccountingIntegration.MYOB
{
    public class MYOBInvoiceController
    {
        public IApiConfiguration ConfigurationCloud;
        public IOAuthKeyService MyOAuthKeyService;
        public ICompanyFileCredentials Credentials;
        public Account[] Accounts;
        public Customer[] Customers;
        public Job[] Jobs;
        public TaxCode[] TaxCodes;

        private static readonly object syncRoot = new Object();
        private static volatile MYOBInvoiceController _instance;
        private Account CurrentAccount;
        private Customer CurrentCustomer;
        private CompanyFile CompanyFile1;
        private string _cacheFolder;
        private string _retryFolder;
        private string _successfulFolder;
        private string _failedFolder;
        private string FileName;

        public MYOBInvoiceController()
        {

        }

        #region Public
        public MYOBInvoiceController(IApiConfiguration configurationCloud, IOAuthKeyService myOAuthKeyService, ICompanyFileCredentials credentials)
        {
            MyOAuthKeyService = myOAuthKeyService;
            ConfigurationCloud = configurationCloud;
            Credentials = credentials;
        }

        Customer GetCustomer(string contactName)
        {
            for (int i = 0; i < Customers.Count(); i++)
            {
                if (Customers[i].IsIndividual && Customers[i].IsActive)
                {
                    string firstLastname = Customers[i].FirstName.ToUpper() + " " + Customers[i].LastName.ToUpper();
                    string lastFirstName = Customers[i].LastName.ToUpper() + " " + Customers[i].FirstName.ToUpper();
                    firstLastname = firstLastname.Contains(",") ? firstLastname.Remove(firstLastname.LastIndexOf(",")) : firstLastname;
                    lastFirstName = lastFirstName.Contains(",") ? lastFirstName.Remove(lastFirstName.LastIndexOf(",")) : lastFirstName;
                    if (contactName.ToUpper() == firstLastname ||
                        contactName.ToUpper() == lastFirstName || contactName.ToUpper() == Customers[i].FirstName.ToUpper())
                    {
                        CurrentCustomer = Customers[i];
                        break;
                    }
                }
                else
                {
                    if ((Customers[i].CompanyName.ToUpper() == contactName.ToUpper()) && Customers[i].IsActive)
                    {
                        CurrentCustomer = Customers[i];
                        break;
                    }
                }
            }
            return CurrentCustomer;
        }

        DateTime GetDate(string inDate)
        {
            DateTime date;
            try
            {
                date = DateTime.ParseExact(inDate, "dd/MM/yyyy HH:mm:ss", null);
            }
            catch (Exception)
            {
                date = DateTime.Now;
            }
            return date;
        }

        public void PostInvoice(CompanyFile CompanyFile, string inFilename)
        {
            try
            {
                CompanyFile1 = CompanyFile;
                InitFileandFolders(inFilename);
                CurrentAccount = null;
                var invoice = XeroHelper.DeSerializeFromFilePath<MYOBInvoice>(inFilename);
                var receiveMoney = new ReceiveMoneyTxn();
                var receiveMoneyService = new ReceiveMoneyTxnService(ConfigurationCloud, null,
                                                                     MyOAuthKeyService);
                decimal amountToBeRecievedIncash = 0.00M;
                receiveMoney.Contact = new CardLink { UID = (Guid)GetCustomer(invoice.Contact.Name).UID };
                receiveMoney.IsTaxInclusive = true;
                receiveMoney.PaymentMethod = "Cash";
                receiveMoney.Date = DateTime.Now;
                receiveMoney.DepositTo = DepositTo.Account;
                receiveMoney.Memo = "Job " + invoice.Jobcode + " Time " + invoice.Date;
                for (int i = 0; i < Accounts.Count(); i++)
                {
                    if (Accounts[i].DisplayID == invoice.CashGLCode)
                    {
                        receiveMoney.Account = new AccountLink { UID = (Guid)Accounts[i].UID };
                        break;
                    }
                }
                var lines = new List<ReceiveMoneyTxnLine>();
                foreach (var item in invoice.Items.Item)
                {
                    if (item.Description != "Cash")
                    {
                        var line = new ReceiveMoneyTxnLine();
                        for (int i = 0; i < Accounts.Count(); i++)
                        {
                            if (item.GLCode == Accounts[i].DisplayID)
                            {
                                line.Account = new AccountLink { UID = (Guid)Accounts[i].UID };
                                CurrentAccount = Accounts[i];
                                break;
                            }
                        }
                        decimal d = 0.0M;
                        Decimal.TryParse(item.UnitAmount, out d);
                        line.Amount = d;

                        if (item.Description == "Rounding" || item.Description == "Cash Variance")
                            amountToBeRecievedIncash += d < 0 ? d : d * (-1);
                        else
                            amountToBeRecievedIncash += d;   // comprises total amount in hand

                        for (int i = 0; i < Jobs.Count(); i++)
                        {
                            if (invoice.Jobcode == Jobs[i].Number)
                            {
                                line.Job = new JobLink { UID = (Guid)Jobs[i].UID };
                                break;
                            }
                        }
                        line.Memo = item.Description;
                        if (item.TaxStatus == "NonZeroTax")
                            line.TaxCode = new TaxCodeLink { UID = (Guid)CurrentAccount.TaxCode.UID };
                        else
                        {
                            for (int i = 0; i < TaxCodes.Count(); i++)
                            {
                                if (TaxCodes[i].Rate == 0.0)
                                {
                                    line.TaxCode = new TaxCodeLink { UID = (Guid)TaxCodes[i].UID };
                                    break;
                                }
                            }
                        }
                        lines.Add(line);
                    }
                }
                receiveMoney.Lines = lines;
                receiveMoney.AmountReceived = amountToBeRecievedIncash > 0.00M ? amountToBeRecievedIncash : (amountToBeRecievedIncash * -1);
                receiveMoneyService.Insert(CompanyFile, receiveMoney, Credentials, OnSaveComplete, OnError);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in PostInvoice " + ex.Message);
                MoveFileToFailedFolder();
            }
        }

        private void OnSaveComplete(HttpStatusCode statusCode, string newURI)
        {
            ServiceLogger.Log("Saved");
            MoveFileToSuccesfulFolder();
        }

        private void InitFileandFolders(string inFileName)
        {
            FileName = inFileName;
            _cacheFolder = MYOBFolder(AccountingIntegration.Properties.Settings.Default.MYOBIntegrationCacheFolder);
            _retryFolder = MYOBFolder(AccountingIntegration.Properties.Settings.Default.MYOBIntegrationRetryFolder);
            _successfulFolder = MYOBFolder(AccountingIntegration.Properties.Settings.Default.MYOBIntegrationSuccessfulFolder);
            _failedFolder = MYOBFolder(AccountingIntegration.Properties.Settings.Default.MYOBIntegrationFailedFolder);
        }

        string MYOBFolder(string inFolder)
        {
            return ServiceInfo.GetFileURI(inFolder);
        }

        private void OnError(Uri uri, Exception ex)
        {
            ServiceLogger.Log("Error Occurred" + ex.Message);
            MoveFileToRetryFolder();
        }

        private void MoveFileToCacheFolder()
        {
            MoveFile(FileName, GetFilePath(_cacheFolder, FileName));
        }

        private void MoveFileToSuccesfulFolder()
        {
            MoveFile(FileName, GetFilePath(_successfulFolder, FileName));
        }

        private void MoveFileToRetryFolder()
        {
            MoveFile(FileName, GetFilePath(_retryFolder, FileName));
        }

        private void MoveFileToFailedFolder()
        {
            MoveFile(FileName, GetFilePath(_failedFolder, FileName));
        }

        private string GetFilePath(string baseDirectory, string inFilePath)
        {
            return string.Format(@"{0}\{1}", baseDirectory, Path.GetFileName(inFilePath));
        }

        private void MoveFile(string inSourceFilename, string inTargetFileName)
        {
            try
            {
                File.Move(inSourceFilename, inTargetFileName);
            }
            catch (Exception exc)
            {
                ServiceLogger.Log(string.Format(@"Failed to move a [{0}] to [{1}]. MYOB Server Validation Error: {2}", inSourceFilename, inTargetFileName, exc.Message));
            }
        }
        #endregion
    }
}
