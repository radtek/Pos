using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using WebMateTestProgram.WebMateServiceReference;

namespace WebMateTestProgram
{
    struct ItemAttr
    {
        public string TPC;   // Third Party Code
        public string Price; // Price
        public string Qty;   // Qty
    };

    //..........................................

    public partial class WebmateClientForm : Form
    {
        /// <summary>
        /// 
        /// </summary>
        public WebmateClientForm()
        {
            InitializeComponent();

            initWebmateClient();
        }

        #region PROTECTED
        int editingItem_;

        WebmateClientItems itemForm_;

        /// <summary>
        /// 
        /// </summary>
        protected void initWebmateClient()
        {
            clearEditingComment();
            resetEditingItem();

            itemForm_ = new WebmateClientItems();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebmateURL"></param>
        private void resetWebmateURL(string inWebmateURL)
        {
            WebmateClientInterface.Instance.Reset(
                        @"basicHttpBinding_ServiceWebMate", 
                        inWebmateURL);
        }

        /// <summary>
        /// 
        /// </summary>
        protected void buildAndSendOrder(
                            string inWebSiteName,
                            string inStoreName)
        {
            try
            {
                WebmateClientInterface webMate = WebmateClientInterface.Instance;
                string orderHandle = webMate.OpenOrder();

                setWebOrderHeaderSection(orderHandle);
                setWebOrderFromSection(orderHandle);
                setWebOrderAccountSection(orderHandle);
                commitWebOrder(orderHandle);

                getAndShowWebOrderStatus(
                   orderHandle, 
                   inWebSiteName,
                   inStoreName);

                //MessageBox.Show(orderHandle);
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message);
            }

            resetEditingItem();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private bool buildAndSendGetStatus(
                          string inOrderHandle,
                          string inWebSiteName,
                          string inStoreName,
                          out WebOrderStatus outCommittedOrderStatus)
        {
            bool successful = true;
            try
            {
                WebmateClientInterface webMate = WebmateClientInterface.Instance;

                webMate.GetCommittedOrderStatus(
                               inWebSiteName,
                               inStoreName,
                               inOrderHandle,
                           out outCommittedOrderStatus);
            }
            catch (Exception exc)
            {
                successful = false;
                outCommittedOrderStatus = WebOrderStatus.Unknown;

                //MessageBox.Show(exc.Message);
            }

            return successful;
        }

        /// <summary>
        /// 
        /// </summary>
        protected void getAndShowWebOrderStatus(
                          string inOrderHandle,
                          string inWebSiteName,
                          string inStoreName)
        {
           WebOrderStatus orderStatus;

            buildAndSendGetStatus(
                 inOrderHandle, 
                 inWebSiteName,
                 inStoreName,
                 out orderStatus);

            showWebOrderStatus(
                inOrderHandle, 
                inWebSiteName,
                inStoreName, 
                orderStatus);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="inWebSiteName"></param>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderStatus"></param>
        protected void showWebOrderStatus(
                           string inOrderHandle,
                           string inWebSiteName,
                           string inStoreName,
                           WebOrderStatus inOrderStatus)
        {
            ListViewItem item = new ListViewItem(weborderStatusToString(inOrderStatus));
            item.SubItems.Add(inWebSiteName);
            item.SubItems.Add(inStoreName);
            item.SubItems.Add(inOrderHandle);

            lvOrders.Items.Insert(0, item);
        }

        /// <summary>
        /// 
        /// </summary>
        protected void refreshOrderStatus()
        {
            int orderIndex = 0;

            foreach(ListViewItem item in lvOrders.Items)
            {
                WebOrderStatus orderStatus;
                
                buildAndSendGetStatus(
                 item.SubItems[3].Text, // Order Handle
                 item.SubItems[1].Text, // Website name
                 item.SubItems[2].Text, // Store name
                 out orderStatus);

                refreshOrderStatusAtIndex(orderIndex, orderStatus);

                orderIndex++;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderIndex"></param>
        /// <param name="inOrderStatus"></param>
        protected void refreshOrderStatusAtIndex(
                            int inOrderIndex, 
                            WebOrderStatus inOrderStatus)
        {
            try
            {
                ListViewItem item = lvOrders.Items[inOrderIndex];
                item.Text = weborderStatusToString(inOrderStatus);
            }
            catch (Exception)
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderStatus"></param>
        /// <returns></returns>
        protected string weborderStatusToString(WebOrderStatus inOrderStatus)
        {
            switch (inOrderStatus)
            {
                case WebOrderStatus.Committed:   return @"COMMITTED";
                case WebOrderStatus.Accepted:    return @"ACCEPTED";
                case WebOrderStatus.NotAccepted: return @"NOT ACCEPTED";
                case WebOrderStatus.Incomplete: return @"INCOMPLETE";

                default: return @"UNKNOW ORDER";
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected void clearEditingComment()
        {
            tbComment.Text = @"";
        }

        /// <summary>
        /// 
        /// </summary>
        protected void resetEditingItem()
        {
            editingItem_ = -1;
        }

        /// <summary>
        /// 
        /// </summary>
        protected void addComment()
        {
            tbComment.Text = @"Your comment here ...";
            tbComment.SelectAll();
            tbComment.Focus();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void editComment()
        {
            editingItem_ = lbComments.SelectedIndex;
            tbComment.Text = (string)lbComments.Items[editingItem_];
        }

        /// <summary>
        /// 
        /// </summary>
        protected void delComment()
        {
            if (lbComments.SelectedIndex > -1)
            {
                lbComments.Items.RemoveAt(lbComments.SelectedIndex);
                clearEditingComment();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected void acceptComment()
        {
            if ((tbComment.Text != null) && (!tbComment.Text.Trim().Equals(@"")))
            {
                if (editingItem_ > -1)
                {
                    lbComments.Items[editingItem_] = tbComment.Text;
                    resetEditingItem();
                }
                else
                {
                    lbComments.Items.Add(tbComment.Text);
                }

                tbComment.Focus();
            }
            else
            {
                MessageBox.Show(@"Cannot accept an empty comment");
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected void clearComments()
        {
            lbComments.Items.Clear();
            clearEditingComment();
            resetEditingItem();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void addItem()
        {
            if( showItemForm(true) )
            {
                if (itemForm_.itemChecked) // Add an item (item size)
                {
                    addItemNode(itemForm_.TPC, itemForm_.Price, itemForm_.Qty);
                }
                else // Add an item side
                {
                    addItemSideNode(tvItems.SelectedNode, itemForm_.TPC, itemForm_.Price, itemForm_.Qty);
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected void editItem()
        {
            if (showItemForm(false))
            {
                editItemNode(tvItems.SelectedNode, itemForm_.TPC, itemForm_.Price, itemForm_.Qty);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTPC"></param>
        /// <param name="inPrice"></param>
        /// <param name="inQty"></param>
        protected void addItemNode(
                          string inTPC,
                          string inPrice,
                          string inQty)
        {
            TreeNode node = tvItems.TopNode.Nodes.Add(string.Format(@"TPC:{0}  Price:{1}  Qty:{2}", inTPC, inPrice, inQty));

            ItemAttr attr = new ItemAttr();
            attr.TPC   = inTPC;
            attr.Price = inPrice;
            attr.Qty   = inQty;

            node.Tag = attr;

            tvItems.TopNode.Expand();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTPC"></param>
        /// <param name="inPrice"></param>
        /// <param name="inQty"></param>
        protected void addItemSideNode(
                          TreeNode inItemNode,
                          string inTPC,
                          string inPrice,
                          string inQty)
        {
            TreeNode node = inItemNode.Nodes.Add(string.Format(@"TPC:{0}  Price:{1:###.##}  Qty:{2:###.##}", inTPC, inPrice, inQty));

            ItemAttr attr = new ItemAttr();
            attr.TPC   = inTPC;
            attr.Price = inPrice;
            attr.Qty   = inQty;

            node.Tag = attr;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSideNode"></param>
        /// <param name="inTPC"></param>
        /// <param name="inPrice"></param>
        /// <param name="inQty"></param>
        protected void editItemNode(
                          TreeNode inItemNode,
                          string inTPC,
                          string inPrice,
                          string inQty)
        {
            inItemNode.Text = string.Format(@"TPC:{0}  Price:{1:###.##}  Qty:{2:###.##}", inTPC, inPrice, inQty);

            ItemAttr attr = (ItemAttr)inItemNode.Tag;
            attr.TPC = inTPC;
            attr.Price = inPrice;
            attr.Qty = inQty;

            inItemNode.Tag = attr;
        }

        /// <summary>
        /// 
        /// </summary>
        protected void delItem()
        {
            if (tvItems.SelectedNode == tvItems.TopNode)
            {
                clearItems();

                return;
            }

            if (tvItems.SelectedNode != null && tvItems.SelectedNode != tvItems.TopNode)
            {
                tvItems.Nodes.Remove(tvItems.SelectedNode);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected void clearItems()
        {
            tvItems.TopNode.Nodes.Clear();   
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected bool showItemForm(bool inNewItem)
        {
            TreeNode selectedNode = tvItems.SelectedNode;

            itemForm_.ItemOnly = (selectedNode == null) || (selectedNode == tvItems.TopNode);
            itemForm_.NewItem  = inNewItem;

            if (inNewItem) // Add new item / side
            {
                if (!itemForm_.ItemOnly && selectedNode.Parent == tvItems.TopNode)
                {
                    itemForm_.CheckSideAttr();
                }
                else
                {
                    itemForm_.CheckItemAttr();
                }

                return itemForm_.ShowDialog() == System.Windows.Forms.DialogResult.OK;
            }
            else if (!itemForm_.ItemOnly )  // Edit an existing item / side
            {
                ItemAttr attr = (ItemAttr)selectedNode.Tag;

                itemForm_.TPC   = attr.TPC;
                itemForm_.Price = attr.Price;
                itemForm_.Qty   = attr.Qty;

                if (selectedNode.Parent != tvItems.TopNode)
                {
                    itemForm_.CheckSideAttr();
                }
                else
                {
                    itemForm_.CheckItemAttr();
                }

                return itemForm_.ShowDialog() == System.Windows.Forms.DialogResult.OK;
            }

            return false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outOrderHandle"></param>
        /// <returns></returns>
        private void setWebOrderHeaderSection(string outOrderHandle)
        {
            TreeNode itemNode = tvItems.TopNode.FirstNode;

            WebmateClientInterface.Instance.SaveHeaderSection(outOrderHandle,
                                                              Guid.NewGuid().ToString(),
                                                              calculateTotalPrice(itemNode),
                                                              DateTime.Now,
                                                              Convert.ToDateTime(dtpPickupTime.Text),
                                                              1,
                                                              tbStoreName.Text,
                                                              cbRespond.Checked);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outOrderHandle"></param>
        /// <returns></returns>
        private void setWebOrderFromSection(string outOrderHandle)
        {
            WebmateClientInterface.Instance.SaveFromSection(
                                                outOrderHandle,
                                                tbWebsiteID.Text,
                                                tbWebsiteName.Text);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outOrderHandle"></param>
        /// <returns></returns>
        private void setWebOrderAccountSection(string outOrderHandle)
        {
            WebmateClientInterface webMate = WebmateClientInterface.Instance;
            DTO_WebOrderDelivery delivery = webMate.CreateAccountDelivery(tbRecipient.Text,
                                                                          tbStreetNo.Text,
                                                                          tbStreetName.Text,
                                                                          tbSuburb.Text,
                                                                          tbCity.Text,
                                                                          tbCountry.Text,
                                                                          tbPhoneNo.Text,
                                                                          tbNote.Text);
            DTO_WebOrderPayments payments = webMate.CreateAccountPayments(cbPaymentRequired.Checked);
            webMate.SaveAccountSection(outOrderHandle,
                                       Convert.ToUInt32(numCovers.Value),
                                       tbMemberNumber.Text,
                                       tbCustomerName.Text,
                                       createComments(),
                                       delivery,
                                       payments,
                                       createItems());
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private List<string> createComments()
        {
            List<string> result = new List<string>();

            //.......................................................

            foreach (string comment in lbComments.Items)
            {
                result.Add(comment); 
            }

            //.......................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private List<DTO_WebOrderItem> createItems()
        {
            List<DTO_WebOrderItem> result = new List<DTO_WebOrderItem>();

            //......................................

            TreeNode itemNode = tvItems.TopNode.FirstNode;

            WebmateClientInterface webMate = WebmateClientInterface.Instance;
            DTO_WebOrderItem webOrderItem;

            while(itemNode != null)
            {
                ItemAttr itemAttr = (ItemAttr)itemNode.Tag;
                
                webOrderItem = webMate.CreateAccountOrderItem(1, // Discount ID
                                                              itemAttr.TPC,
                                                              Convert.ToSingle(itemAttr.Qty),
                                                              Convert.ToDecimal(itemAttr.Price),
                                                              new List<DTO_WebOrderItemOption>(),
                                                              createItemSides(itemNode));
                result.Add(webOrderItem);

                itemNode = itemNode.NextNode;
            }

            //......................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private List<DTO_WebOrderItemSide> createItemSides(TreeNode inItemNode)
        {
            List<DTO_WebOrderItemSide> result = new List<DTO_WebOrderItemSide>();

            WebmateClientInterface webMate = WebmateClientInterface.Instance;

            //......................................

            TreeNode sideNode = inItemNode.FirstNode;

            while (sideNode != null)
            {
                ItemAttr sideAttr = (ItemAttr)sideNode.Tag;

                DTO_WebOrderItemSide webOrderItemSide;
                webOrderItemSide = webMate.CreateAccountOrderItemSide(1,  // Discount ID
                                                                      sideAttr.TPC,
                                                                      Convert.ToSingle(sideAttr.Qty),
                                                                      Convert.ToDecimal(sideAttr.Price));
                result.Add(webOrderItemSide);

                sideNode = sideNode.NextNode;
            }

            //......................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outOrderHandle"></param>
        /// <returns></returns>
        private void commitWebOrder(string outOrderHandle)
        {
            WebmateClientInterface.Instance.CommitOrder(outOrderHandle);
        }


        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private decimal calculateTotalPrice(TreeNode inItemNode)
        {
            decimal totalPrice = 0.0M;

            while(inItemNode != null)
            {
                ItemAttr itemAttr = (ItemAttr)inItemNode.Tag;
                totalPrice += Convert.ToInt32(itemAttr.Qty) * Convert.ToDecimal(itemAttr.Price);
                totalPrice += calculateTotalPrice(inItemNode.FirstNode);
                inItemNode = inItemNode.NextNode;
            }

            return totalPrice;
        }
        #endregion

        #region EVENTS
        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnAcceptWebmateURL_Click(object sender, EventArgs e)
        {
            resetWebmateURL(tbWebmateURL.Text);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnClose_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnSendOrder_Click(object sender, EventArgs e)
        {
            buildAndSendOrder(
                 tbWebsiteName.Text,
                 tbStoreName.Text);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnRefreshStatus_Click(object sender, EventArgs e)
        {
            refreshOrderStatus();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnAddComment_Click(object sender, EventArgs e)
        {
            addComment();
        }
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnEditComment_Click(object sender, EventArgs e)
        {
            editComment();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDelComment_Click(object sender, EventArgs e)
        {
            delComment();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnAcceptComment_Click(object sender, EventArgs e)
        {
            acceptComment();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnAddItem_Click(object sender, EventArgs e)
        {
            addItem();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnEditItem_Click(object sender, EventArgs e)
        {
            editItem();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDelItem_Click(object sender, EventArgs e)
        {
            delItem();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnClearComments_Click(object sender, EventArgs e)
        {
            clearComments();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnClearItems_Click(object sender, EventArgs e)
        {
            clearItems();
        }
        #endregion

        private void btnViewOrder_Click(object sender, EventArgs e)
        {
/*
           string orderHandle = getSelectedOrderHandle();
            if (!string.IsNullOrEmpty(orderHandle))
            {
                frmOrderDetails details = new frmOrderDetails(orderHandle);
                details.Show();
            }
            else
                MessageBox.Show("Select an order first");
*/
        }

        private string getSelectedOrderHandle()
        {
            string handle = string.Empty;

            if (lvOrders.SelectedItems.Count > 0)
            {
                ListViewItem selectedItem = lvOrders.Items[0];
                handle = selectedItem.SubItems[3].Text;
            }

            return handle;
        }
    }
}
