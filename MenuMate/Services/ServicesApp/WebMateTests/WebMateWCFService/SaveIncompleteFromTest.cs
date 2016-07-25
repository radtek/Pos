using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MenumateServices.WebMate.DTO;

namespace WebMateTests.WebMateWCFService
{
    [TestClass]
    public class SaveIncompleteFromTest
    {
        [TestMethod]
        public void SaveFrom_ValidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                DTO_WebOrderFrom from = settings.CreateFromSection();
                var response = settings.Client.SaveIncompleteOrderFromSection(handle, from);
                Assert.IsTrue(response.Succesful);
            }
        }

        [TestMethod]
        public void SaveFrom_InvalidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "e6a9bb54-da25-102b-9a03-2db401e887ec";
                DTO_WebOrderFrom from = settings.CreateFromSection();
                var response = settings.Client.SaveIncompleteOrderFromSection(handle, from);
                Assert.IsFalse(response.Succesful);
            }
        }
    }
}
