using System.Collections.Generic;
using System.Xml;

namespace Safemate.Configurator
{
    public class EmailErrorNotification : XmlSerializable
    {
        public void
        AddRecipient(
            string address)
        {
            recipients.Add(address);
        }

        internal
        EmailErrorNotification()
        {
            recipients = new List<string>();
        }

        internal override void
        Deserialize(
            XmlNode sourceNode)
        {
            foreach (XmlNode recipient in
                     sourceNode.SelectNodes("child::recipient"))
                recipients.Add(recipient.InnerText);

            senderConfiguration = new SenderConfiguration();
            senderConfiguration.Deserialize(
                sourceNode.SelectSingleNode("Sender"));
        }

        public void
        RemoveRecipient(
            string address)
        {
            List<string> matches = new List<string>();

            foreach (string m in recipients)
                if (m == address)
                    matches.Add(m);

            foreach (string n in matches)
                recipients.Remove(n);
        }

        internal override XmlNode
        Serialize(
            XmlDocument serializedConfiguration)
        {
            XmlNode notifications =
                serializedConfiguration.CreateElement("emailErrorNotification");
            XmlNode recipient;

            foreach (string m in recipients) {
                recipient =
                    serializedConfiguration.CreateElement("recipient");
                recipient.InnerText = m;
                notifications.AppendChild(recipient);
            }

            notifications.AppendChild(
                senderConfiguration.Serialize(serializedConfiguration));

            return notifications;
        }

        public void
        SetSenderDetails(
            Credential authorizationCredential,
            string replyToAddress,
            string serverAddress,
            string serverPort,
            bool usingSsl)
        {
            senderConfiguration =
                new SenderConfiguration(authorizationCredential,
                                        replyToAddress,
                                        serverAddress,
                                        serverPort,
                                        usingSsl);
        }

        public void
        UpdateRecipient(
            string thisAddress,
            string toThatAddress)
        {
            RemoveRecipient(thisAddress);
            AddRecipient(toThatAddress);
        }

        List<string> recipients;
        SenderConfiguration senderConfiguration;

        public List<string> Recipients
        {
            get {
                return recipients;
            }
        }

        public SenderConfiguration SenderConfiguration
        {
            get {
                return this.senderConfiguration;
            }
        }
    };
};
