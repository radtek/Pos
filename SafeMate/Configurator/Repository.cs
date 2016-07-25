using System.Text.RegularExpressions;
using System.Xml;

namespace Safemate.Configurator
{
    public class Repository : XmlSerializable
    {
        internal
        Repository(
            string elementName)
        {
            this.elementName = elementName;
        }

        internal
        Repository(
            string elementName,
            string name,
            string location,
            Credential authorizationCredential)
        {
            this.elementName = elementName;
            this.name = name;
            this.location = location;
            this.authorizationCredential = authorizationCredential;

            ValidateLocation();
        }

        internal override void
        Deserialize(
            XmlNode sourceNode)
        {
            if (sourceNode.Name != elementName)
                throw new ParseException(
                    ParseError.MissingRepositoryName, null);

            if (sourceNode.Attributes["name"] != null)
                name = sourceNode.Attributes["name"].Value;
            location = sourceNode.Attributes["url"].Value;
            ValidateLocation();

            if ((node =
                     sourceNode.SelectSingleNode("credentials")) != null) {
                authorizationCredential = new Credential();
                authorizationCredential.Deserialize(node);
            }
        }

        internal override XmlNode
        Serialize(
            XmlDocument serializedConfiguration)
        {
            XmlNode repository =
                serializedConfiguration.CreateElement(elementName);
            XmlAttribute attribute =
                serializedConfiguration.CreateAttribute("name");

            attribute.InnerText = name;
            repository.Attributes.Append(attribute);

            attribute = serializedConfiguration.CreateAttribute("url");
            attribute.InnerText = location;
            repository.Attributes.Append(attribute);

            if (authorizationCredential != null)
                repository.AppendChild(
                    authorizationCredential.Serialize(
                        serializedConfiguration));

            return repository;
        }

        void
        ValidateLocation()
        {
            if (!(new Regex(
                      @"^(ftp://|\\|[a-zA-Z]:\\).*$")).IsMatch(location))
                throw new ParseException(
                    ParseError.InvalidRepositoryPath, null);
        }

        public Credential AuthorizationCredential
        {
            get {
                return authorizationCredential;
            }
        }

        public string Location
        {
            get {
                return location;
            }
        }

        public string Name
        {
            get {
                return name;
            }
        }

        protected Credential authorizationCredential;
        string elementName;
        protected string location;
        protected string name;
    };
};
