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
    public class LoadCompleteOrderTest
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
        public void loadBasicOrderDataTest()
        {
            int itemSizeKey = getItemSizeKeyFromTPC(WebMateTestSettings.ThirdPartyCodeKey);
            WebOrderDBBaseItem item = new WebOrderDBItem();
            target.loadBaseOrderData(itemSizeKey, ref item);
            Assert.IsTrue(item.ItemId > 0);
        }

        private int getItemSizeKeyFromTPC(int p)
        {
            return target.getItemSizeKeyFromThirdPartyCodeKey(p);
        }

        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void LoadBreakdownCategoriesTest()
        {
            int itemSizeKey = getItemSizeKeyFromTPC(1);
            WebOrderDBBaseItem item = new WebOrderDBItem();
            target.loadBaseOrderBreakdownCategories(itemSizeKey, ref item);
            Assert.IsTrue(item.Categories.Count > 0);
        }

        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void LoadOptionsTest()
        {
            int itemSizeKey = getItemSizeKeyFromTPC(WebMateTestSettings.ThirdPartyCodeKey);
            WebOrderDBItem item = createWebOrderDBItem();
            target.loadOptions(ref item);
            Assert.IsTrue(allOptionsLoaded(item));
        }

        private bool allOptionsLoaded(WebOrderDBItem item)
        {
            foreach (WebOrderDBItemOption o in item.ItemOptions)
            {
                if (o.OptionID == 0)
                    return false;
            }

            return true;
        }

        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void LoadSidesTest()
        {
            int itemSizeKey = getItemSizeKeyFromTPC(WebMateTestSettings.ThirdPartyCodeKey);
            WebOrderDBItem item = createWebOrderDBItem();
            target.loadSides(ref item);
            Assert.IsTrue(allItemSidesLoaded(item));
        }

        private bool allItemSidesLoaded(WebOrderDBItem item)
        {
            foreach (WebOrderDBItemSide s in item.ItemSides)
            {
                if (s.ItemKey == 0)
                    return false;
            }

            return true;
        }

        private WebOrderDBItem createWebOrderDBItem()
        {
            WebOrderDBItem item = new WebOrderDBItem();
            item.ThirdPartyCodeKey = WebMateTestSettings.ThirdPartyCodeKey;

            item.ItemSides.Add(
                new WebOrderDBItemSide() { ThirdPartyCodeKey = WebMateTestSettings.SideThirdPartyCode1 });
            item.ItemSides.Add(
                new WebOrderDBItemSide() { ThirdPartyCodeKey = WebMateTestSettings.SideThirdPartyCode2 });
            item.ItemSides.Add(
                new WebOrderDBItemSide() { ThirdPartyCodeKey = WebMateTestSettings.SideThirdPartyCode3 });

            item.ItemOptions.Add(
                new WebOrderDBItemOption() { OptionKey = WebMateTestSettings.OptionKey1 });
            item.ItemOptions.Add(
                new WebOrderDBItemOption() { OptionKey = WebMateTestSettings.OptionKey2 });
            item.ItemOptions.Add(
                new WebOrderDBItemOption() { OptionKey = WebMateTestSettings.OptionKey3 });

            return item;
        }
    }
}
