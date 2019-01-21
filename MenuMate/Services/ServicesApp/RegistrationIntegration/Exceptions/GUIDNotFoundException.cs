using System;

namespace RegistrationIntegration.Exceptions
{
    public class GUIDNotFoundException :Exception
    {
        public GUIDNotFoundException()
            : base("404 : GUID doesn't Exist")
        {
            
        }
    }
}