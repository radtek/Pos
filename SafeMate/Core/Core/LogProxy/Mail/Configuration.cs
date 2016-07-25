		using System;
using System.Net;

namespace Safemate.Core.LogProxy.Mail
{
    public class MailConfiguration
    {
        public MailConfiguration(String from, String password, String server,
                                 Int32 port, String[] recipients, String username,
                                 bool ssl)
        {
            credentials = new NetworkCredential(username,
                                                password);
            this.from = from;
            this.recipients = recipients;
            sendingServer = server;
            sendingServerPort = port;
            useSsl = ssl;
        }

        public NetworkCredential Credentials
        {
            get {
                return credentials;
            }
        }

        public String From
        {
            get {
                return from;
            }
        }

        public String[] Recipients
        {
            get {
                return recipients;
            }
        }

        public String SendingServer
        {
            get {
                return sendingServer;
            }
        }

        public Int32 SendingServerPort
        {
            get {
                return sendingServerPort;
            }
        }

        public bool UseSsl
        {
            get {
                return useSsl;
            }
        }

        NetworkCredential credentials;
        String from;
        String[] recipients;
        String sendingServer;
        Int32 sendingServerPort;
        bool useSsl;
    };
};
