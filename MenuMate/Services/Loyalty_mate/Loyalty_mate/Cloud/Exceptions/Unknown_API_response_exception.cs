namespace Loyalty_mate.Cloud.Exceptions {

using System.Net;

/// <summary>
///   Thrown when we get a result back from the cloud that doesn't meet
///   any of our expectations!
/// </summary>
public class Unknown_API_response_exception : Exception {
        public
        Unknown_API_response_exception(HttpWebResponse response)
            : base("The cloud's response makes no sense to me!")
        {
                Response = response;
        }

        public readonly HttpWebResponse Response;
} /* Unknown_API_response_exception */

} /* Loyalty_mate.Cloud */
