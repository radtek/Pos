using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;
using Chefmate.Core.Commands;
using Chefmate.Core.Enums;
using Chefmate.Core.Model;
using Chefmate.Database.DbModels;
using Chefmate.Infrastructure.Controller;
using Chefmate.UI.Annotations;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for TransferView.xaml
    /// </summary>
    public partial class TransferView : Window, INotifyPropertyChanged
    {
        private ObservableCollection<Terminal> _terminals;
        private Terminal _selectedTerminal;
        public TransferView()
        {
            InitializeComponent();
            GetTerminals();
            CloseCommand = new DelegateCommand(CloseCommandHamdler);
            this.DataContext = this;
        }
        public Order Order { get; set; }
        public ICommand CloseCommand { get; set; }
        public ObservableCollection<Terminal> Terminals
        {
            get { return _terminals; }
            set
            {
                _terminals = value;
                OnPropertyChanged("Terminals");
            }
        }
        public bool ModalResul { get; set; }
        public Terminal SelectedTerminal
        {
            get { return _selectedTerminal; }
            set
            {
                _selectedTerminal = value;
                OnPropertyChanged("SelectedTerminal");
            }
        }
        private void GetTerminals()
        {
            var terminals = DbTerminal.GetAllTerminals();
            Terminals = new ObservableCollection<Terminal>();
            foreach (var terminal in terminals)
            {
                if (terminal.TerminalId != ChefmateController.Instance.CurrenTerminal.TerminalId && terminal.TerminalType != TerminalType.Runner)
                    Terminals.Add(terminal);
            }
        }
        private void CloseCommandHamdler(object param)
        {
            var command = Convert.ToString(param);
            if (command == "Transfer")
            {
                if (_selectedTerminal == null)
                {
                    ChefmateController.Instance.ShowMessageBox("Warning", "Please select a terminal.");
                    return;
                }
                ModalResul = true;
            }
            this.Close();
        }

        #region PropertyChanged
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
