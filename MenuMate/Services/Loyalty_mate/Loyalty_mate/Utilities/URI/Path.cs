using System;
using System.Collections.Generic;
using System.Web;

namespace Loyalty_mate.Utilities.URI {

public sealed class Path {
        public
        Path()
        {
        }

        public
        Path(params string[] breadcrumb)
        {
                foreach (string i in breadcrumb) {
                        string[] path_components = i.Split('/');
                        foreach (string j in path_components)
                                Add_to_breadcrumb(j);
                }
        }

        public Path
        File(string file)
        {
                return Add_to_breadcrumb(file);
        }

        public Path
        Subdirectory(string subdirectory)
        {
                return Add_to_breadcrumb(subdirectory);
        }

        Path
        Add_to_breadcrumb(string value)
        {
                breadcrumb_.Add(value);
                return this;
        }

        public static
        implicit operator string(Path p)
        {
                string s = String.Join("/", p.breadcrumb_);
                return HttpUtility.UrlPathEncode(s);
        }

        List<string> breadcrumb_ = new List<string>();
} /* Path */

} /* Utilities.URI */
