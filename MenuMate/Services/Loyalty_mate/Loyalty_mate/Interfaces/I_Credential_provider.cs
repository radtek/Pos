using Loyalty_mate.Types;

namespace Loyalty_mate.Interfaces
{

/// <summary>
///   Provides a way for users to retrieve their ``base'' credentials. These
///   credentials are used to authenticate with Loyalty mate in the cloud.
/// </summary>
public interface I_Credential_provider {
        /// <summary>
        ///   Retrieves the base authentication credentials for a syndicate.
        /// </summary>
        /// <param name="syndicate_code">
        ///   The syndicate code uniquely identifying a syndicate.
        /// </param>
        /// <returns>
        ///   The credentials of that syndicate.
        /// </returns>
    Credentials Get_syndicate_credentials(
                     string inSyndicateCode,
                     string inSiteID);
} /* I_Credential_provider */

} /* Loyalty_mate */
