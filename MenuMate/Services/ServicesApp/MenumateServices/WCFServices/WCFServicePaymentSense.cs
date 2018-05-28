﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Net.Http;
using System.Net.Http.Headers;
using Newtonsoft.Json;
using System.IO;
using System.Diagnostics;
using PaymentSenseIntegration;
using PaymentSenseIntegration.Domain;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WCFServicePaymentSense" in both code and config file together.
    public class WCFServicePaymentSense : IWCFServicePaymentSense
    {
        public WCFServicePaymentSense()
        {
        }

        public PACTerminalWrapper GetAllCardTerminals(AuthorizationDetails autorizationDetails)
        {
            PACTerminalWrapper response = new PACTerminalWrapper();
            try
            {
                PaymentSenseIntegrationController controller = new PaymentSenseIntegrationController();
                response = controller.GetAllTerminals(autorizationDetails);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in GetAllCardTerminals " + ex.Message);
            }
            return response;
        }

        public PACTerminal PingTerminal(AuthorizationDetails autorizationDetails)
        {
            PACTerminal response = new PACTerminal();
            try
            {
                PaymentSenseIntegrationController controller = new PaymentSenseIntegrationController();
                response = controller.PingTerminal(autorizationDetails);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in GetAllCardTerminals " + ex.Message);
            }
            return response;
        }

        public TransactionDataResponse DoTransaction(AuthorizationDetails autorizationDetails, TransactionRequest request)
        {
            TransactionDataResponse response = new TransactionDataResponse();
            try
            {
                PaymentSenseIntegrationController controller = new PaymentSenseIntegrationController();
                response = controller.DoTransaction(autorizationDetails, request);

            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in DoPurchase " + ex.Message);
            }
            return response;
        }

        public bool PrintReports(AuthorizationDetails autorizationDetails, Reports reportData)
        {
            bool reportPrinted = false;
            try
            {
                PaymentSenseIntegrationController controller = new PaymentSenseIntegrationController();
                reportPrinted = controller.PrintReports(autorizationDetails, reportData);

            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in DoPurchase " + ex.Message);
            }
            return reportPrinted;
        }
    }
}
