using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WebMateTestProgram
{
    public class Item
    {
        public string  ThirdPartyCode { get; private set; }
        public Decimal Price          { get; private set; }
        public uint    DiscountID     { get; private set; }
        public uint    Qty            { get; private set; }

        private List<SideItem> sideItems;

        public Item(string inThirdPartyCode, decimal inPrice, uint inQty)
        {
            sideItems = new List<SideItem>();
            ThirdPartyCode = inThirdPartyCode;
            Price = inPrice;
            DiscountID = 0;
            Qty = inQty;
        }

        public int GetSideItemsCount()
        {
            return sideItems.Count;
        }

        public SideItem GetSideItemAtIndex(int i)
        {
            return sideItems.ElementAt(i);
        }

        public void AddSideItem(SideItem inSideItem)
        {
            sideItems.Add(inSideItem);
        }

        public void RemoveSideItem(SideItem inSideItem)
        {
            sideItems.Remove(inSideItem);
        }

        public decimal CalculateTotalPrice()
        {
            decimal totalPrice = Price;

            foreach (SideItem sideItem in sideItems)
            {
                totalPrice += sideItem.Price;
            }

            return totalPrice;
        }
    }
}
