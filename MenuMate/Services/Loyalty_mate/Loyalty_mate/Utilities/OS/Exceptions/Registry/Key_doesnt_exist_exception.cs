namespace Loyalty_mate.Utilities.OS.Exceptions.Registry {

/// <summary>
///   Thrown when the target key in a registry traversal doesn't exist.
/// </summary>
public class Key_doesnt_exist_exception : Registry_exception {
        /// <summary>
        ///   Creates an instance of the exception.
        /// </summary>
        /// <param name="iterator">
        ///   The iterator that raised the exception.
        /// </param>
        public
        Key_doesnt_exist_exception(Loyalty_mate.Utilities.OS.Registry.Iterator iterator)
            : base("The target key doesn't exist.", null, iterator)
        {
        }
} /* Key_doesnt_exist_exception */

} /* Utilities.OS */
