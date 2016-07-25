using System;
using System.Text;
using Loyalty_mate.Cloud.Types;
using System.Security.Cryptography;
using System.Configuration;
using System.Net;
using System.Data.SqlClient;
using Loyalty_mate.Interfaces;
using Loyalty_mate.Types;
using Loyalty_mate.SalesforceServiceRef;
using System.ServiceModel;
using NLog;
using XMLManager;
using System.Xml;
using System.IO;
using Loyalty_mate.Utilities.DB;
using FirebirdSql.Data.FirebirdClient;
using MenumateUtils.Encrypt;
using MenumateUtils.Helpers;

namespace Loyalty_mate.Cloud.Salesforce
{
    /// <summary>
    ///   Retrieves the credentials for a syndicate from Salesforce. The credentials
    ///   returned will be used when authenticating with Loyalty mate in the cloud.
    /// </summary>
    public sealed class Salesforce_credential_provider : I_Credential_provider
    {
        #region Private Members

        private SforceService salesforceServiceClient;
        private LoginResult sfLoginResult;
        private string credentialsFileName = "LoyaltyMateCredentials.info";
        private Credentials trineoCredentials;
        private SalesforceCredentials sfCredentials;
        private int salesforceSiteIdLength = 5;
        private Logger logger; 

        #endregion

        #region Public Members

        public Credentials TrineoCredentials
        {
            get { return trineoCredentials; }
        }

        public SalesforceCredentials SalesforceCredentials
        {
            get { return sfCredentials; }
        }

        #endregion

        #region Constructors
        /// <summary>
        /// constructor for credential provider 
        /// </summary>
        internal Salesforce_credential_provider()
        {
            // initialize the service client
            this.salesforceServiceClient = new SforceService();
            logger = LogManager.GetLogger("Trace");
            trineoCredentials = new Credentials();
            sfCredentials = new SalesforceCredentials();
            sfLoginResult = null;
        }

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly Salesforce_credential_provider instance = new Salesforce_credential_provider();
        }
        #endregion

        #region Public Methods

        /// <summary>
        /// Singleton instance for this class
        /// </summary>
        public static 
        Salesforce_credential_provider Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        ///   Retrieves the base authentication credentials for a syndicate.
        /// </summary>
        /// <param name="syndicate_code">
        ///   The syndicate code uniquely identifying a site.
        ///   This syndicate code will be sent from the POS as soon as it decrypts a card and invoking the trineo cloud services
        /// </param>
        /// <returns>
        ///   The credentials of that syndicate.
        /// </returns>
        public Credentials
        Get_syndicate_credentials(
                         string inSyndicateCode,
                         string inSiteID)
        {   
            try
            {
                // load credentials from encrypted xml file                
                getCredentialsFromXML();

                if(!sfCredentials.IsValid() 
                    || !this.loginToSalesforce(
                                    sfCredentials,
                                    out sfLoginResult))
                    throw new Exception("Salesforce authentication failed");

                // retrieve the account by site id and syndicate code
                SalesforceServiceRef.Account account = getSalesforceAccount(
                                                          inSiteID,
                                                          inSyndicateCode);

                if (account == null)
                    throw new Exception("A matching account was not found on Salesforce");
                
                // assign properties for trineo credentials from account
                trineoCredentials.Username = account.Loyalty_Mate_Email__c;
                trineoCredentials.Password = account.Loyalty_Mate_Password__c;
                trineoCredentials.Site_name = account.Loyalty_Mate_Subdomain__c;            

                // logout from salesforce
                salesforceServiceClient.logout();
            }
            catch (Exception ex)
            {
                logger.ErrorException(
                    "Failed to get credentials for the trineo cloud", 
                    ex);
                throw ex; // throwing the exception back to calling application so they know the login process failed
            }

            return trineoCredentials;
        }

        #endregion
               
        #region Private Methods

        /// <summary>
        /// executes a formatted query on salesforce api
        /// </summary>
        /// <param name="queryStr"></param>
        /// <returns></returns>
        private QueryResult 
        executeQueryOnSalesforce(
            string queryStr)
        {
            QueryResult qr = null;
            try
            {
                qr = salesforceServiceClient.query(queryStr);
            }
            catch (Exception ex)
            {
                logger.ErrorException(
                    "Failed to execute the query on salesforce", 
                    ex);
            }

            return qr;
        }
        
        /// <summary>
        /// retrieves the menumate account from salesforce 
        /// this account contains the credentials for the trineo cloud service
        /// </summary>
        /// <returns></returns>
        private SalesforceServiceRef.Account 
        getSalesforceAccount(
            string siteId, 
            string syndicateCode)
        {
            SalesforceServiceRef.Account accountInfo = null;

            // add the leading zeros to the site id so that it will match with what we have in the salesforce
            siteId = addLeadingZerosToNumber(
                Convert.ToInt32(siteId),
                this.salesforceSiteIdLength);

            QueryResult qr = this.executeQueryOnSalesforce(
                string.Format(
                    "Select ID, Loyalty_Mate_Email__c, Loyalty_Mate_Password__c, Loyalty_Mate_Subdomain__c from Account where Site_ID__c='{0}' and Syndicate_Code__c='{1}' ",
                    siteId,
                    syndicateCode));

            if (qr != null
                && qr.records != null
                && qr.records.Length > 0)
            {
                sObject[] records = qr.records;
                accountInfo = (SalesforceServiceRef.Account)records[0];
            }

            return accountInfo;
        }

        /// <summary>
        /// adds leading zeros to a number until a given length appears
        /// </summary>
        /// <param name="originalNumber"></param>
        /// <param name="numberLength"></param>
        /// <returns></returns>
        private string 
        addLeadingZerosToNumber(
            int originalNumber, 
            int numberLength)
        {
            return originalNumber.ToString("D" + numberLength.ToString());
        }

        /// <summary>
        /// attempts to log-in using the provided credentials
        /// </summary>
        /// <param name="sfCredentials"></param>
        /// <param name="loginResult"></param>
        /// <returns></returns>
        private bool 
        loginToSalesforce(
            SalesforceCredentials sfCredentials, 
            out LoginResult loginResult)
        {
            bool result = false;

            try
            {
                loginResult = salesforceServiceClient.login(
                    sfCredentials.UserName, 
                    sfCredentials.Password + sfCredentials.SecurityToken);

                result = !string.IsNullOrEmpty(loginResult.sessionId)
                            && !string.IsNullOrEmpty(loginResult.userId)
                            && loginResult.userInfo != null;

                if (result)
                {
                    // assign the endpoint to the virtual server which serves for this application
                    string authenticationUri = salesforceServiceClient.Url;
                    salesforceServiceClient.Url = loginResult.serverUrl;

                    // set the session id in the header
                    salesforceServiceClient.SessionHeaderValue = new SessionHeader();
                    salesforceServiceClient.SessionHeaderValue.sessionId = loginResult.sessionId;
                }
            }
            catch (Exception ex)
            {
                loginResult = null;
                logger.ErrorException(
                    "Salesforce login failed", 
                    ex);
                result = false;
            }

            return result;
        }

        /// <summary>
        /// gets salesforce credential information from the xml file
        /// </summary>
        /// <returns></returns>
        private void 
        getCredentialsFromXML()
        {
            XmlDocument doc = getCredentialsFile();
            if (doc == null)
                return;

            SalesforceCredentials temp_sf_crd = getSalesforceCredentialsFromXmlNode(XMLDocManager.GetNode(doc, "salesforce"));
            Credentials temp_tr_crd = getTrineoCredentialsFromXmlNode(XMLDocManager.GetNode(doc, "trineo"));

            this.sfCredentials.UserName = temp_sf_crd.UserName;
            this.sfCredentials.Password = temp_sf_crd.Password;
            this.sfCredentials.SecurityToken = temp_sf_crd.SecurityToken;

            this.trineoCredentials.Client_ID = temp_tr_crd.Client_ID;
            this.trineoCredentials.Client_secret = temp_tr_crd.Client_secret;
        }

        /// <summary>
        /// reads a xml node to get salesforce credential information
        /// </summary>
        /// <param name="xmlNode"></param>
        /// <returns></returns>
        private SalesforceCredentials 
        getSalesforceCredentialsFromXmlNode(
            XmlNode xmlNode)
        {
            SalesforceCredentials crd = new SalesforceCredentials();

            if (xmlNode != null)
            {
                crd.UserName = XMLDocManager.GetAttribute(xmlNode, "username", null);
                crd.Password = XMLDocManager.GetAttribute(xmlNode, "password", null);
                crd.SecurityToken = XMLDocManager.GetAttribute(xmlNode, "token", null);
            }

            return crd;
        }

        /// <summary>
        /// reads xml node to get Trineo credentials information
        /// </summary>
        /// <param name="xmlNode"></param>
        /// <returns></returns>
        private Credentials
        getTrineoCredentialsFromXmlNode(
            XmlNode xmlNode)
        {
            Credentials t_crd = new Credentials();

            if (xmlNode != null)
            {
                t_crd.Client_ID = XMLDocManager.GetAttribute(xmlNode, "client_id", null);
                t_crd.Client_secret = XMLDocManager.GetAttribute(xmlNode, "client_secret", null);
            }

            return t_crd;
        }

        /// <summary>
        /// loads the credentials xml file into memory
        /// </summary>
        /// <returns></returns>
        private XmlDocument 
        getCredentialsFile()
        {
            XmlDocument doc = null;
            try
            {
                string fileURI = Helper.Instance.SystemFileURI(this.credentialsFileName);

                if (File.Exists(fileURI))
                {
                    doc = new XmlDocument();
                    string sfCrdStr = File.ReadAllText(@"C:\Program Files\MenuMate\Menumate Services\LoyaltyMateCredentials.txt"); 
                    /*string sfCrdStr = getDecryptedFileContentStr(fileURI); //File.ReadAllText(fileURI, System.Text.Encoding.Default);  //
                    if (sfCrdStr.IndexOf('<') == -1)
                        throw new Exception("Credentials file missing the xml start tag");
                    int startIndex = sfCrdStr.IndexOf('<');
                    int length = sfCrdStr.LastIndexOf('>') + 1 - startIndex;
                    sfCrdStr = sfCrdStr.Substring(startIndex, length);*/
                    doc.LoadXml(sfCrdStr);
                }
                else
                    throw new Exception("Missing credentials file");
            }
            catch (XmlException ex)
            {
                doc = null;
                logger.ErrorException("Invalid credentials xml file", ex);

                throw ex;
            }
            return doc;
        }

        /// <summary>
        /// uses the utility class to decrypt the xml file and returns a string containing all the file content
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        private string getDecryptedFileContentStr(
            string filename)
        {
            string str = "";

            if (Helper.Instance.FileExists(filename))
            {
                byte[] decryptedFileBytes = null;

                
                if (MMEncrypt.Instance.Decrypt(
                        filename, 
                        EncryptAttributes.Instance.Password,
                        ref decryptedFileBytes))
                {
                    str = System.Text.Encoding.Default.GetString(decryptedFileBytes);
                }
            }

            return str;
        }

        /// <summary>
        /// decrypts an encrypted string
        /// </summary>
        /// <param name="encryptedString"></param>
        /// <returns></returns>
        private string 
        getDecryptedString(
            string encryptedString)
        {
            string str = "";

            MMEncrypt.Instance.Decrypt(
                encryptedString, 
                EncryptAttributes.Instance.Password, 
                ref str);

            return str;
        }

        #endregion
    } /* Salesforce_credential_provider */

} /* Loyalty_mate.Cloud */
