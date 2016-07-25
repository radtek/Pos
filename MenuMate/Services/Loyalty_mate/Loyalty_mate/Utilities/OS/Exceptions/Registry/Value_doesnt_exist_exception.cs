using System;

namespace Loyalty_mate.Utilities.OS.Exceptions.Registry {

/// <summary>
///   Thrown when the user tries to retrieve a value that doesn't exist from a key.
/// </summary>
public class Value_doesnt_exist_exception : Registry_exception {
        /// <summary>
        ///   Creates an instance of the exception.
        /// </summary>
        /// <param name="iterator">
        ///   The iterator that raised the exception.
        /// </param>
        /// <param name="value_name">
        ///   The value that was being retrieved.
        /// </param>
        public
        Value_doesnt_exist_exception(Loyalty_mate.Utilities.OS.Registry.Iterator iterator,
                                     string value_name)
            : base("The value specified doesn't exist in this key.",
                   null,
                   iterator)
        {
                Value_name = value_name;
        }

        /// <summary>
        ///   The name of the value that doesn't exist.
        /// </summary>
        public readonly string Value_name;
} /* Value_doesnt_exist_exception */

} /* Utilities.OS */
