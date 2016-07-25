using System.Collections.Generic;
using System.Xml;

namespace Safemate.Configurator
{
    public class TargetCollection : XmlSerializable
    {
        public void
        AddTarget(
            string name,
            string location,
            Credential authorizationCredential)
        {
            targetCollection.Add(name,
                                 new Target(name, location, authorizationCredential));
        }

        internal override void
        Deserialize(
            XmlNode sourceNode)
        {
            Target target;

            foreach (XmlNode targetNode in
                     sourceNode.SelectNodes("child::target")) {
                target = new Target();
                target.Deserialize(targetNode);
                targetCollection.Add(target.Name, target);
            }
        }

        public void
        RemoveTarget(
            string name)
        {
            targetCollection.Remove(name);
        }

        internal override XmlNode
        Serialize(
            XmlDocument serializedConfiguration)
        {
            XmlNode targets =
                serializedConfiguration.CreateElement("targets");

            foreach (KeyValuePair<string, Target> vp in targetCollection)
                targets.AppendChild(
                    vp.Value.Serialize(serializedConfiguration));

            return targets;
        }

        internal
        TargetCollection()
        {
            targetCollection = new Dictionary<string, Target>();
        }

        ~TargetCollection()
        {
            targetCollection.Clear();
        }

        public void
        UpdateTarget(
            string name,
            string location,
            Credential authorizationCredential)
        {
            RemoveTarget(name);
            AddTarget(name, location, authorizationCredential);
        }

        Dictionary<string, Target> targetCollection;

        public Dictionary<string, Target> Targets
        {
            get {
                return targetCollection;
            }
        }
    };
};
