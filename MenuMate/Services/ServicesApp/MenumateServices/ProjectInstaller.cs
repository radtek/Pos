using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration.Install;
using System.Linq;


namespace MenumateServices
{
    [RunInstaller(true)]
    public partial class ProjectInstaller : System.Configuration.Install.Installer
    {
        public ProjectInstaller()
        {
            InitializeComponent();
        }

        private void serviceInstallerEFTPOS_AfterInstall(object sender, InstallEventArgs e)
        {

        }

        private void serviceInstallerLoyaltyMate_AfterInstall(object sender, InstallEventArgs e)
        {

        }

        private void serviceInstallerXero_AfterInstall(object sender, InstallEventArgs e)
        {

        }

        private void serviceProcessInstaller_AfterInstall(object sender, InstallEventArgs e)
        {

        }

        private void serviceInstallerXeroIntegration_AfterInstall(object sender, InstallEventArgs e)
        {

        }
    }
}
