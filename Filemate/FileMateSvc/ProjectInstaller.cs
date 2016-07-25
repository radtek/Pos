using System.ComponentModel;
using System.Configuration.Install;


namespace FileMateSvc {
    [RunInstaller(true)]
    public partial class ProjectInstaller : Installer {
        public ProjectInstaller() {
            InitializeComponent();
        }
    }
}
