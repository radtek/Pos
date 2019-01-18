using System.Runtime.Serialization;

namespace MenumateServices.DTO.MenumateRegistration
{
    [DataContract]
    public class LicenceSettingMapping  
    {
        [DataMember]
        public long TerminalId { get; set; }
        [DataMember]
        public Terminal Terminal { get; set; }
        [DataMember]
        public long LicenceSettingId { get; set; }
        [DataMember]
        public LicenceSetting LicenceSettingSetting { get; set; }
        [DataMember]
        public bool IsEnabled { get; set; }
        [DataMember]
        public string Value { get; set; }
        [DataMember]
        public string Text { get; set; }
    }
}