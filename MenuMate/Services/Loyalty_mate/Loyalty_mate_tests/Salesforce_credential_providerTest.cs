using Loyalty_mate.Cloud.Salesforce;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Xml;
using Loyalty_mate.Types;
using XMLManager;
using Loyalty_mate.SalesforceServiceRef;

namespace Loyalty_mate_tests
{
    
    
    /// <summary>
    ///This is a test class for Salesforce_credential_providerTest and is intended
    ///to contain all Salesforce_credential_providerTest Unit Tests
    ///</summary>
    [TestClass()]
    public class Salesforce_credential_providerTest
    {

        private Salesforce_credential_provider_Accessor target;
        private TestContext testContextInstance;
        private LoginResult loginResult;
        private string siteId;
        private string syndicateCode;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        [TestInitialize]
        public void Setup()
        {
            target = new Salesforce_credential_provider_Accessor(); 
            target.credentialsFileName = @"LoyaltyMateCredentials.info";
            target.salesforceServiceClient = new SforceService();
            siteId = "987654321";
            syndicateCode = "menumatefoodco";
        }

        #region Additional test attributes
        // 
        //You can use the following additional attributes as you write your tests:
        //
        //Use ClassInitialize to run code before running the first test in the class
        //[ClassInitialize()]
        //public static void MyClassInitialize(TestContext testContext)
        //{
        //}
        //
        //Use ClassCleanup to run code after all tests in a class have run
        //[ClassCleanup()]
        //public static void MyClassCleanup()
        //{
        //}
        //
        //Use TestInitialize to run code before running each test
        //[TestInitialize()]
        //public void MyTestInitialize()
        //{
        //}
        //
        //Use TestCleanup to run code after each test has run
        //[TestCleanup()]
        //public void MyTestCleanup()
        //{
        //}
        //
        #endregion


        /// <summary>
        ///A test for getCredentialsFile
        ///</summary>
        [TestMethod()]
        [DeploymentItem("Loyalty_mate.dll")]
        [DeploymentItem("LoyaltyMateCredentials.info")]
        public void getCredentialsFileTest()
        { 
            XmlDocument actual;
            actual = target.getCredentialsFile();
            Assert.AreNotEqual(null, actual);
        }
        
        /// <summary>
        ///A test for getSalesforceCredentialsFromXML
        ///</summary>
        [TestMethod()]
        [DeploymentItem("Loyalty_mate.dll")]
        [DeploymentItem("LoyaltyMateCredentials.info")]
        public void getSalesforceCredentialsFromXMLTest()
        {
            target.getCredentialsFromXML();
            Assert.IsTrue(target.SalesforceCredentials.IsValid());
        }

        /// <summary>
        ///A test for loginToSalesforce
        ///</summary>
        [TestMethod()]
        [DeploymentItem("Loyalty_mate.dll")]
        [DeploymentItem("LoyaltyMateCredentials.info")]
        public void loginToSalesforceTest()
        {
            target.getCredentialsFromXML();
            LoginResult loginResult = null; 
            LoginResult loginResultExpected = null; 
            bool expected = true; 
            bool actual;
            actual = target.loginToSalesforce(target.SalesforceCredentials, out loginResult);
            Assert.AreNotEqual(loginResultExpected, loginResult);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for getSalesforceAccount
        ///</summary>
        [TestMethod()]
        [DeploymentItem("Loyalty_mate.dll")]
        [DeploymentItem("LoyaltyMateCredentials.info")]
        public void getSalesforceAccountTest()
        {
            target.getCredentialsFromXML();
            target.loginToSalesforce(target.SalesforceCredentials, out loginResult);
            target.sfLoginResult = loginResult;            
            Account expected = null; 
            Account actual;
            actual = target.getSalesforceAccount(siteId, syndicateCode);
            Assert.AreNotEqual(expected, actual);
        }


        /// <summary>
        ///A test for Get_syndicate_credentials
        ///</summary>
        [TestMethod()]
        [DeploymentItem("LoyaltyMateCredentials.info")]
        public void Get_syndicate_credentialsTest()
        { 
            Credentials actual;
            actual = target.Get_syndicate_credentials(syndicateCode,siteId);
            Assert.IsTrue(actual.IsValid());
        }
    }
}
