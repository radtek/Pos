using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Input;
using Chefmate.Core.Commands;
using Chefmate.Core.Model;
using Chefmate.Database.DbModels;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for RecallOrderView.xaml
    /// </summary>
    public partial class RecallOrderView : Window, INotifyPropertyChanged
    {
        private ObservableCollection<Order> _orders;
        private Order _selectedOrder;
        private double _windowWidth;
        private string _statusText;
        public RecallOrderView()
        {
            InitializeComponent();
            RecallCommand = new DelegateCommand(RecallCommandHandler);
            CloseCommand = new DelegateCommand(CloseCommandHandler);
            this.Loaded += RecallOrderView_Loaded;
            WindowWidth = Screen.PrimaryScreen.Bounds.Width * .8;
            this.DataContext = this;
        }



        public ObservableCollection<Order> Orders
        {
            get { return _orders; }
            set
            {
                _orders = value;
                OnPropertyChanged("Orders");
            }
        }
        public double WindowWidth
        {
            get { return _windowWidth; }
            set
            {
                _windowWidth = value;
                OnPropertyChanged("WindowWidth");
            }
        }
        public Order SelectedOrder
        {
            get { return _selectedOrder; }
            set
            {
                _selectedOrder = value;
                OnPropertyChanged("SelectedOrder");
            }
        }
        public bool ModalResult { get; set; }
        public string StatusText
        {
            get { return _statusText; }
            set
            {
                _statusText = value;
                OnPropertyChanged("StatusText");
            }
        }
        public ICommand RecallCommand { get; set; }
        public ICommand CloseCommand { get; set; }
        private void RecallCommandHandler(object sender)
        {
            ModalResult = true;
            Close();
        }
        private void CloseCommandHandler(object sender)
        {
            ModalResult = false;
            Close();
        }
        private void RecallOrderView_Loaded(object sender, RoutedEventArgs e)
        {
            this.IsEnabled = false;
            StatusText = "Loading Orders......";
            this.Dispatcher.BeginInvoke(new Action(() =>
            {
                Orders = DbOrder.GetBumpedOrders();
                this.IsEnabled = true;
                StatusText = "";
            }));

        }

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
