using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Chefmate.Core.Enums;
using Chefmate.Core.Model;
using Chefmate.Infrastructure.Controller;
using Chefmate.Logger;
using Chefmate.UI.Annotations;

namespace Chefmate.UI.UserControls
{
    public partial class PopupButton : UserControl, INotifyPropertyChanged
    {
        public static readonly DependencyProperty OrderProperty = DependencyProperty.Register("Order", typeof(Order), typeof(PopupButton));
        public static readonly DependencyProperty GroupProperty = DependencyProperty.Register("Group", typeof(Group), typeof(PopupButton));
        public static readonly DependencyProperty ItemProperty = DependencyProperty.Register("Item", typeof(Item), typeof(PopupButton));
        public static readonly DependencyProperty ItemTypeProperty = DependencyProperty.Register("ItemType", typeof(int), typeof(PopupButton));

        public PopupButton()
        {
            InitializeComponent();
        }
        public ICommand ShowPopupCommand { get; set; }
        public Order Order
        {
            get { return (Order)GetValue(OrderProperty); }
            set
            {
                SetValue(OrderProperty, value);
                this.DataContext = value;
            }
        }
        public Group Group
        {
            get { return (Group)GetValue(GroupProperty); }
            set
            {
                SetValue(GroupProperty, value);
                this.DataContext = value;
            }
        }
        public Item Item
        {
            get { return (Item)GetValue(ItemProperty); }
            set
            {
                SetValue(ItemProperty, value);
                this.DataContext = value;
            }
        }
        public int ItemType
        {
            get { return (int)GetValue(ItemTypeProperty); }
            set { SetValue(ItemTypeProperty, value); }
        }

        private void ShowPopup(object sender, RoutedEventArgs e)
        {
            LoadCommands();
            if (CommandComboBox.Items.Count > 0)
            {
                CommandComboBox.Visibility = Visibility.Visible;
                CommandComboBox.IsDropDownOpen = true;
            }
        }
        private void CommandComboBox_OnDropDownClosed(object sender, EventArgs e)
        {
            CommandComboBox.Visibility = Visibility.Hidden;
            try
            {
                if (CommandComboBox.SelectionBoxItem != null)
                {
                    var command = Convert.ToString(CommandComboBox.SelectionBoxItem);
                    switch (ItemType)
                    {
                        case 1:
                            Controller.OrderController.Instance.OrderCommandHandler(command, Order);
                            break;
                        case 2:
                            Controller.OrderController.Instance.OrderCommandHandler(command, Group);
                            break;
                        case 3:
                            Controller.OrderController.Instance.OrderCommandHandler(command, Item);
                            break;
                    }
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("CommandComboBox_OnDropDownClosed", ex.Message);
            }
            CommandComboBox.SelectedIndex = -1;
        }
        private void LoadCommands()
        {
            try
            {
                CommandComboBox.Items.Clear();
                switch (ItemType)
                {
                    case 1:
                        LoadOrderCommand();
                        break;
                    case 2:
                        LoadGroupCommand();
                        break;
                    case 3:
                        LoadItemCommand();
                        break;
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("PopupControl.LoadCommands", ex.Message);
            }
        }
        private void LoadOrderCommand()
        {
            if (ChefmateController.Instance.CurrentSettings.BumpOnly)
            {
                CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Bump" });
            }
            else
            {
                if (Order.OrderState == OrderState.Credit || Order.OrderState == OrderState.Cancel || Order.OrderState == OrderState.Runner)
                {
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Bump" });
                }
                else if (Order.OrderState == OrderState.Complete)
                {
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Start" });
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Hold" });
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Bump" });
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Transfer" });
                }
                CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Hide All" });
            }
        }
        private void LoadGroupCommand()
        {
            if (ChefmateController.Instance.CurrentSettings.BumpOnly && Group.OrderStatus != OrderStatus.Bumped)
            {
                CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Bump" });
            }
            else
            {
                if (Group.IsVisible)
                {
                    if (Group.OrderStatus != OrderStatus.Bumped)
                    {
                        if (Group.Order.OrderState == OrderState.Complete)
                        {
                            CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Start" });
                            CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Hold" });
                        }
                        if (Group.Order.OrderState == OrderState.Complete || Group.Order.OrderState == OrderState.Runner)
                            CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Bump" });
                    }
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Hide" });
                }
                else
                {
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Show" });
                }
            }
        }
        private void LoadItemCommand()
        {
            if (Item.IsChildVisible)
            {
                if (Item.SCourseGroup.Order.OrderState == OrderState.Complete && Item.OrderStatus != OrderStatus.Bumped
                    && !ChefmateController.Instance.CurrentSettings.BumpOnly)
                {
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Start" });
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Hold" });
                }

                if ((Item.Options.Count > 0
                     || Item.Sides.Count > 0
                     || Item.Note != "")
                    && !ChefmateController.Instance.CurrentSettings.BumpOnly)
                {
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Hide" });
                }

                if (Item.OrderStatus != OrderStatus.Bumped)
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Bump" });

                if (!ChefmateController.Instance.CurrentSettings.BumpOnly)
                    CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Note" });
            }
            else
            {
                CommandComboBox.Items.Add(new ComboBoxItem() { Content = "Show" });
            }
        }
        private void CommandComboBox_OnDropDownOpened(object sender, EventArgs e)
        {
            LoadCommands();
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
