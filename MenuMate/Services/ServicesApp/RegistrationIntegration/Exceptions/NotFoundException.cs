using System;
namespace RegistrationIntegration.Exceptions
{
    public class NotFoundException : Exception
    {
        public NotFoundException()
            : base("404 : Site code doesn't exist.")
        {
            
        }
    }
}