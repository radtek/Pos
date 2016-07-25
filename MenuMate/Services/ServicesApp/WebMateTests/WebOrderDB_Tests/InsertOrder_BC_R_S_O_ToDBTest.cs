using MenumateServices.WebMate.InternalClasses;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace WebMateTests
{
    
    
    /// <summary>
    ///This is a test class for WebOrderDBTest and is intended
    ///to contain all WebOrderDBTest Unit Tests
    ///</summary>
    [TestClass()]
    public class InsertOrder_BC_R_S_O_ToDBTest
    {

        static WebOrderDB_Accessor target = new WebOrderDB_Accessor();
        private TestContext testContextInstance;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        // 
        //You can use the following additional attributes as you write your tests:
        //
        //Use ClassInitialize to run code before running the first test in the class
        [ClassInitialize()]
        public static void MyClassInitialize(TestContext testContext)
        {
            target.BeginTransaction();
        }
        //
        //Use ClassCleanup to run code after all tests in a class have run
        [ClassCleanup()]
        public static void MyClassCleanup()
        {
            target.EndTransaction();
        }
        //
        //Use TestInitialize to run code before running each test
        //[TestInitialize()]
        //public void MyTestInitialize()
        //{
            
        //}
        //
        //Use TestCleanup to run code after each test has run
        //[TestCleanup()]
        //public void MyTestCleanup()
        //{
        //}
        //
        #endregion


        /// <summary>
        ///A test for checkItemIsActive
        ///</summary>
        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void insertOrder_BC_R_S_O_ToDBTest()
        {
            WebOrderDBBaseItem order = createBasicOrderFromTPCCode(WebMateTestSettings.ThirdPartyCodeKey);
            order.TabContainerName = getGuidFromWebOrderKey(WebMateTestSettings.WebOrderKey);
            order.TabKey = getTabKeyForWebOrder(
                                WebMateTestSettings.WebOrderKey,
                                WebMateTestSettings.WebOrderTabName);
            order.TimeKey = setTimeKey();
            order.SecurityRef = getNextSecurityRef();
            order.TransactionNumber = getNextTransactionNumber();
            order.Categories.PrimaryArcCategoryKey = getArcCategoryKeyFromCategory(order.Categories.FinancialCategory);

            bool result = target.insertOrderBaseDataToDB(
                                          target.connection_,
                                          target.transaction_,
                                          ref order);

            result = target.insertOrderBreakdownCategoriesToDB(
                                                target.connection_,
                                                target.transaction_,
                                                order.OrderKey,
                                                order.Categories);

            WebOrderDBItem dbItem = (WebOrderDBItem)order;

            result = target.insertOrderRecipesToDB(
                                        target.connection_,
                                        target.transaction_,
                                        dbItem.OrderKey,                                        
                                        dbItem.ItemRecipes);

            result = target.insertOrderSidesToDB(WebMateTestSettings.WebOrderKey,
                                        target.connection_,
                                        target.transaction_,
                                        dbItem.OrderKey,
                                        dbItem.TabKey,
                                        dbItem.TransactionNumber,
                                        dbItem.ItemSides,
                                        dbItem.SecurityRef,
                                        dbItem.TimeKey);

            result = target.insertOrderOptionsToDB(
                                        target.connection_,
                                        target.transaction_,
                                        dbItem.OrderKey,
                                        dbItem.ItemOptions);

            Assert.AreEqual(true, result);
        }

        private string getNextTransactionNumber()
        {
            return Convert.ToString(target.getNextTransactionNumber());
        }

        private int getArcCategoryKeyFromCategory(string categoryName)
        {
            return target.getArcCategoryKeyFromCategory(categoryName);
        }

        private int getNextSecurityRef()
        {
            return target.getNextSecurityRef();
        }

        private int setTimeKey()
        {
            return target.setTimeKey();
        }

        private int getTabKeyForWebOrder(int webOrderKey, string webOrderTabName)
        {
            return target.getOrCreateTabForWebOrder(webOrderKey, webOrderTabName);
        }

        private string getGuidFromWebOrderKey(int webOrderKey)
        {
            return target.getGuidFromWebOrderKey(webOrderKey);
        }

        private WebOrderDBBaseItem createBasicOrderFromTPCCode(int p)
        {
            int itemSizeKey = target.getItemSizeKeyFromThirdPartyCodeKey(p);
            WebOrderDBBaseItem item = new WebOrderDBItem();
            item.TabName = "Mobi2Go";
            item.Price = WebMateTestSettings.MainItemPrice;
            item.PriceLevel0 = WebMateTestSettings.MainItemPriceL0;
            item.PriceLevel1 = WebMateTestSettings.MainItemPriceL1;
            item.Qty = WebMateTestSettings.MainItemQty;

            target.loadBaseOrderData(
                itemSizeKey,
                ref item);

            target.loadBaseOrderBreakdownCategories(
                                    itemSizeKey,
                                    ref item);

            WebOrderDBItem dbItem = (WebOrderDBItem)item;

            target.loadRecipes(
                            itemSizeKey,
                            ref dbItem);

            setSidesInformation(ref dbItem);

            target.loadSides(
                        ref dbItem);

            setOptionsInformation(ref dbItem);

            target.loadOptions(
                        ref dbItem);

            return item;
        }

        private void setOptionsInformation(ref WebOrderDBItem dbItem)
        {
            dbItem.ItemOptions.Add(
                new WebOrderDBItemOption()
                {
                    OptionKey = WebMateTestSettings.OptionKey1
                });
            dbItem.ItemOptions.Add(
                new WebOrderDBItemOption()
                {
                    OptionKey = WebMateTestSettings.OptionKey2
                });
            dbItem.ItemOptions.Add(
                new WebOrderDBItemOption()
                {
                    OptionKey = WebMateTestSettings.OptionKey3
                });
        }

        private void setSidesInformation(ref WebOrderDBItem dbItem)
        {
            dbItem.ItemSides.Add(                
                new WebOrderDBItemSide()
                {
                    ThirdPartyCodeKey = WebMateTestSettings.SideThirdPartyCode1,
                    Price = WebMateTestSettings.SideItem1Price,
                    PriceLevel0 = WebMateTestSettings.SideItem1PriceL0,
                    PriceLevel1 = WebMateTestSettings.SideItem1PriceL1,
                    Qty = WebMateTestSettings.Side1Qty
                });
            dbItem.ItemSides.Add(
                new WebOrderDBItemSide()
                {
                    ThirdPartyCodeKey = WebMateTestSettings.SideThirdPartyCode2,
                    Price = WebMateTestSettings.SideItem2Price,
                    PriceLevel0 = WebMateTestSettings.SideItem2PriceL0,
                    PriceLevel1 = WebMateTestSettings.SideItem2PriceL1,
                    Qty = WebMateTestSettings.Side3Qty
                });
            dbItem.ItemSides.Add(
                new WebOrderDBItemSide()
                {
                    ThirdPartyCodeKey = WebMateTestSettings.SideThirdPartyCode3,
                    Price = WebMateTestSettings.SideItem3Price,
                    PriceLevel0 = WebMateTestSettings.SideItem3PriceL0,
                    PriceLevel1 = WebMateTestSettings.SideItem3PriceL1,
                    Qty = WebMateTestSettings.Side3Qty
                });
        }
    }
}
