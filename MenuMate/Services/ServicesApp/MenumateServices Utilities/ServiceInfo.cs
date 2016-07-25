using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;

namespace MenumateServices.Utilities
{
    /// <summary>
    /// 
    /// </summary>
    public sealed class ServiceInfo
    {
        //:::::::::::::::::::::::::::::::::

        ServiceInfo() 
        { 
        }

        public static ServiceInfo Instance
        {
            get
            {
               return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly ServiceInfo instance = new ServiceInfo();
        }

        //:::::::::::::::::::::::::::::::::

        /// <summary>
        /// Returns the startup path
        /// </summary>
        public string StartPath
        {
            get
            {
                return System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase).Substring(6);
            }
        }

        //:::::::::::::::::::::::::::::::::

        public string GetFileURI(string inFileName)
        {
            return StartPath + @"\" + inFileName;
        }

        //:::::::::::::::::::::::::::::::::

        public XmlDocument GetXMLDocFromFile(string inConfigFilename)
        {
            XmlDocument result = new XmlDocument();

            //:::::::::::::::::::::::::::::

            XmlReader reader = new XmlTextReader(GetFileURI(inConfigFilename));
            reader.Read();
            result.Load(reader);

            //::::::::::::::::::::::::::::

            return result;
        }
    }
}
