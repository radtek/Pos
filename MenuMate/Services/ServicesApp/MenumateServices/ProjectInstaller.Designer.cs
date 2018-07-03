using System.ServiceProcess;

namespace MenumateServices
{
    partial class ProjectInstaller
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.serviceProcessInstaller = new System.ServiceProcess.ServiceProcessInstaller();
            this.serviceInstallerAccounting = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerLoyaltyMate = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerWebMate = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerSmartLink = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerPocketVoucher = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerSalesForceIntegration = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerSiHotIntegration = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerWalletPayments = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerSmartConnect = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerAdyenIntegration = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerPaymentSense = new System.ServiceProcess.ServiceInstaller();
            // 
            // serviceProcessInstaller
            // 
            this.serviceProcessInstaller.Account = System.ServiceProcess.ServiceAccount.LocalSystem;
            this.serviceProcessInstaller.Password = null;
            this.serviceProcessInstaller.Username = null;
            this.serviceProcessInstaller.AfterInstall += new System.Configuration.Install.InstallEventHandler(this.serviceProcessInstaller_AfterInstall);
            // 
            // serviceInstallerAccounting
            // 
            this.serviceInstallerAccounting.DelayedAutoStart = true;
            this.serviceInstallerAccounting.Description = "Menumate Accounting server";
            this.serviceInstallerAccounting.DisplayName = "Menumate Accounting";
            this.serviceInstallerAccounting.ServiceName = "MenumateServiceAccounting";
            this.serviceInstallerAccounting.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerLoyaltyMate
            // 
            this.serviceInstallerLoyaltyMate.Description = "Menumate LoyaltyMate server";
            this.serviceInstallerLoyaltyMate.DisplayName = "Menumate LoyaltyMate";
            this.serviceInstallerLoyaltyMate.ServiceName = "MenumateServiceLoyaltyMate";
            this.serviceInstallerLoyaltyMate.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerWebMate
            // 
            this.serviceInstallerWebMate.Description = "Menumate Webmate server";
            this.serviceInstallerWebMate.DisplayName = "Menumate Webmate";
            this.serviceInstallerWebMate.ServiceName = "MenumateServiceWebMate";
            this.serviceInstallerWebMate.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerSmartLink
            // 
            this.serviceInstallerSmartLink.Description = "Menumate SmartLink Server";
            this.serviceInstallerSmartLink.DisplayName = "Menumate SmartLink";
            this.serviceInstallerSmartLink.ServiceName = "MenumateServiceSmartLink";
            this.serviceInstallerSmartLink.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerPocketVoucher
            // 
            this.serviceInstallerPocketVoucher.Description = "Menumate Pocket Voucher";
            this.serviceInstallerPocketVoucher.DisplayName = "MenumateServicePocketVoucher";
            this.serviceInstallerPocketVoucher.ServiceName = "MenumateServicePocketVoucher";
            this.serviceInstallerPocketVoucher.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerSalesForceIntegration
            // 
            this.serviceInstallerSalesForceIntegration.Description = "Menumate Sales Force";
            this.serviceInstallerSalesForceIntegration.DisplayName = "MenumateServiceSalesForce";
            this.serviceInstallerSalesForceIntegration.ServiceName = "MenumateServiceSalesForce";
            this.serviceInstallerSalesForceIntegration.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerSiHotIntegration
            // 
            this.serviceInstallerSiHotIntegration.Description = "Menumate SiHot Integration ";
            this.serviceInstallerSiHotIntegration.DisplayName = "MenumateServiceSiHotIntegration";
            this.serviceInstallerSiHotIntegration.ServiceName = "MenumateServiceSiHotIntegration";
            this.serviceInstallerSiHotIntegration.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerWalletPayments
            // 
            this.serviceInstallerWalletPayments.Description = "Menumate Wallet Payments Sevice";
            this.serviceInstallerWalletPayments.DisplayName = "Menumate Wallet Payments Sevice";
            this.serviceInstallerWalletPayments.ServiceName = "MenumateServiceWalletPayments";
            this.serviceInstallerWalletPayments.StartType = System.ServiceProcess.ServiceStartMode.Disabled;
            // 
            // serviceInstallerSmartConnect
            // 
            this.serviceInstallerSmartConnect.Description = "Menumate Smart Connect Server";
            this.serviceInstallerSmartConnect.DisplayName = "Menumate SmartConnect";
            this.serviceInstallerSmartConnect.ServiceName = "MenumateServiceSmartConnect";
            this.serviceInstallerSmartConnect.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerAdyenIntegration
            // 
            this.serviceInstallerAdyenIntegration.Description = "Menumate Adyen Integration Sevice";
            this.serviceInstallerAdyenIntegration.DisplayName = "Menumate Adyen Integration Sevice";
            this.serviceInstallerAdyenIntegration.ServiceName = "MenumateServiceAdyenIntegration";
            this.serviceInstallerAdyenIntegration.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerPaymentSense
            // 
            this.serviceInstallerPaymentSense.Description = "Menumate Payment Sense Server";
            this.serviceInstallerPaymentSense.DisplayName = "Menumate PaymentSense";
            this.serviceInstallerPaymentSense.ServiceName = "MenumateServicePaymentSense";
            this.serviceInstallerPaymentSense.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // ProjectInstaller
            // 
            this.Installers.AddRange(new System.Configuration.Install.Installer[] {
            this.serviceProcessInstaller,
            this.serviceInstallerAccounting,
            this.serviceInstallerLoyaltyMate,
            this.serviceInstallerWebMate,
            this.serviceInstallerSmartLink,
            this.serviceInstallerPocketVoucher,
            this.serviceInstallerSalesForceIntegration,
            this.serviceInstallerSiHotIntegration,
            this.serviceInstallerWalletPayments,
            this.serviceInstallerSmartConnect,
            this.serviceInstallerAdyenIntegration,
            this.serviceInstallerPaymentSense});

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inID"></param>
        /// <returns></returns>
        private string serviceName(MenumateServicesIdentifier inID)
        {
            return ServiceNameManager.Instance.ServiceNameWithID(inID);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inID"></param>
        /// <returns></returns>
        private string serviceDisplayName(MenumateServicesIdentifier inID)
        {
            return ServiceNameManager.Instance.ServiceDisplayNameWithID(inID);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inID"></param>
        /// <returns></returns>
        private string serviceDescription(MenumateServicesIdentifier inID)
        {
            return ServiceNameManager.Instance.ServiceDescriptionWithID(inID);
        }

        #endregion

        private System.ServiceProcess.ServiceProcessInstaller serviceProcessInstaller;
        private System.ServiceProcess.ServiceInstaller serviceInstallerAccounting;
        private System.ServiceProcess.ServiceInstaller serviceInstallerLoyaltyMate;
        private System.ServiceProcess.ServiceInstaller serviceInstallerWebMate;
        private System.ServiceProcess.ServiceInstaller serviceInstallerSmartLink;
        private ServiceInstaller serviceInstallerPocketVoucher;
        private ServiceInstaller serviceInstallerSalesForceIntegration;
        private ServiceInstaller serviceInstallerSiHotIntegration;
        private ServiceInstaller serviceInstallerWalletPayments;
        private ServiceInstaller serviceInstallerSmartConnect;
        private ServiceInstaller serviceInstallerAdyenIntegration;
        private ServiceInstaller serviceInstallerPaymentSense;
    }
}