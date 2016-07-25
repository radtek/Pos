using System;
using System.Globalization;
using System.Windows.Data;

namespace Chefmate.UI.Converters
{
    public class OptionSignConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (System.Convert.ToBoolean(value))
                return "+";
            return "-";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (System.Convert.ToString(value) == "+")
                return true;
            return false;
        }
    }
}