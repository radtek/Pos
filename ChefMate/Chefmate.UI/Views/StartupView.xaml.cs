using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Input;
using Chefmate.Core.Commands;
using Chefmate.Core.Enums;
using Chefmate.Infrastructure.Utility;
using Chefmate.UI.KeyBoards;
using OpenFileDialog = Microsoft.Win32.OpenFileDialog;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for StartupView.xaml
    /// </summary>
    public partial class StartupView : Window, INotifyPropertyChanged
    {
        private string _terminalIpAddress;
        private string _dbIpAddress;
        private string _dbPath;
        private string _validationError;
        private string _displayName;
        private bool _isLocalDatabase;
        public StartupView()
        {
            InitializeComponent();
            CloseCommand = new DelegateCommand(CloseCommandHandler);
            CreateDatabaseCommand = new DelegateCommand(CreateDatabaseCommandHandler);
            TerminalNamePreviewMouseUpCommand = new DelegateCommand(TerminalNameClick);
            TerminalAddressPreviewMouseUpCommand = new DelegateCommand(TerminalIpAddressClick);
            DatabasePathPreviewMouseUpCommand = new DelegateCommand(DatabasePathClick);
            DatabaseAddressPreviewMouseUpCommand = new DelegateCommand(DatabaseIpAddressClick);
            this.DataContext = this;
        }
       
        #region Properties
        public DialogResult ModalResult { get; set; }
        public string TerminalIpAddress
        {
            get { return _terminalIpAddress; }
            set
            {
                _terminalIpAddress = value;
                OnPropertyChanged("TerminalIpAddress");
                CheckIpAddress(_terminalIpAddress, "Terminal IpAddress");
            }
        }
        public string DbIpAddress
        {
            get { return _dbIpAddress; }
            set
            {
                _dbIpAddress = value;
                OnPropertyChanged("DbIpAddress");
                CheckIpAddress(_dbIpAddress, "Databse IpAddress");
            }
        }
        public string DbPath
        {
            get { return _dbPath; }
            set
            {
                _dbPath = value;
                OnPropertyChanged("DbPath");
            }
        }
        public string ValidationError
        {
            get { return _validationError; }
            set
            {
                _validationError = value;
                OnPropertyChanged("ValidationError");
            }
        }
        public string DisplayName
        {
            get { return _displayName; }
            set
            {
                _displayName = value;
                OnPropertyChanged("DisplayName");
            }
        }
        public bool IsLocalDatabase
        {
            get { return _isLocalDatabase; }
            set
            {
                _isLocalDatabase = value;
                OnPropertyChanged("IsLocalDatabase");
                DbIpAddressTextBox.IsEnabled = !_isLocalDatabase;
                if (_isLocalDatabase)
                    DbIpAddress = "127.0.0.1";
            }
        }
        #endregion
     
        #region Commands
        public ICommand CloseCommand { get; set; }
        public ICommand CreateDatabaseCommand { get; set; }
        public ICommand TerminalNamePreviewMouseUpCommand { get; set; }
        public ICommand TerminalAddressPreviewMouseUpCommand { get; set; }
        public ICommand DatabasePathPreviewMouseUpCommand { get; set; }
        public ICommand DatabaseAddressPreviewMouseUpCommand { get; set; }
        private void CheckIpAddress(string ipAddress, string prop)
        {
            if (string.IsNullOrWhiteSpace(ipAddress) || !ChefmateUtility.IsValidIpAddress(ipAddress))
                ValidationError = "Invalid " + prop;
            else
                ValidationError = String.Empty;
        }
        private bool CheckDataBase()
        {
            if (!ChefmateUtility.TestConnection(_dbIpAddress, _dbPath))
            {
                ValidationError = "Couldn't connect to Database.";
                return false;
            }
            return true;
        }
        private bool DoValidations()
        {
            ValidationError = "";
            if (string.IsNullOrWhiteSpace(DisplayName))
            {
                ValidationError = "Please Enter a display name for terminal.";
                return false;
            }

            if (string.IsNullOrWhiteSpace(TerminalIpAddress) || !ChefmateUtility.IsValidIpAddress(TerminalIpAddress))
            {
                ValidationError = "Please Enter a valid ip address for terminal.";
                return false;
            }
            if (string.IsNullOrWhiteSpace(DbPath))
            {
                ValidationError = "Please Enter a path for database.";
                return false;
            }

            if (string.IsNullOrWhiteSpace(DbIpAddress) || !ChefmateUtility.IsValidIpAddress(DbIpAddress))
            {
                ValidationError = "Please Enter a valid ip address for databse.";
                return false;
            }
            try
            {
                if (!CheckDataBase())
                {
                    ValidationError = "Database connection failed. Please enter valid ipaddress and path for database.";
                    return false;
                }
            }
            catch (Exception)
            {
                ValidationError = "Database connection failed. Please enter valid ipaddress and path for database.";
                return false;
            }

            return true;
        }
        #endregion
  
        #region CommandHandlers
        private void CloseCommandHandler(object param)
        {
            var command = Convert.ToString(param);
            if (command.Equals("Ok"))
            {
                if (DoValidations())
                {
                    if (string.IsNullOrWhiteSpace(_displayName))
                        DisplayName = Environment.MachineName;
                    ModalResult = System.Windows.Forms.DialogResult.OK;
                    Close();
                }
            }
            else
            {
                ModalResult = System.Windows.Forms.DialogResult.Cancel;
                Close();
            }
        }
        private void CreateDatabaseCommandHandler(object obj)
        {
            var parserView = new ChefmateParserView();
            parserView.ShowInTaskbar = false;
            parserView.Topmost = true;
            parserView.ShowDialog();
        }
        private void TerminalNameClick(object sender)
        {
            DisplayName = KeyboardController.Instance.OpenKeyBoard("Enter display name for terminal", DisplayName);
        }
        private void TerminalIpAddressClick(object sender)
        {
            TerminalIpAddress = KeyboardController.Instance.OpenNumPad(TerminalIpAddress, NumpadMode.IpAddress);
        }
        private void DatabaseIpAddressClick(object sender)
        {
            DbIpAddress = KeyboardController.Instance.OpenNumPad(DbIpAddress, NumpadMode.IpAddress);
        }
        private void DatabasePathClick(object sender)
        {
            if (IsLocalDatabase)
            {
                var dialog = new OpenFileDialog
                {
                    Filter = "database files (*.fdb)|*.fdb",
                    Multiselect = false,
                    ShowReadOnly = true
                };
                var result = dialog.ShowDialog();
                if (result.HasValue && result.Value)
                    DbPath = dialog.FileName;
            }
            else
            {
                DbPath = KeyboardController.Instance.OpenKeyBoard("Enter database path", DbPath);
            }
        }
        #endregion

        #region PropertyChanged Implementation
        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged(string propertyName = null)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion
    }
}
