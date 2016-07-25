using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using PocketVoucherIntegration.PocketVoucherServiceReference;
using PocketVoucherIntegration.RedemptionDetails;
using System.Net;
using System.ServiceModel;
using PocketVoucherIntegration.Tools;




namespace PocketVoucherIntegration
{
    public class PocketVoucherCommunication
    {
        public PocketVoucherCommunication()
        {
            ServicePointManager.ServerCertificateValidationCallback +=
            (sender, cert, chain, sslPolicyErrors) => true;
        }
        public RedemptionResponseDetails RedemptionRequest(VoucherRedemptionDetails redemptionDetails)
        {
            RedemptionResponseDetails responseDetails = new RedemptionResponseDetails();
            string reference = "";
            string message = "";
            // Extract Details of Voucher being redeemed
            string merchantReference = redemptionDetails.MerchantReference; 
            string merchantId = redemptionDetails.MerchantId;
            string terminalId = redemptionDetails.TerminalId;
            decimal transactionAmount = Convert.ToDecimal(redemptionDetails.TransactionAmount);
            decimal voucherAmount = Convert.ToDecimal(redemptionDetails.VoucherAmount);
            string voucherCode = redemptionDetails.VoucherCode;
            RedemptionServiceV1Client redemptionClient = new RedemptionServiceV1Client();
            redemptionClient.ClientCredentials.UserName.UserName = redemptionDetails.UserName;
            redemptionClient.ClientCredentials.UserName.Password = redemptionDetails.Password;
            redemptionClient.Open();
            var result = RedeemResult.Error;
            try
            {
                result = redemptionClient.RedeemWithVoucherAmountTransactionAmount(out reference, out message, voucherCode, voucherAmount, transactionAmount, merchantId, merchantReference, terminalId);
            }
            catch (Exception exception)
            {
                ServiceLogger.Log(string.Format(@"Pocket Voucher Redemption Error:", exception.Message, exception.Message));    
            }

            switch (result)
            {
                case RedeemResult.Success:
                    responseDetails.RedemptionStatus = "Successful";
                    responseDetails.ResponseCode = reference;
                    responseDetails.ResponseMessage = "SUCCESS - Voucher Accepted";
                    break;
                default:
                    if (message.Length != 0)
                    {
                        responseDetails.RedemptionStatus = "Unsuccessful";
                        //if (!message.Contains("call"))
                        //{
                        //    message += " Please call NZ-0800 786 824 or AU-1800 210 044 for help";
                        //}
                        responseDetails.ResponseMessage = message;
                    }
                    else
                    {
                        responseDetails.RedemptionStatus = "Unknown";                        
                        responseDetails.ResponseMessage = "Could Not Communicate with Pocket Voucher.Please Verify Merchant Id and and Password.";
                    }
                    break;
            }
            return responseDetails;
        }
    }
}
