using SCLPME = Safemate.Core.LogProxy.Mail.Exceptions;

using System;
using System.Collections.Generic;

namespace Safemate.Core.LogProxy.Mail
{
    public delegate Base MailerCreationMethod(
        MailConfiguration configuration);

    public class Factory
    {
        static Factory()
        {
            registeredMailers =
                new Dictionary<String, MailerCreationMethod>();
            RegisterProviders();
        }

        static void RegisterProviders()
        {
            registeredMailers.Add("Smtp", Smtp.Create);
        }

        static public Base CreateMailerOfType(String type,
                                              MailConfiguration configuration)
        {
            try {
                return registeredMailers[type](
                           configuration);
            } catch (KeyNotFoundException mailerNotRegistered) {
                Console.WriteLine("E!");
                throw new SCLPME.ImplementationNotPresent(
                    type, mailerNotRegistered);
            }
        }

        static Dictionary < String,
               MailerCreationMethod > registeredMailers;
    };
};