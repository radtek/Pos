using System.ComponentModel;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using Chefmate.Core.Commands;
using Chefmate.Core.Model;
using Chefmate.Database.DbModels;


namespace Chefmate.UI.UserControls
{
    /// <summary>
    /// Interaction logic for OrderControl.xaml
    /// </summary>
    public partial class OrderControl : UserControl, INotifyPropertyChanged
    {
        private double _orderWidth;
        private Order _order;
        public OrderControl()
        {
            InitializeComponent();
            
        }

        public OrderControl(Order inOrder, double inWidth)
            : this()
        {
            this.Order = inOrder;
            this.DataContext = this;
            OrderWidth = inWidth;
            CallAwayStopCommand = new DelegateCommand(StopCallAway);
            HeaderPoppup.Order = inOrder;
        }
        public ICommand CallAwayStopCommand { get; set; }
        public Order Order
        {
            get { return _order; }
            set
            {
                _order = value;
                OnPropertyChanged("Order");
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
        private void StopCallAway(object obj)
        {
            var group = obj as Group;
            if (group != null)
            {
                group.DisplayAttributes.IsBlinkingEnable = false;
                DbOrderGroup.CallAwayGroup(group.OrderGroupKey, false);
            }
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
