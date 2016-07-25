using MenumateServices.InternalClasses.LoyaltyMate;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using MenumateServices.DTO.LoyaltyMate;
using System.Net;
using System.Security.Cryptography.X509Certificates;
using System.Net.Security;
using Loyalty_mate.Types;

namespace LoyaltyServiceTests
{


    /// <summary>
    ///This is a test class for LoyaltyMemberTest and is intended
    ///to contain all LoyaltyMemberTest Unit Tests
    ///</summary>
    [TestClass()]
    public class LoyaltyMemberTest
    {


        private TestContext testContextInstance;
        private string inSyndicateCode;
        private int inSiteID;
        private string inUUID;

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
            //ServicePointManager.ServerCertificateValidationCallback =
            //        ignore_self_signed_certificates;

            inSiteID = 7716;
            inSyndicateCode = "Underground124";
            inUUID = "7a690028-00ce-462c-9edb-9cbcd06e83a9";
        }
        //
        //Use TestCleanup to run code after each test has run
        [TestCleanup()]
        public void MyTestCleanup()
        {
        }
        //
        #endregion


        /// <summary>
        ///A test for createMemberInTheCloud
        ///</summary>
        [DeploymentItem("MenumateServices.exe"), DeploymentItem("SalesforceCredentials.info"), TestMethod()]
        public void createMemberInTheCloudTest()
        {
            LoyaltyMember_Accessor target = new LoyaltyMember_Accessor();
            DTO_MemberInfo inInfo = createMemberInfo();
            DTO_LoyaltyMemberResponse expected = null;
            DTO_LoyaltyMemberResponse actual;
            actual = target.createMemberInTheCloud(inSyndicateCode, inSiteID, inInfo);
        }

        private DTO_MemberInfo createMemberInfo()
        {
            DTO_MemberInfo info = new DTO_MemberInfo();
            info.CellPhone = "0221239584";
            info.EarnedPoints = 50;
            info.LoadedPoints = 30;
            info.EMailAddress = "jp@hotmail.com";
            info.Name = "CDE";
            return info;
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

        /// <summary>
        ///A test for memberExistsInTheCloud
        ///</summary>
        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void memberExistsInTheCloudTest()
        {
            LoyaltyMember_Accessor target = new LoyaltyMember_Accessor();
            bool expected = true;
            bool actual;
            actual = target.memberExistsInTheCloud(inSyndicateCode, inSiteID, inUUID);
            Assert.AreEqual(expected, actual);
        }

        /// <summary>
        ///A test for getMemberFromTheCloud
        ///</summary>
        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        [DeploymentItem("SalesforceCredentials.info")]
        public void getMemberFromTheCloudTest()
        {
            LoyaltyMember_Accessor target = new LoyaltyMember_Accessor();
            DTO_LoyaltyMemberResponse actual;
            //actual = target.getMemberFromTheCloud(inSyndicateCode, inSiteID, inUUID);
            actual = target.Get(inSyndicateCode, inSiteID, inUUID);
            Assert.AreEqual(inUUID, actual.MemberInfo.UUID);
            Assert.IsTrue(actual.ResponseCode == LoyaltyResponseCode.MemberDoesNotExistInTheCloud);
        }

        /// <summary>
        ///A test for getMemberListFromTheCloud
        ///</summary>
        [TestMethod()]
        public void getMemberListFromTheCloudTest()
        {
            LoyaltyMember target = new LoyaltyMember();
            DTO_LoyaltyMemberListResponse actual;
            actual = target.getMemberListFromTheCloud(inSyndicateCode, inSiteID);
            Assert.IsTrue(actual.MemberList.Length > 0);
        }

        /// <summary>
        ///A test for updateMemberInTheCloud
        ///</summary>
        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void updateMemberInTheCloudTest()
        {
            LoyaltyMember_Accessor target = new LoyaltyMember_Accessor();
            DTO_MemberInfo inInfo = target.getMemberFromTheCloud(inSyndicateCode, inSiteID, inUUID).MemberInfo;
            inInfo.EarnedPoints = 50;
            inInfo.LoadedPoints = 30;
            DTO_MemberInfo expected = inInfo;
            DTO_LoyaltyMemberResponse response = target.updateMemberInTheCloud(inSyndicateCode, inSiteID, inInfo);
            var retrievedInfo = target.getMemberFromTheCloud(inSyndicateCode, inSiteID, inUUID);
            Assert.IsTrue(compareMemberInformation(expected, retrievedInfo));
        }

        private bool compareMemberInformation(DTO_MemberInfo expected, DTO_LoyaltyMemberResponse actual)
        {
            return expected.Activated == actual.MemberInfo.Activated
                && expected.CellPhone == actual.MemberInfo.CellPhone
                && expected.DateOfBirth == actual.MemberInfo.DateOfBirth
                && expected.EarnedPoints == actual.MemberInfo.EarnedPoints
                && expected.EMailAddress == actual.MemberInfo.EMailAddress
                && expected.LoadedPoints == actual.MemberInfo.LoadedPoints
                && expected.Name == actual.MemberInfo.Name
                && expected.UUID == actual.MemberInfo.UUID;
        }

        /// <summary>
        ///A test for deleteMemberFromTheCloud
        ///</summary>
        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void deleteMemberFromTheCloudTest()
        {
            LoyaltyMember_Accessor target = new LoyaltyMember_Accessor();
            DTO_LoyaltyResponse actual;
            actual = target.deleteMemberFromTheCloud(inSyndicateCode, inSiteID, inUUID);
            Assert.IsTrue(actual.ResponseCode == LoyaltyResponseCode.Successful);
        }

        /// <summary>
        ///A test for createCloudCredentials
        ///</summary>
        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void createCloudCredentialsTest()
        {
            LoyaltyMember_Accessor target = new LoyaltyMember_Accessor();
            Credentials actual;
            actual = target.createCloudCredentials(inSyndicateCode, inSiteID);
            Assert.AreNotEqual(null, actual);
        }

        /// <summary>
        ///A test for Update
        ///</summary>
        [TestMethod()]
        public void UpdateTest()
        {
            LoyaltyMember target = new LoyaltyMember();
            DTO_MemberInfo inInfo = target.Get(inSyndicateCode, inSiteID, inUUID).MemberInfo;
            inInfo.DateOfBirth = DateTime.Parse("1986/12/04");
            inInfo.CellPhone = "0221239584";
            inInfo.Name = "Nadun";
            inInfo.EMailAddress = "LMN@gmail.com";
            inInfo.Activated = true;
            bool inForceCreate = true;
            DTO_LoyaltyMemberResponse actual;
            actual = target.Update(inSyndicateCode, inSiteID, inInfo, inForceCreate);
            Assert.IsTrue(actual.ResponseCode == LoyaltyResponseCode.Successful);
        }

        /// <summary>
        ///A test for PostTransaction
        ///</summary>
        [TestMethod()]
        [DeploymentItem("SalesforceCredentials.info")]
        public void PostTransactionTest()
        {
            LoyaltyMember target = new LoyaltyMember();
            DTO_TransactionInfo transaction = createTransactionObject();
            DTO_LoyaltyResponse actual;
            actual = target.PostTransaction(inSyndicateCode, inSiteID, transaction);
            Assert.IsTrue(actual.ResponseCode == LoyaltyResponseCode.Successful);
        }

        private DTO_TransactionInfo createTransactionObject()
        {
            return new DTO_TransactionInfo()
            {
                UUID = "9a4e6b77-27b0-4a38-b36f-37067c95cb04",
                OccurredAt = DateTime.Now,
                EarnedPoints = 20.8,
                EarnedPointsDelta = 10,
                LoadedPoints = 10.5,
                LoadedPointsDelta = 8
            };
        }
    }
}
