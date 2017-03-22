using System;
using System.ServiceModel;
using System.ServiceModel.Description;
using System.Diagnostics;

namespace MenumateServices.MenumateRunners
{
    public abstract class MenumateWebServiceRunner : MenumateRunner
    {
        protected ServiceHost ServiceHost;

        protected bool OpenServiceHost()
        {
            bool result = false;

            try
            {
                ServiceHost = CreateServiceHost();
                ServiceHost.Open();

                LogServiceInformation(ServiceHost);

                result = true;
            }
            catch (SystemException e)
            {
                EventLog.WriteEntry("In OpenServiceHost WebServiceRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 148, short.MaxValue);
                ServiceLogger.LogException(String.Format("{0} Service Host failed to open.", ServiceHost.Description.Name), e);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In OpenServiceHost WebServiceRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 149, short.MaxValue);
                ServiceLogger.LogException(String.Format("{0} Service Host failed to open: {1}", ServiceHost.Description.Name, e.Message), e);
            }
            finally
            {
                //inServiceHost.Close();
            }

            return result;
        }

        public bool CloseServiceHost()
        {
            bool result = false;

            try
            {
                ServiceHost.Close();
                result = true;
            }
            catch (SystemException e)
            {
                EventLog.WriteEntry("In CloseServiceHost WebServiceRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 150, short.MaxValue);
                ServiceLogger.LogException(String.Format("{0} Service Host failed to stop.", ServiceHost.Description.Name), e);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In CloseServiceHost WebServiceRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 151, short.MaxValue);
                ServiceLogger.LogException(String.Format("{0} Service Host failed to stop: {1}", ServiceHost.Description.Name, e.Message), e);
            }
            finally
            {
                //inServiceHost.Close() ;
            }

            return result;
        }

        protected virtual ServiceHost CreateServiceHost()
        {
            ServiceLogger.Log(@"public override void MenumateWCFRunner.CreateServiceHost {");
            return null;
        }

        protected void LogServiceInformation<T>(T serviceHost)
            where T : ServiceHost
        {
            try
            {
                LogBaseAddressesInformation(serviceHost);
                LogEndPointsInformation(serviceHost);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In LogServiceInformation WebServiceRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 152, short.MaxValue);
            }
        }

        protected void LogBaseAddressesInformation<T>(T serviceHost)
            where T : ServiceHost
        {
            if (serviceHost.BaseAddresses.Count == 0)
            {
                ServiceLogger.Log("No base addresses defined");
            }
            else
            {
                foreach (Uri uri in serviceHost.BaseAddresses)
                {
                    ServiceLogger.Log(uri.ToString());
                }
            }
        }

        protected void LogEndPointsInformation<T>(T serviceHost)
            where T : ServiceHost
        {
            if (serviceHost.Description.Endpoints.Count > 0)
            {
                ServiceLogger.Log("Endpoints:");
                foreach (ServiceEndpoint serviceEndpoint in serviceHost.Description.Endpoints)
                {
                    ServiceLogger.Log(String.Format("\t{0} ({1})", serviceEndpoint.Name, serviceEndpoint.ListenUri));
                }
            }
        }
    }
}
