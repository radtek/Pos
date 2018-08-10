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
using System.Windows.Data;
using System.Windows.Media;
using System.Windows.Controls;
using ChefMate.Database;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for SummaryView.xaml
    /// </summary>
    /// 

    public class PercentageConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return System.Convert.ToDouble(value) * System.Convert.ToDouble(parameter);
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class FontSizeConverter : IValueConverter
    {

        #region IValueConverter Members

        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ChefmateController.Instance.CurrentSettings.ItemSummaryFontSize;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ChefmateController.Instance.CurrentSettings.ItemSummaryFontSize;
        }

        #endregion
    }

    public class FontFamilyConverter : IValueConverter
    {

        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ChefmateController.Instance.CurrentSettings.ItemSummaryFontFamily;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ChefmateController.Instance.CurrentSettings.ItemSummaryFontFamily;
        }
    }

    public class BackGroundColorConverter : IValueConverter
    {

        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ChefmateController.Instance.CurrentSettings.ItemSummaryBackgroundColor;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ChefmateController.Instance.CurrentSettings.ItemSummaryBackgroundColor;
        }
    }

    public partial class SummaryView : Window, INotifyPropertyChanged
    {
        private List<string> _itemList;
        private double _windowWidth;
        private string _statusText;
        private GuiStyles _guiStyles;

        public SummaryView()
        {
            InitializeComponent();
            Initialize();
            AutoUpdateCommand = new DelegateCommand(AutoUpdateCommandHandler);
            CloseCommand = new DelegateCommand(CloseCommandHandler);
            LoadCommand = new DelegateCommand(LoadOrdersCommandHandler);
            SetTimer();
            this.Loaded += SummaryView_Loaded;
            WindowWidth = Screen.PrimaryScreen.Bounds.Width * .8;
            ItemList = new List<string>();
            this.DataContext = this;
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
        public GuiStyles GuiStyles
        {
            get { return _guiStyles; }
            set
            {
                _guiStyles = value;
                OnPropertyChanged("GuiStyles");
            }
        }


        #region Commands
        public ICommand AutoUpdateCommand { get; set; }
        public ICommand CloseCommand { get; set; }
        public ICommand LoadCommand { get; set; }
        public ICommand NavigateForwardCommand { get; set; }
        public ICommand NavigateBackwardCommand { get; set; }
        public ICommand NavigateLeftCommand { get; set; }
        public ICommand NavigateRightCommand { get; set; }

        #endregion

        private void Initialize()
        {
            NavigateBackwardCommand = new DelegateCommand(Backward);
            NavigateForwardCommand = new DelegateCommand(Forward);
            NavigateLeftCommand = new DelegateCommand(ScrollToLeft);
            NavigateRightCommand = new DelegateCommand(ScrollToRight);
            LoadFontInfo();
        }

        private void LoadFontInfo()
        {
            GuiStyles = new GuiStyles();
            GuiStyles.AnalysisFontWeight = ChefmateController.Instance.CurrentSettings.AnalysisFontBold ? FontWeights.Bold : FontWeights.Normal;
            GuiStyles.AnalysisFontFamily = new FontFamily(ChefmateController.Instance.CurrentSettings.AnalysisFontFamily);
            GuiStyles.HeaderForeGround = new SolidColorBrush((Color)ColorConverter.ConvertFromString(ChefmateController.Instance.CurrentSettings.HeaderForegroundColor));
            GuiStyles.HeaderBackGround = new SolidColorBrush((Color)ColorConverter.ConvertFromString(ChefmateController.Instance.CurrentSettings.HeaderBackgroundColor));
            GuiStyles.AnalysisFontSize = ChefmateController.Instance.CurrentSettings.AnalysisFontSize;
            //following are loaded bacause later this binding will be done without converter.
            GuiStyles.ItemSummaryFontFamily = new FontFamily(ChefmateController.Instance.CurrentSettings.ItemSummaryFontFamily);
            GuiStyles.ItemSummaryBackGround = new SolidColorBrush((Color)ColorConverter.ConvertFromString(ChefmateController.Instance.CurrentSettings.ItemSummaryBackgroundColor));
            GuiStyles.ItemSummaryFontSize = ChefmateController.Instance.CurrentSettings.ItemSummaryFontSize;
        }

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
                ItemSummaryList.ItemsSource = ItemList;
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

        #region Navigations
        public static T FindVisualChild<T>(DependencyObject obj) where T : DependencyObject
        {

            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(obj); i++)
            {
                DependencyObject child = VisualTreeHelper.GetChild(obj, i);

                if (child != null && child is T)
                {
                    return (T)child;
                }
                else
                {
                    T childOfChild = FindVisualChild<T>(child);

                    if (childOfChild != null) return childOfChild;
                }
            }

            return null;
        }
        private void Backward(object sender)
        {
            var scroller = FindVisualChild<ScrollViewer>(ItemSummaryList);
            ItemOffset = System.Windows.SystemParameters.PrimaryScreenWidth;
            scroller.ScrollToHorizontalOffset(scroller.HorizontalOffset - ItemOffset);
        }
        double ItemOffset = 0;
        private void Forward(object sender)
        {


            var scroller = FindVisualChild<ScrollViewer>(ItemSummaryList);
            ItemOffset = System.Windows.SystemParameters.PrimaryScreenWidth;
            scroller.ScrollToHorizontalOffset(scroller.HorizontalOffset + ItemOffset);

        }
        private void ScrollToRight(object obj)
        {
            var scroller = FindVisualChild<ScrollViewer>(ItemSummaryList);
            scroller.ScrollToHorizontalOffset(double.MaxValue);
        }
        private void ScrollToLeft(object obj)
        {
            var scroller = FindVisualChild<ScrollViewer>(ItemSummaryList);
            scroller.ScrollToHorizontalOffset(-double.MaxValue);
        }
        #endregion

    }
}
