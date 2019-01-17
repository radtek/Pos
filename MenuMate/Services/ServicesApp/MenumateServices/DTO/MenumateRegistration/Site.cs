using System.Collections.Generic;


namespace MenumateServices.DTO.MenumateRegistration
{
    public class Site
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public long SiteCode { get; set; }
        public long CompanyId { get; set; }
      //  public Company Company { get; set; }
        public List<Terminal> Terminals { get; set; }
        public long? SiteProfileId { get; set; }
    //    public Profile SiteProfile { get; set; }
       // public List<SiteTransactionSettingMapping> SiteTransactionSettingMappings { get; set; }

    }
}