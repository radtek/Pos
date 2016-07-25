using System;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;

namespace Safemate.Configurator
{
    public partial class AddTarget : Window
    {
        public enum Type {
            Ftp,
            Local
        }

        public enum Mode {
            Add,
            Edit
        }

        public
        AddTarget(Mode mode, Type type)
        {
            InitializeComponent();
            ConfigureForSpecificType(mode, type);
        }

        void
        ConfigureForSpecificType(
            Mode mode,
            Type type)
        {
            Title = contentTweaks[(int)mode << 1] + " target";
            btnAddTarget.Content = contentTweaks[((int)mode << 1) + 1];

            locationChecker =
                new Regex(locationValidator[(int)type]);

            btnBrowse.IsEnabled = type != Type.Ftp;
        }

        public string TargetLocation
        {
            get {
                return tbLocation.Text;
            } set {
                tbLocation.Text = value;
            }
        }

        public string TargetName
        {
            get {
                return tbName.Text;
            } set {
                tbName.Text = value;
            }
        }

        public Credential TargetAuthenticationCredential
        {
            get {
                if (!cbRequiresAuthentication.IsChecked.Value)
                    return null;

                return new Credential(tbUsername.Text,
                                      tbPassword.Password);
            } set {
                if (value != null) {
                    cbRequiresAuthentication.IsChecked =
                        tbUsername.IsEnabled =
                            tbPassword.IsEnabled = true;
                    tbUsername.Text = value.Username;
                    tbPassword.Password = value.Password;
                }
            }
        }

        Regex nameChecker = new Regex(@"^[^\s]+.*$");
        Regex locationChecker;

        string[] locationValidator = new string[] {
            "^ftp://.+/$",
            @"^(([a-zA-Z]:|\\\\[^\\]+\\.+)\\.*)$"
        };

        string[] contentTweaks = new string[] {
            "Add", "_Add target",
            "Edit", "C_ommit changes"
        };

        void
        cbRequiresAuthentication_Click(
            object sender,
            RoutedEventArgs e)
        {
            tbPassword.IsEnabled =
                tbUsername.IsEnabled = cbRequiresAuthentication.IsChecked.Value;
        }

        void
        btnAddTarget_Click(
            object sender,
            RoutedEventArgs e)
        {
            bool targetValid =
                nameChecker.IsMatch(tbName.Text)
                & locationChecker.IsMatch(tbLocation.Text);

            if (cbRequiresAuthentication.IsChecked.Value)
                targetValid &=
                    tbUsername.Text.Length > 0
                    && tbPassword.Password.Length > 0;

            if (!targetValid) {
                System.Windows.Forms.MessageBox.Show(
                    "Invalid target settings! Ensure that the target name "
                    + "doesn't begin with whitespace and that the location "
                    + "is set correctly.",
                    "Invaild target settings",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                return;
            }

            DialogResult = targetValid;
            Close();
        }

        private void btnBrowse_Click(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "Where would you like to mirror the databases?";
            fbd.ShowNewFolderButton = true;
            fbd.RootFolder = Environment.SpecialFolder.MyComputer;

            if (fbd.ShowDialog()
                == System.Windows.Forms.DialogResult.OK)
                tbLocation.Text = fbd.SelectedPath;
        }
    };
};
