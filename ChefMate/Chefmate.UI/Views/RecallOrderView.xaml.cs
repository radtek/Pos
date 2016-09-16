using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Input;
using Chefmate.Core.Commands;
using Chefmate.Core.Extensions;
using Chefmate.Core.Model;
using Chefmate.Database.DbModels;
using Chefmate.Infrastructure.Controller;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for RecallOrderView.xaml
    /// </summary>
    public partial class RecallOrderView : Window, INotifyPropertyChanged
    {
        private ObservableCollection<Order> _orders;
        private ObservableCollection<Item> _selectedOrderItems;
        private Order _selectedOrder;
        private double _windowWidth;
        private string _statusText;
        private int _skipCount;
        private bool _canLoadMoreOrders;
        public RecallOrderView()
        {
            InitializeComponent();
            RecallCommand = new DelegateCommand(RecallCommandHandler);
            CloseCommand = new DelegateCommand(CloseCommandHandler);
            LoadCommand = new DelegateCommand(LoadOrdersCommandHandler);
            this.Loaded += RecallOrderView_Loaded;
            WindowWidth = Screen.PrimaryScreen.Bounds.Width * .8;
            _skipCount = 0;
            Orders = new ObservableCollection<Order>();
            this.DataContext = this;
        }
        public bool CanLoadMoreOrders
        {
            get { return _canLoadMoreOrders; }
            set
            {
                _canLoadMoreOrders = value;
                OnPropertyChanged("CanLoadMoreOrders");
            }
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
                SelectedOrderChangedHandler();
            }
        }

        public ObservableCollection<Item> SelectedOrderItems
        {
            get { return _selectedOrderItems; }
            set
            {
                _selectedOrderItems = value;
                OnPropertyChanged("SelectedOrderItems");
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
        public ICommand LoadCommand { get; set; }

        private void SelectedOrderChangedHandler()
        {
            if (_selectedOrder != null)
            {
                this.IsEnabled = false;
                StatusText = "Loading Items......";
                var order = DbOrder.GetOrder(_selectedOrder.OrderKey, 0, true);
                SelectedOrderItems = order.Items;
                this.IsEnabled = true;
                StatusText = "";
            }
        }
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
        private void LoadOrdersCommandHandler(object sender)
        {
            this.IsEnabled = false;
            StatusText = "Loading Orders......";
            this.Dispatcher.BeginInvoke(new Action(() =>
            {
                bool canLoadMoreOrders = false;
                var orders = DbOrder.GetBumpedOrders(_skipCount, ChefmateController.Instance.CurrentSettings.RecallCount, ref canLoadMoreOrders);
                CanLoadMoreOrders = canLoadMoreOrders;
                _skipCount += ChefmateController.Instance.CurrentSettings.RecallCount;
                Orders.AddRange(orders);
                this.IsEnabled = true;
                StatusText = "";
            }));
        }
        private void RecallOrderView_Loaded(object sender, RoutedEventArgs e)
        {
            LoadOrdersCommandHandler(sender);
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
