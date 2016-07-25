using System;

namespace Loyalty_mate.Utilities.JSON.Exceptions
{

public sealed class Invalid_JSON_exception : Exception {
        public
        Invalid_JSON_exception(string input)
            : base("The input to be deserialized is bogus.")
        {
                Input = input;
        }

        public readonly string Input;
} /* Invalid_JSON_exception */

} /* Utilities.JSON */
