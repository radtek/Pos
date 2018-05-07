using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AdyenIntegration.Domain;
using AdyenIntegration.Domain.Common;
using AdyenIntegration.Domain.RequestEnvelop;
using AdyenIntegration.Domain.ResponseEnvelop;
using AdyenIntegration.Domain.Constants;
using System.Net;
using AdyenIntegration;

namespace MenumateServices.WCFServices
{
        public class AdyenIntegrationWebService : IAdyenIntegrationWebService
    {
        public AdyenIntegrationWebService()
        {
            ServicePointManager.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => { return true; };
            ServicePointManager.SecurityProtocol = (SecurityProtocolType)3072; 
        }
        
        public SaleToPOIResponse PingTerminal(Envelop envelop,ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                AdyenIntegrationController controller = new AdyenIntegrationController();
                response = controller.PingTerminal(envelop, details);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Ping Terminal   " + ex.Message);
            }
            return response;
        }

        public SaleToPOIResponse LoginToSystem(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                AdyenIntegrationController controller = new AdyenIntegrationController();
                response = controller.PingTerminal(envelop, details);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in LogIn Terminal   " + ex.Message);
            }
            return response;
        }

        public SaleToPOIResponse LogoutSystem(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                AdyenIntegrationController controller = new AdyenIntegrationController();
                response = controller.PingTerminal(envelop, details);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in LogOut Terminal   " + ex.Message);
            }
            return response;
        }

        public SaleToPOIResponse Purchase(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                AdyenIntegrationController controller = new AdyenIntegrationController();
                response = controller.Purchase(envelop, details);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Purchase        " + ex.Message);
            }
            return response; 
        }

        public SaleToPOIResponse Refund(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                AdyenIntegrationController controller = new AdyenIntegrationController();
                response = controller.Refund(envelop, details);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Refund          " + ex.Message);
            }
            return response; 
        }
        public SaleToPOIResponse GetTransactionStatus(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                AdyenIntegrationController controller = new AdyenIntegrationController();
                response = controller.GetTransactionStatus(envelop, details);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in GetTransactionStatus        " + ex.Message);
            }
            return response; 
        }
    }
}
