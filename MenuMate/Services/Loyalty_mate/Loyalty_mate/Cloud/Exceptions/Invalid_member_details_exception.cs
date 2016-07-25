using System;
using Loyalty_mate.Cloud.Exceptions.Exceptions;

namespace Loyalty_mate.Cloud.Exceptions {

/// <summary>
///   Thrown when the details used to create a member are invalid in some way.
/// </summary>
public class Invalid_member_details_exception : Member_operation_exception {
        /// <summary>
        ///   Creates an instance of the exception.
        /// </summary>
        /// <param name="member">
        ///   The member definition with invalid details.
        /// </param>
        public
        Invalid_member_details_exception(Member member)
            : base("Invalid member details")
        {
                Member = member;
        }

        /// <summary>
        ///   Creates an instance of the exception.
        /// </summary>
        /// <param name="message">
        ///   A message describing the exception.
        /// </param>
        /// <param name="member">
        ///   The member definition with invalid details.
        /// </param>
        public
        Invalid_member_details_exception(string message, Member member)
            : base(message)
        {
                Member = member;
        }

        public readonly Member Member;
} /* Invalid_member_details_exception */

} /* Loyalty_mate.Cloud */
