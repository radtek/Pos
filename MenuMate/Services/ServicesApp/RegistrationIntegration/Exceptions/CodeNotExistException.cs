using System;
namespace RegistrationIntegration.Exceptions
{
    public class CodeNotExistException : Exception
    {
        public CodeNotExistException()
            : base("404 : Site code doesn't exist.")
        {
            
        }
    }
}