using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Loyalty_mate_wcf_service_test.LoyaltyMateWCFServiceRef;
using System.IO;
using System.Net;
using System.Security.Cryptography.X509Certificates;
using System.Net.Security;

namespace Loyalty_mate_wcf_service_test
{
    [TestClass]
    public class WCFServiceTests
    {
        private LoyaltyMateWCFServiceRef.WCFServiceLoyaltyMateClient client;
        private string syndicateCode;
        private int siteId;
        private string clouduuid;

        [TestInitialize]
        public void Setup()
        {            
            client  = new LoyaltyMateWCFServiceRef.WCFServiceLoyaltyMateClient();
            siteId = 7717;
            syndicateCode = "Underground124";
            clouduuid = "ab1e69f5-3ac2-44a2-8325-b7f51e2a85bd";
            //client.SetSalesforceAuthInfo(getSalesforceAuthInfo());
            client.Open();
        }

        private string getCloudUUIDFromFile()
        {
            FileStream fs = File.OpenRead("UUID.txt");
            byte[] strBytes = null;
            fs.Read(strBytes, 0, 1000);
            fs.Close();
            string str = System.Text.Encoding.UTF8.GetString(strBytes);
            return str;
        }

        private void saveCloudUUIDToFile(string uuid)
        {
            FileStream fs = File.Open("UUID.txt", FileMode.OpenOrCreate, FileAccess.Write);
            byte[] strBytes = System.Text.Encoding.UTF8.GetBytes(uuid);
            fs.Write(strBytes, 0, strBytes.Length);
            fs.Flush();
            fs.Close();
        }
        
        [TestMethod]
        public void GetMemberTest()
        {
            DTO_LoyaltyMemberResponse response = client.GetMember(syndicateCode, siteId, clouduuid);
            Assert.AreEqual(true, response.Succesful);
        }

        [TestMethod]
        public void CreateMemberTest()
        {
            DTO_LoyaltyMemberResponse response = client.CreateMember(syndicateCode, siteId, createMemberInfo());
            Assert.AreEqual(true, response.Succesful);
            Assert.IsTrue(!string.IsNullOrEmpty(response.MemberInfo.UUID));
            //saveCloudUUIDToFile(response.MemberInfo.UUID);
        }

        [TestMethod]
        public void UpdateMemberTest()
        {

                DTO_MemberInfo info = createMemberInfo();
                info.Name = "Sam Gamage";
                info.DateOfBirth = DateTime.Parse("1986/05/20");
                info.CellPhone = "0362230525";
                info.UUID = "ab1e69f5-3ac2-44a2-8325-b7f51e2a85bd";
                DTO_LoyaltyMemberResponse response = client.UpdateMember(syndicateCode, siteId, info, true);
                if (response.Succesful)
                {
                    response = client.GetMember(syndicateCode, siteId, info.UUID);
                    Assert.AreEqual(info.Name, response.MemberInfo.Name);
                }
                else
                    Assert.Fail("Failed to update member information");
            
        }

        [TestMethod]
        public void GetMemberListTest()
        {
            DTO_LoyaltyMemberListResponse response = client.GetMemberList(syndicateCode, siteId);
            Assert.AreEqual(true, response.Succesful);
            Assert.IsTrue(response.MemberList.Count > 0);
        }
        
        [TestMethod]
        public void DeleteMemberTest()
        {
            DTO_LoyaltyResponse response = client.DeleteMember(syndicateCode, siteId, clouduuid);
            Assert.AreEqual(true, response.Succesful);
        }

        [TestMethod]
        public void PostTransactionTest()
        {
            DTO_LoyaltyResponse response = client.PostTransaction(syndicateCode, siteId, createTransactionObject());
            Assert.AreEqual(true, response.Succesful);
        }

        private DTO_TransactionInfo createTransactionObject()
        {
            return new DTO_TransactionInfo()
            {
                UUID = "9a4e6b77-27b0-4a38-b36f-37067c95cb04",
                OccurredAt = DateTime.Now.AddDays(2),
                EarnedPoints = 10.5,
                EarnedPointsDelta = 17,
                LoadedPoints = 20.2,
                LoadedPointsDelta = 8
            };
        }

        private DTO_MemberInfo createMemberInfo()
        {
            DTO_MemberInfo info = new DTO_MemberInfo();
            info.CellPhone = "0221239584";
            info.EarnedPoints = 50;
            info.LoadedPoints = 30;
            info.EMailAddress = "nadun_dev@hotmail.com";
            info.Activated = true;
            info.Name = "Nadun";
            return info;
        }

        [TestMethod()]
        public void LoyaltyMateServiceCreationTest()
        {
            bool status = false;
            DTO_LoyaltyMemberResponse response;
            DTO_MemberInfo info = createMemberInfo();            
            info.UUID = clouduuid;
            info.EMailAddress = "acn@gmail.com";
            for (int i = 0; i <= 1; i++)
            {
                //Thread.Sleep(100);
                try
                {
                    info.Name = "Sam_" + i;
                    //response = client.UpdateMember(syndicateCode, siteId, info, false);
                    response = client.GetMember(syndicateCode, siteId, clouduuid);
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

        [TestCleanup]
        public void TearDown()
        {
            client.Close();
        }
    }
}
