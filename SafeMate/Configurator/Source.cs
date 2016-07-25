using System.Xml;

namespace Safemate.Configurator
{
    public class Source : Repository
    {
        internal override XmlNode
        Serialize(XmlDocument serializedConfiguration)
        {
            return base.Serialize(serializedConfiguration);
        }

        internal
        Source()
        : base("source")
        {
        }

        internal
        Source(
            string name,
            string location,
            Credential authorizationCredential)
        : base("source",
               name,
               location,
               authorizationCredential)
        {
        }
    };
};
