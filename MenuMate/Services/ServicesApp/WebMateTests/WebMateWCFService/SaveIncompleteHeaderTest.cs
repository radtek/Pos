using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MenumateServices.WebMate.DTO;

namespace WebMateTests.WebMateWCFService
{
    [TestClass]
    public class SaveIncompleteHeaderTest
    {
        [TestMethod]
        public void SaveHeader_ValidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                DTO_WebOrderHeader header = settings.CreateHeaderSection();
                var response = settings.Client.SaveIncompleteOrderHeaderSection(handle, header);
                Assert.IsTrue(response.Succesful);
            }
        }

        [TestMethod]
        public void SaveHeader_InvalidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "e6a9bb54-da25-102b-9a03-2db401e887ec";
                DTO_WebOrderHeader header = settings.CreateHeaderSection();
                var response = settings.Client.SaveIncompleteOrderHeaderSection(handle, header);
                Assert.IsFalse(response.Succesful);
            }
        }
    }
}
