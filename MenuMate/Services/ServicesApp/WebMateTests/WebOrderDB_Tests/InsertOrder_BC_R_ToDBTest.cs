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
    public class InsertOrder_BC_R_ToDBTest
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
            target.RollbackTransaction();
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
        public void insertOrder_BC_R_ToDBTest()
        {
            WebOrderDBBaseItem order = createBasicOrderFromTPCCode(WebMateTestSettings.ThirdPartyCodeKey);
            order.TabContainerName = getGuidFromWebOrderKey(WebMateTestSettings.WebOrderKey);
            order.TabKey = getTabKeyForWebOrder(
                                WebMateTestSettings.WebOrderKey,
                                WebMateTestSettings.WebOrderTabName);
            order.TimeKey = setTimeKey();
            order.SecurityRef = getNextSecurityRef();
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

            Assert.AreEqual(true, result);
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

            return item;
        }
    }
}
