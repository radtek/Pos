using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Chefmate.Core;
using Chefmate.Core.Commands;
using Chefmate.Core.Extensions;
using Chefmate.UI.Annotations;

namespace Chefmate.UI.KeyBoards
{
    /// <summary>
    /// Interaction logic for TextKeyBoard.xaml
    /// </summary>
    public partial class TextKeyBoard : Window, INotifyPropertyChanged
    {
        private ObservableCollection<char> _firstRowCharacters;
        private ObservableCollection<char> _secondRowCharacters;
        private ObservableCollection<char> _thirdRowCharacters;
        private ObservableCollection<char> _fourthRowCharacters;
        private bool _isShiftPressed;
        private bool _isCapsOn;
        private string _headerText;
        public TextKeyBoard()
        {
            InitializeComponent();
            InitializeKeyBoard();
            LoadRows();
            this.DataContext = this;
        }


        public ICommand ButtonPressCommand { get; set; }
        public ObservableCollection<char> FirstRowCharacters
        {
            get { return _firstRowCharacters; }
            set
            {
                _firstRowCharacters = value;
                OnPropertyChanged("FirstRowCharacters");
            }
        }
        public ObservableCollection<char> SecondRowCharacters
        {
            get { return _secondRowCharacters; }
            set
            {
                _secondRowCharacters = value;
                OnPropertyChanged("SecondRowCharacters");
            }
        }
        public ObservableCollection<char> ThirdRowCharacters
        {
            get { return _thirdRowCharacters; }
            set
            {
                _thirdRowCharacters = value;
                OnPropertyChanged("ThirdRowCharacters");
            }
        }
        public ObservableCollection<char> FourthRowCharacters
        {
            get { return _fourthRowCharacters; }
            set
            {
                _fourthRowCharacters = value;
                OnPropertyChanged("FourthRowCharacters");
            }
        }
        public bool ModalResult { get; set; }
        public string HeaderText
        {
            get { return _headerText; }
            set
            {
                _headerText = value;
                OnPropertyChanged("HeaderText");
            }
        }
        public void InitializeParameters(string header, string keybordText)
        {
            HeaderText = string.IsNullOrWhiteSpace(header) ? "Keybord" : header;
            KeyboardTextBox.Text = keybordText;
            if (!string.IsNullOrWhiteSpace(keybordText))
                KeyboardTextBox.Select(keybordText.Length + 1, 0);
        }
        public string GetResult()
        {
            return KeyboardTextBox.Text;
        }
        private void InitializeKeyBoard()
        {
            ButtonPressCommand = new DelegateCommand(ButtonPressCommandHandler);
            FirstRowCharacters = new ObservableCollection<char>();
            SecondRowCharacters = new ObservableCollection<char>();
            ThirdRowCharacters = new ObservableCollection<char>();
            FourthRowCharacters = new ObservableCollection<char>();
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
                case "Space":
                    AddCharacter(" ");
                    break;
                case "Tab":
                    AddCharacter("\t");
                    break;
                case "Enter":
                    AddCharacter("\n");
                    break;
                case "Shift":
                    Shift();
                    break;
                case "Caps":
                    CapsLock();
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
            if (character == "&&")
            {
                character = "&";
            }

            int caretPosition = KeyboardTextBox.SelectionStart;
            int selectionLength = KeyboardTextBox.SelectionLength;

            KeyboardTextBox.Text = KeyboardTextBox.Text.Remove(caretPosition, selectionLength);
            KeyboardTextBox.Text = KeyboardTextBox.Text.Insert(caretPosition, character);
            KeyboardTextBox.Select(caretPosition + 1, 0);
            Shift();
        }
        private void LoadRows()
        {
            if (_isShiftPressed)
            {
                FirstRowCharacters = ChefmateConstants.FirstRowWithShift.ToList().ToObservableCollection();
                if (_isCapsOn)
                {
                    SecondRowCharacters = ChefmateConstants.SecondRowWithShiftCapsOn.ToList().ToObservableCollection();
                    ThirdRowCharacters = ChefmateConstants.ThirdRowWithShiftCapsOn.ToList().ToObservableCollection();
                    FourthRowCharacters = ChefmateConstants.FourthRowWithShiftCapsOn.ToList().ToObservableCollection();
                }
                else
                {
                    SecondRowCharacters = ChefmateConstants.SecondRowWithShiftCapsOff.ToList().ToObservableCollection();
                    ThirdRowCharacters = ChefmateConstants.ThirdRowWithShiftCapsOff.ToList().ToObservableCollection();
                    FourthRowCharacters = ChefmateConstants.FourthRowWithShiftCapsOff.ToList().ToObservableCollection();
                }
            }
            else
            {
                FirstRowCharacters = ChefmateConstants.FirstRowWithoutShift.ToList().ToObservableCollection();
                if (_isCapsOn)
                {
                    SecondRowCharacters = ChefmateConstants.SecondRowWithoutShiftCapsOn.ToList().ToObservableCollection();
                    ThirdRowCharacters = ChefmateConstants.ThirdRowWithoutShiftCapsOn.ToList().ToObservableCollection();
                    FourthRowCharacters = ChefmateConstants.FourthRowWithoutShiftCapsOn.ToList().ToObservableCollection();
                }
                else
                {
                    SecondRowCharacters = ChefmateConstants.SecondRowWithoutShiftCapsOff.ToList().ToObservableCollection();
                    ThirdRowCharacters = ChefmateConstants.ThirdRowWithoutShiftCapsOff.ToList().ToObservableCollection();
                    FourthRowCharacters = ChefmateConstants.FourthRowWithoutShiftCapsOff.ToList().ToObservableCollection();
                }
            }

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
            Shift();
        }
        private void Shift()
        {
            _isShiftPressed = !_isShiftPressed;
            LoadRows();
        }
        private void CapsLock()
        {
            _isCapsOn = !_isCapsOn;
            LoadRows();
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
