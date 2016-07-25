using System.Xml;

namespace Safemate.Configurator
{
    public class CrmDetails : XmlSerializable
    {
        internal
        CrmDetails()
        {
        }

        internal
        CrmDetails(
            string crmType,
            Credential authorizationCredential)
        {
            this.authorizationCredential = authorizationCredential;
            type = crmType;
        }

        internal override void
        Deserialize(
            XmlNode sourceNode)
        {
            type = sourceNode.Attributes["type"].Value;
            authorizationCredential.Deserialize(sourceNode);
        }

        internal override XmlNode
        Serialize(
            XmlDocument serializedConfiguration)
        {
            XmlNode crmEngine =
                serializedConfiguration.CreateElement("CrmEngine");
            XmlAttribute attribute =
                serializedConfiguration.CreateAttribute("type");

            attribute.InnerText = type;
            crmEngine.Attributes.Append(attribute);

            attribute =
                serializedConfiguration.CreateAttribute("password");
            attribute.InnerText = authorizationCredential.Password;
            crmEngine.Attributes.Append(attribute);

            attribute =
                serializedConfiguration.CreateAttribute("username");
            attribute.InnerText = authorizationCredential.Username;
            crmEngine.Attributes.Append(attribute);

            return crmEngine;
        }

        public Credential AuthorizationCredential
        {
            get {
                return authorizationCredential;
            }
        }

        public string Type
        {
            get {
                return type;
            }
        }

        Credential authorizationCredential;
        string type;
    };
};
