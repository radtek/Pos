using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;
using Chefmate.Core.Enums;
using Chefmate.Core.Extensions;

namespace Chefmate.UI.Converters
{
    public class OrderStatusToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var orderStatus = (OrderStatus)Enum.Parse(typeof(OrderStatus), System.Convert.ToString(value));
            switch (orderStatus)
            {
                case OrderStatus.Normal:
                    return Brushes.White;
                case OrderStatus.Transfered:
                    return Brushes.Purple;
                case OrderStatus.Started:
                    return Brushes.Green;
                case OrderStatus.Hold:
                    return Brushes.Red;
                case OrderStatus.Bumped:
                    return Brushes.White;
                case OrderStatus.Canceled:
                    return Brushes.Black;
            }
            return Brushes.White;
        }
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}