using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MenumateServices.WebMate.DTO;

namespace WebMateTests.WebMateWCFService
{
    [TestClass]
    public class GetStatusTest
    {
        [TestMethod]
        public void GetStatus_Incomplete()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                WebOrderStatus status = WebOrderStatus.Unknown;
                var response = settings.Client.GetCommittedWebOrderStatus(out status, settings.siteName, settings.storeName, handle);
                Assert.IsTrue(response.Succesful);
                Assert.AreEqual(status , WebOrderStatus.Incomplete);
            }
        }

        [TestMethod]
        public void GetStatus_Committed()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                settings.Client.SaveIncompleteOrderHeaderSection(handle, settings.CreateHeaderSection());
                settings.Client.SaveIncompleteOrderFromSection(handle, settings.CreateFromSection());
                settings.Client.SaveIncompleteOrderAccountSection(handle, settings.CreateAccountSection());
                settings.Client.CommitOrder(handle);

                WebOrderStatus status = WebOrderStatus.Unknown;
                var response = settings.Client.GetCommittedWebOrderStatus(out status, settings.siteName, settings.storeName, handle);
                Assert.IsTrue(response.Succesful);
                Assert.AreEqual(status, WebOrderStatus.Committed);
            }
        }

        [TestMethod]
        public void GetStatus_Failed()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                settings.Client.SaveIncompleteOrderHeaderSection(handle, settings.CreateHeaderSection());
                settings.Client.SaveIncompleteOrderFromSection(handle, settings.CreateFromSection());
                DTO_WebOrderAccount account = settings.CreateAccountSection();
                account.OrderItems[0].UID = "50";
                settings.Client.SaveIncompleteOrderAccountSection(handle, account);
                settings.Client.CommitOrder(handle);

                WebOrderStatus status = WebOrderStatus.Unknown;
                var response = settings.Client.GetCommittedWebOrderStatus(out status, settings.siteName, settings.storeName, handle);
                Assert.IsTrue(response.Succesful);
                Assert.AreEqual(status, WebOrderStatus.NotAccepted);
            }
        }

        [TestMethod]
        public void GetStatus_Accepted()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                settings.Client.SaveIncompleteOrderHeaderSection(handle, settings.CreateHeaderSection());
                settings.Client.SaveIncompleteOrderFromSection(handle, settings.CreateFromSection());
                settings.Client.SaveIncompleteOrderAccountSection(handle, settings.CreateAccountSection());
                settings.Client.CommitOrder(handle);

                WebOrderStatus status = WebOrderStatus.Unknown;
                var response = settings.Client.GetCommittedWebOrderStatus(out status, settings.siteName, settings.storeName, handle);
                Assert.IsTrue(response.Succesful);
                Assert.AreEqual(status, WebOrderStatus.Accepted);
            }
        }

        [TestMethod]
        public void GetStatus_InvalidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "e6a9bb54-da25-102b-9a03-2db401e887ec";
                WebOrderStatus status = WebOrderStatus.Unknown;
                var response = settings.Client.GetCommittedWebOrderStatus(out status, settings.siteName, settings.storeName, handle);
                Assert.AreEqual(status, WebOrderStatus.Committed);
            }
        }
    }
}
