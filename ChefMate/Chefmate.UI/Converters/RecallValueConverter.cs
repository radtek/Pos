using System;
using System.Globalization;
using System.Windows.Data;

namespace Chefmate.UI.Converters
{
    public class RecallValueConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var selectedValue = System.Convert.ToInt32(value);
            var param = System.Convert.ToInt32(parameter);
            bool returnValue = selectedValue == param;
            return returnValue;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return parameter;
        }
    }
}