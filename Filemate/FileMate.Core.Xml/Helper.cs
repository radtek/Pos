using System;
using System.Globalization;
using System.IO;
using System.Text.RegularExpressions;
using System.Xml;
using FCXE = FileMate.Core.Xml.Exceptions;

using System.Text;

namespace FileMate.Core.Xml
{
    public enum RepositoryType
    {
        Disk, Ftp, None
    }

    /// <summary>
    /// File type for a Repository Operation
    /// </summary>
    public enum OperationFileType
    {
        Text, ZIP, Binary
    }

    /// <summary>
    /// Contains static members to help with xml parsing.
    /// </summary>
    public static class Helper
    {
        /// <summary>
        /// Checks to see if there is a trailing slash on the url, if not
        /// will place one and return it as a new string.
        /// </summary>
        /// <param name="urlToFormat"></param>
        /// <returns></returns>
        public static string FormatUrl(string urlToFormat)
        {

            string endChar = urlToFormat.Substring(urlToFormat.Length - 1, 1);
            string returnString = null;

            if (GetRepositoryType(urlToFormat) == RepositoryType.Disk)
            {
                if (!endChar.Equals(new String(Path.DirectorySeparatorChar, 1)))
                {
                    returnString = urlToFormat + Path.DirectorySeparatorChar;
                }
                else
                {
                    returnString = urlToFormat;
                }
            }
            else if (GetRepositoryType(urlToFormat) == RepositoryType.Ftp)
            {
                if (!endChar.Equals("/"))
                {
                    returnString = urlToFormat + "/";
                }
                else
                {
                    returnString = urlToFormat;
                }
            }
            else
            {
                returnString = urlToFormat;
            }

            return returnString;
        }

        public static RepositoryType GetRepositoryType(string p)
        {

            if (_regexFtp.IsMatch(p))
                return RepositoryType.Ftp;
            else if (_regexDisk.IsMatch(p))
                return RepositoryType.Disk;

            throw new FCXE.UnsupportedRepositoryType(p);
        }

        /// <summary>
        /// Extract the list of CopyOperationNodes and return them as a node list.
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static XmlNodeList GetCopyOperationNodes(XmlDocument doc)
        {
            return doc.DocumentElement.SelectNodes("//CopyOperation");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="opNode"></param>
        /// <returns></returns>
        public static XmlNode GetSourceNodeForOperation(XmlNode opNode)
        {
            return opNode.SelectSingleNode("source");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="opNode"></param>
        /// <returns></returns>
        public static XmlNode GetSourceCredentialsNode(XmlNode opNode)
        {
            return opNode.SelectSingleNode("source/credentials");
        }

        public static String GetTargetName(XmlNode targetNode)
        {
            return targetNode.Attributes["name"].Value;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="opNode"></param>
        /// <returns></returns>
        public static string GetFilePatternForOperation(XmlNode opNode)
        {
            return opNode.SelectSingleNode("pattern").InnerText;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="opNode"></param>
        /// <returns></returns>
        public static OperationFileType GetFileTypeForOperation(XmlNode opNode)
        {
            try
            {
                string opFileType = opNode.SelectSingleNode("FileType").InnerText;

                switch (opFileType)
                {
                    case "Text": return OperationFileType.Text;
                    case "ZIP": return OperationFileType.ZIP;
                    case "Default": return GetDefaultFileTypeForOperation(opNode);
                    default: return OperationFileType.Binary;
                }
            }
            catch
            {
                return GetDefaultFileTypeForOperation(opNode);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="opNode"></param>
        /// <returns></returns>
        public static OperationFileType GetDefaultFileTypeForOperation(XmlNode opNode)
        {
            string pattern = GetFilePatternForOperation(opNode);

            if (Regex.IsMatch(pattern, "txt", RegexOptions.IgnoreCase))
            {
                return OperationFileType.Text;
            }
            else if (Regex.IsMatch(pattern, "zip", RegexOptions.IgnoreCase))
            {
                return OperationFileType.ZIP;
            }
            else
            {
                return OperationFileType.Binary;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="opNode"></param>
        /// <returns></returns>
        public static XmlNodeList GetTargetsForOperation(XmlNode opNode)
        {
            return opNode.SelectSingleNode("targets").SelectNodes("target");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="opNode"></param>
        /// <returns></returns>
        public static string GetCopyOperationName(XmlNode opNode)
        {
            string n = opNode.Attributes["name"].Value;
            return n;
        }

        /// <summary>
        /// Gets whether a copy operation is enabled or not
        /// Defaults to true
        /// </summary>
        /// <param name="opNode"></param>
        /// <returns></returns>
        public static bool GetCopyOperationEnabled(XmlNode opNode)
        {
            bool status = true;
            if (opNode.Attributes["enabled"] != null)
            {
                status = opNode.Attributes["enabled"].Value == "true" ? true : false;
            }
            return status;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="opNode"></param>
        /// <returns></returns>
        public static bool GetCopyOperationDeleteSource(XmlNode opNode)
        {
            string d = opNode.Attributes["deleteSource"].Value;
            return d.Equals("true");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="opNode"></param>
        /// <param name="username"></param>
        /// <param name="password"></param>
        public static void GetSourceUsernameAndPassword(XmlNode opNode,
                out string username,
                out string password)
        {

            username = null;
            password = null;

            XmlNode credentialsNode = Helper.GetSourceCredentialsNode(opNode);
            if (credentialsNode != null)
            {

                username = credentialsNode.Attributes["username"].Value;
                password = credentialsNode.Attributes["password"].Value;
            }
        }

        public static void GetTargetUsernameAndPassword(XmlNode credentialsNode,
                out string username,
                out string password)
        {

            username = null;
            password = null;
            if (credentialsNode != null)
            {

                username = credentialsNode.Attributes["username"].Value;
                password = credentialsNode.Attributes["password"].Value;
            }
        }

        /// <summary>
        /// Extract URL from a source node.
        /// </summary>
        /// <param name="sourceNode"></param>
        /// <returns></returns>
        public static string GetSourceUrl(XmlNode sourceNode)
        {
            return sourceNode.Attributes["url"].Value;
        }

        public static string GetScheduleForOperation(XmlNode opNode)
        {
            XmlNode scheduleNode = opNode.SelectSingleNode("schedule");
            return scheduleNode.InnerText;

        }

        /// <summary>
        /// Replace tokens with real value. Note that only {date} is supported, and returns a
        /// date/time formated string as "yyyyMMddHHmmss".
        /// </summary>
        /// <param name="rawUrl"></param>
        /// <returns></returns>
        public static string ExpandTargetUrl(string rawUrl, DateTime dt)
        {

            string expandedUrl = rawUrl.Replace("{date}", dt.ToString("yyyyMMddHHmmss", new CultureInfo("en-NZ")));
            return expandedUrl;
        }

        public static Regex _regexFtp = new Regex(
             @"^ftp://.*$",
             RegexOptions.IgnoreCase
             | RegexOptions.Singleline
             | RegexOptions.IgnorePatternWhitespace
             | RegexOptions.Compiled
             );

        public static Regex _regexDisk = new Regex(
             @"^\.?([a-z]:)?\\.*$",
             RegexOptions.IgnoreCase
            | RegexOptions.Singleline
            | RegexOptions.IgnorePatternWhitespace
            | RegexOptions.Compiled
            );

        public static XmlNode GetTargetCredentialsNode(XmlNode targetNode)
        {
            return targetNode.SelectSingleNode("credentials");
        }

        public static string GetTargetUrl(XmlNode targetNode)
        {
            return targetNode.Attributes["url"].Value;
        }

        public static bool GetAllowOverwriteValue(XmlNode targetNode)
        {
            return targetNode.Attributes["allowOverwrite"] != null && targetNode.Attributes["allowOverwrite"].Value == "true" ? true : false;
        }

        public static bool GetDeleteSourceValueForOperation(XmlNode opNode)
        {
            return opNode.Attributes["deleteSource"].Value.Equals("true");
        }

        public static bool GetCalledFromForOperation(XmlNode opNode)
        {
            return opNode.Attributes["calledFromExternal"].Value.Equals("true");
        }

        static public DateTime GetCurrentWriteTime(
            FileInfo fileInfo)
        {
            fileInfo.Refresh();
            return fileInfo.LastWriteTime;
        }

        static public void ToggleFileReadOnly(FileInfo info)
        {
            info.IsReadOnly = !info.IsReadOnly;
        }
    }
}
