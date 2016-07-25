using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WebMateTests
{
    public static class WebMateTestSettings
    {
        static WebMateTestSettings()
        {
            WebOrderKey = 1;
            ThirdPartyCodeKey = 1;
            SideThirdPartyCode1 = 2;
            SideThirdPartyCode2 = 3;
            SideThirdPartyCode3 = 4;
            MatchingItemSizeKey = 423;
            WebOrderTabName = "WebMate";
            OptionKey1 = 206;
            OptionKey2 = 207;
            OptionKey3 = 208;

            MainItemPrice = MainItemPriceL0 = MainItemPriceL1 = 12.75m;
            SideItem1Price = SideItem1PriceL0 = SideItem1PriceL1 = 7.50m;
            SideItem2Price = SideItem2PriceL0 = SideItem2PriceL1 = 8.00m;
            SideItem3Price = SideItem3PriceL0 = SideItem3PriceL1 = 6.70m;

            MainItemQty = 1;
            Side1Qty = 1;
            Side2Qty = 2;
            Side3Qty = 3;
        }

        public static int WebOrderKey { get; set; }
        public static int ThirdPartyCodeKey { get; set; }
        public static int MatchingItemSizeKey { get; set; }
        public static string WebOrderTabName { get; set; }
        public static int SideThirdPartyCode1 { get; set; }
        public static int SideThirdPartyCode2 { get; set; }
        public static int SideThirdPartyCode3 { get; set; }
        public static int OptionKey1 { get; set; }
        public static int OptionKey2 { get; set; }
        public static int OptionKey3 { get; set; }
              

        public static decimal MainItemPrice { get; set; }
        public static decimal MainItemPriceL0 { get; set; }
        public static decimal MainItemPriceL1 { get; set; }

        public static decimal SideItem1Price { get; set; }
        public static decimal SideItem1PriceL0 { get; set; }
        public static decimal SideItem1PriceL1 { get; set; }

        public static decimal SideItem2Price { get; set; }
        public static decimal SideItem2PriceL0 { get; set; }
        public static decimal SideItem2PriceL1 { get; set; }

        public static decimal SideItem3Price { get; set; }
        public static decimal SideItem3PriceL0 { get; set; }
        public static decimal SideItem3PriceL1 { get; set; }

        public static int MainItemQty { get; set; }
        public static int Side1Qty { get; set; }
        public static int Side2Qty { get; set; }
        public static int Side3Qty { get; set; }
    }
}
