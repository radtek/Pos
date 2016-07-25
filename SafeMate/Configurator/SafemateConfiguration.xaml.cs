using FirebirdSql.Data.FirebirdClient;
using Microsoft.Win32;
using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Forms;
using System.Xml;
using System.ServiceProcess;
using System.Reflection;

using System.Runtime.InteropServices;

namespace Safemate.Configurator
{
    public partial class SafemateConfigurationWindow : Window
    {
        bool
        DetermineClientAccount(
            string datasource)
        {
            FbConnection conn = new FbConnection();
            bool wasSuccessful = true;

            try {
                FbCommand c;
                FbConnectionStringBuilder csb =
                    new FbConnectionStringBuilder();
                FbDataReader dr;

                csb.Charset = "NONE";
                csb.Database = tbSourcePath.Text;
                csb.DataSource = datasource;
                csb.Password = "masterkey";
                csb.UserID = "sysdba";

                conn.ConnectionString = csb.ToString();
                conn.Open();

                c = new FbCommand(
                    "select first 1 Company from Registration",
                    conn);

                (dr = c.ExecuteReader()).Read();
                lblRegisteredTo.Content = dr.GetString(0);
            } catch {
                System.Windows.Forms.MessageBox.Show(
                    "Couldn't determine your account. Verify that your "
                    + "Menumate installation is registered. The configurator"
                    + " will now exit. If this problem persists, contact "
                    + "Menumate technical support.",
                    "Error determining registration details",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                wasSuccessful = false;
            } finally {
                conn.Close();
            }

            return wasSuccessful;
        }

        string
        DetermineInstallationAndDatabasePath()
        {
            string registryKey =
                @"HKEY_LOCAL_MACHINE\SOFTWARE\IQWorks\MenuMate";
            string databasePath =
                (string)Registry.GetValue(registryKey
                                          + @"\Database",
                                          "DatabasePath",
                                          null);
            string databaseIp =
                (string)Registry.GetValue(registryKey
                                          + @"\Database",
                                          "InterbaseIP",
                                          null);

            installationPath =
                (string)Registry.GetValue(registryKey,
                                          "InstallDir",
                                          null);
            if (installationPath == null)
                installationPath = @"C:\Program Files\Menumate";
            if (databasePath == null)
                databasePath = installationPath + @"\Menumate.fdb";
            if (databaseIp == null)
                databaseIp = "127.0.0.1";

            installationPath += @"\Safemate";
            tbSourcePath.Text = databasePath;
            return databaseIp;
        }

        public bool
        InitializeConfigurator()
        {
            if (!DetermineClientAccount(
                    DetermineInstallationAndDatabasePath()))
                return false;
            else if (!LoadOrKeepDefaultConfiguration())
                return false;

            IsServiceInstalled();
            return true;
        }

        bool
        LoadOrKeepDefaultConfiguration()
        {
            XmlDocument configFileAsXml = new XmlDocument();
            string[] potentialConfigurationFiles = {
                @"\Safemate.Configuration.xml",
                @"\Safemate.Configuration.Default.xml"
            };
            string fullPath;

            for (int i = 0; i < 2; i++)
                try {
                    fullPath = installationPath
                               + potentialConfigurationFiles[i];
                    if (!(new FileInfo(fullPath)).Exists)
                        continue;
                    configFileAsXml.Load(fullPath);
                    configuration = new Configuration(
                        (string)lblRegisteredTo.Content, configFileAsXml);
                    SynchronizeDisplayWithConfiguration();
                    return true;
                } catch {
                    ;
                }

            System.Windows.Forms.MessageBox.Show(
                "Couldn't load either the base "
                + "configuration file nor the default "
                + "template. Safemate installation corrupt.",
                "Configuration read error.",
                MessageBoxButtons.OK,
                MessageBoxIcon.Error);

            return false;
        }

        void
        SynchronizeDisplayWithConfiguration()
        {
            SynchronizeEmailNotifications();
            SynchronizeBackupConfiguration();
        }

        void
        SynchronizeEmailNotifications()
        {
            EmailErrorNotification een =
                configuration.CopyGroupInstance.CopyOperation.EmailErrorNotification;
            SenderConfiguration sc = een.SenderConfiguration;

            tbSendingFrom.Text = sc.ReplyToAddress;
            tbSmtpServer.Text = sc.ServerAddress;
            tbSmtpPort.Text = sc.ServerPort;
            tbSmtpUsername.Text = sc.Credential.Username;
            tbSmtpPassword.Text = sc.Credential.Password;

            chbUseSsl.IsChecked = sc.UsingSsl;

            foreach (String m in een.Recipients)
                lbRecipients.Items.Add(m);
        }

        void
        SynchronizeBackupConfiguration()
        {
            CopyOperation co = configuration.CopyGroupInstance.CopyOperation;
            TargetCollection tc = co.Targets;

            tbSourcePath.Text = co.Source.Location;

            foreach (KeyValuePair<string, Target> t in tc.Targets)
                lbTargets.Items.Add(t.Key);

            cbIntervalUnit.SelectedIndex =
                (int)co.ScheduledFor.TimeUnitOfSchedule;
            tbInterval.Text = co.ScheduledFor.ScheduleString;
        }

        public
        SafemateConfigurationWindow()
        {
            InitializeComponent();
        }

        void
        btnSelectSource_Click(
            object sender,
            RoutedEventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();

            fbd.Description =
                "Where are the Menumate databases stored?";
            fbd.RootFolder =
                System.Environment.SpecialFolder.MyComputer;
            fbd.SelectedPath = tbSourcePath.Text;
            fbd.ShowNewFolderButton = false;

            if (fbd.ShowDialog() ==
                System.Windows.Forms.DialogResult.OK)
                tbSourcePath.Text = fbd.SelectedPath;
        }

        void
        btnRemoveRecipient_Click(
            object sender,
            RoutedEventArgs e)
        {
            EmailErrorNotification een =
                configuration.CopyGroupInstance.CopyOperation.EmailErrorNotification;

            een.RemoveRecipient((string)lbRecipients.SelectedItem);
            lbRecipients.Items.RemoveAt(lbRecipients.SelectedIndex);

            btnRemoveRecipient.IsEnabled = false;
        }

        void
        btnRemoveTarget_Click(
            object sender,
            RoutedEventArgs e)
        {
            TargetCollection tc =
                configuration.CopyGroupInstance.CopyOperation.Targets;

            tc.RemoveTarget((string)lbTargets.SelectedItem);
            lbTargets.Items.RemoveAt(lbTargets.SelectedIndex);

            btnRemoveTarget.IsEnabled = btnEditTarget.IsEnabled = false;
        }

        void
        lbTargets_SelectionChanged(
            object sender,
            System.Windows.Controls.SelectionChangedEventArgs e)
        {
            btnRemoveTarget.IsEnabled =
                btnEditTarget.IsEnabled = lbTargets.SelectedIndex != 0;
        }

        void
        btnAddRecipient_Click(
            object sender,
            RoutedEventArgs e)
        {
            EmailErrorNotification een =
                configuration.CopyGroupInstance.CopyOperation.EmailErrorNotification;

            if (tbRecipientAddress.Text.Length == 0)
                return;

            een.AddRecipient(tbRecipientAddress.Text);
            lbRecipients.Items.Add(tbRecipientAddress.Text);
            tbRecipientAddress.Text = "";
        }

        void
        lbRecipients_SelectionChanged(
            object sender,
            System.Windows.Controls.SelectionChangedEventArgs e)
        {
            btnRemoveRecipient.IsEnabled = true;
        }

        void
        btnAddLocalTarget_Click(
            object sender,
            RoutedEventArgs e)
        {
            AddTarget at = new AddTarget(AddTarget.Mode.Add,
                                         AddTarget.Type.Local);

            if (at.ShowDialog().Value) {
                TargetCollection tc =
                    configuration.CopyGroupInstance.CopyOperation.Targets;

                tc.AddTarget(at.TargetName,
                             at.TargetLocation,
                             at.TargetAuthenticationCredential);
                lbTargets.Items.Add(at.TargetName);
            }
        }

        void
        lbTargets_MouseDoubleClick(
            object sender,
            System.Windows.Input.MouseButtonEventArgs e)
        {
            if (lbTargets.SelectedItems.Count != 0
                && lbTargets.SelectedIndex != 0)
                btnEditTarget_Click(sender, e);
        }

        void
        btnAddFtpTarget_Click(
            object sender,
            RoutedEventArgs e)
        {
            AddTarget at = new AddTarget(AddTarget.Mode.Edit,
                                         AddTarget.Type.Ftp);

            if (at.ShowDialog().Value) {
                TargetCollection tc =
                    configuration.CopyGroupInstance.CopyOperation.Targets;

                tc.AddTarget(at.TargetName,
                             at.TargetLocation,
                             at.TargetAuthenticationCredential);
                lbTargets.Items.Add(at.TargetName);
            }
        }

        void
        btnEditTarget_Click(
            object sender,
            RoutedEventArgs e)
        {
            if (lbTargets.SelectedItem != null) {
                TargetCollection tc =
                    configuration.CopyGroupInstance.CopyOperation.Targets;
                Target t = tc.Targets[lbTargets.SelectedValue.ToString()];
                Regex r = new Regex("^ftp://.*/$");

                AddTarget at =
                    new AddTarget(AddTarget.Mode.Edit,
                                  r.IsMatch(t.Location) ? AddTarget.Type.Ftp
                                  : AddTarget.Type.Local);
                at.TargetName = t.Name;
                at.TargetLocation = t.Location;
                at.TargetAuthenticationCredential = t.AuthorizationCredential;

                if ((at.ShowDialog()).Value) {
                    tc.RemoveTarget(t.Name);
                    lbTargets.Items.RemoveAt(lbTargets.SelectedIndex);
                    tc.AddTarget(at.TargetName,
                                 at.TargetLocation,
                                 at.TargetAuthenticationCredential);
                    lbTargets.Items.Add(at.TargetName);
                }
            }
        }

        void
        btnSaveSettings_Click(
            object sender,
            RoutedEventArgs e)
        {
            FileInfo existingConfiguration = new FileInfo(
                installationPath + @"\Safemate.Configuration.xml");

            try {
                XmlDocument serializedConfiguration = configuration.Serialize();

                if (existingConfiguration.Exists)
                    existingConfiguration.CopyTo(
                        existingConfiguration.FullName + ".bak", true);

                serializedConfiguration.Save(existingConfiguration.FullName);
            } catch {
                System.Windows.Forms.MessageBox.Show(
                    String.Format("The settings could not be saved to '{0}'. "
                                  + "Please try again. If the problem persists, "
                                  + "ensure that the Safemate directory still exists"
                                  + " and that the program has been launched with "
                                  + "sufficient privileges.",
                                  existingConfiguration.FullName),
                    "Error writing configuration",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
            }
        }

        void
        RefreshServiceStatus()
        {
            if ((btnStopService.IsEnabled =
                     service.Status != ServiceControllerStatus.Stopped)) {
                btnToggleService.Content = "Restart service";
                lblServiceStatus.Content = "Service running";
            } else {
                btnToggleService.Content = "Start service";
                lblServiceStatus.Content = "Service stopped.";
                btnStopService.IsEnabled = false;
            }
        }

        bool
        IsServiceInstalled()
        {
            try {
                service =
                    new ServiceController("SafemateBackupService");

                RefreshServiceStatus();
                return true;
            } catch {
                btnToggleService.IsEnabled = false;
                btnStopService.IsEnabled = false;
                lblServiceStatus.Content = "Service not installed.";
            }

            return false;
        }

        void
        ToggleServiceState()
        {
            if (service.Status == ServiceControllerStatus.Stopped)
                service.Start();
            else service.Stop();
        }

        void
        RestartService()
        {
            service.Stop();
            service.Start();
        }


        Configuration configuration;
        string installationPath;
        ServiceController service;

        void
        tbRecipientAddress_TextChanged(
            object sender,
            System.Windows.Controls.TextChangedEventArgs e)
        {
            Regex check = new Regex(@"^[^\s]+");

            btnAddRecipient.IsEnabled =
                check.IsMatch(tbRecipientAddress.Text);
        }
    };
};
