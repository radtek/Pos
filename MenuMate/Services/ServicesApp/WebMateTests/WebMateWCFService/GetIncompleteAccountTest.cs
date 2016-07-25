using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MenumateServices.WebMate.DTO;

namespace WebMateTests.WebMateWCFService
{
    [TestClass]
    public class GetIncompleteAccountTest
    {
        [TestMethod]
        public void GetAccount_ValidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                DTO_WebOrderAccount account = settings.CreateAccountSection();
                var response = settings.Client.SaveIncompleteOrderAccountSection(handle, account);
                DTO_WebOrderAccount accountPart = new DTO_WebOrderAccount();
                settings.Client.RetrieveIncompleteOrderAccountSection(out accountPart, handle);

                Assert.AreEqual(account.Comments.Length,accountPart.Comments.Length);
                Assert.AreEqual(account.Covers, accountPart.Covers);
                Assert.AreEqual(account.Delivery.City, accountPart.Delivery.City);
                Assert.AreEqual(account.Delivery.Country, accountPart.Delivery.Country);
                Assert.AreEqual(account.Delivery.Note, accountPart.Delivery.Note);
                Assert.AreEqual(account.Delivery.Phone, accountPart.Delivery.Phone);
                Assert.AreEqual(account.Delivery.Recipient, accountPart.Delivery.Recipient);
                Assert.AreEqual(account.Delivery.StreetName, accountPart.Delivery.StreetName);
                Assert.AreEqual(account.Delivery.StreetNo, accountPart.Delivery.StreetNo);
                Assert.AreEqual(account.Delivery.Suburb, accountPart.Delivery.Suburb);
                Assert.AreEqual(account.MemberNumber, accountPart.MemberNumber);
                Assert.AreEqual(account.Name, accountPart.Name);
                Assert.AreEqual(account.OrderItems.Length, accountPart.OrderItems.Length);
                Assert.AreEqual(account.Payments.PaymentRequired, accountPart.Payments.PaymentRequired);
            }
        }

        [TestMethod]
        public void GetAccount_InvalidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "e6a9bb54-da25-102b-9a03-2db401e887ec";
                DTO_WebOrderAccount account = new DTO_WebOrderAccount();
                var response = settings.Client.RetrieveIncompleteOrderAccountSection(out account, handle);
                Assert.IsFalse(response.Succesful);
            }
        }
    }
}
