using MenumateServices.DTO.MenumateRegistration.Enums;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.MenumateRegistration   
{
    [DataContract]
    public class LicenceSetting
    {
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public bool IsEnabledByDefault { get; set; }
        [DataMember]
        public LicenceType SettingType { get; set; }
        [DataMember]
        public int SettingSubType { get; set; }
       // public SettingDisplayType SettingDisplayType { get; set; }
       // public SettingValueType SettingValueType { get; set; }
    }
}
    