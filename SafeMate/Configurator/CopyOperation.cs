using System;
using System.Xml;

namespace Safemate.Configurator
{
    public class CopyOperation : XmlSerializable
    {
        internal
        CopyOperation()
        {

            emailNotifications = new EmailErrorNotification();
            targets = new TargetCollection();

            emailNotifications.AddRecipient("support@menumate.com");
        }

        ~CopyOperation()
        {
        }

        internal override void
        Deserialize(
            XmlNode sourceNode)
        {
            emailNotifications.Deserialize(
                sourceNode.SelectSingleNode(
                    "CopyGroup/CopyOperation/emailErrorNotification"));
            pattern = sourceNode.SelectSingleNode(
                          "CopyGroup/CopyOperation/pattern").InnerText;
            scheduledFor = new Schedule(
                sourceNode.SelectSingleNode(
                    "CopyGroup/CopyOperation/schedule").InnerText);

            source = new Source();
            source.Deserialize(
                sourceNode.SelectSingleNode("CopyGroup/CopyOperation/source"));
            targets.Deserialize(
                sourceNode.SelectSingleNode(
                    "CopyGroup/CopyOperation/targets"));
        }

        internal override XmlNode
        Serialize(XmlDocument serializedConfiguration)
        {
            XmlNode copyOperation =
                serializedConfiguration.CreateElement("CopyOperation");
            XmlAttribute attribute =
                serializedConfiguration.CreateAttribute("calledFromExternal");
            XmlNode temp =
                serializedConfiguration.CreateElement("pattern");

            attribute.InnerText = "false";
            copyOperation.Attributes.Append(attribute);

            attribute =
                serializedConfiguration.CreateAttribute("deleteSource");
            attribute.InnerText = "true";
            copyOperation.Attributes.Append(attribute);

            attribute = serializedConfiguration.CreateAttribute("name");
            attribute.InnerText = "Menumate";
            copyOperation.Attributes.Append(attribute);

            copyOperation.AppendChild(
                emailNotifications.Serialize(serializedConfiguration));

            temp = serializedConfiguration.CreateElement("pattern");
            temp.InnerText = pattern;
            copyOperation.AppendChild(temp);

            copyOperation.AppendChild(
                scheduledFor.Serialize(serializedConfiguration));
            copyOperation.AppendChild(
                targets.Serialize(serializedConfiguration));
            copyOperation.AppendChild(
                source.Serialize(serializedConfiguration));

            return copyOperation;
        }

        public void
        ScheduleBackupsFor(
            string rawSchedulestring)
        {
            scheduledFor = new Schedule(rawSchedulestring);
        }

        public EmailErrorNotification EmailErrorNotification
        {
            get {
                return emailNotifications;
            }
        }

        public string Pattern
        {
            get {
                return pattern;
            } set {
                if (value == null)
                    pattern = "*.zip";
                pattern = value;
            }
        }

        public Schedule ScheduledFor
        {
            get {
                return scheduledFor;
            }
        }

        public Source Source
        {
            get {
                return source;
            } set {
                if (value == null)
                    throw new Exception
                    ("Attempt to create null source!");
                source = value;
            }
        }

        public TargetCollection Targets
        {
            get {
                return targets;
            }
        }

        EmailErrorNotification emailNotifications;
        string pattern;
        Schedule scheduledFor;
        Source source;
        TargetCollection targets;
    };
};
