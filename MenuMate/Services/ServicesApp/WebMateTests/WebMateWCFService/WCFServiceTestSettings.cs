using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.WebMate.DTO;

namespace WebMateTests.WebMateWCFService
{
    public class WCFServiceTestSettings : IDisposable
    {
        public WebMateWCFRef.WCFServiceWebMateClient Client;
        public string storeName;
        public string siteName;

        public WCFServiceTestSettings()
        {
            initVars();
            Client = new WebMateWCFRef.WCFServiceWebMateClient();
            Client.Open();
        }

        private void initVars()
        {
            storeName = "CoffeCulture";
            siteName = "Mobi2Go";
        }

        public void Dispose()
        {
            Client.Close();
            Client = null;
        }

        internal MenumateServices.WebMate.DTO.DTO_WebOrderHeader CreateHeaderSection()
        {
            return new MenumateServices.WebMate.DTO.DTO_WebOrderHeader()
            {
                ExpectedDate = DateTime.Now.AddHours(2),
                OrderDate = DateTime.Now,
                Respond = true,
                StoreName = storeName
            };
        }

        internal MenumateServices.WebMate.DTO.DTO_WebOrderFrom CreateFromSection()
        {
            return new MenumateServices.WebMate.DTO.DTO_WebOrderFrom()
            {
                SiteID = "7102",
                SiteName = siteName
            };
        }

        internal MenumateServices.WebMate.DTO.DTO_WebOrderAccount CreateAccountSection()
        {
            return new MenumateServices.WebMate.DTO.DTO_WebOrderAccount()
            {
                Comments = createComments(),
                Covers = 2,
                Delivery = createDeliverySection(),
                MemberNumber = "2572",
                Name = "Pete Josh",
                OrderItems = createOrderItems(),
                Payments = createPayments()
            };
        }

        private MenumateServices.WebMate.DTO.DTO_WebOrderPayments createPayments()
        {
            return new DTO_WebOrderPayments()
            {
                PaymentRequired = true
            };
        }

        private MenumateServices.WebMate.DTO.DTO_WebOrderItem[] createOrderItems()
        {
            return new DTO_WebOrderItem[]{
                new DTO_WebOrderItem(){ UID="105", Qty=2, BasePrice=27.50m, Note="Hello"},
                new DTO_WebOrderItem(){ UID="106", Qty=1, BasePrice=10.50m, Note="Hello"},
                new DTO_WebOrderItem(){ 
                    UID="108", 
                    Qty=3, 
                    BasePrice=5.00m,
                    Note ="Hello",
                    ItemSides = new DTO_WebOrderItemSide[]{
                        new DTO_WebOrderItemSide(){ UID="101",Qty=2, BasePrice=8.20m, Note="Hello"},
                        new DTO_WebOrderItemSide(){ UID="102",Qty=1, BasePrice=5.40m, Note="Hello"},
                    }
                },
            };
        }

        private MenumateServices.WebMate.DTO.DTO_WebOrderDelivery createDeliverySection()
        {
            return new MenumateServices.WebMate.DTO.DTO_WebOrderDelivery()
            {
                City = "Christchurch",
                Country = "New Zealand",
                Note = "This is a test note. item created as the unit test process",
                Phone = "0224563251",
                Recipient = "Nadun Perera",
                StreetName = "Carlyle Street",
                StreetNo = "25",
                Suburb = "Sydenham"
            };
        }

        private string[] createComments()
        {
            return new string[] { "Comment one", "Comment two", "Comment three" };
        }

    }
}
