using System;
using System.Collections.Specialized;
using System.Web;

namespace Loyalty_mate.Utilities.URI {

public sealed class Query {
        public
        Query()
        {
        }

        public Query
        Element(string name, string value = null)
        {
                elements_[HttpUtility.UrlEncode(name)] = value;
                return this;
        }

        public static
        implicit operator string(Query q)
        {
                return q.elements_.ToString();
        }

        NameValueCollection elements_ =
            HttpUtility.ParseQueryString(String.Empty);
} /* Query */

} /* Utilities.URI */
