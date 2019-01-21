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
        public RegistrationResponse UpdateTerminalRegistrationInfo(string inSyndicateCode, Terminal terminalRegistrationInfo)
        {
            try
            {
                stringList.Add("-------------------------------------------------------Inside UpdateTerminalRegistrationInfo-------------------------------------------------------...");
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

        private void WriteAndClearStringList()
        {
            FileWriter.WriteToFile(stringList,"Registration Integration Logs","RegistrationLogs ");
            stringList.Clear();
        }
    }
}
