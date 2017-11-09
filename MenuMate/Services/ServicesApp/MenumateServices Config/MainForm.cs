using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using MenumateServices.Utilities;
using WinOSInfo;

namespace MenumateServices.Config
{
    public partial class MainForm : Form
    {
        enum DatabaseConfigurationFolder
        {
            Unique,
            Webmate,
        };

        private MenumateServicesConfigurationManager _configurationManager;
        private DatabaseConfigurationFolder _databaseConfigurationFolder;
        private readonly Dictionary<DatabaseConfigurationFolder, string> _databaseFolders = new Dictionary<DatabaseConfigurationFolder, string>();
        private System.Timers.Timer _modifiedTimer;
        private readonly WinInfo _opertationSystemInformation = WinInfo.Instance;
        private bool _restartingService = false;

        delegate void InvokeMethod();

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            const string version = "6.0";

            lbVersion.Text = string.Format(@"Version {0}", version);
            _configurationManager = MenumateServicesConfigurationManager.Instance;

            InitializeDatabaseFolders();
            LoadExistingSettings();

            if (_configurationManager.UniqueDatabaseServer)
            {
                EnableUniqueDbServerSettings(true);
                EnableDatabaseServersSettings(false);
            }
            else
            {
                EnableUniqueDbServerSettings(false);
                EnableDatabaseServersSettings(true);
            }

            _modifiedTimer = CreateModifiedTimer();
            lbWarnning.Visible = false;
        }


        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (_restartingService)
            {
                MessageBox.Show(@"Failed to close. The Menumate Services application is still restarting");
                e.Cancel = true;
                return;
            }

            if (_configurationManager.Modified)
            {
                DialogResult result = MessageBox.Show(@"Settings have been modified. You must save before close",
                                                        @"Menumate Services Configuration",
                                                        MessageBoxButtons.YesNo, MessageBoxIcon.Question);

                e.Cancel = result == DialogResult.Yes;
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            CloseApplication();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (_configurationManager.Modified)
            {
                SaveSettings();
                RestartMenumateService();
            }
        }

        private void rbUniqueDBserver_Click(object sender, EventArgs e)
        {
            UpdateUniqueDatabaseServerSetting(rbUniqueDBserver.Checked);
            UpdateDatabaseServersSetting(rbDatabaseServers.Checked);

            EnableUniqueDbServerSettings(rbUniqueDBserver.Checked);
            EnableDatabaseServersSettings(rbDatabaseServers.Checked);
        }

        private void tbUniqueDBserverName_TextChanged(object sender, EventArgs e)
        {
            UpdateUniqueDatabaseServerNameSetting(tbUniqueDBserverName.Text);
        }

        private void tbUniqueDBserverFolder_TextChanged(object sender, EventArgs e)
        {
            UpdateUniqueDatabaseFolderSetting(tbUniqueDBserverFolder.Text);
        }

        private void rbDatabaseServers_CheckedChanged(object sender, EventArgs e)
        {
            UpdateDatabaseServersSetting(rbDatabaseServers.Checked);
            UpdateUniqueDatabaseServerSetting(rbUniqueDBserver.Checked);

            EnableUniqueDbServerSettings(rbUniqueDBserver.Checked);
            EnableDatabaseServersSettings(rbDatabaseServers.Checked);
        }

        private void tbDBServerWebmateName_TextChanged(object sender, EventArgs e)
        {
            UpdateDatabaseServerWebmateNameSetting(tbDBServerWebmateName.Text);
        }

        private void tbDBServerWebmateFolder_TextChanged(object sender, EventArgs e)
        {
            UpdateDbServerWebmateFolderSetting(tbDBServerWebmateFolder.Text);
        }

        private void btnUniqueDBFolderBrowse_Click(object sender, EventArgs e)
        {
            if (ShowDatabaseFolder(DatabaseConfigurationFolder.Unique))
            {
                tbUniqueDBserverFolder.Text = _databaseFolders[DatabaseConfigurationFolder.Unique];
                UpdateUniqueDatabaseFolderSetting(tbUniqueDBserverFolder.Text);
            }
        }

        private void btnWebmateDBFolderBrowse_Click(object sender, EventArgs e)
        {
            if (ShowDatabaseFolder(DatabaseConfigurationFolder.Webmate))
            {
                tbDBServerWebmateFolder.Text = _databaseFolders[DatabaseConfigurationFolder.Webmate];
                UpdateDbServerWebmateFolderSetting(tbDBServerWebmateFolder.Text);
            }
        }

        protected void CloseApplication()
        {
            Close();
        }

        protected void InitializeDatabaseFolders()
        {
            _databaseFolders[DatabaseConfigurationFolder.Unique] = _configurationManager.UniqueDatabaseServerFolder;
            _databaseFolders[DatabaseConfigurationFolder.Webmate] = _configurationManager.WebmateDatabaseServerFolder;
        }

        protected void LoadExistingSettings()
        {
            rbUniqueDBserver.Checked = _configurationManager.UniqueDatabaseServer;
            tbUniqueDBserverName.Text = _configurationManager.UniqueDatabaseServerName;
            tbUniqueDBserverFolder.Text = _configurationManager.UniqueDatabaseServerFolder;

            rbDatabaseServers.Checked = _configurationManager.DatabaseServers;
            tbDBServerWebmateName.Text = _configurationManager.WebmateDatabaseServerName;
            tbDBServerWebmateFolder.Text = _configurationManager.WebmateDatabaseServerFolder;

            //Clipp Settings
            tbClippServerIpAddress.Text = _configurationManager.ClippIpAddress;
            tbClippServerPort.Text = _configurationManager.ClippPort;

            _configurationManager.Reset();
        }

        protected void SaveSettings()
        {
            _configurationManager.SaveSettings();
        }

        private bool ShowDatabaseFolder(DatabaseConfigurationFolder databaseConfigurationFolder)
        {
            _databaseConfigurationFolder = databaseConfigurationFolder;

            fbdDatabase.SelectedPath = _databaseFolders[_databaseConfigurationFolder];
            var result = fbdDatabase.ShowDialog();
            if (result == DialogResult.OK)
            {
                _databaseFolders[_databaseConfigurationFolder] = fbdDatabase.SelectedPath;
                return true;
            }

            return false;
        }

        private void UpdateUniqueDatabaseServerSetting(bool inChecked)
        {
            _configurationManager.UniqueDatabaseServer = inChecked;
        }

        private void UpdateUniqueDatabaseServerNameSetting(string serverName)
        {
            _configurationManager.UniqueDatabaseServerName = serverName;
        }

        private void UpdateUniqueDatabaseFolderSetting(string folderPath)
        {
            _configurationManager.UniqueDatabaseServerFolder = folderPath;
        }

        private void UpdateDatabaseServersSetting(bool inChecked)
        {
            _configurationManager.DatabaseServers = inChecked;
        }

        private void UpdateDatabaseServerWebmateNameSetting(string inName)
        {
            _configurationManager.WebmateDatabaseServerName = inName;
        }

        private void UpdateDbServerWebmateFolderSetting(string inUrl)
        {
            _configurationManager.WebmateDatabaseServerFolder = inUrl;
        }

        private void EnableUniqueDbServerSettings(bool inEnabled)
        {
            tbUniqueDBserverName.Enabled = inEnabled;
            tbUniqueDBserverFolder.Enabled = inEnabled;

            btnUniqueDBFolderBrowse.Enabled = inEnabled;
        }

        private void EnableDatabaseServersSettings(bool inEnabled)
        {
            tbDBServerWebmateName.Enabled = inEnabled;
            tbDBServerWebmateFolder.Enabled = inEnabled;

            btnWebmateDBFolderBrowse.Enabled = inEnabled;
        }

        protected System.Timers.Timer CreateModifiedTimer()
        {
            //Create timer for 1 second..
            var result = new System.Timers.Timer(1000);

            // Hook up the Elapsed event for the timer. 
            result.Elapsed += OnModifiedTimerEvent;
            result.Enabled = true;

            return result;
        }

        protected void OnModifiedTimerEvent(Object source, EventArgs e)
        {
            if (lbWarnning.InvokeRequired)
            {
                lbWarnning.Invoke(new InvokeMethod(RefershWarningLabel));
            }
        }

        protected void RefershWarningLabel()
        {
            lbWarnning.Visible = _configurationManager.Modified;
        }



        private void RestartMenumateService()
        {
            if (IsAdministrator())
            {
                if (!_opertationSystemInformation.IsAdministrator())
                {
                    ShowErrorMessage(@"Failed to restart Xero Service. Close Xero Config and run it as administrator.");
                    return;
                }
            }

            ThrowRestartServiceThread();
        }

        private bool IsAdministrator()
        {
            WinOSTypes wType = _opertationSystemInformation.WinOSType();

            return !((wType == WinOSTypes.Win95) &&
                      (wType == WinOSTypes.Win98) &&
                      (wType == WinOSTypes.Win98SE) &&
                      (wType == WinOSTypes.WinME) &&
                      (wType == WinOSTypes.WinNT351) &&
                      (wType == WinOSTypes.WinNT4) &&
                      (wType == WinOSTypes.Win2000) &&
                      (wType == WinOSTypes.WinXP));
        }


        private void ThrowRestartServiceThread()
        {
            Thread thread = CreateRestartSvcThread();
            thread.Start();
        }

        private Thread CreateRestartSvcThread()
        {
            return new Thread(RestartService);
        }

        private void RestartService()
        {
            _restartingService = true;

            if (lbResetingMMServices.InvokeRequired)
            {
                lbResetingMMServices.Invoke(new InvokeMethod(ShowRestartingMenumateSvc));
            }

            const string serviceUri = @".\MenumateServices.exe";

            if (StopService(serviceUri))
            {
                Thread.Sleep(15000);
                StartService(serviceUri);
            }
            else
            {
                MessageBox.Show(@"Failed to stop Menumate Services. MenumateServices.exe was not found");
            }

            if (lbResetingMMServices.InvokeRequired)
            {
                lbResetingMMServices.Invoke(new InvokeMethod(HideRestartingMenumateSvc));
            }

            _restartingService = false;
            _configurationManager.Reset();
        }

        private bool StopService(string inServiceUri)
        {
            // Prepare the process to run
            var start = new ProcessStartInfo
            {
                Arguments = @"--stop",
                FileName = inServiceUri,
                WindowStyle = ProcessWindowStyle.Hidden,
                CreateNoWindow = true
            };

            // Enter in the command line arguments, everything you would enter after the executable name itself
            // Enter the executable to run, including the complete path
            // Do you want to show a console window?
            try
            {
                // Run the external process & wait for it to finish
                using (Process process = Process.Start(start))
                {
                    process.WaitForExit();

                    // Retrieve the app's exit code
                    int exitCode = process.ExitCode;
                }

                return true;
            }
            catch
            {
                return false;
            }
        }

        private bool StartService(string inServiceUri)
        {
            // Prepare the process to run
            var start = new ProcessStartInfo
            {
                Arguments = @"--start",
                FileName = inServiceUri,
                WindowStyle = ProcessWindowStyle.Hidden,
                CreateNoWindow = true
            };

            // Enter in the command line arguments, everything you would enter after the executable name itself
            // Enter the executable to run, including the complete path
            // Do you want to show a console window?
            try
            {
                // Run the external process & wait for it to finish
                using (Process process = Process.Start(start))
                {
                    process.WaitForExit();

                    // Retrieve the app's exit code
                    int exitCode = process.ExitCode;
                }

                return true;
            }
            catch
            {
                return false;
            }
        }

        private void ShowRestartingMenumateSvc()
        {
            lbResetingMMServices.Visible = true;

            EnableUniqueDbServerSettings(false);
            EnableDatabaseServersSettings(false);

            btnSave.Enabled = false;
            btnClose.Enabled = false;
        }

        private void HideRestartingMenumateSvc()
        {
            lbResetingMMServices.Visible = false;

            if (_configurationManager.UniqueDatabaseServer)
            {
                EnableUniqueDbServerSettings(true);
            }
            else
            {
                EnableDatabaseServersSettings(true);
            }

            btnSave.Enabled = true;
            btnClose.Enabled = true;
        }

        private void ShowErrorMessage(string inErrorMsg)
        {
            MessageBox.Show(inErrorMsg);
        }

        private void tbClippServerIpAddress_TextChanged(object sender, EventArgs e)
        {
            _configurationManager.ClippIpAddress = tbClippServerIpAddress.Text;
        }

        private void tbClippServerPort_TextChanged(object sender, EventArgs e)
        {
            _configurationManager.ClippPort = tbClippServerPort.Text;
        }
    }
}
