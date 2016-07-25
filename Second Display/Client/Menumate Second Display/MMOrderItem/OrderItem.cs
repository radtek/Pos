using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MMOrderItem
{
    public class OrderItem : BaseOrderItem
    {
        #region PUBLIC

        /// <summary>
        /// 
        /// </summary>
        public ItemOption[] ItemOptions 
        {
            get { return itemOptions.ToArray(); } 
        }

        /// <summary>
        /// 
        /// </summary>
        public ItemSide[] ItemSides
        {
            get { return itemSides.ToArray(); }
        }

        /// <summary>
        /// 
        /// </summary>
        public OrderItem() : base()
        {
            itemOptions = new List<ItemOption>();
            itemSides = new List<ItemSide>();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOption"></param>
        public void AddOption(ItemOption inOption)
        {
            itemOptions.Add(inOption);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inIndex"></param>
        public void RemoveOptionAtIndex(int inIndex)
        {
            try
            {
                itemOptions.RemoveAt(inIndex);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public void RemoveAllOptions()
        {
            itemOptions.Clear();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSide"></param>
        public void AddSide(ItemSide inSide)
        {
            itemSides.Add(inSide);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inIndex"></param>
        public void RemoveSideAtIndex(int inIndex)
        {
            try
            {
                itemSides.RemoveAt(inIndex);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public void RemoveAllSides()
        {
            itemSides.Clear();
        }

        #endregion

        #region PRIVATE

        List<ItemOption> itemOptions;
        List<ItemSide> itemSides;

        #endregion
    }
}
