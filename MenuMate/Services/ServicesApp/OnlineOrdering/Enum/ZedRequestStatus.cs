using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Enum
{
    public enum ZedRequestStatus
    {
        ZedRequestAccepted = 0,
        ZedRequestAlreadyPresent,
        ZedRequestInsertionError,
        ZedRequestNotEnabled
    }
}
