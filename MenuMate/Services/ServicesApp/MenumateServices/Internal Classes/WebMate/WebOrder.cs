using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;
using System.Globalization;

using XMLManager;
using MenumateServices.WebMate.DTO;

namespace MenumateServices.WebMate.InternalClasses
{
    public class WebOrder
    {
        private static string DATE_TIME_FORMAT_STRING = "yyyy-MM-dd HH:mm:ss";

        Guid _handle;

        DTO_WebOrderHeader  _headerSection;
        DTO_WebOrderFrom    _fromSection;
        DTO_WebOrderAccount _accountSection;

        WebOrderCommit web_order_commit;
        XmlDocument    xml_doc;

        public WebOrder()
        {
            _handle = initDBWebOrder();
        }

        #region PUBLIC
        public string Handle
        {
            get { return _handle.ToString(); }
        }

        /// <summary>
        /// 
        /// </summary>
        public DTO_WebOrderHeader EmptyHeaderSection
        {
            get { return getEmptyHeaderSection(); }
        }

        /// <summary>
        /// 
        /// </summary>
        public DTO_WebOrderFrom EmptyFromSection
        {
            get { return getEmptyFromSection(); }
        }

        /// <summary>
        /// 
        /// </summary>
        public DTO_WebOrderAccount EmptyAccountSection
        {
            get { return getEmptyAccountSection(); }
        }
        
        /// <summary>
        /// 
        /// </summary>
        public DTO_WebOrderHeader HeaderSection
        {
            get { return getHeaderSection(); }
            set { setHeaderSection(value);   }
        }

        /// <summary>
        /// 
        /// </summary>
        public DTO_WebOrderFrom FromSection
        {
            get { return getFromSection(); }
            set { setFromSection(value);   }
        }
    
        /// <summary>
        /// 
        /// </summary>
        public DTO_WebOrderAccount AccountSection
        {
            get { return getAccountSection(); }
            set { setAccountSection(value);   }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public void Open()
        {
            web_order_commit.OpenOrder(this.Handle);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        public void Commit()
        {
            web_order_commit.CommitOrder(this);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        public void Load(string inOrderHandle)
        {
            web_order_commit.LoadOrder(inOrderHandle, this);
        }

        /// <summary>
        /// IS BEING NOT USED
        /// </summary>
        public void Remove()
        {
            web_order_commit.RemoveOrder(this);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileURI"></param>
        public void SaveXML(string inFileURI)
        {
            xml_doc.Save(inFileURI);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHeaderNode"></param>
        public void SetCompleteHeaderSection(XmlNode inHeaderNode)
        {
            string defaulDateTimeStr = DateTime.Now.ToString(DATE_TIME_FORMAT_STRING);

            string orderDateStr = getXMLOrderAttribute(inHeaderNode, @"orderDate", defaulDateTimeStr);
            string expectedDateStr = getXMLOrderAttribute(inHeaderNode, @"expectedDate", defaulDateTimeStr);

            //...............................................................

            _headerSection.StoreName = getXMLOrderAttribute(inHeaderNode, @"storeName", @"");
            _headerSection.GUID = getXMLOrderAttribute(inHeaderNode, @"guid", Guid.Empty.ToString());
            _headerSection.Respond = Convert.ToBoolean(getXMLOrderAttribute(inHeaderNode, @"respond", "false"));
            _headerSection.Scheduled = Convert.ToUInt16(getXMLOrderAttribute(inHeaderNode, @"schedule", @"1"));
            _headerSection.OrderTotal = Convert.ToDecimal(getXMLOrderAttribute(inHeaderNode, @"orderTotal", "0.00"));
            _headerSection.OrderDate    = DateTime.Parse(orderDateStr, CultureInfo.CreateSpecificCulture("en-NZ"));
            _headerSection.ExpectedDate = DateTime.Parse(expectedDateStr, CultureInfo.CreateSpecificCulture("en-NZ"));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHeaderNode"></param>
        public void SetCompleteFromSection(XmlNode inFromNode)
        {
            _fromSection.SiteID = getXMLOrderAttribute(inFromNode, @"siteID", "");
            _fromSection.SiteName = getXMLOrderAttribute(inFromNode, @"siteName", "");
        }
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHeaderNode"></param>
        public void SetCompleteAccountSection(XmlNode inAccountNode)
        {
            _accountSection.Covers = Convert.ToUInt32(getXMLOrderAttribute(inAccountNode, @"covers", @"0"));
            _accountSection.Name = getXMLOrderAttribute(inAccountNode, @"name", @"");
            _accountSection.MemberNumber = getXMLOrderAttribute(inAccountNode, @"memberNumber", @"");
            

            //............................................................

            _accountSection.Delivery = new DTO_WebOrderDelivery();
            _accountSection.Delivery.Recipient = @"";
            _accountSection.Delivery.StreetNo = @"";
            _accountSection.Delivery.StreetName = @"";
            _accountSection.Delivery.Suburb = @"";
            _accountSection.Delivery.City = @"";
            _accountSection.Delivery.Country = @"";
            _accountSection.Delivery.Note = @"";
            _accountSection.Delivery.Phone = @"";            

            List<string> commentList = new List<string>();            
            List<DTO_WebOrderItem> itemList = new List<DTO_WebOrderItem>();

            _accountSection.Comments = commentList.ToArray();
            _accountSection.Email = getXMLOrderAttribute(inAccountNode, @"email", @"");
            _accountSection.OrderType = getXMLOrderAttribute(inAccountNode, @"ordertype", @"");
            _accountSection.Payments = new DTO_WebOrderPayments();
            _accountSection.OrderItems = itemList.ToArray();

            //............................................................

            readWebOrderDeliveryComplete(inAccountNode.FirstChild, _accountSection);
            readWebOrderPaymentsComplete(inAccountNode.FirstChild.NextSibling, _accountSection);
            readWebOrderCommentsComplete(inAccountNode.FirstChild.NextSibling.NextSibling, _accountSection);
            readWebOrderItemsComplete(inAccountNode.FirstChild.NextSibling.NextSibling.NextSibling, _accountSection);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHeaderNode"></param>
        public DTO_WebOrderHeader GetCompleteHeaderSection()
        {
            return _headerSection;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHeaderNode"></param>
        public DTO_WebOrderFrom GetCompleteFromSection()
        {
            return _fromSection;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHeaderNode"></param>
        public DTO_WebOrderAccount GetCompleteAccountSection()
        {
            return _accountSection;
        }

        #endregion

        #region INTERNAL
        /// <summary>
        /// 
        /// </summary>
        internal void SetHandle(string inHandleStr)
        {
            _handle = Guid.Parse(inHandleStr);
        }
        #endregion

        #region PRIVATE
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        Guid initDBWebOrder()
        {
            _headerSection  = getEmptyHeaderSection();
            _fromSection    = getEmptyFromSection();
            _accountSection = getEmptyAccountSection();

            //...............................................

            web_order_commit = new WebOrderCommit();
            xml_doc = new XmlDocument();

            //...............................................

            return Guid.NewGuid();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DTO_WebOrderHeader getEmptyHeaderSection()
        {
            DTO_WebOrderHeader result = new DTO_WebOrderHeader();

            //..................................

            result.StoreName    = @"";
            result.GUID         = Guid.Empty.ToString();
            result.Scheduled    = 0;
            result.Respond      = true;
            result.OrderTotal   = 0;
            result.ExpectedDate = DateTime.Now;
            result.OrderDate    = DateTime.Now;

            //..................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DTO_WebOrderFrom getEmptyFromSection()
        {
            DTO_WebOrderFrom result = new DTO_WebOrderFrom();

            //..................................

            result.SiteID      = Guid.Empty.ToString();
            result.SiteName = @"";

            //..................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DTO_WebOrderAccount getEmptyAccountSection()
        {
            DTO_WebOrderAccount result = new DTO_WebOrderAccount();

            //..................................

            result.Covers       = 0;
            result.Name         = @"";
            result.MemberNumber = @"";

            result.Delivery = new DTO_WebOrderDelivery();
            result.Delivery.Recipient  = @"";
            result.Delivery.StreetNo   = @"";
            result.Delivery.StreetName = @"";
            result.Delivery.Suburb     = @"";
            result.Delivery.City       = @"";
            result.Delivery.Country    = @"";
            result.Delivery.Note       = @"";
            result.Delivery.Phone      = @"";

            List<string> commentList = new List<string>();
            List<DTO_WebOrderItem> itemList = new List<DTO_WebOrderItem>();

            result.Comments   = commentList.ToArray();
            result.Payments   = new DTO_WebOrderPayments();
            result.OrderItems = itemList.ToArray();
            result.Email = @"";
            result.OrderType = @"";

            //..................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DTO_WebOrderHeader getHeaderSection()
        {
            try
            {
                web_order_commit.LoadHeaderSection(_handle.ToString(), _headerSection);

                return _headerSection;
            }
            catch 
            {
                return EmptyHeaderSection;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DTO_WebOrderFrom getFromSection()
        {
            try
            {
                web_order_commit.LoadFromSection(_handle.ToString(), _fromSection);

                return _fromSection;
            }
            catch
            {
                return EmptyFromSection;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DTO_WebOrderAccount getAccountSection()
        {
            try
            {
                web_order_commit.LoadAccountSection(_handle.ToString(), _accountSection);

                return _accountSection;
            }
            catch
            {
                return EmptyAccountSection;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHeader"></param>
        /// <returns></returns>
        void setHeaderSection(DTO_WebOrderHeader inOrderHeader)
        {
            _headerSection.StoreName    = inOrderHeader.StoreName;
            _headerSection.GUID         = inOrderHeader.GUID;
            _headerSection.Scheduled    = inOrderHeader.Scheduled;
            _headerSection.Respond      = inOrderHeader.Respond;
            _headerSection.OrderTotal   = inOrderHeader.OrderTotal;
            _headerSection.ExpectedDate = inOrderHeader.ExpectedDate;
            _headerSection.OrderDate    = inOrderHeader.OrderDate;

            web_order_commit.SetHeaderSection(_handle.ToString(), _headerSection);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderFrom"></param>
        /// <returns></returns>
        void setFromSection(DTO_WebOrderFrom inOrderFrom)
        {
            _fromSection.SiteID      = inOrderFrom.SiteID;
            _fromSection.SiteName = inOrderFrom.SiteName;

            web_order_commit.SetFromSection(_handle.ToString(), inOrderFrom);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="intOrderAccount"></param>
        /// <returns></returns>
        void setAccountSection(DTO_WebOrderAccount intOrderAccount)
        {
            _accountSection.Covers                   = intOrderAccount.Covers;
            _accountSection.Name                     = intOrderAccount.Name;
            _accountSection.MemberNumber             = intOrderAccount.MemberNumber;
            _accountSection.Comments                 = intOrderAccount.Comments;
            _accountSection.Payments.PaymentRequired = intOrderAccount.Payments.PaymentRequired;
            _accountSection.OrderItems               = intOrderAccount.OrderItems;

            _accountSection.Delivery.Recipient  = intOrderAccount.Delivery.Recipient;
            _accountSection.Delivery.StreetNo   = intOrderAccount.Delivery.StreetNo;
            _accountSection.Delivery.StreetName = intOrderAccount.Delivery.StreetName;
            _accountSection.Delivery.Suburb     = intOrderAccount.Delivery.Suburb;
            _accountSection.Delivery.City       = intOrderAccount.Delivery.City;
            _accountSection.Delivery.Country    = intOrderAccount.Delivery.Country;
            _accountSection.Delivery.Note       = intOrderAccount.Delivery.Note;
            _accountSection.Delivery.Phone      = intOrderAccount.Delivery.Phone;
            _accountSection.Email               = intOrderAccount.Email;
            _accountSection.OrderType           = intOrderAccount.OrderType;

            web_order_commit.SetAccountSection(_handle.ToString(), intOrderAccount);
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDeliverySectionNode"></param>
        /// <param name="outOrderHeaderAccount"></param>
        void readWebOrderDeliveryComplete(
                    XmlNode inDeliverySectionNode,
                    DTO_WebOrderAccount outOrderHeaderAccount)
        {
            outOrderHeaderAccount.Delivery.Recipient =
                    getXMLOrderAttribute(inDeliverySectionNode, @"recipient", @"");
            outOrderHeaderAccount.Delivery.StreetNo =
                    getXMLOrderAttribute(inDeliverySectionNode, @"streetNo", @"");
            outOrderHeaderAccount.Delivery.StreetName =
                    getXMLOrderAttribute(inDeliverySectionNode, @"streetName", @"");
            outOrderHeaderAccount.Delivery.Suburb =
                    getXMLOrderAttribute(inDeliverySectionNode, @"suburb", @"");
            outOrderHeaderAccount.Delivery.City =
                    getXMLOrderAttribute(inDeliverySectionNode, @"city", @"");
            outOrderHeaderAccount.Delivery.Country =
                    getXMLOrderAttribute(inDeliverySectionNode, @"country", @"");
            outOrderHeaderAccount.Delivery.Note =
                    getXMLOrderAttribute(inDeliverySectionNode, @"note", @"");
            outOrderHeaderAccount.Delivery.Phone =
                    getXMLOrderAttribute(inDeliverySectionNode, @"phone", @"");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPaymentsSectionNode"></param>
        /// <param name="outOrderHeaderAccount"></param>
        void readWebOrderPaymentsComplete(
                    XmlNode inPaymentsSectionNode,
                    DTO_WebOrderAccount outOrderHeaderAccount)
        {
            outOrderHeaderAccount.Payments.PaymentRequired =
                    stringToBool(getXMLOrderAttribute(inPaymentsSectionNode, @"required", @"yes"));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCommentsSectionNode"></param>
        /// <param name="outOrderHeaderAccount"></param>
        void readWebOrderCommentsComplete(
                    XmlNode inCommentsSectionNode,
                    DTO_WebOrderAccount outOrderHeaderAccount)
        {
            XmlNodeList commentNodeList = XMLDocManager.GetNodeList(inCommentsSectionNode, @"Comment");

            List<string> commentList = new List<string>();
            foreach (XmlNode commentNode in commentNodeList)
            {
                commentList.Add(commentNode.InnerText);
            }

            outOrderHeaderAccount.Comments = commentList.ToArray();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemsSectionNode"></param>
        /// <param name="outOrderHeaderAccount"></param>
        void readWebOrderItemsComplete(
                    XmlNode inItemsSectionNode,
                    DTO_WebOrderAccount outOrderHeaderAccount)
        {
            XmlNodeList itemNodeList = XMLDocManager.GetNodeList(inItemsSectionNode, @"Item");

            List<DTO_WebOrderItem> itemList = new List<DTO_WebOrderItem>();
            foreach (XmlNode itemNode in itemNodeList)
            {
                DTO_WebOrderItem item = new DTO_WebOrderItem();

                item.UID = getXMLOrderAttribute(itemNode, @"uid", @"");
                item.BasePrice = Convert.ToDecimal(getXMLOrderAttribute(itemNode, @"basePrice", @"0.00"));
                item.Qty = Convert.ToSingle(getXMLOrderAttribute(itemNode, @"quantity", @"0.0"));
                item.DiscountID = Convert.ToUInt32(getXMLOrderAttribute(itemNode, @"discountID", @"0"));
                item.Note = getXMLOrderAttribute(itemNode, @"note", @""); 

                item.ItemOptions = readWebOrderItemOptionsComplete(itemNode.FirstChild); // Options node
                item.ItemSides = readWebOrderItemSidesComplete(itemNode.FirstChild.NextSibling); // Sides node

                itemList.Add(item);
            }

            outOrderHeaderAccount.OrderItems = itemList.ToArray();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemOptionsNode"></param>
        /// <returns></returns>
        DTO_WebOrderItemOption[] readWebOrderItemOptionsComplete(XmlNode inItemOptionsNode)
        {
            XmlNodeList itemNodeList = XMLDocManager.GetNodeList(inItemOptionsNode, @"Option");

            List<DTO_WebOrderItemOption> itemOptionList = new List<DTO_WebOrderItemOption>();
            foreach (XmlNode optionNode in inItemOptionsNode)
            {
                DTO_WebOrderItemOption option = new DTO_WebOrderItemOption();

                option.UID = getXMLOrderAttribute(optionNode, @"uid", @"");
                option.BasePrice = Convert.ToDecimal(getXMLOrderAttribute(optionNode, @"basePrice", @"0.00"));
                option.Qty = Convert.ToSingle(getXMLOrderAttribute(optionNode, @"quantity", @"0.0"));
                option.DiscountID = Convert.ToUInt32(getXMLOrderAttribute(optionNode, @"discountID", @"0"));
                option.Note = getXMLOrderAttribute(optionNode, @"note", @"");

                itemOptionList.Add(option);
            }

            return itemOptionList.ToArray();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemSidesNode"></param>
        /// <returns></returns>
        DTO_WebOrderItemSide[] readWebOrderItemSidesComplete(XmlNode inItemSidesNode)
        {
            XmlNodeList itemNodeList = XMLDocManager.GetNodeList(inItemSidesNode, @"Option");

            List<DTO_WebOrderItemSide> itemSideList = new List<DTO_WebOrderItemSide>();
            foreach (XmlNode sideNode in inItemSidesNode)
            {
                DTO_WebOrderItemSide side = new DTO_WebOrderItemSide();

                side.UID = getXMLOrderAttribute(sideNode, @"uid", @"");
                side.BasePrice = Convert.ToDecimal(getXMLOrderAttribute(sideNode, @"basePrice", @"0.00"));
                side.Qty = Convert.ToSingle(getXMLOrderAttribute(sideNode, @"quantity", @"0.0"));
                side.DiscountID = Convert.ToUInt32(getXMLOrderAttribute(sideNode, @"discountID", @"0"));
                side.Note = getXMLOrderAttribute(sideNode, @"note", @"");

                itemSideList.Add(side);
            }

            return itemSideList.ToArray();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inNode"></param>
        /// <param name="inAttrName"></param>
        /// <param name="inDefaultValue"></param>
        /// <returns></returns>
		string getXMLOrderAttribute
		(
			XmlNode inNode,
			string inAttrName,
			string inDefaultValue)
        {
			string returnValue = inDefaultValue;
			try
			{
				returnValue = XMLDocManager.GetAttribute(inNode, inAttrName, inDefaultValue);
			}
			catch (Exception ex)
			{
				
			}
			return returnValue;
		}
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inValue"></param>
        /// <returns></returns>
        string boolToString(bool inValue)
        {
            return inValue ? "true" : "false";
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inValue"></param>
        /// <returns></returns>
        bool stringToBool(string inValue)
        {
            return inValue.ToUpper() == @"TRUE";
        }
        #endregion
    }
}
