using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MenumateServices.WebMate.DTO;

namespace WebMateTests.WebMateWCFService
{
    [TestClass]
    public class CommitOrderTest
    {
        [TestMethod]
        public void CommitOrder_InvalidHandle()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "ABC";
                var response = settings.Client.CommitOrder(handle);
                Assert.IsFalse(response.Succesful);
            }
        }

        [TestMethod]
        public void CommitOrder_InvalidSiteID()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                settings.Client.SaveIncompleteOrderHeaderSection(handle, settings.CreateHeaderSection());
                DTO_WebOrderFrom from = settings.CreateFromSection();
                from.SiteID = "";
                settings.Client.SaveIncompleteOrderFromSection(handle, from );
                settings.Client.SaveIncompleteOrderAccountSection(handle, settings.CreateAccountSection());
                var response = settings.Client.CommitOrder(handle);

                Assert.IsFalse(response.Succesful);
            }
        }

        [TestMethod]
        public void CommitOrder_InvalidSiteName()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                settings.Client.SaveIncompleteOrderHeaderSection(handle, settings.CreateHeaderSection());
                DTO_WebOrderFrom from = settings.CreateFromSection();
                from.SiteName = "";
                settings.Client.SaveIncompleteOrderFromSection(handle, from);
                settings.Client.SaveIncompleteOrderAccountSection(handle, settings.CreateAccountSection());
                var response = settings.Client.CommitOrder(handle);

                Assert.IsFalse(response.Succesful);
            }
        }

        [TestMethod]
        public void CommitOrder_EmptyItems()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                settings.Client.SaveIncompleteOrderHeaderSection(handle, settings.CreateHeaderSection());
                settings.Client.SaveIncompleteOrderFromSection(handle, settings.CreateFromSection());
                DTO_WebOrderAccount account = settings.CreateAccountSection();
                account.OrderItems = new DTO_WebOrderItem[]{};
                settings.Client.SaveIncompleteOrderAccountSection(handle, account);
                var response = settings.Client.CommitOrder(handle);

                Assert.IsFalse(response.Succesful);
            }
        }
    }
}
