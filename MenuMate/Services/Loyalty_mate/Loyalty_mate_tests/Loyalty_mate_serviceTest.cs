using Loyalty_mate.Cloud;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using Loyalty_mate.Types;
using Loyalty_mate.Cloud.Salesforce;
using System.Net;
using System.Security.Cryptography.X509Certificates;
using System.Net.Security;
using Loyalty_mate.Cloud.Types;

namespace Loyalty_mate_tests
{


    /// <summary>
    ///This is a test class for Loyalty_mate_serviceTest and is intended
    ///to contain all Loyalty_mate_serviceTest Unit Tests
    ///</summary>
    [TestClass()]
    public class Loyalty_mate_serviceTest
    {


        private TestContext testContextInstance;
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
            siteId = "7717";
            syndicateCode = "Underground124";
            //ServicePointManager.ServerCertificateValidationCallback = ignore_self_signed_certificates;
        }

        [TestCleanup]
        public void TearDown()
        {
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
        ///A test for Post_transaction
        ///</summary>
        [TestMethod()]
        [DeploymentItem("LoyaltyMateCredentials.info")]
        public void Post_transactionTest()
        {
            Credentials credentials = Salesforce_credential_provider.Instance.Get_syndicate_credentials(syndicateCode, siteId);
            bool immediately_authenticate = true;
            Loyalty_mate_service target = new Loyalty_mate_service(credentials, immediately_authenticate);
            Transaction transaction = createTransaction();
            target.Post_transaction(transaction);
        }

        /// <summary>
        ///A test for request member uuid with activation code
        ///</summary>
        [TestMethod()]
        [DeploymentItem("LoyaltyMateCredentials.info")]
        public void Get_member_by_activation_token_test()
        {
            Credentials credentials = Salesforce_credential_provider.Instance.Get_syndicate_credentials(syndicateCode, siteId);
            bool immediately_authenticate = true;
            Loyalty_mate_service target = new Loyalty_mate_service(credentials, immediately_authenticate);
            MemberActivationInfo token = target.Get_member_by_token("P4NUQ3SI");
            Assert.IsFalse(string.IsNullOrEmpty(token.uuid));
        }

        private Transaction createTransaction()
        {
            return new Transaction(
                "9a4e6b77-27b0-4a38-b36f-37067c95cb04",
                DateTime.Now.ToString(),
                "35",
                "5",
                "64",
                "14");
        }

        static bool
        ignore_self_signed_certificates(
                object o,
                X509Certificate server_certificate,
                X509Chain trust_chain,
                SslPolicyErrors ssl_errors)
        {
            return true;
        }
    }
}
