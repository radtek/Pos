namespace SalesForceCommunication.Domain
{
    public class SalesForceCredential
    {
        public string UserName { get; set; }
        public string Password { get; set; }
        public string SecurityToken { get; set; }

        public SalesForceCredential()
        {
            UserName = "";
            Password = "";
            SecurityToken = "";
        }
    }
}