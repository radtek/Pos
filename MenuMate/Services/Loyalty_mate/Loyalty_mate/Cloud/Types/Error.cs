using System.Text.RegularExpressions;
using System.Collections.Generic;
using Loyalty_mate.Utilities.Collections;

namespace Loyalty_mate.Cloud.Responses {

/// <summary>
///   Represents an error returned from the cloud.
/// </summary>
public sealed class Error {
        public
        Error()
        {
        }

        /// <summary>
        ///   Message returned by the cloud when there's only a single error.
        /// </summary>
        public string error {
                get;
                set;
        }

        /// <summary>
        ///   A dictionary of error messages keyed on error class.
        /// </summary>
        public Dictionary<string, List<string> > errors {
                get;
                set;
        } 

        /// <summary>
        ///   Collects all error messages for errors with names matching pattern.
        /// </summary>
        /// <param name="pattern">
        ///   The pattern against which error names are matched.
        /// </param>
        /// <returns>
        ///   A list of all collected error messages.
        /// </returns>
        public List<string>
        Get_all_error_messages_for_matching_error_types(
            string pattern)
        {
                Predicate<string, List<string> > matcher =
                    delegate(KeyValuePair<string, List<string> > pair) {
                            return Regex.Matches(pair.Key, pattern).Count > 0;
                    };

                List<string> error_messages = new List<string>();
                List<KeyValuePair<string,
                                  List<string> > > matched_pairs =
                    Dictionary.Find<string, List<string> >(errors, matcher);

                foreach (KeyValuePair<string, List<string> > i in matched_pairs)
                        error_messages.AddRange(i.Value);

                return error_messages;
        }
} /* Error */

} /* Loyalty_mate.Cloud.Responses */
