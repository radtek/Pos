using System.Xml;

namespace Safemate.Configurator
{
    public class Credential : XmlSerializable
    {
        internal
        Credential()
        {
        }

        internal Credential(
            string username,
            string password)
        {
            this.username = username;
            this.password = password;
        }

        internal override void
        Deserialize(
            XmlNode sourceNode)
        {
            username = sourceNode.Attributes["username"].Value;
            password = sourceNode.Attributes["password"].Value;
        }

        internal override XmlNode
        Serialize(XmlDocument serializedConfiguration)
        {
            XmlNode credential =
                serializedConfiguration.CreateElement("credentials");
            XmlAttribute attribute =
                serializedConfiguration.CreateAttribute("username");

            attribute.InnerText = username;
            credential.Attributes.Append(attribute);

            attribute =
                serializedConfiguration.CreateAttribute("password");
            attribute.InnerText = password;
            credential.Attributes.Append(attribute);

            return credential;
        }

        public string Username
        {
            get {
                return username;
            }
        }

        public string Password
        {
            get {
                return password;
            }
        }

        string username = null;
        string password = null;
    };
};
