namespace Menumate.WinServices
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
            this.mmServiceProcessInstaller = new System.ServiceProcess.ServiceProcessInstaller();
            this.mmServiceInstaller = new System.ServiceProcess.ServiceInstaller();
            // 
            // mmServiceProcessInstaller
            // 
            this.mmServiceProcessInstaller.Account = System.ServiceProcess.ServiceAccount.LocalSystem;
            this.mmServiceProcessInstaller.Password = null;
            this.mmServiceProcessInstaller.Username = null;
            this.mmServiceProcessInstaller.AfterInstall += new System.Configuration.Install.InstallEventHandler(this.mmServiceProcessInstaller_AfterInstall);
            // 
            // mmServiceInstaller
            // 
            this.mmServiceInstaller.Description = "Menumate PosIntegration and Reservations";
            this.mmServiceInstaller.DisplayName = "Menumate Service";
            this.mmServiceInstaller.ServiceName = "MMWinService";
            this.mmServiceInstaller.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            this.mmServiceInstaller.AfterInstall += new System.Configuration.Install.InstallEventHandler(this.posIntegrationServiceInstaller_AfterInstall);
            // 
            // ProjectInstaller
            // 
            this.Installers.AddRange(new System.Configuration.Install.Installer[] {
            this.mmServiceInstaller,
            this.mmServiceProcessInstaller});

        }

        #endregion

        private System.ServiceProcess.ServiceProcessInstaller mmServiceProcessInstaller;
        private System.ServiceProcess.ServiceInstaller mmServiceInstaller;
    }
}