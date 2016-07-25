using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web.Script.Serialization;
using System.Reflection;
using Loyalty_mate.Cloud.Types;

namespace Loyalty_mate.Utilities.JSON.Converters
{
    public class MemberProfileConverter : JavaScriptConverter
    {
        public override object Deserialize(IDictionary<string, object> dictionary, Type type, JavaScriptSerializer serializer)
        {
            return new JavaScriptSerializer().ConvertToType(dictionary, type);
        }

        public override IDictionary<string, object> Serialize(object obj, JavaScriptSerializer serializer)
        {
            IDictionary<string, object> serializedContent = new Dictionary<string, object>();
            foreach (PropertyInfo pi in obj.GetType().GetProperties())
            {
                if (pi.PropertyType == typeof(Nullable<DateTime>))
                {
                    Nullable<DateTime> dt = pi.GetValue(obj,null) as Nullable<DateTime>;
                    if (dt.HasValue)
                        serializedContent[pi.Name] = dt.Value.ToString("o");
                }
                else if (pi.PropertyType == typeof(String))
                {
                    serializedContent[pi.Name] = Convert.ToString(pi.GetValue(obj, null));
                }
            }

            return serializedContent;
        }

        /// <summary>
        /// currently this converter supports only datetime fields
        /// </summary>
        public override IEnumerable<Type> SupportedTypes
        {
            get { return new[] { typeof(Profile),typeof(MiscInfo) }; }
        }
    }
}
