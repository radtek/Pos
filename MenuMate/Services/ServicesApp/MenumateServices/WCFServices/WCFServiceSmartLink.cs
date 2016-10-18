using System;
using System.Diagnostics;
using MenumateServices.DTO.SmartLink;
using Smartpay.Eftpos;
using Smartpay.Eftpos.Logging;
using System.ServiceModel;

namespace MenumateServices.WCFServices
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall)]
    public class WCFServiceSmartLink : IWCFServiceSmartLink
    {
        public WCFServiceSmartLink()
        {
            _waitflag = false;
            _response = new DTO_SmartLinkTransactionResponse();
        }

        private DTO_SmartLinkTransactionResponse _response;
        private bool _waitflag;
        private string SampleText = "****** SAMPLE ******\n" +
            "Terminal print\n" +
            "function test\n" +
            "The following two\n" +
            "lines should be\n" +
            "blank.\n" +
            "                    \n" +
            "\n" +
            "********************\n" +
            "************************\n" +
            "\n\n\n\n\n\n\n\n\n";

        public DTO_SmartLinkTransactionResponse PingTerminal(string ipAddress)
        {
            var response = new DTO_SmartLinkTransactionResponse();
            try
            {
                using (var ping = new System.Net.NetworkInformation.Ping())
                {
                    System.Net.NetworkInformation.PingReply pingReply = ping.Send(ipAddress);
                    response.Successful = (pingReply.Status == System.Net.NetworkInformation.IPStatus.Success);
                    if (!response.Successful)
                    {
                        response.ErrorText = "Login Failed - " + Convert.ToString(pingReply.Status);
                    }
                }
            }
            catch (Exception ex)
            {
                response.Successful = false;
                response.ErrorText = ex.Message;
                ServiceLogger.LogException("Exception in PingTerminal", ex);
            }
            return response;
        }

        public DTO_SmartLinkTransactionResponse Login(string ipAddress, string versionString)
        {
            try
            {
                CallPaymentProvider(delegate(ISimplePaymentProvider provider)
                {
                    provider.Logon(versionString);
                }, ipAddress);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in Login", ex);
            }
            return _response;
        }

        public DTO_SmartLinkTransactionResponse SettlementInquiry(string ipAddress)
        {
            try
            {
                CallPaymentProvider(delegate(ISimplePaymentProvider provider)
                {
                    MessageObject request = provider.CreateRequest(Function.SettlementInquiry);
                    provider.ProcessRequestAsync(request);
                }, ipAddress);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in SettlementInquiry", ex);
            }
            return _response;
        }

        public DTO_SmartLinkTransactionResponse SettlementCutover(string ipAddress)
        {
            try
            {
                CallPaymentProvider(delegate(ISimplePaymentProvider provider)
                {
                    MessageObject request = provider.CreateRequest(Function.SettlementCutover);
                    provider.ProcessRequestAsync(request);
                }, ipAddress);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in SettlementCutover", ex);
            }
            return _response;
        }

        public DTO_SmartLinkTransactionResponse Purchase(string ipAddress, double amount)
        {
            try
            {
                CallPaymentProvider(delegate(ISimplePaymentProvider provider)
                {
                    provider.Purchase(GetAmount(amount));
                }, ipAddress);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in Purchase", ex);
            }
            return _response;
        }

        public DTO_SmartLinkTransactionResponse PurchasePlusCash(string ipAddress, double amount, double cash)
        {
            try
            {
                CallPaymentProvider(delegate(ISimplePaymentProvider provider)
                {
                    provider.PurchasePlusCash(GetAmount(amount), GetAmount(cash));
                }, ipAddress);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in PurchasePlusCash", ex);
            }
            return _response;
        }

        public DTO_SmartLinkTransactionResponse CashOnly(string ipAddress, double cash)
        {
            try
            {
                CallPaymentProvider(delegate(ISimplePaymentProvider provider)
                {
                    provider.CashAdvance(GetAmount(cash));
                }, ipAddress);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in CashOnly", ex);
            }
            return _response;
        }

        public DTO_SmartLinkTransactionResponse Refund(string ipAddress, double refundAmount)
        {
            try
            {
                CallPaymentProvider(delegate(ISimplePaymentProvider provider)
                {
                    provider.Refund(GetAmount(refundAmount));
                }, ipAddress);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in Refund", ex);
            }
            return _response;
        }

        public DTO_SmartLinkTransactionResponse ReprintLastReceipt(string ipAddress)
        {
            try
            {
                CallPaymentProvider(delegate(ISimplePaymentProvider provider)
                {
                    provider.ReprintReceipt();
                }, ipAddress);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in ReprintLastReceipt", ex);
            }
            return _response;
        }

        public DTO_SmartLinkTransactionResponse PrintReceipt(string ipAddress)
        {
            try
            {
                CallPaymentProvider(delegate(ISimplePaymentProvider provider)
                {
                    provider.PrintOnTerminal(SampleText);
                }, ipAddress);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in PrintReceipt", ex);
            }
            return _response;
        }


        private void Reset()
        {
            _response = new DTO_SmartLinkTransactionResponse();
            _waitflag = true;
        }

        private void WaitForResponse()
        {
            try
            {
                Reset();
                Stopwatch watch = new Stopwatch();
                watch.Reset();
                watch.Start();
                while (_waitflag)
                {
                    if (watch.Elapsed.TotalMinutes > 2.00)
                    {
                        _waitflag = false;
                        _response.TransactionResult = "Failed - Time-Out";
                        _response.Successful = false;
                        _response.ResultText = "Time-Out";
                        _response.ErrorText = "Time-Out";
                        _response.TimeOut = true;
                    }
                }
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in WaitForResponse", ex);
            }
        }
        private void CallPaymentProvider(Action<ISimplePaymentProvider> paymentFunction, string ipAddress)
        {
            try
            {
                ISimplePaymentProvider provider = new SimplePaymentProvider(ipAddress, 11234, LogEventLevel.Debug);
                provider.OnResponse += Provider_OnResponse;
                paymentFunction(provider);
                WaitForResponse();
            }
            catch (Exception ex)
            {
                _waitflag = false;
                _response.Successful = false;
                _response.ErrorText = ex.Message;
                ServiceLogger.LogException("Exception in CallPaymentProvider", ex);
            }
        }

        private void Provider_OnResponse(object sender, ResponseEventArgs e)
        {
            try
            {
                ParseResponse(e); 
                _waitflag = false;
            }
            catch (Exception ex)
            {
                _response.Successful = false;
                _response.TransactionResult = "Failed";
                _response.ErrorText = ex.Message;
                ServiceLogger.LogException("Exception in Provider_OnResponse", ex);
            }

        }

        private void ParseResponse(ResponseEventArgs responseEventArgs)
        {
            try
            {
                if (responseEventArgs.Response.Args.ContainsKey("TransactionResult"))
                    _response.TransactionResult = responseEventArgs.Response.Args["TransactionResult"];
                if (responseEventArgs.Response.Args.ContainsKey("ResultText"))
                    _response.ResultText = responseEventArgs.Response.Args["ResultText"];
                _response.ErrorText = _response.ResultText;
                if (responseEventArgs.Response.Args.ContainsKey("AcquirerRef"))
                _response.AcquirerRef = responseEventArgs.Response.Args["AcquirerRef"];
                if (_response.TransactionResult == "OK-ACCEPTED")
                    _response.Successful = true;
                if ((_response.TransactionResult != null) && (_response.ResultText == null) && (_response.AcquirerRef == null)
                    && (_response.Successful == false))
                    _response.TimeOut = true;

            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in ParseResponse", ex);
            }
        }

        private uint GetAmount(double amount)
        {
            uint retAmount = 0;
            try
            {
                Decimal decAmount = Convert.ToDecimal(amount);
                retAmount = (uint)(decAmount * 100);
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in ParseResponse", ex);
            }
            return retAmount;
        }

    }



}
