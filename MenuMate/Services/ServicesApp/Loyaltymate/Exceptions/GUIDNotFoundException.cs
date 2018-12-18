using System;

namespace Loyaltymate.Exceptions
{
    public class GUIDNotFoundException :Exception
    {
        public GUIDNotFoundException()
            : base("404 : GUID doesn't Exist")
        {
            
        }
    }
}