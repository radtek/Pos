using System;
namespace Loyaltymate.Exceptions
{
    public class ApiDownException : Exception
    {
        public ApiDownException()
            :base("500 : Api Down")
        {
            
        }
    }
}