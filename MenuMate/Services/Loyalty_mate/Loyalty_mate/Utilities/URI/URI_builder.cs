using System;

namespace Loyalty_mate.Utilities.URI {

public static class URI_builder {
        public static Uri
        Create(string scheme,
               string authority,
               string path = "",
               string query = "",
               string fragment = "")
        {
                string s = scheme + "://" + authority + "/" + path;

                if (query.Length > 0)
                        s += "?" + query;
                if (fragment.Length > 0)
                        s += "#" + fragment;

                return new Uri(s);
        }
} /* URI_builder */

} /* Utilities.URI */
