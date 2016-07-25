namespace Loyalty_mate.Utilities.OS.Exceptions.Registry {

/// <summary>
///   Thrown when someone tries to modify a key that's read-only.
/// </summary>
public class Modifying_read_only_key_exception : Registry_exception {
        public enum Faulting_operation {
                Creating_sub_key,
                Setting_value
        } /* Faulting_operation */

        /// <summary>
        ///   Creates an instance of the exception.
        /// </summary>
        /// <param name="iterator">
        ///   The iterator that raised the exception.
        /// </param>
        /// <param name="operation">
        ///   The operation that was being attempted.
        /// </param>
        /// <param name="name">
        ///   The name of the value or key that was being created or set.
        /// </param>
        public
        Modifying_read_only_key_exception(Loyalty_mate.Utilities.OS.Registry.Iterator iterator,
                                          Faulting_operation operation,
                                          string name)
            : base("Attempt to modify a read-only key.",
                   null,
                   iterator)
        {
                Operation = operation;
                Name = name;
        }

        public readonly Faulting_operation Operation;
        public readonly string Name;
} /* Modifying_read_only_key_exception */

} /* Utilities.OS */
