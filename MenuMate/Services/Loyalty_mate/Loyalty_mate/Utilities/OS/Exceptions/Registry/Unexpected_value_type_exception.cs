using Microsoft.Win32;

namespace Loyalty_mate.Utilities.OS.Exceptions.Registry {

/// <summary>
///   Thrown when a value being retrieved isn't of the type the
///   user specified.
/// </summary>
public class Unexpected_value_type_exception : Registry_exception {
        /// <summary>
        ///   Creates an instance of this exception.
        /// </summary>
        /// <param name="desired_type">
        ///   The registry type that the user was expecting.
        /// </param>
        /// <param name="discovered_type">
        ///   The registry type that the value actually was.
        /// </param>
        /// <param name="iterator">
        ///   The iterator that threw the exception.
        /// </param>
        /// <param name="value_name">
        ///   The name of the value that was being retrieved.
        /// </param>
        public
        Unexpected_value_type_exception(RegistryValueKind desired_type,
                                        RegistryValueKind discovered_type,
                                        Loyalty_mate.Utilities.OS.Registry.Iterator iterator,
                                        string value_name)
            : base("The value specified isn't of the expected type.",
                   null,
                   iterator)
        {
                Desired_type = desired_type;
                Discovered_type = discovered_type;
                Value_name = value_name;
        }

        /// <summary>
        ///   The type that the user wanted.
        /// </summary>
        public readonly RegistryValueKind Desired_type;
        /// <summary>
        ///   The type that the value actually is.
        /// </summary>
        public readonly RegistryValueKind Discovered_type;
        /// <summary>
        ///   The name of the value that was being retrieved.
        /// </summary>
        public readonly string Value_name;
} /* Unexpected_value_type_exception */

} /* Utilities.OS */
