using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices
{
    class ServiceNameManager
    {
        ServiceNameManager()
        {
        }

        public static ServiceNameManager Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly ServiceNameManager instance = new ServiceNameManager();
        }

        #region Public

        public string ServiceNameWithID(MenumateServicesIdentifier inID)
        {
            switch (inID)
            {
                case MenumateServicesIdentifier.AccountingIntegration: return "MenumateServiceAccounting";
                case MenumateServicesIdentifier.Reservations: return "MenumateServiceReservations";
                case MenumateServicesIdentifier.WebMate: return "MenumateServiceWebMate";
                case MenumateServicesIdentifier.LoyaltyMate: return "MenumateServiceLoyaltyMate";
                case MenumateServicesIdentifier.EFTPosIntegration: return "MenumateServiceEFTPosIntegration";
                case MenumateServicesIdentifier.PosIntegration: return "MenumateServicePosIntegration";
                case MenumateServicesIdentifier.SmartLink: return "MenumateServiceSmartLink";
                case MenumateServicesIdentifier.ThorLink: return "MenumateServiceThorlink";
                case MenumateServicesIdentifier.PocketVoucher: return "PocketVoucherWebService";
                case MenumateServicesIdentifier.SalesForceIntegration: return "SalesForceIntegrationWebService";
                case MenumateServicesIdentifier.SiHotIntegration: return "SiHotIntegrationWebService";
                case MenumateServicesIdentifier.WalletPayments: return "MenumateServiceWalletPayments";
                default: return @"";
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inID"></param>
        /// <returns></returns>
        public string ServiceDisplayNameWithID(MenumateServicesIdentifier inID)
        {
            switch (inID)
            {
                case MenumateServicesIdentifier.AccountingIntegration: return "Menumate Accounting Integration";
                case MenumateServicesIdentifier.SmartLink: return "Menumate SmartLink";
                case MenumateServicesIdentifier.WebMate: return "Menumate WebMate";
                case MenumateServicesIdentifier.LoyaltyMate: return "Menumate LoyaltyMate";
                case MenumateServicesIdentifier.EFTPosIntegration: return "Menumate EFTPOS Integration";
                case MenumateServicesIdentifier.PosIntegration: return "Menumate FloorPlan Integration";
                case MenumateServicesIdentifier.Reservations: return "Menumate Table Reservation Integration";
                case MenumateServicesIdentifier.ThorLink: return "Menumate Thor Link Integration";
                case MenumateServicesIdentifier.PocketVoucher: return "Menumate Pocket Voucher Service";
                case MenumateServicesIdentifier.SalesForceIntegration: return "Sales-Force Integration Service";
                case MenumateServicesIdentifier.WalletPayments: return "Menumate Wallet Payments Sevice";
                default: return @"";
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inID"></param>
        /// <returns></returns>
        public string ServiceDescriptionWithID(MenumateServicesIdentifier inID)
        {
            switch (inID)
            {
                case MenumateServicesIdentifier.AccountingIntegration: return "Menumate Interface to Accounting server";
                case MenumateServicesIdentifier.WebMate: return "Menumate WebMate server";
                case MenumateServicesIdentifier.LoyaltyMate: return "Menumate LoyaltyMate server";
                case MenumateServicesIdentifier.SmartLink: return "Menumate SmartLink Server";
                case MenumateServicesIdentifier.EFTPosIntegration: return "Not implemented in this version. Menumate Interface to EFTPOS Devices";
                case MenumateServicesIdentifier.PosIntegration: return "Not implemented in this version. Menumate Interface to FloorPlan";
                case MenumateServicesIdentifier.Reservations: return "Not implemented in this version. Menumate Interface to Table Reservation";
                case MenumateServicesIdentifier.ThorLink: return "Menumate Thor Link server";
                case MenumateServicesIdentifier.PocketVoucher: return "Menumate Pocket Voucher Service";
                case MenumateServicesIdentifier.SalesForceIntegration: return "Sales-Force Integration Service";
                case MenumateServicesIdentifier.WalletPayments: return "Menumate Wallet Payments Sevice";
                default: return @"";
            }
        }

        #endregion
    }
}
