using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WebMateTestProgram
{
    public partial class WebmateClientItems : Form
    {
        bool newItem_;
        bool itemOnly_;

        /// <summary>
        /// 
        /// </summary>
        public WebmateClientItems()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 
        /// </summary>
        public bool NewItem 
        {
            get { return newItem_; }
            set { setNewItem(value); }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool ItemOnly
        {
            get { return itemOnly_; }
            set { setItemOnly(value); }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool itemChecked
        {
            get { return rbItemAttr.Checked;  }
        }

        /// <summary>
        /// 
        /// </summary>
        public string TPC
        {
            get { return tbThirdPartyCode.Text; }
            set { setTPC(value); }

        }
            
        /// <summary>
        /// 
        /// </summary>
        public string Price 
        {
            get { return tbPrice.Text; }
            set { setPrice(value); }
        }
        
        /// <summary>
        /// 
        /// </summary>
        public string Qty
        {
            get { return tbQuantity.Text; }
            set { setQty(value); }
        }

        /// <summary>
        /// 
        /// </summary>
        public void CheckItemAttr()
        {
            rbItemAttr.Checked = true;
            refreshItemLabel();
        }

        /// <summary>
        /// 
        /// </summary>
        public void CheckSideAttr()
        {
            rbSideAttr.Checked = true;
            refreshItemLabel();
        }
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inNewItem"></param>
        void setNewItem(bool inNewItem)
        {
            newItem_ = inNewItem;
            refreshItemLabel();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inNewItem"></param>
        void setItemOnly(bool inItemOnly)
        {
            itemOnly_ = inItemOnly;

            rbSideAttr.Enabled = !itemOnly_;
            if (!rbSideAttr.Enabled)
            {
                rbItemAttr.Checked = true;
            }

            refreshItemLabel();
        }
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTPC"></param>
        void setTPC(string inTPC)
        {
            tbThirdPartyCode.Text = inTPC;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPrice"></param>
        void setPrice(string inPrice)
        {
            tbPrice.Text = inPrice;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inQty"></param>
        void setQty(string inQty)
        {
            tbQuantity.Text = inQty;
        }

        /// <summary>
        /// 
        /// </summary>
        void refreshItemLabel()
        {
            if(newItem_)
            {
                lbItem.Text = rbItemAttr.Checked ? @"New Item" : @"New Side";
            }
            else
            {
                lbItem.Text = rbItemAttr.Checked ? @"Edit Item" : @"Edit Side";
            }           
        }
    }
}
