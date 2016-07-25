using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration.Install;
using System.Linq;

namespace Menumate.WinServices
{
    [RunInstaller(true)]
    public partial class ProjectInstaller : System.Configuration.Install.Installer
    {
        public ProjectInstaller()
        {
            InitializeComponent();
        }

        private void mmServiceProcessInstaller_AfterInstall(object sender, InstallEventArgs e)
        {

        }

        private void posIntegrationServiceInstaller_AfterInstall(object sender, InstallEventArgs e)
        {

        }

        private void reservationsServiceInstaller_AfterInstall(object sender, InstallEventArgs e)
        {

        }
    }
}
