using System;

namespace OnlineOrdering.Exceptions
{
    public class GUIDNotFoundException : Exception
    {
        public GUIDNotFoundException()
            : base("404 : GUID doesn't Exist")
        {

        }
    }
}
