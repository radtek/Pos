using System;
using System.ServiceModel;
using MenumateServices.DTO.SmartLink;
using Smartpay.Eftpos;
using MenumateServices.DTO;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IWCFServiceSmartLink
    {
        [OperationContract]
        DTO_SmartLinkTransactionResponse PingTerminal(string ipAddress);
        [OperationContract]
        DTO_SmartLinkTransactionResponse Login(string ipAddress,string versionString);
        [OperationContract]
        DTO_SmartLinkTransactionResponse SettlementInquiry(string ipAddress);
        [OperationContract]
        DTO_SmartLinkTransactionResponse SettlementCutover(string ipAddress);
        [OperationContract]
        DTO_SmartLinkTransactionResponse Purchase(string ipAddress,double amount);
        [OperationContract]
        DTO_SmartLinkTransactionResponse PurchasePlusCash(string ipAddress,double amount, double cash);
        [OperationContract]
        DTO_SmartLinkTransactionResponse CashOnly(string ipAddress,double cash);
        [OperationContract]
        DTO_SmartLinkTransactionResponse Refund(string ipAddress,double refundAmount);
        [OperationContract]
        DTO_SmartLinkTransactionResponse ReprintLastReceipt(string ipAddress);
        [OperationContract]
        DTO_SmartLinkTransactionResponse PrintReceipt(string ipAddress);
    }
}
