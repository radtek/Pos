using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;
using Chefmate.Core.Commands;
using Chefmate.Core.Enums;
using Chefmate.UI.Annotations;

namespace Chefmate.UI.KeyBoards
{


    /// <summary>
    /// Interaction logic for NumKeyboard.xaml
    /// </summary>
    public partial class NumKeyboard : Window, INotifyPropertyChanged
    {
        private ObservableCollection<char> _numericCharacters;
        private NumpadMode _numpadMode;
        public NumKeyboard()
        {
            InitializeComponent();
            ButtonPressCommand = new DelegateCommand(ButtonPressCommandHandler);
            NumericCharacters = new ObservableCollection<char>() { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.' };
            this.DataContext = this;
        }
        public ICommand ButtonPressCommand { get; set; }
        public ObservableCollection<char> NumericCharacters
        {
            get { return _numericCharacters; }
            set
            {
                _numericCharacters = value;
                OnPropertyChanged("NumericCharacters");
            }
        }
        public bool ModalResult { get; set; }
        public void SetMode(NumpadMode numpadMode)
        {
            _numpadMode = numpadMode;
            if (_numpadMode == NumpadMode.Numeric)
            {
                KeyboardTextBox.MaxLength = 4;
                DecimalButton.Visibility = Visibility.Hidden;
            }
            else
            {
                KeyboardTextBox.MaxLength = 15;
                DecimalButton.Visibility = Visibility.Visible;
            }
        }
        public void InitializeParameters(string keybordText)
        {
            KeyboardTextBox.Text = keybordText;
            if (!string.IsNullOrWhiteSpace(keybordText))
                KeyboardTextBox.Select(keybordText.Length + 1, 0);
        }
        public string GetResult()
        {
            return KeyboardTextBox.Text;
        }
        private void ButtonPressCommandHandler(object obj)
        {
            var character = Convert.ToString(obj);
            switch (character)
            {
                case "Ok":
                    ModalResult = true;
                    Close();
                    break;
                case "Clear":
                    KeyboardTextBox.Text = "";
                    break;
                case "Cancel":
                    ModalResult = false;
                    Close();
                    break;
                case "Back":
                    Backspace();
                    break;
                default:
                    AddCharacter(character);
                    break;
            }
            KeyboardTextBox.Focus();
        }
        private void AddCharacter(string character)
        {
            if (KeyboardTextBox.Text.Length < KeyboardTextBox.MaxLength)
            {
                int caretPosition = KeyboardTextBox.SelectionStart;
                int selectionLength = KeyboardTextBox.SelectionLength;
                var keybordText = KeyboardTextBox.Text;
                keybordText = keybordText.Remove(caretPosition, selectionLength);
                keybordText = keybordText.Insert(caretPosition, character);
                if (MatchText(keybordText))
                {
                    KeyboardTextBox.Text = keybordText;
                    KeyboardTextBox.Select(caretPosition + 1, 0);
                }
            }
        }
        private bool MatchText(string text)
        {
            var retVal = true;
            switch (_numpadMode)
            {
                case NumpadMode.Numeric:
                    Regex regex = new Regex(@"^[\d]+$");
                    retVal = regex.IsMatch(text);
                    break;
                case NumpadMode.IpAddress:
                    Regex ipRegex = new Regex(@"^-*[0-9,\.]+$");
                    retVal = ipRegex.IsMatch(text);
                    break;
            }
            return retVal;
        }
        private void Backspace()
        {
            int caretPosition = KeyboardTextBox.SelectionStart;
            int selectionLength = KeyboardTextBox.SelectionLength;
            if (selectionLength > 0)
            {
                KeyboardTextBox.Text = KeyboardTextBox.Text.Remove(caretPosition, selectionLength);
                KeyboardTextBox.Select(caretPosition, 0);
            }
            else if (caretPosition > 0)
            {
                KeyboardTextBox.Text = KeyboardTextBox.Text.Remove(caretPosition - 1, 1);
                KeyboardTextBox.Select(caretPosition - 1, 0);
            }
        }
        private void KeyboardTextBox_OnPreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !MatchText(e.Text);
        }

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }



    }
}
