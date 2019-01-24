using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.MenumateRegistration
{
    [DataContract]
    public class TerminalModel
    {
        [DataMember]
        public long SiteCode { get; set; }
        [DataMember]
        public string SyndicateCode { get; set; }
        [DataMember]
        public string TerminalName { get; set; }
        [DataMember]
        public string TerminalDescription { get; set; }
        [DataMember]
        public string StaffName { get; set; }
        [DataMember]
        public string MacAdress { get; set; }
        [DataMember]
        public string ComputerName { get; set; }
        [DataMember]
        public string OperatingSystemName { get; set; }
        [DataMember]
        public string MenumateVersion { get; set; }
        [DataMember]
        public List<LicenceSettingModel> LicenceSettingsModel { get; set; }
    }
}
