using MenumateServices.DTO.MenumateRegistration.Enums;

namespace MenumateServices.DTO.MenumateRegistration   
{
    public class LicenceSetting
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public bool IsEnabledByDefault { get; set; }
        public LicenceType SettingType { get; set; }
        public int SettingSubType { get; set; }
       // public SettingDisplayType SettingDisplayType { get; set; }
       // public SettingValueType SettingValueType { get; set; }
    }
}
    