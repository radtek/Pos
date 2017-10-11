using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SmartConnectIntegration.Domain;
using System.Diagnostics;

namespace MenumateServices.WCFServices
{

    public class WCFServiceSmartConnect : IWCFSmartConnect
    {
        public WCFServiceSmartConnect()
        {
        }

        private SmartConnectResponse _response;

        public string Pairing(PairingTerminal param)
        {
            return null;
        }

        public SmartConnectResponse Logon(TransactionTypes logonType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Logon SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 1, short.MaxValue);
                ServiceLogger.LogException("Exception in Logon", ex);
            }
            return _response;
        }

        public SmartConnectResponse SettlementInquiry(TransactionTypes settlementEnquiryType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In SettlementInquiry SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 2, short.MaxValue);
                ServiceLogger.LogException("Exception in SettlementInquiry", ex);
            }
            return _response;
        }

        public SmartConnectResponse SettlementCutover(TransactionTypes settlementCutoverType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In SettlementCutover SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 3, short.MaxValue);
                ServiceLogger.LogException("Exception in SettlementCutover", ex);
            }
            return _response;
        }

        public SmartConnectResponse Purchase(TransactionTypes purchaseType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Purchase SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 4, short.MaxValue);
                ServiceLogger.LogException("Exception in Purchase", ex);
            }
            return _response;
        }

        public SmartConnectResponse PurchasePlusCash(TransactionTypes purchasePlusCashType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In PurchasePlusCash SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 5, short.MaxValue);
                ServiceLogger.LogException("Exception in PurchasePlusCash", ex);
            }
            return _response;
        }

        public SmartConnectResponse CashOutOnly(TransactionTypes cashOutOnlyType)
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In CashOutOnly SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 6, short.MaxValue);
                ServiceLogger.LogException("Exception in CashOutOnly", ex);
            }
            return _response;
        }

        public SmartConnectResponse Refund(TransactionTypes refundType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Refund SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 7, short.MaxValue);
                ServiceLogger.LogException("Exception in Refund", ex);
            }
            return _response;
        }

        public SmartConnectResponse Authorise(TransactionTypes authoriseType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Authorise SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 8, short.MaxValue);
                ServiceLogger.LogException("Exception in Authorise", ex);
            }
            return _response;
        }

        public SmartConnectResponse Finalise(TransactionTypes finaliseType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Finalise SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 9, short.MaxValue);
                ServiceLogger.LogException("Exception in Finalise", ex);
            }
            return _response;
        }

        public SmartConnectResponse GetTransactionResult(TransactionTypes transResultType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetTransactionResult SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 10, short.MaxValue);
                ServiceLogger.LogException("Exception in GetTransactionResult", ex);
            }
            return _response;
        }

        public SmartConnectResponse ReprintLastReceipt(TransactionTypes reprintReceiptType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In ReprintLastReceipt SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 11, short.MaxValue);
                ServiceLogger.LogException("Exception in ReprintLastReceipt", ex);
            }
            return _response;
        }

        public SmartConnectResponse GetTerminalStatus(TransactionTypes terminalStatusType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetTerminalStatus SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 12, short.MaxValue);
                ServiceLogger.LogException("Exception in GetTerminalStatus", ex);
            }
            return _response;
        }

        public SmartConnectResponse TerminalReadCard(TransactionTypes readCardType) 
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In TerminalReadCard SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 13, short.MaxValue);
                ServiceLogger.LogException("Exception in TerminalReadCard", ex);
            }
            return _response;
        }

        public SmartConnectResponse PrintReceipt(TransactionTypes printReceiptType)
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In PrintReceipt SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in PrintReceipt", ex);
            }
            return _response;
        }
    }
}
