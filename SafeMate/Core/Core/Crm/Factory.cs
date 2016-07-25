using SCCE = Safemate.Core.Crm.Exceptions;
using SCCS = Safemate.Core.Crm.Salesforce;

using System;
using System.Collections.Generic;
using System.Xml;

namespace Safemate.Core.Crm
{
    delegate Base CreationMethod(XmlNode crmConfiguration, String account);

    static public class Factory
    {
        static Factory()
        {
            registeredCrms =
                new Dictionary<String, CreationMethod>();
            RegisterProviders();
        }

        static public Base CreateCrmOfType(String type,
                                           XmlNode crmConfiguration, String account)
        {
            try {
                return registeredCrms[type](crmConfiguration, account);
            } catch (KeyNotFoundException crmNotRegistered) {
                throw new SCCE.ImplementationNotPresent(
                    type, crmNotRegistered);
            }
        }

        static void RegisterProviders()
        {
            registeredCrms.Add("Salesforce", SCCS.Salesforce.Create);
        }

        static Dictionary<String, CreationMethod> registeredCrms;
    };
};
