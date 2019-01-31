using System;

namespace RegistrationIntegration.Exceptions
{
    public class BadRequestException :Exception
    {
        public BadRequestException()
            : base("300 : Bad Request Exception.")
        {
            
        }
    }
}