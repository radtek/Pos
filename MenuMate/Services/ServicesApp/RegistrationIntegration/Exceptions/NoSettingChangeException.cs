using System;

namespace RegistrationIntegration.Exceptions
{
    public class NoSettingChangeException :Exception
    {
        public NoSettingChangeException()
            : base("304 : No new Setting found for Update.")
        {
            
        }
    }
}