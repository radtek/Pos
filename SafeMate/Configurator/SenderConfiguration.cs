using System;
using System.Collections.Generic;
using System.Xml;

namespace Safemate.Configurator
{
    public class SenderConfiguration : XmlSerializable
    {
        internal override void
        Deserialize(
            XmlNode sourceNode)
        {
            authorizationCredential.Deserialize(sourceNode);
            replyToAddress = sourceNode.Attributes["address"].Value;
            serverAddress = sourceNode.Attributes["server"].Value;
            serverPort = sourceNode.Attributes["port"].Value;
            usingSsl =
                Boolean.Parse(sourceNode.Attributes["useSsl"].Value);
        }

        internal override XmlNode
        Serialize(
            XmlDocument serializedConfiguration)
        {
            XmlNode sender =
                serializedConfiguration.CreateElement("Sender");
            XmlAttribute attribute =
                serializedConfiguration.CreateAttribute("username");

            attribute.InnerText = authorizationCredential.Username;
            sender.Attributes.Append(attribute);

            attribute =
                serializedConfiguration.CreateAttribute("password");
            attribute.InnerText = authorizationCredential.Password;
            sender.Attributes.Append(attribute);

            attribute =
                serializedConfiguration.CreateAttribute("address");
            attribute.InnerText = replyToAddress;
            sender.Attributes.Append(attribute);

            attribute =
                serializedConfiguration.CreateAttribute("server");
            attribute.InnerText = serverAddress;
            sender.Attributes.Append(attribute);

            attribute =
                serializedConfiguration.CreateAttribute("port");
            attribute.InnerText = serverPort;
            sender.Attributes.Append(attribute);

            attribute =
                serializedConfiguration.CreateAttribute("useSsl");
            attribute.InnerText = usingSsl ? "true" : "false";
            sender.Attributes.Append(attribute);

            return sender;
        }

        internal
        SenderConfiguration()
        {
            authorizationCredential = new Credential();
        }

        internal
        SenderConfiguration(
            Credential authorizationCredential,
            string replyToAddress,
            string serverAddress,
            string serverPort,
            bool usingSsl)
        {
            this.authorizationCredential =
                authorizationCredential;
            this.replyToAddress = replyToAddress;
            this.serverAddress = serverAddress;
            this.serverPort = serverPort;
            this.usingSsl = usingSsl;
        }

        public string ReplyToAddress
        {
            get {
                return replyToAddress;
            }
        }

        public string ServerAddress
        {
            get {
                return serverAddress;
            }
        }

        public string ServerPort
        {
            get {
                return serverPort;
            }
        }

        public bool UsingSsl
        {
            get {
                return usingSsl;
            }
        }

        public Credential Credential
        {
            get {
                return authorizationCredential;
            }
        }

        Credential authorizationCredential;
        string replyToAddress;
        string serverAddress;
        string serverPort;
        bool usingSsl;
    };
};
