using System.Collections.Generic;
using System.Runtime.Serialization;


namespace MenumateServices.DTO.MenumateRegistration
{
    [DataContract]
    public class Site
    {
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public long SiteCode { get; set; }
        [DataMember]
        public long CompanyId { get; set; }
        [DataMember]
      //  public Company Company { get; set; }
        public List<Terminal> Terminals { get; set; }
        [DataMember]
        public long? SiteProfileId { get; set; }
    //    public Profile SiteProfile { get; set; }
       // public List<SiteTransactionSettingMapping> SiteTransactionSettingMappings { get; set; }

    }
}