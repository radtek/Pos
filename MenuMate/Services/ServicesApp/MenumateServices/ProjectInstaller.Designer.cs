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
            this.serviceInstallerXero = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerLoyaltyMate = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerWebMate = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerChefMate = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerSmartLink = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerClippIntegration = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerThorLink = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerPocketVoucher = new System.ServiceProcess.ServiceInstaller();
            this.serviceInstallerSalesForceIntegration = new System.ServiceProcess.ServiceInstaller();
            // 
            // serviceProcessInstaller
            // 
            this.serviceProcessInstaller.Account = System.ServiceProcess.ServiceAccount.LocalSystem;
            this.serviceProcessInstaller.Password = null;
            this.serviceProcessInstaller.Username = null;
            this.serviceProcessInstaller.AfterInstall += new System.Configuration.Install.InstallEventHandler(this.serviceProcessInstaller_AfterInstall);
            // 
            // serviceInstallerXero
            // 
            this.serviceInstallerXero.DelayedAutoStart = true;
            this.serviceInstallerXero.Description = "Menumate Xero server";
            this.serviceInstallerXero.DisplayName = "Menumate Xero";
            this.serviceInstallerXero.ServiceName = "MenumateServiceXero";
            this.serviceInstallerXero.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
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
            // serviceInstallerChefMate
            // 
            this.serviceInstallerChefMate.Description = "Menumate Chefmate server: Provide all the Chefmate terminals with database access" +
                "";
            this.serviceInstallerChefMate.DisplayName = "Menumate Chefmate";
            this.serviceInstallerChefMate.ServiceName = "MenumateServiceChefMate";
            this.serviceInstallerChefMate.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerSmartLink
            // 
            this.serviceInstallerSmartLink.Description = "Menumate SmartLink Server";
            this.serviceInstallerSmartLink.DisplayName = "Menumate SmartLink";
            this.serviceInstallerSmartLink.ServiceName = "MenumateServiceSmartLink";
            this.serviceInstallerSmartLink.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerClippIntegration
            // 
            this.serviceInstallerClippIntegration.Description = "Menumate Clipp Integration";
            this.serviceInstallerClippIntegration.DisplayName = "MenumateServiceClippIntegration";
            this.serviceInstallerClippIntegration.ServiceName = "MenumateServiceClippIntegration";
            this.serviceInstallerClippIntegration.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerThorLink
            // 
            this.serviceInstallerThorLink.Description = "Menumate Thor Link Integration";
            this.serviceInstallerThorLink.DisplayName = "MenumateThorLinkIntegration";
            this.serviceInstallerThorLink.ServiceName = "MenumateServiceThorlink";
            this.serviceInstallerThorLink.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstallerPocketVoucher
            // 
            this.serviceInstallerPocketVoucher.Description = "Menumate Pocket Voucher";
            this.serviceInstallerPocketVoucher.DisplayName = "MenumateServicePocketVoucher";
            this.serviceInstallerPocketVoucher.ServiceName = "MenumateServicePocketVoucher";
            this.serviceInstallerPocketVoucher.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // serviceInstaller1
            // 
            this.serviceInstallerSalesForceIntegration.ServiceName = "MenumateServiceSalesForce";
            this.serviceInstallerSalesForceIntegration.Description = "Menumate Sales Force";
            this.serviceInstallerSalesForceIntegration.DisplayName = "MenumateServiceSalesForce";
            this.serviceInstallerSalesForceIntegration.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // ProjectInstaller
            // 
            this.Installers.AddRange(new System.Configuration.Install.Installer[] {
            this.serviceProcessInstaller,
            this.serviceInstallerXero,
            this.serviceInstallerLoyaltyMate,
            this.serviceInstallerWebMate,
            this.serviceInstallerChefMate,
            this.serviceInstallerSmartLink,
            this.serviceInstallerClippIntegration,
            this.serviceInstallerThorLink,
            this.serviceInstallerPocketVoucher,
            this.serviceInstallerSalesForceIntegration});

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
        private System.ServiceProcess.ServiceInstaller serviceInstallerXero;
        private System.ServiceProcess.ServiceInstaller serviceInstallerLoyaltyMate;
        private System.ServiceProcess.ServiceInstaller serviceInstallerWebMate;
        private System.ServiceProcess.ServiceInstaller serviceInstallerChefMate;
        private System.ServiceProcess.ServiceInstaller serviceInstallerSmartLink;
        private ServiceInstaller serviceInstallerClippIntegration;
        private ServiceInstaller serviceInstallerThorLink;
        private ServiceInstaller serviceInstallerPocketVoucher;
        private ServiceInstaller serviceInstallerSalesForceIntegration;
    }
}