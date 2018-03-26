using System;
using MenumateServices.DTO.SalesForce;
using SalesForceCommunication;
using SalesForceCommunication.Domain;
using System.Net;
using System.Security.Cryptography.X509Certificates;
using PocketVoucherIntegration;
using System.Diagnostics;
using PocketVoucherIntegration.RedemptionDetails;

namespace MenumateServices.WCFServices
{
    public class PocketVoucherWebService : IPocketVoucherWebService
    {
        private readonly SalesForceInterface _salesForceInterface;
        private readonly SalesForceCredentialFactory _salesForceCredentialFactory;

        public PocketVoucherWebService()
        {
            _salesForceInterface = new SalesForceInterface();
            _salesForceCredentialFactory = new SalesForceCredentialFactory();
        }
        
        public bool EnablePocketVouchers(string accountName)
        {
            try
            {
                //We need to login to sales force to initiate the process..
                //Get the credentials for logging into sales force..
                var credentials = _salesForceCredentialFactory.Create();

                if (_salesForceInterface.Login(credentials))
                {
                    //Update the pocket voucher fields to sales force..
                    var isEnabled = _salesForceInterface.EnablePocketVouchers(accountName);
                    var isDateChanged = _salesForceInterface.UpdatePocketVouchersDateUpdated(accountName);

                    //Since we have completed all the processing we will log out..
                    _salesForceInterface.Logout();

                    return isEnabled && isDateChanged;
                }
            }
            catch (Exception exception)
            {
                //EventLog.WriteEntry("In EnablePocketVouchers Pocket Voucher", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 11, short.MaxValue);
                ServiceLogger.LogException("Exception in EnablePocketVouchers", exception);
            }

            return false;
        }

        public bool DisablePocketVouchers(string accountName)
        {
            try
            {
                //We need to login to sales force to initiate the process..
                //Get the credentials for logging into sales force..
                var credentials = _salesForceCredentialFactory.Create();

                if (_salesForceInterface.Login(credentials))
                {
                    //Update the pocket voucher fields to sales force..
                    var isEnabled = _salesForceInterface.DisablePocketVouchers(accountName);
                    var isDateChanged = _salesForceInterface.UpdatePocketVouchersDateUpdated(accountName);

                    //Since we have completed all the processing we will log out..
                    _salesForceInterface.Logout();

                    return isEnabled && isDateChanged;
                }
            }
            catch (Exception exception)
            {
                //EventLog.WriteEntry("In DisablePocketVouchers Pocket Voucher", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 12, short.MaxValue);
                ServiceLogger.LogException("Exception in DisablePocketVouchers", exception);
            }

            return false;
        }
        public RedemptionResponseDetails RedeemVoucher(VoucherRedemptionDetails redemptionDetails)
        {
            try
            {
                PocketVoucherCommunication pvCommunication = new PocketVoucherCommunication();
                RedemptionResponseDetails responseDetails = new RedemptionResponseDetails();
                responseDetails = pvCommunication.RedemptionRequest(redemptionDetails);
                return responseDetails;
            }
            catch (Exception exception)
            {
                //EventLog.WriteEntry("In ForceCloseAllTabs Pocket Voucher", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 9, short.MaxValue);
                ServiceLogger.LogException("Exception in RedeemVoucher", exception);
            }
            return null;
        }
        public PocketVoucherDetail GetPocketVoucherDetail(string accountName)
        {
            try
            {
                //We need to login to sales force to initiate the process..
                //Get the credentials for logging into sales force..

                var credentials = _salesForceCredentialFactory.Create();

                if (_salesForceInterface.Login(credentials))
                {
                    //Get the pocket voucher fields from sales force to return to POS..
                    var pocketVoucherDetail = _salesForceInterface.GetPocketVoucherDetail(accountName);

                    //Since we have completed all the processing we will log out..
                    _salesForceInterface.Logout();

                    return pocketVoucherDetail;
                }
            }
            catch (Exception exception)
            {
                //EventLog.WriteEntry("In GetPocketVoucherDetail Pocket Voucher", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 10, short.MaxValue);
                ServiceLogger.LogException("Exception in GetPocketVoucherDetail", exception);
            }

            return null;
        }
    }
}