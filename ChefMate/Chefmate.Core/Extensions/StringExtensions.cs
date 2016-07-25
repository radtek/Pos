using System;
using Chefmate.Core.Enums;

namespace Chefmate.Core.Extensions
{
    public static class StringExtensions
    {
        public static DateTime ToDateTime(this string dateString)
        {
            DateTime dateTime;
            if (DateTime.TryParse(dateString, out dateTime))
            {
                return dateTime;
            }
            return DateTime.MinValue;
        }
        public static int ToInteger(this string integerString)
        {
            int intValue;
            if (int.TryParse(integerString, out intValue))
            {
                return intValue;
            }
            return default(int);
        }
        public static bool ToBoolean(this string boolString)
        {
            bool boolValue;
            if (bool.TryParse(boolString, out boolValue))
            {
                return boolValue;
            }
            return boolValue;
        }



    }
}