namespace SalesForceCommunication.Domain
{
    public class SalesForceModuleStatus
    {
        public bool IsMobiToGoActivated { get; set; }
        public bool IsXeroActivated { get; set; }
        public bool IsThorlinkActivated { get; set; }
        public bool IsStockActivated { get; set; }
        public bool IsWebMateActivated { get; set; }
        public bool IsChefMateActivated { get; set; }
        public bool IsPalmMateActivated { get; set; }
        public bool IsLoyaltyActivated { get; set; }
        public string SalesForceVersion { get; set; }
        public string AccountName { get; set; }
    }
}