using System;
using System.IO;
using System.Net;
using System.Text.RegularExpressions;
using System.Xml.Serialization;
using ChefMate.Database;

namespace Chefmate.Infrastructure.Utility
{
    public class ChefmateUtility
    {
        public static bool IsValidIpAddress(string ipAddress)
        {
            IPAddress outIpAddress;
            bool regXmatch = false;
            bool result = IPAddress.TryParse(ipAddress, out outIpAddress);

            Match match = Regex.Match(ipAddress, @"\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}");
            if (match.Success)
            {
                regXmatch = true;
            }
            return result && regXmatch;
        }
   
        public static bool TestConnection(string ipAddress, string dbpath)
        {
            return DatabaseCore.Instance.TestConnection(ipAddress, dbpath);
        }

        public static string Serialize<T>(T inObject)
        {
            var xmlSerializer = new XmlSerializer(typeof(T));
            var stream = new MemoryStream();
            xmlSerializer.Serialize(stream, inObject);
            stream.Position = 0;
            var streamReader = new StreamReader(stream);
            var convertedString = streamReader.ReadToEnd();
            streamReader.Close();
            return convertedString;
        }

        public static T DeSerializeString<T>(string objString)
        {
            var xmlSerializer = new XmlSerializer(typeof(T));
            var textReader = new StringReader(objString);
            var convertedObject = (T)xmlSerializer.Deserialize(textReader);
            return convertedObject;
        }

        public static T DeSerializeFromFilePath<T>(string filePath)
        {
            var xmlSerializer = new XmlSerializer(typeof(T));
            var textReader = new StringReader(File.ReadAllText(filePath));
            var convertedObject = (T)xmlSerializer.Deserialize(textReader);
            return convertedObject;
        }

    }


}