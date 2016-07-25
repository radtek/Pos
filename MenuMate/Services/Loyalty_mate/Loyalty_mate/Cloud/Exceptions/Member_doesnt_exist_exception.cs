using System;
using Loyalty_mate.Cloud.Exceptions.Exceptions;

namespace Loyalty_mate.Cloud.Exceptions {

/// <summary>
///   Thrown when a member being created already exist in the cloud.
/// </summary>
public class Member_doesnt_exist_exception : Member_operation_exception {
        /// <summary>
        ///   Creates an instance of the exception.
        /// </summary>
        /// <param name="uuid">
        ///   The UUID identifying the member.
        /// </param>
        public
        Member_doesnt_exist_exception(string uuid)
            : base("The member with this UUID doesn't exist.")
        {
                UUID = uuid;
        }

        public readonly string UUID;
} /* Member_doesnt_exist_exception */

} /* Loyalty_mate.Cloud */
