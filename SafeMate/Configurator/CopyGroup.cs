using System.Xml;

namespace Safemate.Configurator
{
    public class CopyGroup : XmlSerializable
    {
        internal
        CopyGroup()
        {
            copyOperation = new CopyOperation();
        }

        ~CopyGroup()
        {
        }

        internal override void Deserialize(
            XmlNode sourceNode)
        {
            copyOperation.Deserialize(sourceNode);
        }

        internal override XmlNode
        Serialize(XmlDocument serializedConfiguration)
        {
            XmlNode copyGroup =
                serializedConfiguration.CreateElement("CopyGroup");
            copyGroup.AppendChild(
                copyOperation.Serialize(serializedConfiguration));

            return copyGroup;
        }

        public CopyOperation CopyOperation
        {
            get {
                return copyOperation;
            }
        }

        CopyOperation copyOperation;
    };
};
