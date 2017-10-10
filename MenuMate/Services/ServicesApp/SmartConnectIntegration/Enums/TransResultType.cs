﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SmartConnectIntegration.Enums
{
    public enum TransResultType
    {
        OK_ACCEPTED = 1, 
        OK_DECLINED,
        OK_UNAVAILABLE,
        CANCELLED,
        FAILED, 
        FAILED_INTERFACE 
    }
}
