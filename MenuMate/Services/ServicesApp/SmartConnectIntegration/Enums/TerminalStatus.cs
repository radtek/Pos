using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SmartConnectIntegration.Enums
{
    public enum TerminalStatus
    {
        READY = 1,
        NOT_INITIALIZED,
        NOT_LOGGED_ON, 
        REVERSAL_PENDING,
        OFFLINE 
    }
}
