using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.WebMate.InternalClasses
{
    /// <summary>
    /// 
    /// </summary>
    public class ExceptionOrderGUIDDuplicate : Exception
    {
        public ExceptionOrderGUIDDuplicate(string inMessage) : base(inMessage)
        {
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public class ExceptionOrderEmpty : Exception
    {
        public ExceptionOrderEmpty(string inMessage)
            : base(inMessage)
        {
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public class ExceptionOrderInvalidSiteID : Exception
    {
        public ExceptionOrderInvalidSiteID(string inMessage)
            : base(inMessage)
        {
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public class ExceptionOrderEmptySiteName : Exception
    {
        public ExceptionOrderEmptySiteName(string inMessage)
            : base(inMessage)
        {
        }
    }
}
