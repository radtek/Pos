using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Chefmate.Core.Commands;
using Chefmate.Core.Model;
using Chefmate.UI.Annotations;
using Chefmate.UI.Views;

namespace Chefmate.UI.UserControls
{
    /// <summary>
    /// Interaction logic for OrderControl.xaml
    /// </summary>
    public partial class OrderControl : UserControl,INotifyPropertyChanged
    {
        public static readonly DependencyProperty ChefmateOrderProperty = DependencyProperty.Register("ChefmateOrder", typeof(Order), typeof(OrderControl));
        public OrderControl()
        {
            InitializeComponent();
            PopUpCommand = new DelegateCommand(ExceutePopUp);
            this.DataContext = this;
        }

        #region Commands
        public ICommand PopUpCommand { get; set; }
        #endregion

        public Order ChefmateOrder
        {
            get
            {
                return (Order)GetValue(ChefmateOrderProperty);
            }
            set
            {
                SetValue(ChefmateOrderProperty, value);
            }
        }



        private void ExceutePopUp(object param)
        {
            //int orderNumber = Convert.ToInt32(param);
            //var order = TotalOrders.FirstOrDefault(s => s.OrderNumber == orderNumber);
            //OrderPopUp orderPopUp = new OrderPopUp
            //{
            //    Order = order,
            //    ShowInTaskbar = false
            //};
            //orderPopUp.ShowDialog();
        }

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
