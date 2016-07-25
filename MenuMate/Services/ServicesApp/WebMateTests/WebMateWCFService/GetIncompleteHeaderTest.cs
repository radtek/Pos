using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MenumateServices.WebMate.DTO;

namespace WebMateTests.WebMateWCFService
{
    [TestClass]
    public class GetIncompleteHeaderTest
    {
        [TestMethod]
        public void GetHeader_ValidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                DTO_WebOrderHeader header = settings.CreateHeaderSection();
                var response = settings.Client.SaveIncompleteOrderHeaderSection(handle, header);
                DTO_WebOrderHeader headerPart = new DTO_WebOrderHeader();
                settings.Client.RetrieveIncompleteOrderHeaderSection(out headerPart, handle);

                Assert.AreEqual(header.StoreName,headerPart.StoreName);
                Assert.AreEqual(header.Scheduled, headerPart.Scheduled);
                Assert.AreEqual(header.Respond, headerPart.Respond);
                Assert.AreEqual(header.OrderTotal, headerPart.OrderTotal);
                Assert.AreEqual(header.OrderDate.ToShortDateString(), headerPart.OrderDate.ToShortDateString());
                Assert.AreEqual(header.ExpectedDate.ToShortDateString(), headerPart.ExpectedDate.ToShortDateString());
            }
        }

        [TestMethod]
        public void GetHeader_InvalidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "e6a9bb54-da25-102b-9a03-2db401e887ec";
                DTO_WebOrderHeader header = new DTO_WebOrderHeader();
                var response = settings.Client.RetrieveIncompleteOrderHeaderSection(out header, handle);
                Assert.IsFalse(response.Succesful);
            }
        }
    }
}
