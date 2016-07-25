using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;
using System.IO;

namespace MenumateServices.InternalClasses
{
    public class Helper
    {
        /// <summary>
        /// 
        /// </summary>
        internal Helper()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly Helper instance = new Helper();
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        public static Helper Instance
        {
            get
            {
                return Nested.instance;
            }
        }

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

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileName"></param>
        /// <returns></returns>
        public string SystemFileURI(string inFileName)
        {
            return StartPath + @"\" + inFileName;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileName"></param>
        /// <returns></returns>
        public bool FileExists(string inFileName)
        {
            return File.Exists(inFileName);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDirName"></param>
        /// <returns></returns>
        public bool DirectoryExists(string inDirName)
        {
            return Directory.Exists(inDirName);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inConfigFilename"></param>
        /// <returns></returns>
        public XmlDocument XMLDocFromFile(string inConfigFilename)
        {
            XmlDocument result = new XmlDocument();

            //:::::::::::::::::::::::::::::

            XmlReader reader = new XmlTextReader(inConfigFilename);
            reader.Read();
            result.Load(reader);

            //::::::::::::::::::::::::::::

            return result;
        }
        #endregion
    }
}
