using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WebMateTestProgram
{
    public class SideItem
    {
        public string  ThirdPartyCode { get; private set; }
        public decimal Price          { get; private set; }
        public uint    DiscountID     { get; private set; }
        public uint    Qty            { get; private set; }

        public SideItem(string inThirdPartyCode, decimal inPrice, uint inQty)
        {
            ThirdPartyCode = inThirdPartyCode;
            Price = inPrice;
            DiscountID = 0;
            Qty = inQty;
        }
    }
}
