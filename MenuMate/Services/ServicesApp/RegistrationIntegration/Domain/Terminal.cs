using System.Collections.Generic;
using System.Runtime.Serialization;


namespace RegistrationIntegration.Domain
{
    public class Terminal 
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public string TerminalCode { get; set; }
        public string MacAdress { get; set; }
        public string ComputerName { get; set; }
        public string OperatingSystemName { get; set; }
        public string MenumateVersionSystemName { get; set; }
        public long SiteId { get; set; }
        public Site Site { get; set; }
        public long TerminalProfileId { get; set; }
       // public Profile TerminalProfile { get; set; }
        public List<LicenceSettingMapping> LicenceSettingMappings { get; set; }
    }
}