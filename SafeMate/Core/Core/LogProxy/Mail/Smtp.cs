using SCLP   = Safemate.Core.LogProxy;
using SCLPME = Safemate.Core.LogProxy.Mail.Exceptions;

using System;
using System.Net;
using System.Net.Mail;

namespace Safemate.Core.LogProxy.Mail
{
    public class Smtp : Base
    {
        static public Base Create(MailConfiguration configuration)
        {
            return new Smtp(configuration);
        }

        public override void Send(String message, String subject)
        {
            int i = 0;

            this.message.Body = message;
            this.message.Subject = subject;

            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              String.Format("Attempting to send email \"{0}\"...", subject));

            for ( ; i < 8; i++)
                try {
                    client.Send(this.message);
                    return;
                } catch (SmtpFailedRecipientException
                             cause) {
                    SCLP.LogProxy.Log(SCLP.LogLevel.Warning,
                                      "Receiving server reports " +
                                      cause.FailedRecipient +
                                      " is an invalid address. Ignoring...");
                } catch {
                    SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                      "Unable to send error notification email.");
                }
        }

        ~Smtp()
        {
        }

        public Smtp(MailConfiguration configuration)
        {
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "SMTP Mailer is being initialized.");

            client = new SmtpClient(configuration.SendingServer,
                                    configuration.SendingServerPort);
            client.Credentials = configuration.Credentials;
            client.EnableSsl = configuration.UseSsl;
            client.Timeout = 16000;

            type = "SMTP";

            message = new MailMessage();
            message.From = new MailAddress(configuration.From);

            foreach (String m in configuration.Recipients)
                message.To.Add(m);
        }

        SmtpClient client;
        MailMessage message;
    };
};