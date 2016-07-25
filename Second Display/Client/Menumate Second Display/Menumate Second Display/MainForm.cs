using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Windows.Forms;

using System.Xml;

using MMExtensions;

using MMOrderItemsManager;
using MMOrderItemsController;
using MMOrderItemsController.EventArgs;

using MMChangingImageManager;
using MMChangingImageController;
using MMChangingImageController.EventArgs;

using XMLManager;
using System.Xml.Linq;
using System.Globalization;

namespace Menumate_Second_Display
{
    public partial class MainForm : Form
    {
        const double CONVERSIONFACTOR = 3.58;
        enum OrderItemType { ServingCourse, Item, Option, Side }
        private System.Windows.Forms.Timer _timer;
        private string _currencySymbol;
        private double _setTextFontSize;
        private double _ratio = 1;
        private int _orderPanelHeight = 0;       
        

        public double SetTextFontSize
        {
            get
            {
                return _setTextFontSize;
            }
            set
            {
                if (value == 0)
                    _ratio = 1;
                else
                    _ratio = value / 7.5;
                _setTextFontSize = value;
            }
        }


        public MainForm()
        {
            InitializeComponent();
            _timer = new System.Windows.Forms.Timer();
            _timer.Tick += new EventHandler(TimerTickEventHanlder);
            _timer.Interval = 5000;
            this.Resize += new EventHandler(MainForm_Resize);
        }

        void MainForm_Resize(object sender, EventArgs e)
        {
            if (_ratio > 0)
            {
                RefreshView();
            }

        }

        #region PRIVATE

        string last_order_handle;

        OrderItemsManager order_items_manager;
        ChangingImageManager changing_image_manager;

        const int ITEM_HEIGHT = 25;
        int TOTALS_PANEL_HEIGHT = 127;

        public delegate void CloseCustomerDisplay();
        public delegate void ResetCustomerDisplay();
        public delegate void RefreshOrder();
        public delegate void RefreshBckImage();

        Image bck_image;
        XmlDocument order_xml;

        private void MainForm_Load(object sender, EventArgs e)
        {
            initView();

            resetImageAnimation(1000);
            startImageAnimation();

            resetOrderItemsRefreshInterval(500);
            startOrderItemsRefresh();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Shown(object sender, EventArgs e)
        {
        }

        /// <summary> 
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tmImageAnimation_Tick(object sender, EventArgs e)
        {
            //pnAnimateImage.BackgroundImage = changeOpacity(pnAnimateImage, imgOpacity);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void pnLogo_Click(object sender, EventArgs e)
        {
            Close();
        }        
        /// <summary>
        /// 
        /// </summary>
        private void initView()
        {
            showInSecondDisplay();

            gbOrderItems.Height = spDisplay.Panel2.Height - TOTALS_PANEL_HEIGHT;//Convert.ToInt32(TOTALS_PANEL_HEIGHT * _ratio); //TOTALS_PANEL_HEIGHT;
            //_orderPanelHeight = gbOrderItems.Height;
            // kept last panel height..
            _orderPanelHeight = spDisplay.Panel2.Height;

            //::::::::::::::::::::::::::::::::::::::::::::::::::
            // Receipt area's width = 3/4 * Total screen's width
            spDisplay.SplitterDistance = Convert.ToInt32(Math.Round((3.0 / 4.0) * spDisplay.Width));           
            //::::::::::::::::::::::::::::::::::::::::::::::::::
            //pnLogo.Left = gbOrderItemTotals.Left;
            //pnLogo.Left += gbOrderItemTotals.Left / 2;
            changing_image_manager = new ChangingImageManager();
            changing_image_manager.SetImageChangedHandler(onImageChanged, ChangingImageManager.ImageManagerType.Folder);
            changing_image_manager.UpdateImages(ChangingImageManager.ImageManagerType.Folder);

            //::::::::::::::::::::::::::::::::::::::::::::::::::

            order_items_manager = new OrderItemsManager();
            order_items_manager.SetCloseCustomerDisplayHandler(onCloseCustomerDisplay, OrderItemsManager.OrderManagerType.MenumatePOS);
            order_items_manager.SetResetCustomerDisplayHandler(onResetCustomerDisplay, OrderItemsManager.OrderManagerType.MenumatePOS);
            order_items_manager.SetOrderItemsUpdatedHandler(onOrderUpdated, OrderItemsManager.OrderManagerType.MenumatePOS);

            //::::::::::::::::::::::::::::::::::::::::::::::::::

            order_xml = new XmlDocument();
        }

        /// <summary>
        /// 
        /// </summary>
        private void showInSecondDisplay()
        {
            var primaryDisplay = Screen.AllScreens.ElementAtOrDefault(0);
            var extendedDisplay = Screen.AllScreens.FirstOrDefault(s => s != primaryDisplay) ?? primaryDisplay;

            this.Left = extendedDisplay.WorkingArea.X;
            this.Top = extendedDisplay.WorkingArea.Y;

            this.Width = extendedDisplay.WorkingArea.Width;
            this.Height = extendedDisplay.WorkingArea.Height;

            //this.Left = extendedDisplay.WorkingArea.Left + (extendedDisplay.Bounds.Size.Width / 2) - (this.Size.Width / 2);
            //this.Top  = extendedDisplay.WorkingArea.Top  + (extendedDisplay.Bounds.Size.Height / 2) - (this.Size.Height / 2);

            // There is no Second Display
            if (extendedDisplay == primaryDisplay)
            {
                Close();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInterval"></param>
        private void resetImageAnimation(int inInterval)
        {
            changing_image_manager.SetRefreshInterval(inInterval, ChangingImageManager.ImageManagerType.Folder);
        }

        /// <summary>
        /// 
        /// </summary>
        private void startImageAnimation()
        {
            changing_image_manager.StartRefresh(ChangingImageManager.ImageManagerType.Folder);
        }

        /// <summary>
        /// 
        /// </summary>
        private void stopImageAnimation()
        {
            changing_image_manager.StopRefresh(ChangingImageManager.ImageManagerType.Folder);
        }

        Bitmap changeOpacity(Image inImg, float inOpacity)
        {
            Bitmap result = new Bitmap(inImg.Width, inImg.Height);

            //::::::::::::::::::::::::::::::::::::::::::::::::

            //::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInterval"></param>
        private void resetOrderItemsRefreshInterval(int inInterval)
        {
            order_items_manager.SetRefreshInterval(inInterval, OrderItemsManager.OrderManagerType.MenumatePOS);
        }

        /// <summary>
        /// 
        /// </summary>
        private void startOrderItemsRefresh()
        {
            order_items_manager.StartRefresh(OrderItemsManager.OrderManagerType.MenumatePOS);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void onImageChanged(object sender, ChangingImageEventArgs args)
        {
            bck_image = args.ShowingImage;

            RefreshBckImage delegateRefreshBckImage = refreshBckImage;
            this.Invoke(new MethodInvoker(delegateRefreshBckImage));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void onCloseCustomerDisplay(object sender, EventArgs args)
        {
            CloseCustomerDisplay delegateCloseCustomerDisplay = closeCustomerDisplay;
            this.Invoke(new MethodInvoker(delegateCloseCustomerDisplay));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void onResetCustomerDisplay(object sender, EventArgs args)
        {
            ResetCustomerDisplay delegateResetCustomerDisplay = resetCustomerDisplay;
            this.Invoke(new MethodInvoker(delegateResetCustomerDisplay));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void onOrderUpdated(object sender, OrderItemsUpdatedEventArgs args)
        {
            string xmlStr = args.POSOrder.AsString();
            order_xml.LoadXml(xmlStr);

            RefreshOrder delegateRefresh = refreshOrder;
            this.Invoke(new MethodInvoker(delegateRefresh));
        }

        /// <summary>
        /// 
        /// </summary>
        private void refreshBckImage()
        {
            pnAnimateImage.BackgroundImage = bck_image;
        }

        /// <summary>
        /// 
        /// </summary>
        private void closeCustomerDisplay()
        {
            Close();
        }

        /// <summary>
        /// 
        /// </summary>
        private void resetCustomerDisplay()
        {
            clearOrderItemsContainer();
            clearOrderTotalsContainer();
            ClearBottomTextPanel();

        }

        /// <summary>
        /// 
        /// </summary>
        private void refreshOrder()
        {
            string orderHandle = getOrderHandle(order_xml);

            if (orderHandleError(orderHandle))
            {
                if (orderHandle != last_order_handle)
                {
                    showError(order_xml);
                }
            }
            else
            {
                showOrder(order_xml);
            }

            last_order_handle = orderHandle;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderXML"></param>
        private void showError(XmlDocument inOrderXML)
        {
            clearOrderItemsContainer();
            clearOrderTotalsContainer();
            ClearBottomTextPanel();

            XmlNodeList servingCourseList = getServingCourseList(order_xml);
            showServingCourses(servingCourseList);

            showOrderItemsContainer();
            showOrderTotalsContainer();
        }
        public bool ImageExist()
        {
            bool ifExist = false;
            var exeutingFolder = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            string logoimagepath = Path.Combine(exeutingFolder, "images");


            var directory = new DirectoryInfo(logoimagepath);
            var allowedExtensions = new string[] { ".jpg", ".bmp" };

            var imageFiles = from file in directory.EnumerateFiles("*", SearchOption.AllDirectories)
                             where allowedExtensions.Contains(file.Extension.ToLower())
                             select file;

            foreach (var file in imageFiles)
            {
                string abc = file.ToString();
                if (abc != "FirstVisit.jpg" && abc != "BIRTHDAY.jpg" && abc != "1.jpg" && abc != "2.jpg" && abc != "3.jpg")
                    ifExist = true;
            }

            return ifExist;
        }

        int counter = 0;
        public void LoyaltyImagechng(XmlDocument inOrderXML)
        {
            bool CheckImageExist = ImageExist();
            FileInfo fileInfo;
            string logoimagepath;
            // pnAnimateImage.BackgroundImage = Properties.Resources.Ruby_Tuesday_Med_Pasta;
            var exeutingFolder = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);

            string firstVisitBonusValue = getLoyaltyValue(inOrderXML, "LoyaltyUser/FirstVisitBonus");
            string birthDayBonusValue = getLoyaltyValue(inOrderXML, "LoyaltyUser/BirthDayBonus ");
            string tierIncreaseValue = getLoyaltyValue(inOrderXML, "LoyaltyUser/TierIncrease");

            if (firstVisitBonusValue == "true")
            {
                logoimagepath = Path.Combine(exeutingFolder, "images\\FirstVisit\\FirstVisit.jpg");
                fileInfo = new FileInfo(logoimagepath);
                if (fileInfo.Exists)
                {
                    pnAnimateImage.BackgroundImage = Image.FromFile(logoimagepath);
                    stopImageAnimation();
                }

            }

            else if (birthDayBonusValue == "true")
            {
                logoimagepath = Path.Combine(exeutingFolder, "images\\Birthday\\BIRTHDAY.jpg");
                fileInfo = new FileInfo(logoimagepath);
                if (fileInfo.Exists)
                {
                    pnAnimateImage.BackgroundImage = Image.FromFile(logoimagepath);
                    stopImageAnimation();
                }
            }

            else
            {
                switch (Convert.ToInt32(tierIncreaseValue))
                {
                    case 0:
                        // logoimagepath = Path.Combine(exeutingFolder, "images\\Tiers\\Ruby_Tuesday_Med_Pasta.jpg");
                        //fileInfo = new FileInfo(logoimagepath);
                        // if (fileInfo.Exists)
                        // {
                        // pnAnimateImage.BackgroundImage = Image.FromFile(logoimagepath);
                        if (!CheckImageExist)
                        {
                            //pnAnimateImage.BackgroundImage = Properties.Resources.Ruby_Tuesday_Med_Pasta;

                        }
                        startImageAnimation();
                        // }

                        break;

                    case 1:
                        logoimagepath = Path.Combine(exeutingFolder, "images\\Tiers\\1.jpg");
                        fileInfo = new FileInfo(logoimagepath);
                        if (fileInfo.Exists)
                        {
                            pnAnimateImage.BackgroundImage = Image.FromFile(logoimagepath);
                            stopImageAnimation();
                        }


                        break;
                    case 2:
                        logoimagepath = Path.Combine(exeutingFolder, "images\\Tiers\\2.jpg");
                        fileInfo = new FileInfo(logoimagepath);
                        if (fileInfo.Exists)
                        {
                            pnAnimateImage.BackgroundImage = Image.FromFile(logoimagepath);
                            stopImageAnimation();
                        }
                        break;

                    default:
                        logoimagepath = Path.Combine(exeutingFolder, "images\\Tiers\\3.jpg");
                        fileInfo = new FileInfo(logoimagepath);
                        if (fileInfo.Exists)
                        {
                            pnAnimateImage.BackgroundImage = Image.FromFile(logoimagepath);
                            stopImageAnimation();
                        }

                        break;

                }

            }

            if (firstVisitBonusValue == "true" && birthDayBonusValue == "true")
            {
                _timer.Start(); counter = 0;

            }
            else { _timer.Stop(); }


        }

        private void TimerTickEventHanlder(object sender, EventArgs e)
        {
            FileInfo fileInfo;
            string logoimagepath;
            var exeutingFolder = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);

            if (counter == 0)
            {
                logoimagepath = Path.Combine(exeutingFolder, "images\\FirstVisit\\FirstVisit.jpg");
                fileInfo = new FileInfo(logoimagepath);
                if (fileInfo.Exists)
                    pnAnimateImage.BackgroundImage = Image.FromFile(logoimagepath);
                counter = counter + 1;
            }
            else
            {
                logoimagepath = Path.Combine(exeutingFolder, "images\\Birthday\\BIRTHDAY.jpg");
                fileInfo = new FileInfo(logoimagepath);
                if (fileInfo.Exists)
                    pnAnimateImage.BackgroundImage = Image.FromFile(logoimagepath);
                counter = 0;
            }

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderXML"></param>
        private void showOrder(XmlDocument inOrderXML)
        {
            
            clearOrderItemsContainer();
            clearOrderTotalsContainer();
            LoyaltyImagechng(inOrderXML);
            ClearBottomTextPanel();

            XmlNode currencySymbolNode = getCurrencySymbolNode(order_xml);
            setCurrencySymbol(currencySymbolNode);
            if (_currencySymbol == "true")
            {
                _currencySymbol = System.Globalization.CultureInfo.CurrentCulture.NumberFormat.CurrencySymbol;
            }
            else
            {
                _currencySymbol = "";
            }

            //add for setTextFontSize
            XmlNode textFontSize = getTextFontSize(order_xml);
            setTextFontSize(textFontSize);

            XmlNode totalDiscountNode = getTotalDiscountNode(order_xml);
            showTotalDiscount(totalDiscountNode);

            XmlNodeList servingCourseList = getServingCourseList(order_xml);
            showServingCourses(servingCourseList);

            XmlNode totalsNode = getTotalsNode(order_xml);
            showTotalsNode(totalsNode);

            // XmlNode LoyaltyNodes = getLoyaltyNodes(order_xml);
            // showTotalsNode(totalsNode);

            showOrderItemsContainer();
            showOrderTotalsContainer();
            RefreshView();

        }

        private void ClearBottomTextPanel() //clear itemlabel and item price;
        {
            lblItemName.Text = ""; 
            lblItemPrice.Text = "";
            pnlDisplayItemName.Height = ITEM_HEIGHT;
        }

        private void setCurrencySymbol(XmlNode currencySymbolNode)
        {
            _currencySymbol = XMLDocManager.GetAttribute(currencySymbolNode, @"symbol", @"");
        }

        private void setTextFontSize(XmlNode textFontSize)
        {

            try
            {
                SetTextFontSize = Convert.ToDouble(XMLDocManager.GetAttribute(textFontSize, @"fontsize", @""));
            }
            catch (Exception)
            {
                SetTextFontSize = 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderXML"></param>
        /// <returns></returns>
        private string getOrderHandle(XmlDocument inOrderXML)
        {
            XmlNode root = XMLDocManager.GetRoot(inOrderXML);

            return XMLDocManager.GetAttribute(root, @"handle", @"");
        }

        private bool orderHandleError(string inOrderHandle)
        {
            try
            {
                MMOrderItemsError errorCode = (MMOrderItemsError)Convert.ToInt32(inOrderHandle);

                return (errorCode == MMOrderItemsError.ServerNotAvailable) |
                       (errorCode == MMOrderItemsError.InvalidData);
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void clearOrderItemsContainer()
        {
            this.pnOrderItemsContainer.Visible = false;
            this.pnOrderItemsContainer.Controls.Clear();
        }

        /// <summary>
        /// 
        /// </summary>
        private void clearOrderTotalsContainer()
        {
            this.pnOrderTotalsContainer.Visible = false;
            this.pnOrderTotalsContainer.Controls.Clear();
        }

        /// <summary>
        /// 
        /// </summary>
        private void showOrderItemsContainer()
        {
            this.pnOrderItemsContainer.Visible = true;
        }

        /// <summary>
        /// 
        /// </summary>
        private void showOrderTotalsContainer()
        {
            this.pnOrderTotalsContainer.Visible = true;
        }


        private string getLoyaltyValue(XmlDocument inOrderXML, string node)
        {
            XmlNodeList xnList = inOrderXML.SelectNodes("/POSOrder[@*]");
            if (xnList != null)
                return (xnList.Cast<XmlNode>()
                    .Select(xn => xn.SelectSingleNode(node))
                    .Where(anode => anode != null)
                    .Select(anode => XDocument.Parse(anode.OuterXml))
                    .Select(firstVisit => firstVisit.Elements().Attributes().Select(d => new { Name = d.Name, Value = d.Value }).ToArray())
                    .Select(firstVisitData => new { firstVisitData, firstVisitName = firstVisitData[0].Value }).Select(
                        @t => @t.firstVisitData[1].Value)).FirstOrDefault();
            return null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderXML"></param>
        private XmlNode getTotalDiscountNode(XmlDocument inOrderXML)
        {
            XmlNode node = XMLDocManager.GetNode(inOrderXML, "Totals").FirstChild;
            return node;
        }

        private XmlNode getCurrencySymbolNode(XmlDocument inOrderXML)
        {
            XmlNode node = XMLDocManager.GetNode(inOrderXML, "CurrencySymbol");
            return node;
        }

        private XmlNode getTextFontSize(XmlDocument inOrderXML)
        {
            XmlNode node = XMLDocManager.GetNode(inOrderXML, "TextFontSize");
            return node;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderXML"></param>
        private XmlNodeList getServingCourseList(XmlDocument inOrderXML)
        {
            XmlNodeList node = XMLDocManager.GetNode(inOrderXML, "ServingCourses").ChildNodes;
            return node;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderXML"></param>
        /// <returns></returns>
        private XmlNode getTotalsNode(XmlDocument inOrderXML)
        {
            XmlNode node = XMLDocManager.GetNode(inOrderXML, "Totals");
            return node;
        }
        private XmlNode getLoyaltyNodes(XmlDocument inOrderXML)
        {
            XmlNode node = XMLDocManager.GetNode(inOrderXML, "LoyaltyUser");
            return node;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="inServingCourseNode"></param>
        /// <returns></returns>
        private XmlNodeList getItemList(XmlNode inServingCourseNode)
        {
            return XMLDocManager.GetNodeList(inServingCourseNode, @"Items/Item");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemNode"></param>
        /// <returns></returns>
        private XmlNodeList getOptionList(XmlNode inItemNode)
        {
            return XMLDocManager.GetNodeList(inItemNode, @"Options/Option");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemNode"></param>
        /// <returns></returns>
        private XmlNodeList getSideList(XmlNode inItemNode)
        {
            return XMLDocManager.GetNodeList(inItemNode, @"Sides/Side");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTotalDiscountNode"></param>
        private void showTotalDiscount(XmlNode inTotalDiscountNode)
        {
            string value = XMLDocManager.GetAttribute(inTotalDiscountNode, @"value", @"0.00");

            if (Convert.ToDecimal(value) != 0.0m)
            {
                string name = XMLDocManager.GetAttribute(inTotalDiscountNode, @"name", @"none");

                Color nameColor = Color.White;
                Color nameBckColor = Color.Teal;
                Font nameFont = new Font(@"Tahoma", 12.0f, FontStyle.Regular);

                Color valueColor = Color.White;
                Color valueBckColor = Color.Teal;
                Font valueFont = new Font(@"Tahoma", 12.0f, FontStyle.Regular);

                insertDisplayItemWithName(name, nameFont, nameColor, nameBckColor, value, valueFont, valueColor, valueBckColor);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inServingCourseList"></param>
        private void showServingCourses(XmlNodeList inServingCourseList)
        {
            int index = inServingCourseList.Count - 1;
            for (int i = inServingCourseList.Count - 1; i >= 0; i--)
            {
                showServingCourse(inServingCourseList[i], index);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inServingCourseNode"></param>
        /// <param name="index"></param>
        private void showServingCourse(XmlNode inServingCourseNode, int index)
        {
            // Serving Course's items are shown first
            XmlNodeList itemList = getItemList(inServingCourseNode);
            showItems(itemList);

           //::::::::::::::::::::::::::::::::::::::::::::::::::::::

            string name = @"";
            string value = XMLDocManager.GetAttribute(inServingCourseNode, @"name", @"none");

            Color nameColor = Color.White;
            Color nameBckColor = Color.FromArgb(0xBB - index, 0xBB - index, 0xDD - index);
            Font nameFont = new Font(@"Tahoma", 12.0f, FontStyle.Bold);

            Color valueColor = Color.White;
            Color valueBckColor = Color.FromArgb(0xBB - index, 0xBB - index, 0xDD - index);
            Font valueFont = new Font(@"Tahoma", 12.0f, FontStyle.Bold);
            if (!string.IsNullOrWhiteSpace(_currencySymbol))  // MM-2827
                value += " (" + _currencySymbol + ")";
            insertDisplayItemWithName(name, nameFont, nameColor, nameBckColor, value, valueFont, valueColor, valueBckColor);

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemList"></param>
        private void showItems(XmlNodeList inItemList)
        {
            for (int i = inItemList.Count - 1; i >= 0; i--)
            {
                showItem(inItemList[i]);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemNode"></param>
        /// <param name="inOrderControl"></param>
        private void showItem(XmlNode inItemNode)
        {
            // Item's sides are shown first ...
            XmlNodeList sideList = getSideList(inItemNode);
            showSides(sideList);

            //::::::::::::::::::::::::::::::::::::::

            // ... then Item's options ...
            XmlNodeList optionList = getOptionList(inItemNode);
            showOptions(optionList);

            //::::::::::::::::::::::::::::::::::::::

            // ... and finally the Item
            string itemName = XMLDocManager.GetAttribute(inItemNode, @"displayName", @"Item");
            string itemPrice = XMLDocManager.GetAttribute(inItemNode, @"price", @"");

            string isLastItem = XMLDocManager.GetAttribute(inItemNode, @"lastAddedItem", @"");
            showOrderItem(itemName, OrderItemType.Item, itemPrice, isLastItem);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOptionList"></param>
        private void showOptions(XmlNodeList inOptionList)
        {
            for (int i = inOptionList.Count - 1; i >= 0; i--)
            {
                showOption(inOptionList[i]);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOptionNode"></param>
        /// <param name="inOrderControl"></param>
        private void showOption(XmlNode inOptionNode)
        {
            bool added = XMLDocManager.GetAttribute(inOptionNode, @"added", @"true").ToUpper() == @"TRUE";
            string optionModifier = added ? @"+" : @"-";
            string optionName = XMLDocManager.GetAttribute(inOptionNode, @"name", @"Option");
            string optionInfo = string.Format("    {0} {1}", optionModifier, optionName);

            showOrderItemOption(optionInfo, OrderItemType.Option, @"");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSideList"></param>
        private void showSides(XmlNodeList inSideList)
        {
            for (int i = inSideList.Count - 1; i >= 0; i--)
            {
                showSide(inSideList[i]);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOptionNode"></param>
        /// <param name="inOrderControl"></param>
        private void showSide(XmlNode inSideNode)
        {
            string sideName = XMLDocManager.GetAttribute(inSideNode, @"name", @"Option");
            string sideInfo = string.Format("    {0}", sideName);
            string sidePrice = XMLDocManager.GetAttribute(inSideNode, @"price", @"");

            showOrderItemSide(sideInfo, OrderItemType.Side, sidePrice);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTotalsNode"></param>
        private void showTotalsNode(XmlNode inTotalsNode)
        {
            XmlNode totalNode = inTotalsNode.FirstChild.NextSibling;
            XmlNode changeNode = totalNode.NextSibling;
            XmlNode serviceChargeNode = changeNode.NextSibling;
            XmlNode taxNode = serviceChargeNode.NextSibling;
            bool showSC = XMLDocManager.GetAttribute(serviceChargeNode, @"visible", @"false").ToUpper() == @"TRUE";
            bool showTax = XMLDocManager.GetAttribute(taxNode, @"visible", @"false").ToUpper() == @"TRUE";
            gbOrderItems.Height = _orderPanelHeight - TOTALS_PANEL_HEIGHT;

            int tmp = 0;

            if (showTax)
            {
                gbOrderItems.Height = spDisplay.Panel2.Height - TOTALS_PANEL_HEIGHT + tmp;
                showOrderTotal(XMLDocManager.GetAttribute(taxNode, @"name", @"Tax"), XMLDocManager.GetAttribute(taxNode, @"value", @""));
            }
            else
            {
                tmp += ITEM_HEIGHT;
            }

            if (showSC)
            {
                gbOrderItems.Height = spDisplay.Panel2.Height - TOTALS_PANEL_HEIGHT + tmp;
                showOrderTotal(XMLDocManager.GetAttribute(serviceChargeNode, @"name", @"Service Charge"), XMLDocManager.GetAttribute(serviceChargeNode, @"value", @""));
            }
            else
            {
                tmp += ITEM_HEIGHT;
            }

            showOrderTotal(XMLDocManager.GetAttribute(changeNode, @"name", @"Change"), XMLDocManager.GetAttribute(changeNode, @"value", @""));


            string totalCaption = XMLDocManager.GetAttribute(totalNode, @"name", @"Total");
            if (!string.IsNullOrWhiteSpace(_currencySymbol))
            {
                totalCaption += " (" + _currencySymbol + ")";
            }
            showOrderTotalToPay(totalCaption, XMLDocManager.GetAttribute(totalNode, @"value", @""));
            gbOrderItems.Height = _orderPanelHeight - Convert.ToInt32(TOTALS_PANEL_HEIGHT * _ratio) + tmp;

        }





        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inType"></param>
        /// <param name="inPrice"></param>
        private void showOrderItem(string inName, OrderItemType inType, string inPrice, string isLastAddedItem)
        {          

            string name = inName;
            string value = inPrice;
            float fontSize = GetActualFontSize();

            Color nameColor = Color.FromArgb(0x88, 0x88, 0x88);
            Color nameBckColor = Color.White;
            Font nameFont = new Font(@"Tahoma", 12.0f, FontStyle.Bold);

            Color valueColor = Color.Teal;
            Color valueBckColor = Color.White;
            Font valueFont = new Font(@"Tahoma", 12.0f, FontStyle.Bold);

            //insertDisplayItemWithName(name, nameFont, nameColor, nameBckColor, value, valueFont, valueColor, valueBckColor);
            insertDisplayOrderItemWithName(name, nameFont, nameColor, nameBckColor, value, valueFont, valueColor, valueBckColor);
            if (fontSize > 12.0)
            {
                if (isLastAddedItem == "True")
                {
                    nameFont = new Font(@"Tahoma", fontSize, FontStyle.Bold);
                    valueFont = new Font(@"Tahoma", fontSize, FontStyle.Bold);
                    insertDisplayLastOrderItemWithName(name, nameFont, nameColor, nameBckColor, value, valueFont, valueColor, valueBckColor);
                }
            }

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inType"></param>
        /// <param name="inPrice"></param>
        private void showOrderItemOption(string inName, OrderItemType inType, string inPrice)
        {
            string name = inName;
            string value = inPrice;

            Color nameColor = Color.FromArgb(0x55, 0x55, 0xFF);
            Color nameBckColor = Color.White;
            Font nameFont = new Font(@"Tahoma", 10.0f, FontStyle.Regular);

            Color valueColor = Color.Teal;
            Color valueBckColor = Color.White;
            Font valueFont = new Font(@"Tahoma", 10.0f, FontStyle.Regular);

            insertDisplayItemWithName(name, nameFont, nameColor, nameBckColor, value, valueFont, valueColor, valueBckColor);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inType"></param>
        /// <param name="inPrice"></param>
        private void showOrderItemSide(string inName, OrderItemType inType, string inPrice)
        {
            string name = inName;
            string value = inPrice;

            Color nameColor = Color.FromArgb(0xFF, 0x88, 0x88);
            Color nameBckColor = Color.White;
            Font nameFont = new Font(@"Tahoma", 10.0f, FontStyle.Regular);

            Color valueColor = Color.Teal;
            Color valueBckColor = Color.White;
            Font valueFont = new Font(@"Tahoma", 10.0f, FontStyle.Regular);

            insertDisplayItemWithName(name, nameFont, nameColor, nameBckColor, value, valueFont, valueColor, valueBckColor);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inPrice"></param>
        private void showOrderTotalToPay(string inName, string inPrice)
        {
            string name = inName;
            string value = inPrice;
            
            Color nameColor = Color.Teal;
            Color nameBckColor = Color.FromArgb(0x99, 0xBB, 0x99);
            Font nameFont = new Font(@"Tahoma", 12.0f, FontStyle.Bold);

            Color valueColor = Color.Teal;
            Color valueBckColor = Color.FromArgb(0x99, 0xBB, 0x99);
            Font valueFont = new Font(@"Tahoma", 12.0f, FontStyle.Bold);

            insertDisplayTotalWithName(name, nameFont, nameColor, nameBckColor, value, valueFont, valueColor, valueBckColor);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inPrice"></param>
        private void showOrderTotal(string inName, string inPrice)
        {
            string name = inName;
            string value = inPrice;
            
            Color nameColor = Color.White;
            Color nameBckColor = Color.FromArgb(0x99, 0xBB, 0x99);
            Font nameFont = new Font(@"Tahoma", 12.0f, FontStyle.Bold);

            Color valueColor = Color.White;
            Color valueBckColor = Color.FromArgb(0x99, 0xBB, 0x99);
            Font valueFont = new Font(@"Tahoma", 12.0f, FontStyle.Bold);

            insertDisplayTotalWithName(name, nameFont, nameColor, nameBckColor, value, valueFont, valueColor, valueBckColor);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inNameColor"></param>
        /// <param name="inNameBckColor"></param>
        /// <param name="inValue"></param>
        /// <param name="inValueColor"></param>
        /// <param name="inValueBckColor"></param>
        private void insertDisplayItemWithName(
                        string inName,
                        Font inNameFont,
                        Color inNameColor,
                        Color inNameBckColor,
                        string inValue,
                        Font inValueFont,
                        Color inValueColor,
                        Color inValueBckColor)
        {
            if (noEnoughRoomInContainer(this.pnOrderItemsContainer)) return;

            // There is enough room for another item
            int left = 0;
            int top = 0;

            int width = this.pnOrderItemsContainer.Width;
            int height = ITEM_HEIGHT;

            Panel orderItemPanel = new Panel();
            orderItemPanel.Left = left;
            orderItemPanel.Top = top;
            orderItemPanel.Width = width;
            orderItemPanel.Height = height;
            orderItemPanel.AutoSize = true;
            orderItemPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowOnly;
            orderItemPanel.Dock = DockStyle.Top;

            showItemName(inName, inNameFont, inNameColor, inNameBckColor, orderItemPanel);
            showItemValue(inValue, inValueFont, inValueColor, inValueBckColor, orderItemPanel);

            this.pnOrderItemsContainer.Controls.Add(orderItemPanel);
            orderItemPanel.Show();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inNameColor"></param>
        /// <param name="inNameBckColor"></param>
        /// <param name="inValue"></param>
        /// <param name="inValueColor"></param>
        /// <param name="inValueBckColor"></param>
        private void insertDisplayTotalWithName(
                        string inName,
                        Font inNameFont,
                        Color inNameColor,
                        Color inNameBckColor,
                        string inValue,
                        Font inValueFont,
                        Color inValueColor,
                        Color inValueBckColor)
        {
            if (noEnoughRoomInContainer(this.pnOrderTotalsContainer)) return;

            // There is enough room for another item
            int left = 0;
            int top = 0;

           
            int width = this.pnOrderTotalsContainer.Width;
            int height = ITEM_HEIGHT;           
            
            Panel orderTotalPanel = new Panel();
            orderTotalPanel.Left = left;
            orderTotalPanel.Top = top;
            orderTotalPanel.Width = width;
            orderTotalPanel.Height = height;
            orderTotalPanel.Visible = true;
            // add autosize
            orderTotalPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowOnly;
            this.pnOrderTotalsContainer.Controls.Add(orderTotalPanel);
            orderTotalPanel.Dock = DockStyle.Top;
            //orderTotalPanel.Padding = new System.Windows.Forms.Padding(0, 0, 0, 8);

            showItemName(inName, inNameFont, inNameColor, inNameBckColor, orderTotalPanel);
            showItemValue(inValue, inValueFont, inValueColor, inValueBckColor, orderTotalPanel);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inNameColor"></param>
        /// <param name="inNameBckColor"></param>
        /// <param name="inValue"></param>
        /// <param name="inValueColor"></param>
        /// <param name="inValueBckColor"></param>
        private void insertDisplayOrderItemWithName(
                        string inName,
                        Font inNameFont,
                        Color inNameColor,
                        Color inNameBckColor,
                        string inValue,
                        Font inValueFont,
                        Color inValueColor,
                        Color inValueBckColor)
        {
            if (noEnoughRoomInContainer(this.pnOrderItemsContainer)) return;

            // There is enough room for another item
            int left = 0;
            int top = 0;

            int width = this.pnOrderItemsContainer.Width;
            int height = ITEM_HEIGHT;

            Panel orderItemPanel = new Panel();
            orderItemPanel.Left = left;
            orderItemPanel.Top = top;
            orderItemPanel.Width = width;
            orderItemPanel.Height = height;
            orderItemPanel.AutoSize = true;
            orderItemPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowOnly;
            orderItemPanel.Dock = DockStyle.Top;
            orderItemPanel.Padding = new System.Windows.Forms.Padding(0, 0, 0, 8);

            showItemName(inName, inNameFont, inNameColor, inNameBckColor, orderItemPanel);
            showItemValue(inValue, inValueFont, inValueColor, inValueBckColor, orderItemPanel);

            this.pnOrderItemsContainer.Controls.Add(orderItemPanel);
            orderItemPanel.Show();
        }

        private bool noEnoughRoomInContainer(Control inContainer)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::::::::::::::::

            result = (inContainer.Controls.Count * ITEM_HEIGHT) >= inContainer.Height - ITEM_HEIGHT;

            //:::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inNameColor"></param>
        /// <param name="inNameBckColor"></param>
        /// <param name="inParent"></param>
        private void showItemName(
                        string inName,
                        Font inNameFont,
                        Color inNameColor,
                        Color inNameBckColor,
                        Control inParent)
        {
            int left = 0;
            int top = 0;

            int width = inParent.Width - 100;
            int height = 20;

            Panel itemNamePanel = new Panel();
            itemNamePanel.Parent = inParent;
            itemNamePanel.Dock = DockStyle.Fill;
            itemNamePanel.AutoSize = true;
            itemNamePanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowOnly;
            itemNamePanel.Top = top;
            itemNamePanel.Left = left;
            itemNamePanel.BackColor = inNameBckColor;

            Label itemNameLabel = new Label();
            itemNameLabel.Parent = itemNamePanel;
            itemNameLabel.AutoSize = true;
            itemNameLabel.Text = inName;
            itemNameLabel.Font = inNameFont;
            itemNameLabel.ForeColor = inNameColor;
            /*            
                        Label itemNameLabel = new Label();
                        itemNameLabel.Left = left;
                        itemNameLabel.Top = top;
                        itemNameLabel.Width = width;
                        itemNameLabel.Height = height;
                        itemNameLabel.Parent = inParent;
                        itemNameLabel.Dock = DockStyle.Fill;
                        itemNameLabel.AutoSize = false;
                        itemNameLabel.TextAlign = ContentAlignment.MiddleLeft;

                        itemNameLabel.Text = inName;
                        itemNameLabel.Font = inNameFont;
                        itemNameLabel.ForeColor = inNameColor;
                        itemNameLabel.BackColor = inNameBckColor;
            */

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inValue"></param>
        /// <param name="inValueColor"></param>
        /// <param name="inValueBckColor"></param>
        /// <param name="inParent"></param>
        private void showItemValue(
                        string inValue,
                        Font inValueFont,
                        Color inValueColor,
                        Color inValueBckColor,
                        Control inParent)
        {
            int left = 0;
            int top = 0;

            int width = 100;
            int height = 20;

            Panel itemValuePanel = new Panel();
            itemValuePanel.Parent = inParent;
            itemValuePanel.Top = top;
            itemValuePanel.Left = left;
            itemValuePanel.Dock = DockStyle.Right;
            itemValuePanel.Width = width;
            itemValuePanel.AutoSize = true;
            itemValuePanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowOnly;
            itemValuePanel.BackColor = inValueBckColor;

            Label itemValueLabel = new Label();
            itemValueLabel.Parent = itemValuePanel;
            itemValueLabel.Text = inValue;
            itemValueLabel.Font = inValueFont;
            itemValueLabel.ForeColor = inValueColor;
            itemValueLabel.AutoSize = true;
            itemValueLabel.TextAlign = ContentAlignment.MiddleRight;

        }


        private float GetActualFontSize()
        {            
            if (_setTextFontSize == 9.5)
            {
                _setTextFontSize = _setTextFontSize * CONVERSIONFACTOR;
               
            }
            return (float)_setTextFontSize;
        }

        private void RefreshView()
        {
            //changes panel width on the basis of font ratio's resolution;
            gpBottom.Height = Convert.ToInt32(TOTALS_PANEL_HEIGHT * _ratio);
            //changes panel width on the basis of scale resolution;
            pnlLastItem.Width = gbAnimateImage.Width;
            pnlDisplayItemName.Width = gbAnimateImage.Width;
        }

        private void insertDisplayLastOrderItemWithName(
                       string inName,
                       Font inNameFont,
                       Color inNameColor,
                       Color inNameBckColor,
                       string inValue,
                       Font inValueFont,
                       Color inValueColor,
                       Color inValueBckColor)
        {          
            pnlDisplayItemName.Width = gbAnimateImage.Width;
            pnlLastItem.Width = gbAnimateImage.Width;
            showLastItemName(inName, inNameFont, inNameColor, inNameBckColor, pnlDisplayItemName);
            showLastItemValue(inValue, inValueFont, inValueColor, inValueBckColor, pnlDisplayItemName);
            pnlDisplayItemName.Show();
        }



        private void showLastItemName(
                string inName,
                Font inNameFont,
                Color inNameColor,
                Color inNameBckColor,
                Control inParent)
        {
            lblItemName.Text = inName;
            lblItemName.Font = inNameFont;
            lblItemName.ForeColor = Color.Black;
            lblItemName.Width = spDisplay.SplitterDistance;            
       }


        private void showLastItemValue(
                string inValue,
                Font inValueFont,
                Color inValueColor,
                Color inValueBckColor,
                Control inParent
                )
        {
            if (!string.IsNullOrWhiteSpace(_currencySymbol))
            {
                lblItemPrice.Text = _currencySymbol + inValue;
            }
            else
            {
                lblItemPrice.Text = inValue;
            }            
            lblItemPrice.Font = inValueFont;
            lblItemPrice.ForeColor = Color.Black;
            lblItemPrice.Dock = DockStyle.Right;
        }



        #endregion

        //private void pnLogo_Click(object sender, EventArgs e)
        //{
        //    Close();
        //}
    }
}
