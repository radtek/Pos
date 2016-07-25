using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;

namespace XMLManager
{
    public class XMLDocManager
    {
        public static void CreateXmlDeclaration(XmlDocument inDoc, string version, string encoding, string standalone)
        {
            XmlDeclaration dec = inDoc.CreateXmlDeclaration("1.0", null, null);
            inDoc.AppendChild(dec);
        }

        public static XmlElement CreateRoot(XmlDocument inDoc, string inRootName)
        {
            XmlElement root = inDoc.CreateElement(inRootName);
            inDoc.AppendChild(root);

            return root;
        }

        public static XmlNode AddNode(XmlDocument inDoc, XmlNode inParentNode, string inNodeName)
        {
            XmlNode node = inDoc.CreateElement(inNodeName);
            inParentNode.AppendChild(node);

            return node;
        }

        public static XmlNode AddNodeChild(XmlDocument inDoc, XmlNode inParent, string inNodeName)
        {
            XmlNode node = inDoc.CreateElement(inNodeName);
            inParent.AppendChild(node);

            return node;
        }

        public static void AddAttribute(XmlDocument inDoc, XmlNode inNode, string inAttribute, string inValue)
        {
            XmlAttribute attr = inDoc.CreateAttribute(inAttribute);
            attr.Value = inValue;
            inNode.Attributes.Append(attr);
        }

        public static void AddAttribute(XmlDocument inDoc, string inNodePath, string inAttribute, string inValue)
        {
            XmlAttribute attr = inDoc.CreateAttribute(inAttribute);
            attr.Value = inValue;
            GetNode(inDoc, string.Format(@"./{0}", inNodePath)).Attributes.Append(attr);
        }

        public static void SetAttribute(XmlDocument inDoc, string inNodePath, string inAttrName, string inAttrValue)
        {
            try
            {
                GetNode(inDoc, string.Format(@"./{0}", inNodePath)).Attributes[inAttrName].Value = inAttrValue;
            }
            catch(Exception)
            {
            }
        }

        public static void SetAttribute(XmlNode inNode, string inAttrName, string inAttrValue)
        {
            try
            {
                inNode.Attributes[inAttrName].Value = inAttrValue;
            }
            catch (Exception)
            {
            }
        }

        public static XmlNode GetRoot(XmlDocument inDoc)
        {
            return inDoc.DocumentElement.SelectSingleNode(".");
        }

        public static XmlNode GetNode(XmlDocument inDoc, string inNodePath)
        {
            return inDoc.DocumentElement.SelectSingleNode(string.Format(@"./{0}", inNodePath));
        }

        public static XmlNodeList GetNodeList(XmlDocument inDoc, string inNodePath)
        {
            return inDoc.DocumentElement.SelectNodes(string.Format(@"./{0}", inNodePath));
        }

        public static XmlNodeList GetNodeList(XmlNode inParent, string inNodePath)
        {
            return inParent.SelectNodes(string.Format(@"./{0}", inNodePath));
        }
    }
}
