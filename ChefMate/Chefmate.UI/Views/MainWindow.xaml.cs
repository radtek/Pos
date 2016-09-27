using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
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
using Chefmate.Logger;
using Chefmate.UI.Controller;
using Chefmate.UI.UserControls;
using Application = System.Windows.Application;
using KeyEventArgs = System.Windows.Input.KeyEventArgs;
using Chefmate.Database.DbModels;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private double _orderWidth;
        private string _chitTableNumber;
        private DispatcherTimer _orderTimer;
        private double _horizontalScale;
        private bool _isRecallEnabled;
        private int _lastSelectedIndex = -1;
        private double _cordX;
        private double _cordY;
        private double _maxHeight;
        private GuiStyles _guiStyles;
        public MainWindow()
        {
            InitializeComponent();
            Initialize();
            InitializeTimer();
            _chitTableNumber = "";
            Subscribe();
            if (ChefmateController.Instance.CurrentSettings != null)
            {
                IsRecallEnabled = ChefmateController.Instance.CurrentSettings.TerminalType == TerminalType.Kitchen;
                SetFont();
            }
            this.DataContext = this;
        }

        #region Start up
        private void LoadAllOrders()
        {
            try
            {
                this.Dispatcher.BeginInvoke(new Action(() =>
                {
                    ChefmateController.Instance.LoadAllOrders();
                    RedrawOrders();
                    AnalyticalData.TotalOrdersCount += TotalOrders.Count;
                    AnalyticalData.CurrentOrdersCount += TotalOrders.Count;
                    TotalOrders.ForEach(s => { AnalyticalData.CurrentItems += s.Items.Count; });
                    UpdateOrderInfoDisplay();
                    this.IsEnabled = true;
                }));
            }
            catch (Exception ex)
            {
                this.IsEnabled = true;
                ChefmateLogger.Instance.LogError("LoadAllOrders", ex.Message);
            }
        }
        private void Initialize()
        {
            AnalyticalData = new AnalyticalData();
            NavigateBackwardCommand = new DelegateCommand(Backward);
            NavigateForwardCommand = new DelegateCommand(Forward);
            NavigateLeftCommand = new DelegateCommand(ScrollToLeft);
            NavigateRightCommand = new DelegateCommand(ScrollToRight);
            CurrentDisplayOrders = new ObservableCollection<Order>();
            TotalOrders = new ObservableCollection<Order>();
            WebOrders = new ObservableCollection<Order>();
            this.IsEnabled = false;
            GuiStyles = new GuiStyles();
        }
        #endregion

        #region Commands
        public ICommand NavigateForwardCommand { get; set; }
        public ICommand NavigateBackwardCommand { get; set; }
        public ICommand NavigateLeftCommand { get; set; }
        public ICommand NavigateRightCommand { get; set; }

        #endregion

        #region Properties

        public GuiStyles GuiStyles
        {
            get { return _guiStyles; }
            set
            {
                _guiStyles = value;
                OnPropertyChanged("GuiStyles");
            }
        }
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
        public double OrderWidth
        {
            get { return _orderWidth; }
            set
            {
                _orderWidth = value;
                OnPropertyChanged("OrderWidth");
            }
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
        public ObservableCollection<Order> WebOrders
        {
            get
            {
                return ChefmateController.Instance.WebOrders;
            }
            set
            {
                ChefmateController.Instance.WebOrders = value;
                OnPropertyChanged("WebOrders");
            }
        }
        #endregion

        #region EventHandles

        private void Subscribe()
        {
            this.SizeChanged += MainWindow_SizeChanged;
            this.Activated += MainWindow_Activated;
            this.PreviewKeyUp += MainWindow_PreviewKeyUp;
            this.Loaded += MainWindow_Loaded;
            FileSystemInitializer.Instance.FileCreatedEvent += Instance_FileCreatedEvent;
            Builder.Instance.DatabaseOrderReceivedEvent += Instance_DatabaseOrderReceivedEvent;
            ChefmateController.Instance.OrderArrivedEvent += OrderArrivedEventHandler;
            ChefmateController.Instance.AddOrderEvent += AddOrderEventHandler;
            ChefmateController.Instance.OrderRedrawEvent += OrderRedrawEvent;
            OrderController.Instance.OrderRedrawEvent += OrderRedrawEvent;
        }
        private void OrderRedrawEvent()
        {
            RedrawOrders();
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
            OrderContainer.Focus();
        }
        private void MainWindow_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (OrderContainer.ActualWidth > 0)
            {
                OrderWidth = this.ActualWidth / ChefmateController.Instance.PageColumns;
                HorizontalScale = this.ActualWidth / (ChefmateController.Instance.PageColumns * OrderWidth);

            }
            _maxHeight = OrderContainer.ActualHeight;
            this.SizeChanged -= MainWindow_SizeChanged;

        }
        private void MainWindow_Activated(object sender, EventArgs e)
        {
            if (OrderContainer.ActualWidth > 0)
            {
                OrderWidth = this.ActualWidth / ChefmateController.Instance.PageColumns;
                HorizontalScale = this.ActualWidth / (ChefmateController.Instance.PageColumns * OrderWidth);
            }
            _maxHeight = OrderContainer.ActualHeight;
            ParentScroller.Width = this.ActualWidth;
            OrderContainer.MinWidth = this.ActualWidth;
            OrderContainer.Focus();
        }
        private void CloseClick(object sender, RoutedEventArgs e)
        {
            var dialogResult = ChefmateController.Instance.ShowMessageBox("Chefmate", "Are you sure to quit ?", true);
            if (dialogResult)
                Application.Current.Shutdown();
        }
        private void Setting(object sender, RoutedEventArgs e)
        {
            var displayType = ChefmateController.Instance.CurrentSettings.GroupType;
            var settingView = new SettingView { ShowInTaskbar = false, Topmost = false };
            settingView.ShowDialog();
            IsRecallEnabled = ChefmateController.Instance.CurrentSettings.TerminalType == TerminalType.Kitchen;
            Properties.Settings.Default.DatabasePath = ChefmateController.Instance.CurrentSettings.DbPath;
            Properties.Settings.Default.DatabaseAddress = ChefmateController.Instance.CurrentSettings.DbIpAddress;
            Properties.Settings.Default.Save();
            ChefmateController.Instance.SaveSettings();
            SetFont();
            if (displayType != ChefmateController.Instance.CurrentSettings.GroupType)
                RedrawOrders();
            OrderContainer.Focus();
        }
        private void Recall(object sender, RoutedEventArgs e)
        {
            RecallOrderView recallOrderView = new RecallOrderView
            {
                ShowInTaskbar = false
            };
            recallOrderView.ShowDialog();
            if (recallOrderView.ModalResult && recallOrderView.SelectedOrder != null)
            {
                ChefmateController.Instance.AddRecallOrder(recallOrderView.SelectedOrder);
            }
            OrderContainer.Focus();
        }
        #endregion

        #region Navigations
        private void Backward(object sender)
        {
            var position = ParentScroller.HorizontalOffset;
            var offset = position - OrderWidth * ChefmateController.Instance.PageColumns;
            ParentScroller.ScrollToHorizontalOffset(offset);
            UpdateNavigationButtonDisplay(offset);
        }
        private void Forward(object sender)
        {
            var position = ParentScroller.HorizontalOffset;
            var offset = position + OrderWidth * ChefmateController.Instance.PageColumns;
            ParentScroller.ScrollToHorizontalOffset(offset);
            UpdateNavigationButtonDisplay(offset);

        }
        private void ScrollToRight(object obj)
        {
            ParentScroller.ScrollToRightEnd();
            double index = Math.Floor(_cordX / (OrderWidth * ChefmateController.Instance.PageColumns)) + 1;
            UpdateNavigationButtonDisplay(index * ChefmateController.Instance.PageColumns * OrderWidth);
        }
        private void ScrollToLeft(object obj)
        {
            ParentScroller.ScrollToLeftEnd();
            UpdateNavigationButtonDisplay(0);
        }
        private void UpdateNavigationButtonDisplay(double offset = 0)
        {
            var backwardEnabled = offset > 0;
            var forwardEnabled = offset + (this.ActualWidth + 10) < _cordX + _orderWidth;
            BackwardNavigationButton.IsEnabled = backwardEnabled;
            LeftNavigationButton.IsEnabled = backwardEnabled;
            ForwardNavigationButton.IsEnabled = forwardEnabled;
            RightNavigationButton.IsEnabled = forwardEnabled;
            OrderContainer.Focus();
        }
        #endregion

        #region Drawing
        private void SetFont()
        {
            GuiStyles.ItemFontSize = ChefmateController.Instance.CurrentSettings.CmFontSize;
            GuiStyles.GroupFontSize = ChefmateController.Instance.CurrentSettings.GroupFontSize;
            GuiStyles.OrderHeaderFontSize = ChefmateController.Instance.CurrentSettings.OrderHeaderFontSize;
            GuiStyles.AnalysisFontSize = ChefmateController.Instance.CurrentSettings.AnalysisFontSize;

            GuiStyles.GroupFontFamily = new FontFamily(ChefmateController.Instance.CurrentSettings.GroupFontFamily);
            GuiStyles.ItemFontFamily = new FontFamily(ChefmateController.Instance.CurrentSettings.ItemFontFamily);
            GuiStyles.AnalysisFontFamily = new FontFamily(ChefmateController.Instance.CurrentSettings.AnalysisFontFamily);

            GuiStyles.AnalysisFontWeight = ChefmateController.Instance.CurrentSettings.AnalysisFontBold ? FontWeights.Bold : FontWeights.Normal;
            GuiStyles.GroupFontWeight = ChefmateController.Instance.CurrentSettings.GroupFontBold ? FontWeights.Bold : FontWeights.Normal;
            GuiStyles.HeaderForeGround = new SolidColorBrush((Color)ColorConverter.ConvertFromString(ChefmateController.Instance.CurrentSettings.HeaderForegroundColor));
            GuiStyles.HeaderBackGround = new SolidColorBrush((Color)ColorConverter.ConvertFromString(ChefmateController.Instance.CurrentSettings.HeaderBackgroundColor));
            ChefmateConstants.FirstWarningOrderColor = new SolidColorBrush((Color)ColorConverter.ConvertFromString(ChefmateController.Instance.CurrentSettings.FirstWarningColor));
            ChefmateConstants.SecondWarningOrderColor = new SolidColorBrush((Color)ColorConverter.ConvertFromString(ChefmateController.Instance.CurrentSettings.SecondWarningColor));
            ChefmateConstants.NormalOrderColor = new SolidColorBrush((Color)ColorConverter.ConvertFromString(ChefmateController.Instance.CurrentSettings.NewOrderColor));
        }
        private void RedrawOrders()
        {
            this.Dispatcher.Invoke(new Action(() =>
            {
                _cordX = 0;
                _cordY = 0;
                CurrentDisplayOrders.Clear();
                OrderContainer.Children.Clear();
                foreach (var order in TotalOrders)
                {
                    FilterOrder(order);
                }
                UpdateNavigationButtonDisplay();
            }));

        }
        private void FilterOrder(Order inOrder)
        {
            inOrder.FilterOrders(ChefmateController.Instance.CurrentSettings.GroupType, ChefmateController.Instance.CurrentSettings.OrderInfoDisplay);
            double remainingHeight = _maxHeight - _cordY;
            var orderheight = inOrder.GetOrderActualHeight();
            if (remainingHeight > orderheight)
            {
                AddOrderToCanvas(inOrder);
            }
            else
            {
                _cordY = 0;
                if (CurrentDisplayOrders.Count > 0)
                    _cordX += OrderWidth;
                if (orderheight > _maxHeight)
                    DrawLargeOrder(inOrder, orderheight);
                else
                    FilterOrder(inOrder);
            }

        }
        private void DrawLargeOrder(Order inOrder, double orderHeight)
        {
            OrderGuiIndex orderGuiIndex = new OrderGuiIndex();
            bool headerVisible = true;
            while (orderGuiIndex.GroupIndex < inOrder.DisplayGroups.Count)
            {
                double remainingHeight = _maxHeight - _cordY;
                var order = GetOrderForHeight(inOrder, remainingHeight, orderGuiIndex, headerVisible);
                order.DisplayAttributes.IsHeaderVisible = headerVisible;
                headerVisible = false;
                AddOrderToCanvas(order);
                orderHeight -= order.GetOrderActualHeight();
                if (orderHeight > 0)
                {
                    _cordX += _orderWidth;
                    _cordY = 0;
                }
            }

        }
        private Order GetOrderForHeight(Order inOrder, double remainigHeight, OrderGuiIndex orderGuiIndex, bool inHeaderVisible)
        {
            var order = new Order(inOrder);
            if (inHeaderVisible)
                remainigHeight -= inOrder.OrderAction == ChefmateConstants.WebOrderAction ? ChefmateConstants.WebOrderHeaderHeight : ChefmateConstants.OrderHeaderHeight;
            while (remainigHeight >= ChefmateConstants.UnitHeight && orderGuiIndex.GroupIndex < inOrder.DisplayGroups.Count)
            {
                var group = GetGroupForHeight(inOrder.DisplayGroups[orderGuiIndex.GroupIndex], ref remainigHeight, orderGuiIndex);
                if (group.Items.Count > 0)
                {
                    group.Order = order;
                    order.DisplayGroups.Add(group);
                    if (orderGuiIndex.GroupItemIndex == inOrder.DisplayGroups[orderGuiIndex.GroupIndex].Items.Count)
                    {
                        orderGuiIndex.GroupIndex++;
                        orderGuiIndex.GroupItemIndex = 0;
                        orderGuiIndex.ItemOptionIndex = 0;
                        orderGuiIndex.ItemSideIndex = 0;
                        orderGuiIndex.IsLastItempartial = false;
                    }
                }
            }
            return order;
        }
        private Group GetGroupForHeight(Group inGroup, ref double height, OrderGuiIndex orderGuiIndex)
        {
            var group = new Group(inGroup);
            if (orderGuiIndex.GroupItemIndex == 0 && orderGuiIndex.ItemOptionIndex == 0 && orderGuiIndex.ItemSideIndex == 0)
                height -= ChefmateConstants.UnitHeight;
            else
                group.DisplayAttributes.IsHeaderVisible = false;
            while (height >= ChefmateConstants.UnitHeight && orderGuiIndex.GroupItemIndex < inGroup.Items.Count)
            {
                var item = inGroup.Items[orderGuiIndex.GroupItemIndex];
                var newItem = GetItemForHeight(item, ref height, orderGuiIndex);
                group.Items.Add(newItem);
            }
            return group;
        }
        private Item GetItemForHeight(Item inItem, ref double height, OrderGuiIndex orderGuiIndex)
        {
            var itemHeight = inItem.GetItemActualHeight();
            if (itemHeight <= height)
            {
                orderGuiIndex.IsLastItempartial = false;
                orderGuiIndex.GroupItemIndex++;
                height -= itemHeight;
                return inItem;
            }
            else
            {
                var item = new Item(inItem);
                if (!orderGuiIndex.IsLastItempartial)
                    height -= ChefmateConstants.UnitHeight;
                else
                    item.DisplayAttributes.IsHeaderVisible = false;
                while (height > ChefmateConstants.UnitHeight && orderGuiIndex.ItemSideIndex < inItem.Sides.Count)
                {
                    var side = new Side(inItem.Sides[orderGuiIndex.ItemSideIndex]);
                    item.Sides.Add(side);
                    height -= ChefmateConstants.UnitHeight;
                    orderGuiIndex.ItemSideIndex++;
                }
                while (height > ChefmateConstants.UnitHeight && orderGuiIndex.ItemOptionIndex < inItem.Options.Count)
                {
                    var option = new Option(inItem.Options[orderGuiIndex.ItemOptionIndex]);
                    item.Options.Add(option);
                    height -= ChefmateConstants.UnitHeight;
                    orderGuiIndex.ItemOptionIndex++;
                }
                if (orderGuiIndex.ItemSideIndex == inItem.Sides.Count &&
                    orderGuiIndex.ItemOptionIndex == inItem.Options.Count)
                {
                    orderGuiIndex.GroupItemIndex++;
                    orderGuiIndex.IsLastItempartial = false;
                }
                else
                {
                    orderGuiIndex.IsLastItempartial = true;
                }
                return item;
            }
        }
        private void AddOrderToCanvas(Order inOrder)
        {
            OrderWidth = this.ActualWidth / ChefmateController.Instance.PageColumns;
            var orderControl = new OrderControl(inOrder, OrderWidth);
            OrderContainer.Children.Add(orderControl);
            Canvas.SetLeft(orderControl, _cordX);
            Canvas.SetTop(orderControl, _cordY);
            inOrder.CordX = _cordX;
            inOrder.CordY = _cordY;
            _cordY += inOrder.GetOrderActualHeight();
            CurrentDisplayOrders.Add(inOrder);
            OrderContainer.Width = (Math.Floor(_cordX / this.ActualWidth) + 1) * OrderWidth * ChefmateController.Instance.PageColumns;
        }
        private bool IsOrderInView(Order inOrder)
        {
            return inOrder.CordX >= ParentScroller.HorizontalOffset && inOrder.CordX < (ParentScroller.HorizontalOffset + this.ActualWidth);
        }
        private bool CanAddOrderToGui(Order inOrder)
        {
            bool retVal = inOrder.OrderAction.Equals(ChefmateConstants.OrderAction, StringComparison.InvariantCultureIgnoreCase) &&
                inOrder.OrderState == OrderState.Complete && OutputTime.Immediately != ChefmateController.Instance.CurrentSettings.OutputTime;
            return retVal;
        }
        private void AddPendingWebOrdersToGui()
        {
            var ordersToRemove = new List<int>();
            foreach (var webOrder in ChefmateController.Instance.WebOrders)
            {
                var canAdd = webOrder.DeliveryTime.Subtract(DateTime.Now).TotalMinutes <= ChefmateController.Instance.CurrentSettings.WebOrderTime;
                if (canAdd)
                {
                    webOrder.ArrivalTime = DateTime.Now;
                    ordersToRemove.Add(webOrder.OrderKey);
                    TotalOrders.Add(webOrder);
                    FilterOrder(webOrder);
                    AnalyticalData.TotalOrdersCount++;
                    AnalyticalData.CurrentOrdersCount++;
                    AnalyticalData.CurrentItems += webOrder.Items.Count;
                    DbOrder.UpdateOrderArrivalTime(webOrder.OrderKey);
                }
            }
            ChefmateController.Instance.WebOrders.RemoveAll(s => ordersToRemove.Contains(s.OrderKey));
        }


        #endregion

        #region OrderDisplay
        private void AddOrderEventHandler(Order order)
        {
            Application.Current.Dispatcher.Invoke(new
                Action(() =>
                {
                    StopTimer();
                    FilterOrder(order);
                    StartTimer();
                    UpdateNavigationButtonDisplay();
                }));
        }
        private void OrderArrivedEventHandler(Order order)
        {
            Application.Current.Dispatcher.Invoke(new
                Action(() =>
                {
                    StopTimer();
                    bool addOrder = CanAddOrderToGui(order);
                    ChefmateController.Instance.AddOrder(order);
                    if (addOrder)
                        FilterOrder(order);
                    StartTimer();
                    UpdateNavigationButtonDisplay();
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
                    var selectedOrder = TotalOrders[_lastSelectedIndex];
                    selectedOrder = CurrentDisplayOrders.FirstOrDefault(s => s.OrderKey == selectedOrder.OrderKey);
                    if (selectedOrder != null && IsOrderInView(selectedOrder))
                    {
                        if (doBump)
                        {
                            OrderController.Instance.BumpOrder(selectedOrder);
                            _lastSelectedIndex = -1;
                            RedrawOrders();
                        }
                        else
                            selectedOrder.DisplayAttributes.ExecutePopUp = true;
                    }
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
                    if (!BumpByChitTableNumber(_chitTableNumber))
                    {
                        ChefmateController.Instance.ShowMessageBox("Error",
                            "Table/Chit Number does not exist on the current screen. Please press enter to continue.");
                    }
                    else
                    {
                        RedrawOrders();
                    }
                    _chitTableNumber = "";
                    break;
                case Key.Escape:
                    _chitTableNumber = "";
                    break;
            }
        }
        public bool BumpByChitTableNumber(string chitTableNumber)
        {
            if (string.IsNullOrWhiteSpace(chitTableNumber))
                return false;
            bool bumpChit = BumpByChitNumber(chitTableNumber);
            bool bumpTable = BumpByTableNumber(chitTableNumber);
            return bumpTable || bumpChit;
        }
        private bool BumpByChitNumber(string chitNumber)
        {
            var order = CurrentDisplayOrders.FirstOrDefault(s => s.ChitValue.Contains(chitNumber));
            if (order != null && IsOrderInView(order))
            {
                OrderController.Instance.BumpOrder(order);
                return true;
            }
            return false;
        }
        private bool BumpByTableNumber(string tableNumber)
        {
            var order = CurrentDisplayOrders.FirstOrDefault(s => s.TableTabName.Contains(tableNumber));
            if (order != null && IsOrderInView(order))
            {
                OrderController.Instance.BumpOrder(order);
                return true;
            }
            return false;
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
            Application.Current.Dispatcher.Invoke(new Action(() =>
            {
                AddPendingWebOrdersToGui();
                UpdateOrderInfoDisplay();
            }));
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
            order.DisplayGroups.ToList().ForEach(s => { SetGroupColor(s, solidColorBrush); });

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
