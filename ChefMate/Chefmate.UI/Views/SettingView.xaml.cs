using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Drawing.Text;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using Chefmate.Core.Commands;
using Chefmate.Core.Enums;
using Chefmate.Core.Extensions;
using Chefmate.Core.Model;
using Chefmate.Database.DbModels;
using Chefmate.Infrastructure.Controller;
using Chefmate.Infrastructure.Utility;
using Chefmate.SoundPlayer;
using Chefmate.UI.Annotations;
using Chefmate.UI.KeyBoards;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for SettingView.xaml
    /// </summary>
    public partial class SettingView : Window, INotifyPropertyChanged
    {
        private ObservableCollection<Terminal> _terminals;
        private ObservableCollection<string> _soundList;
        private ObservableCollection<string> _fontFamilies;
        private ObservableCollection<int> _fontSizes;
        private double _sliderValue;
        private Terminal _selectedTerminal;
        private string _validationError;
        private Settings _currentSettings;

        public SettingView()
        {
            InitializeComponent();
            CurrentSettings = ChefmateController.Instance.CurrentSettings.Clone() as Settings;
            Initialize();
            TestConnectionCommand = new DelegateCommand(TestConnectionHandler);
            SaveCommand = new DelegateCommand(SaveCommandHandler);
            CloseCommand = new DelegateCommand(CloseCommandHandler);
            TerminalNamePreviewMouseUpCommand = new DelegateCommand(TerminalNameClick);
            TerminalAddressPreviewMouseUpCommand = new DelegateCommand(TerminalAddressClick);
            DatabasePathPreviewMouseUpCommand = new DelegateCommand(DatabasePathPreviewMouseUp);
            DatabaseAddressPreviewMouseUpCommand = new DelegateCommand(DatabaseAddreddMouseUp);
            FirstWarningPreviewMouseUpCommand = new DelegateCommand(FirstWarningTimeClick);
            SecondWarningPreviewMouseUpCommand = new DelegateCommand(SecondWarningTimeClick);
            WebOrderTimePreviewMouseUpCommand = new DelegateCommand(WebOrderTimeClick);
            this.DataContext = this;
        }

        #region Commands
        public ICommand TestConnectionCommand { get; set; }
        public ICommand SaveCommand { get; set; }
        public ICommand CloseCommand { get; set; }
        public ICommand DatabasePathPreviewMouseUpCommand { get; set; }
        public ICommand DatabaseAddressPreviewMouseUpCommand { get; set; }
        public ICommand TerminalNamePreviewMouseUpCommand { get; set; }
        public ICommand TerminalAddressPreviewMouseUpCommand { get; set; }
        public ICommand FirstWarningPreviewMouseUpCommand { get; set; }
        public ICommand SecondWarningPreviewMouseUpCommand { get; set; }
        public ICommand WebOrderTimePreviewMouseUpCommand { get; set; }
        #endregion

        #region Properties
        public Settings CurrentSettings
        {
            get
            {
                return _currentSettings;
            }
            set
            {
                _currentSettings = value;
                OnPropertyChanged("CurrentSettings");
            }
        }
        public double SliderValue
        {
            get { return _sliderValue; }
            set
            {
                _sliderValue = value;
                SoundPlayerUtilities.Instance.SetSystemVolume(Convert.ToInt32(value));
                OnPropertyChanged("SliderValue");
            }
        }
        public ObservableCollection<string> SoundList
        {
            get { return _soundList; }
            set
            {
                _soundList = value;
                OnPropertyChanged("SoundList");
            }
        }
        public ObservableCollection<Terminal> Terminals
        {
            get { return _terminals; }
            set
            {
                _terminals = value;
                OnPropertyChanged("Terminals");
            }
        }
        public ICommand TestSoundCommand { get; set; }
        public Terminal SelectedTerminal
        {
            get { return _selectedTerminal; }
            set
            {
                _selectedTerminal = value;
                OnPropertyChanged("SelectedTerminal");
                SetOutputType();
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
        public ObservableCollection<int> FontSizes
        {
            get { return _fontSizes; }
            set
            {
                _fontSizes = value;
                OnPropertyChanged("FontSizes");
            }
        }
        public ObservableCollection<string> FontFamiles
        {
            get { return _fontFamilies; }
            set
            {
                _fontFamilies = value;
                OnPropertyChanged("FontFamiles");
            }
        }

        #endregion

        #region Command Handlers
        private void DatabaseAddreddMouseUp(object sender)
        {
            CurrentSettings.DbIpAddress = KeyboardController.Instance.OpenNumPad(CurrentSettings.DbIpAddress, NumpadMode.IpAddress);
        }
        private void DatabasePathPreviewMouseUp(object sender)
        {
            CurrentSettings.DbPath = KeyboardController.Instance.OpenKeyBoard("Enter database path", CurrentSettings.DbPath);
        }
        private void FirstWarningTimeClick(object sender)
        {
            string firstWarningTime = KeyboardController.Instance.OpenNumPad(Convert.ToString(CurrentSettings.FirstWarningTime), NumpadMode.Numeric);
            int time = 0;
            if (int.TryParse(firstWarningTime, out time))
            {
                CurrentSettings.FirstWarningTime = time;
            }
        }
        private void SecondWarningTimeClick(object sender)
        {
            string secondWarningTime = KeyboardController.Instance.OpenNumPad(Convert.ToString(CurrentSettings.SecondWarningTime), NumpadMode.Numeric);
            int time = 0;
            if (int.TryParse(secondWarningTime, out time))
            {
                CurrentSettings.SecondWarningTime = time;
            }
        }
        private void WebOrderTimeClick(object sender)
        {
            string webOrderTime = KeyboardController.Instance.OpenNumPad(Convert.ToString(CurrentSettings.WebOrderTime), NumpadMode.Numeric);
            int time = 0;
            if (int.TryParse(webOrderTime, out time))
            {
                CurrentSettings.WebOrderTime = time;
            }
        }
        private void TerminalAddressClick(object sender)
        {
            CurrentSettings.TerminalIpAddress = KeyboardController.Instance.OpenNumPad(CurrentSettings.TerminalIpAddress, NumpadMode.IpAddress);
        }
        private void TerminalNameClick(object sender)
        {
            CurrentSettings.DisplayName = KeyboardController.Instance.OpenKeyBoard("Enter display name for terminal",
                CurrentSettings.DisplayName);
        }
        private void TestConnectionHandler(object sender)
        {
            if (CheckDataBase())
            {
                ChefmateController.Instance.ShowMessageBox("Chefmate", "Connection Successful!");
            }
            else
            {
                ChefmateController.Instance.ShowMessageBox("Chefmate", "Connection Failed!");
            }
        }
        private void SaveCommandHandler(object sender)
        {
            if (DoValidations())
            {
                ChefmateController.Instance.CurrentSettings = CurrentSettings.Clone() as Settings;
                Close();
            }
        }
        private void CloseCommandHandler(object sender)
        {
            Close();
        }
        #endregion

        #region Private Method
        private bool DoValidations()
        {
            ValidationError = "";
            if (string.IsNullOrWhiteSpace(CurrentSettings.DisplayName))
            {
                ValidationError = "Please Enter a display name for terminal.";
                return false;
            }

            if (string.IsNullOrWhiteSpace(CurrentSettings.TerminalIpAddress) || !ChefmateUtility.IsValidIpAddress(CurrentSettings.TerminalIpAddress))
            {
                ValidationError = "Please Enter a valid ip address for terminal.";
                return false;
            }
            if (string.IsNullOrWhiteSpace(CurrentSettings.DbPath))
            {
                ValidationError = "Please Enter a path for database.";
                return false;
            }

            if (string.IsNullOrWhiteSpace(CurrentSettings.TerminalIpAddress) || !ChefmateUtility.IsValidIpAddress(CurrentSettings.DbIpAddress))
            {
                ValidationError = "Please Enter a valid ip address for databse.";
                return false;
            }
            if (CurrentSettings.SecondWarningTime == 0)
            {
                ValidationError = "Second warning time should be greater than 0.";
                return false;
            }
            if (CurrentSettings.FirstWarningTime == 0)
            {
                ValidationError = "First warning time should be greater than 0.";
                return false;
            }
            if (CurrentSettings.FirstWarningTime >= CurrentSettings.SecondWarningTime)
            {
                ValidationError = "First warning time can't be greater than or equal to second warning time.";
                return false;
            }
            if (CurrentSettings.WebOrderTime == 0)
            {
                ValidationError = "Web Order time should be greater than 0.";
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
        private void SetOutputType()
        {
            if (_selectedTerminal != null)
            {
                CurrentSettings.OutputTerminal = _selectedTerminal.TerminalId;
                if (CurrentSettings.OutputTerminal == -1)
                    CurrentSettings.OutputType = OutputType.Printer;
                else if (CurrentSettings.OutputTerminal == 0)
                    CurrentSettings.OutputType = OutputType.None;
                else
                {
                    if (_selectedTerminal.TerminalType == TerminalType.Kitchen)
                        CurrentSettings.OutputType = OutputType.KitchenTerminal;
                    else
                        CurrentSettings.OutputType = OutputType.RunnerTerminal;
                }
            }
            else
            {
                CurrentSettings.OutputTerminal = 0;
                CurrentSettings.OutputType = OutputType.None;
            }

        }
        private bool CheckDataBase()
        {
            if (!ChefmateUtility.TestConnection(CurrentSettings.DbIpAddress, CurrentSettings.DbPath))
            {
                return false;
            }
            return true;
        }
        private void LoadSoundNames()
        {
            var soundList = SoundPlayerUtilities.Instance.GetSoundList();
            soundList.ToList().ForEach(s => SoundList.Add(s));
        }
        private void LoadTerminals()
        {
            var terminals = DbTerminal.GetAllTerminals();
            _terminals.Add(new Terminal() { TerminalId = 0, DisplayName = "None" });
            _terminals.Add(new Terminal() { TerminalId = -1, DisplayName = "Printer" });
            terminals.ToList().ForEach(s =>
            {
                if (s.TerminalId != ChefmateController.Instance.CurrenTerminal.TerminalId)
                    _terminals.Add(s);
            });
        }
        private void LoadFontSizes()
        {
            FontSizes = new ObservableCollection<int>() { 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 22, 24 };
        }
        private void LoadFontFamilies()
        {
            FontFamiles = new ObservableCollection<string>();
            InstalledFontCollection installedFontCollection = new InstalledFontCollection();
            var fontFamilies = installedFontCollection.Families;
            fontFamilies.ToList().ForEach(s => FontFamiles.Add(s.Name));
        }
        private void Initialize()
        {
            _terminals = new ObservableCollection<Terminal>();
            TestSoundCommand = new DelegateCommand(PlaySound);
            SoundList = new ObservableCollection<string>();
            SliderValue = SoundPlayerUtilities.Instance.GetSystemVolume();
            LoadSoundNames();
            LoadTerminals();
            LoadFontSizes();
            LoadFontFamilies();
            SelectedTerminal = Terminals.FirstOrDefault(s => s.TerminalId == CurrentSettings.OutputTerminal);
        }
        private void PlaySound(object param)
        {
            var soundName = Convert.ToString(param);
            SoundPlayerUtilities.Instance.PlaySound(soundName);
        }
        #endregion

        #region Property Changed

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged(string propertyName = null)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion


    }
}
