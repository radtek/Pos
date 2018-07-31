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
using System.Collections.Generic;
using System.Windows.Threading;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for SummaryView.xaml
    /// </summary>
    public partial class SummaryView : Window, INotifyPropertyChanged
    {
        private List<string> _itemList;
        private double _windowWidth;
        private string _statusText;
        private int _skipCount;
        private bool _canLoadMoreOrders;
        public SummaryView()
        {
            InitializeComponent();
            AutoUpdateCommand = new DelegateCommand(AutoUpdateCommandHandler);
            CloseCommand = new DelegateCommand(CloseCommandHandler);
            LoadCommand = new DelegateCommand(LoadOrdersCommandHandler);
            SetTimer();
            this.Loaded += SummaryView_Loaded;
            WindowWidth = Screen.PrimaryScreen.Bounds.Width * .8;
            _skipCount = 0;
            ItemList = new List<string>();
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
        public List<String> ItemList
        {
            get { return _itemList; }
            set
            {
                _itemList = value;
                OnPropertyChanged("ItemList");
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
        public ICommand AutoUpdateCommand { get; set; }
        public ICommand CloseCommand { get; set; }
        public ICommand LoadCommand { get; set; }

        private void AutoUpdateCommandHandler(object sender)
        {
            LoadOrdersCommandHandler(sender);
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
                ItemList = DbOrder.GetAccumulatedItems(ChefmateController.Instance.CurrenTerminal.TerminalId);
                ;
                McDataGrid.ItemsSource = ItemList;
                this.IsEnabled = true;
                StatusText = "";

            }));
        }

        private void SummaryView_Loaded(object sender, RoutedEventArgs e)
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

        private void SetTimer()
        {
            DispatcherTimer dispatcherTimer = new DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(AutoRefreshTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 30);
            dispatcherTimer.Start();
        }

        private void AutoRefreshTimer_Tick(object sender, EventArgs e)
        {
            LoadOrdersCommandHandler(sender);
        }
    }
}
