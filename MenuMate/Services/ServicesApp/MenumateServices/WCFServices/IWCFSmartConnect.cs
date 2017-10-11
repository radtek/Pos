using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using SmartConnectIntegration.Domain;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IWCFSmartConnect
    {
        [OperationContract]
        string Pairing(PairingTerminal param);
        [OperationContract]
        SmartConnectResponse Logon(TransactionTypes logonType);
        [OperationContract]
        SmartConnectResponse SettlementInquiry(TransactionTypes settlementEnquiryType);
        [OperationContract]
        SmartConnectResponse SettlementCutover(TransactionTypes settlementCutoverType);
        [OperationContract]
        SmartConnectResponse Purchase(TransactionTypes purchaseType, double amount);
        [OperationContract]
        SmartConnectResponse PurchasePlusCash(TransactionTypes purchasePlusCashType, double totalAmount, double cashAmount);
        [OperationContract]
        SmartConnectResponse CashOutOnly(TransactionTypes cashOutOnlyType, double cashAmount);
        [OperationContract]
        SmartConnectResponse Refund(TransactionTypes refundType, double refundAmount);
        [OperationContract]
        SmartConnectResponse Authorise(TransactionTypes authoriseType, double amountAuth, string transactionRef);
        [OperationContract]
        SmartConnectResponse Finalise(TransactionTypes finaliseType, double amountAuth, string transactionRef);
        [OperationContract]
        SmartConnectResponse GetTransactionResult(TransactionTypes transResultType);
        [OperationContract]
        SmartConnectResponse ReprintLastReceipt(TransactionTypes reprintReceiptType);
        [OperationContract]
        SmartConnectResponse GetTerminalStatus(TransactionTypes terminalStatusType);
        [OperationContract]
        SmartConnectResponse TerminalReadCard(TransactionTypes readCardType);
        [OperationContract]
        SmartConnectResponse PrintReceipt(TransactionTypes printReceiptType);
        
    }
}
