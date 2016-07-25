using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MenumateServices.WebMate.DTO;

namespace WebMateTests.WebMateWCFService
{
    [TestClass]
    public class GetIncompleteFromTest
    {
        [TestMethod]
        public void GetFrom_ValidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                DTO_WebOrderFrom from = settings.CreateFromSection();
                var response = settings.Client.SaveIncompleteOrderFromSection(handle, from);
                DTO_WebOrderFrom fromPart = new DTO_WebOrderFrom();
                settings.Client.RetrieveIncompleteOrderFromSection(out fromPart, handle);

                Assert.AreEqual(from.SiteID,fromPart.SiteID);
                Assert.AreEqual(from.SiteName, fromPart.SiteName);                
            }
        }

        [TestMethod]
        public void GetFrom_InvalidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "e6a9bb54-da25-102b-9a03-2db401e887ec";
                DTO_WebOrderFrom from = new DTO_WebOrderFrom();
                var response = settings.Client.RetrieveIncompleteOrderFromSection(out from, handle);
                Assert.IsFalse(response.Succesful);
            }
        }
    }
}
