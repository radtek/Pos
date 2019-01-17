

namespace MenumateServices.DTO.MenumateRegistration
{
    public class LicenceSettingMapping  
    {
        public long TerminalId { get; set; }
        public Terminal Terminal { get; set; }
        public long LicenceSettingId { get; set; }
        public LicenceSetting LicenceSettingSetting { get; set; }
        public bool IsEnabled { get; set; }
        public string Value { get; set; }
        public string Text { get; set; }
    }
}