using Loyalty_mate.Utilities.OS;

namespace Loyalty_mate.Utilities.OS.Exceptions.Registry {

/// <summary>
///   Thrown when the user hands a path rather than a name to
///   an Iterator's Create_sub_key(...) method.
/// </summary>
public class Nested_registry_path_exception : Registry_exception {
        /// <summary>
        ///   Creates an instance of the exception.
        /// </summary>
        /// <param name="iterator">
        ///   The iterator that raised the exception.
        /// </param>
        /// <param name="bad_key_name">
        ///   The "key" the user tried to create.
        /// </param>
        public
        Nested_registry_path_exception(Loyalty_mate.Utilities.OS.Registry.Iterator iterator,
                                       string bad_key_name)
            : base("Was given a path, was expecting a simple name.",
                   null,
                   iterator)
        {
                Bad_key_name = bad_key_name;
        }

        /// <summary>
        ///   The key that the user gave. In this case, it'll be a path
        ///   rather than a simple key name.
        /// </summary>
        public readonly string Bad_key_name;
} /* Nested_registry_path_exception */

} /* Utilities.OS */
