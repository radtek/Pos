using Loyalty_mate.Utilities.JSON;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using Loyalty_mate.Cloud;
using Loyalty_mate.Utilities.JSON.Converters;
using Loyalty_mate.Types;
using Loyalty_mate.Cloud.Salesforce;
using System.Threading;
using System.Diagnostics;
using System.Net;
using System.Security.Cryptography.X509Certificates;
using System.Net.Security;

namespace Loyalty_mate_tests
{
    
    
    /// <summary>
    ///This is a test class for JSONTest and is intended
    ///to contain all JSONTest Unit Tests
    ///</summary>
    [TestClass()]
    public class JSONTest
    {


        private TestContext testContextInstance;

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
        [TestInitialize()]
        public void MyTestInitialize()
        {
            ServicePointManager.ServerCertificateValidationCallback = ignore_self_signed_certificates;
        }
        //
        //Use TestCleanup to run code after each test has run
        [TestCleanup()]
        public void MyTestCleanup()
        {
        }
        //
        #endregion


        private Member createMemberObject()
        {
            Member m = new Member()
            {
                account = new Account("sam@gmail.com"),
                activated = true,
                activation_token = "123456",
                earned_points="50",
                loaded_points="40",
                profile=new Loyalty_mate.Cloud.Types.Profile(
                    "nadun",
                    "0221239586",
                    DateTime.Parse("1986/12/18"),
                    "Mr","Nadun","Parera","CHCH","CHCH","CH","CH","123333","NZ","1234"),
                uuid="ABCDEFGT",
                member_misc_info = new Loyalty_mate.Cloud.Types.MiscInfo(DateTime.Parse("1986/12/18"),"1",
                    DateTime.Parse("1986/12/18"),"POS3",DateTime.Parse("1986/12/18"),"1234","abcd")
            };

            return m;
        }

        [TestMethod()]
        public void SerializeTest()
        {
            Member member = createMemberObject();
            string strOne = JSON.Serialize<Member>(member);

            JSON.Register_Converters(new[] { new MemberProfileConverter() });
            string strTwo = JSON.Serialize<Member>(member);

            Assert.AreNotEqual(strOne, strTwo);                       
        }

        [TestMethod()]
        [DeploymentItem("Loyalty_mate.dll")]
        [DeploymentItem("LoyaltyMateCredentials.info")]
        public void LoyaltyMateServiceCreationTest()
        {
            bool status = false;
            Salesforce_credential_provider provider = new Salesforce_credential_provider();
            Credentials crd = provider.Get_syndicate_credentials("Underground124","7717");
            for (int i = 0; i <= 100; i++)
            {
                //Thread.Sleep(100);
                try
                {
                    Trace.WriteLine(string.Format("Creating instance: {0}", i));
                    Loyalty_mate_service service = new Loyalty_mate_service(crd);
                    status = true;
                }
                catch
                {
                    status = false;
                    throw;
                }
            }

            Assert.AreNotEqual(false, status);
        }

        [TestMethod()]
        [DeploymentItem("Loyalty_mate.dll")]
        [DeploymentItem("LoyaltyMateCredentials.info")]
        public void LoyaltyMateServiceRepeatedRetrieveTest()
        {
            bool status = false;
            Salesforce_credential_provider provider = new Salesforce_credential_provider();
            Credentials crd = provider.Get_syndicate_credentials("Underground124", "7717");
            string uuid = "17ef6150-f12c-412a-b503-b12dbf0a368f";
            Member m = createMemberObject();
            m.account.email = "acn@gmail.com";
            m.uuid = uuid;
            for (int i = 0; i <= 3; i++)
            {
                //Thread.Sleep(100);
                try
                {
                    Loyalty_mate_service service = new Loyalty_mate_service(crd);
                    m.profile.name = "acn_" + i;
                    service.Update_member_details(m);
                    m = service.Get_member_details(uuid);
                    status = true;
                }
                catch
                {
                    status = false;
                    throw;
                }
            }

            Assert.AreNotEqual(false, status);
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
