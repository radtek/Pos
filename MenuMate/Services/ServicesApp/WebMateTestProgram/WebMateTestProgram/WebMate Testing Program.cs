using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using WebMateTestProgram.WebMateServiceReference;

// hh:mm tt dd/MM/yyyy

namespace WebMateTestProgram
{
    public partial class WebMateTestingProgram : Form
    {
        private WCFServiceWebMateClient wmClient_;
        private ItemsForm itemsForm;
        private string orderHandle_;

        public WebMateTestingProgram()
        {
            InitializeComponent();
            wmClient_ = new WCFServiceWebMateClient();
            itemsForm = new ItemsForm();
        }

        private bool buildAndSendOrder()
        {
            bool successful = true;
            try
            {
                WebmateClientInterface webMate = WebmateClientInterface.Instance;
                string orderHandle = webMate.OpenOrder();

                setWebOrderHeaderSection(orderHandle);
                setWebOrderFromSection(orderHandle);
                setWebOrderAccountSection(orderHandle);
                commitWebOrder(orderHandle);

                orderHandle_ = orderHandle;

                MessageBox.Show(orderHandle);
            }

            catch (Exception exc)
            {
                successful = false;
                MessageBox.Show(exc.Message);
            }

            return successful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private bool buildAndSendGetStatus(out WebOrderStatus outCommittedOrderStatus)
        {
            bool successful = true;
            try
            {
                WebmateClientInterface webMate = WebmateClientInterface.Instance;

                webMate.GetCommittedOrderStatus(
                               SiteNameBox.Text,
                               StoreNameTextBox.Text,
                               orderHandle_,
                           out outCommittedOrderStatus);
            }

            catch (Exception exc)
            {
                successful = false;
                outCommittedOrderStatus = WebOrderStatus.Unknown;

                MessageBox.Show(exc.Message);
            }

            return successful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outOrderHandle"></param>
        /// <returns></returns>
        private void setWebOrderHeaderSection(string outOrderHandle)
        {
            WebmateClientInterface.Instance.SaveHeaderSection(outOrderHandle, 
                                                              Guid.NewGuid().ToString(), 
                                                              calculateTotalPrice(), 
                                                              DateTime.Now, 
                                                              Convert.ToDateTime(PickupTimeDateTimePicker.Text), 
                                                              1, 
                                                              StoreNameTextBox.Text);
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
                                                Guid.NewGuid().ToString(),
                                                SiteNameBox.Text);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outOrderHandle"></param>
        /// <returns></returns>
        private void setWebOrderAccountSection(string outOrderHandle)
        {
            WebmateClientInterface webMate = WebmateClientInterface.Instance;
            DTO_WebOrderDelivery delivery = webMate.CreateAccountDelivery(RecipientTextBox.Text,
                                                                          StreetNumberTextBox.Text,
                                                                          StreetNameTextBox.Text,
                                                                          SuburbTextBox.Text,
                                                                          CityTextBox.Text,
                                                                          CountryTextBox.Text,
                                                                          PhoneNumberTextBox.Text);
            DTO_WebOrderPayments payments = webMate.CreateAccountPayments(PaymentRequiredCheckBox.Checked);
            webMate.SaveAccountSection(outOrderHandle, 
                                       Convert.ToUInt32(CoversNumeric.Value), 
                                       MemberNumberTextBox.Text,
                                       CustomerNameTextBox.Text,
                                       createComments(),
                                       delivery,
                                       payments,
                                       createItems());
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

        private decimal calculateTotalPrice()
        {
            decimal totalPrice = 0.0M;
            for (int i = 0; i < itemsForm.GetItemsCount(); i++)
            {
                totalPrice += itemsForm.GetItemAtIndex(i).CalculateTotalPrice();
            }
            return totalPrice;
        }
        
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private List<string> createComments()
        {
            List<string> result = new List<string>();

            //.......................................................

            result.Add(CommentsTextBox.Text); //TODO Improve comments GUI

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

            for (int i = 0; i < itemsForm.GetItemsCount(); i++)
            {
                Item item = itemsForm.GetItemAtIndex(i);
                WebmateClientInterface webMate = WebmateClientInterface.Instance;
                DTO_WebOrderItem webOrderItem; 
                webOrderItem = webMate.CreateAccountOrderItem(item.DiscountID,
                                                              item.ThirdPartyCode,
                                                              item.Qty,
                                                              item.Price,
                                                              new List<DTO_WebOrderItemOption>(),
                                                              createItemSides(item));
                result.Add(webOrderItem);
            }

            //......................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private List<DTO_WebOrderItemSide> createItemSides(Item inItem)
        {
            List<DTO_WebOrderItemSide> result = new List<DTO_WebOrderItemSide>();

            WebmateClientInterface webMate = WebmateClientInterface.Instance;

            //......................................

            for (int i = 0; i < inItem.GetSideItemsCount(); i++)
            {
                SideItem sideItem = inItem.GetSideItemAtIndex(i);
                DTO_WebOrderItemSide webOrderItemSide; 
                webOrderItemSide = webMate.CreateAccountOrderItemSide(sideItem.DiscountID,
                                                                      sideItem.ThirdPartyCode,
                                                                      sideItem.Qty,
                                                                      sideItem.Price);
                result.Add(webOrderItemSide);
            }

            //......................................

            return result;
        }

        private void clear()
        {
            StoreNameTextBox.Clear();
            PickupTimeDateTimePicker.ResetText();
            CoversNumeric.Value = 0;
            CustomerNameTextBox.Clear();
            MemberNumberTextBox.Clear();
            CommentsTextBox.Clear();
            RespondCheckBox.Checked = false;
            PaymentRequiredCheckBox.Checked = false;

        }

        private void SendButton_Click(object sender, EventArgs e)
        {
            if (buildAndSendOrder())
            {
                MessageBox.Show("Success");
                //clear();
            }
        }

        private void ItemsButton_Click(object sender, EventArgs e)
        {
            itemsForm.ShowDialog(this);
        }

        private void CheckOrderStatusButton_Click(object sender, EventArgs e)
        {
            WebOrderStatus committedOrderStatus;

            if (buildAndSendGetStatus(out committedOrderStatus))
            {
                switch (committedOrderStatus)
                {
                    case WebOrderStatus.Committed:   MessageBox.Show(string.Format(@"Order {0} committed", orderHandle_)); break;
                    case WebOrderStatus.NotAccepted: MessageBox.Show(string.Format(@"Order {0} not accepted", orderHandle_)); break;
                    case WebOrderStatus.Accepted:    MessageBox.Show(string.Format(@"Order {0} accepted", orderHandle_)); break;
                    case WebOrderStatus.Unknown:     MessageBox.Show(string.Format(@"Order {0} unknown", orderHandle_)); break;
                }
                //clear(); 
            }
        }
    }
}
