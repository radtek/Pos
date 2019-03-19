using System;
namespace OnlineOrdering.Exceptions
{
    public class AuthenticationFailedException : Exception
    {
        public AuthenticationFailedException()
            : base("401 : Authentication Failed")
        {

        }
    }
}
