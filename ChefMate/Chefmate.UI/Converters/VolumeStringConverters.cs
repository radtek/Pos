using System;
using System.Globalization;
using System.Windows.Data;

namespace Chefmate.UI.Converters
{
    public class VolumeStringConverters:IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            int volume = System.Convert.ToInt32(value);
            return volume.ToString("G") + " %";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}