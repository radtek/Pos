using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;
using Chefmate.Communication.EventArguments;
using Chefmate.Core;
using Chefmate.Core.Commands;
using Chefmate.Core.Enums;
using Chefmate.Core.Extensions;
using Chefmate.Core.Model;
using Chefmate.Infrastructure.Builder;
using Chefmate.Infrastructure.Controller;
using Chefmate.Infrastructure.Utility;
using Chefmate.Logger;
using Chefmate.UI.Controller;
using ChefMate.Database;
using Application = System.Windows.Application;
using KeyEventArgs = System.Windows.Input.KeyEventArgs;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private double _orderHeight;
        private double _orderWidth;
        private string _chitTableNumber;
        private DispatcherTimer _orderTimer;
        private double _horizontalScale;
        private bool _isRecallEnabled;
        private string _lastStatus;
        private int _lastSelectedIndex = -1;
        public MainWindow()
        {
            InitializeComponent();
            Initialize();
            InitializeChefmate();
            InitializeTimer();
            _chitTableNumber = "";
            this.SizeChanged += MainWindow_SizeChanged;
            this.Activated += MainWindow_Activated;
            this.PreviewKeyUp += MainWindow_PreviewKeyUp;
            this.Loaded += MainWindow_Loaded;
            FileSystemInitializer.Instance.FileCreatedEvent += Instance_FileCreatedEvent;
            Builder.Instance.DatabaseOrderReceivedEvent += Instance_DatabaseOrderReceivedEvent;
            ChefmateController.Instance.OrderArrivedEvent += OrderArrivedEventHandler;
            ChefmateController.Instance.NavigationUpdateEvent += UpdateNavigationButtonDisplay;
            ChefmateController.Instance.StatusChangeEvent += StatusChangeEventHandler;
            if (ChefmateController.Instance.CurrentSettings != null)
                IsRecallEnabled = ChefmateController.Instance.CurrentSettings.TerminalType == TerminalType.Kitchen;
            this.DataContext = this;

        }

       


        #region Start up
        private void InitializeChefmate()
        {
            try
            {
                if (!Properties.Settings.Default.TerminalInitialized)
                {
                    InitializeTerminal();
                }
                else
                {
                    bool result = DatabaseCore.Instance.InitializeDatabae(Properties.Settings.Default.DatabaseAddress, Properties.Settings.Default.DatabasePath);
                    if (result)
                    {
                        CheckVersionInformation();
                        bool terminalExist = ChefmateController.Instance.LoadTerminal(Environment.MachineName);
                        if (!terminalExist)
                        {
                            InitializeTerminal();
                        }
                    }
                    else
                    {
                        InitializeTerminal();
                    }
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("InitializeChefmate", ex.Message);
            }

        }
        private void InitializeTerminal()
        {
            try
            {
                StartupView startupView = new StartupView();
                startupView.ShowInTaskbar = false;
                startupView.Topmost = true;
                startupView.ShowDialog();
                if (startupView.ModalResult == System.Windows.Forms.DialogResult.Cancel)
                {
                    Application.Current.Shutdown();
                }
                else
                {
                    Properties.Settings.Default.DatabaseAddress = startupView.DbIpAddress;
                    Properties.Settings.Default.DatabasePath = startupView.DbPath;
                    Properties.Settings.Default.TerminalInitialized = true;
                    DatabaseCore.Instance.InitializeDatabae(Properties.Settings.Default.DatabaseAddress, Properties.Settings.Default.DatabasePath);
                    ChefmateController.Instance.AddTerminal(startupView.DisplayName, startupView.TerminalIpAddress, startupView.DbIpAddress, startupView.DbPath);
                    Properties.Settings.Default.Save();
                    CheckVersionInformation();
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("InitializeTerminal", ex.Message);
            }
        }
        private void CheckVersionInformation()
        {
            try
            {
                string versionErrorMessage = "";
                var versionMode = VersionChecker.Instance.CheckVersionInformation(ChefmateConstants.DatabaseVersion, ref versionErrorMessage);
                if (versionMode == DatabaseMode.Correct)
                {
                    ChefmateInitializer.Start();
                }
                else
                {
                    ChefmateController.Instance.ShowMessageBox("Version Error", versionErrorMessage);
                    var chefmateParser = new ChefmateParserView();
                    chefmateParser.ShowInTaskbar = false;
                    chefmateParser.Topmost = true;
                    chefmateParser.ShowDialog();
                    Application.Current.Shutdown();
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("CheckVersionInformation", ex.Message);
            }
        }
        private void LoadAllOrders()
        {
            try
            {
                this.Dispatcher.BeginInvoke(new Action(() =>
                {
                    LastStatus = "Loading orders.....";
                    ChefmateController.Instance.LoadAllOrders();
                    ChefmateController.Instance.ManageOrderDisplay();
                    UpdateNavigationButtonDisplay();
                    AnalyticalData.TotalOrdersCount += TotalOrders.Count;
                    AnalyticalData.CurrentOrdersCount += TotalOrders.Count;
                    TotalOrders.ForEach(s => { AnalyticalData.CurrentItems += s.Items.Count; });
                    UpdateOrderInfoDisplay();
                    this.IsEnabled = true;
                    LastStatus = "Chefmate Started Successfully!";
                }));
            }
            catch (Exception ex)
            {
                this.IsEnabled = true;
                LastStatus = "Failed to Load Orders.";
                ChefmateLogger.Instance.LogError("LoadAllOrders", ex.Message);
            }
        }
        private void Initialize()
        {
            AnalyticalData = new AnalyticalData();
            PopUpCommand = new DelegateCommand(ExceutePopUp);
            NavigateBackwardCommand = new DelegateCommand(Backward);
            NavigateForwardCommand = new DelegateCommand(Forward);
            CallAwayStopCommand = new DelegateCommand(StopCallAway);
            CurrentDisplayOrders = new ObservableCollection<Order>();
            TotalOrders = new ObservableCollection<Order>();
            this.IsEnabled = false;
        }
        #endregion

        #region Commands
        public ICommand PopUpCommand { get; set; }
        public ICommand NavigateForwardCommand { get; set; }
        public ICommand NavigateBackwardCommand { get; set; }
        public ICommand CallAwayStopCommand { get; set; }
        #endregion

        #region Properties
        public ObservableCollection<Order> TotalOrders
        {
            get
            {
                return ChefmateController.Instance.TotalOrders;
            }
            set
            {
                ChefmateController.Instance.TotalOrders = value;
                OnPropertyChanged("TotalOrders");
            }
        }
        public ObservableCollection<Order> CurrentDisplayOrders
        {
            get
            {
                return ChefmateController.Instance.CurrentDisplayOrders;
            }
            set
            {
                ChefmateController.Instance.CurrentDisplayOrders = value;
                OnPropertyChanged("CurrentDisplayOrders");
            }
        }
        public AnalyticalData AnalyticalData
        {
            get
            {
                return ChefmateController.Instance.AnalyticalData;
            }
            set
            {
                ChefmateController.Instance.AnalyticalData = value;
                OnPropertyChanged("AnalyticalData");
            }
        }
        public string LastStatus
        {
            get { return _lastStatus; }
            set
            {
                _lastStatus = value;
                OnPropertyChanged("LastStatus");
            }
        }
        public double OrderHeight
        {
            get { return _orderHeight; }
            set
            {
                _orderHeight = value;
                OnPropertyChanged("OrderHeight");
            }
        }
        public double OrderWidth
        {
            get { return _orderWidth; }
            set
            {
                _orderWidth = value;
                OnPropertyChanged("OrderWidth");
            }
        }
        public int PageIndex
        {
            get { return ChefmateController.Instance.PageIndex; }
            set { ChefmateController.Instance.PageIndex = value; }
        }
        public double HorizontalScale
        {
            get { return _horizontalScale; }
            set
            {
                _horizontalScale = value;
                OnPropertyChanged("HorizontalScale");
            }
        }
        public bool IsRecallEnabled
        {
            get { return _isRecallEnabled; }
            set
            {
                _isRecallEnabled = value;
                OnPropertyChanged("IsRecallEnabled");
            }
        }
        #endregion

        #region EventHandles
        private void StopCallAway(object obj)
        {
            var group = obj as Group;
            if (group != null)
            {
                group.DisplayAttributes.IsBlinkingEnable = false;
            }
        }
        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            LoadAllOrders();
            StartTimer();
        }
        private void Instance_DatabaseOrderReceivedEvent(DbOrderReceivedEventArgs eventArgs)
        {
            this.Dispatcher.Invoke(new Action(() =>
            {
                Builder.Instance.BuildOrderFromDatabase(eventArgs);
            }));

        }
        private void Instance_FileCreatedEvent(string fileUri)
        {
            this.Dispatcher.Invoke(new Action(() =>
            {
                FileSystemInitializer.Instance.SupplyFile(fileUri);
            }));
        }
        private void MainWindow_PreviewKeyUp(object sender, KeyEventArgs e)
        {
            this.Focus();
            if (ChefmateController.Instance.CurrentSettings.KeypadOperation == KeypadOperation.BumpByLocation)
                BumpByLoacation(e.Key);
            else
                BumpByChitTableNumber(e.Key);

        }
        private void ExceutePopUp(object param)
        {
            int orderNumber = Convert.ToInt32(param);
            var order = TotalOrders.FirstOrDefault(s => s.OrderNumber == orderNumber);
            OrderPopUp orderPopUp = new OrderPopUp
            {
                Order = order,
                ShowInTaskbar = false,
                Topmost = true
            };
            orderPopUp.ShowDialog();
            OrderContainer.Focus();
        }
        private void MainWindow_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            OrderHeight = (this.ActualHeight * .75) / ChefmateController.Instance.PageRows;
            if (OrderContainer.ActualWidth > 0)
            {
                OrderWidth = OrderContainer.ActualWidth / ChefmateController.Instance.PageColumns;
                HorizontalScale = OrderContainer.ActualWidth / (ChefmateController.Instance.PageColumns * OrderWidth);
            }
            this.SizeChanged -= MainWindow_SizeChanged;
        }
        private void MainWindow_Activated(object sender, EventArgs e)
        {
            OrderHeight = (this.ActualHeight * .75) / ChefmateController.Instance.PageRows;
            if (OrderContainer.ActualWidth > 0)
            {
                OrderWidth = OrderContainer.ActualWidth / ChefmateController.Instance.PageColumns;
                HorizontalScale = OrderContainer.ActualWidth / (ChefmateController.Instance.PageColumns * OrderWidth);
            }
            
        }
        private void CloseClick(object sender, RoutedEventArgs e)
        {
            var dialogResult = ChefmateController.Instance.ShowMessageBox("Chefmate", "Are you sure to quit ?", true);
            if (dialogResult)
                Application.Current.Shutdown();
        }
        private void Setting(object sender, RoutedEventArgs e)
        {
            var settingView = new SettingView { ShowInTaskbar = false, Topmost = true };
            settingView.ShowDialog();
            IsRecallEnabled = ChefmateController.Instance.CurrentSettings.TerminalType == TerminalType.Kitchen;
            Properties.Settings.Default.DatabasePath = ChefmateController.Instance.CurrentSettings.DbPath;
            Properties.Settings.Default.DatabaseAddress = ChefmateController.Instance.CurrentSettings.DbIpAddress;
            Properties.Settings.Default.Save();
            ChefmateController.Instance.SaveSettings();
            ChefmateController.Instance.ManageOrderDisplay();
            OrderContainer.Focus();
        }
        private void Recall(object sender, RoutedEventArgs e)
        {
            RecallOrderView recallOrderView = new RecallOrderView
            {
                ShowInTaskbar = false,
                Topmost = true
            };
            recallOrderView.ShowDialog();
            if (recallOrderView.ModalResult && recallOrderView.SelectedOrder != null)
            {
                ChefmateController.Instance.AddRecallOrder(recallOrderView.SelectedOrder);
            }
            OrderContainer.Focus();
        }
        private void StatusChangeEventHandler(string status)
        {
            this.Dispatcher.Invoke(new Action(() =>
            {
                LastStatus = status;
            }));
        }
        #endregion

        #region Navigations
        private void Backward(object sender)
        {
            if (PageIndex > 0)
            {
                var orderInPage = ChefmateController.Instance.PageRows * ChefmateController.Instance.PageColumns;
                PageIndex--;
                CurrentDisplayOrders.Clear();
                int remainingOrders = TotalOrders.Count - PageIndex * orderInPage;
                remainingOrders = remainingOrders >= orderInPage ? orderInPage : remainingOrders;
                var orders = TotalOrders.ToList().GetRange(PageIndex * orderInPage, remainingOrders);
                foreach (var order in orders)
                {
                    order.UpdateOrderInfoDisplay(ChefmateController.Instance.CurrentSettings.OrderInfoDisplay);
                    ChefmateController.Instance.ManageOrderGroups(order);
                    CurrentDisplayOrders.Add(order);
                }
            }

            UpdateNavigationButtonDisplay();

        }
        private void Forward(object sender)
        {
            var orderInPage = ChefmateController.Instance.PageRows * ChefmateController.Instance.PageColumns;
            if (TotalOrders.Count > (PageIndex + 1) * orderInPage)
            {
                PageIndex++;
                CurrentDisplayOrders.Clear();
                int remainingOrders = TotalOrders.Count - PageIndex * orderInPage;
                remainingOrders = remainingOrders >= orderInPage ? orderInPage : remainingOrders;
                var orders = TotalOrders.ToList().GetRange(PageIndex * orderInPage, remainingOrders);
                foreach (var order in orders)
                {
                    order.UpdateOrderInfoDisplay(ChefmateController.Instance.CurrentSettings.OrderInfoDisplay);
                    ChefmateController.Instance.ManageOrderGroups(order);
                    CurrentDisplayOrders.Add(order);
                }
            }
            UpdateNavigationButtonDisplay();
        }
        private void UpdateNavigationButtonDisplay()
        {
            this.Dispatcher.Invoke(new Action(() =>
            {
                var orderInPage = ChefmateController.Instance.PageRows * ChefmateController.Instance.PageColumns;
                BackwardNavigationButton.IsEnabled = PageIndex > 0;
                ForwardNavigationButton.IsEnabled = TotalOrders.Count > (PageIndex + 1) * orderInPage;
            }));

        }

        #endregion

        #region OrderDisplay
        private void OrderArrivedEventHandler(Order order)
        {
            Application.Current.Dispatcher.Invoke(new
                Action(() =>
                {
                    StopTimer();
                    ChefmateController.Instance.AddOrder(order);
                    StartTimer();
                }));
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

        #region Keypad Operations
        private void BumpByLoacation(Key pressedKey)
        {
            bool doBump = false;
            CurrentDisplayOrders.ToList().ForEach(s => s.DisplayAttributes.ExecutePopUp = false);
            switch (pressedKey)
            {
                case Key.D0:
                case Key.NumPad0:
                    Forward(null);
                    break;
                case Key.D1:
                case Key.NumPad1:
                    _lastSelectedIndex = 0;
                    break;
                case Key.D2:
                case Key.NumPad2:
                    _lastSelectedIndex = 1;
                    break;
                case Key.D3:
                case Key.NumPad3:
                    _lastSelectedIndex = 2;
                    break;
                case Key.D4:
                case Key.NumPad4:
                    _lastSelectedIndex = 3;
                    break;
                case Key.D5:
                case Key.NumPad5:
                    _lastSelectedIndex = 4;
                    break;
                case Key.D6:
                case Key.NumPad6:
                    _lastSelectedIndex = 5;
                    break;
                case Key.D7:
                case Key.NumPad7:
                    _lastSelectedIndex = 6;
                    break;
                case Key.D8:
                case Key.NumPad8:
                    _lastSelectedIndex = 7;
                    break;
                case Key.D9:
                case Key.NumPad9:
                    Backward(null);
                    break;
                case Key.Enter:
                    doBump = true;
                    break;
                case Key.Escape:
                    _lastSelectedIndex = -1;
                    this.Focus();
                    break;
            }
            if (_lastSelectedIndex > -1)
            {
                if (CurrentDisplayOrders.Count > _lastSelectedIndex)
                {
                    var selectedOrder = CurrentDisplayOrders[_lastSelectedIndex];
                    if (doBump)
                    {
                        OrderController.Instance.BumpOrder(selectedOrder);
                        _lastSelectedIndex = -1;
                    }
                    else
                        selectedOrder.DisplayAttributes.ExecutePopUp = true;
                }
            }
        }
        private void BumpByChitTableNumber(Key pressedKey)
        {
            switch (pressedKey)
            {
                case Key.D0:
                case Key.NumPad0:
                    _chitTableNumber += "0";
                    break;
                case Key.D1:
                case Key.NumPad1:
                    _chitTableNumber += "1";
                    break;
                case Key.D2:
                case Key.NumPad2:
                    _chitTableNumber += "2";
                    break;
                case Key.D3:
                case Key.NumPad3:
                    _chitTableNumber += "3";
                    break;
                case Key.D4:
                case Key.NumPad4:
                    _chitTableNumber += "4";
                    break;
                case Key.D5:
                case Key.NumPad5:
                    _chitTableNumber += "5";
                    break;
                case Key.D6:
                case Key.NumPad6:
                    _chitTableNumber += "6";
                    break;
                case Key.D7:
                case Key.NumPad7:
                    _chitTableNumber += "7";
                    break;
                case Key.D8:
                case Key.NumPad8:
                    _chitTableNumber += "8";
                    break;
                case Key.D9:
                case Key.NumPad9:
                    _chitTableNumber += "9";
                    break;
                case Key.Enter:
                    if (!OrderController.Instance.BumpByChitTableNumber(_chitTableNumber))
                    {
                        ChefmateController.Instance.ShowMessageBox("Error",
                            "Table/Chit Number does not exist on the current screen. Please press enter to continue.");
                    }
                    _chitTableNumber = "";
                    break;
                case Key.Escape:
                    _chitTableNumber = "";
                    break;
            }
        }
        #endregion region

        #region Timer
        private void InitializeTimer()
        {
            _orderTimer = new DispatcherTimer();
            _orderTimer.Interval = new TimeSpan(0, 0, 1);
            _orderTimer.Tick += OrderTimerOnTick;
        }
        private void OrderTimerOnTick(object sender, EventArgs eventArgs)
        {
            Application.Current.Dispatcher.Invoke(new Action(UpdateOrderInfoDisplay));
        }
        private void StartTimer()
        {
            _orderTimer.Start();
        }
        private void StopTimer()
        {
            _orderTimer.Stop();
            Thread.Sleep(1000);
        }
        private void UpdateOrderInfoDisplay()
        {
            lock (ChefmateController._syncRoot)
            {
                foreach (var order in CurrentDisplayOrders)
                {
                    TimeSpan elapsedTime = DateTime.Now - order.ArrivalTime;
                    order.DisplayAttributes.TimerDisplayText = elapsedTime.ToString(@"%h\:mm\:ss");
                    if (elapsedTime.TotalMinutes >= ChefmateController.Instance.CurrentSettings.SecondWarningTime)
                    {
                        order.DisplayAttributes.BackGroundColor = ChefmateConstants.SecondWarningOrderColor;
                        SetGroupsColor(order, ChefmateConstants.SecondWarningGroupColor);
                    }
                    else if (elapsedTime.TotalMinutes >= ChefmateController.Instance.CurrentSettings.FirstWarningTime)
                    {
                        order.DisplayAttributes.BackGroundColor = ChefmateConstants.FirstWarningOrderColor;
                        SetGroupsColor(order, ChefmateConstants.FirstWarningGroupColor);
                    }
                    else
                    {
                        order.DisplayAttributes.BackGroundColor = ChefmateConstants.NormalOrderColor;
                        SetGroupsColor(order, ChefmateConstants.NormalGroupColor);
                    }
                }
            }

        }
        private void SetGroupsColor(Order order, SolidColorBrush solidColorBrush)
        {
            order.ServingCourseGroups.ToList().ForEach(s => { SetGroupColor(s, solidColorBrush); });
            order.CourseGroups.ToList().ForEach(s => { SetGroupColor(s, solidColorBrush); });
        }
        private void SetGroupColor(Group group, SolidColorBrush solidColorBrush)
        {
            if (group.CalledAway && group.DisplayAttributes.IsBlinkingEnable)
            {
                if (group.DisplayAttributes.BackGroundColor != solidColorBrush)
                {
                    group.DisplayAttributes.BackGroundColor = solidColorBrush;
                }
                else
                {
                    group.DisplayAttributes.BackGroundColor = ChefmateConstants.LightRed;
                }
            }
            else
            {
                group.DisplayAttributes.BackGroundColor = solidColorBrush;
            }
        }
        #endregion


    }
}
