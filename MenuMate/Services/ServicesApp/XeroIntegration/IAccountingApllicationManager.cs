using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AccountingIntegration.Model;

namespace AccountingIntegration
{
    interface IAccountingApllicationManager
    {
        AccountingIntegrationError RunAppWithCredentials(Credentials inCredentials);
        AccountingIntegrationError StopAppWithCredentials(Credentials inCredentials);
        AccountingIntegrationError PauseAppWithCredentials(Credentials inCredentials);
        AccountingIntegrationError ResumeAppWithCredentials(Credentials inCredentials);
    }
}
