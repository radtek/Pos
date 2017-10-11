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
        SmartConnectResponse Purchase(TransactionTypes purchaseType);
        [OperationContract]
        SmartConnectResponse PurchasePlusCash(TransactionTypes purchasePlusCashType);
        [OperationContract]
        SmartConnectResponse CashOutOnly(TransactionTypes cashOutOnlyType);
        [OperationContract]
        SmartConnectResponse Refund(TransactionTypes refundType);
        [OperationContract]
        SmartConnectResponse Authorise(TransactionTypes authoriseType);
        [OperationContract]
        SmartConnectResponse Finalise(TransactionTypes finaliseType);
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
