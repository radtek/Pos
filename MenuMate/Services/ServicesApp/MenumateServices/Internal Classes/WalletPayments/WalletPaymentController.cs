using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using MenumateServices.DTO.Wallet_Payments;
using WalletPayments;
using WalletPayments.Enums;
using WalletPayments.Model;
using WalletPaymets.Model;

namespace MenumateServices.Internal_Classes.WalletPayments
{
    public class WalletPaymentController
    {
        private static volatile WalletPaymentController _instance;
        private static object syncRoot = new Object();

        private WalletPaymentController()
        {
            ServicePointManager.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => { return true; };
        }

        public static WalletPaymentController Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new WalletPaymentController();
                    }
                }

                return _instance;
            }
        }


        internal WalletActionResponse ProcessPurchaseTransaction(WalletAccount inWalletAccount, WalletTransaction inWalletTransaction)
        {
            try
            {
                WalletPaymentInterface walletPaymentInterface = new WalletPaymentInterface((WalletType)inWalletAccount.WalletType);
                var response = walletPaymentInterface.DoPurchaseTransaction(CreateWalletTransactionInfo(inWalletAccount, inWalletTransaction));
                return CreateResponse(response);
            }
            catch (Exception ex)
            {
                return CreateErrorResponse(ex.Message);
            }
        }

        internal WalletActionResponse ProcessRefundTransaction(WalletAccount inWalletAccount, WalletTransaction inWalletTransaction)
        {
            try
            {
                WalletPaymentInterface walletPaymentInterface = new WalletPaymentInterface((WalletType)inWalletAccount.WalletType);
                var response = walletPaymentInterface.DoRefundTransaction(CreateWalletTransactionInfo(inWalletAccount, inWalletTransaction));
                return CreateResponse(response);
            }
            catch (Exception ex)
            {
                return CreateErrorResponse(ex.Message);
            }
        }

        WalletTransactionInfo CreateWalletTransactionInfo(WalletAccount inWalletAccount, WalletTransaction inWalletTransaction)
        {
            var walletTransactionInfo = new WalletTransactionInfo();
            walletTransactionInfo.ScannedCode = inWalletTransaction.ScannedCode;
            walletTransactionInfo.Amount = inWalletTransaction.Amount;
            walletTransactionInfo.RefundFee = inWalletTransaction.RefundFee;
            walletTransactionInfo.OrderRefernce = inWalletTransaction.ReferenceNumber;
            walletTransactionInfo.AccountInformation = new AccountInformation()
            {
                UserName = inWalletAccount.UserName,
                Password = inWalletAccount.Password,
                SignKey = inWalletAccount.SignKey,
                Token = inWalletAccount.SignKey
            };
            return walletTransactionInfo;

        }

        WalletActionResponse CreateResponse(WalletResponse inWalletResponse)
        {
            return new WalletActionResponse()
            {
                ResponseSuccessful = inWalletResponse.ResponseSuccessful,
                ResponseMessage = inWalletResponse.ResponseMessage,
                OrderId = inWalletResponse.OrderId,
                SignKey = inWalletResponse.SignKey,
                SecurityToken = inWalletResponse.SecurityToken,
                ExpiresIn = inWalletResponse.ExpiresIn,
                RefundFee = inWalletResponse.RefundFee,
                ApplyTime = inWalletResponse.ApplyTime,
                OrderNo = inWalletResponse.OrderNo,
                OutRefundNo = inWalletResponse.OutRefundNo,
                RefundStatus = inWalletResponse.RefundStatus,
                RefundTransactionId = inWalletResponse.RefundTransactionId
            };
        }

        WalletActionResponse CreateErrorResponse(string inMessage)
        {
            return new WalletActionResponse()
            {
                ResponseSuccessful = false,
                ResponseMessage = inMessage
            };
        }



    }
}
