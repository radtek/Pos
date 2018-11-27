namespace MewsIntegration.Utilities
{
    public class MewsRequestAddress
    {
        public const string ServicesAll = "/api/connector/v1/services/getAll/";
        public const string AccountingCategoriesAll = "/api/connector/v1/accountingCategories/getAll/";
        public const string OutletsAll = "/api/connector/v1/outlets/getAll/";
        public const string CustomerSearchByName = "/api/connector/v1/customers/getAllByName/";
        public const string CustomerSearch = "/api/connector/v1/customers/search/";
        public const string SpaceIds = "/api/connector/v1/spaces/getAll/";
        public const string AddOrder = "/api/connector/v1/orders/add";
        public const string AddBill = "/api/connector/v1/outletBills/add";
       // public const string ClientToken = "E0D439EE522F44368DC78E1BFB03710C-D24FB11DBE31D4621C4817E028D9E1D";//Stagging
       public const string ClientToken = "DE25AC50EC564CDE8DF59B914B8AC03F-2ED92DEA9B15B14C31567BE6AC22F46";//Live
    }
}
