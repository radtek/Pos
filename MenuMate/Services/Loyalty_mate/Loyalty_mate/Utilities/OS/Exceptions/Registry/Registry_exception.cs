using System;

namespace Loyalty_mate.Utilities.OS.Exceptions.Registry {

/// <summary>
///   The base class for errors relating to the registry.
/// </summary>
public class Registry_exception : Exception {
        /// <summary>
        ///   Initializing constructor.
        /// </summary>
        /// <param name="message">
        ///   A message describing the exception.
        /// </param>
        /// <param name="inner_exception">
        ///   The error encountered that triggered this.
        /// </param>
        /// <param name="iterator">
        ///   The iterator that was traversing the registry when
        ///   the error was encountered.
        /// </param>
        public
        Registry_exception(string message,
                           Exception inner_exception = null,
                           Loyalty_mate.Utilities.OS.Registry.Iterator iterator = null)
            : base(message, inner_exception)
        {
                Iterator = iterator;
        }

        /// <summary>
        ///   The iterator that threw the exception.
        /// </summary>
        public readonly Loyalty_mate.Utilities.OS.Registry.Iterator Iterator;
} /* Registry_exception */

} /* Utilities.OS */
