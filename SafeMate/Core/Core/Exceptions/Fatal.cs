using SC = Safemate.Core;

using System;
using System.Collections.Generic;

namespace Safemate.Core.Exceptions
{
    public class Fatal : Base
    {
        public Fatal(Dictionary < String,
                     CopyOperationErrorBlock > accumulatedErrors)
        : base("Fatal error encountered, sending " +
               "exception global.")
        {
            this.accumulatedErrors = accumulatedErrors;
        }

        public Dictionary < String,
               CopyOperationErrorBlock > AccumulatedErrors
        {
            get {
                return accumulatedErrors;
            }
        }

        Dictionary < String,
        CopyOperationErrorBlock > accumulatedErrors;
    };
};
