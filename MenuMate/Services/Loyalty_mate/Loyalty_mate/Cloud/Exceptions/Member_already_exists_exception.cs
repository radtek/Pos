using System;
using Loyalty_mate.Cloud.Exceptions.Exceptions;

namespace Loyalty_mate.Cloud.Exceptions {

/// <summary>
///   Thrown when a member being created already exists in the cloud.
/// </summary>
public class Member_already_exists_exception : Member_operation_exception {
        /// <summary>
        ///   Creates an instance of the exception.
        /// </summary>
        /// <param name="member">
        ///   The details of the already extant member.
        /// </param>
        public
        Member_already_exists_exception(Member member)
            : base("The member already exists in the cloud.")
        {
                Member = member;
        }

        public readonly Member Member;
} /* Member_already_exists_exception */

} /* Loyalty_mate.Cloud */
