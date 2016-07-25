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
    public class SetTimeKeyTest
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
        public void GetTimeKeyTest()
        {
            int timeKey = target.getTimeKey();
            Assert.IsTrue(timeKey > 0);
        }

        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void OpenSaleStartTimeTest()
        {
            int timeKey = target.getTimeKey();
            target.openSaleStartTime(timeKey);
            Assert.IsTrue(true);
        }

        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void CloseSaleStartTimeTest()
        {
            int timeKey = target.getTimeKey();
            target.openSaleStartTime(timeKey);
            target.closeSaleStartTime(timeKey);
            Assert.IsTrue(true);
        }
    }
}
