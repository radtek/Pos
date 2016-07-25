using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ServiceModel;
using System.ServiceModel.Description;

namespace MenumateServices.Tools
{
    /// <summary>
    /// 
    /// </summary>
    public class MenumateServiceHostRunner
    {
        ServiceHost serviceHost_;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inServiceHost"></param>
        public MenumateServiceHostRunner(ServiceHost inServiceHost)
        {
            serviceHost_ = inServiceHost;
        }

        #region public
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool Open()
        {
            bool result = false;

            try
            {
                serviceHost_.Open();
                logServiceInformation(serviceHost_);

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.LogException(String.Format("{0} Service Host failed to open.", serviceHost_.Description.Name), e);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(String.Format("{0} Service Host failed to open: {1}", serviceHost_.Description.Name, e.Message), e);
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
        /// <returns></returns>
        public bool Stop()
        {
            bool result = false;

            try
            {
                serviceHost_.Close();

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.LogException(String.Format("{0} Service Host failed to stop.", serviceHost_.Description.Name), e);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(String.Format("{0} Service Host failed to stop: {1}", serviceHost_.Description.Name, e.Message), e);
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
        /// <returns></returns>
        public bool Pause()
        {
            bool result = false;

            try
            {
                serviceHost_.Close();
 
                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.LogException(String.Format("{0} Service Host failed to pause.", serviceHost_.Description.Name), e);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(String.Format("{0} Service Host failed to pause: {1}", serviceHost_.Description.Name, e.Message), e);
            }
            finally
            {
                //inServiceHost.Close();
            }

            return result;
        }
        #endregion

        #region Protected
        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="serviceHost"></param>
        protected void logServiceInformation<T>(T inServiceHost) where T : ServiceHost
        {
            logBaseAddressesInformation(inServiceHost);
            logEndPointsInformation(inServiceHost);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="inServiceHost"></param>
        protected void logBaseAddressesInformation<T>(T inServiceHost) where T : ServiceHost
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
        protected void logEndPointsInformation<T>(T inServiceHost) where T : ServiceHost
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
        #endregion
    }
}
