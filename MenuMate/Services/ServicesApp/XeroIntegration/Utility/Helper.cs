using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Windows.Forms;
using System.Xml.Serialization;
using System.IO;
using System.Xml;

namespace XeroIntegration.Utility
{
    public class XeroHelper
    {
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
