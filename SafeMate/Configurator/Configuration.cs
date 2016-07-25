using System;
using System.Xml;

namespace Safemate.Configurator
{
    public class Configuration
    {
        public
        Configuration(
            string accountName)
        {
            this.accountName = accountName;

            copyGroup = new CopyGroup();
            copyOperation = copyGroup.CopyOperation;
        }

        public
        Configuration(
            string accountName,
            XmlDocument serializedConfigurationFile)
        {
            XmlNode rootData =
                serializedConfigurationFile.SelectSingleNode(
                    "/SafemateConfiguration");
            XmlNode tempData;

            if ((tempData = rootData.SelectSingleNode(
                 "ConfigurationCheckterval")) == null)
                checkInterval = 60;

            crmDetails = new CrmDetails("Safesforce",
                                        new Credential("foo", "bar"));

            copyGroup = new CopyGroup();
            copyGroup.Deserialize(
                serializedConfigurationFile.SelectSingleNode(
                  "/SafemateConfiguration"));
        }

        ~Configuration()
        {
        }

        public XmlDocument
        Serialize()
        {
            XmlDocument configurationFile = new XmlDocument();
            XmlNode safemateNode =
              configurationFile.CreateElement("SafemateConfiguration");
            XmlAttribute tempAttribute;
            XmlNode tempNode =
              configurationFile.CreateElement("ConfigurationCheckInterval");

            tempNode.InnerXml = checkInterval.ToString();
            safemateNode.AppendChild(tempNode);

            safemateNode.AppendChild(crmDetails.Serialize(configurationFile));

            tempNode = configurationFile.CreateElement("Account");
            tempAttribute = configurationFile.CreateAttribute("name");
            tempAttribute.Value = accountName;
            tempNode.Attributes.Append(tempAttribute);
            safemateNode.AppendChild(tempNode);

            safemateNode.AppendChild(copyGroup.Serialize(configurationFile));

            configurationFile.CreateXmlDeclaration("1.0", "utf-8", null);
            configurationFile.AppendChild(safemateNode);

            return configurationFile;
        }

        public CopyGroup CopyGroupInstance
        {
            get {
                return copyGroup;
            }
        }

        string accountName;
        UInt32 checkInterval;
        CopyGroup copyGroup;
        CopyOperation copyOperation;
        CrmDetails crmDetails;
    };
};
