using System.Collections.Generic;

namespace Loyalty_mate.Utilities.Collections {

/// <summary>
///   Checks whether the input pair matches a specific condition.
/// </summary>
/// <typeparam name="K">
///   The key type.
/// </typeparam>
/// <typeparam name="V">
///   The value type.
/// </typeparam>
/// <param name="pair">
///   The pair to attempt a match on.
/// </param>
/// <returns>
///   True if the predicate matches, false otherwise.
/// </returns>
public delegate bool Predicate<K, V>(KeyValuePair<K, V> pair);

/// <summary>
///   A container for a few functions that extend the ways in which we
///   can locate values in a dictionary.
/// </summary>
public sealed class Dictionary {
        /// <summary>
        ///   Checks to see whether the dictionary contains something that
        ///   matches a given predicate.
        /// </summary>
        /// <typeparam name="K">
        ///   The key type.
        /// </typeparam>
        /// <typeparam name="V">
        ///   The value type.
        /// </typeparam>
        /// <param name="dictionary">
        ///   The dictionary to search.
        /// </param>
        /// <param name="predicate">
        ///   The predicate to be used when searching.
        /// </param>
        /// <returns></returns>
        public static bool
        Contains<K, V>(
            Dictionary<K, V> dictionary,
            Predicate<K, V> predicate)
        {
                bool present = dictionary.Count > 0;

                foreach (KeyValuePair<K, V> i in dictionary)
                        present &= predicate(i);

                return present;
        }

        /// <summary>
        ///   Returns all pairs that match a given predicate.
        /// </summary>
        /// <typeparam name="K">
        ///   The key type.
        /// </typeparam>
        /// <typeparam name="V">
        ///   The value type.
        /// </typeparam>
        /// <param name="dictionary">
        ///   The dictionary to search.
        /// </param>
        /// <param name="predicate">
        ///   The predicate to be used when searching.
        /// </param>
        /// <returns></returns>
        public static List<KeyValuePair<K, V> >
        Find<K, V>(Dictionary<K, V> dictionary,
                   Predicate<K, V> predicate)
        {
                List<KeyValuePair<K, V> > pairs =
                    new List<KeyValuePair<K, V> >();

                foreach (KeyValuePair<K, V> i in dictionary)
                        if (predicate(i))
                                pairs.Add(i);

                return pairs;
        }
} /* Dictionary_lookup */

} /* Utilities.Collections */
