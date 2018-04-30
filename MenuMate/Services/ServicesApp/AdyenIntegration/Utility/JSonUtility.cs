using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace AdyenIntegration.Utility
{
    public class JSonUtility
    {
        public static T Deserialize<T>(string data)
        {
            try
            {
                var instance = JsonConvert.DeserializeObject<T>(data);
                return instance;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public static string Serialize<T>(T data)
        {
            var strObj = JsonConvert.SerializeObject(data, Newtonsoft.Json.Formatting.None,
            new JsonSerializerSettings
            {
                NullValueHandling = NullValueHandling.Ignore
            });
            return strObj;
        }
    }
}
