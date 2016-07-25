using System.Windows;
using System.Windows.Controls.Primitives;
using System.Windows.Input;

namespace Chefmate.UI.Behaviours
{
    public class PreviewMouseUpBehaviour
    {
        public static readonly DependencyProperty CommandProperty = DependencyProperty.RegisterAttached("PreviewCommand", typeof(ICommand),
                                                                        typeof(PreviewMouseUpBehaviour), new PropertyMetadata(PropertyChangedCallback));

        public static void PropertyChangedCallback(DependencyObject depObj, DependencyPropertyChangedEventArgs args)
        {
            Selector selector = (Selector)depObj;
            if (selector != null)
            {
                selector.PreviewMouseUp += Selector_PreviewMouseUp;
            }
        }

        private static void Selector_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            Selector selector = (Selector)sender;
            if (selector != null)
            {
                ICommand command = selector.GetValue(CommandProperty) as ICommand;
                if (command != null)
                {
                    command.Execute(selector.SelectedItem);
                }
            }
        }

        public static ICommand GetCommand(UIElement element)
        {
            return (ICommand)element.GetValue(CommandProperty);
        }

        public static void SetCommand(UIElement element, ICommand command)
        {
            element.SetValue(CommandProperty, command);
        }

    }
}