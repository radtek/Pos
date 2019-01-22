using System;
namespace Loyaltymate.Exceptions
{
    public class InvalidRequestException : Exception
    {
        public InvalidRequestException()
            : base("422 : Invalid Request")
        {
            
        }
    }
}