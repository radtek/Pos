using System;
using System.Collections.Generic;
using System.Linq;
using AustriaFiscalIntegration.AustriaFiscalApplication;
using AustriaFiscalIntegration.Domain;
using fiskaltrust.ifPOS.v0;
using Newtonsoft.Json;
using AustriaFiscalIntegration.Utility;

namespace AustriaFiscalIntegration
{
    public class AustriaFiscalController
    {
        public string InitAustriaFiscal(string url, string cashBoxId,List<string> logsList)
        {
            string response = "";
            response = AustriaFiscalAppManager.Instance.Init(url, cashBoxId,logsList);
            return response;
        }

        public ReceiptReponseLocal ProcessSaleReceipt(ReceiptRequestLocal receiptData,List<string> logsList)
        {
            ReceiptReponseLocal responseLocal = new ReceiptReponseLocal();
            try
            {
                ReceiptRequest receiptRequest = ExtractRequest(receiptData,logsList);
                if (receiptRequest != null)
                {
                    
                    string jsonForRequest = "";
                    jsonForRequest = JSonUtility.Serialize<ReceiptRequest>(receiptRequest);
                    logsList.Add("Receipt Data is:-                         " + jsonForRequest);
                    logsList.Add("Sending request at:-                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                    var response = AustriaFiscalAppManager.Instance.proxy.Sign(receiptRequest);
                    logsList.Add("Response received at:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                    responseLocal = ExtractResponse(response,logsList);
                }
            }
            catch (Exception ex)
            {
                logsList.Add("Exception occured at:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Exception is:-                            " + ex.Message);
                responseLocal.UnableToConnectToFiscalService = true; 
            }
            return responseLocal;
        }
        public bool CommissionAustriaFiscal(string url, string cashBoxId, List<string> logsList, string terminalId)
        {
            bool retValue = false;
            try 
            {
                var reqdata = new ReceiptRequest()
                {
                    ftCashBoxID = cashBoxId,
                    cbTerminalID = terminalId,
                    ftReceiptCase = 0x4154000000000003,
                    cbReceiptReference = "Commision Receipt",//n.ToString(),
                    cbReceiptMoment = DateTime.UtcNow,
                    cbChargeItems = new ChargeItem[] { },
                    cbPayItems = new PayItem[] { }
                };
                var response = AustriaFiscalAppManager.Instance.proxy.Sign(reqdata);
                if (response.ftState == 0x4154000000000000 || response.ftState == 0x4154000000000010 || response.ftState == 0x4154000000000020)
                    retValue = true;
            }
            catch (Exception ex)
            {
                logsList.Add("Exception occured at:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Exception is:-                            " + ex.Message);
            }
            return retValue;
        }
        public bool SendZeroReceipt(string url, string cashBoxId, List<string> logsList, string terminalId)
        {
            bool retValue = false;
            try
            {
                var reqdata = new ReceiptRequest()
                {
                    ftCashBoxID = cashBoxId,
                    cbTerminalID = terminalId,
                    ftReceiptCase = 0x4154000000000002,
                    cbReceiptReference = "Zero Receipt",//n.ToString(),
                    cbReceiptMoment = DateTime.UtcNow,
                    cbChargeItems = new ChargeItem[] { },
                    cbPayItems = new PayItem[] { }
                };
                var response = AustriaFiscalAppManager.Instance.proxy.Sign(reqdata);
                if (response.ftState == 0x4154000000000000 || response.ftState == 0x4154000000000010 || response.ftState == 0x4154000000000020)
                    retValue = true;
            }
            catch (Exception ex)
            {
                logsList.Add("Exception occured at:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Exception is:-                            " + ex.Message);
            }
            return retValue;
        }
        public bool SendMonthlyReceipt(string url, string cashBoxId, List<string> logsList, string terminalId)
        {
            bool retValue = false;
            try
            {
                var reqdata = new ReceiptRequest()
                {
                    ftCashBoxID = cashBoxId,
                    cbTerminalID = terminalId,
                    ftReceiptCase = 0x4154000000000005,
                    cbReceiptReference = "Monthly Receipt",//n.ToString(),
                    cbReceiptMoment = DateTime.UtcNow,
                    cbChargeItems = new ChargeItem[] { },
                    cbPayItems = new PayItem[] { }
                };
                var response = AustriaFiscalAppManager.Instance.proxy.Sign(reqdata);
                if (response.ftState == 0x4154000000000000 || response.ftState == 0x4154000000000010 || response.ftState == 0x4154000000000020)
                    retValue = true;
            }
            catch (Exception ex)
            {
                logsList.Add("Exception occured at:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Exception is:-                            " + ex.Message);
            }
            return retValue;
        }
        public bool SendAnnualReceipt(string url, string cashBoxId, List<string> logsList, string terminalId)
        {
            bool retValue = false;
            try
            {
                var reqdata = new ReceiptRequest()
                {
                    ftCashBoxID = cashBoxId,
                    cbTerminalID = terminalId,
                    ftReceiptCase = 0x4154000000000006,
                    cbReceiptReference = "Annual Receipt",//n.ToString(),
                    cbReceiptMoment = DateTime.UtcNow,
                    cbChargeItems = new ChargeItem[] { },
                    cbPayItems = new PayItem[] { }
                };
                var response = AustriaFiscalAppManager.Instance.proxy.Sign(reqdata);
                if (response.ftState == 0x4154000000000000 || response.ftState == 0x4154000000000010 || response.ftState == 0x4154000000000020)
                    retValue = true;
            }
            catch (Exception ex)
            {
                logsList.Add("Exception occured at:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Exception is:-                            " + ex.Message);
            }
            return retValue;
        }
        private ReceiptRequest ExtractRequest(ReceiptRequestLocal receiptData, List<string> logsList)
        {
            ReceiptRequest receiptRequest;
            try
            {
                receiptRequest = new ReceiptRequest
                {
                    ftCashBoxID = receiptData.ftCashBoxID,
                    cbTerminalID = receiptData.TerminalID,
                    ftReceiptCase = receiptData.ftReceiptCase,
                    cbReceiptReference = receiptData.ReceiptReference,
                    cbReceiptMoment = receiptData.ReceiptMoment.ToUniversalTime(),
                    cbChargeItems = new ChargeItem[receiptData.ChargeItems.Count()],
                    cbPayItems = new PayItem[receiptData.PayItems.Count()]
                };
                int index = 0;
                foreach (var chargeItem in receiptData.ChargeItems)
                {
                    ChargeItem item = new ChargeItem
                    {
                        ftChargeItemCase = chargeItem.ChargeItemCase,
                        Description = chargeItem.Description,
                        Quantity = DecimalFromDouble(chargeItem.Quantity),//(decimal)chargeItem.Quantity,
                        VATRate = DecimalFromDouble(chargeItem.VATRate),//(decimal)chargeItem.VATRate,
                        Amount = DecimalFromDouble(chargeItem.Amount),
                    };
                    receiptRequest.cbChargeItems[index] = item;
                    index++;
                }
                index = 0;
                foreach (var payItem in receiptData.PayItems)
                {
                    PayItem item = new PayItem
                    {
                        ftPayItemCase = payItem.PayItemCase,
                        Description = payItem.Description,
                        Quantity = DecimalFromDouble(payItem.Quantity),//(decimal)payItem.Quantity,
                        Amount = DecimalFromDouble(payItem.Amount),//(decimal)payItem.Amount
                    };
                    receiptRequest.cbPayItems[index] = item;
                    index++;
                }
                return receiptRequest;
            }
            catch (Exception ex)
            {
                logsList.Add("Exception occurred at:-                   " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Exception in arranging request data:-     " + ex.Message);
            }
            return null;
        }
        private ReceiptReponseLocal ExtractResponse(ReceiptResponse response, List<string> logsList)
        {
            ReceiptReponseLocal responseLocal = new ReceiptReponseLocal();
            try
            {
                if (response != null)
                {
                    string jsonForResponse = "";
                    jsonForResponse = JSonUtility.Serialize<ReceiptResponse>(response);
                    logsList.Add("Response Data is:-                        " + jsonForResponse);
                    responseLocal.CashBoxID = response.ftCashBoxID;
                    responseLocal.CashBoxIdentification = response.ftCashBoxIdentification;
                    responseLocal.QueueID = response.ftQueueID;
                    responseLocal.QueueItemID = response.ftQueueItemID;
                    responseLocal.QueueRow = response.ftQueueRow;
                    responseLocal.ReceiptIdentification = response.ftReceiptIdentification;
                    responseLocal.ReceiptMoment = response.ftReceiptMoment;
                    responseLocal.State = response.ftState;
                    responseLocal.StateData = response.ftStateData;
                    responseLocal.TerminalID = response.cbTerminalID;
                    responseLocal.ReceiptReference = response.cbReceiptReference;
                    if (response.ftSignatures != null)
                    {
                        responseLocal.Signatures = new SignaturItemLocal[response.ftSignatures.Count()];
                        int index = 0;
                        foreach (var signature in response.ftSignatures)
                        {
                            SignaturItemLocal signatureLocal = new SignaturItemLocal
                            {
                                Caption = signature.Caption,
                                Data = (signature.ftSignatureType== 0x4154000000000001 && signature.ftSignatureFormat==3) ? 
                                        fiskaltrust.ifPOS.Utilities.AT_RKSV_Signature_ToBase32(signature.Data) : "",
                                SignatureFormat = signature.ftSignatureFormat,
                                SignatureType = signature.ftSignatureType
                            };
                            responseLocal.Signatures[index] = signatureLocal;
                            index++;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                logsList.Add("Exception occurred at:-                   " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Exception in extracting response:-        " + ex.Message);
            }
            return responseLocal;
        }
        private decimal DecimalFromDouble(double argument)
        {
            decimal decValue;

            // Convert the double argument to a decimal value.
            try
            {
                decValue = (decimal)argument;
            }
            catch (Exception ex)
            {
                decValue = 0;
            }
            return decValue;           
        }
    }
}
