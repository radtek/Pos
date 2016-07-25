using System.Windows;

namespace Safemate.Configurator
{
    public partial class App : Application
    {
        void
        App_StartupHandler(
            object sender,
            StartupEventArgs eventArgs)
        {
            SafemateConfigurationWindow scw =
                new SafemateConfigurationWindow();
            if (!scw.InitializeConfigurator())
                Application.Current.Shutdown(-1);
            scw.Show();
        }
    };
};
