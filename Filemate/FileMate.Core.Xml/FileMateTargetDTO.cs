using System.Xml;
using System;

namespace FileMate.Core.Xml {
    public class FileMateTargetDTO {

        private string _targetUrl;

        public string TargetUrl {
            get { return _targetUrl; }
            set { _targetUrl = value; }
        }
        private string _username;

        public string Username {
            get { return _username; }
            set { _username = value; }
        }
        private string _password;

        public string Password {
            get { return _password; }
            set { _password = value; }
        }

        private bool _allowOverwrite;

        public bool AllowOverwrite
        {
            get { return _allowOverwrite; }
            set { _allowOverwrite = value; }
        }

        public FileMateTargetDTO() { }

        public FileMateTargetDTO(XmlNode targetNode) {

            _targetUrl = targetNode.Attributes["url"].Value;
            XmlNode credentialsNode = targetNode.SelectSingleNode("credentials");

            if (credentialsNode != null) {
                _username = credentialsNode.Attributes["username"].Value;
                _password = credentialsNode.Attributes["password"].Value;
            }

            _allowOverwrite = targetNode.Attributes["allowOverwrite"] != null && targetNode.Attributes["allowOverwrite"].Value == "true" ? true : false;
        }


        public XmlNode AsXml(XmlDocument doc) {


            XmlNode targetNode = doc.CreateNode(XmlNodeType.Element, "target", "");
            XmlAttribute targetUrlAttr = doc.CreateAttribute("url");
            XmlAttribute targetNameAttr = doc.CreateAttribute("name");
            XmlAttribute allowOverwriteAttr = doc.CreateAttribute("allowOverwrite");

            targetUrlAttr.InnerText = Helper.FormatUrl(TargetUrl);
            targetNameAttr.InnerText = "a target ...";
            allowOverwriteAttr.InnerText = AllowOverwrite ? "true" : "false";
            targetNode.Attributes.Append(targetUrlAttr);
            targetNode.Attributes.Append(targetNameAttr);
            targetNode.Attributes.Append(allowOverwriteAttr);

            if (Username != string.Empty && Username != null &
                Password != string.Empty && Password != null) {

                XmlNode targetCredNode = doc.CreateNode(XmlNodeType.Element, "credentials", "");

                XmlAttribute targetUnameAttr = doc.CreateAttribute("username");
                targetUnameAttr.InnerText = Username;
                targetCredNode.Attributes.Append(targetUnameAttr);

                XmlAttribute targetPwdAttr = doc.CreateAttribute("password");
                targetPwdAttr.InnerText = Password;
                targetCredNode.Attributes.Append(targetPwdAttr);

                targetNode.AppendChild(targetCredNode);
            }

            return targetNode;
        }
    }
}
