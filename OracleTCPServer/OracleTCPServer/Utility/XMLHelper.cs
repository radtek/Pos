using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Serialization;

namespace OracleTCPServer.Utility
{
    public class XMLHelper
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
            string strValue = convertedString.Substring(convertedString.IndexOf("<LinkDesc"),convertedString.Length - convertedString.IndexOf("LinkDesc"));
            //string strValue = convertedString.Substring(convertedString.IndexOf("Lin"))
            return convertedString;
        }
    }
}
