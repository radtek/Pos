using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace Chefmate.UI.Converters
{
    public class StringToColorConverter:IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null)
                return Colors.Transparent;
            else
                return ColorConverter.ConvertFromString(System.Convert.ToString(value));

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return System.Convert.ToString(value);
        }
    }
}