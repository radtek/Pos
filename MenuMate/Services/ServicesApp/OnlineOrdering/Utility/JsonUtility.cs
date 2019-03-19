using System;
using System.IO;
using Newtonsoft.Json;

namespace OnlineOrdering.Utility
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
            var strObj = JsonConvert.SerializeObject(data);
            return strObj;
        }
    }
}
