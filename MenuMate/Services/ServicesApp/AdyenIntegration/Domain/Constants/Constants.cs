﻿using System;
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
        [DataMember]
        public const string CustCancel = "CustCancel";
        [DataMember]
        public const string MerchantCancel = "MerchantCancel";
        [DataMember]
        public const string Malfunction = "Malfunction";
        [DataMember]
        public const string Unable2Compl = "Unable2Compl";
    }
    [DataContract]
    public class TerminalEnvironment
    {
        [DataMember]
        public const string Attended = "Attended";
        [DataMember]
        public const string SemiAttended = "SemiAttended";
        [DataMember]
        public const string Unattended = "Unattended";
    }
    [DataContract]
    public class TokenRequestedType
    {
        [DataMember]
        public const string Transaction = "Transaction";
        [DataMember]
        public const string Customer = "Customer";
    }
    [DataContract]
    public class Result
    {
        [DataMember]
        public string Success = "Success";
        [DataMember]
        public string Partial = "Partial";
        [DataMember]
        public string Failure = "Failure";
    }
    [DataContract]
    public class ErrorCondition
    {
        [DataMember]
        public const string Aborted = "Aborted";
        [DataMember]
        public const string Busy = "Busy";
        [DataMember]
        public const string Cancel = "Cancel";
        [DataMember]
        public const string DeviceOut = "DeviceOut";
        [DataMember]
        public const string InsertedCard = "InsertedCard";
        [DataMember]
        public const string InProgress = "InProgress";
        [DataMember]
        public const string LoggedOut = "LoggedOut";
        [DataMember]
        public const string MessageFormat = "MessageFormat";
        [DataMember]
        public const string NotAllowed = "NotAllowed";
        [DataMember]
        public const string NotFound = "NotFound";
        [DataMember]
        public const string PaymentRestriction = "PaymentRestriction";
        [DataMember]
        public const string Refusal = "Refusal";
        [DataMember]
        public const string UnavailableDevice = "UnavailableDevice";
        [DataMember]
        public const string UnavailableService = "UnavailableService";
        [DataMember]
        public const string InvalidCard = "InvalidCard";
        [DataMember]
        public const string UnreachableHost = "UnreachableHost";
        [DataMember]
        public const string WrongPIN = "WrongPIN";
    }
    [DataContract]
    public class DocumentQualifier
    {
        [DataMember]
        public const string SaleReceipt = "SaleReceipt";
        [DataMember]
        public const string CashierReceipt = "CashierReceipt";
        [DataMember]
        public const string CustomerReceipt = "CustomerReceipt";
        [DataMember]
        public const string Document = "Document";
        [DataMember]
        public const string Voucher = "Voucher";
        [DataMember]
        public const string Journal = "Journal";
    }
    [DataContract]
    public class OutputFormat
    {
        [DataMember]
        public const string MessageRef = "MessageRef";
        [DataMember]
        public const string Text = "Text";
        [DataMember]
        public const string XHTML = "XHTML";
        [DataMember]
        public const string BarCode = "BarCode";
    }
    [DataContract]
    public class PaymentInstrumentType
    {
        [DataMember]
        public const string Card = "Card";
        [DataMember]
        public const string Check = "Check";
        [DataMember]
        public const string Mobile = "Mobile";
        [DataMember]
        public const string StoredValue = "StoredValue";
        [DataMember]
        public const string Cash = "Cash";
    }
    [DataContract]
    public class InstalmentType
    {
        [DataMember]
        public const string DeferredInstalment = "DeferredInstalment";
        [DataMember]
        public const string EqualInstalments = "EqualInstalments";
        [DataMember]
        public const string UnequalInstalments = "UnequalInstalments";

    }
    [DataContract]
    public class PeriodUnit
    {
        [DataMember]
        public const string Daily = "Daily";
        [DataMember]
        public const string Weekly = "Weekly";
        [DataMember]
        public const string Monthly = "Monthly";
        [DataMember]
        public const string Annual = "Annual";
    }
    [DataContract]
    public class TypeCode
    {
        [DataMember]
        public const string Personal = "Personal";
        [DataMember]
        public const string Company = "Company";
    }
    [DataContract]
    public class StoredValueAccountType
    {
        [DataMember]
        public const string GiftCard = "GiftCard";
        [DataMember]
        public const string PhoneCard = "PhoneCard";
        [DataMember]
        public const string Other = "Other";
    }
    [DataContract]
    public class IdentificationType
    {
        [DataMember]
        public const string PAN = "PAN";
        [DataMember]
        public const string ISOTrack2 = "ISOTrack2";
        [DataMember]
        public const string BarCode = "BarCode";
        [DataMember]
        public const string AccountNumber = "AccountNumber";
        [DataMember]
        public const string PhoneNumber = "PhoneNumber";
    }
}
