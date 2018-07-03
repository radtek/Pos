using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.Constants
{
    [DataContract]
    public class GenericProfileClass
    {
        [DataMember]
        public const string BasicVar = "Basic";
        [DataMember]
        public const string StandardVar = "Extended";
        [DataMember]
        public const string ExtendedVar = "Extended";
    }
    [DataContract]
    public class MessageTypeClass
    {
        [DataMember]
        public const string Request = "Request";
        [DataMember]
        public const string Response = "Response";
        [DataMember]
        public const string Notification = "Notification";
    }
    [DataContract]
    public class MessageCategoryClass
    {
        [DataMember]
        public const string Abort = "Abort";
        [DataMember]
        public const string Admin = "Admin";
        [DataMember]
        public const string BalanceInquiry = "BalanceInquiry";
        [DataMember]
        public const string Batch = "Batch";
        [DataMember]
        public const string CardAcquisition = "CardAcquisition";
        [DataMember]
        public const string CardReaderAPDU = "CardReaderAPDU";
        [DataMember]
        public const string CardReaderInit = "CardReaderInit";
        [DataMember]
        public const string CardReaderPowerOff = "CardReaderPowerOff";
        [DataMember]
        public const string Diagnosis = "Diagnosis";
        [DataMember]
        public const string Display = "Display";
        [DataMember]
        public const string EnableService = "EnableService";
        [DataMember]
        public const string Event = "Event";
        [DataMember]
        public const string GetTotals = "GetTotals";
        [DataMember]
        public const string Input = "Input";
        [DataMember]
        public const string InputUpdate = "InputUpdate";
        [DataMember]
        public const string Login = "Login";
        [DataMember]
        public const string Logout = "Logout";
        [DataMember]
        public const string Loyalty = "Loyalty";
        [DataMember]
        public const string Payment = "Payment";
        [DataMember]
        public const string PIN = "PIN";
        [DataMember]
        public const string Print = "Print";
        [DataMember]
        public const string Reconciliation = "Reconciliation";
        [DataMember]
        public const string Reversal = "Reversal";
        [DataMember]
        public const string Sound = "Sound";
        [DataMember]
        public const string StoredValue = "StoredValue";
        [DataMember]
        public const string TransactionStatus = "TransactionStatus";
        [DataMember]
        public const string Transmit = "Transmit";
    }
    [DataContract]
    public class MessageClassClass
    {
        [DataMember]
        public const string Service = "Service";
        [DataMember]
        public const string Device = "Device";
        [DataMember]
        public const string Event = "Event";
    }
    [DataContract]
    public class Resources
    {
        [DataMember]
        public string ResourceURL { get; set; }
        [DataMember]
        public string APIKEY { get; set; }
    }
    [DataContract]
    public class PaymentType
    {
        [DataMember]
        public const string Normal = "Normal";
        [DataMember]
        public const string Refund = "Refund";
        [DataMember]
        public const string OneTimeReservation = "OneTimeReservation";
        [DataMember]
        public const string FirstReservation = "FirstReservation";
        [DataMember]
        public const string UpdateReservation = "UpdateReservation";
        [DataMember]
        public const string Completion = "Completion";
        [DataMember]
        public const string CashAdvance = "CashAdvance";
        [DataMember]
        public const string CashDeposit = "CashDeposit";
        [DataMember]
        public const string Recurring = "Recurring";
        [DataMember]
        public const string Instalment = "Instalment";
        [DataMember]
        public const string IssuerInstalment = "IssuerInstalment";
        [DataMember]
        public const string PaidOut = "PaidOut";
    }
    [DataContract]
    public class ReversalReason
    {
        public const string CustCancel = "CustCancel";
        [DataMember]
        public const string MerchantCancel = "MerchantCancel";
        [DataMember]
        public const string Malfunction = "Malfunction";
        [DataMember]
        public const string Unable2Compl = "Unable2Compl";
    }
}
