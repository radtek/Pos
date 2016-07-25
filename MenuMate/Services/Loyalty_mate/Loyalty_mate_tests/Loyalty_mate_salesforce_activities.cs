using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Loyalty_mate.Cloud.Salesforce;
using Loyalty_mate.Types;
using Loyalty_mate.SalesforceServiceRef;

namespace Loyalty_mate_tests
{
    [TestClass]
    public class Loyalty_mate_salesforce_activities
    {
        Salesforce_credential_provider credentialProvider;
        SforceService sfClient;
        SalesforceCredentials credentials;
        LoginResult result;

        [TestInitialize]
        public void Setup()
        {
            credentialProvider = Salesforce_credential_provider.Instance;
            sfClient = new SforceService();
        }
        public void CheckSandboxConnection()
        {
            credentials = new SalesforceCredentials();
            credentials.UserName = "andrew@menumate.com";
            credentials.Password = "18mcclurg18";
            credentials.SecurityToken = "dif5imP71pxuepPz4OM7aEMJX";

            result = sfClient.login(credentials.UserName, credentials.Password + credentials.SecurityToken);
            Assert.AreEqual(true, result.sandbox);
        }

        [TestCleanup]
        public void Teardown()
        {
            credentialProvider = null;
            sfClient = null;
            credentials = null;
            result = null;
        }
    }
}
