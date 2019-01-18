using System.Collections.Generic;
using System.Runtime.Serialization;


namespace MenumateServices.DTO.MenumateRegistration
{
    [DataContract]
    public class Terminal 
    {
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public string TerminalCode { get; set; }
        [DataMember]
        public string MacAdress { get; set; }
        [DataMember]
        public string ComputerName { get; set; }
        [DataMember]
        public string OperatingSystemName { get; set; }
        [DataMember]
        public string MenumateVersionSystemName { get; set; }
        [DataMember]
        public long SiteId { get; set; }
        [DataMember]
        public Site Site { get; set; }
        [DataMember]
        public long TerminalProfileId { get; set; }
       // public Profile TerminalProfile { get; set; }
        [DataMember]
        public List<LicenceSettingMapping> LicenceSettingMappings { get; set; }
    }
}