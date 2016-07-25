using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;

using System.ServiceModel;
using System.ServiceModel.Description;

namespace MenumateServices
{
    /// <summary>
    /// Implements a MenumateServiceHost which holds a ServiceHost object that implements a WCF Service
    /// createServiceHost() is a virtual method that must be implemented in every directly derived class, it returns null in this class
    /// </summary>
    partial class MenumateServiceHost : MenumateService
    {
        /// <summary>
        /// 
        /// </summary>
        public MenumateServiceHost() : base()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        protected override void OnStart(string[] args)
        {
            base.OnStart(args);

            // TODO: Add code here to start your service.
        }

        /// <summary>
        /// 
        /// </summary>
        protected override void OnStop()
        {
            base.OnStop();

            // TODO: Add code here to perform any tear-down necessary to stop your service.
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected override bool startServiceTask()
        {
            bool result = false;

            //::::::::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                ServiceHost serviceHost = createServiceHost();

                result = OpenServiceHost(serviceHost);
            }
            catch(Exception)
            {
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// Virtual method: This method must be implemented in every directly derived class
        /// </summary>
        /// <returns></returns>
        protected virtual ServiceHost createServiceHost()
        {
            return null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inServiceHost"></param>
        /// <returns></returns>
        protected bool OpenServiceHost(ServiceHost inServiceHost)
        {
            bool result = false;

            try
            {
                inServiceHost.Open();
                LogServiceInformation(inServiceHost);

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.LogException(String.Format("{0} Service Host failed to open.", inServiceHost.Description.Name), e);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(String.Format("{0} Service Host Service failed to open: {1}", inServiceHost.Description.Name, e.Message), e);
            }
            finally
            {
                //inServiceHost.Close();
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="serviceHost"></param>
        protected void LogServiceInformation<T>(T inServiceHost) where T : ServiceHost
        {
            LogBaseAddressesInformation(inServiceHost);
            LogEndPointsInformation(inServiceHost);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="inServiceHost"></param>
        protected void LogBaseAddressesInformation<T>(T inServiceHost) where T : ServiceHost
        {
            if (inServiceHost.BaseAddresses.Count == 0)
            {
                ServiceLogger.Log("No base addresses defined");
            }
            else
            {
                foreach (Uri uri in inServiceHost.BaseAddresses)
                {
                    ServiceLogger.Log(uri.ToString());
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="inServiceHost"></param>
        protected void LogEndPointsInformation<T>(T inServiceHost) where T : ServiceHost
        {
            if (inServiceHost.Description.Endpoints.Count > 0)
            {
                ServiceLogger.Log("Endpoints:");

                foreach (ServiceEndpoint se in inServiceHost.Description.Endpoints)
                {
                    ServiceLogger.Log(String.Format("\t{0} ({1})", se.Name, se.ListenUri));
                }
            }
        }
    }
}
