using System.Windows.Media;

namespace Chefmate.Core
{
    public static class ChefmateConstants
    {
        public const double OrderHeaderHeight = 54;
        public const double UnitHeight = 50;
        public const double NoteHeight = 40;
        public const double WebOrderHeaderHeight = 100;
        public const string Number = "6.60.0.2";
        public const string DatabaseVersion = "1.4";
        public const string OrderAction = "order";
        public const string WebOrderAction = "webOrder";



        public static SolidColorBrush NormalOrderColor = new SolidColorBrush(Color.FromRgb(180, 199, 180));
        public static SolidColorBrush FirstWarningOrderColor = new SolidColorBrush(Color.FromRgb(241, 218, 180));
        public static SolidColorBrush SecondWarningOrderColor = new SolidColorBrush(Color.FromRgb(244, 180, 180));
        public static SolidColorBrush NormalGroupColor = new SolidColorBrush(Color.FromRgb(120, 157, 120));
        public static SolidColorBrush FirstWarningGroupColor = new SolidColorBrush(Color.FromRgb(243, 196, 120));
        public static SolidColorBrush SecondWarningGroupColor = new SolidColorBrush(Color.FromRgb(248, 120, 120));
        public static SolidColorBrush LightGrayColor = new SolidColorBrush(Color.FromRgb(169, 171, 174));
        public static SolidColorBrush LightRed = new SolidColorBrush(Color.FromRgb(102, 178, 255));

        public static char[] FirstRowWithoutShift = { '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=' };
        public static char[] FirstRowWithShift = { '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+' };

        public static char[] SecondRowWithoutShiftCapsOn = { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\\' };
        public static char[] SecondRowWithoutShiftCapsOff = { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\' };
        public static char[] SecondRowWithShiftCapsOn = { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '|' };
        public static char[] SecondRowWithShiftCapsOff = { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '|' };

        public static char[] ThirdRowWithoutShiftCapsOn = { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'' };
        public static char[] ThirdRowWithoutShiftCapsOff = { 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'' };
        public static char[] ThirdRowWithShiftCapsOn = { 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"' };
        public static char[] ThirdRowWithShiftCapsOff = { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"' };

        public static char[] FourthRowWithoutShiftCapsOn = { 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/' };
        public static char[] FourthRowWithoutShiftCapsOff = { 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/' };
        public static char[] FourthRowWithShiftCapsOn = { 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?' };
        public static char[] FourthRowWithShiftCapsOff = { 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?' };
    }
}