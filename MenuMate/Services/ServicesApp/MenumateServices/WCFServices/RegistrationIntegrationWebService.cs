using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.DTO.MenumateRegistration;
using MenumateServices.Internal_Classes.Registration;
using MenumateServices.Tools;

namespace MenumateServices.WCFServices
{
    public class RegistrationIntegrationWebService : IRegistrationIntegrationWebService
    {
        private List<string> stringList = new List<string>();
        public RegistrationResponse UpdateTerminalRegistrationInfo(string inSyndicateCode, TerminalModel terminalRegistrationInfo)
        {
            try
            {
                stringList.Add("-------------------------------------------------------Inside UpdateTerminalRegistrationInfo-------------------------------------------------------...");
                stringList.Add("syndicate code is: " + inSyndicateCode);
                stringList.Add("site code is: " + terminalRegistrationInfo.SiteCode);
                WriteAndClearStringList();
                return TerminalRegistration.Instance.UpdateTerminalRegistrationInfo(inSyndicateCode, terminalRegistrationInfo);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in UpdateTerminalRegistrationInfo     " + exc.Message);
                WriteAndClearStringList();
            }
            return null;
        }

        public CompanySiteModelResponse ValidateCompanyInfo(string inSyndicateCode, int siteCode)
        {
            try
            {
                stringList.Add("-------------------------------------------------------Inside ValidateCompanyInfo-------------------------------------------------------...");
                stringList.Add("syndicate code is: " + inSyndicateCode);
                stringList.Add("site code is: " + siteCode);
                WriteAndClearStringList();
                return TerminalRegistration.Instance.ValidateCompanyInfo(inSyndicateCode, siteCode);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in ValidateCompanyInfo     " + exc.Message);
                WriteAndClearStringList();
            }
            return null;
        }

        private void WriteAndClearStringList()
        {
            FileWriter.WriteToFile(stringList,"Registration Integration Logs","RegistrationLogs ");
            stringList.Clear();
        }
    }
}
