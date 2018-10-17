using System;
using Newtonsoft.Json;

namespace MewsIntegration.Utilities
{
    public class JsonUtility
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
            var strObj = JsonConvert.SerializeObject(data,Formatting.None,
                         new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore });
            return strObj;
        }
    }
}
