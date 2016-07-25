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
    public class GetArcCategoryFromCategoryTest
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
        public void getArcCategoryFromCategoryTest()
        {
            WebOrderDBBaseItem order = createOrderFromTPCCode(WebMateTestSettings.ThirdPartyCodeKey);
            int arcCategoryKey = target.getArcCategoryKeyFromCategory(order.Categories.FinancialCategory);
            Assert.IsTrue(arcCategoryKey > 0);
        }

        private WebOrderDBBaseItem createOrderFromTPCCode(int p)
        {
            int itemSizeKey = target.getItemSizeKeyFromThirdPartyCodeKey(p);
            WebOrderDBBaseItem item = new WebOrderDBItem();
            target.loadBaseOrderData(
                itemSizeKey,
                ref item);
            return item;
        }
    }
}
