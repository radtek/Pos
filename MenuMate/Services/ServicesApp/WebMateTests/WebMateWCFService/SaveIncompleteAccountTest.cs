using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MenumateServices.WebMate.DTO;

namespace WebMateTests.WebMateWCFService
{
    [TestClass]
    public class SaveIncompleteAccountTest
    {
        [TestMethod]
        public void SaveAccount_ValidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                DTO_WebOrderAccount account = settings.CreateAccountSection();
                var response = settings.Client.SaveIncompleteOrderAccountSection(handle, account);
                Assert.IsTrue(response.Succesful);
            }
        }

        [TestMethod]
        public void SaveAccount_InvalidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "e6a9bb54-da25-102b-9a03-2db401e887ec";
                DTO_WebOrderAccount account = settings.CreateAccountSection();
                var response = settings.Client.SaveIncompleteOrderAccountSection(handle, account);
                Assert.IsFalse(response.Succesful);
            }
        }
    }
}
