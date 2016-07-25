using System.Xml;

namespace Safemate.Configurator
{
    public class Target : Repository
    {
        internal override XmlNode
        Serialize(XmlDocument serializedConfiguration)
        {
            return base.Serialize(serializedConfiguration);
        }

        internal
        Target()
        : base("target")
        {
        }

        internal
        Target(
            string name,
            string location,
            Credential authorizationCredential)
        : base("target",
               name,
               location,
               authorizationCredential)
        {
        }
    };
};
