using System;
namespace Loyaltymate.Exceptions
{
    public class NotFoundException : Exception
    {
        public NotFoundException()
            : base("404 : Api Not Found")
        {
            
        }
    }
}