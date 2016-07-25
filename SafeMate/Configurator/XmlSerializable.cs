using System.Xml;

namespace Safemate.Configurator
{
    public abstract class XmlSerializable
    {
        internal abstract void Deserialize(XmlNode sourceNode);
        internal abstract XmlNode Serialize(
            XmlDocument serializedConfigurationFile);

        protected XmlAttribute attribute;
        protected XmlNode node;
    };
};
