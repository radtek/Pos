using System;
namespace Loyaltymate.Exceptions
{
    public class AuthenticationFailedException :Exception
    {
        public AuthenticationFailedException()
            : base("401 : Authentication Failed")
        {
            
        }
    }
}