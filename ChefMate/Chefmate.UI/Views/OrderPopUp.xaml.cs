using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Chefmate.Core.Commands;
using Chefmate.Core.Model;
using Chefmate.UI.Annotations;
using Chefmate.UI.Controller;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for OrderPopUp.xaml
    /// </summary>
    public partial class OrderPopUp : Window, INotifyPropertyChanged
    {
        private Order _order;
        private double _orderHeight;
        private double _orderWidth;
        public OrderPopUp()
        {
            InitializeComponent();
            this.DataContext = this;
            CloseCommand = new DelegateCommand(CloseClick);
            this.SizeChanged += OrderPopUp_SizeChanged;
        }
        private void OrderPopUp_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            OrderHeight = (this.ActualHeight * .85);
            if (OrderContainer.ActualWidth > 0)
            {
                OrderWidth = OrderContainer.ActualWidth / 4;
            }
        }
        public Order Order
        {
            get { return _order; }
            set
            {
                _order = value;
                OnPropertyChanged("Order");
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
        public ICommand CloseCommand { get; set; }

        private void CloseClick(object sender)
        {
            Close();
        }

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged(string propertyName = null)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

    }
}
