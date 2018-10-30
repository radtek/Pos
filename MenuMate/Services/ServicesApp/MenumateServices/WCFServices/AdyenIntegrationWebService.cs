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
using AdyenIntegration.Domain.AdjustAuthorisation;

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
                response = controller.LoginToSystem(envelop, details);
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
                response = controller.LogoutSystem(envelop, details);
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
                envelop.SaleToPOIRequest.TransactionStatusRequest.DocumentQualifier = new string[2];
                envelop.SaleToPOIRequest.TransactionStatusRequest.DocumentQualifier[0] = "CashierReceipt";
                envelop.SaleToPOIRequest.TransactionStatusRequest.DocumentQualifier[1] = "CustomerReceipt";
                response = controller.GetTransactionStatus(envelop, details);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in GetTransactionStatus        " + ex.Message);
            }
            return response; 
        }

        public AdjustAndCaptureResponse AdjustAuthorisation(AdjustAuthorisation authRequest, ResourceDetails details)
        {
            AdjustAndCaptureResponse response = null;
            try
            {
                AdyenIntegrationController controller = new AdyenIntegrationController();
                response = controller.AdjustAuthorisation(authRequest, details);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in AdjustAuthorisation        " + ex.Message);
            }
            return response;
        }

        public AdjustAndCaptureResponse CaptureModifiedAmount(AdjustAuthorisation authRequest, ResourceDetails details)
        {
            AdjustAndCaptureResponse response = null;
            try
            {
                AdyenIntegrationController controller = new AdyenIntegrationController();
                response = controller.AdjustAuthorisation(authRequest, details);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in CaptureModifiedAmount        " + ex.Message);
            }
            return response;
        }
    }
}
