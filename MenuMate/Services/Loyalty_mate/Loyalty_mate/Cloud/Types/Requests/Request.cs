using System;
using Loyalty_mate.Utilities.URI;
using Loyalty_mate.Utilities.JSON;
using System.Net;
using Loyalty_mate.Cloud.Exceptions;

namespace Loyalty_mate.Cloud.Types.Requests {

/// <summary>
///   The base class for requests made to the Cloud.
/// </summary>
public abstract class Request {
        /// <summary>
        ///   Represents an action to perform when a request is made.
        /// </summary>
        /// <param name="request">
        ///   The request object that's triggering the action to occur.
        /// </param>
        public delegate void Execution_hook(object request);

        /// <summary>
        ///   Actions to execute before a request is made. Useful for diagnostics.
        /// </summary>
        public event Execution_hook Pre_execution_hooks;
        /// <summary>
        ///   Actions to execute after the request has been made. Useful for
        ///   diagnostics.
        /// </summary>
        public event Execution_hook Post_execution_hooks;

        /// <summary>
        ///   Creates the request.
        /// </summary>
        /// <param name="base_request_address">
        ///   The base address to be used when making the request. I.e. the request
        ///   address without query components or fragments appended.
        /// </param>
        protected
        Request(string base_request_address)
        {
                Base_request_address = base_request_address;
        }

        /// <summary>
        ///   The request driver. This triggers the execution of the request proper
        ///   and all hook chains.
        /// </summary>
        public virtual void
        Execute()
        {
                try {
                        if (Pre_execution_hooks != null)
                                Pre_execution_hooks(this);

                        On_execute();

                        if (Post_execution_hooks != null)
                                Post_execution_hooks(this);
                } catch (WebException we) {
                        HttpWebResponse r = (HttpWebResponse)we.Response;

                        if (r == null)
                                throw we;

                        if (we.Status == WebExceptionStatus.Timeout)
                            throw new Operation_timed_out_exception();

                        switch (r.StatusCode) {
                        case HttpStatusCode.InternalServerError:
                                throw new API_down_exception();
                        case HttpStatusCode.NotFound:
                                throw new API_request_inaccessible_exception(we);
                        default:
                                throw we;
                        }
                }
        }

        /// <summary>
        ///   Does what has to be done to make the request.
        /// </summary>
        protected abstract void On_execute();

        /// <summary>
        ///   The address used to perform the request without query
        ///   or fragment components appended.
        /// </summary>
        protected readonly string Base_request_address;
} /* Request */

}
